<<<<<<< HEAD
/*
 * An RTC driver for the NVIDIA Tegra 200 series internal RTC.
 *
 * Copyright (c) 2010, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
=======
// SPDX-License-Identifier: GPL-2.0+
/*
 * An RTC driver for the NVIDIA Tegra 200 series internal RTC.
 *
 * Copyright (c) 2010-2019, NVIDIA Corporation.
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/rtc.h>
#include <linux/slab.h>

<<<<<<< HEAD
/* set to 1 = busy every eight 32kHz clocks during copy of sec+msec to AHB */
#define TEGRA_RTC_REG_BUSY			0x004
#define TEGRA_RTC_REG_SECONDS			0x008
/* when msec is read, the seconds are buffered into shadow seconds. */
=======
/* Set to 1 = busy every eight 32 kHz clocks during copy of sec+msec to AHB. */
#define TEGRA_RTC_REG_BUSY			0x004
#define TEGRA_RTC_REG_SECONDS			0x008
/* When msec is read, the seconds are buffered into shadow seconds. */
>>>>>>> upstream/android-13
#define TEGRA_RTC_REG_SHADOW_SECONDS		0x00c
#define TEGRA_RTC_REG_MILLI_SECONDS		0x010
#define TEGRA_RTC_REG_SECONDS_ALARM0		0x014
#define TEGRA_RTC_REG_SECONDS_ALARM1		0x018
#define TEGRA_RTC_REG_MILLI_SECONDS_ALARM0	0x01c
#define TEGRA_RTC_REG_INTR_MASK			0x028
/* write 1 bits to clear status bits */
#define TEGRA_RTC_REG_INTR_STATUS		0x02c

/* bits in INTR_MASK */
#define TEGRA_RTC_INTR_MASK_MSEC_CDN_ALARM	(1<<4)
#define TEGRA_RTC_INTR_MASK_SEC_CDN_ALARM	(1<<3)
#define TEGRA_RTC_INTR_MASK_MSEC_ALARM		(1<<2)
#define TEGRA_RTC_INTR_MASK_SEC_ALARM1		(1<<1)
#define TEGRA_RTC_INTR_MASK_SEC_ALARM0		(1<<0)

/* bits in INTR_STATUS */
#define TEGRA_RTC_INTR_STATUS_MSEC_CDN_ALARM	(1<<4)
#define TEGRA_RTC_INTR_STATUS_SEC_CDN_ALARM	(1<<3)
#define TEGRA_RTC_INTR_STATUS_MSEC_ALARM	(1<<2)
#define TEGRA_RTC_INTR_STATUS_SEC_ALARM1	(1<<1)
#define TEGRA_RTC_INTR_STATUS_SEC_ALARM0	(1<<0)

struct tegra_rtc_info {
<<<<<<< HEAD
	struct platform_device	*pdev;
	struct rtc_device	*rtc_dev;
	void __iomem		*rtc_base; /* NULL if not initialized. */
	struct clk		*clk;
	int			tegra_rtc_irq; /* alarm and periodic irq */
	spinlock_t		tegra_rtc_lock;
};

/* RTC hardware is busy when it is updating its values over AHB once
 * every eight 32kHz clocks (~250uS).
 * outside of these updates the CPU is free to write.
 * CPU is always free to read.
 */
static inline u32 tegra_rtc_check_busy(struct tegra_rtc_info *info)
{
	return readl(info->rtc_base + TEGRA_RTC_REG_BUSY) & 1;
}

/* Wait for hardware to be ready for writing.
 * This function tries to maximize the amount of time before the next update.
 * It does this by waiting for the RTC to become busy with its periodic update,
 * then returning once the RTC first becomes not busy.
 * This periodic update (where the seconds and milliseconds are copied to the
 * AHB side) occurs every eight 32kHz clocks (~250uS).
 * The behavior of this function allows us to make some assumptions without
 * introducing a race, because 250uS is plenty of time to read/write a value.
=======
	struct platform_device *pdev;
	struct rtc_device *rtc;
	void __iomem *base; /* NULL if not initialized */
	struct clk *clk;
	int irq; /* alarm and periodic IRQ */
	spinlock_t lock;
};

