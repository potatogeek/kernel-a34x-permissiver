<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/sound/wm9081.h -- Platform data for WM9081
 *
 * Copyright 2009 Wolfson Microelectronics. PLC.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_SND_WM_9081_H
#define __LINUX_SND_WM_9081_H

struct wm9081_retune_mobile_setting {
	const char *name;
	unsigned int rate;
	u16 config[20];
};

struct wm9081_pdata {
	bool irq_high;   /* IRQ is active high */
	bool irq_cmos;   /* IRQ is in CMOS mode */

	struct wm9081_retune_mobile_setting *retune_configs;
	int num_retune_configs;
};

#endif
