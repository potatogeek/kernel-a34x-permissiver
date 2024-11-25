<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/sound/cs42l73.h -- Platform data for CS42L73
 *
 * Copyright (c) 2012 Cirrus Logic Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __CS42L73_H
#define __CS42L73_H

struct cs42l73_platform_data {
	/* RST GPIO */
	unsigned int reset_gpio;
	unsigned int chgfreq;
	int jack_detection;
	unsigned int mclk_freq;
};

#endif /* __CS42L73_H */
