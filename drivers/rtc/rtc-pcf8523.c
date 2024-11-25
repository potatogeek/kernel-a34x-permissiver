<<<<<<< HEAD
/*
 * Copyright (C) 2012 Avionic Design GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Avionic Design GmbH
>>>>>>> upstream/android-13
 */

#include <linux/bcd.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/rtc.h>
#include <linux/of.h>
<<<<<<< HEAD

#define DRIVER_NAME "rtc-pcf8523"

#define REG_CONTROL1 0x00
#define REG_CONTROL1_CAP_SEL (1 << 7)
#define REG_CONTROL1_STOP    (1 << 5)

#define REG_CONTROL3 0x02
#define REG_CONTROL3_PM_BLD (1 << 7) /* battery low detection disabled */
#define REG_CONTROL3_PM_VDD (1 << 6) /* switch-over disabled */
#define REG_CONTROL3_PM_DSM (1 << 5) /* direct switching mode */
#define REG_CONTROL3_PM_MASK 0xe0
#define REG_CONTROL3_BLF (1 << 2) /* battery low bit, read-only */

#define REG_SECONDS  0x03
#define REG_SECONDS_OS (1 << 7)

#define REG_MINUTES  0x04
#define REG_HOURS    0x05
#define REG_DAYS     0x06
#define REG_WEEKDAYS 0x07
#define REG_MONTHS   0x08
#define REG_YEARS    0x09

#define REG_OFFSET   0x0e
#define REG_OFFSET_MODE BIT(7)

struct pcf8523 {
	struct rtc_device *rtc;
=======
#include <linux/pm_wakeirq.h>

#define PCF8523_REG_CONTROL1 0x00
#define PCF8523_CONTROL1_CAP_SEL BIT(7)
#define PCF8523_CONTROL1_STOP    BIT(5)
#define PCF8523_CONTROL1_AIE    BIT(1)

#define PCF8523_REG_CONTROL2 0x01
#define PCF8523_CONTROL2_AF BIT(3)

#define PCF8523_REG_CONTROL3 0x02
#define PCF8523_CONTROL3_PM_BLD BIT(7) /* battery low detection disabled */
#define PCF8523_CONTROL3_PM_VDD BIT(6) /* switch-over disabled */
#define PCF8523_CONTROL3_PM_DSM BIT(5) /* direct switching mode */
#define PCF8523_CONTROL3_PM_MASK 0xe0
#define PCF8523_CONTROL3_BLF BIT(2) /* battery low bit, read-only */

#define PCF8523_REG_SECONDS  0x03
#define PCF8523_SECONDS_OS BIT(7)

#define PCF8523_REG_MINUTES  0x04
#define PCF8523_REG_HOURS    0x05
#define PCF8523_REG_DAYS     0x06
#define PCF8523_REG_WEEKDAYS 0x07
#define PCF8523_REG_MONTHS   0x08
#define PCF8523_REG_YEARS    0x09

#define PCF8523_REG_MINUTE_ALARM	0x0a
#define PCF8523_REG_HOUR_ALARM		0x0b
#define PCF8523_REG_DAY_ALARM		0x0c
#define PCF8523_REG_WEEKDAY_ALARM	0x0d
#define ALARM_DIS BIT(7)

#define PCF8523_REG_OFFSET   0x0e
#define PCF8523_OFFSET_MODE BIT(7)

#define PCF8523_TMR_CLKOUT_CTRL 0x0f

struct pcf8523 {
	struct rtc_device *rtc;
	struct i2c_client *client;
>>>>>>> upstream/android-13
};

static int pcf8523_read(struct i2c_client *client, u8 reg, u8 *valuep)
{
	struct i2c_msg msgs[2];
	u8 value = 0;
	int err;

	msgs[0].addr = client->addr;
	msgs[0].flags = 0;
	msgs[0].len = sizeof(reg);
	msgs[0].buf = &reg;

	msgs[1].addr = client->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = sizeof(value);
	msgs[1].buf = &value;

	err = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (err < 0)
		return err;

	*valuep = value;

	return 0;
}

static int pcf8523_write(struct i2c_client *client, u8 reg, u8 value)
{
	u8 buffer[2] = { reg, value };
	struct i2c_msg msg;
	int err;

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = sizeof(buffer);
	msg.buf = buffer;

	err = i2c_transfer(client->adapter, &msg, 1);
	if (err < 0)
		return err;

	return 0;
}

static int pcf8523_voltage_low(struct i2c_client *client)
{
	u8 value;
	int err;

<<<<<<< HEAD
	err = pcf8523_read(client, REG_CONTROL3, &value);
	if (err < 0)
		return err;

	return !!(value & REG_CONTROL3_BLF);
=======
	err = pcf8523_read(client, PCF8523_REG_CONTROL3, &value);
	if (err < 0)
		return err;

	return !!(value & PCF8523_CONTROL3_BLF);
>>>>>>> upstream/android-13
}

static int pcf8523_load_capacitance(struct i2c_client *client)
{
	u32 load;
	u8 value;
	int err;

<<<<<<< HEAD
	err = pcf8523_read(client, REG_CONTROL1, &value);
=======
	err = pcf8523_read(client, PCF8523_REG_CONTROL1, &value);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	load = 12500;
	of_property_read_u32(client->dev.of_node, "quartz-load-femtofarads",
			     &load);

	switch (load) {
	default:
		dev_warn(&client->dev, "Unknown quartz-load-femtofarads value: %d. Assuming 12500",
			 load);
<<<<<<< HEAD
		/* fall through */
	case 12500:
		value |= REG_CONTROL1_CAP_SEL;
		break;
	case 7000:
		value &= ~REG_CONTROL1_CAP_SEL;
		break;
	}

	err = pcf8523_write(client, REG_CONTROL1, value);
=======
		fallthrough;
	case 12500:
		value |= PCF8523_CONTROL1_CAP_SEL;
		break;
	case 7000:
		value &= ~PCF8523_CONTROL1_CAP_SEL;
		break;
	}

