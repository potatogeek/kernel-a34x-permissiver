<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics magnetometers driver
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
#include <linux/i2c.h>
#include <linux/iio/iio.h>

#include <linux/iio/common/st_sensors.h>
#include <linux/iio/common/st_sensors_i2c.h>
#include "st_magn.h"

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
static const struct of_device_id st_magn_of_match[] = {
	{
		.compatible = "st,lsm303dlh-magn",
		.data = LSM303DLH_MAGN_DEV_NAME,
	},
	{
		.compatible = "st,lsm303dlhc-magn",
		.data = LSM303DLHC_MAGN_DEV_NAME,
	},
	{
		.compatible = "st,lsm303dlm-magn",
		.data = LSM303DLM_MAGN_DEV_NAME,
	},
	{
		.compatible = "st,lis3mdl-magn",
		.data = LIS3MDL_MAGN_DEV_NAME,
	},
	{
		.compatible = "st,lsm303agr-magn",
		.data = LSM303AGR_MAGN_DEV_NAME,
	},
	{
		.compatible = "st,lis2mdl",
		.data = LIS2MDL_MAGN_DEV_NAME,
	},
<<<<<<< HEAD
	{},
};
MODULE_DEVICE_TABLE(of, st_magn_of_match);
#else
#define st_magn_of_match NULL
#endif

static int st_magn_i2c_probe(struct i2c_client *client,
						const struct i2c_device_id *id)
{
	struct iio_dev *indio_dev;
	struct st_sensor_data *mdata;
	int err;

=======
	{
		.compatible = "st,lsm9ds1-magn",
		.data = LSM9DS1_MAGN_DEV_NAME,
	},
	{
		.compatible = "st,iis2mdc",
		.data = IIS2MDC_MAGN_DEV_NAME,
	},
	{},
};
MODULE_DEVICE_TABLE(of, st_magn_of_match);

static int st_magn_i2c_probe(struct i2c_client *client,
			     const struct i2c_device_id *id)
{
	const struct st_sensor_settings *settings;
	struct st_sensor_data *mdata;
	struct iio_dev *indio_dev;
	int err;

	st_sensors_dev_name_probe(&client->dev, client->name, sizeof(client->name));

	settings = st_magn_get_settings(client->name);
	if (!settings) {
		dev_err(&client->dev, "device name %s not recognized.\n",
			client->name);
		return -ENODEV;
	}

>>>>>>> upstream/android-13
	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*mdata));
	if (!indio_dev)
		return -ENOMEM;

	mdata = iio_priv(indio_dev);
<<<<<<< HEAD
	st_sensors_of_name_probe(&client->dev, st_magn_of_match,
				 client->name, sizeof(client->name));

	st_sensors_i2c_configure(indio_dev, client, mdata);

	err = st_magn_common_probe(indio_dev);
	if (err < 0)
		return err;

	return 0;
=======
	mdata->sensor_settings = (struct st_sensor_settings *)settings;

	err = st_sensors_i2c_configure(indio_dev, client);
	if (err < 0)
		return err;

	err = st_sensors_power_enable(indio_dev);
	if (err)
		return err;

	err = st_magn_common_probe(indio_dev);
	if (err < 0)
		goto st_magn_power_off;

	return 0;

st_magn_power_off:
	st_sensors_power_disable(indio_dev);

	return err;
>>>>>>> upstream/android-13
}

static int st_magn_i2c_remove(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
<<<<<<< HEAD
	st_magn_common_remove(indio_dev);

=======

	st_magn_common_remove(indio_dev);

	st_sensors_power_disable(indio_dev);

>>>>>>> upstream/android-13
	return 0;
}

static const struct i2c_device_id st_magn_id_table[] = {
	{ LSM303DLH_MAGN_DEV_NAME },
	{ LSM303DLHC_MAGN_DEV_NAME },
	{ LSM303DLM_MAGN_DEV_NAME },
	{ LIS3MDL_MAGN_DEV_NAME },
	{ LSM303AGR_MAGN_DEV_NAME },
	{ LIS2MDL_MAGN_DEV_NAME },
<<<<<<< HEAD
=======
	{ LSM9DS1_MAGN_DEV_NAME },
	{ IIS2MDC_MAGN_DEV_NAME },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(i2c, st_magn_id_table);

static struct i2c_driver st_magn_driver = {
	.driver = {
		.name = "st-magn-i2c",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(st_magn_of_match),
=======
		.of_match_table = st_magn_of_match,
>>>>>>> upstream/android-13
	},
	.probe = st_magn_i2c_probe,
	.remove = st_magn_i2c_remove,
	.id_table = st_magn_id_table,
};
module_i2c_driver(st_magn_driver);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics magnetometers i2c driver");
MODULE_LICENSE("GPL v2");
