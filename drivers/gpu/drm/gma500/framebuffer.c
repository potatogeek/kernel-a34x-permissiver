<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/**************************************************************************
 * Copyright (c) 2007-2011, Intel Corporation.
 * All Rights Reserved.
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
 **************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/pfn_t.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/console.h>

#include <drm/drmP.h>
#include <drm/drm.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>

#include "psb_drv.h"
#include "psb_intel_reg.h"
#include "psb_intel_drv.h"
#include "framebuffer.h"
#include "gtt.h"
=======
 **************************************************************************/

#include <linux/console.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/pfn_t.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/tty.h>

#include <drm/drm.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_framebuffer_helper.h>

#include "framebuffer.h"
#include "gem.h"
#include "gtt.h"
#include "psb_drv.h"
#include "psb_intel_drv.h"
#include "psb_intel_reg.h"
>>>>>>> upstream/android-13

static const struct drm_framebuffer_funcs psb_fb_funcs = {
	.destroy = drm_gem_fb_destroy,
	.create_handle = drm_gem_fb_create_handle,
};

#define CMAP_TOHW(_val, _width) ((((_val) << (_width)) + 0x7FFF - (_val)) >> 16)

static int psbfb_setcolreg(unsigned regno, unsigned red, unsigned green,
			   unsigned blue, unsigned transp,
			   struct fb_info *info)
{
<<<<<<< HEAD
	struct psb_fbdev *fbdev = info->par;
	struct drm_framebuffer *fb = fbdev->psb_fb_helper.fb;
=======
	struct drm_fb_helper *fb_helper = info->par;
	struct drm_framebuffer *fb = fb_helper->fb;
>>>>>>> upstream/android-13
	uint32_t v;

	if (!fb)
		return -ENOMEM;

	if (regno > 255)
		return 1;

	red = CMAP_TOHW(red, info->var.red.length);
	blue = CMAP_TOHW(blue, info->var.blue.length);
	green = CMAP_TOHW(green, info->var.green.length);
	transp = CMAP_TOHW(transp, info->var.transp.length);

	v = (red << info->var.red.offset) |
	    (green << info->var.green.offset) |
	    (blue << info->var.blue.offset) |
	    (transp << info->var.transp.offset);

	if (regno < 16) {
		switch (fb->format->cpp[0] * 8) {
		case 16:
			((uint32_t *) info->pseudo_palette)[regno] = v;
			break;
		case 24:
		case 32:
			((uint32_t *) info->pseudo_palette)[regno] = v;
			break;
		}
	}

	return 0;
}

<<<<<<< HEAD
static int psbfb_pan(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct psb_fbdev *fbdev = info->par;
	struct psb_framebuffer *psbfb = &fbdev->pfb;
	struct drm_device *dev = psbfb->base.dev;
	struct gtt_range *gtt = to_gtt_range(psbfb->base.obj[0]);

	/*
	 *	We have to poke our nose in here. The core fb code assumes
	 *	panning is part of the hardware that can be invoked before
	 *	the actual fb is mapped. In our case that isn't quite true.
	 */
	if (gtt->npage) {
		/* GTT roll shifts in 4K pages, we need to shift the right
		   number of pages */
		int pages = info->fix.line_length >> 12;
		psb_gtt_roll(dev, gtt, var->yoffset * pages);
	}
        return 0;
}

