<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* drivers/leds/leds-s3c24xx.c
 *
 * (c) 2006 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX - LEDs GPIO driver
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
*/

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
<<<<<<< HEAD
#include <linux/gpio.h>
=======
#include <linux/gpio/consumer.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_data/leds-s3c24xx.h>

<<<<<<< HEAD
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

=======
>>>>>>> upstream/android-13
/* our context */

struct s3c24xx_gpio_led {
	struct led_classdev		 cdev;
	struct s3c24xx_led_platdata	*pdata;
<<<<<<< HEAD
=======
	struct gpio_desc		*gpiod;
>>>>>>> upstream/android-13
};

static inline struct s3c24xx_gpio_led *to_gpio(struct led_classdev *led_cdev)
{
	return container_of(led_cdev, struct s3c24xx_gpio_led, cdev);
}

static void s3c24xx_led_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	struct s3c24xx_gpio_led *led = to_gpio(led_cdev);
<<<<<<< HEAD
	struct s3c24xx_led_platdata *pd = led->pdata;
	int state = (value ? 1 : 0) ^ (pd->flags & S3C24XX_LEDF_ACTLOW);

	/* there will be a short delay between setting the output and
	 * going from output to input when using tristate. */

	gpio_set_value(pd->gpio, state);

	if (pd->flags & S3C24XX_LEDF_TRISTATE) {
		if (value)
			gpio_direction_output(pd->gpio, state);
		else
			gpio_direction_input(pd->gpio);
	}
=======

	gpiod_set_value(led->gpiod, !!value);
>>>>>>> upstream/android-13
}

static int s3c24xx_led_probe(struct platform_device *dev)
{
	struct s3c24xx_led_platdata *pdata = dev_get_platdata(&dev->dev);
	struct s3c24xx_gpio_led *led;
	int ret;

	led = devm_kzalloc(&dev->dev, sizeof(struct s3c24xx_gpio_led),
			   GFP_KERNEL);
	if (!led)
		return -ENOMEM;

	led->cdev.brightness_set = s3c24xx_led_set;
	led->cdev.default_trigger = pdata->def_trigger;
	led->cdev.name = pdata->name;
	led->cdev.flags |= LED_CORE_SUSPENDRESUME;

	led->pdata = pdata;

<<<<<<< HEAD
	ret = devm_gpio_request(&dev->dev, pdata->gpio, "S3C24XX_LED");
	if (ret < 0)
		return ret;

	/* no point in having a pull-up if we are always driving */

	s3c_gpio_setpull(pdata->gpio, S3C_GPIO_PULL_NONE);

	if (pdata->flags & S3C24XX_LEDF_TRISTATE)
		gpio_direction_input(pdata->gpio);
	else
		gpio_direction_output(pdata->gpio,
			pdata->flags & S3C24XX_LEDF_ACTLOW ? 1 : 0);

	/* register our new led device */

=======
	/* Default to off */
	led->gpiod = devm_gpiod_get(&dev->dev, NULL, GPIOD_OUT_LOW);
	if (IS_ERR(led->gpiod))
		return PTR_ERR(led->gpiod);

	/* register our new led device */
>>>>>>> upstream/android-13
	ret = devm_led_classdev_register(&dev->dev, &led->cdev);
	if (ret < 0)
		dev_err(&dev->dev, "led_classdev_register failed\n");

	return ret;
}

static struct platform_driver s3c24xx_led_driver = {
	.probe		= s3c24xx_led_probe,
	.driver		= {
		.name		= "s3c24xx_led",
	},
};

module_platform_driver(s3c24xx_led_driver);

MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_DESCRIPTION("S3C24XX LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:s3c24xx_led");
