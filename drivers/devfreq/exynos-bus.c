<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Generic Exynos Bus frequency driver with DEVFREQ Framework
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 * Author : Chanwoo Choi <cw00.choi@samsung.com>
 *
 * This driver support Exynos Bus frequency feature by using
 * DEVFREQ framework and is based on drivers/devfreq/exynos/exynos4_bus.c.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/devfreq.h>
#include <linux/devfreq-event.h>
#include <linux/device.h>
#include <linux/export.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/of_device.h>
#include <linux/pm_opp.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>

#define DEFAULT_SATURATION_RATIO	40
#define DEFAULT_VOLTAGE_TOLERANCE	2

struct exynos_bus {
	struct device *dev;
=======
#include <linux/of.h>
#include <linux/pm_opp.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#define DEFAULT_SATURATION_RATIO	40

struct exynos_bus {
	struct device *dev;
	struct platform_device *icc_pdev;
>>>>>>> upstream/android-13

	struct devfreq *devfreq;
	struct devfreq_event_dev **edev;
	unsigned int edev_count;
	struct mutex lock;

	unsigned long curr_freq;

<<<<<<< HEAD
	struct regulator *regulator;
	struct clk *clk;
	unsigned int voltage_tolerance;
=======
	struct opp_table *opp_table;
	struct clk *clk;
>>>>>>> upstream/android-13
	unsigned int ratio;
};

/*
 * Control the devfreq-event device to get the current state of bus
 */
#define exynos_bus_ops_edev(ops)				\
static int exynos_bus_##ops(struct exynos_bus *bus)		\
{								\
	int i, ret;						\
								\
	for (i = 0; i < bus->edev_count; i++) {			\
		if (!bus->edev[i])				\
			continue;				\
		ret = devfreq_event_##ops(bus->edev[i]);	\
		if (ret < 0)					\
			return ret;				\
	}							\
								\
	return 0;						\
}
exynos_bus_ops_edev(enable_edev);
exynos_bus_ops_edev(disable_edev);
exynos_bus_ops_edev(set_event);

static int exynos_bus_get_event(struct exynos_bus *bus,
				struct devfreq_event_data *edata)
{
	struct devfreq_event_data event_data;
	unsigned long load_count = 0, total_count = 0;
	int i, ret = 0;

	for (i = 0; i < bus->edev_count; i++) {
		if (!bus->edev[i])
			continue;

		ret = devfreq_event_get_event(bus->edev[i], &event_data);
		if (ret < 0)
			return ret;

		if (i == 0 || event_data.load_count > load_count) {
			load_count = event_data.load_count;
			total_count = event_data.total_count;
		}
	}

	edata->load_count = load_count;
	edata->total_count = total_count;

	return ret;
}

/*
<<<<<<< HEAD
 * Must necessary function for devfreq simple-ondemand governor
=======
 * devfreq function for both simple-ondemand and passive governor
>>>>>>> upstream/android-13
 */
static int exynos_bus_target(struct device *dev, unsigned long *freq, u32 flags)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);
	struct dev_pm_opp *new_opp;
<<<<<<< HEAD
	unsigned long old_freq, new_freq, new_volt, tol;
	int ret = 0;

	/* Get new opp-bus instance according to new bus clock */
=======
	int ret = 0;

	/* Get correct frequency for bus. */
>>>>>>> upstream/android-13
	new_opp = devfreq_recommended_opp(dev, freq, flags);
	if (IS_ERR(new_opp)) {
		dev_err(dev, "failed to get recommended opp instance\n");
		return PTR_ERR(new_opp);
	}

