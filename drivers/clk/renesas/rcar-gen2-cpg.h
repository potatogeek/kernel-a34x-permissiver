<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * R-Car Gen2 Clock Pulse Generator
 *
 * Copyright (C) 2016 Cogent Embedded Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation; version 2 of the License.
=======
>>>>>>> upstream/android-13
 */

#ifndef __CLK_RENESAS_RCAR_GEN2_CPG_H__
#define __CLK_RENESAS_RCAR_GEN2_CPG_H__

enum rcar_gen2_clk_types {
	CLK_TYPE_GEN2_MAIN = CLK_TYPE_CUSTOM,
	CLK_TYPE_GEN2_PLL0,
	CLK_TYPE_GEN2_PLL1,
	CLK_TYPE_GEN2_PLL3,
	CLK_TYPE_GEN2_Z,
	CLK_TYPE_GEN2_LB,
	CLK_TYPE_GEN2_ADSP,
	CLK_TYPE_GEN2_SDH,
	CLK_TYPE_GEN2_SD0,
	CLK_TYPE_GEN2_SD1,
	CLK_TYPE_GEN2_QSPI,
	CLK_TYPE_GEN2_RCAN,
};

struct rcar_gen2_cpg_pll_config {
<<<<<<< HEAD
	unsigned int extal_div;
	unsigned int pll1_mult;
	unsigned int pll3_mult;
	unsigned int pll0_mult;		/* leave as zero if PLL0CR exists */
=======
	u8 extal_div;
	u8 pll1_mult;
	u8 pll3_mult;
	u8 pll0_mult;		/* leave as zero if PLL0CR exists */
>>>>>>> upstream/android-13
};

struct clk *rcar_gen2_cpg_clk_register(struct device *dev,
	const struct cpg_core_clk *core, const struct cpg_mssr_info *info,
	struct clk **clks, void __iomem *base,
	struct raw_notifier_head *notifiers);
int rcar_gen2_cpg_init(const struct rcar_gen2_cpg_pll_config *config,
		       unsigned int pll0_div, u32 mode);

#endif
