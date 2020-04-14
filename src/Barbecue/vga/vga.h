/*
 * vga.h
 *
 * Main VGA hardware intialization, framebuffer and pixel drawing routines.
 *
 *  Created on: 21.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#ifndef VGA_H_
#define VGA_H_

#include <stdbool.h>
#include <stdint.h>
#include "vga_types.h"

#define DISPLAY_WIDTH 	640
#define DISPLAY_HEIGHT	480

/**
 * vga_set_mux
 *
 * sets the output vga mux to generate the selected pattern
 * or pixel data from system memory
 *
 * @param type output mux select value
 */
void vga_set_mux(vga_mux_t type);

/**
 * vga_dma_init
 *
 * initializes dma logic with address of framebuffer in system memory
 * and sets output mux to display framebuffer pixel data
 */
void vga_dma_init(void);

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
void vga_pixel(uint16_t x, uint16_t y, uint8_t color);

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
void vga_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);

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
void vga_quad(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, bool filled, uint8_t color);

/**
 * vga_circle
 *
 * draws 1px wide circle with radius around point x0,y0
 *
 * @param x0 x-index of center point
 * @param y0 y-index of center point
 * @param radius circle radius in pixel
 */
void vga_circle(int x0, int y0, int radius, uint8_t color);

/**
 * vga_clear
 *
 * clears the framebuffer to black.
 *
 * @param color Color.
 */
void vga_clear(uint8_t color);

/**
 * vga_flush
 *
 * mark vga framebuffer as dirty and force the system to
 * flush any cached data to system memory
 */
void vga_flush(void);

#endif /* VGA_H_ */
