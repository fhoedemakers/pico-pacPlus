#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"
#include "ff.h"
#include "tusb.h"
#include "gamepad.h"
#include "menu.h"
#include "nespad.h"
#include "wiipad.h"
#include "FrensHelpers.h"
#include "settings.h"
#include "FrensFonts.h"
#include "vumeter.h"
#include "menu_settings.h"
#include "pico/bootrom.h"

extern "C"
{
#include "vmachine.h"
#include "cpu.h"
#include "vdc.h"
#include "vpp.h"
#include "audio.h"
#include "keyboard.h"
#include "bios.h"
#include "roms.h"
#include "o2em_pico.h"
#include "o2em_pico_callbacks.h"
#include "o2em2.h"
}

#define EMULATOR_CLOCKFREQ_KHZ 252000

#if PICO_RP2350
#define AUDIOBUFFERSIZE 1024
#else
#define AUDIOBUFFERSIZE 256
#endif

#ifndef DVI_AUDIO_GAIN_Q8
#define DVI_AUDIO_GAIN_Q8 1024
#endif
static int g_dvi_audio_gain_q8 = DVI_AUDIO_GAIN_Q8;

static uint32_t CPUFreqKHz = EMULATOR_CLOCKFREQ_KHZ;

bool isFatalError = false;
char *romName;
static bool showSettings = false;
static uint32_t start_tick_us = 0;
static uint32_t fps = 0;

const int8_t g_settings_visibility_o2em[MOPT_COUNT] = {
    0,                               // Exit Game
    0,                               // Reset Game
    0,                               // Save / Restore State (no save states)
    1,                               // Screen Mode
    0,                               // Scanlines toggle
    HSTX,                            // Scanline Type (HSTX only)
    1,                               // FPS Overlay
    0,                               // Audio Enable
    0,                               // Frame Skip
    HSTX && ENABLEDVI,               // Display Mode
    (EXT_AUDIO_IS_ENABLED),          // External Audio
    1,                               // Font Color
    1,                               // Font Back Color
    ENABLE_VU_METER,                 // VU Meter
    (HW_CONFIG == 8),                // Fruit Jam Volume Control
    0,                               // DMG Palette (not applicable)
    0,                               // Border Mode (not applicable)
    0,                               // Rapid Fire on A (not applicable)
    0,                               // Rapid Fire on B (not applicable)
    0,                               // Auto Swap FDS (not applicable)
    0,                               // Auto Insert Disk A (not applicable)
    1,                               // Enter bootsel mode
    0                                // FDS Disk Swap (not applicable)
};

const uint8_t g_available_screen_modes_o2em[] = {
    0, // SCANLINE_8_7     
    0, // NOSCANLINE_8_7   
    1, // SCANLINE_1_1
    1  // NOSCANLINE_1_1
};

#if WII_PIN_SDA >= 0 and WII_PIN_SCL >= 0
static uint16_t wiipad_raw_cached = 0;
#endif

static DWORD prevButtons[2]{};

unsigned long ROM_FILE_SIZE = 0;

static inline int16_t apply_dvi_gain_i32(int x)
{
    int64_t v = (int64_t)x * (int64_t)g_dvi_audio_gain_q8;
    v >>= 8;
    if (v > 32767) v = 32767;
    else if (v < -32768) v = -32768;
    return (int16_t)v;
}

#define O2EM_COMBINED_MAGIC 0x4F32454DUL
#define O2EM_COMBINED_FILE "/o2em_combined.bin"

struct O2emCombinedHeader {
    uint32_t magic;
    uint32_t crc;
    uint32_t rom_file_size;
    uint8_t bank;
    uint8_t three_k;
    uint8_t exrom;
    uint8_t openb;
    uint8_t megaxrom;
    uint8_t need_bios;
    uint8_t vpp;
    uint8_t bios;
};

static bool rom_table_is_flash = false;

