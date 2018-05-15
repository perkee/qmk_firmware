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
          KC_0,    KC_1,    KC_2,    KC_3,
          KC_4,    KC_5,    KC_6,    KC_7,
          KC_8,    KC_9,    KC_A,    KC_B,
          KC_C,    KC_D,    KC_E,    FN1
  ),

  [_FN1] = KEYMAP(
          RESET,   _______, _______, _______,
          _______, _______, _______, _______,
          _______, _______, _______, _______,
          _______, _______, _______, _______
  ),

  [_FN2] = KEYMAP(
          _______, _______, _______, _______,
          _______, _______, _______, _______,
          _______, _______, _______, _______,
          _______, _______, _______, _______
  )

};
