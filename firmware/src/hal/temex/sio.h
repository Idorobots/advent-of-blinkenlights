#pragma once

#if defined(Z80_ARCH_TEMEX)

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal.h"

__sfr __at(0x40) SIO_DATA_A;
__sfr __at(0x41) SIO_DATA_B;
__sfr __at(0x42) SIO_CTRL_A;
__sfr __at(0x43) SIO_CTRL_B;

void initSIO(int8_t interruptVector);
void disableARTS(void);
void enableARTS(void);
void flushARX(void);
void flushATX(void);

int getchar(void);
int putchar(int c);

#endif
