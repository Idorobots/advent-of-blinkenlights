#include "hal.h"

// Arduino-based boards

#if defined(ARDUINO)

uint32_t highMillis = 0;
uint32_t prevMillis = 0;

uint64_t currMillis(void) {
  uint32_t now = millis();

  if (prevMillis > now) {
    highMillis++;
  }
  prevMillis = now;

  return ((uint64_t) highMillis << 32) | (uint64_t) now;
}

void delayMillis(uint64_t ms) {
  delay(ms);
}

void initSerial(void) {
  Serial.begin(115200);
}

void display(const char *str) {
  Serial.print(str);
}

void displayUInt(uint32_t value) {
  Serial.print(value);
}

#endif