	err = pcf8523_write(client, PCF8523_REG_CONTROL1, value);
>>>>>>> upstream/android-13

	return err;
}

static int pcf8523_set_pm(struct i2c_client *client, u8 pm)
{
	u8 value;
	int err;

<<<<<<< HEAD
	err = pcf8523_read(client, REG_CONTROL3, &value);
	if (err < 0)
		return err;

	value = (value & ~REG_CONTROL3_PM_MASK) | pm;

	err = pcf8523_write(client, REG_CONTROL3, value);
=======
	err = pcf8523_read(client, PCF8523_REG_CONTROL3, &value);
	if (err < 0)
		return err;

	value = (value & ~PCF8523_CONTROL3_PM_MASK) | pm;

	err = pcf8523_write(client, PCF8523_REG_CONTROL3, value);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	return 0;
}

<<<<<<< HEAD
=======
static irqreturn_t pcf8523_irq(int irq, void *dev_id)
{
	struct pcf8523 *pcf8523 = i2c_get_clientdata(dev_id);
	u8 value;
	int err;

	err = pcf8523_read(pcf8523->client, PCF8523_REG_CONTROL2, &value);
	if (err < 0)
		return IRQ_HANDLED;

	if (value & PCF8523_CONTROL2_AF) {
		value &= ~PCF8523_CONTROL2_AF;
		pcf8523_write(pcf8523->client, PCF8523_REG_CONTROL2, value);
		rtc_update_irq(pcf8523->rtc, 1, RTC_IRQF | RTC_AF);

		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

>>>>>>> upstream/android-13
static int pcf8523_stop_rtc(struct i2c_client *client)
{
	u8 value;
	int err;

<<<<<<< HEAD
	err = pcf8523_read(client, REG_CONTROL1, &value);
	if (err < 0)
		return err;

	value |= REG_CONTROL1_STOP;

	err = pcf8523_write(client, REG_CONTROL1, value);
=======
	err = pcf8523_read(client, PCF8523_REG_CONTROL1, &value);
	if (err < 0)
		return err;

	value |= PCF8523_CONTROL1_STOP;

	err = pcf8523_write(client, PCF8523_REG_CONTROL1, value);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	return 0;
}

static int pcf8523_start_rtc(struct i2c_client *client)
{
	u8 value;
	int err;

<<<<<<< HEAD
	err = pcf8523_read(client, REG_CONTROL1, &value);
	if (err < 0)
		return err;

	value &= ~REG_CONTROL1_STOP;

	err = pcf8523_write(client, REG_CONTROL1, value);
=======
	err = pcf8523_read(client, PCF8523_REG_CONTROL1, &value);
	if (err < 0)
		return err;

	value &= ~PCF8523_CONTROL1_STOP;

	err = pcf8523_write(client, PCF8523_REG_CONTROL1, value);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	return 0;
}

static int pcf8523_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
	u8 start = REG_SECONDS, regs[7];
=======
	u8 start = PCF8523_REG_SECONDS, regs[7];
>>>>>>> upstream/android-13
	struct i2c_msg msgs[2];
	int err;

	err = pcf8523_voltage_low(client);
	if (err < 0) {
		return err;
	} else if (err > 0) {
		dev_err(dev, "low voltage detected, time is unreliable\n");
		return -EINVAL;
	}

	msgs[0].addr = client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &start;

	msgs[1].addr = client->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = sizeof(regs);
	msgs[1].buf = regs;

	err = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (err < 0)
		return err;

<<<<<<< HEAD
	if (regs[0] & REG_SECONDS_OS)
=======
	if (regs[0] & PCF8523_SECONDS_OS)
>>>>>>> upstream/android-13
		return -EINVAL;

	tm->tm_sec = bcd2bin(regs[0] & 0x7f);
	tm->tm_min = bcd2bin(regs[1] & 0x7f);
	tm->tm_hour = bcd2bin(regs[2] & 0x3f);
	tm->tm_mday = bcd2bin(regs[3] & 0x3f);
	tm->tm_wday = regs[4] & 0x7;
	tm->tm_mon = bcd2bin(regs[5] & 0x1f) - 1;
	tm->tm_year = bcd2bin(regs[6]) + 100;

	return 0;
}

static int pcf8523_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct i2c_msg msg;
	u8 regs[8];
	int err;

<<<<<<< HEAD
	/*
	 * The hardware can only store values between 0 and 99 in it's YEAR
	 * register (with 99 overflowing to 0 on increment).
	 * After 2100-02-28 we could start interpreting the year to be in the
	 * interval [2100, 2199], but there is no path to switch in a smooth way
	 * because the chip handles YEAR=0x00 (and the out-of-spec
	 * YEAR=0xa0) as a leap year, but 2100 isn't.
	 */
	if (tm->tm_year < 100 || tm->tm_year >= 200)
		return -EINVAL;

=======
>>>>>>> upstream/android-13
	err = pcf8523_stop_rtc(client);
	if (err < 0)
		return err;

<<<<<<< HEAD
	regs[0] = REG_SECONDS;
	/* This will purposely overwrite REG_SECONDS_OS */
=======
	regs[0] = PCF8523_REG_SECONDS;
	/* This will purposely overwrite PCF8523_SECONDS_OS */
>>>>>>> upstream/android-13
	regs[1] = bin2bcd(tm->tm_sec);
	regs[2] = bin2bcd(tm->tm_min);
	regs[3] = bin2bcd(tm->tm_hour);
	regs[4] = bin2bcd(tm->tm_mday);
	regs[5] = tm->tm_wday;
	regs[6] = bin2bcd(tm->tm_mon + 1);
	regs[7] = bin2bcd(tm->tm_year - 100);

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = sizeof(regs);
	msg.buf = regs;

	err = i2c_transfer(client->adapter, &msg, 1);
	if (err < 0) {
		/*
		 * If the time cannot be set, restart the RTC anyway. Note
		 * that errors are ignored if the RTC cannot be started so
		 * that we have a chance to propagate the original error.
		 */
		pcf8523_start_rtc(client);
		return err;
	}

	return pcf8523_start_rtc(client);
}

