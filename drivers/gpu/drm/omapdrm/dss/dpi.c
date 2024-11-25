<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@ti.com>
 *
 * Some code and ideas taken from drivers/video/omap/ driver
 * by Imre Deak.
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 */

#define DSS_SUBSYS_NAME "DPI"

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/sys_soc.h>

#include "omapdss.h"
#include "dss.h"
=======
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>
#include <linux/sys_soc.h>

#include <drm/drm_bridge.h>

#include "dss.h"
#include "omapdss.h"
>>>>>>> upstream/android-13

struct dpi_data {
	struct platform_device *pdev;
	enum dss_model dss_model;
	struct dss_device *dss;
<<<<<<< HEAD
=======
	unsigned int id;
>>>>>>> upstream/android-13

	struct regulator *vdds_dsi_reg;
	enum dss_clk_source clk_src;
	struct dss_pll *pll;

<<<<<<< HEAD
	struct mutex lock;

	struct videomode vm;
	struct dss_lcd_mgr_config mgr_config;
	int data_lines;

	struct omap_dss_device output;
};

static struct dpi_data *dpi_get_data_from_dssdev(struct omap_dss_device *dssdev)
{
	return container_of(dssdev, struct dpi_data, output);
}
=======
	struct dss_lcd_mgr_config mgr_config;
	unsigned long pixelclock;
	int data_lines;

	struct omap_dss_device output;
	struct drm_bridge bridge;
};

#define drm_bridge_to_dpi(bridge) container_of(bridge, struct dpi_data, bridge)

/* -----------------------------------------------------------------------------
 * Clock Handling and PLL
 */
>>>>>>> upstream/android-13

static enum dss_clk_source dpi_get_clk_src_dra7xx(struct dpi_data *dpi,
						  enum omap_channel channel)
{
	/*
	 * Possible clock sources:
	 * LCD1: FCK/PLL1_1/HDMI_PLL
	 * LCD2: FCK/PLL1_3/HDMI_PLL (DRA74x: PLL2_3)
	 * LCD3: FCK/PLL1_3/HDMI_PLL (DRA74x: PLL2_1)
	 */

	switch (channel) {
	case OMAP_DSS_CHANNEL_LCD:
	{
		if (dss_pll_find_by_src(dpi->dss, DSS_CLK_SRC_PLL1_1))
			return DSS_CLK_SRC_PLL1_1;
		break;
	}
	case OMAP_DSS_CHANNEL_LCD2:
	{
		if (dss_pll_find_by_src(dpi->dss, DSS_CLK_SRC_PLL1_3))
			return DSS_CLK_SRC_PLL1_3;
		if (dss_pll_find_by_src(dpi->dss, DSS_CLK_SRC_PLL2_3))
			return DSS_CLK_SRC_PLL2_3;
		break;
	}
	case OMAP_DSS_CHANNEL_LCD3:
	{
		if (dss_pll_find_by_src(dpi->dss, DSS_CLK_SRC_PLL2_1))
			return DSS_CLK_SRC_PLL2_1;
		if (dss_pll_find_by_src(dpi->dss, DSS_CLK_SRC_PLL1_3))
			return DSS_CLK_SRC_PLL1_3;
		break;
	}
	default:
		break;
	}

	return DSS_CLK_SRC_FCK;
}

static enum dss_clk_source dpi_get_clk_src(struct dpi_data *dpi)
{
	enum omap_channel channel = dpi->output.dispc_channel;

