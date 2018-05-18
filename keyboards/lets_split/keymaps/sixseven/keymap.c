// Six-Seven - minimal layout for Let's Split.
#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _MAIN 0
#define _ALPHA 1
#define _BETA 2

enum custom_keycodes {
  KC_MAIN = SAFE_RANGE,
  KC_ALPHA,
  KC_BETA
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
#define SH(x) LSFT(KC_##x)
#define KC_SH SH
#define KC_DEBUG DEBUG
#define KC_RESET RESET
#define KC_AON AU_ON
#define KC_AOFF AU_OFF
#define KC______ _______
#define KC_K1 1
#define KC_K2 2
#define KC_K3 3
#define KC_K4 4
#define KC_K5 5
#define KC_K6 6
#define KC_K7 7
#define KC_K8 8
#define KC_K9 9
#define KC_K0 0

// α/ESC timeout.
// If α is tapped for less than this value, send ESC in addition to enabling α layer.
#define ALPHA_ESC_TIMEOUT 90

// TAP shortcut
#define TAP(key) register_code(key); unregister_code(key)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Main layer
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃ TAB ┃  Q  │  W  │  E  │  R  │  T  ┃      ┃  Y  │  U  │  I  │  O  │  P  ┃ BSP ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃α/ESC┃  A  │  S  │  D  │  F  │  G  ┃      ┃  H  │  J  │  K  │  L  │  ;  ┃ RET ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃SHIFT┃  Z  │  X  │  C  │  V  │  B  ┃      ┃  N  │  M  │  ,  │  .  │  /  ┃ CTL ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┣━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃ MOD ┃ ALT ┃SPACE┃      ┃SPACE┃  β  ┃  '  ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_MAIN] = LAYOUT_kc( \
        TAB,  Q,    W,    E,    R,    T,           Y,    U,    I,    O,    P,    BSPC, \
        ALPHA,A,    S,    D,    F,    G,           H,    J,    K,    L,    SCLN, ENT,  \
        LSFT, Z,    X,    C,    V,    B,           N,    M,    COMM, DOT,  SLSH, LCTRL,\
        _____,_____,_____,LGUI, LALT, SPC,         SPC,  BETA, QUOT, _____,_____,_____ \
    ),

    /* Alpha layer (α)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃     ┃PREV │PLAY │NEXT │     │     ┃      ┃     │ ^^^ │  ^  │ vvv │  ~  ┃ DEL ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │VOL -│VOL +│     │     ┃      ┃HOME │ <-- │  v  │ --> │  `  ┃  \  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃     │     │     │     │     ┃      ┃ END │ INS │  [  │  ]  │  (  ┃  )  ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┠━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_ALPHA] = LAYOUT_kc( \
        _____,MPRV, MPLY, MNXT, _____,_____,       _____,PGUP, UP,   PGDN, TILD, DEL,  \
        _____,_____,VOLD, VOLU, _____,_____,       HOME, LEFT, DOWN, RIGHT,GRV,  BSLS, \
        _____,_____,_____,_____,_____,_____,       END,  INS,  LBRC, RBRC, SH(9),SH(0),\
        _____,_____,_____,_____,_____,_____,       _____,_____,_____,_____,_____,_____ \
    ),

    /* Beta layer (β)
       ┏━━━━━┳━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┓      ┏━━━━━┯━━━━━┯━━━━━┯━━━━━┯━━━━━┳━━━━━┓
       ┃  `  ┃  1  │  2  │  3  │  4  │  5  ┃      ┃  6  │  7  │  8  │  9  │  0  ┃ del ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃ F1  │ F2  │ F3  │ F4  │ F5  ┃      ┃ F6  │ F7  │ F8  │ F9  │ F10 ┃  |  ┃
       ┣━━━━━╉─────┼─────┼─────┼─────┼─────┨      ┠─────┼─────┼─────┼─────┼─────╊━━━━━┫
       ┃     ┃RESET│DEBUG│     │ F11 │ F12 ┃      ┃SND +│SND -│  {  │  }  │  -  ┃  =  ┃
       ┗━━━━━┻━━━━━┷━━━━━╈━━━━━╈━━━━━╈━━━━━┫      ┣━━━━━╈━━━━━╈━━━━━╈━━━━━┷━━━━━┻━━━━━┛
                         ┃     ┃     ┃     ┃      ┃     ┃     ┃     ┃
                         ┗━━━━━┻━━━━━┻━━━━━┛      ┗━━━━━┻━━━━━┻━━━━━┛
       */
    [_BETA] = LAYOUT_kc( \
        _____,K1,   K2,   K3,   K4,   K5,          K6,   K7,   K8,   K9,   K0,   DEL,  \
        _____,F1,   F2,   F3,   F4,   F5,          F6,   F7,   F8,   F9,   F10,  PIPE, \
        _____,RESET,DEBUG,_____,F11,  F12,         AON,  AOFF, LCBR,RCBR, MINS, EQUAL,\
        _____,_____,_____,_____,_____,_____,       _____,_____,_____,_____,_____,_____ \
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t alpha_esc_start = 0;
    switch (keycode) {
        case KC_ALPHA:
            if (record->event.pressed) {
                layer_on(_ALPHA);
                alpha_esc_start = timer_read();
            } else {
                layer_off(_ALPHA);
                if (timer_elapsed(alpha_esc_start) < ALPHA_ESC_TIMEOUT) {
                    TAP(KC_ESC);
                }
            }
            return false;
        case KC_BETA:
            if (record->event.pressed) {
                layer_on(_BETA);
            } else {
                layer_off(_BETA);
            }
            return false;
    }
    return true;
}
