// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017 Free Electrons
 * Maxime Ripard <maxime.ripard@free-electrons.com>
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
#include "sun4i_lvds.h"

struct sun4i_lvds {
	struct drm_connector	connector;
	struct drm_encoder	encoder;

<<<<<<< HEAD
	struct sun4i_tcon	*tcon;
=======
	struct drm_panel	*panel;
>>>>>>> upstream/android-13
};

static inline struct sun4i_lvds *
drm_connector_to_sun4i_lvds(struct drm_connector *connector)
{
	return container_of(connector, struct sun4i_lvds,
			    connector);
}

static inline struct sun4i_lvds *
drm_encoder_to_sun4i_lvds(struct drm_encoder *encoder)
{
	return container_of(encoder, struct sun4i_lvds,
			    encoder);
}

static int sun4i_lvds_get_modes(struct drm_connector *connector)
{
	struct sun4i_lvds *lvds =
		drm_connector_to_sun4i_lvds(connector);
<<<<<<< HEAD
	struct sun4i_tcon *tcon = lvds->tcon;

	return drm_panel_get_modes(tcon->panel);
}

static struct drm_connector_helper_funcs sun4i_lvds_con_helper_funcs = {
=======

	return drm_panel_get_modes(lvds->panel, connector);
}

static const struct drm_connector_helper_funcs sun4i_lvds_con_helper_funcs = {
>>>>>>> upstream/android-13
	.get_modes	= sun4i_lvds_get_modes,
};

static void
sun4i_lvds_connector_destroy(struct drm_connector *connector)
{
<<<<<<< HEAD
	struct sun4i_lvds *lvds = drm_connector_to_sun4i_lvds(connector);
	struct sun4i_tcon *tcon = lvds->tcon;

	drm_panel_detach(tcon->panel);
=======
>>>>>>> upstream/android-13
	drm_connector_cleanup(connector);
}

static const struct drm_connector_funcs sun4i_lvds_con_funcs = {
	.fill_modes		= drm_helper_probe_single_connector_modes,
	.destroy		= sun4i_lvds_connector_destroy,
	.reset			= drm_atomic_helper_connector_reset,
	.atomic_duplicate_state	= drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state	= drm_atomic_helper_connector_destroy_state,
};

static void sun4i_lvds_encoder_enable(struct drm_encoder *encoder)
{
	struct sun4i_lvds *lvds = drm_encoder_to_sun4i_lvds(encoder);
<<<<<<< HEAD
	struct sun4i_tcon *tcon = lvds->tcon;

	DRM_DEBUG_DRIVER("Enabling LVDS output\n");

	if (!IS_ERR(tcon->panel)) {
		drm_panel_prepare(tcon->panel);
		drm_panel_enable(tcon->panel);
=======

	DRM_DEBUG_DRIVER("Enabling LVDS output\n");

	if (lvds->panel) {
		drm_panel_prepare(lvds->panel);
		drm_panel_enable(lvds->panel);
>>>>>>> upstream/android-13
	}
}

static void sun4i_lvds_encoder_disable(struct drm_encoder *encoder)
{
	struct sun4i_lvds *lvds = drm_encoder_to_sun4i_lvds(encoder);
<<<<<<< HEAD
	struct sun4i_tcon *tcon = lvds->tcon;

	DRM_DEBUG_DRIVER("Disabling LVDS output\n");

	if (!IS_ERR(tcon->panel)) {
		drm_panel_disable(tcon->panel);
		drm_panel_unprepare(tcon->panel);
=======

	DRM_DEBUG_DRIVER("Disabling LVDS output\n");

	if (lvds->panel) {
		drm_panel_disable(lvds->panel);
		drm_panel_unprepare(lvds->panel);
>>>>>>> upstream/android-13
	}
}

static const struct drm_encoder_helper_funcs sun4i_lvds_enc_helper_funcs = {
	.disable	= sun4i_lvds_encoder_disable,
	.enable		= sun4i_lvds_encoder_enable,
};

<<<<<<< HEAD
static const struct drm_encoder_funcs sun4i_lvds_enc_funcs = {
	.destroy	= drm_encoder_cleanup,
};

=======
>>>>>>> upstream/android-13
int sun4i_lvds_init(struct drm_device *drm, struct sun4i_tcon *tcon)
{
	struct drm_encoder *encoder;
	struct drm_bridge *bridge;
	struct sun4i_lvds *lvds;
	int ret;

	lvds = devm_kzalloc(drm->dev, sizeof(*lvds), GFP_KERNEL);
	if (!lvds)
		return -ENOMEM;
<<<<<<< HEAD
	lvds->tcon = tcon;
	encoder = &lvds->encoder;

	ret = drm_of_find_panel_or_bridge(tcon->dev->of_node, 1, 0,
					  &tcon->panel, &bridge);
=======
	encoder = &lvds->encoder;

	ret = drm_of_find_panel_or_bridge(tcon->dev->of_node, 1, 0,
					  &lvds->panel, &bridge);
>>>>>>> upstream/android-13
	if (ret) {
		dev_info(drm->dev, "No panel or bridge found... LVDS output disabled\n");
		return 0;
	}

	drm_encoder_helper_add(&lvds->encoder,
			       &sun4i_lvds_enc_helper_funcs);
<<<<<<< HEAD
	ret = drm_encoder_init(drm,
			       &lvds->encoder,
			       &sun4i_lvds_enc_funcs,
			       DRM_MODE_ENCODER_LVDS,
			       NULL);
=======
	ret = drm_simple_encoder_init(drm, &lvds->encoder,
				      DRM_MODE_ENCODER_LVDS);
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(drm->dev, "Couldn't initialise the lvds encoder\n");
		goto err_out;
	}

	/* The LVDS encoder can only work with the TCON channel 0 */
	lvds->encoder.possible_crtcs = drm_crtc_mask(&tcon->crtc->crtc);

<<<<<<< HEAD
	if (tcon->panel) {
=======
	if (lvds->panel) {
>>>>>>> upstream/android-13
		drm_connector_helper_add(&lvds->connector,
					 &sun4i_lvds_con_helper_funcs);
		ret = drm_connector_init(drm, &lvds->connector,
					 &sun4i_lvds_con_funcs,
					 DRM_MODE_CONNECTOR_LVDS);
		if (ret) {
			dev_err(drm->dev, "Couldn't initialise the lvds connector\n");
			goto err_cleanup_connector;
		}

		drm_connector_attach_encoder(&lvds->connector,
						  &lvds->encoder);
<<<<<<< HEAD

		ret = drm_panel_attach(tcon->panel, &lvds->connector);
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

	if (bridge) {
		ret = drm_bridge_attach(encoder, bridge, NULL, 0);
		if (ret)
			goto err_cleanup_connector;
>>>>>>> upstream/android-13
	}

	return 0;

err_cleanup_connector:
	drm_encoder_cleanup(&lvds->encoder);
err_out:
	return ret;
}
EXPORT_SYMBOL(sun4i_lvds_init);
