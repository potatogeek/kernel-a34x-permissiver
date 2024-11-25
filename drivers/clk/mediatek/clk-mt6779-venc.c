// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Wendell Lin <wendell.lin@mediatek.com>
 */

<<<<<<< HEAD
#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
=======
#include <linux/module.h>
#include <linux/clk-provider.h>
#include <linux/platform_device.h>
>>>>>>> upstream/android-13

#include "clk-mtk.h"
#include "clk-gate.h"

#include <dt-bindings/clock/mt6779-clk.h>

<<<<<<< HEAD
#define MT_CLKMGR_MODULE_INIT	0
#define CCF_SUBSYS_DEBUG		1

=======
>>>>>>> upstream/android-13
static const struct mtk_gate_regs venc_cg_regs = {
	.set_ofs = 0x0004,
	.clr_ofs = 0x0008,
	.sta_ofs = 0x0000,
};

<<<<<<< HEAD
#define GATE_VENC_DUMMY(_id, _name, _parent, _shift) {	\
	.id = _id,					\
	.name = _name,					\
	.parent_name = _parent,				\
	.regs = &venc_cg_regs,				\
	.shift = _shift,				\
	.ops = &mtk_clk_gate_ops_setclr_inv_dummy,	\
}

static const struct mtk_gate venc_clks[] = {
	GATE_VENC_DUMMY(CLK_VENC_GCON_LARB, "venc_larb", "venc_sel", 0),
	GATE_VENC_DUMMY(CLK_VENC_GCON_VENC, "venc_venc", "venc_sel", 4),
	GATE_VENC_DUMMY(CLK_VENC_GCON_JPGENC, "venc_jpgenc", "venc_sel", 8),
	GATE_VENC_DUMMY(CLK_VENC_GCON_GALS, "venc_gals", "venc_sel", 28),
=======
#define GATE_VENC_I(_id, _name, _parent, _shift)		\
	GATE_MTK(_id, _name, _parent, &venc_cg_regs, _shift,	\
		&mtk_clk_gate_ops_setclr_inv)

static const struct mtk_gate venc_clks[] = {
	GATE_VENC_I(CLK_VENC_GCON_LARB, "venc_larb", "venc_sel", 0),
	GATE_VENC_I(CLK_VENC_GCON_VENC, "venc_venc", "venc_sel", 4),
	GATE_VENC_I(CLK_VENC_GCON_JPGENC, "venc_jpgenc", "venc_sel", 8),
	GATE_VENC_I(CLK_VENC_GCON_GALS, "venc_gals", "venc_sel", 28),
>>>>>>> upstream/android-13
};

static const struct of_device_id of_match_clk_mt6779_venc[] = {
	{ .compatible = "mediatek,mt6779-vencsys", },
	{}
};

static int clk_mt6779_venc_probe(struct platform_device *pdev)
{
	struct clk_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
<<<<<<< HEAD
	int ret;

	clk_data = mtk_alloc_clk_data(CLK_VENC_GCON_NR_CLK);
	if (!clk_data) {
		pr_notice("%s(): alloc clk data failed\n", __func__);
		return -ENOMEM;
	}

#if CCF_SUBSYS_DEBUG
	pr_info("%s(): clk data number: %d\n", __func__, clk_data->clk_num);
#endif
=======

	clk_data = mtk_alloc_clk_data(CLK_VENC_GCON_NR_CLK);
>>>>>>> upstream/android-13

	mtk_clk_register_gates(node, venc_clks, ARRAY_SIZE(venc_clks),
			       clk_data);

<<<<<<< HEAD
	ret = of_clk_add_provider(node, of_clk_src_onecell_get, clk_data);

	if (ret) {
		pr_notice("%s(): could not register clock provider: %d\n",
					__func__, ret);

		kfree(clk_data);
	}

	return ret;
=======
	return of_clk_add_provider(node, of_clk_src_onecell_get, clk_data);
>>>>>>> upstream/android-13
}

static struct platform_driver clk_mt6779_venc_drv = {
	.probe = clk_mt6779_venc_probe,
	.driver = {
		.name = "clk-mt6779-venc",
		.of_match_table = of_match_clk_mt6779_venc,
	},
};

<<<<<<< HEAD

#if MT_CLKMGR_MODULE_INIT

builtin_platform_driver(clk_mt6779_venc_drv);

#else

static int __init clk_mt6779_venc_platform_init(void)
{
	return platform_driver_register(&clk_mt6779_venc_drv);
}

arch_initcall_sync(clk_mt6779_venc_platform_init);

#endif /* MT_CLKMGR_MODULE_INIT */

=======
module_platform_driver(clk_mt6779_venc_drv);
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
