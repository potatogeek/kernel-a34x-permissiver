<<<<<<< HEAD
/*
 * Copyright (C) 2012 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Russell King
 *
>>>>>>> upstream/android-13
 * Armada 510 (aka Dove) variant support
 */
#include <linux/clk.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <drm/drm_crtc_helper.h>
=======
#include <drm/drm_probe_helper.h>
>>>>>>> upstream/android-13
#include "armada_crtc.h"
#include "armada_drm.h"
#include "armada_hw.h"

<<<<<<< HEAD
static int armada510_crtc_init(struct armada_crtc *dcrtc, struct device *dev)
{
	struct clk *clk;

	clk = devm_clk_get(dev, "ext_ref_clk1");
	if (IS_ERR(clk))
		return PTR_ERR(clk) == -ENOENT ? -EPROBE_DEFER : PTR_ERR(clk);

	dcrtc->extclk[0] = clk;

	/* Lower the watermark so to eliminate jitter at higher bandwidths */
	armada_updatel(0x20, (1 << 11) | 0xff, dcrtc->base + LCD_CFG_RDREG4F);
=======
struct armada510_variant_data {
	struct clk *clks[4];
	struct clk *sel_clk;
};

static int armada510_crtc_init(struct armada_crtc *dcrtc, struct device *dev)
{
	struct armada510_variant_data *v;
	struct clk *clk;
	int idx;

	v = devm_kzalloc(dev, sizeof(*v), GFP_KERNEL);
	if (!v)
		return -ENOMEM;

	dcrtc->variant_data = v;

	if (dev->of_node) {
		struct property *prop;
		const char *s;

		of_property_for_each_string(dev->of_node, "clock-names", prop,
					    s) {
			if (!strcmp(s, "ext_ref_clk0"))
				idx = 0;
			else if (!strcmp(s, "ext_ref_clk1"))
				idx = 1;
			else if (!strcmp(s, "plldivider"))
				idx = 2;
			else if (!strcmp(s, "axibus"))
				idx = 3;
			else
				continue;

			clk = devm_clk_get(dev, s);
			if (IS_ERR(clk))
				return PTR_ERR(clk) == -ENOENT ? -EPROBE_DEFER :
					PTR_ERR(clk);
			v->clks[idx] = clk;
		}
	} else {
		clk = devm_clk_get(dev, "ext_ref_clk1");
		if (IS_ERR(clk))
			return PTR_ERR(clk) == -ENOENT ? -EPROBE_DEFER :
				PTR_ERR(clk);

		v->clks[1] = clk;
	}

	/*
	 * Lower the watermark so to eliminate jitter at higher bandwidths.
	 * Disable SRAM read wait state to avoid system hang with external
	 * clock.
	 */
	armada_updatel(CFG_DMA_WM(0x20), CFG_SRAM_WAIT | CFG_DMA_WM_MASK,
		       dcrtc->base + LCD_CFG_RDREG4F);
>>>>>>> upstream/android-13

	/* Initialise SPU register */
	writel_relaxed(ADV_HWC32ENABLE | ADV_HWC32ARGB | ADV_HWC32BLEND,
		       dcrtc->base + LCD_SPU_ADV_REG);

	return 0;
}

<<<<<<< HEAD
=======
static const u32 armada510_clk_sels[] = {
	SCLK_510_EXTCLK0,
	SCLK_510_EXTCLK1,
	SCLK_510_PLL,
	SCLK_510_AXI,
};

static const struct armada_clocking_params armada510_clocking = {
	/* HDMI requires -0.6%..+0.5% */
	.permillage_min = 994,
	.permillage_max = 1005,
	.settable = BIT(0) | BIT(1),
	.div_max = SCLK_510_INT_DIV_MASK,
};

>>>>>>> upstream/android-13
/*
 * Armada510 specific SCLK register selection.
 * This gets called with sclk = NULL to test whether the mode is
 * supportable, and again with sclk != NULL to set the clocks up for
 * that.  The former can return an error, but the latter is expected
 * not to.
<<<<<<< HEAD
 *
 * We currently are pretty rudimentary here, always selecting
 * EXT_REF_CLK_1 for LCD0 and erroring LCD1.  This needs improvement!
=======
>>>>>>> upstream/android-13
 */
static int armada510_crtc_compute_clock(struct armada_crtc *dcrtc,
	const struct drm_display_mode *mode, uint32_t *sclk)
{
<<<<<<< HEAD
	struct clk *clk = dcrtc->extclk[0];
	int ret;

	if (dcrtc->num == 1)
		return -EINVAL;

	if (IS_ERR(clk))
		return PTR_ERR(clk);

	if (dcrtc->clk != clk) {
		ret = clk_prepare_enable(clk);
		if (ret)
			return ret;
		dcrtc->clk = clk;
	}

	if (sclk) {
		uint32_t rate, ref, div;

		rate = mode->clock * 1000;
		ref = clk_round_rate(clk, rate);
		div = DIV_ROUND_UP(ref, rate);
		if (div < 1)
			div = 1;

		clk_set_rate(clk, ref);
		*sclk = div | SCLK_510_EXTCLK1;
	}

=======
	struct armada510_variant_data *v = dcrtc->variant_data;
	unsigned long desired_khz = mode->crtc_clock;
	struct armada_clk_result res;
	int ret, idx;

	idx = armada_crtc_select_clock(dcrtc, &res, &armada510_clocking,
				       v->clks, ARRAY_SIZE(v->clks),
				       desired_khz);
	if (idx < 0)
		return idx;

	ret = clk_prepare_enable(res.clk);
	if (ret)
		return ret;

	if (sclk) {
		clk_set_rate(res.clk, res.desired_clk_hz);

		*sclk = res.div | armada510_clk_sels[idx];

		/* We are now using this clock */
		v->sel_clk = res.clk;
		swap(dcrtc->clk, res.clk);
	}

	clk_disable_unprepare(res.clk);

>>>>>>> upstream/android-13
	return 0;
}

static void armada510_crtc_disable(struct armada_crtc *dcrtc)
{
<<<<<<< HEAD
	if (!IS_ERR(dcrtc->clk)) {
		clk_disable_unprepare(dcrtc->clk);
		dcrtc->clk = ERR_PTR(-EINVAL);
=======
	if (dcrtc->clk) {
		clk_disable_unprepare(dcrtc->clk);
		dcrtc->clk = NULL;
>>>>>>> upstream/android-13
	}
}

static void armada510_crtc_enable(struct armada_crtc *dcrtc,
	const struct drm_display_mode *mode)
{
<<<<<<< HEAD
	if (IS_ERR(dcrtc->clk)) {
		dcrtc->clk = dcrtc->extclk[0];
		WARN_ON(clk_prepare_enable(dcrtc->clk));
=======
	struct armada510_variant_data *v = dcrtc->variant_data;

	if (!dcrtc->clk && v->sel_clk) {
		if (!WARN_ON(clk_prepare_enable(v->sel_clk)))
			dcrtc->clk = v->sel_clk;
>>>>>>> upstream/android-13
	}
}

const struct armada_variant armada510_ops = {
	.has_spu_adv_reg = true,
	.init = armada510_crtc_init,
	.compute_clock = armada510_crtc_compute_clock,
	.disable = armada510_crtc_disable,
	.enable = armada510_crtc_enable,
};