static vm_fault_t psbfb_vm_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	struct psb_framebuffer *psbfb = vma->vm_private_data;
	struct drm_device *dev = psbfb->base.dev;
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct gtt_range *gtt = to_gtt_range(psbfb->base.obj[0]);
=======
static vm_fault_t psbfb_vm_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	struct drm_framebuffer *fb = vma->vm_private_data;
	struct drm_device *dev = fb->dev;
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct gtt_range *gtt = to_gtt_range(fb->obj[0]);
>>>>>>> upstream/android-13
	int page_num;
	int i;
	unsigned long address;
	vm_fault_t ret = VM_FAULT_SIGBUS;
	unsigned long pfn;
	unsigned long phys_addr = (unsigned long)dev_priv->stolen_base +
				  gtt->offset;

	page_num = vma_pages(vma);
	address = vmf->address - (vmf->pgoff << PAGE_SHIFT);

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	for (i = 0; i < page_num; i++) {
		pfn = (phys_addr >> PAGE_SHIFT);

		ret = vmf_insert_mixed(vma, address,
				__pfn_to_pfn_t(pfn, PFN_DEV));
		if (unlikely(ret & VM_FAULT_ERROR))
			break;
		address += PAGE_SIZE;
		phys_addr += PAGE_SIZE;
	}
	return ret;
}

static void psbfb_vm_open(struct vm_area_struct *vma)
{
}

static void psbfb_vm_close(struct vm_area_struct *vma)
{
}

static const struct vm_operations_struct psbfb_vm_ops = {
	.fault	= psbfb_vm_fault,
	.open	= psbfb_vm_open,
	.close	= psbfb_vm_close
};

static int psbfb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	struct psb_fbdev *fbdev = info->par;
	struct psb_framebuffer *psbfb = &fbdev->pfb;
=======
	struct drm_fb_helper *fb_helper = info->par;
	struct drm_framebuffer *fb = fb_helper->fb;
>>>>>>> upstream/android-13

	if (vma->vm_pgoff != 0)
		return -EINVAL;
	if (vma->vm_pgoff > (~0UL >> PAGE_SHIFT))
		return -EINVAL;

<<<<<<< HEAD
	if (!psbfb->addr_space)
		psbfb->addr_space = vma->vm_file->f_mapping;
=======
>>>>>>> upstream/android-13
	/*
	 * If this is a GEM object then info->screen_base is the virtual
	 * kernel remapping of the object. FIXME: Review if this is
	 * suitable for our mmap work
	 */
	vma->vm_ops = &psbfb_vm_ops;
<<<<<<< HEAD
	vma->vm_private_data = (void *)psbfb;
=======
	vma->vm_private_data = (void *)fb;
>>>>>>> upstream/android-13
	vma->vm_flags |= VM_IO | VM_MIXEDMAP | VM_DONTEXPAND | VM_DONTDUMP;
	return 0;
}

<<<<<<< HEAD
static struct fb_ops psbfb_ops = {
	.owner = THIS_MODULE,
	DRM_FB_HELPER_DEFAULT_OPS,
	.fb_setcolreg = psbfb_setcolreg,
	.fb_fillrect = drm_fb_helper_cfb_fillrect,
	.fb_copyarea = psbfb_copyarea,
	.fb_imageblit = drm_fb_helper_cfb_imageblit,
	.fb_mmap = psbfb_mmap,
	.fb_sync = psbfb_sync,
};

static struct fb_ops psbfb_roll_ops = {
	.owner = THIS_MODULE,
	DRM_FB_HELPER_DEFAULT_OPS,
	.fb_setcolreg = psbfb_setcolreg,
	.fb_fillrect = drm_fb_helper_cfb_fillrect,
	.fb_copyarea = drm_fb_helper_cfb_copyarea,
	.fb_imageblit = drm_fb_helper_cfb_imageblit,
	.fb_pan_display = psbfb_pan,
	.fb_mmap = psbfb_mmap,
};

