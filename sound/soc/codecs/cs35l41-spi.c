<<<<<<< HEAD
/*
 * SPI bus interface to Cirrus Logic CS35L41 codec
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
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/of.h>
#include <linux/regulator/consumer.h>

#include "wm_adsp.h"
#include "sound/cs35l41.h"
=======
// SPDX-License-Identifier: GPL-2.0
//
// cs35l41-spi.c -- CS35l41 SPI driver
//
// Copyright 2017-2021 Cirrus Logic, Inc.
//
// Author: David Rhodes	<david.rhodes@cirrus.com>

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>

#include <sound/cs35l41.h>
>>>>>>> upstream/android-13
#include "cs35l41.h"

static struct regmap_config cs35l41_regmap_spi = {
	.reg_bits = 32,
	.val_bits = 32,
	.pad_bits = 16,
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

static const struct of_device_id cs35l41_of_match[] = {
	{.compatible = "cirrus,cs35l40"},
	{.compatible = "cirrus,cs35l41"},
	{},
};
MODULE_DEVICE_TABLE(of, cs35l41_of_match);

static const struct spi_device_id cs35l41_id_spi[] = {
	{"cs35l40", 0},
	{"cs35l41", 0},
=======
	.precious_reg = cs35l41_precious_reg,
	.cache_type = REGCACHE_RBTREE,
};

static const struct spi_device_id cs35l41_id_spi[] = {
	{ "cs35l40", 0 },
	{ "cs35l41", 0 },
>>>>>>> upstream/android-13
	{}
};

MODULE_DEVICE_TABLE(spi, cs35l41_id_spi);

<<<<<<< HEAD
static int cs35l41_spi_probe(struct spi_device *spi)
{
	const struct regmap_config *regmap_config = &cs35l41_regmap_spi;
	struct cs35l41_private *cs35l41;
	int ret;

	cs35l41 = devm_kzalloc(&spi->dev,
			       sizeof(struct cs35l41_private),
			       GFP_KERNEL);
	if (cs35l41 == NULL)
=======
static void cs35l41_spi_otp_setup(struct cs35l41_private *cs35l41,
				  bool is_pre_setup, unsigned int *freq)
{
	struct spi_device *spi;
	u32 orig_spi_freq;

	spi = to_spi_device(cs35l41->dev);

	if (!spi) {
		dev_err(cs35l41->dev, "%s: No SPI device\n", __func__);
		return;
	}

	if (is_pre_setup) {
		orig_spi_freq = spi->max_speed_hz;
		if (orig_spi_freq > CS35L41_SPI_MAX_FREQ_OTP) {
			spi->max_speed_hz = CS35L41_SPI_MAX_FREQ_OTP;
			spi_setup(spi);
		}
		*freq = orig_spi_freq;
	} else {
		if (spi->max_speed_hz != *freq) {
			spi->max_speed_hz = *freq;
			spi_setup(spi);
		}
	}
}

static int cs35l41_spi_probe(struct spi_device *spi)
{
	const struct regmap_config *regmap_config = &cs35l41_regmap_spi;
	struct cs35l41_platform_data *pdata = dev_get_platdata(&spi->dev);
	struct cs35l41_private *cs35l41;
	int ret;

	cs35l41 = devm_kzalloc(&spi->dev, sizeof(struct cs35l41_private), GFP_KERNEL);
	if (!cs35l41)
>>>>>>> upstream/android-13
		return -ENOMEM;

	spi_set_drvdata(spi, cs35l41);
	cs35l41->regmap = devm_regmap_init_spi(spi, regmap_config);
	if (IS_ERR(cs35l41->regmap)) {
		ret = PTR_ERR(cs35l41->regmap);
<<<<<<< HEAD
		dev_err(&spi->dev, "Failed to allocate register map: %d\n",
			ret);
=======
		dev_err(&spi->dev, "Failed to allocate register map: %d\n", ret);
>>>>>>> upstream/android-13
		return ret;
	}

	cs35l41->dev = &spi->dev;
	cs35l41->irq = spi->irq;
<<<<<<< HEAD

	dev_info(&spi->dev, "CS35L41 SPI MFD probe\n");

	return cs35l41_probe(cs35l41);
=======
	cs35l41->otp_setup = cs35l41_spi_otp_setup;

	return cs35l41_probe(cs35l41, pdata);
>>>>>>> upstream/android-13
}

static int cs35l41_spi_remove(struct spi_device *spi)
{
	struct cs35l41_private *cs35l41 = spi_get_drvdata(spi);

<<<<<<< HEAD
	return cs35l41_remove(cs35l41);
}

static struct spi_driver cs35l41_spi_driver = {
	.driver = {
		.name		= "cs35l41",
		.of_match_table = cs35l41_of_match,
=======
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

static struct spi_driver cs35l41_spi_driver = {
	.driver = {
		.name		= "cs35l41",
		.of_match_table = of_match_ptr(cs35l41_of_match),
		.acpi_match_table = ACPI_PTR(cs35l41_acpi_match),
>>>>>>> upstream/android-13
	},
	.id_table	= cs35l41_id_spi,
	.probe		= cs35l41_spi_probe,
	.remove		= cs35l41_spi_remove,
};

module_spi_driver(cs35l41_spi_driver);

MODULE_DESCRIPTION("SPI CS35L41 driver");
MODULE_AUTHOR("David Rhodes, Cirrus Logic Inc, <david.rhodes@cirrus.com>");
MODULE_LICENSE("GPL");
