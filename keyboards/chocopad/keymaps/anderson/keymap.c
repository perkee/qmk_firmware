#include "chocopad.h"

#define _BASE 0
#define _FN1 1
#define _FN2 2

#define KC_ KC_TRNS
#define _______ KC_TRNS

#define FN1 MO(_FN1)
#define FN2 MO(_FN2)

enum custom_keycodes {
    KC_SR = SAFE_RANGE,
    ACTIONS_RANGE = 0x4000
};

// Mouse macro
#define MS(key) KC_MS_##key

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = KEYMAP(
          KC_ACL0, _______, MS(U),   _______,
          KC_ACL1, MS(L),   MS(D),   MS(R),
          KC_ACL2, _______, _______, _______,
          /*LT(_FN1,MS(BTN1)),LT(_FN2,MS(BTN2)),_______, _______*/
          MS(BTN1),MS(BTN2),_______, _______
  ),

  [_FN1] = KEYMAP(
          RESET,   KC_PGUP, KC_UP,   KC_PGDN,
          KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,
          KC_END,  _______, _______, _______,
          FN1,     FN2,     _______, RESET
  ),

  [_FN2] = KEYMAP(
          KC_1,    KC_2,    KC_3,    KC_BSPC,
          KC_4,    KC_5,    KC_6,    KC_ENTER,
          KC_7,    KC_8,    KC_9,    KC_0,
          FN1,     FN2,     _______, _______
  )

};

const uint16_t digit_codes[] = {
    KC_0, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9
};

void print_number(uint16_t number) {
    uint16_t digits[8];
    uint16_t digitsCount = 0;
    while (number > 0) {
        digits[digitsCount++] = number % 10;
        number /= 10;
    }
    while (digitsCount > 0) {
        uint16_t code = digit_codes[digits[--digitsCount]];
        register_code(code);
        unregister_code(code);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_SR:
            if (record->event.pressed) {
                print_number(SAFE_RANGE);
            }
            return false;
            break;
        /*case KC_A1:*/
        /*case KC_A2:*/
        /*    register_code(keycode);*/
        /*    unregister_code(keycode);*/
        /*    return false;*/
        /*    break;*/
    }
    return true;
}
