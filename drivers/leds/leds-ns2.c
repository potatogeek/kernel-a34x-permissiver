<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * leds-ns2.c - Driver for the Network Space v2 (and parents) dual-GPIO LED
 *
 * Copyright (C) 2010 LaCie
 *
 * Author: Simon Guinot <sguinot@lacie.com>
 *
 * Based on leds-gpio.c by Raphael Assenat <raph@8d.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/platform_data/leds-kirkwood-ns2.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include "leds.h"

=======
#include <linux/gpio/consumer.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/of.h>
#include "leds.h"

enum ns2_led_modes {
	NS_V2_LED_OFF,
	NS_V2_LED_ON,
	NS_V2_LED_SATA,
};

/*
 * If the size of this structure or types of its members is changed,
 * the filling of array modval in function ns2_led_register must be changed
 * accordingly.
 */
struct ns2_led_modval {
	u32			mode;
	u32			cmd_level;
	u32			slow_level;
} __packed;

>>>>>>> upstream/android-13
/*
 * The Network Space v2 dual-GPIO LED is wired to a CPLD. Three different LED
 * modes are available: off, on and SATA activity blinking. The LED modes are
 * controlled through two GPIOs (command and slow): each combination of values
 * for the command/slow GPIOs corresponds to a LED mode.
 */

<<<<<<< HEAD
struct ns2_led_data {
	struct led_classdev	cdev;
	unsigned int		cmd;
	unsigned int		slow;
=======
struct ns2_led {
	struct led_classdev	cdev;
	struct gpio_desc	*cmd;
	struct gpio_desc	*slow;
>>>>>>> upstream/android-13
	bool			can_sleep;
	unsigned char		sata; /* True when SATA mode active. */
	rwlock_t		rw_lock; /* Lock GPIOs. */
	int			num_modes;
	struct ns2_led_modval	*modval;
};

<<<<<<< HEAD
static int ns2_led_get_mode(struct ns2_led_data *led_dat,
			    enum ns2_led_modes *mode)
{
	int i;
	int ret = -EINVAL;
	int cmd_level;
	int slow_level;

	cmd_level = gpio_get_value_cansleep(led_dat->cmd);
	slow_level = gpio_get_value_cansleep(led_dat->slow);

	for (i = 0; i < led_dat->num_modes; i++) {
		if (cmd_level == led_dat->modval[i].cmd_level &&
		    slow_level == led_dat->modval[i].slow_level) {
			*mode = led_dat->modval[i].mode;
			ret = 0;
			break;
		}
	}

	return ret;
}

static void ns2_led_set_mode(struct ns2_led_data *led_dat,
			     enum ns2_led_modes mode)
{
	int i;
	bool found = false;
	unsigned long flags;

	for (i = 0; i < led_dat->num_modes; i++)
		if (mode == led_dat->modval[i].mode) {
			found = true;
			break;
		}

	if (!found)
		return;

	write_lock_irqsave(&led_dat->rw_lock, flags);

	if (!led_dat->can_sleep) {
		gpio_set_value(led_dat->cmd,
			       led_dat->modval[i].cmd_level);
		gpio_set_value(led_dat->slow,
			       led_dat->modval[i].slow_level);
		goto exit_unlock;
	}

	gpio_set_value_cansleep(led_dat->cmd, led_dat->modval[i].cmd_level);
	gpio_set_value_cansleep(led_dat->slow, led_dat->modval[i].slow_level);

exit_unlock:
	write_unlock_irqrestore(&led_dat->rw_lock, flags);
=======
static int ns2_led_get_mode(struct ns2_led *led, enum ns2_led_modes *mode)
{
	int i;
	int cmd_level;
	int slow_level;

	cmd_level = gpiod_get_value_cansleep(led->cmd);
	slow_level = gpiod_get_value_cansleep(led->slow);

	for (i = 0; i < led->num_modes; i++) {
		if (cmd_level == led->modval[i].cmd_level &&
		    slow_level == led->modval[i].slow_level) {
			*mode = led->modval[i].mode;
			return 0;
		}
	}

	return -EINVAL;
}

static void ns2_led_set_mode(struct ns2_led *led, enum ns2_led_modes mode)
{
	int i;
	unsigned long flags;

	for (i = 0; i < led->num_modes; i++)
		if (mode == led->modval[i].mode)
			break;

	if (i == led->num_modes)
		return;

	write_lock_irqsave(&led->rw_lock, flags);

	if (!led->can_sleep) {
		gpiod_set_value(led->cmd, led->modval[i].cmd_level);
		gpiod_set_value(led->slow, led->modval[i].slow_level);
		goto exit_unlock;
	}

	gpiod_set_value_cansleep(led->cmd, led->modval[i].cmd_level);
	gpiod_set_value_cansleep(led->slow, led->modval[i].slow_level);

exit_unlock:
	write_unlock_irqrestore(&led->rw_lock, flags);
>>>>>>> upstream/android-13
}

static void ns2_led_set(struct led_classdev *led_cdev,
			enum led_brightness value)
{
<<<<<<< HEAD
	struct ns2_led_data *led_dat =
		container_of(led_cdev, struct ns2_led_data, cdev);
=======
	struct ns2_led *led = container_of(led_cdev, struct ns2_led, cdev);
>>>>>>> upstream/android-13
	enum ns2_led_modes mode;

