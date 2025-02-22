<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * mpc8xxx_wdt.c - MPC8xx/MPC83xx/MPC86xx watchdog userspace interface
 *
 * Authors: Dave Updegraff <dave@cray.org>
 *	    Kumar Gala <galak@kernel.crashing.org>
 *		Attribution: from 83xx_wst: Florian Schirmer <jolt@tuxbox.org>
 *				..and from sc520_wdt
 * Copyright (c) 2008  MontaVista Software, Inc.
 *                     Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * Note: it appears that you can only actually ENABLE or DISABLE the thing
 * once after POR. Once enabled, you cannot disable, and vice versa.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

=======
 */

>>>>>>> upstream/android-13
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/module.h>
#include <linux/watchdog.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <sysdev/fsl_soc.h>

#define WATCHDOG_TIMEOUT 10

struct mpc8xxx_wdt {
	__be32 res0;
	__be32 swcrr; /* System watchdog control register */
#define SWCRR_SWTC 0xFFFF0000 /* Software Watchdog Time Count. */
#define SWCRR_SWF  0x00000008 /* Software Watchdog Freeze (mpc8xx). */
#define SWCRR_SWEN 0x00000004 /* Watchdog Enable bit. */
#define SWCRR_SWRI 0x00000002 /* Software Watchdog Reset/Interrupt Select bit.*/
#define SWCRR_SWPR 0x00000001 /* Software Watchdog Counter Prescale bit. */
	__be32 swcnr; /* System watchdog count register */
	u8 res1[2];
	__be16 swsrr; /* System watchdog service register */
	u8 res2[0xF0];
};

struct mpc8xxx_wdt_type {
	int prescaler;
	bool hw_enabled;
<<<<<<< HEAD
=======
	u32 rsr_mask;
>>>>>>> upstream/android-13
};

struct mpc8xxx_wdt_ddata {
	struct mpc8xxx_wdt __iomem *base;
	struct watchdog_device wdd;
	spinlock_t lock;
	u16 swtc;
};

static u16 timeout;
module_param(timeout, ushort, 0);
MODULE_PARM_DESC(timeout,
	"Watchdog timeout in seconds. (1<timeout<65535, default="
	__MODULE_STRING(WATCHDOG_TIMEOUT) ")");

static bool reset = 1;
module_param(reset, bool, 0);
MODULE_PARM_DESC(reset,
	"Watchdog Interrupt/Reset Mode. 0 = interrupt, 1 = reset");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
		 "(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static void mpc8xxx_wdt_keepalive(struct mpc8xxx_wdt_ddata *ddata)
{
	/* Ping the WDT */
	spin_lock(&ddata->lock);
	out_be16(&ddata->base->swsrr, 0x556c);
	out_be16(&ddata->base->swsrr, 0xaa39);
	spin_unlock(&ddata->lock);
}

static int mpc8xxx_wdt_start(struct watchdog_device *w)
{
	struct mpc8xxx_wdt_ddata *ddata =
		container_of(w, struct mpc8xxx_wdt_ddata, wdd);
	u32 tmp = in_be32(&ddata->base->swcrr);

	/* Good, fire up the show */
	tmp &= ~(SWCRR_SWTC | SWCRR_SWF | SWCRR_SWEN | SWCRR_SWRI | SWCRR_SWPR);
	tmp |= SWCRR_SWEN | SWCRR_SWPR | (ddata->swtc << 16);

	if (reset)
		tmp |= SWCRR_SWRI;

	out_be32(&ddata->base->swcrr, tmp);

	tmp = in_be32(&ddata->base->swcrr);
	if (!(tmp & SWCRR_SWEN))
		return -EOPNOTSUPP;

	ddata->swtc = tmp >> 16;
	set_bit(WDOG_HW_RUNNING, &ddata->wdd.status);

	return 0;
}

