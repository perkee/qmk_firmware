#include "onekey.h"

enum keys {
    KC_A1 = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {{ KC_A1 }}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_A1:
            if (record->event.pressed) {
                SEND_STRING("BR=`git symbolic-ref --short HEAD`\n");
                SEND_STRING("git co dev && git pull && git co $BR && git merge dev && git push\n");
            }
            return false;
    }
    return true;
}
