<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Driver for the Epson RTC module RX-8010 SJ
 *
 * Copyright(C) Timesys Corporation 2015
 * Copyright(C) General Electric Company 2015
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/bcd.h>
#include <linux/bitops.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/rtc.h>

#define RX8010_SEC     0x10
#define RX8010_MIN     0x11
#define RX8010_HOUR    0x12
#define RX8010_WDAY    0x13
#define RX8010_MDAY    0x14
#define RX8010_MONTH   0x15
#define RX8010_YEAR    0x16
#define RX8010_RESV17  0x17
#define RX8010_ALMIN   0x18
#define RX8010_ALHOUR  0x19
#define RX8010_ALWDAY  0x1A
#define RX8010_TCOUNT0 0x1B
#define RX8010_TCOUNT1 0x1C
#define RX8010_EXT     0x1D
#define RX8010_FLAG    0x1E
#define RX8010_CTRL    0x1F
/* 0x20 to 0x2F are user registers */
#define RX8010_RESV30  0x30
#define RX8010_RESV31  0x31
#define RX8010_IRQ     0x32

#define RX8010_EXT_WADA  BIT(3)

#define RX8010_FLAG_VLF  BIT(1)
#define RX8010_FLAG_AF   BIT(3)
#define RX8010_FLAG_TF   BIT(4)
#define RX8010_FLAG_UF   BIT(5)

#define RX8010_CTRL_AIE  BIT(3)
#define RX8010_CTRL_UIE  BIT(5)
#define RX8010_CTRL_STOP BIT(6)
#define RX8010_CTRL_TEST BIT(7)

#define RX8010_ALARM_AE  BIT(7)
=======
#include <linux/regmap.h>
#include <linux/rtc.h>

#define RX8010_SEC		0x10
#define RX8010_MIN		0x11
#define RX8010_HOUR		0x12
#define RX8010_WDAY		0x13
#define RX8010_MDAY		0x14
#define RX8010_MONTH		0x15
#define RX8010_YEAR		0x16
#define RX8010_RESV17		0x17
#define RX8010_ALMIN		0x18
#define RX8010_ALHOUR		0x19
#define RX8010_ALWDAY		0x1A
#define RX8010_TCOUNT0		0x1B
#define RX8010_TCOUNT1		0x1C
#define RX8010_EXT		0x1D
#define RX8010_FLAG		0x1E
#define RX8010_CTRL		0x1F
/* 0x20 to 0x2F are user registers */
#define RX8010_RESV30		0x30
#define RX8010_RESV31		0x31
#define RX8010_IRQ		0x32

#define RX8010_EXT_WADA		BIT(3)

#define RX8010_FLAG_VLF		BIT(1)
#define RX8010_FLAG_AF		BIT(3)
#define RX8010_FLAG_TF		BIT(4)
#define RX8010_FLAG_UF		BIT(5)

#define RX8010_CTRL_AIE		BIT(3)
#define RX8010_CTRL_UIE		BIT(5)
#define RX8010_CTRL_STOP	BIT(6)
#define RX8010_CTRL_TEST	BIT(7)

#define RX8010_ALARM_AE		BIT(7)
>>>>>>> upstream/android-13

