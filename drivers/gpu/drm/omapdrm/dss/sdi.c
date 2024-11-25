<<<<<<< HEAD
/*
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@ti.com>
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
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@ti.com>
>>>>>>> upstream/android-13
 */

#define DSS_SUBSYS_NAME "SDI"

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/regulator/consumer.h>
#include <linux/export.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/of.h>

#include "omapdss.h"
#include "dss.h"
=======
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>

#include <drm/drm_bridge.h>

#include "dss.h"
#include "omapdss.h"
>>>>>>> upstream/android-13

struct sdi_device {
	struct platform_device *pdev;
	struct dss_device *dss;

	bool update_enabled;
	struct regulator *vdds_sdi_reg;

	struct dss_lcd_mgr_config mgr_config;
<<<<<<< HEAD
	struct videomode vm;
	int datapairs;

	struct omap_dss_device output;
};

#define dssdev_to_sdi(dssdev) container_of(dssdev, struct sdi_device, output)
=======
	unsigned long pixelclock;
	int datapairs;

	struct omap_dss_device output;
	struct drm_bridge bridge;
};

#define drm_bridge_to_sdi(bridge) \
	container_of(bridge, struct sdi_device, bridge)
>>>>>>> upstream/android-13

struct sdi_clk_calc_ctx {
	struct sdi_device *sdi;
	unsigned long pck_min, pck_max;

	unsigned long fck;
	struct dispc_clock_info dispc_cinfo;
};

static bool dpi_calc_dispc_cb(int lckd, int pckd, unsigned long lck,
		unsigned long pck, void *data)
{
	struct sdi_clk_calc_ctx *ctx = data;

	ctx->dispc_cinfo.lck_div = lckd;
	ctx->dispc_cinfo.pck_div = pckd;
	ctx->dispc_cinfo.lck = lck;
	ctx->dispc_cinfo.pck = pck;

	return true;
}

static bool dpi_calc_dss_cb(unsigned long fck, void *data)
{
	struct sdi_clk_calc_ctx *ctx = data;

	ctx->fck = fck;

	return dispc_div_calc(ctx->sdi->dss->dispc, fck,
			      ctx->pck_min, ctx->pck_max,
			      dpi_calc_dispc_cb, ctx);
}

static int sdi_calc_clock_div(struct sdi_device *sdi, unsigned long pclk,
			      unsigned long *fck,
			      struct dispc_clock_info *dispc_cinfo)
{
	int i;
	struct sdi_clk_calc_ctx ctx;

	/*
	 * DSS fclk gives us very few possibilities, so finding a good pixel
	 * clock may not be possible. We try multiple times to find the clock,
	 * each time widening the pixel clock range we look for, up to
	 * +/- 1MHz.
	 */

	for (i = 0; i < 10; ++i) {
		bool ok;

		memset(&ctx, 0, sizeof(ctx));

		ctx.sdi = sdi;

		if (pclk > 1000 * i * i * i)
			ctx.pck_min = max(pclk - 1000 * i * i * i, 0lu);
		else
			ctx.pck_min = 0;
		ctx.pck_max = pclk + 1000 * i * i * i;

		ok = dss_div_calc(sdi->dss, pclk, ctx.pck_min,
				  dpi_calc_dss_cb, &ctx);
		if (ok) {
			*fck = ctx.fck;
			*dispc_cinfo = ctx.dispc_cinfo;
			return 0;
		}
	}

	return -EINVAL;
}

static void sdi_config_lcd_manager(struct sdi_device *sdi)
{
	sdi->mgr_config.io_pad_mode = DSS_IO_PAD_MODE_BYPASS;

	sdi->mgr_config.stallmode = false;
	sdi->mgr_config.fifohandcheck = false;

	sdi->mgr_config.video_port_width = 24;
	sdi->mgr_config.lcden_sig_polarity = 1;

	dss_mgr_set_lcd_config(&sdi->output, &sdi->mgr_config);
}

<<<<<<< HEAD
static int sdi_display_enable(struct omap_dss_device *dssdev)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);
	struct videomode *vm = &sdi->vm;
	unsigned long fck;
	struct dispc_clock_info dispc_cinfo;
	unsigned long pck;
	int r;

	if (!sdi->output.dispc_channel_connected) {
		DSSERR("failed to enable display: no output/manager\n");
		return -ENODEV;
	}

	r = regulator_enable(sdi->vdds_sdi_reg);
	if (r)
		goto err_reg_enable;
