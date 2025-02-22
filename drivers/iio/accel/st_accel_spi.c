<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics accelerometers driver
 *
 * Copyright 2012-2013 STMicroelectronics Inc.
 *
 * Denis Ciocca <denis.ciocca@st.com>
<<<<<<< HEAD
 *
 * Licensed under the GPL-2.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/slab.h>
=======
#include <linux/mod_devicetable.h>
>>>>>>> upstream/android-13
#include <linux/spi/spi.h>
#include <linux/iio/iio.h>

#include <linux/iio/common/st_sensors.h>
#include <linux/iio/common/st_sensors_spi.h>
#include "st_accel.h"

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
/*
 * For new single-chip sensors use <device_name> as compatible string.
 * For old single-chip devices keep <device_name>-accel to maintain
 * compatibility
 */
static const struct of_device_id st_accel_of_match[] = {
	{
		/* An older compatible */
		.compatible = "st,lis302dl-spi",
		.data = LIS3LV02DL_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis3lv02dl-accel",
		.data = LIS3LV02DL_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis3dh-accel",
		.data = LIS3DH_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lsm330d-accel",
		.data = LSM330D_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lsm330dl-accel",
		.data = LSM330DL_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lsm330dlc-accel",
		.data = LSM330DLC_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis331dlh-accel",
		.data = LIS331DLH_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lsm330-accel",
		.data = LSM330_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lsm303agr-accel",
		.data = LSM303AGR_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis2dh12-accel",
		.data = LIS2DH12_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis3l02dq",
		.data = LIS3L02DQ_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lng2dm-accel",
		.data = LNG2DM_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,h3lis331dl-accel",
		.data = H3LIS331DL_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis331dl-accel",
		.data = LIS331DL_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis2dw12",
		.data = LIS2DW12_ACCEL_DEV_NAME,
	},
	{
		.compatible = "st,lis3dhh",
		.data = LIS3DHH_ACCEL_DEV_NAME,
	},
<<<<<<< HEAD
	{}
};
MODULE_DEVICE_TABLE(of, st_accel_of_match);
#else
#define st_accel_of_match	NULL
#endif

static int st_accel_spi_probe(struct spi_device *spi)
{
	struct iio_dev *indio_dev;
	struct st_sensor_data *adata;
	int err;

=======
	{
		.compatible = "st,lis3de",
		.data = LIS3DE_ACCEL_DEV_NAME,
	},
	{}
};
MODULE_DEVICE_TABLE(of, st_accel_of_match);

static int st_accel_spi_probe(struct spi_device *spi)
{
	const struct st_sensor_settings *settings;
	struct st_sensor_data *adata;
	struct iio_dev *indio_dev;
	int err;

	st_sensors_dev_name_probe(&spi->dev, spi->modalias, sizeof(spi->modalias));

	settings = st_accel_get_settings(spi->modalias);
	if (!settings) {
		dev_err(&spi->dev, "device name %s not recognized.\n",
			spi->modalias);
		return -ENODEV;
	}

>>>>>>> upstream/android-13
	indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*adata));
	if (!indio_dev)
		return -ENOMEM;

	adata = iio_priv(indio_dev);
<<<<<<< HEAD

	st_sensors_of_name_probe(&spi->dev, st_accel_of_match,
				 spi->modalias, sizeof(spi->modalias));
	st_sensors_spi_configure(indio_dev, spi, adata);

	err = st_accel_common_probe(indio_dev);
	if (err < 0)
		return err;

	return 0;
=======
	adata->sensor_settings = (struct st_sensor_settings *)settings;

	err = st_sensors_spi_configure(indio_dev, spi);
	if (err < 0)
		return err;

	err = st_sensors_power_enable(indio_dev);
	if (err)
		return err;

	err = st_accel_common_probe(indio_dev);
	if (err < 0)
		goto st_accel_power_off;

	return 0;

st_accel_power_off:
	st_sensors_power_disable(indio_dev);

	return err;
>>>>>>> upstream/android-13
}

static int st_accel_spi_remove(struct spi_device *spi)
{
<<<<<<< HEAD
	st_accel_common_remove(spi_get_drvdata(spi));
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);

	st_accel_common_remove(indio_dev);

	st_sensors_power_disable(indio_dev);
>>>>>>> upstream/android-13

	return 0;
}

static const struct spi_device_id st_accel_id_table[] = {
	{ LIS3DH_ACCEL_DEV_NAME },
	{ LSM330D_ACCEL_DEV_NAME },
	{ LSM330DL_ACCEL_DEV_NAME },
	{ LSM330DLC_ACCEL_DEV_NAME },
	{ LIS331DLH_ACCEL_DEV_NAME },
	{ LSM330_ACCEL_DEV_NAME },
	{ LSM303AGR_ACCEL_DEV_NAME },
	{ LIS2DH12_ACCEL_DEV_NAME },
	{ LIS3L02DQ_ACCEL_DEV_NAME },
	{ LNG2DM_ACCEL_DEV_NAME },
	{ H3LIS331DL_ACCEL_DEV_NAME },
	{ LIS331DL_ACCEL_DEV_NAME },
	{ LIS3LV02DL_ACCEL_DEV_NAME },
	{ LIS2DW12_ACCEL_DEV_NAME },
	{ LIS3DHH_ACCEL_DEV_NAME },
<<<<<<< HEAD
=======
	{ LIS3DE_ACCEL_DEV_NAME },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(spi, st_accel_id_table);

static struct spi_driver st_accel_driver = {
	.driver = {
		.name = "st-accel-spi",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(st_accel_of_match),
=======
		.of_match_table = st_accel_of_match,
>>>>>>> upstream/android-13
	},
	.probe = st_accel_spi_probe,
	.remove = st_accel_spi_remove,
	.id_table = st_accel_id_table,
};
module_spi_driver(st_accel_driver);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics accelerometers spi driver");
MODULE_LICENSE("GPL v2");
