<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) Icenowy Zheng <icenowy@aosc.io>
 *
 * Based on sun4i_layer.h, which is:
 *   Copyright (C) 2015 Free Electrons
 *   Copyright (C) 2015 NextThing Co
 *
 *   Maxime Ripard <maxime.ripard@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
<<<<<<< HEAD
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_plane_helper.h>
#include <drm/drmP.h>

#include "sun8i_ui_layer.h"
#include "sun8i_mixer.h"
=======
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_probe_helper.h>

#include "sun8i_mixer.h"
#include "sun8i_ui_layer.h"
>>>>>>> upstream/android-13
#include "sun8i_ui_scaler.h"

static void sun8i_ui_layer_enable(struct sun8i_mixer *mixer, int channel,
				  int overlay, bool enable, unsigned int zpos,
				  unsigned int old_zpos)
{
<<<<<<< HEAD
	u32 val;
=======
	u32 val, bld_base, ch_base;

	bld_base = sun8i_blender_base(mixer);
	ch_base = sun8i_channel_base(mixer, channel);
>>>>>>> upstream/android-13

	DRM_DEBUG_DRIVER("%sabling channel %d overlay %d\n",
			 enable ? "En" : "Dis", channel, overlay);

	if (enable)
		val = SUN8I_MIXER_CHAN_UI_LAYER_ATTR_EN;
	else
		val = 0;

	regmap_update_bits(mixer->engine.regs,
<<<<<<< HEAD
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR(channel, overlay),
=======
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR(ch_base, overlay),
>>>>>>> upstream/android-13
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR_EN, val);

	if (!enable || zpos != old_zpos) {
		regmap_update_bits(mixer->engine.regs,
<<<<<<< HEAD
				   SUN8I_MIXER_BLEND_PIPE_CTL,
=======
				   SUN8I_MIXER_BLEND_PIPE_CTL(bld_base),
>>>>>>> upstream/android-13
				   SUN8I_MIXER_BLEND_PIPE_CTL_EN(old_zpos),
				   0);

		regmap_update_bits(mixer->engine.regs,
<<<<<<< HEAD
				   SUN8I_MIXER_BLEND_ROUTE,
=======
				   SUN8I_MIXER_BLEND_ROUTE(bld_base),
>>>>>>> upstream/android-13
				   SUN8I_MIXER_BLEND_ROUTE_PIPE_MSK(old_zpos),
				   0);
	}

	if (enable) {
		val = SUN8I_MIXER_BLEND_PIPE_CTL_EN(zpos);

		regmap_update_bits(mixer->engine.regs,
<<<<<<< HEAD
				   SUN8I_MIXER_BLEND_PIPE_CTL, val, val);
=======
				   SUN8I_MIXER_BLEND_PIPE_CTL(bld_base),
				   val, val);
>>>>>>> upstream/android-13

		val = channel << SUN8I_MIXER_BLEND_ROUTE_PIPE_SHIFT(zpos);

		regmap_update_bits(mixer->engine.regs,
<<<<<<< HEAD
				   SUN8I_MIXER_BLEND_ROUTE,
=======
				   SUN8I_MIXER_BLEND_ROUTE(bld_base),
>>>>>>> upstream/android-13
				   SUN8I_MIXER_BLEND_ROUTE_PIPE_MSK(zpos),
				   val);
	}
}

<<<<<<< HEAD
=======
static void sun8i_ui_layer_update_alpha(struct sun8i_mixer *mixer, int channel,
					int overlay, struct drm_plane *plane)
{
	u32 mask, val, ch_base;

	ch_base = sun8i_channel_base(mixer, channel);

	mask = SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_MASK |
		SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MASK;

	val = SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA(plane->state->alpha >> 8);

	val |= (plane->state->alpha == DRM_BLEND_ALPHA_OPAQUE) ?
		SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_PIXEL :
		SUN8I_MIXER_CHAN_UI_LAYER_ATTR_ALPHA_MODE_COMBINED;

	regmap_update_bits(mixer->engine.regs,
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR(ch_base, overlay),
			   mask, val);
}

