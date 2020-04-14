/*
 * ps2_regs.h
 *
 * Hardware register addresses and bit positions for PS2 module.
 *
 *  Created on: 26.10.2015
 *      Author: Thorsten Frizke (tinf100053)
 */

#ifndef PS2_REGS_H_
#define PS2_REGS_H_

// register offsets, symbol increments (1 symbol = 4 byte)
#define PS2_CSR			0
#define PS2_DATA		1

// control bit definitions
// PS2_CSR
#define ERR_REC				0
#define NEW_DATA			1
#define BUSY_REC			2
#define ERR_SEND			3
#define BUSY_SEND			4
#define NEW_DATA_IRQ_EN		5
#define SEND_DONE_IRQ_EN	6

#endif /* PS2_REGS_H_ */