<<<<<<< HEAD
=======
static int pcf8523_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *tm)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 start = PCF8523_REG_MINUTE_ALARM, regs[4];
	struct i2c_msg msgs[2];
	u8 value;
	int err;

	msgs[0].addr = client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &start;

	msgs[1].addr = client->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = sizeof(regs);
	msgs[1].buf = regs;

	err = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (err < 0)
		return err;

	tm->time.tm_sec = 0;
	tm->time.tm_min = bcd2bin(regs[0] & 0x7F);
	tm->time.tm_hour = bcd2bin(regs[1] & 0x3F);
	tm->time.tm_mday = bcd2bin(regs[2] & 0x3F);
	tm->time.tm_wday = bcd2bin(regs[3] & 0x7);

	err = pcf8523_read(client, PCF8523_REG_CONTROL1, &value);
	if (err < 0)
		return err;
	tm->enabled = !!(value & PCF8523_CONTROL1_AIE);

	err = pcf8523_read(client, PCF8523_REG_CONTROL2, &value);
	if (err < 0)
		return err;
	tm->pending = !!(value & PCF8523_CONTROL2_AF);

	return 0;
}

static int pcf8523_irq_enable(struct device *dev, unsigned int enabled)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 value;
	int err;

	err = pcf8523_read(client, PCF8523_REG_CONTROL1, &value);
	if (err < 0)
		return err;

	value &= PCF8523_CONTROL1_AIE;

	if (enabled)
		value |= PCF8523_CONTROL1_AIE;

	err = pcf8523_write(client, PCF8523_REG_CONTROL1, value);
	if (err < 0)
		return err;

	return 0;
}