static struct fb_ops psbfb_unaccel_ops = {
=======
static const struct fb_ops psbfb_unaccel_ops = {
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
	DRM_FB_HELPER_DEFAULT_OPS,
	.fb_setcolreg = psbfb_setcolreg,
	.fb_fillrect = drm_fb_helper_cfb_fillrect,
	.fb_copyarea = drm_fb_helper_cfb_copyarea,
	.fb_imageblit = drm_fb_helper_cfb_imageblit,
	.fb_mmap = psbfb_mmap,
};

/**
 *	psb_framebuffer_init	-	initialize a framebuffer
 *	@dev: our DRM device
 *	@fb: framebuffer to set up
 *	@mode_cmd: mode description
<<<<<<< HEAD
 *	@gt: backing object
=======
 *	@obj: backing object
>>>>>>> upstream/android-13
 *
 *	Configure and fill in the boilerplate for our frame buffer. Return
 *	0 on success or an error code if we fail.
 */
static int psb_framebuffer_init(struct drm_device *dev,
<<<<<<< HEAD
					struct psb_framebuffer *fb,
					const struct drm_mode_fb_cmd2 *mode_cmd,
					struct gtt_range *gt)
=======
					struct drm_framebuffer *fb,
					const struct drm_mode_fb_cmd2 *mode_cmd,
					struct drm_gem_object *obj)
>>>>>>> upstream/android-13
{
	const struct drm_format_info *info;
	int ret;

	/*
	 * Reject unknown formats, YUV formats, and formats with more than
	 * 4 bytes per pixel.
	 */
<<<<<<< HEAD
	info = drm_format_info(mode_cmd->pixel_format);
=======
	info = drm_get_format_info(dev, mode_cmd);
>>>>>>> upstream/android-13
	if (!info || !info->depth || info->cpp[0] > 4)
		return -EINVAL;

	if (mode_cmd->pitches[0] & 63)
		return -EINVAL;

<<<<<<< HEAD
	drm_helper_mode_fill_fb_struct(dev, &fb->base, mode_cmd);
	fb->base.obj[0] = &gt->gem;
	ret = drm_framebuffer_init(dev, &fb->base, &psb_fb_funcs);
=======
	drm_helper_mode_fill_fb_struct(dev, fb, mode_cmd);
	fb->obj[0] = obj;
	ret = drm_framebuffer_init(dev, fb, &psb_fb_funcs);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev->dev, "framebuffer init failed: %d\n", ret);
		return ret;
	}
	return 0;
}

/**
 *	psb_framebuffer_create	-	create a framebuffer backed by gt
 *	@dev: our DRM device
 *	@mode_cmd: the description of the requested mode
<<<<<<< HEAD
 *	@gt: the backing object
=======
 *	@obj: the backing object
>>>>>>> upstream/android-13
 *
 *	Create a framebuffer object backed by the gt, and fill in the
 *	boilerplate required
 *
 *	TODO: review object references
 */

static struct drm_framebuffer *psb_framebuffer_create
			(struct drm_device *dev,
			 const struct drm_mode_fb_cmd2 *mode_cmd,
<<<<<<< HEAD
			 struct gtt_range *gt)
{
	struct psb_framebuffer *fb;
=======
			 struct drm_gem_object *obj)
{
	struct drm_framebuffer *fb;
>>>>>>> upstream/android-13
	int ret;

	fb = kzalloc(sizeof(*fb), GFP_KERNEL);
	if (!fb)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	ret = psb_framebuffer_init(dev, fb, mode_cmd, gt);
=======
	ret = psb_framebuffer_init(dev, fb, mode_cmd, obj);
>>>>>>> upstream/android-13
	if (ret) {
		kfree(fb);
		return ERR_PTR(ret);
	}
<<<<<<< HEAD
	return &fb->base;
=======
	return fb;
>>>>>>> upstream/android-13
}

/**
 *	psbfb_alloc		-	allocate frame buffer memory
 *	@dev: the DRM device
 *	@aligned_size: space needed
 *
 *	Allocate the frame buffer. In the usual case we get a GTT range that
 *	is stolen memory backed and life is simple. If there isn't sufficient
 *	we fail as we don't have the virtual mapping space to really vmap it
 *	and the kernel console code can't handle non linear framebuffers.
 *
 *	Re-address this as and if the framebuffer layer grows this ability.
 */
