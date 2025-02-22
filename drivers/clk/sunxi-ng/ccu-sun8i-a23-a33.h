<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Copyright 2016 Maxime Ripard
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _CCU_SUN8I_A23_A33_H_
#define _CCU_SUN8I_A23_A33_H_

#include <dt-bindings/clock/sun8i-a23-a33-ccu.h>
#include <dt-bindings/reset/sun8i-a23-a33-ccu.h>

#define CLK_PLL_CPUX		0
#define CLK_PLL_AUDIO_BASE	1
#define CLK_PLL_AUDIO		2
#define CLK_PLL_AUDIO_2X	3
#define CLK_PLL_AUDIO_4X	4
#define CLK_PLL_AUDIO_8X	5
#define CLK_PLL_VIDEO		6
#define CLK_PLL_VIDEO_2X	7
#define CLK_PLL_VE		8
#define CLK_PLL_DDR0		9
#define CLK_PLL_PERIPH		10
#define CLK_PLL_PERIPH_2X	11
#define CLK_PLL_GPU		12
<<<<<<< HEAD
#define CLK_PLL_MIPI		13
=======

/* The PLL MIPI clock is exported */

>>>>>>> upstream/android-13
#define CLK_PLL_HSIC		14
#define CLK_PLL_DE		15
#define CLK_PLL_DDR1		16
#define CLK_PLL_DDR		17

/* The CPUX clock is exported */

#define CLK_AXI			19
#define CLK_AHB1		20
#define CLK_APB1		21
#define CLK_APB2		22

/* All the bus gates are exported */

/* The first part of the mod clocks is exported */

#define CLK_DRAM		79

/* Some more module clocks are exported */

#define CLK_MBUS		95

/* And the last module clocks are exported */

#define CLK_NUMBER		(CLK_ATS + 1)

#endif /* _CCU_SUN8I_A23_A33_H_ */
