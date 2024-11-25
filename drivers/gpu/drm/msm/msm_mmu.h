<<<<<<< HEAD
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
>>>>>>> upstream/android-13
 */

#ifndef __MSM_MMU_H__
#define __MSM_MMU_H__

#include <linux/iommu.h>

struct msm_mmu_funcs {
<<<<<<< HEAD
	int (*attach)(struct msm_mmu *mmu, const char * const *names, int cnt);
	void (*detach)(struct msm_mmu *mmu, const char * const *names, int cnt);
	int (*map)(struct msm_mmu *mmu, uint64_t iova, struct sg_table *sgt,
			unsigned len, int prot);
	int (*unmap)(struct msm_mmu *mmu, uint64_t iova, struct sg_table *sgt,
			unsigned len);
	void (*destroy)(struct msm_mmu *mmu);
=======
	void (*detach)(struct msm_mmu *mmu);
	int (*map)(struct msm_mmu *mmu, uint64_t iova, struct sg_table *sgt,
			size_t len, int prot);
	int (*unmap)(struct msm_mmu *mmu, uint64_t iova, size_t len);
	void (*destroy)(struct msm_mmu *mmu);
	void (*resume_translation)(struct msm_mmu *mmu);
};

enum msm_mmu_type {
	MSM_MMU_GPUMMU,
	MSM_MMU_IOMMU,
	MSM_MMU_IOMMU_PAGETABLE,
>>>>>>> upstream/android-13
};

struct msm_mmu {
	const struct msm_mmu_funcs *funcs;
	struct device *dev;
<<<<<<< HEAD
	int (*handler)(void *arg, unsigned long iova, int flags);
	void *arg;
};

static inline void msm_mmu_init(struct msm_mmu *mmu, struct device *dev,
		const struct msm_mmu_funcs *funcs)
{
	mmu->dev = dev;
	mmu->funcs = funcs;
=======
	int (*handler)(void *arg, unsigned long iova, int flags, void *data);
	void *arg;
	enum msm_mmu_type type;
};

static inline void msm_mmu_init(struct msm_mmu *mmu, struct device *dev,
		const struct msm_mmu_funcs *funcs, enum msm_mmu_type type)
{
	mmu->dev = dev;
	mmu->funcs = funcs;
	mmu->type = type;
>>>>>>> upstream/android-13
}

struct msm_mmu *msm_iommu_new(struct device *dev, struct iommu_domain *domain);
struct msm_mmu *msm_gpummu_new(struct device *dev, struct msm_gpu *gpu);

static inline void msm_mmu_set_fault_handler(struct msm_mmu *mmu, void *arg,
<<<<<<< HEAD
		int (*handler)(void *arg, unsigned long iova, int flags))
=======
		int (*handler)(void *arg, unsigned long iova, int flags, void *data))
>>>>>>> upstream/android-13
{
	mmu->arg = arg;
	mmu->handler = handler;
}

<<<<<<< HEAD
=======
struct msm_mmu *msm_iommu_pagetable_create(struct msm_mmu *parent);

void msm_gpummu_params(struct msm_mmu *mmu, dma_addr_t *pt_base,
		dma_addr_t *tran_error);


int msm_iommu_pagetable_params(struct msm_mmu *mmu, phys_addr_t *ttbr,
		int *asid);

>>>>>>> upstream/android-13
#endif /* __MSM_MMU_H__ */
