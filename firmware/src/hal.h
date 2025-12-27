#pragma once

#if defined(ARDUINO)
  #include <Arduino.h>

#else
  #include <stdint.h>
  #include <stdbool.h>
  #include <stdarg.h>

  #define INPUT 0
  #define OUTPUT 1

  void pinMode(uint8_t pin, uint8_t mode);

  #define HIGH 1
  #define LOW 0

  void digitalWrite(uint8_t pin, uint8_t value);
  uint8_t digitalRead(uint8_t pin);

  #if defined(Z80_ARCH_TEMEX)
    #include "hal/temex/platform.h"
  #elif defined(HC11_ARCH_CME11)
    #include "hal/cme11.h"
  #endif

#endif

uint64_t currMillis(void);
void delayMillis(uint64_t ms);

void initSerial(void);
void display(const char *str);
void displayUInt(uint32_t value);

