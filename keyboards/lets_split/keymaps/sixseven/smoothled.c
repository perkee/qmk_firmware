#include <smoothled.h>

static uint8_t sourceColor[3] = {0, 0, 0};
static uint8_t currentColor[3] = {0, 0, 0};
static uint8_t targetColor[3] = {0, 0, 0};
static int32_t smoothledTimer = -1;

void smoothled_set(uint8_t r, uint8_t g, uint8_t b) {
    smoothledTimer = timer_read32();
    sourceColor[0] = currentColor[0];
    sourceColor[1] = currentColor[1];
    sourceColor[2] = currentColor[2];
    targetColor[0] = r;
    targetColor[1] = g;
    targetColor[2] = b;
}

void smoothled_process(void) {
    if (smoothledTimer < 0) {
        return;
    }
    int32_t kb = timer_elapsed32(smoothledTimer);
    int32_t ka = SMOOTH_DURATION - kb;
    if (kb > SMOOTH_DURATION) {
        kb = SMOOTH_DURATION;
        ka = 0;
        smoothledTimer = -1;
    }
    for (int i = 0; i < 3; i++) {
        currentColor[i] = (ka * sourceColor[i] + kb * targetColor[i]) / SMOOTH_DURATION;
    }
    rgblight_setrgb(currentColor[0], currentColor[1], currentColor[2]);
}
