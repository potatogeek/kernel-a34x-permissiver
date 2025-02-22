<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2017 NXP Semiconductors.
 * Author: Marco Franchi <marco.franchi@nxp.com>
 *
 * Based on Panel Simple driver by Thierry Reding <treding@nvidia.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 */

#include <linux/backlight.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regulator/consumer.h>

#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_panel.h>

#include <video/display_timing.h>
#include <video/videomode.h>

=======
 */

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#include <video/display_timing.h>
#include <video/videomode.h>

#include <drm/drm_crtc.h>
#include <drm/drm_device.h>
#include <drm/drm_panel.h>

>>>>>>> upstream/android-13
struct seiko_panel_desc {
	const struct drm_display_mode *modes;
	unsigned int num_modes;
	const struct display_timing *timings;
	unsigned int num_timings;

	unsigned int bpc;

	/**
	 * @width: width (in millimeters) of the panel's active display area
	 * @height: height (in millimeters) of the panel's active display area
	 */
	struct {
		unsigned int width;
		unsigned int height;
	} size;

	u32 bus_format;
	u32 bus_flags;
};

struct seiko_panel {
	struct drm_panel base;
	bool prepared;
	bool enabled;
	const struct seiko_panel_desc *desc;
<<<<<<< HEAD
	struct backlight_device *backlight;
=======
>>>>>>> upstream/android-13
	struct regulator *dvdd;
	struct regulator *avdd;
};

static inline struct seiko_panel *to_seiko_panel(struct drm_panel *panel)
{
	return container_of(panel, struct seiko_panel, base);
}

