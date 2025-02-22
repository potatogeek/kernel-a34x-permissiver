<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * GPIO driver for TI TPS65912x PMICs
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *	Andrew F. Davis <afd@ti.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether expressed or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
=======
>>>>>>> upstream/android-13
 * Based on the Arizona GPIO driver and the previous TPS65912 driver by
 * Margarita Olaya Cabrera <magi@slimlogic.co.uk>
 */

<<<<<<< HEAD
#include <linux/gpio.h>
=======
#include <linux/gpio/driver.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/platform_device.h>

#include <linux/mfd/tps65912.h>

struct tps65912_gpio {
	struct gpio_chip gpio_chip;
	struct tps65912 *tps;
};

static int tps65912_gpio_get_direction(struct gpio_chip *gc,
				       unsigned offset)
{
	struct tps65912_gpio *gpio = gpiochip_get_data(gc);

	int ret, val;

	ret = regmap_read(gpio->tps->regmap, TPS65912_GPIO1 + offset, &val);
	if (ret)
		return ret;

	if (val & GPIO_CFG_MASK)
<<<<<<< HEAD
		return GPIOF_DIR_OUT;
	else
		return GPIOF_DIR_IN;
=======
		return GPIO_LINE_DIRECTION_OUT;
	else
		return GPIO_LINE_DIRECTION_IN;
>>>>>>> upstream/android-13
}

static int tps65912_gpio_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct tps65912_gpio *gpio = gpiochip_get_data(gc);

	return regmap_update_bits(gpio->tps->regmap, TPS65912_GPIO1 + offset,
				  GPIO_CFG_MASK, 0);
}

static int tps65912_gpio_direction_output(struct gpio_chip *gc,
					  unsigned offset, int value)
{
	struct tps65912_gpio *gpio = gpiochip_get_data(gc);

	/* Set the initial value */
	regmap_update_bits(gpio->tps->regmap, TPS65912_GPIO1 + offset,
			   GPIO_SET_MASK, value ? GPIO_SET_MASK : 0);

	return regmap_update_bits(gpio->tps->regmap, TPS65912_GPIO1 + offset,
				  GPIO_CFG_MASK, GPIO_CFG_MASK);
}

static int tps65912_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	struct tps65912_gpio *gpio = gpiochip_get_data(gc);
	int ret, val;

	ret = regmap_read(gpio->tps->regmap, TPS65912_GPIO1 + offset, &val);
	if (ret)
		return ret;

	if (val & GPIO_STS_MASK)
		return 1;

	return 0;
}

static void tps65912_gpio_set(struct gpio_chip *gc, unsigned offset,
			      int value)
{
	struct tps65912_gpio *gpio = gpiochip_get_data(gc);

	regmap_update_bits(gpio->tps->regmap, TPS65912_GPIO1 + offset,
			   GPIO_SET_MASK, value ? GPIO_SET_MASK : 0);
}

static const struct gpio_chip template_chip = {
	.label			= "tps65912-gpio",
	.owner			= THIS_MODULE,
	.get_direction		= tps65912_gpio_get_direction,
	.direction_input	= tps65912_gpio_direction_input,
	.direction_output	= tps65912_gpio_direction_output,
	.get			= tps65912_gpio_get,
	.set			= tps65912_gpio_set,
	.base			= -1,
	.ngpio			= 5,
	.can_sleep		= true,
};

static int tps65912_gpio_probe(struct platform_device *pdev)
{
	struct tps65912 *tps = dev_get_drvdata(pdev->dev.parent);
	struct tps65912_gpio *gpio;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> upstream/android-13

	gpio = devm_kzalloc(&pdev->dev, sizeof(*gpio), GFP_KERNEL);
	if (!gpio)
		return -ENOMEM;

	gpio->tps = dev_get_drvdata(pdev->dev.parent);
	gpio->gpio_chip = template_chip;
	gpio->gpio_chip.parent = tps->dev;

<<<<<<< HEAD
	ret = devm_gpiochip_add_data(&pdev->dev, &gpio->gpio_chip,
				     gpio);
	if (ret < 0) {
		dev_err(&pdev->dev, "Could not register gpiochip, %d\n", ret);
		return ret;
	}

	platform_set_drvdata(pdev, gpio);

	return 0;
=======
	return devm_gpiochip_add_data(&pdev->dev, &gpio->gpio_chip, gpio);
>>>>>>> upstream/android-13
}

static const struct platform_device_id tps65912_gpio_id_table[] = {
	{ "tps65912-gpio", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, tps65912_gpio_id_table);

static struct platform_driver tps65912_gpio_driver = {
	.driver = {
		.name = "tps65912-gpio",
	},
	.probe = tps65912_gpio_probe,
	.id_table = tps65912_gpio_id_table,
};
module_platform_driver(tps65912_gpio_driver);

MODULE_AUTHOR("Andrew F. Davis <afd@ti.com>");
MODULE_DESCRIPTION("TPS65912 GPIO driver");
MODULE_LICENSE("GPL v2");
