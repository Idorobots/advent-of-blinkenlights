#pragma once

#if defined(Z80_ARCH_TEMEX)

#include "hal.h"
#include "hal/temex/sio.h"
#include "hal/temex/pio.h"
#include "hal/temex/ctc.h"
#include "hal/temex/interrupts.h"
#include "hal/temex/rtc.h"
#include "hal/temex/ports.h"

#define PA0 0x00
#define PA1 0x01
#define PA2 0x02
#define PA3 0x03
#define PA4 0x04
#define PA5 0x05
#define PA6 0x06
#define PA7 0x07

#define PB0 0x10
#define PB1 0x11
#define PB2 0x12
#define PB3 0x13
#define PB4 0x14
#define PB5 0x15
#define PB6 0x16
#define PB7 0x17

void setup(void);
void loop(void);

#endif
