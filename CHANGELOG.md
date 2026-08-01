# CHANGELOG

**pico-pacPlus** is a Magnavox Odyssey² / Philips VideoPac (G7000) emulator for RP2040 and RP2350 boards. Cartridge dumps are loaded straight from an SD card, video and audio go out over HDMI or DVI, and USB, directly wired NES/SNES and Wii Classic controllers all work — as does a USB keyboard, which many Odyssey² titles need.

Release notes are below, newest first. [Binaries for every board configuration are at the end of this page](#downloads___).

# Getting started

1. Format an SD card as FAT32 (recommended) or exFAT and copy your cartridge dumps (`.bin`) to `/roms/O2E`. Subdirectories are supported.
2. Copy the Odyssey² BIOS to `/bios/o2rom.bin` on the same card. It is not distributed with the emulator and has to be supplied by you; without it, games fail to load.
3. Pick the `.uf2` for your board from the [table at the bottom of this page](#downloads___), hold **BOOTSEL** while connecting the board over USB, and copy the file to the drive that appears. The board reboots into the emulator by itself.
4. Optionally extract the [metadata pack](#downloads___) to the root of the card for box art and descriptions.

[Full setup instructions are in the readme](https://github.com/fhoedemakers/pico-pacPlus#setup-overview). For board-by-board wiring, display modes, PCB designs and 3D-printable cases, see the [pico-infonesPlus documentation](https://github.com/fhoedemakers/pico-infonesPlus#setup) — the supported boards and their pinouts are identical between the two projects, only the firmware differs.

> [!IMPORTANT]
> Both RP2040 (Pico 1) and RP2350 (Pico 2 and variants) boards are supported. RP2350 is recommended: a few games show minor visual glitches on RP2040.

> [!NOTE]
> Upgrading from an earlier version only means flashing the new `.uf2`. The settings file on the SD card is unchanged, so your screen mode, colours and other preferences carry over.


# v0.2

## New

**Controller Test**

The settings menu (SELECT) has a new **Controller Test** entry. Press a button on any controller and watch it light up on a drawing of a gamepad, with a list underneath of everything that is connected. Useful when a controller does not respond and you want to know whether it is the pad, the port, or the wiring. Hold SELECT+START for two seconds to go back.

**SNES controllers**

The port for directly wired controllers now accepts SNES controllers as well as NES ones. There is nothing to set: the emulator recognises which type is plugged in. NES controllers keep working exactly as before.

**Keyboard, mouse and gamepad together**

A USB mouse — or a wireless keyboard with a built-in touchpad, which counts as one — no longer takes up a controller slot. Keyboard, mouse and gamepad can all be connected at once without one of them being ignored.

**Xbox controllers**

The left thumbstick now works for movement, next to the d-pad.

**More buttons recognised**

Extra buttons on the DualShock 4 / DualSense, PlayStation Classic, Retro-bit Mega Drive Arcade pad, Wii Classic Controller and USB keyboards are now picked up. An Odyssey² joystick has only one action button, so you will mostly notice this in the new Controller Test screen.

## Fixes

**Sound**

- **HDMI (Pico 2 / RP2350 boards)**: fixed a brief audio dropout that returned every few seconds. Also fixed silent HDMI audio on televisions and monitors that are strict about the audio format — displays that stayed quiet before should now play sound.
- **Fruit Jam and Feather RP2350 with DAC**: fixed having no sound at all when a Wii Classic or SNES Classic Mini controller was already plugged in at power-on. That controller now also works from the moment the menu appears, instead of after a short delay.

**SD card and games**

- The game browser no longer gets stuck when the folder you last used is missing from the card; it returns to the root folder instead.
- More reliable SD card access, from an updated version of the SD card driver.
- On boards without PSRAM, starting a game can no longer erase the small area of flash memory where the board keeps its start-up settings.

**Stability**

- Fixed a rare crash caused by a memory alignment problem.
- Reduced the chance of the screen staying black at start-up on RP2040 boards.
- The status LEDs no longer cause problems when they fail to start.

## Playing several emulators on one board

pico-pacPlus can also run as part of [pico-bootLoader](https://github.com/fhoedemakers/pico-bootLoader), which keeps several emulators — and a version of Doom — on a single RP2350 board and shows a menu at power-on to choose between them. From the settings menu you can return to that menu without touching the board. Those images are released together with pico-bootLoader; the downloads at the bottom of this page are the normal standalone versions.

The layout used for these combined installations changed in this release, so a bootable pico-pacPlus image and pico-bootLoader itself have to come from the same generation — do not mix a v0.1 image with a newer loader or the other way around. Building such an image also works again with the latest version of picotool.

## Known issues

- A few titles show minor visual glitches on RP2040. RP2350 is recommended for best compatibility.
- Not all games have been tested; please register an issue when you encounter one.

## Use of AI

Parts of this release were developed with the help of [Anthropic Claude](https://www.anthropic.com/claude).


# v0.1

First public release. There will be bugs. Please register an issue when you encounter one.

## Features

**Odyssey 2 / VideoPac**

- Standard Odyssey 2 and Philips VideoPac cartridge ROMs (`.bin`) are loaded directly from the SD card.
- Runs on RP2040 (Pico 1, Pico W, Waveshare RP2040 boards, and similar) and on RP2350 (Pico 2, Pico 2 W, Waveshare RP2350 boards, Adafruit Metro RP2350, Adafruit Fruit Jam, and similar).
- On boards without PSRAM the ROM is written to flash on launch, which triggers a short reboot; on PSRAM boards the ROM loads straight into PSRAM and starts immediately.

**BIOS**

- The Odyssey 2 BIOS is loaded from `/bios/o2rom.bin` on the SD card. Only this filename is recognised; the BIOS itself must be supplied by the user.

**Controllers**

- USB controllers including Sony DualShock 4 / DualSense, 8BitDo, XInput (Xbox 360 / Xbox One), Sega Genesis Mini 1 and 2, PlayStation Classic, and USB keyboards.
- Directly wired NES gamepads (one or two).
- Wii Classic Controller over I²C.
- Dual joystick ports with single-controller **auto-mirroring**: with one USB gamepad connected the emulator mirrors input to both emulated ports, so games that read port 1 (*K.C. Munchkin!*) and games that read port 2 (*Alien Invaders*) both work; with two USB gamepads connected each controls its own port for true two-player games.

**USB keyboard**

- Keyboard passthrough to the Odyssey 2 keyboard matrix: letters A–Z, digits 0–9, Space, Enter, Minus, Equals, Period, Slash, and numpad `+ / *`. Educational titles and other keyboard-driven games are fully playable.

**Audio**

- Odyssey 2 / VideoPac+ sound emulation.
- Optional I²S external audio output on supported boards (Pimoroni Pico DV Demo Base line-out, Fruit Jam speaker + jack, Adafruit Feather RP2350 + TLV320DAC3100).

**Display**

- HDMI output via HSTX on RP2350 boards and via PicoDVI on RP2040 boards.
- Screen mode switching, scanline effect, FPS overlay, VU meter (Fruit Jam).

**Metadata**

- Box art and short text descriptions are displayed in the file browser when a metadata pack is installed under `/metadata/O2E/`. The screensaver shows random box art.

## Known issues

- A few titles show minor visual glitches on RP2040. RP2350 is recommended for best compatibility.
- Not all games have been tested; please register an issue when you encounter one.

## Use of AI

The Pico port of the O2EM emulator core and general code optimizations were developed with the help of [Anthropic Claude Opus 4.6](https://www.anthropic.com/claude/opus).


<a name="downloads___"></a>
## Downloads by configuration

Binaries for each configuration are listed below. For board-by-board wiring, supported display modes, PCB designs, 3D-printable cases, and which UF2 file to flash, refer to the [pico-infonesPlus documentation](https://github.com/fhoedemakers/pico-infonesPlus#setup). The set of supported boards and their pinouts is identical between the two projects.

### Standalone boards

| Board | Binary |
|:--|:--|
| Adafruit Metro RP2350 | [picoPacPlus_AdafruitMetroRP2350_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_AdafruitMetroRP2350_arm.uf2) |
| Adafruit Fruit Jam | [picoPacPlus_AdafruitFruitJam_arm_piousb.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_AdafruitFruitJam_arm_piousb.uf2) |
| Waveshare RP2350-PiZero | [picoPacPlus_WaveShareRP2350PiZero_arm_piousb.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_WaveShareRP2350PiZero_arm_piousb.uf2) |
| Adafruit Feather RP2040 DVI | [picoPacPlus_AdafruitFeatherDVI_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_AdafruitFeatherDVI_arm.uf2) |
| Waveshare RP2040-PiZero | [picoPacPlus_WaveShareRP2040PiZero_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_WaveShareRP2040PiZero_arm.uf2) |

### Breadboard / Custom PCB

Adafruit DVI Breakout + MicroSD card breakout, or the custom PCB.

| Board | Binary |
|:--|:--|
| Pico | [picoPacPlus_AdafruitDVISD_pico_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_AdafruitDVISD_pico_arm.uf2) |
| Pico W | [picoPacPlus_AdafruitDVISD_pico_w_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_AdafruitDVISD_pico_w_arm.uf2) |
| Pico 2 / Pimoroni Pico Plus 2 | [picoPacPlus_AdafruitDVISD_pico2_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_AdafruitDVISD_pico2_arm.uf2) |

### PCB Waveshare RP2040-Zero / RP2350-Zero (PCB required)

| Board | Binary |
|:--|:--|
| Waveshare RP2040-Zero | [picoPacPlus_WaveShareRP2040ZeroWithPCB_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_WaveShareRP2040ZeroWithPCB_arm.uf2) |
| Waveshare RP2350-Zero | [picoPacPlus_WaveShareRP2350ZeroWithPCB_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_WaveShareRP2350ZeroWithPCB_arm.uf2) |

### PCB Waveshare RP2350-USBA (PCB required)

[Binary](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_WaveShare2350USBA_arm_piousb.uf2)

### Pimoroni Pico DV Demo Base

| Board | Binary |
|:--|:--|
| Pico | [picoPacPlus_PimoroniDVI_pico_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_PimoroniDVI_pico_arm.uf2) |
| Pico 2 / Pimoroni Pico Plus 2 | [picoPacPlus_PimoroniDVI_pico2_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_PimoroniDVI_pico2_arm.uf2) |

### SpotPear HDMI

For more info about the SpotPear HDMI see https://spotpear.com/index/product/detail/id/1207.html and https://spotpear.com/index/study/detail/id/971.html.

| Board | Binary |
|:--|:--|
| Pico | [picoPacPlus_SpotpearHDMI_pico_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_SpotpearHDMI_pico_arm.uf2) |
| Pico 2 / Pico 2 W | [picoPacPlus_SpotpearHDMI_pico2_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_SpotpearHDMI_pico2_arm.uf2) |

### Murmulator M1

For more info about the Murmulator see https://murmulator.ru/.

| Board | Binary |
|:--|:--|
| Pico | [picoPacPlus_MurmulatorM1_pico_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_MurmulatorM1_pico_arm.uf2) |
| Pico 2 / Pico 2 W | [picoPacPlus_MurmulatorM1_pico2_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_MurmulatorM1_pico2_arm.uf2) |

### Murmulator M2

For more info about the Murmulator see https://murmulator.ru/.

| Board | Binary |
|:--|:--|
| Pico 2 / Pico 2 W | [picoPacPlus_MurmulatorM2_arm.uf2](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/picoPacPlus_MurmulatorM2_arm.uf2) |

### Other downloads

- Metadata: [pacPlusMetadata.zip](https://github.com/fhoedemakers/pico-pacPlus/releases/latest/download/pacPlusMetadata.zip)

Extract the zip file to the root folder of the SD card. Select a game in the menu and press START to show more information and box art. Works for most official released games. The screensaver shows floating random cover art.
