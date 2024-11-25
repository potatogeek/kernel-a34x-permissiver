<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@google.com>
 *	Based on arch/arm/plat-omap/cpu-omap.c, (C) 2005 Nokia Corporation
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>

static struct cpufreq_frequency_table freq_table[] = {
	{ .frequency = 216000 },
	{ .frequency = 312000 },
	{ .frequency = 456000 },
	{ .frequency = 608000 },
	{ .frequency = 760000 },
	{ .frequency = 816000 },
	{ .frequency = 912000 },
	{ .frequency = 1000000 },
	{ .frequency = CPUFREQ_TABLE_END },
};

struct tegra20_cpufreq {
	struct device *dev;
	struct cpufreq_driver driver;
	struct clk *cpu_clk;
	struct clk *pll_x_clk;
	struct clk *pll_p_clk;
	bool pll_x_prepared;
};

static unsigned int tegra_get_intermediate(struct cpufreq_policy *policy,
					   unsigned int index)
{
	struct tegra20_cpufreq *cpufreq = cpufreq_get_driver_data();
	unsigned int ifreq = clk_get_rate(cpufreq->pll_p_clk) / 1000;

	/*
	 * Don't switch to intermediate freq if:
	 * - we are already at it, i.e. policy->cur == ifreq
	 * - index corresponds to ifreq
	 */
	if (freq_table[index].frequency == ifreq || policy->cur == ifreq)
		return 0;

	return ifreq;
}

static int tegra_target_intermediate(struct cpufreq_policy *policy,
				     unsigned int index)
{
	struct tegra20_cpufreq *cpufreq = cpufreq_get_driver_data();
	int ret;

	/*
	 * Take an extra reference to the main pll so it doesn't turn
	 * off when we move the cpu off of it as enabling it again while we
	 * switch to it from tegra_target() would take additional time.
	 *
	 * When target-freq is equal to intermediate freq we don't need to
	 * switch to an intermediate freq and so this routine isn't called.
	 * Also, we wouldn't be using pll_x anymore and must not take extra
	 * reference to it, as it can be disabled now to save some power.
	 */
	clk_prepare_enable(cpufreq->pll_x_clk);

	ret = clk_set_parent(cpufreq->cpu_clk, cpufreq->pll_p_clk);
	if (ret)
		clk_disable_unprepare(cpufreq->pll_x_clk);
	else
		cpufreq->pll_x_prepared = true;

	return ret;
}

static int tegra_target(struct cpufreq_policy *policy, unsigned int index)
{
	struct tegra20_cpufreq *cpufreq = cpufreq_get_driver_data();
	unsigned long rate = freq_table[index].frequency;
	unsigned int ifreq = clk_get_rate(cpufreq->pll_p_clk) / 1000;
	int ret;

	/*
	 * target freq == pll_p, don't need to take extra reference to pll_x_clk
	 * as it isn't used anymore.
	 */
	if (rate == ifreq)
		return clk_set_parent(cpufreq->cpu_clk, cpufreq->pll_p_clk);

	ret = clk_set_rate(cpufreq->pll_x_clk, rate * 1000);
	/* Restore to earlier frequency on error, i.e. pll_x */
	if (ret)
		dev_err(cpufreq->dev, "Failed to change pll_x to %lu\n", rate);

	ret = clk_set_parent(cpufreq->cpu_clk, cpufreq->pll_x_clk);
	/* This shouldn't fail while changing or restoring */
	WARN_ON(ret);

	/*
	 * Drop count to pll_x clock only if we switched to intermediate freq
	 * earlier while transitioning to a target frequency.
	 */
	if (cpufreq->pll_x_prepared) {
		clk_disable_unprepare(cpufreq->pll_x_clk);
		cpufreq->pll_x_prepared = false;
	}

	return ret;
}

static int tegra_cpu_init(struct cpufreq_policy *policy)
{
	struct tegra20_cpufreq *cpufreq = cpufreq_get_driver_data();
	int ret;

	clk_prepare_enable(cpufreq->cpu_clk);

	/* FIXME: what's the actual transition time? */
	ret = cpufreq_generic_init(policy, freq_table, 300 * 1000);
	if (ret) {
		clk_disable_unprepare(cpufreq->cpu_clk);
		return ret;
	}

	policy->clk = cpufreq->cpu_clk;
	policy->suspend_freq = freq_table[0].frequency;
	return 0;
}