static void init_rom_table()
{
    if (rom_table) return;
    // Allocate in SRAM (plain malloc), NOT Frens::f_malloc which would
    // route to PSRAM on PSRAM-enabled boards. cpu_exec() does ROM(pc)
    // on every emulated opcode - millions of accesses per second - and
    // routing those through PSRAM creates enough QMI/AHB bus pressure
    // to delay the HSTX DMA IRQ and drop the HDMI signal in CPU-heavy
    // games (e.g. Demon Attack). 32KB SRAM cost is well worth it.
    rom_table = (Byte (*)[4096])malloc(8 * 4096);
    memset(rom_table, 0, 8 * 4096);
}

static void free_rom_table()
{
    if (rom_table && !rom_table_is_flash) {
        free(rom_table);
    }
    rom_table = NULL;
    rom_table_is_flash = false;
}

static void init_app_data()
{
    memset(&app_data, 0, sizeof(app_data));
    app_data.bank = 1;
    app_data.limit = 1;
    app_data.sound_en = 1;
    app_data.speed = 100;
    app_data.wsize = 2;
    app_data.filter = 1;
    app_data.euro = 0;
    app_data.openb = 0;
    app_data.vpp = 0;
    app_data.exrom = 0;
    app_data.three_k = 0;
    app_data.megaxrom = 0;
    app_data.voice = 0;
    app_data.svolume = 100;
    app_data.vvolume = 100;
    app_data.scoretype = 0;
    app_data.scoreaddress = 0;
    app_data.default_highscore = 0;
    app_data.need_bios = 1;
    app_data.force_bios = 0;
    app_data.show_fps = 0;
    app_data.handleevbl = 0;
    app_data.handleevbll = 0;
    app_data.allowvdcalways = 0;
    app_data.c7420 = 0;
    app_data.disableJoyDefault = 0;
    app_data.stick[0] = 1;
    app_data.stick[1] = 1;
    app_data.sticknumber[0] = 0;
    app_data.sticknumber[1] = 1;
    strcpy(app_data.biosdir, "/bios");
}

// Convert the emulator's 8-bit indexed vscreen to the active display
// driver's pixel format and write it in one shot at end of frame.
//
// Drawing happens against vscreen during emulator execution; this function
// does a single bulk conversion so the display FB is updated atomically.
// That avoids the in-frame "clear-to-bg then redraw-sprite" flicker that
// would happen if the emulator wrote directly into the live HSTX FB.
extern "C" void __not_in_flash_func(o2em_render_frame)(Byte *vscreen, Byte *col, int width, int height,
                                   const unsigned short *palette_lut, int vpp_enabled)
{
    (void)col;
    (void)vpp_enabled;
    (void)height;

    int vis_w = WNDW; // 320
    int vis_h = WNDH; // 240

#if HSTX
    for (int y = 0; y < vis_h; y++)
    {
        WORD *dst = hstx_getlineFromFramebuffer(y);
        Byte *src = vscreen + y * width;
        for (int x = 0; x < vis_w; x++)
            dst[x] = palette_lut[src[x] & 0x1f];
    }
#else
#if FRAMEBUFFERISPOSSIBLE
    if (Frens::isFrameBufferUsed())
    {
        for (int y = 0; y < vis_h; y++)
        {
            WORD *dst = &Frens::framebuffer[y * 320];
            Byte *src = vscreen + y * width;
            for (int x = 0; x < vis_w; x++)
                dst[x] = palette_lut[src[x] & 0x1f];
        }
    }
    else
#endif
    {
        for (int y = 0; y < vis_h; y++)
        {
            auto b = dvi_->getLineBuffer();
            WORD *dst = b->data();
            Byte *src = vscreen + y * width;
            int max_w = b->size() < (size_t)vis_w ? b->size() : vis_w;
            for (int x = 0; x < max_w; x++)
                dst[x] = palette_lut[src[x] & 0x1f];
            dvi_->setLineBuffer(y, b);
        }
    }
#endif

    // FPS overlay (writes directly into the display FB; brief tearing
    // window is acceptable for a few small digits at top-left).
    if (settings.flags.displayFrameRate)
    {
        uint32_t tick_us = Frens::time_us() - start_tick_us;
        fps = (1000000 - 1) / tick_us + 1;
        start_tick_us = Frens::time_us();
        WORD fgc = palette_lut[15];
        WORD bgc = palette_lut[0];
        char fpsString[2] = { (char)('0' + (fps / 10)), (char)('0' + (fps % 10)) };
#if HSTX
        for (int y = 8; y < 16; y++)
        {
            WORD *fpsBuffer = hstx_getlineFromFramebuffer(y) + 4;
            int rowInChar = y - 8;
            for (int i = 0; i < 2; i++)
            {
                char fontSlice = getcharslicefrom8x8font(fpsString[i], rowInChar);
                for (int bit = 0; bit < 8; bit++)
                {
                    *fpsBuffer++ = (fontSlice & 1) ? fgc : bgc;
                    fontSlice >>= 1;
                }
            }
        }
#elif FRAMEBUFFERISPOSSIBLE
        if (Frens::isFrameBufferUsed())
        {
            for (int y = 8; y < 16; y++)
            {
                WORD *fpsBuffer = &Frens::framebuffer[y * 320 + 4];
                int rowInChar = y - 8;
                for (int i = 0; i < 2; i++)
                {
                    char fontSlice = getcharslicefrom8x8font(fpsString[i], rowInChar);
                    for (int bit = 0; bit < 8; bit++)
                    {
                        *fpsBuffer++ = (fontSlice & 1) ? fgc : bgc;
                        fontSlice >>= 1;
                    }
                }
            }
        }
#else
        (void)fgc; (void)bgc; (void)fpsString;
#endif
    }
}