	if (value == LED_OFF)
		mode = NS_V2_LED_OFF;
<<<<<<< HEAD
	else if (led_dat->sata)
=======
	else if (led->sata)
>>>>>>> upstream/android-13
		mode = NS_V2_LED_SATA;
	else
		mode = NS_V2_LED_ON;

<<<<<<< HEAD
	ns2_led_set_mode(led_dat, mode);
=======
	ns2_led_set_mode(led, mode);
>>>>>>> upstream/android-13
}

static int ns2_led_set_blocking(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	ns2_led_set(led_cdev, value);
	return 0;
}

static ssize_t ns2_led_sata_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buff, size_t count)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct ns2_led_data *led_dat =
		container_of(led_cdev, struct ns2_led_data, cdev);
=======
	struct ns2_led *led = container_of(led_cdev, struct ns2_led, cdev);
>>>>>>> upstream/android-13
	int ret;
	unsigned long enable;

	ret = kstrtoul(buff, 10, &enable);
	if (ret < 0)
		return ret;

	enable = !!enable;

<<<<<<< HEAD
	if (led_dat->sata == enable)
		goto exit;

	led_dat->sata = enable;
=======
	if (led->sata == enable)
		goto exit;

	led->sata = enable;
>>>>>>> upstream/android-13

	if (!led_get_brightness(led_cdev))
		goto exit;

	if (enable)
<<<<<<< HEAD
		ns2_led_set_mode(led_dat, NS_V2_LED_SATA);
	else
		ns2_led_set_mode(led_dat, NS_V2_LED_ON);
=======
		ns2_led_set_mode(led, NS_V2_LED_SATA);
	else
		ns2_led_set_mode(led, NS_V2_LED_ON);
>>>>>>> upstream/android-13

exit:
	return count;
}

static ssize_t ns2_led_sata_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct ns2_led_data *led_dat =
		container_of(led_cdev, struct ns2_led_data, cdev);

	return sprintf(buf, "%d\n", led_dat->sata);
=======
	struct ns2_led *led = container_of(led_cdev, struct ns2_led, cdev);

	return sprintf(buf, "%d\n", led->sata);
>>>>>>> upstream/android-13
}

static DEVICE_ATTR(sata, 0644, ns2_led_sata_show, ns2_led_sata_store);

static struct attribute *ns2_led_attrs[] = {
	&dev_attr_sata.attr,
	NULL
};
ATTRIBUTE_GROUPS(ns2_led);

