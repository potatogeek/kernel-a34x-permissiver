<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * ALSA SoC TLV320AIC23 codec driver I2C interface
 *
 * Author:      Arun KS, <arunks@mistralsolutions.com>
 * Copyright:   (C) 2008 Mistral Solutions Pvt Ltd.,
 *
 * Based on sound/soc/codecs/wm8731.c by Richard Purdie
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <sound/soc.h>

#include "tlv320aic23.h"

static int tlv320aic23_i2c_probe(struct i2c_client *i2c,
				 const struct i2c_device_id *i2c_id)
{
	struct regmap *regmap;

	if (!i2c_check_functionality(i2c->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EINVAL;

	regmap = devm_regmap_init_i2c(i2c, &tlv320aic23_regmap);
	return tlv320aic23_probe(&i2c->dev, regmap);
}

static const struct i2c_device_id tlv320aic23_id[] = {
	{"tlv320aic23", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, tlv320aic23_id);

<<<<<<< HEAD
=======
#ifdef CONFIG_OF
>>>>>>> upstream/android-13
static const struct of_device_id tlv320aic23_of_match[] = {
	{ .compatible = "ti,tlv320aic23", },
	{ }
};
MODULE_DEVICE_TABLE(of, tlv320aic23_of_match);
<<<<<<< HEAD
=======
#endif
>>>>>>> upstream/android-13

static struct i2c_driver tlv320aic23_i2c_driver = {
	.driver = {
		   .name = "tlv320aic23-codec",
		   .of_match_table = of_match_ptr(tlv320aic23_of_match),
		   },
	.probe = tlv320aic23_i2c_probe,
	.id_table = tlv320aic23_id,
};

module_i2c_driver(tlv320aic23_i2c_driver);

MODULE_DESCRIPTION("ASoC TLV320AIC23 codec driver I2C");
MODULE_AUTHOR("Arun KS <arunks@mistralsolutions.com>");
MODULE_LICENSE("GPL");
