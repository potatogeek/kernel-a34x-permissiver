<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2015 Free Electrons
 * Copyright (C) 2015 NextThing Co
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
=======
#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
#include <drm/drm_print.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>
>>>>>>> upstream/android-13

#include "sun4i_crtc.h"
#include "sun4i_tcon.h"
#include "sun4i_rgb.h"

struct sun4i_rgb {
	struct drm_connector	connector;
	struct drm_encoder	encoder;

	struct sun4i_tcon	*tcon;
<<<<<<< HEAD
=======
	struct drm_panel	*panel;
	struct drm_bridge	*bridge;
>>>>>>> upstream/android-13
};

static inline struct sun4i_rgb *
drm_connector_to_sun4i_rgb(struct drm_connector *connector)
{
	return container_of(connector, struct sun4i_rgb,
			    connector);
}

static inline struct sun4i_rgb *
drm_encoder_to_sun4i_rgb(struct drm_encoder *encoder)
{
	return container_of(encoder, struct sun4i_rgb,
			    encoder);
}

static int sun4i_rgb_get_modes(struct drm_connector *connector)
{
	struct sun4i_rgb *rgb =
		drm_connector_to_sun4i_rgb(connector);
<<<<<<< HEAD
	struct sun4i_tcon *tcon = rgb->tcon;

	return drm_panel_get_modes(tcon->panel);
}

=======

	return drm_panel_get_modes(rgb->panel, connector);
}

/*
 * VESA DMT defines a tolerance of 0.5% on the pixel clock, while the
 * CVT spec reuses that tolerance in its examples, so it looks to be a
 * good default tolerance for the EDID-based modes. Define it to 5 per
 * mille to avoid floating point operations.
 */
#define SUN4I_RGB_DOTCLOCK_TOLERANCE_PER_MILLE	5

>>>>>>> upstream/android-13
static enum drm_mode_status sun4i_rgb_mode_valid(struct drm_encoder *crtc,
						 const struct drm_display_mode *mode)
{
	struct sun4i_rgb *rgb = drm_encoder_to_sun4i_rgb(crtc);
	struct sun4i_tcon *tcon = rgb->tcon;
	u32 hsync = mode->hsync_end - mode->hsync_start;
	u32 vsync = mode->vsync_end - mode->vsync_start;
<<<<<<< HEAD
	unsigned long rate = mode->clock * 1000;
	long rounded_rate;
=======
	unsigned long long rate = mode->clock * 1000;
	unsigned long long lowest, highest;
	unsigned long long rounded_rate;
>>>>>>> upstream/android-13

	DRM_DEBUG_DRIVER("Validating modes...\n");

	if (hsync < 1)
		return MODE_HSYNC_NARROW;

	if (hsync > 0x3ff)
		return MODE_HSYNC_WIDE;

	if ((mode->hdisplay < 1) || (mode->htotal < 1))
		return MODE_H_ILLEGAL;

	if ((mode->hdisplay > 0x7ff) || (mode->htotal > 0xfff))
		return MODE_BAD_HVALUE;

	DRM_DEBUG_DRIVER("Horizontal parameters OK\n");

	if (vsync < 1)
		return MODE_VSYNC_NARROW;

	if (vsync > 0x3ff)
		return MODE_VSYNC_WIDE;

	if ((mode->vdisplay < 1) || (mode->vtotal < 1))
		return MODE_V_ILLEGAL;

	if ((mode->vdisplay > 0x7ff) || (mode->vtotal > 0xfff))
		return MODE_BAD_VVALUE;

	DRM_DEBUG_DRIVER("Vertical parameters OK\n");

<<<<<<< HEAD
	tcon->dclk_min_div = 6;
	tcon->dclk_max_div = 127;
	rounded_rate = clk_round_rate(tcon->dclk, rate);
	if (rounded_rate < rate)
		return MODE_CLOCK_LOW;

	if (rounded_rate > rate)
		return MODE_CLOCK_HIGH;

=======
	/*
	 * TODO: We should use the struct display_timing if available
	 * and / or trying to stretch the timings within that
	 * tolerancy to take care of panels that we wouldn't be able
	 * to have a exact match for.
	 */
	if (rgb->panel) {
		DRM_DEBUG_DRIVER("RGB panel used, skipping clock rate checks");
		goto out;
	}

	/*
	 * That shouldn't ever happen unless something is really wrong, but it
	 * doesn't harm to check.
	 */
	if (!rgb->bridge)
		goto out;

	tcon->dclk_min_div = 6;
	tcon->dclk_max_div = 127;
	rounded_rate = clk_round_rate(tcon->dclk, rate);

	lowest = rate * (1000 - SUN4I_RGB_DOTCLOCK_TOLERANCE_PER_MILLE);
	do_div(lowest, 1000);
	if (rounded_rate < lowest)
		return MODE_CLOCK_LOW;

	highest = rate * (1000 + SUN4I_RGB_DOTCLOCK_TOLERANCE_PER_MILLE);
	do_div(highest, 1000);
	if (rounded_rate > highest)
		return MODE_CLOCK_HIGH;

out:
>>>>>>> upstream/android-13
	DRM_DEBUG_DRIVER("Clock rate OK\n");

	return MODE_OK;
}

