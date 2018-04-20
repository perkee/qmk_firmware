# Stuff for my "Let's Split"

Featuring:

- Custom highlights (including custom knight-rider-like one which I've named DMC-12 by mistake.)
- LOW_ESC key (LOWER on hold, ESC on tap.)
- Rotary encoder knob for horizontal/vertical scrolling & volume control.

# Knob installation

I've used pins 4 & 5 (PD4 & PC6) to read digital value from the rotary encoder because Let's Split doesn't use them (Split keyboards FTW! Moar precious free pins!)

**Note:** If you scroll too fast, the scroll becomes laggy. Unfortunately there's no way to deal with this as matrix_scan_user isn't called frequently enough and some signals from encoder are simply lost.

**Note:** Seems like pin PD4 has an ADC (ADC8) attached. Might be better to use this one pin and attach both encoder legs to it via 4.7 kOhm & 10 kOhm resistors, however ADC is much slower than reading digital values which will result in even more lag.

See `knob.c` for implementation && usage info.

- Left image: without pull-up (this is the variant that I implemented - most AVRs have internal pull-up resistors.)
- Right image: with pull-up via 2 x 10 kOhm resistors (you can use this if your board/pin doesn't have internal pull-up.)

![Knob layout](./encoder.png)
![Knob schematic](./encoder2.png)

