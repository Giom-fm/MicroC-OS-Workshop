/*
 * vga.c
 *
 * Main VGA hardware intialization, framebuffer and pixel drawing routines.
 *
 *  Created on: 21.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#include <io.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "system.h"
#include "HAL/inc/sys/alt_cache.h"
#include "vga.h"
#include "vga_regs.h"

// framebuffer in system memory (hardware is dma-ing data from this base address)
static uint8_t framebuf[DISPLAY_WIDTH * DISPLAY_HEIGHT] = { 0 };

/**
 * reg_wr
 *
 * write word to selected control register
 *
 * @param adr address of register
 * @param val value to be written
 */
static void reg_wr(uint32_t adr, uint32_t val) {
	IOWR(VGA_0_BASE, adr, val);
}

/**
 * reg_rd
 *
 * read word from selected control register
 *
 * @param adr address of register
 *
 * @return register value
 */
static uint32_t reg_rd(uint32_t adr) {
	return IORD(VGA_0_BASE, adr);
}

/**
 * reg_rmw
 *
 * read-modify-write cycle for control registers
 *
 * @param adr address of register
 * @param mask bits to be affected in the call
 * @param val value of bits to be changed to
 */
static void reg_rmw(uint32_t adr, uint32_t mask, uint32_t val) {
	uint32_t reg = reg_rd(adr);
	reg &= ~mask;
	reg |= val;
	reg_wr(adr, reg);
}

/**
 * vga_set_mux
 *
 * sets the output vga mux to generate the selected pattern
 * or pixel data from system memory
 *
 * @param type output mux select value
 */
void vga_set_mux(vga_mux_t type) {
	reg_rmw(VGA_CSR, 1<<MUX_SEL1^1<<MUX_SEL0, type << 1);
}

/**
 * vga_dma_init
 *
 * initializes dma logic with address of framebuffer in system memory
 * and sets output mux to display framebuffer pixel data
 */
void vga_dma_init() {

	// burst setup
	reg_wr(VGA_MEMADDR, (uint32_t)((void *)framebuf)); // base address
	reg_wr(VGA_BURSTLEN, DISPLAY_WIDTH / 4); // burst length (one line in DWORDS)
	reg_rmw(VGA_CSR, 1<<DMA_EN, 1<<DMA_EN); // enable initiator

	vga_set_mux(MUX_FRAMEBUFFER);
}

/**
 * vga_pixel
 *
 * sets the addressed pixel to the specified color
 * color is represented in RGB332 format, meaning:
 * bits 7-5 represent 3 bits of red intensity
 * bits 4-2 represent 3 bits of green intensity
 * bits 1-0 represent 2 bits of blue intensity
 *
 * @param x x-index of addressed pixel
 * @param y y-index of addressed pixel
 * @param color RGB332 color value to set the pixel to
 */
void vga_pixel(uint16_t x, uint16_t y, uint8_t color) {
	// argument sanity check
	if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
		// set pixel value
		framebuf[y * DISPLAY_WIDTH + x] = color;
	}
}

/**
 * vga_line
 *
 * draw single pixel width line between two points.
 * https://de.wikipedia.org/wiki/Bresenham-Algorithmus
 *
 * @param x0 x-index of start
 * @param y0 y-index of start
 * @param x1 x-index of end
 * @param y1 y-index of end
 * @param color color of the line
 */
void vga_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {

	int dx = abs((int)x1 - (int)x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs((int)y1 - (int)y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	while (1) {
		vga_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 > dy) {
			err += dy;
			x0 += sx;
		} /* e_xy+e_x > 0 */
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		} /* e_xy+e_y < 0 */
	}
}

/**
 * vga_quad
 *
 * draws a rectangle between points x1,y1 and x2,y2
 *
 * @param x1 x-index of start
 * @param y1 y-index of start
 * @param x2 x-index of end
 * @param y2 y-index of end
 * @param filled filled quad or line only
 * @param color color of the line
 */
void vga_quad(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool filled, uint8_t color) {

	if (filled) {
		uint16_t top = y1 <= y2 ? y1 : y2;
		uint16_t bot = y2 >= y1 ? y2 : y1;

		for (;top <= bot; top++) {
			vga_line(x1, top, x2, top, color);
		}
	} else {
		// simple lines connecting endpoints
		vga_line(x1, y1, x2, y1, color);
		vga_line(x2, y1, x2, y2, color);
		vga_line(x2, y2, x1, y2, color);
		vga_line(x1, y2, x1, y1, color);
	}
}

/**
 * vga_circle
 *
 * draws 1px wide circle with radius around point x0,y0
 *
 * @param x0 x-index of center point
 * @param y0 y-index of center point
 * @param radius circle radius in pixel
 */

void vga_circle(int x0, int y0, int radius, uint8_t color) {
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	// startpunkte
	vga_pixel(x0, y0 + radius, color);
	vga_pixel(x0, y0 - radius, color);
	vga_pixel(x0 + radius, y0, color);
	vga_pixel(x0 - radius, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		// achtelkreise in beide richtungen
		vga_pixel(x0 + x, y0 + y, color);
		vga_pixel(x0 - x, y0 + y, color);
		vga_pixel(x0 + x, y0 - y, color);
		vga_pixel(x0 - x, y0 - y, color);
		vga_pixel(x0 + y, y0 + x, color);
		vga_pixel(x0 - y, y0 + x, color);
		vga_pixel(x0 + y, y0 - x, color);
		vga_pixel(x0 - y, y0 - x, color);
	}
}

/**
 * vga_clear
 *
 * clears the framebuffer to black.
 */
void vga_clear(uint8_t color) {
	memset(framebuf, color, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}

/**
 * vga_flush
 *
 * mark vga framebuffer as dirty and force the system to
 * flush any cached data to system memory
 */
void vga_flush() {
	// push to memory
	alt_dcache_flush ((void*)framebuf, DISPLAY_WIDTH * DISPLAY_HEIGHT);
}
