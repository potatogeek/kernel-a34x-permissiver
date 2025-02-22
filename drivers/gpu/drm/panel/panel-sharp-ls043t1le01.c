<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
*/

#include <linux/backlight.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2015 Red Hat
 * Copyright (C) 2015 Sony Mobile Communications Inc.
 * Author: Werner Johansson <werner.johansson@sonymobile.com>
 *
 * Based on AUO panel driver by Rob Clark <robdclark@gmail.com>
 */

#include <linux/delay.h>
>>>>>>> upstream/android-13
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regulator/consumer.h>

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>

#include <video/mipi_display.h>

=======
#include <video/mipi_display.h>

#include <drm/drm_crtc.h>
#include <drm/drm_device.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>

>>>>>>> upstream/android-13
struct sharp_nt_panel {
	struct drm_panel base;
	struct mipi_dsi_device *dsi;

<<<<<<< HEAD
	struct backlight_device *backlight;
=======
>>>>>>> upstream/android-13
	struct regulator *supply;
	struct gpio_desc *reset_gpio;

	bool prepared;
	bool enabled;

	const struct drm_display_mode *mode;
};

static inline struct sharp_nt_panel *to_sharp_nt_panel(struct drm_panel *panel)
{
	return container_of(panel, struct sharp_nt_panel, base);
}

static int sharp_nt_panel_init(struct sharp_nt_panel *sharp_nt)
{
	struct mipi_dsi_device *dsi = sharp_nt->dsi;
	int ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret < 0)
		return ret;

	msleep(120);

	/* Novatek two-lane operation */
	ret = mipi_dsi_dcs_write(dsi, 0xae, (u8[]){ 0x03 }, 1);
	if (ret < 0)
		return ret;

	/* Set both MCU and RGB I/F to 24bpp */
	ret = mipi_dsi_dcs_set_pixel_format(dsi, MIPI_DCS_PIXEL_FMT_24BIT |
					(MIPI_DCS_PIXEL_FMT_24BIT << 4));
	if (ret < 0)
		return ret;

	return 0;
}

static int sharp_nt_panel_on(struct sharp_nt_panel *sharp_nt)
{
	struct mipi_dsi_device *dsi = sharp_nt->dsi;
	int ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret < 0)
		return ret;

	return 0;
}

static int sharp_nt_panel_off(struct sharp_nt_panel *sharp_nt)
{
	struct mipi_dsi_device *dsi = sharp_nt->dsi;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_off(dsi);
	if (ret < 0)
		return ret;

	ret = mipi_dsi_dcs_enter_sleep_mode(dsi);
	if (ret < 0)
		return ret;

	return 0;
}


static int sharp_nt_panel_disable(struct drm_panel *panel)
{
	struct sharp_nt_panel *sharp_nt = to_sharp_nt_panel(panel);

	if (!sharp_nt->enabled)
		return 0;

<<<<<<< HEAD
	if (sharp_nt->backlight) {
		sharp_nt->backlight->props.power = FB_BLANK_POWERDOWN;
		backlight_update_status(sharp_nt->backlight);
	}

=======
>>>>>>> upstream/android-13
	sharp_nt->enabled = false;

	return 0;
}

static int sharp_nt_panel_unprepare(struct drm_panel *panel)
{
	struct sharp_nt_panel *sharp_nt = to_sharp_nt_panel(panel);
	int ret;

	if (!sharp_nt->prepared)
		return 0;

	ret = sharp_nt_panel_off(sharp_nt);
	if (ret < 0) {
		dev_err(panel->dev, "failed to set panel off: %d\n", ret);
		return ret;
	}

	regulator_disable(sharp_nt->supply);
	if (sharp_nt->reset_gpio)
		gpiod_set_value(sharp_nt->reset_gpio, 0);

	sharp_nt->prepared = false;

	return 0;
}

