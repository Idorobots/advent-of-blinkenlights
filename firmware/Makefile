AS=sdasz80 
ASFLAGS=-xlos
CC=sdcc
CFLAGS=--std c23 -mz80 --no-std-crt0 -Isrc --opt-code-size --Werror -DZ80_ARCH_TEMEX
LD=sdcc
LDFLAGS=-mz80 --no-std-crt0 -Wl -y --code-size 8192 --code-loc 0x0090 --data-loc 0xb000 --stack-loc 0x8200
OBJCOPY=sdobjcopy

all: rom.bin

rom.bin: rom.ihx
	$(OBJCOPY) -I ihex -O binary $^ $@

rom.ihx: crt0.rel main.rel platform.rel interrupts.rel sio.rel pio.rel ctc.rel rtc.rel ports.rel
	$(LD) $(LDFLAGS) -o $@ $^

main.rel: src/main.cpp
	@cp $^ /tmp/main.c
	$(CC) $(CFLAGS) -o $@ -c /tmp/main.c

crt0.rel: src/hal/temex/crt0.s
	$(AS) $(ASFLAGS) -o $@ $^

%.rel: src/hal/temex/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	rm -f *.lst
	rm -f *.sym
	rm -f *.asm
	rm -f *.ihx
	rm -f *.lk
	rm -f *.noi	
	rm -f *.map
	rm -f *.rel
	rm -f *.?db	
	rm -f *.bin
	rm -f *.mem
	rm -f *.rst
	rm -f *.omf
