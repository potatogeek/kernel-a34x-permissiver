<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * STMicroelectronics sensors spi library driver
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

#ifndef ST_SENSORS_SPI_H
#define ST_SENSORS_SPI_H

#include <linux/spi/spi.h>
#include <linux/iio/common/st_sensors.h>

<<<<<<< HEAD
void st_sensors_spi_configure(struct iio_dev *indio_dev,
			struct spi_device *spi, struct st_sensor_data *sdata);
=======
int st_sensors_spi_configure(struct iio_dev *indio_dev,
			     struct spi_device *spi);
>>>>>>> upstream/android-13

#endif /* ST_SENSORS_SPI_H */
