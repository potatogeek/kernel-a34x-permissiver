// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (C) 2011-2012 Freescale Semiconductor, Inc.

#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
<<<<<<< HEAD
#include <linux/of_device.h>
#include <linux/platform_device.h>
=======
#include <linux/platform_device.h>
#include <linux/pm_wakeirq.h>
>>>>>>> upstream/android-13
#include <linux/rtc.h>
#include <linux/clk.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#define SNVS_LPREGISTER_OFFSET	0x34

/* These register offsets are relative to LP (Low Power) range */
#define SNVS_LPCR		0x04
#define SNVS_LPSR		0x18
#define SNVS_LPSRTCMR		0x1c
#define SNVS_LPSRTCLR		0x20
#define SNVS_LPTAR		0x24
#define SNVS_LPPGDR		0x30

#define SNVS_LPCR_SRTC_ENV	(1 << 0)
#define SNVS_LPCR_LPTA_EN	(1 << 1)
#define SNVS_LPCR_LPWUI_EN	(1 << 3)
#define SNVS_LPSR_LPTA		(1 << 0)

#define SNVS_LPPGDR_INIT	0x41736166
#define CNTR_TO_SECS_SH		15

struct snvs_rtc_data {
	struct rtc_device *rtc;
	struct regmap *regmap;
	int offset;
	int irq;
	struct clk *clk;
};

/* Read 64 bit timer register, which could be in inconsistent state */
static u64 rtc_read_lpsrt(struct snvs_rtc_data *data)
{
	u32 msb, lsb;

	regmap_read(data->regmap, data->offset + SNVS_LPSRTCMR, &msb);
	regmap_read(data->regmap, data->offset + SNVS_LPSRTCLR, &lsb);
	return (u64)msb << 32 | lsb;
}

/* Read the secure real time counter, taking care to deal with the cases of the
 * counter updating while being read.
 */
static u32 rtc_read_lp_counter(struct snvs_rtc_data *data)
{
	u64 read1, read2;
	unsigned int timeout = 100;

	/* As expected, the registers might update between the read of the LSB
	 * reg and the MSB reg.  It's also possible that one register might be
	 * in partially modified state as well.
	 */
	read1 = rtc_read_lpsrt(data);
	do {
		read2 = read1;
		read1 = rtc_read_lpsrt(data);
	} while (read1 != read2 && --timeout);
	if (!timeout)
		dev_err(&data->rtc->dev, "Timeout trying to get valid LPSRT Counter read\n");

	/* Convert 47-bit counter to 32-bit raw second count */
	return (u32) (read1 >> CNTR_TO_SECS_SH);
}

/* Just read the lsb from the counter, dealing with inconsistent state */
static int rtc_read_lp_counter_lsb(struct snvs_rtc_data *data, u32 *lsb)
{
	u32 count1, count2;
	unsigned int timeout = 100;

	regmap_read(data->regmap, data->offset + SNVS_LPSRTCLR, &count1);
	do {
		count2 = count1;
		regmap_read(data->regmap, data->offset + SNVS_LPSRTCLR, &count1);
	} while (count1 != count2 && --timeout);
	if (!timeout) {
		dev_err(&data->rtc->dev, "Timeout trying to get valid LPSRT Counter read\n");
		return -ETIMEDOUT;
	}

	*lsb = count1;
	return 0;
}

static int rtc_write_sync_lp(struct snvs_rtc_data *data)
{
	u32 count1, count2;
	u32 elapsed;
	unsigned int timeout = 1000;
	int ret;

	ret = rtc_read_lp_counter_lsb(data, &count1);
	if (ret)
		return ret;

	/* Wait for 3 CKIL cycles, about 61.0-91.5 µs */
	do {
		ret = rtc_read_lp_counter_lsb(data, &count2);
		if (ret)
			return ret;
		elapsed = count2 - count1; /* wrap around _is_ handled! */
	} while (elapsed < 3 && --timeout);
	if (!timeout) {
		dev_err(&data->rtc->dev, "Timeout waiting for LPSRT Counter to change\n");
		return -ETIMEDOUT;
	}
	return 0;
}

