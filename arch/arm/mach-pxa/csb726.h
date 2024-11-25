<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  Support for Cogent CSB726
 *
 *  Copyright (c) 2008 Dmitry Baryshkov
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */
#ifndef CSB726_H
#define CSB726_H

#include <mach/irqs.h> /* PXA_GPIO_TO_IRQ */

#define CSB726_GPIO_IRQ_LAN	52
#define CSB726_GPIO_IRQ_SM501	53
#define CSB726_GPIO_MMC_DETECT	100
#define CSB726_GPIO_MMC_RO	101

#define CSB726_FLASH_SIZE	(64 * 1024 * 1024)
#define CSB726_FLASH_uMON	(8 * 1024 * 1024)

#define CSB726_IRQ_LAN		PXA_GPIO_TO_IRQ(CSB726_GPIO_IRQ_LAN)
#define CSB726_IRQ_SM501	PXA_GPIO_TO_IRQ(CSB726_GPIO_IRQ_SM501)

#endif

