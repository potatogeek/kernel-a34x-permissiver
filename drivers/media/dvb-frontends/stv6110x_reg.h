<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
	STV6110(A) Silicon tuner driver

	Copyright (C) Manu Abraham <abraham.manu@gmail.com>

	Copyright (C) ST Microelectronics

<<<<<<< HEAD
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
*/

#ifndef __STV6110x_REG_H
#define __STV6110x_REG_H

#define STV6110x_CTRL1				0x00
#define STV6110x_OFFST_CTRL1_K			3
#define STV6110x_WIDTH_CTRL1_K			5
#define STV6110x_OFFST_CTRL1_LPT		2
#define STV6110x_WIDTH_CTRL1_LPT		1
#define STV6110x_OFFST_CTRL1_RX			1
#define STV6110x_WIDTH_CTRL1_RX			1
#define STV6110x_OFFST_CTRL1_SYN		0
#define STV6110x_WIDTH_CTRL1_SYN		1

#define STV6110x_CTRL2				0x01
#define STV6110x_OFFST_CTRL2_CO_DIV		6
#define STV6110x_WIDTH_CTRL2_CO_DIV		2
#define STV6110x_OFFST_CTRL2_RSVD		5
#define STV6110x_WIDTH_CTRL2_RSVD		1
#define STV6110x_OFFST_CTRL2_REFOUT_SEL		4
#define STV6110x_WIDTH_CTRL2_REFOUT_SEL		1
#define STV6110x_OFFST_CTRL2_BBGAIN		0
#define STV6110x_WIDTH_CTRL2_BBGAIN		4

#define STV6110x_TNG0				0x02
#define STV6110x_OFFST_TNG0_N_DIV_7_0		0
#define STV6110x_WIDTH_TNG0_N_DIV_7_0		8

#define STV6110x_TNG1				0x03
#define STV6110x_OFFST_TNG1_R_DIV		6
#define STV6110x_WIDTH_TNG1_R_DIV		2
#define STV6110x_OFFST_TNG1_PRESC32_ON		5
#define STV6110x_WIDTH_TNG1_PRESC32_ON		1
#define STV6110x_OFFST_TNG1_DIV4SEL		4
#define STV6110x_WIDTH_TNG1_DIV4SEL		1
#define STV6110x_OFFST_TNG1_N_DIV_11_8		0
#define STV6110x_WIDTH_TNG1_N_DIV_11_8		4


#define STV6110x_CTRL3				0x04
#define STV6110x_OFFST_CTRL3_DCLOOP_OFF		7
#define STV6110x_WIDTH_CTRL3_DCLOOP_OFF		1
#define STV6110x_OFFST_CTRL3_RCCLK_OFF		6
#define STV6110x_WIDTH_CTRL3_RCCLK_OFF		1
#define STV6110x_OFFST_CTRL3_ICP		5
#define STV6110x_WIDTH_CTRL3_ICP		1
#define STV6110x_OFFST_CTRL3_CF			0
#define STV6110x_WIDTH_CTRL3_CF			5

#define STV6110x_STAT1				0x05
#define STV6110x_OFFST_STAT1_CALVCO_STRT	2
#define STV6110x_WIDTH_STAT1_CALVCO_STRT	1
#define STV6110x_OFFST_STAT1_CALRC_STRT		1
#define STV6110x_WIDTH_STAT1_CALRC_STRT		1
#define STV6110x_OFFST_STAT1_LOCK		0
#define STV6110x_WIDTH_STAT1_LOCK		1

#define STV6110x_STAT2				0x06
#define STV6110x_STAT3				0x07

#endif /* __STV6110x_REG_H */
