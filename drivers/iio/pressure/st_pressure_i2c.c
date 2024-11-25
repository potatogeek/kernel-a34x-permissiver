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
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/acpi.h>
=======
 */

#include <linux/acpi.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
>>>>>>> upstream/android-13
#include <linux/i2c.h>
#include <linux/iio/iio.h>

#include <linux/iio/common/st_sensors.h>
#include <linux/iio/common/st_sensors_i2c.h>
#include "st_pressure.h"

<<<<<<< HEAD
#ifdef CONFIG_OF
=======
>>>>>>> upstream/android-13
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
#define st_press_of_match NULL
#endif
=======
	{
		.compatible = "st,lps22hh",
		.data = LPS22HH_PRESS_DEV_NAME,
	},
	{},
};
MODULE_DEVICE_TABLE(of, st_press_of_match);
>>>>>>> upstream/android-13

#ifdef CONFIG_ACPI
static const struct acpi_device_id st_press_acpi_match[] = {
	{"SNO9210", LPS22HB},
	{ },
};
MODULE_DEVICE_TABLE(acpi, st_press_acpi_match);
<<<<<<< HEAD
#else
#define st_press_acpi_match NULL
=======
>>>>>>> upstream/android-13
#endif

static const struct i2c_device_id st_press_id_table[] = {
	{ LPS001WP_PRESS_DEV_NAME, LPS001WP },
	{ LPS25H_PRESS_DEV_NAME,  LPS25H },
	{ LPS331AP_PRESS_DEV_NAME, LPS331AP },
	{ LPS22HB_PRESS_DEV_NAME, LPS22HB },
	{ LPS33HW_PRESS_DEV_NAME, LPS33HW },
	{ LPS35HW_PRESS_DEV_NAME, LPS35HW },
<<<<<<< HEAD
=======
	{ LPS22HH_PRESS_DEV_NAME, LPS22HH },
>>>>>>> upstream/android-13
	{},
};
MODULE_DEVICE_TABLE(i2c, st_press_id_table);

static int st_press_i2c_probe(struct i2c_client *client,
<<<<<<< HEAD
						const struct i2c_device_id *id)
{
	struct iio_dev *indio_dev;
	struct st_sensor_data *press_data;
	int ret;

=======
			      const struct i2c_device_id *id)
{
	const struct st_sensor_settings *settings;
	struct st_sensor_data *press_data;
	struct iio_dev *indio_dev;
	int ret;

	st_sensors_dev_name_probe(&client->dev, client->name, sizeof(client->name));

	settings = st_press_get_settings(client->name);
	if (!settings) {
		dev_err(&client->dev, "device name %s not recognized.\n",
			client->name);
		return -ENODEV;
	}

>>>>>>> upstream/android-13
	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*press_data));
	if (!indio_dev)
		return -ENOMEM;

	press_data = iio_priv(indio_dev);
<<<<<<< HEAD

	if (client->dev.of_node) {
		st_sensors_of_name_probe(&client->dev, st_press_of_match,
					 client->name, sizeof(client->name));
	} else if (ACPI_HANDLE(&client->dev)) {
		ret = st_sensors_match_acpi_device(&client->dev);
		if ((ret < 0) || (ret >= ST_PRESS_MAX))
			return -ENODEV;

		strlcpy(client->name, st_press_id_table[ret].name,
				sizeof(client->name));
	} else if (!id)
		return -ENODEV;

	st_sensors_i2c_configure(indio_dev, client, press_data);

	ret = st_press_common_probe(indio_dev);
	if (ret < 0)
		return ret;

	return 0;
=======
	press_data->sensor_settings = (struct st_sensor_settings *)settings;

	ret = st_sensors_i2c_configure(indio_dev, client);
	if (ret < 0)
		return ret;

	ret = st_sensors_power_enable(indio_dev);
	if (ret)
		return ret;

	ret = st_press_common_probe(indio_dev);
	if (ret < 0)
		goto st_press_power_off;

	return 0;

st_press_power_off:
	st_sensors_power_disable(indio_dev);

	return ret;
>>>>>>> upstream/android-13
}

static int st_press_i2c_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	st_press_common_remove(i2c_get_clientdata(client));
=======
	struct iio_dev *indio_dev = i2c_get_clientdata(client);

	st_press_common_remove(indio_dev);

	st_sensors_power_disable(indio_dev);
>>>>>>> upstream/android-13

	return 0;
}

static struct i2c_driver st_press_driver = {
	.driver = {
		.name = "st-press-i2c",
<<<<<<< HEAD
		.of_match_table = of_match_ptr(st_press_of_match),
=======
		.of_match_table = st_press_of_match,
>>>>>>> upstream/android-13
		.acpi_match_table = ACPI_PTR(st_press_acpi_match),
	},
	.probe = st_press_i2c_probe,
	.remove = st_press_i2c_remove,
	.id_table = st_press_id_table,
};
module_i2c_driver(st_press_driver);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics pressures i2c driver");
MODULE_LICENSE("GPL v2");
