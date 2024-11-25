<<<<<<< HEAD
/*
 * gpio_backlight.c - Simple GPIO-controlled backlight
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * gpio_backlight.c - Simple GPIO-controlled backlight
>>>>>>> upstream/android-13
 */

#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/fb.h>
<<<<<<< HEAD
#include <linux/gpio.h> /* Only for legacy support */
=======
>>>>>>> upstream/android-13
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
<<<<<<< HEAD
#include <linux/of_gpio.h>
#include <linux/platform_data/gpio_backlight.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

struct gpio_backlight {
	struct device *dev;
	struct device *fbdev;

	struct gpio_desc *gpiod;
	int def_value;
=======
#include <linux/platform_data/gpio_backlight.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/slab.h>

struct gpio_backlight {
	struct device *fbdev;
	struct gpio_desc *gpiod;
>>>>>>> upstream/android-13
};

static int gpio_backlight_update_status(struct backlight_device *bl)
{
	struct gpio_backlight *gbl = bl_get_data(bl);
<<<<<<< HEAD
	int brightness = bl->props.brightness;

	if (bl->props.power != FB_BLANK_UNBLANK ||
	    bl->props.fb_blank != FB_BLANK_UNBLANK ||
	    bl->props.state & (BL_CORE_SUSPENDED | BL_CORE_FBBLANK))
		brightness = 0;

	gpiod_set_value_cansleep(gbl->gpiod, brightness);
=======

	gpiod_set_value_cansleep(gbl->gpiod, backlight_get_brightness(bl));
>>>>>>> upstream/android-13

	return 0;
}

static int gpio_backlight_check_fb(struct backlight_device *bl,
				   struct fb_info *info)
{
	struct gpio_backlight *gbl = bl_get_data(bl);

	return gbl->fbdev == NULL || gbl->fbdev == info->dev;
}

static const struct backlight_ops gpio_backlight_ops = {
	.options	= BL_CORE_SUSPENDRESUME,
	.update_status	= gpio_backlight_update_status,
	.check_fb	= gpio_backlight_check_fb,
};

<<<<<<< HEAD
static int gpio_backlight_probe_dt(struct platform_device *pdev,
				   struct gpio_backlight *gbl)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	enum gpiod_flags flags;
	int ret;

	gbl->def_value = of_property_read_bool(np, "default-on");
	flags = gbl->def_value ? GPIOD_OUT_HIGH : GPIOD_OUT_LOW;

	gbl->gpiod = devm_gpiod_get(dev, NULL, flags);
	if (IS_ERR(gbl->gpiod)) {
		ret = PTR_ERR(gbl->gpiod);

		if (ret != -EPROBE_DEFER) {
			dev_err(dev,
				"Error: The gpios parameter is missing or invalid.\n");
		}
		return ret;
	}

	return 0;
}

static int gpio_backlight_probe(struct platform_device *pdev)
{
	struct gpio_backlight_platform_data *pdata =
		dev_get_platdata(&pdev->dev);
	struct backlight_properties props;
	struct backlight_device *bl;
	struct gpio_backlight *gbl;
	struct device_node *np = pdev->dev.of_node;
	int ret;

	if (!pdata && !np) {
		dev_err(&pdev->dev,
			"failed to find platform data or device tree node.\n");
		return -ENODEV;
	}

	gbl = devm_kzalloc(&pdev->dev, sizeof(*gbl), GFP_KERNEL);
	if (gbl == NULL)
		return -ENOMEM;

	gbl->dev = &pdev->dev;

	if (np) {
		ret = gpio_backlight_probe_dt(pdev, gbl);
		if (ret)
			return ret;
	} else {
		/*
		 * Legacy platform data GPIO retrieveal. Do not expand
		 * the use of this code path, currently only used by one
		 * SH board.
		 */
		unsigned long flags = GPIOF_DIR_OUT;

		gbl->fbdev = pdata->fbdev;
		gbl->def_value = pdata->def_value;
		flags |= gbl->def_value ? GPIOF_INIT_HIGH : GPIOF_INIT_LOW;

		ret = devm_gpio_request_one(gbl->dev, pdata->gpio, flags,
					    pdata ? pdata->name : "backlight");
		if (ret < 0) {
			dev_err(&pdev->dev, "unable to request GPIO\n");
			return ret;
		}
		gbl->gpiod = gpio_to_desc(pdata->gpio);
		if (!gbl->gpiod)
			return -EINVAL;
=======
static int gpio_backlight_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct gpio_backlight_platform_data *pdata = dev_get_platdata(dev);
	struct device_node *of_node = dev->of_node;
	struct backlight_properties props;
	struct backlight_device *bl;
	struct gpio_backlight *gbl;
	int ret, init_brightness, def_value;

	gbl = devm_kzalloc(dev, sizeof(*gbl), GFP_KERNEL);
	if (gbl == NULL)
		return -ENOMEM;

	if (pdata)
		gbl->fbdev = pdata->fbdev;

	def_value = device_property_read_bool(dev, "default-on");

	gbl->gpiod = devm_gpiod_get(dev, NULL, GPIOD_ASIS);
	if (IS_ERR(gbl->gpiod)) {
		ret = PTR_ERR(gbl->gpiod);
		if (ret != -EPROBE_DEFER)
			dev_err(dev,
				"Error: The gpios parameter is missing or invalid.\n");
		return ret;
>>>>>>> upstream/android-13
	}

	memset(&props, 0, sizeof(props));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = 1;
<<<<<<< HEAD
	bl = devm_backlight_device_register(&pdev->dev, dev_name(&pdev->dev),
					&pdev->dev, gbl, &gpio_backlight_ops,
					&props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
		return PTR_ERR(bl);
	}

	bl->props.brightness = gbl->def_value;
	backlight_update_status(bl);
=======
	bl = devm_backlight_device_register(dev, dev_name(dev), dev, gbl,
					    &gpio_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(dev, "failed to register backlight\n");
		return PTR_ERR(bl);
	}

	/* Set the initial power state */
	if (!of_node || !of_node->phandle)
		/* Not booted with device tree or no phandle link to the node */
		bl->props.power = def_value ? FB_BLANK_UNBLANK
					    : FB_BLANK_POWERDOWN;
	else if (gpiod_get_direction(gbl->gpiod) == 0 &&
		 gpiod_get_value_cansleep(gbl->gpiod) == 0)
		bl->props.power = FB_BLANK_POWERDOWN;
	else
		bl->props.power = FB_BLANK_UNBLANK;

	bl->props.brightness = 1;

	init_brightness = backlight_get_brightness(bl);
	ret = gpiod_direction_output(gbl->gpiod, init_brightness);
	if (ret) {
		dev_err(dev, "failed to set initial brightness\n");
		return ret;
	}
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, bl);
	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
static struct of_device_id gpio_backlight_of_match[] = {
	{ .compatible = "gpio-backlight" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, gpio_backlight_of_match);
<<<<<<< HEAD
#endif
=======
>>>>>>> upstream/android-13

static struct platform_driver gpio_backlight_driver = {
	.driver		= {
		.name		= "gpio-backlight",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(gpio_backlight_of_match),
=======
		.of_match_table = gpio_backlight_of_match,
>>>>>>> upstream/android-13
	},
	.probe		= gpio_backlight_probe,
};

module_platform_driver(gpio_backlight_driver);

MODULE_AUTHOR("Laurent Pinchart <laurent.pinchart@ideasonboard.com>");
MODULE_DESCRIPTION("GPIO-based Backlight Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio-backlight");
