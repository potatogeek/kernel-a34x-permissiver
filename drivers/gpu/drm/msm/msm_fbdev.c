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

#include <drm/drm_crtc.h>
#include <drm/drm_fb_helper.h>

#include "msm_drv.h"
#include "msm_kms.h"

extern int msm_gem_mmap_obj(struct drm_gem_object *obj,
					struct vm_area_struct *vma);
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 */

#include <drm/drm_aperture.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_prime.h>

#include "msm_drv.h"
#include "msm_gem.h"
#include "msm_kms.h"

>>>>>>> upstream/android-13
static int msm_fbdev_mmap(struct fb_info *info, struct vm_area_struct *vma);

/*
 * fbdev funcs, to implement legacy fbdev interface on top of drm driver
 */

#define to_msm_fbdev(x) container_of(x, struct msm_fbdev, base)

struct msm_fbdev {
	struct drm_fb_helper base;
	struct drm_framebuffer *fb;
};

<<<<<<< HEAD
static struct fb_ops msm_fb_ops = {
=======
static const struct fb_ops msm_fb_ops = {
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
	DRM_FB_HELPER_DEFAULT_OPS,

	/* Note: to properly handle manual update displays, we wrap the
	 * basic fbdev ops which write to the framebuffer
	 */
	.fb_read = drm_fb_helper_sys_read,
	.fb_write = drm_fb_helper_sys_write,
	.fb_fillrect = drm_fb_helper_sys_fillrect,
	.fb_copyarea = drm_fb_helper_sys_copyarea,
	.fb_imageblit = drm_fb_helper_sys_imageblit,
	.fb_mmap = msm_fbdev_mmap,
};

static int msm_fbdev_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	struct drm_fb_helper *helper = (struct drm_fb_helper *)info->par;
	struct msm_fbdev *fbdev = to_msm_fbdev(helper);
	struct drm_gem_object *bo = msm_framebuffer_bo(fbdev->fb, 0);
<<<<<<< HEAD
	int ret = 0;

	ret = drm_gem_mmap_obj(bo, bo->size, vma);
	if (ret) {
		pr_err("%s:drm_gem_mmap_obj fail\n", __func__);
		return ret;
	}

	return msm_gem_mmap_obj(bo, vma);
=======

	return drm_gem_prime_mmap(bo, vma);
>>>>>>> upstream/android-13
}

static int msm_fbdev_create(struct drm_fb_helper *helper,
		struct drm_fb_helper_surface_size *sizes)
{
	struct msm_fbdev *fbdev = to_msm_fbdev(helper);
	struct drm_device *dev = helper->dev;
	struct msm_drm_private *priv = dev->dev_private;
	struct drm_framebuffer *fb = NULL;
	struct drm_gem_object *bo;
	struct fb_info *fbi = NULL;
	uint64_t paddr;
	uint32_t format;
	int ret, pitch;

	format = drm_mode_legacy_fb_format(sizes->surface_bpp, sizes->surface_depth);

	DBG("create fbdev: %dx%d@%d (%dx%d)", sizes->surface_width,
			sizes->surface_height, sizes->surface_bpp,
			sizes->fb_width, sizes->fb_height);

	pitch = align_pitch(sizes->surface_width, sizes->surface_bpp);
	fb = msm_alloc_stolen_fb(dev, sizes->surface_width,
			sizes->surface_height, pitch, format);

	if (IS_ERR(fb)) {
<<<<<<< HEAD
		dev_err(dev->dev, "failed to allocate fb\n");
=======
		DRM_DEV_ERROR(dev->dev, "failed to allocate fb\n");
>>>>>>> upstream/android-13
		return PTR_ERR(fb);
	}

	bo = msm_framebuffer_bo(fb, 0);

	mutex_lock(&dev->struct_mutex);