static int sharp_nt_panel_prepare(struct drm_panel *panel)
{
	struct sharp_nt_panel *sharp_nt = to_sharp_nt_panel(panel);
	int ret;

	if (sharp_nt->prepared)
		return 0;

	ret = regulator_enable(sharp_nt->supply);
	if (ret < 0)
		return ret;

	msleep(20);

	if (sharp_nt->reset_gpio) {
		gpiod_set_value(sharp_nt->reset_gpio, 1);
		msleep(1);
		gpiod_set_value(sharp_nt->reset_gpio, 0);
		msleep(1);
		gpiod_set_value(sharp_nt->reset_gpio, 1);
		msleep(10);
	}

	ret = sharp_nt_panel_init(sharp_nt);
	if (ret < 0) {
		dev_err(panel->dev, "failed to init panel: %d\n", ret);
		goto poweroff;
	}

	ret = sharp_nt_panel_on(sharp_nt);
	if (ret < 0) {
		dev_err(panel->dev, "failed to set panel on: %d\n", ret);
		goto poweroff;
	}

	sharp_nt->prepared = true;

	return 0;

poweroff:
	regulator_disable(sharp_nt->supply);
	if (sharp_nt->reset_gpio)
		gpiod_set_value(sharp_nt->reset_gpio, 0);
	return ret;
}

static int sharp_nt_panel_enable(struct drm_panel *panel)
{
	struct sharp_nt_panel *sharp_nt = to_sharp_nt_panel(panel);

	if (sharp_nt->enabled)
		return 0;

<<<<<<< HEAD
	if (sharp_nt->backlight) {
		sharp_nt->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(sharp_nt->backlight);
	}

=======
>>>>>>> upstream/android-13
	sharp_nt->enabled = true;

	return 0;
}

static const struct drm_display_mode default_mode = {
	.clock = 41118,
	.hdisplay = 540,
	.hsync_start = 540 + 48,
	.hsync_end = 540 + 48 + 80,
	.htotal = 540 + 48 + 80 + 32,
	.vdisplay = 960,
	.vsync_start = 960 + 3,
	.vsync_end = 960 + 3 + 15,
	.vtotal = 960 + 3 + 15 + 1,
<<<<<<< HEAD
	.vrefresh = 60,
};

static int sharp_nt_panel_get_modes(struct drm_panel *panel)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(panel->drm, &default_mode);
	if (!mode) {
		dev_err(panel->drm->dev, "failed to add mode %ux%ux@%u\n",
				default_mode.hdisplay, default_mode.vdisplay,
				default_mode.vrefresh);
=======
};

static int sharp_nt_panel_get_modes(struct drm_panel *panel,
				    struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &default_mode);
	if (!mode) {
		dev_err(panel->dev, "failed to add mode %ux%u@%u\n",
			default_mode.hdisplay, default_mode.vdisplay,
			drm_mode_vrefresh(&default_mode));
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	drm_mode_set_name(mode);

<<<<<<< HEAD
	drm_mode_probed_add(panel->connector, mode);

	panel->connector->display_info.width_mm = 54;
	panel->connector->display_info.height_mm = 95;
=======
	drm_mode_probed_add(connector, mode);

	connector->display_info.width_mm = 54;
	connector->display_info.height_mm = 95;
>>>>>>> upstream/android-13

	return 1;
}

static const struct drm_panel_funcs sharp_nt_panel_funcs = {
	.disable = sharp_nt_panel_disable,
	.unprepare = sharp_nt_panel_unprepare,
	.prepare = sharp_nt_panel_prepare,
	.enable = sharp_nt_panel_enable,
	.get_modes = sharp_nt_panel_get_modes,
};

static int sharp_nt_panel_add(struct sharp_nt_panel *sharp_nt)
{
	struct device *dev = &sharp_nt->dsi->dev;
<<<<<<< HEAD
	struct device_node *np;
=======
>>>>>>> upstream/android-13
	int ret;

	sharp_nt->mode = &default_mode;

	sharp_nt->supply = devm_regulator_get(dev, "avdd");
	if (IS_ERR(sharp_nt->supply))
		return PTR_ERR(sharp_nt->supply);

	sharp_nt->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(sharp_nt->reset_gpio)) {
		dev_err(dev, "cannot get reset-gpios %ld\n",
			PTR_ERR(sharp_nt->reset_gpio));
		sharp_nt->reset_gpio = NULL;
	} else {
		gpiod_set_value(sharp_nt->reset_gpio, 0);
	}

<<<<<<< HEAD
	np = of_parse_phandle(dev->of_node, "backlight", 0);
	if (np) {
		sharp_nt->backlight = of_find_backlight_by_node(np);
		of_node_put(np);

		if (!sharp_nt->backlight)
			return -EPROBE_DEFER;
	}

	drm_panel_init(&sharp_nt->base);
	sharp_nt->base.funcs = &sharp_nt_panel_funcs;
	sharp_nt->base.dev = &sharp_nt->dsi->dev;

	ret = drm_panel_add(&sharp_nt->base);
	if (ret < 0)
		goto put_backlight;

	return 0;

