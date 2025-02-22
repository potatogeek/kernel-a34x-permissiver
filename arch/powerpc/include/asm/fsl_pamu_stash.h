<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Copyright (C) 2013 Freescale Semiconductor, Inc.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *
 * Copyright (C) 2013 Freescale Semiconductor, Inc.
>>>>>>> upstream/android-13
 */

#ifndef __FSL_PAMU_STASH_H
#define __FSL_PAMU_STASH_H

<<<<<<< HEAD
=======
struct iommu_domain;

>>>>>>> upstream/android-13
/* cache stash targets */
enum pamu_stash_target {
	PAMU_ATTR_CACHE_L1 = 1,
	PAMU_ATTR_CACHE_L2,
	PAMU_ATTR_CACHE_L3,
};

<<<<<<< HEAD
/*
 * This attribute allows configuring stashig specific parameters
 * in the PAMU hardware.
 */

struct pamu_stash_attribute {
	u32	cpu;	/* cpu number */
	u32	cache;	/* cache to stash to: L1,L2,L3 */
};
=======
int fsl_pamu_configure_l1_stash(struct iommu_domain *domain, u32 cpu);
>>>>>>> upstream/android-13

#endif  /* __FSL_PAMU_STASH_H */
