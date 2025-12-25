#include "hal.h"

#if defined(ARDUINO_ARCH_AVR)
  const uint8_t LED_PINS[] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 15, 16, 17, 18, 19 };
#elif defined(ARDUINO_ARCH_STM32)
  const uint8_t LED_PINS[] = { PB11, PB12, PB13, PB14, PB15, PA8, PA9, PA10, /*PA11, PA12,*/ PA15, PB3, PB4, PB5, PB6, PB7, PB8, PB9 };
#elif defined(ARDUINO_ARCH_ESP32)
const uint8_t LED_PINS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, /*15, */ 18, 19, 20, /*12, 13*/ 21, 23 };
#endif

void ledBar(uint16_t value) {
  for (uint8_t i = 0; i < 16; i++) {
    // NOTE Pins are supposed to sink the current.
    digitalWrite(LED_PINS[i], (value & (1 << i)) == 0);
  }
}

const uint64_t STEP_INTERVAL = 50; // 50 ms
const uint64_t ANIM_INTERVAL = 60000; // 1 minute

static uint8_t anim = 0;
static uint16_t value = 0;
static bool direction = false;

void nextAnim(void) {
  anim++;
  value = 0;
  direction = false;
}

const uint16_t animDouble[] = {
  0b0000000110000000,
  0b0000001001000000,
  0b0000010000100000,
  0b0000100000010000,
  0b0001000000001000,
  0b0010000000000100,
  0b0100000000000010,
  0b1000000000000001
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
}

static uint64_t nextAnimTs = 0;
static uint64_t nextStepTs = 0;

void loop(void) {
  uint64_t now = currMillis();

  if (now > nextStepTs) {
    step(now);
    nextStepTs = nextStepTs + STEP_INTERVAL;
  }

  if (now > nextAnimTs) {
    nextAnim();

    display("Current time: ");
    displayUInt(now);
    display("\r\n");
    display("Current animation: ");
    displayUInt(anim);
    display("\r\n");

    nextAnimTs = nextAnimTs + ANIM_INTERVAL;
  }

  delay(STEP_INTERVAL/5);
}