	/*
	 * XXX we can't currently use DSI PLL for DPI with OMAP3, as the DSI PLL
	 * would also be used for DISPC fclk. Meaning, when the DPI output is
	 * disabled, DISPC clock will be disabled, and TV out will stop.
	 */
	switch (dpi->dss_model) {
	case DSS_MODEL_OMAP2:
	case DSS_MODEL_OMAP3:
		return DSS_CLK_SRC_FCK;

	case DSS_MODEL_OMAP4:
		switch (channel) {
		case OMAP_DSS_CHANNEL_LCD:
			return DSS_CLK_SRC_PLL1_1;
		case OMAP_DSS_CHANNEL_LCD2:
			return DSS_CLK_SRC_PLL2_1;
		default:
			return DSS_CLK_SRC_FCK;
		}

	case DSS_MODEL_OMAP5:
		switch (channel) {
		case OMAP_DSS_CHANNEL_LCD:
			return DSS_CLK_SRC_PLL1_1;
		case OMAP_DSS_CHANNEL_LCD3:
			return DSS_CLK_SRC_PLL2_1;
		case OMAP_DSS_CHANNEL_LCD2:
		default:
			return DSS_CLK_SRC_FCK;
		}

	case DSS_MODEL_DRA7:
		return dpi_get_clk_src_dra7xx(dpi, channel);

	default:
		return DSS_CLK_SRC_FCK;
	}
}

struct dpi_clk_calc_ctx {
	struct dpi_data *dpi;
	unsigned int clkout_idx;

	/* inputs */

	unsigned long pck_min, pck_max;

	/* outputs */

	struct dss_pll_clock_info pll_cinfo;
	unsigned long fck;
	struct dispc_clock_info dispc_cinfo;
};

static bool dpi_calc_dispc_cb(int lckd, int pckd, unsigned long lck,
		unsigned long pck, void *data)
{
	struct dpi_clk_calc_ctx *ctx = data;

	/*
	 * Odd dividers give us uneven duty cycle, causing problem when level
	 * shifted. So skip all odd dividers when the pixel clock is on the
	 * higher side.
	 */
	if (ctx->pck_min >= 100000000) {
		if (lckd > 1 && lckd % 2 != 0)
			return false;

		if (pckd > 1 && pckd % 2 != 0)
			return false;
	}

	ctx->dispc_cinfo.lck_div = lckd;
	ctx->dispc_cinfo.pck_div = pckd;
	ctx->dispc_cinfo.lck = lck;
	ctx->dispc_cinfo.pck = pck;

	return true;
}


static bool dpi_calc_hsdiv_cb(int m_dispc, unsigned long dispc,
		void *data)
{
	struct dpi_clk_calc_ctx *ctx = data;

	ctx->pll_cinfo.mX[ctx->clkout_idx] = m_dispc;
	ctx->pll_cinfo.clkout[ctx->clkout_idx] = dispc;

	return dispc_div_calc(ctx->dpi->dss->dispc, dispc,
			      ctx->pck_min, ctx->pck_max,
			      dpi_calc_dispc_cb, ctx);
}


static bool dpi_calc_pll_cb(int n, int m, unsigned long fint,
		unsigned long clkdco,
		void *data)
{
	struct dpi_clk_calc_ctx *ctx = data;

	ctx->pll_cinfo.n = n;
	ctx->pll_cinfo.m = m;
	ctx->pll_cinfo.fint = fint;
	ctx->pll_cinfo.clkdco = clkdco;

	return dss_pll_hsdiv_calc_a(ctx->dpi->pll, clkdco,
		ctx->pck_min, dss_get_max_fck_rate(ctx->dpi->dss),
		dpi_calc_hsdiv_cb, ctx);
}

static bool dpi_calc_dss_cb(unsigned long fck, void *data)
{
	struct dpi_clk_calc_ctx *ctx = data;

	ctx->fck = fck;

	return dispc_div_calc(ctx->dpi->dss->dispc, fck,
			      ctx->pck_min, ctx->pck_max,
			      dpi_calc_dispc_cb, ctx);
}

static bool dpi_pll_clk_calc(struct dpi_data *dpi, unsigned long pck,
		struct dpi_clk_calc_ctx *ctx)
{
	unsigned long clkin;

	memset(ctx, 0, sizeof(*ctx));
	ctx->dpi = dpi;
	ctx->clkout_idx = dss_pll_get_clkout_idx_for_src(dpi->clk_src);

	clkin = clk_get_rate(dpi->pll->clkin);

