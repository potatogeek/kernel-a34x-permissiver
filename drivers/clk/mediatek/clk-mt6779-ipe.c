// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Wendell Lin <wendell.lin@mediatek.com>
 */

<<<<<<< HEAD
#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <dt-bindings/clock/mt6779-clk.h>
#include <linux/slab.h>
=======
#include <linux/module.h>
#include <linux/clk-provider.h>
#include <linux/platform_device.h>
#include <dt-bindings/clock/mt6779-clk.h>
>>>>>>> upstream/android-13

#include "clk-mtk.h"
#include "clk-gate.h"

<<<<<<< HEAD
#define MT_CLKMGR_MODULE_INIT	0
#define CCF_SUBSYS_DEBUG		1

=======
>>>>>>> upstream/android-13
static const struct mtk_gate_regs ipe_cg_regs = {
	.set_ofs = 0x0004,
	.clr_ofs = 0x0008,
	.sta_ofs = 0x0000,
};

<<<<<<< HEAD
#define GATE_IPE(_id, _name, _parent, _shift) {	\
	.id = _id,			\
	.name = _name,			\
	.parent_name = _parent,		\
	.regs = &ipe_cg_regs,		\
	.shift = _shift,		\
	.ops = &mtk_clk_gate_ops_setclr,\
}

#define GATE_IPE_DUMMY(_id, _name, _parent, _shift) {	\
	.id = _id,				\
	.name = _name,				\
	.parent_name = _parent,			\
	.regs = &ipe_cg_regs,			\
	.shift = _shift,			\
	.ops = &mtk_clk_gate_ops_setclr_dummy,	\
}

static const struct mtk_gate ipe_clks[] = {
	GATE_IPE_DUMMY(CLK_IPE_LARB7, "ipe_larb7", "ipe_sel", 0),
	GATE_IPE_DUMMY(CLK_IPE_LARB8, "ipe_larb8", "ipe_sel", 1),
=======
#define GATE_IPE(_id, _name, _parent, _shift)			\
	GATE_MTK(_id, _name, _parent, &ipe_cg_regs, _shift,	\
		&mtk_clk_gate_ops_setclr)

static const struct mtk_gate ipe_clks[] = {
	GATE_IPE(CLK_IPE_LARB7, "ipe_larb7", "ipe_sel", 0),
	GATE_IPE(CLK_IPE_LARB8, "ipe_larb8", "ipe_sel", 1),
>>>>>>> upstream/android-13
	GATE_IPE(CLK_IPE_SMI_SUBCOM, "ipe_smi_subcom", "ipe_sel", 2),
	GATE_IPE(CLK_IPE_FD, "ipe_fd", "ipe_sel", 3),
	GATE_IPE(CLK_IPE_FE, "ipe_fe", "ipe_sel", 4),
	GATE_IPE(CLK_IPE_RSC, "ipe_rsc", "ipe_sel", 5),
	GATE_IPE(CLK_IPE_DPE, "ipe_dpe", "ipe_sel", 6),
};

static const struct of_device_id of_match_clk_mt6779_ipe[] = {
	{ .compatible = "mediatek,mt6779-ipesys", },
	{}
};

static int clk_mt6779_ipe_probe(struct platform_device *pdev)
{
	struct clk_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
<<<<<<< HEAD
	int ret;

	clk_data = mtk_alloc_clk_data(CLK_IPE_NR_CLK);
	if (!clk_data) {
		pr_notice("%s(): alloc clk data failed\n", __func__);
		return -ENOMEM;
	}

#if CCF_SUBSYS_DEBUG
	pr_info("%s(): clk data number: %d\n", __func__, clk_data->clk_num);
#endif
=======

	clk_data = mtk_alloc_clk_data(CLK_IPE_NR_CLK);
>>>>>>> upstream/android-13

	mtk_clk_register_gates(node, ipe_clks, ARRAY_SIZE(ipe_clks),
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

static struct platform_driver clk_mt6779_ipe_drv = {
	.probe = clk_mt6779_ipe_probe,
	.driver = {
		.name = "clk-mt6779-ipe",
		.of_match_table = of_match_clk_mt6779_ipe,
	},
};

<<<<<<< HEAD

#if MT_CLKMGR_MODULE_INIT

builtin_platform_driver(clk_mt6779_ipe_drv);

#else

static int __init clk_mt6779_ipe_platform_init(void)
{
	return platform_driver_register(&clk_mt6779_ipe_drv);
}

arch_initcall_sync(clk_mt6779_ipe_platform_init);

#endif /* MT_CLKMGR_MODULE_INIT */

=======
module_platform_driver(clk_mt6779_ipe_drv);
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
