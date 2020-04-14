/*
 * ps2_keyscan.h
 *
 * PS2-to-ASCII 2-stage keystroke converstion lookup tables.
 *
 *  Created on: 27.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#ifndef PS2_KEYSCAN_H_
#define PS2_KEYSCAN_H_

#include <stdint.h>

#define SCAN_BREAK		0xF0
#define SCAN_EXTENDED	0xE0
#define SCAN_EXTENDED1	0xE1

#define LED_ROLL		0x01
#define LED_NUMLOCK		0x02
#define LED_CAPSLOCK	0x04

// internal key type
typedef enum {
	KEY_INVALID,
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_TEGU, KEY_UUML, KEY_OUML, KEY_AUML, KEY_PLUS, KEY_POUND, KEY_COMMA, KEY_DOT, KEY_DASH,
	KEY_SZ, KEY_ACCENT,
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
	KEY_PRINT, KEY_ROLL, KEY_PAUSE,
	KEY_ESC, KEY_TAB, KEY_CAPSLOCK, KEY_LSHIFT, KEY_LCTRL, KEY_LWIN, KEY_LALT, KEY_SPACE, KEY_RALT, KEY_RWIN, KEY_RCTX, KEY_RCTRL, KEY_RSHIFT, KEY_ENTER, KEY_BSPACE,
	KEY_INS, KEY_HOME, KEY_PGUP, KEY_DEL, KEY_END, KEY_PGDOWN,
	KEY_ARRUP, KEY_ARRLEFT, KEY_ARRRIGHT, KEY_ARRDOWN,
	// numblock
	KEY_NUM, KEY_DIV, KEY_MUL, KEY_MINUS, KEY_KPPLUS, KEY_KPENTER, KEY_KPCOMMA,
	KEY_KP0, KEY_KP1, KEY_KP2, KEY_KP3, KEY_KP4, KEY_KP5, KEY_KP6, KEY_KP7, KEY_KP8, KEY_KP9,
	KEY_LAST
} eKey_t;

// lookuptables
extern const uint8_t ps2_keylut[];
extern const uint8_t ps2_keylut_e0[];
extern const uint8_t ps2_lutNormal[];
extern const uint8_t ps2_lutShift[];
extern const uint8_t ps2_lutAltGr[];
extern const char *ps2_keynames[];

#endif /* PS2_KEYSCAN_H_ */
