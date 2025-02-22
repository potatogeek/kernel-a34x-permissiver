<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * STMicroelectronics STMPE811 Touchscreen Driver
 *
 * (C) 2010 Luotao Fu <l.fu@pengutronix.de>
 * All rights reserved.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/input.h>
<<<<<<< HEAD
=======
#include <linux/input/touchscreen.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/workqueue.h>

#include <linux/mfd/stmpe.h>

/* Register layouts and functionalities are identical on all stmpexxx variants
 * with touchscreen controller
 */
#define STMPE_REG_INT_STA		0x0B
<<<<<<< HEAD
#define STMPE_REG_ADC_CTRL1		0x20
#define STMPE_REG_ADC_CTRL2		0x21
=======
>>>>>>> upstream/android-13
#define STMPE_REG_TSC_CTRL		0x40
#define STMPE_REG_TSC_CFG		0x41
#define STMPE_REG_FIFO_TH		0x4A
#define STMPE_REG_FIFO_STA		0x4B
#define STMPE_REG_FIFO_SIZE		0x4C
#define STMPE_REG_TSC_DATA_XYZ		0x52
#define STMPE_REG_TSC_FRACTION_Z	0x56
#define STMPE_REG_TSC_I_DRIVE		0x58

#define OP_MOD_XYZ			0

#define STMPE_TSC_CTRL_TSC_EN		(1<<0)

#define STMPE_FIFO_STA_RESET		(1<<0)

#define STMPE_IRQ_TOUCH_DET		0

<<<<<<< HEAD
#define SAMPLE_TIME(x)			((x & 0xf) << 4)
#define MOD_12B(x)			((x & 0x1) << 3)
#define REF_SEL(x)			((x & 0x1) << 1)
#define ADC_FREQ(x)			(x & 0x3)
#define AVE_CTRL(x)			((x & 0x3) << 6)
#define DET_DELAY(x)			((x & 0x7) << 3)
#define SETTLING(x)			(x & 0x7)
#define FRACTION_Z(x)			(x & 0x7)
#define I_DRIVE(x)			(x & 0x1)
#define OP_MODE(x)			((x & 0x7) << 1)

=======
>>>>>>> upstream/android-13
#define STMPE_TS_NAME			"stmpe-ts"
#define XY_MASK				0xfff

/**
 * struct stmpe_touch - stmpe811 touch screen controller state
 * @stmpe: pointer back to STMPE MFD container
 * @idev: registered input device
 * @work: a work item used to scan the device
 * @dev: a pointer back to the MFD cell struct device*
<<<<<<< HEAD
 * @sample_time: ADC converstion time in number of clock.
 * (0 -> 36 clocks, 1 -> 44 clocks, 2 -> 56 clocks, 3 -> 64 clocks,
 * 4 -> 80 clocks, 5 -> 96 clocks, 6 -> 144 clocks),
 * recommended is 4.
 * @mod_12b: ADC Bit mode (0 -> 10bit ADC, 1 -> 12bit ADC)
 * @ref_sel: ADC reference source
 * (0 -> internal reference, 1 -> external reference)
 * @adc_freq: ADC Clock speed
 * (0 -> 1.625 MHz, 1 -> 3.25 MHz, 2 || 3 -> 6.5 MHz)
=======
 * @prop: Touchscreen properties
>>>>>>> upstream/android-13
 * @ave_ctrl: Sample average control
 * (0 -> 1 sample, 1 -> 2 samples, 2 -> 4 samples, 3 -> 8 samples)
 * @touch_det_delay: Touch detect interrupt delay
 * (0 -> 10 us, 1 -> 50 us, 2 -> 100 us, 3 -> 500 us,
 * 4-> 1 ms, 5 -> 5 ms, 6 -> 10 ms, 7 -> 50 ms)
 * recommended is 3
 * @settling: Panel driver settling time
 * (0 -> 10 us, 1 -> 100 us, 2 -> 500 us, 3 -> 1 ms,
 * 4 -> 5 ms, 5 -> 10 ms, 6 for 50 ms, 7 -> 100 ms)
 * recommended is 2
 * @fraction_z: Length of the fractional part in z
 * (fraction_z ([0..7]) = Count of the fractional part)
 * recommended is 7
 * @i_drive: current limit value of the touchscreen drivers
 * (0 -> 20 mA typical 35 mA max, 1 -> 50 mA typical 80 mA max)
 */
