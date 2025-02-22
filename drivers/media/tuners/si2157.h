<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Silicon Labs Si2146/2147/2148/2157/2158 silicon tuner driver
 *
 * Copyright (C) 2014 Antti Palosaari <crope@iki.fi>
<<<<<<< HEAD
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef SI2157_H
#define SI2157_H

#include <media/media-device.h>
#include <media/dvb_frontend.h>

<<<<<<< HEAD
/*
 * I2C address
 * 0x60
 */
struct si2157_config {
	/*
	 * frontend
	 */
=======
/**
 * struct si2157_config - configuration parameters for si2157
 *
 * @fe:
 *	frontend returned by driver
 * @mdev:
 *	media device returned by driver
 * @inversion:
 *	spectral inversion
 * @dont_load_firmware:
 *	Instead of uploading a new firmware, use the existing one
 * @if_port:
 *	Port selection
 *	Select the RF interface to use (pins 9+11 or 12+13)
 *
 * Note:
 *	The I2C address of this demod is 0x60.
 */
struct si2157_config {
>>>>>>> upstream/android-13
	struct dvb_frontend *fe;

#if defined(CONFIG_MEDIA_CONTROLLER)
	struct media_device *mdev;
#endif

<<<<<<< HEAD
	/*
	 * Spectral Inversion
	 */
	bool inversion;

	/*
	 * Port selection
	 * Select the RF interface to use (pins 9+11 or 12+13)
	 */
=======
	unsigned int inversion:1;
	unsigned int dont_load_firmware:1;

>>>>>>> upstream/android-13
	u8 if_port;
};

#endif
