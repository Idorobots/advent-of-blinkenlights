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
  // NOTE Effectively display(ultoa(value))

  char buf[20];
  char* p;

  p = &buf[20];
  *--p = 0;

  do {
    unsigned char c = value % 10;
    value = value / 10;
    *--p = c + '0';
  } while (value != 0);

  display(p);
}

volatile uint64_t __currTicks = 0;
volatile uint8_t __currTicksAdjust0 = 0;
volatile uint8_t __currTicksAdjust1 = 0;

void __attribute__((interrupt)) timer_interrupt (void) {
  __currTicks++;
  __currTicksAdjust0++;
  __currTicksAdjust1++;

  // NOTE Each ms is about 4.096 ticks, we treat that as 4 below,
  // NOTE so we add one extra tick whenever an extra ms would otherwise pass.
  // NOTE This still drifts, but to a much lowe degree.
  if (__currTicksAdjust0 == 40) {
    __currTicksAdjust0 = 0;
    __currTicks++;
  }
  // NOTE Now, to avoid the extra drift due to the overestimation 4.096 ~ 4.1, we drop one tick.
  if (__currTicksAdjust1 == 250) {
    __currTicksAdjust1 = 0;
    __currTicks--;
  }

  timer_acknowledge();
}

#define TIMER_DIV  (8192L)

uint64_t currMillis(void) {
  lock();
  uint64_t v = __currTicks;
  unlock();

  // NOTE This is basically 4 and this inflates the code size with unwanted divisions.
  // uint32_t msPerTick = 1000L / (M6811_CPU_E_CLOCK / TIMER_DIV);
  // return v * msPerTick;
  return v * 4;
}

void delayMillis(uint64_t ms) {
  uint64_t t = currMillis() + ms;
  while (currMillis() < t) {}
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

int main(void) {
  initSerial();

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

#endif
