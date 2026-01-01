# Advent of Blinkenlights

This repository contains a sort-of an Advent of Code, but it's blinkenlights on various microcontroller boards.
The goal was simple - create a nontrivial project that runs on multiple different platform dating as far back as the 1970'ties. Natuarally, the project couldn't be too complex, as some of these platforms are fairly limited in terms of memory, so a 16-LED strip "blinkenlights" was chosen.
Each board expects to sink current for 16 LEDs connected to the pins listed in `main.cpp`. There are several time-based animations available that switch each minute. The boards are also logging some stuff on their respective main serial consoles.

![](./images/blink.gif)

## Temex CPU card
![](./images/temex.jpg)

Core: Z80, 8 bit

Clock: 2.4576 MHz

SRAM: 8k

EEPROM: 8k

GPIO: 16 I/O via Z80 PIO, 16 input only via memory mapped registers.

Peripherals: 2 * UART (Z80 SIO), RTC (ICM7170IPG), 4 Timers (Z80 CTC), WDT (MAX691CPE).

Other: SRAM & RTC battery backup (MAX691CPE), second EEPROM slot (max 8k).

Power draw: 2.5W (9V input)

More on the card here: https://github.com/Idorobots/temex-reverse

Some caveats with this board:
- Most of the chips on the board had to be replaced due to all kinds of weird failures - the original chips include the CPU, SIO, MAX691CPE and the misc 74XX logic.
- The CPU reset line (from MAX691CPE) had a massive capacitor on it, causing erractic boot up behaviour. Had to be replaced with a smaller value.
- Neither EEPROM nor RAM slots utilize the top two JEDEC-pinout address lines, meaning that no more than 8k of memory is addressable in each slot, despite the memory mapping allowing for 16k addresses. The board can be modified to support 32k ROM and 16 RAM (although, all slots need to be populated with 32k chips).
- RTC/NVRAM battery was not present, not sure what type should be used.
- RTC clock's oscillator circuit is hooked up to the main 5V line, meaning that when there's no power, it does not oscillate. Very real time, much convenient.
- RTC chip requires pullups on RD & WR lines during external backup. These were not present on the board preventing the chip from counting time during backup.

### Build
Requires SDCC & related binutils. Tested under the following version:

```
SDCC : mcs51/z80/z180/r2k/r2ka/r3ka/sm83/tlcs90/ez80_z80/z80n/r800/ds390/pic16/pic14/TININative/ds400/hc08/s08/stm8/pdk13/pdk14/pdk15/mos6502/mos65c02/f8 4.5.0 #15242 (Linux)
```

To build the project:

```
make temex-clean && make temex
truncate --size=8k firmware/temex.bin
```

Flash the ROM chip in EEPROM1 position.

## Axiom CME11A
![](./images/cme11a.jpg)

Core: MC68HC11A1FN, M68HC11, 8 bit

Clock: 8 MHz

SRAM: 8k (external), 512 internal

EEPROM: 16k (external), 512 internal

GPIO: 11 I/O (Port D, Port A), 11 input only (Port A, Port E)

Peripherals: UART (with RS232 port), SCI, SPI, 8-channel 8-bit ADC, 5 timers (16-bit, varying functions), WDT

Other: Ports B and C are used for memory bus expansion, board supports up to 32k chips (any mix of EEPROM and SRAM), on-board bootloader and debugger (Bufalo 3.4, occupies 8k of EEPROM), LCD connector, SS keypad connector, expandable memory-mapped peripherals.

Power draw: 0.5W (9V input)

### Build
Requires an m68hc11 toolchain:

```
cd hc11
mkdir -p build/bin
export PATH=$PATH:`pwd`/build/bin

# Binutils
# wget https://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.gz
tar zxvf binutils-2.39.tar.gz
mv binutils-2.39 binutils-m68hc11-elf-2.39
cd binutils-m68hc11-elf-2.39
./configure --prefix=`pwd`/../build --target=m68hc11-elf
make
make install

cd ..

# GCC
# wget https://ftp.gnu.org/gnu/gcc/gcc-3.4.6/gcc-core-3.4.6.tar.gz
tar zxvf gcc-core-3.4.6.tar.gz

# Patch the sources (see below)!
patch -s -p0 < gcc.patch

mv gcc-3.4.6 gcc-m68hc11-elf-3.4.6
cd gcc-m68hc11-elf-3.4.6
./configure --prefix=`pwd`/../build --target=m68hc11-elf
make
make install

cd ..

# GEL LibC
# https://sourceforge.net/projects/gel/files/gel-hc1x/1.6.1/gel-hc1x-1.6.1.tar.gz/download
tar zxvf gel-hc1x-1.6.1.tar.gz

# Patch the sources (see below)!
patch -s -p0 < gel.patch

cd gel-hc1x-1.6.1

make TARGET_BOARD=m68hc11-cme11

cd ..
mkdir -p build/lib
mkdir -p build/include
cp -r gel-hc1x-1.6.1/lib/* build/lib
cp -r gel-hc1x-1.6.1/include/* build/include

```

The GCC sources need a patch:

- Patch `config.guess` by copying the one from binutils package.
- Patch `intl/configure` at line 1151 to change `main(){return(0);}` to `int main(){return(0);}`
- Patch `libiberty/regex.c` replacing the bogus declarations of `char *malloc();` and `char *realloc();` at lines 132 and 133 with `#include <stdlib.h>`
- Patch `libiberty/md5.c` adding `#include <string.h>`.
- Patch `libiberty/getcwd.c` replacing `extern char *getwd();` on line 33 with `extern char *getwd(char*);`
- Patch `libiberty/getpwd.c` replacing `extern char *getwd();` on line 41 with `extern char *getwd(char*);`
- Patch `libiberty/pex-unix.c` replacing `extern int execv();` on line 47 with `extern int execv(const char*, char * const*);`
- Patch `libiberty/pex-unix.c` replacing `extern int execvp();` on line 48 with `extern int execvp(const char*, char * const*);`
- Patch `libiberty/pex-unix.c` replacing `int (*func)();` on line 59 with `int (*func)(const char*, char * const*);`

