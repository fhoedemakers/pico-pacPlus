# Pico-pacPlus, a Philips VideoPac G7000/Magnavox Odyssey 2 emulator for RP2350

This software is a Philips VideoPac G7000/Magnavox Odyssey 2 emulator for RP2350 based microcontroller boards like the Raspberry Pi Pico 2. Sound and video are output over HDMI.

Put your VideoPac/Odyssey 2 rom files (.bin) and optional [metadata](#using-metadata) on a FAT32 or exFAT formatted SD card. You can organize the roms in directories. A menu is displayed on which you can select the rom to play.

> [!NOTE]
> The emulator only runs on RP2350 based boards. It is not supported on the original Raspberry Pi Pico or other RP2040 based boards.

## BIOS requirements

The Philips VideoPac G7000/Magnavox Odyssey 2 requires a BIOS ROM file to function. You must place the BIOS file on the SD card root directory.

| System | BIOS filename | Description |
| ------ | ------------- | ----------- |
| Philips VideoPac G7000 / Magnavox Odyssey 2 | `o2rom.bin` | Required system BIOS |

> [!IMPORTANT]
> The emulator will not start games without the BIOS file present on the SD card. You must legally obtain the BIOS file from your own hardware.

## System requirements - What do you need?

> [!NOTE]
> For detailed instructions and specific configurations, see the [Pico-InfonesPlus sister project](https://github.com/fhoedemakers/pico-infonesPlus).

### Hardware

A **Raspberry Pi Pico 2** on a Pimoroni Pico DV Demo Base, or a **Raspberry Pi Pico 2** on a breadboard or PCB.

Other boards that can be used:
- [Adafruit Fruit Jam](https://www.adafruit.com/product/6200)
- [Adafruit Metro RP2350](https://www.adafruit.com/product/6003) or [Adafruit Metro RP2350 with PSRAM](https://www.adafruit.com/product/6267)
- [Pimoroni Pico Plus 2](https://shop.pimoroni.com/products/pimoroni-pico-plus-2?variant=42092668289107)
  Use the breadboard config or Pimoroni Pico DV Demo Base. This board does not fit the PCB because of the SP/CE connector on the back of the board.
  The PSRAM on the board is used instead of flash to load the roms from SD.

The binary specific for your config and optional PCB gerber files can be downloaded from the [releases](https://github.com/fhoedemakers/pico-pacPlus/releases/latest) page.

## Supported controllers and in-game button mapping

- Dual Shock/Dual Sense and PSClassic.
- Xbox style controllers (XInput)
- Vintage NES controller
- AliExpress SNES USB controller: **Note** To enable B-button you need to press Y on this controller every time you start a game or boot into the menu.
- AliExpress NES USB controller
- Fruit Jam: SNES Classic/WII Classic Pro controllers over I2C. Connect controller to [Adafruit Wii Nunchuck Breakout Adapter - Qwiic / STEMMA QT](https://www.adafruit.com/product/4836).
- USB Keyboard

|     | (S)NES | Genesis | XInput | Dual Shock/Sense |
| --- | ------ | ------- | ------ | ---------------- |
| Button1 | B  |    A    |   A    |    X             |
| Button2 | A  |    B    |   B    |   Circle         |
| Select  | select | Mode or C | Select | Select     |

## Menu

Gamepad buttons:
- UP/DOWN: Next/previous item in the menu.
- LEFT/RIGHT: Next/previous page.
- Button2: Open folder/flash and start game.
- Button1: Back to parent folder.
- START: Show [metadata](#using-metadata) and box art (when available).
- SELECT: Opens a settings menu. Here you can change settings like screen mode, scanlines, framerate display, menu colors and other board specific settings. Settings can also be changed in-game by pressing some button combinations as explained below. The settings menu can also be opened in-game.

When using a USB keyboard:
- Cursor keys: Up, Down, Left, Right
- Z: Back to parent folder
- X: Open folder/flash and start a game
- S: Show [metadata](#using-metadata) and box art (when available).
- A: Acts as the SELECT button.

## Emulator (in game)

Gamepad buttons:
- SELECT + START, Xbox button: Opens the settings menu. From there, you can:
  - Quit the game and return to the SD card menu
  - Adjust settings and resume your game.
- SELECT + UP/SELECT + DOWN: Switches screen modes.
- SELECT + Button1/Button2: Toggle rapid-fire.
- START + Button2: Toggle framerate display.
- **Pimoroni Pico DV Demo Base only**: SELECT + LEFT: Switch audio output to the connected speakers on the line-out jack of the Pimoroni Pico DV Demo Base. The speaker setting will be remembered when the emulator is restarted.
- **Fruit Jam Only**
  - SELECT + UP: Toggle scanlines.
  - Pushbutton 1 (on board): Mute audio of built-in speaker. Audio is still outputted to the audio jack.
  - Pushbutton 2 (on board) or SELECT + RIGHT: Toggles the VU meter on or off. (NeoPixel LEDs light up in sync with the music rhythm)
- **Genesis Mini Controller**: When using a Genesis Mini 3 button controller, press C for SELECT. On the 8-button Genesis controllers, MODE acts as SELECT.
- **USB keyboard**: When using a USB keyboard:
  - Cursor keys: Up, Down, Left, Right
  - A: SELECT
  - S: START
  - Z: Button1
  - X: Button2

## Using metadata

Download the metadata pack from the [releases page](https://github.com/fhoedemakers/pico-pacPlus/releases/latest) (when available). It contains box art and game info for many games. The metadata is used in the menu to show box art and game info when a rom is selected. Press START to view the information. When the screensaver is started, random box art is shown.

- Download the pack from the releases page.
  - Extract the zip contents to the **root of the SD card**.
  - In the menu:
    - Highlight a game and press **START** → show cover art and metadata.
    - Press **SELECT** → show full game description.
    - Press **Button1** → return to menu.
    - Press **START** or **Button2** → start the game.

## Building from source

Raspberry Pi Pico 2 ARM-S is the only supported config. Builds for RISC-V currently do not work.

Building for breadboard and PCB configurations:

```bash
git clone https://github.com/fhoedemakers/pico-pacPlus.git
cd pico-pacPlus
git submodule update --init
./bld.sh -c2 -2
```

Building for the Pimoroni [Pico DV Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-dv-demo-base?variant=39494203998291):

```bash
git clone https://github.com/fhoedemakers/pico-pacPlus.git
cd pico-pacPlus
git submodule update --init
./bld.sh -c1 -2
```

Building for the Adafruit [Fruit Jam](https://www.adafruit.com/product/6200):

```bash
git clone https://github.com/fhoedemakers/pico-pacPlus.git
cd pico-pacPlus
git submodule update --init
./bld.sh -c8
```

Use `./bld.sh -h` for all available options.

The resulting .uf2 file will be in the `releases/` folder. Copy it to the Pico when in bootloader mode.
