// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
/**
 * dwc3-of-simple.c - OF glue layer for simple integrations
 *
 * Copyright (c) 2015 Texas Instruments Incorporated - http://www.ti.com
=======
/*
 * dwc3-of-simple.c - OF glue layer for simple integrations
 *
 * Copyright (c) 2015 Texas Instruments Incorporated - https://www.ti.com
>>>>>>> upstream/android-13
 *
 * Author: Felipe Balbi <balbi@ti.com>
 *
 * This is a combination of the old dwc3-qcom.c by Ivan T. Ivanov
 * <iivanov@mm-sol.com> and the original patch adding support for Xilinx' SoC
 * by Subbaraya Sundeep Bhatta <subbaraya.sundeep.bhatta@xilinx.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>

struct dwc3_of_simple {
	struct device		*dev;
<<<<<<< HEAD
	struct clk		**clks;
	int			num_clocks;
	struct reset_control	*resets;
	bool			pulse_resets;
	bool			need_reset;
};

static int dwc3_of_simple_clk_init(struct dwc3_of_simple *simple, int count)
{
	struct device		*dev = simple->dev;
	struct device_node	*np = dev->of_node;
	int			i;

	simple->num_clocks = count;

	if (!count)
		return 0;

	simple->clks = devm_kcalloc(dev, simple->num_clocks,
			sizeof(struct clk *), GFP_KERNEL);
	if (!simple->clks)
		return -ENOMEM;

	for (i = 0; i < simple->num_clocks; i++) {
		struct clk	*clk;
		int		ret;

		clk = of_clk_get(np, i);
		if (IS_ERR(clk)) {
			while (--i >= 0) {
				clk_disable_unprepare(simple->clks[i]);
				clk_put(simple->clks[i]);
			}
			return PTR_ERR(clk);
		}

		ret = clk_prepare_enable(clk);
		if (ret < 0) {
			while (--i >= 0) {
				clk_disable_unprepare(simple->clks[i]);
				clk_put(simple->clks[i]);
			}
			clk_put(clk);

			return ret;
		}

		simple->clks[i] = clk;
	}

	return 0;
}

=======
	struct clk_bulk_data	*clks;
	int			num_clocks;
	struct reset_control	*resets;
	bool			need_reset;
};

>>>>>>> upstream/android-13
static int dwc3_of_simple_probe(struct platform_device *pdev)
{
	struct dwc3_of_simple	*simple;
	struct device		*dev = &pdev->dev;
	struct device_node	*np = dev->of_node;

	int			ret;
<<<<<<< HEAD
	int			i;
	bool			shared_resets = false;
=======
>>>>>>> upstream/android-13

	simple = devm_kzalloc(dev, sizeof(*simple), GFP_KERNEL);
	if (!simple)
		return -ENOMEM;

	platform_set_drvdata(pdev, simple);
	simple->dev = dev;

	/*
	 * Some controllers need to toggle the usb3-otg reset before trying to
	 * initialize the PHY, otherwise the PHY times out.
	 */
	if (of_device_is_compatible(np, "rockchip,rk3399-dwc3"))
		simple->need_reset = true;

<<<<<<< HEAD
	if (of_device_is_compatible(np, "amlogic,meson-axg-dwc3") ||
	    of_device_is_compatible(np, "amlogic,meson-gxl-dwc3")) {
		shared_resets = true;
		simple->pulse_resets = true;
	}

	simple->resets = of_reset_control_array_get(np, shared_resets, true);
=======
	simple->resets = of_reset_control_array_get(np, false, true,
						    true);
>>>>>>> upstream/android-13
	if (IS_ERR(simple->resets)) {
		ret = PTR_ERR(simple->resets);
		dev_err(dev, "failed to get device resets, err=%d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	if (simple->pulse_resets) {
		ret = reset_control_reset(simple->resets);
		if (ret)
			goto err_resetc_put;
	} else {
		ret = reset_control_deassert(simple->resets);
		if (ret)
			goto err_resetc_put;
	}

	ret = dwc3_of_simple_clk_init(simple, of_count_phandle_with_args(np,
						"clocks", "#clock-cells"));
=======
	ret = reset_control_deassert(simple->resets);
	if (ret)
		goto err_resetc_put;

	ret = clk_bulk_get_all(simple->dev, &simple->clks);
	if (ret < 0)
		goto err_resetc_assert;

	simple->num_clocks = ret;
	ret = clk_bulk_prepare_enable(simple->num_clocks, simple->clks);
>>>>>>> upstream/android-13
	if (ret)
		goto err_resetc_assert;

	ret = of_platform_populate(np, NULL, NULL, dev);
<<<<<<< HEAD
	if (ret) {
		for (i = 0; i < simple->num_clocks; i++) {
			clk_disable_unprepare(simple->clks[i]);
			clk_put(simple->clks[i]);
		}

		goto err_resetc_assert;
	}
=======
	if (ret)
		goto err_clk_put;
>>>>>>> upstream/android-13

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);

	return 0;

<<<<<<< HEAD
err_resetc_assert:
	if (!simple->pulse_resets)
		reset_control_assert(simple->resets);
=======
err_clk_put:
	clk_bulk_disable_unprepare(simple->num_clocks, simple->clks);
	clk_bulk_put_all(simple->num_clocks, simple->clks);

err_resetc_assert:
	reset_control_assert(simple->resets);
>>>>>>> upstream/android-13

