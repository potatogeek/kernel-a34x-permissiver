<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * An SPI driver for the Philips PCF2123 RTC
 * Copyright 2009 Cyber Switching, Inc.
 *
 * Author: Chris Verges <chrisv@cyberswitching.com>
 * Maintainers: http://www.cyberswitching.com
 *
 * based on the RS5C348 driver in this same directory.
 *
 * Thanks to Christian Pellegrin <chripell@fsfe.org> for
 * the sysfs contributions to this driver.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * Please note that the CS is active high, so platform data
 * should look something like:
 *
 * static struct spi_board_info ek_spi_devices[] = {
 *	...
 *	{
 *		.modalias		= "rtc-pcf2123",
 *		.chip_select		= 1,
 *		.controller_data	= (void *)AT91_PIN_PA10,
 *		.max_speed_hz		= 1000 * 1000,
 *		.mode			= SPI_CS_HIGH,
 *		.bus_num		= 0,
 *	},
 *	...
 *};
<<<<<<< HEAD
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/bcd.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/rtc.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/sysfs.h>
=======
#include <linux/regmap.h>
>>>>>>> upstream/android-13

/* REGISTERS */
#define PCF2123_REG_CTRL1	(0x00)	/* Control Register 1 */
#define PCF2123_REG_CTRL2	(0x01)	/* Control Register 2 */
#define PCF2123_REG_SC		(0x02)	/* datetime */
#define PCF2123_REG_MN		(0x03)
#define PCF2123_REG_HR		(0x04)
#define PCF2123_REG_DM		(0x05)
#define PCF2123_REG_DW		(0x06)
#define PCF2123_REG_MO		(0x07)
#define PCF2123_REG_YR		(0x08)
#define PCF2123_REG_ALRM_MN	(0x09)	/* Alarm Registers */
#define PCF2123_REG_ALRM_HR	(0x0a)
#define PCF2123_REG_ALRM_DM	(0x0b)
#define PCF2123_REG_ALRM_DW	(0x0c)
#define PCF2123_REG_OFFSET	(0x0d)	/* Clock Rate Offset Register */
#define PCF2123_REG_TMR_CLKOUT	(0x0e)	/* Timer Registers */
#define PCF2123_REG_CTDWN_TMR	(0x0f)

/* PCF2123_REG_CTRL1 BITS */
#define CTRL1_CLEAR		(0)	/* Clear */
#define CTRL1_CORR_INT		BIT(1)	/* Correction irq enable */
#define CTRL1_12_HOUR		BIT(2)	/* 12 hour time */
#define CTRL1_SW_RESET	(BIT(3) | BIT(4) | BIT(6))	/* Software reset */
#define CTRL1_STOP		BIT(5)	/* Stop the clock */
#define CTRL1_EXT_TEST		BIT(7)	/* External clock test mode */

/* PCF2123_REG_CTRL2 BITS */
#define CTRL2_TIE		BIT(0)	/* Countdown timer irq enable */
#define CTRL2_AIE		BIT(1)	/* Alarm irq enable */
#define CTRL2_TF		BIT(2)	/* Countdown timer flag */
#define CTRL2_AF		BIT(3)	/* Alarm flag */
#define CTRL2_TI_TP		BIT(4)	/* Irq pin generates pulse */
#define CTRL2_MSF		BIT(5)	/* Minute or second irq flag */
#define CTRL2_SI		BIT(6)	/* Second irq enable */
#define CTRL2_MI		BIT(7)	/* Minute irq enable */

/* PCF2123_REG_SC BITS */
#define OSC_HAS_STOPPED		BIT(7)	/* Clock has been stopped */

/* PCF2123_REG_ALRM_XX BITS */
<<<<<<< HEAD
#define ALRM_ENABLE		BIT(7)	/* MN, HR, DM, or DW alarm enable */
=======
#define ALRM_DISABLE		BIT(7)	/* MN, HR, DM, or DW alarm matching */
>>>>>>> upstream/android-13