	if (dpi->pll->hw->type == DSS_PLL_TYPE_A) {
		unsigned long pll_min, pll_max;

		ctx->pck_min = pck - 1000;
		ctx->pck_max = pck + 1000;

		pll_min = 0;
		pll_max = 0;

		return dss_pll_calc_a(ctx->dpi->pll, clkin,
				pll_min, pll_max,
				dpi_calc_pll_cb, ctx);
	} else { /* DSS_PLL_TYPE_B */
		dss_pll_calc_b(dpi->pll, clkin, pck, &ctx->pll_cinfo);

		ctx->dispc_cinfo.lck_div = 1;
		ctx->dispc_cinfo.pck_div = 1;
		ctx->dispc_cinfo.lck = ctx->pll_cinfo.clkout[0];
		ctx->dispc_cinfo.pck = ctx->dispc_cinfo.lck;

		return true;
	}
}

static bool dpi_dss_clk_calc(struct dpi_data *dpi, unsigned long pck,
			     struct dpi_clk_calc_ctx *ctx)
{
	int i;

	/*
	 * DSS fck gives us very few possibilities, so finding a good pixel
	 * clock may not be possible. We try multiple times to find the clock,
	 * each time widening the pixel clock range we look for, up to
	 * +/- ~15MHz.
	 */

	for (i = 0; i < 25; ++i) {
		bool ok;

		memset(ctx, 0, sizeof(*ctx));
		ctx->dpi = dpi;
		if (pck > 1000 * i * i * i)
			ctx->pck_min = max(pck - 1000 * i * i * i, 0lu);
		else
			ctx->pck_min = 0;
		ctx->pck_max = pck + 1000 * i * i * i;

		ok = dss_div_calc(dpi->dss, pck, ctx->pck_min,
				  dpi_calc_dss_cb, ctx);
		if (ok)
			return ok;
	}

	return false;
}



<<<<<<< HEAD
static int dpi_set_pll_clk(struct dpi_data *dpi, enum omap_channel channel,
		unsigned long pck_req, unsigned long *fck, int *lck_div,
		int *pck_div)
=======
static int dpi_set_pll_clk(struct dpi_data *dpi, unsigned long pck_req)
>>>>>>> upstream/android-13
{
	struct dpi_clk_calc_ctx ctx;
	int r;
	bool ok;

	ok = dpi_pll_clk_calc(dpi, pck_req, &ctx);
	if (!ok)
		return -EINVAL;

	r = dss_pll_set_config(dpi->pll, &ctx.pll_cinfo);
	if (r)
		return r;

<<<<<<< HEAD
	dss_select_lcd_clk_source(dpi->dss, channel, dpi->clk_src);

	dpi->mgr_config.clock_info = ctx.dispc_cinfo;

	*fck = ctx.pll_cinfo.clkout[ctx.clkout_idx];
	*lck_div = ctx.dispc_cinfo.lck_div;
	*pck_div = ctx.dispc_cinfo.pck_div;

	return 0;
}

static int dpi_set_dispc_clk(struct dpi_data *dpi, unsigned long pck_req,
		unsigned long *fck, int *lck_div, int *pck_div)
=======
	dss_select_lcd_clk_source(dpi->dss, dpi->output.dispc_channel,
				  dpi->clk_src);

	dpi->mgr_config.clock_info = ctx.dispc_cinfo;

	return 0;
}

static int dpi_set_dispc_clk(struct dpi_data *dpi, unsigned long pck_req)
>>>>>>> upstream/android-13
{
	struct dpi_clk_calc_ctx ctx;
	int r;
	bool ok;

	ok = dpi_dss_clk_calc(dpi, pck_req, &ctx);
	if (!ok)
		return -EINVAL;

	r = dss_set_fck_rate(dpi->dss, ctx.fck);
	if (r)
		return r;

	dpi->mgr_config.clock_info = ctx.dispc_cinfo;

<<<<<<< HEAD
	*fck = ctx.fck;
	*lck_div = ctx.dispc_cinfo.lck_div;
	*pck_div = ctx.dispc_cinfo.pck_div;

=======
>>>>>>> upstream/android-13
	return 0;
}

