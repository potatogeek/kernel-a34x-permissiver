<<<<<<< HEAD
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015, The Linux Foundation. All rights reserved.
>>>>>>> upstream/android-13
 */

#include "dsi_phy.h"
#include "dsi.xml.h"
<<<<<<< HEAD
=======
#include "dsi_phy_20nm.xml.h"
>>>>>>> upstream/android-13

static void dsi_20nm_dphy_set_timing(struct msm_dsi_phy *phy,
		struct msm_dsi_dphy_timing *timing)
{
	void __iomem *base = phy->base;

	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_0,
		DSI_20nm_PHY_TIMING_CTRL_0_CLK_ZERO(timing->clk_zero));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_1,
		DSI_20nm_PHY_TIMING_CTRL_1_CLK_TRAIL(timing->clk_trail));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_2,
		DSI_20nm_PHY_TIMING_CTRL_2_CLK_PREPARE(timing->clk_prepare));
	if (timing->clk_zero & BIT(8))
		dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_3,
			DSI_20nm_PHY_TIMING_CTRL_3_CLK_ZERO_8);
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_4,
		DSI_20nm_PHY_TIMING_CTRL_4_HS_EXIT(timing->hs_exit));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_5,
		DSI_20nm_PHY_TIMING_CTRL_5_HS_ZERO(timing->hs_zero));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_6,
		DSI_20nm_PHY_TIMING_CTRL_6_HS_PREPARE(timing->hs_prepare));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_7,
		DSI_20nm_PHY_TIMING_CTRL_7_HS_TRAIL(timing->hs_trail));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_8,
		DSI_20nm_PHY_TIMING_CTRL_8_HS_RQST(timing->hs_rqst));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_9,
		DSI_20nm_PHY_TIMING_CTRL_9_TA_GO(timing->ta_go) |
		DSI_20nm_PHY_TIMING_CTRL_9_TA_SURE(timing->ta_sure));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_10,
		DSI_20nm_PHY_TIMING_CTRL_10_TA_GET(timing->ta_get));
	dsi_phy_write(base + REG_DSI_20nm_PHY_TIMING_CTRL_11,
		DSI_20nm_PHY_TIMING_CTRL_11_TRIG3_CMD(0));
}

static void dsi_20nm_phy_regulator_ctrl(struct msm_dsi_phy *phy, bool enable)
{
	void __iomem *base = phy->reg_base;

	if (!enable) {
		dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CAL_PWR_CFG, 0);
		return;
	}

	if (phy->regulator_ldo_mode) {
		dsi_phy_write(phy->base + REG_DSI_20nm_PHY_LDO_CNTRL, 0x1d);
		return;
	}

	/* non LDO mode */
	dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CTRL_1, 0x03);
	dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CTRL_2, 0x03);
	dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CTRL_3, 0x00);
	dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CTRL_4, 0x20);
	dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CAL_PWR_CFG, 0x01);
	dsi_phy_write(phy->base + REG_DSI_20nm_PHY_LDO_CNTRL, 0x00);
	dsi_phy_write(base + REG_DSI_20nm_PHY_REGULATOR_CTRL_0, 0x03);
}

