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

void customIsrTrap(void) __critical __interrupt(99) {
  SIO_CTRL_B = 0x02; // Select RR2
  uint8_t isr = SIO_CTRL_B;

  display("Invalid interrupt: (SIO ");
  displayHexByte(isr);
  display(") (CTC ?)");
  display("\r\n");

  reset();
}

void initSerial(void) {
  initCTC0(1, -1); // EFfectively 9600 baud rate.
  initCTC1(1, -1); // EFfectively 9600 baud rate.
  initSIO(0x00);
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

  for(uint8_t i = 0; i < 32; i++) {
    setInterruptVector(i, customIsrTrap);
  }

  setInterruptVector(0x16, ctcC3Timer);
  setInterruptVector(0x0c, sioARXByteAvailable);
  setInterruptVector(0x0e, sioARXByteCondition);

  // No harm in reinitializing these.
  initCTC3(154, 0x10); // Interrupt every 1 ms.
  initSerial();
  initPIOA(PIO_INPUT); // Input
  initPIOB(PIO_INPUT); // Input
  initPortsCD();

  initRTC(false, 0x00);
  struct tm time = {
    .tm_hundredth = 0,
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 1,
    .tm_mon = 1,
    .tm_year = 25,
    .tm_wday = 0,
    .tm_isdst = 0
  };
  setRTCTime(&time);
  initRTC(true, 0x00);

  // Re-enable interrupts just in case.
  initInterrupts();

  setup();

  for(;;) {
    loop();
  }
}

#endif
