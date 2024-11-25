<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * tca6416 keypad platform support
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * Author: Sriramakrishnan <srk@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _TCA6416_KEYS_H
#define _TCA6416_KEYS_H

#include <linux/types.h>

struct tca6416_button {
	/* Configuration parameters */
	int code;		/* input event code (KEY_*, SW_*) */
	int active_low;
	int type;		/* input event type (EV_KEY, EV_SW) */
};

struct tca6416_keys_platform_data {
	struct tca6416_button *buttons;
	int nbuttons;
	unsigned int rep:1;	/* enable input subsystem auto repeat */
	uint16_t pinmask;
	uint16_t invert;
	int irq_is_gpio;
	int use_polling;	/* use polling if Interrupt is not connected*/
};
#endif
