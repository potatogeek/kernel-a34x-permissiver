<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * rcar_du_vsp.h  --  R-Car Display Unit VSP-Based Compositor
 *
 * Copyright (C) 2015 Renesas Electronics Corporation
 *
 * Contact: Laurent Pinchart (laurent.pinchart@ideasonboard.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <drm/drmP.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_plane_helper.h>
=======
 */

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_vblank.h>
>>>>>>> upstream/android-13

#include <linux/bitops.h>
#include <linux/dma-mapping.h>
#include <linux/of_platform.h>
#include <linux/scatterlist.h>
<<<<<<< HEAD
=======
#include <linux/slab.h>
>>>>>>> upstream/android-13
#include <linux/videodev2.h>

#include <media/vsp1.h>

#include "rcar_du_drv.h"
#include "rcar_du_kms.h"
#include "rcar_du_vsp.h"
<<<<<<< HEAD

static void rcar_du_vsp_complete(void *private, bool completed, u32 crc)
=======
#include "rcar_du_writeback.h"

static void rcar_du_vsp_complete(void *private, unsigned int status, u32 crc)
>>>>>>> upstream/android-13
{
	struct rcar_du_crtc *crtc = private;

	if (crtc->vblank_enable)
		drm_crtc_handle_vblank(&crtc->crtc);

<<<<<<< HEAD
	if (completed)
		rcar_du_crtc_finish_page_flip(crtc);
=======
	if (status & VSP1_DU_STATUS_COMPLETE)
		rcar_du_crtc_finish_page_flip(crtc);
	if (status & VSP1_DU_STATUS_WRITEBACK)
		rcar_du_writeback_complete(crtc);
>>>>>>> upstream/android-13

	drm_crtc_add_crc_entry(&crtc->crtc, false, 0, &crc);
}

void rcar_du_vsp_enable(struct rcar_du_crtc *crtc)
{
	const struct drm_display_mode *mode = &crtc->crtc.state->adjusted_mode;
<<<<<<< HEAD
	struct rcar_du_device *rcdu = crtc->group->dev;
	struct vsp1_du_lif_config cfg = {
		.width = mode->hdisplay,
		.height = mode->vdisplay,
=======
	struct rcar_du_device *rcdu = crtc->dev;
	struct vsp1_du_lif_config cfg = {
		.width = mode->hdisplay,
		.height = mode->vdisplay,
		.interlaced = mode->flags & DRM_MODE_FLAG_INTERLACE,
>>>>>>> upstream/android-13
		.callback = rcar_du_vsp_complete,
		.callback_data = crtc,
	};
	struct rcar_du_plane_state state = {
		.state = {
			.alpha = DRM_BLEND_ALPHA_OPAQUE,
			.crtc = &crtc->crtc,
			.dst.x1 = 0,
			.dst.y1 = 0,
			.dst.x2 = mode->hdisplay,
			.dst.y2 = mode->vdisplay,
			.src.x1 = 0,
			.src.y1 = 0,
			.src.x2 = mode->hdisplay << 16,
			.src.y2 = mode->vdisplay << 16,
			.zpos = 0,
		},
		.format = rcar_du_format_info(DRM_FORMAT_ARGB8888),
		.source = RCAR_DU_PLANE_VSPD1,
		.colorkey = 0,
	};

	if (rcdu->info->gen >= 3)
		state.hwindex = (crtc->index % 2) ? 2 : 0;
	else
		state.hwindex = crtc->index % 2;

	__rcar_du_plane_setup(crtc->group, &state);

	/*
	 * Ensure that the plane source configuration takes effect by requesting
	 * a restart of the group. See rcar_du_plane_atomic_update() for a more
	 * detailed explanation.
	 *
	 * TODO: Check whether this is still needed on Gen3.
	 */
	crtc->group->need_restart = true;

	vsp1_du_setup_lif(crtc->vsp->vsp, crtc->vsp_pipe, &cfg);
}

void rcar_du_vsp_disable(struct rcar_du_crtc *crtc)
{
	vsp1_du_setup_lif(crtc->vsp->vsp, crtc->vsp_pipe, NULL);
}

void rcar_du_vsp_atomic_begin(struct rcar_du_crtc *crtc)
{
	vsp1_du_atomic_begin(crtc->vsp->vsp, crtc->vsp_pipe);
}

void rcar_du_vsp_atomic_flush(struct rcar_du_crtc *crtc)
{
	struct vsp1_du_atomic_pipe_config cfg = { { 0, } };
	struct rcar_du_crtc_state *state;

	state = to_rcar_crtc_state(crtc->crtc.state);
	cfg.crc = state->crc;

<<<<<<< HEAD
	vsp1_du_atomic_flush(crtc->vsp->vsp, crtc->vsp_pipe, &cfg);
}

/* Keep the two tables in sync. */
static const u32 formats_kms[] = {
=======
	rcar_du_writeback_setup(crtc, &cfg.writeback);

	vsp1_du_atomic_flush(crtc->vsp->vsp, crtc->vsp_pipe, &cfg);
}

static const u32 rcar_du_vsp_formats[] = {
>>>>>>> upstream/android-13
	DRM_FORMAT_RGB332,
	DRM_FORMAT_ARGB4444,
	DRM_FORMAT_XRGB4444,
	DRM_FORMAT_ARGB1555,
	DRM_FORMAT_XRGB1555,
	DRM_FORMAT_RGB565,
	DRM_FORMAT_BGR888,
	DRM_FORMAT_RGB888,
	DRM_FORMAT_BGRA8888,
	DRM_FORMAT_BGRX8888,
	DRM_FORMAT_ARGB8888,
	DRM_FORMAT_XRGB8888,
	DRM_FORMAT_UYVY,
<<<<<<< HEAD
	DRM_FORMAT_VYUY,
=======
>>>>>>> upstream/android-13
	DRM_FORMAT_YUYV,
	DRM_FORMAT_YVYU,
	DRM_FORMAT_NV12,
	DRM_FORMAT_NV21,
	DRM_FORMAT_NV16,
	DRM_FORMAT_NV61,
	DRM_FORMAT_YUV420,
	DRM_FORMAT_YVU420,
	DRM_FORMAT_YUV422,
	DRM_FORMAT_YVU422,
	DRM_FORMAT_YUV444,
	DRM_FORMAT_YVU444,
};

<<<<<<< HEAD
static const u32 formats_v4l2[] = {
	V4L2_PIX_FMT_RGB332,
	V4L2_PIX_FMT_ARGB444,
	V4L2_PIX_FMT_XRGB444,
	V4L2_PIX_FMT_ARGB555,
	V4L2_PIX_FMT_XRGB555,
	V4L2_PIX_FMT_RGB565,
	V4L2_PIX_FMT_RGB24,
	V4L2_PIX_FMT_BGR24,
	V4L2_PIX_FMT_ARGB32,
	V4L2_PIX_FMT_XRGB32,
	V4L2_PIX_FMT_ABGR32,
	V4L2_PIX_FMT_XBGR32,
	V4L2_PIX_FMT_UYVY,
	V4L2_PIX_FMT_VYUY,
	V4L2_PIX_FMT_YUYV,
	V4L2_PIX_FMT_YVYU,
	V4L2_PIX_FMT_NV12M,
	V4L2_PIX_FMT_NV21M,
	V4L2_PIX_FMT_NV16M,
	V4L2_PIX_FMT_NV61M,
	V4L2_PIX_FMT_YUV420M,
	V4L2_PIX_FMT_YVU420M,
	V4L2_PIX_FMT_YUV422M,
	V4L2_PIX_FMT_YVU422M,
	V4L2_PIX_FMT_YUV444M,
	V4L2_PIX_FMT_YVU444M,
};

=======
>>>>>>> upstream/android-13
static void rcar_du_vsp_plane_setup(struct rcar_du_vsp_plane *plane)
{
	struct rcar_du_vsp_plane_state *state =
		to_rcar_vsp_plane_state(plane->plane.state);
	struct rcar_du_crtc *crtc = to_rcar_crtc(state->state.crtc);
	struct drm_framebuffer *fb = plane->plane.state->fb;
<<<<<<< HEAD
=======
	const struct rcar_du_format_info *format;
>>>>>>> upstream/android-13
	struct vsp1_du_atomic_config cfg = {
		.pixelformat = 0,
		.pitch = fb->pitches[0],
		.alpha = state->state.alpha >> 8,
		.zpos = state->state.zpos,
	};
	unsigned int i;

	cfg.src.left = state->state.src.x1 >> 16;
	cfg.src.top = state->state.src.y1 >> 16;
	cfg.src.width = drm_rect_width(&state->state.src) >> 16;
	cfg.src.height = drm_rect_height(&state->state.src) >> 16;

	cfg.dst.left = state->state.dst.x1;
	cfg.dst.top = state->state.dst.y1;
	cfg.dst.width = drm_rect_width(&state->state.dst);
	cfg.dst.height = drm_rect_height(&state->state.dst);

	for (i = 0; i < state->format->planes; ++i)
		cfg.mem[i] = sg_dma_address(state->sg_tables[i].sgl)
			   + fb->offsets[i];

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(formats_kms); ++i) {
		if (formats_kms[i] == state->format->fourcc) {
			cfg.pixelformat = formats_v4l2[i];
			break;
		}
	}
=======
	format = rcar_du_format_info(state->format->fourcc);
	cfg.pixelformat = format->v4l2;
>>>>>>> upstream/android-13

	vsp1_du_atomic_update(plane->vsp->vsp, crtc->vsp_pipe,
			      plane->index, &cfg);
}

<<<<<<< HEAD
=======
int rcar_du_vsp_map_fb(struct rcar_du_vsp *vsp, struct drm_framebuffer *fb,
		       struct sg_table sg_tables[3])
{
	struct rcar_du_device *rcdu = vsp->dev;
	unsigned int i;
	int ret;

	for (i = 0; i < fb->format->num_planes; ++i) {
		struct drm_gem_cma_object *gem = drm_fb_cma_get_gem_obj(fb, i);
		struct sg_table *sgt = &sg_tables[i];

		ret = dma_get_sgtable(rcdu->dev, sgt, gem->vaddr, gem->paddr,
				      gem->base.size);
		if (ret)
			goto fail;

		ret = vsp1_du_map_sg(vsp->vsp, sgt);
		if (ret) {
			sg_free_table(sgt);
			goto fail;
		}
	}

	return 0;

fail:
	while (i--) {
		struct sg_table *sgt = &sg_tables[i];

		vsp1_du_unmap_sg(vsp->vsp, sgt);
		sg_free_table(sgt);
	}

	return ret;
}

>>>>>>> upstream/android-13
static int rcar_du_vsp_plane_prepare_fb(struct drm_plane *plane,
					struct drm_plane_state *state)
{
	struct rcar_du_vsp_plane_state *rstate = to_rcar_vsp_plane_state(state);
	struct rcar_du_vsp *vsp = to_rcar_vsp_plane(plane)->vsp;
<<<<<<< HEAD
	struct rcar_du_device *rcdu = vsp->dev;
	unsigned int i;
=======
>>>>>>> upstream/android-13
	int ret;

	/*
	 * There's no need to prepare (and unprepare) the framebuffer when the
	 * plane is not visible, as it will not be displayed.
	 */
	if (!state->visible)
		return 0;

<<<<<<< HEAD
	for (i = 0; i < rstate->format->planes; ++i) {
		struct drm_gem_cma_object *gem =
			drm_fb_cma_get_gem_obj(state->fb, i);
		struct sg_table *sgt = &rstate->sg_tables[i];

		ret = dma_get_sgtable(rcdu->dev, sgt, gem->vaddr, gem->paddr,
				      gem->base.size);
		if (ret)
			goto fail;

		ret = vsp1_du_map_sg(vsp->vsp, sgt);
		if (!ret) {
			sg_free_table(sgt);
			ret = -ENOMEM;
			goto fail;
		}
	}

	ret = drm_gem_fb_prepare_fb(plane, state);
	if (ret)
		goto fail;

	return 0;

fail:
	while (i--) {
		struct sg_table *sgt = &rstate->sg_tables[i];
=======
	ret = rcar_du_vsp_map_fb(vsp, state->fb, rstate->sg_tables);
	if (ret < 0)
		return ret;

	return drm_gem_plane_helper_prepare_fb(plane, state);
}

void rcar_du_vsp_unmap_fb(struct rcar_du_vsp *vsp, struct drm_framebuffer *fb,
			  struct sg_table sg_tables[3])
{
	unsigned int i;

	for (i = 0; i < fb->format->num_planes; ++i) {
		struct sg_table *sgt = &sg_tables[i];
>>>>>>> upstream/android-13

		vsp1_du_unmap_sg(vsp->vsp, sgt);
		sg_free_table(sgt);
	}
<<<<<<< HEAD

	return ret;
=======
>>>>>>> upstream/android-13
}

static void rcar_du_vsp_plane_cleanup_fb(struct drm_plane *plane,
					 struct drm_plane_state *state)
{
	struct rcar_du_vsp_plane_state *rstate = to_rcar_vsp_plane_state(state);
	struct rcar_du_vsp *vsp = to_rcar_vsp_plane(plane)->vsp;
<<<<<<< HEAD
	unsigned int i;
=======
>>>>>>> upstream/android-13

	if (!state->visible)
		return;

<<<<<<< HEAD
	for (i = 0; i < rstate->format->planes; ++i) {
		struct sg_table *sgt = &rstate->sg_tables[i];

		vsp1_du_unmap_sg(vsp->vsp, sgt);
		sg_free_table(sgt);
	}
}

static int rcar_du_vsp_plane_atomic_check(struct drm_plane *plane,
					  struct drm_plane_state *state)
{
	struct rcar_du_vsp_plane_state *rstate = to_rcar_vsp_plane_state(state);

	return __rcar_du_plane_atomic_check(plane, state, &rstate->format);
}

static void rcar_du_vsp_plane_atomic_update(struct drm_plane *plane,
					struct drm_plane_state *old_state)
{
	struct rcar_du_vsp_plane *rplane = to_rcar_vsp_plane(plane);
	struct rcar_du_crtc *crtc = to_rcar_crtc(old_state->crtc);

	if (plane->state->visible)
		rcar_du_vsp_plane_setup(rplane);
	else
=======
	rcar_du_vsp_unmap_fb(vsp, state->fb, rstate->sg_tables);
}

static int rcar_du_vsp_plane_atomic_check(struct drm_plane *plane,
					  struct drm_atomic_state *state)
{
	struct drm_plane_state *new_plane_state = drm_atomic_get_new_plane_state(state,
										 plane);
	struct rcar_du_vsp_plane_state *rstate = to_rcar_vsp_plane_state(new_plane_state);

	return __rcar_du_plane_atomic_check(plane, new_plane_state,
					    &rstate->format);
}

static void rcar_du_vsp_plane_atomic_update(struct drm_plane *plane,
					struct drm_atomic_state *state)
{
	struct drm_plane_state *old_state = drm_atomic_get_old_plane_state(state, plane);
	struct drm_plane_state *new_state = drm_atomic_get_new_plane_state(state, plane);
	struct rcar_du_vsp_plane *rplane = to_rcar_vsp_plane(plane);
	struct rcar_du_crtc *crtc = to_rcar_crtc(old_state->crtc);

	if (new_state->visible)
		rcar_du_vsp_plane_setup(rplane);
	else if (old_state->crtc)
>>>>>>> upstream/android-13
		vsp1_du_atomic_update(rplane->vsp->vsp, crtc->vsp_pipe,
				      rplane->index, NULL);
}

static const struct drm_plane_helper_funcs rcar_du_vsp_plane_helper_funcs = {
	.prepare_fb = rcar_du_vsp_plane_prepare_fb,
	.cleanup_fb = rcar_du_vsp_plane_cleanup_fb,
	.atomic_check = rcar_du_vsp_plane_atomic_check,
	.atomic_update = rcar_du_vsp_plane_atomic_update,
};

static struct drm_plane_state *
rcar_du_vsp_plane_atomic_duplicate_state(struct drm_plane *plane)
{
	struct rcar_du_vsp_plane_state *copy;

	if (WARN_ON(!plane->state))
		return NULL;

	copy = kzalloc(sizeof(*copy), GFP_KERNEL);
	if (copy == NULL)
		return NULL;

	__drm_atomic_helper_plane_duplicate_state(plane, &copy->state);

	return &copy->state;
}

static void rcar_du_vsp_plane_atomic_destroy_state(struct drm_plane *plane,
						   struct drm_plane_state *state)
{
	__drm_atomic_helper_plane_destroy_state(state);
	kfree(to_rcar_vsp_plane_state(state));
}

static void rcar_du_vsp_plane_reset(struct drm_plane *plane)
{
	struct rcar_du_vsp_plane_state *state;

	if (plane->state) {
		rcar_du_vsp_plane_atomic_destroy_state(plane, plane->state);
		plane->state = NULL;
	}

	state = kzalloc(sizeof(*state), GFP_KERNEL);
	if (state == NULL)
		return;

<<<<<<< HEAD
	state->state.alpha = DRM_BLEND_ALPHA_OPAQUE;
	state->state.zpos = plane->type == DRM_PLANE_TYPE_PRIMARY ? 0 : 1;

	plane->state = &state->state;
	plane->state->plane = plane;
=======
	__drm_atomic_helper_plane_reset(plane, &state->state);
	state->state.zpos = plane->type == DRM_PLANE_TYPE_PRIMARY ? 0 : 1;
>>>>>>> upstream/android-13
}

static const struct drm_plane_funcs rcar_du_vsp_plane_funcs = {
	.update_plane = drm_atomic_helper_update_plane,
	.disable_plane = drm_atomic_helper_disable_plane,
	.reset = rcar_du_vsp_plane_reset,
	.destroy = drm_plane_cleanup,
	.atomic_duplicate_state = rcar_du_vsp_plane_atomic_duplicate_state,
	.atomic_destroy_state = rcar_du_vsp_plane_atomic_destroy_state,
};

<<<<<<< HEAD
=======
static void rcar_du_vsp_cleanup(struct drm_device *dev, void *res)
{
	struct rcar_du_vsp *vsp = res;
	unsigned int i;

	for (i = 0; i < vsp->num_planes; ++i) {
		struct rcar_du_vsp_plane *plane = &vsp->planes[i];

		drm_plane_cleanup(&plane->plane);
	}

	kfree(vsp->planes);

	put_device(vsp->vsp);
}

>>>>>>> upstream/android-13
int rcar_du_vsp_init(struct rcar_du_vsp *vsp, struct device_node *np,
		     unsigned int crtcs)
{
	struct rcar_du_device *rcdu = vsp->dev;
	struct platform_device *pdev;
	unsigned int num_crtcs = hweight32(crtcs);
<<<<<<< HEAD
=======
	unsigned int num_planes;
>>>>>>> upstream/android-13
	unsigned int i;
	int ret;

	/* Find the VSP device and initialize it. */
	pdev = of_find_device_by_node(np);
	if (!pdev)
		return -ENXIO;

	vsp->vsp = &pdev->dev;

<<<<<<< HEAD
=======
	ret = drmm_add_action_or_reset(&rcdu->ddev, rcar_du_vsp_cleanup, vsp);
	if (ret < 0)
		return ret;

>>>>>>> upstream/android-13
	ret = vsp1_du_init(vsp->vsp);
	if (ret < 0)
		return ret;

	 /*
	  * The VSP2D (Gen3) has 5 RPFs, but the VSP1D (Gen2) is limited to
	  * 4 RPFs.
	  */
<<<<<<< HEAD
	vsp->num_planes = rcdu->info->gen >= 3 ? 5 : 4;

	vsp->planes = devm_kcalloc(rcdu->dev, vsp->num_planes,
				   sizeof(*vsp->planes), GFP_KERNEL);
	if (!vsp->planes)
		return -ENOMEM;

	for (i = 0; i < vsp->num_planes; ++i) {
=======
	num_planes = rcdu->info->gen >= 3 ? 5 : 4;

	vsp->planes = kcalloc(num_planes, sizeof(*vsp->planes), GFP_KERNEL);
	if (!vsp->planes)
		return -ENOMEM;

	for (i = 0; i < num_planes; ++i) {
>>>>>>> upstream/android-13
		enum drm_plane_type type = i < num_crtcs
					 ? DRM_PLANE_TYPE_PRIMARY
					 : DRM_PLANE_TYPE_OVERLAY;
		struct rcar_du_vsp_plane *plane = &vsp->planes[i];

		plane->vsp = vsp;
		plane->index = i;

<<<<<<< HEAD
		ret = drm_universal_plane_init(rcdu->ddev, &plane->plane, crtcs,
					       &rcar_du_vsp_plane_funcs,
					       formats_kms,
					       ARRAY_SIZE(formats_kms),
=======
		ret = drm_universal_plane_init(&rcdu->ddev, &plane->plane,
					       crtcs, &rcar_du_vsp_plane_funcs,
					       rcar_du_vsp_formats,
					       ARRAY_SIZE(rcar_du_vsp_formats),
>>>>>>> upstream/android-13
					       NULL, type, NULL);
		if (ret < 0)
			return ret;

		drm_plane_helper_add(&plane->plane,
				     &rcar_du_vsp_plane_helper_funcs);

<<<<<<< HEAD
		if (type == DRM_PLANE_TYPE_PRIMARY)
			continue;

		drm_plane_create_alpha_property(&plane->plane);
		drm_plane_create_zpos_property(&plane->plane, 1, 1,
					       vsp->num_planes - 1);
=======
		if (type == DRM_PLANE_TYPE_PRIMARY) {
			drm_plane_create_zpos_immutable_property(&plane->plane,
								 0);
		} else {
			drm_plane_create_alpha_property(&plane->plane);
			drm_plane_create_zpos_property(&plane->plane, 1, 1,
						       num_planes - 1);
		}

		vsp->num_planes++;
>>>>>>> upstream/android-13
	}

	return 0;
}