static const struct i2c_device_id rx8010_id[] = {
	{ "rx8010", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, rx8010_id);

<<<<<<< HEAD
static const struct of_device_id rx8010_of_match[] = {
=======
static const __maybe_unused struct of_device_id rx8010_of_match[] = {
>>>>>>> upstream/android-13
	{ .compatible = "epson,rx8010" },
	{ }
};
MODULE_DEVICE_TABLE(of, rx8010_of_match);

struct rx8010_data {
<<<<<<< HEAD
	struct i2c_client *client;
=======
	struct regmap *regs;
>>>>>>> upstream/android-13
	struct rtc_device *rtc;
	u8 ctrlreg;
};

static irqreturn_t rx8010_irq_1_handler(int irq, void *dev_id)
{
	struct i2c_client *client = dev_id;
	struct rx8010_data *rx8010 = i2c_get_clientdata(client);
<<<<<<< HEAD
	int flagreg;

	mutex_lock(&rx8010->rtc->ops_lock);

	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);

	if (flagreg <= 0) {
		mutex_unlock(&rx8010->rtc->ops_lock);
=======
	int flagreg, err;

	rtc_lock(rx8010->rtc);

	err = regmap_read(rx8010->regs, RX8010_FLAG, &flagreg);
	if (err) {
		rtc_unlock(rx8010->rtc);
>>>>>>> upstream/android-13
		return IRQ_NONE;
	}

	if (flagreg & RX8010_FLAG_VLF)
		dev_warn(&client->dev, "Frequency stop detected\n");

	if (flagreg & RX8010_FLAG_TF) {
		flagreg &= ~RX8010_FLAG_TF;
		rtc_update_irq(rx8010->rtc, 1, RTC_PF | RTC_IRQF);
	}

	if (flagreg & RX8010_FLAG_AF) {
		flagreg &= ~RX8010_FLAG_AF;
		rtc_update_irq(rx8010->rtc, 1, RTC_AF | RTC_IRQF);
	}

	if (flagreg & RX8010_FLAG_UF) {
		flagreg &= ~RX8010_FLAG_UF;
		rtc_update_irq(rx8010->rtc, 1, RTC_UF | RTC_IRQF);
	}

<<<<<<< HEAD
	i2c_smbus_write_byte_data(client, RX8010_FLAG, flagreg);

	mutex_unlock(&rx8010->rtc->ops_lock);
	return IRQ_HANDLED;
=======
	err = regmap_write(rx8010->regs, RX8010_FLAG, flagreg);
	rtc_unlock(rx8010->rtc);
	return err ? IRQ_NONE : IRQ_HANDLED;
>>>>>>> upstream/android-13
}

static int rx8010_get_time(struct device *dev, struct rtc_time *dt)
{
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
<<<<<<< HEAD
	u8 date[7];
	int flagreg;
	int err;

	flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
	if (flagreg < 0)
		return flagreg;
=======
	u8 date[RX8010_YEAR - RX8010_SEC + 1];
	int flagreg, err;

	err = regmap_read(rx8010->regs, RX8010_FLAG, &flagreg);
	if (err)
		return err;
>>>>>>> upstream/android-13

	if (flagreg & RX8010_FLAG_VLF) {
		dev_warn(dev, "Frequency stop detected\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	err = i2c_smbus_read_i2c_block_data(rx8010->client, RX8010_SEC,
					    7, date);
	if (err != 7)
		return err < 0 ? err : -EIO;
=======
	err = regmap_bulk_read(rx8010->regs, RX8010_SEC, date, sizeof(date));
	if (err)
		return err;
>>>>>>> upstream/android-13

	dt->tm_sec = bcd2bin(date[RX8010_SEC - RX8010_SEC] & 0x7f);
	dt->tm_min = bcd2bin(date[RX8010_MIN - RX8010_SEC] & 0x7f);
	dt->tm_hour = bcd2bin(date[RX8010_HOUR - RX8010_SEC] & 0x3f);
	dt->tm_mday = bcd2bin(date[RX8010_MDAY - RX8010_SEC] & 0x3f);
	dt->tm_mon = bcd2bin(date[RX8010_MONTH - RX8010_SEC] & 0x1f) - 1;
	dt->tm_year = bcd2bin(date[RX8010_YEAR - RX8010_SEC]) + 100;
	dt->tm_wday = ffs(date[RX8010_WDAY - RX8010_SEC] & 0x7f);

	return 0;
}

static int rx8010_set_time(struct device *dev, struct rtc_time *dt)
{
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
<<<<<<< HEAD
	u8 date[7];
	int ctrl, flagreg;
	int ret;

	if ((dt->tm_year < 100) || (dt->tm_year > 199))
		return -EINVAL;

	/* set STOP bit before changing clock/calendar */
	ctrl = i2c_smbus_read_byte_data(rx8010->client, RX8010_CTRL);
	if (ctrl < 0)
		return ctrl;
	rx8010->ctrlreg = ctrl | RX8010_CTRL_STOP;
	ret = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
					rx8010->ctrlreg);
	if (ret < 0)
		return ret;
=======
	u8 date[RX8010_YEAR - RX8010_SEC + 1];
	int err;

	/* set STOP bit before changing clock/calendar */
	err = regmap_set_bits(rx8010->regs, RX8010_CTRL, RX8010_CTRL_STOP);
	if (err)
		return err;
>>>>>>> upstream/android-13

	date[RX8010_SEC - RX8010_SEC] = bin2bcd(dt->tm_sec);
	date[RX8010_MIN - RX8010_SEC] = bin2bcd(dt->tm_min);
	date[RX8010_HOUR - RX8010_SEC] = bin2bcd(dt->tm_hour);
	date[RX8010_MDAY - RX8010_SEC] = bin2bcd(dt->tm_mday);
	date[RX8010_MONTH - RX8010_SEC] = bin2bcd(dt->tm_mon + 1);
	date[RX8010_YEAR - RX8010_SEC] = bin2bcd(dt->tm_year - 100);
	date[RX8010_WDAY - RX8010_SEC] = bin2bcd(1 << dt->tm_wday);

<<<<<<< HEAD
	ret = i2c_smbus_write_i2c_block_data(rx8010->client,
					     RX8010_SEC, 7, date);
	if (ret < 0)
		return ret;

	/* clear STOP bit after changing clock/calendar */
	ctrl = i2c_smbus_read_byte_data(rx8010->client, RX8010_CTRL);
	if (ctrl < 0)
		return ctrl;
	rx8010->ctrlreg = ctrl & ~RX8010_CTRL_STOP;
	ret = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
					rx8010->ctrlreg);
	if (ret < 0)
		return ret;

	flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
	if (flagreg < 0) {
		return flagreg;
	}

	if (flagreg & RX8010_FLAG_VLF)
		ret = i2c_smbus_write_byte_data(rx8010->client, RX8010_FLAG,
						flagreg & ~RX8010_FLAG_VLF);
=======
	err = regmap_bulk_write(rx8010->regs, RX8010_SEC, date, sizeof(date));
	if (err)
		return err;

	/* clear STOP bit after changing clock/calendar */
	err = regmap_clear_bits(rx8010->regs, RX8010_CTRL, RX8010_CTRL_STOP);
	if (err)
		return err;

	err = regmap_clear_bits(rx8010->regs, RX8010_FLAG, RX8010_FLAG_VLF);
	if (err)
		return err;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int rx8010_init_client(struct i2c_client *client)
{
	struct rx8010_data *rx8010 = i2c_get_clientdata(client);
	u8 ctrl[2];
	int need_clear = 0, err = 0;

	/* Initialize reserved registers as specified in datasheet */
	err = i2c_smbus_write_byte_data(client, RX8010_RESV17, 0xD8);
	if (err < 0)
		return err;

	err = i2c_smbus_write_byte_data(client, RX8010_RESV30, 0x00);
	if (err < 0)
		return err;

	err = i2c_smbus_write_byte_data(client, RX8010_RESV31, 0x08);
	if (err < 0)
		return err;

	err = i2c_smbus_write_byte_data(client, RX8010_IRQ, 0x00);
	if (err < 0)
		return err;

	err = i2c_smbus_read_i2c_block_data(rx8010->client, RX8010_FLAG,
					    2, ctrl);
	if (err != 2)
		return err < 0 ? err : -EIO;

	if (ctrl[0] & RX8010_FLAG_VLF)
		dev_warn(&client->dev, "Frequency stop was detected\n");

	if (ctrl[0] & RX8010_FLAG_AF) {
		dev_warn(&client->dev, "Alarm was detected\n");
=======
static int rx8010_init(struct device *dev)
{
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	u8 ctrl[2];
	int need_clear = 0, err;

	/* Initialize reserved registers as specified in datasheet */
	err = regmap_write(rx8010->regs, RX8010_RESV17, 0xD8);
	if (err)
		return err;

	err = regmap_write(rx8010->regs, RX8010_RESV30, 0x00);
	if (err)
		return err;

	err = regmap_write(rx8010->regs, RX8010_RESV31, 0x08);
	if (err)
		return err;

	err = regmap_write(rx8010->regs, RX8010_IRQ, 0x00);
	if (err)
		return err;

	err = regmap_bulk_read(rx8010->regs, RX8010_FLAG, ctrl, 2);
	if (err)
		return err;

	if (ctrl[0] & RX8010_FLAG_VLF)
		dev_warn(dev, "Frequency stop was detected\n");

	if (ctrl[0] & RX8010_FLAG_AF) {
		dev_warn(dev, "Alarm was detected\n");
>>>>>>> upstream/android-13
		need_clear = 1;
	}

	if (ctrl[0] & RX8010_FLAG_TF)
		need_clear = 1;

	if (ctrl[0] & RX8010_FLAG_UF)
		need_clear = 1;

	if (need_clear) {
		ctrl[0] &= ~(RX8010_FLAG_AF | RX8010_FLAG_TF | RX8010_FLAG_UF);
<<<<<<< HEAD
		err = i2c_smbus_write_byte_data(client, RX8010_FLAG, ctrl[0]);
		if (err < 0)
=======
		err = regmap_write(rx8010->regs, RX8010_FLAG, ctrl[0]);
		if (err)
>>>>>>> upstream/android-13
			return err;
	}

	rx8010->ctrlreg = (ctrl[1] & ~RX8010_CTRL_TEST);

	return 0;
}

static int rx8010_read_alarm(struct device *dev, struct rtc_wkalrm *t)
{
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct i2c_client *client = rx8010->client;
	u8 alarmvals[3];
	int flagreg;
	int err;

	err = i2c_smbus_read_i2c_block_data(client, RX8010_ALMIN, 3, alarmvals);
	if (err != 3)
		return err < 0 ? err : -EIO;

	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);
	if (flagreg < 0)
		return flagreg;
=======
	u8 alarmvals[3];
	int flagreg, err;

	err = regmap_bulk_read(rx8010->regs, RX8010_ALMIN, alarmvals, 3);
	if (err)
		return err;

	err = regmap_read(rx8010->regs, RX8010_FLAG, &flagreg);
	if (err)
		return err;
>>>>>>> upstream/android-13

	t->time.tm_sec = 0;
	t->time.tm_min = bcd2bin(alarmvals[0] & 0x7f);
	t->time.tm_hour = bcd2bin(alarmvals[1] & 0x3f);

	if (!(alarmvals[2] & RX8010_ALARM_AE))
		t->time.tm_mday = bcd2bin(alarmvals[2] & 0x7f);

	t->enabled = !!(rx8010->ctrlreg & RX8010_CTRL_AIE);
	t->pending = (flagreg & RX8010_FLAG_AF) && t->enabled;

	return 0;
}

static int rx8010_set_alarm(struct device *dev, struct rtc_wkalrm *t)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	u8 alarmvals[3];
	int extreg, flagreg;
	int err;

	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);
	if (flagreg < 0) {
		return flagreg;
	}

	if (rx8010->ctrlreg & (RX8010_CTRL_AIE | RX8010_CTRL_UIE)) {
		rx8010->ctrlreg &= ~(RX8010_CTRL_AIE | RX8010_CTRL_UIE);
		err = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
						rx8010->ctrlreg);
		if (err < 0) {
			return err;
		}
	}

	flagreg &= ~RX8010_FLAG_AF;
	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_FLAG, flagreg);
	if (err < 0)