/* PCF2123_REG_TMR_CLKOUT BITS */
#define CD_TMR_4096KHZ		(0)	/* 4096 KHz countdown timer */
#define CD_TMR_64HZ		(1)	/* 64 Hz countdown timer */
#define CD_TMR_1HZ		(2)	/* 1 Hz countdown timer */
#define CD_TMR_60th_HZ		(3)	/* 60th Hz countdown timer */
#define CD_TMR_TE		BIT(3)	/* Countdown timer enable */

/* PCF2123_REG_OFFSET BITS */
#define OFFSET_SIGN_BIT		6	/* 2's complement sign bit */
#define OFFSET_COARSE		BIT(7)	/* Coarse mode offset */
#define OFFSET_STEP		(2170)	/* Offset step in parts per billion */
<<<<<<< HEAD
=======
#define OFFSET_MASK		GENMASK(6, 0)	/* Offset value */
>>>>>>> upstream/android-13

/* READ/WRITE ADDRESS BITS */
#define PCF2123_WRITE		BIT(4)
#define PCF2123_READ		(BIT(4) | BIT(7))


static struct spi_driver pcf2123_driver;

<<<<<<< HEAD
struct pcf2123_sysfs_reg {
	struct device_attribute attr;
	char name[2];
};

struct pcf2123_plat_data {
	struct rtc_device *rtc;
	struct pcf2123_sysfs_reg regs[16];
};

/*
 * Causes a 30 nanosecond delay to ensure that the PCF2123 chip select
 * is released properly after an SPI write.  This function should be
 * called after EVERY read/write call over SPI.
 */
static inline void pcf2123_delay_trec(void)
{
	ndelay(30);
}

static int pcf2123_read(struct device *dev, u8 reg, u8 *rxbuf, size_t size)
{
	struct spi_device *spi = to_spi_device(dev);
	int ret;

	reg |= PCF2123_READ;
	ret = spi_write_then_read(spi, &reg, 1, rxbuf, size);
	pcf2123_delay_trec();

	return ret;
}

static int pcf2123_write(struct device *dev, u8 *txbuf, size_t size)
{
	struct spi_device *spi = to_spi_device(dev);
	int ret;

	txbuf[0] |= PCF2123_WRITE;
	ret = spi_write(spi, txbuf, size);
	pcf2123_delay_trec();

	return ret;
}

static int pcf2123_write_reg(struct device *dev, u8 reg, u8 val)
{
	u8 txbuf[2];

	txbuf[0] = reg;
	txbuf[1] = val;
	return pcf2123_write(dev, txbuf, sizeof(txbuf));
}

static ssize_t pcf2123_show(struct device *dev, struct device_attribute *attr,
			    char *buffer)
{
	struct pcf2123_sysfs_reg *r;
	u8 rxbuf[1];
	unsigned long reg;
	int ret;

	r = container_of(attr, struct pcf2123_sysfs_reg, attr);

	ret = kstrtoul(r->name, 16, &reg);
	if (ret)
		return ret;

	ret = pcf2123_read(dev, reg, rxbuf, 1);
	if (ret < 0)
		return -EIO;

	return sprintf(buffer, "0x%x\n", rxbuf[0]);
}

static ssize_t pcf2123_store(struct device *dev, struct device_attribute *attr,
			     const char *buffer, size_t count)
{
	struct pcf2123_sysfs_reg *r;
	unsigned long reg;
	unsigned long val;

	int ret;

	r = container_of(attr, struct pcf2123_sysfs_reg, attr);

	ret = kstrtoul(r->name, 16, &reg);
	if (ret)
		return ret;

	ret = kstrtoul(buffer, 10, &val);
	if (ret)
		return ret;

	ret = pcf2123_write_reg(dev, reg, val);
	if (ret < 0)
		return -EIO;
	return count;
}