<<<<<<< HEAD
	new_freq = dev_pm_opp_get_freq(new_opp);
	new_volt = dev_pm_opp_get_voltage(new_opp);
	dev_pm_opp_put(new_opp);

	old_freq = bus->curr_freq;

	if (old_freq == new_freq)
		return 0;
	tol = new_volt * bus->voltage_tolerance / 100;

	/* Change voltage and frequency according to new OPP level */
	mutex_lock(&bus->lock);

	if (old_freq < new_freq) {
		ret = regulator_set_voltage_tol(bus->regulator, new_volt, tol);
		if (ret < 0) {
			dev_err(bus->dev, "failed to set voltage\n");
			goto out;
		}
	}

	ret = clk_set_rate(bus->clk, new_freq);
	if (ret < 0) {
		dev_err(dev, "failed to change clock of bus\n");
		clk_set_rate(bus->clk, old_freq);
		goto out;
	}

	if (old_freq > new_freq) {
		ret = regulator_set_voltage_tol(bus->regulator, new_volt, tol);
		if (ret < 0) {
			dev_err(bus->dev, "failed to set voltage\n");
			goto out;
		}
	}
	bus->curr_freq = new_freq;

	dev_dbg(dev, "Set the frequency of bus (%luHz -> %luHz, %luHz)\n",
			old_freq, new_freq, clk_get_rate(bus->clk));
out:
=======
	dev_pm_opp_put(new_opp);

	/* Change voltage and frequency according to new OPP level */
	mutex_lock(&bus->lock);
	ret = dev_pm_opp_set_rate(dev, *freq);
	if (!ret)
		bus->curr_freq = *freq;

>>>>>>> upstream/android-13
	mutex_unlock(&bus->lock);

	return ret;
}

static int exynos_bus_get_dev_status(struct device *dev,
				     struct devfreq_dev_status *stat)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);
	struct devfreq_event_data edata;
	int ret;

	stat->current_frequency = bus->curr_freq;

	ret = exynos_bus_get_event(bus, &edata);
	if (ret < 0) {
<<<<<<< HEAD
=======
		dev_err(dev, "failed to get event from devfreq-event devices\n");
>>>>>>> upstream/android-13
		stat->total_time = stat->busy_time = 0;
		goto err;
	}

	stat->busy_time = (edata.load_count * 100) / bus->ratio;
	stat->total_time = edata.total_count;

	dev_dbg(dev, "Usage of devfreq-event : %lu/%lu\n", stat->busy_time,
							stat->total_time);

err:
	ret = exynos_bus_set_event(bus);
	if (ret < 0) {
		dev_err(dev, "failed to set event to devfreq-event devices\n");
		return ret;
	}

	return ret;
}

static void exynos_bus_exit(struct device *dev)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);
	int ret;

	ret = exynos_bus_disable_edev(bus);
	if (ret < 0)
		dev_warn(dev, "failed to disable the devfreq-event devices\n");

<<<<<<< HEAD
	dev_pm_opp_of_remove_table(dev);
	clk_disable_unprepare(bus->clk);
	if (bus->regulator)
		regulator_disable(bus->regulator);
}

/*
 * Must necessary function for devfreq passive governor
 */
static int exynos_bus_passive_target(struct device *dev, unsigned long *freq,
					u32 flags)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);
	struct dev_pm_opp *new_opp;
	unsigned long old_freq, new_freq;
	int ret = 0;

	/* Get new opp-bus instance according to new bus clock */
	new_opp = devfreq_recommended_opp(dev, freq, flags);
	if (IS_ERR(new_opp)) {
		dev_err(dev, "failed to get recommended opp instance\n");
		return PTR_ERR(new_opp);
	}

	new_freq = dev_pm_opp_get_freq(new_opp);
	dev_pm_opp_put(new_opp);

	old_freq = bus->curr_freq;

	if (old_freq == new_freq)
		return 0;

	/* Change the frequency according to new OPP level */
	mutex_lock(&bus->lock);

	ret = clk_set_rate(bus->clk, new_freq);
	if (ret < 0) {
		dev_err(dev, "failed to set the clock of bus\n");
		goto out;
	}

	*freq = new_freq;
	bus->curr_freq = new_freq;

	dev_dbg(dev, "Set the frequency of bus (%luHz -> %luHz, %luHz)\n",
			old_freq, new_freq, clk_get_rate(bus->clk));
out:
	mutex_unlock(&bus->lock);

	return ret;
