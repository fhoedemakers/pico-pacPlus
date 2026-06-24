# pico-pacPlus

## Introduction

**pico-pacPlus** is an Odyssey 2 / VideoPac+ (G7400) emulator for Raspberry Pi Pico, Pico 2, and other RP2040/RP2350-based microcontrollers. It is based on the [O2EM](https://sourceforge.net/projects/o2em/) emulator by Daniel Boris, Andre de la Rocha, and LABBE Corentin, ported to the Raspberry Pi Pico platform with SD card support, an on-screen menu system, and HDMI video output.

### Features

- **Odyssey 2 / VideoPac Emulation** -- Play Odyssey 2 and VideoPac cartridge ROM files (`.bin`) directly from an SD card
- **VideoPac+ (G7400) Support** -- Full support for the enhanced G7400 / VideoPac+ with its 16-color extended graphics
- **SD Card Menu System** -- Browse and launch games from an on-screen menu interface
- **USB Keyboard Support** -- Full keyboard mapping for games that use the Odyssey 2's built-in keyboard (letters, numbers, and special keys)
- **Dual Controller Support** -- Two simultaneous controllers; a single USB gamepad automatically mirrors to both emulated joystick ports, since Odyssey 2 games use either port 1 or port 2
- **BIOS Support** -- Automatically detects and loads the appropriate BIOS for Odyssey 2 or G7400/VideoPac+ systems
- **Flexible Hardware** -- [Compatible with standard DVI/HDMI breakout boards](#possible-configurations), with optional [custom PCB](#pcb-with-raspberry-pi-pico-or-pico-2) and 3D-printed case designs

### Setup Overview

1. Prepare an SD card formatted as FAT32 or exFAT
2. Copy your Odyssey 2 / VideoPac ROM files (`.bin`) to the card (subdirectory organization is supported)
3. Copy the required BIOS file(s) to the `/bios/` directory on the SD card (see [BIOS Setup](#bios-setup) below)
4. Insert the SD card into the device
5. Use the menu to browse, select, and play games

### BIOS Setup

The emulator requires a BIOS ROM to run. Place your BIOS file(s) in the `/bios/` directory on the SD card. The emulator will search this directory and auto-detect the BIOS type.

- For **Odyssey 2 / VideoPac** games, use the standard Odyssey 2 BIOS
- For **VideoPac+ / G7400** games, the G7400 BIOS is needed for enhanced graphics support

### Project Information

This project is based on [O2EM](https://sourceforge.net/projects/o2em/) (originally by Daniel Boris, continued by Andre de la Rocha) and [O2EM2](https://music.mzis.net/bin/o2em2/) by LABBE Corentin, ported to the Raspberry Pi Pico platform by [@frenskefrens](https://github.com/fhoedemakers).

There are also emulator ports for other systems using the same hardware platform. You can find them here:

- NES (Nintendo Entertainment System): [pico-infonesPlus](https://github.com/fhoedemakers/pico-infonesPlus)
- Sega Master System / Game Gear: [pico-smsplus](https://github.com/fhoedemakers/pico-smsplus)
- Nintendo Game Boy / Game Boy Color: [pico-peanutGB](https://github.com/fhoedemakers/pico-peanutGB)
- Sega Mega Drive / Genesis: [pico-genesisPlus](https://github.com/fhoedemakers/pico-genesisPlus)
- If you have an [Adafruit Fruit Jam](https://www.adafruit.com/product/6200): See this multi-emulator project: [retroJam](https://github.com/fhoedemakers/retroJam)

***

## Possible configurations

For detailed instructions on all possible hardware configurations, wiring diagrams, pinouts, flashing instructions, PCB designs, 3D-printed cases, and board-specific setup, please refer to the **[pico-infonesPlus README](https://github.com/fhoedemakers/pico-infonesPlus/blob/main/README.md)**. The hardware setup is identical -- only the firmware (`.uf2` file) is different.

You can use pico-pacPlus with the following RP2040/RP2350 boards and configurations:

- A custom printed circuit board (PCB) designed by [@johnedgarpark](https://twitter.com/johnedgarpark). (requires soldering) Up to two NES controller ports can be added to this PCB. Can also be used with a USB gamecontroller. You can 3D print your own case for the PCB.

- An additional PCB design for Waveshare RP2040 & RP2350 Zero including case design by DynaMight1124 based around cheaper but harder to solder components for those that fancy a bigger challenge. It also allows the design to be smaller.

- [Adafruit Feather RP2040 with DVI](https://www.adafruit.com/product/5710) (HDMI) Output Port. For use with a USB gamecontroller, up to two legacy NES controllers, or even a WII classic controller. Requires an SD card breakout board and breadboard.

- [Waveshare RP2040-PiZero Development Board](https://www.waveshare.com/rp2040-pizero.htm). For use with a USB gamecontroller, up to two legacy NES controllers, or a WII classic controller. (No soldering required)

- [Waveshare RP2350-PiZero Development Board](https://www.waveshare.com/rp2350-pizero.htm). Supports the optional PSRAM chip.

- [Adafruit Metro RP2350](https://www.adafruit.com/product/6003). Supports the optional PSRAM chip.

- [Pimoroni Pico Plus 2](https://shop.pimoroni.com/products/pimoroni-pico-plus-2?variant=42092668289107). Use the breadboard config. Works also on the discontinued Pimoroni Pico DV Demo base.

- [Adafruit Fruit Jam](https://www.adafruit.com/product/6200). No additional hardware is required apart from a USB gamepad. Audio is output through the monitor or the included speaker, with the option to connect external speakers and a Wii Classic controller via Stemma QT.

- [SpotPear HDMI](https://spotpear.com/index/product/detail/id/1207.html)

- [Murmulator M1 and M2 boards](https://murmulator.ru)

- (Discontinued) [Pimoroni Pico DV Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-dv-demo-base?variant=39494203998291). For use with a Raspberry Pi Pico, Pico 2, or Pimoroni Pico Plus 2.

- Raspberry Pi Pico or Pico 2 with [Adafruit DVI Breakout](https://www.adafruit.com/product/4984) and [Adafruit Micro-SD breakout board+](https://www.adafruit.com/product/254) on a breadboard. For use with a USB gamecontroller or up to two legacy NES controllers. (No soldering required)

***

## PSRAM

Some boards support additional memory called PSRAM with a capacity of 8MB. On certain boards this comes pre-installed, while on others it is optional and must be soldered manually. When PSRAM is detected, the emulator will automatically make use of it.

Without PSRAM, selecting a game ROM triggers a reboot: the ROM is written to flash memory during startup to prevent the system from locking up. This process is relatively slow, taking several seconds before the game starts.

With PSRAM, games are loaded directly from the SD card into PSRAM and executed immediately, resulting in much faster startup times.

| Board | PSRAM Included |
|:--|:--|
| [Waveshare RP2350-PiZero](https://www.waveshare.com/rp2350-pizero.htm) | No -- optional, must be soldered |
| [Adafruit Metro RP2350 with PSRAM](https://www.adafruit.com/product/6267) | Yes -- pre-installed |
| [Pimoroni Pico Plus 2](https://shop.pimoroni.com/products/pimoroni-pico-plus-2) | Yes -- pre-installed |
| [Adafruit Fruit Jam](https://www.adafruit.com/product/6200) | Yes -- pre-installed |

***

## Gamecontroller support

Depending on the hardware configuration, the emulator supports the following gamecontrollers. In some configurations, a USB-Y cable is needed to both connect power and a gamecontroller to the USB port.

### USB game controllers
- Sony DualShock 4
- Sony DualSense
- Genesis Mini 1 and 2
- PlayStation Classic
- USB Keyboard
- XInput type of controllers like Xbox 360 and Xbox One controllers and other XInput compatible controllers like 8BitDo

> [!NOTE]
> There is some input lag when using USB controllers.

### Legacy controllers
- One or two original NES controllers. In some configurations, soldering is required.
- WII Classic controller: supported on Adafruit Feather RP2040, WaveShare RP2040/RP2350-PiZero, Adafruit Metro RP2350, and Adafruit Fruit Jam boards.

### About the Odyssey 2 keyboard

The Odyssey 2 / VideoPac had a built-in membrane keyboard, and many games require keyboard input (for example, to select game modes, enter names, or play educational titles). When a USB keyboard is connected, the emulator maps standard keyboard keys directly to the Odyssey 2 keyboard matrix:

- Letters A-Z
- Numbers 0-9
- Space, Enter, Minus, Equals, Period, Slash
- Numpad: +, /, *

This makes keyboard-driven games fully playable.

### About two player games

The Odyssey 2 had two joystick ports, and games freely chose which port to read -- for example, *K.C. Munchkin!* reads port 1, while *Alien Invaders* reads port 2. When only one USB gamepad is connected (the common case), the emulator automatically mirrors input to both emulated joystick ports so all games work with a single controller. When two USB gamepads are connected, each controls its own port for true two-player games.

***

## Warning

The emulator overclocks the Pico to 252 MHz in order to get the emulator working fast enough. Overclocking can reduce the Pico's lifespan.

Use this software at your own risk! I will not be responsible in any way for any damage to your Pico and/or connected peripherals caused by using this software.

I also do not take responsibility in any way when damage is caused to the Pico or display due to incorrect wiring or voltages.

***

## Gamepad and keyboard usage

|     | (S)NES | Genesis | XInput | DualShock/Sense |
| --- | ------ | ------- | ------ | --------------- |
| Button1 | B  |    A    |   A    |    X            |
| Button2 | A  |    B    |   B    |   Circle        |
| Select  | Select | Mode or C | Select | Select   |

### Menu
Gamepad buttons:
- UP/DOWN: Next/previous item in the menu
- LEFT/RIGHT: Next/previous page
- Button2: Open folder / flash and start game
- Button1: Back to parent folder
- SELECT: Opens the settings menu. Here you can change settings like screen mode, scanlines, framerate display, menu colors and other board-specific settings.

When using a USB keyboard:
- Cursor keys: Up, Down, Left, Right
- Z: Back to parent folder
- X: Open folder / flash and start a game
- A: Acts as the SELECT button

### Emulator (in game)
Gamepad buttons:
- SELECT + START: Opens the settings menu. From there you can quit the game and return to the SD card menu, or adjust settings and resume.
- SELECT + UP / SELECT + DOWN: Switch screen modes
- START + Button2: Toggle framerate display
- **Pimoroni Pico DV Demo Base only**: SELECT + LEFT: Switch audio output to the connected speakers on the line-out jack.
- **Fruit Jam only**:
  - SELECT + UP: Toggle scanlines
  - Pushbutton 2 (on board) or SELECT + RIGHT: Toggle VU meter on or off
  - START + LEFT/RIGHT: Adjust volume of built-in speaker and external audio jack

When using a USB keyboard in-game:
- Cursor keys: Up, Down, Left, Right
- A: SELECT
- S: START
- Z: Button1
- X: Button2
- All letter keys, number keys, and supported special keys are passed through to the Odyssey 2 keyboard matrix for games that require keyboard input.

***

## Known issues and limitations

- This is a work in progress. Not all games may run correctly.
- Some games may have graphical or audio glitches.

***

## Building from source

Clone the repository and initialize the submodules:

```bash
git clone https://github.com/fhoedemakers/pico-pacPlus.git
cd pico-pacPlus
git submodule update --init
chmod +x pico_shared/bld.sh buildAll.sh
```

To build every supported hardware configuration in one go, run `./buildAll.sh`. It invokes `bld.sh` for each config, puts the resulting `.uf2` files in the `releases/` folder, and prints `picotool info` for each one. Copy the `.uf2` matching your board to your Pico via BOOTSEL.

To build a single configuration, use `bld.sh` (a thin wrapper around `pico_shared/bld.sh`):

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
  -b: build for the resident emuLoader bootloader (links the image at 0x10100000 instead of 0x10000000)
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

When using Visual Studio Code, choose the Release or the RelWithDebugInfo build variant.

***

## Credits

O2EM is originally programmed by [Daniel Boris](https://sourceforge.net/projects/o2em/) and continued by [Andre de la Rocha](https://sourceforge.net/projects/o2em/). [O2EM2](https://music.mzis.net/bin/o2em2/) is an enhanced version by [LABBE Corentin](https://music.mzis.net/).

Pico port, SD card support, menu system, and platform integration by [@frenskefrens](https://github.com/fhoedemakers).

HSTX HDMI/DVI driver with audio using [pico_hdmi](https://github.com/fliperama86/pico_hdmi) by [fliperama86](https://github.com/fliperama86).

Pico DVI driver by [Shuichi Takano](https://github.com/shuichitakano).

PCB design by [John Edgar Park](https://twitter.com/johnedgarpark).

Additional PCB design and 3D-printable case by [Gavin Knight (DynaMight1124)](https://github.com/DynaMight1124).

NES gamepad and WII Classic controller support contributed by [PaintYourDragon](https://github.com/PaintYourDragon) & [Adafruit](https://github.com/adafruit).

XInput driver: [tusb_XInput](https://github.com/Ryzee119/tusb_XInput) by [Ryzee119](https://github.com/Ryzee119).

FatFS driver: [pico_fatfs](https://github.com/elehobica/pico_fatfs) by [elehobica](https://github.com/elehobica).

[Anthropic Claude Opus 4.6](https://www.anthropic.com/claude/opus) assisted with the Pico port of the O2EM emulator core and general code optimizations.

***

## Licenses

- **O2EM / O2EM2 emulator core**: [Clarified Artistic License](o2em2-1.51/LICENSE.TXT)
- **pico_shared**: GNU GPLv3
- **pico_lib (DVI driver)**: MIT License
- **tusb_xinput**: MIT License