static int dpi_set_mode(struct dpi_data *dpi)
{
<<<<<<< HEAD
	struct videomode *vm = &dpi->vm;
	int lck_div = 0, pck_div = 0;
	unsigned long fck = 0;
	unsigned long pck;
	int r = 0;

	if (dpi->pll)
		r = dpi_set_pll_clk(dpi, dpi->output.dispc_channel,
				    vm->pixelclock, &fck, &lck_div, &pck_div);
	else
		r = dpi_set_dispc_clk(dpi, vm->pixelclock, &fck,
				&lck_div, &pck_div);
	if (r)
		return r;

	pck = fck / lck_div / pck_div;

	if (pck != vm->pixelclock) {
		DSSWARN("Could not find exact pixel clock. Requested %lu Hz, got %lu Hz\n",
			vm->pixelclock, pck);

		vm->pixelclock = pck;
	}

	dss_mgr_set_timings(&dpi->output, vm);

	return 0;
=======
	int r;

	if (dpi->pll)
		r = dpi_set_pll_clk(dpi, dpi->pixelclock);
	else
		r = dpi_set_dispc_clk(dpi, dpi->pixelclock);

	return r;
>>>>>>> upstream/android-13
}

static void dpi_config_lcd_manager(struct dpi_data *dpi)
{
	dpi->mgr_config.io_pad_mode = DSS_IO_PAD_MODE_BYPASS;

	dpi->mgr_config.stallmode = false;
	dpi->mgr_config.fifohandcheck = false;

	dpi->mgr_config.video_port_width = dpi->data_lines;

	dpi->mgr_config.lcden_sig_polarity = 0;

	dss_mgr_set_lcd_config(&dpi->output, &dpi->mgr_config);
}

<<<<<<< HEAD
static int dpi_display_enable(struct omap_dss_device *dssdev)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);
	struct omap_dss_device *out = &dpi->output;
	int r;

	mutex_lock(&dpi->lock);

	if (!out->dispc_channel_connected) {
		DSSERR("failed to enable display: no output/manager\n");
		r = -ENODEV;
		goto err_no_out_mgr;
	}

	if (dpi->vdds_dsi_reg) {
		r = regulator_enable(dpi->vdds_dsi_reg);
		if (r)
			goto err_reg_enable;
	}

	r = dispc_runtime_get(dpi->dss->dispc);
	if (r)
		goto err_get_dispc;

	r = dss_dpi_select_source(dpi->dss, out->port_num, out->dispc_channel);
	if (r)
		goto err_src_sel;

	if (dpi->pll) {
		r = dss_pll_enable(dpi->pll);
		if (r)
			goto err_pll_init;
	}

	r = dpi_set_mode(dpi);
	if (r)
		goto err_set_mode;

	dpi_config_lcd_manager(dpi);

	mdelay(2);

	r = dss_mgr_enable(&dpi->output);
	if (r)
		goto err_mgr_enable;

	mutex_unlock(&dpi->lock);

	return 0;

err_mgr_enable:
err_set_mode:
	if (dpi->pll)
		dss_pll_disable(dpi->pll);
err_pll_init:
err_src_sel:
	dispc_runtime_put(dpi->dss->dispc);
err_get_dispc:
	if (dpi->vdds_dsi_reg)
		regulator_disable(dpi->vdds_dsi_reg);
err_reg_enable:
err_no_out_mgr:
	mutex_unlock(&dpi->lock);
	return r;
}

static void dpi_display_disable(struct omap_dss_device *dssdev)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);

	mutex_lock(&dpi->lock);

	dss_mgr_disable(&dpi->output);

	if (dpi->pll) {
		dss_select_lcd_clk_source(dpi->dss, dpi->output.dispc_channel,
					  DSS_CLK_SRC_FCK);
		dss_pll_disable(dpi->pll);
	}

	dispc_runtime_put(dpi->dss->dispc);

	if (dpi->vdds_dsi_reg)
		regulator_disable(dpi->vdds_dsi_reg);

	mutex_unlock(&dpi->lock);
}

static void dpi_set_timings(struct omap_dss_device *dssdev,
			    struct videomode *vm)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);

	DSSDBG("dpi_set_timings\n");

	mutex_lock(&dpi->lock);

	dpi->vm = *vm;

	mutex_unlock(&dpi->lock);
}