=======
	platform_device_unregister(bus->icc_pdev);

	dev_pm_opp_of_remove_table(dev);
	clk_disable_unprepare(bus->clk);
	dev_pm_opp_put_regulators(bus->opp_table);
	bus->opp_table = NULL;
>>>>>>> upstream/android-13
}

static void exynos_bus_passive_exit(struct device *dev)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);

<<<<<<< HEAD
=======
	platform_device_unregister(bus->icc_pdev);

>>>>>>> upstream/android-13
	dev_pm_opp_of_remove_table(dev);
	clk_disable_unprepare(bus->clk);
}

static int exynos_bus_parent_parse_of(struct device_node *np,
					struct exynos_bus *bus)
{
	struct device *dev = bus->dev;
<<<<<<< HEAD
	int i, ret, count, size;

	/* Get the regulator to provide each bus with the power */
	bus->regulator = devm_regulator_get(dev, "vdd");
	if (IS_ERR(bus->regulator)) {
		dev_err(dev, "failed to get VDD regulator\n");
		return PTR_ERR(bus->regulator);
	}

	ret = regulator_enable(bus->regulator);
	if (ret < 0) {
		dev_err(dev, "failed to enable VDD regulator\n");
		return ret;
	}

=======
	struct opp_table *opp_table;
	const char *vdd = "vdd";
	int i, ret, count, size;

	opp_table = dev_pm_opp_set_regulators(dev, &vdd, 1);
	if (IS_ERR(opp_table)) {
		ret = PTR_ERR(opp_table);
		dev_err(dev, "failed to set regulators %d\n", ret);
		return ret;
	}

	bus->opp_table = opp_table;

>>>>>>> upstream/android-13
	/*
	 * Get the devfreq-event devices to get the current utilization of
	 * buses. This raw data will be used in devfreq ondemand governor.
	 */
<<<<<<< HEAD
	count = devfreq_event_get_edev_count(dev);
=======
	count = devfreq_event_get_edev_count(dev, "devfreq-events");
>>>>>>> upstream/android-13
	if (count < 0) {
		dev_err(dev, "failed to get the count of devfreq-event dev\n");
		ret = count;
		goto err_regulator;
	}
	bus->edev_count = count;

	size = sizeof(*bus->edev) * count;
	bus->edev = devm_kzalloc(dev, size, GFP_KERNEL);
	if (!bus->edev) {
		ret = -ENOMEM;
		goto err_regulator;
	}

	for (i = 0; i < count; i++) {
<<<<<<< HEAD
		bus->edev[i] = devfreq_event_get_edev_by_phandle(dev, i);
=======
		bus->edev[i] = devfreq_event_get_edev_by_phandle(dev,
							"devfreq-events", i);
>>>>>>> upstream/android-13
		if (IS_ERR(bus->edev[i])) {
			ret = -EPROBE_DEFER;
			goto err_regulator;
		}
	}

	/*
	 * Optionally, Get the saturation ratio according to Exynos SoC
	 * When measuring the utilization of each AXI bus with devfreq-event
	 * devices, the measured real cycle might be much lower than the
	 * total cycle of bus during sampling rate. In result, the devfreq
	 * simple-ondemand governor might not decide to change the current
	 * frequency due to too utilization (= real cycle/total cycle).
	 * So, this property is used to adjust the utilization when calculating
	 * the busy_time in exynos_bus_get_dev_status().
	 */
	if (of_property_read_u32(np, "exynos,saturation-ratio", &bus->ratio))
		bus->ratio = DEFAULT_SATURATION_RATIO;

<<<<<<< HEAD
	if (of_property_read_u32(np, "exynos,voltage-tolerance",
					&bus->voltage_tolerance))
		bus->voltage_tolerance = DEFAULT_VOLTAGE_TOLERANCE;

	return 0;

err_regulator:
	regulator_disable(bus->regulator);
=======
	return 0;

err_regulator:
	dev_pm_opp_put_regulators(bus->opp_table);
	bus->opp_table = NULL;
>>>>>>> upstream/android-13

	return ret;
}