static int snvs_rtc_enable(struct snvs_rtc_data *data, bool enable)
{
	int timeout = 1000;
	u32 lpcr;

	regmap_update_bits(data->regmap, data->offset + SNVS_LPCR, SNVS_LPCR_SRTC_ENV,
			   enable ? SNVS_LPCR_SRTC_ENV : 0);

	while (--timeout) {
		regmap_read(data->regmap, data->offset + SNVS_LPCR, &lpcr);

		if (enable) {
			if (lpcr & SNVS_LPCR_SRTC_ENV)
				break;
		} else {
			if (!(lpcr & SNVS_LPCR_SRTC_ENV))
				break;
		}
	}

	if (!timeout)
		return -ETIMEDOUT;

	return 0;
}

static int snvs_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long time = rtc_read_lp_counter(data);

	rtc_time_to_tm(time, tm);
=======
	unsigned long time;
	int ret;

	ret = clk_enable(data->clk);
	if (ret)
		return ret;

	time = rtc_read_lp_counter(data);
	rtc_time64_to_tm(time, tm);

	clk_disable(data->clk);
>>>>>>> upstream/android-13

	return 0;
}

static int snvs_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long time;
	int ret;

	rtc_tm_to_time(tm, &time);
=======
	unsigned long time = rtc_tm_to_time64(tm);
	int ret;

	ret = clk_enable(data->clk);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	/* Disable RTC first */
	ret = snvs_rtc_enable(data, false);
	if (ret)
		return ret;

	/* Write 32-bit time to 47-bit timer, leaving 15 LSBs blank */
	regmap_write(data->regmap, data->offset + SNVS_LPSRTCLR, time << CNTR_TO_SECS_SH);
	regmap_write(data->regmap, data->offset + SNVS_LPSRTCMR, time >> (32 - CNTR_TO_SECS_SH));

	/* Enable RTC again */
	ret = snvs_rtc_enable(data, true);

<<<<<<< HEAD
=======
	clk_disable(data->clk);

>>>>>>> upstream/android-13
	return ret;
}

static int snvs_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);
	u32 lptar, lpsr;
<<<<<<< HEAD

	regmap_read(data->regmap, data->offset + SNVS_LPTAR, &lptar);
	rtc_time_to_tm(lptar, &alrm->time);
=======
	int ret;

	ret = clk_enable(data->clk);
	if (ret)
		return ret;

	regmap_read(data->regmap, data->offset + SNVS_LPTAR, &lptar);
	rtc_time64_to_tm(lptar, &alrm->time);
>>>>>>> upstream/android-13

	regmap_read(data->regmap, data->offset + SNVS_LPSR, &lpsr);
	alrm->pending = (lpsr & SNVS_LPSR_LPTA) ? 1 : 0;

<<<<<<< HEAD
=======
	clk_disable(data->clk);

>>>>>>> upstream/android-13
	return 0;
}

static int snvs_rtc_alarm_irq_enable(struct device *dev, unsigned int enable)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
=======
	int ret;

	ret = clk_enable(data->clk);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	regmap_update_bits(data->regmap, data->offset + SNVS_LPCR,
			   (SNVS_LPCR_LPTA_EN | SNVS_LPCR_LPWUI_EN),
			   enable ? (SNVS_LPCR_LPTA_EN | SNVS_LPCR_LPWUI_EN) : 0);

<<<<<<< HEAD
	return rtc_write_sync_lp(data);
=======
	ret = rtc_write_sync_lp(data);

	clk_disable(data->clk);

	return ret;
>>>>>>> upstream/android-13
}

static int snvs_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct rtc_time *alrm_tm = &alrm->time;
	unsigned long time;
	int ret;

	rtc_tm_to_time(alrm_tm, &time);
=======
	unsigned long time = rtc_tm_to_time64(&alrm->time);
	int ret;

	ret = clk_enable(data->clk);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	regmap_update_bits(data->regmap, data->offset + SNVS_LPCR, SNVS_LPCR_LPTA_EN, 0);
	ret = rtc_write_sync_lp(data);
	if (ret)
		return ret;
	regmap_write(data->regmap, data->offset + SNVS_LPTAR, time);

	/* Clear alarm interrupt status bit */
	regmap_write(data->regmap, data->offset + SNVS_LPSR, SNVS_LPSR_LPTA);