err_resetc_put:
	reset_control_put(simple->resets);
	return ret;
}

<<<<<<< HEAD
static int dwc3_of_simple_remove(struct platform_device *pdev)
{
	struct dwc3_of_simple	*simple = platform_get_drvdata(pdev);
	struct device		*dev = &pdev->dev;
	int			i;

	of_platform_depopulate(dev);

	for (i = 0; i < simple->num_clocks; i++) {
		clk_disable_unprepare(simple->clks[i]);
		clk_put(simple->clks[i]);
	}
	simple->num_clocks = 0;

	if (!simple->pulse_resets)
		reset_control_assert(simple->resets);

	reset_control_put(simple->resets);

	pm_runtime_disable(dev);
	pm_runtime_put_noidle(dev);
	pm_runtime_set_suspended(dev);
=======
static void __dwc3_of_simple_teardown(struct dwc3_of_simple *simple)
{
	of_platform_depopulate(simple->dev);

	clk_bulk_disable_unprepare(simple->num_clocks, simple->clks);
	clk_bulk_put_all(simple->num_clocks, simple->clks);
	simple->num_clocks = 0;

	reset_control_assert(simple->resets);

	reset_control_put(simple->resets);

	pm_runtime_disable(simple->dev);
	pm_runtime_put_noidle(simple->dev);
	pm_runtime_set_suspended(simple->dev);
}

static int dwc3_of_simple_remove(struct platform_device *pdev)
{
	struct dwc3_of_simple	*simple = platform_get_drvdata(pdev);

	__dwc3_of_simple_teardown(simple);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int __maybe_unused dwc3_of_simple_runtime_suspend(struct device *dev)
{
	struct dwc3_of_simple	*simple = dev_get_drvdata(dev);
	int			i;

	for (i = 0; i < simple->num_clocks; i++)
		clk_disable(simple->clks[i]);
=======
static void dwc3_of_simple_shutdown(struct platform_device *pdev)
{
	struct dwc3_of_simple	*simple = platform_get_drvdata(pdev);

	__dwc3_of_simple_teardown(simple);
}

static int __maybe_unused dwc3_of_simple_runtime_suspend(struct device *dev)
{
	struct dwc3_of_simple	*simple = dev_get_drvdata(dev);

	clk_bulk_disable(simple->num_clocks, simple->clks);
>>>>>>> upstream/android-13

	return 0;
}

static int __maybe_unused dwc3_of_simple_runtime_resume(struct device *dev)
{
	struct dwc3_of_simple	*simple = dev_get_drvdata(dev);
<<<<<<< HEAD
	int			ret;
	int			i;

	for (i = 0; i < simple->num_clocks; i++) {
		ret = clk_enable(simple->clks[i]);
		if (ret < 0) {
			while (--i >= 0)
				clk_disable(simple->clks[i]);
			return ret;
		}
	}

	return 0;
=======

	return clk_bulk_enable(simple->num_clocks, simple->clks);
>>>>>>> upstream/android-13
}

static int __maybe_unused dwc3_of_simple_suspend(struct device *dev)
{
	struct dwc3_of_simple *simple = dev_get_drvdata(dev);

	if (simple->need_reset)
		reset_control_assert(simple->resets);

	return 0;
}

static int __maybe_unused dwc3_of_simple_resume(struct device *dev)
{
	struct dwc3_of_simple *simple = dev_get_drvdata(dev);

	if (simple->need_reset)
		reset_control_deassert(simple->resets);

	return 0;
}

static const struct dev_pm_ops dwc3_of_simple_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwc3_of_simple_suspend, dwc3_of_simple_resume)
	SET_RUNTIME_PM_OPS(dwc3_of_simple_runtime_suspend,
			dwc3_of_simple_runtime_resume, NULL)
};

static const struct of_device_id of_dwc3_simple_match[] = {
	{ .compatible = "rockchip,rk3399-dwc3" },
<<<<<<< HEAD
	{ .compatible = "xlnx,zynqmp-dwc3" },
	{ .compatible = "cavium,octeon-7130-usb-uctl" },
	{ .compatible = "sprd,sc9860-dwc3" },
	{ .compatible = "amlogic,meson-axg-dwc3" },
	{ .compatible = "amlogic,meson-gxl-dwc3" },
	{ .compatible = "allwinner,sun50i-h6-dwc3" },
	{ .compatible = "hisilicon,hi3670-dwc3" },
=======
	{ .compatible = "cavium,octeon-7130-usb-uctl" },
	{ .compatible = "sprd,sc9860-dwc3" },
	{ .compatible = "allwinner,sun50i-h6-dwc3" },
	{ .compatible = "hisilicon,hi3670-dwc3" },
	{ .compatible = "intel,keembay-dwc3" },
>>>>>>> upstream/android-13
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_dwc3_simple_match);

static struct platform_driver dwc3_of_simple_driver = {
	.probe		= dwc3_of_simple_probe,
	.remove		= dwc3_of_simple_remove,
<<<<<<< HEAD
=======
	.shutdown	= dwc3_of_simple_shutdown,
>>>>>>> upstream/android-13
	.driver		= {
		.name	= "dwc3-of-simple",
		.of_match_table = of_dwc3_simple_match,
		.pm	= &dwc3_of_simple_dev_pm_ops,
	},
};

module_platform_driver(dwc3_of_simple_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DesignWare USB3 OF Simple Glue Layer");
MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
