<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * STMicroelectronics sensors i2c library driver
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

#ifndef ST_SENSORS_I2C_H
#define ST_SENSORS_I2C_H

#include <linux/i2c.h>
#include <linux/iio/common/st_sensors.h>
<<<<<<< HEAD
#include <linux/of.h>

void st_sensors_i2c_configure(struct iio_dev *indio_dev,
		struct i2c_client *client, struct st_sensor_data *sdata);

#ifdef CONFIG_ACPI
int st_sensors_match_acpi_device(struct device *dev);
#else
static inline int st_sensors_match_acpi_device(struct device *dev)
{
	return -ENODEV;
}
#endif
=======

int st_sensors_i2c_configure(struct iio_dev *indio_dev,
			     struct i2c_client *client);
>>>>>>> upstream/android-13

#endif /* ST_SENSORS_I2C_H */
