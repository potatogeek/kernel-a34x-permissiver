<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * db8500_thermal.c - DB8500 Thermal Management Implementation
 *
 * Copyright (C) 2012 ST-Ericsson
<<<<<<< HEAD
 * Copyright (C) 2012 Linaro Ltd.
 *
 * Author: Hongbo Zhang <hongbo.zhang@linaro.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
 * Copyright (C) 2012-2019 Linaro Ltd.
 *
 * Authors: Hongbo Zhang, Linus Walleij
>>>>>>> upstream/android-13
 */

#include <linux/cpu_cooling.h>
#include <linux/interrupt.h>
#include <linux/mfd/dbx500-prcmu.h>
#include <linux/module.h>
#include <linux/of.h>
<<<<<<< HEAD
#include <linux/platform_data/db8500_thermal.h>
=======
>>>>>>> upstream/android-13
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/thermal.h>

#define PRCMU_DEFAULT_MEASURE_TIME	0xFFF
#define PRCMU_DEFAULT_LOW_TEMP		0

<<<<<<< HEAD
struct db8500_thermal_zone {
	struct thermal_zone_device *therm_dev;
	struct mutex th_lock;
	struct work_struct therm_work;
	struct db8500_thsens_platform_data *trip_tab;
	enum thermal_device_mode mode;
	enum thermal_trend trend;
	unsigned long cur_temp_pseudo;
	unsigned int cur_index;
};

/* Local function to check if thermal zone matches cooling devices */
static int db8500_thermal_match_cdev(struct thermal_cooling_device *cdev,
		struct db8500_trip_point *trip_point)
{
	int i;

	if (!strlen(cdev->type))
		return -EINVAL;

	for (i = 0; i < COOLING_DEV_MAX; i++) {
		if (!strcmp(trip_point->cdev_name[i], cdev->type))
			return 0;
	}

	return -ENODEV;
}

/* Callback to bind cooling device to thermal zone */
static int db8500_cdev_bind(struct thermal_zone_device *thermal,
		struct thermal_cooling_device *cdev)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;
	unsigned long max_state, upper, lower;
	int i, ret = -EINVAL;

	cdev->ops->get_max_state(cdev, &max_state);

	for (i = 0; i < ptrips->num_trips; i++) {
		if (db8500_thermal_match_cdev(cdev, &ptrips->trip_points[i]))
			continue;

		upper = lower = i > max_state ? max_state : i;

		ret = thermal_zone_bind_cooling_device(thermal, i, cdev,
			upper, lower, THERMAL_WEIGHT_DEFAULT);

		dev_info(&cdev->device, "%s bind to %d: %d-%s\n", cdev->type,
			i, ret, ret ? "fail" : "succeed");
	}

	return ret;
}

/* Callback to unbind cooling device from thermal zone */
static int db8500_cdev_unbind(struct thermal_zone_device *thermal,
		struct thermal_cooling_device *cdev)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;
	int i, ret = -EINVAL;

	for (i = 0; i < ptrips->num_trips; i++) {
		if (db8500_thermal_match_cdev(cdev, &ptrips->trip_points[i]))
			continue;

		ret = thermal_zone_unbind_cooling_device(thermal, i, cdev);

		dev_info(&cdev->device, "%s unbind from %d: %s\n", cdev->type,
			i, ret ? "fail" : "succeed");
	}

	return ret;
}

/* Callback to get current temperature */
static int db8500_sys_get_temp(struct thermal_zone_device *thermal, int *temp)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;
=======
/**
 * db8500_thermal_points - the interpolation points that trigger
 * interrupts
 */
static const unsigned long db8500_thermal_points[] = {
	15000,
	20000,
	25000,
	30000,
	35000,
	40000,
	45000,
	50000,
	55000,
	60000,
	65000,
	70000,
	75000,
	80000,
	/*
	 * This is where things start to get really bad for the
	 * SoC and the thermal zones should be set up to trigger
	 * critical temperature at 85000 mC so we don't get above
	 * this point.
	 */
	85000,
	90000,
	95000,
	100000,
};

