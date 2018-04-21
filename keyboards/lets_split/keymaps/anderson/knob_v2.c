#include "knob_v2.h"

bool knob_prev_a = false;
int8_t knob_dir = 0;

void knob_init(void) {
    // I use pins D1 (ISR1) & C6 for a knob.

    // Set pin mode for C6 as input.
    DDRC &= ~(0UL << PC6);

    // Enable internal pull-up for C6.
    // This is done by "writing" 1 to a pin that has its mode set to input.
    PORTC |= (1 << PC6);

    // Enable interrupt for D1
    // For more info on the below flags see this awesome section 11.1 (pages 89-90) here:
    // https://cdn-shop.adafruit.com/datasheets/atmel-7766-8-bit-avr-atmega16u4-32u4_datasheet.pdf
    // Set pin mode & pull-up.
    DDRD &= ~(0UL << PD1);
    PORTD |= (1UL << PD1);

    // INT:    33221100
    EICRA |= 0b00000100;  // 0b01 - any edge
    // INT:     6  3210
    EIMSK |= 0b00000010;
}

ISR(INT1_vect) {
    // Pin D2 (PD1)
    bool a = PIND & (1 << PD1);

    if (knob_prev_a != a) {
        // "A" channel has REALLY changed.
        knob_prev_a = a;
        bool b = PINC & (1 << PC6);
        if (a == b) {
            // Halfway through CCW rotation (A == B)
            //
            //         +---YOU ARE HERE (A=1, B=1)
            //         |     +---OR HERE (A=0, B=0)
            //         |     |
            //         v     v
            // A: _____/^^^^^\__
            // B: __/^^^^^\_____
            knob_dir++;
        } else {
            // Halfway through CW rotation (A != B)
            //
            //         +---YOU ARE HERE (A=1, B=0)
            //         |     +---OR HERE (A=0, B=1)
            //         |     |
            //         v     v
            // A: _____/^^^^^\_____
            // B: ________/^^^^^\__
            knob_dir--;
        }
    }
}

int8_t knob_read(void) {
    // Determine if the knob was rotated since last call.
    // Return 0 if not, non-zero if N rotations occured.
    // Call this as often as possible (likely from within matrix_scan_*)
    int dir = knob_dir;
    knob_dir = 0;
    return dir;
}

