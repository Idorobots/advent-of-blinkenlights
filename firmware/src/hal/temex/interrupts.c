#include "hal/temex/interrupts.h"

#if defined(Z80_ARCH_TEMEX)

void isrTrap(void)  __critical __interrupt(0) {
  reset();
}

// NMI interrupt.
void nmiTrap(void) __critical __interrupt {
  __asm__("halt");
}

// Reset vectors
void* __at(0x8000) RESET_VECTORS[13] = {
  isrTrap, // RST 8
  isrTrap, // RST 10
  isrTrap, // RST 18
  isrTrap, // RST 20
  isrTrap, // RST 28
  isrTrap, // RST 30
  isrTrap, // RST 38
  (void*) 0x00,
  (void*) 0x00,
  (void*) 0x00,
  (void*) 0x00,
  (void*) 0x00,
  nmiTrap // RST 66
};

void setResetVector(uint8_t offset, void* vector) {
  RESET_VECTORS[offset>>3] = vector;
}

void *getResetVector(uint8_t offset) {
  return RESET_VECTORS[offset>>3];
}

// Interrupt vectors
void* __at(0x8100) INTERRUPT_VECTORS[32] = {
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap,
    isrTrap
};

void setInterruptVector(uint8_t offset, void* vector) {
  INTERRUPT_VECTORS[offset>>1] = vector;
}

void *getInterruptVector(uint8_t offset) {
  return INTERRUPT_VECTORS[offset>>1];
}

void initInterrupts(void) {
  __asm__(
    "ld  a, #0x81\n"
    "ld  i, a\n"
    "im  2\n"
    "ei\n"
  );
}

void reset(void) {
  __asm__("jp 0x0000");
}

#endif