<<<<<<< HEAD
static struct drm_connector_helper_funcs sun4i_rgb_con_helper_funcs = {
=======
static const struct drm_connector_helper_funcs sun4i_rgb_con_helper_funcs = {
>>>>>>> upstream/android-13
	.get_modes	= sun4i_rgb_get_modes,
};

static void
sun4i_rgb_connector_destroy(struct drm_connector *connector)
{
<<<<<<< HEAD
	struct sun4i_rgb *rgb = drm_connector_to_sun4i_rgb(connector);
	struct sun4i_tcon *tcon = rgb->tcon;

	drm_panel_detach(tcon->panel);
=======
>>>>>>> upstream/android-13
	drm_connector_cleanup(connector);
}

static const struct drm_connector_funcs sun4i_rgb_con_funcs = {
	.fill_modes		= drm_helper_probe_single_connector_modes,
	.destroy		= sun4i_rgb_connector_destroy,
	.reset			= drm_atomic_helper_connector_reset,
	.atomic_duplicate_state	= drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state	= drm_atomic_helper_connector_destroy_state,
};

static void sun4i_rgb_encoder_enable(struct drm_encoder *encoder)
{
	struct sun4i_rgb *rgb = drm_encoder_to_sun4i_rgb(encoder);
<<<<<<< HEAD
	struct sun4i_tcon *tcon = rgb->tcon;

	DRM_DEBUG_DRIVER("Enabling RGB output\n");

	if (!IS_ERR(tcon->panel)) {
		drm_panel_prepare(tcon->panel);
		drm_panel_enable(tcon->panel);
=======

	DRM_DEBUG_DRIVER("Enabling RGB output\n");

	if (rgb->panel) {
		drm_panel_prepare(rgb->panel);
		drm_panel_enable(rgb->panel);
>>>>>>> upstream/android-13
	}
}

static void sun4i_rgb_encoder_disable(struct drm_encoder *encoder)
{
	struct sun4i_rgb *rgb = drm_encoder_to_sun4i_rgb(encoder);
<<<<<<< HEAD
	struct sun4i_tcon *tcon = rgb->tcon;

	DRM_DEBUG_DRIVER("Disabling RGB output\n");

	if (!IS_ERR(tcon->panel)) {
		drm_panel_disable(tcon->panel);
		drm_panel_unprepare(tcon->panel);
	}
}

static struct drm_encoder_helper_funcs sun4i_rgb_enc_helper_funcs = {
=======

	DRM_DEBUG_DRIVER("Disabling RGB output\n");

	if (rgb->panel) {
		drm_panel_disable(rgb->panel);
		drm_panel_unprepare(rgb->panel);
	}
}

static const struct drm_encoder_helper_funcs sun4i_rgb_enc_helper_funcs = {
>>>>>>> upstream/android-13
	.disable	= sun4i_rgb_encoder_disable,
	.enable		= sun4i_rgb_encoder_enable,
	.mode_valid	= sun4i_rgb_mode_valid,
};

<<<<<<< HEAD
static void sun4i_rgb_enc_destroy(struct drm_encoder *encoder)
{
	drm_encoder_cleanup(encoder);
}

static struct drm_encoder_funcs sun4i_rgb_enc_funcs = {
	.destroy	= sun4i_rgb_enc_destroy,
};

int sun4i_rgb_init(struct drm_device *drm, struct sun4i_tcon *tcon)
{
	struct drm_encoder *encoder;
	struct drm_bridge *bridge;
=======
int sun4i_rgb_init(struct drm_device *drm, struct sun4i_tcon *tcon)
{
	struct drm_encoder *encoder;
>>>>>>> upstream/android-13
	struct sun4i_rgb *rgb;
	int ret;

	rgb = devm_kzalloc(drm->dev, sizeof(*rgb), GFP_KERNEL);
	if (!rgb)
		return -ENOMEM;
	rgb->tcon = tcon;
	encoder = &rgb->encoder;

	ret = drm_of_find_panel_or_bridge(tcon->dev->of_node, 1, 0,
<<<<<<< HEAD
					  &tcon->panel, &bridge);
=======
					  &rgb->panel, &rgb->bridge);
>>>>>>> upstream/android-13
	if (ret) {
		dev_info(drm->dev, "No panel or bridge found... RGB output disabled\n");
		return 0;
	}

	drm_encoder_helper_add(&rgb->encoder,
			       &sun4i_rgb_enc_helper_funcs);
<<<<<<< HEAD
	ret = drm_encoder_init(drm,
			       &rgb->encoder,
			       &sun4i_rgb_enc_funcs,
			       DRM_MODE_ENCODER_NONE,
			       NULL);
=======
	ret = drm_simple_encoder_init(drm, &rgb->encoder,
				      DRM_MODE_ENCODER_NONE);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(drm->dev, "Couldn't initialise the rgb encoder\n");
		goto err_out;
	}

	/* The RGB encoder can only work with the TCON channel 0 */
	rgb->encoder.possible_crtcs = drm_crtc_mask(&tcon->crtc->crtc);

<<<<<<< HEAD
	if (tcon->panel) {
=======
	if (rgb->panel) {
>>>>>>> upstream/android-13
		drm_connector_helper_add(&rgb->connector,
					 &sun4i_rgb_con_helper_funcs);
		ret = drm_connector_init(drm, &rgb->connector,
					 &sun4i_rgb_con_funcs,
					 DRM_MODE_CONNECTOR_Unknown);
		if (ret) {
			dev_err(drm->dev, "Couldn't initialise the rgb connector\n");
			goto err_cleanup_connector;
		}

		drm_connector_attach_encoder(&rgb->connector,
						  &rgb->encoder);
<<<<<<< HEAD

		ret = drm_panel_attach(tcon->panel, &rgb->connector);
		if (ret) {
			dev_err(drm->dev, "Couldn't attach our panel\n");
			goto err_cleanup_connector;
		}
	}

	if (bridge) {
		ret = drm_bridge_attach(encoder, bridge, NULL);
		if (ret) {
			dev_err(drm->dev, "Couldn't attach our bridge\n");
			goto err_cleanup_connector;
		}
=======
	}

	if (rgb->bridge) {
		ret = drm_bridge_attach(encoder, rgb->bridge, NULL, 0);
		if (ret)
			goto err_cleanup_connector;
>>>>>>> upstream/android-13
	}

	return 0;

err_cleanup_connector:
	drm_encoder_cleanup(&rgb->encoder);
err_out:
	return ret;
}
EXPORT_SYMBOL(sun4i_rgb_init);
