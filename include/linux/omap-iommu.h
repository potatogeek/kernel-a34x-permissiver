<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * omap iommu: simple virtual address space management
 *
 * Copyright (C) 2008-2009 Nokia Corporation
 *
 * Written by Hiroshi DOYU <Hiroshi.DOYU@nokia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _OMAP_IOMMU_H_
#define _OMAP_IOMMU_H_

<<<<<<< HEAD
#ifdef CONFIG_OMAP_IOMMU
extern void omap_iommu_save_ctx(struct device *dev);
extern void omap_iommu_restore_ctx(struct device *dev);
#else
static inline void omap_iommu_save_ctx(struct device *dev) {}
static inline void omap_iommu_restore_ctx(struct device *dev) {}
=======
struct iommu_domain;

#ifdef CONFIG_OMAP_IOMMU
extern void omap_iommu_save_ctx(struct device *dev);
extern void omap_iommu_restore_ctx(struct device *dev);

int omap_iommu_domain_deactivate(struct iommu_domain *domain);
int omap_iommu_domain_activate(struct iommu_domain *domain);
#else
static inline void omap_iommu_save_ctx(struct device *dev) {}
static inline void omap_iommu_restore_ctx(struct device *dev) {}

static inline int omap_iommu_domain_deactivate(struct iommu_domain *domain)
{
	return -ENODEV;
}

static inline int omap_iommu_domain_activate(struct iommu_domain *domain)
{
	return -ENODEV;
}
>>>>>>> upstream/android-13
#endif

#endif