static struct gtt_range *psbfb_alloc(struct drm_device *dev, int aligned_size)
{
	struct gtt_range *backing;
	/* Begin by trying to use stolen memory backing */
	backing = psb_gtt_alloc_range(dev, aligned_size, "fb", 1, PAGE_SIZE);
	if (backing) {
<<<<<<< HEAD
=======
		backing->gem.funcs = &psb_gem_object_funcs;
>>>>>>> upstream/android-13
		drm_gem_private_object_init(dev, &backing->gem, aligned_size);
		return backing;
	}
	return NULL;
}

/**
 *	psbfb_create		-	create a framebuffer
<<<<<<< HEAD
 *	@fbdev: the framebuffer device
=======
 *	@fb_helper: the framebuffer helper
>>>>>>> upstream/android-13
 *	@sizes: specification of the layout
 *
 *	Create a framebuffer to the specifications provided
 */
<<<<<<< HEAD
static int psbfb_create(struct psb_fbdev *fbdev,
				struct drm_fb_helper_surface_size *sizes)
{
	struct drm_device *dev = fbdev->psb_fb_helper.dev;
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct fb_info *info;
	struct drm_framebuffer *fb;
	struct psb_framebuffer *psbfb = &fbdev->pfb;
=======
static int psbfb_create(struct drm_fb_helper *fb_helper,
				struct drm_fb_helper_surface_size *sizes)
{
	struct drm_device *dev = fb_helper->dev;
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	struct fb_info *info;
	struct drm_framebuffer *fb;
>>>>>>> upstream/android-13
	struct drm_mode_fb_cmd2 mode_cmd;
	int size;
	int ret;
	struct gtt_range *backing;
	u32 bpp, depth;
<<<<<<< HEAD
	int gtt_roll = 0;
	int pitch_lines = 0;
=======
>>>>>>> upstream/android-13

	mode_cmd.width = sizes->surface_width;
	mode_cmd.height = sizes->surface_height;
	bpp = sizes->surface_bpp;
	depth = sizes->surface_depth;

	/* No 24bit packed */
	if (bpp == 24)
		bpp = 32;

<<<<<<< HEAD
	do {
		/*
		 * Acceleration via the GTT requires pitch to be
		 * power of two aligned. Preferably page but less
		 * is ok with some fonts
		 */
        	mode_cmd.pitches[0] =  ALIGN(mode_cmd.width * ((bpp + 7) / 8), 4096 >> pitch_lines);

        	size = mode_cmd.pitches[0] * mode_cmd.height;
        	size = ALIGN(size, PAGE_SIZE);

		/* Allocate the fb in the GTT with stolen page backing */
		backing = psbfb_alloc(dev, size);

		if (pitch_lines)
			pitch_lines *= 2;
		else
			pitch_lines = 1;
		gtt_roll++;
	} while (backing == NULL && pitch_lines <= 16);

	/* The final pitch we accepted if we succeeded */
	pitch_lines /= 2;

	if (backing == NULL) {
		/*
		 *	We couldn't get the space we wanted, fall back to the
		 *	display engine requirement instead.  The HW requires
		 *	the pitch to be 64 byte aligned
		 */

		gtt_roll = 0;	/* Don't use GTT accelerated scrolling */
		pitch_lines = 64;

		mode_cmd.pitches[0] =  ALIGN(mode_cmd.width * ((bpp + 7) / 8), 64);

		size = mode_cmd.pitches[0] * mode_cmd.height;
		size = ALIGN(size, PAGE_SIZE);

		/* Allocate the framebuffer in the GTT with stolen page backing */
		backing = psbfb_alloc(dev, size);
		if (backing == NULL)
			return -ENOMEM;
	}

	memset(dev_priv->vram_addr + backing->offset, 0, size);

	info = drm_fb_helper_alloc_fbi(&fbdev->psb_fb_helper);
=======
	mode_cmd.pitches[0] = ALIGN(mode_cmd.width * DIV_ROUND_UP(bpp, 8), 64);

