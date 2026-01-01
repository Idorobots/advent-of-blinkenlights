#pragma once

#if defined(Z80_ARCH_TEMEX)

#include "hal.h"
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

__sfr __at(0xc0) RTC_C_HSEC;
__sfr __at(0xc1) RTC_C_H;
__sfr __at(0xc2) RTC_C_M;
__sfr __at(0xc3) RTC_C_S;
__sfr __at(0xc4) RTC_C_MO;
__sfr __at(0xc5) RTC_C_D;
__sfr __at(0xc6) RTC_C_Y;
__sfr __at(0xc7) RTC_C_DOW;
__sfr __at(0xc8) RTC_R_HSEC;
__sfr __at(0xc9) RTC_R_H;
__sfr __at(0xca) RTC_R_M;
__sfr __at(0xcb) RTC_R_S;
__sfr __at(0xcc) RTC_R_MO;
__sfr __at(0xcd) RTC_R_D;
__sfr __at(0xce) RTC_R_Y;
__sfr __at(0xcf) RTC_R_DOW;
__sfr __at(0xd0) RTC_INT;
__sfr __at(0xd1) RTC_CTRL;

void toggleRTC(bool run);
void setRTCTime(struct tm *t);
void getRTCTime(struct tm *t);
void initRTCInt(bool interrupts, uint8_t interrupt);

// TODO For SDCC libc time() support.
#ifdef HAVE_RTC
unsigned char RtcRead(struct tm *t);
void RtcWrite(struct tm *t);
#endif

#endif
