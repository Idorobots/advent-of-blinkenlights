#pragma once

#include <sys/sio.h>
#include <sys/ports.h>
#include <sys/time.h>
#include <sys/locks.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <util.h>

#define PA0 0x00
#define PA1 0x01
#define PA2 0x02
#define PA3 0x03
#define PA4 0x04
#define PA5 0x05
#define PA6 0x06
#define PA7 0x07

#define PD0 0x30
#define PD1 0x31
#define PD2 0x32
#define PD3 0x33
#define PD4 0x34
#define PD5 0x35
#define PD6 0x36
#define PD7 0x37

#define PE0 0x40
#define PE1 0x41
#define PE2 0x42
#define PE3 0x43
#define PE4 0x44
#define PE5 0x45
#define PE6 0x46
#define PE7 0x47

#define PX0 0x40
#define PX1 0x41
#define PX2 0x42
#define PX3 0x43
#define PX4 0x44
#define PX5 0x45
#define PX6 0x46
#define PX7 0x47

#define PORTX ((uint8_t*) 0xb580)

void setup(void);
void loop(void);