static void dpi_get_timings(struct omap_dss_device *dssdev,
			    struct videomode *vm)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);

	mutex_lock(&dpi->lock);

	*vm = dpi->vm;

	mutex_unlock(&dpi->lock);
}

static int dpi_check_timings(struct omap_dss_device *dssdev,
			     struct videomode *vm)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);
	enum omap_channel channel = dpi->output.dispc_channel;
	int lck_div, pck_div;
	unsigned long fck;
	unsigned long pck;
	struct dpi_clk_calc_ctx ctx;
	bool ok;

	if (vm->hactive % 8 != 0)
		return -EINVAL;

	if (!dispc_mgr_timings_ok(dpi->dss->dispc, channel, vm))
		return -EINVAL;

	if (vm->pixelclock == 0)
		return -EINVAL;

	if (dpi->pll) {
		ok = dpi_pll_clk_calc(dpi, vm->pixelclock, &ctx);
		if (!ok)
=======
static int dpi_clock_update(struct dpi_data *dpi, unsigned long *clock)
{
	int lck_div, pck_div;
	unsigned long fck;
	struct dpi_clk_calc_ctx ctx;

	if (dpi->pll) {
		if (!dpi_pll_clk_calc(dpi, *clock, &ctx))
>>>>>>> upstream/android-13
			return -EINVAL;

		fck = ctx.pll_cinfo.clkout[ctx.clkout_idx];
	} else {
<<<<<<< HEAD
		ok = dpi_dss_clk_calc(dpi, vm->pixelclock, &ctx);
		if (!ok)
=======
		if (!dpi_dss_clk_calc(dpi, *clock, &ctx))
>>>>>>> upstream/android-13
			return -EINVAL;

		fck = ctx.fck;
	}

	lck_div = ctx.dispc_cinfo.lck_div;
	pck_div = ctx.dispc_cinfo.pck_div;

<<<<<<< HEAD
	pck = fck / lck_div / pck_div;

	vm->pixelclock = pck;
=======
	*clock = fck / lck_div / pck_div;
>>>>>>> upstream/android-13

	return 0;
}

static int dpi_verify_pll(struct dss_pll *pll)
{
	int r;

	/* do initial setup with the PLL to see if it is operational */

	r = dss_pll_enable(pll);
	if (r)
		return r;

	dss_pll_disable(pll);

	return 0;
}

<<<<<<< HEAD
static const struct soc_device_attribute dpi_soc_devices[] = {
	{ .machine = "OMAP3[456]*" },
	{ .machine = "[AD]M37*" },
	{ /* sentinel */ }
};

static int dpi_init_regulator(struct dpi_data *dpi)
{
	struct regulator *vdds_dsi;

	/*
	 * The DPI uses the DSI VDDS on OMAP34xx, OMAP35xx, OMAP36xx, AM37xx and
	 * DM37xx only.
	 */
	if (!soc_device_match(dpi_soc_devices))
		return 0;

	if (dpi->vdds_dsi_reg)
		return 0;

	vdds_dsi = devm_regulator_get(&dpi->pdev->dev, "vdds_dsi");
	if (IS_ERR(vdds_dsi)) {
		if (PTR_ERR(vdds_dsi) != -EPROBE_DEFER)
			DSSERR("can't get VDDS_DSI regulator\n");
		return PTR_ERR(vdds_dsi);
	}

	dpi->vdds_dsi_reg = vdds_dsi;

	return 0;
}

=======
>>>>>>> upstream/android-13
static void dpi_init_pll(struct dpi_data *dpi)
{
	struct dss_pll *pll;

	if (dpi->pll)
		return;

	dpi->clk_src = dpi_get_clk_src(dpi);

	pll = dss_pll_find_by_src(dpi->dss, dpi->clk_src);
	if (!pll)
		return;

	if (dpi_verify_pll(pll)) {
		DSSWARN("PLL not operational\n");
		return;
	}

	dpi->pll = pll;
}

<<<<<<< HEAD
=======
/* -----------------------------------------------------------------------------
 * DRM Bridge Operations
 */

static int dpi_bridge_attach(struct drm_bridge *bridge,
			     enum drm_bridge_attach_flags flags)
{
	struct dpi_data *dpi = drm_bridge_to_dpi(bridge);

	if (!(flags & DRM_BRIDGE_ATTACH_NO_CONNECTOR))
		return -EINVAL;

	dpi_init_pll(dpi);

	return drm_bridge_attach(bridge->encoder, dpi->output.next_bridge,
				 bridge, flags);
}

static enum drm_mode_status
dpi_bridge_mode_valid(struct drm_bridge *bridge,
		       const struct drm_display_info *info,
		       const struct drm_display_mode *mode)
{
	struct dpi_data *dpi = drm_bridge_to_dpi(bridge);
	unsigned long clock = mode->clock * 1000;
	int ret;

	if (mode->hdisplay % 8 != 0)
		return MODE_BAD_WIDTH;

	if (mode->clock == 0)
		return MODE_NOCLOCK;

	ret = dpi_clock_update(dpi, &clock);
	if (ret < 0)
		return MODE_CLOCK_RANGE;

	return MODE_OK;
}

static bool dpi_bridge_mode_fixup(struct drm_bridge *bridge,
				   const struct drm_display_mode *mode,
				   struct drm_display_mode *adjusted_mode)
{
	struct dpi_data *dpi = drm_bridge_to_dpi(bridge);
	unsigned long clock = mode->clock * 1000;
	int ret;

	ret = dpi_clock_update(dpi, &clock);
	if (ret < 0)
		return false;

	adjusted_mode->clock = clock / 1000;

	return true;
}

static void dpi_bridge_mode_set(struct drm_bridge *bridge,
				 const struct drm_display_mode *mode,
				 const struct drm_display_mode *adjusted_mode)
{
	struct dpi_data *dpi = drm_bridge_to_dpi(bridge);

	dpi->pixelclock = adjusted_mode->clock * 1000;
}

static void dpi_bridge_enable(struct drm_bridge *bridge)
{
	struct dpi_data *dpi = drm_bridge_to_dpi(bridge);
	int r;

	if (dpi->vdds_dsi_reg) {
		r = regulator_enable(dpi->vdds_dsi_reg);
		if (r)
			return;
	}

	r = dispc_runtime_get(dpi->dss->dispc);
	if (r)
		goto err_get_dispc;

	r = dss_dpi_select_source(dpi->dss, dpi->id, dpi->output.dispc_channel);
	if (r)
		goto err_src_sel;

	if (dpi->pll) {
		r = dss_pll_enable(dpi->pll);
		if (r)
			goto err_pll_init;
	}

	r = dpi_set_mode(dpi);
	if (r)
		goto err_set_mode;

	dpi_config_lcd_manager(dpi);

	mdelay(2);

	r = dss_mgr_enable(&dpi->output);
	if (r)
		goto err_mgr_enable;

	return;

err_mgr_enable:
err_set_mode:
	if (dpi->pll)
		dss_pll_disable(dpi->pll);
err_pll_init:
err_src_sel:
	dispc_runtime_put(dpi->dss->dispc);
err_get_dispc:
	if (dpi->vdds_dsi_reg)
		regulator_disable(dpi->vdds_dsi_reg);
}

static void dpi_bridge_disable(struct drm_bridge *bridge)
{
	struct dpi_data *dpi = drm_bridge_to_dpi(bridge);

	dss_mgr_disable(&dpi->output);

	if (dpi->pll) {
		dss_select_lcd_clk_source(dpi->dss, dpi->output.dispc_channel,
					  DSS_CLK_SRC_FCK);
		dss_pll_disable(dpi->pll);
	}

	dispc_runtime_put(dpi->dss->dispc);

	if (dpi->vdds_dsi_reg)
		regulator_disable(dpi->vdds_dsi_reg);
}

static const struct drm_bridge_funcs dpi_bridge_funcs = {
	.attach = dpi_bridge_attach,
	.mode_valid = dpi_bridge_mode_valid,
	.mode_fixup = dpi_bridge_mode_fixup,
	.mode_set = dpi_bridge_mode_set,
	.enable = dpi_bridge_enable,
	.disable = dpi_bridge_disable,
};

static void dpi_bridge_init(struct dpi_data *dpi)
{
	dpi->bridge.funcs = &dpi_bridge_funcs;
	dpi->bridge.of_node = dpi->pdev->dev.of_node;
	dpi->bridge.type = DRM_MODE_CONNECTOR_DPI;

	drm_bridge_add(&dpi->bridge);
}

static void dpi_bridge_cleanup(struct dpi_data *dpi)
{
	drm_bridge_remove(&dpi->bridge);
}

/* -----------------------------------------------------------------------------
 * Initialisation and Cleanup
 */

>>>>>>> upstream/android-13
/*
 * Return a hardcoded channel for the DPI output. This should work for
 * current use cases, but this can be later expanded to either resolve
 * the channel in some more dynamic manner, or get the channel as a user
 * parameter.
 */
<<<<<<< HEAD
static enum omap_channel dpi_get_channel(struct dpi_data *dpi, int port_num)
=======
static enum omap_channel dpi_get_channel(struct dpi_data *dpi)
>>>>>>> upstream/android-13
{
	switch (dpi->dss_model) {
	case DSS_MODEL_OMAP2:
	case DSS_MODEL_OMAP3:
		return OMAP_DSS_CHANNEL_LCD;

	case DSS_MODEL_DRA7:
<<<<<<< HEAD
		switch (port_num) {
=======
		switch (dpi->id) {
>>>>>>> upstream/android-13
		case 2:
			return OMAP_DSS_CHANNEL_LCD3;
		case 1:
			return OMAP_DSS_CHANNEL_LCD2;
		case 0:
		default:
			return OMAP_DSS_CHANNEL_LCD;
		}

	case DSS_MODEL_OMAP4:
		return OMAP_DSS_CHANNEL_LCD2;

	case DSS_MODEL_OMAP5:
		return OMAP_DSS_CHANNEL_LCD3;

	default:
		DSSWARN("unsupported DSS version\n");
		return OMAP_DSS_CHANNEL_LCD;
	}
}

<<<<<<< HEAD
static int dpi_connect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);
	int r;

	r = dpi_init_regulator(dpi);
	if (r)
		return r;

	dpi_init_pll(dpi);

	r = dss_mgr_connect(&dpi->output, dssdev);
	if (r)
		return r;

	r = omapdss_output_set_device(dssdev, dst);
	if (r) {
		DSSERR("failed to connect output to new device: %s\n",
				dst->name);
		dss_mgr_disconnect(&dpi->output, dssdev);
		return r;
	}

	return 0;
}