/*
 * RTC hardware is busy when it is updating its values over AHB once every
 * eight 32 kHz clocks (~250 us). Outside of these updates the CPU is free to
 * write. CPU is always free to read.
 */
static inline u32 tegra_rtc_check_busy(struct tegra_rtc_info *info)
{
	return readl(info->base + TEGRA_RTC_REG_BUSY) & 1;
}

/*
 * Wait for hardware to be ready for writing. This function tries to maximize
 * the amount of time before the next update. It does this by waiting for the
 * RTC to become busy with its periodic update, then returning once the RTC
 * first becomes not busy.
 *
 * This periodic update (where the seconds and milliseconds are copied to the
 * AHB side) occurs every eight 32 kHz clocks (~250 us). The behavior of this
 * function allows us to make some assumptions without introducing a race,
 * because 250 us is plenty of time to read/write a value.
>>>>>>> upstream/android-13
 */
static int tegra_rtc_wait_while_busy(struct device *dev)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD

	int retries = 500; /* ~490 us is the worst case, ~250 us is best. */

	/* first wait for the RTC to become busy. this is when it
	 * posts its updated seconds+msec registers to AHB side. */
	while (tegra_rtc_check_busy(info)) {
		if (!retries--)
			goto retry_failed;
=======
	int retries = 500; /* ~490 us is the worst case, ~250 us is best */

	/*
	 * First wait for the RTC to become busy. This is when it posts its
	 * updated seconds+msec registers to AHB side.
	 */
	while (tegra_rtc_check_busy(info)) {
		if (!retries--)
			goto retry_failed;

>>>>>>> upstream/android-13
		udelay(1);
	}

	/* now we have about 250 us to manipulate registers */
	return 0;

retry_failed:
<<<<<<< HEAD
	dev_err(dev, "write failed:retry count exceeded.\n");
=======
	dev_err(dev, "write failed: retry count exceeded\n");
>>>>>>> upstream/android-13
	return -ETIMEDOUT;
}

static int tegra_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long sec, msec;
	unsigned long sl_irq_flags;

	/* RTC hardware copies seconds to shadow seconds when a read
	 * of milliseconds occurs. use a lock to keep other threads out. */
	spin_lock_irqsave(&info->tegra_rtc_lock, sl_irq_flags);

	msec = readl(info->rtc_base + TEGRA_RTC_REG_MILLI_SECONDS);
	sec = readl(info->rtc_base + TEGRA_RTC_REG_SHADOW_SECONDS);

	spin_unlock_irqrestore(&info->tegra_rtc_lock, sl_irq_flags);

	rtc_time_to_tm(sec, tm);

	dev_vdbg(dev, "time read as %lu. %d/%d/%d %d:%02u:%02u\n",
		sec,
		tm->tm_mon + 1,
		tm->tm_mday,
		tm->tm_year + 1900,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec
	);
=======
	unsigned long flags;
	u32 sec;

	/*
	 * RTC hardware copies seconds to shadow seconds when a read of
	 * milliseconds occurs. use a lock to keep other threads out.
	 */
	spin_lock_irqsave(&info->lock, flags);

	readl(info->base + TEGRA_RTC_REG_MILLI_SECONDS);
	sec = readl(info->base + TEGRA_RTC_REG_SHADOW_SECONDS);

	spin_unlock_irqrestore(&info->lock, flags);

	rtc_time64_to_tm(sec, tm);

	dev_vdbg(dev, "time read as %u, %ptR\n", sec, tm);
>>>>>>> upstream/android-13

	return 0;
}

static int tegra_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long sec;
	int ret;

	/* convert tm to seconds. */
	rtc_tm_to_time(tm, &sec);

	dev_vdbg(dev, "time set to %lu. %d/%d/%d %d:%02u:%02u\n",
		sec,
		tm->tm_mon+1,
		tm->tm_mday,
		tm->tm_year+1900,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec
	);

	/* seconds only written if wait succeeded. */
	ret = tegra_rtc_wait_while_busy(dev);
	if (!ret)
		writel(sec, info->rtc_base + TEGRA_RTC_REG_SECONDS);

	dev_vdbg(dev, "time read back as %d\n",
		readl(info->rtc_base + TEGRA_RTC_REG_SECONDS));