<<<<<<< HEAD
=======
	clk_disable(data->clk);

>>>>>>> upstream/android-13
	return snvs_rtc_alarm_irq_enable(dev, alrm->enabled);
}

static const struct rtc_class_ops snvs_rtc_ops = {
	.read_time = snvs_rtc_read_time,
	.set_time = snvs_rtc_set_time,
	.read_alarm = snvs_rtc_read_alarm,
	.set_alarm = snvs_rtc_set_alarm,
	.alarm_irq_enable = snvs_rtc_alarm_irq_enable,
};

static irqreturn_t snvs_rtc_irq_handler(int irq, void *dev_id)
{
	struct device *dev = dev_id;
	struct snvs_rtc_data *data = dev_get_drvdata(dev);
	u32 lpsr;
	u32 events = 0;

<<<<<<< HEAD
=======
	clk_enable(data->clk);

>>>>>>> upstream/android-13
	regmap_read(data->regmap, data->offset + SNVS_LPSR, &lpsr);

	if (lpsr & SNVS_LPSR_LPTA) {
		events |= (RTC_AF | RTC_IRQF);

		/* RTC alarm should be one-shot */
		snvs_rtc_alarm_irq_enable(dev, 0);

		rtc_update_irq(data->rtc, 1, events);
	}

	/* clear interrupt status */
	regmap_write(data->regmap, data->offset + SNVS_LPSR, lpsr);

<<<<<<< HEAD
=======
	clk_disable(data->clk);

>>>>>>> upstream/android-13
	return events ? IRQ_HANDLED : IRQ_NONE;
}

static const struct regmap_config snvs_rtc_config = {
	.reg_bits = 32,
	.val_bits = 32,
	.reg_stride = 4,
};

<<<<<<< HEAD
static int snvs_rtc_probe(struct platform_device *pdev)
{
	struct snvs_rtc_data *data;
	struct resource *res;
=======
static void snvs_rtc_action(void *data)
{
	clk_disable_unprepare(data);
}

static int snvs_rtc_probe(struct platform_device *pdev)
{
	struct snvs_rtc_data *data;
>>>>>>> upstream/android-13
	int ret;
	void __iomem *mmio;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(data->rtc))
		return PTR_ERR(data->rtc);

	data->regmap = syscon_regmap_lookup_by_phandle(pdev->dev.of_node, "regmap");

	if (IS_ERR(data->regmap)) {
		dev_warn(&pdev->dev, "snvs rtc: you use old dts file, please update it\n");
<<<<<<< HEAD
		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

		mmio = devm_ioremap_resource(&pdev->dev, res);
=======

		mmio = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
		if (IS_ERR(mmio))
			return PTR_ERR(mmio);

		data->regmap = devm_regmap_init_mmio(&pdev->dev, mmio, &snvs_rtc_config);
	} else {
		data->offset = SNVS_LPREGISTER_OFFSET;
		of_property_read_u32(pdev->dev.of_node, "offset", &data->offset);
	}

	if (IS_ERR(data->regmap)) {
		dev_err(&pdev->dev, "Can't find snvs syscon\n");
		return -ENODEV;
	}

	data->irq = platform_get_irq(pdev, 0);
	if (data->irq < 0)
		return data->irq;

	data->clk = devm_clk_get(&pdev->dev, "snvs-rtc");
	if (IS_ERR(data->clk)) {
		data->clk = NULL;
	} else {
		ret = clk_prepare_enable(data->clk);
		if (ret) {
			dev_err(&pdev->dev,
				"Could not prepare or enable the snvs clock\n");
			return ret;
		}
	}

<<<<<<< HEAD
=======
	ret = devm_add_action_or_reset(&pdev->dev, snvs_rtc_action, data->clk);
	if (ret)
		return ret;

>>>>>>> upstream/android-13
	platform_set_drvdata(pdev, data);

	/* Initialize glitch detect */
	regmap_write(data->regmap, data->offset + SNVS_LPPGDR, SNVS_LPPGDR_INIT);

	/* Clear interrupt status */
	regmap_write(data->regmap, data->offset + SNVS_LPSR, 0xffffffff);

	/* Enable RTC */
	ret = snvs_rtc_enable(data, true);
	if (ret) {
		dev_err(&pdev->dev, "failed to enable rtc %d\n", ret);
<<<<<<< HEAD
		goto error_rtc_device_register;
	}

	device_init_wakeup(&pdev->dev, true);
=======
		return ret;
	}

	device_init_wakeup(&pdev->dev, true);
	ret = dev_pm_set_wake_irq(&pdev->dev, data->irq);
	if (ret)
		dev_err(&pdev->dev, "failed to enable irq wake\n");