>>>>>>> upstream/android-13
static int sun8i_ui_layer_update_coord(struct sun8i_mixer *mixer, int channel,
				       int overlay, struct drm_plane *plane,
				       unsigned int zpos)
{
	struct drm_plane_state *state = plane->state;
	u32 src_w, src_h, dst_w, dst_h;
<<<<<<< HEAD
=======
	u32 bld_base, ch_base;
>>>>>>> upstream/android-13
	u32 outsize, insize;
	u32 hphase, vphase;

	DRM_DEBUG_DRIVER("Updating UI channel %d overlay %d\n",
			 channel, overlay);

<<<<<<< HEAD
=======
	bld_base = sun8i_blender_base(mixer);
	ch_base = sun8i_channel_base(mixer, channel);

>>>>>>> upstream/android-13
	src_w = drm_rect_width(&state->src) >> 16;
	src_h = drm_rect_height(&state->src) >> 16;
	dst_w = drm_rect_width(&state->dst);
	dst_h = drm_rect_height(&state->dst);

	hphase = state->src.x1 & 0xffff;
	vphase = state->src.y1 & 0xffff;

	insize = SUN8I_MIXER_SIZE(src_w, src_h);
	outsize = SUN8I_MIXER_SIZE(dst_w, dst_h);

	if (plane->type == DRM_PLANE_TYPE_PRIMARY) {
		bool interlaced = false;
		u32 val;

		DRM_DEBUG_DRIVER("Primary layer, updating global size W: %u H: %u\n",
				 dst_w, dst_h);
		regmap_write(mixer->engine.regs,
			     SUN8I_MIXER_GLOBAL_SIZE,
			     outsize);
<<<<<<< HEAD
		regmap_write(mixer->engine.regs, SUN8I_MIXER_BLEND_OUTSIZE,
			     outsize);
=======
		regmap_write(mixer->engine.regs,
			     SUN8I_MIXER_BLEND_OUTSIZE(bld_base), outsize);
>>>>>>> upstream/android-13

		if (state->crtc)
			interlaced = state->crtc->state->adjusted_mode.flags
				& DRM_MODE_FLAG_INTERLACE;

		if (interlaced)
			val = SUN8I_MIXER_BLEND_OUTCTL_INTERLACED;
		else
			val = 0;

		regmap_update_bits(mixer->engine.regs,
<<<<<<< HEAD
				   SUN8I_MIXER_BLEND_OUTCTL,
=======
				   SUN8I_MIXER_BLEND_OUTCTL(bld_base),
>>>>>>> upstream/android-13
				   SUN8I_MIXER_BLEND_OUTCTL_INTERLACED,
				   val);

		DRM_DEBUG_DRIVER("Switching display mixer interlaced mode %s\n",
				 interlaced ? "on" : "off");
	}

	/* Set height and width */
	DRM_DEBUG_DRIVER("Layer source offset X: %d Y: %d\n",
			 state->src.x1 >> 16, state->src.y1 >> 16);
	DRM_DEBUG_DRIVER("Layer source size W: %d H: %d\n", src_w, src_h);
	regmap_write(mixer->engine.regs,
<<<<<<< HEAD
		     SUN8I_MIXER_CHAN_UI_LAYER_SIZE(channel, overlay),
		     insize);
	regmap_write(mixer->engine.regs,
		     SUN8I_MIXER_CHAN_UI_OVL_SIZE(channel),
=======
		     SUN8I_MIXER_CHAN_UI_LAYER_SIZE(ch_base, overlay),
		     insize);
	regmap_write(mixer->engine.regs,
		     SUN8I_MIXER_CHAN_UI_OVL_SIZE(ch_base),
>>>>>>> upstream/android-13
		     insize);

	if (insize != outsize || hphase || vphase) {
		u32 hscale, vscale;

		DRM_DEBUG_DRIVER("HW scaling is enabled\n");

		hscale = state->src_w / state->crtc_w;
		vscale = state->src_h / state->crtc_h;

		sun8i_ui_scaler_setup(mixer, channel, src_w, src_h, dst_w,
				      dst_h, hscale, vscale, hphase, vphase);
		sun8i_ui_scaler_enable(mixer, channel, true);
	} else {
		DRM_DEBUG_DRIVER("HW scaling is not needed\n");
		sun8i_ui_scaler_enable(mixer, channel, false);
	}

	/* Set base coordinates */
	DRM_DEBUG_DRIVER("Layer destination coordinates X: %d Y: %d\n",
			 state->dst.x1, state->dst.y1);
	DRM_DEBUG_DRIVER("Layer destination size W: %d H: %d\n", dst_w, dst_h);
	regmap_write(mixer->engine.regs,
<<<<<<< HEAD
		     SUN8I_MIXER_BLEND_ATTR_COORD(zpos),
		     SUN8I_MIXER_COORD(state->dst.x1, state->dst.y1));
	regmap_write(mixer->engine.regs,
		     SUN8I_MIXER_BLEND_ATTR_INSIZE(zpos),
=======
		     SUN8I_MIXER_BLEND_ATTR_COORD(bld_base, zpos),
		     SUN8I_MIXER_COORD(state->dst.x1, state->dst.y1));
	regmap_write(mixer->engine.regs,
		     SUN8I_MIXER_BLEND_ATTR_INSIZE(bld_base, zpos),
>>>>>>> upstream/android-13
		     outsize);

	return 0;
}

