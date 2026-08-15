# pico-pacPlus

## Introduction

**pico-pacPlus** is an Odyssey 2 / VideoPac (G7000) emulator for RP2040- and RP2350-based microcontrollers. It is based on the [O2EM](https://sourceforge.net/projects/o2em/) emulator core by Daniel Boris and Andre de la Rocha and the [O2EM2](https://music.mzis.net/bin/o2em2/) enhancements by LABBE Corentin, integrated with the video, audio, menu, and SD card framework from [pico-infonesPlus](https://github.com/fhoedemakers/pico-infonesPlus).

| | |
|-----|-----|
| <img alt="Screenshot 2026-07-02 16-31-27" src="https://github.com/user-attachments/assets/f056ad3d-8a29-4955-9451-97eaac7ffc91" /> | <img alt="Screenshot 2026-07-02 16-31-46" src="https://github.com/user-attachments/assets/4cf6b178-cedf-438a-b2be-506193192d8e" /> |

> [!IMPORTANT]
> Both RP2040 (Pico 1) and RP2350 (Pico 2 and variants) boards are supported. RP2350 is recommended: a few games show minor visual glitches on RP2040.

This project is part of a family of Raspberry Pi Pico emulator projects:

- NES: [pico-infonesPlus](https://github.com/fhoedemakers/pico-infonesPlus)
- Sega Master System / Game Gear: [pico-smsplus](https://github.com/fhoedemakers/pico-smsplus)
- Game Boy / Game Boy Color: [pico-peanutGB](https://github.com/fhoedemakers/pico-peanutGB)
- Sega Mega Drive / Genesis: [pico-genesisPlus](https://github.com/fhoedemakers/pico-genesisPlus)
- PC Engine / TurboGrafx-16: [pico-pcePlus](https://github.com/fhoedemakers/pico-pcePlus)
- Multi-emulator bundle for the [Adafruit Fruit Jam](https://www.adafruit.com/product/6200): [retroJam](https://github.com/fhoedemakers/retroJam)

***

## What it emulates

- **Odyssey 2 / VideoPac cartridges** — Standard Odyssey 2 and Philips VideoPac cartridge dumps (`.bin`) are loaded directly from the SD card.
- **Dual joystick ports with auto-mirroring** — Odyssey 2 games freely choose which port to read (for example, *K.C. Munchkin!* reads port 1, *Alien Invaders* reads port 2). When only one USB gamepad is connected, the emulator automatically mirrors input to both emulated ports so all games work with a single controller. With two USB gamepads connected, each controls its own port for two-player games.
- **USB keyboard passthrough** — The Odyssey 2 had a built-in membrane keyboard and many games (particularly the educational titles) require keyboard input. A connected USB keyboard is mapped directly onto the Odyssey 2 keyboard matrix.
- **BIOS** — The Odyssey 2 BIOS (`o2rom.bin`) is loaded from `/bios/` on the SD card. See [BIOS](#bios) below.

***

## Setup Overview

1. Prepare an SD card formatted as FAT32 (recommended) or exFAT.
2. Transfer ROM files (`.bin`) to the card, preferably in `/roms/O2E` (subdirectory organization is supported).
3. Place BIOS files in `/bios/` on the SD card (see [BIOS](#bios) below).
4. Optionally install a [metadata pack](#metadata) for box art and descriptions.
5. Insert the SD card into the device and use the menu to browse, select, and play games.

***

## Hardware requirements

The emulator runs on both RP2040 and RP2350 boards:

- **RP2040 boards (Pico 1 / Pico W / Waveshare RP2040 boards / etc.)** — Fully supported. Because there is no PSRAM, the selected ROM is first written to flash on launch, which triggers a reboot; startup takes a few seconds. Some titles exhibit minor visual glitches.
- **RP2350 boards (Pico 2 / Pico 2 W / Waveshare RP2350 boards / Adafruit Metro RP2350 / Adafruit Fruit Jam / etc.)** — Recommended. On boards equipped with PSRAM the ROM is loaded straight into PSRAM and started immediately, without the reboot-and-write step.

For board-by-board wiring, supported display modes, PCB designs, 3D-printed cases, and which UF2 file to flash, refer to the [pico-infonesPlus documentation](https://github.com/fhoedemakers/pico-infonesPlus#setup). The set of supported boards and their pinouts is identical between the two projects — only the firmware (`.uf2` file) differs.

### PSRAM

Some boards support up to 8 MB of PSRAM. When PSRAM is detected the emulator uses it automatically, giving much faster game startup (no flash-and-reboot step).

| Board | PSRAM Included |
|:--|:--|
| [Waveshare RP2350-PiZero](https://www.waveshare.com/rp2350-pizero.htm) | No — optional, must be soldered |
| [Adafruit Metro RP2350 with PSRAM](https://www.adafruit.com/product/6267) | Yes — pre-installed |
| [Pimoroni Pico Plus 2](https://shop.pimoroni.com/products/pimoroni-pico-plus-2) | Yes — pre-installed |
| [Adafruit Fruit Jam](https://www.adafruit.com/product/6200) | Yes — pre-installed |

***

## BIOS

The emulator requires the Odyssey 2 BIOS to run. The BIOS is not distributed with the emulator; it must be supplied by the user.

### Placement

- Create a `/bios/` folder in the root of the SD card.
- Place the Odyssey 2 BIOS file there, named exactly `o2rom.bin`.

> [!NOTE]
> Without `/bios/o2rom.bin` on the SD card, games will fail to load.

***

## Metadata

The emulator can display box art and a short text description for each ROM when a metadata pack is present on the SD card. With the pack installed, pressing **START** on a ROM in the file browser displays its metadata; the screensaver also shows random box art.

A metadata pack can be downloaded from the [releases page](https://github.com/fhoedemakers/pico-pacPlus/releases) and extracted to the root of the SD card. It is installed under:

```
/metadata/O2E/
├── images/   (box art, named by ROM CRC32)
└── descr/    (text descriptions, named by ROM CRC32)
```

<img width="1920" height="1080" alt="Screenshot 2026-07-02 16-31-19" src="https://github.com/user-attachments/assets/4885c1a3-555f-4ee8-a8cb-1d2a3a4c935f" />

***

## Controls

The emulator presents the Odyssey 2 joystick + keyboard mapping on any connected USB controller, NES controller (directly wired), or Wii Classic Controller (via I²C). Supported USB devices include Sony DualShock 4 / DualSense, Sega Genesis Mini 1 and 2, PlayStation Classic, USB keyboards, and XInput-compatible controllers (Xbox 360, Xbox One, 8BitDo, and similar).

> [!NOTE]
> There is some input lag when using USB controllers.

### Legacy controllers

- One or two original NES controllers. In some configurations, soldering is required.
- Wii Classic Controller: supported on Adafruit Feather RP2040, WaveShare RP2040/RP2350-PiZero, Adafruit Metro RP2350, and Adafruit Fruit Jam boards.

### Button mapping

|         | (S)NES | Genesis   | XInput | DualShock/Sense | Wii Classic |
| ------- | ------ | --------- | ------ | --------------- | ----------- |
| Button1 | B      | A         | A      | X               | B           |
| Button2 | A      | B         | B      | Circle          | A           |
| Button3 | X (SNES only) | C  | Y      | Triangle        | X           |
| Select  | Select | Mode or C | Select | Select          | Select      |

> [!NOTE]
> An original NES controller has no Button3. Everything reachable with it can also be reached from the settings menu.

A **SNES controller wired to a NES controller port** uses the same buttons as the table above: Button1 is B and Button2 is A. Before v0.3 those pads were read positionally, so physical B acted as Button2, physical Y as Button1, and physical A did nothing at all. SNES→NES adapter cables with conversion logic inside report NES buttons in NES order and are unaffected.

### Menu

Gamepad:

- **UP/DOWN**: next / previous item
- **LEFT/RIGHT**: next / previous page
- **Button2**: open folder / flash and start game
- **Button1**: back to parent folder
- **Button3**: open the [recently played list](#recently-played-games)
- **START**: show [metadata](#metadata) and box art (when available)
- **SELECT**: open the settings menu (screen mode, scanlines, framerate display, menu colors, board-specific settings)

USB keyboard:

- **Cursor keys**: Up / Down / Left / Right
- **Z**: back to parent folder
- **X**: open folder / flash and start a game
- **C**: open the [recently played list](#recently-played-games)
- **S**: show [metadata](#metadata) and box art (when available)
- **A**: SELECT

### Recently played games

The menu keeps a list of the **last 20 games you started**, most recent first. Open it with **Button3** in the menu, or with the **Recently played** entry at the top of the settings menu. That entry is only there when the settings menu is opened from the menu — a game cannot be started from inside a running game.

> [!NOTE]
> On an original 3-button Genesis Mini controller, C acts as SELECT and opens the settings menu instead. Take the **Recently played** entry there.

In the list:

| Button | Action |
| ------ | ------ |
| UP/DOWN | Select a game. |
| Button2 | Start the highlighted game. |
| Button1 | Close the list and return to the menu. |
| SELECT | Remove the highlighted game from the list. Asks for confirmation first. This only removes the entry, the ROM on the SD card is left alone. |
| START | Show [metadata](#metadata) and box art (when available). |

Games are added to the list automatically when you start them, so nothing has to be enabled. Starting a game that is already in the list moves it back to the top. The list closes by itself after a minute without input.

The list is kept in **`/recent_O2E.txt`** in the root of the SD card, as plain text with one game per line. It survives a reboot and can be read, edited or deleted on a PC. Deleting the file simply empties the list, and a damaged file is treated as an empty list — unlike the settings file, nothing else is reset. Each emulator running under [pico-bootLoader](#running-under-pico-bootloader) keeps its own list.

If a game was moved, renamed or deleted on the SD card in the meantime, the list says so instead of starting it. Use SELECT to remove such an entry.

> [!NOTE]
> On boards without PSRAM, starting a game from the list still writes to flash and reboots, exactly as starting it from the file browser does. Nothing is cached between launches. See [PSRAM](#psram).

### In game

Gamepad:

- **SELECT + START**: open the settings menu — quit back to the SD card menu, or adjust settings and resume.
- **SELECT + UP / SELECT + DOWN**: switch screen modes
- **START + Button2**: toggle framerate display
- **Pimoroni Pico DV Demo Base only** — SELECT + LEFT: switch audio output to the speakers on the line-out jack.
- **Fruit Jam only**:
  - SELECT + UP: toggle scanlines
  - Pushbutton 2 (on board) or SELECT + RIGHT: toggle VU meter on/off
  - START + LEFT/RIGHT: adjust volume of the built-in speaker and external audio jack

USB keyboard in-game:

- **Cursor keys**: Up / Down / Left / Right
- **A**: SELECT
- **S**: START
- **Z**: Button1
- **X**: Button2
- All letter keys, number keys, and supported special keys are passed through to the Odyssey 2 keyboard matrix (letters A–Z, digits 0–9, Space, Enter, Minus, Equals, Period, Slash, and numpad `+ / *`), so keyboard-driven games are fully playable.

***

## Running under pico-bootLoader

Instead of flashing this emulator as the only application on your board, you can install it under [pico-bootLoader](https://github.com/fhoedemakers/pico-bootLoader). The bootloader turns an RP2350 board into a multi-system console: on power-on it shows a menu from which you pick an emulator or game, which is then launched straight from the SD card. Any reset or power cycle brings you back to that menu, so you no longer have to reconnect the board to a computer to switch systems.

Alongside pico-pacPlus, the bootloader can run the NES, SNES, Game Boy / Game Boy Color, Sega Master System / Game Gear, Sega Mega Drive / Genesis and PC Engine emulators, plus native *Doom* and *Duke Nukem 3D* ports.

**You do not have to build anything.** The bootloader's releases page provides a prebuilt bootloader UF2 for each supported board plus `pico-bootLoader_sdcard.zip`, an SD card archive containing ready-to-use emulator builds. In short:

1. Flash the bootloader UF2 for your board via BOOTSEL.
2. Unpack `pico-bootLoader_sdcard.zip` onto a FAT32 or exFAT SD card.
3. Copy your cartridge dumps to `/roms/O2E` and the Odyssey 2 BIOS to `/bios/o2rom.bin`, exactly as for a standalone install. The BIOS is not included in the archive — see [BIOS](#bios).
4. Insert the card and power on.

> [!NOTE]
> pico-bootLoader requires an **RP2350** board; RP2040 boards are not supported, because the flash layout and UF2 checks are RP2350-specific. See the bootloader's own readme for the current list of supported boards and for the SD card layout.

When the emulator runs under the bootloader, the settings menu gains an extra **Return to emulator selection** entry that takes you back to the picker.

Each emulator installed under the bootloader keeps its own settings file and its own [recently played list](#recently-played-games), so they never overwrite each other's.

If you do want to build the emulator for the bootloader yourself, use the `-b` flag described in [Building from source](#building-from-source). Those builds are relinked to the application partition at `0x10080000` and are written to the `releases_bl` folder instead of `releases`. Note that the binaries on this project's releases page are standalone builds and will **not** work under the bootloader.

***

## Building from source

### Prerequisites

- The [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk), with the `PICO_SDK_PATH` environment variable set.
- CMake 3.13 or later.
- The ARM GCC toolchain (and, for RISC-V builds, the RISC-V toolchain in `$PICO_SDK_PATH/toolchain/RISCV_RPI_2_0_0_2/bin`).
- `picotool` on the `PATH`.

### Clone and build

```bash
git clone --recurse-submodules https://github.com/fhoedemakers/pico-pacPlus.git
cd pico-pacPlus
chmod +x pico_shared/bld.sh buildAll.sh
```

Run `./buildAll.sh` to build every supported configuration and collect the resulting UF2 files in the `releases/` folder. Copy the `.uf2` matching your board to your Pico via BOOTSEL.

To build a single configuration, use `./bld.sh` (a thin wrapper around `pico_shared/bld.sh`):

```
Usage: ./bld.sh [-d] [-2 | -r] [-w] [-u] [-m] [-D] [-e] [-b] [-t path to toolchain] [-p nprocessors] [-c <hwconfig>]
Options:
  -d: build in DEBUG configuration
  -2: build for Pico 2 board (RP2350)
  -r: build for Pico 2 board (RP2350) with riscv core
  -u: enable PIO USB support (RP2350 only)
  -w: build for Pico_w or Pico2_w
  -m: run cmake only, do not build the project
  -e: use the pico-extras based I2S audio driver (default: legacy custom driver)
  -b: (RP2350 only, must also use -2) build for the resident emuLoader bootloader; relinks the
      image to the application partition at 0x10080000 instead of 0x10000000 and copies the
      resulting UF2 to releases_bl instead of releases
  -D: Force DVI over HSTX
  -c <hwconfig>: specify the hardware configuration
     1: Pimoroni Pico DV Demo Base (Default)
     2: Breadboard with Adafruit DVI Breakout Board and MicroSD card breakout board / Custom PCB
     3: Adafruit Feather RP2040 DVI
     4: Waveshare RP2040-PiZero
     5: Adafruit Metro RP2350
     6: Waveshare RP2040-Zero/RP2350-Zero with custom PCB
     7: WaveShare RP2350-PiZero (PIO USB enabled)
     8: Adafruit Fruit Jam (PIO USB enabled)
     9: WaveShare RP2350-USBA (PIO USB enabled)
     10: Spotpear HDMI board
     12: Murmulator M1
     13: Murmulator M2 (RP2350 only)
     14: Adafruit Feather RP2350 with TLV320DAC3100 I2S DAC, SD card breakout and PIO USB
```

When using Visual Studio Code, choose the Release or the RelWithDebInfo build variant.

> [!WARNING]
> The emulator overclocks the Pico (to 252 MHz on most configurations) to reach full speed. Overclocking can reduce the Pico's lifespan. Use this software at your own risk — I take no responsibility for any damage to your Pico or connected peripherals, including damage caused by incorrect wiring or voltages.

***

## Flashing

1. Hold the **BOOTSEL** button on the Pico while connecting it to a computer via USB.
2. The board mounts as a USB mass-storage device.
3. Copy the appropriate `.uf2` file to the device.
4. The board reboots automatically and starts the emulator.

***

## License

- **O2EM / O2EM2 emulator core**: [Clarified Artistic License](o2em2-1.51/LICENSE.TXT)
- **pico_shared**: GNU GPLv3
- **pico_lib (DVI driver)**: MIT License
- **tusb_xinput**: MIT License
- **pico_hdmi**: Unlicense license
***

## Credits

### Emulation core and framework

- [O2EM](https://sourceforge.net/projects/o2em/) — original Odyssey 2 emulator by [Daniel Boris](https://sourceforge.net/projects/o2em/), continued by [Andre de la Rocha](https://sourceforge.net/projects/o2em/).
- [O2EM2](https://music.mzis.net/bin/o2em2/) — enhanced O2EM fork by [LABBE Corentin](https://music.mzis.net/).
- [pico-infonesPlus](https://github.com/fhoedemakers/pico-infonesPlus) — shared video, audio, menu, and SD card infrastructure, by Frank Hoedemakers.
- Pico port, SD card support, menu system, and platform integration by [@frenskefrens](https://github.com/fhoedemakers).

### Video and display

- [PicoDVI](https://github.com/Wren6991/PicoDVI) — DVI output library by Wren6991.
- [pico_lib](https://github.com/shuichitakano/pico_lib) — PicoDVI driver by [Shuichi Takano](https://github.com/shuichitakano).
- [pico_hdmi](https://github.com/fliperama86/pico_hdmi) — HSTX HDMI/DVI driver with audio by [fliperama86](https://github.com/fliperama86).

### Storage

- [pico_fatfs](https://github.com/elehobica/pico_fatfs) — FatFs SD card driver by [elehobica](https://github.com/elehobica).

### USB and controllers

- [TinyUSB](https://github.com/hathach/tinyusb) — USB host stack.
- [Pico-PIO-USB](https://github.com/sekigon-gonnoc/Pico-PIO-USB) — software USB host implementation on PIO, by [sekigon-gonnoc](https://github.com/sekigon-gonnoc), used to expose a second USB port for controllers.
- [tusb_XInput](https://github.com/Ryzee119/tusb_XInput) — XInput controller driver by [Ryzee119](https://github.com/Ryzee119).
- NES gamepad and Wii Classic Controller support contributed by [PaintYourDragon](https://github.com/PaintYourDragon) and [Adafruit](https://github.com/adafruit).

### Hardware and assets

- PCB design by [John Edgar Park](https://twitter.com/johnedgarpark).
- Additional PCB design and 3D-printable case by [Gavin Knight (DynaMight1124)](https://github.com/DynaMight1124).
- Metadata box art sourced from [odyssey2.info](https://odyssey2.info).

### Contributions and assistance

- [Anthropic Claude Opus](https://www.anthropic.com/claude/opus) assisted with the Pico port of the O2EM emulator core and general code optimizations.
