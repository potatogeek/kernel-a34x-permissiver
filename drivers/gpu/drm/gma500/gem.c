<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  psb GEM interface
 *
 * Copyright (c) 2011, Intel Corporation.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
>>>>>>> upstream/android-13
 * Authors: Alan Cox
 *
 * TODO:
 *	-	we need to work out if the MMU is relevant (eg for
 *		accelerated operations on a GEM object)
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm.h>
#include <drm/gma_drm.h>
#include <drm/drm_vma_manager.h>
#include "psb_drv.h"

void psb_gem_free_object(struct drm_gem_object *obj)
=======
#include <linux/pagemap.h>

#include <drm/drm.h>
#include <drm/drm_vma_manager.h>

#include "gem.h"
#include "psb_drv.h"

static vm_fault_t psb_gem_fault(struct vm_fault *vmf);

static void psb_gem_free_object(struct drm_gem_object *obj)
>>>>>>> upstream/android-13
{
	struct gtt_range *gtt = container_of(obj, struct gtt_range, gem);

	/* Remove the list map if one is present */
	drm_gem_free_mmap_offset(obj);
	drm_gem_object_release(obj);

	/* This must occur last as it frees up the memory of the GEM object */
	psb_gtt_free_range(obj->dev, gtt);
}

<<<<<<< HEAD
int psb_gem_get_aperture(struct drm_device *dev, void *data,
				struct drm_file *file)
{
	return -EINVAL;
}
=======
static const struct vm_operations_struct psb_gem_vm_ops = {
	.fault = psb_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

const struct drm_gem_object_funcs psb_gem_object_funcs = {
	.free = psb_gem_free_object,
	.vm_ops = &psb_gem_vm_ops,
};
>>>>>>> upstream/android-13

/**
 *	psb_gem_create		-	create a mappable object
 *	@file: the DRM file of the client
 *	@dev: our device
 *	@size: the size requested
 *	@handlep: returned handle (opaque number)
<<<<<<< HEAD
=======
 *	@stolen: unused
 *	@align: unused
>>>>>>> upstream/android-13
 *
 *	Create a GEM object, fill in the boilerplate and attach a handle to
 *	it so that userspace can speak about it. This does the core work
 *	for the various methods that do/will create GEM objects for things
 */
int psb_gem_create(struct drm_file *file, struct drm_device *dev, u64 size,
		   u32 *handlep, int stolen, u32 align)
{
	struct gtt_range *r;
	int ret;
	u32 handle;

	size = roundup(size, PAGE_SIZE);

	/* Allocate our object - for now a direct gtt range which is not
	   stolen memory backed */
	r = psb_gtt_alloc_range(dev, size, "gem", 0, PAGE_SIZE);
	if (r == NULL) {
		dev_err(dev->dev, "no memory for %lld byte GEM object\n", size);
		return -ENOSPC;
	}
<<<<<<< HEAD
=======
	r->gem.funcs = &psb_gem_object_funcs;
>>>>>>> upstream/android-13
	/* Initialize the extra goodies GEM needs to do all the hard work */
	if (drm_gem_object_init(dev, &r->gem, size) != 0) {
		psb_gtt_free_range(dev, r);
		/* GEM doesn't give an error code so use -ENOMEM */
		dev_err(dev->dev, "GEM init failed for %lld\n", size);
		return -ENOMEM;
	}
	/* Limit the object to 32bit mappings */
	mapping_set_gfp_mask(r->gem.filp->f_mapping, GFP_KERNEL | __GFP_DMA32);
	/* Give the object a handle so we can carry it more easily */
	ret = drm_gem_handle_create(file, &r->gem, &handle);
	if (ret) {
		dev_err(dev->dev, "GEM handle failed for %p, %lld\n",
							&r->gem, size);
		drm_gem_object_release(&r->gem);
		psb_gtt_free_range(dev, r);
		return ret;
	}
	/* We have the initial and handle reference but need only one now */
<<<<<<< HEAD
	drm_gem_object_put_unlocked(&r->gem);
=======
	drm_gem_object_put(&r->gem);
>>>>>>> upstream/android-13
	*handlep = handle;
	return 0;
}

/**
 *	psb_gem_dumb_create	-	create a dumb buffer
<<<<<<< HEAD
 *	@drm_file: our client file
=======
 *	@file: our client file
>>>>>>> upstream/android-13
 *	@dev: our device
 *	@args: the requested arguments copied from userspace
 *
 *	Allocate a buffer suitable for use for a frame buffer of the
 *	form described by user space. Give userspace a handle by which
 *	to reference it.
 */
int psb_gem_dumb_create(struct drm_file *file, struct drm_device *dev,
			struct drm_mode_create_dumb *args)
{
	args->pitch = ALIGN(args->width * ((args->bpp + 7) / 8), 64);
	args->size = args->pitch * args->height;
	return psb_gem_create(file, dev, args->size, &args->handle, 0,
			      PAGE_SIZE);
}

/**
 *	psb_gem_fault		-	pagefault handler for GEM objects
<<<<<<< HEAD
 *	@vma: the VMA of the GEM object
=======
>>>>>>> upstream/android-13
 *	@vmf: fault detail
 *
 *	Invoked when a fault occurs on an mmap of a GEM managed area. GEM
 *	does most of the work for us including the actual map/unmap calls
 *	but we need to do the actual page work.
 *
 *	This code eventually needs to handle faulting objects in and out
 *	of the GTT and repacking it when we run out of space. We can put
 *	that off for now and for our simple uses
 *
 *	The VMA was set up by GEM. In doing so it also ensured that the
 *	vma->vm_private_data points to the GEM object that is backing this
 *	mapping.
 */
<<<<<<< HEAD
vm_fault_t psb_gem_fault(struct vm_fault *vmf)
=======
static vm_fault_t psb_gem_fault(struct vm_fault *vmf)
>>>>>>> upstream/android-13
{
	struct vm_area_struct *vma = vmf->vma;
	struct drm_gem_object *obj;
	struct gtt_range *r;
	int err;
	vm_fault_t ret;
	unsigned long pfn;
	pgoff_t page_offset;
	struct drm_device *dev;
	struct drm_psb_private *dev_priv;

	obj = vma->vm_private_data;	/* GEM object */
	dev = obj->dev;
	dev_priv = dev->dev_private;

	r = container_of(obj, struct gtt_range, gem);	/* Get the gtt range */

	/* Make sure we don't parallel update on a fault, nor move or remove
	   something from beneath our feet */
	mutex_lock(&dev_priv->mmap_mutex);

	/* For now the mmap pins the object and it stays pinned. As things
	   stand that will do us no harm */
	if (r->mmapping == 0) {
		err = psb_gtt_pin(r);
		if (err < 0) {
			dev_err(dev->dev, "gma500: pin failed: %d\n", err);
			ret = vmf_error(err);
			goto fail;
		}
		r->mmapping = 1;
	}

	/* Page relative to the VMA start - we must calculate this ourselves
	   because vmf->pgoff is the fake GEM offset */
	page_offset = (vmf->address - vma->vm_start) >> PAGE_SHIFT;

	/* CPU view of the page, don't go via the GART for CPU writes */
	if (r->stolen)
		pfn = (dev_priv->stolen_base + r->offset) >> PAGE_SHIFT;
	else
		pfn = page_to_pfn(r->pages[page_offset]);
	ret = vmf_insert_pfn(vma, vmf->address, pfn);
fail:
	mutex_unlock(&dev_priv->mmap_mutex);

	return ret;
}