static int exynos_bus_parse_of(struct device_node *np,
			      struct exynos_bus *bus)
{
	struct device *dev = bus->dev;
	struct dev_pm_opp *opp;
	unsigned long rate;
	int ret;

	/* Get the clock to provide each bus with source clock */
	bus->clk = devm_clk_get(dev, "bus");
	if (IS_ERR(bus->clk)) {
		dev_err(dev, "failed to get bus clock\n");
		return PTR_ERR(bus->clk);
	}

	ret = clk_prepare_enable(bus->clk);
	if (ret < 0) {
		dev_err(dev, "failed to get enable clock\n");
		return ret;
	}

	/* Get the freq and voltage from OPP table to scale the bus freq */
	ret = dev_pm_opp_of_add_table(dev);
	if (ret < 0) {
		dev_err(dev, "failed to get OPP table\n");
		goto err_clk;
	}

	rate = clk_get_rate(bus->clk);

	opp = devfreq_recommended_opp(dev, &rate, 0);
	if (IS_ERR(opp)) {
		dev_err(dev, "failed to find dev_pm_opp\n");
		ret = PTR_ERR(opp);
		goto err_opp;
	}
	bus->curr_freq = dev_pm_opp_get_freq(opp);
	dev_pm_opp_put(opp);

	return 0;

err_opp:
	dev_pm_opp_of_remove_table(dev);
err_clk:
	clk_disable_unprepare(bus->clk);

	return ret;
}

<<<<<<< HEAD
=======
static int exynos_bus_profile_init(struct exynos_bus *bus,
				   struct devfreq_dev_profile *profile)
{
	struct device *dev = bus->dev;
	struct devfreq_simple_ondemand_data *ondemand_data;
	int ret;

	/* Initialize the struct profile and governor data for parent device */
	profile->polling_ms = 50;
	profile->target = exynos_bus_target;
	profile->get_dev_status = exynos_bus_get_dev_status;
	profile->exit = exynos_bus_exit;

	ondemand_data = devm_kzalloc(dev, sizeof(*ondemand_data), GFP_KERNEL);
	if (!ondemand_data)
		return -ENOMEM;

	ondemand_data->upthreshold = 40;
	ondemand_data->downdifferential = 5;

	/* Add devfreq device to monitor and handle the exynos bus */
	bus->devfreq = devm_devfreq_add_device(dev, profile,
						DEVFREQ_GOV_SIMPLE_ONDEMAND,
						ondemand_data);
	if (IS_ERR(bus->devfreq)) {
		dev_err(dev, "failed to add devfreq device\n");
		return PTR_ERR(bus->devfreq);
	}

	/* Register opp_notifier to catch the change of OPP  */
	ret = devm_devfreq_register_opp_notifier(dev, bus->devfreq);
	if (ret < 0) {
		dev_err(dev, "failed to register opp notifier\n");
		return ret;
	}

	/*
	 * Enable devfreq-event to get raw data which is used to determine
	 * current bus load.
	 */
	ret = exynos_bus_enable_edev(bus);
	if (ret < 0) {
		dev_err(dev, "failed to enable devfreq-event devices\n");
		return ret;
	}

	ret = exynos_bus_set_event(bus);
	if (ret < 0) {
		dev_err(dev, "failed to set event to devfreq-event devices\n");
		goto err_edev;
	}

	return 0;

err_edev:
	if (exynos_bus_disable_edev(bus))
		dev_warn(dev, "failed to disable the devfreq-event devices\n");

	return ret;
}

static int exynos_bus_profile_init_passive(struct exynos_bus *bus,
					   struct devfreq_dev_profile *profile)
{
	struct device *dev = bus->dev;
	struct devfreq_passive_data *passive_data;
	struct devfreq *parent_devfreq;

	/* Initialize the struct profile and governor data for passive device */
	profile->target = exynos_bus_target;
	profile->exit = exynos_bus_passive_exit;

	/* Get the instance of parent devfreq device */
	parent_devfreq = devfreq_get_devfreq_by_phandle(dev, "devfreq", 0);
	if (IS_ERR(parent_devfreq))
		return -EPROBE_DEFER;

