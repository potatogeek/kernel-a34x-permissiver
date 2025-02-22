<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * i.MX drm driver - parallel display implementation
 *
 * Copyright (C) 2012 Sascha Hauer, Pengutronix
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/component.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
#include <linux/videodev2.h>
#include <video/of_display_timing.h>

#include "imx-drm.h"

struct imx_parallel_display {
	struct drm_connector connector;
	struct drm_encoder encoder;
	struct device *dev;
	void *edid;
	int edid_len;
=======
#include <linux/platform_device.h>
#include <linux/videodev2.h>

#include <video/of_display_timing.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_of.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>

#include "imx-drm.h"

struct imx_parallel_display_encoder {
	struct drm_connector connector;
	struct drm_encoder encoder;
	struct drm_bridge bridge;
	struct imx_parallel_display *pd;
};

struct imx_parallel_display {
	struct device *dev;
	void *edid;
>>>>>>> upstream/android-13
	u32 bus_format;
	u32 bus_flags;
	struct drm_display_mode mode;
	struct drm_panel *panel;
<<<<<<< HEAD
	struct drm_bridge *bridge;
=======
	struct drm_bridge *next_bridge;
>>>>>>> upstream/android-13
};

static inline struct imx_parallel_display *con_to_imxpd(struct drm_connector *c)
{
<<<<<<< HEAD
	return container_of(c, struct imx_parallel_display, connector);
}

static inline struct imx_parallel_display *enc_to_imxpd(struct drm_encoder *e)
{
	return container_of(e, struct imx_parallel_display, encoder);
=======
	return container_of(c, struct imx_parallel_display_encoder, connector)->pd;
}

static inline struct imx_parallel_display *bridge_to_imxpd(struct drm_bridge *b)
{
	return container_of(b, struct imx_parallel_display_encoder, bridge)->pd;
>>>>>>> upstream/android-13
}

static int imx_pd_connector_get_modes(struct drm_connector *connector)
{
	struct imx_parallel_display *imxpd = con_to_imxpd(connector);
	struct device_node *np = imxpd->dev->of_node;
<<<<<<< HEAD
	int num_modes = 0;

	if (imxpd->panel && imxpd->panel->funcs &&
	    imxpd->panel->funcs->get_modes) {
		num_modes = imxpd->panel->funcs->get_modes(imxpd->panel);
		if (num_modes > 0)
			return num_modes;
	}
=======
	int num_modes;

	num_modes = drm_panel_get_modes(imxpd->panel, connector);
	if (num_modes > 0)
		return num_modes;
>>>>>>> upstream/android-13

	if (imxpd->edid) {
		drm_connector_update_edid_property(connector, imxpd->edid);
		num_modes = drm_add_edid_modes(connector, imxpd->edid);
	}

	if (np) {
		struct drm_display_mode *mode = drm_mode_create(connector->dev);
		int ret;

		if (!mode)
			return -EINVAL;

		ret = of_get_drm_display_mode(np, &imxpd->mode,
					      &imxpd->bus_flags,
					      OF_USE_NATIVE_MODE);
<<<<<<< HEAD
		if (ret)
			return ret;

		drm_mode_copy(mode, &imxpd->mode);
		mode->type |= DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
=======
		if (ret) {
			drm_mode_destroy(connector->dev, mode);
			return ret;
		}

		drm_mode_copy(mode, &imxpd->mode);
		mode->type |= DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
>>>>>>> upstream/android-13
		drm_mode_probed_add(connector, mode);
		num_modes++;
	}

	return num_modes;
}

<<<<<<< HEAD
static struct drm_encoder *imx_pd_connector_best_encoder(
		struct drm_connector *connector)
{
	struct imx_parallel_display *imxpd = con_to_imxpd(connector);

