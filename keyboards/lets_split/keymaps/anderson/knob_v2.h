// Rotary knob implementation - Version 2.
// Uses 2 digital pins - D1 (via interrupt) & C6.
#include "lets_split.h"

void knob_init(void);
int8_t knob_read(void);

bool knob_prev_a;
int8_t knob_score;
int8_t last_dir;

