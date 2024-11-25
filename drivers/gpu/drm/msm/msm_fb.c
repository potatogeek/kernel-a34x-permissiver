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
#include <drm/drm_crtc_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 */

#include <drm/drm_crtc.h>
#include <drm/drm_damage_helper.h>
#include <drm/drm_file.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_probe_helper.h>
>>>>>>> upstream/android-13

#include "msm_drv.h"
#include "msm_kms.h"
#include "msm_gem.h"

struct msm_framebuffer {
	struct drm_framebuffer base;
	const struct msm_format *format;
};
#define to_msm_framebuffer(x) container_of(x, struct msm_framebuffer, base)

static struct drm_framebuffer *msm_framebuffer_init(struct drm_device *dev,
		const struct drm_mode_fb_cmd2 *mode_cmd, struct drm_gem_object **bos);

static const struct drm_framebuffer_funcs msm_framebuffer_funcs = {
	.create_handle = drm_gem_fb_create_handle,
	.destroy = drm_gem_fb_destroy,
<<<<<<< HEAD
=======
	.dirty = drm_atomic_helper_dirtyfb,
>>>>>>> upstream/android-13
};

#ifdef CONFIG_DEBUG_FS
void msm_framebuffer_describe(struct drm_framebuffer *fb, struct seq_file *m)
{
<<<<<<< HEAD
=======
	struct msm_gem_stats stats = {};
>>>>>>> upstream/android-13
	int i, n = fb->format->num_planes;

	seq_printf(m, "fb: %dx%d@%4.4s (%2d, ID:%d)\n",
			fb->width, fb->height, (char *)&fb->format->format,
			drm_framebuffer_read_refcount(fb), fb->base.id);

	for (i = 0; i < n; i++) {
		seq_printf(m, "   %d: offset=%d pitch=%d, obj: ",
				i, fb->offsets[i], fb->pitches[i]);
<<<<<<< HEAD
		msm_gem_describe(fb->obj[i], m);
=======
		msm_gem_describe(fb->obj[i], m, &stats);
>>>>>>> upstream/android-13
	}
}
#endif

/* prepare/pin all the fb's bo's for scanout.  Note that it is not valid
 * to prepare an fb more multiple different initiator 'id's.  But that
 * should be fine, since only the scanout (mdpN) side of things needs
 * this, the gpu doesn't care about fb's.
 */
int msm_framebuffer_prepare(struct drm_framebuffer *fb,
		struct msm_gem_address_space *aspace)
{
	int ret, i, n = fb->format->num_planes;
	uint64_t iova;

	for (i = 0; i < n; i++) {
<<<<<<< HEAD
		ret = msm_gem_get_iova(fb->obj[i], aspace, &iova);
		DBG("FB[%u]: iova[%d]: %08llx (%d)", fb->base.id, i, iova, ret);
=======
		ret = msm_gem_get_and_pin_iova(fb->obj[i], aspace, &iova);
		drm_dbg_state(fb->dev, "FB[%u]: iova[%d]: %08llx (%d)", fb->base.id, i, iova, ret);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

	return 0;
}

void msm_framebuffer_cleanup(struct drm_framebuffer *fb,
		struct msm_gem_address_space *aspace)
{
	int i, n = fb->format->num_planes;

