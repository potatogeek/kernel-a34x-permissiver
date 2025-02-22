<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * linux/sound/cs35l34.h -- Platform data for CS35l34
 *
 * Copyright (c) 2016 Cirrus Logic Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __CS35L34_H
#define __CS35L34_H

struct cs35l34_platform_data {
	/* Set AIF to half drive strength */
	bool aif_half_drv;
	/* Digital Soft Ramp Disable */
	bool digsft_disable;
	/* Amplifier Invert */
	bool amp_inv;
	/* Peak current (mA) */
	unsigned int boost_peak;
	/* Boost inductor value (nH) */
	unsigned int boost_ind;
	/* Boost Controller Voltage Setting (mV) */
	unsigned int boost_vtge;
	/* Gain Change Zero Cross */
	bool gain_zc_disable;
	/* SDIN Left/Right Selection */
	unsigned int i2s_sdinloc;
	/* TDM Rising Edge */
	bool tdm_rising_edge;
};

#endif /* __CS35L34_H */
