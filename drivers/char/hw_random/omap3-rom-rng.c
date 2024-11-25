/*
 * omap3-rom-rng.c - RNG driver for TI OMAP3 CPU family
 *
 * Copyright (C) 2009 Nokia Corporation
 * Author: Juha Yrjola <juha.yrjola@solidboot.com>
 *
<<<<<<< HEAD
 * Copyright (C) 2013 Pali Rohár <pali.rohar@gmail.com>
=======
 * Copyright (C) 2013 Pali Rohár <pali@kernel.org>
>>>>>>> upstream/android-13
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

<<<<<<< HEAD
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

=======
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/init.h>
#include <linux/random.h>
#include <linux/hw_random.h>
#include <linux/workqueue.h>
#include <linux/clk.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
=======
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
>>>>>>> upstream/android-13

#define RNG_RESET			0x01
#define RNG_GEN_PRNG_HW_INIT		0x02
#define RNG_GEN_HW			0x08

<<<<<<< HEAD
/* param1: ptr, param2: count, param3: flag */
static u32 (*omap3_rom_rng_call)(u32, u32, u32);

static struct delayed_work idle_work;
static int rng_idle;
static struct clk *rng_clk;

static void omap3_rom_rng_idle(struct work_struct *work)
{
	int r;

	r = omap3_rom_rng_call(0, 0, RNG_RESET);
	if (r != 0) {
		pr_err("reset failed: %d\n", r);
		return;
	}
	clk_disable_unprepare(rng_clk);
	rng_idle = 1;
}

static int omap3_rom_rng_get_random(void *buf, unsigned int count)
{
	u32 r;
	u32 ptr;

	cancel_delayed_work_sync(&idle_work);
	if (rng_idle) {
		r = clk_prepare_enable(rng_clk);
		if (r)
			return r;

		r = omap3_rom_rng_call(0, 0, RNG_GEN_PRNG_HW_INIT);
		if (r != 0) {
			clk_disable_unprepare(rng_clk);
			pr_err("HW init failed: %d\n", r);
			return -EIO;
		}
		rng_idle = 0;
	}

	ptr = virt_to_phys(buf);
	r = omap3_rom_rng_call(ptr, count, RNG_GEN_HW);
	schedule_delayed_work(&idle_work, msecs_to_jiffies(500));
	if (r != 0)
		return -EINVAL;
	return 0;
}

static int omap3_rom_rng_read(struct hwrng *rng, void *data, size_t max, bool w)
{
	int r;

	r = omap3_rom_rng_get_random(data, 4);
	if (r < 0)
		return r;
	return 4;
}

static struct hwrng omap3_rom_rng_ops = {
	.name		= "omap3-rom",
};