	for (i = 0; i < n; i++)
<<<<<<< HEAD
		msm_gem_put_iova(fb->obj[i], aspace);
=======
		msm_gem_unpin_iova(fb->obj[i], aspace);
>>>>>>> upstream/android-13
}

uint32_t msm_framebuffer_iova(struct drm_framebuffer *fb,
		struct msm_gem_address_space *aspace, int plane)
{
	if (!fb->obj[plane])
		return 0;
	return msm_gem_iova(fb->obj[plane], aspace) + fb->offsets[plane];
}

struct drm_gem_object *msm_framebuffer_bo(struct drm_framebuffer *fb, int plane)
{
	return drm_gem_fb_get_obj(fb, plane);
}

const struct msm_format *msm_framebuffer_format(struct drm_framebuffer *fb)
{
	struct msm_framebuffer *msm_fb = to_msm_framebuffer(fb);
	return msm_fb->format;
}

struct drm_framebuffer *msm_framebuffer_create(struct drm_device *dev,
		struct drm_file *file, const struct drm_mode_fb_cmd2 *mode_cmd)
{
<<<<<<< HEAD
	struct drm_gem_object *bos[4] = {0};
	struct drm_framebuffer *fb;
	int ret, i, n = drm_format_num_planes(mode_cmd->pixel_format);
=======
	const struct drm_format_info *info = drm_get_format_info(dev,
								 mode_cmd);
	struct drm_gem_object *bos[4] = {0};
	struct drm_framebuffer *fb;
	int ret, i, n = info->num_planes;
>>>>>>> upstream/android-13

	for (i = 0; i < n; i++) {
		bos[i] = drm_gem_object_lookup(file, mode_cmd->handles[i]);
		if (!bos[i]) {
			ret = -ENXIO;
			goto out_unref;
		}
	}

	fb = msm_framebuffer_init(dev, mode_cmd, bos);
	if (IS_ERR(fb)) {
		ret = PTR_ERR(fb);
		goto out_unref;
	}

