#include "hal/temex/rtc.h"

#if defined(Z80_ARCH_TEMEX)

void toggleRTC(bool run) {
  if (run) {
    RTC_CTRL = 0x0c; // Run on, 24h, 32k crystal.
  } else {
    RTC_CTRL = 0x04; // Run off, 24h, 32k crystal.
  }
}

void initRTCInt(bool interrupts, uint8_t interrupt) {
  RTC_INT = interrupt; // Reset interrupts.

  uint8_t curr = RTC_CTRL;
  if (interrupts) {
    RTC_CTRL = curr | 0x10; // Enable interrupts.
  } else {
    RTC_CTRL = curr & ~0x10; // Disable interrupts.
  }
}

void setRTCTime(struct tm *t) {
  RTC_C_HSEC = t->tm_hundredth;
  RTC_C_S = t->tm_sec;
  RTC_C_M = t->tm_min;
  RTC_C_H = t->tm_hour;
  RTC_C_D = t->tm_mday;
  RTC_C_MO = t->tm_mon;
  RTC_C_Y = t->tm_year;
  RTC_C_DOW = t->tm_wday;
}

void getRTCTime(struct tm *t) {
  t->tm_hundredth = RTC_C_HSEC;
  t->tm_sec = RTC_C_S;
  t->tm_min = RTC_C_M;
  t->tm_hour = RTC_C_H;
  t->tm_mday = RTC_C_D;
  t->tm_mon = RTC_C_MO;
  t->tm_year = RTC_C_Y + 100; // NOTE That's how Y2K was handled.
  t->tm_wday = RTC_C_DOW;
  t->tm_yday = 365; // TODO
  t->tm_isdst = 0;
}

#ifdef HAVE_RTC
unsigned char RtcRead(struct tm *t) {
  getRTCTime(t);
  return 0;
}

void RtcWrite(struct tm *t) {
  setRTCTime(t);
}
#endif

#endif
