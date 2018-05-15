#include "chocopad.h"

#define _BASE 0
#define _FN1 1
#define _FN2 2

#define KC_ KC_TRNS
#define _______ KC_TRNS

#define FN1 MO(_FN1)
#define FN2 MO(_FN2)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = KEYMAP(
          KC_1,    KC_2,    KC_3,    KC_BSPC,
          KC_4,    KC_5,    KC_6,    KC_ENTER,
          KC_7,    KC_8,    KC_9,    _______,
          FN1,     KC_0,    _______, FN2
  ),

  [_FN1] = KEYMAP(
          RESET,   KC_PGUP, KC_UP,   KC_PGDN,
          KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,
          KC_END,  _______, _______, _______,
          _______, _______, _______, _______
  ),

  [_FN2] = KEYMAP(
          RESET,   _______, _______, _______,
          _______, _______, _______, _______,
          _______, _______, _______, _______,
          _______, _______, _______, _______
  )

};
