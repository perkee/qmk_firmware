#include "onekey.h"

enum keys {
    KC_A1 = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {{ KC_A1 }}
};

void matrix_init_user(void) {
    set_unicode_input_mode(UC_LNX);
}

#define TUC(x) unicode_input_start(); register_hex(x); unicode_input_finish()

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_A1:
            if (record->event.pressed) {
                TUC(0x00AF);
                SEND_STRING("\\_(");
                TUC(0x30C4);
                SEND_STRING(")_/");
                TUC(0x00AF);
                /*SEND_STRING("BR=`git symbolic-ref --short HEAD`\n");*/
                /*SEND_STRING("git co dev && git pull && git co $BR && git merge dev && git push\n");*/
            }
            return false;
    }
    return true;
}
