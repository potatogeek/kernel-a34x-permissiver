<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Tegra host1x GEM implementation
 *
 * Copyright (c) 2012-2013, NVIDIA Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __HOST1X_GEM_H
#define __HOST1X_GEM_H

#include <linux/host1x.h>

#include <drm/drm.h>
<<<<<<< HEAD
#include <drm/drmP.h>
=======
>>>>>>> upstream/android-13
#include <drm/drm_gem.h>

#define TEGRA_BO_BOTTOM_UP (1 << 0)

enum tegra_bo_tiling_mode {
	TEGRA_BO_TILING_MODE_PITCH,
	TEGRA_BO_TILING_MODE_TILED,
	TEGRA_BO_TILING_MODE_BLOCK,
};

<<<<<<< HEAD
struct tegra_bo_tiling {
	enum tegra_bo_tiling_mode mode;
	unsigned long value;
=======
enum tegra_bo_sector_layout {
	TEGRA_BO_SECTOR_LAYOUT_TEGRA,
	TEGRA_BO_SECTOR_LAYOUT_GPU,
};

struct tegra_bo_tiling {
	enum tegra_bo_tiling_mode mode;
	unsigned long value;
	enum tegra_bo_sector_layout sector_layout;
>>>>>>> upstream/android-13
};

struct tegra_bo {
	struct drm_gem_object gem;
	struct host1x_bo base;
	unsigned long flags;
	struct sg_table *sgt;
<<<<<<< HEAD
	dma_addr_t paddr;
=======
	dma_addr_t iova;
>>>>>>> upstream/android-13
	void *vaddr;

	struct drm_mm_node *mm;
	unsigned long num_pages;
	struct page **pages;
	/* size of IOMMU mapping */
	size_t size;

	struct tegra_bo_tiling tiling;
};

static inline struct tegra_bo *to_tegra_bo(struct drm_gem_object *gem)
{
	return container_of(gem, struct tegra_bo, gem);
}

static inline struct tegra_bo *host1x_to_tegra_bo(struct host1x_bo *bo)
{
	return container_of(bo, struct tegra_bo, base);
}

struct tegra_bo *tegra_bo_create(struct drm_device *drm, size_t size,
				 unsigned long flags);
struct tegra_bo *tegra_bo_create_with_handle(struct drm_file *file,
					     struct drm_device *drm,
					     size_t size,
					     unsigned long flags,
					     u32 *handle);
void tegra_bo_free_object(struct drm_gem_object *gem);
int tegra_bo_dumb_create(struct drm_file *file, struct drm_device *drm,
			 struct drm_mode_create_dumb *args);

extern const struct vm_operations_struct tegra_bo_vm_ops;

int __tegra_gem_mmap(struct drm_gem_object *gem, struct vm_area_struct *vma);
int tegra_drm_mmap(struct file *file, struct vm_area_struct *vma);

<<<<<<< HEAD
struct dma_buf *tegra_gem_prime_export(struct drm_device *drm,
				       struct drm_gem_object *gem,
=======
struct dma_buf *tegra_gem_prime_export(struct drm_gem_object *gem,
>>>>>>> upstream/android-13
				       int flags);
struct drm_gem_object *tegra_gem_prime_import(struct drm_device *drm,
					      struct dma_buf *buf);

<<<<<<< HEAD
=======
struct host1x_bo *tegra_gem_lookup(struct drm_file *file, u32 handle);

>>>>>>> upstream/android-13
#endif