static int pcf2123_read_offset(struct device *dev, long *offset)
{
	int ret;
	s8 reg;

	ret = pcf2123_read(dev, PCF2123_REG_OFFSET, &reg, 1);
	if (ret < 0)
		return ret;

	if (reg & OFFSET_COARSE)
		reg <<= 1; /* multiply by 2 and sign extend */
	else
		reg = sign_extend32(reg, OFFSET_SIGN_BIT);

	*offset = ((long)reg) * OFFSET_STEP;
=======
struct pcf2123_data {
	struct rtc_device *rtc;
	struct regmap *map;
};

static const struct regmap_config pcf2123_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.read_flag_mask = PCF2123_READ,
	.write_flag_mask = PCF2123_WRITE,
	.max_register = PCF2123_REG_CTDWN_TMR,
};

static int pcf2123_read_offset(struct device *dev, long *offset)
{
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	int ret, val;
	unsigned int reg;

	ret = regmap_read(pcf2123->map, PCF2123_REG_OFFSET, &reg);
	if (ret)
		return ret;

	val = sign_extend32((reg & OFFSET_MASK), OFFSET_SIGN_BIT);

	if (reg & OFFSET_COARSE)
		val *= 2;

	*offset = ((long)val) * OFFSET_STEP;
>>>>>>> upstream/android-13

	return 0;
}

/*
 * The offset register is a 7 bit signed value with a coarse bit in bit 7.
 * The main difference between the two is normal offset adjusts the first
 * second of n minutes every other hour, with 61, 62 and 63 being shoved
 * into the 60th minute.
 * The coarse adjustment does the same, but every hour.
 * the two overlap, with every even normal offset value corresponding
 * to a coarse offset. Based on this algorithm, it seems that despite the
 * name, coarse offset is a better fit for overlapping values.
 */
static int pcf2123_set_offset(struct device *dev, long offset)
{
<<<<<<< HEAD
=======
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
>>>>>>> upstream/android-13
	s8 reg;

	if (offset > OFFSET_STEP * 127)
		reg = 127;
	else if (offset < OFFSET_STEP * -128)
		reg = -128;
	else
<<<<<<< HEAD
		reg = (s8)((offset + (OFFSET_STEP >> 1)) / OFFSET_STEP);
=======
		reg = DIV_ROUND_CLOSEST(offset, OFFSET_STEP);
>>>>>>> upstream/android-13

	/* choose fine offset only for odd values in the normal range */
	if (reg & 1 && reg <= 63 && reg >= -64) {
		/* Normal offset. Clear the coarse bit */
		reg &= ~OFFSET_COARSE;
	} else {
		/* Coarse offset. Divide by 2 and set the coarse bit */
		reg >>= 1;
		reg |= OFFSET_COARSE;
	}

<<<<<<< HEAD
	return pcf2123_write_reg(dev, PCF2123_REG_OFFSET, reg);
=======
	return regmap_write(pcf2123->map, PCF2123_REG_OFFSET, (unsigned int)reg);
>>>>>>> upstream/android-13
}

static int pcf2123_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	u8 rxbuf[7];
	int ret;

	ret = pcf2123_read(dev, PCF2123_REG_SC, rxbuf, sizeof(rxbuf));
	if (ret < 0)
=======
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	u8 rxbuf[7];
	int ret;

	ret = regmap_bulk_read(pcf2123->map, PCF2123_REG_SC, rxbuf,
				sizeof(rxbuf));
	if (ret)
>>>>>>> upstream/android-13
		return ret;

	if (rxbuf[0] & OSC_HAS_STOPPED) {
		dev_info(dev, "clock was stopped. Time is not valid\n");
		return -EINVAL;
	}

	tm->tm_sec = bcd2bin(rxbuf[0] & 0x7F);
	tm->tm_min = bcd2bin(rxbuf[1] & 0x7F);
	tm->tm_hour = bcd2bin(rxbuf[2] & 0x3F); /* rtc hr 0-23 */
	tm->tm_mday = bcd2bin(rxbuf[3] & 0x3F);
	tm->tm_wday = rxbuf[4] & 0x07;
	tm->tm_mon = bcd2bin(rxbuf[5] & 0x1F) - 1; /* rtc mn 1-12 */
