#include "hal/temex/sio.h"

#if defined(Z80_ARCH_TEMEX)

void enableARX(void) {
  SIO_CTRL_A = 0x03; // Select WR3
  SIO_CTRL_A = 0xc1; // RX 8 bit, auto enable off, RX on
}

void disableARX(void) {
  SIO_CTRL_A = 0x03; // Select WR3
  SIO_CTRL_A = 0xc0; // RX 8 bit, auto enable off, RX on
}

void enableARTS(void) {
  SIO_CTRL_A = 0x05; // Select WR5
  SIO_CTRL_A = 0xea; // DTR on, break off, TX 8 bit, TX on, RTS on
}

void disableARTS(void) {
  SIO_CTRL_A = 0x05; // Select WR5
  SIO_CTRL_A = 0xe8; // DTR on, break off, TX 8 bit, TX on, RTS off
}

void initSIO(int8_t interrupts) {
  SIO_CTRL_A = 0x30; // Error reset.
  SIO_CTRL_A = 0x18; // Reset & abort

  SIO_CTRL_A = 0x04; // Select WR4
  SIO_CTRL_A = 0x44; // CLK x16, 1 stop bit, no parity

  enableARTS();

  if(interrupts == -1) {
    SIO_CTRL_A = 0x01; // Select WR1
    SIO_CTRL_A = 0x00; // Disable interrupts
  } else {
    SIO_CTRL_B = 0x01; // Select WR1
    SIO_CTRL_B = 0x04; // No interrupt in channel B, special RX condition affects vector

    SIO_CTRL_B = 0x02; // Select WR2
    SIO_CTRL_B = 0x00; // Set up the interrupt vector

    SIO_CTRL_A = 0x01; // Select WR1
    SIO_CTRL_A = 0x18; // Interrupt on all RX characters, buffer overrun is a special RX condition, parity is not
  }

  enableARX();
}

void flushARX(void) {
  bool flushed = false;

  do {
    SIO_CTRL_A = 0x00; // Select RR0;
    uint8_t b = SIO_CTRL_A;

    if (b & 0x01) {
      char c = SIO_DATA_A; // Read data.
    } else {
      flushed = true;
    }

  } while(!flushed);
}

void flushATX(void) {
  bool flushed = false;

  do {
      SIO_CTRL_A = 0x01; // SELECT RR1
      uint8_t b = SIO_CTRL_A;

      if (b & 0x01) {
        flushed = true;
      }
  } while (!flushed);
}

int putchar(int c) {
  SIO_DATA_A = c & 0xff;
  flushATX();
  return 0;
}

int getchar(void) {
  for (;;) {
    SIO_CTRL_A = 0x00; // Select RR0
    uint8_t b = SIO_CTRL_A;

    if (b & 0x01) {
      char c = SIO_DATA_A;
      flushARX();
      return c;
    }
  }
}

void display(const char* s) {
  const char *i = s;
  while (*i != '\0') {
    putchar(*i);
    i++;
  }
}

void displayUInt(uint32_t value) {
  char buf[20];
  __ultoa(value, buf, 10);
  display(buf);
}

#endif
