<<<<<<< HEAD
/*
 *  Copyright (C) 2010, Paul Cercueil <paul@crapouillou.net>
 *  JZ4740 Watchdog driver
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2010, Paul Cercueil <paul@crapouillou.net>
 *  JZ4740 Watchdog driver
 */

#include <linux/mfd/ingenic-tcu.h>
#include <linux/mfd/syscon.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/watchdog.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/of.h>
<<<<<<< HEAD

#include <asm/mach-jz4740/timer.h>

#define JZ_REG_WDT_TIMER_DATA     0x0
#define JZ_REG_WDT_COUNTER_ENABLE 0x4
#define JZ_REG_WDT_TIMER_COUNTER  0x8
#define JZ_REG_WDT_TIMER_CONTROL  0xC

#define JZ_WDT_CLOCK_PCLK 0x1
#define JZ_WDT_CLOCK_RTC  0x2
#define JZ_WDT_CLOCK_EXT  0x4

#define JZ_WDT_CLOCK_DIV_SHIFT   3

#define JZ_WDT_CLOCK_DIV_1    (0 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_4    (1 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_16   (2 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_64   (3 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_256  (4 << JZ_WDT_CLOCK_DIV_SHIFT)
#define JZ_WDT_CLOCK_DIV_1024 (5 << JZ_WDT_CLOCK_DIV_SHIFT)
=======
#include <linux/regmap.h>
>>>>>>> upstream/android-13

#define DEFAULT_HEARTBEAT 5
#define MAX_HEARTBEAT     2048

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout,
		 "Watchdog cannot be stopped once started (default="
		 __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static unsigned int heartbeat = DEFAULT_HEARTBEAT;
module_param(heartbeat, uint, 0);
MODULE_PARM_DESC(heartbeat,
		"Watchdog heartbeat period in seconds from 1 to "
		__MODULE_STRING(MAX_HEARTBEAT) ", default "
		__MODULE_STRING(DEFAULT_HEARTBEAT));

struct jz4740_wdt_drvdata {
	struct watchdog_device wdt;
<<<<<<< HEAD
	void __iomem *base;
	struct clk *rtc_clk;
=======
	struct regmap *map;
	struct clk *clk;
	unsigned long clk_rate;
>>>>>>> upstream/android-13
};

static int jz4740_wdt_ping(struct watchdog_device *wdt_dev)
{
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);

<<<<<<< HEAD
	writew(0x0, drvdata->base + JZ_REG_WDT_TIMER_COUNTER);
=======
	regmap_write(drvdata->map, TCU_REG_WDT_TCNT, 0);

>>>>>>> upstream/android-13
	return 0;
}

static int jz4740_wdt_set_timeout(struct watchdog_device *wdt_dev,
				    unsigned int new_timeout)
{
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);
<<<<<<< HEAD
	unsigned int rtc_clk_rate;
	unsigned int timeout_value;
	unsigned short clock_div = JZ_WDT_CLOCK_DIV_1;

	rtc_clk_rate = clk_get_rate(drvdata->rtc_clk);

	timeout_value = rtc_clk_rate * new_timeout;
	while (timeout_value > 0xffff) {
		if (clock_div == JZ_WDT_CLOCK_DIV_1024) {
			/* Requested timeout too high;
			* use highest possible value. */
			timeout_value = 0xffff;
			break;
		}
		timeout_value >>= 2;
		clock_div += (1 << JZ_WDT_CLOCK_DIV_SHIFT);
	}

	writeb(0x0, drvdata->base + JZ_REG_WDT_COUNTER_ENABLE);
	writew(clock_div, drvdata->base + JZ_REG_WDT_TIMER_CONTROL);

	writew((u16)timeout_value, drvdata->base + JZ_REG_WDT_TIMER_DATA);
	writew(0x0, drvdata->base + JZ_REG_WDT_TIMER_COUNTER);
	writew(clock_div | JZ_WDT_CLOCK_RTC,
		drvdata->base + JZ_REG_WDT_TIMER_CONTROL);

	writeb(0x1, drvdata->base + JZ_REG_WDT_COUNTER_ENABLE);
=======
	u16 timeout_value = (u16)(drvdata->clk_rate * new_timeout);
	unsigned int tcer;

	regmap_read(drvdata->map, TCU_REG_WDT_TCER, &tcer);
	regmap_write(drvdata->map, TCU_REG_WDT_TCER, 0);

	regmap_write(drvdata->map, TCU_REG_WDT_TDR, timeout_value);
	regmap_write(drvdata->map, TCU_REG_WDT_TCNT, 0);

	if (tcer & TCU_WDT_TCER_TCEN)
		regmap_write(drvdata->map, TCU_REG_WDT_TCER, TCU_WDT_TCER_TCEN);
>>>>>>> upstream/android-13

	wdt_dev->timeout = new_timeout;
	return 0;
}

static int jz4740_wdt_start(struct watchdog_device *wdt_dev)
{
<<<<<<< HEAD
	jz4740_timer_enable_watchdog();
	jz4740_wdt_set_timeout(wdt_dev, wdt_dev->timeout);

=======
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);
	unsigned int tcer;
	int ret;

	ret = clk_prepare_enable(drvdata->clk);
	if (ret)
		return ret;

	regmap_read(drvdata->map, TCU_REG_WDT_TCER, &tcer);

	jz4740_wdt_set_timeout(wdt_dev, wdt_dev->timeout);

	/* Start watchdog if it wasn't started already */
	if (!(tcer & TCU_WDT_TCER_TCEN))
		regmap_write(drvdata->map, TCU_REG_WDT_TCER, TCU_WDT_TCER_TCEN);

