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

void pinMode(uint8_t pin, uint8_t mode) {
  // TOOD
}

void digitalWrite(uint8_t pin, uint8_t value) {
  // TOOD
}

#endif