=======
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	u8 alarmvals[3];
	int err;

	if (rx8010->ctrlreg & (RX8010_CTRL_AIE | RX8010_CTRL_UIE)) {
		rx8010->ctrlreg &= ~(RX8010_CTRL_AIE | RX8010_CTRL_UIE);
		err = regmap_write(rx8010->regs, RX8010_CTRL, rx8010->ctrlreg);
		if (err)
			return err;
	}

	err = regmap_clear_bits(rx8010->regs, RX8010_FLAG, RX8010_FLAG_AF);
	if (err)
>>>>>>> upstream/android-13
		return err;

	alarmvals[0] = bin2bcd(t->time.tm_min);
	alarmvals[1] = bin2bcd(t->time.tm_hour);
	alarmvals[2] = bin2bcd(t->time.tm_mday);

<<<<<<< HEAD
	err = i2c_smbus_write_i2c_block_data(rx8010->client, RX8010_ALMIN,
					     2, alarmvals);
	if (err < 0)
		return err;

	extreg = i2c_smbus_read_byte_data(client, RX8010_EXT);
	if (extreg < 0)
		return extreg;

	extreg |= RX8010_EXT_WADA;
	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_EXT, extreg);
	if (err < 0)
=======
	err = regmap_bulk_write(rx8010->regs, RX8010_ALMIN, alarmvals, 2);
	if (err)
		return err;

	err = regmap_clear_bits(rx8010->regs, RX8010_EXT, RX8010_EXT_WADA);
	if (err)
