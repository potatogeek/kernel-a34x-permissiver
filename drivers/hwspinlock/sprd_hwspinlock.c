// SPDX-License-Identifier: GPL-2.0
/*
 * Spreadtrum hardware spinlock driver
 * Copyright (C) 2017 Spreadtrum  - http://www.spreadtrum.com
 */

<<<<<<< HEAD
#include <linux/bitops.h>
=======
>>>>>>> upstream/android-13
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/hwspinlock.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/pm_runtime.h>
#include <linux/slab.h>
=======
>>>>>>> upstream/android-13

#include "hwspinlock_internal.h"

/* hwspinlock registers definition */
#define HWSPINLOCK_RECCTRL		0x4
#define HWSPINLOCK_MASTERID(_X_)	(0x80 + 0x4 * (_X_))
#define HWSPINLOCK_TOKEN(_X_)		(0x800 + 0x4 * (_X_))

/* unlocked value */
#define HWSPINLOCK_NOTTAKEN		0x55aa10c5
/* bits definition of RECCTRL reg */
#define HWSPINLOCK_USER_BITS		0x1

/* hwspinlock number */
#define SPRD_HWLOCKS_NUM		32

struct sprd_hwspinlock_dev {
	void __iomem *base;
	struct clk *clk;
	struct hwspinlock_device bank;
};

/* try to lock the hardware spinlock */
static int sprd_hwspinlock_trylock(struct hwspinlock *lock)
{
	struct sprd_hwspinlock_dev *sprd_hwlock =
		dev_get_drvdata(lock->bank->dev);
	void __iomem *addr = lock->priv;
	int user_id, lock_id;

	if (!readl(addr))
		return 1;

	lock_id = hwlock_to_id(lock);
	/* get the hardware spinlock master/user id */
	user_id = readl(sprd_hwlock->base + HWSPINLOCK_MASTERID(lock_id));
	dev_warn(sprd_hwlock->bank.dev,
		 "hwspinlock [%d] lock failed and master/user id = %d!\n",
		 lock_id, user_id);
	return 0;
}

/* unlock the hardware spinlock */
static void sprd_hwspinlock_unlock(struct hwspinlock *lock)
{
	void __iomem *lock_addr = lock->priv;

	writel(HWSPINLOCK_NOTTAKEN, lock_addr);
}

/* The specs recommended below number as the retry delay time */
static void sprd_hwspinlock_relax(struct hwspinlock *lock)
{
	ndelay(10);
}

static const struct hwspinlock_ops sprd_hwspinlock_ops = {
	.trylock = sprd_hwspinlock_trylock,
	.unlock = sprd_hwspinlock_unlock,
	.relax = sprd_hwspinlock_relax,
};

<<<<<<< HEAD
=======
static void sprd_hwspinlock_disable(void *data)
{
	struct sprd_hwspinlock_dev *sprd_hwlock = data;

	clk_disable_unprepare(sprd_hwlock->clk);
}

>>>>>>> upstream/android-13
static int sprd_hwspinlock_probe(struct platform_device *pdev)
{
	struct sprd_hwspinlock_dev *sprd_hwlock;
	struct hwspinlock *lock;
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> upstream/android-13
	int i, ret;

	if (!pdev->dev.of_node)
		return -ENODEV;

	sprd_hwlock = devm_kzalloc(&pdev->dev,
				   sizeof(struct sprd_hwspinlock_dev) +
				   SPRD_HWLOCKS_NUM * sizeof(*lock),
				   GFP_KERNEL);
	if (!sprd_hwlock)
		return -ENOMEM;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	sprd_hwlock->base = devm_ioremap_resource(&pdev->dev, res);
=======
	sprd_hwlock->base = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(sprd_hwlock->base))
		return PTR_ERR(sprd_hwlock->base);

	sprd_hwlock->clk = devm_clk_get(&pdev->dev, "enable");
	if (IS_ERR(sprd_hwlock->clk)) {
		dev_err(&pdev->dev, "get hwspinlock clock failed!\n");
		return PTR_ERR(sprd_hwlock->clk);
	}

<<<<<<< HEAD
	clk_prepare_enable(sprd_hwlock->clk);
=======
	ret = clk_prepare_enable(sprd_hwlock->clk);
	if (ret)
		return ret;

	ret = devm_add_action_or_reset(&pdev->dev, sprd_hwspinlock_disable,
				       sprd_hwlock);
	if (ret) {
		dev_err(&pdev->dev,
			"Failed to add hwspinlock disable action\n");
		return ret;
	}
>>>>>>> upstream/android-13

	/* set the hwspinlock to record user id to identify subsystems */
	writel(HWSPINLOCK_USER_BITS, sprd_hwlock->base + HWSPINLOCK_RECCTRL);

	for (i = 0; i < SPRD_HWLOCKS_NUM; i++) {
		lock = &sprd_hwlock->bank.lock[i];
		lock->priv = sprd_hwlock->base + HWSPINLOCK_TOKEN(i);
	}

	platform_set_drvdata(pdev, sprd_hwlock);
<<<<<<< HEAD
	pm_runtime_enable(&pdev->dev);

	ret = hwspin_lock_register(&sprd_hwlock->bank, &pdev->dev,
				   &sprd_hwspinlock_ops, 0, SPRD_HWLOCKS_NUM);
	if (ret) {
		pm_runtime_disable(&pdev->dev);
		clk_disable_unprepare(sprd_hwlock->clk);
		return ret;
	}

	return 0;
}

static int sprd_hwspinlock_remove(struct platform_device *pdev)
{
	struct sprd_hwspinlock_dev *sprd_hwlock = platform_get_drvdata(pdev);

	hwspin_lock_unregister(&sprd_hwlock->bank);
	pm_runtime_disable(&pdev->dev);
	clk_disable_unprepare(sprd_hwlock->clk);
	return 0;
=======

	return devm_hwspin_lock_register(&pdev->dev, &sprd_hwlock->bank,
					 &sprd_hwspinlock_ops, 0,
					 SPRD_HWLOCKS_NUM);
>>>>>>> upstream/android-13
}

static const struct of_device_id sprd_hwspinlock_of_match[] = {
	{ .compatible = "sprd,hwspinlock-r3p0", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sprd_hwspinlock_of_match);

static struct platform_driver sprd_hwspinlock_driver = {
	.probe = sprd_hwspinlock_probe,
<<<<<<< HEAD
	.remove = sprd_hwspinlock_remove,
	.driver = {
		.name = "sprd_hwspinlock",
		.of_match_table = of_match_ptr(sprd_hwspinlock_of_match),
	},
};

static int __init sprd_hwspinlock_init(void)
{
	return platform_driver_register(&sprd_hwspinlock_driver);
}
postcore_initcall(sprd_hwspinlock_init);

static void __exit sprd_hwspinlock_exit(void)
{
	platform_driver_unregister(&sprd_hwspinlock_driver);
}
module_exit(sprd_hwspinlock_exit);
=======
	.driver = {
		.name = "sprd_hwspinlock",
		.of_match_table = sprd_hwspinlock_of_match,
	},
};
module_platform_driver(sprd_hwspinlock_driver);
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Hardware spinlock driver for Spreadtrum");
MODULE_AUTHOR("Baolin Wang <baolin.wang@spreadtrum.com>");
MODULE_AUTHOR("Lanqing Liu <lanqing.liu@spreadtrum.com>");
MODULE_AUTHOR("Long Cheng <aiden.cheng@spreadtrum.com>");
