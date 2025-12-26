.module crt0

.globl _main
.globl ___sdcc_external_startup
.globl l__DATA
.globl s__DATA
.globl l__INITIALIZER
.globl s__INITIALIZED
.globl s__INITIALIZER

.area _HEADER (ABS)

;; Reset vector
.org 0
jp init

;; Other reset vectors (assumes a table at 0x8000)
.org 0x08
push hl
ld   hl, (#0x8000)
ex   (sp), hl
ret

.org 0x10
push hl
ld   hl, (#0x8002)
ex   (sp), hl
ret

.org 0x18
push hl
ld   hl, (#0x8004)
ex   (sp), hl
ret

.org 0x20
push hl
ld   hl, (#0x8006)
ex   (sp), hl
ret

.org 0x28
push hl
ld   hl, (#0x8008)
ex   (sp), hl
ret

.org 0x30
push hl
ld   hl, (#0x800a)
ex   (sp), hl
ret

.org 0x38
push hl
ld   hl, (#0x800c)
ex   (sp), hl
ret

;; NMI reset vector
.org 0x66
push hl
ld   hl, (#0x8018)
ex   (sp), hl
ret

.org 0x70
init:

;; Set up stack pointer 
ld      sp, #0x8200

;; Initialise global variables
call	___sdcc_external_startup
or	a, a
call	Z, gsinit

call 	_main
halt

;; Ordering of segments for the linker.
.area _HOME
.area _CODE
.area _INITIALIZER
.area _GSINIT
.area _GSFINAL
.area _DATA 
.area _INITIALIZED
.area _BSEG
.area _BSS
.area _HEAP

.area _GSINIT
gsinit::

; Default-initialized global variables.
ld      bc, #l__DATA
ld      a, b
or      a, c
jr      Z, zeroed_data
ld      hl, #s__DATA
ld      (hl), #0x00
dec     bc
ld      a, b
or      a, c
jr      Z, zeroed_data
ld      e, l
ld      d, h
inc     de
ldir

zeroed_data:
; Explicitly initialized global variables.
ld	bc, #l__INITIALIZER
ld	a, b
or	a, c
jr	Z, gsinit_next
ld	de, #s__INITIALIZED
ld	hl, #s__INITIALIZER
ldir

gsinit_next:

.area _GSFINAL
ret