	passive_data = devm_kzalloc(dev, sizeof(*passive_data), GFP_KERNEL);
	if (!passive_data)
		return -ENOMEM;

	passive_data->parent = parent_devfreq;

	/* Add devfreq device for exynos bus with passive governor */
	bus->devfreq = devm_devfreq_add_device(dev, profile, DEVFREQ_GOV_PASSIVE,
						passive_data);
	if (IS_ERR(bus->devfreq)) {
		dev_err(dev,
			"failed to add devfreq dev with passive governor\n");
		return PTR_ERR(bus->devfreq);
	}

	return 0;
}

>>>>>>> upstream/android-13
static int exynos_bus_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node, *node;
	struct devfreq_dev_profile *profile;
<<<<<<< HEAD
	struct devfreq_simple_ondemand_data *ondemand_data;
	struct devfreq_passive_data *passive_data;
	struct devfreq *parent_devfreq;
=======
>>>>>>> upstream/android-13
	struct exynos_bus *bus;
	int ret, max_state;
	unsigned long min_freq, max_freq;
	bool passive = false;

	if (!np) {
		dev_err(dev, "failed to find devicetree node\n");
		return -EINVAL;
	}

	bus = devm_kzalloc(&pdev->dev, sizeof(*bus), GFP_KERNEL);
	if (!bus)
		return -ENOMEM;
	mutex_init(&bus->lock);
	bus->dev = &pdev->dev;
	platform_set_drvdata(pdev, bus);

	profile = devm_kzalloc(dev, sizeof(*profile), GFP_KERNEL);
	if (!profile)
		return -ENOMEM;

	node = of_parse_phandle(dev->of_node, "devfreq", 0);
	if (node) {
		of_node_put(node);
		passive = true;
	} else {
		ret = exynos_bus_parent_parse_of(np, bus);
		if (ret < 0)
			return ret;
	}

	/* Parse the device-tree to get the resource information */
	ret = exynos_bus_parse_of(np, bus);
	if (ret < 0)
		goto err_reg;

	if (passive)
<<<<<<< HEAD
		goto passive;

	/* Initialize the struct profile and governor data for parent device */
	profile->polling_ms = 50;
	profile->target = exynos_bus_target;
	profile->get_dev_status = exynos_bus_get_dev_status;
	profile->exit = exynos_bus_exit;

	ondemand_data = devm_kzalloc(dev, sizeof(*ondemand_data), GFP_KERNEL);
	if (!ondemand_data) {
		ret = -ENOMEM;
		goto err;
	}
	ondemand_data->upthreshold = 40;
	ondemand_data->downdifferential = 5;

	/* Add devfreq device to monitor and handle the exynos bus */
	bus->devfreq = devm_devfreq_add_device(dev, profile,
						DEVFREQ_GOV_SIMPLE_ONDEMAND,
						ondemand_data);
	if (IS_ERR(bus->devfreq)) {
		dev_err(dev, "failed to add devfreq device\n");
		ret = PTR_ERR(bus->devfreq);
		goto err;
	}

	/* Register opp_notifier to catch the change of OPP  */
	ret = devm_devfreq_register_opp_notifier(dev, bus->devfreq);
	if (ret < 0) {
		dev_err(dev, "failed to register opp notifier\n");
		goto err;
	}

	/*
	 * Enable devfreq-event to get raw data which is used to determine
	 * current bus load.
	 */
	ret = exynos_bus_enable_edev(bus);
	if (ret < 0) {
		dev_err(dev, "failed to enable devfreq-event devices\n");
		goto err;
	}

	ret = exynos_bus_set_event(bus);
	if (ret < 0) {
		dev_err(dev, "failed to set event to devfreq-event devices\n");
		goto err;
	}

	goto out;
