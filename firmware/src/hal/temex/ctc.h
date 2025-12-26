#pragma once

#if defined(Z80_ARCH_TEMEX)

#include <stdint.h>
#include "hal.h"

__sfr __at(0x00) CTC_C0;
__sfr __at(0x01) CTC_C1;
__sfr __at(0x02) CTC_C2;
__sfr __at(0x03) CTC_C3;

void initCTC0(uint8_t tc, int8_t interrupt);
void initCTC1(uint8_t tc, int8_t interrupt);
void initCTC2(uint8_t tc, int8_t interrupt);
void initCTC3(uint8_t tc, int8_t interrupt);

#endif
