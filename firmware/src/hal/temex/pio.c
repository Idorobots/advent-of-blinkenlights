#include "hal/temex/pio.h"

#if defined(Z80_ARCH_TEMEX)

void initPIOA(uint8_t conf) {
  PIO_CTRL_A = conf;
}

void initPIOB(uint8_t conf) {
  PIO_CTRL_B = conf;
}

// NOTE Curr value stored in the port, for digitalWrite
volatile uint8_t PIO_VALUE_A = 0x00;
volatile uint8_t PIO_VALUE_B = 0x00;

void writePort(void* port, uint8_t value) {
  if (port == PORTA) {
    PIO_VALUE_A = value;
    PIO_DATA_A = value;
  } else if (port == PORTB) {
    PIO_VALUE_B = value;
    PIO_DATA_B = value;
  } else {
    // Not supported.
    __asm__("rst 0x8");
  }
}

uint8_t readPort(void* port) {
  if (port == PORTA) {
    return PIO_DATA_A;
  } else if (port == PORTB) {
    return PIO_DATA_B;
  } else {
    return *((uint8_t*)port);
  }
}

// FIXME Only allows setting the whole port as input or output.
void pinMode(uint8_t pin, uint8_t mode) {
  uint8_t value = (mode == OUTPUT) ? PIO_OUTPUT : PIO_INPUT;

  if (pin < PB0) {
    initPIOA(value);
  } else {
    initPIOB(value);
  }
}

void digitalWrite(uint8_t pin, uint8_t value) {
  uint8_t off = pin & 0xf;
  uint8_t mask = 1 << off;

  if (pin < PB0) {
    if (value == LOW) {
      writePort(PORTA, PIO_VALUE_A & ~mask);
    } else {
      writePort(PORTA, PIO_VALUE_A | mask);
    }
  } else {
    if (value == LOW) {
      writePort(PORTB, PIO_VALUE_B & ~mask);
    } else {
      writePort(PORTB, PIO_VALUE_B | mask);
    }
  }
}
#endif
