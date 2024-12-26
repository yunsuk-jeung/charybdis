/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum custom_keycodes {
    M_PRN = SAFE_RANGE,
    M_BRC,
    M_CBR,
    M_RAR,
    M_CKD,
    M_CSF,
    M_GAC,
};

enum combos {
  JK_F18,
  TY_CAPS,
};

const uint16_t PROGMEM jk_f18[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM ty_caps[] = {KC_T, KC_Y, COMBO_END};

combo_t key_combos[] = {
  [JK_F18] = COMBO(jk_f18, KC_F18),
  [TY_CAPS] = COMBO(ty_caps, KC_CAPS),
};


enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_MEDIA,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
};

// Automatically enable sniping-mode on the pointer layer.
// #define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define TAB_FUN LT(LAYER_FUNCTION, KC_TAB)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
/** \brief QWERTY layout (3 rows, 10 columns). */
#define LAYOUT_LAYER_BASE                                                                     \
       KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, \
       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, \
       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, \
                      ESC_MED, SPC_NAV, TAB_FUN, ENT_SYM, BSP_NUM

/** Convenience row shorthands. */
#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL
#define ______________HOME_ROW_ARROW_R_____________ KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, XXXXXXX

/*
 * Layers used on the Charybdis Nano.
 *
 * These layers started off heavily inspired by the Miryoku layout, but trimmed
 * down and tailored for a stock experience that is meant to be fundation for
 * further personalization.
 *
 * See https://github.com/manna-harbour/miryoku for the original layout.
 */

/**
 * \brief Function layer.
 *
 * Secondary right-hand layer has function keys mirroring the numerals on the
 * primary layer with extras on the pinkie column, plus system keys on the inner
 * column. App is on the tertiary thumb key and other thumb keys are duplicated
 * from the base layer to enable auto-repeat.
 */
#define LAYOUT_LAYER_FUNCTION                                                                 \
    _______________DEAD_HALF_ROW_______________, KC_PSCR,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
    ______________HOME_ROW_GACS_L______________, KC_SCRL,   KC_F5,   KC_F6,   KC_F7,   KC_F8, \
    _______________DEAD_HALF_ROW_______________, XXXXXXX,   KC_F1,   KC_F2,   KC_F3,   KC_F4, \
                      XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX

/**
 * \brief Media layer.
 *
 * Tertiary left- and right-hand layer is media and RGB control.  This layer is
 * symmetrical to accomodate the left- and right-hand trackball.
 */
#define LAYOUT_LAYER_MEDIA                                                                    \
    LAG(KC_Q), LAG(KC_W), LAG(KC_E), LAG(KC_R), LAG(KC_T),    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX, XXXXXXX,  M_CKD,  M_CSF  , XXXXXXX,              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    LAG(KC_Z), LAG(KC_X), KC_DEL,  LAG(KC_V), LAG(KC_B),      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                               _______,    KC_MPLY,              KC_MSTP, KC_MSTP, KC_MPLY

/** \brief Mouse emulation and pointer functions. */
#define LAYOUT_LAYER_POINTER                                                                  \
    XXXXXXX, S_D_RMOD, DPI_RMOD, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, DPI_RMOD, S_D_RMOD, XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    _______,  XXXXXXX, SNIPING, DRGSCRL,  XXXXXXX, XXXXXXX,  DRGSCRL, SNIPING, XXXXXXX, _______, \
                      KC_BTN2, KC_BTN1, KC_BTN3, KC_BTN3, KC_BTN1

/**
 * \brief Navigation layer.
 *
 * Primary right-hand layer (left home thumb) is navigation and editing. Cursor
 * keys are on the home position, line and page movement below, clipboard above,
 * caps lock and insert on the inner column. Thumb keys are duplicated from the
 * base layer to avoid having to layer change mid edit and to enable auto-repeat.
 */

#define LAYOUT_LAYER_NAVIGATION                                                               \
    LAG(KC_Q), LAG(KC_W), LAG(KC_E), LAG(KC_R), LAG(KC_T),   XXXXXXX, KC_F18 , KC_UP,   LAG(KC_O), LAG(KC_P), \
    ______________HOME_ROW_GACS_L______________,             ______________HOME_ROW_ARROW_R_____________, \
    LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V), LCTL(LSFT(KC_B)),   KC_PGUP, KC_HOME, KC_DEL,  KC_END,  KC_PGDN,\
                          XXXXXXX,    _______,    XXXXXXX, KC_ENT, KC_DEL

