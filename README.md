# Description

This is a simple emulator of the Soviet game "Nu, Pogodi!" written for the Raspberry Pico. At the moment, it is implemented on a breadboard.

You can watch a video of how it works below.

[![Nu, Pogodi! Emulator on Raspberry Pico](https://img.youtube.com/vi/PANg4w-u8EY/0.jpg)](https://www.youtube.com/watch?v=PANg4w-u8EY)

# Used components

1. Raspberry Pi Pico (I used the WiFi version, but the regular one or even a Chinese clone should work),
2. Solderless breadboard,
3. 8 momentary tactile buttons,
4. 1 Button with position locking,
5. TFT display (I used one with the ILI9341 driver),
6. Passive piezo speaker,
7. 100 Ohm resistor,
8. 1 kOhm resistor,
9. 2N222 transistor,
10. Set of connecting wires with Dupont terminals,
11. Set of jumpers for solderless breadboards.

# Fritzing wiring diagram

![Fritzing wiring diagram](https://github.com/artyomsoft/pico-nu-pogodi/blob/1601ca157914c6db7b796f094fc5f9011df93252/resources/NuPogodi.png)

# How to build

1. Install dependencies
   ```bash
   $ sudo apt install cmake python3 build-essential gcc-arm-none-eabi   libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
   ```
2. Navigate to the working directory
   ```bash
   $ cd ~
   $ mkdir emulator && cd emulator
   ```
3. Clone the Pico SDK
   ```bash
   $ git clone https://github.com/raspberrypi/pico-sdk
   $ cd pico-sdk
   $ git submodule update --init --recursive
   ```
4. Clone the repository with the emulator
   ```bash
   $ cd ..
   $ git clone git@github.com:artyomsoft/pico-nu-pogodi.git
   $ cd pico-nu-pogodi
   $ git submodule update --init --recursive
   ```
5. Build the emulator
   ```bash
   export PICO_SDK_PATH=$(pwd)/../pico-sdk
   mkdir -p build
   cd build
   cmake .. -DPICO_COPY_TO_RAM=1
   make
   ```
6. Connect the Raspberry Pi Pico in bootloader mode and copy the pico_nupogodi.uf2 file to it
