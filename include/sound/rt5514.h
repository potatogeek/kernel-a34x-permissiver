<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/sound/rt5514.h -- Platform data for RT5514
 *
 * Copyright 2016 Realtek Semiconductor Corp.
 * Author: Oder Chiou <oder_chiou@realtek.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_SND_RT5514_H
#define __LINUX_SND_RT5514_H

struct rt5514_platform_data {
	unsigned int dmic_init_delay;
	const char *dsp_calib_clk_name;
	unsigned int dsp_calib_clk_rate;
};

#endif

