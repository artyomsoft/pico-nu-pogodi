# Description

This is a simple emulator of the Soviet game "Nu, Pogodi!" written for the Raspberry Pico.
It is supposed to be implemented on perboard and optionally with 3D printed case.
Old version which implemented for breadboad is in 1.0.x branch

You can watch videos of how it works below.

[![Nu, Pogodi! emulator vs original](https://img.youtube.com/vi/qAGqy1fqH50/0.jpg)](https://www.youtube.com/watch?v=qAGqy1fqH50)

**Nu, Pogodi! emulator vs original**

[![Nu, Pogodi! emulator gameplay](https://img.youtube.com/vi/n9CVoQ6EzJo/0.jpg)](https://www.youtube.com/watch?v=n9CVoQ6EzJo)

**Nu, Pogodi! emulator gameplay**

# Used components

1. Raspberry Pi Pico (I used the WiFi version, but the regular one or even a Chinese clone should work).
2. Perfboard 120x80.
3. 8 momentary tactile buttons.
4. 2 Button with position locking.
5. MSP2806/MSP2807 display.
6. Passive piezo speaker.
7. 100 Ohm resistor.
8. 1 kOhm resistor.
9. 2N222 transistor.
10. TP4056 based charging module (HW-373).
11. Li-po 503450 battery.
12. 3D prited case.
13. 10 Screws 2x8.

# Schematic

![Schematic](https://github.com/artyomsoft/nu-pododi-pcb/blob/master/images/schematic-nu-pogodi.png)

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

## Usefull links

1. [PCB and schematic](https://github.com/artyomsoft/nu-pododi-pcb)
2. [3D Models of the case](https://github.com/artyomsoft/nu-pogodi-case)
3. [These 3D Models ready for printing on MakerWorld](https://makerworld.com/en/models/1602053-nu-pogodi-game-emulator)
