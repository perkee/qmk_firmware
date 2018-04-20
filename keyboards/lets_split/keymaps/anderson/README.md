# Stuff for my "Let's Split"

Featuring:

- Custom highlights (including custom knight-rider-like one which I've named DMC-12 by mistake.)
- LOW_ESC key (LOWER on hold, ESC on tap.)
- Rotary encoder knob for horizontal/vertical scrolling & volume control.

# Knob installation

See `knob.c` for implementation && usage info.

- Left image: without pull-up (this is the variant that I implemented - most AVRs have internal pull-up resistors.)
- Right image: with pull-up via 2 x 10 kOhm resistors (you can use this if your board/pin doesn't have internal pull-up.)

![Knob layout](./encoder.png)
![Knob schematic](./encoder2.png)

