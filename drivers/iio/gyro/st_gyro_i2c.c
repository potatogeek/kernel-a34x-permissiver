<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics gyroscopes driver
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
#include "st_gyro.h"

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
static const struct of_device_id st_gyro_of_match[] = {
	{
		.compatible = "st,l3g4200d-gyro",
		.data = L3G4200D_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,lsm330d-gyro",
		.data = LSM330D_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,lsm330dl-gyro",
		.data = LSM330DL_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,lsm330dlc-gyro",
		.data = LSM330DLC_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,l3gd20-gyro",
		.data = L3GD20_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,l3gd20h-gyro",
		.data = L3GD20H_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,l3g4is-gyro",
		.data = L3G4IS_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,lsm330-gyro",
		.data = LSM330_GYRO_DEV_NAME,
	},
	{
		.compatible = "st,lsm9ds0-gyro",
		.data = LSM9DS0_GYRO_DEV_NAME,
	},
	{},
};
MODULE_DEVICE_TABLE(of, st_gyro_of_match);
<<<<<<< HEAD
#else
#define st_gyro_of_match NULL
#endif

static int st_gyro_i2c_probe(struct i2c_client *client,
						const struct i2c_device_id *id)
{
	struct iio_dev *indio_dev;
	struct st_sensor_data *gdata;
	int err;

=======

static int st_gyro_i2c_probe(struct i2c_client *client,
			     const struct i2c_device_id *id)
{
	const struct st_sensor_settings *settings;
	struct st_sensor_data *gdata;
	struct iio_dev *indio_dev;
	int err;

	st_sensors_dev_name_probe(&client->dev, client->name, sizeof(client->name));

	settings = st_gyro_get_settings(client->name);
	if (!settings) {
		dev_err(&client->dev, "device name %s not recognized.\n",
			client->name);
		return -ENODEV;
	}

>>>>>>> upstream/android-13
	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*gdata));
	if (!indio_dev)
		return -ENOMEM;

	gdata = iio_priv(indio_dev);
<<<<<<< HEAD
	st_sensors_of_name_probe(&client->dev, st_gyro_of_match,
				 client->name, sizeof(client->name));

	st_sensors_i2c_configure(indio_dev, client, gdata);

	err = st_gyro_common_probe(indio_dev);
	if (err < 0)
		return err;

	return 0;
=======
	gdata->sensor_settings = (struct st_sensor_settings *)settings;

	err = st_sensors_i2c_configure(indio_dev, client);
	if (err < 0)
		return err;

	err = st_sensors_power_enable(indio_dev);
	if (err)
		return err;

	err = st_gyro_common_probe(indio_dev);
	if (err < 0)
		goto st_gyro_power_off;

	return 0;

st_gyro_power_off:
	st_sensors_power_disable(indio_dev);

	return err;
>>>>>>> upstream/android-13
}

static int st_gyro_i2c_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	st_gyro_common_remove(i2c_get_clientdata(client));
=======
	struct iio_dev *indio_dev = i2c_get_clientdata(client);

	st_gyro_common_remove(indio_dev);

	st_sensors_power_disable(indio_dev);
>>>>>>> upstream/android-13

	return 0;
}

static const struct i2c_device_id st_gyro_id_table[] = {
	{ L3G4200D_GYRO_DEV_NAME },
	{ LSM330D_GYRO_DEV_NAME },
	{ LSM330DL_GYRO_DEV_NAME },
	{ LSM330DLC_GYRO_DEV_NAME },
	{ L3GD20_GYRO_DEV_NAME },
	{ L3GD20H_GYRO_DEV_NAME },
	{ L3G4IS_GYRO_DEV_NAME },
	{ LSM330_GYRO_DEV_NAME },
	{ LSM9DS0_GYRO_DEV_NAME },
	{},
};
MODULE_DEVICE_TABLE(i2c, st_gyro_id_table);

static struct i2c_driver st_gyro_driver = {
	.driver = {
		.name = "st-gyro-i2c",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(st_gyro_of_match),
=======
		.of_match_table = st_gyro_of_match,
>>>>>>> upstream/android-13
	},
	.probe = st_gyro_i2c_probe,
	.remove = st_gyro_i2c_remove,
	.id_table = st_gyro_id_table,
};
module_i2c_driver(st_gyro_driver);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics gyroscopes i2c driver");
MODULE_LICENSE("GPL v2");
