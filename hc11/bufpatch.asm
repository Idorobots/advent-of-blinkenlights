FLASH	equ	  $8000
ROM     equ       $E000
PORTE	equ	  $100A

	org       ROM+$1f50
START	proc
	ldx       #PORTE
        brclr     ,X,#1,SKIP       ; if bit 0 of port e is 1
        jmp       FLASH            ; then jump to the start of FLASH
SKIP
        jmp       ROM              ; then jump to the start of FLASH

	org       ROM+$1ffe
VRST	dw        START
