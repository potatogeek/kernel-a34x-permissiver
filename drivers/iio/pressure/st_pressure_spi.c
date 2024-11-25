<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics pressures driver
 *
 * Copyright 2013 STMicroelectronics Inc.
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
#include "st_pressure.h"

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
/*
 * For new single-chip sensors use <device_name> as compatible string.
 * For old single-chip devices keep <device_name>-press to maintain
 * compatibility
 */
static const struct of_device_id st_press_of_match[] = {
	{
		.compatible = "st,lps001wp-press",
		.data = LPS001WP_PRESS_DEV_NAME,
	},
	{
		.compatible = "st,lps25h-press",
		.data = LPS25H_PRESS_DEV_NAME,
	},
	{
		.compatible = "st,lps331ap-press",
		.data = LPS331AP_PRESS_DEV_NAME,
	},
	{
		.compatible = "st,lps22hb-press",
		.data = LPS22HB_PRESS_DEV_NAME,
	},
	{
		.compatible = "st,lps33hw",
		.data = LPS33HW_PRESS_DEV_NAME,
	},
	{
		.compatible = "st,lps35hw",
		.data = LPS35HW_PRESS_DEV_NAME,
	},
<<<<<<< HEAD
	{},
};
MODULE_DEVICE_TABLE(of, st_press_of_match);
#else
#define st_press_of_match	NULL
#endif

static int st_press_spi_probe(struct spi_device *spi)
{
	struct iio_dev *indio_dev;
	struct st_sensor_data *press_data;
	int err;

	indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*press_data));
	if (indio_dev == NULL)
		return -ENOMEM;

	press_data = iio_priv(indio_dev);

	st_sensors_of_name_probe(&spi->dev, st_press_of_match,
				 spi->modalias, sizeof(spi->modalias));
	st_sensors_spi_configure(indio_dev, spi, press_data);

	err = st_press_common_probe(indio_dev);
	if (err < 0)
		return err;

	return 0;
=======
	{
		.compatible = "st,lps22hh",
		.data = LPS22HH_PRESS_DEV_NAME,
	},
	{},
};
MODULE_DEVICE_TABLE(of, st_press_of_match);

static int st_press_spi_probe(struct spi_device *spi)
{
	const struct st_sensor_settings *settings;
	struct st_sensor_data *press_data;
	struct iio_dev *indio_dev;
	int err;

	st_sensors_dev_name_probe(&spi->dev, spi->modalias, sizeof(spi->modalias));

	settings = st_press_get_settings(spi->modalias);
	if (!settings) {
		dev_err(&spi->dev, "device name %s not recognized.\n",
			spi->modalias);
		return -ENODEV;
	}

	indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*press_data));
	if (!indio_dev)
		return -ENOMEM;

	press_data = iio_priv(indio_dev);
	press_data->sensor_settings = (struct st_sensor_settings *)settings;

	err = st_sensors_spi_configure(indio_dev, spi);
	if (err < 0)
		return err;

	err = st_sensors_power_enable(indio_dev);
	if (err)
		return err;

	err = st_press_common_probe(indio_dev);
	if (err < 0)
		goto st_press_power_off;

	return 0;

st_press_power_off:
	st_sensors_power_disable(indio_dev);

	return err;
>>>>>>> upstream/android-13
}

static int st_press_spi_remove(struct spi_device *spi)
{
<<<<<<< HEAD
	st_press_common_remove(spi_get_drvdata(spi));
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);

	st_press_common_remove(indio_dev);

	st_sensors_power_disable(indio_dev);
>>>>>>> upstream/android-13

	return 0;
}

static const struct spi_device_id st_press_id_table[] = {
	{ LPS001WP_PRESS_DEV_NAME },
	{ LPS25H_PRESS_DEV_NAME },
	{ LPS331AP_PRESS_DEV_NAME },
	{ LPS22HB_PRESS_DEV_NAME },
	{ LPS33HW_PRESS_DEV_NAME },
	{ LPS35HW_PRESS_DEV_NAME },
<<<<<<< HEAD
=======
	{ LPS22HH_PRESS_DEV_NAME },
	{ "lps001wp-press" },
	{ "lps25h-press", },
	{ "lps331ap-press" },
	{ "lps22hb-press" },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(spi, st_press_id_table);

static struct spi_driver st_press_driver = {
	.driver = {
		.name = "st-press-spi",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(st_press_of_match),
=======
		.of_match_table = st_press_of_match,
>>>>>>> upstream/android-13
	},
	.probe = st_press_spi_probe,
	.remove = st_press_spi_remove,
	.id_table = st_press_id_table,
};
module_spi_driver(st_press_driver);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics pressures spi driver");
MODULE_LICENSE("GPL v2");