	/*
	 * NOTE: if we can be guaranteed to be able to map buffer
	 * in panic (ie. lock-safe, etc) we could avoid pinning the
	 * buffer now:
	 */
<<<<<<< HEAD
	ret = msm_gem_get_iova(bo, priv->kms->aspace, &paddr);
	if (ret) {
		dev_err(dev->dev, "failed to get buffer obj iova: %d\n", ret);
=======
	ret = msm_gem_get_and_pin_iova(bo, priv->kms->aspace, &paddr);
	if (ret) {
		DRM_DEV_ERROR(dev->dev, "failed to get buffer obj iova: %d\n", ret);
>>>>>>> upstream/android-13
		goto fail_unlock;
	}

	fbi = drm_fb_helper_alloc_fbi(helper);
	if (IS_ERR(fbi)) {
<<<<<<< HEAD
		dev_err(dev->dev, "failed to allocate fb info\n");
=======
		DRM_DEV_ERROR(dev->dev, "failed to allocate fb info\n");
>>>>>>> upstream/android-13
		ret = PTR_ERR(fbi);
		goto fail_unlock;
	}

	DBG("fbi=%p, dev=%p", fbi, dev);

	fbdev->fb = fb;
	helper->fb = fb;

<<<<<<< HEAD
	fbi->par = helper;
	fbi->fbops = &msm_fb_ops;

	strcpy(fbi->fix.id, "msm");

	drm_fb_helper_fill_fix(fbi, fb->pitches[0], fb->format->depth);
	drm_fb_helper_fill_var(fbi, helper, sizes->fb_width, sizes->fb_height);
=======
	fbi->fbops = &msm_fb_ops;

	drm_fb_helper_fill_info(fbi, helper, sizes);
>>>>>>> upstream/android-13

	dev->mode_config.fb_base = paddr;

	fbi->screen_base = msm_gem_get_vaddr(bo);
	if (IS_ERR(fbi->screen_base)) {
		ret = PTR_ERR(fbi->screen_base);
		goto fail_unlock;
	}
	fbi->screen_size = bo->size;
	fbi->fix.smem_start = paddr;
	fbi->fix.smem_len = bo->size;

	DBG("par=%p, %dx%d", fbi->par, fbi->var.xres, fbi->var.yres);
	DBG("allocated %dx%d fb", fbdev->fb->width, fbdev->fb->height);

	mutex_unlock(&dev->struct_mutex);

	return 0;

fail_unlock:
	mutex_unlock(&dev->struct_mutex);
	drm_framebuffer_remove(fb);
	return ret;
}

static const struct drm_fb_helper_funcs msm_fb_helper_funcs = {
	.fb_probe = msm_fbdev_create,
};

/* initialize fbdev helper */
struct drm_fb_helper *msm_fbdev_init(struct drm_device *dev)
{
	struct msm_drm_private *priv = dev->dev_private;
	struct msm_fbdev *fbdev = NULL;
	struct drm_fb_helper *helper;
	int ret;

	fbdev = kzalloc(sizeof(*fbdev), GFP_KERNEL);
	if (!fbdev)
		goto fail;

	helper = &fbdev->base;

	drm_fb_helper_prepare(dev, helper, &msm_fb_helper_funcs);

<<<<<<< HEAD
	ret = drm_fb_helper_init(dev, helper, priv->num_connectors);
	if (ret) {
		dev_err(dev->dev, "could not init fbdev: ret=%d\n", ret);
		goto fail;
	}

	ret = drm_fb_helper_single_add_all_connectors(helper);
=======
	ret = drm_fb_helper_init(dev, helper);
	if (ret) {
		DRM_DEV_ERROR(dev->dev, "could not init fbdev: ret=%d\n", ret);
		goto fail;
	}

	/* the fw fb could be anywhere in memory */
	ret = drm_aperture_remove_framebuffers(false, dev->driver);
>>>>>>> upstream/android-13
	if (ret)
		goto fini;

	ret = drm_fb_helper_initial_config(helper, 32);
	if (ret)
		goto fini;

	priv->fbdev = helper;

	return helper;

fini:
	drm_fb_helper_fini(helper);
fail:
	kfree(fbdev);
	return NULL;
}

void msm_fbdev_free(struct drm_device *dev)
{
	struct msm_drm_private *priv = dev->dev_private;
	struct drm_fb_helper *helper = priv->fbdev;
	struct msm_fbdev *fbdev;

	DBG();

	drm_fb_helper_unregister_fbi(helper);

	drm_fb_helper_fini(helper);

	fbdev = to_msm_fbdev(priv->fbdev);

	/* this will free the backing object */
	if (fbdev->fb) {
		struct drm_gem_object *bo =
			msm_framebuffer_bo(fbdev->fb, 0);
		msm_gem_put_vaddr(bo);
		drm_framebuffer_remove(fbdev->fb);
	}

	kfree(fbdev);

	priv->fbdev = NULL;
}
