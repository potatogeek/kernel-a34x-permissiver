<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
/*
 * Access to GPOs on TWL6040 chip
 *
 * Copyright (C) 2012 Texas Instruments, Inc.
 *
 * Authors:
 *	Sergio Aguirre <saaguirre@ti.com>
 *	Peter Ujfalusi <peter.ujfalusi@ti.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/platform_device.h>
=======
#include <linux/gpio/driver.h>
#include <linux/platform_device.h>
#include <linux/bitops.h>
>>>>>>> upstream/android-13
#include <linux/of.h>

#include <linux/mfd/twl6040.h>

static int twl6040gpo_get(struct gpio_chip *chip, unsigned offset)
{
	struct twl6040 *twl6040 = dev_get_drvdata(chip->parent->parent);
	int ret = 0;

	ret = twl6040_reg_read(twl6040, TWL6040_REG_GPOCTL);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	return (ret >> offset) & 1;
=======
	return !!(ret & BIT(offset));
}

static int twl6040gpo_get_direction(struct gpio_chip *chip, unsigned offset)
{
	return GPIO_LINE_DIRECTION_OUT;
>>>>>>> upstream/android-13
}

static int twl6040gpo_direction_out(struct gpio_chip *chip, unsigned offset,
				    int value)
{
	/* This only drives GPOs, and can't change direction */
	return 0;
}

static void twl6040gpo_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct twl6040 *twl6040 = dev_get_drvdata(chip->parent->parent);
	int ret;
	u8 gpoctl;

	ret = twl6040_reg_read(twl6040, TWL6040_REG_GPOCTL);
	if (ret < 0)
		return;

	if (value)
<<<<<<< HEAD
		gpoctl = ret | (1 << offset);
	else
		gpoctl = ret & ~(1 << offset);
=======
		gpoctl = ret | BIT(offset);
	else
		gpoctl = ret & ~BIT(offset);
>>>>>>> upstream/android-13

	twl6040_reg_write(twl6040, TWL6040_REG_GPOCTL, gpoctl);
}

static struct gpio_chip twl6040gpo_chip = {
	.label			= "twl6040",
	.owner			= THIS_MODULE,
	.get			= twl6040gpo_get,
	.direction_output	= twl6040gpo_direction_out,
<<<<<<< HEAD
=======
	.get_direction		= twl6040gpo_get_direction,
>>>>>>> upstream/android-13
	.set			= twl6040gpo_set,
	.can_sleep		= true,
};

/*----------------------------------------------------------------------*/

static int gpo_twl6040_probe(struct platform_device *pdev)
{
	struct device *twl6040_core_dev = pdev->dev.parent;
	struct twl6040 *twl6040 = dev_get_drvdata(twl6040_core_dev);
	int ret;

	twl6040gpo_chip.base = -1;

	if (twl6040_get_revid(twl6040) < TWL6041_REV_ES2_0)
		twl6040gpo_chip.ngpio = 3; /* twl6040 have 3 GPO */
	else
		twl6040gpo_chip.ngpio = 1; /* twl6041 have 1 GPO */

	twl6040gpo_chip.parent = &pdev->dev;
#ifdef CONFIG_OF_GPIO
	twl6040gpo_chip.of_node = twl6040_core_dev->of_node;
#endif

	ret = devm_gpiochip_add_data(&pdev->dev, &twl6040gpo_chip, NULL);
	if (ret < 0) {
		dev_err(&pdev->dev, "could not register gpiochip, %d\n", ret);
		twl6040gpo_chip.ngpio = 0;
	}

	return ret;
}

/* Note:  this hardware lives inside an I2C-based multi-function device. */
MODULE_ALIAS("platform:twl6040-gpo");

static struct platform_driver gpo_twl6040_driver = {
	.driver = {
		.name	= "twl6040-gpo",
	},
	.probe		= gpo_twl6040_probe,
};

module_platform_driver(gpo_twl6040_driver);

MODULE_AUTHOR("Texas Instruments, Inc.");
MODULE_DESCRIPTION("GPO interface for TWL6040");
MODULE_LICENSE("GPL");