>>>>>>> upstream/android-13
		return err;

	if (alarmvals[2] == 0)
		alarmvals[2] |= RX8010_ALARM_AE;

<<<<<<< HEAD
	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_ALWDAY,
					alarmvals[2]);
	if (err < 0)
=======
	err = regmap_write(rx8010->regs, RX8010_ALWDAY, alarmvals[2]);
	if (err)
>>>>>>> upstream/android-13
		return err;

	if (t->enabled) {
		if (rx8010->rtc->uie_rtctimer.enabled)
			rx8010->ctrlreg |= RX8010_CTRL_UIE;
		if (rx8010->rtc->aie_timer.enabled)
			rx8010->ctrlreg |=
				(RX8010_CTRL_AIE | RX8010_CTRL_UIE);

<<<<<<< HEAD
		err = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
						rx8010->ctrlreg);
		if (err < 0)
=======
		err = regmap_write(rx8010->regs, RX8010_CTRL, rx8010->ctrlreg);
		if (err)
>>>>>>> upstream/android-13
			return err;
	}

	return 0;
}

static int rx8010_alarm_irq_enable(struct device *dev,
				   unsigned int enabled)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	int flagreg;
	u8 ctrl;
	int err;
=======
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	int err;
	u8 ctrl;
>>>>>>> upstream/android-13

	ctrl = rx8010->ctrlreg;

	if (enabled) {
		if (rx8010->rtc->uie_rtctimer.enabled)
			ctrl |= RX8010_CTRL_UIE;
		if (rx8010->rtc->aie_timer.enabled)
			ctrl |= (RX8010_CTRL_AIE | RX8010_CTRL_UIE);
	} else {
		if (!rx8010->rtc->uie_rtctimer.enabled)
			ctrl &= ~RX8010_CTRL_UIE;
		if (!rx8010->rtc->aie_timer.enabled)
			ctrl &= ~RX8010_CTRL_AIE;
	}

