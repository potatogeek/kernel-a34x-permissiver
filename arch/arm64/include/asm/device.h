<<<<<<< HEAD
/*
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_DEVICE_H
#define __ASM_DEVICE_H

struct dev_archdata {
<<<<<<< HEAD
#ifdef CONFIG_IOMMU_API
	void *iommu;			/* private IOMMU data */
#endif
#ifdef CONFIG_XEN
	const struct dma_map_ops *dev_dma_ops;
#endif
	bool dma_coherent;
#ifdef CONFIG_ARM64_DMA_USE_IOMMU
	struct dma_iommu_mapping       *mapping;
#endif
=======
>>>>>>> upstream/android-13
};

struct pdev_archdata {
};

<<<<<<< HEAD
#ifdef CONFIG_ARM64_DMA_USE_IOMMU
#define to_dma_iommu_mapping(dev) ((dev)->archdata.mapping)
#else
#define to_dma_iommu_mapping(dev) NULL
#endif

=======
>>>>>>> upstream/android-13
#endif
