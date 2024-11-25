<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * UDA1380 ALSA SoC Codec driver
 *
 * Copyright 2009 Philipp Zabel
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __UDA1380_H
#define __UDA1380_H

struct uda1380_platform_data {
	int gpio_power;
	int gpio_reset;
	int dac_clk;
#define UDA1380_DAC_CLK_SYSCLK 0
#define UDA1380_DAC_CLK_WSPLL  1
};

#endif /* __UDA1380_H */