	size = mode_cmd.pitches[0] * mode_cmd.height;
	size = ALIGN(size, PAGE_SIZE);

	/* Allocate the framebuffer in the GTT with stolen page backing */
	backing = psbfb_alloc(dev, size);
	if (backing == NULL)
		return -ENOMEM;

	memset(dev_priv->vram_addr + backing->offset, 0, size);

	info = drm_fb_helper_alloc_fbi(fb_helper);
>>>>>>> upstream/android-13
	if (IS_ERR(info)) {
		ret = PTR_ERR(info);
		goto out;
	}
<<<<<<< HEAD
	info->par = fbdev;

	mode_cmd.pixel_format = drm_mode_legacy_fb_format(bpp, depth);

	ret = psb_framebuffer_init(dev, psbfb, &mode_cmd, backing);
	if (ret)
		goto out;

	fb = &psbfb->base;
	psbfb->fbdev = info;

	fbdev->psb_fb_helper.fb = fb;

	drm_fb_helper_fill_fix(info, fb->pitches[0], fb->format->depth);
	strcpy(info->fix.id, "psbdrmfb");

	info->flags = FBINFO_DEFAULT;
	if (dev_priv->ops->accel_2d && pitch_lines > 8)	/* 2D engine */
		info->fbops = &psbfb_ops;
	else if (gtt_roll) {	/* GTT rolling seems best */
		info->fbops = &psbfb_roll_ops;
		info->flags |= FBINFO_HWACCEL_YPAN;
	} else	/* Software */
		info->fbops = &psbfb_unaccel_ops;

	info->fix.smem_start = dev->mode_config.fb_base;
	info->fix.smem_len = size;
	info->fix.ywrapstep = gtt_roll;
=======

	mode_cmd.pixel_format = drm_mode_legacy_fb_format(bpp, depth);

	fb = psb_framebuffer_create(dev, &mode_cmd, &backing->gem);
	if (IS_ERR(fb)) {
		ret = PTR_ERR(fb);
		goto out;
	}

	fb_helper->fb = fb;

	info->fbops = &psbfb_unaccel_ops;

	info->fix.smem_start = dev->mode_config.fb_base;
	info->fix.smem_len = size;
	info->fix.ywrapstep = 0;
>>>>>>> upstream/android-13
	info->fix.ypanstep = 0;

	/* Accessed stolen memory directly */
	info->screen_base = dev_priv->vram_addr + backing->offset;
	info->screen_size = size;

	if (dev_priv->gtt.stolen_size) {
		info->apertures->ranges[0].base = dev->mode_config.fb_base;
		info->apertures->ranges[0].size = dev_priv->gtt.stolen_size;
	}

<<<<<<< HEAD
	drm_fb_helper_fill_var(info, &fbdev->psb_fb_helper,
				sizes->fb_width, sizes->fb_height);

	info->fix.mmio_start = pci_resource_start(dev->pdev, 0);
	info->fix.mmio_len = pci_resource_len(dev->pdev, 0);

	/* Use default scratch pixmap (info->pixmap.flags = FB_PIXMAP_SYSTEM) */

	dev_dbg(dev->dev, "allocated %dx%d fb\n",
					psbfb->base.width, psbfb->base.height);
=======
	drm_fb_helper_fill_info(info, fb_helper, sizes);

	info->fix.mmio_start = pci_resource_start(pdev, 0);
	info->fix.mmio_len = pci_resource_len(pdev, 0);

	/* Use default scratch pixmap (info->pixmap.flags = FB_PIXMAP_SYSTEM) */

	dev_dbg(dev->dev, "allocated %dx%d fb\n", fb->width, fb->height);
>>>>>>> upstream/android-13

