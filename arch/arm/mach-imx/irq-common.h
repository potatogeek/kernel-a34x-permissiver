<<<<<<< HEAD
/*
 * Copyright (C) BitBox Ltd 2010
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) BitBox Ltd 2010
>>>>>>> upstream/android-13
 */

#ifndef __PLAT_MXC_IRQ_COMMON_H__
#define __PLAT_MXC_IRQ_COMMON_H__

/* all normal IRQs can be FIQs */
#define FIQ_START	0

struct mxc_extra_irq
{
	int (*set_irq_fiq)(unsigned int irq, unsigned int type);
};

#endif