=======
/* -----------------------------------------------------------------------------
 * DRM Bridge Operations
 */

static int sdi_bridge_attach(struct drm_bridge *bridge,
			     enum drm_bridge_attach_flags flags)
{
	struct sdi_device *sdi = drm_bridge_to_sdi(bridge);

	if (!(flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR))
		return -EINVAL;

	return drm_bridge_attach(bridge->encoder, sdi->output.next_bridge,
				 bridge, flags);
}

static enum drm_mode_status
sdi_bridge_mode_valid(struct drm_bridge *bridge,
		      const struct drm_display_info *info,
		      const struct drm_display_mode *mode)
{
	struct sdi_device *sdi = drm_bridge_to_sdi(bridge);
	unsigned long pixelclock = mode->clock * 1000;
	struct dispc_clock_info dispc_cinfo;
	unsigned long fck;
	int ret;

	if (pixelclock == 0)
		return MODE_NOCLOCK;

	ret = sdi_calc_clock_div(sdi, pixelclock, &fck, &dispc_cinfo);
	if (ret < 0)
		return MODE_CLOCK_RANGE;

	return MODE_OK;
}

static bool sdi_bridge_mode_fixup(struct drm_bridge *bridge,
				  const struct drm_display_mode *mode,
				  struct drm_display_mode *adjusted_mode)
{
	struct sdi_device *sdi = drm_bridge_to_sdi(bridge);
	unsigned long pixelclock = mode->clock * 1000;
	struct dispc_clock_info dispc_cinfo;
	unsigned long fck;
	unsigned long pck;
	int ret;

	ret = sdi_calc_clock_div(sdi, pixelclock, &fck, &dispc_cinfo);
	if (ret < 0)
		return false;

	pck = fck / dispc_cinfo.lck_div / dispc_cinfo.pck_div;

	if (pck != pixelclock)
		dev_dbg(&sdi->pdev->dev,
			"pixel clock adjusted from %lu Hz to %lu Hz\n",
			pixelclock, pck);

	adjusted_mode->clock = pck / 1000;

	return true;
}

static void sdi_bridge_mode_set(struct drm_bridge *bridge,
				const struct drm_display_mode *mode,
				const struct drm_display_mode *adjusted_mode)
{
	struct sdi_device *sdi = drm_bridge_to_sdi(bridge);

	sdi->pixelclock = adjusted_mode->clock * 1000;
}

static void sdi_bridge_enable(struct drm_bridge *bridge)
{
	struct sdi_device *sdi = drm_bridge_to_sdi(bridge);
	struct dispc_clock_info dispc_cinfo;
	unsigned long fck;
	int r;

	r = regulator_enable(sdi->vdds_sdi_reg);
	if (r)
		return;
>>>>>>> upstream/android-13

	r = dispc_runtime_get(sdi->dss->dispc);
	if (r)
		goto err_get_dispc;

<<<<<<< HEAD
	/* 15.5.9.1.2 */
	vm->flags |= DISPLAY_FLAGS_PIXDATA_POSEDGE | DISPLAY_FLAGS_SYNC_POSEDGE;

	r = sdi_calc_clock_div(sdi, vm->pixelclock, &fck, &dispc_cinfo);
=======
	r = sdi_calc_clock_div(sdi, sdi->pixelclock, &fck, &dispc_cinfo);
>>>>>>> upstream/android-13
	if (r)
		goto err_calc_clock_div;

	sdi->mgr_config.clock_info = dispc_cinfo;

<<<<<<< HEAD
	pck = fck / dispc_cinfo.lck_div / dispc_cinfo.pck_div;

	if (pck != vm->pixelclock) {
		DSSWARN("Could not find exact pixel clock. Requested %lu Hz, got %lu Hz\n",
			vm->pixelclock, pck);

		vm->pixelclock = pck;
	}


	dss_mgr_set_timings(&sdi->output, vm);

=======
>>>>>>> upstream/android-13
	r = dss_set_fck_rate(sdi->dss, fck);
	if (r)
		goto err_set_dss_clock_div;

	sdi_config_lcd_manager(sdi);

	/*
	 * LCLK and PCLK divisors are located in shadow registers, and we
	 * normally write them to DISPC registers when enabling the output.
	 * However, SDI uses pck-free as source clock for its PLL, and pck-free
	 * is affected by the divisors. And as we need the PLL before enabling
	 * the output, we need to write the divisors early.
	 *
	 * It seems just writing to the DISPC register is enough, and we don't
	 * need to care about the shadow register mechanism for pck-free. The
	 * exact reason for this is unknown.
	 */
	dispc_mgr_set_clock_div(sdi->dss->dispc, sdi->output.dispc_channel,
				&sdi->mgr_config.clock_info);

	dss_sdi_init(sdi->dss, sdi->datapairs);
	r = dss_sdi_enable(sdi->dss);
	if (r)
		goto err_sdi_enable;
	mdelay(2);

	r = dss_mgr_enable(&sdi->output);
	if (r)
		goto err_mgr_enable;

<<<<<<< HEAD
	return 0;
=======
	return;
>>>>>>> upstream/android-13

err_mgr_enable:
	dss_sdi_disable(sdi->dss);
err_sdi_enable:
err_set_dss_clock_div:
err_calc_clock_div:
	dispc_runtime_put(sdi->dss->dispc);
err_get_dispc:
	regulator_disable(sdi->vdds_sdi_reg);
<<<<<<< HEAD
err_reg_enable:
	return r;
}

