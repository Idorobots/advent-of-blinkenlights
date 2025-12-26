#include "hal/temex/ctc.h"

#if defined(Z80_ARCH_TEMEX)

void delayUntil(uint64_t t) {
  while (currMillis() < t) {}
}

void delayMillis(uint64_t dt) {
  delayUntil(currMillis() + dt);
}

void initCTC0(uint8_t tc, int8_t interrupt) {
  if (interrupt != -1) {
    CTC_C0 = 0x87; // Reset & use time constant
    CTC_C0 = tc; // Set time constant to a value.
    CTC_C0 = (uint8_t) interrupt;
  } else {
    CTC_C0 = 0x07; // Reset & use time constant
    CTC_C0 = tc; // Set time constant to a value.
  }
}

void initCTC1(uint8_t tc, int8_t interrupt) {
  if (interrupt != -1) {
    CTC_C1 = 0x87; // Reset & use time constant
    CTC_C1 = tc; // Set time constant to a value.
    CTC_C0 = (uint8_t) interrupt;
  } else {
    CTC_C1 = 0x07; // Reset & use time constant
    CTC_C1 = tc; // Set time constant to a value.
  }
}

void initCTC2(uint8_t tc, int8_t interrupt) {
  if (interrupt != -1) {
    CTC_C2 = 0x87; // Reset & use time constant
    CTC_C2 = tc; // Set time constant to a value.
    CTC_C0 = (uint8_t) interrupt;
  } else {
    CTC_C2 = 0x07; // Reset & use time constant
    CTC_C2 = tc; // Set time constant to a value.
  }
}

void initCTC3(uint8_t tc, int8_t interrupt) {
  if (interrupt != -1) {
    CTC_C3 = 0x87; // Reset & use time constant
    CTC_C3 = tc; // Set time constant to a value.
    CTC_C0 = (uint8_t) interrupt;
  } else {
    CTC_C3 = 0x07; // Reset & use time constant
    CTC_C3 = tc; // Set time constant to a value.
  }
}
#endif
