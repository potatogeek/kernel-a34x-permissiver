<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 * Author: Tomasz Figa <t.figa@samsung.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * Clock driver for Exynos clock output
 */

#include <linux/slab.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
<<<<<<< HEAD
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/syscore_ops.h>
=======
#include <linux/module.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
>>>>>>> upstream/android-13

#define EXYNOS_CLKOUT_NR_CLKS		1
#define EXYNOS_CLKOUT_PARENTS		32

#define EXYNOS_PMU_DEBUG_REG		0xa00
#define EXYNOS_CLKOUT_DISABLE_SHIFT	0
#define EXYNOS_CLKOUT_MUX_SHIFT		8
#define EXYNOS4_CLKOUT_MUX_MASK		0xf
#define EXYNOS5_CLKOUT_MUX_MASK		0x1f

struct exynos_clkout {
	struct clk_gate gate;
	struct clk_mux mux;
	spinlock_t slock;
	void __iomem *reg;
<<<<<<< HEAD
=======
	struct device_node *np;
>>>>>>> upstream/android-13
	u32 pmu_debug_save;
	struct clk_hw_onecell_data data;
};

<<<<<<< HEAD
static struct exynos_clkout *clkout;

static int exynos_clkout_suspend(void)
{
	clkout->pmu_debug_save = readl(clkout->reg + EXYNOS_PMU_DEBUG_REG);
=======
struct exynos_clkout_variant {
	u32 mux_mask;
};

static const struct exynos_clkout_variant exynos_clkout_exynos4 = {
	.mux_mask	= EXYNOS4_CLKOUT_MUX_MASK,
};

static const struct exynos_clkout_variant exynos_clkout_exynos5 = {
	.mux_mask	= EXYNOS5_CLKOUT_MUX_MASK,
};

static const struct of_device_id exynos_clkout_ids[] = {
	{
		.compatible = "samsung,exynos3250-pmu",
		.data = &exynos_clkout_exynos4,
	}, {
		.compatible = "samsung,exynos4210-pmu",
		.data = &exynos_clkout_exynos4,
	}, {
		.compatible = "samsung,exynos4412-pmu",
		.data = &exynos_clkout_exynos4,
	}, {
		.compatible = "samsung,exynos5250-pmu",
		.data = &exynos_clkout_exynos5,
	}, {
		.compatible = "samsung,exynos5410-pmu",
		.data = &exynos_clkout_exynos5,
	}, {
		.compatible = "samsung,exynos5420-pmu",
		.data = &exynos_clkout_exynos5,
	}, {
		.compatible = "samsung,exynos5433-pmu",
		.data = &exynos_clkout_exynos5,
	}, { }
};
MODULE_DEVICE_TABLE(of, exynos_clkout_ids);

/*
 * Device will be instantiated as child of PMU device without its own
 * device node.  Therefore match compatibles against parent.
 */
static int exynos_clkout_match_parent_dev(struct device *dev, u32 *mux_mask)
{
	const struct exynos_clkout_variant *variant;
	const struct of_device_id *match;

	if (!dev->parent) {
		dev_err(dev, "not instantiated from MFD\n");
		return -EINVAL;
	}

	match = of_match_device(exynos_clkout_ids, dev->parent);
	if (!match) {
		dev_err(dev, "cannot match parent device\n");
		return -EINVAL;
	}
	variant = match->data;

	*mux_mask = variant->mux_mask;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static void exynos_clkout_resume(void)
{
	writel(clkout->pmu_debug_save, clkout->reg + EXYNOS_PMU_DEBUG_REG);
}

static struct syscore_ops exynos_clkout_syscore_ops = {
	.suspend = exynos_clkout_suspend,
	.resume = exynos_clkout_resume,
};

static void __init exynos_clkout_init(struct device_node *node, u32 mux_mask)
{
	const char *parent_names[EXYNOS_CLKOUT_PARENTS];
	struct clk *parents[EXYNOS_CLKOUT_PARENTS];
	int parent_count;
	int ret;
	int i;

	clkout = kzalloc(struct_size(clkout, data.hws, EXYNOS_CLKOUT_NR_CLKS),
			 GFP_KERNEL);
	if (!clkout)
		return;
=======
static int exynos_clkout_probe(struct platform_device *pdev)
{
	const char *parent_names[EXYNOS_CLKOUT_PARENTS];
	struct clk *parents[EXYNOS_CLKOUT_PARENTS];
	struct exynos_clkout *clkout;
	int parent_count, ret, i;
	u32 mux_mask;

	clkout = devm_kzalloc(&pdev->dev,
			      struct_size(clkout, data.hws, EXYNOS_CLKOUT_NR_CLKS),
			      GFP_KERNEL);
	if (!clkout)
		return -ENOMEM;

	ret = exynos_clkout_match_parent_dev(&pdev->dev, &mux_mask);
	if (ret)
		return ret;

	clkout->np = pdev->dev.of_node;
	if (!clkout->np) {
		/*
		 * pdev->dev.parent was checked by exynos_clkout_match_parent_dev()
		 * so it is not NULL.
		 */
		clkout->np = pdev->dev.parent->of_node;
	}

	platform_set_drvdata(pdev, clkout);
>>>>>>> upstream/android-13

	spin_lock_init(&clkout->slock);

	parent_count = 0;
	for (i = 0; i < EXYNOS_CLKOUT_PARENTS; ++i) {
		char name[] = "clkoutXX";

		snprintf(name, sizeof(name), "clkout%d", i);
<<<<<<< HEAD
		parents[i] = of_clk_get_by_name(node, name);
=======
		parents[i] = of_clk_get_by_name(clkout->np, name);
>>>>>>> upstream/android-13
		if (IS_ERR(parents[i])) {
			parent_names[i] = "none";
			continue;
		}

		parent_names[i] = __clk_get_name(parents[i]);
		parent_count = i + 1;
	}

	if (!parent_count)
<<<<<<< HEAD
		goto free_clkout;

	clkout->reg = of_iomap(node, 0);
	if (!clkout->reg)
		goto clks_put;
=======
		return -EINVAL;

	clkout->reg = of_iomap(clkout->np, 0);
	if (!clkout->reg) {
		ret = -ENODEV;
		goto clks_put;
	}
>>>>>>> upstream/android-13

	clkout->gate.reg = clkout->reg + EXYNOS_PMU_DEBUG_REG;
	clkout->gate.bit_idx = EXYNOS_CLKOUT_DISABLE_SHIFT;
	clkout->gate.flags = CLK_GATE_SET_TO_DISABLE;
	clkout->gate.lock = &clkout->slock;

	clkout->mux.reg = clkout->reg + EXYNOS_PMU_DEBUG_REG;
	clkout->mux.mask = mux_mask;
	clkout->mux.shift = EXYNOS_CLKOUT_MUX_SHIFT;
	clkout->mux.lock = &clkout->slock;

	clkout->data.hws[0] = clk_hw_register_composite(NULL, "clkout",
				parent_names, parent_count, &clkout->mux.hw,
				&clk_mux_ops, NULL, NULL, &clkout->gate.hw,
				&clk_gate_ops, CLK_SET_RATE_PARENT
				| CLK_SET_RATE_NO_REPARENT);
<<<<<<< HEAD
	if (IS_ERR(clkout->data.hws[0]))
		goto err_unmap;

	clkout->data.num = EXYNOS_CLKOUT_NR_CLKS;
	ret = of_clk_add_hw_provider(node, of_clk_hw_onecell_get, &clkout->data);
	if (ret)
		goto err_clk_unreg;

	register_syscore_ops(&exynos_clkout_syscore_ops);

	return;
=======
	if (IS_ERR(clkout->data.hws[0])) {
		ret = PTR_ERR(clkout->data.hws[0]);
		goto err_unmap;
	}

	clkout->data.num = EXYNOS_CLKOUT_NR_CLKS;
	ret = of_clk_add_hw_provider(clkout->np, of_clk_hw_onecell_get, &clkout->data);
	if (ret)
		goto err_clk_unreg;

	return 0;
>>>>>>> upstream/android-13

err_clk_unreg:
	clk_hw_unregister(clkout->data.hws[0]);
err_unmap:
	iounmap(clkout->reg);
clks_put:
	for (i = 0; i < EXYNOS_CLKOUT_PARENTS; ++i)
		if (!IS_ERR(parents[i]))
			clk_put(parents[i]);
<<<<<<< HEAD
free_clkout:
	kfree(clkout);

	pr_err("%s: failed to register clkout clock\n", __func__);
}

/*
 * We use CLK_OF_DECLARE_DRIVER initialization method to avoid setting
 * the OF_POPULATED flag on the pmu device tree node, so later the
 * Exynos PMU platform device can be properly probed with PMU driver.
 */

static void __init exynos4_clkout_init(struct device_node *node)
{
	exynos_clkout_init(node, EXYNOS4_CLKOUT_MUX_MASK);
}
CLK_OF_DECLARE_DRIVER(exynos4210_clkout, "samsung,exynos4210-pmu",
		exynos4_clkout_init);
CLK_OF_DECLARE_DRIVER(exynos4412_clkout, "samsung,exynos4412-pmu",
		exynos4_clkout_init);
CLK_OF_DECLARE_DRIVER(exynos3250_clkout, "samsung,exynos3250-pmu",
		exynos4_clkout_init);

static void __init exynos5_clkout_init(struct device_node *node)
{
	exynos_clkout_init(node, EXYNOS5_CLKOUT_MUX_MASK);
}
CLK_OF_DECLARE_DRIVER(exynos5250_clkout, "samsung,exynos5250-pmu",
		exynos5_clkout_init);
CLK_OF_DECLARE_DRIVER(exynos5410_clkout, "samsung,exynos5410-pmu",
		exynos5_clkout_init);
CLK_OF_DECLARE_DRIVER(exynos5420_clkout, "samsung,exynos5420-pmu",
		exynos5_clkout_init);
CLK_OF_DECLARE_DRIVER(exynos5433_clkout, "samsung,exynos5433-pmu",
		exynos5_clkout_init);
=======

	dev_err(&pdev->dev, "failed to register clkout clock\n");

	return ret;
}

static int exynos_clkout_remove(struct platform_device *pdev)
{
	struct exynos_clkout *clkout = platform_get_drvdata(pdev);

	of_clk_del_provider(clkout->np);
	clk_hw_unregister(clkout->data.hws[0]);
	iounmap(clkout->reg);

	return 0;
}

static int __maybe_unused exynos_clkout_suspend(struct device *dev)
{
	struct exynos_clkout *clkout = dev_get_drvdata(dev);

	clkout->pmu_debug_save = readl(clkout->reg + EXYNOS_PMU_DEBUG_REG);

	return 0;
}

static int __maybe_unused exynos_clkout_resume(struct device *dev)
{
	struct exynos_clkout *clkout = dev_get_drvdata(dev);

	writel(clkout->pmu_debug_save, clkout->reg + EXYNOS_PMU_DEBUG_REG);

	return 0;
}

static SIMPLE_DEV_PM_OPS(exynos_clkout_pm_ops, exynos_clkout_suspend,
			 exynos_clkout_resume);

static struct platform_driver exynos_clkout_driver = {
	.driver = {
		.name = "exynos-clkout",
		.of_match_table = exynos_clkout_ids,
		.pm = &exynos_clkout_pm_ops,
	},
	.probe = exynos_clkout_probe,
	.remove = exynos_clkout_remove,
};
module_platform_driver(exynos_clkout_driver);

MODULE_AUTHOR("Krzysztof Kozlowski <krzk@kernel.org>");
MODULE_AUTHOR("Tomasz Figa <tomasz.figa@gmail.com>");
MODULE_DESCRIPTION("Samsung Exynos clock output driver");
MODULE_LICENSE("GPL");
>>>>>>> upstream/android-13
