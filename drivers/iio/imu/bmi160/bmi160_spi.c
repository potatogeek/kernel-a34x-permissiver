<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * BMI160 - Bosch IMU, SPI bits
 *
 * Copyright (c) 2016, Intel Corporation.
 *
<<<<<<< HEAD
 * This file is subject to the terms and conditions of version 2 of
 * the GNU General Public License.  See the file COPYING in the main
 * directory of this archive for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/spi/spi.h>

#include "bmi160.h"

static int bmi160_spi_probe(struct spi_device *spi)
{
	struct regmap *regmap;
	const struct spi_device_id *id = spi_get_device_id(spi);

	regmap = devm_regmap_init_spi(spi, &bmi160_regmap_config);
	if (IS_ERR(regmap)) {
<<<<<<< HEAD
		dev_err(&spi->dev, "Failed to register spi regmap %d\n",
			(int)PTR_ERR(regmap));
=======
		dev_err(&spi->dev, "Failed to register spi regmap: %pe\n",
			regmap);
>>>>>>> upstream/android-13
		return PTR_ERR(regmap);
	}
	return bmi160_core_probe(&spi->dev, regmap, id->name, true);
}

<<<<<<< HEAD
static int bmi160_spi_remove(struct spi_device *spi)
{
	bmi160_core_remove(&spi->dev);

	return 0;
}

=======
>>>>>>> upstream/android-13
static const struct spi_device_id bmi160_spi_id[] = {
	{"bmi160", 0},
	{}
};
MODULE_DEVICE_TABLE(spi, bmi160_spi_id);

static const struct acpi_device_id bmi160_acpi_match[] = {
	{"BMI0160", 0},
	{ },
};
MODULE_DEVICE_TABLE(acpi, bmi160_acpi_match);

#ifdef CONFIG_OF
static const struct of_device_id bmi160_of_match[] = {
	{ .compatible = "bosch,bmi160" },
	{ },
};
MODULE_DEVICE_TABLE(of, bmi160_of_match);
#endif

static struct spi_driver bmi160_spi_driver = {
	.probe		= bmi160_spi_probe,
<<<<<<< HEAD
	.remove		= bmi160_spi_remove,
=======
>>>>>>> upstream/android-13
	.id_table	= bmi160_spi_id,
	.driver = {
		.acpi_match_table	= ACPI_PTR(bmi160_acpi_match),
		.of_match_table		= of_match_ptr(bmi160_of_match),
		.name			= "bmi160_spi",
	},
};
module_spi_driver(bmi160_spi_driver);

MODULE_AUTHOR("Daniel Baluta <daniel.baluta@intel.com");
MODULE_DESCRIPTION("Bosch BMI160 SPI driver");
MODULE_LICENSE("GPL v2");
