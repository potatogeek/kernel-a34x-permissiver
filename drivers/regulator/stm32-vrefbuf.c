<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) STMicroelectronics 2017
 *
 * Author: Fabrice Gasnier <fabrice.gasnier@st.com>
<<<<<<< HEAD
 *
 * License terms:  GNU General Public License (GPL), version 2
=======
>>>>>>> upstream/android-13
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
<<<<<<< HEAD
=======
#include <linux/pm_runtime.h>
>>>>>>> upstream/android-13

/* STM32 VREFBUF registers */
#define STM32_VREFBUF_CSR		0x00

/* STM32 VREFBUF CSR bitfields */
#define STM32_VRS			GENMASK(6, 4)
#define STM32_VRR			BIT(3)
#define STM32_HIZ			BIT(1)
#define STM32_ENVR			BIT(0)

<<<<<<< HEAD
struct stm32_vrefbuf {
	void __iomem *base;
	struct clk *clk;
=======
#define STM32_VREFBUF_AUTO_SUSPEND_DELAY_MS	10

struct stm32_vrefbuf {
	void __iomem *base;
	struct clk *clk;
	struct device *dev;
>>>>>>> upstream/android-13
};

static const unsigned int stm32_vrefbuf_voltages[] = {
	/* Matches resp. VRS = 000b, 001b, 010b, 011b */
	2500000, 2048000, 1800000, 1500000,
};

static int stm32_vrefbuf_enable(struct regulator_dev *rdev)
{
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	u32 val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);
	int ret;

=======
	u32 val;
	int ret;

	ret = pm_runtime_get_sync(priv->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->dev);
		return ret;
	}

	val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);
>>>>>>> upstream/android-13
	val = (val & ~STM32_HIZ) | STM32_ENVR;
	writel_relaxed(val, priv->base + STM32_VREFBUF_CSR);

	/*
	 * Vrefbuf startup time depends on external capacitor: wait here for
	 * VRR to be set. That means output has reached expected value.
	 * ~650us sleep should be enough for caps up to 1.5uF. Use 10ms as
	 * arbitrary timeout.
	 */
	ret = readl_poll_timeout(priv->base + STM32_VREFBUF_CSR, val,
				 val & STM32_VRR, 650, 10000);
	if (ret) {
		dev_err(&rdev->dev, "stm32 vrefbuf timed out!\n");
		val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);
		val = (val & ~STM32_ENVR) | STM32_HIZ;
		writel_relaxed(val, priv->base + STM32_VREFBUF_CSR);
	}

<<<<<<< HEAD
=======
	pm_runtime_mark_last_busy(priv->dev);
	pm_runtime_put_autosuspend(priv->dev);

>>>>>>> upstream/android-13
	return ret;
}

static int stm32_vrefbuf_disable(struct regulator_dev *rdev)
{
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	u32 val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);

	val = (val & ~STM32_ENVR) | STM32_HIZ;
	writel_relaxed(val, priv->base + STM32_VREFBUF_CSR);

=======
	u32 val;
	int ret;

	ret = pm_runtime_get_sync(priv->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->dev);
		return ret;
	}

	val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);
	val &= ~STM32_ENVR;
	writel_relaxed(val, priv->base + STM32_VREFBUF_CSR);

	pm_runtime_mark_last_busy(priv->dev);
	pm_runtime_put_autosuspend(priv->dev);

>>>>>>> upstream/android-13
	return 0;
}

static int stm32_vrefbuf_is_enabled(struct regulator_dev *rdev)
{
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);
<<<<<<< HEAD

	return readl_relaxed(priv->base + STM32_VREFBUF_CSR) & STM32_ENVR;
=======
	int ret;

	ret = pm_runtime_get_sync(priv->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->dev);
		return ret;
	}

	ret = readl_relaxed(priv->base + STM32_VREFBUF_CSR) & STM32_ENVR;

	pm_runtime_mark_last_busy(priv->dev);
	pm_runtime_put_autosuspend(priv->dev);

	return ret;
>>>>>>> upstream/android-13
}

static int stm32_vrefbuf_set_voltage_sel(struct regulator_dev *rdev,
					 unsigned sel)
{
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	u32 val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);

	val = (val & ~STM32_VRS) | FIELD_PREP(STM32_VRS, sel);
	writel_relaxed(val, priv->base + STM32_VREFBUF_CSR);

=======
	u32 val;
	int ret;

	ret = pm_runtime_get_sync(priv->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->dev);
		return ret;
	}

	val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);
	val = (val & ~STM32_VRS) | FIELD_PREP(STM32_VRS, sel);
	writel_relaxed(val, priv->base + STM32_VREFBUF_CSR);

	pm_runtime_mark_last_busy(priv->dev);
	pm_runtime_put_autosuspend(priv->dev);

>>>>>>> upstream/android-13
	return 0;
}

static int stm32_vrefbuf_get_voltage_sel(struct regulator_dev *rdev)
{
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	u32 val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);

	return FIELD_GET(STM32_VRS, val);
=======
	u32 val;
	int ret;

	ret = pm_runtime_get_sync(priv->dev);
	if (ret < 0) {
		pm_runtime_put_noidle(priv->dev);
		return ret;
	}

	val = readl_relaxed(priv->base + STM32_VREFBUF_CSR);
	ret = FIELD_GET(STM32_VRS, val);

	pm_runtime_mark_last_busy(priv->dev);
	pm_runtime_put_autosuspend(priv->dev);

	return ret;
>>>>>>> upstream/android-13
}