>>>>>>> upstream/android-13
	return 0;
}

static int jz4740_wdt_stop(struct watchdog_device *wdt_dev)
{
	struct jz4740_wdt_drvdata *drvdata = watchdog_get_drvdata(wdt_dev);

<<<<<<< HEAD
	writeb(0x0, drvdata->base + JZ_REG_WDT_COUNTER_ENABLE);
	jz4740_timer_disable_watchdog();
=======
	regmap_write(drvdata->map, TCU_REG_WDT_TCER, 0);
	clk_disable_unprepare(drvdata->clk);
>>>>>>> upstream/android-13

	return 0;
}

static int jz4740_wdt_restart(struct watchdog_device *wdt_dev,
			      unsigned long action, void *data)
{
	wdt_dev->timeout = 0;
	jz4740_wdt_start(wdt_dev);
	return 0;
}

static const struct watchdog_info jz4740_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.identity = "jz4740 Watchdog",
};

static const struct watchdog_ops jz4740_wdt_ops = {
	.owner = THIS_MODULE,
	.start = jz4740_wdt_start,
	.stop = jz4740_wdt_stop,
	.ping = jz4740_wdt_ping,
	.set_timeout = jz4740_wdt_set_timeout,
	.restart = jz4740_wdt_restart,
};

#ifdef CONFIG_OF
static const struct of_device_id jz4740_wdt_of_matches[] = {
	{ .compatible = "ingenic,jz4740-watchdog", },
	{ .compatible = "ingenic,jz4780-watchdog", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, jz4740_wdt_of_matches);
#endif

static int jz4740_wdt_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct jz4740_wdt_drvdata *drvdata;
	struct watchdog_device *jz4740_wdt;
	struct resource	*res;
	int ret;

	drvdata = devm_kzalloc(&pdev->dev, sizeof(struct jz4740_wdt_drvdata),
=======
	struct device *dev = &pdev->dev;
	struct jz4740_wdt_drvdata *drvdata;
	struct watchdog_device *jz4740_wdt;
	long rate;
	int ret;

	drvdata = devm_kzalloc(dev, sizeof(struct jz4740_wdt_drvdata),
>>>>>>> upstream/android-13
			       GFP_KERNEL);
	if (!drvdata)
		return -ENOMEM;

<<<<<<< HEAD
	if (heartbeat < 1 || heartbeat > MAX_HEARTBEAT)
		heartbeat = DEFAULT_HEARTBEAT;

	jz4740_wdt = &drvdata->wdt;
	jz4740_wdt->info = &jz4740_wdt_info;
	jz4740_wdt->ops = &jz4740_wdt_ops;
	jz4740_wdt->timeout = heartbeat;
	jz4740_wdt->min_timeout = 1;
	jz4740_wdt->max_timeout = MAX_HEARTBEAT;
	jz4740_wdt->parent = &pdev->dev;
	watchdog_set_nowayout(jz4740_wdt, nowayout);
	watchdog_set_drvdata(jz4740_wdt, drvdata);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	drvdata->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(drvdata->base))
		return PTR_ERR(drvdata->base);

	drvdata->rtc_clk = devm_clk_get(&pdev->dev, "rtc");
	if (IS_ERR(drvdata->rtc_clk)) {
		dev_err(&pdev->dev, "cannot find RTC clock\n");
		return PTR_ERR(drvdata->rtc_clk);
	}

	ret = devm_watchdog_register_device(&pdev->dev, &drvdata->wdt);
	if (ret < 0)
		return ret;

	platform_set_drvdata(pdev, drvdata);

	return 0;
=======
	drvdata->clk = devm_clk_get(&pdev->dev, "wdt");
	if (IS_ERR(drvdata->clk)) {
		dev_err(&pdev->dev, "cannot find WDT clock\n");
		return PTR_ERR(drvdata->clk);
	}

	/* Set smallest clock possible */
	rate = clk_round_rate(drvdata->clk, 1);
	if (rate < 0)
		return rate;

	ret = clk_set_rate(drvdata->clk, rate);
	if (ret)
		return ret;

	drvdata->clk_rate = rate;
	jz4740_wdt = &drvdata->wdt;
	jz4740_wdt->info = &jz4740_wdt_info;
	jz4740_wdt->ops = &jz4740_wdt_ops;
	jz4740_wdt->min_timeout = 1;
	jz4740_wdt->max_timeout = 0xffff / rate;
	jz4740_wdt->timeout = clamp(heartbeat,
				    jz4740_wdt->min_timeout,
				    jz4740_wdt->max_timeout);
	jz4740_wdt->parent = dev;
	watchdog_set_nowayout(jz4740_wdt, nowayout);
	watchdog_set_drvdata(jz4740_wdt, drvdata);

	drvdata->map = device_node_to_regmap(dev->parent->of_node);
	if (IS_ERR(drvdata->map)) {
		dev_err(dev, "regmap not found\n");
		return PTR_ERR(drvdata->map);
	}

	return devm_watchdog_register_device(dev, &drvdata->wdt);
>>>>>>> upstream/android-13
}

static struct platform_driver jz4740_wdt_driver = {
	.probe = jz4740_wdt_probe,
	.driver = {
		.name = "jz4740-wdt",
		.of_match_table = of_match_ptr(jz4740_wdt_of_matches),
	},
};

module_platform_driver(jz4740_wdt_driver);

MODULE_AUTHOR("Paul Cercueil <paul@crapouillou.net>");
MODULE_DESCRIPTION("jz4740 Watchdog Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:jz4740-wdt");
