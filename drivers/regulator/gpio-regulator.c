<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * gpio-regulator.c
 *
 * Copyright 2011 Heiko Stuebner <heiko@sntech.de>
 *
 * based on fixed.c
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * Copyright (c) 2009 Nokia Corporation
 * Roger Quadros <ext-roger.quadros@nokia.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
=======
>>>>>>> upstream/android-13
 * This is useful for systems with mixed controllable and
 * non-controllable regulators, as well as for allowing testing on
 * systems with no controllable regulators.
 */

#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/gpio-regulator.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

struct gpio_regulator_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;

	struct gpio *gpios;
=======
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/of.h>

struct gpio_regulator_data {
	struct regulator_desc desc;

	struct gpio_desc **gpiods;
>>>>>>> upstream/android-13
	int nr_gpios;

	struct gpio_regulator_state *states;
	int nr_states;

	int state;
};

static int gpio_regulator_get_value(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].gpios == data->state)
			return data->states[ptr].value;

	return -EINVAL;
}

static int gpio_regulator_set_voltage(struct regulator_dev *dev,
					int min_uV, int max_uV,
					unsigned *selector)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target = 0, state, best_val = INT_MAX;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value < best_val &&
		    data->states[ptr].value >= min_uV &&
		    data->states[ptr].value <= max_uV) {
			target = data->states[ptr].gpios;
			best_val = data->states[ptr].value;
			if (selector)
				*selector = ptr;
		}

	if (best_val == INT_MAX)
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
<<<<<<< HEAD
		gpio_set_value_cansleep(data->gpios[ptr].gpio, state);
=======
		gpiod_set_value_cansleep(data->gpiods[ptr], state);
>>>>>>> upstream/android-13
	}
	data->state = target;

	return 0;
}

static int gpio_regulator_list_voltage(struct regulator_dev *dev,
				      unsigned selector)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (selector >= data->nr_states)
		return -EINVAL;

	return data->states[selector].value;
}

static int gpio_regulator_set_current_limit(struct regulator_dev *dev,
					int min_uA, int max_uA)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target = 0, state, best_val = 0;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value > best_val &&
		    data->states[ptr].value >= min_uA &&
		    data->states[ptr].value <= max_uA) {
			target = data->states[ptr].gpios;
			best_val = data->states[ptr].value;
		}

	if (best_val == 0)
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
<<<<<<< HEAD
		gpio_set_value_cansleep(data->gpios[ptr].gpio, state);
=======
		gpiod_set_value_cansleep(data->gpiods[ptr], state);
>>>>>>> upstream/android-13
	}
	data->state = target;

	return 0;
}