struct db8500_thermal_zone {
	struct thermal_zone_device *tz;
	enum thermal_trend trend;
	unsigned long interpolated_temp;
	unsigned int cur_index;
};

/* Callback to get current temperature */
static int db8500_thermal_get_temp(void *data, int *temp)
{
	struct db8500_thermal_zone *th = data;
>>>>>>> upstream/android-13

	/*
	 * TODO: There is no PRCMU interface to get temperature data currently,
	 * so a pseudo temperature is returned , it works for thermal framework
	 * and this will be fixed when the PRCMU interface is available.
	 */
<<<<<<< HEAD
	*temp = pzone->cur_temp_pseudo;
=======
	*temp = th->interpolated_temp;
>>>>>>> upstream/android-13

	return 0;
}

/* Callback to get temperature changing trend */
<<<<<<< HEAD
static int db8500_sys_get_trend(struct thermal_zone_device *thermal,
		int trip, enum thermal_trend *trend)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;

	*trend = pzone->trend;
=======
static int db8500_thermal_get_trend(void *data, int trip, enum thermal_trend *trend)
{
	struct db8500_thermal_zone *th = data;

	*trend = th->trend;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/* Callback to get thermal zone mode */
static int db8500_sys_get_mode(struct thermal_zone_device *thermal,
		enum thermal_device_mode *mode)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;

	mutex_lock(&pzone->th_lock);
	*mode = pzone->mode;
	mutex_unlock(&pzone->th_lock);

	return 0;
}

/* Callback to set thermal zone mode */
static int db8500_sys_set_mode(struct thermal_zone_device *thermal,
		enum thermal_device_mode mode)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;

	mutex_lock(&pzone->th_lock);

	pzone->mode = mode;
	if (mode == THERMAL_DEVICE_ENABLED)
		schedule_work(&pzone->therm_work);

	mutex_unlock(&pzone->th_lock);

	return 0;
}

/* Callback to get trip point type */
static int db8500_sys_get_trip_type(struct thermal_zone_device *thermal,
		int trip, enum thermal_trip_type *type)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;

	if (trip >= ptrips->num_trips)
		return -EINVAL;

	*type = ptrips->trip_points[trip].type;

	return 0;
}

/* Callback to get trip point temperature */
static int db8500_sys_get_trip_temp(struct thermal_zone_device *thermal,
		int trip, int *temp)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;

	if (trip >= ptrips->num_trips)
		return -EINVAL;

	*temp = ptrips->trip_points[trip].temp;

	return 0;
}

/* Callback to get critical trip point temperature */
static int db8500_sys_get_crit_temp(struct thermal_zone_device *thermal,
		int *temp)
{
	struct db8500_thermal_zone *pzone = thermal->devdata;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;
	int i;

	for (i = ptrips->num_trips - 1; i > 0; i--) {
		if (ptrips->trip_points[i].type == THERMAL_TRIP_CRITICAL) {
			*temp = ptrips->trip_points[i].temp;
			return 0;
		}
	}

	return -EINVAL;
}

static struct thermal_zone_device_ops thdev_ops = {
	.bind = db8500_cdev_bind,
	.unbind = db8500_cdev_unbind,
	.get_temp = db8500_sys_get_temp,
	.get_trend = db8500_sys_get_trend,
	.get_mode = db8500_sys_get_mode,
	.set_mode = db8500_sys_set_mode,
	.get_trip_type = db8500_sys_get_trip_type,
	.get_trip_temp = db8500_sys_get_trip_temp,
	.get_crit_temp = db8500_sys_get_crit_temp,
};

