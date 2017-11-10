#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"

extern keymap_config_t keymap_config;

static bool option_interrupted[2] = {0, 0};

static uint16_t space_cadet_control_timer = 0;

static uint16_t space_cadet_timer[4] = { 0, 0, 0, 0 };

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

  KC_LCDQ // left control double quote "
};

#define KC_ KC_TRNS

#define KC_CAPW LGUI(LSFT(KC_3))        // Capture whole screen
#define KC_CAPP LGUI(LSFT(KC_4))        // Capture portion of screen
#define KC_JUMP LSFT(KC_ENT)        // Capture portion of screen

#define KC_RSET RESET
#define KC_RC_ENT MT(MOD_RCTL, KC_ENT)
#define KC_RCQU MT(MOD_RCTL, KC_QUOT)
#define KC_LS_G MT(MOD_LSFT, KC_G)
#define KC_RS_H MT(MOD_RSFT, KC_H)

#define KC_LCBS MT(MOD_LGUI, KC_BSPC) // left command backspace
#define KC_RCSP MT(MOD_RGUI, KC_SPC)  // right command space

#define KC_ARWF LT(_ARROWS, KC_F)
#define KC_NUMD LT(_NUMPAD, KC_D)
#define KC_SYMS LT(_SYMBOL, KC_S)

#define KC_M0 M(0)
#define KC_M1 M(1)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐    ┌────┬────┬────┬────┬────┬────┐
     TAB , Q  , W  , E  , R  , T  ,      Y  , U  , I  , O  , P  , EQL,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
     LCDQ, A  ,SYMS,NUMD,ARWF, G  ,      H  , J  , K  , L  ,SCLN,RCQU,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
     LSPO, Z  , X  , C  , V  , B  ,      N  , M  ,COMM,DOT ,SLSH,RSPC,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
     GRV , ESC,LBRC,LHAO,LOBO,LCBS,     RCSP,ROBC,RHAC,RBRC, F2 ,JUMP
  //└────┴────┴────┴────┴────┴────┘    └────┴────┴────┴────┴────┴────┘
  ),

  [_ARROWS] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐    ┌────┬────┬────┬────┬────┬────┐
         ,    ,    ,    ,    ,    ,         ,    ,    ,    ,    ,    ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
     CAPS,    ,    ,    ,    ,    ,     LEFT,DOWN, UP ,RGHT,    ,    ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,         ,    ,    ,    ,    ,    ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
     RSET,    ,    ,    ,    ,    ,     ENT ,    ,    ,    ,    ,
  //└────┴────┴────┴────┴────┴────┘    └────┴────┴────┴────┴────┴────┘
  ),

  [_NUMPAD] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐    ┌────┬────┬────┬────┬────┬────┐
         ,    ,    ,    ,    ,    ,         , 7  , 8  , 9  ,PLUS,    ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,         , 4  , 5  , 6  ,MINS,    ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,         , 1  , 2  , 3  ,ASTR, M0 ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     MINS, 0  ,DOT ,ENT ,SLSH, M1
  //└────┴────┴────┴────┴────┴────┘    └────┴────┴────┴────┴────┴────┘
  ),

  [_SYMBOL] = KC_KEYMAP(
  //┌────┬────┬────┬────┬────┬────┐    ┌────┬────┬────┬────┬────┬────┐
         ,    ,    ,    ,    ,    ,         ,AMPR,ASTR,BSLS,PIPE,CAPW,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     QUOT,DLR ,PERC,CIRC,UNDS,CAPP,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     GRV ,EXLM, AT ,HASH,    ,    ,
  //├────┼────┼────┼────┼────┼────┤    ├────┼────┼────┼────┼────┼────┤
         ,    ,    ,    ,    ,    ,     UNDS,TILD,    ,MUTE,VOLD,VOLU
  //└────┴────┴────┴────┴────┴────┘    └────┴────┴────┴────┴────┴────┘
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

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  if (record->event.pressed) {
    switch(id) {
      case 0:
        SEND_STRING("argument from popularity much");
        return false;
      case 1:
        SEND_STRING(
          "What the fuck did you just fucking say about me, you little bitch?"
          " I'll have you know I graduated top of my class in the Navy Seals, and"
          " I've been involved in secret raids on Al-Quaeda, and I have over 300"
          " confirmed kills. I am trained in gorilla warfare and I'm the top sniper"
          " in the entire US armed forces. You are nothing to me but just another"
          " target. I will wipe you out with precision the likes of which has"
          " never been seen before on this Earth, mark my words. You think you"
          " can get away with saying shit to me over the Internet? Think again,"
          " fucker. As we speak I am contacting my network of spies across the"
          " USA and your IP is being traced right now so you better prepare for"
          " the storm, maggot. The storm that wipes out the pathetic little thing"
          " you call your life. You're fucking dead, kid. I can be anywhere,"
          " anytime, and I can kill you in over seven hundred ways, and that's"
          " just with my bare hands. Not only am I extensively trained in unarmed"
          " combat, but I have access to the entire arsenal of the United States"
          " Marine Corps and I will use it to its full extent to wipe your ass"
          " off the face of the continent, you little shit. If only you could"
          " have known what unholy retribution your little \"clever\" comment was"
          " about to bring down upon you, maybe you would have held your tongue."
          " You didn't, and now you're paying the price, you goddamn idiot."
          " I will shit all over you and you will drown in it."
          " You're fucking dead, kiddo."
        );
        return false;
    }
  }
  return MACRO_NONE;
};
