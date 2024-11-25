<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  LCD / Backlight control code for Sharp SL-6000x (tosa)
 *
 *  Copyright (c) 2005		Dirk Opfer
 *  Copyright (c) 2007,2008	Dmitry Baryshkov
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/gpio.h>
=======
#include <linux/gpio/consumer.h>
>>>>>>> upstream/android-13
#include <linux/delay.h>
#include <linux/lcd.h>
#include <linux/fb.h>

#include <asm/mach/sharpsl_param.h>

<<<<<<< HEAD
#include <mach/tosa.h>
=======
#include "tosa_bl.h"
>>>>>>> upstream/android-13

#define POWER_IS_ON(pwr)	((pwr) <= FB_BLANK_NORMAL)

#define TG_REG0_VQV	0x0001
#define TG_REG0_COLOR	0x0002
#define TG_REG0_UD	0x0004
#define TG_REG0_LR	0x0008

<<<<<<< HEAD
=======
/*
 * Timing Generator
 */
#define TG_PNLCTL 	0x00
#define TG_TPOSCTL 	0x01
#define TG_DUTYCTL 	0x02
#define TG_GPOSR 	0x03
#define TG_GPODR1 	0x04
#define TG_GPODR2 	0x05
#define TG_PINICTL 	0x06
#define TG_HPOSCTL 	0x07


>>>>>>> upstream/android-13
#define	DAC_BASE	0x4e

struct tosa_lcd_data {
	struct spi_device *spi;
	struct lcd_device *lcd;
	struct i2c_client *i2c;
<<<<<<< HEAD
=======
	struct gpio_desc *gpiod_tg;
>>>>>>> upstream/android-13

	int lcd_power;
	bool is_vga;
};

static int tosa_tg_send(struct spi_device *spi, int adrs, uint8_t data)
{
	u8 buf[1];
	struct spi_message msg;
	struct spi_transfer xfer = {
		.len		= 1,
		.cs_change	= 0,
		.tx_buf		= buf,
	};

	buf[0] = ((adrs & 0x07) << 5) | (data & 0x1f);
	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);

	return spi_sync(spi, &msg);
}

int tosa_bl_enable(struct spi_device *spi, int enable)
{
	/* bl_enable GP04=1 otherwise GP04=0*/
	return tosa_tg_send(spi, TG_GPODR2, enable ? 0x01 : 0x00);
}
EXPORT_SYMBOL(tosa_bl_enable);

static void tosa_lcd_tg_init(struct tosa_lcd_data *data)
{
	/* TG on */
<<<<<<< HEAD
	gpio_set_value(TOSA_GPIO_TG_ON, 0);
=======
	gpiod_set_value(data->gpiod_tg, 0);
>>>>>>> upstream/android-13

	mdelay(60);

	/* delayed 0clk TCTL signal for VGA */
	tosa_tg_send(data->spi, TG_TPOSCTL, 0x00);
	/* GPOS0=powercontrol, GPOS1=GPIO, GPOS2=TCTL */
	tosa_tg_send(data->spi, TG_GPOSR, 0x02);
}

