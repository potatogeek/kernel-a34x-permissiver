<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * STMicroelectronics sensors platform-data driver
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

#ifndef ST_SENSORS_PDATA_H
#define ST_SENSORS_PDATA_H

/**
 * struct st_sensors_platform_data - Platform data for the ST sensors
 * @drdy_int_pin: Redirect DRDY on pin 1 (1) or pin 2 (2).
<<<<<<< HEAD
 *	Available only for accelerometer and pressure sensors.
 *	Accelerometer DRDY on LSM330 available only on pin 1 (see datasheet).
 * @open_drain: set the interrupt line to be open drain if possible.
 * @spi_3wire: enable spi-3wire mode.
=======
 *	Available only for accelerometer, magnetometer and pressure sensors.
 *	Accelerometer DRDY on LSM330 available only on pin 1 (see datasheet).
 *	Magnetometer DRDY is supported only on LSM9DS0.
 * @open_drain: set the interrupt line to be open drain if possible.
 * @spi_3wire: enable spi-3wire mode.
 * @pullups: enable/disable i2c controller pullup resistors.
 * @wakeup_source: enable/disable device as wakeup generator.
>>>>>>> upstream/android-13
 */
struct st_sensors_platform_data {
	u8 drdy_int_pin;
	bool open_drain;
	bool spi_3wire;
<<<<<<< HEAD
=======
	bool pullups;
	bool wakeup_source;
>>>>>>> upstream/android-13
};

#endif /* ST_SENSORS_PDATA_H */