/**
 * \brief Numeral layout.
 *
 * Primary left-hand layer (right home thumb) is numerals and symbols. Numerals
 * are in the standard numpad locations with symbols in the remaining positions.
 * `KC_DOT` is duplicated from the base layer.
 */
#define LAYOUT_LAYER_NUMERAL                                                                  \
    KC_LBRC,    KC_9,  KC_8,   KC_7,  KC_RBRC, XXXXXXX, M_PRN,   XXXXXXX,   M_CBR,   M_BRC, \
    KC_QUOT,    KC_6,  KC_5,   KC_4,  KC_EQL,  ______________HOME_ROW_ARROW_R_____________, \
     KC_GRV,    KC_3,  KC_2,   KC_1,  KC_BSLS, M_GAC  , M_RAR  , _______, _______, _______, \
                       KC_DOT, KC_0,  KC_MINS, XXXXXXX, _______

/**
 * \brief Symbols layer.
 *
 * Secondary left-hand layer has shifted symbols in the same locations to reduce
 * chording when using mods with shifted symbols. `KC_LPRN` is duplicated next to
 * `KC_RPRN`.
 */
#define LAYOUT_LAYER_SYMBOLS                                                                  \
    KC_LCBR,  KC_LPRN, KC_ASTR, KC_AMPR, KC_RCBR, _______________DEAD_HALF_ROW_______________, \
    KC_DQUO,  KC_CIRC, KC_PERC,  KC_DLR, KC_PLUS, ______________HOME_ROW_GACS_R______________, \
    KC_TILD,  KC_HASH,   KC_AT, KC_EXLM, KC_PIPE, _______________DEAD_HALF_ROW_______________, \
                       KC_LPRN, KC_RPRN, KC_UNDS, _______, XXXXXXX

/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 10-key per row layout.  Adds support for GACS (Gui, Alt, Ctl, Shift)
 * home row.  The layout passed in parameter must contain at least 20 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LCTL_T(L10), LALT_T(L11), LGUI_T(L12), LSFT_T(L13),        L14,  \
             R15,  LSFT_T(R16), LGUI_T(R17), LALT_T(R18), LCTL_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

/**
 * \brief Add pointer layer keys to a layout.
 *
 * Expects a 10-key per row layout.  The layout passed in parameter must contain
 * at least 30 keycodes.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     POINTER_MOD(LAYER_ALPHAS_QWERTY)
 */
#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
             L20,         _L_PTR(L21), L22,         L23,         L24,  \
             R25,         R26,         R27,         _L_PTR(R28), R29, \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
  ),
  [LAYER_FUNCTION] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  [LAYER_MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [LAYER_NUMERAL] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case M_PRN: // Types [], {}, or <> and puts cursor between braces.
            if (record->event.pressed) {
                SEND_STRING("()");
                // tap_code(KC_LEFT);  //Move cursor between braces.
            }
            return false;
        case M_BRC: // Types [], {}, or <> and puts cursor between braces.
            if (record->event.pressed) {
                SEND_STRING("[]");
                // tap_code(KC_LEFT);  //Move cursor between braces.
            }
            return false;
        case M_CBR: // Types [], {}, or <> and puts cursor between braces.
            if (record->event.pressed) {
                SEND_STRING("{}");
                // tap_code(KC_LEFT);  //Move cursor between braces.
            }
            return false;
        case M_RAR:
            if (record->event.pressed) {
                SEND_STRING("->");
            }
            return false;
        case M_CKD:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("kd"));
            }
            return false;
        case M_CSF:
            if (record->event.pressed) {
                // when keycode CTRL_SHIFT_F is pressed
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_F);
            } else {
                // when keycode CTRL_SHIFT_F is released
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                unregister_code(KC_F);
            }
            return false; // Skip all further processing of this key
        case M_GAC:
            if (record->event.pressed) {
                // 왼쪽 Shift, Ctrl, Alt 동시에 누르기
                register_code(KC_LGUI);
                register_code(KC_LCTL);
                register_code(KC_LALT);
            } else {
                // 키 해제
                unregister_code(KC_LGUI);
                unregister_code(KC_LCTL);
                unregister_code(KC_LALT);
            }
            return false; // Skip all further processing of this key
    }

    return true;
}