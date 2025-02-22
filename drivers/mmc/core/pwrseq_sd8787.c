<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * pwrseq_sd8787.c - power sequence support for Marvell SD8787 BT + Wifi chip
 *
 * Copyright (C) 2016 Matt Ranostay <matt@ranostay.consulting>
 *
 * Based on the original work pwrseq_simple.c
 *  Copyright (C) 2014 Linaro Ltd
 *  Author: Ulf Hansson <ulf.hansson@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>

#include <linux/mmc/host.h>

#include "pwrseq.h"

struct mmc_pwrseq_sd8787 {
	struct mmc_pwrseq pwrseq;
	struct gpio_desc *reset_gpio;
	struct gpio_desc *pwrdn_gpio;
<<<<<<< HEAD
=======
	u32 reset_pwrdwn_delay_ms;
>>>>>>> upstream/android-13
};

#define to_pwrseq_sd8787(p) container_of(p, struct mmc_pwrseq_sd8787, pwrseq)

static void mmc_pwrseq_sd8787_pre_power_on(struct mmc_host *host)
{
	struct mmc_pwrseq_sd8787 *pwrseq = to_pwrseq_sd8787(host->pwrseq);

	gpiod_set_value_cansleep(pwrseq->reset_gpio, 1);

<<<<<<< HEAD
	msleep(300);
=======
	msleep(pwrseq->reset_pwrdwn_delay_ms);
>>>>>>> upstream/android-13
	gpiod_set_value_cansleep(pwrseq->pwrdn_gpio, 1);
}

static void mmc_pwrseq_sd8787_power_off(struct mmc_host *host)
{
	struct mmc_pwrseq_sd8787 *pwrseq = to_pwrseq_sd8787(host->pwrseq);

	gpiod_set_value_cansleep(pwrseq->pwrdn_gpio, 0);
	gpiod_set_value_cansleep(pwrseq->reset_gpio, 0);
}

static const struct mmc_pwrseq_ops mmc_pwrseq_sd8787_ops = {
	.pre_power_on = mmc_pwrseq_sd8787_pre_power_on,
	.power_off = mmc_pwrseq_sd8787_power_off,
};

<<<<<<< HEAD
static const struct of_device_id mmc_pwrseq_sd8787_of_match[] = {
	{ .compatible = "mmc-pwrseq-sd8787",},
=======
static const u32 sd8787_delay_ms = 300;
static const u32 wilc1000_delay_ms = 5;

static const struct of_device_id mmc_pwrseq_sd8787_of_match[] = {
	{ .compatible = "mmc-pwrseq-sd8787", .data = &sd8787_delay_ms },
	{ .compatible = "mmc-pwrseq-wilc1000", .data = &wilc1000_delay_ms },
>>>>>>> upstream/android-13
	{/* sentinel */},
};
MODULE_DEVICE_TABLE(of, mmc_pwrseq_sd8787_of_match);

static int mmc_pwrseq_sd8787_probe(struct platform_device *pdev)
{
	struct mmc_pwrseq_sd8787 *pwrseq;
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
=======
	const struct of_device_id *match;
>>>>>>> upstream/android-13

	pwrseq = devm_kzalloc(dev, sizeof(*pwrseq), GFP_KERNEL);
	if (!pwrseq)
		return -ENOMEM;

<<<<<<< HEAD
=======
	match = of_match_node(mmc_pwrseq_sd8787_of_match, pdev->dev.of_node);
	pwrseq->reset_pwrdwn_delay_ms = *(u32 *)match->data;

>>>>>>> upstream/android-13
	pwrseq->pwrdn_gpio = devm_gpiod_get(dev, "powerdown", GPIOD_OUT_LOW);
	if (IS_ERR(pwrseq->pwrdn_gpio))
		return PTR_ERR(pwrseq->pwrdn_gpio);

	pwrseq->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(pwrseq->reset_gpio))
		return PTR_ERR(pwrseq->reset_gpio);

	pwrseq->pwrseq.dev = dev;
	pwrseq->pwrseq.ops = &mmc_pwrseq_sd8787_ops;
	pwrseq->pwrseq.owner = THIS_MODULE;
	platform_set_drvdata(pdev, pwrseq);

	return mmc_pwrseq_register(&pwrseq->pwrseq);
}

static int mmc_pwrseq_sd8787_remove(struct platform_device *pdev)
{
	struct mmc_pwrseq_sd8787 *pwrseq = platform_get_drvdata(pdev);

	mmc_pwrseq_unregister(&pwrseq->pwrseq);

	return 0;
}

static struct platform_driver mmc_pwrseq_sd8787_driver = {
	.probe = mmc_pwrseq_sd8787_probe,
	.remove = mmc_pwrseq_sd8787_remove,
	.driver = {
		.name = "pwrseq_sd8787",
		.of_match_table = mmc_pwrseq_sd8787_of_match,
	},
};

module_platform_driver(mmc_pwrseq_sd8787_driver);
MODULE_LICENSE("GPL v2");
