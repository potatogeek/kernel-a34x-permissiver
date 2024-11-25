<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *      intel-mid_wdt: generic Intel MID SCU watchdog driver
 *
 *      Platforms supported so far:
 *      - Merrifield only
 *
 *      Copyright (C) 2014 Intel Corporation. All rights reserved.
 *      Contact: David Cohen <david.a.cohen@linux.intel.com>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of version 2 of the GNU General
 *      Public License as published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/nmi.h>
#include <linux/platform_device.h>
#include <linux/watchdog.h>
#include <linux/platform_data/intel-mid_wdt.h>

#include <asm/intel_scu_ipc.h>
#include <asm/intel-mid.h>

#define IPC_WATCHDOG 0xf8

#define MID_WDT_PRETIMEOUT		15
#define MID_WDT_TIMEOUT_MIN		(1 + MID_WDT_PRETIMEOUT)
#define MID_WDT_TIMEOUT_MAX		170
#define MID_WDT_DEFAULT_TIMEOUT		90

/* SCU watchdog messages */
enum {
	SCU_WATCHDOG_START = 0,
	SCU_WATCHDOG_STOP,
	SCU_WATCHDOG_KEEPALIVE,
};

<<<<<<< HEAD
static inline int wdt_command(int sub, u32 *in, int inlen)
{
	return intel_scu_ipc_command(IPC_WATCHDOG, sub, in, inlen, NULL, 0);
=======
struct mid_wdt {
	struct watchdog_device wd;
	struct device *dev;
	struct intel_scu_ipc_dev *scu;
};

static inline int
wdt_command(struct mid_wdt *mid, int sub, const void *in, size_t inlen, size_t size)
{
	struct intel_scu_ipc_dev *scu = mid->scu;

	return intel_scu_ipc_dev_command_with_size(scu, IPC_WATCHDOG, sub, in,
						   inlen, size, NULL, 0);
>>>>>>> upstream/android-13
}

static int wdt_start(struct watchdog_device *wd)
{
<<<<<<< HEAD
	struct device *dev = watchdog_get_drvdata(wd);
=======
	struct mid_wdt *mid = watchdog_get_drvdata(wd);
>>>>>>> upstream/android-13
	int ret, in_size;
	int timeout = wd->timeout;
	struct ipc_wd_start {
		u32 pretimeout;
		u32 timeout;
	} ipc_wd_start = { timeout - MID_WDT_PRETIMEOUT, timeout };

	/*
<<<<<<< HEAD
	 * SCU expects the input size for watchdog IPC to
	 * be based on 4 bytes
	 */
	in_size = DIV_ROUND_UP(sizeof(ipc_wd_start), 4);

	ret = wdt_command(SCU_WATCHDOG_START, (u32 *)&ipc_wd_start, in_size);
	if (ret)
		dev_crit(dev, "error starting watchdog: %d\n", ret);
=======
	 * SCU expects the input size for watchdog IPC to be 2 which is the
	 * size of the structure in dwords. SCU IPC normally takes bytes
	 * but this is a special case where we specify size to be different
	 * than inlen.
	 */
	in_size = DIV_ROUND_UP(sizeof(ipc_wd_start), 4);

	ret = wdt_command(mid, SCU_WATCHDOG_START, &ipc_wd_start,
			  sizeof(ipc_wd_start), in_size);
	if (ret)
		dev_crit(mid->dev, "error starting watchdog: %d\n", ret);
>>>>>>> upstream/android-13

	return ret;
}

static int wdt_ping(struct watchdog_device *wd)
{
<<<<<<< HEAD
	struct device *dev = watchdog_get_drvdata(wd);
	int ret;

	ret = wdt_command(SCU_WATCHDOG_KEEPALIVE, NULL, 0);
	if (ret)
		dev_crit(dev, "Error executing keepalive: %d\n", ret);
=======
	struct mid_wdt *mid = watchdog_get_drvdata(wd);
	int ret;

	ret = wdt_command(mid, SCU_WATCHDOG_KEEPALIVE, NULL, 0, 0);
	if (ret)
		dev_crit(mid->dev, "Error executing keepalive: %d\n", ret);
>>>>>>> upstream/android-13

	return ret;
}

static int wdt_stop(struct watchdog_device *wd)
{
<<<<<<< HEAD
	struct device *dev = watchdog_get_drvdata(wd);
	int ret;

	ret = wdt_command(SCU_WATCHDOG_STOP, NULL, 0);
	if (ret)
		dev_crit(dev, "Error stopping watchdog: %d\n", ret);
=======
	struct mid_wdt *mid = watchdog_get_drvdata(wd);
	int ret;

	ret = wdt_command(mid, SCU_WATCHDOG_STOP, NULL, 0, 0);
	if (ret)
		dev_crit(mid->dev, "Error stopping watchdog: %d\n", ret);
>>>>>>> upstream/android-13

	return ret;
}

static irqreturn_t mid_wdt_irq(int irq, void *dev_id)
{
	panic("Kernel Watchdog");

	/* This code should not be reached */
	return IRQ_HANDLED;
}

static const struct watchdog_info mid_wdt_info = {
	.identity = "Intel MID SCU watchdog",
	.options = WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE,
};

static const struct watchdog_ops mid_wdt_ops = {
	.owner = THIS_MODULE,
	.start = wdt_start,
	.stop = wdt_stop,
	.ping = wdt_ping,
};

static int mid_wdt_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct watchdog_device *wdt_dev;
	struct intel_mid_wdt_pdata *pdata = pdev->dev.platform_data;
	int ret;

	if (!pdata) {
		dev_err(&pdev->dev, "missing platform data\n");
=======
	struct device *dev = &pdev->dev;
	struct watchdog_device *wdt_dev;
	struct intel_mid_wdt_pdata *pdata = dev->platform_data;
	struct mid_wdt *mid;
	int ret;

	if (!pdata) {
		dev_err(dev, "missing platform data\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	if (pdata->probe) {
		ret = pdata->probe(pdev);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	wdt_dev = devm_kzalloc(&pdev->dev, sizeof(*wdt_dev), GFP_KERNEL);
	if (!wdt_dev)
		return -ENOMEM;

=======
	mid = devm_kzalloc(dev, sizeof(*mid), GFP_KERNEL);
	if (!mid)
		return -ENOMEM;

	mid->dev = dev;
	wdt_dev = &mid->wd;

>>>>>>> upstream/android-13
	wdt_dev->info = &mid_wdt_info;
	wdt_dev->ops = &mid_wdt_ops;
	wdt_dev->min_timeout = MID_WDT_TIMEOUT_MIN;
	wdt_dev->max_timeout = MID_WDT_TIMEOUT_MAX;
	wdt_dev->timeout = MID_WDT_DEFAULT_TIMEOUT;
<<<<<<< HEAD
	wdt_dev->parent = &pdev->dev;

	watchdog_set_drvdata(wdt_dev, &pdev->dev);

	ret = devm_request_irq(&pdev->dev, pdata->irq, mid_wdt_irq,
			       IRQF_SHARED | IRQF_NO_SUSPEND, "watchdog",
			       wdt_dev);
	if (ret) {
		dev_err(&pdev->dev, "error requesting warning irq %d\n",
			pdata->irq);
=======
	wdt_dev->parent = dev;

	watchdog_set_nowayout(wdt_dev, WATCHDOG_NOWAYOUT);
	watchdog_set_drvdata(wdt_dev, mid);

	mid->scu = devm_intel_scu_ipc_dev_get(dev);
	if (!mid->scu)
		return -EPROBE_DEFER;

	ret = devm_request_irq(dev, pdata->irq, mid_wdt_irq,
			       IRQF_SHARED | IRQF_NO_SUSPEND, "watchdog",
			       wdt_dev);
	if (ret) {
		dev_err(dev, "error requesting warning irq %d\n", pdata->irq);
>>>>>>> upstream/android-13
		return ret;
	}

	/*
	 * The firmware followed by U-Boot leaves the watchdog running
	 * with the default threshold which may vary. When we get here
	 * we should make a decision to prevent any side effects before
	 * user space daemon will take care of it. The best option,
	 * taking into consideration that there is no way to read values
	 * back from hardware, is to enforce watchdog being run with
	 * deterministic values.
	 */
	ret = wdt_start(wdt_dev);
	if (ret)
		return ret;

	/* Make sure the watchdog is serviced */
	set_bit(WDOG_HW_RUNNING, &wdt_dev->status);

<<<<<<< HEAD
	ret = devm_watchdog_register_device(&pdev->dev, wdt_dev);
	if (ret) {
		dev_err(&pdev->dev, "error registering watchdog device\n");
		return ret;
	}

	dev_info(&pdev->dev, "Intel MID watchdog device probed\n");
=======
	ret = devm_watchdog_register_device(dev, wdt_dev);
	if (ret)
		return ret;

	dev_info(dev, "Intel MID watchdog device probed\n");
>>>>>>> upstream/android-13

	return 0;
}

static struct platform_driver mid_wdt_driver = {
	.probe		= mid_wdt_probe,
	.driver		= {
		.name	= "intel_mid_wdt",
	},
};

module_platform_driver(mid_wdt_driver);

MODULE_AUTHOR("David Cohen <david.a.cohen@linux.intel.com>");
MODULE_DESCRIPTION("Watchdog Driver for Intel MID platform");
MODULE_LICENSE("GPL");