static void db8500_thermal_update_config(struct db8500_thermal_zone *pzone,
		unsigned int idx, enum thermal_trend trend,
		unsigned long next_low, unsigned long next_high)
{
	prcmu_stop_temp_sense();

	pzone->cur_index = idx;
	pzone->cur_temp_pseudo = (next_low + next_high)/2;
	pzone->trend = trend;

=======
static struct thermal_zone_of_device_ops thdev_ops = {
	.get_temp = db8500_thermal_get_temp,
	.get_trend = db8500_thermal_get_trend,
};

static void db8500_thermal_update_config(struct db8500_thermal_zone *th,
					 unsigned int idx,
					 enum thermal_trend trend,
					 unsigned long next_low,
					 unsigned long next_high)
{
	prcmu_stop_temp_sense();

	th->cur_index = idx;
	th->interpolated_temp = (next_low + next_high)/2;
	th->trend = trend;

	/*
	 * The PRCMU accept absolute temperatures in celsius so divide
	 * down the millicelsius with 1000
	 */
>>>>>>> upstream/android-13
	prcmu_config_hotmon((u8)(next_low/1000), (u8)(next_high/1000));
	prcmu_start_temp_sense(PRCMU_DEFAULT_MEASURE_TIME);
}

static irqreturn_t prcmu_low_irq_handler(int irq, void *irq_data)
{
<<<<<<< HEAD
	struct db8500_thermal_zone *pzone = irq_data;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;
	unsigned int idx = pzone->cur_index;
	unsigned long next_low, next_high;

	if (unlikely(idx == 0))
=======
	struct db8500_thermal_zone *th = irq_data;
	unsigned int idx = th->cur_index;
	unsigned long next_low, next_high;

	if (idx == 0)
>>>>>>> upstream/android-13
		/* Meaningless for thermal management, ignoring it */
		return IRQ_HANDLED;

	if (idx == 1) {
<<<<<<< HEAD
		next_high = ptrips->trip_points[0].temp;
		next_low = PRCMU_DEFAULT_LOW_TEMP;
	} else {
		next_high = ptrips->trip_points[idx-1].temp;
		next_low = ptrips->trip_points[idx-2].temp;
	}
	idx -= 1;

	db8500_thermal_update_config(pzone, idx, THERMAL_TREND_DROPPING,
		next_low, next_high);

	dev_dbg(&pzone->therm_dev->device,
		"PRCMU set max %ld, min %ld\n", next_high, next_low);

	schedule_work(&pzone->therm_work);
=======
		next_high = db8500_thermal_points[0];
		next_low = PRCMU_DEFAULT_LOW_TEMP;
	} else {
		next_high = db8500_thermal_points[idx - 1];
		next_low = db8500_thermal_points[idx - 2];
	}
	idx -= 1;

	db8500_thermal_update_config(th, idx, THERMAL_TREND_DROPPING,
				     next_low, next_high);
	dev_dbg(&th->tz->device,
		"PRCMU set max %ld, min %ld\n", next_high, next_low);

	thermal_zone_device_update(th->tz, THERMAL_EVENT_UNSPECIFIED);
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

static irqreturn_t prcmu_high_irq_handler(int irq, void *irq_data)
{
<<<<<<< HEAD
	struct db8500_thermal_zone *pzone = irq_data;
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;
	unsigned int idx = pzone->cur_index;
	unsigned long next_low, next_high;

	if (idx < ptrips->num_trips - 1) {
		next_high = ptrips->trip_points[idx+1].temp;
		next_low = ptrips->trip_points[idx].temp;
		idx += 1;

		db8500_thermal_update_config(pzone, idx, THERMAL_TREND_RAISING,
			next_low, next_high);

		dev_dbg(&pzone->therm_dev->device,
		"PRCMU set max %ld, min %ld\n", next_high, next_low);
	} else if (idx == ptrips->num_trips - 1)
		pzone->cur_temp_pseudo = ptrips->trip_points[idx].temp + 1;

	schedule_work(&pzone->therm_work);
=======
	struct db8500_thermal_zone *th = irq_data;
	unsigned int idx = th->cur_index;
	unsigned long next_low, next_high;
	int num_points = ARRAY_SIZE(db8500_thermal_points);

	if (idx < num_points - 1) {
		next_high = db8500_thermal_points[idx+1];
		next_low = db8500_thermal_points[idx];
		idx += 1;

		db8500_thermal_update_config(th, idx, THERMAL_TREND_RAISING,
					     next_low, next_high);

		dev_dbg(&th->tz->device,
			"PRCMU set max %ld, min %ld\n", next_high, next_low);
	} else if (idx == num_points - 1)
		/* So we roof out 1 degree over the max point */
		th->interpolated_temp = db8500_thermal_points[idx] + 1;

	thermal_zone_device_update(th->tz, THERMAL_EVENT_UNSPECIFIED);
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void db8500_thermal_work(struct work_struct *work)
{
	enum thermal_device_mode cur_mode;
	struct db8500_thermal_zone *pzone;

	pzone = container_of(work, struct db8500_thermal_zone, therm_work);

	mutex_lock(&pzone->th_lock);
	cur_mode = pzone->mode;
	mutex_unlock(&pzone->th_lock);

	if (cur_mode == THERMAL_DEVICE_DISABLED)
		return;

	thermal_zone_device_update(pzone->therm_dev, THERMAL_EVENT_UNSPECIFIED);
	dev_dbg(&pzone->therm_dev->device, "thermal work finished.\n");
}

#ifdef CONFIG_OF
static struct db8500_thsens_platform_data*
		db8500_thermal_parse_dt(struct platform_device *pdev)
{
	struct db8500_thsens_platform_data *ptrips;
	struct device_node *np = pdev->dev.of_node;
	char prop_name[32];
	const char *tmp_str;
	u32 tmp_data;
	int i, j;

	ptrips = devm_kzalloc(&pdev->dev, sizeof(*ptrips), GFP_KERNEL);
	if (!ptrips)
		return NULL;

	if (of_property_read_u32(np, "num-trips", &tmp_data))
		goto err_parse_dt;

	if (tmp_data > THERMAL_MAX_TRIPS)
		goto err_parse_dt;

	ptrips->num_trips = tmp_data;

	for (i = 0; i < ptrips->num_trips; i++) {
		sprintf(prop_name, "trip%d-temp", i);
		if (of_property_read_u32(np, prop_name, &tmp_data))
			goto err_parse_dt;

		ptrips->trip_points[i].temp = tmp_data;

		sprintf(prop_name, "trip%d-type", i);
		if (of_property_read_string(np, prop_name, &tmp_str))
			goto err_parse_dt;

		if (!strcmp(tmp_str, "active"))
			ptrips->trip_points[i].type = THERMAL_TRIP_ACTIVE;
		else if (!strcmp(tmp_str, "passive"))
			ptrips->trip_points[i].type = THERMAL_TRIP_PASSIVE;
		else if (!strcmp(tmp_str, "hot"))
			ptrips->trip_points[i].type = THERMAL_TRIP_HOT;
		else if (!strcmp(tmp_str, "critical"))
			ptrips->trip_points[i].type = THERMAL_TRIP_CRITICAL;
		else
			goto err_parse_dt;

		sprintf(prop_name, "trip%d-cdev-num", i);
		if (of_property_read_u32(np, prop_name, &tmp_data))
			goto err_parse_dt;

		if (tmp_data > COOLING_DEV_MAX)
			goto err_parse_dt;

		for (j = 0; j < tmp_data; j++) {
			sprintf(prop_name, "trip%d-cdev-name%d", i, j);
			if (of_property_read_string(np, prop_name, &tmp_str))
				goto err_parse_dt;

			if (strlen(tmp_str) >= THERMAL_NAME_LENGTH)
				goto err_parse_dt;

			strcpy(ptrips->trip_points[i].cdev_name[j], tmp_str);
		}
	}
	return ptrips;

err_parse_dt:
	dev_err(&pdev->dev, "Parsing device tree data error.\n");
	return NULL;
}
#else
static inline struct db8500_thsens_platform_data*
		db8500_thermal_parse_dt(struct platform_device *pdev)
{
	return NULL;
}
#endif

static int db8500_thermal_probe(struct platform_device *pdev)
{
	struct db8500_thermal_zone *pzone = NULL;
	struct db8500_thsens_platform_data *ptrips = NULL;
	struct device_node *np = pdev->dev.of_node;
	int low_irq, high_irq, ret = 0;
	unsigned long dft_low, dft_high;

	if (np)
		ptrips = db8500_thermal_parse_dt(pdev);
	else
		ptrips = dev_get_platdata(&pdev->dev);

	if (!ptrips)
		return -EINVAL;

	pzone = devm_kzalloc(&pdev->dev, sizeof(*pzone), GFP_KERNEL);
	if (!pzone)
		return -ENOMEM;

	mutex_init(&pzone->th_lock);
	mutex_lock(&pzone->th_lock);

	pzone->mode = THERMAL_DEVICE_DISABLED;
	pzone->trip_tab = ptrips;

	INIT_WORK(&pzone->therm_work, db8500_thermal_work);

	low_irq = platform_get_irq_byname(pdev, "IRQ_HOTMON_LOW");
	if (low_irq < 0) {
		dev_err(&pdev->dev, "Get IRQ_HOTMON_LOW failed.\n");
		ret = low_irq;
		goto out_unlock;
	}

	ret = devm_request_threaded_irq(&pdev->dev, low_irq, NULL,
		prcmu_low_irq_handler, IRQF_NO_SUSPEND | IRQF_ONESHOT,
		"dbx500_temp_low", pzone);
	if (ret < 0) {
		dev_err(&pdev->dev, "Failed to allocate temp low irq.\n");
		goto out_unlock;
=======
static int db8500_thermal_probe(struct platform_device *pdev)
{
	struct db8500_thermal_zone *th = NULL;
	struct device *dev = &pdev->dev;
	int low_irq, high_irq, ret = 0;

	th = devm_kzalloc(dev, sizeof(*th), GFP_KERNEL);
	if (!th)
		return -ENOMEM;

	low_irq = platform_get_irq_byname(pdev, "IRQ_HOTMON_LOW");
	if (low_irq < 0) {
		dev_err(dev, "Get IRQ_HOTMON_LOW failed\n");
		return low_irq;
	}

	ret = devm_request_threaded_irq(dev, low_irq, NULL,
		prcmu_low_irq_handler, IRQF_NO_SUSPEND | IRQF_ONESHOT,
		"dbx500_temp_low", th);
	if (ret < 0) {
		dev_err(dev, "failed to allocate temp low irq\n");
		return ret;
>>>>>>> upstream/android-13
	}

	high_irq = platform_get_irq_byname(pdev, "IRQ_HOTMON_HIGH");
	if (high_irq < 0) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "Get IRQ_HOTMON_HIGH failed.\n");
		ret = high_irq;
		goto out_unlock;
	}

	ret = devm_request_threaded_irq(&pdev->dev, high_irq, NULL,
		prcmu_high_irq_handler, IRQF_NO_SUSPEND | IRQF_ONESHOT,
		"dbx500_temp_high", pzone);
	if (ret < 0) {
		dev_err(&pdev->dev, "Failed to allocate temp high irq.\n");
		goto out_unlock;
	}

	pzone->therm_dev = thermal_zone_device_register("db8500_thermal_zone",
		ptrips->num_trips, 0, pzone, &thdev_ops, NULL, 0, 0);

	if (IS_ERR(pzone->therm_dev)) {
		dev_err(&pdev->dev, "Register thermal zone device failed.\n");
		ret = PTR_ERR(pzone->therm_dev);
		goto out_unlock;
	}
	dev_info(&pdev->dev, "Thermal zone device registered.\n");

	dft_low = PRCMU_DEFAULT_LOW_TEMP;
	dft_high = ptrips->trip_points[0].temp;

	db8500_thermal_update_config(pzone, 0, THERMAL_TREND_STABLE,
		dft_low, dft_high);

	platform_set_drvdata(pdev, pzone);
	pzone->mode = THERMAL_DEVICE_ENABLED;

out_unlock:
	mutex_unlock(&pzone->th_lock);

	return ret;
}

static int db8500_thermal_remove(struct platform_device *pdev)
{
	struct db8500_thermal_zone *pzone = platform_get_drvdata(pdev);

	thermal_zone_device_unregister(pzone->therm_dev);
	cancel_work_sync(&pzone->therm_work);
	mutex_destroy(&pzone->th_lock);
=======
		dev_err(dev, "Get IRQ_HOTMON_HIGH failed\n");
		return high_irq;
	}

	ret = devm_request_threaded_irq(dev, high_irq, NULL,
		prcmu_high_irq_handler, IRQF_NO_SUSPEND | IRQF_ONESHOT,
		"dbx500_temp_high", th);
	if (ret < 0) {
		dev_err(dev, "failed to allocate temp high irq\n");
		return ret;
	}

	/* register of thermal sensor and get info from DT */
	th->tz = devm_thermal_zone_of_sensor_register(dev, 0, th, &thdev_ops);
	if (IS_ERR(th->tz)) {
		dev_err(dev, "register thermal zone sensor failed\n");
		return PTR_ERR(th->tz);
	}
	dev_info(dev, "thermal zone sensor registered\n");

	/* Start measuring at the lowest point */
	db8500_thermal_update_config(th, 0, THERMAL_TREND_STABLE,
				     PRCMU_DEFAULT_LOW_TEMP,
				     db8500_thermal_points[0]);

	platform_set_drvdata(pdev, th);
>>>>>>> upstream/android-13

	return 0;
}

static int db8500_thermal_suspend(struct platform_device *pdev,
		pm_message_t state)
{
<<<<<<< HEAD
	struct db8500_thermal_zone *pzone = platform_get_drvdata(pdev);

	flush_work(&pzone->therm_work);
=======
>>>>>>> upstream/android-13
	prcmu_stop_temp_sense();

	return 0;
}

static int db8500_thermal_resume(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct db8500_thermal_zone *pzone = platform_get_drvdata(pdev);
	struct db8500_thsens_platform_data *ptrips = pzone->trip_tab;
	unsigned long dft_low, dft_high;

	dft_low = PRCMU_DEFAULT_LOW_TEMP;
	dft_high = ptrips->trip_points[0].temp;

	db8500_thermal_update_config(pzone, 0, THERMAL_TREND_STABLE,
		dft_low, dft_high);
=======
	struct db8500_thermal_zone *th = platform_get_drvdata(pdev);

	/* Resume and start measuring at the lowest point */
	db8500_thermal_update_config(th, 0, THERMAL_TREND_STABLE,
				     PRCMU_DEFAULT_LOW_TEMP,
				     db8500_thermal_points[0]);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
static const struct of_device_id db8500_thermal_match[] = {
	{ .compatible = "stericsson,db8500-thermal" },
	{},
};
MODULE_DEVICE_TABLE(of, db8500_thermal_match);
<<<<<<< HEAD
#endif
=======
>>>>>>> upstream/android-13

static struct platform_driver db8500_thermal_driver = {
	.driver = {
		.name = "db8500-thermal",
		.of_match_table = of_match_ptr(db8500_thermal_match),
	},
	.probe = db8500_thermal_probe,
	.suspend = db8500_thermal_suspend,
	.resume = db8500_thermal_resume,
<<<<<<< HEAD
	.remove = db8500_thermal_remove,
=======
>>>>>>> upstream/android-13
};

module_platform_driver(db8500_thermal_driver);

MODULE_AUTHOR("Hongbo Zhang <hongbo.zhang@stericsson.com>");
MODULE_DESCRIPTION("DB8500 thermal driver");
MODULE_LICENSE("GPL");