passive:
	/* Initialize the struct profile and governor data for passive device */
	profile->target = exynos_bus_passive_target;
	profile->exit = exynos_bus_passive_exit;

	/* Get the instance of parent devfreq device */
	parent_devfreq = devfreq_get_devfreq_by_phandle(dev, 0);
	if (IS_ERR(parent_devfreq)) {
		ret = -EPROBE_DEFER;
		goto err;
	}

	passive_data = devm_kzalloc(dev, sizeof(*passive_data), GFP_KERNEL);
	if (!passive_data) {
		ret = -ENOMEM;
		goto err;
	}
	passive_data->parent = parent_devfreq;

	/* Add devfreq device for exynos bus with passive governor */
	bus->devfreq = devm_devfreq_add_device(dev, profile, DEVFREQ_GOV_PASSIVE,
						passive_data);
	if (IS_ERR(bus->devfreq)) {
		dev_err(dev,
			"failed to add devfreq dev with passive governor\n");
		ret = PTR_ERR(bus->devfreq);
		goto err;
	}

out:
=======
		ret = exynos_bus_profile_init_passive(bus, profile);
	else
		ret = exynos_bus_profile_init(bus, profile);

	if (ret < 0)
		goto err;

	/* Create child platform device for the interconnect provider */
	if (of_get_property(dev->of_node, "#interconnect-cells", NULL)) {
		bus->icc_pdev = platform_device_register_data(
						dev, "exynos-generic-icc",
						PLATFORM_DEVID_AUTO, NULL, 0);

		if (IS_ERR(bus->icc_pdev)) {
			ret = PTR_ERR(bus->icc_pdev);
			goto err;
		}
	}

>>>>>>> upstream/android-13
	max_state = bus->devfreq->profile->max_state;
	min_freq = (bus->devfreq->profile->freq_table[0] / 1000);
	max_freq = (bus->devfreq->profile->freq_table[max_state - 1] / 1000);
	pr_info("exynos-bus: new bus device registered: %s (%6ld KHz ~ %6ld KHz)\n",
			dev_name(dev), min_freq, max_freq);

	return 0;

err:
	dev_pm_opp_of_remove_table(dev);
	clk_disable_unprepare(bus->clk);
err_reg:
<<<<<<< HEAD
	if (!passive)
		regulator_disable(bus->regulator);
=======
	dev_pm_opp_put_regulators(bus->opp_table);
	bus->opp_table = NULL;
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
=======
static void exynos_bus_shutdown(struct platform_device *pdev)
{
	struct exynos_bus *bus = dev_get_drvdata(&pdev->dev);

	devfreq_suspend_device(bus->devfreq);
}

>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
static int exynos_bus_resume(struct device *dev)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);
	int ret;

	ret = exynos_bus_enable_edev(bus);
	if (ret < 0) {
		dev_err(dev, "failed to enable the devfreq-event devices\n");
		return ret;
	}

	return 0;
}

static int exynos_bus_suspend(struct device *dev)
{
	struct exynos_bus *bus = dev_get_drvdata(dev);
	int ret;

	ret = exynos_bus_disable_edev(bus);
	if (ret < 0) {
		dev_err(dev, "failed to disable the devfreq-event devices\n");
		return ret;
	}

	return 0;
}
#endif

static const struct dev_pm_ops exynos_bus_pm = {
	SET_SYSTEM_SLEEP_PM_OPS(exynos_bus_suspend, exynos_bus_resume)
};

static const struct of_device_id exynos_bus_of_match[] = {
	{ .compatible = "samsung,exynos-bus", },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, exynos_bus_of_match);

static struct platform_driver exynos_bus_platdrv = {
	.probe		= exynos_bus_probe,
<<<<<<< HEAD
=======
	.shutdown	= exynos_bus_shutdown,
>>>>>>> upstream/android-13
	.driver = {
		.name	= "exynos-bus",
		.pm	= &exynos_bus_pm,
		.of_match_table = of_match_ptr(exynos_bus_of_match),
	},
};
module_platform_driver(exynos_bus_platdrv);

MODULE_DESCRIPTION("Generic Exynos Bus frequency driver");
MODULE_AUTHOR("Chanwoo Choi <cw00.choi@samsung.com>");
MODULE_LICENSE("GPL v2");