static int omap3_rom_rng_probe(struct platform_device *pdev)
{
	int ret = 0;

	omap3_rom_rng_ops.read = of_device_get_match_data(&pdev->dev);
	if (!omap3_rom_rng_ops.read) {
=======
struct omap_rom_rng {
	struct clk *clk;
	struct device *dev;
	struct hwrng ops;
	u32 (*rom_rng_call)(u32 ptr, u32 count, u32 flag);
};

static int omap3_rom_rng_read(struct hwrng *rng, void *data, size_t max, bool w)
{
	struct omap_rom_rng *ddata;
	u32 ptr;
	int r;

	ddata = (struct omap_rom_rng *)rng->priv;

	r = pm_runtime_get_sync(ddata->dev);
	if (r < 0) {
		pm_runtime_put_noidle(ddata->dev);

		return r;
	}

	ptr = virt_to_phys(data);
	r = ddata->rom_rng_call(ptr, 4, RNG_GEN_HW);
	if (r != 0)
		r = -EINVAL;
	else
		r = 4;

	pm_runtime_mark_last_busy(ddata->dev);
	pm_runtime_put_autosuspend(ddata->dev);

	return r;
}

static int __maybe_unused omap_rom_rng_runtime_suspend(struct device *dev)
{
	struct omap_rom_rng *ddata;
	int r;

	ddata = dev_get_drvdata(dev);

	r = ddata->rom_rng_call(0, 0, RNG_RESET);
	if (r != 0)
		dev_err(dev, "reset failed: %d\n", r);

	clk_disable_unprepare(ddata->clk);

	return 0;
}

static int __maybe_unused omap_rom_rng_runtime_resume(struct device *dev)
{
	struct omap_rom_rng *ddata;
	int r;

	ddata = dev_get_drvdata(dev);

	r = clk_prepare_enable(ddata->clk);
	if (r < 0)
		return r;

	r = ddata->rom_rng_call(0, 0, RNG_GEN_PRNG_HW_INIT);
	if (r != 0) {
		clk_disable(ddata->clk);
		dev_err(dev, "HW init failed: %d\n", r);

		return -EIO;
	}

	return 0;
}

static void omap_rom_rng_finish(void *data)
{
	struct omap_rom_rng *ddata = data;

	pm_runtime_dont_use_autosuspend(ddata->dev);
	pm_runtime_disable(ddata->dev);
}

static int omap3_rom_rng_probe(struct platform_device *pdev)
{
	struct omap_rom_rng *ddata;
	int ret = 0;

	ddata = devm_kzalloc(&pdev->dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->dev = &pdev->dev;
	ddata->ops.priv = (unsigned long)ddata;
	ddata->ops.name = "omap3-rom";
	ddata->ops.read = of_device_get_match_data(&pdev->dev);
	ddata->ops.quality = 900;
	if (!ddata->ops.read) {
>>>>>>> upstream/android-13
		dev_err(&pdev->dev, "missing rom code handler\n");

		return -ENODEV;
	}
<<<<<<< HEAD

	omap3_rom_rng_call = pdev->dev.platform_data;
	if (!omap3_rom_rng_call) {
		pr_err("omap3_rom_rng_call is NULL\n");
		return -EINVAL;
	}

	INIT_DELAYED_WORK(&idle_work, omap3_rom_rng_idle);
	rng_clk = devm_clk_get(&pdev->dev, "ick");
	if (IS_ERR(rng_clk)) {
		pr_err("unable to get RNG clock\n");
		return PTR_ERR(rng_clk);
	}

	/* Leave the RNG in reset state. */
	ret = clk_prepare_enable(rng_clk);
	if (ret)
		return ret;
	omap3_rom_rng_idle(0);

	return hwrng_register(&omap3_rom_rng_ops);
}

static int omap3_rom_rng_remove(struct platform_device *pdev)
{
	cancel_delayed_work_sync(&idle_work);
	hwrng_unregister(&omap3_rom_rng_ops);
	if (!rng_idle)
		clk_disable_unprepare(rng_clk);
	return 0;
=======
	dev_set_drvdata(ddata->dev, ddata);

	ddata->rom_rng_call = pdev->dev.platform_data;
	if (!ddata->rom_rng_call) {
		dev_err(ddata->dev, "rom_rng_call is NULL\n");
		return -EINVAL;
	}

	ddata->clk = devm_clk_get(ddata->dev, "ick");
	if (IS_ERR(ddata->clk)) {
		dev_err(ddata->dev, "unable to get RNG clock\n");
		return PTR_ERR(ddata->clk);
	}

	pm_runtime_enable(&pdev->dev);
	pm_runtime_set_autosuspend_delay(&pdev->dev, 500);
	pm_runtime_use_autosuspend(&pdev->dev);

	ret = devm_add_action_or_reset(ddata->dev, omap_rom_rng_finish,
				       ddata);
	if (ret)
		return ret;

	return devm_hwrng_register(ddata->dev, &ddata->ops);
>>>>>>> upstream/android-13
}

static const struct of_device_id omap_rom_rng_match[] = {
	{ .compatible = "nokia,n900-rom-rng", .data = omap3_rom_rng_read, },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, omap_rom_rng_match);

<<<<<<< HEAD
=======
static const struct dev_pm_ops omap_rom_rng_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(omap_rom_rng_runtime_suspend,
				omap_rom_rng_runtime_resume)
};

>>>>>>> upstream/android-13
static struct platform_driver omap3_rom_rng_driver = {
	.driver = {
		.name		= "omap3-rom-rng",
		.of_match_table = omap_rom_rng_match,
<<<<<<< HEAD
	},
	.probe		= omap3_rom_rng_probe,
	.remove		= omap3_rom_rng_remove,
=======
		.pm = &omap_rom_rng_pm_ops,
	},
	.probe		= omap3_rom_rng_probe,
>>>>>>> upstream/android-13
};

module_platform_driver(omap3_rom_rng_driver);

MODULE_ALIAS("platform:omap3-rom-rng");
MODULE_AUTHOR("Juha Yrjola");
<<<<<<< HEAD
MODULE_AUTHOR("Pali Rohár <pali.rohar@gmail.com>");
=======
MODULE_AUTHOR("Pali Rohár <pali@kernel.org>");
>>>>>>> upstream/android-13
MODULE_LICENSE("GPL");