>>>>>>> upstream/android-13

	ret = devm_request_irq(&pdev->dev, data->irq, snvs_rtc_irq_handler,
			       IRQF_SHARED, "rtc alarm", &pdev->dev);
	if (ret) {
		dev_err(&pdev->dev, "failed to request irq %d: %d\n",
			data->irq, ret);
<<<<<<< HEAD
		goto error_rtc_device_register;
	}

	data->rtc->ops = &snvs_rtc_ops;
	ret = rtc_register_device(data->rtc);
	if (ret) {
		dev_err(&pdev->dev, "failed to register rtc: %d\n", ret);
		goto error_rtc_device_register;
	}

	return 0;

error_rtc_device_register:
	if (data->clk)
		clk_disable_unprepare(data->clk);

	return ret;
}

#ifdef CONFIG_PM_SLEEP
static int snvs_rtc_suspend(struct device *dev)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		return enable_irq_wake(data->irq);
=======
		return ret;
	}

	data->rtc->ops = &snvs_rtc_ops;
	data->rtc->range_max = U32_MAX;

	return devm_rtc_register_device(data->rtc);
}

static int __maybe_unused snvs_rtc_suspend_noirq(struct device *dev)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);

	clk_disable(data->clk);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int snvs_rtc_suspend_noirq(struct device *dev)
=======
static int __maybe_unused snvs_rtc_resume_noirq(struct device *dev)
>>>>>>> upstream/android-13
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);

	if (data->clk)
<<<<<<< HEAD
		clk_disable_unprepare(data->clk);

	return 0;
}

static int snvs_rtc_resume(struct device *dev)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		return disable_irq_wake(data->irq);

	return 0;
}

static int snvs_rtc_resume_noirq(struct device *dev)
{
	struct snvs_rtc_data *data = dev_get_drvdata(dev);

	if (data->clk)
		return clk_prepare_enable(data->clk);
=======
		return clk_enable(data->clk);
>>>>>>> upstream/android-13

	return 0;
}

static const struct dev_pm_ops snvs_rtc_pm_ops = {
<<<<<<< HEAD
	.suspend = snvs_rtc_suspend,
	.suspend_noirq = snvs_rtc_suspend_noirq,
	.resume = snvs_rtc_resume,
	.resume_noirq = snvs_rtc_resume_noirq,
};

#define SNVS_RTC_PM_OPS	(&snvs_rtc_pm_ops)

#else

#define SNVS_RTC_PM_OPS	NULL

#endif

=======
	SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(snvs_rtc_suspend_noirq, snvs_rtc_resume_noirq)
};

>>>>>>> upstream/android-13
static const struct of_device_id snvs_dt_ids[] = {
	{ .compatible = "fsl,sec-v4.0-mon-rtc-lp", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, snvs_dt_ids);

static struct platform_driver snvs_rtc_driver = {
	.driver = {
		.name	= "snvs_rtc",
<<<<<<< HEAD
		.pm	= SNVS_RTC_PM_OPS,
=======
		.pm	= &snvs_rtc_pm_ops,
>>>>>>> upstream/android-13
		.of_match_table = snvs_dt_ids,
	},
	.probe		= snvs_rtc_probe,
};
module_platform_driver(snvs_rtc_driver);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("Freescale SNVS RTC Driver");
MODULE_LICENSE("GPL");