=======
	u32 sec;
	int ret;

	/* convert tm to seconds */
	sec = rtc_tm_to_time64(tm);

	dev_vdbg(dev, "time set to %u, %ptR\n", sec, tm);

	/* seconds only written if wait succeeded */
	ret = tegra_rtc_wait_while_busy(dev);
	if (!ret)
		writel(sec, info->base + TEGRA_RTC_REG_SECONDS);

	dev_vdbg(dev, "time read back as %d\n",
		 readl(info->base + TEGRA_RTC_REG_SECONDS));
>>>>>>> upstream/android-13

	return ret;
}

static int tegra_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long sec;
	unsigned tmp;

	sec = readl(info->rtc_base + TEGRA_RTC_REG_SECONDS_ALARM0);

	if (sec == 0) {
		/* alarm is disabled. */
		alarm->enabled = 0;
	} else {
		/* alarm is enabled. */
		alarm->enabled = 1;
		rtc_time_to_tm(sec, &alarm->time);
	}

	tmp = readl(info->rtc_base + TEGRA_RTC_REG_INTR_STATUS);
	alarm->pending = (tmp & TEGRA_RTC_INTR_STATUS_SEC_ALARM0) != 0;
=======
	u32 sec, value;

	sec = readl(info->base + TEGRA_RTC_REG_SECONDS_ALARM0);

	if (sec == 0) {
		/* alarm is disabled */
		alarm->enabled = 0;
	} else {
		/* alarm is enabled */
		alarm->enabled = 1;
		rtc_time64_to_tm(sec, &alarm->time);
	}

	value = readl(info->base + TEGRA_RTC_REG_INTR_STATUS);
	alarm->pending = (value & TEGRA_RTC_INTR_STATUS_SEC_ALARM0) != 0;
>>>>>>> upstream/android-13

	return 0;
}

static int tegra_rtc_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned status;
	unsigned long sl_irq_flags;

	tegra_rtc_wait_while_busy(dev);
	spin_lock_irqsave(&info->tegra_rtc_lock, sl_irq_flags);

	/* read the original value, and OR in the flag. */
	status = readl(info->rtc_base + TEGRA_RTC_REG_INTR_MASK);
=======
	unsigned long flags;
	u32 status;

	tegra_rtc_wait_while_busy(dev);
	spin_lock_irqsave(&info->lock, flags);

	/* read the original value, and OR in the flag */
	status = readl(info->base + TEGRA_RTC_REG_INTR_MASK);
>>>>>>> upstream/android-13
	if (enabled)
		status |= TEGRA_RTC_INTR_MASK_SEC_ALARM0; /* set it */
	else
		status &= ~TEGRA_RTC_INTR_MASK_SEC_ALARM0; /* clear it */

<<<<<<< HEAD
	writel(status, info->rtc_base + TEGRA_RTC_REG_INTR_MASK);

	spin_unlock_irqrestore(&info->tegra_rtc_lock, sl_irq_flags);
=======
	writel(status, info->base + TEGRA_RTC_REG_INTR_MASK);

	spin_unlock_irqrestore(&info->lock, flags);
>>>>>>> upstream/android-13

	return 0;
}

static int tegra_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long sec;

	if (alarm->enabled)
		rtc_tm_to_time(&alarm->time, &sec);
=======
	u32 sec;

	if (alarm->enabled)
		sec = rtc_tm_to_time64(&alarm->time);
>>>>>>> upstream/android-13
	else
		sec = 0;

	tegra_rtc_wait_while_busy(dev);
<<<<<<< HEAD
	writel(sec, info->rtc_base + TEGRA_RTC_REG_SECONDS_ALARM0);
	dev_vdbg(dev, "alarm read back as %d\n",
		readl(info->rtc_base + TEGRA_RTC_REG_SECONDS_ALARM0));
=======
	writel(sec, info->base + TEGRA_RTC_REG_SECONDS_ALARM0);
	dev_vdbg(dev, "alarm read back as %d\n",
		 readl(info->base + TEGRA_RTC_REG_SECONDS_ALARM0));
