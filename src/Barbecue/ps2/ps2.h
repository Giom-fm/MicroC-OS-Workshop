/*
 * ps2.h
 *
 * Main PS2 hardware initialization and keystroke decoding.
 * Handles special keys for ASCII decoding (shift, caps, numlock, ...) internally.
 * Invokes user callback function (if set up) for each new ASCII keystroke.
 *
 *  Created on: 26.10.2015
 *      Author: Thorsten Fritzke (tinf100053)
 */

#ifndef PS2_H_
#define PS2_H_

#define KEY_SPECIAL		0x80

#include <stdint.h>
#include <stdbool.h>

/**
 * ps2_init
 *
 * enable the ps2 macro and register the ISR for incoming ps2 interrupts.
 */
void ps2_init(void);

/**
 * ps2_keyListener
 *
 * registers a user callback for receiving new key events.
 * the user callback is called in an interrupt context and should return
 * as fast as possible.
 *
 * @param cb user callback function, pass 0 to disable event calling
 */
void ps2_keyListener(void (*cb)(uint8_t key));

/*
 * ps2_setLeds
 *
 * sets keyboard leds to the supplied values
 *
 * @param caps enable capslock led, when true
 * @param num enable numlock led, when true
 * @param roll enable scrolllock led, when true
 */
void ps2_setLeds(bool caps, bool num, bool roll);

#endif /* PS2_H_ */