<<<<<<< HEAD
	flagreg = i2c_smbus_read_byte_data(client, RX8010_FLAG);
	if (flagreg < 0)
		return flagreg;

	flagreg &= ~RX8010_FLAG_AF;
	err = i2c_smbus_write_byte_data(rx8010->client, RX8010_FLAG, flagreg);
	if (err < 0)
=======
	err = regmap_clear_bits(rx8010->regs, RX8010_FLAG, RX8010_FLAG_AF);
	if (err)
>>>>>>> upstream/android-13
		return err;

	if (ctrl != rx8010->ctrlreg) {
		rx8010->ctrlreg = ctrl;
<<<<<<< HEAD
		err = i2c_smbus_write_byte_data(rx8010->client, RX8010_CTRL,
						rx8010->ctrlreg);
		if (err < 0)
=======
		err = regmap_write(rx8010->regs, RX8010_CTRL, rx8010->ctrlreg);
		if (err)
>>>>>>> upstream/android-13
			return err;
	}

	return 0;
}

static int rx8010_ioctl(struct device *dev, unsigned int cmd, unsigned long arg)
{
<<<<<<< HEAD
	struct i2c_client *client = to_i2c_client(dev);
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	int ret, tmp;
	int flagreg;

	switch (cmd) {
	case RTC_VL_READ:
		flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
		if (flagreg < 0)
			return flagreg;

		tmp = !!(flagreg & RX8010_FLAG_VLF);
		if (copy_to_user((void __user *)arg, &tmp, sizeof(int)))
			return -EFAULT;

		return 0;

	case RTC_VL_CLR:
		flagreg = i2c_smbus_read_byte_data(rx8010->client, RX8010_FLAG);
		if (flagreg < 0) {
			return flagreg;
		}

		flagreg &= ~RX8010_FLAG_VLF;
		ret = i2c_smbus_write_byte_data(client, RX8010_FLAG, flagreg);
		if (ret < 0)
			return ret;

		return 0;
=======
	struct rx8010_data *rx8010 = dev_get_drvdata(dev);
	int tmp, flagreg, err;

	switch (cmd) {
	case RTC_VL_READ:
		err = regmap_read(rx8010->regs, RX8010_FLAG, &flagreg);
		if (err)
			return err;

		tmp = flagreg & RX8010_FLAG_VLF ? RTC_VL_DATA_INVALID : 0;
		return put_user(tmp, (unsigned int __user *)arg);
>>>>>>> upstream/android-13

	default:
		return -ENOIOCTLCMD;
	}
}

<<<<<<< HEAD
static const struct rtc_class_ops rx8010_rtc_ops_default = {
	.read_time = rx8010_get_time,
	.set_time = rx8010_set_time,
	.ioctl = rx8010_ioctl,
};

static const struct rtc_class_ops rx8010_rtc_ops_alarm = {
=======
static const struct rtc_class_ops rx8010_rtc_ops = {
>>>>>>> upstream/android-13
	.read_time = rx8010_get_time,
	.set_time = rx8010_set_time,
	.ioctl = rx8010_ioctl,
	.read_alarm = rx8010_read_alarm,
	.set_alarm = rx8010_set_alarm,
	.alarm_irq_enable = rx8010_alarm_irq_enable,
};

<<<<<<< HEAD
static int rx8010_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	const struct rtc_class_ops *rtc_ops;
	struct rx8010_data *rx8010;
	int err = 0;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA
		| I2C_FUNC_SMBUS_I2C_BLOCK)) {
		dev_err(&adapter->dev, "doesn't support required functionality\n");
		return -EIO;
	}

	rx8010 = devm_kzalloc(&client->dev, sizeof(struct rx8010_data),
			      GFP_KERNEL);
	if (!rx8010)
		return -ENOMEM;

	rx8010->client = client;
	i2c_set_clientdata(client, rx8010);

	err = rx8010_init_client(client);
	if (err)
		return err;

	if (client->irq > 0) {
		dev_info(&client->dev, "IRQ %d supplied\n", client->irq);
		err = devm_request_threaded_irq(&client->dev, client->irq, NULL,
						rx8010_irq_1_handler,
						IRQF_TRIGGER_LOW | IRQF_ONESHOT,
						"rx8010", client);

		if (err) {
			dev_err(&client->dev, "unable to request IRQ\n");
			return err;
		}

		rtc_ops = &rx8010_rtc_ops_alarm;
	} else {
		rtc_ops = &rx8010_rtc_ops_default;
	}

	rx8010->rtc = devm_rtc_device_register(&client->dev, client->name,
					       rtc_ops, THIS_MODULE);

	if (IS_ERR(rx8010->rtc)) {
		dev_err(&client->dev, "unable to register the class device\n");
		return PTR_ERR(rx8010->rtc);
	}

	rx8010->rtc->max_user_freq = 1;

	return err;
