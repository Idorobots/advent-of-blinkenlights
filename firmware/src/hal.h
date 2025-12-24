#pragma once

#if defined(ARDUINO)
  #include <Arduino.h>

#else
  #include <stdint.h>
  #include <stdbool.h>
#endif

uint64_t currMillis(void);

void initSerial(void);
void display(const char *str);
void displayUInt(uint32_t value);