static int sun8i_ui_layer_update_formats(struct sun8i_mixer *mixer, int channel,
					 int overlay, struct drm_plane *plane)
{
	struct drm_plane_state *state = plane->state;
<<<<<<< HEAD
	const struct de2_fmt_info *fmt_info;
	u32 val;

	fmt_info = sun8i_mixer_format_info(state->fb->format->format);
	if (!fmt_info || !fmt_info->rgb) {
=======
	const struct drm_format_info *fmt;
	u32 val, ch_base, hw_fmt;
	int ret;

	ch_base = sun8i_channel_base(mixer, channel);

	fmt = state->fb->format;
	ret = sun8i_mixer_drm_format_to_hw(fmt->format, &hw_fmt);
	if (ret || fmt->is_yuv) {
>>>>>>> upstream/android-13
		DRM_DEBUG_DRIVER("Invalid format\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	val = fmt_info->de2_fmt << SUN8I_MIXER_CHAN_UI_LAYER_ATTR_FBFMT_OFFSET;
	regmap_update_bits(mixer->engine.regs,
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR(channel, overlay),
=======
	val = hw_fmt << SUN8I_MIXER_CHAN_UI_LAYER_ATTR_FBFMT_OFFSET;
	regmap_update_bits(mixer->engine.regs,
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR(ch_base, overlay),
>>>>>>> upstream/android-13
			   SUN8I_MIXER_CHAN_UI_LAYER_ATTR_FBFMT_MASK, val);

	return 0;
}

static int sun8i_ui_layer_update_buffer(struct sun8i_mixer *mixer, int channel,
					int overlay, struct drm_plane *plane)
{
	struct drm_plane_state *state = plane->state;
	struct drm_framebuffer *fb = state->fb;
	struct drm_gem_cma_object *gem;
	dma_addr_t paddr;
<<<<<<< HEAD
	int bpp;

=======
	u32 ch_base;
	int bpp;

	ch_base = sun8i_channel_base(mixer, channel);

>>>>>>> upstream/android-13
	/* Get the physical address of the buffer in memory */
	gem = drm_fb_cma_get_gem_obj(fb, 0);

	DRM_DEBUG_DRIVER("Using GEM @ %pad\n", &gem->paddr);

	/* Compute the start of the displayed memory */
	bpp = fb->format->cpp[0];
	paddr = gem->paddr + fb->offsets[0];

	/* Fixup framebuffer address for src coordinates */
	paddr += (state->src.x1 >> 16) * bpp;
	paddr += (state->src.y1 >> 16) * fb->pitches[0];

	/* Set the line width */
	DRM_DEBUG_DRIVER("Layer line width: %d bytes\n", fb->pitches[0]);
	regmap_write(mixer->engine.regs,
<<<<<<< HEAD
		     SUN8I_MIXER_CHAN_UI_LAYER_PITCH(channel, overlay),
=======
		     SUN8I_MIXER_CHAN_UI_LAYER_PITCH(ch_base, overlay),
>>>>>>> upstream/android-13
		     fb->pitches[0]);

	DRM_DEBUG_DRIVER("Setting buffer address to %pad\n", &paddr);

	regmap_write(mixer->engine.regs,
<<<<<<< HEAD
		     SUN8I_MIXER_CHAN_UI_LAYER_TOP_LADDR(channel, overlay),
=======
		     SUN8I_MIXER_CHAN_UI_LAYER_TOP_LADDR(ch_base, overlay),
>>>>>>> upstream/android-13
		     lower_32_bits(paddr));

	return 0;
}

static int sun8i_ui_layer_atomic_check(struct drm_plane *plane,
<<<<<<< HEAD
				       struct drm_plane_state *state)
{
	struct sun8i_ui_layer *layer = plane_to_sun8i_ui_layer(plane);
	struct drm_crtc *crtc = state->crtc;
=======
				       struct drm_atomic_state *state)
{
	struct drm_plane_state *new_plane_state = drm_atomic_get_new_plane_state(state,
										 plane);
	struct sun8i_ui_layer *layer = plane_to_sun8i_ui_layer(plane);
	struct drm_crtc *crtc = new_plane_state->crtc;
>>>>>>> upstream/android-13
	struct drm_crtc_state *crtc_state;
	int min_scale, max_scale;

	if (!crtc)
		return 0;

<<<<<<< HEAD
	crtc_state = drm_atomic_get_existing_crtc_state(state->state, crtc);
=======
	crtc_state = drm_atomic_get_existing_crtc_state(state,
							crtc);
>>>>>>> upstream/android-13
	if (WARN_ON(!crtc_state))
		return -EINVAL;

	min_scale = DRM_PLANE_HELPER_NO_SCALING;
	max_scale = DRM_PLANE_HELPER_NO_SCALING;

	if (layer->mixer->cfg->scaler_mask & BIT(layer->channel)) {
		min_scale = SUN8I_UI_SCALER_SCALE_MIN;
		max_scale = SUN8I_UI_SCALER_SCALE_MAX;
	}

<<<<<<< HEAD
	return drm_atomic_helper_check_plane_state(state, crtc_state,
=======
	return drm_atomic_helper_check_plane_state(new_plane_state,
						   crtc_state,
>>>>>>> upstream/android-13
						   min_scale, max_scale,
						   true, true);
}

static void sun8i_ui_layer_atomic_disable(struct drm_plane *plane,
<<<<<<< HEAD
					  struct drm_plane_state *old_state)
{
=======
					  struct drm_atomic_state *state)
{
	struct drm_plane_state *old_state = drm_atomic_get_old_plane_state(state,
									   plane);
>>>>>>> upstream/android-13
	struct sun8i_ui_layer *layer = plane_to_sun8i_ui_layer(plane);
	unsigned int old_zpos = old_state->normalized_zpos;
	struct sun8i_mixer *mixer = layer->mixer;

	sun8i_ui_layer_enable(mixer, layer->channel, layer->overlay, false, 0,
			      old_zpos);
}

static void sun8i_ui_layer_atomic_update(struct drm_plane *plane,
<<<<<<< HEAD
					 struct drm_plane_state *old_state)
{
	struct sun8i_ui_layer *layer = plane_to_sun8i_ui_layer(plane);
	unsigned int zpos = plane->state->normalized_zpos;
	unsigned int old_zpos = old_state->normalized_zpos;
	struct sun8i_mixer *mixer = layer->mixer;

	if (!plane->state->visible) {
=======
					 struct drm_atomic_state *state)
{
	struct drm_plane_state *old_state = drm_atomic_get_old_plane_state(state,
									   plane);
	struct drm_plane_state *new_state = drm_atomic_get_new_plane_state(state,
									   plane);
	struct sun8i_ui_layer *layer = plane_to_sun8i_ui_layer(plane);
	unsigned int zpos = new_state->normalized_zpos;
	unsigned int old_zpos = old_state->normalized_zpos;
	struct sun8i_mixer *mixer = layer->mixer;

	if (!new_state->visible) {
>>>>>>> upstream/android-13
		sun8i_ui_layer_enable(mixer, layer->channel,
				      layer->overlay, false, 0, old_zpos);
		return;
	}

	sun8i_ui_layer_update_coord(mixer, layer->channel,
				    layer->overlay, plane, zpos);
<<<<<<< HEAD
=======
	sun8i_ui_layer_update_alpha(mixer, layer->channel,
				    layer->overlay, plane);
>>>>>>> upstream/android-13
	sun8i_ui_layer_update_formats(mixer, layer->channel,
				      layer->overlay, plane);
	sun8i_ui_layer_update_buffer(mixer, layer->channel,
				     layer->overlay, plane);
	sun8i_ui_layer_enable(mixer, layer->channel, layer->overlay,
			      true, zpos, old_zpos);
}

<<<<<<< HEAD
static struct drm_plane_helper_funcs sun8i_ui_layer_helper_funcs = {
=======
static const struct drm_plane_helper_funcs sun8i_ui_layer_helper_funcs = {
>>>>>>> upstream/android-13
	.atomic_check	= sun8i_ui_layer_atomic_check,
	.atomic_disable	= sun8i_ui_layer_atomic_disable,
	.atomic_update	= sun8i_ui_layer_atomic_update,
};

static const struct drm_plane_funcs sun8i_ui_layer_funcs = {
	.atomic_destroy_state	= drm_atomic_helper_plane_destroy_state,
	.atomic_duplicate_state	= drm_atomic_helper_plane_duplicate_state,
	.destroy		= drm_plane_cleanup,
	.disable_plane		= drm_atomic_helper_disable_plane,
	.reset			= drm_atomic_helper_plane_reset,
	.update_plane		= drm_atomic_helper_update_plane,
};

static const u32 sun8i_ui_layer_formats[] = {
	DRM_FORMAT_ABGR1555,
	DRM_FORMAT_ABGR4444,
	DRM_FORMAT_ABGR8888,
	DRM_FORMAT_ARGB1555,
	DRM_FORMAT_ARGB4444,
	DRM_FORMAT_ARGB8888,
	DRM_FORMAT_BGR565,
	DRM_FORMAT_BGR888,
	DRM_FORMAT_BGRA5551,
	DRM_FORMAT_BGRA4444,
	DRM_FORMAT_BGRA8888,
	DRM_FORMAT_BGRX8888,
	DRM_FORMAT_RGB565,
	DRM_FORMAT_RGB888,
	DRM_FORMAT_RGBA4444,
	DRM_FORMAT_RGBA5551,
	DRM_FORMAT_RGBA8888,
	DRM_FORMAT_RGBX8888,
	DRM_FORMAT_XBGR8888,
	DRM_FORMAT_XRGB8888,
};

<<<<<<< HEAD
=======
static const uint64_t sun8i_layer_modifiers[] = {
	DRM_FORMAT_MOD_LINEAR,
	DRM_FORMAT_MOD_INVALID
};

>>>>>>> upstream/android-13
struct sun8i_ui_layer *sun8i_ui_layer_init_one(struct drm_device *drm,
					       struct sun8i_mixer *mixer,
					       int index)
{
	enum drm_plane_type type = DRM_PLANE_TYPE_OVERLAY;
	int channel = mixer->cfg->vi_num + index;
	struct sun8i_ui_layer *layer;
	unsigned int plane_cnt;
	int ret;

	layer = devm_kzalloc(drm->dev, sizeof(*layer), GFP_KERNEL);
	if (!layer)
		return ERR_PTR(-ENOMEM);

	if (index == 0)
		type = DRM_PLANE_TYPE_PRIMARY;

	/* possible crtcs are set later */
	ret = drm_universal_plane_init(drm, &layer->plane, 0,
				       &sun8i_ui_layer_funcs,
				       sun8i_ui_layer_formats,
				       ARRAY_SIZE(sun8i_ui_layer_formats),
<<<<<<< HEAD
				       NULL, type, NULL);
=======
				       sun8i_layer_modifiers, type, NULL);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(drm->dev, "Couldn't initialize layer\n");
		return ERR_PTR(ret);
	}

	plane_cnt = mixer->cfg->ui_num + mixer->cfg->vi_num;

<<<<<<< HEAD
=======
	ret = drm_plane_create_alpha_property(&layer->plane);
	if (ret) {
		dev_err(drm->dev, "Couldn't add alpha property\n");
		return ERR_PTR(ret);
	}

>>>>>>> upstream/android-13
	ret = drm_plane_create_zpos_property(&layer->plane, channel,
					     0, plane_cnt - 1);
	if (ret) {
		dev_err(drm->dev, "Couldn't add zpos property\n");
		return ERR_PTR(ret);
	}

	drm_plane_helper_add(&layer->plane, &sun8i_ui_layer_helper_funcs);
	layer->mixer = mixer;
	layer->channel = channel;
	layer->overlay = 0;

	return layer;
}
