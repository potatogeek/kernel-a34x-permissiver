<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * GPIO driver for Exar XR17V35X chip
 *
 * Copyright (C) 2015 Sudip Mukherjee <sudip.mukherjee@codethink.co.uk>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/gpio/driver.h>
=======
 */

#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/gpio/driver.h>
#include <linux/idr.h>
>>>>>>> upstream/android-13
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> upstream/android-13

#define EXAR_OFFSET_MPIOLVL_LO 0x90
#define EXAR_OFFSET_MPIOSEL_LO 0x93
#define EXAR_OFFSET_MPIOLVL_HI 0x96
#define EXAR_OFFSET_MPIOSEL_HI 0x99

#define DRIVER_NAME "gpio_exar"

static DEFINE_IDA(ida_index);

struct exar_gpio_chip {
	struct gpio_chip gpio_chip;
<<<<<<< HEAD
	struct mutex lock;
	int index;
	void __iomem *regs;
=======
	struct regmap *regmap;
	int index;
>>>>>>> upstream/android-13
	char name[20];
	unsigned int first_pin;
};

<<<<<<< HEAD
static void exar_update(struct gpio_chip *chip, unsigned int reg, int val,
			unsigned int offset)
{
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
	int temp;

	mutex_lock(&exar_gpio->lock);
	temp = readb(exar_gpio->regs + reg);
	temp &= ~BIT(offset);
	if (val)
		temp |= BIT(offset);
	writeb(temp, exar_gpio->regs + reg);
	mutex_unlock(&exar_gpio->lock);
}

static int exar_set_direction(struct gpio_chip *chip, int direction,
			      unsigned int offset)
{
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
	unsigned int addr = (offset + exar_gpio->first_pin) / 8 ?
		EXAR_OFFSET_MPIOSEL_HI : EXAR_OFFSET_MPIOSEL_LO;
	unsigned int bit  = (offset + exar_gpio->first_pin) % 8;

	exar_update(chip, addr, direction, bit);
	return 0;
}