extern "C" void o2em_sound_output(unsigned char *buffer, int len)
{
    // 1056 unsigned 8-bit mono samples at 44100Hz -> downsample to ~735 per frame (44100/60)
    // and convert to signed 16-bit stereo
    int target_samples = 735;
    float step = (float)len / (float)target_samples;

#if !HSTX
#if EXT_AUDIO_IS_ENABLED
    if (settings.flags.useExtAudio)
    {
        for (int i = 0; i < target_samples; i++)
        {
            int idx = (int)(i * step);
            if (idx >= len) idx = len - 1;
            int sample = ((int)buffer[idx] - 128) << 8;
            EXT_AUDIO_ENQUEUE_SAMPLE(sample, sample);
#if ENABLE_VU_METER
            if (settings.flags.enableVUMeter)
                addSampleToVUMeter(sample);
#endif
        }
        return;
    }
#endif
    {
        auto &ring = dvi_->getAudioRingBuffer();
        for (int i = 0; i < target_samples; i++)
        {
            int idx = (int)(i * step);
            if (idx >= len) idx = len - 1;
            int sample = ((int)buffer[idx] - 128) << 8;
            int l = apply_dvi_gain_i32(sample);
            int r = l;
            auto n = ring.getWritableSize();
            if (n > 0)
            {
                auto p = ring.getWritePointer();
                *p = {static_cast<short>(l), static_cast<short>(r)};
                ring.advanceWritePointer(1);
            }
#if ENABLE_VU_METER
            if (settings.flags.enableVUMeter)
                addSampleToVUMeter(l);
#endif
        }
    }
#else
#if EXT_AUDIO_IS_ENABLED
    bool audioJackConnected = Frens::isHeadPhoneJackConnected();
#endif
    for (int i = 0; i < target_samples; i++)
    {
        int idx = (int)(i * step);
        if (idx >= len) idx = len - 1;
        int sample = ((int)buffer[idx] - 128) << 8;
        int l = sample;
        int r = sample;

#if ENABLE_VU_METER
        if (settings.flags.enableVUMeter)
            addSampleToVUMeter(l);
#endif

#if EXT_AUDIO_IS_ENABLED
        if (settings.flags.useExtAudio || audioJackConnected)
        {
            EXT_AUDIO_ENQUEUE_SAMPLE(l, r);
            continue;
        }
#endif
        int gl = apply_dvi_gain_i32(l);
        int gr = apply_dvi_gain_i32(r);
        hstx_push_audio_sample(gl, gr);
    }
#endif
}

