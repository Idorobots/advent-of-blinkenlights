#include "hal.h"

// AVR-based boards
#if defined(__AVR__)

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

void display(char *str) {
  Serial.print(str);
}

void displayUInt(uint32_t value) {
  Serial.print(value);
}

// Default, do nothing.
#else
const uint8_t LED_PINS[] = {};

uint64_t currMillis(void) {
  return 0;
}

void display(char *str) {}
void displayUInt(uint32_t value) {}
#endif
