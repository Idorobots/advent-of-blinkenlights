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

#define PE0 0x10
#define PE1 0x11
#define PE2 0x12
#define PE3 0x13
#define PE4 0x14
#define PE5 0x15
#define PE6 0x16
#define PE7 0x17

void setup(void);
void loop(void);