static void tosa_lcd_tg_on(struct tosa_lcd_data *data)
{
	struct spi_device *spi = data->spi;
	int value = TG_REG0_COLOR | TG_REG0_UD | TG_REG0_LR;

	if (data->is_vga)
		value |= TG_REG0_VQV;

	tosa_tg_send(spi, TG_PNLCTL, value);

	/* TG LCD pannel power up */
	tosa_tg_send(spi, TG_PINICTL, 0x4);
	mdelay(50);

	/* TG LCD GVSS */
	tosa_tg_send(spi, TG_PINICTL, 0x0);

<<<<<<< HEAD
	if (!data->i2c) {
=======
	if (IS_ERR_OR_NULL(data->i2c)) {
>>>>>>> upstream/android-13
		/*
		 * after the pannel is powered up the first time,
		 * we can access the i2c bus so probe for the DAC
		 */
		struct i2c_adapter *adap = i2c_get_adapter(0);
		struct i2c_board_info info = {
<<<<<<< HEAD
=======
			.dev_name = "tosa-bl",
>>>>>>> upstream/android-13
			.type	= "tosa-bl",
			.addr	= DAC_BASE,
			.platform_data = data->spi,
		};
<<<<<<< HEAD
		data->i2c = i2c_new_device(adap, &info);
=======
		data->i2c = i2c_new_client_device(adap, &info);
>>>>>>> upstream/android-13
	}
}

static void tosa_lcd_tg_off(struct tosa_lcd_data *data)
{
	struct spi_device *spi = data->spi;

	/* TG LCD VHSA off */
	tosa_tg_send(spi, TG_PINICTL, 0x4);
	mdelay(50);

	/* TG LCD signal off */
	tosa_tg_send(spi, TG_PINICTL, 0x6);
	mdelay(50);

	/* TG Off */
<<<<<<< HEAD
	gpio_set_value(TOSA_GPIO_TG_ON, 1);
=======
	gpiod_set_value(data->gpiod_tg, 1);
>>>>>>> upstream/android-13
	mdelay(100);
}

int tosa_lcd_set_power(struct lcd_device *lcd, int power)
{
	struct tosa_lcd_data *data = lcd_get_data(lcd);

	if (POWER_IS_ON(power) && !POWER_IS_ON(data->lcd_power))
		tosa_lcd_tg_on(data);

	if (!POWER_IS_ON(power) && POWER_IS_ON(data->lcd_power))
		tosa_lcd_tg_off(data);

	data->lcd_power = power;
	return 0;
}

static int tosa_lcd_get_power(struct lcd_device *lcd)
{
	struct tosa_lcd_data *data = lcd_get_data(lcd);

	return data->lcd_power;
}

static int tosa_lcd_set_mode(struct lcd_device *lcd, struct fb_videomode *mode)
{
	struct tosa_lcd_data *data = lcd_get_data(lcd);

	if (mode->xres == 320 || mode->yres == 320)
		data->is_vga = false;
	else
		data->is_vga = true;

	if (POWER_IS_ON(data->lcd_power))
		tosa_lcd_tg_on(data);

	return 0;
}

static struct lcd_ops tosa_lcd_ops = {
	.set_power = tosa_lcd_set_power,
	.get_power = tosa_lcd_get_power,
	.set_mode = tosa_lcd_set_mode,
};

static int tosa_lcd_probe(struct spi_device *spi)
{
	int ret;
	struct tosa_lcd_data *data;

	data = devm_kzalloc(&spi->dev, sizeof(struct tosa_lcd_data),
				GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->is_vga = true; /* default to VGA mode */

	/*
	 * bits_per_word cannot be configured in platform data
	 */
	spi->bits_per_word = 8;

	ret = spi_setup(spi);
	if (ret < 0)
		return ret;

	data->spi = spi;
	spi_set_drvdata(spi, data);

<<<<<<< HEAD
	ret = devm_gpio_request_one(&spi->dev, TOSA_GPIO_TG_ON,
				GPIOF_OUT_INIT_LOW, "tg #pwr");
	if (ret < 0)
		return ret;
=======
	data->gpiod_tg = devm_gpiod_get(&spi->dev, "tg #pwr", GPIOD_OUT_LOW);
	if (IS_ERR(data->gpiod_tg))
		return PTR_ERR(data->gpiod_tg);
>>>>>>> upstream/android-13

	mdelay(60);

	tosa_lcd_tg_init(data);

	tosa_lcd_tg_on(data);

	data->lcd = devm_lcd_device_register(&spi->dev, "tosa-lcd", &spi->dev,
					data, &tosa_lcd_ops);

	if (IS_ERR(data->lcd)) {
		ret = PTR_ERR(data->lcd);
		data->lcd = NULL;
		goto err_register;
	}

	return 0;

err_register:
	tosa_lcd_tg_off(data);
	return ret;
}

static int tosa_lcd_remove(struct spi_device *spi)
{
	struct tosa_lcd_data *data = spi_get_drvdata(spi);

<<<<<<< HEAD
	if (data->i2c)
		i2c_unregister_device(data->i2c);
=======
	i2c_unregister_device(data->i2c);
>>>>>>> upstream/android-13

	tosa_lcd_tg_off(data);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int tosa_lcd_suspend(struct device *dev)
{
	struct tosa_lcd_data *data = dev_get_drvdata(dev);

	tosa_lcd_tg_off(data);

	return 0;
}

static int tosa_lcd_resume(struct device *dev)
{
	struct tosa_lcd_data *data = dev_get_drvdata(dev);

	tosa_lcd_tg_init(data);
	if (POWER_IS_ON(data->lcd_power))
		tosa_lcd_tg_on(data);
	else
		tosa_lcd_tg_off(data);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(tosa_lcd_pm_ops, tosa_lcd_suspend, tosa_lcd_resume);

static struct spi_driver tosa_lcd_driver = {
	.driver = {
		.name		= "tosa-lcd",
		.pm		= &tosa_lcd_pm_ops,
	},
	.probe		= tosa_lcd_probe,
	.remove		= tosa_lcd_remove,
};

module_spi_driver(tosa_lcd_driver);

MODULE_AUTHOR("Dmitry Baryshkov");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("LCD/Backlight control for Sharp SL-6000 PDA");
MODULE_ALIAS("spi:tosa-lcd");
