/*
 * vga_font_tables.h
 *
 * Storage for included font sizes.
 *
 *  Created on: 23.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#ifndef VGA_FONT_TABLES_H_
#define VGA_FONT_TABLES_H_

#define NUM_FONTS	2

#include "6x8_vertikal_LSB_1.h"
#include "12x16_vertikal_LSB_1.h"

// font storage type
struct fontType {
	uint8_t height;
	uint8_t width;
	const uint8_t *table;
};

// font storage
const struct fontType fontList[NUM_FONTS] = {
		{.height = 8,
		 .width = 6,
		 .table = &font_6x8[0][0]},
		{.height = 16,
		 .width = 12,
		 .table = &font_12x16[0][0]}
};

#endif /* VGA_FONT_TABLES_H_ */