	return 0;
out:
	psb_gtt_free_range(dev, backing);
	return ret;
}

/**
 *	psb_user_framebuffer_create	-	create framebuffer
 *	@dev: our DRM device
 *	@filp: client file
 *	@cmd: mode request
 *
 *	Create a new framebuffer backed by a userspace GEM object
 */
static struct drm_framebuffer *psb_user_framebuffer_create
			(struct drm_device *dev, struct drm_file *filp,
			 const struct drm_mode_fb_cmd2 *cmd)
{
<<<<<<< HEAD
	struct gtt_range *r;
	struct drm_gem_object *obj;
=======
	struct drm_gem_object *obj;
	struct drm_framebuffer *fb;
>>>>>>> upstream/android-13

	/*
	 *	Find the GEM object and thus the gtt range object that is
	 *	to back this space
	 */
	obj = drm_gem_object_lookup(filp, cmd->handles[0]);
	if (obj == NULL)
		return ERR_PTR(-ENOENT);

	/* Let the core code do all the work */
<<<<<<< HEAD
	r = container_of(obj, struct gtt_range, gem);
	return psb_framebuffer_create(dev, cmd, r);
}

static int psbfb_probe(struct drm_fb_helper *helper,
				struct drm_fb_helper_surface_size *sizes)
{
	struct psb_fbdev *psb_fbdev =
		container_of(helper, struct psb_fbdev, psb_fb_helper);
	struct drm_device *dev = psb_fbdev->psb_fb_helper.dev;
=======
	fb = psb_framebuffer_create(dev, cmd, obj);
	if (IS_ERR(fb))
		drm_gem_object_put(obj);

	return fb;
}

static int psbfb_probe(struct drm_fb_helper *fb_helper,
				struct drm_fb_helper_surface_size *sizes)
{
	struct drm_device *dev = fb_helper->dev;
>>>>>>> upstream/android-13
	struct drm_psb_private *dev_priv = dev->dev_private;
	unsigned int fb_size;
	int bytespp;

	bytespp = sizes->surface_bpp / 8;
	if (bytespp == 3)	/* no 24bit packed */
		bytespp = 4;

	/* If the mode will not fit in 32bit then switch to 16bit to get
	   a console on full resolution. The X mode setting server will
	   allocate its own 32bit GEM framebuffer */
	fb_size = ALIGN(sizes->surface_width * bytespp, 64) *
		  sizes->surface_height;
	fb_size = ALIGN(fb_size, PAGE_SIZE);