// Translate a HID usage code (HID_KEY_*) into an O2 KEY_* index, or -1 if
// unmapped. Covers the keys the O2 / G7400 keyboard exposes via the
// key_map_G7400[][] matrix in vmachine.c.
static int hidKeyToO2Key(uint8_t hid)
{
    switch (hid) {
        case HID_KEY_A: return KEY_A;
        case HID_KEY_B: return KEY_B;
        case HID_KEY_C: return KEY_C;
        case HID_KEY_D: return KEY_D;
        case HID_KEY_E: return KEY_E;
        case HID_KEY_F: return KEY_F;
        case HID_KEY_G: return KEY_G;
        case HID_KEY_H: return KEY_H;
        case HID_KEY_I: return KEY_I;
        case HID_KEY_J: return KEY_J;
        case HID_KEY_K: return KEY_K;
        case HID_KEY_L: return KEY_L;
        case HID_KEY_M: return KEY_M;
        case HID_KEY_N: return KEY_N;
        case HID_KEY_O: return KEY_O;
        case HID_KEY_P: return KEY_P;
        case HID_KEY_Q: return KEY_Q;
        case HID_KEY_R: return KEY_R;
        case HID_KEY_S: return KEY_S;
        case HID_KEY_T: return KEY_T;
        case HID_KEY_U: return KEY_U;
        case HID_KEY_V: return KEY_V;
        case HID_KEY_W: return KEY_W;
        case HID_KEY_X: return KEY_X;
        case HID_KEY_Y: return KEY_Y;
        case HID_KEY_Z: return KEY_Z;
        case HID_KEY_1: return KEY_1;
        case HID_KEY_2: return KEY_2;
        case HID_KEY_3: return KEY_3;
        case HID_KEY_4: return KEY_4;
        case HID_KEY_5: return KEY_5;
        case HID_KEY_6: return KEY_6;
        case HID_KEY_7: return KEY_7;
        case HID_KEY_8: return KEY_8;
        case HID_KEY_9: return KEY_9;
        case HID_KEY_0: return KEY_0;
        case HID_KEY_SPACE: return KEY_SPACE;
        case HID_KEY_RETURN: return KEY_ENTER;
        case HID_KEY_MINUS: return KEY_MINUS;
        case HID_KEY_EQUAL: return KEY_EQUALS;
        case HID_KEY_PERIOD: return KEY_STOP;
        case HID_KEY_SLASH: return KEY_SLASH;
        case HID_KEY_KEYPAD_ADD: return KEY_PLUS_PAD;
        case HID_KEY_KEYPAD_DIVIDE: return KEY_SLASH_PAD;
        case HID_KEY_KEYPAD_MULTIPLY: return KEY_ASTERISK;
        default: return -1;
    }
}

