#!/bin/env python

FILENAME = "anim_{}.bin"
SIZE = 16384
ANIMS = 12
ANIM_TIME = 60000
FRAME_TIME = 50
FRAMES = 16;
SCAN = [0b1000000000000000,
        0b0100000000000000,
        0b0010000000000000,
        0b0001000000000000,
        0b0000100000000000,
        0b0000010000000000,
        0b0000001000000000,
        0b0000000100000000,
        0b0000000010000000,
        0b0000000001000000,
        0b0000000000100000,
        0b0000000000010000,
        0b0000000000001000,
        0b0000000000000100,
        0b0000000000000010,
        0b0000000000000001]

MIDDLE = [0b1000000000000001,
          0b0100000000000010,
          0b0010000000000100,
          0b0001000000001000,
          0b0000100000010000,
          0b0000010000100000,
          0b0000001001000000,
          0b0000000110000000]

def animate(frames, repeats = ANIM_TIME//FRAME_TIME//FRAMES):
    data = [x.to_bytes(2) for x in frames]
    return bytes([b for datum in data for b in datum]) * repeats

def count():
    return animate(range(0, ANIM_TIME//FRAME_TIME), 1)

def scan_left():
    return animate(SCAN)

def scan_right():
    return animate(SCAN[::-1])

def bounce():
    return animate(SCAN + SCAN[::-1], ANIM_TIME//FRAME_TIME//(FRAMES * 2))

def mid_in():
    return animate(MIDDLE + MIDDLE)

def mid_out():
    return animate(MIDDLE[::-1] + MIDDLE[::-1])

def mid_bounce():
    return animate(MIDDLE + MIDDLE[::-1])

def generate():
    a = bounce() + \
        scan_left() + \
        scan_right() + \
        mid_bounce() + \
        mid_in() + \
        mid_out() + \
        count()
    return a + b"\x00" * (2 * SIZE - len(a))

if __name__ == "__main__":
    anim = generate()

    with open(FILENAME.format("high"), "wb") as f:
         f.write(anim[0::2])

    with open(FILENAME.format("low"), "wb") as f:
        f.write(anim[1::2])