<<<<<<< HEAD
static int
create_ns2_led(struct platform_device *pdev, struct ns2_led_data *led_dat,
	       const struct ns2_led *template)
{
	int ret;
	enum ns2_led_modes mode;

	ret = devm_gpio_request_one(&pdev->dev, template->cmd,
			gpio_get_value_cansleep(template->cmd) ?
			GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW,
			template->name);
	if (ret) {
		dev_err(&pdev->dev, "%s: failed to setup command GPIO\n",
			template->name);
		return ret;
	}

	ret = devm_gpio_request_one(&pdev->dev, template->slow,
			gpio_get_value_cansleep(template->slow) ?
			GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW,
			template->name);
	if (ret) {
		dev_err(&pdev->dev, "%s: failed to setup slow GPIO\n",
			template->name);
		return ret;
	}

	rwlock_init(&led_dat->rw_lock);

	led_dat->cdev.name = template->name;
	led_dat->cdev.default_trigger = template->default_trigger;
	led_dat->cdev.blink_set = NULL;
	led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;
	led_dat->cdev.groups = ns2_led_groups;
	led_dat->cmd = template->cmd;
	led_dat->slow = template->slow;
	led_dat->can_sleep = gpio_cansleep(led_dat->cmd) |
				gpio_cansleep(led_dat->slow);
	if (led_dat->can_sleep)
		led_dat->cdev.brightness_set_blocking = ns2_led_set_blocking;
	else
		led_dat->cdev.brightness_set = ns2_led_set;
	led_dat->modval = template->modval;
	led_dat->num_modes = template->num_modes;

	ret = ns2_led_get_mode(led_dat, &mode);
=======
static int ns2_led_register(struct device *dev, struct fwnode_handle *node,
			    struct ns2_led *led)
{
	struct led_init_data init_data = {};
	struct ns2_led_modval *modval;
	enum ns2_led_modes mode;
	int nmodes, ret;

	led->cmd = devm_fwnode_gpiod_get_index(dev, node, "cmd", 0, GPIOD_ASIS,
					       fwnode_get_name(node));
	if (IS_ERR(led->cmd))
		return PTR_ERR(led->cmd);

	led->slow = devm_fwnode_gpiod_get_index(dev, node, "slow", 0,
						GPIOD_ASIS,
						fwnode_get_name(node));
	if (IS_ERR(led->slow))
		return PTR_ERR(led->slow);

	ret = fwnode_property_count_u32(node, "modes-map");
	if (ret < 0 || ret % 3) {
		dev_err(dev, "Missing or malformed modes-map for %pfw\n", node);
		return -EINVAL;
	}

	nmodes = ret / 3;
	modval = devm_kcalloc(dev, nmodes, sizeof(*modval), GFP_KERNEL);
	if (!modval)
		return -ENOMEM;

	fwnode_property_read_u32_array(node, "modes-map", (void *)modval,
				       nmodes * 3);

	rwlock_init(&led->rw_lock);

	led->cdev.blink_set = NULL;
	led->cdev.flags |= LED_CORE_SUSPENDRESUME;
	led->cdev.groups = ns2_led_groups;
	led->can_sleep = gpiod_cansleep(led->cmd) || gpiod_cansleep(led->slow);
	if (led->can_sleep)
		led->cdev.brightness_set_blocking = ns2_led_set_blocking;
	else
		led->cdev.brightness_set = ns2_led_set;
	led->num_modes = nmodes;
	led->modval = modval;

	ret = ns2_led_get_mode(led, &mode);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	/* Set LED initial state. */
<<<<<<< HEAD
	led_dat->sata = (mode == NS_V2_LED_SATA) ? 1 : 0;
	led_dat->cdev.brightness =
		(mode == NS_V2_LED_OFF) ? LED_OFF : LED_FULL;

	ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
	if (ret < 0)
		return ret;

	return 0;
}

static void delete_ns2_led(struct ns2_led_data *led_dat)
{
	led_classdev_unregister(&led_dat->cdev);
}

#ifdef CONFIG_OF_GPIO
/*
 * Translate OpenFirmware node properties into platform_data.
 */
static int
ns2_leds_get_of_pdata(struct device *dev, struct ns2_led_platform_data *pdata)
{
	struct device_node *np = dev->of_node;
	struct device_node *child;
	struct ns2_led *led, *leds;
	int num_leds = 0;

	num_leds = of_get_child_count(np);
	if (!num_leds)
		return -ENODEV;

	leds = devm_kcalloc(dev, num_leds, sizeof(struct ns2_led),
			    GFP_KERNEL);
	if (!leds)
		return -ENOMEM;

	led = leds;
	for_each_child_of_node(np, child) {
		const char *string;
		int ret, i, num_modes;
		struct ns2_led_modval *modval;

		ret = of_get_named_gpio(child, "cmd-gpio", 0);
		if (ret < 0)
			return ret;
		led->cmd = ret;
		ret = of_get_named_gpio(child, "slow-gpio", 0);
		if (ret < 0)
			return ret;
		led->slow = ret;
		ret = of_property_read_string(child, "label", &string);
		led->name = (ret == 0) ? string : child->name;
		ret = of_property_read_string(child, "linux,default-trigger",
					      &string);
		if (ret == 0)
			led->default_trigger = string;

		ret = of_property_count_u32_elems(child, "modes-map");
		if (ret < 0 || ret % 3) {
			dev_err(dev,
				"Missing or malformed modes-map property\n");
			return -EINVAL;
		}

		num_modes = ret / 3;
		modval = devm_kcalloc(dev,
				      num_modes,
				      sizeof(struct ns2_led_modval),
				      GFP_KERNEL);
		if (!modval)
			return -ENOMEM;

		for (i = 0; i < num_modes; i++) {
			of_property_read_u32_index(child,
						"modes-map", 3 * i,
						(u32 *) &modval[i].mode);
			of_property_read_u32_index(child,
						"modes-map", 3 * i + 1,
						(u32 *) &modval[i].cmd_level);
			of_property_read_u32_index(child,
						"modes-map", 3 * i + 2,
						(u32 *) &modval[i].slow_level);
		}

		led->num_modes = num_modes;
		led->modval = modval;

		led++;
	}

	pdata->leds = leds;
	pdata->num_leds = num_leds;

=======
	led->sata = (mode == NS_V2_LED_SATA) ? 1 : 0;
	led->cdev.brightness = (mode == NS_V2_LED_OFF) ? LED_OFF : LED_FULL;

	init_data.fwnode = node;

	ret = devm_led_classdev_register_ext(dev, &led->cdev, &init_data);
	if (ret)
		dev_err(dev, "Failed to register LED for node %pfw\n", node);

	return ret;
}

static int ns2_led_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct fwnode_handle *child;
	struct ns2_led *leds;
	int count;
	int ret;

	count = device_get_child_node_count(dev);
	if (!count)
		return -ENODEV;

	leds = devm_kzalloc(dev, array_size(sizeof(*leds), count), GFP_KERNEL);
	if (!leds)
		return -ENOMEM;

	device_for_each_child_node(dev, child) {
		ret = ns2_led_register(dev, child, leds++);
		if (ret) {
			fwnode_handle_put(child);
			return ret;
		}
	}

>>>>>>> upstream/android-13
	return 0;
}

