#pragma once

#if defined(ARDUINO_ARCH_ESP32)
  #ifndef ESP32_SSID
    #error "ESP32_SSID not defined"
  #endif

  #ifndef ESP32_PASS
    #error "ESP32_PASS not defined"
  #endif
#endif

#include "hal.h"
#include <time.h>
#include <WiFi.h>
