#include "hal/temex/rtc.h"

#if defined(Z80_ARCH_TEMEX)

void initRTC(bool run, uint8_t interrupt) {
  RTC_INT = interrupt; // Reset interrupts.

  if (run) {
    RTC_CTRL = 0x0c; // Interrupt off, run on, 24h, 32k crystal.
  } else {
    RTC_CTRL = 0x04; // Interrupt off, run off, 24h, 32k crystal.
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
  t->tm_year = RTC_C_Y;
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
