<<<<<<< HEAD
/*
 * bd6107.h - Rohm BD6107 LEDs Driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * bd6107.h - Rohm BD6107 LEDs Driver
>>>>>>> upstream/android-13
 */
#ifndef __BD6107_H__
#define __BD6107_H__

struct device;

struct bd6107_platform_data {
	struct device *fbdev;
<<<<<<< HEAD
	int reset;			/* Reset GPIO */
=======
>>>>>>> upstream/android-13
	unsigned int def_value;
};

#endif
