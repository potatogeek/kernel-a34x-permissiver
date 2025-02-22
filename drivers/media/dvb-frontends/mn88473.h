<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Panasonic MN88473 DVB-T/T2/C demodulator driver
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

#ifndef MN88473_H
#define MN88473_H

#include <linux/dvb/frontend.h>

struct mn88473_config {
	/*
	 * Max num of bytes given I2C adapter could write at once.
	 * Default: unlimited
	 */
	u16 i2c_wr_max;

	/*
	 * Xtal frequency Hz.
	 * Default: 25000000
	 */
	u32 xtal;


	/* Everything after that is returned by the driver. */

	/*
	 * DVB frontend.
	 */
	struct dvb_frontend **fe;
};

#endif