- Patch `gcc/collect2.c` at line 1537 to add `(S_IRUSR|S_IWUSR)` as third parameter to open (required with the `O_CREAT` flag, see here for the error you get without patching)
- Patch `gcc/config/m68hc11/larith.asm` at line 97 to read `.space 2` instead of `.dc.w 1` (to reserve uninitialized space for the virtual regs)
- Patch `gcc/cpplib.c` replacing `U"#";` on line 194 with `(const uchar*)U"#"`
- Patch `gcc/cpplib.c` replacing `U"pragma dependency";` on line 659 with `(const uchar*)U"pragma dependency"`
- Patch `gcc/cpplex.c` adding `(const uchar*)` before every `U`-prefixed string on line 42
- Patch `gcc/cpplex.c` adding `(const uchar*)` before every `U`-prefixed string on line 42
- Patch `gcc/cppmacro.c` adding `(const uchar*)` before `U`-prefixed strings on lines 223 and 224
- Patch `gcc/insn-output.c` adding  `extern int m68hc11_is_far_symbol (rtx);` at the top.
- Patch `gcc/insn-output.c` adding  `extern int m68hc11_is_trap_symbol (rtx);` at the top.

As a convenience, there is a patch file in the repo: `hc11/gcc.patch`

The GEL sources also need patching:
- Changing the `m6811-elf-` binutils prefix to `m68hc11-elf-` where appropriate.
- Removing any `.cc` files from the makefiles as C++ is not supported.

As a convenience, there is a patch file in the repo: `hc11/gel.patch`

Building the project is then a matter of:

```
make cme11a-clean && make cme11a
truncate --size=8k firmware/cme11a.bin
```

The project can be uploaded to a CME11-type board via serial and `hc11/upload.py` script. You may need to upgrade the U5 RAM chip to 32k.

Alternatively, you can update the memory map to point the `text` section to U6 EPROM space and burn it into a ROM chip. You will need to update the reset vector in Buffalo EPROM U7 to point to `0x8000` for the program to start automatically.

If you want to retain the interactive debugging bootloader, a modified Buffalo monitor is provided in `hc11/buf341.asm`. In can be compiled with `asm11` and burned to the U7 ROM chip. It's not exctly the same as the version on the board, but the functionality is complete. The modification causes Buffalo to jump to `0x8000` when pin 0 of `PORTE` is high at boot.

A modified version of the original Axiom Buffalo 3.4AX ROM is available in `hc11/buf34ax.bin` and a modified version that allows running ROM code in `hc11/buf341ax.bin`. This was modified by hand using the `hc11/bufpatch.asm`

The Buffalo Monitor sources come from this repository: https://github.com/tonypdmtr/buffalo
The code seems to be licensed under the MIT license terms (included in `hc11/LICENSE.buffalo`).

## Arduino Uno
![](./images/uno.jpg)

Core: ATmega328p, AVR, 8bit

Clock: 16 MHz

SRAM: 2k

EEPROM: 1k

FLASH: 32k

GPIO: 23

Peripherals: 2 * SPI, I2C, UART, 8-channel 10-bit ADC, 3 timers (2 8-bit and one 16-bit), WDT

Other: On-board serial programmer/debugger based around another ATmega chip.

Power draw: 0.15W

### Build
```
make uno-clean && make uno-upload
```

## STM32 Bluepill
![](./images/bluepill.jpg)

Core: STM32F103C8T6, ARM Cortex-M3, 32 bit

Clock: 72 MHz

SRAM: 20K

FLASH: 64K

GPIO: 32 I/O (some only 3V3-capable, PA13, PA14 and PA15 with reduced electrical capability)

Peripherals: 2 * SPI, 2 * I2C, 2 * 10-channel 12-bit ADC, 3 * UART, CAN, USB 2.0, 4 timers, RTC

Other: External batery connection for RTC backup, JTAG debugger support

Power draw: 0.15W

### Build
```
make bluepill-clean && make bluepill-upload
```

## ESP32 C6 super mini
![](./images/supermini.jpg)

Cores: ESP32-C6, High Performance Risc-V and Low Power Risc-V, 32 bit

Clock: 160 MHz for HP, 20 MHz for LP

SRAM: 512K for HP, 16k for LP

FLASH: 320K (internal, expandable externally)

GPIO: 22 (some not available on a convenient header)

Peripherals: 2.4 GHz Wi-Fi 6 (802.11ax), Bluetooth 5 (LE), IEEE 802.15.4 (Zigbee), 3 * SPI (two for external flash), 3 * UART (one low-power), 2 * I2C (one low-power), I2S, RMT, 2 * TWAI, SDIO, Motor Control PWM, 7-channel 12-bit ADC, USB 2.0, WDT, 7 timers (various functions)

Other: Built-in core temperature sensor, built-in AES, RSA and HMAC. JTAG debugger support, LiPo battery charger

Power draw: 0.1W (not changing the LiPo battery, WiFi & BT powered down)

The caveat for this board is the fact that all the GPIOs used for USB communication are also used for the sketch making flashing harder (requires putting the board into the bootloader mode by resetting with the BOOT button depressed at just the right moment for the flash utility to find it).

### Build
```
export WIFI_SSID='\"...\"'
export WIFI_PASS='\"...\"'
make supermini-clean && make supermini-upload
```
