<<<<<<< HEAD
/*
 * Copyright (C) 2014-2015 ARM Ltd.
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
 * Copyright (C) 2014-2015 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __DMA_IOMMU_H
#define __DMA_IOMMU_H

<<<<<<< HEAD
#ifdef __KERNEL__
#include <linux/types.h>
#include <asm/errno.h>
=======
#include <linux/errno.h>
#include <linux/types.h>
>>>>>>> upstream/android-13

#ifdef CONFIG_IOMMU_DMA
#include <linux/dma-mapping.h>
#include <linux/iommu.h>
#include <linux/msi.h>

<<<<<<< HEAD
struct iova_domain;

int iommu_dma_init(void);

=======
>>>>>>> upstream/android-13
/* Domain management interface for IOMMU drivers */
int iommu_get_dma_cookie(struct iommu_domain *domain);
int iommu_get_msi_cookie(struct iommu_domain *domain, dma_addr_t base);
void iommu_put_dma_cookie(struct iommu_domain *domain);

/* Setup call for arch DMA mapping code */
<<<<<<< HEAD
int iommu_dma_init_domain(struct iommu_domain *domain, dma_addr_t base,
		u64 size, struct device *dev);

/* General helpers for DMA-API <-> IOMMU-API interaction */
int dma_info_to_prot(enum dma_data_direction dir, bool coherent,
		     unsigned long attrs);

/*
 * These implement the bulk of the relevant DMA mapping callbacks, but require
 * the arch code to take care of attributes and cache maintenance
 */
struct page **iommu_dma_alloc(struct device *dev, size_t size, gfp_t gfp,
		unsigned long attrs, int prot, dma_addr_t *handle,
		void (*flush_page)(struct device *, const void *, phys_addr_t));
void iommu_dma_free(struct device *dev, struct page **pages, size_t size,
		dma_addr_t *handle);

int iommu_dma_mmap(struct page **pages, size_t size, struct vm_area_struct *vma);

dma_addr_t iommu_dma_map_page(struct device *dev, struct page *page,
		unsigned long offset, size_t size, int prot);
int iommu_dma_map_sg(struct device *dev, struct scatterlist *sg,
		int nents, int prot);
size_t iommu_dma_prepare_map_sg(struct device *dev, struct iova_domain *iovad,
				struct scatterlist *sg, int nents);
int iommu_dma_finalise_sg(struct device *dev, struct scatterlist *sg,
		int nents, dma_addr_t dma_addr);
void iommu_dma_invalidate_sg(struct scatterlist *sg, int nents);

/*
 * Arch code with no special attribute handling may use these
 * directly as DMA mapping callbacks for simplicity
 */
void iommu_dma_unmap_page(struct device *dev, dma_addr_t handle, size_t size,
		enum dma_data_direction dir, unsigned long attrs);
void iommu_dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nents,
		enum dma_data_direction dir, unsigned long attrs);
dma_addr_t iommu_dma_map_resource(struct device *dev, phys_addr_t phys,
		size_t size, enum dma_data_direction dir, unsigned long attrs);
void iommu_dma_unmap_resource(struct device *dev, dma_addr_t handle,
		size_t size, enum dma_data_direction dir, unsigned long attrs);
int iommu_dma_mapping_error(struct device *dev, dma_addr_t dma_addr);

/* The DMA API isn't _quite_ the whole story, though... */
void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg);
void iommu_dma_get_resv_regions(struct device *dev, struct list_head *list);

#ifdef CONFIG_MTK_IOMMU_V2
void iommu_dma_dump_iovad(struct iommu_domain *domain,
		unsigned long target);
int iommu_dma_get_iovad_info(struct device *dev,
	unsigned long *base, unsigned long *max);
#endif

int iommu_dma_reserve_iova(struct device *dev, dma_addr_t base,
			   u64 size);

int iommu_dma_enable_best_fit_algo(struct device *dev);

#else

struct iommu_domain;
struct msi_msg;
struct device;

static inline int iommu_dma_init(void)
{
	return 0;
=======
void iommu_setup_dma_ops(struct device *dev, u64 dma_base, u64 dma_limit);
int iommu_dma_init_fq(struct iommu_domain *domain);

/* The DMA API isn't _quite_ the whole story, though... */
/*
 * iommu_dma_prepare_msi() - Map the MSI page in the IOMMU device
 *
 * The MSI page will be stored in @desc.
 *
 * Return: 0 on success otherwise an error describing the failure.
 */
int iommu_dma_prepare_msi(struct msi_desc *desc, phys_addr_t msi_addr);

/* Update the MSI message if required. */
void iommu_dma_compose_msi_msg(struct msi_desc *desc,
			       struct msi_msg *msg);

void iommu_dma_get_resv_regions(struct device *dev, struct list_head *list);

void iommu_dma_free_cpu_cached_iovas(unsigned int cpu,
		struct iommu_domain *domain);

extern bool iommu_dma_forcedac;

int iommu_dma_enable_best_fit_algo(struct device *dev);

#else /* CONFIG_IOMMU_DMA */

struct iommu_domain;
struct msi_desc;
struct msi_msg;
struct device;

static inline void iommu_setup_dma_ops(struct device *dev, u64 dma_base,
				       u64 dma_limit)
{
}

static inline int iommu_dma_init_fq(struct iommu_domain *domain)
{
	return -EINVAL;
>>>>>>> upstream/android-13
}

static inline int iommu_get_dma_cookie(struct iommu_domain *domain)
{
	return -ENODEV;
}

static inline int iommu_get_msi_cookie(struct iommu_domain *domain, dma_addr_t base)
{
	return -ENODEV;
}

static inline void iommu_put_dma_cookie(struct iommu_domain *domain)
{
}

<<<<<<< HEAD
static inline void iommu_dma_map_msi_msg(int irq, struct msi_msg *msg)
=======
static inline int iommu_dma_prepare_msi(struct msi_desc *desc,
					phys_addr_t msi_addr)
{
	return 0;
}

static inline void iommu_dma_compose_msi_msg(struct msi_desc *desc,
					     struct msi_msg *msg)
>>>>>>> upstream/android-13
{
}

static inline void iommu_dma_get_resv_regions(struct device *dev, struct list_head *list)
{
}

<<<<<<< HEAD
static inline int iommu_dma_reserve_iova(struct device *dev, dma_addr_t base,
					 u64 size)
{
	return -ENODEV;
}

=======
>>>>>>> upstream/android-13
static inline int iommu_dma_enable_best_fit_algo(struct device *dev)
{
	return -ENODEV;
}

#endif	/* CONFIG_IOMMU_DMA */
<<<<<<< HEAD
#endif	/* __KERNEL__ */
=======
>>>>>>> upstream/android-13
#endif	/* __DMA_IOMMU_H */