<<<<<<< HEAD
	tm->tm_year = bcd2bin(rxbuf[6]);
	if (tm->tm_year < 70)
		tm->tm_year += 100;	/* assume we are in 1970...2069 */

	dev_dbg(dev, "%s: tm is secs=%d, mins=%d, hours=%d, "
			"mday=%d, mon=%d, year=%d, wday=%d\n",
			__func__,
			tm->tm_sec, tm->tm_min, tm->tm_hour,
			tm->tm_mday, tm->tm_mon, tm->tm_year, tm->tm_wday);
=======
	tm->tm_year = bcd2bin(rxbuf[6]) + 100;

	dev_dbg(dev, "%s: tm is %ptR\n", __func__, tm);
>>>>>>> upstream/android-13

	return 0;
}

static int pcf2123_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	u8 txbuf[8];
	int ret;

	dev_dbg(dev, "%s: tm is secs=%d, mins=%d, hours=%d, "
			"mday=%d, mon=%d, year=%d, wday=%d\n",
			__func__,
			tm->tm_sec, tm->tm_min, tm->tm_hour,
			tm->tm_mday, tm->tm_mon, tm->tm_year, tm->tm_wday);

	/* Stop the counter first */
	ret = pcf2123_write_reg(dev, PCF2123_REG_CTRL1, CTRL1_STOP);
	if (ret < 0)
		return ret;

	/* Set the new time */
	txbuf[0] = PCF2123_REG_SC;
	txbuf[1] = bin2bcd(tm->tm_sec & 0x7F);
	txbuf[2] = bin2bcd(tm->tm_min & 0x7F);
	txbuf[3] = bin2bcd(tm->tm_hour & 0x3F);
	txbuf[4] = bin2bcd(tm->tm_mday & 0x3F);
	txbuf[5] = tm->tm_wday & 0x07;
	txbuf[6] = bin2bcd((tm->tm_mon + 1) & 0x1F); /* rtc mn 1-12 */
	txbuf[7] = bin2bcd(tm->tm_year < 100 ? tm->tm_year : tm->tm_year - 100);

	ret = pcf2123_write(dev, txbuf, sizeof(txbuf));
	if (ret < 0)
		return ret;

	/* Start the counter */
	ret = pcf2123_write_reg(dev, PCF2123_REG_CTRL1, CTRL1_CLEAR);
	if (ret < 0)
=======
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	u8 txbuf[7];
	int ret;

	dev_dbg(dev, "%s: tm is %ptR\n", __func__, tm);

	/* Stop the counter first */
	ret = regmap_write(pcf2123->map, PCF2123_REG_CTRL1, CTRL1_STOP);
	if (ret)
		return ret;

	/* Set the new time */
	txbuf[0] = bin2bcd(tm->tm_sec & 0x7F);
	txbuf[1] = bin2bcd(tm->tm_min & 0x7F);
	txbuf[2] = bin2bcd(tm->tm_hour & 0x3F);
	txbuf[3] = bin2bcd(tm->tm_mday & 0x3F);
	txbuf[4] = tm->tm_wday & 0x07;
	txbuf[5] = bin2bcd((tm->tm_mon + 1) & 0x1F); /* rtc mn 1-12 */
	txbuf[6] = bin2bcd(tm->tm_year - 100);

	ret = regmap_bulk_write(pcf2123->map, PCF2123_REG_SC, txbuf,
				sizeof(txbuf));
	if (ret)
		return ret;

	/* Start the counter */
	ret = regmap_write(pcf2123->map, PCF2123_REG_CTRL1, CTRL1_CLEAR);
	if (ret)
