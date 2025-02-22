// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2015 Broadcom Corporation
 *
 */

#include <linux/clk.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/watchdog.h>

#define WDT_START_1		0xff00
#define WDT_START_2		0x00ff
#define WDT_STOP_1		0xee00
#define WDT_STOP_2		0x00ee

#define WDT_TIMEOUT_REG		0x0
#define WDT_CMD_REG		0x4

#define WDT_MIN_TIMEOUT		1 /* seconds */
#define WDT_DEFAULT_TIMEOUT	30 /* seconds */
#define WDT_DEFAULT_RATE	27000000

struct bcm7038_watchdog {
	void __iomem		*base;
	struct watchdog_device	wdd;
	u32			rate;
	struct clk		*clk;
};

static bool nowayout = WATCHDOG_NOWAYOUT;

<<<<<<< HEAD
=======
static inline void bcm7038_wdt_write(u32 value, void __iomem *addr)
{
	/* MIPS chips strapped for BE will automagically configure the
	 * peripheral registers for CPU-native byte order.
	 */
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(CONFIG_CPU_BIG_ENDIAN))
		__raw_writel(value, addr);
	else
		writel_relaxed(value, addr);
}

static inline u32 bcm7038_wdt_read(void __iomem *addr)
{
	if (IS_ENABLED(CONFIG_MIPS) && IS_ENABLED(CONFIG_CPU_BIG_ENDIAN))
		return __raw_readl(addr);
	else
		return readl_relaxed(addr);
}

>>>>>>> upstream/android-13
static void bcm7038_wdt_set_timeout_reg(struct watchdog_device *wdog)
{
	struct bcm7038_watchdog *wdt = watchdog_get_drvdata(wdog);
	u32 timeout;

	timeout = wdt->rate * wdog->timeout;

<<<<<<< HEAD
	writel(timeout, wdt->base + WDT_TIMEOUT_REG);
=======
	bcm7038_wdt_write(timeout, wdt->base + WDT_TIMEOUT_REG);
>>>>>>> upstream/android-13
}

static int bcm7038_wdt_ping(struct watchdog_device *wdog)
{
	struct bcm7038_watchdog *wdt = watchdog_get_drvdata(wdog);

<<<<<<< HEAD
	writel(WDT_START_1, wdt->base + WDT_CMD_REG);
	writel(WDT_START_2, wdt->base + WDT_CMD_REG);
=======
	bcm7038_wdt_write(WDT_START_1, wdt->base + WDT_CMD_REG);
	bcm7038_wdt_write(WDT_START_2, wdt->base + WDT_CMD_REG);
>>>>>>> upstream/android-13

	return 0;
}

static int bcm7038_wdt_start(struct watchdog_device *wdog)
{
	bcm7038_wdt_set_timeout_reg(wdog);
	bcm7038_wdt_ping(wdog);

	return 0;
}

static int bcm7038_wdt_stop(struct watchdog_device *wdog)
{
	struct bcm7038_watchdog *wdt = watchdog_get_drvdata(wdog);

<<<<<<< HEAD
	writel(WDT_STOP_1, wdt->base + WDT_CMD_REG);
	writel(WDT_STOP_2, wdt->base + WDT_CMD_REG);
=======
	bcm7038_wdt_write(WDT_STOP_1, wdt->base + WDT_CMD_REG);
	bcm7038_wdt_write(WDT_STOP_2, wdt->base + WDT_CMD_REG);
>>>>>>> upstream/android-13

	return 0;
}

static int bcm7038_wdt_set_timeout(struct watchdog_device *wdog,
				   unsigned int t)
{
	/* Can't modify timeout value if watchdog timer is running */
	bcm7038_wdt_stop(wdog);
	wdog->timeout = t;
	bcm7038_wdt_start(wdog);

	return 0;
}

static unsigned int bcm7038_wdt_get_timeleft(struct watchdog_device *wdog)
{
	struct bcm7038_watchdog *wdt = watchdog_get_drvdata(wdog);
	u32 time_left;

<<<<<<< HEAD
	time_left = readl(wdt->base + WDT_CMD_REG);
=======
	time_left = bcm7038_wdt_read(wdt->base + WDT_CMD_REG);
>>>>>>> upstream/android-13

	return time_left / wdt->rate;
}

static const struct watchdog_info bcm7038_wdt_info = {
	.identity	= "Broadcom BCM7038 Watchdog Timer",
	.options	= WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING |
				WDIOF_MAGICCLOSE
};

static const struct watchdog_ops bcm7038_wdt_ops = {
	.owner		= THIS_MODULE,
	.start		= bcm7038_wdt_start,
	.stop		= bcm7038_wdt_stop,
	.set_timeout	= bcm7038_wdt_set_timeout,
	.get_timeleft	= bcm7038_wdt_get_timeleft,
};