static void sdi_display_disable(struct omap_dss_device *dssdev)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);
=======
}

static void sdi_bridge_disable(struct drm_bridge *bridge)
{
	struct sdi_device *sdi = drm_bridge_to_sdi(bridge);
>>>>>>> upstream/android-13

	dss_mgr_disable(&sdi->output);

	dss_sdi_disable(sdi->dss);

	dispc_runtime_put(sdi->dss->dispc);

	regulator_disable(sdi->vdds_sdi_reg);
}

<<<<<<< HEAD
static void sdi_set_timings(struct omap_dss_device *dssdev,
			    struct videomode *vm)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);

	sdi->vm = *vm;
}

static void sdi_get_timings(struct omap_dss_device *dssdev,
			    struct videomode *vm)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);

	*vm = sdi->vm;
}

static int sdi_check_timings(struct omap_dss_device *dssdev,
			     struct videomode *vm)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);
	enum omap_channel channel = dssdev->dispc_channel;

	if (!dispc_mgr_timings_ok(sdi->dss->dispc, channel, vm))
		return -EINVAL;

	if (vm->pixelclock == 0)
		return -EINVAL;

	return 0;
}

static int sdi_init_regulator(struct sdi_device *sdi)
{
	struct regulator *vdds_sdi;

	if (sdi->vdds_sdi_reg)
		return 0;

	vdds_sdi = devm_regulator_get(&sdi->pdev->dev, "vdds_sdi");
	if (IS_ERR(vdds_sdi)) {
		if (PTR_ERR(vdds_sdi) != -EPROBE_DEFER)
			DSSERR("can't get VDDS_SDI regulator\n");
		return PTR_ERR(vdds_sdi);
	}

	sdi->vdds_sdi_reg = vdds_sdi;

	return 0;
}

static int sdi_connect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);
	int r;

	r = sdi_init_regulator(sdi);
	if (r)
		return r;

	r = dss_mgr_connect(&sdi->output, dssdev);
	if (r)
		return r;

	r = omapdss_output_set_device(dssdev, dst);
	if (r) {
		DSSERR("failed to connect output to new device: %s\n",
				dst->name);
		dss_mgr_disconnect(&sdi->output, dssdev);
		return r;
	}

	return 0;
}

static void sdi_disconnect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct sdi_device *sdi = dssdev_to_sdi(dssdev);

	WARN_ON(dst != dssdev->dst);

	if (dst != dssdev->dst)
		return;

	omapdss_output_unset_device(dssdev);

	dss_mgr_disconnect(&sdi->output, dssdev);
}

static const struct omapdss_sdi_ops sdi_ops = {
	.connect = sdi_connect,
	.disconnect = sdi_disconnect,

	.enable = sdi_display_enable,
	.disable = sdi_display_disable,

	.check_timings = sdi_check_timings,
	.set_timings = sdi_set_timings,
	.get_timings = sdi_get_timings,
};