static int pcf8523_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *tm)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct i2c_msg msg;
	u8 regs[5];
	int err;

	err = pcf8523_irq_enable(dev, 0);
	if (err)
		return err;

	err = pcf8523_write(client, PCF8523_REG_CONTROL2, 0);
	if (err < 0)
		return err;

	/* The alarm has no seconds, round up to nearest minute */
	if (tm->time.tm_sec) {
		time64_t alarm_time = rtc_tm_to_time64(&tm->time);

		alarm_time += 60 - tm->time.tm_sec;
		rtc_time64_to_tm(alarm_time, &tm->time);
	}

	regs[0] = PCF8523_REG_MINUTE_ALARM;
	regs[1] = bin2bcd(tm->time.tm_min);
	regs[2] = bin2bcd(tm->time.tm_hour);
	regs[3] = bin2bcd(tm->time.tm_mday);
	regs[4] = ALARM_DIS;
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = sizeof(regs);
	msg.buf = regs;
	err = i2c_transfer(client->adapter, &msg, 1);
	if (err < 0)
		return err;

	if (tm->enabled)
		return pcf8523_irq_enable(dev, tm->enabled);

	return 0;
}

>>>>>>> upstream/android-13
#ifdef CONFIG_RTC_INTF_DEV
static int pcf8523_rtc_ioctl(struct device *dev, unsigned int cmd,
			     unsigned long arg)
{
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
=======
	unsigned int flags = 0;
	u8 value;
>>>>>>> upstream/android-13
	int ret;

	switch (cmd) {
	case RTC_VL_READ:
		ret = pcf8523_voltage_low(client);
		if (ret < 0)
			return ret;
<<<<<<< HEAD

		if (copy_to_user((void __user *)arg, &ret, sizeof(int)))
			return -EFAULT;

		return 0;
=======
		if (ret)
			flags |= RTC_VL_BACKUP_LOW;

		ret = pcf8523_read(client, PCF8523_REG_SECONDS, &value);
		if (ret < 0)
			return ret;

		if (value & PCF8523_SECONDS_OS)
			flags |= RTC_VL_DATA_INVALID;

		return put_user(flags, (unsigned int __user *)arg);

>>>>>>> upstream/android-13
	default:
		return -ENOIOCTLCMD;
	}
}
#else
#define pcf8523_rtc_ioctl NULL
#endif

static int pcf8523_rtc_read_offset(struct device *dev, long *offset)
{
	struct i2c_client *client = to_i2c_client(dev);
	int err;
	u8 value;
	s8 val;

<<<<<<< HEAD
	err = pcf8523_read(client, REG_OFFSET, &value);
=======
	err = pcf8523_read(client, PCF8523_REG_OFFSET, &value);
>>>>>>> upstream/android-13
	if (err < 0)
		return err;

	/* sign extend the 7-bit offset value */
	val = value << 1;
<<<<<<< HEAD
	*offset = (value & REG_OFFSET_MODE ? 4069 : 4340) * (val >> 1);
=======
	*offset = (value & PCF8523_OFFSET_MODE ? 4069 : 4340) * (val >> 1);
>>>>>>> upstream/android-13

	return 0;
}

static int pcf8523_rtc_set_offset(struct device *dev, long offset)
{
	struct i2c_client *client = to_i2c_client(dev);
	long reg_m0, reg_m1;
	u8 value;

	reg_m0 = clamp(DIV_ROUND_CLOSEST(offset, 4340), -64L, 63L);
	reg_m1 = clamp(DIV_ROUND_CLOSEST(offset, 4069), -64L, 63L);

	if (abs(reg_m0 * 4340 - offset) < abs(reg_m1 * 4069 - offset))
		value = reg_m0 & 0x7f;
	else
<<<<<<< HEAD
		value = (reg_m1 & 0x7f) | REG_OFFSET_MODE;

	return pcf8523_write(client, REG_OFFSET, value);
=======
		value = (reg_m1 & 0x7f) | PCF8523_OFFSET_MODE;

	return pcf8523_write(client, PCF8523_REG_OFFSET, value);
>>>>>>> upstream/android-13
}

