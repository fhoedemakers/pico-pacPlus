# CHANGELOG

First public release of pico-pacPlus.

# General Info

[Binaries for each configuration and PCB design are at the end of this page](#downloads___).

[See setup section in readme how to install and wire up](https://github.com/fhoedemakers/pico-pacPlus#setup-overview)

> [!IMPORTANT]
> Both RP2040 (Pico 1) and RP2350 (Pico 2 and variants) boards are supported. RP2350 is recommended: a few games show minor visual glitches on RP2040.


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
