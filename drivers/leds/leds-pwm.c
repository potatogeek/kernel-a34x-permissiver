<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * linux/drivers/leds-pwm.c
 *
 * simple PWM based LED control
 *
 * Copyright 2009 Luotao Fu @ Pengutronix (l.fu@pengutronix.de)
 *
 * based on leds-gpio.c by Raphael Assenat <raph@8d.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
<<<<<<< HEAD
#include <linux/leds_pwm.h>
#include <linux/slab.h>
=======
#include <linux/slab.h>
#include "leds.h"

struct led_pwm {
	const char	*name;
	u8		active_low;
	u8		default_state;
	unsigned int	max_brightness;
};
>>>>>>> upstream/android-13

struct led_pwm_data {
	struct led_classdev	cdev;
	struct pwm_device	*pwm;
<<<<<<< HEAD
	unsigned int		active_low;
	unsigned int		period;
	int			duty;
=======
	struct pwm_state	pwmstate;
	unsigned int		active_low;
>>>>>>> upstream/android-13
};

struct led_pwm_priv {
	int num_leds;
<<<<<<< HEAD
	struct led_pwm_data leds[0];
};

static void __led_pwm_set(struct led_pwm_data *led_dat)
{
	int new_duty = led_dat->duty;

	pwm_config(led_dat->pwm, new_duty, led_dat->period);

	if (new_duty == 0)
		pwm_disable(led_dat->pwm);
	else
		pwm_enable(led_dat->pwm);
}

=======
	struct led_pwm_data leds[];
};

>>>>>>> upstream/android-13
static int led_pwm_set(struct led_classdev *led_cdev,
		       enum led_brightness brightness)
{
	struct led_pwm_data *led_dat =
		container_of(led_cdev, struct led_pwm_data, cdev);
	unsigned int max = led_dat->cdev.max_brightness;
<<<<<<< HEAD
	unsigned long long duty =  led_dat->period;
=======
	unsigned long long duty = led_dat->pwmstate.period;
>>>>>>> upstream/android-13

	duty *= brightness;
	do_div(duty, max);

	if (led_dat->active_low)
<<<<<<< HEAD
		duty = led_dat->period - duty;

	led_dat->duty = duty;

	__led_pwm_set(led_dat);

	return 0;
}

static inline size_t sizeof_pwm_leds_priv(int num_leds)
{
	return sizeof(struct led_pwm_priv) +
		      (sizeof(struct led_pwm_data) * num_leds);
}

static void led_pwm_cleanup(struct led_pwm_priv *priv)
{
	while (priv->num_leds--)
		led_classdev_unregister(&priv->leds[priv->num_leds].cdev);
}

static int led_pwm_add(struct device *dev, struct led_pwm_priv *priv,
		       struct led_pwm *led, struct device_node *child)
{
	struct led_pwm_data *led_data = &priv->leds[priv->num_leds];
	struct pwm_args pargs;
=======
		duty = led_dat->pwmstate.period - duty;

	led_dat->pwmstate.duty_cycle = duty;
	led_dat->pwmstate.enabled = duty > 0;
	return pwm_apply_state(led_dat->pwm, &led_dat->pwmstate);
}

__attribute__((nonnull))
static int led_pwm_add(struct device *dev, struct led_pwm_priv *priv,
		       struct led_pwm *led, struct fwnode_handle *fwnode)
{
	struct led_pwm_data *led_data = &priv->leds[priv->num_leds];
	struct led_init_data init_data = { .fwnode = fwnode };
>>>>>>> upstream/android-13
	int ret;

	led_data->active_low = led->active_low;
	led_data->cdev.name = led->name;
<<<<<<< HEAD
	led_data->cdev.default_trigger = led->default_trigger;
=======
>>>>>>> upstream/android-13
	led_data->cdev.brightness = LED_OFF;
	led_data->cdev.max_brightness = led->max_brightness;
	led_data->cdev.flags = LED_CORE_SUSPENDRESUME;

<<<<<<< HEAD
	if (child)
		led_data->pwm = devm_of_pwm_get(dev, child, NULL);
	else
		led_data->pwm = devm_pwm_get(dev, led->name);
	if (IS_ERR(led_data->pwm)) {
		ret = PTR_ERR(led_data->pwm);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "unable to request PWM for %s: %d\n",
				led->name, ret);
		return ret;
	}

	led_data->cdev.brightness_set_blocking = led_pwm_set;

	/*
	 * FIXME: pwm_apply_args() should be removed when switching to the
	 * atomic PWM API.
	 */
	pwm_apply_args(led_data->pwm);

	pwm_get_args(led_data->pwm, &pargs);

	led_data->period = pargs.period;
	if (!led_data->period && (led->pwm_period_ns > 0))
		led_data->period = led->pwm_period_ns;

	ret = led_classdev_register(dev, &led_data->cdev);
	if (ret == 0) {
		priv->num_leds++;
		led_pwm_set(&led_data->cdev, led_data->cdev.brightness);
	} else {
		dev_err(dev, "failed to register PWM led for %s: %d\n",
			led->name, ret);
	}

	return ret;
}