static const struct rtc_class_ops pcf8523_rtc_ops = {
	.read_time = pcf8523_rtc_read_time,
	.set_time = pcf8523_rtc_set_time,
<<<<<<< HEAD
=======
	.read_alarm = pcf8523_rtc_read_alarm,
	.set_alarm = pcf8523_rtc_set_alarm,
	.alarm_irq_enable = pcf8523_irq_enable,
>>>>>>> upstream/android-13
	.ioctl = pcf8523_rtc_ioctl,
	.read_offset = pcf8523_rtc_read_offset,
	.set_offset = pcf8523_rtc_set_offset,
};

static int pcf8523_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
<<<<<<< HEAD
	struct pcf8523 *pcf;
=======
	struct pcf8523 *pcf8523;
	struct rtc_device *rtc;
	bool wakeup_source = false;
>>>>>>> upstream/android-13
	int err;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -ENODEV;

<<<<<<< HEAD
	pcf = devm_kzalloc(&client->dev, sizeof(*pcf), GFP_KERNEL);
	if (!pcf)
		return -ENOMEM;

=======
	pcf8523 = devm_kzalloc(&client->dev, sizeof(struct pcf8523), GFP_KERNEL);
	if (!pcf8523)
		return -ENOMEM;

	i2c_set_clientdata(client, pcf8523);
	pcf8523->client = client;

>>>>>>> upstream/android-13
	err = pcf8523_load_capacitance(client);
	if (err < 0)
		dev_warn(&client->dev, "failed to set xtal load capacitance: %d",
			 err);

	err = pcf8523_set_pm(client, 0);
	if (err < 0)
		return err;

<<<<<<< HEAD
	pcf->rtc = devm_rtc_device_register(&client->dev, DRIVER_NAME,
				       &pcf8523_rtc_ops, THIS_MODULE);
	if (IS_ERR(pcf->rtc))
		return PTR_ERR(pcf->rtc);

	i2c_set_clientdata(client, pcf);

	return 0;
=======
	rtc = devm_rtc_allocate_device(&client->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	pcf8523->rtc = rtc;
	rtc->ops = &pcf8523_rtc_ops;
	rtc->range_min = RTC_TIMESTAMP_BEGIN_2000;
	rtc->range_max = RTC_TIMESTAMP_END_2099;
	rtc->uie_unsupported = 1;

	if (client->irq > 0) {
		err = pcf8523_write(client, PCF8523_TMR_CLKOUT_CTRL, 0x38);
		if (err < 0)
			return err;

		err = devm_request_threaded_irq(&client->dev, client->irq,
						NULL, pcf8523_irq,
						IRQF_SHARED | IRQF_ONESHOT | IRQF_TRIGGER_LOW,
						dev_name(&rtc->dev), client);
		if (err)
			return err;

		dev_pm_set_wake_irq(&client->dev, client->irq);
	}

#ifdef CONFIG_OF
	wakeup_source = of_property_read_bool(client->dev.of_node, "wakeup-source");
#endif
	if (client->irq > 0 || wakeup_source)
		device_init_wakeup(&client->dev, true);

	return devm_rtc_register_device(rtc);
>>>>>>> upstream/android-13
}

static const struct i2c_device_id pcf8523_id[] = {
	{ "pcf8523", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, pcf8523_id);

#ifdef CONFIG_OF
static const struct of_device_id pcf8523_of_match[] = {
	{ .compatible = "nxp,pcf8523" },
<<<<<<< HEAD
=======
	{ .compatible = "microcrystal,rv8523" },
>>>>>>> upstream/android-13
	{ }
};
MODULE_DEVICE_TABLE(of, pcf8523_of_match);
#endif

static struct i2c_driver pcf8523_driver = {
	.driver = {
<<<<<<< HEAD
		.name = DRIVER_NAME,
=======
		.name = "rtc-pcf8523",
>>>>>>> upstream/android-13
		.of_match_table = of_match_ptr(pcf8523_of_match),
	},
	.probe = pcf8523_probe,
	.id_table = pcf8523_id,
};
module_i2c_driver(pcf8523_driver);

MODULE_AUTHOR("Thierry Reding <thierry.reding@avionic-design.de>");
MODULE_DESCRIPTION("NXP PCF8523 RTC driver");
MODULE_LICENSE("GPL v2");