static int exar_get(struct gpio_chip *chip, unsigned int reg)
{
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
	int value;

	mutex_lock(&exar_gpio->lock);
	value = readb(exar_gpio->regs + reg);
	mutex_unlock(&exar_gpio->lock);

	return value;
=======
static unsigned int
exar_offset_to_sel_addr(struct exar_gpio_chip *exar_gpio, unsigned int offset)
{
	return (offset + exar_gpio->first_pin) / 8 ? EXAR_OFFSET_MPIOSEL_HI
						   : EXAR_OFFSET_MPIOSEL_LO;
}

static unsigned int
exar_offset_to_lvl_addr(struct exar_gpio_chip *exar_gpio, unsigned int offset)
{
	return (offset + exar_gpio->first_pin) / 8 ? EXAR_OFFSET_MPIOLVL_HI
						   : EXAR_OFFSET_MPIOLVL_LO;
}

static unsigned int
exar_offset_to_bit(struct exar_gpio_chip *exar_gpio, unsigned int offset)
{
	return (offset + exar_gpio->first_pin) % 8;
>>>>>>> upstream/android-13
}

static int exar_get_direction(struct gpio_chip *chip, unsigned int offset)
{
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
<<<<<<< HEAD
	unsigned int addr = (offset + exar_gpio->first_pin) / 8 ?
		EXAR_OFFSET_MPIOSEL_HI : EXAR_OFFSET_MPIOSEL_LO;
	unsigned int bit  = (offset + exar_gpio->first_pin) % 8;

	return !!(exar_get(chip, addr) & BIT(bit));
=======
	unsigned int addr = exar_offset_to_sel_addr(exar_gpio, offset);
	unsigned int bit = exar_offset_to_bit(exar_gpio, offset);

	if (regmap_test_bits(exar_gpio->regmap, addr, BIT(bit)))
		return GPIO_LINE_DIRECTION_IN;

	return GPIO_LINE_DIRECTION_OUT;
>>>>>>> upstream/android-13
}

static int exar_get_value(struct gpio_chip *chip, unsigned int offset)
{
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
<<<<<<< HEAD
	unsigned int addr = (offset + exar_gpio->first_pin) / 8 ?
		EXAR_OFFSET_MPIOLVL_HI : EXAR_OFFSET_MPIOLVL_LO;
	unsigned int bit  = (offset + exar_gpio->first_pin) % 8;

	return !!(exar_get(chip, addr) & BIT(bit));
=======
	unsigned int addr = exar_offset_to_lvl_addr(exar_gpio, offset);
	unsigned int bit = exar_offset_to_bit(exar_gpio, offset);

	return !!(regmap_test_bits(exar_gpio->regmap, addr, BIT(bit)));
>>>>>>> upstream/android-13
}

static void exar_set_value(struct gpio_chip *chip, unsigned int offset,
			   int value)
{
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
<<<<<<< HEAD
	unsigned int addr = (offset + exar_gpio->first_pin) / 8 ?
		EXAR_OFFSET_MPIOLVL_HI : EXAR_OFFSET_MPIOLVL_LO;
	unsigned int bit  = (offset + exar_gpio->first_pin) % 8;

	exar_update(chip, addr, value, bit);
=======
	unsigned int addr = exar_offset_to_lvl_addr(exar_gpio, offset);
	unsigned int bit = exar_offset_to_bit(exar_gpio, offset);

	if (value)
		regmap_set_bits(exar_gpio->regmap, addr, BIT(bit));
	else
		regmap_clear_bits(exar_gpio->regmap, addr, BIT(bit));
>>>>>>> upstream/android-13
}

static int exar_direction_output(struct gpio_chip *chip, unsigned int offset,
				 int value)
{
<<<<<<< HEAD
	exar_set_value(chip, offset, value);
	return exar_set_direction(chip, 0, offset);
=======
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
	unsigned int addr = exar_offset_to_sel_addr(exar_gpio, offset);
	unsigned int bit = exar_offset_to_bit(exar_gpio, offset);

	exar_set_value(chip, offset, value);
	regmap_clear_bits(exar_gpio->regmap, addr, BIT(bit));

	return 0;
>>>>>>> upstream/android-13
}

static int exar_direction_input(struct gpio_chip *chip, unsigned int offset)
{
<<<<<<< HEAD
	return exar_set_direction(chip, 1, offset);
}

static int gpio_exar_probe(struct platform_device *pdev)
{
	struct pci_dev *pcidev = to_pci_dev(pdev->dev.parent);
=======
	struct exar_gpio_chip *exar_gpio = gpiochip_get_data(chip);
	unsigned int addr = exar_offset_to_sel_addr(exar_gpio, offset);
	unsigned int bit = exar_offset_to_bit(exar_gpio, offset);

	regmap_set_bits(exar_gpio->regmap, addr, BIT(bit));

	return 0;
}

static void exar_devm_ida_free(void *data)
{
	struct exar_gpio_chip *exar_gpio = data;

	ida_free(&ida_index, exar_gpio->index);
}

static const struct regmap_config exar_regmap_config = {
	.name		= "exar-gpio",
	.reg_bits	= 16,
	.val_bits	= 8,
};

static int gpio_exar_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pci_dev *pcidev = to_pci_dev(dev->parent);
>>>>>>> upstream/android-13
	struct exar_gpio_chip *exar_gpio;
	u32 first_pin, ngpios;
	void __iomem *p;
	int index, ret;

	/*
	 * The UART driver must have mapped region 0 prior to registering this
	 * device - use it.
	 */
	p = pcim_iomap_table(pcidev)[0];
	if (!p)
		return -ENOMEM;

<<<<<<< HEAD
	ret = device_property_read_u32(&pdev->dev, "exar,first-pin",
				       &first_pin);
	if (ret)
		return ret;

	ret = device_property_read_u32(&pdev->dev, "ngpios", &ngpios);
	if (ret)
		return ret;

	exar_gpio = devm_kzalloc(&pdev->dev, sizeof(*exar_gpio), GFP_KERNEL);
	if (!exar_gpio)
		return -ENOMEM;

	mutex_init(&exar_gpio->lock);

	index = ida_simple_get(&ida_index, 0, 0, GFP_KERNEL);
	if (index < 0) {
		ret = index;
		goto err_mutex_destroy;
	}

	sprintf(exar_gpio->name, "exar_gpio%d", index);
	exar_gpio->gpio_chip.label = exar_gpio->name;
	exar_gpio->gpio_chip.parent = &pdev->dev;
=======
	ret = device_property_read_u32(dev, "exar,first-pin", &first_pin);
	if (ret)
		return ret;

	ret = device_property_read_u32(dev, "ngpios", &ngpios);
	if (ret)
		return ret;

	exar_gpio = devm_kzalloc(dev, sizeof(*exar_gpio), GFP_KERNEL);
	if (!exar_gpio)
		return -ENOMEM;

	/*
	 * We don't need to check the return values of mmio regmap operations (unless
	 * the regmap has a clock attached which is not the case here).
	 */
	exar_gpio->regmap = devm_regmap_init_mmio(dev, p, &exar_regmap_config);
	if (IS_ERR(exar_gpio->regmap))
		return PTR_ERR(exar_gpio->regmap);

	index = ida_alloc(&ida_index, GFP_KERNEL);
	if (index < 0)
		return index;

	ret = devm_add_action_or_reset(dev, exar_devm_ida_free, exar_gpio);
	if (ret)
		return ret;

	sprintf(exar_gpio->name, "exar_gpio%d", index);
	exar_gpio->gpio_chip.label = exar_gpio->name;
	exar_gpio->gpio_chip.parent = dev;
>>>>>>> upstream/android-13
	exar_gpio->gpio_chip.direction_output = exar_direction_output;
	exar_gpio->gpio_chip.direction_input = exar_direction_input;
	exar_gpio->gpio_chip.get_direction = exar_get_direction;
	exar_gpio->gpio_chip.get = exar_get_value;
	exar_gpio->gpio_chip.set = exar_set_value;
	exar_gpio->gpio_chip.base = -1;
	exar_gpio->gpio_chip.ngpio = ngpios;
<<<<<<< HEAD
	exar_gpio->regs = p;
	exar_gpio->index = index;
	exar_gpio->first_pin = first_pin;

	ret = devm_gpiochip_add_data(&pdev->dev,
				     &exar_gpio->gpio_chip, exar_gpio);
	if (ret)
		goto err_destroy;
=======
	exar_gpio->index = index;
	exar_gpio->first_pin = first_pin;

	ret = devm_gpiochip_add_data(dev, &exar_gpio->gpio_chip, exar_gpio);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, exar_gpio);

	return 0;
<<<<<<< HEAD

err_destroy:
	ida_simple_remove(&ida_index, index);
err_mutex_destroy:
	mutex_destroy(&exar_gpio->lock);
	return ret;
}

static int gpio_exar_remove(struct platform_device *pdev)
{
	struct exar_gpio_chip *exar_gpio = platform_get_drvdata(pdev);

	ida_simple_remove(&ida_index, exar_gpio->index);
	mutex_destroy(&exar_gpio->lock);

	return 0;
=======
>>>>>>> upstream/android-13
}

static struct platform_driver gpio_exar_driver = {
	.probe	= gpio_exar_probe,
<<<<<<< HEAD
	.remove	= gpio_exar_remove,
=======
>>>>>>> upstream/android-13
	.driver	= {
		.name = DRIVER_NAME,
	},
};

module_platform_driver(gpio_exar_driver);

MODULE_ALIAS("platform:" DRIVER_NAME);
MODULE_DESCRIPTION("Exar GPIO driver");
MODULE_AUTHOR("Sudip Mukherjee <sudip.mukherjee@codethink.co.uk>");
MODULE_LICENSE("GPL");