<<<<<<< HEAD
static struct regulator_ops gpio_regulator_voltage_ops = {
=======
static const struct regulator_ops gpio_regulator_voltage_ops = {
>>>>>>> upstream/android-13
	.get_voltage = gpio_regulator_get_value,
	.set_voltage = gpio_regulator_set_voltage,
	.list_voltage = gpio_regulator_list_voltage,
};

static struct gpio_regulator_config *
of_get_gpio_regulator_config(struct device *dev, struct device_node *np,
			     const struct regulator_desc *desc)
{
	struct gpio_regulator_config *config;
	const char *regtype;
<<<<<<< HEAD
	int proplen, gpio, i;
=======
	int proplen, i;
	int ngpios;
>>>>>>> upstream/android-13
	int ret;

	config = devm_kzalloc(dev,
			sizeof(struct gpio_regulator_config),
			GFP_KERNEL);
	if (!config)
		return ERR_PTR(-ENOMEM);

	config->init_data = of_get_regulator_init_data(dev, np, desc);
	if (!config->init_data)
		return ERR_PTR(-EINVAL);

	config->supply_name = config->init_data->constraints.name;

<<<<<<< HEAD
	if (of_property_read_bool(np, "enable-active-high"))
		config->enable_high = true;

=======
	if (config->init_data->constraints.boot_on)
		config->enabled_at_boot = true;

	/*
	 * Do not use: undocumented device tree property.
	 * This is kept around solely for device tree ABI stability.
	 */
>>>>>>> upstream/android-13
	if (of_property_read_bool(np, "enable-at-boot"))
		config->enabled_at_boot = true;

	of_property_read_u32(np, "startup-delay-us", &config->startup_delay);

<<<<<<< HEAD
	config->enable_gpio = of_get_named_gpio(np, "enable-gpio", 0);
	if (config->enable_gpio < 0 && config->enable_gpio != -ENOENT)
		return ERR_PTR(config->enable_gpio);

	/* Fetch GPIOs. - optional property*/
	ret = of_gpio_count(np);
	if ((ret < 0) && (ret != -ENOENT))
		return ERR_PTR(ret);

	if (ret > 0) {
		config->nr_gpios = ret;
		config->gpios = devm_kcalloc(dev,
					config->nr_gpios, sizeof(struct gpio),
					GFP_KERNEL);
		if (!config->gpios)
			return ERR_PTR(-ENOMEM);

		proplen = of_property_count_u32_elems(np, "gpios-states");
		/* optional property */
		if (proplen < 0)
			proplen = 0;

		if (proplen > 0 && proplen != config->nr_gpios) {
			dev_warn(dev, "gpios <-> gpios-states mismatch\n");
			proplen = 0;
		}

		for (i = 0; i < config->nr_gpios; i++) {
			gpio = of_get_named_gpio(np, "gpios", i);
			if (gpio < 0) {
				if (gpio != -ENOENT)
					return ERR_PTR(gpio);
				break;
			}
			config->gpios[i].gpio = gpio;
			config->gpios[i].label = config->supply_name;
			if (proplen > 0) {
				of_property_read_u32_index(np, "gpios-states",
							   i, &ret);
				if (ret)
					config->gpios[i].flags =
							   GPIOF_OUT_INIT_HIGH;
			}
		}
	}
=======
	/* Fetch GPIO init levels */
	ngpios = gpiod_count(dev, NULL);
	if (ngpios > 0) {
		config->gflags = devm_kzalloc(dev,
					      sizeof(enum gpiod_flags)
					      * ngpios,
					      GFP_KERNEL);
		if (!config->gflags)
			return ERR_PTR(-ENOMEM);

		for (i = 0; i < ngpios; i++) {
			u32 val;

			ret = of_property_read_u32_index(np, "gpios-states", i,
							 &val);

			/* Default to high per specification */
			if (ret)
				config->gflags[i] = GPIOD_OUT_HIGH;
			else
				config->gflags[i] =
					val ? GPIOD_OUT_HIGH : GPIOD_OUT_LOW;
		}
	}
	config->ngpios = ngpios;
>>>>>>> upstream/android-13

	/* Fetch states. */
	proplen = of_property_count_u32_elems(np, "states");
	if (proplen < 0) {
		dev_err(dev, "No 'states' property found\n");
		return ERR_PTR(-EINVAL);
	}

	config->states = devm_kcalloc(dev,
				proplen / 2,
				sizeof(struct gpio_regulator_state),
				GFP_KERNEL);
	if (!config->states)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < proplen / 2; i++) {
		of_property_read_u32_index(np, "states", i * 2,
					   &config->states[i].value);
		of_property_read_u32_index(np, "states", i * 2 + 1,
					   &config->states[i].gpios);
	}
	config->nr_states = i;

	config->type = REGULATOR_VOLTAGE;
	ret = of_property_read_string(np, "regulator-type", &regtype);
	if (ret >= 0) {
		if (!strncmp("voltage", regtype, 7))
			config->type = REGULATOR_VOLTAGE;
		else if (!strncmp("current", regtype, 7))
			config->type = REGULATOR_CURRENT;
		else
			dev_warn(dev, "Unknown regulator-type '%s'\n",
				 regtype);
	}

	return config;
}

<<<<<<< HEAD
static struct regulator_ops gpio_regulator_current_ops = {
=======
static const struct regulator_ops gpio_regulator_current_ops = {
>>>>>>> upstream/android-13
	.get_current_limit = gpio_regulator_get_value,
	.set_current_limit = gpio_regulator_set_current_limit,
};

static int gpio_regulator_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct gpio_regulator_config *config = dev_get_platdata(&pdev->dev);
	struct device_node *np = pdev->dev.of_node;
	struct gpio_regulator_data *drvdata;
	struct regulator_config cfg = { };
	int ptr, ret, state;

