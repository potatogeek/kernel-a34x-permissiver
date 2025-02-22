<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm/mach-tegra/board.h
 *
 * Copyright (c) 2013 NVIDIA Corporation. All rights reserved.
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@google.com>
 *	Erik Gilling <konkers@google.com>
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __MACH_TEGRA_BOARD_H
#define __MACH_TEGRA_BOARD_H

#include <linux/types.h>
#include <linux/reboot.h>

void __init tegra_map_common_io(void);
void __init tegra_init_irq(void);

void __init tegra_paz00_wifikill_init(void);

#endif