	return &imxpd->encoder;
}

static void imx_pd_encoder_enable(struct drm_encoder *encoder)
{
	struct imx_parallel_display *imxpd = enc_to_imxpd(encoder);
=======
static void imx_pd_bridge_enable(struct drm_bridge *bridge)
{
	struct imx_parallel_display *imxpd = bridge_to_imxpd(bridge);
>>>>>>> upstream/android-13

	drm_panel_prepare(imxpd->panel);
	drm_panel_enable(imxpd->panel);
}

<<<<<<< HEAD
static void imx_pd_encoder_disable(struct drm_encoder *encoder)
{
	struct imx_parallel_display *imxpd = enc_to_imxpd(encoder);
=======
static void imx_pd_bridge_disable(struct drm_bridge *bridge)
{
	struct imx_parallel_display *imxpd = bridge_to_imxpd(bridge);
>>>>>>> upstream/android-13

	drm_panel_disable(imxpd->panel);
	drm_panel_unprepare(imxpd->panel);
}

<<<<<<< HEAD
static int imx_pd_encoder_atomic_check(struct drm_encoder *encoder,
				       struct drm_crtc_state *crtc_state,
				       struct drm_connector_state *conn_state)
{
	struct imx_crtc_state *imx_crtc_state = to_imx_crtc_state(crtc_state);
	struct drm_display_info *di = &conn_state->connector->display_info;
	struct imx_parallel_display *imxpd = enc_to_imxpd(encoder);

	if (!imxpd->bus_format && di->num_bus_formats) {
		imx_crtc_state->bus_flags = di->bus_flags;
		imx_crtc_state->bus_format = di->bus_formats[0];
	} else {
		imx_crtc_state->bus_flags = imxpd->bus_flags;
		imx_crtc_state->bus_format = imxpd->bus_format;
	}
=======
static const u32 imx_pd_bus_fmts[] = {
	MEDIA_BUS_FMT_RGB888_1X24,
	MEDIA_BUS_FMT_BGR888_1X24,
	MEDIA_BUS_FMT_GBR888_1X24,
	MEDIA_BUS_FMT_RGB666_1X18,
	MEDIA_BUS_FMT_RGB666_1X24_CPADHI,
	MEDIA_BUS_FMT_RGB565_1X16,
};

static u32 *
imx_pd_bridge_atomic_get_output_bus_fmts(struct drm_bridge *bridge,
					 struct drm_bridge_state *bridge_state,
					 struct drm_crtc_state *crtc_state,
					 struct drm_connector_state *conn_state,
					 unsigned int *num_output_fmts)
{
	struct drm_display_info *di = &conn_state->connector->display_info;
	struct imx_parallel_display *imxpd = bridge_to_imxpd(bridge);
	u32 *output_fmts;

	if (!imxpd->bus_format && !di->num_bus_formats) {
		*num_output_fmts = ARRAY_SIZE(imx_pd_bus_fmts);
		return kmemdup(imx_pd_bus_fmts, sizeof(imx_pd_bus_fmts),
			       GFP_KERNEL);
	}

	*num_output_fmts = 1;
	output_fmts = kmalloc(sizeof(*output_fmts), GFP_KERNEL);
	if (!output_fmts)
		return NULL;

	if (!imxpd->bus_format && di->num_bus_formats)
		output_fmts[0] = di->bus_formats[0];
	else
		output_fmts[0] = imxpd->bus_format;

	return output_fmts;
}

static bool imx_pd_format_supported(u32 output_fmt)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(imx_pd_bus_fmts); i++) {
		if (imx_pd_bus_fmts[i] == output_fmt)
			return true;
	}

	return false;
}

static u32 *
imx_pd_bridge_atomic_get_input_bus_fmts(struct drm_bridge *bridge,
					struct drm_bridge_state *bridge_state,
					struct drm_crtc_state *crtc_state,
					struct drm_connector_state *conn_state,
					u32 output_fmt,
					unsigned int *num_input_fmts)
{
	struct imx_parallel_display *imxpd = bridge_to_imxpd(bridge);
	u32 *input_fmts;

	/*
	 * If the next bridge does not support bus format negotiation, let's
	 * use the static bus format definition (imxpd->bus_format) if it's
	 * specified, RGB888 when it's not.
	 */
	if (output_fmt == MEDIA_BUS_FMT_FIXED)
		output_fmt = imxpd->bus_format ? : MEDIA_BUS_FMT_RGB888_1X24;

	/* Now make sure the requested output format is supported. */
	if ((imxpd->bus_format && imxpd->bus_format != output_fmt) ||
	    !imx_pd_format_supported(output_fmt)) {
		*num_input_fmts = 0;
		return NULL;
	}

	*num_input_fmts = 1;
	input_fmts = kmalloc(sizeof(*input_fmts), GFP_KERNEL);
	if (!input_fmts)
		return NULL;

	input_fmts[0] = output_fmt;
	return input_fmts;
}

static int imx_pd_bridge_atomic_check(struct drm_bridge *bridge,
				      struct drm_bridge_state *bridge_state,
				      struct drm_crtc_state *crtc_state,
				      struct drm_connector_state *conn_state)
{
	struct imx_crtc_state *imx_crtc_state = to_imx_crtc_state(crtc_state);
	struct drm_display_info *di = &conn_state->connector->display_info;
	struct imx_parallel_display *imxpd = bridge_to_imxpd(bridge);
	struct drm_bridge_state *next_bridge_state = NULL;
	struct drm_bridge *next_bridge;
	u32 bus_flags, bus_fmt;

	next_bridge = drm_bridge_get_next_bridge(bridge);
	if (next_bridge)
		next_bridge_state = drm_atomic_get_new_bridge_state(crtc_state->state,
								    next_bridge);

	if (next_bridge_state)
		bus_flags = next_bridge_state->input_bus_cfg.flags;
	else if (di->num_bus_formats)
		bus_flags = di->bus_flags;
	else
		bus_flags = imxpd->bus_flags;

	bus_fmt = bridge_state->input_bus_cfg.format;
	if (!imx_pd_format_supported(bus_fmt))
		return -EINVAL;

	bridge_state->output_bus_cfg.flags = bus_flags;
	bridge_state->input_bus_cfg.flags = bus_flags;
	imx_crtc_state->bus_flags = bus_flags;
	imx_crtc_state->bus_format = bridge_state->input_bus_cfg.format;
>>>>>>> upstream/android-13
	imx_crtc_state->di_hsync_pin = 2;
	imx_crtc_state->di_vsync_pin = 3;

	return 0;
}

static const struct drm_connector_funcs imx_pd_connector_funcs = {
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = imx_drm_connector_destroy,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static const struct drm_connector_helper_funcs imx_pd_connector_helper_funcs = {
	.get_modes = imx_pd_connector_get_modes,
<<<<<<< HEAD
	.best_encoder = imx_pd_connector_best_encoder,
};

static const struct drm_encoder_funcs imx_pd_encoder_funcs = {
	.destroy = imx_drm_encoder_destroy,
};

static const struct drm_encoder_helper_funcs imx_pd_encoder_helper_funcs = {
	.enable = imx_pd_encoder_enable,
	.disable = imx_pd_encoder_disable,
	.atomic_check = imx_pd_encoder_atomic_check,
};

static int imx_pd_register(struct drm_device *drm,
	struct imx_parallel_display *imxpd)
{
	struct drm_encoder *encoder = &imxpd->encoder;
	int ret;

=======
};

static const struct drm_bridge_funcs imx_pd_bridge_funcs = {
	.enable = imx_pd_bridge_enable,
	.disable = imx_pd_bridge_disable,
	.atomic_reset = drm_atomic_helper_bridge_reset,
	.atomic_duplicate_state = drm_atomic_helper_bridge_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_bridge_destroy_state,
	.atomic_check = imx_pd_bridge_atomic_check,
	.atomic_get_input_bus_fmts = imx_pd_bridge_atomic_get_input_bus_fmts,
	.atomic_get_output_bus_fmts = imx_pd_bridge_atomic_get_output_bus_fmts,
};

static int imx_pd_bind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *drm = data;
	struct imx_parallel_display *imxpd = dev_get_drvdata(dev);
	struct imx_parallel_display_encoder *imxpd_encoder;
	struct drm_connector *connector;
	struct drm_encoder *encoder;
	struct drm_bridge *bridge;
	int ret;

	imxpd_encoder = drmm_simple_encoder_alloc(drm, struct imx_parallel_display_encoder,
						  encoder, DRM_MODE_ENCODER_NONE);
	if (IS_ERR(imxpd_encoder))
		return PTR_ERR(imxpd_encoder);

	imxpd_encoder->pd = imxpd;
	connector = &imxpd_encoder->connector;
	encoder = &imxpd_encoder->encoder;
	bridge = &imxpd_encoder->bridge;

>>>>>>> upstream/android-13
	ret = imx_drm_encoder_parse_of(drm, encoder, imxpd->dev->of_node);
	if (ret)
		return ret;

	/* set the connector's dpms to OFF so that
	 * drm_helper_connector_dpms() won't return
	 * immediately since the current state is ON
	 * at this point.
	 */
<<<<<<< HEAD
	imxpd->connector.dpms = DRM_MODE_DPMS_OFF;

	drm_encoder_helper_add(encoder, &imx_pd_encoder_helper_funcs);
	drm_encoder_init(drm, encoder, &imx_pd_encoder_funcs,
			 DRM_MODE_ENCODER_NONE, NULL);

	if (!imxpd->bridge) {
		drm_connector_helper_add(&imxpd->connector,
				&imx_pd_connector_helper_funcs);
		drm_connector_init(drm, &imxpd->connector,
				   &imx_pd_connector_funcs,
				   DRM_MODE_CONNECTOR_DPI);
	}

	if (imxpd->panel)
		drm_panel_attach(imxpd->panel, &imxpd->connector);

	if (imxpd->bridge) {
		ret = drm_bridge_attach(encoder, imxpd->bridge, NULL);
		if (ret < 0) {
			dev_err(imxpd->dev, "failed to attach bridge: %d\n",
				ret);
			return ret;
		}
	} else {
		drm_connector_attach_encoder(&imxpd->connector, encoder);
=======
	connector->dpms = DRM_MODE_DPMS_OFF;

	bridge->funcs = &imx_pd_bridge_funcs;
	drm_bridge_attach(encoder, bridge, NULL, 0);

	if (imxpd->next_bridge) {
		ret = drm_bridge_attach(encoder, imxpd->next_bridge, bridge, 0);
		if (ret < 0)
			return ret;
	} else {
		drm_connector_helper_add(connector,
					 &imx_pd_connector_helper_funcs);
		drm_connector_init(drm, connector, &imx_pd_connector_funcs,
				   DRM_MODE_CONNECTOR_DPI);

		drm_connector_attach_encoder(connector, encoder);
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static int imx_pd_bind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *drm = data;
	struct device_node *np = dev->of_node;
	const u8 *edidp;
	struct imx_parallel_display *imxpd;
=======
static const struct component_ops imx_pd_ops = {
	.bind	= imx_pd_bind,
};

static int imx_pd_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	const u8 *edidp;
	struct imx_parallel_display *imxpd;
	int edid_len;
>>>>>>> upstream/android-13
	int ret;
	u32 bus_format = 0;
	const char *fmt;

	imxpd = devm_kzalloc(dev, sizeof(*imxpd), GFP_KERNEL);
	if (!imxpd)
		return -ENOMEM;

<<<<<<< HEAD
	edidp = of_get_property(np, "edid", &imxpd->edid_len);
	if (edidp)
		imxpd->edid = kmemdup(edidp, imxpd->edid_len, GFP_KERNEL);
=======
	/* port@1 is the output port */
	ret = drm_of_find_panel_or_bridge(np, 1, 0, &imxpd->panel,
					  &imxpd->next_bridge);
	if (ret && ret != -ENODEV)
		return ret;

	edidp = of_get_property(np, "edid", &edid_len);
	if (edidp)
		imxpd->edid = devm_kmemdup(dev, edidp, edid_len, GFP_KERNEL);
>>>>>>> upstream/android-13

	ret = of_property_read_string(np, "interface-pix-fmt", &fmt);
	if (!ret) {
		if (!strcmp(fmt, "rgb24"))
			bus_format = MEDIA_BUS_FMT_RGB888_1X24;
		else if (!strcmp(fmt, "rgb565"))
			bus_format = MEDIA_BUS_FMT_RGB565_1X16;
		else if (!strcmp(fmt, "bgr666"))
			bus_format = MEDIA_BUS_FMT_RGB666_1X18;
		else if (!strcmp(fmt, "lvds666"))
			bus_format = MEDIA_BUS_FMT_RGB666_1X24_CPADHI;
	}
	imxpd->bus_format = bus_format;

<<<<<<< HEAD
	/* port@1 is the output port */
	ret = drm_of_find_panel_or_bridge(np, 1, 0, &imxpd->panel, &imxpd->bridge);
	if (ret && ret != -ENODEV)
		return ret;

	imxpd->dev = dev;

	ret = imx_pd_register(drm, imxpd);
	if (ret)
		return ret;

	dev_set_drvdata(dev, imxpd);

	return 0;
}

static void imx_pd_unbind(struct device *dev, struct device *master,
	void *data)
{
	struct imx_parallel_display *imxpd = dev_get_drvdata(dev);

	if (imxpd->panel)
		drm_panel_detach(imxpd->panel);

	kfree(imxpd->edid);
}

static const struct component_ops imx_pd_ops = {
	.bind	= imx_pd_bind,
	.unbind	= imx_pd_unbind,
};

static int imx_pd_probe(struct platform_device *pdev)
{
	return component_add(&pdev->dev, &imx_pd_ops);
=======
	imxpd->dev = dev;

	platform_set_drvdata(pdev, imxpd);

	return component_add(dev, &imx_pd_ops);
>>>>>>> upstream/android-13
}

static int imx_pd_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &imx_pd_ops);

	return 0;
}

static const struct of_device_id imx_pd_dt_ids[] = {
	{ .compatible = "fsl,imx-parallel-display", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, imx_pd_dt_ids);

static struct platform_driver imx_pd_driver = {
	.probe		= imx_pd_probe,
	.remove		= imx_pd_remove,
	.driver		= {
		.of_match_table = imx_pd_dt_ids,
		.name	= "imx-parallel-display",
	},
};

module_platform_driver(imx_pd_driver);

MODULE_DESCRIPTION("i.MX parallel display driver");
MODULE_AUTHOR("Sascha Hauer, Pengutronix");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:imx-parallel-display");