	drvdata = devm_kzalloc(&pdev->dev, sizeof(struct gpio_regulator_data),
=======
	struct device *dev = &pdev->dev;
	struct gpio_regulator_config *config = dev_get_platdata(dev);
	struct device_node *np = dev->of_node;
	struct gpio_regulator_data *drvdata;
	struct regulator_config cfg = { };
	struct regulator_dev *rdev;
	enum gpiod_flags gflags;
	int ptr, ret, state, i;

	drvdata = devm_kzalloc(dev, sizeof(struct gpio_regulator_data),
>>>>>>> upstream/android-13
			       GFP_KERNEL);
	if (drvdata == NULL)
		return -ENOMEM;

	if (np) {
<<<<<<< HEAD
		config = of_get_gpio_regulator_config(&pdev->dev, np,
=======
		config = of_get_gpio_regulator_config(dev, np,
>>>>>>> upstream/android-13
						      &drvdata->desc);
		if (IS_ERR(config))
			return PTR_ERR(config);
	}

<<<<<<< HEAD
	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		return -ENOMEM;
	}

	if (config->nr_gpios != 0) {
		drvdata->gpios = kmemdup(config->gpios,
					 config->nr_gpios * sizeof(struct gpio),
					 GFP_KERNEL);
		if (drvdata->gpios == NULL) {
			dev_err(&pdev->dev, "Failed to allocate gpio data\n");
			ret = -ENOMEM;
			goto err_name;
		}

		drvdata->nr_gpios = config->nr_gpios;
		ret = gpio_request_array(drvdata->gpios, drvdata->nr_gpios);
		if (ret) {
			if (ret != -EPROBE_DEFER)
				dev_err(&pdev->dev,
					"Could not obtain regulator setting GPIOs: %d\n",
					ret);
			goto err_memgpio;
		}
	}

	drvdata->states = kmemdup(config->states,
				  config->nr_states *
					 sizeof(struct gpio_regulator_state),
				  GFP_KERNEL);
	if (drvdata->states == NULL) {
		dev_err(&pdev->dev, "Failed to allocate state data\n");
		ret = -ENOMEM;
		goto err_stategpio;
=======
	drvdata->desc.name = devm_kstrdup(dev, config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(dev, "Failed to allocate supply name\n");
		return -ENOMEM;
	}

	drvdata->gpiods = devm_kzalloc(dev, sizeof(struct gpio_desc *),
				       GFP_KERNEL);
	if (!drvdata->gpiods)
		return -ENOMEM;
	for (i = 0; i < config->ngpios; i++) {
		drvdata->gpiods[i] = devm_gpiod_get_index(dev,
							  NULL,
							  i,
							  config->gflags[i]);
		if (IS_ERR(drvdata->gpiods[i]))
			return PTR_ERR(drvdata->gpiods[i]);
		/* This is good to know */
		gpiod_set_consumer_name(drvdata->gpiods[i], drvdata->desc.name);
	}
	drvdata->nr_gpios = config->ngpios;

	drvdata->states = devm_kmemdup(dev,
				       config->states,
				       config->nr_states *
				       sizeof(struct gpio_regulator_state),
				       GFP_KERNEL);
	if (drvdata->states == NULL) {
		dev_err(dev, "Failed to allocate state data\n");
		return -ENOMEM;
>>>>>>> upstream/android-13
	}
	drvdata->nr_states = config->nr_states;

	drvdata->desc.owner = THIS_MODULE;
	drvdata->desc.enable_time = config->startup_delay;

	/* handle regulator type*/
	switch (config->type) {
	case REGULATOR_VOLTAGE:
		drvdata->desc.type = REGULATOR_VOLTAGE;
		drvdata->desc.ops = &gpio_regulator_voltage_ops;
		drvdata->desc.n_voltages = config->nr_states;
		break;
	case REGULATOR_CURRENT:
		drvdata->desc.type = REGULATOR_CURRENT;
		drvdata->desc.ops = &gpio_regulator_current_ops;
		break;
	default:
<<<<<<< HEAD
		dev_err(&pdev->dev, "No regulator type set\n");
		ret = -EINVAL;
		goto err_memstate;
=======
		dev_err(dev, "No regulator type set\n");
		return -EINVAL;
>>>>>>> upstream/android-13
	}

	/* build initial state from gpio init data. */
	state = 0;
	for (ptr = 0; ptr < drvdata->nr_gpios; ptr++) {
<<<<<<< HEAD
		if (config->gpios[ptr].flags & GPIOF_OUT_INIT_HIGH)
=======
		if (config->gflags[ptr] == GPIOD_OUT_HIGH)
>>>>>>> upstream/android-13
			state |= (1 << ptr);
	}
	drvdata->state = state;

<<<<<<< HEAD
	cfg.dev = &pdev->dev;
=======
	cfg.dev = dev;
>>>>>>> upstream/android-13
	cfg.init_data = config->init_data;
	cfg.driver_data = drvdata;
	cfg.of_node = np;

<<<<<<< HEAD
	if (gpio_is_valid(config->enable_gpio)) {
		cfg.ena_gpio = config->enable_gpio;
		cfg.ena_gpio_initialized = true;
	}
	cfg.ena_gpio_invert = !config->enable_high;
	if (config->enabled_at_boot) {
		if (config->enable_high)
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_HIGH;
		else
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_LOW;
	} else {
		if (config->enable_high)
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_LOW;
		else
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_HIGH;
	}

	drvdata->dev = regulator_register(&drvdata->desc, &cfg);
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err_memstate;
=======
	/*
	 * The signal will be inverted by the GPIO core if flagged so in the
	 * descriptor.
	 */
	if (config->enabled_at_boot)
		gflags = GPIOD_OUT_HIGH | GPIOD_FLAGS_BIT_NONEXCLUSIVE;
	else
		gflags = GPIOD_OUT_LOW | GPIOD_FLAGS_BIT_NONEXCLUSIVE;

	cfg.ena_gpiod = gpiod_get_optional(dev, "enable", gflags);
	if (IS_ERR(cfg.ena_gpiod))
		return PTR_ERR(cfg.ena_gpiod);

	rdev = devm_regulator_register(dev, &drvdata->desc, &cfg);
	if (IS_ERR(rdev)) {
		ret = PTR_ERR(rdev);
		dev_err(dev, "Failed to register regulator: %d\n", ret);
		return ret;
>>>>>>> upstream/android-13
	}

	platform_set_drvdata(pdev, drvdata);

	return 0;
<<<<<<< HEAD

err_memstate:
	kfree(drvdata->states);
err_stategpio:
	gpio_free_array(drvdata->gpios, drvdata->nr_gpios);
err_memgpio:
	kfree(drvdata->gpios);
err_name:
	kfree(drvdata->desc.name);
	return ret;
}

static int gpio_regulator_remove(struct platform_device *pdev)
{
	struct gpio_regulator_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);

	gpio_free_array(drvdata->gpios, drvdata->nr_gpios);

	kfree(drvdata->states);
	kfree(drvdata->gpios);

	kfree(drvdata->desc.name);

	return 0;
=======
>>>>>>> upstream/android-13
}

#if defined(CONFIG_OF)
static const struct of_device_id regulator_gpio_of_match[] = {
	{ .compatible = "regulator-gpio", },
	{},
};
MODULE_DEVICE_TABLE(of, regulator_gpio_of_match);
#endif

static struct platform_driver gpio_regulator_driver = {
	.probe		= gpio_regulator_probe,
<<<<<<< HEAD
	.remove		= gpio_regulator_remove,
=======
>>>>>>> upstream/android-13
	.driver		= {
		.name		= "gpio-regulator",
		.of_match_table = of_match_ptr(regulator_gpio_of_match),
	},
};

static int __init gpio_regulator_init(void)
{
	return platform_driver_register(&gpio_regulator_driver);
}
subsys_initcall(gpio_regulator_init);

static void __exit gpio_regulator_exit(void)
{
	platform_driver_unregister(&gpio_regulator_driver);
}
module_exit(gpio_regulator_exit);

MODULE_AUTHOR("Heiko Stuebner <heiko@sntech.de>");
MODULE_DESCRIPTION("gpio voltage regulator");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio-regulator");