struct stmpe_touch {
	struct stmpe *stmpe;
	struct input_dev *idev;
	struct delayed_work work;
	struct device *dev;
<<<<<<< HEAD
	u8 sample_time;
	u8 mod_12b;
	u8 ref_sel;
	u8 adc_freq;
=======
	struct touchscreen_properties prop;
>>>>>>> upstream/android-13
	u8 ave_ctrl;
	u8 touch_det_delay;
	u8 settling;
	u8 fraction_z;
	u8 i_drive;
};

static int __stmpe_reset_fifo(struct stmpe *stmpe)
{
	int ret;

	ret = stmpe_set_bits(stmpe, STMPE_REG_FIFO_STA,
			STMPE_FIFO_STA_RESET, STMPE_FIFO_STA_RESET);
	if (ret)
		return ret;

	return stmpe_set_bits(stmpe, STMPE_REG_FIFO_STA,
			STMPE_FIFO_STA_RESET, 0);
}

static void stmpe_work(struct work_struct *work)
{
	int int_sta;
	u32 timeout = 40;

	struct stmpe_touch *ts =
	    container_of(work, struct stmpe_touch, work.work);

	int_sta = stmpe_reg_read(ts->stmpe, STMPE_REG_INT_STA);

	/*
	 * touch_det sometimes get desasserted or just get stuck. This appears
	 * to be a silicon bug, We still have to clearify this with the
	 * manufacture. As a workaround We release the key anyway if the
	 * touch_det keeps coming in after 4ms, while the FIFO contains no value
	 * during the whole time.
	 */
	while ((int_sta & (1 << STMPE_IRQ_TOUCH_DET)) && (timeout > 0)) {
		timeout--;
		int_sta = stmpe_reg_read(ts->stmpe, STMPE_REG_INT_STA);
		udelay(100);
	}

	/* reset the FIFO before we report release event */
	__stmpe_reset_fifo(ts->stmpe);

	input_report_abs(ts->idev, ABS_PRESSURE, 0);
	input_report_key(ts->idev, BTN_TOUCH, 0);
	input_sync(ts->idev);
}

static irqreturn_t stmpe_ts_handler(int irq, void *data)
{
	u8 data_set[4];
	int x, y, z;
	struct stmpe_touch *ts = data;

	/*
	 * Cancel scheduled polling for release if we have new value
	 * available. Wait if the polling is already running.
	 */
	cancel_delayed_work_sync(&ts->work);

	/*
	 * The FIFO sometimes just crashes and stops generating interrupts. This
	 * appears to be a silicon bug. We still have to clearify this with
	 * the manufacture. As a workaround we disable the TSC while we are
	 * collecting data and flush the FIFO after reading
	 */
	stmpe_set_bits(ts->stmpe, STMPE_REG_TSC_CTRL,
				STMPE_TSC_CTRL_TSC_EN, 0);

	stmpe_block_read(ts->stmpe, STMPE_REG_TSC_DATA_XYZ, 4, data_set);

	x = (data_set[0] << 4) | (data_set[1] >> 4);
	y = ((data_set[1] & 0xf) << 8) | data_set[2];
	z = data_set[3];

<<<<<<< HEAD
	input_report_abs(ts->idev, ABS_X, x);
	input_report_abs(ts->idev, ABS_Y, y);
=======
	touchscreen_report_pos(ts->idev, &ts->prop, x, y, false);
>>>>>>> upstream/android-13
	input_report_abs(ts->idev, ABS_PRESSURE, z);
	input_report_key(ts->idev, BTN_TOUCH, 1);
	input_sync(ts->idev);

       /* flush the FIFO after we have read out our values. */
	__stmpe_reset_fifo(ts->stmpe);

	/* reenable the tsc */
	stmpe_set_bits(ts->stmpe, STMPE_REG_TSC_CTRL,
			STMPE_TSC_CTRL_TSC_EN, STMPE_TSC_CTRL_TSC_EN);

	/* start polling for touch_det to detect release */
	schedule_delayed_work(&ts->work, msecs_to_jiffies(50));

	return IRQ_HANDLED;
}