static void dpi_disconnect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct dpi_data *dpi = dpi_get_data_from_dssdev(dssdev);

	WARN_ON(dst != dssdev->dst);

	if (dst != dssdev->dst)
		return;

	omapdss_output_unset_device(dssdev);

	dss_mgr_disconnect(&dpi->output, dssdev);
}

static const struct omapdss_dpi_ops dpi_ops = {
	.connect = dpi_connect,
	.disconnect = dpi_disconnect,

	.enable = dpi_display_enable,
	.disable = dpi_display_disable,

	.check_timings = dpi_check_timings,
	.set_timings = dpi_set_timings,
	.get_timings = dpi_get_timings,
};

static void dpi_init_output_port(struct dpi_data *dpi, struct device_node *port)
{
	struct omap_dss_device *out = &dpi->output;
	int r;
	u32 port_num;

	r = of_property_read_u32(port, "reg", &port_num);
	if (r)
		port_num = 0;
=======
static int dpi_init_output_port(struct dpi_data *dpi, struct device_node *port)
{
	struct omap_dss_device *out = &dpi->output;
	u32 port_num = 0;
	int r;

	dpi_bridge_init(dpi);

	of_property_read_u32(port, "reg", &port_num);
	dpi->id = port_num <= 2 ? port_num : 0;
>>>>>>> upstream/android-13

	switch (port_num) {
	case 2:
		out->name = "dpi.2";
		break;
	case 1:
		out->name = "dpi.1";
		break;
	case 0:
	default:
		out->name = "dpi.0";
		break;
	}

	out->dev = &dpi->pdev->dev;
	out->id = OMAP_DSS_OUTPUT_DPI;
<<<<<<< HEAD
	out->output_type = OMAP_DISPLAY_TYPE_DPI;
	out->dispc_channel = dpi_get_channel(dpi, port_num);
	out->port_num = port_num;
	out->ops.dpi = &dpi_ops;
	out->owner = THIS_MODULE;

	omapdss_register_output(out);
=======
	out->type = OMAP_DISPLAY_TYPE_DPI;
	out->dispc_channel = dpi_get_channel(dpi);
	out->of_port = port_num;

	r = omapdss_device_init_output(out, &dpi->bridge);
	if (r < 0) {
		dpi_bridge_cleanup(dpi);
		return r;
	}

	omapdss_device_register(out);

	return 0;
>>>>>>> upstream/android-13
}

static void dpi_uninit_output_port(struct device_node *port)
{
	struct dpi_data *dpi = port->data;
	struct omap_dss_device *out = &dpi->output;

<<<<<<< HEAD
	omapdss_unregister_output(out);
=======
	omapdss_device_unregister(out);
	omapdss_device_cleanup_output(out);

	dpi_bridge_cleanup(dpi);
}

/* -----------------------------------------------------------------------------
 * Initialisation and Cleanup
 */

static const struct soc_device_attribute dpi_soc_devices[] = {
	{ .machine = "OMAP3[456]*" },
	{ .machine = "[AD]M37*" },
	{ /* sentinel */ }
};

static int dpi_init_regulator(struct dpi_data *dpi)
{
	struct regulator *vdds_dsi;

	/*
	 * The DPI uses the DSI VDDS on OMAP34xx, OMAP35xx, OMAP36xx, AM37xx and
	 * DM37xx only.
	 */
	if (!soc_device_match(dpi_soc_devices))
		return 0;

	vdds_dsi = devm_regulator_get(&dpi->pdev->dev, "vdds_dsi");
	if (IS_ERR(vdds_dsi)) {
		if (PTR_ERR(vdds_dsi) != -EPROBE_DEFER)
			DSSERR("can't get VDDS_DSI regulator\n");
		return PTR_ERR(vdds_dsi);
	}

	dpi->vdds_dsi_reg = vdds_dsi;

	return 0;
>>>>>>> upstream/android-13
}

int dpi_init_port(struct dss_device *dss, struct platform_device *pdev,
		  struct device_node *port, enum dss_model dss_model)
{
	struct dpi_data *dpi;
	struct device_node *ep;
	u32 datalines;
	int r;

	dpi = devm_kzalloc(&pdev->dev, sizeof(*dpi), GFP_KERNEL);
	if (!dpi)
		return -ENOMEM;

	ep = of_get_next_child(port, NULL);
	if (!ep)
		return 0;

	r = of_property_read_u32(ep, "data-lines", &datalines);
<<<<<<< HEAD
	if (r) {
		DSSERR("failed to parse datalines\n");
		goto err_datalines;
=======
	of_node_put(ep);
	if (r) {
		DSSERR("failed to parse datalines\n");
		return r;
>>>>>>> upstream/android-13
	}

	dpi->data_lines = datalines;

<<<<<<< HEAD
	of_node_put(ep);

=======
>>>>>>> upstream/android-13
	dpi->pdev = pdev;
	dpi->dss_model = dss_model;
	dpi->dss = dss;
	port->data = dpi;

<<<<<<< HEAD
	mutex_init(&dpi->lock);

	dpi_init_output_port(dpi, port);

	return 0;

err_datalines:
	of_node_put(ep);

	return r;
=======
	r = dpi_init_regulator(dpi);
	if (r)
		return r;

	return dpi_init_output_port(dpi, port);
>>>>>>> upstream/android-13
}

void dpi_uninit_port(struct device_node *port)
{
	struct dpi_data *dpi = port->data;

	if (!dpi)
		return;

	dpi_uninit_output_port(port);
}
