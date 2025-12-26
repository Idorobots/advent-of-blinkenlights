#pragma once

#if defined(Z80_ARCH_TEMEX)

#include "hal.h"
#include <stdint.h>

#define PORTC ((void*) 0xc000)
#define PORTD ((void*) 0xc001)

void initPortsCD(void);

#endif