>>>>>>> upstream/android-13

	/* if successfully written and alarm is enabled ... */
	if (sec) {
		tegra_rtc_alarm_irq_enable(dev, 1);
<<<<<<< HEAD

		dev_vdbg(dev, "alarm set as %lu. %d/%d/%d %d:%02u:%02u\n",
			sec,
			alarm->time.tm_mon+1,
			alarm->time.tm_mday,
			alarm->time.tm_year+1900,
			alarm->time.tm_hour,
			alarm->time.tm_min,
			alarm->time.tm_sec);
	} else {
		/* disable alarm if 0 or write error. */
=======
		dev_vdbg(dev, "alarm set as %u, %ptR\n", sec, &alarm->time);
	} else {
		/* disable alarm if 0 or write error */
>>>>>>> upstream/android-13
		dev_vdbg(dev, "alarm disabled\n");
		tegra_rtc_alarm_irq_enable(dev, 0);
	}

	return 0;
}

static int tegra_rtc_proc(struct device *dev, struct seq_file *seq)
{
	if (!dev || !dev->driver)
		return 0;

	seq_printf(seq, "name\t\t: %s\n", dev_name(dev));

	return 0;
}

static irqreturn_t tegra_rtc_irq_handler(int irq, void *data)
{
	struct device *dev = data;
	struct tegra_rtc_info *info = dev_get_drvdata(dev);
	unsigned long events = 0;
<<<<<<< HEAD
	unsigned status;
	unsigned long sl_irq_flags;

	status = readl(info->rtc_base + TEGRA_RTC_REG_INTR_STATUS);
	if (status) {
		/* clear the interrupt masks and status on any irq. */
		tegra_rtc_wait_while_busy(dev);
		spin_lock_irqsave(&info->tegra_rtc_lock, sl_irq_flags);
		writel(0, info->rtc_base + TEGRA_RTC_REG_INTR_MASK);
		writel(status, info->rtc_base + TEGRA_RTC_REG_INTR_STATUS);
		spin_unlock_irqrestore(&info->tegra_rtc_lock, sl_irq_flags);
	}

	/* check if Alarm */
	if ((status & TEGRA_RTC_INTR_STATUS_SEC_ALARM0))
		events |= RTC_IRQF | RTC_AF;

	/* check if Periodic */
	if ((status & TEGRA_RTC_INTR_STATUS_SEC_CDN_ALARM))
		events |= RTC_IRQF | RTC_PF;

	rtc_update_irq(info->rtc_dev, 1, events);
=======
	u32 status;

	status = readl(info->base + TEGRA_RTC_REG_INTR_STATUS);
	if (status) {
		/* clear the interrupt masks and status on any IRQ */
		tegra_rtc_wait_while_busy(dev);

		spin_lock(&info->lock);
		writel(0, info->base + TEGRA_RTC_REG_INTR_MASK);
		writel(status, info->base + TEGRA_RTC_REG_INTR_STATUS);
		spin_unlock(&info->lock);
	}

	/* check if alarm */
	if (status & TEGRA_RTC_INTR_STATUS_SEC_ALARM0)
		events |= RTC_IRQF | RTC_AF;

	/* check if periodic */
	if (status & TEGRA_RTC_INTR_STATUS_SEC_CDN_ALARM)
		events |= RTC_IRQF | RTC_PF;

	rtc_update_irq(info->rtc, 1, events);
>>>>>>> upstream/android-13

	return IRQ_HANDLED;
}

static const struct rtc_class_ops tegra_rtc_ops = {
<<<<<<< HEAD
	.read_time	= tegra_rtc_read_time,
	.set_time	= tegra_rtc_set_time,
	.read_alarm	= tegra_rtc_read_alarm,
	.set_alarm	= tegra_rtc_set_alarm,
	.proc		= tegra_rtc_proc,
=======
	.read_time = tegra_rtc_read_time,
	.set_time = tegra_rtc_set_time,
	.read_alarm = tegra_rtc_read_alarm,
	.set_alarm = tegra_rtc_set_alarm,
	.proc = tegra_rtc_proc,
>>>>>>> upstream/android-13
	.alarm_irq_enable = tegra_rtc_alarm_irq_enable,
};