static const struct regulator_ops stm32_vrefbuf_volt_ops = {
	.enable		= stm32_vrefbuf_enable,
	.disable	= stm32_vrefbuf_disable,
	.is_enabled	= stm32_vrefbuf_is_enabled,
	.get_voltage_sel = stm32_vrefbuf_get_voltage_sel,
	.set_voltage_sel = stm32_vrefbuf_set_voltage_sel,
	.list_voltage	= regulator_list_voltage_table,
};

static const struct regulator_desc stm32_vrefbuf_regu = {
	.name = "vref",
	.supply_name = "vdda",
	.volt_table = stm32_vrefbuf_voltages,
	.n_voltages = ARRAY_SIZE(stm32_vrefbuf_voltages),
	.ops = &stm32_vrefbuf_volt_ops,
<<<<<<< HEAD
=======
	.off_on_delay = 1000,
>>>>>>> upstream/android-13
	.type = REGULATOR_VOLTAGE,
	.owner = THIS_MODULE,
};

static int stm32_vrefbuf_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	struct stm32_vrefbuf *priv;
	struct regulator_config config = { };
	struct regulator_dev *rdev;
	int ret;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
<<<<<<< HEAD

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->base = devm_ioremap_resource(&pdev->dev, res);
=======
	priv->dev = &pdev->dev;

	priv->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(priv->base))
		return PTR_ERR(priv->base);

	priv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk))
		return PTR_ERR(priv->clk);

<<<<<<< HEAD
	ret = clk_prepare_enable(priv->clk);
	if (ret) {
		dev_err(&pdev->dev, "clk prepare failed with error %d\n", ret);
		return ret;
=======
	pm_runtime_get_noresume(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_set_autosuspend_delay(&pdev->dev,
					 STM32_VREFBUF_AUTO_SUSPEND_DELAY_MS);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	ret = clk_prepare_enable(priv->clk);
	if (ret) {
		dev_err(&pdev->dev, "clk prepare failed with error %d\n", ret);
		goto err_pm_stop;
>>>>>>> upstream/android-13
	}

	config.dev = &pdev->dev;
	config.driver_data = priv;
	config.of_node = pdev->dev.of_node;
	config.init_data = of_get_regulator_init_data(&pdev->dev,
						      pdev->dev.of_node,
						      &stm32_vrefbuf_regu);

	rdev = regulator_register(&stm32_vrefbuf_regu, &config);
	if (IS_ERR(rdev)) {
		ret = PTR_ERR(rdev);
		dev_err(&pdev->dev, "register failed with error %d\n", ret);
		goto err_clk_dis;
	}
	platform_set_drvdata(pdev, rdev);

<<<<<<< HEAD
=======
	pm_runtime_mark_last_busy(&pdev->dev);
	pm_runtime_put_autosuspend(&pdev->dev);

>>>>>>> upstream/android-13
	return 0;

err_clk_dis:
	clk_disable_unprepare(priv->clk);
<<<<<<< HEAD
=======
err_pm_stop:
	pm_runtime_disable(&pdev->dev);
	pm_runtime_set_suspended(&pdev->dev);
	pm_runtime_put_noidle(&pdev->dev);
>>>>>>> upstream/android-13

	return ret;
}

static int stm32_vrefbuf_remove(struct platform_device *pdev)
{
	struct regulator_dev *rdev = platform_get_drvdata(pdev);
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);

<<<<<<< HEAD
	regulator_unregister(rdev);
	clk_disable_unprepare(priv->clk);
=======
	pm_runtime_get_sync(&pdev->dev);
	regulator_unregister(rdev);
	clk_disable_unprepare(priv->clk);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_set_suspended(&pdev->dev);
	pm_runtime_put_noidle(&pdev->dev);
>>>>>>> upstream/android-13

	return 0;
};

<<<<<<< HEAD
static const struct of_device_id stm32_vrefbuf_of_match[] = {
=======
static int __maybe_unused stm32_vrefbuf_runtime_suspend(struct device *dev)
{
	struct regulator_dev *rdev = dev_get_drvdata(dev);
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);

	clk_disable_unprepare(priv->clk);

	return 0;
}

static int __maybe_unused stm32_vrefbuf_runtime_resume(struct device *dev)
{
	struct regulator_dev *rdev = dev_get_drvdata(dev);
	struct stm32_vrefbuf *priv = rdev_get_drvdata(rdev);

	return clk_prepare_enable(priv->clk);
}

static const struct dev_pm_ops stm32_vrefbuf_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				pm_runtime_force_resume)
	SET_RUNTIME_PM_OPS(stm32_vrefbuf_runtime_suspend,
			   stm32_vrefbuf_runtime_resume,
			   NULL)
};

static const struct of_device_id __maybe_unused stm32_vrefbuf_of_match[] = {
>>>>>>> upstream/android-13
	{ .compatible = "st,stm32-vrefbuf", },
	{},
};
MODULE_DEVICE_TABLE(of, stm32_vrefbuf_of_match);

static struct platform_driver stm32_vrefbuf_driver = {
	.probe = stm32_vrefbuf_probe,
	.remove = stm32_vrefbuf_remove,
	.driver = {
		.name  = "stm32-vrefbuf",
		.of_match_table = of_match_ptr(stm32_vrefbuf_of_match),
<<<<<<< HEAD
=======
		.pm = &stm32_vrefbuf_pm_ops,
>>>>>>> upstream/android-13
	},
};
module_platform_driver(stm32_vrefbuf_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Fabrice Gasnier <fabrice.gasnier@st.com>");
MODULE_DESCRIPTION("STMicroelectronics STM32 VREFBUF driver");
MODULE_ALIAS("platform:stm32-vrefbuf");
