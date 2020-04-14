/*
 * ps2.c
 *
 * Main PS2 hardware initialization and keystroke decoding.
 * Handles special keys for ASCII decoding (shift, caps, numlock, ...) internally.
 * Invokes user callback function (if set up) for each new ASCII keystroke.
 *
 *  Created on: 27.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */


#include <io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "system.h"
#include "HAL/inc/sys/alt_irq.h"
#include "ps2.h"
#include "ps2_regs.h"
#include "ps2_keyscan.h"

// user event callback for new keys
void (*user_cb)(uint8_t key) = 0;

// scancode decoding
static bool breakCode = false, extendedCode = false;
static bool shift = false, capslock = false, rolllock = false;
static bool numlock = false, altGr = false;

/**
 * reg_wr
 *
 * write word to selected control register
 *
 * @param adr address of register
 * @param val value to be written
 */
static void reg_wr(uint32_t adr, uint32_t val) {
	IOWR(PS2_0_BASE, adr, val);
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
	return IORD(PS2_0_BASE, adr);
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
 * key_lookup
 *
 * multistage scancode lookup into direct ascii chars or special keycodes.
 * handles automatic capslock and numlock led actication.
 */
static uint8_t key_lookup(uint8_t keyscan) {
	eKey_t ekey = KEY_INVALID;
	uint8_t key = 0;

	// first stage key type lookup
	if (!extendedCode) {
		ekey = ps2_keylut[keyscan];
	} else {
		ekey = ps2_keylut_e0[keyscan];
	}

	// catch control keys
	// make shift/altGr
	if (!breakCode) {
		if (ekey == KEY_LSHIFT || ekey == KEY_RSHIFT)
			shift = true;
		else if (ekey == KEY_RALT)
			altGr = true;
	}
	// break shift/altGr
	else if (breakCode) {
		if (ekey == KEY_LSHIFT || ekey == KEY_RSHIFT)
			shift = false;
		else if (ekey == KEY_RALT)
			altGr = false;
	}
	// toggle capslock
	if (!breakCode && (ekey == KEY_CAPSLOCK)) {
		capslock = !capslock;
		ps2_setLeds(capslock, numlock, false);
	}
	// toggle numlock
	else if (!breakCode && (ekey == KEY_NUM)) {
		numlock = !numlock;
		ps2_setLeds(capslock, numlock, false);
	}
	// toggle rolllock
	else if (!breakCode && (ekey == KEY_ROLL)) {
		rolllock = !rolllock;
		ps2_setLeds(capslock, numlock, rolllock);
	}

	// numlock and non-numlock keys are shared into the normal and shifted lookuptables
	if (ekey >= KEY_KPCOMMA && ekey <= KEY_KP9) {
		if (numlock) {
			key = ps2_lutShift[ekey];
		} else {
			key = ps2_lutNormal[ekey];
		}
	} else {
		// second stage ascii lookup
		if (altGr) {
			// altGr
			key = ps2_lutAltGr[ekey];
		} else if (capslock == shift) {
			// normal
			key = ps2_lutNormal[ekey];
		} else {
			// shift
			key = ps2_lutShift[ekey];
		}
	}
	return key;
}

/**
 * ps2_decode
 *
 * decode incoming raw scancode bytes into make/break/extended commands.
 * invoke key lookup function for scancodes.
 */
static uint8_t ps2_decode(uint8_t keyscan) {
	uint8_t key = 0;

	// save break and extended codes
	if (keyscan == SCAN_BREAK) {
		breakCode = true;
	} else if (keyscan == SCAN_EXTENDED) {
		extendedCode = true;
	} else if (keyscan == SCAN_EXTENDED1) {
		// empty catch
	} else {
		// if break code, clear flags
		// char lookup if not break code
		// lookup intermediate key
		key = key_lookup(keyscan);

		if (breakCode) {
			breakCode = false;
			extendedCode = false;
			key = KEY_INVALID;
		}
	}

	// valid keycode when != 0
	return key;
}

/**
 * ps2_isr
 *
 * interrupt service routine for ps2 macro.
 * called by NIOS subsystem, when interrupts for new data or transmission completion
 * are enabled.
 *
 * if enabled, will call user hook for notification of a new incoming key.
 * the user should be aware of the fact, that the hook is called in an interrupt context
 * and should design his code accordingly.
 */
static void ps2_isr(void *context) {
	uint32_t reg = reg_rd(PS2_CSR);

	// handle new keyboard data
	if (reg & (1<<NEW_DATA)) {
		uint32_t data = reg_rd(PS2_DATA); // read data register clears receive irq
		// decode key and send to user (if event listener is present)
		uint8_t key = ps2_decode(data);

		if (key && user_cb) {
			user_cb(key);
		}
	}
}

/**
 * ps2_init
 *
 * enable the ps2 macro and register the ISR for incoming ps2 interrupts.
 */
void ps2_init() {
	// ps2 register irq isr
	alt_ic_isr_register(0, PS2_0_IRQ, ps2_isr, 0, 0);

	// enable ps2 new data irq
	reg_rmw(PS2_CSR, 1<<NEW_DATA_IRQ_EN, 1<<NEW_DATA_IRQ_EN);
}

/**
 * ps2_keyListener
 *
 * registers a user callback for receiving new key events.
 * the user callback is called in an interrupt context and should return
 * as fast as possible.
 *
 * @param cb user callback function, pass 0 to disable event calling
 */
void ps2_keyListener(void (*cb)(uint8_t key)) {
	user_cb = cb;
}

/*
 * ps2_setLeds
 *
 * sets keyboard leds to the supplied values
 *
 * @param caps enable capslock led, when true
 * @param num enable numlock led, when true
 * @param roll enable scrolllock led, when true
 */
void ps2_setLeds(bool caps, bool num, bool roll) {
	uint8_t leds = 0;
	// build led byte
	if (caps) leds |= LED_CAPSLOCK;
	if (num) leds |= LED_NUMLOCK;
	if (roll) leds |= LED_ROLL;

	uint32_t reg = reg_rd(PS2_CSR);
	// disable ps2 receive interrupt during transmission handshake
	reg_rmw(PS2_CSR, 1<<NEW_DATA_IRQ_EN, 0);
	// write data to keyboard
	reg_wr(PS2_DATA, 0xED);
	// busy poll done
	while(reg_rd(PS2_CSR) & (1<<BUSY_SEND));

	// wait for ack
	while(!(reg_rd(PS2_CSR) & (1<<NEW_DATA)));
	reg_rd(PS2_DATA); // handshake receive fsm

	// set leds
	reg_wr(PS2_DATA, leds);
	// busy poll done
	while(reg_rd(PS2_CSR) & (1<<BUSY_SEND));

	// wait for ack
	while(!(reg_rd(PS2_CSR) & (1<<NEW_DATA)));
	reg_rd(PS2_DATA); // handshake receive fsm

	// reenable interrupt (if previously enabled)
	if (reg & (1<<NEW_DATA_IRQ_EN)) {
		reg_rmw(PS2_CSR, 1<<NEW_DATA_IRQ_EN, 1<<NEW_DATA_IRQ_EN);
	}
}
