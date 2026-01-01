#if defined(ARDUINO_ARCH_STM32)

#include "hal/stm32.h"

STM32RTC& rtc = STM32RTC::getInstance();

void initRTC(void) {
  rtc.setClockSource(STM32RTC::LSE_CLOCK);
  rtc.begin(STM32RTC::HOUR_24);
}

void toggleRTC(bool run) {
  // NOTE Nothing to do here.
}

void setRTCTime(struct tm *time) {
  rtc.setDate(time->tm_wday, time->tm_mday, time->tm_mon, time->tm_year);
  rtc.setTime(time->tm_hour, time->tm_min, time->tm_sec);
}

void getRTCTime(struct tm *time) {
  uint8_t wday, mday, mon, year, hour, min, sec;
  uint32_t subsec;

  rtc.getDate(&wday, &mday, &mon, &year);
  rtc.getTime(&hour, &min, &sec, &subsec);

  time->tm_wday = wday;
  time->tm_mday = mday;
  time->tm_mon = mon;
  time->tm_year = year;
  time->tm_hour = hour;
  time->tm_min = min;
  time->tm_sec = sec;
}

#endif
