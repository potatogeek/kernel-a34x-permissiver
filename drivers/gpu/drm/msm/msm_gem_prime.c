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
 */

#include "msm_drv.h"
#include "msm_gem.h"

#include <linux/dma-buf.h>

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 */

#include <linux/dma-buf.h>

#include <drm/drm_prime.h>

#include "msm_drv.h"
#include "msm_gem.h"

>>>>>>> upstream/android-13
struct sg_table *msm_gem_prime_get_sg_table(struct drm_gem_object *obj)
{
	struct msm_gem_object *msm_obj = to_msm_bo(obj);
	int npages = obj->size >> PAGE_SHIFT;

	if (WARN_ON(!msm_obj->pages))  /* should have already pinned! */
		return NULL;

<<<<<<< HEAD
	return drm_prime_pages_to_sg(msm_obj->pages, npages);
}

void *msm_gem_prime_vmap(struct drm_gem_object *obj)
{
	return msm_gem_get_vaddr(obj);
}

void msm_gem_prime_vunmap(struct drm_gem_object *obj, void *vaddr)
=======
	return drm_prime_pages_to_sg(obj->dev, msm_obj->pages, npages);
}

int msm_gem_prime_vmap(struct drm_gem_object *obj, struct dma_buf_map *map)
{
	void *vaddr;

	vaddr = msm_gem_get_vaddr(obj);
	if (IS_ERR(vaddr))
		return PTR_ERR(vaddr);
	dma_buf_map_set_vaddr(map, vaddr);

	return 0;
}

void msm_gem_prime_vunmap(struct drm_gem_object *obj, struct dma_buf_map *map)
>>>>>>> upstream/android-13
{
	msm_gem_put_vaddr(obj);
}

<<<<<<< HEAD
int msm_gem_prime_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma)
{
	int ret;

	ret = drm_gem_mmap_obj(obj, obj->size, vma);
	if (ret < 0)
		return ret;

	return msm_gem_mmap_obj(vma->vm_private_data, vma);
}

=======
>>>>>>> upstream/android-13
struct drm_gem_object *msm_gem_prime_import_sg_table(struct drm_device *dev,
		struct dma_buf_attachment *attach, struct sg_table *sg)
{
	return msm_gem_import(dev, attach->dmabuf, sg);
}

int msm_gem_prime_pin(struct drm_gem_object *obj)
{
	if (!obj->import_attach)
		msm_gem_get_pages(obj);
	return 0;
}

void msm_gem_prime_unpin(struct drm_gem_object *obj)
{
	if (!obj->import_attach)
		msm_gem_put_pages(obj);
}
<<<<<<< HEAD

struct reservation_object *msm_gem_prime_res_obj(struct drm_gem_object *obj)
{
	struct msm_gem_object *msm_obj = to_msm_bo(obj);

	return msm_obj->resv;
}
=======
>>>>>>> upstream/android-13
