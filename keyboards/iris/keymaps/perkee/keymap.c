#include "iris.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

static bool option_interrupted[2] = { 0, 0 };

static uint16_t space_cadet_control_timer = 0;

static uint16_t space_cadet_timer[5] = { 0, 0, 0, 0, 0 };

uint8_t suspended_mods = 0;

#define _QWERTY 0
#define _ARROWS 1
#define _NUMPAD 2
#define _SYMBOL 3

enum custom_keycodes {
  KC_LHAO  // left hyper angle open <
    = SAFE_RANGE,
  KC_RHAC, // right hyper angle close >

  KC_LOBO, // left option brace open {
  KC_ROBC, // right option brace close }

  KC_LCDQ, // left control double quote "
};

#define KC_ KC_TRNS
#define RGB_ KC_TRNS

#define KC_CAPC LGUI(LCTL(LSFT(KC_4)))        // Capture portion of the screen to pasteboard
#define KC_CAPF LGUI(LSFT(KC_4))        // Capture portion of screen to file
#define KC_JUMP LSFT(KC_ENT)        // Jump in Atom using Jumpy https://atom.io/packages/jumpy

#define KC_RSET RESET
#define KC_RC_ENT MT(MOD_RCTL, KC_ENT)
#define KC_RCQU MT(MOD_RCTL, KC_QUOT)
#define KC_LS_TAB MT(MOD_LSFT, KC_TAB)
#define KC_GENT MT(MOD_RGUI, KC_ENT)
#define KC_GUESC MT(MOD_LGUI, KC_ESC)

#define KC_LGBS MT(MOD_LGUI, KC_BSPC) // left command (GUI) backspace

#define KC_ARWF LT(_ARROWS, KC_F)
#define KC_NUMD LT(_NUMPAD, KC_D)
#define KC_SYMS LT(_SYMBOL, KC_S)
#define KC_RGBG LT(_RGB, KC_GRV)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐              ┌────┬────┬────┬────┬────┬────┐
     GRV , 1  , 2  , 3  , 4  , 5  ,                6  , 7  , 8  , 9  , 0  ,MINS,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
     TAB , Q  , W  , E  , R  , T  ,                Y  , U  , I  , O  , P  , EQL,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
     LCDQ, A  ,SYMS,NUMD,ARWF, G  ,                H  , J  , K  , L  ,SCLN,RCQU,
  //├────┼────┼────┼────┼────┼────┤ ╱────╱╲────╲ ├────┼────┼────┼────┼────┼────┤
     LSPO, Z  , X  , C  , V  , B  , GUESC,  GENT,  N  , M  ,COMM,DOT ,SLSH,RSPC,
  //├────┼────┼────┼────┼────┼────╱────╱    ╲────╲────┼────┼────┼────┼────┼────┤
                        LHAO,LOBO,BSPC,       SPC ,ROBC,RHAC
  //                  ╱────╱────╱────╱        ╲────╲────╲────╲
  ),

  [_ARROWS] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐              ┌────┬────┬────┬────┬────┬────┐
         ,    ,    ,    ,    ,    ,                   ,    ,    ,    ,    ,    ,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,                   ,    ,    ,    ,    ,    ,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
     CAPS,    ,    ,    ,    ,    ,               LEFT,DOWN, UP ,RGHT,    ,    ,
  //├────┼────┼────┼────┼────┼────┤ ╱────╱╲────╲ ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     ,       ,     ,    ,    ,    ,    ,    ,
  //├────┼────┼────┼────┼────┼────╱────╱    ╲────╲────┼────┼────┼────┼────┼────┤
                            ,    ,    ,           ,    ,
  //                  ╱────╱────╱────╱        ╲────╲────╲────╲
  ),

  [_NUMPAD] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐              ┌────┬────┬────┬────┬────┬────┐
         ,    ,    ,    ,    ,    ,                   ,    ,EQL ,SLSH,PAST,    ,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,                   , 7  , 8  , 9  ,PMNS,    ,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,               RBRC, 4  , 5  , 6  ,PPLS,    ,
  //├────┼────┼────┼────┼────┼────┤ ╱────╱╲────╲ ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     ,       , LBRC, 1  , 2  , 3  ,    ,    ,
  //├────┼────┼────┼────┼────┼────╱────╱    ╲────╲────┼────┼────┼────┼────┼────┤
                            ,    ,    ,       MINS,    , 0
  //                  ╱────╱────╱────╱        ╲────╲────╲────╲
  ),

  [_SYMBOL] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐              ┌────┬────┬────┬────┬────┬────┐
         ,    ,    ,    ,    ,    ,                   ,    ,    ,    ,    ,    ,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,                   ,AMPR,ASTR,BSLS,COLN,    ,
  //├────┼────┼────┼────┼────┼────┤              ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,               RBRC,DLR ,PERC,CIRC,PIPE,    ,
  //├────┼────┼────┼────┼────┼────┤ ╱────╱╲────╲ ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     ,       , LBRC,EXLM, AT ,HASH,    ,    ,
  //├────┼────┼────┼────┼────┼────╱────╱    ╲────╲────┼────┼────┼────┼────┼────┤
                            ,    ,    ,       UNDS,    ,TILD
  //                  ╱────╱────╱────╱        ╲────╲────╲────╲
  )
};

