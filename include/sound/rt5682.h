<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/sound/rt5682.h -- Platform data for RT5682
 *
 * Copyright 2018 Realtek Microelectronics
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_SND_RT5682_H
#define __LINUX_SND_RT5682_H

enum rt5682_dmic1_data_pin {
	RT5682_DMIC1_NULL,
	RT5682_DMIC1_DATA_GPIO2,
	RT5682_DMIC1_DATA_GPIO5,
};

enum rt5682_dmic1_clk_pin {
	RT5682_DMIC1_CLK_GPIO1,
	RT5682_DMIC1_CLK_GPIO3,
};

enum rt5682_jd_src {
	RT5682_JD_NULL,
	RT5682_JD1,
};

<<<<<<< HEAD
=======
enum rt5682_dai_clks {
	RT5682_DAI_WCLK_IDX,
	RT5682_DAI_BCLK_IDX,
	RT5682_DAI_NUM_CLKS,
};

>>>>>>> upstream/android-13
struct rt5682_platform_data {

	int ldo1_en; /* GPIO for LDO1_EN */

	enum rt5682_dmic1_data_pin dmic1_data_pin;
	enum rt5682_dmic1_clk_pin dmic1_clk_pin;
	enum rt5682_jd_src jd_src;
<<<<<<< HEAD
=======
	unsigned int btndet_delay;
	unsigned int dmic_clk_rate;
	unsigned int dmic_delay;
	bool dmic_clk_driving_high;

	const char *dai_clk_names[RT5682_DAI_NUM_CLKS];
>>>>>>> upstream/android-13
};

#endif