static int tegra_cpu_exit(struct cpufreq_policy *policy)
{
	struct tegra20_cpufreq *cpufreq = cpufreq_get_driver_data();

	clk_disable_unprepare(cpufreq->cpu_clk);
	return 0;
=======
 */

#include <linux/bits.h>
#include <linux/cpu.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_opp.h>
#include <linux/types.h>

#include <soc/tegra/common.h>
#include <soc/tegra/fuse.h>

static bool cpu0_node_has_opp_v2_prop(void)
{
	struct device_node *np = of_cpu_device_node_get(0);
	bool ret = false;

	if (of_get_property(np, "operating-points-v2", NULL))
		ret = true;

	of_node_put(np);
	return ret;
}

static void tegra20_cpufreq_put_supported_hw(void *opp_table)
{
	dev_pm_opp_put_supported_hw(opp_table);
}

static void tegra20_cpufreq_dt_unregister(void *cpufreq_dt)
{
	platform_device_unregister(cpufreq_dt);
>>>>>>> upstream/android-13
}

static int tegra20_cpufreq_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct tegra20_cpufreq *cpufreq;
	int err;

	cpufreq = devm_kzalloc(&pdev->dev, sizeof(*cpufreq), GFP_KERNEL);
	if (!cpufreq)
		return -ENOMEM;

	cpufreq->cpu_clk = clk_get_sys(NULL, "cclk");
	if (IS_ERR(cpufreq->cpu_clk))
		return PTR_ERR(cpufreq->cpu_clk);

	cpufreq->pll_x_clk = clk_get_sys(NULL, "pll_x");
	if (IS_ERR(cpufreq->pll_x_clk)) {
		err = PTR_ERR(cpufreq->pll_x_clk);
		goto put_cpu;
	}

	cpufreq->pll_p_clk = clk_get_sys(NULL, "pll_p");
	if (IS_ERR(cpufreq->pll_p_clk)) {
		err = PTR_ERR(cpufreq->pll_p_clk);
		goto put_pll_x;
	}

	cpufreq->dev = &pdev->dev;
	cpufreq->driver.get = cpufreq_generic_get;
	cpufreq->driver.attr = cpufreq_generic_attr;
	cpufreq->driver.init = tegra_cpu_init;
	cpufreq->driver.exit = tegra_cpu_exit;
	cpufreq->driver.flags = CPUFREQ_NEED_INITIAL_FREQ_CHECK;
	cpufreq->driver.verify = cpufreq_generic_frequency_table_verify;
	cpufreq->driver.suspend = cpufreq_generic_suspend;
	cpufreq->driver.driver_data = cpufreq;
	cpufreq->driver.target_index = tegra_target;
	cpufreq->driver.get_intermediate = tegra_get_intermediate;
	cpufreq->driver.target_intermediate = tegra_target_intermediate;
	snprintf(cpufreq->driver.name, CPUFREQ_NAME_LEN, "tegra");

	err = cpufreq_register_driver(&cpufreq->driver);
	if (err)
		goto put_pll_p;

	platform_set_drvdata(pdev, cpufreq);

	return 0;

put_pll_p:
	clk_put(cpufreq->pll_p_clk);
put_pll_x:
	clk_put(cpufreq->pll_x_clk);
put_cpu:
	clk_put(cpufreq->cpu_clk);

	return err;
}

static int tegra20_cpufreq_remove(struct platform_device *pdev)
{
	struct tegra20_cpufreq *cpufreq = platform_get_drvdata(pdev);

	cpufreq_unregister_driver(&cpufreq->driver);

	clk_put(cpufreq->pll_p_clk);
	clk_put(cpufreq->pll_x_clk);
	clk_put(cpufreq->cpu_clk);
=======
	struct platform_device *cpufreq_dt;
	struct opp_table *opp_table;
	struct device *cpu_dev;
	u32 versions[2];
	int err;

	if (!cpu0_node_has_opp_v2_prop()) {
		dev_err(&pdev->dev, "operating points not found\n");
		dev_err(&pdev->dev, "please update your device tree\n");
		return -ENODEV;
	}

	if (of_machine_is_compatible("nvidia,tegra20")) {
		versions[0] = BIT(tegra_sku_info.cpu_process_id);
		versions[1] = BIT(tegra_sku_info.soc_speedo_id);
	} else {
		versions[0] = BIT(tegra_sku_info.cpu_process_id);
		versions[1] = BIT(tegra_sku_info.cpu_speedo_id);
	}

	dev_info(&pdev->dev, "hardware version 0x%x 0x%x\n",
		 versions[0], versions[1]);

	cpu_dev = get_cpu_device(0);
	if (WARN_ON(!cpu_dev))
		return -ENODEV;

	opp_table = dev_pm_opp_set_supported_hw(cpu_dev, versions, 2);
	err = PTR_ERR_OR_ZERO(opp_table);
	if (err) {
		dev_err(&pdev->dev, "failed to set supported hw: %d\n", err);
		return err;
	}

	err = devm_add_action_or_reset(&pdev->dev,
				       tegra20_cpufreq_put_supported_hw,
				       opp_table);
	if (err)
		return err;

	cpufreq_dt = platform_device_register_simple("cpufreq-dt", -1, NULL, 0);
	err = PTR_ERR_OR_ZERO(cpufreq_dt);
	if (err) {
		dev_err(&pdev->dev,
			"failed to create cpufreq-dt device: %d\n", err);
		return err;
	}

	err = devm_add_action_or_reset(&pdev->dev,
				       tegra20_cpufreq_dt_unregister,
				       cpufreq_dt);
	if (err)
		return err;
>>>>>>> upstream/android-13

	return 0;
}

static struct platform_driver tegra20_cpufreq_driver = {
	.probe		= tegra20_cpufreq_probe,
<<<<<<< HEAD
	.remove		= tegra20_cpufreq_remove,
=======
>>>>>>> upstream/android-13
	.driver		= {
		.name	= "tegra20-cpufreq",
	},
};
module_platform_driver(tegra20_cpufreq_driver);

MODULE_ALIAS("platform:tegra20-cpufreq");
MODULE_AUTHOR("Colin Cross <ccross@android.com>");
MODULE_DESCRIPTION("NVIDIA Tegra20 cpufreq driver");
MODULE_LICENSE("GPL");
