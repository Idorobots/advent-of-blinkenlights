#if defined(ARDUINO_ARCH_AVR)

#include "hal/atmega.h"

RTC_DS3231 rtc;

void initRTC(void) {
  rtc.begin();
}

void toggleRTC(bool run) {
  // NOTE Nothing to do.
}

void setRTCTime(struct tm *time) {
  rtc.adjust(DateTime(time->tm_year, time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec));
}

void getRTCTime(struct tm *time) {
  DateTime now = rtc.now();

  time->tm_wday = now.dayOfTheWeek();
  time->tm_mday = now.day();
  time->tm_mon = now.month();
  time->tm_year = now.year() - 2000;
  time->tm_hour = now.hour();
  time->tm_min = now.minute();
  time->tm_sec = now.second();
}

#endif