static int stmpe_init_hw(struct stmpe_touch *ts)
{
	int ret;
<<<<<<< HEAD
	u8 adc_ctrl1, adc_ctrl1_mask, tsc_cfg, tsc_cfg_mask;
=======
	u8 tsc_cfg, tsc_cfg_mask;
>>>>>>> upstream/android-13
	struct stmpe *stmpe = ts->stmpe;
	struct device *dev = ts->dev;

	ret = stmpe_enable(stmpe, STMPE_BLOCK_TOUCHSCREEN | STMPE_BLOCK_ADC);
	if (ret) {
		dev_err(dev, "Could not enable clock for ADC and TS\n");
		return ret;
	}

<<<<<<< HEAD
	adc_ctrl1 = SAMPLE_TIME(ts->sample_time) | MOD_12B(ts->mod_12b) |
		REF_SEL(ts->ref_sel);
	adc_ctrl1_mask = SAMPLE_TIME(0xff) | MOD_12B(0xff) | REF_SEL(0xff);

	ret = stmpe_set_bits(stmpe, STMPE_REG_ADC_CTRL1,
			adc_ctrl1_mask, adc_ctrl1);
	if (ret) {
		dev_err(dev, "Could not setup ADC\n");
		return ret;
	}

	ret = stmpe_set_bits(stmpe, STMPE_REG_ADC_CTRL2,
			ADC_FREQ(0xff), ADC_FREQ(ts->adc_freq));
	if (ret) {
		dev_err(dev, "Could not setup ADC\n");
		return ret;
	}

	tsc_cfg = AVE_CTRL(ts->ave_ctrl) | DET_DELAY(ts->touch_det_delay) |
			SETTLING(ts->settling);
	tsc_cfg_mask = AVE_CTRL(0xff) | DET_DELAY(0xff) | SETTLING(0xff);
=======
	ret = stmpe811_adc_common_init(stmpe);
	if (ret) {
		stmpe_disable(stmpe, STMPE_BLOCK_TOUCHSCREEN | STMPE_BLOCK_ADC);
		return ret;
	}

	tsc_cfg = STMPE_AVE_CTRL(ts->ave_ctrl) |
		  STMPE_DET_DELAY(ts->touch_det_delay) |
		  STMPE_SETTLING(ts->settling);
	tsc_cfg_mask = STMPE_AVE_CTRL(0xff) | STMPE_DET_DELAY(0xff) |
		       STMPE_SETTLING(0xff);
>>>>>>> upstream/android-13

	ret = stmpe_set_bits(stmpe, STMPE_REG_TSC_CFG, tsc_cfg_mask, tsc_cfg);
	if (ret) {
		dev_err(dev, "Could not config touch\n");
		return ret;
	}

	ret = stmpe_set_bits(stmpe, STMPE_REG_TSC_FRACTION_Z,
<<<<<<< HEAD
			FRACTION_Z(0xff), FRACTION_Z(ts->fraction_z));
=======
			STMPE_FRACTION_Z(0xff), STMPE_FRACTION_Z(ts->fraction_z));
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "Could not config touch\n");
		return ret;
	}

	ret = stmpe_set_bits(stmpe, STMPE_REG_TSC_I_DRIVE,
<<<<<<< HEAD
			I_DRIVE(0xff), I_DRIVE(ts->i_drive));
=======
			STMPE_I_DRIVE(0xff), STMPE_I_DRIVE(ts->i_drive));
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "Could not config touch\n");
		return ret;
	}

	/* set FIFO to 1 for single point reading */
	ret = stmpe_reg_write(stmpe, STMPE_REG_FIFO_TH, 1);
	if (ret) {
		dev_err(dev, "Could not set FIFO\n");
		return ret;
	}

	ret = stmpe_set_bits(stmpe, STMPE_REG_TSC_CTRL,
<<<<<<< HEAD
			OP_MODE(0xff), OP_MODE(OP_MOD_XYZ));
=======
			STMPE_OP_MODE(0xff), STMPE_OP_MODE(OP_MOD_XYZ));
>>>>>>> upstream/android-13
	if (ret) {
		dev_err(dev, "Could not set mode\n");
		return ret;
	}

	return 0;
}

static int stmpe_ts_open(struct input_dev *dev)
{
	struct stmpe_touch *ts = input_get_drvdata(dev);
	int ret = 0;

	ret = __stmpe_reset_fifo(ts->stmpe);
	if (ret)
		return ret;

	return stmpe_set_bits(ts->stmpe, STMPE_REG_TSC_CTRL,
			STMPE_TSC_CTRL_TSC_EN, STMPE_TSC_CTRL_TSC_EN);
}

static void stmpe_ts_close(struct input_dev *dev)
{
	struct stmpe_touch *ts = input_get_drvdata(dev);

	cancel_delayed_work_sync(&ts->work);

	stmpe_set_bits(ts->stmpe, STMPE_REG_TSC_CTRL,
			STMPE_TSC_CTRL_TSC_EN, 0);
}

static void stmpe_ts_get_platform_info(struct platform_device *pdev,
					struct stmpe_touch *ts)
{
	struct device_node *np = pdev->dev.of_node;
	u32 val;

