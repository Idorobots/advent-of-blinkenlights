#include "hal/temex/platform.h"

#if defined(Z80_ARCH_TEMEX)

void sioARXByteAvailable(void) __critical __interrupt(1) {
  disableARTS();

  char c = getchar();
  putchar(c); // Echo the character.

  enableARTS();
}

void sioARXByteCondition(void) __critical __interrupt(2) {
  // TODO
  reset();
}

volatile uint64_t __currMillis = 0;

void ctcC3Timer(void) __critical __interrupt(3) {
  __currMillis++;
}

uint64_t currMillis(void) {
  // NOTE Needed for atomic access.
  __asm__("di");
  uint64_t value = __currMillis;
  __asm__("ei");
  return value;
}

void displayHexByte(uint8_t byte) {
  const char* chars = "0123456789abcdef";
  putchar(chars[byte >> 4]);
  putchar(chars[byte & 0x0f]);
}

#define DEF_ISR(n) \
  void isrTrap##n(void) __critical __interrupt(100 + (n)) { \
    display("Invalid interrupt: "); \
    displayHexByte((n)); \
    display("\r\n"); \
  }

DEF_ISR(0)
DEF_ISR(1)
DEF_ISR(2)
DEF_ISR(3)
DEF_ISR(4)
DEF_ISR(5)
DEF_ISR(6)
DEF_ISR(7)
DEF_ISR(8)
DEF_ISR(9)
DEF_ISR(10)
DEF_ISR(11)
DEF_ISR(12)
DEF_ISR(13)
DEF_ISR(14)
DEF_ISR(15)
DEF_ISR(16)
DEF_ISR(17)
DEF_ISR(18)
DEF_ISR(19)
DEF_ISR(20)
DEF_ISR(21)
DEF_ISR(22)
DEF_ISR(23)
DEF_ISR(24)
DEF_ISR(25)
DEF_ISR(26)
DEF_ISR(27)
DEF_ISR(28)
DEF_ISR(29)
DEF_ISR(30)
DEF_ISR(31)
DEF_ISR(32)

#define ISR(n) isrTrap##n

void initSerial(void) {
  initCTC0(1, -1); // EFfectively 9600 baud rate.
  initCTC1(1, -1); // EFfectively 9600 baud rate.
  initSIO(0x00);
}

void initRTC(void) {
  initRTCInt(false, 0x00); // No interrupts, keep running.
  toggleRTC(true);
}

unsigned char __sdcc_external_startup(void) {
  if (getInterruptVector(0x16) == ctcC3Timer
      && getInterruptVector(0x0c) == sioARXByteAvailable
      && getInterruptVector(0x0e) == sioARXByteCondition) {
    // Setup is correct, skip initializing RAM.
    return 1;
  } else {
    // Setup stuff that needs to be set up just once.
    return 0;
  }
}

int main(void) {
  setInterruptVector(0, ISR(0));
  setInterruptVector(1, ISR(1));
  setInterruptVector(2, ISR(2));
  setInterruptVector(3, ISR(3));
  setInterruptVector(4, ISR(4));
  setInterruptVector(5, ISR(5));
  setInterruptVector(6, ISR(6));
  setInterruptVector(7, ISR(7));
  setInterruptVector(8, ISR(8));
  setInterruptVector(9, ISR(9));
  setInterruptVector(10, ISR(10));
  setInterruptVector(11, ISR(11));
  setInterruptVector(12, ISR(12));
  setInterruptVector(13, ISR(13));
  setInterruptVector(14, ISR(14));
  setInterruptVector(15, ISR(15));
  setInterruptVector(16, ISR(16));
  setInterruptVector(17, ISR(17));
  setInterruptVector(18, ISR(18));
  setInterruptVector(19, ISR(19));
  setInterruptVector(20, ISR(20));
  setInterruptVector(21, ISR(21));
  setInterruptVector(22, ISR(22));
  setInterruptVector(23, ISR(23));
  setInterruptVector(24, ISR(24));
  setInterruptVector(25, ISR(25));
  setInterruptVector(26, ISR(26));
  setInterruptVector(27, ISR(27));
  setInterruptVector(28, ISR(28));
  setInterruptVector(29, ISR(29));
  setInterruptVector(30, ISR(30));
  setInterruptVector(31, ISR(31));
  setInterruptVector(32, ISR(32));

  setInterruptVector(0x16, ctcC3Timer);
  setInterruptVector(0x0c, sioARXByteAvailable);
  setInterruptVector(0x0e, sioARXByteCondition);

  // No harm in reinitializing these.
  initCTC3(154, 0x10); // Interrupt every 1 ms.
  initSerial();
  initPIOA(PIO_INPUT); // Input
  initPIOB(PIO_INPUT); // Input
  initPortsCD();
  initRTCInt(false, 0x00);

  // Re-enable interrupts just in case.
  initInterrupts();

  setup();

  for(;;) {
    loop();
  }
}

#endif
