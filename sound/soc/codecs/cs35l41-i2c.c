<<<<<<< HEAD
/*
 * I2C bus interface to Cirrus Logic CS35L41 codec
 *
 * Copyright 2017 Cirrus Logic
 *
 * Author:	David Rhodes	<david.rhodes@cirrus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/of.h>
#include <linux/regulator/consumer.h>

#include "wm_adsp.h"
#include "sound/cs35l41.h"
=======
// SPDX-License-Identifier: GPL-2.0
//
// cs35l41-i2c.c -- CS35l41 I2C driver
//
// Copyright 2017-2021 Cirrus Logic, Inc.
//
// Author: David Rhodes <david.rhodes@cirrus.com>

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include <sound/cs35l41.h>
>>>>>>> upstream/android-13
#include "cs35l41.h"

static struct regmap_config cs35l41_regmap_i2c = {
	.reg_bits = 32,
	.val_bits = 32,
<<<<<<< HEAD
	.reg_stride = 4,
=======
	.reg_stride = CS35L41_REGSTRIDE,
>>>>>>> upstream/android-13
	.reg_format_endian = REGMAP_ENDIAN_BIG,
	.val_format_endian = REGMAP_ENDIAN_BIG,
	.max_register = CS35L41_LASTREG,
	.reg_defaults = cs35l41_reg,
	.num_reg_defaults = ARRAY_SIZE(cs35l41_reg),
	.volatile_reg = cs35l41_volatile_reg,
	.readable_reg = cs35l41_readable_reg,
<<<<<<< HEAD
	.cache_type = REGCACHE_RBTREE,
};

#if 0
static const struct of_device_id cs35l41_of_match[] = {
	{.compatible = "cirrus,cs35l40"},
	{.compatible = "cirrus,cs35l41"},
	{},
};
MODULE_DEVICE_TABLE(of, cs35l41_of_match);

static const struct i2c_device_id cs35l41_id_i2c[] = {
	{"cs35l40", 0},
	{"cs35l41", 0},
=======
	.precious_reg = cs35l41_precious_reg,
	.cache_type = REGCACHE_RBTREE,
};

static const struct i2c_device_id cs35l41_id_i2c[] = {
	{ "cs35l40", 0 },
	{ "cs35l41", 0 },
>>>>>>> upstream/android-13
	{}
};

MODULE_DEVICE_TABLE(i2c, cs35l41_id_i2c);
<<<<<<< HEAD
#endif

int cs35l41_i2c_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	const struct regmap_config *regmap_config = &cs35l41_regmap_i2c;
	struct cs35l41_private *cs35l41;
=======

static int cs35l41_i2c_probe(struct i2c_client *client,
			     const struct i2c_device_id *id)
{
	struct cs35l41_private *cs35l41;
	struct device *dev = &client->dev;
	struct cs35l41_platform_data *pdata = dev_get_platdata(dev);
	const struct regmap_config *regmap_config = &cs35l41_regmap_i2c;
>>>>>>> upstream/android-13
	int ret;

	cs35l41 = devm_kzalloc(dev, sizeof(struct cs35l41_private), GFP_KERNEL);

<<<<<<< HEAD
	if (cs35l41 == NULL)
=======
	if (!cs35l41)
>>>>>>> upstream/android-13
		return -ENOMEM;

	cs35l41->dev = dev;
	cs35l41->irq = client->irq;

	i2c_set_clientdata(client, cs35l41);
	cs35l41->regmap = devm_regmap_init_i2c(client, regmap_config);
	if (IS_ERR(cs35l41->regmap)) {
		ret = PTR_ERR(cs35l41->regmap);
<<<<<<< HEAD
		dev_err(cs35l41->dev, "Failed to allocate register map: %d\n",
			ret);
		return ret;
	}

	return cs35l41_probe(cs35l41);
}

int cs35l41_i2c_remove(struct i2c_client *client)
{
	struct cs35l41_private *cs35l41 = i2c_get_clientdata(client);

	return cs35l41_remove(cs35l41);
}

#if 0
static struct i2c_driver cs35l41_i2c_driver = {
	.driver = {
		.name		= "cs35l41",
		.of_match_table = cs35l41_of_match,
=======
		dev_err(cs35l41->dev, "Failed to allocate register map: %d\n", ret);
		return ret;
	}

	return cs35l41_probe(cs35l41, pdata);
}

static int cs35l41_i2c_remove(struct i2c_client *client)
{
	struct cs35l41_private *cs35l41 = i2c_get_clientdata(client);

	cs35l41_remove(cs35l41);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id cs35l41_of_match[] = {
	{ .compatible = "cirrus,cs35l40" },
	{ .compatible = "cirrus,cs35l41" },
	{},
};
MODULE_DEVICE_TABLE(of, cs35l41_of_match);
#endif

#ifdef CONFIG_ACPI
static const struct acpi_device_id cs35l41_acpi_match[] = {
	{ "CSC3541", 0 }, /* Cirrus Logic PnP ID + part ID */
	{},
};
MODULE_DEVICE_TABLE(acpi, cs35l41_acpi_match);
#endif

static struct i2c_driver cs35l41_i2c_driver = {
	.driver = {
		.name		= "cs35l41",
		.of_match_table = of_match_ptr(cs35l41_of_match),
		.acpi_match_table = ACPI_PTR(cs35l41_acpi_match),
>>>>>>> upstream/android-13
	},
	.id_table	= cs35l41_id_i2c,
	.probe		= cs35l41_i2c_probe,
	.remove		= cs35l41_i2c_remove,
};

module_i2c_driver(cs35l41_i2c_driver);
<<<<<<< HEAD
#endif
=======
>>>>>>> upstream/android-13

MODULE_DESCRIPTION("I2C CS35L41 driver");
MODULE_AUTHOR("David Rhodes, Cirrus Logic Inc, <david.rhodes@cirrus.com>");
MODULE_LICENSE("GPL");