	return fb;

out_unref:
	for (i = 0; i < n; i++)
<<<<<<< HEAD
		drm_gem_object_put_unlocked(bos[i]);
=======
		drm_gem_object_put(bos[i]);
>>>>>>> upstream/android-13
	return ERR_PTR(ret);
}

static struct drm_framebuffer *msm_framebuffer_init(struct drm_device *dev,
		const struct drm_mode_fb_cmd2 *mode_cmd, struct drm_gem_object **bos)
{
<<<<<<< HEAD
=======
	const struct drm_format_info *info = drm_get_format_info(dev,
								 mode_cmd);
>>>>>>> upstream/android-13
	struct msm_drm_private *priv = dev->dev_private;
	struct msm_kms *kms = priv->kms;
	struct msm_framebuffer *msm_fb = NULL;
	struct drm_framebuffer *fb;
	const struct msm_format *format;
	int ret, i, n;
<<<<<<< HEAD
	unsigned int hsub, vsub;

	DBG("create framebuffer: dev=%p, mode_cmd=%p (%dx%d@%4.4s)",
			dev, mode_cmd, mode_cmd->width, mode_cmd->height,
			(char *)&mode_cmd->pixel_format);

	n = drm_format_num_planes(mode_cmd->pixel_format);
	hsub = drm_format_horz_chroma_subsampling(mode_cmd->pixel_format);
	vsub = drm_format_vert_chroma_subsampling(mode_cmd->pixel_format);

	format = kms->funcs->get_format(kms, mode_cmd->pixel_format,
			mode_cmd->modifier[0]);
	if (!format) {
		dev_err(dev->dev, "unsupported pixel format: %4.4s\n",
=======

	drm_dbg_state(dev, "create framebuffer: mode_cmd=%p (%dx%d@%4.4s)",
			mode_cmd, mode_cmd->width, mode_cmd->height,
			(char *)&mode_cmd->pixel_format);

	n = info->num_planes;
	format = kms->funcs->get_format(kms, mode_cmd->pixel_format,
			mode_cmd->modifier[0]);
	if (!format) {
		DRM_DEV_ERROR(dev->dev, "unsupported pixel format: %4.4s\n",
>>>>>>> upstream/android-13
				(char *)&mode_cmd->pixel_format);
		ret = -EINVAL;
		goto fail;
	}

	msm_fb = kzalloc(sizeof(*msm_fb), GFP_KERNEL);
	if (!msm_fb) {
		ret = -ENOMEM;
		goto fail;
	}

	fb = &msm_fb->base;

	msm_fb->format = format;

	if (n > ARRAY_SIZE(fb->obj)) {
		ret = -EINVAL;
		goto fail;
	}

	for (i = 0; i < n; i++) {
<<<<<<< HEAD
		unsigned int width = mode_cmd->width / (i ? hsub : 1);
		unsigned int height = mode_cmd->height / (i ? vsub : 1);
		unsigned int min_size;

		min_size = (height - 1) * mode_cmd->pitches[i]
			 + width * drm_format_plane_cpp(mode_cmd->pixel_format, i)
=======
		unsigned int width = mode_cmd->width / (i ? info->hsub : 1);
		unsigned int height = mode_cmd->height / (i ? info->vsub : 1);
		unsigned int min_size;

		min_size = (height - 1) * mode_cmd->pitches[i]
			 + width * info->cpp[i]
>>>>>>> upstream/android-13
			 + mode_cmd->offsets[i];

		if (bos[i]->size < min_size) {
			ret = -EINVAL;
			goto fail;
		}

		msm_fb->base.obj[i] = bos[i];
	}

	drm_helper_mode_fill_fb_struct(dev, fb, mode_cmd);

	ret = drm_framebuffer_init(dev, fb, &msm_framebuffer_funcs);
	if (ret) {
<<<<<<< HEAD
		dev_err(dev->dev, "framebuffer init failed: %d\n", ret);
		goto fail;
	}

	DBG("create: FB ID: %d (%p)", fb->base.id, fb);
=======
		DRM_DEV_ERROR(dev->dev, "framebuffer init failed: %d\n", ret);
		goto fail;
	}

	drm_dbg_state(dev, "create: FB ID: %d (%p)", fb->base.id, fb);
>>>>>>> upstream/android-13

	return fb;

fail:
	kfree(msm_fb);

	return ERR_PTR(ret);
}

struct drm_framebuffer *
msm_alloc_stolen_fb(struct drm_device *dev, int w, int h, int p, uint32_t format)
{
	struct drm_mode_fb_cmd2 mode_cmd = {
		.pixel_format = format,
		.width = w,
		.height = h,
		.pitches = { p },
	};
	struct drm_gem_object *bo;
	struct drm_framebuffer *fb;
	int size;

	/* allocate backing bo */
	size = mode_cmd.pitches[0] * mode_cmd.height;
	DBG("allocating %d bytes for fb %d", size, dev->primary->index);
	bo = msm_gem_new(dev, size, MSM_BO_SCANOUT | MSM_BO_WC | MSM_BO_STOLEN);
	if (IS_ERR(bo)) {
		dev_warn(dev->dev, "could not allocate stolen bo\n");
		/* try regular bo: */
		bo = msm_gem_new(dev, size, MSM_BO_SCANOUT | MSM_BO_WC);
	}
	if (IS_ERR(bo)) {
<<<<<<< HEAD
		dev_err(dev->dev, "failed to allocate buffer object\n");
		return ERR_CAST(bo);
	}

	fb = msm_framebuffer_init(dev, &mode_cmd, &bo);
	if (IS_ERR(fb)) {
		dev_err(dev->dev, "failed to allocate fb\n");
		/* note: if fb creation failed, we can't rely on fb destroy
		 * to unref the bo:
		 */
		drm_gem_object_put_unlocked(bo);
=======
		DRM_DEV_ERROR(dev->dev, "failed to allocate buffer object\n");
		return ERR_CAST(bo);
	}

	msm_gem_object_set_name(bo, "stolenfb");

	fb = msm_framebuffer_init(dev, &mode_cmd, &bo);
	if (IS_ERR(fb)) {
		DRM_DEV_ERROR(dev->dev, "failed to allocate fb\n");
		/* note: if fb creation failed, we can't rely on fb destroy
		 * to unref the bo:
		 */
		drm_gem_object_put(bo);
>>>>>>> upstream/android-13
		return ERR_CAST(fb);
	}

	return fb;
}
