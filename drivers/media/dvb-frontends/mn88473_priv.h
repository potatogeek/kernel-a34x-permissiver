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

#ifndef MN88473_PRIV_H
#define MN88473_PRIV_H

#include <media/dvb_frontend.h>
#include <media/dvb_math.h>
#include "mn88473.h"
#include <linux/math64.h>
#include <linux/firmware.h>
#include <linux/regmap.h>

#define MN88473_FIRMWARE "dvb-demod-mn88473-01.fw"

struct mn88473_dev {
	struct i2c_client *client[3];
	struct regmap *regmap[3];
	struct dvb_frontend frontend;
	u16 i2c_wr_max;
	bool active;
	u32 clk;
};

#endif
