<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Murata ZPA2326 SPI pressure and temperature sensor driver
 *
 * Copyright (c) 2016 Parrot S.A.
 *
 * Author: Gregor Boirie <gregor.boirie@parrot.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/spi/spi.h>
<<<<<<< HEAD
#include <linux/of_device.h>
=======
#include <linux/mod_devicetable.h>
>>>>>>> upstream/android-13
#include "zpa2326.h"

/*
 * read_flag_mask:
 *   - address bit 7 must be set to request a register read operation
 *   - address bit 6 must be set to request register address auto increment
 */
static const struct regmap_config zpa2326_regmap_spi_config = {
	.reg_bits       = 8,
	.val_bits       = 8,
	.writeable_reg  = zpa2326_isreg_writeable,
	.readable_reg   = zpa2326_isreg_readable,
	.precious_reg   = zpa2326_isreg_precious,
	.max_register   = ZPA2326_TEMP_OUT_H_REG,
	.read_flag_mask = BIT(7) | BIT(6),
	.cache_type     = REGCACHE_NONE,
};

static int zpa2326_probe_spi(struct spi_device *spi)
{
	struct regmap *regmap;
	int            err;

	regmap = devm_regmap_init_spi(spi, &zpa2326_regmap_spi_config);
	if (IS_ERR(regmap)) {
		dev_err(&spi->dev, "failed to init registers map");
		return PTR_ERR(regmap);
	}

	/*
	 * Enforce SPI slave settings to prevent from DT misconfiguration.
	 *
	 * Clock is idle high. Sampling happens on trailing edge, i.e., rising
	 * edge. Maximum bus frequency is 1 MHz. Registers are 8 bits wide.
	 */
	spi->mode = SPI_MODE_3;
	spi->max_speed_hz = min(spi->max_speed_hz, 1000000U);
	spi->bits_per_word = 8;
	err = spi_setup(spi);
	if (err < 0)
		return err;

	return zpa2326_probe(&spi->dev, spi_get_device_id(spi)->name,
			     spi->irq, ZPA2326_DEVICE_ID, regmap);
}

static int zpa2326_remove_spi(struct spi_device *spi)
{
	zpa2326_remove(&spi->dev);

	return 0;
}

static const struct spi_device_id zpa2326_spi_ids[] = {
	{ "zpa2326", 0 },
	{ },
};
MODULE_DEVICE_TABLE(spi, zpa2326_spi_ids);

<<<<<<< HEAD
#if defined(CONFIG_OF)
=======
>>>>>>> upstream/android-13
static const struct of_device_id zpa2326_spi_matches[] = {
	{ .compatible = "murata,zpa2326" },
	{ }
};
MODULE_DEVICE_TABLE(of, zpa2326_spi_matches);
<<<<<<< HEAD
#endif
=======
>>>>>>> upstream/android-13

static struct spi_driver zpa2326_spi_driver = {
	.driver = {
		.name           = "zpa2326-spi",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(zpa2326_spi_matches),
=======
		.of_match_table = zpa2326_spi_matches,
>>>>>>> upstream/android-13
		.pm             = ZPA2326_PM_OPS,
	},
	.probe    = zpa2326_probe_spi,
	.remove   = zpa2326_remove_spi,
	.id_table = zpa2326_spi_ids,
};
module_spi_driver(zpa2326_spi_driver);

MODULE_AUTHOR("Gregor Boirie <gregor.boirie@parrot.com>");
MODULE_DESCRIPTION("SPI driver for Murata ZPA2326 pressure sensor");
MODULE_LICENSE("GPL v2");
