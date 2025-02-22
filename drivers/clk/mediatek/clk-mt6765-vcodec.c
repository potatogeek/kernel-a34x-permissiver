// SPDX-License-Identifier: GPL-2.0
/*
<<<<<<< HEAD
 * Copyright (c) 2019 MediaTek Inc.
=======
 * Copyright (c) 2018 MediaTek Inc.
>>>>>>> upstream/android-13
 * Author: Owen Chen <owen.chen@mediatek.com>
 */

#include <linux/clk-provider.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
=======
#include <linux/platform_device.h>
>>>>>>> upstream/android-13

#include "clk-mtk.h"
#include "clk-gate.h"

#include <dt-bindings/clock/mt6765-clk.h>

<<<<<<< HEAD
/* Regular Number Definition */
#define INV_OFS			-1
#define INV_BIT			-1

/* get spm power status struct to register inside clk_data */
static struct pwr_status pwr_stat = GATE_PWR_STAT(0x180, 0x184, INV_OFS, BIT(26), BIT(26));

=======
>>>>>>> upstream/android-13
static const struct mtk_gate_regs venc_cg_regs = {
	.set_ofs = 0x4,
	.clr_ofs = 0x8,
	.sta_ofs = 0x0,
};

#define GATE_VENC(_id, _name, _parent, _shift) {	\
		.id = _id,				\
		.name = _name,				\
		.parent_name = _parent,			\
		.regs = &venc_cg_regs,			\
		.shift = _shift,			\
		.ops = &mtk_clk_gate_ops_setclr_inv,	\
<<<<<<< HEAD
		.pwr_stat = &pwr_stat,			\
	}

static const struct mtk_gate venc_clks[] __initconst = {
=======
	}

static const struct mtk_gate venc_clks[] = {
>>>>>>> upstream/android-13
	GATE_VENC(CLK_VENC_SET0_LARB, "venc_set0_larb", "mm_ck", 0),
	GATE_VENC(CLK_VENC_SET1_VENC, "venc_set1_venc", "mm_ck", 4),
	GATE_VENC(CLK_VENC_SET2_JPGENC, "jpgenc", "mm_ck", 8),
	GATE_VENC(CLK_VENC_SET3_VDEC, "venc_set3_vdec", "mm_ck", 12),
};

static int clk_mt6765_vcodec_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct device_node *node = pdev->dev.of_node;
	struct clk_onecell_data *clk_data;
	int r;

	clk_data = mtk_alloc_clk_data(CLK_VENC_NR_CLK);
	if (!clk_data)
		return -ENOMEM;
=======
	struct clk_onecell_data *clk_data;
	int r;
	struct device_node *node = pdev->dev.of_node;

	clk_data = mtk_alloc_clk_data(CLK_VENC_NR_CLK);
>>>>>>> upstream/android-13

	mtk_clk_register_gates(node, venc_clks,
			       ARRAY_SIZE(venc_clks), clk_data);

	r = of_clk_add_provider(node, of_clk_src_onecell_get, clk_data);

<<<<<<< HEAD
	if (r) {
		kfree(clk_data);
		pr_err("%s(): could not register clock provider: %d\n",
				__func__, r);
	}
=======
	if (r)
		pr_err("%s(): could not register clock provider: %d\n",
		       __func__, r);
>>>>>>> upstream/android-13

	return r;
}

static const struct of_device_id of_match_clk_mt6765_vcodec[] = {
	{ .compatible = "mediatek,mt6765-vcodecsys", },
	{}
};

static struct platform_driver clk_mt6765_vcodec_drv = {
	.probe = clk_mt6765_vcodec_probe,
	.driver = {
		.name = "clk-mt6765-vcodec",
		.of_match_table = of_match_clk_mt6765_vcodec,
	},
};

<<<<<<< HEAD
static int __init clk_mt6765_vcodec_init(void)
{
	return platform_driver_register(&clk_mt6765_vcodec_drv);
}

static void __exit clk_mt6765_vcodec_exit(void)
{
}

postcore_initcall(clk_mt6765_vcodec_init);
module_exit(clk_mt6765_vcodec_exit);
MODULE_LICENSE("GPL");
=======
builtin_platform_driver(clk_mt6765_vcodec_drv);
>>>>>>> upstream/android-13