<<<<<<< HEAD
static int seiko_panel_get_fixed_modes(struct seiko_panel *panel)
{
	struct drm_connector *connector = panel->base.connector;
	struct drm_device *drm = panel->base.drm;
=======
static int seiko_panel_get_fixed_modes(struct seiko_panel *panel,
				       struct drm_connector *connector)
{
>>>>>>> upstream/android-13
	struct drm_display_mode *mode;
	unsigned int i, num = 0;

	if (!panel->desc)
		return 0;

	for (i = 0; i < panel->desc->num_timings; i++) {
		const struct display_timing *dt = &panel->desc->timings[i];
		struct videomode vm;

		videomode_from_timing(dt, &vm);
<<<<<<< HEAD
		mode = drm_mode_create(drm);
		if (!mode) {
			dev_err(drm->dev, "failed to add mode %ux%u\n",
=======
		mode = drm_mode_create(connector->dev);
		if (!mode) {
			dev_err(panel->base.dev, "failed to add mode %ux%u\n",
>>>>>>> upstream/android-13
				dt->hactive.typ, dt->vactive.typ);
			continue;
		}

		drm_display_mode_from_videomode(&vm, mode);

		mode->type |= DRM_MODE_TYPE_DRIVER;

		if (panel->desc->num_timings == 1)
			mode->type |= DRM_MODE_TYPE_PREFERRED;

		drm_mode_probed_add(connector, mode);
		num++;
	}

	for (i = 0; i < panel->desc->num_modes; i++) {
		const struct drm_display_mode *m = &panel->desc->modes[i];

<<<<<<< HEAD
		mode = drm_mode_duplicate(drm, m);
		if (!mode) {
			dev_err(drm->dev, "failed to add mode %ux%u@%u\n",
				m->hdisplay, m->vdisplay, m->vrefresh);
=======
		mode = drm_mode_duplicate(connector->dev, m);
		if (!mode) {
			dev_err(panel->base.dev, "failed to add mode %ux%u@%u\n",
				m->hdisplay, m->vdisplay,
				drm_mode_vrefresh(m));
>>>>>>> upstream/android-13
			continue;
		}

		mode->type |= DRM_MODE_TYPE_DRIVER;

		if (panel->desc->num_modes == 1)
			mode->type |= DRM_MODE_TYPE_PREFERRED;

		drm_mode_set_name(mode);

		drm_mode_probed_add(connector, mode);
		num++;
	}

	connector->display_info.bpc = panel->desc->bpc;
	connector->display_info.width_mm = panel->desc->size.width;
	connector->display_info.height_mm = panel->desc->size.height;
	if (panel->desc->bus_format)
		drm_display_info_set_bus_formats(&connector->display_info,
						 &panel->desc->bus_format, 1);
	connector->display_info.bus_flags = panel->desc->bus_flags;

	return num;
}

static int seiko_panel_disable(struct drm_panel *panel)
{
	struct seiko_panel *p = to_seiko_panel(panel);

	if (!p->enabled)
		return 0;

<<<<<<< HEAD
	if (p->backlight) {
		p->backlight->props.power = FB_BLANK_POWERDOWN;
		p->backlight->props.state |= BL_CORE_FBBLANK;
		backlight_update_status(p->backlight);
	}

=======
>>>>>>> upstream/android-13
	p->enabled = false;

	return 0;
}

static int seiko_panel_unprepare(struct drm_panel *panel)
{
	struct seiko_panel *p = to_seiko_panel(panel);

	if (!p->prepared)
		return 0;

	regulator_disable(p->avdd);

	/* Add a 100ms delay as per the panel datasheet */
	msleep(100);

	regulator_disable(p->dvdd);

	p->prepared = false;

	return 0;
}

static int seiko_panel_prepare(struct drm_panel *panel)
{
	struct seiko_panel *p = to_seiko_panel(panel);
	int err;

	if (p->prepared)
		return 0;

	err = regulator_enable(p->dvdd);
	if (err < 0) {
		dev_err(panel->dev, "failed to enable dvdd: %d\n", err);
		return err;
	}

	/* Add a 100ms delay as per the panel datasheet */
	msleep(100);

	err = regulator_enable(p->avdd);
	if (err < 0) {
		dev_err(panel->dev, "failed to enable avdd: %d\n", err);
		goto disable_dvdd;
	}

	p->prepared = true;

	return 0;

disable_dvdd:
	regulator_disable(p->dvdd);
	return err;
}

static int seiko_panel_enable(struct drm_panel *panel)
{
	struct seiko_panel *p = to_seiko_panel(panel);

	if (p->enabled)
		return 0;

<<<<<<< HEAD
	if (p->backlight) {
		p->backlight->props.state &= ~BL_CORE_FBBLANK;
		p->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(p->backlight);
	}

=======
>>>>>>> upstream/android-13
	p->enabled = true;

	return 0;
}

<<<<<<< HEAD
static int seiko_panel_get_modes(struct drm_panel *panel)
=======
static int seiko_panel_get_modes(struct drm_panel *panel,
				 struct drm_connector *connector)
>>>>>>> upstream/android-13
{
	struct seiko_panel *p = to_seiko_panel(panel);

	/* add hard-coded panel modes */
<<<<<<< HEAD
	return seiko_panel_get_fixed_modes(p);
=======
	return seiko_panel_get_fixed_modes(p, connector);
>>>>>>> upstream/android-13
}

static int seiko_panel_get_timings(struct drm_panel *panel,
				    unsigned int num_timings,
				    struct display_timing *timings)
{
	struct seiko_panel *p = to_seiko_panel(panel);
	unsigned int i;

	if (p->desc->num_timings < num_timings)
		num_timings = p->desc->num_timings;

	if (timings)
		for (i = 0; i < num_timings; i++)
			timings[i] = p->desc->timings[i];

	return p->desc->num_timings;
}

static const struct drm_panel_funcs seiko_panel_funcs = {
	.disable = seiko_panel_disable,
	.unprepare = seiko_panel_unprepare,
	.prepare = seiko_panel_prepare,
	.enable = seiko_panel_enable,
	.get_modes = seiko_panel_get_modes,
	.get_timings = seiko_panel_get_timings,
};

static int seiko_panel_probe(struct device *dev,
					const struct seiko_panel_desc *desc)
{
<<<<<<< HEAD
	struct device_node *backlight;
=======
>>>>>>> upstream/android-13
	struct seiko_panel *panel;
	int err;

	panel = devm_kzalloc(dev, sizeof(*panel), GFP_KERNEL);
	if (!panel)
		return -ENOMEM;

	panel->enabled = false;
	panel->prepared = false;
	panel->desc = desc;

	panel->dvdd = devm_regulator_get(dev, "dvdd");
	if (IS_ERR(panel->dvdd))
		return PTR_ERR(panel->dvdd);

	panel->avdd = devm_regulator_get(dev, "avdd");
	if (IS_ERR(panel->avdd))
		return PTR_ERR(panel->avdd);

<<<<<<< HEAD
	backlight = of_parse_phandle(dev->of_node, "backlight", 0);
	if (backlight) {
		panel->backlight = of_find_backlight_by_node(backlight);
		of_node_put(backlight);

		if (!panel->backlight)
			return -EPROBE_DEFER;
	}

	drm_panel_init(&panel->base);
	panel->base.dev = dev;
	panel->base.funcs = &seiko_panel_funcs;

	err = drm_panel_add(&panel->base);
	if (err < 0)
		return err;

=======
	drm_panel_init(&panel->base, dev, &seiko_panel_funcs,
		       DRM_MODE_CONNECTOR_DPI);

	err = drm_panel_of_backlight(&panel->base);
	if (err)
		return err;

	drm_panel_add(&panel->base);

>>>>>>> upstream/android-13
	dev_set_drvdata(dev, panel);

	return 0;
}

static int seiko_panel_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct seiko_panel *panel = dev_get_drvdata(&pdev->dev);

	drm_panel_remove(&panel->base);

	seiko_panel_disable(&panel->base);

	if (panel->backlight)
		put_device(&panel->backlight->dev);
=======
	struct seiko_panel *panel = platform_get_drvdata(pdev);

	drm_panel_remove(&panel->base);
	drm_panel_disable(&panel->base);
>>>>>>> upstream/android-13

	return 0;
}

static void seiko_panel_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct seiko_panel *panel = dev_get_drvdata(&pdev->dev);

	seiko_panel_disable(&panel->base);
=======
	struct seiko_panel *panel = platform_get_drvdata(pdev);

	drm_panel_disable(&panel->base);
>>>>>>> upstream/android-13
}

static const struct display_timing seiko_43wvf1g_timing = {
	.pixelclock = { 33500000, 33500000, 33500000 },
	.hactive = { 800, 800, 800 },
	.hfront_porch = {  164, 164, 164 },
	.hback_porch = { 89, 89, 89 },
	.hsync_len = { 10, 10, 10 },
	.vactive = { 480, 480, 480 },
	.vfront_porch = { 10, 10, 10 },
	.vback_porch = { 23, 23, 23 },
	.vsync_len = { 10, 10, 10 },
	.flags = DISPLAY_FLAGS_DE_LOW,
};

static const struct seiko_panel_desc seiko_43wvf1g = {
	.timings = &seiko_43wvf1g_timing,
	.num_timings = 1,
	.bpc = 8,
	.size = {
		.width = 93,
		.height = 57,
	},
	.bus_format = MEDIA_BUS_FMT_RGB888_1X24,
<<<<<<< HEAD
	.bus_flags = DRM_BUS_FLAG_DE_HIGH | DRM_BUS_FLAG_PIXDATA_NEGEDGE,
=======
	.bus_flags = DRM_BUS_FLAG_DE_HIGH | DRM_BUS_FLAG_PIXDATA_DRIVE_NEGEDGE,
>>>>>>> upstream/android-13
};

static const struct of_device_id platform_of_match[] = {
	{
		.compatible = "sii,43wvf1g",
		.data = &seiko_43wvf1g,
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, platform_of_match);

static int seiko_panel_platform_probe(struct platform_device *pdev)
{
	const struct of_device_id *id;

	id = of_match_node(platform_of_match, pdev->dev.of_node);
	if (!id)
		return -ENODEV;

	return seiko_panel_probe(&pdev->dev, id->data);
}

static struct platform_driver seiko_panel_platform_driver = {
	.driver = {
		.name = "seiko_panel",
		.of_match_table = platform_of_match,
	},
	.probe = seiko_panel_platform_probe,
	.remove = seiko_panel_remove,
	.shutdown = seiko_panel_shutdown,
};
module_platform_driver(seiko_panel_platform_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Marco Franchi <marco.franchi@nxp.com");
=======
MODULE_AUTHOR("Marco Franchi <marco.franchi@nxp.com>");
>>>>>>> upstream/android-13
MODULE_DESCRIPTION("Seiko 43WVF1G panel driver");
MODULE_LICENSE("GPL v2");
