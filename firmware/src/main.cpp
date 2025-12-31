#include "hal.h"

#if defined(ARDUINO_ARCH_AVR)
  const uint8_t LED_PINS[] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 15, 16, 17, 18, 19 };
#elif defined(ARDUINO_ARCH_STM32)
  const uint8_t LED_PINS[] = { PB11, PB12, PB13, PB14, PB15, PA8, PA9, PA10, /*PA11, PA12,*/ PA15, PB3, PB4, PB5, PB6, PB7, PB8, PB9 };
#elif defined(ARDUINO_ARCH_ESP32)
  const uint8_t LED_PINS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, /*15, */ 18, 19, 20, /*12, 13*/ 21, 23 };
#elif defined(Z80_ARCH_TEMEX)
  const uint8_t LED_PINS[] = {PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7 };
#elif defined(HC11_ARCH_CME11)
const uint8_t LED_PINS[] = {PX0, PX1, PX2, PX3, PX4, PX5, PX6, PX7, /*PA0, PA1, PA2,*/ PD2, PD3, PD4, /*PA3,*/ PD5, PA4, PA5, PA6, PA7 };
#endif

void ledBar(uint16_t value) {
  for (uint8_t i = 0; i < 16; i++) {
    // NOTE Pins are supposed to sink the current.
    digitalWrite(LED_PINS[i], (value & (1 << i)) == 0);
  }
}

const uint64_t STEP_INTERVAL = 50; // 50 ms
const uint64_t ANIM_INTERVAL = 60000; // 1 minute
const uint64_t LOG_INTERVAL = 10000; // 10 seconds

static uint8_t anim = 0;
static uint16_t value = 0;
static bool direction = false;

void nextAnim(void) {
  anim++;
  value = 0;
  direction = false;
}

const uint16_t animDouble[] = {
  0x0180, // 0b0000000110000000,
  0x0240, // 0b0000001001000000,
  0x0420, // 0b0000010000100000,
  0x0810, // 0b0000100000010000,
  0x1008, // 0b0001000000001000,
  0x2004, // 0b0010000000000100,
  0x4002, // 0b0100000000000010,
  0x8001  // 0b1000000000000001
};

void step(uint64_t now) {
    switch (anim) {
      case 0: {
        uint64_t v = (now >> 7);
        ledBar(v & 0xffff);
      }
      break;

      case 1: {
        if ((value == 0) || (value > (1 << 15))) value = 1;
        if (value == 1) direction = true;
        if (value == (1 << 15)) direction = false;

        ledBar(value);

        value = direction ? (value << 1) : (value >> 1);
      }
      break;

      case 2: {
        if ((value == 0) || (value > (1 << 15))) value = 1;

        ledBar(value);

        value = value << 1;
      }
      break;

      case 3: {
        if ((value == 0) || (value > (1 << 15))) value = (uint16_t)(1 << 15);

        ledBar(value);

        value = value >> 1;
      }
      break;

      case 4: {
        if (value > 7) value = 0;

        ledBar(animDouble[value]);

        value++;
      }
      break;

      case 5: {
        if (value > 7) value = 0;

        ledBar(animDouble[7 - value]);

        value++;
      }
      break;

      case 6: {
        if (value == 0) {
          direction = true;
        } else if (value > 7) {
          value = 7;
          direction = false;
        }

        ledBar(animDouble[7 - value]);

        value = direction ? (value + 1) : (value - 1);
      }
      break;

      default:
        anim = 0;
      break;
    }
}

void setup(void) {
  initSerial();

  for (uint8_t i = 0; i < 16; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  ledBar(0x0000);

  delayMillis(STEP_INTERVAL);

  ledBar(0xffff);
}

static uint64_t nextAnimTs = 0;
static uint64_t nextStepTs = 0;
static uint64_t nextLogTs = 0;

void loop(void) {
  uint64_t now = currMillis();

  if (now > nextStepTs) {
    step(now);
    nextStepTs = nextStepTs + STEP_INTERVAL;
  }

#if defined(HAS_RTC)
  struct tm time;
  getRTCTime(&time);

  // NOTE This condition is reversed wrt to the millis one.
  if (time.tm_min != nextAnimTs) {
    nextAnim();
    nextAnimTs = time.tm_min;
  }
#else
  if (now > nextAnimTs) {
    nextAnim();
    nextAnimTs = nextAnimTs + ANIM_INTERVAL;
  }
#endif

  if (now > nextLogTs) {
    display("Current millis: ");
    displayUInt(now);
    display("\r\n");

#if defined(HAS_RTC)
    getRTCTime(&time);
    display("Current time: 20");
    displayUInt(time.tm_year);
    display("-");
    if (time.tm_mon < 10) display("0");
    displayUInt(time.tm_mon);
    display("-");
    if (time.tm_mday < 10) display("0");
    displayUInt(time.tm_mday);
    display(" ");
    if (time.tm_hour < 10) display("0");
    displayUInt(time.tm_hour);
    display(":");
    if (time.tm_min < 10) display("0");
    displayUInt(time.tm_min);
    display(":");
    if (time.tm_sec < 10) display("0");
    displayUInt(time.tm_sec);
    display(".");
    if (time.tm_hundredth < 10) display("0");
    displayUInt(time.tm_hundredth);
    display("0\r\n");
#endif

    display("Current animation: ");
    displayUInt(anim);
    display("\r\n");

    nextLogTs = nextLogTs + LOG_INTERVAL;
  }

  delayMillis(STEP_INTERVAL/5);
}
