/*
 * vga_types.h
 *
 * Types used by VGA software module.
 *
 *  Created on: 22.10.2015
 *      Author: tinf100053
 */

#ifndef VGA_TYPES_H_
#define VGA_TYPES_H_

// VGA output multiplexer options
typedef enum {
	MUX_BORDER,
	MUX_CHECKERS,
	MUX_FRAMEBUFFER,
	MUX_BLANK
}vga_mux_t;

// some predefines colors one might find useful
// full intensity
#define VGA_COLOR_RED		0xE0
#define VGA_COLOR_GREEN		0x1C
#define VGA_COLOR_BLUE		0x03
#define VGA_COLOR_YELLOW	(VGA_COLOR_RED | VGA_COLOR_GREEN                 )
#define VGA_COLOR_TEAL		(                VGA_COLOR_GREEN | VGA_COLOR_BLUE)
#define VGA_COLOR_VIOLET	(VGA_COLOR_RED |                   VGA_COLOR_BLUE)
#define VGA_COLOR_WHITE		(VGA_COLOR_RED | VGA_COLOR_GREEN | VGA_COLOR_BLUE)
#define VGA_COLOR_BLACK		0

#endif /* VGA_TYPES_H_ */
