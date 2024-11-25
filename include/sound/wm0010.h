<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * wm0010.h -- Platform data for WM0010 DSP Driver
 *
 * Copyright 2012 Wolfson Microelectronics PLC.
 *
 * Author: Dimitris Papastamos <dp@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef WM0010_PDATA_H
#define WM0010_PDATA_H

struct wm0010_pdata {
	int gpio_reset;

	/* Set if there is an inverter between the GPIO controlling
	 * the reset signal and the device.
	 */
	int reset_active_high;
	int irq_flags;
};

#endif