void mod_type(uint16_t modcode, uint16_t keycode) {
  register_mods(MOD_BIT(modcode));
  register_code(keycode);
  unregister_code(keycode);
  unregister_mods(MOD_BIT(modcode));
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LHAO: {
      if (record->event.pressed) {
        space_cadet_timer[0] = timer_read();
        register_mods(
          MOD_BIT(KC_LSHIFT) |
          MOD_BIT(KC_LGUI)   |
          MOD_BIT(KC_LALT)   |
          MOD_BIT(KC_LCTRL)
        );
      }
      else {
        unregister_mods(
          MOD_BIT(KC_LCTRL) |
          MOD_BIT(KC_LALT)  |
          MOD_BIT(KC_LGUI)
        );
        if (timer_elapsed(space_cadet_timer[0]) < TAPPING_TERM) {
          register_code(KC_COMMA);
          unregister_code(KC_COMMA);
        }
        unregister_mods(MOD_BIT(KC_LSHIFT));
      }
      return false;
    }
    case KC_RHAC: {
      if (record->event.pressed) {
        space_cadet_timer[1] = timer_read();
        register_mods(
          MOD_BIT(KC_RSHIFT) |
          MOD_BIT(KC_RGUI)   |
          MOD_BIT(KC_RALT)   |
          MOD_BIT(KC_RCTRL)
        );
      }
      else {
        unregister_mods(
          MOD_BIT(KC_RCTRL) |
          MOD_BIT(KC_RALT)  |
          MOD_BIT(KC_RGUI)
        );
        if (timer_elapsed(space_cadet_timer[1]) < TAPPING_TERM) {
          register_code(KC_DOT);
          unregister_code(KC_DOT);
        }
        unregister_mods(MOD_BIT(KC_RSHIFT));
      }
      return false;
    }

    case KC_LOBO: {
      if (record->event.pressed) {
        option_interrupted[0] = false;
        space_cadet_timer[2] = timer_read();
        register_mods(MOD_BIT(KC_LALT));
      } else {
        unregister_mods(MOD_BIT(KC_LALT));
        #ifdef DISABLE_SPACE_CADET_ROLLOVER
          if (get_mods() & MOD_BIT(KC_RALT)) { // is opposite side option down?
            option_interrupted[0] = true;
            option_interrupted[1] = true;
          }
        #endif
        if (!option_interrupted[0] && timer_elapsed(space_cadet_timer[2]) < TAPPING_TERM) {
          mod_type(KC_LSHIFT, KC_LBRACKET);
        }
      }
      return false;
    }

    case KC_ROBC: {
      if (record->event.pressed) {
        option_interrupted[1] = false;
        space_cadet_timer[3] = timer_read();
        register_mods(MOD_BIT(KC_RALT));
      }
      else {
        unregister_mods(MOD_BIT(KC_RALT));
        #ifdef DISABLE_SPACE_CADET_ROLLOVER
          if (get_mods() & MOD_BIT(KC_LALT)) { // is opposite side option down?
            option_interrupted[0] = true;
            option_interrupted[1] = true;
          }
        #endif
        if (!option_interrupted[1] && timer_elapsed(space_cadet_timer[3]) < TAPPING_TERM) {
          mod_type(KC_RSFT, KC_RBRACKET);
        }
      }
      return false;
    }

    case KC_LCDQ: {
      if (record->event.pressed) {
        space_cadet_control_timer = timer_read();
        register_mods(MOD_BIT(KC_LCTL));
      }
      else {
        unregister_mods(MOD_BIT(KC_LCTL));
        if (timer_elapsed(space_cadet_control_timer) < TAPPING_TERM) {
          mod_type(KC_LSFT, KC_QUOT);
        }
      }
      return false;
    }
  }
  return true;
}