static void sdi_init_output(struct sdi_device *sdi)
{
	struct omap_dss_device *out = &sdi->output;

	out->dev = &sdi->pdev->dev;
	out->id = OMAP_DSS_OUTPUT_SDI;
	out->output_type = OMAP_DISPLAY_TYPE_SDI;
	out->name = "sdi.0";
	out->dispc_channel = OMAP_DSS_CHANNEL_LCD;
	/* We have SDI only on OMAP3, where it's on port 1 */
	out->port_num = 1;
	out->ops.sdi = &sdi_ops;
	out->owner = THIS_MODULE;

	omapdss_register_output(out);
=======
static const struct drm_bridge_funcs sdi_bridge_funcs = {
	.attach = sdi_bridge_attach,
	.mode_valid = sdi_bridge_mode_valid,
	.mode_fixup = sdi_bridge_mode_fixup,
	.mode_set = sdi_bridge_mode_set,
	.enable = sdi_bridge_enable,
	.disable = sdi_bridge_disable,
};

static void sdi_bridge_init(struct sdi_device *sdi)
{
	sdi->bridge.funcs = &sdi_bridge_funcs;
	sdi->bridge.of_node = sdi->pdev->dev.of_node;
	sdi->bridge.type = DRM_MODE_CONNECTOR_LVDS;

	drm_bridge_add(&sdi->bridge);
}

static void sdi_bridge_cleanup(struct sdi_device *sdi)
{
	drm_bridge_remove(&sdi->bridge);
}

/* -----------------------------------------------------------------------------
 * Initialisation and Cleanup
 */

static int sdi_init_output(struct sdi_device *sdi)
{
	struct omap_dss_device *out = &sdi->output;
	int r;

	sdi_bridge_init(sdi);

	out->dev = &sdi->pdev->dev;
	out->id = OMAP_DSS_OUTPUT_SDI;
	out->type = OMAP_DISPLAY_TYPE_SDI;
	out->name = "sdi.0";
	out->dispc_channel = OMAP_DSS_CHANNEL_LCD;
	/* We have SDI only on OMAP3, where it's on port 1 */
	out->of_port = 1;
	out->bus_flags = DRM_BUS_FLAG_PIXDATA_DRIVE_POSEDGE	/* 15.5.9.1.2 */
		       | DRM_BUS_FLAG_SYNC_DRIVE_POSEDGE;

	r = omapdss_device_init_output(out, &sdi->bridge);
	if (r < 0) {
		sdi_bridge_cleanup(sdi);
		return r;
	}

	omapdss_device_register(out);

	return 0;
>>>>>>> upstream/android-13
}

static void sdi_uninit_output(struct sdi_device *sdi)
{
<<<<<<< HEAD
	omapdss_unregister_output(&sdi->output);
=======
	omapdss_device_unregister(&sdi->output);
	omapdss_device_cleanup_output(&sdi->output);

	sdi_bridge_cleanup(sdi);
>>>>>>> upstream/android-13
}

int sdi_init_port(struct dss_device *dss, struct platform_device *pdev,
		  struct device_node *port)
{
	struct sdi_device *sdi;
	struct device_node *ep;
	u32 datapairs;
	int r;

	sdi = kzalloc(sizeof(*sdi), GFP_KERNEL);
	if (!sdi)
		return -ENOMEM;

	ep = of_get_next_child(port, NULL);
	if (!ep) {
		r = 0;
		goto err_free;
	}

	r = of_property_read_u32(ep, "datapairs", &datapairs);
<<<<<<< HEAD
	if (r) {
		DSSERR("failed to parse datapairs\n");
		goto err_datapairs;
=======
	of_node_put(ep);
	if (r) {
		DSSERR("failed to parse datapairs\n");
		goto err_free;
>>>>>>> upstream/android-13
	}

	sdi->datapairs = datapairs;
	sdi->dss = dss;

<<<<<<< HEAD
	of_node_put(ep);

	sdi->pdev = pdev;
	port->data = sdi;

	sdi_init_output(sdi);

	return 0;

err_datapairs:
	of_node_put(ep);
=======
	sdi->pdev = pdev;
	port->data = sdi;

	sdi->vdds_sdi_reg = devm_regulator_get(&pdev->dev, "vdds_sdi");
	if (IS_ERR(sdi->vdds_sdi_reg)) {
		r = PTR_ERR(sdi->vdds_sdi_reg);
		if (r != -EPROBE_DEFER)
			DSSERR("can't get VDDS_SDI regulator\n");
		goto err_free;
	}

	r = sdi_init_output(sdi);
	if (r)
		goto err_free;

	return 0;

>>>>>>> upstream/android-13
err_free:
	kfree(sdi);

	return r;
}

void sdi_uninit_port(struct device_node *port)
{
	struct sdi_device *sdi = port->data;

	if (!sdi)
		return;

	sdi_uninit_output(sdi);
	kfree(sdi);
}