static int led_pwm_create_of(struct device *dev, struct led_pwm_priv *priv)
{
	struct device_node *child;
	struct led_pwm led;
	int ret = 0;

	memset(&led, 0, sizeof(led));

	for_each_child_of_node(dev->of_node, child) {
		led.name = of_get_property(child, "label", NULL) ? :
			   child->name;

		led.default_trigger = of_get_property(child,
						"linux,default-trigger", NULL);
		led.active_low = of_property_read_bool(child, "active-low");
		of_property_read_u32(child, "max-brightness",
				     &led.max_brightness);

		ret = led_pwm_add(dev, priv, &led, child);
		if (ret) {
			of_node_put(child);
			break;
		}
	}

=======
	led_data->pwm = devm_fwnode_pwm_get(dev, fwnode, NULL);
	if (IS_ERR(led_data->pwm))
		return dev_err_probe(dev, PTR_ERR(led_data->pwm),
				     "unable to request PWM for %s\n",
				     led->name);

	led_data->cdev.brightness_set_blocking = led_pwm_set;

	/* init PWM state */
	switch (led->default_state) {
	case LEDS_DEFSTATE_KEEP:
		pwm_get_state(led_data->pwm, &led_data->pwmstate);
		if (led_data->pwmstate.period)
			break;
		led->default_state = LEDS_DEFSTATE_OFF;
		dev_warn(dev,
			"failed to read period for %s, default to off",
			led->name);
		fallthrough;
	default:
		pwm_init_state(led_data->pwm, &led_data->pwmstate);
		break;
	}

	/* set brightness */
	switch (led->default_state) {
	case LEDS_DEFSTATE_ON:
		led_data->cdev.brightness = led->max_brightness;
		break;
	case LEDS_DEFSTATE_KEEP:
		{
		uint64_t brightness;

		brightness = led->max_brightness;
		brightness *= led_data->pwmstate.duty_cycle;
		do_div(brightness, led_data->pwmstate.period);
		led_data->cdev.brightness = brightness;
		}
		break;
	}

	ret = devm_led_classdev_register_ext(dev, &led_data->cdev, &init_data);
	if (ret) {
		dev_err(dev, "failed to register PWM led for %s: %d\n",
			led->name, ret);
		return ret;
	}

	if (led->default_state != LEDS_DEFSTATE_KEEP) {
		ret = led_pwm_set(&led_data->cdev, led_data->cdev.brightness);
		if (ret) {
			dev_err(dev, "failed to set led PWM value for %s: %d",
				led->name, ret);
			return ret;
		}
	}

	priv->num_leds++;
	return 0;
}

static int led_pwm_create_fwnode(struct device *dev, struct led_pwm_priv *priv)
{
	struct fwnode_handle *fwnode;
	struct led_pwm led;
	int ret;

	memset(&led, 0, sizeof(led));

	device_for_each_child_node(dev, fwnode) {
		ret = fwnode_property_read_string(fwnode, "label", &led.name);
		if (ret && is_of_node(fwnode))
			led.name = to_of_node(fwnode)->name;

		if (!led.name) {
			ret = EINVAL;
			goto err_child_out;
		}

		led.active_low = fwnode_property_read_bool(fwnode,
							   "active-low");
		fwnode_property_read_u32(fwnode, "max-brightness",
					 &led.max_brightness);

		led.default_state = led_init_default_state_get(fwnode);

		ret = led_pwm_add(dev, priv, &led, fwnode);
		if (ret)
			goto err_child_out;
	}

	return 0;

err_child_out:
	fwnode_handle_put(fwnode);
>>>>>>> upstream/android-13
	return ret;
}

static int led_pwm_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct led_pwm_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct led_pwm_priv *priv;
	int count, i;
	int ret = 0;

	if (pdata)
		count = pdata->num_leds;
	else
		count = of_get_child_count(pdev->dev.of_node);
=======
	struct led_pwm_priv *priv;
	int ret = 0;
	int count;

	count = device_get_child_node_count(&pdev->dev);
>>>>>>> upstream/android-13

	if (!count)
		return -EINVAL;

<<<<<<< HEAD
	priv = devm_kzalloc(&pdev->dev, sizeof_pwm_leds_priv(count),
=======
	priv = devm_kzalloc(&pdev->dev, struct_size(priv, leds, count),
>>>>>>> upstream/android-13
			    GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

<<<<<<< HEAD
	if (pdata) {
		for (i = 0; i < count; i++) {
			ret = led_pwm_add(&pdev->dev, priv, &pdata->leds[i],
					  NULL);
			if (ret)
				break;
		}
	} else {
		ret = led_pwm_create_of(&pdev->dev, priv);
	}

	if (ret) {
		led_pwm_cleanup(priv);
		return ret;
	}
=======
	ret = led_pwm_create_fwnode(&pdev->dev, priv);

	if (ret)
		return ret;
>>>>>>> upstream/android-13

	platform_set_drvdata(pdev, priv);

	return 0;
}

<<<<<<< HEAD
static int led_pwm_remove(struct platform_device *pdev)
{
	struct led_pwm_priv *priv = platform_get_drvdata(pdev);

	led_pwm_cleanup(priv);

	return 0;
}

=======
>>>>>>> upstream/android-13
static const struct of_device_id of_pwm_leds_match[] = {
	{ .compatible = "pwm-leds", },
	{},
};
MODULE_DEVICE_TABLE(of, of_pwm_leds_match);

static struct platform_driver led_pwm_driver = {
	.probe		= led_pwm_probe,
<<<<<<< HEAD
	.remove		= led_pwm_remove,
=======
>>>>>>> upstream/android-13
	.driver		= {
		.name	= "leds_pwm",
		.of_match_table = of_pwm_leds_match,
	},
};

module_platform_driver(led_pwm_driver);

MODULE_AUTHOR("Luotao Fu <l.fu@pengutronix.de>");
MODULE_DESCRIPTION("generic PWM LED driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:leds-pwm");
