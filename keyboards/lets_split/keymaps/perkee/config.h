#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

/* Use I2C or Serial, not both */

#define USE_SERIAL
// #define USE_I2C

/* Select hand configuration */

#define MASTER_LEFT
// #define _MASTER_RIGHT
// #define EE_HANDS

#define TAPPING_TERM 250

#undef RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 12
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8

#endif

#ifndef RGB_KEYMAP
// create a layer with all RGB prefixed keys
#define RGB_KEYMAP( \
L00, L01, L02, L03, L04, L05, R00, R01, R02, R03, R04, R05, \
L10, L11, L12, L13, L14, L15, R10, R11, R12, R13, R14, R15, \
L20, L21, L22, L23, L24, L25, R20, R21, R22, R23, R24, R25, \
L30, L31, L32, L33, L34, L35, R30, R31, R32, R33, R34, R35 \
) \
KEYMAP( \
    RGB_##L00, RGB_##L01, RGB_##L02, RGB_##L03, RGB_##L04, RGB_##L05, RGB_##R00, RGB_##R01, RGB_##R02, RGB_##R03, RGB_##R04, RGB_##R05, \
    RGB_##L10, RGB_##L11, RGB_##L12, RGB_##L13, RGB_##L14, RGB_##L15, RGB_##R10, RGB_##R11, RGB_##R12, RGB_##R13, RGB_##R14, RGB_##R15, \
    RGB_##L20, RGB_##L21, RGB_##L22, RGB_##L23, RGB_##L24, RGB_##L25, RGB_##R20, RGB_##R21, RGB_##R22, RGB_##R23, RGB_##R24, RGB_##R25, \
    RGB_##L30, RGB_##L31, RGB_##L32, RGB_##L33, RGB_##L34, RGB_##L35, RGB_##R30, RGB_##R31, RGB_##R32, RGB_##R33, RGB_##R34, RGB_##R35 \
)
#endif

#ifndef NOMOD_KEYMAP
// create a layer with all NOMOD prefixed keys
#define NOMOD_KEYMAP( \
L00, L01, L02, L03, L04, L05, R00, R01, R02, R03, R04, R05, \
L10, L11, L12, L13, L14, L15, R10, R11, R12, R13, R14, R15, \
L20, L21, L22, L23, L24, L25, R20, R21, R22, R23, R24, R25, \
L30, L31, L32, L33, L34, L35, R30, R31, R32, R33, R34, R35 \
) \
KEYMAP( \
    NOMOD_##L00, NOMOD_##L01, NOMOD_##L02, NOMOD_##L03, NOMOD_##L04, NOMOD_##L05, NOMOD_##R00, NOMOD_##R01, NOMOD_##R02, NOMOD_##R03, NOMOD_##R04, NOMOD_##R05, \
    NOMOD_##L10, NOMOD_##L11, NOMOD_##L12, NOMOD_##L13, NOMOD_##L14, NOMOD_##L15, NOMOD_##R10, NOMOD_##R11, NOMOD_##R12, NOMOD_##R13, NOMOD_##R14, NOMOD_##R15, \
    NOMOD_##L20, NOMOD_##L21, NOMOD_##L22, NOMOD_##L23, NOMOD_##L24, NOMOD_##L25, NOMOD_##R20, NOMOD_##R21, NOMOD_##R22, NOMOD_##R23, NOMOD_##R24, NOMOD_##R25, \
    NOMOD_##L30, NOMOD_##L31, NOMOD_##L32, NOMOD_##L33, NOMOD_##L34, NOMOD_##L35, NOMOD_##R30, NOMOD_##R31, NOMOD_##R32, NOMOD_##R33, NOMOD_##R34, NOMOD_##R35 \
)
#endif


#ifndef NOMOD_
#define NOMOD_ KC_TRNS
#endif

#ifndef NOMOD
#define NOMOD(KC)                   \
case NOMOD_##KC: {                  \
  if (record->event.pressed) {      \
    suspended_mods = get_mods();    \
    unregister_mods(suspended_mods);\
    register_code(KC_##KC);         \
  }                                 \
  else {                            \
    unregister_code(KC_##KC);       \
    register_mods(suspended_mods);  \
  }                                 \
  return false;                     \
}
#endif
