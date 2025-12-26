# uC timeline

## Z80 - Temex CPU
Requires SDCC & related binutils.

```
make clean && make
truncate --size=8k rom.bin
```

Flash the ROM chip in EEPROM1 position.

## MC68HC11 - Axiom CMEA11A
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
patch -s -p0 < fix.patch

mv gcc-3.4.6 gcc-m68hc11-elf-3.4.6
cd gcc-m68hc11-elf-3.4.6
./configure --prefix=`pwd`/../build --target=m68hc11-elf
make
make install
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

As a convenience, there is a patch file in the repo: `hc11/fix.patch`

## Arduino-based
These are built with PlatformIO.

### Atmego328p - Arduino Uno

```
pio init
pio run -e atmega -t upload
```

### STM32F103 - Bluepill
```
pio init
pio run -e stm32 -t upload
```

### ESP32-C6 - ESP32 C6 super mini
```
pio init
pio run -e esp32c6 -t upload
```