>>>>>>> upstream/android-13
		return ret;

	return 0;
}

<<<<<<< HEAD
static int pcf2123_reset(struct device *dev)
{
	int ret;
	u8  rxbuf[2];

	ret = pcf2123_write_reg(dev, PCF2123_REG_CTRL1, CTRL1_SW_RESET);
	if (ret < 0)
=======
static int pcf2123_rtc_alarm_irq_enable(struct device *dev, unsigned int en)
{
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);

	return regmap_update_bits(pcf2123->map, PCF2123_REG_CTRL2, CTRL2_AIE,
				  en ? CTRL2_AIE : 0);
}

static int pcf2123_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	u8 rxbuf[4];
	int ret;
	unsigned int val = 0;

	ret = regmap_bulk_read(pcf2123->map, PCF2123_REG_ALRM_MN, rxbuf,
				sizeof(rxbuf));
	if (ret)
		return ret;

	alm->time.tm_min = bcd2bin(rxbuf[0] & 0x7F);
	alm->time.tm_hour = bcd2bin(rxbuf[1] & 0x3F);
	alm->time.tm_mday = bcd2bin(rxbuf[2] & 0x3F);
	alm->time.tm_wday = bcd2bin(rxbuf[3] & 0x07);

	dev_dbg(dev, "%s: alm is %ptR\n", __func__, &alm->time);

	ret = regmap_read(pcf2123->map, PCF2123_REG_CTRL2, &val);
	if (ret)
		return ret;

	alm->enabled = !!(val & CTRL2_AIE);

	return 0;
}

static int pcf2123_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	u8 txbuf[4];
	int ret;

	dev_dbg(dev, "%s: alm is %ptR\n", __func__, &alm->time);

	/* Disable alarm interrupt */
	ret = regmap_update_bits(pcf2123->map, PCF2123_REG_CTRL2, CTRL2_AIE, 0);
	if (ret)
		return ret;

	/* Ensure alarm flag is clear */
	ret = regmap_update_bits(pcf2123->map, PCF2123_REG_CTRL2, CTRL2_AF, 0);
	if (ret)
		return ret;

	/* Set new alarm */
	txbuf[0] = bin2bcd(alm->time.tm_min & 0x7F);
	txbuf[1] = bin2bcd(alm->time.tm_hour & 0x3F);
	txbuf[2] = bin2bcd(alm->time.tm_mday & 0x3F);
	txbuf[3] = ALRM_DISABLE;

	ret = regmap_bulk_write(pcf2123->map, PCF2123_REG_ALRM_MN, txbuf,
				sizeof(txbuf));
	if (ret)
		return ret;

	return pcf2123_rtc_alarm_irq_enable(dev, alm->enabled);
}

static irqreturn_t pcf2123_rtc_irq(int irq, void *dev)
{
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	unsigned int val = 0;
	int ret = IRQ_NONE;

	rtc_lock(pcf2123->rtc);
	regmap_read(pcf2123->map, PCF2123_REG_CTRL2, &val);

	/* Alarm? */
	if (val & CTRL2_AF) {
		ret = IRQ_HANDLED;

		/* Clear alarm flag */
		regmap_update_bits(pcf2123->map, PCF2123_REG_CTRL2, CTRL2_AF, 0);

		rtc_update_irq(pcf2123->rtc, 1, RTC_IRQF | RTC_AF);
	}

	rtc_unlock(pcf2123->rtc);

	return ret;
}