	if (np) {
		if (!of_property_read_u32(np, "st,sample-time", &val))
<<<<<<< HEAD
			ts->sample_time = val;
		if (!of_property_read_u32(np, "st,mod-12b", &val))
			ts->mod_12b = val;
		if (!of_property_read_u32(np, "st,ref-sel", &val))
			ts->ref_sel = val;
		if (!of_property_read_u32(np, "st,adc-freq", &val))
			ts->adc_freq = val;
=======
			ts->stmpe->sample_time = val;
		if (!of_property_read_u32(np, "st,mod-12b", &val))
			ts->stmpe->mod_12b = val;
		if (!of_property_read_u32(np, "st,ref-sel", &val))
			ts->stmpe->ref_sel = val;
		if (!of_property_read_u32(np, "st,adc-freq", &val))
			ts->stmpe->adc_freq = val;
>>>>>>> upstream/android-13
		if (!of_property_read_u32(np, "st,ave-ctrl", &val))
			ts->ave_ctrl = val;
		if (!of_property_read_u32(np, "st,touch-det-delay", &val))
			ts->touch_det_delay = val;
		if (!of_property_read_u32(np, "st,settling", &val))
			ts->settling = val;
		if (!of_property_read_u32(np, "st,fraction-z", &val))
			ts->fraction_z = val;
		if (!of_property_read_u32(np, "st,i-drive", &val))
			ts->i_drive = val;
	}
}

static int stmpe_input_probe(struct platform_device *pdev)
{
	struct stmpe *stmpe = dev_get_drvdata(pdev->dev.parent);
	struct stmpe_touch *ts;
	struct input_dev *idev;
	int error;
	int ts_irq;

	ts_irq = platform_get_irq_byname(pdev, "FIFO_TH");
	if (ts_irq < 0)
		return ts_irq;

	ts = devm_kzalloc(&pdev->dev, sizeof(*ts), GFP_KERNEL);
	if (!ts)
		return -ENOMEM;

	idev = devm_input_allocate_device(&pdev->dev);
	if (!idev)
		return -ENOMEM;

	platform_set_drvdata(pdev, ts);
	ts->stmpe = stmpe;
	ts->idev = idev;
	ts->dev = &pdev->dev;

	stmpe_ts_get_platform_info(pdev, ts);

	INIT_DELAYED_WORK(&ts->work, stmpe_work);

	error = devm_request_threaded_irq(&pdev->dev, ts_irq,
					  NULL, stmpe_ts_handler,
					  IRQF_ONESHOT, STMPE_TS_NAME, ts);
	if (error) {
		dev_err(&pdev->dev, "Failed to request IRQ %d\n", ts_irq);
		return error;
	}

	error = stmpe_init_hw(ts);
	if (error)
		return error;

	idev->name = STMPE_TS_NAME;
	idev->phys = STMPE_TS_NAME"/input0";
	idev->id.bustype = BUS_I2C;

	idev->open = stmpe_ts_open;
	idev->close = stmpe_ts_close;

	input_set_drvdata(idev, ts);

	input_set_capability(idev, EV_KEY, BTN_TOUCH);
	input_set_abs_params(idev, ABS_X, 0, XY_MASK, 0, 0);
	input_set_abs_params(idev, ABS_Y, 0, XY_MASK, 0, 0);
	input_set_abs_params(idev, ABS_PRESSURE, 0x0, 0xff, 0, 0);

<<<<<<< HEAD
=======
	touchscreen_parse_properties(idev, false, &ts->prop);

>>>>>>> upstream/android-13
	error = input_register_device(idev);
	if (error) {
		dev_err(&pdev->dev, "Could not register input device\n");
		return error;
	}

	return 0;
}

static int stmpe_ts_remove(struct platform_device *pdev)
{
	struct stmpe_touch *ts = platform_get_drvdata(pdev);

	stmpe_disable(ts->stmpe, STMPE_BLOCK_TOUCHSCREEN);

	return 0;
}

static struct platform_driver stmpe_ts_driver = {
	.driver = {
		.name = STMPE_TS_NAME,
	},
	.probe = stmpe_input_probe,
	.remove = stmpe_ts_remove,
};
module_platform_driver(stmpe_ts_driver);

static const struct of_device_id stmpe_ts_ids[] = {
	{ .compatible = "st,stmpe-ts", },
	{ },
};
MODULE_DEVICE_TABLE(of, stmpe_ts_ids);

MODULE_AUTHOR("Luotao Fu <l.fu@pengutronix.de>");
MODULE_DESCRIPTION("STMPEXXX touchscreen driver");
MODULE_LICENSE("GPL");