extern "C" void o2em_poll_input()
{
    static constexpr int LEFT = 1 << 6;
    static constexpr int RIGHT = 1 << 7;
    static constexpr int UP = 1 << 4;
    static constexpr int DOWN = 1 << 5;
    static constexpr int SELECT = 1 << 2;
    static constexpr int START = 1 << 3;
    static constexpr int A = 1 << 0;
    static constexpr int B = 1 << 1;

    // Update the O2 emulator's key[] array from the current USB HID keyboard
    // state. The O2 keyboard is shared - not per-player - so do this once.
    memset(key, 0, KEY_MAX);
    const auto &kb = io::getCurrentKeyboardState();
    for (int k = 0; k < 6; k++) {
        int o2key = hidKeyToO2Key(kb.keycode[k]);
        if (o2key >= 0 && o2key < KEY_MAX)
            key[o2key] = 1;
    }

    bool usbConnected = false;
    for (int i = 0; i < 2; ++i)
    {
        auto &gp = io::getCurrentGamePadState(i);
        if (i == 0)
            usbConnected = gp.isConnected();

        int v = (gp.buttons & io::GamePadState::Button::LEFT ? LEFT : 0) |
                (gp.buttons & io::GamePadState::Button::RIGHT ? RIGHT : 0) |
                (gp.buttons & io::GamePadState::Button::UP ? UP : 0) |
                (gp.buttons & io::GamePadState::Button::DOWN ? DOWN : 0) |
                (gp.buttons & io::GamePadState::Button::A ? A : 0) |
                (gp.buttons & io::GamePadState::Button::B ? B : 0) |
                (gp.buttons & io::GamePadState::Button::SELECT ? SELECT : 0) |
                (gp.buttons & io::GamePadState::Button::START ? START : 0) |
                0;
#if NES_PIN_CLK != -1
        if (usbConnected)
        {
            if (i == 1)
                v = v | nespad_states[1] | nespad_states[0];
        }
        else
        {
            v |= nespad_states[i];
        }
#endif

#if WII_PIN_SDA >= 0 and WII_PIN_SCL >= 0
        if (usbConnected)
        {
            if (i == 1)
                v |= wiipad_raw_cached;
        }
        else
        {
            if (i == 0)
                v |= wiipad_raw_cached;
        }
#endif

        // Map to O2EM joy struct
        joy[i].stick[0].axis[1].d1 = (v & UP) ? 1 : 0;
        joy[i].stick[0].axis[1].d2 = (v & DOWN) ? 1 : 0;
        joy[i].stick[0].axis[0].d1 = (v & LEFT) ? 1 : 0;
        joy[i].stick[0].axis[0].d2 = (v & RIGHT) ? 1 : 0;
        joy[i].button[0].b = (v & A) ? 1 : 0;

        if (v & START)
            key[KEY_1] = 1;

        if (i == 0)
        {
            // Reboot to BOOTSEL mode
            if ((v & (SELECT | START | UP | A)) == (SELECT | START | UP | A))
                reset_usb_boot(0, 0);

            auto pushed = v & ~prevButtons[i];

            if (v & START)
            {
                if (pushed & A)
                    settings.flags.displayFrameRate = !settings.flags.displayFrameRate;
                else if (pushed & LEFT)
                {
#if HW_CONFIG == 8
                    settings.fruitjamVolumeLevel = std::max((int8_t)-63, (int8_t)(settings.fruitjamVolumeLevel - 1));
                    EXT_AUDIO_SETVOLUME(settings.fruitjamVolumeLevel);
#endif
                }
                else if (pushed & RIGHT)
                {
#if HW_CONFIG == 8
                    settings.fruitjamVolumeLevel = std::min((int8_t)23, (int8_t)(settings.fruitjamVolumeLevel + 1));
                    EXT_AUDIO_SETVOLUME(settings.fruitjamVolumeLevel);
#endif
                }
            }

            if (v & SELECT)
            {
                if (pushed & START)
                {
                    showSettings = true;
                }
                if (pushed & UP)
                    scaleMode8_7_ = Frens::screenMode(-1);
                else if (pushed & DOWN)
                    scaleMode8_7_ = Frens::screenMode(+1);
                else if (pushed & LEFT)
                {
#if EXT_AUDIO_IS_ENABLED && !HSTX
                    settings.flags.useExtAudio = !settings.flags.useExtAudio;
#else
                    settings.flags.useExtAudio = 0;
#endif
                }
#if ENABLE_VU_METER
                else if (pushed & RIGHT)
                {
                    settings.flags.enableVUMeter = !settings.flags.enableVUMeter;
                    turnOffAllLeds();
                }
#endif
            }
        }
        prevButtons[i] = v;
    }

    // The O2 console had two joystick ports and games picked either port
    // freely - e.g. K.C. Munchkin reads port 1, Alien Invaders reads
    // port 2 (in_bus() in vmachine.c routes si==1 -> joy[0], else -> joy[1]).
    // When only one USB gamepad is connected (the common case), mirror
    // player-0 input into joy[1] so port-2-using games work with the
    // single controller. If a second gamepad IS connected, leave joy[1]
    // alone so true 2-player games still work.
    if (!io::getCurrentGamePadState(1).isConnected())
    {
        joy[1].stick[0].axis[0].d1 = joy[0].stick[0].axis[0].d1;
        joy[1].stick[0].axis[0].d2 = joy[0].stick[0].axis[0].d2;
        joy[1].stick[0].axis[1].d1 = joy[0].stick[0].axis[1].d1;
        joy[1].stick[0].axis[1].d2 = joy[0].stick[0].axis[1].d2;
        joy[1].button[0].b = joy[0].button[0].b;
    }
}