put_backlight:
	if (sharp_nt->backlight)
		put_device(&sharp_nt->backlight->dev);

	return ret;
=======
	drm_panel_init(&sharp_nt->base, &sharp_nt->dsi->dev,
		       &sharp_nt_panel_funcs, DRM_MODE_CONNECTOR_DSI);

	ret = drm_panel_of_backlight(&sharp_nt->base);
	if (ret)
		return ret;

	drm_panel_add(&sharp_nt->base);

	return 0;
>>>>>>> upstream/android-13
}

static void sharp_nt_panel_del(struct sharp_nt_panel *sharp_nt)
{
	if (sharp_nt->base.dev)
		drm_panel_remove(&sharp_nt->base);
<<<<<<< HEAD

	if (sharp_nt->backlight)
		put_device(&sharp_nt->backlight->dev);
=======
>>>>>>> upstream/android-13
}

static int sharp_nt_panel_probe(struct mipi_dsi_device *dsi)
{
	struct sharp_nt_panel *sharp_nt;
	int ret;

	dsi->lanes = 2;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO |
			MIPI_DSI_MODE_VIDEO_HSE |
			MIPI_DSI_CLOCK_NON_CONTINUOUS |
<<<<<<< HEAD
			MIPI_DSI_MODE_EOT_PACKET;
=======
			MIPI_DSI_MODE_NO_EOT_PACKET;
>>>>>>> upstream/android-13

	sharp_nt = devm_kzalloc(&dsi->dev, sizeof(*sharp_nt), GFP_KERNEL);
	if (!sharp_nt)
		return -ENOMEM;

	mipi_dsi_set_drvdata(dsi, sharp_nt);

	sharp_nt->dsi = dsi;

	ret = sharp_nt_panel_add(sharp_nt);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	return mipi_dsi_attach(dsi);
=======
	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		sharp_nt_panel_del(sharp_nt);
		return ret;
	}

	return 0;
>>>>>>> upstream/android-13
}

static int sharp_nt_panel_remove(struct mipi_dsi_device *dsi)
{
	struct sharp_nt_panel *sharp_nt = mipi_dsi_get_drvdata(dsi);
	int ret;

<<<<<<< HEAD
	ret = sharp_nt_panel_disable(&sharp_nt->base);
=======
	ret = drm_panel_disable(&sharp_nt->base);
>>>>>>> upstream/android-13
	if (ret < 0)
		dev_err(&dsi->dev, "failed to disable panel: %d\n", ret);

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "failed to detach from DSI host: %d\n", ret);

<<<<<<< HEAD
	drm_panel_detach(&sharp_nt->base);
=======
>>>>>>> upstream/android-13
	sharp_nt_panel_del(sharp_nt);

	return 0;
}

static void sharp_nt_panel_shutdown(struct mipi_dsi_device *dsi)
{
	struct sharp_nt_panel *sharp_nt = mipi_dsi_get_drvdata(dsi);

<<<<<<< HEAD
	sharp_nt_panel_disable(&sharp_nt->base);
=======
	drm_panel_disable(&sharp_nt->base);
>>>>>>> upstream/android-13
}

static const struct of_device_id sharp_nt_of_match[] = {
	{ .compatible = "sharp,ls043t1le01-qhd", },
	{ }
};
MODULE_DEVICE_TABLE(of, sharp_nt_of_match);

static struct mipi_dsi_driver sharp_nt_panel_driver = {
	.driver = {
		.name = "panel-sharp-ls043t1le01-qhd",
		.of_match_table = sharp_nt_of_match,
	},
	.probe = sharp_nt_panel_probe,
	.remove = sharp_nt_panel_remove,
	.shutdown = sharp_nt_panel_shutdown,
};
module_mipi_dsi_driver(sharp_nt_panel_driver);

MODULE_AUTHOR("Werner Johansson <werner.johansson@sonymobile.com>");
MODULE_DESCRIPTION("Sharp LS043T1LE01 NT35565-based qHD (540x960) video mode panel driver");
MODULE_LICENSE("GPL v2");