static int pcf2123_reset(struct device *dev)
{
	struct pcf2123_data *pcf2123 = dev_get_drvdata(dev);
	int ret;
	unsigned int val = 0;

	ret = regmap_write(pcf2123->map, PCF2123_REG_CTRL1, CTRL1_SW_RESET);
	if (ret)
>>>>>>> upstream/android-13
		return ret;

	/* Stop the counter */
	dev_dbg(dev, "stopping RTC\n");
<<<<<<< HEAD
	ret = pcf2123_write_reg(dev, PCF2123_REG_CTRL1, CTRL1_STOP);
	if (ret < 0)
=======
	ret = regmap_write(pcf2123->map, PCF2123_REG_CTRL1, CTRL1_STOP);
	if (ret)
>>>>>>> upstream/android-13
		return ret;

	/* See if the counter was actually stopped */
	dev_dbg(dev, "checking for presence of RTC\n");
<<<<<<< HEAD
	ret = pcf2123_read(dev, PCF2123_REG_CTRL1, rxbuf, sizeof(rxbuf));
	if (ret < 0)
		return ret;

	dev_dbg(dev, "received data from RTC (0x%02X 0x%02X)\n",
		rxbuf[0], rxbuf[1]);
	if (!(rxbuf[0] & CTRL1_STOP))
		return -ENODEV;

	/* Start the counter */
	ret = pcf2123_write_reg(dev, PCF2123_REG_CTRL1, CTRL1_CLEAR);
	if (ret < 0)
=======
	ret = regmap_read(pcf2123->map, PCF2123_REG_CTRL1, &val);
	if (ret)
		return ret;

	dev_dbg(dev, "received data from RTC (0x%08X)\n", val);
	if (!(val & CTRL1_STOP))
		return -ENODEV;

	/* Start the counter */
	ret = regmap_write(pcf2123->map, PCF2123_REG_CTRL1, CTRL1_CLEAR);
	if (ret)
>>>>>>> upstream/android-13
		return ret;

	return 0;
}

static const struct rtc_class_ops pcf2123_rtc_ops = {
	.read_time	= pcf2123_rtc_read_time,
	.set_time	= pcf2123_rtc_set_time,
	.read_offset	= pcf2123_read_offset,
	.set_offset	= pcf2123_set_offset,
<<<<<<< HEAD

=======
	.read_alarm	= pcf2123_rtc_read_alarm,
	.set_alarm	= pcf2123_rtc_set_alarm,
	.alarm_irq_enable = pcf2123_rtc_alarm_irq_enable,
>>>>>>> upstream/android-13
};

