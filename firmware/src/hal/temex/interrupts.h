#pragma once

#if defined(Z80_ARCH_TEMEX)

#include "hal.h"
#include <stdint.h>

void initInterrupts(void);

void setInterruptVector(uint8_t offset, void* vector);
void* getInterruptVector(uint8_t offset);

void setResetVector(uint8_t offset, void* vector);
void* getResetVector(uint8_t offset);

void isrTrap(void) __critical __interrupt(0);
void nmiTrap(void) __critical __interrupt;
void reset(void);

#endif