static void processPerFrame()
{
    Frens::PaceFrames60fps(false);
    Frens::pollHeadPhoneJack();
    EXT_AUDIO_POLL_HEADPHONE();

#if NES_PIN_CLK != -1
    nespad_read_start();
#endif
    auto count =
#if !HSTX
        dvi_->getFrameCounter();
#else
        hstx_getframecounter();
#endif
    long onOff = (count / 60) & 1;
    Frens::blinkLed(onOff);
#if NES_PIN_CLK != -1
    nespad_read_finish();
#endif
    tuh_task();

#if WII_PIN_SDA >= 0 and WII_PIN_SCL >= 0
    wiipad_raw_cached = wiipad_read();
#endif

#if ENABLE_VU_METER
    if (isVUMeterToggleButtonPressed())
    {
        settings.flags.enableVUMeter = !settings.flags.enableVUMeter;
        FrensSettings::savesettings();
        turnOffAllLeds();
    }
#endif

    if (showSettings)
    {
        showSettings = false;
        int rval = showSettingsMenu(true);
        if (rval == 3)
        {
            key_done = 1;
        }
        if (rval == 5)
        {
            init_cpu();
            init_roms();
            init_vpp();
            clearscr();
        }
    }
}

int main()
{
    char selectedRom[FF_MAX_LFN];
    romName = selectedRom;
    ErrorMessage[0] = selectedRom[0] = 0;

    Frens::setClocksAndStartStdio(CPUFreqKHz, VREG_VOLTAGE_1_20);

    printf("==========================================================================================\n");
    printf("Pico-PacPlus %s\n", SWVERSION);
    printf("Build date: %s\n", __DATE__);
    printf("Build time: %s\n", __TIME__);
    printf("CPU freq: %d kHz\n", clock_get_hz(clk_sys) / 1000);
#if HSTX
    printf("HSTX freq: %d\n", clock_get_hz(clk_hstx) / 1000);
#endif
    printf("Stack size: %d bytes\n", PICO_STACK_SIZE);
    printf("==========================================================================================\n");

    FrensSettings::initSettings(FrensSettings::O2EM);
    isFatalError = !Frens::initAll(selectedRom, CPUFreqKHz, 4, 4, AUDIOBUFFERSIZE, false, true);

    g_settings_visibility = g_settings_visibility_o2em;
    g_available_screen_modes = g_available_screen_modes_o2em;
    if (!g_available_screen_modes[static_cast<int>(settings.screenMode)])
        settings.screenMode = ScreenMode::NOSCANLINE_1_1;
    scaleMode8_7_ = Frens::applyScreenMode(settings.screenMode);
    bool showSplash = true;

    while (true)
    {
        if (strlen(selectedRom) == 0)
        {
            const char *romExtensions = ".bin";
            menu("Pico-PacPlus", ErrorMessage, isFatalError, showSplash, romExtensions, selectedRom);
            printf("Selected ROM: %s\n", selectedRom);
        }

        *ErrorMessage = 0;
        EXT_AUDIO_SETVOLUME(settings.fruitjamVolumeLevel);

        free_rom_table();
        init_app_data();

        bool loaded_from_flash = false;

        if (!Frens::isPsramEnabled()) {
            const O2emCombinedHeader *hdr =
                reinterpret_cast<const O2emCombinedHeader *>(ROM_FILE_ADDR);
            if (hdr->magic == O2EM_COMBINED_MAGIC) {
                rom_table = (Byte (*)[4096])(ROM_FILE_ADDR + sizeof(O2emCombinedHeader));
                rom_table_is_flash = true;
                app_data.crc = hdr->crc;
                app_data.bank = hdr->bank;
                app_data.three_k = hdr->three_k;
                app_data.exrom = hdr->exrom;
                app_data.openb = hdr->openb;
                app_data.megaxrom = hdr->megaxrom;
                app_data.need_bios = hdr->need_bios;
                app_data.vpp = hdr->vpp;
                app_data.bios = hdr->bios;
                ROM_FILE_SIZE = hdr->rom_file_size;
                loaded_from_flash = true;
                f_unlink(O2EM_COMBINED_FILE);
                printf("Using combined ROM+BIOS from flash (CRC: %08lX)\n",
                       (unsigned long)app_data.crc);
            }
        }

        if (!loaded_from_flash) {
            init_rom_table();

            char bios_path[MAXC];
            if (search_for_bios(app_data.biosdir, bios_path, 0) != O2EM_SUCCESS)
            {
                strcpy(ErrorMessage, "No BIOS found in /bios/");
                printf("%s\n", ErrorMessage);
                selectedRom[0] = 0;
                continue;
            }

            if (load_bios(bios_path, (unsigned char **)rom_table, &app_data) != O2EM_SUCCESS)
            {
                strcpy(ErrorMessage, "Error loading BIOS");
                printf("%s\n", ErrorMessage);
                selectedRom[0] = 0;
                continue;
            }

            FILINFO fno;
            if (f_stat(selectedRom, &fno) != FR_OK)
            {
                strcpy(ErrorMessage, "Cannot stat ROM file");
                printf("%s\n", ErrorMessage);
                selectedRom[0] = 0;
                continue;
            }
            ROM_FILE_SIZE = fno.fsize;

            const unsigned char *romData =
                reinterpret_cast<const unsigned char *>(ROM_FILE_ADDR);
            if (load_cart_from_memory(romData, (long)ROM_FILE_SIZE, &app_data) != O2EM_SUCCESS)
            {
                strcpy(ErrorMessage, "Error loading ROM");
                printf("%s\n", ErrorMessage);
                selectedRom[0] = 0;
                continue;
            }

            printf("BIOS: %s, VPP: %d, Euro: %d, Banks: %d\n",
                   bios_path, app_data.vpp, app_data.euro, app_data.bank);

            if (!Frens::isPsramEnabled() && !app_data.megaxrom) {
                O2emCombinedHeader hdr;
                hdr.magic = O2EM_COMBINED_MAGIC;
                hdr.crc = app_data.crc;
                hdr.rom_file_size = ROM_FILE_SIZE;
                hdr.bank = app_data.bank;
                hdr.three_k = app_data.three_k;
                hdr.exrom = app_data.exrom;
                hdr.openb = app_data.openb;
                hdr.megaxrom = app_data.megaxrom;
                hdr.need_bios = app_data.need_bios;
                hdr.vpp = app_data.vpp;
                hdr.bios = app_data.bios;

                FIL fil;
                UINT bw;
                if (f_open(&fil, O2EM_COMBINED_FILE,
                           FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
                    f_write(&fil, &hdr, sizeof(hdr), &bw);
                    f_write(&fil, rom_table, 8 * 4096, &bw);
                    f_close(&fil);

                    FIL info;
                    if (f_open(&info, ROMINFOFILE,
                               FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
                        const char *p = O2EM_COMBINED_FILE;
                        while (*p) f_putc(*p++, &info);
                        f_close(&info);
                    }

                    free_rom_table();
                    printf("Combined BIOS+ROM written, rebooting to flash...\n");
                    watchdog_enable(1, 1);
                    while (1) tight_loop_contents();
                }
            }
        }

        // Initialize emulator
        o2em_init_keyboard();
        o2em_init_audio();
        init_display();
        init_cpu();
        init_system();

        if (showSplash && !Frens::isPsramEnabled())
        {
            showSplash = false;
            menuPumpBlankFrames(180);
        }

        Frens::PaceFrames60fps(true);
        start_tick_us = Frens::time_us();
        prevButtons[0] = prevButtons[1] = 0;

        // Emulation loop
        while (!key_done)
        {
            cpu_exec();
            processPerFrame();
        }

        // Free emulator graphics buffers (vscreen, col) so the next
        // game can re-allocate them. Without this we leak ~152KB per
        // game switch and the heap runs dry within a few menu cycles.
        close_display();

        key_done = 0;
        selectedRom[0] = 0;
        showSplash = false;
    }

    return 0;
}