static int pcf2123_probe(struct spi_device *spi)
{
	struct rtc_device *rtc;
	struct rtc_time tm;
<<<<<<< HEAD
	struct pcf2123_plat_data *pdata;
	int ret, i;

	pdata = devm_kzalloc(&spi->dev, sizeof(struct pcf2123_plat_data),
				GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;
	spi->dev.platform_data = pdata;
=======
	struct pcf2123_data *pcf2123;
	int ret = 0;

	pcf2123 = devm_kzalloc(&spi->dev, sizeof(struct pcf2123_data),
				GFP_KERNEL);
	if (!pcf2123)
		return -ENOMEM;

	dev_set_drvdata(&spi->dev, pcf2123);

	pcf2123->map = devm_regmap_init_spi(spi, &pcf2123_regmap_config);
	if (IS_ERR(pcf2123->map)) {
		dev_err(&spi->dev, "regmap init failed.\n");
		return PTR_ERR(pcf2123->map);
	}
>>>>>>> upstream/android-13

	ret = pcf2123_rtc_read_time(&spi->dev, &tm);
	if (ret < 0) {
		ret = pcf2123_reset(&spi->dev);
		if (ret < 0) {
			dev_err(&spi->dev, "chip not found\n");
<<<<<<< HEAD
			goto kfree_exit;
=======
			return ret;
>>>>>>> upstream/android-13
		}
	}

	dev_info(&spi->dev, "spiclk %u KHz.\n",
			(spi->max_speed_hz + 500) / 1000);

	/* Finalize the initialization */
<<<<<<< HEAD
	rtc = devm_rtc_device_register(&spi->dev, pcf2123_driver.driver.name,
			&pcf2123_rtc_ops, THIS_MODULE);

	if (IS_ERR(rtc)) {
		dev_err(&spi->dev, "failed to register.\n");
		ret = PTR_ERR(rtc);
		goto kfree_exit;
	}

	pdata->rtc = rtc;

	for (i = 0; i < 16; i++) {
		sysfs_attr_init(&pdata->regs[i].attr.attr);
		sprintf(pdata->regs[i].name, "%1x", i);
		pdata->regs[i].attr.attr.mode = S_IRUGO | S_IWUSR;
		pdata->regs[i].attr.attr.name = pdata->regs[i].name;
		pdata->regs[i].attr.show = pcf2123_show;
		pdata->regs[i].attr.store = pcf2123_store;
		ret = device_create_file(&spi->dev, &pdata->regs[i].attr);
		if (ret) {
			dev_err(&spi->dev, "Unable to create sysfs %s\n",
				pdata->regs[i].name);
			goto sysfs_exit;
		}
	}

	return 0;

sysfs_exit:
	for (i--; i >= 0; i--)
		device_remove_file(&spi->dev, &pdata->regs[i].attr);

kfree_exit:
	spi->dev.platform_data = NULL;
	return ret;
}

static int pcf2123_remove(struct spi_device *spi)
{
	struct pcf2123_plat_data *pdata = dev_get_platdata(&spi->dev);
	int i;

	if (pdata) {
		for (i = 0; i < 16; i++)
			if (pdata->regs[i].name[0])
				device_remove_file(&spi->dev,
						   &pdata->regs[i].attr);
	}
=======
	rtc = devm_rtc_allocate_device(&spi->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	pcf2123->rtc = rtc;

	/* Register alarm irq */
	if (spi->irq > 0) {
		ret = devm_request_threaded_irq(&spi->dev, spi->irq, NULL,
				pcf2123_rtc_irq,
				IRQF_TRIGGER_LOW | IRQF_ONESHOT,
				pcf2123_driver.driver.name, &spi->dev);
		if (!ret)
			device_init_wakeup(&spi->dev, true);
		else
			dev_err(&spi->dev, "could not request irq.\n");
	}

	/* The PCF2123's alarm only has minute accuracy. Must add timer
	 * support to this driver to generate interrupts more than once
	 * per minute.
	 */
	rtc->uie_unsupported = 1;
	rtc->ops = &pcf2123_rtc_ops;
	rtc->range_min = RTC_TIMESTAMP_BEGIN_2000;
	rtc->range_max = RTC_TIMESTAMP_END_2099;
	rtc->set_start_time = true;

	ret = devm_rtc_register_device(rtc);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id pcf2123_dt_ids[] = {
<<<<<<< HEAD
=======
	{ .compatible = "nxp,pcf2123", },
	{ .compatible = "microcrystal,rv2123", },
	/* Deprecated, do not use */
>>>>>>> upstream/android-13
	{ .compatible = "nxp,rtc-pcf2123", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, pcf2123_dt_ids);
#endif

<<<<<<< HEAD
=======
static const struct spi_device_id pcf2123_spi_ids[] = {
	{ .name = "pcf2123", },
	{ .name = "rv2123", },
	{ .name = "rtc-pcf2123", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(spi, pcf2123_spi_ids);

>>>>>>> upstream/android-13
static struct spi_driver pcf2123_driver = {
	.driver	= {
			.name	= "rtc-pcf2123",
			.of_match_table = of_match_ptr(pcf2123_dt_ids),
	},
	.probe	= pcf2123_probe,
<<<<<<< HEAD
	.remove	= pcf2123_remove,
=======
	.id_table = pcf2123_spi_ids,
>>>>>>> upstream/android-13
};

module_spi_driver(pcf2123_driver);

MODULE_AUTHOR("Chris Verges <chrisv@cyberswitching.com>");
MODULE_DESCRIPTION("NXP PCF2123 RTC driver");
MODULE_LICENSE("GPL");