static const struct of_device_id tegra_rtc_dt_match[] = {
	{ .compatible = "nvidia,tegra20-rtc", },
	{}
};
MODULE_DEVICE_TABLE(of, tegra_rtc_dt_match);

<<<<<<< HEAD
static int __init tegra_rtc_probe(struct platform_device *pdev)
{
	struct tegra_rtc_info *info;
	struct resource *res;
	int ret;

	info = devm_kzalloc(&pdev->dev, sizeof(struct tegra_rtc_info),
		GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	info->rtc_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(info->rtc_base))
		return PTR_ERR(info->rtc_base);

	info->tegra_rtc_irq = platform_get_irq(pdev, 0);
	if (info->tegra_rtc_irq <= 0)
		return -EBUSY;
=======
static int tegra_rtc_probe(struct platform_device *pdev)
{
	struct tegra_rtc_info *info;
	int ret;

	info = devm_kzalloc(&pdev->dev, sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(info->base))
		return PTR_ERR(info->base);

	ret = platform_get_irq(pdev, 0);
	if (ret <= 0)
		return ret;

	info->irq = ret;

	info->rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(info->rtc))
		return PTR_ERR(info->rtc);

	info->rtc->ops = &tegra_rtc_ops;
	info->rtc->range_max = U32_MAX;
>>>>>>> upstream/android-13

	info->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(info->clk))
		return PTR_ERR(info->clk);

	ret = clk_prepare_enable(info->clk);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	/* set context info. */
	info->pdev = pdev;
	spin_lock_init(&info->tegra_rtc_lock);

	platform_set_drvdata(pdev, info);

	/* clear out the hardware. */
	writel(0, info->rtc_base + TEGRA_RTC_REG_SECONDS_ALARM0);
	writel(0xffffffff, info->rtc_base + TEGRA_RTC_REG_INTR_STATUS);
	writel(0, info->rtc_base + TEGRA_RTC_REG_INTR_MASK);

	device_init_wakeup(&pdev->dev, 1);

	info->rtc_dev = devm_rtc_device_register(&pdev->dev,
				dev_name(&pdev->dev), &tegra_rtc_ops,
				THIS_MODULE);
	if (IS_ERR(info->rtc_dev)) {
		ret = PTR_ERR(info->rtc_dev);
		dev_err(&pdev->dev, "Unable to register device (err=%d).\n",
			ret);
		goto disable_clk;
	}

	ret = devm_request_irq(&pdev->dev, info->tegra_rtc_irq,
			tegra_rtc_irq_handler, IRQF_TRIGGER_HIGH,
			dev_name(&pdev->dev), &pdev->dev);
	if (ret) {
		dev_err(&pdev->dev,
			"Unable to request interrupt for device (err=%d).\n",
			ret);
		goto disable_clk;
	}
=======
	/* set context info */
	info->pdev = pdev;
	spin_lock_init(&info->lock);

	platform_set_drvdata(pdev, info);

	/* clear out the hardware */
	writel(0, info->base + TEGRA_RTC_REG_SECONDS_ALARM0);
	writel(0xffffffff, info->base + TEGRA_RTC_REG_INTR_STATUS);
	writel(0, info->base + TEGRA_RTC_REG_INTR_MASK);

	device_init_wakeup(&pdev->dev, 1);

	ret = devm_request_irq(&pdev->dev, info->irq, tegra_rtc_irq_handler,
			       IRQF_TRIGGER_HIGH, dev_name(&pdev->dev),
			       &pdev->dev);
	if (ret) {
		dev_err(&pdev->dev, "failed to request interrupt: %d\n", ret);
		goto disable_clk;
	}

	ret = devm_rtc_register_device(info->rtc);
	if (ret)
		goto disable_clk;
>>>>>>> upstream/android-13

	dev_notice(&pdev->dev, "Tegra internal Real Time Clock\n");

	return 0;

disable_clk:
	clk_disable_unprepare(info->clk);
	return ret;
}

