<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  Driver for Microtune MT2131 "QAM/8VSB single chip tuner"
 *
 *  Copyright (c) 2006 Steven Toth <stoth@linuxtv.org>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __MT2131_PRIV_H__
#define __MT2131_PRIV_H__

/* Regs */
#define MT2131_PWR              0x07
#define MT2131_UPC_1            0x0b
#define MT2131_AGC_RL           0x10
#define MT2131_MISC_2           0x15

/* frequency values in KHz */
#define MT2131_IF1              1220
#define MT2131_IF2              44000
#define MT2131_FREF             16000

struct mt2131_priv {
	struct mt2131_config *cfg;
	struct i2c_adapter   *i2c;

	u32 frequency;
};

#endif /* __MT2131_PRIV_H__ */