	if (fb_size > dev_priv->vram_stolen_size) {
                sizes->surface_bpp = 16;
                sizes->surface_depth = 16;
        }

<<<<<<< HEAD
	return psbfb_create(psb_fbdev, sizes);
=======
	return psbfb_create(fb_helper, sizes);
>>>>>>> upstream/android-13
}

static const struct drm_fb_helper_funcs psb_fb_helper_funcs = {
	.fb_probe = psbfb_probe,
};

<<<<<<< HEAD
static int psb_fbdev_destroy(struct drm_device *dev, struct psb_fbdev *fbdev)
{
	struct psb_framebuffer *psbfb = &fbdev->pfb;

	drm_fb_helper_unregister_fbi(&fbdev->psb_fb_helper);

	drm_fb_helper_fini(&fbdev->psb_fb_helper);
	drm_framebuffer_unregister_private(&psbfb->base);
	drm_framebuffer_cleanup(&psbfb->base);

	if (psbfb->base.obj[0])
		drm_gem_object_put_unlocked(psbfb->base.obj[0]);
=======
static int psb_fbdev_destroy(struct drm_device *dev,
			     struct drm_fb_helper *fb_helper)
{
	struct drm_framebuffer *fb = fb_helper->fb;

	drm_fb_helper_unregister_fbi(fb_helper);

	drm_fb_helper_fini(fb_helper);
	drm_framebuffer_unregister_private(fb);
	drm_framebuffer_cleanup(fb);

	if (fb->obj[0])
		drm_gem_object_put(fb->obj[0]);
	kfree(fb);

>>>>>>> upstream/android-13
	return 0;
}

int psb_fbdev_init(struct drm_device *dev)
{
<<<<<<< HEAD
	struct psb_fbdev *fbdev;
	struct drm_psb_private *dev_priv = dev->dev_private;
	int ret;

	fbdev = kzalloc(sizeof(struct psb_fbdev), GFP_KERNEL);
	if (!fbdev) {
=======
	struct drm_fb_helper *fb_helper;
	struct drm_psb_private *dev_priv = dev->dev_private;
	int ret;

	fb_helper = kzalloc(sizeof(*fb_helper), GFP_KERNEL);
	if (!fb_helper) {
>>>>>>> upstream/android-13
		dev_err(dev->dev, "no memory\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	dev_priv->fbdev = fbdev;

	drm_fb_helper_prepare(dev, &fbdev->psb_fb_helper, &psb_fb_helper_funcs);

	ret = drm_fb_helper_init(dev, &fbdev->psb_fb_helper,
				 INTELFB_CONN_LIMIT);
	if (ret)
		goto free;

	ret = drm_fb_helper_single_add_all_connectors(&fbdev->psb_fb_helper);
	if (ret)
		goto fini;

	/* disable all the possible outputs/crtcs before entering KMS mode */
	drm_helper_disable_unused_functions(dev);

	ret = drm_fb_helper_initial_config(&fbdev->psb_fb_helper, 32);
=======
	dev_priv->fb_helper = fb_helper;

	drm_fb_helper_prepare(dev, fb_helper, &psb_fb_helper_funcs);

	ret = drm_fb_helper_init(dev, fb_helper);
	if (ret)
		goto free;

	/* disable all the possible outputs/crtcs before entering KMS mode */
	drm_helper_disable_unused_functions(dev);

	ret = drm_fb_helper_initial_config(fb_helper, 32);
>>>>>>> upstream/android-13
	if (ret)
		goto fini;

	return 0;

fini:
<<<<<<< HEAD
	drm_fb_helper_fini(&fbdev->psb_fb_helper);
free:
	kfree(fbdev);
=======
	drm_fb_helper_fini(fb_helper);
free:
	kfree(fb_helper);
>>>>>>> upstream/android-13
	return ret;
}

static void psb_fbdev_fini(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;

<<<<<<< HEAD
	if (!dev_priv->fbdev)
		return;

	psb_fbdev_destroy(dev, dev_priv->fbdev);
	kfree(dev_priv->fbdev);
	dev_priv->fbdev = NULL;
=======
	if (!dev_priv->fb_helper)
		return;

	psb_fbdev_destroy(dev, dev_priv->fb_helper);
	kfree(dev_priv->fb_helper);
	dev_priv->fb_helper = NULL;
>>>>>>> upstream/android-13
}

static const struct drm_mode_config_funcs psb_mode_funcs = {
	.fb_create = psb_user_framebuffer_create,
	.output_poll_changed = drm_fb_helper_output_poll_changed,
};

static void psb_setup_outputs(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct drm_connector *connector;

	drm_mode_create_scaling_mode_property(dev);

	/* It is ok for this to fail - we just don't get backlight control */
	if (!dev_priv->backlight_property)
		dev_priv->backlight_property = drm_property_create_range(dev, 0,
							"backlight", 0, 100);
	dev_priv->ops->output_init(dev);

	list_for_each_entry(connector, &dev->mode_config.connector_list,
			    head) {
		struct gma_encoder *gma_encoder = gma_attached_encoder(connector);
		struct drm_encoder *encoder = &gma_encoder->base;
		int crtc_mask = 0, clone_mask = 0;

		/* valid crtcs */
		switch (gma_encoder->type) {
		case INTEL_OUTPUT_ANALOG:
			crtc_mask = (1 << 0);
			clone_mask = (1 << INTEL_OUTPUT_ANALOG);
			break;
		case INTEL_OUTPUT_SDVO:
			crtc_mask = dev_priv->ops->sdvo_mask;
<<<<<<< HEAD
			clone_mask = (1 << INTEL_OUTPUT_SDVO);
			break;
		case INTEL_OUTPUT_LVDS:
		        crtc_mask = dev_priv->ops->lvds_mask;
			clone_mask = (1 << INTEL_OUTPUT_LVDS);
			break;
		case INTEL_OUTPUT_MIPI:
			crtc_mask = (1 << 0);
			clone_mask = (1 << INTEL_OUTPUT_MIPI);
			break;
		case INTEL_OUTPUT_MIPI2:
			crtc_mask = (1 << 2);
			clone_mask = (1 << INTEL_OUTPUT_MIPI2);
			break;
		case INTEL_OUTPUT_HDMI:
		        crtc_mask = dev_priv->ops->hdmi_mask;
=======
			clone_mask = 0;
			break;
		case INTEL_OUTPUT_LVDS:
			crtc_mask = dev_priv->ops->lvds_mask;
			clone_mask = 0;
			break;
		case INTEL_OUTPUT_MIPI:
			crtc_mask = (1 << 0);
			clone_mask = 0;
			break;
		case INTEL_OUTPUT_MIPI2:
			crtc_mask = (1 << 2);
			clone_mask = 0;
			break;
		case INTEL_OUTPUT_HDMI:
			crtc_mask = dev_priv->ops->hdmi_mask;
>>>>>>> upstream/android-13
			clone_mask = (1 << INTEL_OUTPUT_HDMI);
			break;
		case INTEL_OUTPUT_DISPLAYPORT:
			crtc_mask = (1 << 0) | (1 << 1);
<<<<<<< HEAD
			clone_mask = (1 << INTEL_OUTPUT_DISPLAYPORT);
			break;
		case INTEL_OUTPUT_EDP:
			crtc_mask = (1 << 1);
			clone_mask = (1 << INTEL_OUTPUT_EDP);
=======
			clone_mask = 0;
			break;
		case INTEL_OUTPUT_EDP:
			crtc_mask = (1 << 1);
			clone_mask = 0;
>>>>>>> upstream/android-13
		}
		encoder->possible_crtcs = crtc_mask;
		encoder->possible_clones =
		    gma_connector_clones(dev, clone_mask);
	}
}

void psb_modeset_init(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct psb_intel_mode_device *mode_dev = &dev_priv->mode_dev;
<<<<<<< HEAD
=======
	struct pci_dev *pdev = to_pci_dev(dev->dev);
>>>>>>> upstream/android-13
	int i;

	drm_mode_config_init(dev);

	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;

	dev->mode_config.funcs = &psb_mode_funcs;

	/* set memory base */
	/* Oaktrail and Poulsbo should use BAR 2*/
<<<<<<< HEAD
	pci_read_config_dword(dev->pdev, PSB_BSM, (u32 *)
					&(dev->mode_config.fb_base));
=======
	pci_read_config_dword(pdev, PSB_BSM, (u32 *)&(dev->mode_config.fb_base));
>>>>>>> upstream/android-13

	/* num pipes is 2 for PSB but 1 for Mrst */
	for (i = 0; i < dev_priv->num_pipe; i++)
		psb_intel_crtc_init(dev, i, mode_dev);

	dev->mode_config.max_width = 4096;
	dev->mode_config.max_height = 4096;

	psb_setup_outputs(dev);

	if (dev_priv->ops->errata)
	        dev_priv->ops->errata(dev);

        dev_priv->modeset = true;
}

void psb_modeset_cleanup(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	if (dev_priv->modeset) {
		drm_kms_helper_poll_fini(dev);
		psb_fbdev_fini(dev);
		drm_mode_config_cleanup(dev);
	}
}
