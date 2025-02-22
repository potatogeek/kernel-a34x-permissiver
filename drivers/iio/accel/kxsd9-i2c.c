// SPDX-License-Identifier: GPL-2.0
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/mod_devicetable.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/regmap.h>

#include "kxsd9.h"

static int kxsd9_i2c_probe(struct i2c_client *i2c,
			   const struct i2c_device_id *id)
{
	static const struct regmap_config config = {
		.reg_bits = 8,
		.val_bits = 8,
		.max_register = 0x0e,
	};
	struct regmap *regmap;

	regmap = devm_regmap_init_i2c(i2c, &config);
	if (IS_ERR(regmap)) {
<<<<<<< HEAD
		dev_err(&i2c->dev, "Failed to register i2c regmap %d\n",
			(int)PTR_ERR(regmap));
=======
		dev_err(&i2c->dev, "Failed to register i2c regmap: %pe\n",
			regmap);
>>>>>>> upstream/android-13
		return PTR_ERR(regmap);
	}

	return kxsd9_common_probe(&i2c->dev,
				  regmap,
				  i2c->name);
}

static int kxsd9_i2c_remove(struct i2c_client *client)
{
	return kxsd9_common_remove(&client->dev);
}

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
static const struct of_device_id kxsd9_of_match[] = {
	{ .compatible = "kionix,kxsd9", },
	{ },
};
MODULE_DEVICE_TABLE(of, kxsd9_of_match);
<<<<<<< HEAD
#else
#define kxsd9_of_match NULL
#endif
=======
>>>>>>> upstream/android-13

static const struct i2c_device_id kxsd9_i2c_id[] = {
	{"kxsd9", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, kxsd9_i2c_id);

static struct i2c_driver kxsd9_i2c_driver = {
	.driver = {
		.name	= "kxsd9",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(kxsd9_of_match),
=======
		.of_match_table = kxsd9_of_match,
>>>>>>> upstream/android-13
		.pm = &kxsd9_dev_pm_ops,
	},
	.probe		= kxsd9_i2c_probe,
	.remove		= kxsd9_i2c_remove,
	.id_table	= kxsd9_i2c_id,
};
module_i2c_driver(kxsd9_i2c_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("KXSD9 accelerometer I2C interface");
