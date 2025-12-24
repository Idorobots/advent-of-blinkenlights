#pragma once

#if defined(__AVR__)
  #include <Arduino.h>

#else
  #include <stdint.h>
  #include <stdbool.h>
#endif

uint64_t currMillis(void);
void display(char *str);
void displayUInt(uint32_t value);
