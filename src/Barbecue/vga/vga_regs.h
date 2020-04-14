/*
 * vga_regs.h
 *
 * Hardware register addresses and bit positions for VGA module.
 *
 *  Created on: 22.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#ifndef VGA_REGS_H_
#define VGA_REGS_H_

// register offsets, symbol increments (1 symbol = 4 byte)
#define VGA_CSR			0
#define VGA_MEMADDR		1
#define VGA_BURSTLEN	2

// control bit definitions
// VGA_CSR
#define DMA_EN			0
#define MUX_SEL0		1
#define MUX_SEL1		2

#define MUX_SEL_BORDER		(0)
#define MUX_SEL_CHECKERS	(1<<MUX_SEL0)
#define MUX_SEL_FRAMEBUF	(1<<MUX_SEL1)
#define MUX_SEL_BLANK		(1<<MUX_SEL1^1<<MUX_SEL0)

#endif /* VGA_REGS_H_ */
