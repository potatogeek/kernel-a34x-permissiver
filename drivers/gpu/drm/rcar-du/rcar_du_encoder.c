<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * rcar_du_encoder.c  --  R-Car Display Unit Encoder
 *
 * Copyright (C) 2013-2014 Renesas Electronics Corporation
 *
 * Contact: Laurent Pinchart (laurent.pinchart@ideasonboard.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/export.h>

#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
=======
 */

#include <linux/export.h>
#include <linux/slab.h>

#include <drm/drm_bridge.h>
#include <drm/drm_bridge_connector.h>
#include <drm/drm_crtc.h>
#include <drm/drm_managed.h>
#include <drm/drm_modeset_helper_vtables.h>
>>>>>>> upstream/android-13
#include <drm/drm_panel.h>

#include "rcar_du_drv.h"
#include "rcar_du_encoder.h"
#include "rcar_du_kms.h"
<<<<<<< HEAD
=======
#include "rcar_lvds.h"
>>>>>>> upstream/android-13

/* -----------------------------------------------------------------------------
 * Encoder
 */

<<<<<<< HEAD
static void rcar_du_encoder_mode_set(struct drm_encoder *encoder,
				     struct drm_crtc_state *crtc_state,
				     struct drm_connector_state *conn_state)
{
	struct rcar_du_encoder *renc = to_rcar_encoder(encoder);

	rcar_du_crtc_route_output(crtc_state->crtc, renc->output);
}

static const struct drm_encoder_helper_funcs encoder_helper_funcs = {
	.atomic_mode_set = rcar_du_encoder_mode_set,
};

static const struct drm_encoder_funcs encoder_funcs = {
	.destroy = drm_encoder_cleanup,
=======
static unsigned int rcar_du_encoder_count_ports(struct device_node *node)
{
	struct device_node *ports;
	struct device_node *port;
	unsigned int num_ports = 0;

	ports = of_get_child_by_name(node, "ports");
	if (!ports)
		ports = of_node_get(node);

	for_each_child_of_node(ports, port) {
		if (of_node_name_eq(port, "port"))
			num_ports++;
	}

	of_node_put(ports);

	return num_ports;
}

static const struct drm_encoder_funcs rcar_du_encoder_funcs = {
>>>>>>> upstream/android-13
};

int rcar_du_encoder_init(struct rcar_du_device *rcdu,
			 enum rcar_du_output output,
<<<<<<< HEAD
			 struct device_node *enc_node,
			 struct device_node *con_node)
{
	struct rcar_du_encoder *renc;
	struct drm_encoder *encoder;
	struct drm_bridge *bridge = NULL;
	int ret;

	renc = devm_kzalloc(rcdu->dev, sizeof(*renc), GFP_KERNEL);
	if (renc == NULL)
		return -ENOMEM;

	renc->output = output;
	encoder = rcar_encoder_to_drm_encoder(renc);
=======
			 struct device_node *enc_node)
{
	struct rcar_du_encoder *renc;
	struct drm_connector *connector;
	struct drm_bridge *bridge;
	int ret;

	/*
	 * Locate the DRM bridge from the DT node. For the DPAD outputs, if the
	 * DT node has a single port, assume that it describes a panel and
	 * create a panel bridge.
	 */
	if ((output == RCAR_DU_OUTPUT_DPAD0 ||
	     output == RCAR_DU_OUTPUT_DPAD1) &&
	    rcar_du_encoder_count_ports(enc_node) == 1) {
		struct drm_panel *panel = of_drm_find_panel(enc_node);

		if (IS_ERR(panel))
			return PTR_ERR(panel);

		bridge = devm_drm_panel_bridge_add_typed(rcdu->dev, panel,
							 DRM_MODE_CONNECTOR_DPI);
		if (IS_ERR(bridge))
			return PTR_ERR(bridge);
	} else {
		bridge = of_drm_find_bridge(enc_node);
		if (!bridge)
			return -EPROBE_DEFER;

		if (output == RCAR_DU_OUTPUT_LVDS0 ||
		    output == RCAR_DU_OUTPUT_LVDS1)
			rcdu->lvds[output - RCAR_DU_OUTPUT_LVDS0] = bridge;
	}

	/*
	 * Create and initialize the encoder. On Gen3, skip the LVDS1 output if
	 * the LVDS1 encoder is used as a companion for LVDS0 in dual-link
	 * mode, or any LVDS output if it isn't connected. The latter may happen
	 * on D3 or E3 as the LVDS encoders are needed to provide the pixel
	 * clock to the DU, even when the LVDS outputs are not used.
	 */
	if (rcdu->info->gen >= 3) {
		if (output == RCAR_DU_OUTPUT_LVDS1 &&
		    rcar_lvds_dual_link(bridge))
			return -ENOLINK;

		if ((output == RCAR_DU_OUTPUT_LVDS0 ||
		     output == RCAR_DU_OUTPUT_LVDS1) &&
		    !rcar_lvds_is_connected(bridge))
			return -ENOLINK;
	}
>>>>>>> upstream/android-13

	dev_dbg(rcdu->dev, "initializing encoder %pOF for output %u\n",
		enc_node, output);

<<<<<<< HEAD
	/* Locate the DRM bridge from the encoder DT node. */
	bridge = of_drm_find_bridge(enc_node);
	if (!bridge) {
		ret = -EPROBE_DEFER;
		goto done;
	}

	ret = drm_encoder_init(rcdu->ddev, encoder, &encoder_funcs,
			       DRM_MODE_ENCODER_NONE, NULL);
	if (ret < 0)
		goto done;

	drm_encoder_helper_add(encoder, &encoder_helper_funcs);

	/*
	 * Attach the bridge to the encoder. The bridge will create the
	 * connector.
	 */
	ret = drm_bridge_attach(encoder, bridge, NULL);
	if (ret) {
		drm_encoder_cleanup(encoder);
		return ret;
	}

done:
	if (ret < 0) {
		if (encoder->name)
			encoder->funcs->destroy(encoder);
		devm_kfree(rcdu->dev, renc);
	}

	return ret;
=======
	renc = drmm_encoder_alloc(&rcdu->ddev, struct rcar_du_encoder, base,
				  &rcar_du_encoder_funcs, DRM_MODE_ENCODER_NONE,
				  NULL);
	if (!renc)
		return -ENOMEM;

	renc->output = output;

	/* Attach the bridge to the encoder. */
	ret = drm_bridge_attach(&renc->base, bridge, NULL,
				DRM_BRIDGE_ATTACH_NO_CONNECTOR);
	if (ret) {
		dev_err(rcdu->dev, "failed to attach bridge for output %u\n",
			output);
		return ret;
	}

	/* Create the connector for the chain of bridges. */
	connector = drm_bridge_connector_init(&rcdu->ddev, &renc->base);
	if (IS_ERR(connector)) {
		dev_err(rcdu->dev,
			"failed to created connector for output %u\n", output);
		return PTR_ERR(connector);
	}

	return drm_connector_attach_encoder(connector, &renc->base);
>>>>>>> upstream/android-13
}
