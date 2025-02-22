<<<<<<< HEAD
/*
 * linux/sound/soc/codecs/tlv320aic32x4-i2c.c
 *
 * Copyright 2011 NW Digital Radio
 *
 * Author: Jeremy McDermond <nh6z@nh6z.net>
 *
 * Based on sound/soc/codecs/wm8974 and TI driver for kernel 2.6.27.
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
=======
/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright 2011-2019 NW Digital Radio
 *
 * Author: Annaliese McDermond <nh6z@nh6z.net>
 *
 * Based on sound/soc/codecs/wm8974 and TI driver for kernel 2.6.27.
 *
>>>>>>> upstream/android-13
 */

#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <sound/soc.h>

#include "tlv320aic32x4.h"

<<<<<<< HEAD
=======
static const struct of_device_id aic32x4_of_id[];

>>>>>>> upstream/android-13
static int aic32x4_i2c_probe(struct i2c_client *i2c,
			     const struct i2c_device_id *id)
{
	struct regmap *regmap;
	struct regmap_config config;

	config = aic32x4_regmap_config;
	config.reg_bits = 8;
	config.val_bits = 8;

	regmap = devm_regmap_init_i2c(i2c, &config);
<<<<<<< HEAD
=======

	if (i2c->dev.of_node) {
		const struct of_device_id *oid;

		oid = of_match_node(aic32x4_of_id, i2c->dev.of_node);
		dev_set_drvdata(&i2c->dev, (void *)oid->data);
	} else if (id) {
		dev_set_drvdata(&i2c->dev, (void *)id->driver_data);
	}

>>>>>>> upstream/android-13
	return aic32x4_probe(&i2c->dev, regmap);
}

static int aic32x4_i2c_remove(struct i2c_client *i2c)
{
	return aic32x4_remove(&i2c->dev);
}

static const struct i2c_device_id aic32x4_i2c_id[] = {
<<<<<<< HEAD
	{ "tlv320aic32x4", 0 },
	{ "tlv320aic32x6", 1 },
=======
	{ "tlv320aic32x4", (kernel_ulong_t)AIC32X4_TYPE_AIC32X4 },
	{ "tlv320aic32x6", (kernel_ulong_t)AIC32X4_TYPE_AIC32X6 },
	{ "tas2505", (kernel_ulong_t)AIC32X4_TYPE_TAS2505 },
>>>>>>> upstream/android-13
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(i2c, aic32x4_i2c_id);

static const struct of_device_id aic32x4_of_id[] = {
<<<<<<< HEAD
	{ .compatible = "ti,tlv320aic32x4", },
	{ .compatible = "ti,tlv320aic32x6", },
=======
	{ .compatible = "ti,tlv320aic32x4", .data = (void *)AIC32X4_TYPE_AIC32X4 },
	{ .compatible = "ti,tlv320aic32x6", .data = (void *)AIC32X4_TYPE_AIC32X6 },
	{ .compatible = "ti,tas2505", .data = (void *)AIC32X4_TYPE_TAS2505 },
>>>>>>> upstream/android-13
	{ /* senitel */ }
};
MODULE_DEVICE_TABLE(of, aic32x4_of_id);

static struct i2c_driver aic32x4_i2c_driver = {
	.driver = {
		.name = "tlv320aic32x4",
		.of_match_table = aic32x4_of_id,
	},
	.probe =    aic32x4_i2c_probe,
	.remove =   aic32x4_i2c_remove,
	.id_table = aic32x4_i2c_id,
};

module_i2c_driver(aic32x4_i2c_driver);

MODULE_DESCRIPTION("ASoC TLV320AIC32x4 codec driver I2C");
<<<<<<< HEAD
MODULE_AUTHOR("Jeremy McDermond <nh6z@nh6z.net>");
=======
MODULE_AUTHOR("Annaliese McDermond <nh6z@nh6z.net>");
>>>>>>> upstream/android-13
MODULE_LICENSE("GPL");
