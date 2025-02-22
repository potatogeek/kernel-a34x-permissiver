<<<<<<< HEAD
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 * Author: Rob Clark <rob.clark@linaro.org>
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

#include <linux/dma-buf.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2011 Texas Instruments Incorporated - https://www.ti.com/
 * Author: Rob Clark <rob.clark@linaro.org>
 */

#include <linux/dma-buf.h>
#include <linux/highmem.h>

#include <drm/drm_prime.h>
>>>>>>> upstream/android-13

#include "omap_drv.h"

/* -----------------------------------------------------------------------------
 * DMABUF Export
 */

static struct sg_table *omap_gem_map_dma_buf(
		struct dma_buf_attachment *attachment,
		enum dma_data_direction dir)
{
	struct drm_gem_object *obj = attachment->dmabuf->priv;
	struct sg_table *sg;
	dma_addr_t dma_addr;
	int ret;

	sg = kzalloc(sizeof(*sg), GFP_KERNEL);
	if (!sg)
		return ERR_PTR(-ENOMEM);

	/* camera, etc, need physically contiguous.. but we need a
	 * better way to know this..
	 */
	ret = omap_gem_pin(obj, &dma_addr);
	if (ret)
		goto out;

	ret = sg_alloc_table(sg, 1, GFP_KERNEL);
	if (ret)
		goto out;

	sg_init_table(sg->sgl, 1);
	sg_dma_len(sg->sgl) = obj->size;
	sg_set_page(sg->sgl, pfn_to_page(PFN_DOWN(dma_addr)), obj->size, 0);
	sg_dma_address(sg->sgl) = dma_addr;

	/* this must be after omap_gem_pin() to ensure we have pages attached */
	omap_gem_dma_sync_buffer(obj, dir);

	return sg;
out:
	kfree(sg);
	return ERR_PTR(ret);
}

static void omap_gem_unmap_dma_buf(struct dma_buf_attachment *attachment,
		struct sg_table *sg, enum dma_data_direction dir)
{
	struct drm_gem_object *obj = attachment->dmabuf->priv;
	omap_gem_unpin(obj);
	sg_free_table(sg);
	kfree(sg);
}

static int omap_gem_dmabuf_begin_cpu_access(struct dma_buf *buffer,
		enum dma_data_direction dir)
{
	struct drm_gem_object *obj = buffer->priv;
	struct page **pages;
<<<<<<< HEAD
	if (omap_gem_flags(obj) & OMAP_BO_TILED) {
=======
	if (omap_gem_flags(obj) & OMAP_BO_TILED_MASK) {
>>>>>>> upstream/android-13
		/* TODO we would need to pin at least part of the buffer to
		 * get de-tiled view.  For now just reject it.
		 */
		return -ENOMEM;
	}
	/* make sure we have the pages: */
	return omap_gem_get_pages(obj, &pages, true);
}

static int omap_gem_dmabuf_end_cpu_access(struct dma_buf *buffer,
					  enum dma_data_direction dir)
{
	struct drm_gem_object *obj = buffer->priv;
	omap_gem_put_pages(obj);
	return 0;
}

<<<<<<< HEAD
static void *omap_gem_dmabuf_kmap(struct dma_buf *buffer,
		unsigned long page_num)
{
	struct drm_gem_object *obj = buffer->priv;
	struct page **pages;
	omap_gem_get_pages(obj, &pages, false);
	omap_gem_cpu_sync_page(obj, page_num);
	return kmap(pages[page_num]);
}

static void omap_gem_dmabuf_kunmap(struct dma_buf *buffer,
		unsigned long page_num, void *addr)
{
	struct drm_gem_object *obj = buffer->priv;
	struct page **pages;
	omap_gem_get_pages(obj, &pages, false);
	kunmap(pages[page_num]);
}

=======
>>>>>>> upstream/android-13
static int omap_gem_dmabuf_mmap(struct dma_buf *buffer,
		struct vm_area_struct *vma)
{
	struct drm_gem_object *obj = buffer->priv;
	int ret = 0;

	ret = drm_gem_mmap_obj(obj, omap_gem_mmap_size(obj), vma);
	if (ret < 0)
		return ret;

	return omap_gem_mmap_obj(obj, vma);
}

static const struct dma_buf_ops omap_dmabuf_ops = {
	.map_dma_buf = omap_gem_map_dma_buf,
	.unmap_dma_buf = omap_gem_unmap_dma_buf,
	.release = drm_gem_dmabuf_release,
	.begin_cpu_access = omap_gem_dmabuf_begin_cpu_access,
	.end_cpu_access = omap_gem_dmabuf_end_cpu_access,
<<<<<<< HEAD
	.map = omap_gem_dmabuf_kmap,
	.unmap = omap_gem_dmabuf_kunmap,
	.mmap = omap_gem_dmabuf_mmap,
};

struct dma_buf *omap_gem_prime_export(struct drm_device *dev,
		struct drm_gem_object *obj, int flags)
=======
	.mmap = omap_gem_dmabuf_mmap,
};

struct dma_buf *omap_gem_prime_export(struct drm_gem_object *obj, int flags)
>>>>>>> upstream/android-13
{
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);

	exp_info.ops = &omap_dmabuf_ops;
	exp_info.size = obj->size;
	exp_info.flags = flags;
	exp_info.priv = obj;

<<<<<<< HEAD
	return drm_gem_dmabuf_export(dev, &exp_info);
=======
	return drm_gem_dmabuf_export(obj->dev, &exp_info);
>>>>>>> upstream/android-13
}

/* -----------------------------------------------------------------------------
 * DMABUF Import
 */

struct drm_gem_object *omap_gem_prime_import(struct drm_device *dev,
					     struct dma_buf *dma_buf)
{
	struct dma_buf_attachment *attach;
	struct drm_gem_object *obj;
	struct sg_table *sgt;
	int ret;

	if (dma_buf->ops == &omap_dmabuf_ops) {
		obj = dma_buf->priv;
		if (obj->dev == dev) {
			/*
			 * Importing dmabuf exported from out own gem increases
			 * refcount on gem itself instead of f_count of dmabuf.
			 */
<<<<<<< HEAD
			drm_gem_object_reference(obj);
=======
			drm_gem_object_get(obj);
>>>>>>> upstream/android-13
			return obj;
		}
	}

	attach = dma_buf_attach(dma_buf, dev->dev);
	if (IS_ERR(attach))
		return ERR_CAST(attach);

	get_dma_buf(dma_buf);

	sgt = dma_buf_map_attachment(attach, DMA_TO_DEVICE);
	if (IS_ERR(sgt)) {
		ret = PTR_ERR(sgt);
		goto fail_detach;
	}

	obj = omap_gem_new_dmabuf(dev, dma_buf->size, sgt);
	if (IS_ERR(obj)) {
		ret = PTR_ERR(obj);
		goto fail_unmap;
	}

	obj->import_attach = attach;

	return obj;

fail_unmap:
	dma_buf_unmap_attachment(attach, sgt, DMA_TO_DEVICE);
fail_detach:
	dma_buf_detach(dma_buf, attach);
	dma_buf_put(dma_buf);

	return ERR_PTR(ret);
}