static int tegra_rtc_remove(struct platform_device *pdev)
{
	struct tegra_rtc_info *info = platform_get_drvdata(pdev);

	clk_disable_unprepare(info->clk);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int tegra_rtc_suspend(struct device *dev)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);

	tegra_rtc_wait_while_busy(dev);

<<<<<<< HEAD
	/* only use ALARM0 as a wake source. */
	writel(0xffffffff, info->rtc_base + TEGRA_RTC_REG_INTR_STATUS);
	writel(TEGRA_RTC_INTR_STATUS_SEC_ALARM0,
		info->rtc_base + TEGRA_RTC_REG_INTR_MASK);

	dev_vdbg(dev, "alarm sec = %d\n",
		readl(info->rtc_base + TEGRA_RTC_REG_SECONDS_ALARM0));

	dev_vdbg(dev, "Suspend (device_may_wakeup=%d) irq:%d\n",
		device_may_wakeup(dev), info->tegra_rtc_irq);

	/* leave the alarms on as a wake source. */
	if (device_may_wakeup(dev))
		enable_irq_wake(info->tegra_rtc_irq);
=======
	/* only use ALARM0 as a wake source */
	writel(0xffffffff, info->base + TEGRA_RTC_REG_INTR_STATUS);
	writel(TEGRA_RTC_INTR_STATUS_SEC_ALARM0,
	       info->base + TEGRA_RTC_REG_INTR_MASK);

	dev_vdbg(dev, "alarm sec = %d\n",
		 readl(info->base + TEGRA_RTC_REG_SECONDS_ALARM0));

	dev_vdbg(dev, "Suspend (device_may_wakeup=%d) IRQ:%d\n",
		 device_may_wakeup(dev), info->irq);

	/* leave the alarms on as a wake source */
	if (device_may_wakeup(dev))
		enable_irq_wake(info->irq);
>>>>>>> upstream/android-13

	return 0;
}

static int tegra_rtc_resume(struct device *dev)
{
	struct tegra_rtc_info *info = dev_get_drvdata(dev);

	dev_vdbg(dev, "Resume (device_may_wakeup=%d)\n",
<<<<<<< HEAD
		device_may_wakeup(dev));
	/* alarms were left on as a wake source, turn them off. */
	if (device_may_wakeup(dev))
		disable_irq_wake(info->tegra_rtc_irq);
=======
		 device_may_wakeup(dev));

	/* alarms were left on as a wake source, turn them off */
	if (device_may_wakeup(dev))
		disable_irq_wake(info->irq);
>>>>>>> upstream/android-13

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(tegra_rtc_pm_ops, tegra_rtc_suspend, tegra_rtc_resume);

static void tegra_rtc_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	dev_vdbg(&pdev->dev, "disabling interrupts.\n");
	tegra_rtc_alarm_irq_enable(&pdev->dev, 0);
}

MODULE_ALIAS("platform:tegra_rtc");
static struct platform_driver tegra_rtc_driver = {
	.remove		= tegra_rtc_remove,
	.shutdown	= tegra_rtc_shutdown,
	.driver		= {
		.name	= "tegra_rtc",
		.of_match_table = tegra_rtc_dt_match,
		.pm	= &tegra_rtc_pm_ops,
	},
};

module_platform_driver_probe(tegra_rtc_driver, tegra_rtc_probe);
=======
	dev_vdbg(&pdev->dev, "disabling interrupts\n");
	tegra_rtc_alarm_irq_enable(&pdev->dev, 0);
}

static struct platform_driver tegra_rtc_driver = {
	.probe = tegra_rtc_probe,
	.remove = tegra_rtc_remove,
	.shutdown = tegra_rtc_shutdown,
	.driver = {
		.name = "tegra_rtc",
		.of_match_table = tegra_rtc_dt_match,
		.pm = &tegra_rtc_pm_ops,
	},
};
module_platform_driver(tegra_rtc_driver);
>>>>>>> upstream/android-13

MODULE_AUTHOR("Jon Mayo <jmayo@nvidia.com>");
MODULE_DESCRIPTION("driver for Tegra internal RTC");
MODULE_LICENSE("GPL");
