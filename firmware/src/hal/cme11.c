#include "hal.h"

// mc68hc11-based boards

#if defined(HC11_ARCH_CME11)

#include "hal/cme11.h"

void initSerial(void) {
  serial_init();
}

void display(const char *str) {
  serial_print(str);
}

void displayUInt(uint32_t value) {
  char buf[20];
  buf[19] = 0;
  char* s = ultoa(value, buf+19, 10, 0);
  display(s);
}

static uint64_t __currTicks = 0;

void __attribute__((interrupt)) timer_interrupt (void) {
  __currTicks++;
  timer_acknowledge();
}

#define TIMER_DIV  (8192L)

uint64_t currMillis(void) {
  lock();
  uint64_t v = __currTicks;
  unlock();

  uint64_t millis = 0;
  millis = v * (TIMER_DIV / 2);
  millis = millis + (v * (TIMER_DIV % 2)) / 2;
  millis = millis / 1000;

  return millis;
}

void delayMillis(uint64_t ms) {
  uint64_t t = currMillis() + ms;
  while (currMillis() < t) {}
}

int main(void) {
  serial_init ();

  lock();
  set_interrupt_handler(RTI_VECTOR, timer_interrupt);
  timer_initialize_rate(M6811_TPR_16);
  unlock();

  setup();

  for(;;) {
    loop();
  }

  return 0;
}

// NOTE Only supporting PA, PD & PX (custom) for output and PE for input.
volatile uint8_t PA_VALUE = 0x00;
volatile uint8_t PD_VALUE = 0x00;
volatile uint8_t PX_VALUE = 0x00;

void pinMode(uint8_t pin, uint8_t mode) {
  uint8_t off = pin & 0xf;
  uint8_t mask = 1 << off;

  if (pin == PA7) {
    uint8_t curr = _io_ports[M6811_PACTL];
    _io_ports[M6811_PACTL] = (mode == OUTPUT) ? (curr | mask) : (curr & ~mask);
  }  else if (pin >= PD2 && pin <= PD5) {
    uint8_t curr = _io_ports[M6811_DDRD];
    _io_ports[M6811_DDRD] = (mode == OUTPUT) ? (curr | mask) : (curr & ~mask);
  }
}

void digitalWrite(uint8_t pin, uint8_t value) {
  uint8_t off = pin & 0xf;
  uint8_t mask = 1 << off;

  if (/*pin >= PA0 &&*/ pin <= PA7) {
    PA_VALUE = (value == LOW) ? (PA_VALUE & ~mask) : (PA_VALUE | mask);
    _io_ports[M6811_PORTA] = PA_VALUE;
  } else if (pin >= PD2 && pin <= PD5) {
    PD_VALUE = (value == LOW) ? (PD_VALUE & ~mask) : (PD_VALUE | mask);
    _io_ports[M6811_PORTD] = PD_VALUE;
  } else if (pin >= PX0 && pin <= PX7) {
    PX_VALUE = (value == LOW) ? (PX_VALUE & ~mask) : (PX_VALUE | mask);

    // NOTE This one is handled differently.
    *PORTX = PX_VALUE;
  }
}

#endif
