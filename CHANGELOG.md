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


# v0.3

## New

**Recently played**

The menu now keeps a list of the **last 20 games you started**, newest first. Open it with **X** in the ROM browser — that is button 3 on any pad: X on a SNES controller, Y on XInput, Triangle on PlayStation, C on Genesis — or from the new **Recently played** entry at the top of the settings menu.

In the list, **A** starts the highlighted game, **SELECT** removes it from the list, **START** shows its box art, and **B** closes the list. The settings menu only offers the entry when it is opened from the ROM browser, not from inside a running game.

The list is plain text in `/recent_O2E.txt` in the SD card root, one line per game, so it survives a reboot and can be edited or deleted on a PC. A game that is no longer on the card is reported as missing when you try to start it and can be dropped with SELECT. A damaged or unreadable list simply comes up empty — unlike the settings file, nothing gets reset. Each emulator installed under [pico-bootLoader](https://github.com/fhoedemakers/pico-bootLoader) keeps its own list.

**SNES controllers on a NES controller port use A and B**

A **SNES controller wired to a NES controller port** now uses its A and B buttons. Such a pad shifts out B and Y where a NES pad has A and B, so those were the two buttons that did anything, and physical A did nothing at all — in games, where B fired, and in the menu, where "choose" landed on B. Its four face buttons are now named rather than taken positionally: **A is the Odyssey² fire button**, and in the menu A chooses while B goes back, the same as on USB and Wii Classic pads. The Odyssey² joystick has only one action button, so X, Y, L and R have no equivalent and are ignored, as on those pads.

NES pads are unaffected, and so are SNES→NES adapter cables with conversion logic inside, which report NES buttons in NES order. The 12-button read is now confirmed against genuine SNES hardware, with a SNES controller port wired straight to the NES port GPIOs. Adapter *cables* are the thing to watch out for: several contain a converter, sometimes moulded into the plug, and then only 8 buttons can ever arrive.

## Fixes

**Controller Test**

- The screen now names the buttons of a GPIO-wired pad according to what is actually attached. It used to label them in SNES order unconditionally, which is wrong for a NES pad: a NES pad shifts out the same first bits with different meanings (bit 0 is A, not B, and bit 1 is B, not Y). A NES pad now gets NES names with its A/X/L/R cells blanked, and a SNES pad gets SNES names. A port that has not identified itself yet — an idle SNES pad, an empty port and an 8-bit adapter cable are indistinguishable on the wire — shows NES names but keeps A/X/L/R on screen, so pressing one of those switches it to SNES names.
- The screen also shows the **detected pad type** and, for the two GPIO ports, the **raw word the pad shifted out** (`Sent by pad: 0002 hex`), taken before any NES/SNES interpretation. This tells a button that never reaches the Pico apart from one that is decoded wrong.
- Leaving the Controller Test screen no longer drops into the screensaver. The settings menu's idle timeout mistook the "just came back from another screen" marker for a timestamp, so anything that opened a screen of its own looked like a minute of inactivity on return.

**Display (RP2040 / DVI boards)**

Line buffers queued for a scanline that a later margin change puts inside a blank margin are now retired instead of being stranded. Stranded buffers deadlocked both cores and left the display stuck on red lines. This is the failure the emulator already worked around by running with zero margins; the driver is now safe across the menu↔game transition regardless.

**Settings**

"Reset to defaults" no longer carries a stale scanline flag over from the loaded settings file.

## Developer

- The picoDVI line buffer pool can be sized independently with `-DDVI_N_LINE_BUFFERS=n` (default unchanged at 5). pico-pacPlus does not raise it — SRAM on the RP2040 line-streaming path is already at its ceiling.
- The HSTX debug dump reports HDMI audio underruns **per second** next to the cumulative count. The cumulative counter runs from boot and includes the ~11025/s produced while browsing ROMs, so it says nothing about whether underruns are still happening.
- `bld.sh` passes `$EXTRA_CMAKE_ARGS` through to cmake, so project-specific options can be set without changing the shared script.

## Known issues

- On boards without PSRAM, starting a game still writes to flash and reboots twice — once for the cartridge and once for the combined BIOS+ROM image — so launches are no quicker than in v0.2, including for a game you just played.
- A few titles show minor visual glitches on RP2040. RP2350 is recommended for best compatibility.
- Not all games have been tested; please register an issue when you encounter one.

## Use of AI

Parts of this release were developed with the help of [Anthropic Claude](https://www.anthropic.com/claude).


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
