<<<<<<< HEAD
/*
 * Copyright (C) 2016 NVIDIA CORPORATION, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2016 NVIDIA CORPORATION, All Rights Reserved.
>>>>>>> upstream/android-13
 */
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/irqchip/arm-gic.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/pm_clock.h>
=======
>>>>>>> upstream/android-13
#include <linux/pm_runtime.h>
#include <linux/slab.h>

struct gic_clk_data {
	unsigned int num_clocks;
	const char *const *clocks;
};

<<<<<<< HEAD
static int gic_runtime_resume(struct device *dev)
{
	struct gic_chip_data *gic = dev_get_drvdata(dev);
	int ret;

	ret = pm_clk_resume(dev);
=======
struct gic_chip_pm {
	struct gic_chip_data *chip_data;
	const struct gic_clk_data *clk_data;
	struct clk_bulk_data *clks;
};

static int gic_runtime_resume(struct device *dev)
{
	struct gic_chip_pm *chip_pm = dev_get_drvdata(dev);
	struct gic_chip_data *gic = chip_pm->chip_data;
	const struct gic_clk_data *data = chip_pm->clk_data;
	int ret;

	ret = clk_bulk_prepare_enable(data->num_clocks, chip_pm->clks);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	/*
<<<<<<< HEAD
	 * On the very first resume, the pointer to the driver data
=======
	 * On the very first resume, the pointer to chip_pm->chip_data
>>>>>>> upstream/android-13
	 * will be NULL and this is intentional, because we do not
	 * want to restore the GIC on the very first resume. So if
	 * the pointer is not valid just return.
	 */
	if (!gic)
		return 0;

	gic_dist_restore(gic);
	gic_cpu_restore(gic);

	return 0;
}

static int gic_runtime_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct gic_chip_data *gic = dev_get_drvdata(dev);
=======
	struct gic_chip_pm *chip_pm = dev_get_drvdata(dev);
	struct gic_chip_data *gic = chip_pm->chip_data;
	const struct gic_clk_data *data = chip_pm->clk_data;
>>>>>>> upstream/android-13

	gic_dist_save(gic);
	gic_cpu_save(gic);

<<<<<<< HEAD
	return pm_clk_suspend(dev);
}

static int gic_get_clocks(struct device *dev, const struct gic_clk_data *data)
{
	unsigned int i;
	int ret;

	if (!dev || !data)
		return -EINVAL;

	ret = pm_clk_create(dev);
	if (ret)
		return ret;

	for (i = 0; i < data->num_clocks; i++) {
		ret = of_pm_clk_add_clk(dev, data->clocks[i]);
		if (ret) {
			dev_err(dev, "failed to add clock %s\n",
				data->clocks[i]);
			pm_clk_destroy(dev);
			return ret;
		}
	}
=======
	clk_bulk_disable_unprepare(data->num_clocks, chip_pm->clks);
>>>>>>> upstream/android-13

	return 0;
}

static int gic_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const struct gic_clk_data *data;
<<<<<<< HEAD
	struct gic_chip_data *gic;
	int ret, irq;
=======
	struct gic_chip_pm *chip_pm;
	int ret, irq, i;
>>>>>>> upstream/android-13

	data = of_device_get_match_data(&pdev->dev);
	if (!data) {
		dev_err(&pdev->dev, "no device match found\n");
		return -ENODEV;
	}

<<<<<<< HEAD
=======
	chip_pm = devm_kzalloc(dev, sizeof(*chip_pm), GFP_KERNEL);
	if (!chip_pm)
		return -ENOMEM;

>>>>>>> upstream/android-13
	irq = irq_of_parse_and_map(dev->of_node, 0);
	if (!irq) {
		dev_err(dev, "no parent interrupt found!\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	ret = gic_get_clocks(dev, data);
	if (ret)
		goto irq_dispose;

=======
	chip_pm->clks = devm_kcalloc(dev, data->num_clocks,
				     sizeof(*chip_pm->clks), GFP_KERNEL);
	if (!chip_pm->clks)
		return -ENOMEM;

	for (i = 0; i < data->num_clocks; i++)
		chip_pm->clks[i].id = data->clocks[i];

	ret = devm_clk_bulk_get(dev, data->num_clocks, chip_pm->clks);
	if (ret)
		goto irq_dispose;

	chip_pm->clk_data = data;
	dev_set_drvdata(dev, chip_pm);

>>>>>>> upstream/android-13
	pm_runtime_enable(dev);

	ret = pm_runtime_get_sync(dev);
	if (ret < 0)
		goto rpm_disable;

<<<<<<< HEAD
	ret = gic_of_init_child(dev, &gic, irq);
	if (ret)
		goto rpm_put;

	platform_set_drvdata(pdev, gic);

=======
	ret = gic_of_init_child(dev, &chip_pm->chip_data, irq);
	if (ret)
		goto rpm_put;

>>>>>>> upstream/android-13
	pm_runtime_put(dev);

	dev_info(dev, "GIC IRQ controller registered\n");

	return 0;

rpm_put:
	pm_runtime_put_sync(dev);
rpm_disable:
	pm_runtime_disable(dev);
<<<<<<< HEAD
	pm_clk_destroy(dev);
=======
>>>>>>> upstream/android-13
irq_dispose:
	irq_dispose_mapping(irq);

	return ret;
}

static const struct dev_pm_ops gic_pm_ops = {
	SET_RUNTIME_PM_OPS(gic_runtime_suspend,
			   gic_runtime_resume, NULL)
<<<<<<< HEAD
=======
	SET_LATE_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				     pm_runtime_force_resume)
>>>>>>> upstream/android-13
};

static const char * const gic400_clocks[] = {
	"clk",
};

static const struct gic_clk_data gic400_data = {
	.num_clocks = ARRAY_SIZE(gic400_clocks),
	.clocks = gic400_clocks,
};

static const struct of_device_id gic_match[] = {
	{ .compatible = "nvidia,tegra210-agic",	.data = &gic400_data },
	{},
};
MODULE_DEVICE_TABLE(of, gic_match);

static struct platform_driver gic_driver = {
	.probe		= gic_probe,
	.driver		= {
		.name	= "gic",
		.of_match_table	= gic_match,
		.pm	= &gic_pm_ops,
	}
};

builtin_platform_driver(gic_driver);