=======
static const struct regmap_config rx8010_regmap_config = {
	.name = "rx8010-rtc",
	.reg_bits = 8,
	.val_bits = 8,
};

static int rx8010_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct rx8010_data *rx8010;
	int err = 0;

	rx8010 = devm_kzalloc(dev, sizeof(*rx8010), GFP_KERNEL);
	if (!rx8010)
		return -ENOMEM;

	i2c_set_clientdata(client, rx8010);

	rx8010->regs = devm_regmap_init_i2c(client, &rx8010_regmap_config);
	if (IS_ERR(rx8010->regs))
		return PTR_ERR(rx8010->regs);

	err = rx8010_init(dev);
	if (err)
		return err;

	rx8010->rtc = devm_rtc_allocate_device(dev);
	if (IS_ERR(rx8010->rtc))
		return PTR_ERR(rx8010->rtc);

	if (client->irq > 0) {
		dev_info(dev, "IRQ %d supplied\n", client->irq);
		err = devm_request_threaded_irq(dev, client->irq, NULL,
						rx8010_irq_1_handler,
						IRQF_TRIGGER_LOW | IRQF_ONESHOT,
						"rx8010", client);
		if (err) {
			dev_err(dev, "unable to request IRQ\n");
			return err;
		}
	} else {
		clear_bit(RTC_FEATURE_ALARM, rx8010->rtc->features);
	}

	rx8010->rtc->ops = &rx8010_rtc_ops;
	rx8010->rtc->max_user_freq = 1;
	rx8010->rtc->range_min = RTC_TIMESTAMP_BEGIN_2000;
	rx8010->rtc->range_max = RTC_TIMESTAMP_END_2099;

	return devm_rtc_register_device(rx8010->rtc);
>>>>>>> upstream/android-13
}

static struct i2c_driver rx8010_driver = {
	.driver = {
		.name = "rtc-rx8010",
		.of_match_table = of_match_ptr(rx8010_of_match),
	},
<<<<<<< HEAD
	.probe		= rx8010_probe,
=======
	.probe_new	= rx8010_probe,
>>>>>>> upstream/android-13
	.id_table	= rx8010_id,
};

module_i2c_driver(rx8010_driver);

MODULE_AUTHOR("Akshay Bhat <akshay.bhat@timesys.com>");
MODULE_DESCRIPTION("Epson RX8010SJ RTC driver");
MODULE_LICENSE("GPL v2");
