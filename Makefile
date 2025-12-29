all: temex cme11a uno bluepill supermini

.PHONY: temex
temex:
	@ $(MAKE) -C firmware -f Makefile.z80

.PHONY: temex-clean
temex-clean:
	@ $(MAKE) -C firmware -f Makefile.z80 clean

.PHONY: cme11a
cme11a:
	@ $(MAKE) -C firmware -f Makefile.hc11 PATH="$(PATH):../hc11/build/bin"

.PHONY: cme11a-clean
cme11a-clean:
	@ $(MAKE) -C firmware -f Makefile.hc11 clean

.PHONY: uno
uno:
	@ cd firmware && pio run -e atmega

.PHONY: uno-clean
uno-clean:
	@ cd firmware && pio run -e atmega -t clean

.PHONY: uno-upload
uno-upload:
	@ cd firmware && pio run -e atmega -t upload

.PHONY: bluepill
bluepill:
	@ cd firmware && pio run -e stm32

.PHONY: bluepill-clean
bluepill-clean:
	@ cd firmware && pio run -e stm32 -t clean

.PHONY: bluepill-upload
bluepill-upload:
	@ cd firmware && pio run -e stm32 -t upload

.PHONY: supermini
supermini:
	@ cd firmware && pio run -e esp32c6

.PHONY: supermini-clean
supermini-clean:
	@ cd firmware && pio run -e esp32c6 -t clean

.PHONY: supermini-upload
supermini-upload:
	@ cd firmware && pio run -e esp32c6 -t upload

.PHONY: clean
clean: temex-clean cme11a-clean uno-clean bluepill-clean supermini-clean
