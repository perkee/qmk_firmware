#include "knob.h"

bool knob_prev_a = false;
int8_t knob_score = 0;
int8_t last_dir = 0;

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
    /*EICRA |= (1 << ISC01);    // set INT0 to trigger on ANY logic change*/
    /*EIMSK |= (1 << INT1);     // Turns on INT1*/

    // INT:    33221100
    EICRA |= 0b00000100;
    // INT:     6  3210
    EIMSK |= 0b00000010;
}

ISR(INT1_vect) {
    // Pin D2 (PD1)
    bool a = PIND & (1 << PD1);
    int8_t dir = 0;

    if (knob_prev_a != a) {
        // 'a' has changed
        knob_prev_a = a;
        bool b = PINC & (1 << PC6);
        if (a == b) {
            // Halfway through CCW rotation
            //
            //         +---YOU ARE HERE
            //         |
            //         v
            // A: _____/^^^^^\__
            // B: __/^^^^^\_____
            knob_score++;
        } else {
            // Halfway through CW rotation
            //
            //         +---YOU ARE HERE
            //         |
            //         v
            // A: _____/^^^^^\_____
            // B: ________/^^^^^\__
            knob_score--;
        }
        if (abs(knob_score) > 1) {
            // A changes twice and both times the direction matched
            dir += (knob_score > 0) ? 1 : -1;
            knob_score = 0;
        }
        if (a && b) {
            // Keep score & knob pins in sync.
            knob_score = 0;
        }
    }

    last_dir = dir;
}

int8_t knob_read(void) {
    // Determine if the knob was rotated since last call.
    // Return 0 if not, non-zero if N rotations occured.
    // Call this as often as possible (likely from within matrix_scan_*)
    int dir = last_dir;
    last_dir = 0;
    return dir;
}

