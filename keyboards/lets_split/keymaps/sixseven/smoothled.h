#include "lets_split.h"
#include "config.h"

#define SMOOTH_DURATION 100

extern uint8_t currentColor[3];
extern uint8_t targetColor[3];

void smoothled_set(uint8_t r, uint8_t g, uint8_t b);
void smoothled_process(void);
