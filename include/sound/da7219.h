<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * da7219.h - DA7219 ASoC Codec Driver Platform Data
 *
 * Copyright (c) 2015 Dialog Semiconductor
 *
 * Author: Adam Thomson <Adam.Thomson.Opensource@diasemi.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __DA7219_PDATA_H
#define __DA7219_PDATA_H

/* Mic Bias */
enum da7219_micbias_voltage {
	DA7219_MICBIAS_1_6V = 0,
	DA7219_MICBIAS_1_8V,
	DA7219_MICBIAS_2_0V,
	DA7219_MICBIAS_2_2V,
	DA7219_MICBIAS_2_4V,
	DA7219_MICBIAS_2_6V,
};

/* Mic input type */
enum da7219_mic_amp_in_sel {
	DA7219_MIC_AMP_IN_SEL_DIFF = 0,
	DA7219_MIC_AMP_IN_SEL_SE_P,
	DA7219_MIC_AMP_IN_SEL_SE_N,
};

struct da7219_aad_pdata;

<<<<<<< HEAD
struct da7219_pdata {
	bool wakeup_source;

	const char *dai_clks_name;
=======
enum da7219_dai_clks {
	DA7219_DAI_WCLK_IDX = 0,
	DA7219_DAI_BCLK_IDX,
	DA7219_DAI_NUM_CLKS,
};

struct da7219_pdata {
	bool wakeup_source;

	const char *dai_clk_names[DA7219_DAI_NUM_CLKS];
>>>>>>> upstream/android-13

	/* Mic */
	enum da7219_micbias_voltage micbias_lvl;
	enum da7219_mic_amp_in_sel mic_amp_in_sel;

	/* AAD */
	struct da7219_aad_pdata *aad_pdata;
};

#endif /* __DA7219_PDATA_H */