static int mpc8xxx_wdt_ping(struct watchdog_device *w)
{
	struct mpc8xxx_wdt_ddata *ddata =
		container_of(w, struct mpc8xxx_wdt_ddata, wdd);

	mpc8xxx_wdt_keepalive(ddata);
	return 0;
}

static struct watchdog_info mpc8xxx_wdt_info = {
	.options = WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT,
	.firmware_version = 1,
	.identity = "MPC8xxx",
};

<<<<<<< HEAD
static struct watchdog_ops mpc8xxx_wdt_ops = {
=======
static const struct watchdog_ops mpc8xxx_wdt_ops = {
>>>>>>> upstream/android-13
	.owner = THIS_MODULE,
	.start = mpc8xxx_wdt_start,
	.ping = mpc8xxx_wdt_ping,
};

static int mpc8xxx_wdt_probe(struct platform_device *ofdev)
{
	int ret;
	struct resource *res;
	const struct mpc8xxx_wdt_type *wdt_type;
	struct mpc8xxx_wdt_ddata *ddata;
	u32 freq = fsl_get_sys_freq();
	bool enabled;
<<<<<<< HEAD

	wdt_type = of_device_get_match_data(&ofdev->dev);
=======
	struct device *dev = &ofdev->dev;

	wdt_type = of_device_get_match_data(dev);
>>>>>>> upstream/android-13
	if (!wdt_type)
		return -EINVAL;

	if (!freq || freq == -1)
		return -EINVAL;

<<<<<<< HEAD
	ddata = devm_kzalloc(&ofdev->dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	res = platform_get_resource(ofdev, IORESOURCE_MEM, 0);
	ddata->base = devm_ioremap_resource(&ofdev->dev, res);
=======
	ddata = devm_kzalloc(dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->base = devm_platform_ioremap_resource(ofdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(ddata->base))
		return PTR_ERR(ddata->base);

	enabled = in_be32(&ddata->base->swcrr) & SWCRR_SWEN;
	if (!enabled && wdt_type->hw_enabled) {
<<<<<<< HEAD
		pr_info("could not be enabled in software\n");
		return -ENODEV;
	}

	spin_lock_init(&ddata->lock);

	ddata->wdd.info = &mpc8xxx_wdt_info,
	ddata->wdd.ops = &mpc8xxx_wdt_ops,

	ddata->wdd.timeout = WATCHDOG_TIMEOUT;
	watchdog_init_timeout(&ddata->wdd, timeout, &ofdev->dev);
=======
		dev_info(dev, "could not be enabled in software\n");
		return -ENODEV;
	}

	res = platform_get_resource(ofdev, IORESOURCE_MEM, 1);
	if (res) {
		bool status;
		u32 __iomem *rsr = ioremap(res->start, resource_size(res));

		if (!rsr)
			return -ENOMEM;

		status = in_be32(rsr) & wdt_type->rsr_mask;
		ddata->wdd.bootstatus = status ? WDIOF_CARDRESET : 0;
		 /* clear reset status bits related to watchdog timer */
		out_be32(rsr, wdt_type->rsr_mask);
		iounmap(rsr);

		dev_info(dev, "Last boot was %scaused by watchdog\n",
			 status ? "" : "not ");
	}

	spin_lock_init(&ddata->lock);

	ddata->wdd.info = &mpc8xxx_wdt_info;
	ddata->wdd.ops = &mpc8xxx_wdt_ops;

	ddata->wdd.timeout = WATCHDOG_TIMEOUT;
	watchdog_init_timeout(&ddata->wdd, timeout, dev);
>>>>>>> upstream/android-13

	watchdog_set_nowayout(&ddata->wdd, nowayout);

	ddata->swtc = min(ddata->wdd.timeout * freq / wdt_type->prescaler,
			  0xffffU);

	/*
	 * If the watchdog was previously enabled or we're running on
	 * MPC8xxx, we should ping the wdt from the kernel until the
	 * userspace handles it.
	 */
	if (enabled)
		mpc8xxx_wdt_start(&ddata->wdd);

	ddata->wdd.max_hw_heartbeat_ms = (ddata->swtc * wdt_type->prescaler) /
					 (freq / 1000);
	ddata->wdd.min_timeout = ddata->wdd.max_hw_heartbeat_ms / 1000;
	if (ddata->wdd.timeout < ddata->wdd.min_timeout)
		ddata->wdd.timeout = ddata->wdd.min_timeout;

<<<<<<< HEAD
	ret = watchdog_register_device(&ddata->wdd);
	if (ret) {
		pr_err("cannot register watchdog device (err=%d)\n", ret);
		return ret;
	}

	pr_info("WDT driver for MPC8xxx initialized. mode:%s timeout=%d sec\n",
		reset ? "reset" : "interrupt", ddata->wdd.timeout);
=======
	ret = devm_watchdog_register_device(dev, &ddata->wdd);
	if (ret)
		return ret;

	dev_info(dev,
		 "WDT driver for MPC8xxx initialized. mode:%s timeout=%d sec\n",
		 reset ? "reset" : "interrupt", ddata->wdd.timeout);
>>>>>>> upstream/android-13

	platform_set_drvdata(ofdev, ddata);
	return 0;
}

<<<<<<< HEAD
static int mpc8xxx_wdt_remove(struct platform_device *ofdev)
{
	struct mpc8xxx_wdt_ddata *ddata = platform_get_drvdata(ofdev);

	pr_crit("Watchdog removed, expect the %s soon!\n",
		reset ? "reset" : "machine check exception");
	watchdog_unregister_device(&ddata->wdd);

	return 0;
}

=======
>>>>>>> upstream/android-13
static const struct of_device_id mpc8xxx_wdt_match[] = {
	{
		.compatible = "mpc83xx_wdt",
		.data = &(struct mpc8xxx_wdt_type) {
			.prescaler = 0x10000,
<<<<<<< HEAD
=======
			.rsr_mask = BIT(3), /* RSR Bit SWRS */
>>>>>>> upstream/android-13
		},
	},
	{
		.compatible = "fsl,mpc8610-wdt",
		.data = &(struct mpc8xxx_wdt_type) {
			.prescaler = 0x10000,
			.hw_enabled = true,
<<<<<<< HEAD
=======
			.rsr_mask = BIT(20), /* RSTRSCR Bit WDT_RR */
>>>>>>> upstream/android-13
		},
	},
	{
		.compatible = "fsl,mpc823-wdt",
		.data = &(struct mpc8xxx_wdt_type) {
			.prescaler = 0x800,
			.hw_enabled = true,
<<<<<<< HEAD
=======
			.rsr_mask = BIT(28), /* RSR Bit SWRS */
>>>>>>> upstream/android-13
		},
	},
	{},
};
MODULE_DEVICE_TABLE(of, mpc8xxx_wdt_match);

static struct platform_driver mpc8xxx_wdt_driver = {
	.probe		= mpc8xxx_wdt_probe,
<<<<<<< HEAD
	.remove		= mpc8xxx_wdt_remove,
=======
>>>>>>> upstream/android-13
	.driver = {
		.name = "mpc8xxx_wdt",
		.of_match_table = mpc8xxx_wdt_match,
	},
};

static int __init mpc8xxx_wdt_init(void)
{
	return platform_driver_register(&mpc8xxx_wdt_driver);
}
arch_initcall(mpc8xxx_wdt_init);

static void __exit mpc8xxx_wdt_exit(void)
{
	platform_driver_unregister(&mpc8xxx_wdt_driver);
}
module_exit(mpc8xxx_wdt_exit);

MODULE_AUTHOR("Dave Updegraff, Kumar Gala");
MODULE_DESCRIPTION("Driver for watchdog timer in MPC8xx/MPC83xx/MPC86xx "
		   "uProcessors");
MODULE_LICENSE("GPL");
