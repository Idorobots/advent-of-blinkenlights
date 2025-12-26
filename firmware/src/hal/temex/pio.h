#pragma once

#if defined(Z80_ARCH_TEMEX)

#include "hal.h"
#include <stdint.h>

#define PORTA ((void *) 0x80)
#define PORTB ((void *) 0x81)

#define PIO_INPUT 0x04
#define PIO_OUTPUT 0x0f

__sfr __at(0x80) PIO_DATA_A;
__sfr __at(0x81) PIO_DATA_B;
__sfr __at(0x82) PIO_CTRL_A;
__sfr __at(0x83) PIO_CTRL_B;

void initPIOA(uint8_t conf);
void initPIOB(uint8_t conf);

uint8_t readPort(void* port);
void writePort(void* port, uint8_t value);

#endif
