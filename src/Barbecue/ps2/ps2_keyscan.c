/*
 * ps2_keyscan.c
 *
 * PS2-to-ASCII 2-stage keystroke converstion lookup tables.
 *
 *  Created on: 11.11.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#include <stdint.h>
#include "ps2.h"
#include "ps2_keyscan.h"

// normal scancodes to internal key type
const uint8_t ps2_keylut[] = {
		//0			1			2			3			4			5			6			7
		//8			9			A			B			C			D			E			F
		0			,KEY_F9		,0			,KEY_F5		,KEY_F3		,KEY_F1		,KEY_F2		,KEY_F12	,	// 0x00
		0			,KEY_F10	,KEY_F8		,KEY_F6		,KEY_F4		,KEY_TAB	,KEY_TEGU	,0			,	// 0x08
		0			,KEY_LALT	,KEY_LSHIFT	,0			,KEY_LCTRL	,KEY_Q		,KEY_1		,0			,	// 0x10
		0			,0			,KEY_Y		,KEY_S		,KEY_A		,KEY_W		,KEY_2		,0			,	// 0x18
		0			,KEY_C		,KEY_X		,KEY_D		,KEY_E		,KEY_4		,KEY_3		,0			,	// 0x20
		0			,KEY_SPACE	,KEY_V		,KEY_F		,KEY_T		,KEY_R		,KEY_5		,0			,	// 0x28
		0			,KEY_N		,KEY_B		,KEY_H		,KEY_G		,KEY_Z		,KEY_6		,0			,	// 0x30
		0			,0			,KEY_M		,KEY_J		,KEY_U		,KEY_7		,KEY_8		,0			,	// 0x38
		0			,KEY_COMMA	,KEY_K		,KEY_I		,KEY_O		,KEY_0		,KEY_9		,0			,	// 0x40
		0			,KEY_DOT	,KEY_DASH	,KEY_L		,KEY_OUML	,KEY_P		,KEY_SZ		,0			,	// 0x48
		0			,0			,KEY_AUML	,0			,KEY_UUML	,KEY_ACCENT	,0			,0			,	// 0x50
		KEY_CAPSLOCK,KEY_RSHIFT	,KEY_ENTER	,KEY_PLUS	,0			,KEY_POUND	,0			,0			,	// 0x58
		0			,0			,0			,0			,0			,0			,KEY_BSPACE	,0			,	// 0x60
		0			,KEY_KP1	,0			,KEY_KP4	,KEY_KP7	,0			,0			,0			,	// 0x68
		KEY_KP0		,KEY_KPCOMMA,KEY_KP2	,KEY_KP5	,KEY_KP6	,KEY_KP8	,KEY_ESC	,KEY_NUM	,	// 0x70
		KEY_F11		,KEY_KPPLUS	,KEY_KP3	,KEY_MINUS	,KEY_MUL	,KEY_KP9	,KEY_ROLL	,0			,	// 0x78
		0			,0			,0			,KEY_F7		,0			,0			,0			,0			,	// 0x80
};

// extended scancodes to internal key type
const uint8_t ps2_keylut_e0[] = {
		//0			1			2			3			4			5			6			7
		//8			9			A			B			C			D			E			F
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x00
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x08
		0			,KEY_RALT	,0			,0			,KEY_RCTRL	,0			,0			,0			,	// 0x10
		0			,0			,0			,0			,0			,0			,0			,KEY_LWIN	,	// 0x18
		0			,0			,0			,0			,0			,0			,0			,KEY_RWIN	,	// 0x20
		0			,0			,0			,0			,0			,0			,0			,KEY_RCTX	,	// 0x28
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x30
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x38
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x40
		0			,0			,KEY_DIV	,0			,0			,0			,0			,0			,	// 0x48
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x50
		0			,0			,KEY_KPENTER,0			,0			,0			,0			,0			,	// 0x58
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x60
		0			,KEY_END	,0			,KEY_ARRLEFT,KEY_HOME	,0			,0			,0			,	// 0x68
		KEY_INS		,KEY_DEL	,KEY_ARRDOWN,0			,KEY_ARRRIGHT,KEY_ARRUP	,0			,0			,	// 0x70
		0			,0			,KEY_PGDOWN	,0			,0			,KEY_PGUP	,0			,0			,	// 0x78
		0			,0			,0			,0			,0			,0			,0			,0			,	// 0x80

};

// internal key types to ascii or special keys
// normal (also used for non-numlock keys)
const uint8_t ps2_lutNormal[KEY_LAST] = {
		0, // invalid key
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'^', KEY_SPECIAL | KEY_UUML, KEY_SPECIAL | KEY_OUML, KEY_SPECIAL | KEY_AUML, '+', '#', ',', '.', '-',
		KEY_SPECIAL | KEY_SZ, KEY_SPECIAL | KEY_ACCENT,
		KEY_SPECIAL | KEY_F1, KEY_SPECIAL | KEY_F2, KEY_SPECIAL | KEY_F3, KEY_SPECIAL | KEY_F4, KEY_SPECIAL | KEY_F5, KEY_SPECIAL | KEY_F6, KEY_SPECIAL | KEY_F7, KEY_SPECIAL | KEY_F8, KEY_SPECIAL | KEY_F9, KEY_SPECIAL | KEY_F10, KEY_SPECIAL | KEY_F11, KEY_SPECIAL | KEY_F12,
		KEY_SPECIAL | KEY_PRINT, KEY_SPECIAL | KEY_ROLL, KEY_SPECIAL | KEY_PAUSE,
		KEY_SPECIAL | KEY_ESC, '\t', 0, 0, 0, 0, 0, ' ', 0, 0, 0, 0, 0, '\n', '\b',
		0, 0, 0, 0, 0, 0,
		KEY_SPECIAL | KEY_ARRUP, KEY_SPECIAL | KEY_ARRLEFT, KEY_SPECIAL | KEY_ARRRIGHT, KEY_SPECIAL | KEY_ARRDOWN,
		// numblock
		0, '/', '*', '-', '+', '\n', KEY_SPECIAL | KEY_KPCOMMA,
		KEY_SPECIAL | KEY_KP0, KEY_SPECIAL | KEY_KP1, KEY_SPECIAL | KEY_KP2, KEY_SPECIAL | KEY_KP3, KEY_SPECIAL | KEY_KP4, KEY_SPECIAL | KEY_KP5, KEY_SPECIAL | KEY_KP6, KEY_SPECIAL | KEY_KP7, KEY_SPECIAL | KEY_KP8, KEY_SPECIAL | KEY_KP9
};

// shift active (RSHIFT or LSHIFT or CAPSLOCK active)
// (also used for numlock keys)
const uint8_t ps2_lutShift[KEY_LAST] = {
		0, // invalid key
		'=', '!', '"', '\x015', '$', '%', '&', '/', '(', ')',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'°', KEY_SPECIAL | KEY_UUML, KEY_SPECIAL | KEY_OUML, KEY_SPECIAL | KEY_AUML, '*', '\'', ';', ':', '_',
		'?', '`',
		KEY_SPECIAL | KEY_F1, KEY_SPECIAL | KEY_F2, KEY_SPECIAL | KEY_F3, KEY_SPECIAL | KEY_F4, KEY_SPECIAL | KEY_F5, KEY_SPECIAL | KEY_F6, KEY_SPECIAL | KEY_F7, KEY_SPECIAL | KEY_F8, KEY_SPECIAL | KEY_F9, KEY_SPECIAL | KEY_F10, KEY_SPECIAL | KEY_F11, KEY_SPECIAL | KEY_F12,
		KEY_SPECIAL | KEY_PRINT, KEY_SPECIAL | KEY_ROLL, KEY_SPECIAL | KEY_PAUSE,
		KEY_SPECIAL | KEY_ESC, '\t', 0, 0, 0, 0, 0, ' ', 0, 0, 0, 0, 0, '\n', '\b',
		0, 0, 0, 0, 0, 0,
		KEY_SPECIAL | KEY_ARRUP, KEY_SPECIAL | KEY_ARRLEFT, KEY_SPECIAL | KEY_ARRRIGHT, KEY_SPECIAL | KEY_ARRDOWN,
		// numblock
		0, '/', '*', '-', '+', '\n', ',',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

// AltGr active (RALT active)
const uint8_t ps2_lutAltGr[KEY_LAST] = {
		0, // invalid key
		'}', 0, '²', '³', 0, 0, 0, '{', '[', ']',
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, '~', 0, 0, 0, 0,
		'\\', 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0,
		// numblock
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// debug printable names for keys
const char *ps2_keynames[KEY_LAST] = {
		"invalid",
		"KEY_0", "KEY_1", "KEY_2", "KEY_3", "KEY_4", "KEY_5", "KEY_6", "KEY_7", "KEY_8", "KEY_9",
		"KEY_A", "KEY_B", "KEY_C", "KEY_D", "KEY_E", "KEY_F", "KEY_G", "KEY_H", "KEY_I", "KEY_J", "KEY_K", "KEY_L", "KEY_M", "KEY_N", "KEY_O", "KEY_P", "KEY_Q", "KEY_R", "KEY_S", "KEY_T", "KEY_U", "KEY_V", "KEY_W", "KEY_X", "KEY_Y", "KEY_Z",
		"KEY_TEGU", "KEY_UUML", "KEY_OUML", "KEY_AUML", "KEY_PLUS", "KEY_POUND", "KEY_COMMA", "KEY_DOT", "KEY_DASH",
		"KEY_SZ", "KEY_ACCENT",
		"KEY_F1", "KEY_F2", "KEY_F3", "KEY_F4", "KEY_F5", "KEY_F6", "KEY_F7", "KEY_F8", "KEY_F9", "KEY_F10", "KEY_F11", "KEY_F12",
		"KEY_PRINT", "KEY_ROLL", "KEY_PAUSE",
		"KEY_ESC", "KEY_TAB", "KEY_CAPSLOCK", "KEY_LSHIFT", "KEY_LCTRL", "KEY_LWIN", "KEY_LALT", "KEY_SPACE", "KEY_RALT", "KEY_RWIN", "KEY_RCTX", "KEY_RCTRL", "KEY_RSHIFT", "KEY_ENTER", "KEY_BSPACE",
		"KEY_INS", "KEY_HOME", "KEY_PGUP", "KEY_DEL", "KEY_END", "KEY_PGDOWN",
		"KEY_ARRUP", "KEY_ARRLEFT", "KEY_ARRRIGHT", "KEY_ARRDOWN",
		// numblock
		"KEY_NUM", "KEY_DIV", "KEY_MUL", "KEY_MINUS", "KEY_KPPLUS", "KEY_KPENTER", "KEY_KPCOMMA",
		"KEY_KP0", "KEY_KP1", "KEY_KP2", "KEY_KP3", "KEY_KP4", "KEY_KP5", "KEY_KP6", "KEY_KP7", "KEY_KP8", "KEY_KP9"
};