<<<<<<< HEAD
static int dsi_20nm_phy_enable(struct msm_dsi_phy *phy, int src_pll_id,
=======
static int dsi_20nm_phy_enable(struct msm_dsi_phy *phy,
>>>>>>> upstream/android-13
				struct msm_dsi_phy_clk_request *clk_req)
{
	struct msm_dsi_dphy_timing *timing = &phy->timing;
	int i;
	void __iomem *base = phy->base;
	u32 cfg_4[4] = {0x20, 0x40, 0x20, 0x00};
<<<<<<< HEAD
=======
	u32 val;
>>>>>>> upstream/android-13

	DBG("");

	if (msm_dsi_dphy_timing_calc(timing, clk_req)) {
<<<<<<< HEAD
		dev_err(&phy->pdev->dev,
=======
		DRM_DEV_ERROR(&phy->pdev->dev,
>>>>>>> upstream/android-13
			"%s: D-PHY timing calculation failed\n", __func__);
		return -EINVAL;
	}

	dsi_20nm_phy_regulator_ctrl(phy, true);

	dsi_phy_write(base + REG_DSI_20nm_PHY_STRENGTH_0, 0xff);

<<<<<<< HEAD
	msm_dsi_phy_set_src_pll(phy, src_pll_id,
				REG_DSI_20nm_PHY_GLBL_TEST_CTRL,
				DSI_20nm_PHY_GLBL_TEST_CTRL_BITCLK_HS_SEL);
=======
	val = dsi_phy_read(base + REG_DSI_20nm_PHY_GLBL_TEST_CTRL);
	if (phy->id == DSI_1 && phy->usecase == MSM_DSI_PHY_STANDALONE)
		val |= DSI_20nm_PHY_GLBL_TEST_CTRL_BITCLK_HS_SEL;
	else
		val &= ~DSI_20nm_PHY_GLBL_TEST_CTRL_BITCLK_HS_SEL;
	dsi_phy_write(base + REG_DSI_20nm_PHY_GLBL_TEST_CTRL, val);
>>>>>>> upstream/android-13

	for (i = 0; i < 4; i++) {
		dsi_phy_write(base + REG_DSI_20nm_PHY_LN_CFG_3(i),
							(i >> 1) * 0x40);
		dsi_phy_write(base + REG_DSI_20nm_PHY_LN_TEST_STR_0(i), 0x01);
		dsi_phy_write(base + REG_DSI_20nm_PHY_LN_TEST_STR_1(i), 0x46);
		dsi_phy_write(base + REG_DSI_20nm_PHY_LN_CFG_0(i), 0x02);
		dsi_phy_write(base + REG_DSI_20nm_PHY_LN_CFG_1(i), 0xa0);
		dsi_phy_write(base + REG_DSI_20nm_PHY_LN_CFG_4(i), cfg_4[i]);
	}

	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_CFG_3, 0x80);
	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_TEST_STR0, 0x01);
	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_TEST_STR1, 0x46);
	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_CFG_0, 0x00);
	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_CFG_1, 0xa0);
	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_CFG_2, 0x00);
	dsi_phy_write(base + REG_DSI_20nm_PHY_LNCK_CFG_4, 0x00);

	dsi_20nm_dphy_set_timing(phy, timing);

	dsi_phy_write(base + REG_DSI_20nm_PHY_CTRL_1, 0x00);

	dsi_phy_write(base + REG_DSI_20nm_PHY_STRENGTH_1, 0x06);

	/* make sure everything is written before enable */
	wmb();
	dsi_phy_write(base + REG_DSI_20nm_PHY_CTRL_0, 0x7f);

	return 0;
}

static void dsi_20nm_phy_disable(struct msm_dsi_phy *phy)
{
	dsi_phy_write(phy->base + REG_DSI_20nm_PHY_CTRL_0, 0);
	dsi_20nm_phy_regulator_ctrl(phy, false);
}

const struct msm_dsi_phy_cfg dsi_phy_20nm_cfgs = {
<<<<<<< HEAD
	.type = MSM_DSI_PHY_20NM,
	.src_pll_truthtable = { {false, true}, {false, true} },
=======
	.has_phy_regulator = true,
>>>>>>> upstream/android-13
	.reg_cfg = {
		.num = 2,
		.regs = {
			{"vddio", 100000, 100},	/* 1.8 V */
			{"vcca", 10000, 100},	/* 1.0 V */
		},
	},
	.ops = {
		.enable = dsi_20nm_phy_enable,
		.disable = dsi_20nm_phy_disable,
<<<<<<< HEAD
		.init = msm_dsi_phy_init_common,
=======
>>>>>>> upstream/android-13
	},
	.io_start = { 0xfd998500, 0xfd9a0500 },
	.num_dsi_phy = 2,
};

