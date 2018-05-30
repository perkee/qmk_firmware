#include <smoothled.h>

uint8_t sourceColor[3] = {0, 0, 0};
uint8_t targetColor[3] = {0, 0, 0};
static uint16_t smoothledTimer = 0;

void smoothled_set(uint8_t r, uint8_t g, uint8_t b) {
    smoothledTimer = timer_read();
    sourceColor[0] = targetColor[0];
    sourceColor[1] = targetColor[1];
    sourceColor[2] = targetColor[2];
    targetColor[0] = r;
    targetColor[1] = g;
    targetColor[2] = b;
}

void smoothled_process(void) {
    uint16_t dt = timer_elapsed(smoothledTimer);
    if (dt > SMOOTH_DURATION) {
        rgblight_setrgb(targetColor[0], targetColor[1], targetColor[2]);
    } else {
        uint8_t currentColor[3];
        for (int i = 0; i < 3; i++) {
            currentColor[i] = (((uint16_t)sourceColor[i]) * (SMOOTH_DURATION - dt) + ((uint16_t)targetColor[i]) * dt) / SMOOTH_DURATION;
        }
        rgblight_setrgb(currentColor[0], currentColor[1], currentColor[2]);
    }
}