static const struct of_device_id of_ns2_leds_match[] = {
	{ .compatible = "lacie,ns2-leds", },
	{},
};
MODULE_DEVICE_TABLE(of, of_ns2_leds_match);
<<<<<<< HEAD
#endif /* CONFIG_OF_GPIO */

struct ns2_led_priv {
	int num_leds;
	struct ns2_led_data leds_data[];
};

static inline int sizeof_ns2_led_priv(int num_leds)
{
	return sizeof(struct ns2_led_priv) +
		      (sizeof(struct ns2_led_data) * num_leds);
}

static int ns2_led_probe(struct platform_device *pdev)
{
	struct ns2_led_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct ns2_led_priv *priv;
	int i;
	int ret;

#ifdef CONFIG_OF_GPIO
	if (!pdata) {
		pdata = devm_kzalloc(&pdev->dev,
				     sizeof(struct ns2_led_platform_data),
				     GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;

		ret = ns2_leds_get_of_pdata(&pdev->dev, pdata);
		if (ret)
			return ret;
	}
#else
	if (!pdata)
		return -EINVAL;
#endif /* CONFIG_OF_GPIO */

	priv = devm_kzalloc(&pdev->dev,
			    sizeof_ns2_led_priv(pdata->num_leds), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	priv->num_leds = pdata->num_leds;

	for (i = 0; i < priv->num_leds; i++) {
		ret = create_ns2_led(pdev, &priv->leds_data[i],
				     &pdata->leds[i]);
		if (ret < 0) {
			for (i = i - 1; i >= 0; i--)
				delete_ns2_led(&priv->leds_data[i]);
			return ret;
		}
	}

	platform_set_drvdata(pdev, priv);

	return 0;
}

static int ns2_led_remove(struct platform_device *pdev)
{
	int i;
	struct ns2_led_priv *priv;

	priv = platform_get_drvdata(pdev);

	for (i = 0; i < priv->num_leds; i++)
		delete_ns2_led(&priv->leds_data[i]);

	return 0;
}

static struct platform_driver ns2_led_driver = {
	.probe		= ns2_led_probe,
	.remove		= ns2_led_remove,
	.driver		= {
		.name		= "leds-ns2",
		.of_match_table	= of_match_ptr(of_ns2_leds_match),
=======

static struct platform_driver ns2_led_driver = {
	.probe		= ns2_led_probe,
	.driver		= {
		.name		= "leds-ns2",
		.of_match_table	= of_ns2_leds_match,
>>>>>>> upstream/android-13
	},
};

module_platform_driver(ns2_led_driver);

MODULE_AUTHOR("Simon Guinot <sguinot@lacie.com>");
MODULE_DESCRIPTION("Network Space v2 LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-ns2");