<<<<<<< HEAD
=======
static void bcm7038_clk_disable_unprepare(void *data)
{
	clk_disable_unprepare(data);
}

>>>>>>> upstream/android-13
static int bcm7038_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct bcm7038_watchdog *wdt;
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	int err;

	wdt = devm_kzalloc(dev, sizeof(*wdt), GFP_KERNEL);
	if (!wdt)
		return -ENOMEM;

	platform_set_drvdata(pdev, wdt);

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	wdt->base = devm_ioremap_resource(dev, res);
=======
	wdt->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(wdt->base))
		return PTR_ERR(wdt->base);

	wdt->clk = devm_clk_get(dev, NULL);
	/* If unable to get clock, use default frequency */
	if (!IS_ERR(wdt->clk)) {
		err = clk_prepare_enable(wdt->clk);
		if (err)
			return err;
<<<<<<< HEAD
=======
		err = devm_add_action_or_reset(dev,
					       bcm7038_clk_disable_unprepare,
					       wdt->clk);
		if (err)
			return err;
>>>>>>> upstream/android-13
		wdt->rate = clk_get_rate(wdt->clk);
		/* Prevent divide-by-zero exception */
		if (!wdt->rate)
			wdt->rate = WDT_DEFAULT_RATE;
	} else {
		wdt->rate = WDT_DEFAULT_RATE;
		wdt->clk = NULL;
	}

	wdt->wdd.info		= &bcm7038_wdt_info;
	wdt->wdd.ops		= &bcm7038_wdt_ops;
	wdt->wdd.min_timeout	= WDT_MIN_TIMEOUT;
	wdt->wdd.timeout	= WDT_DEFAULT_TIMEOUT;
	wdt->wdd.max_timeout	= 0xffffffff / wdt->rate;
	wdt->wdd.parent		= dev;
	watchdog_set_drvdata(&wdt->wdd, wdt);

<<<<<<< HEAD
	err = watchdog_register_device(&wdt->wdd);
	if (err) {
		dev_err(dev, "Failed to register watchdog device\n");
		clk_disable_unprepare(wdt->clk);
		return err;
	}
=======
	watchdog_stop_on_reboot(&wdt->wdd);
	watchdog_stop_on_unregister(&wdt->wdd);
	err = devm_watchdog_register_device(dev, &wdt->wdd);
	if (err)
		return err;
>>>>>>> upstream/android-13

	dev_info(dev, "Registered BCM7038 Watchdog\n");

	return 0;
}

<<<<<<< HEAD
static int bcm7038_wdt_remove(struct platform_device *pdev)
{
	struct bcm7038_watchdog *wdt = platform_get_drvdata(pdev);

	if (!nowayout)
		bcm7038_wdt_stop(&wdt->wdd);

	watchdog_unregister_device(&wdt->wdd);
	clk_disable_unprepare(wdt->clk);

	return 0;
}

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_PM_SLEEP
static int bcm7038_wdt_suspend(struct device *dev)
{
	struct bcm7038_watchdog *wdt = dev_get_drvdata(dev);

	if (watchdog_active(&wdt->wdd))
		return bcm7038_wdt_stop(&wdt->wdd);

	return 0;
}

static int bcm7038_wdt_resume(struct device *dev)
{
	struct bcm7038_watchdog *wdt = dev_get_drvdata(dev);

	if (watchdog_active(&wdt->wdd))
		return bcm7038_wdt_start(&wdt->wdd);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(bcm7038_wdt_pm_ops, bcm7038_wdt_suspend,
			 bcm7038_wdt_resume);

<<<<<<< HEAD
static void bcm7038_wdt_shutdown(struct platform_device *pdev)
{
	struct bcm7038_watchdog *wdt = platform_get_drvdata(pdev);

	if (watchdog_active(&wdt->wdd))
		bcm7038_wdt_stop(&wdt->wdd);
}

=======
>>>>>>> upstream/android-13
static const struct of_device_id bcm7038_wdt_match[] = {
	{ .compatible = "brcm,bcm7038-wdt" },
	{},
};
MODULE_DEVICE_TABLE(of, bcm7038_wdt_match);

static struct platform_driver bcm7038_wdt_driver = {
	.probe		= bcm7038_wdt_probe,
<<<<<<< HEAD
	.remove		= bcm7038_wdt_remove,
	.shutdown	= bcm7038_wdt_shutdown,
=======
>>>>>>> upstream/android-13
	.driver		= {
		.name		= "bcm7038-wdt",
		.of_match_table	= bcm7038_wdt_match,
		.pm		= &bcm7038_wdt_pm_ops,
	}
};
module_platform_driver(bcm7038_wdt_driver);

module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
	__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for Broadcom 7038 SoCs Watchdog");
MODULE_AUTHOR("Justin Chen");
