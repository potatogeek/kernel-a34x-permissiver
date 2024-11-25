<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * ITE Tech IT9137 silicon tuner driver
 *
 *  Copyright (C) 2011 Malcolm Priestley (tvboxspy@gmail.com)
 *  IT9137 Copyright (C) ITE Tech Inc.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef IT913X_H
#define IT913X_H

#include <media/dvb_frontend.h>

/**
 * struct it913x_platform_data - Platform data for the it913x driver
 * @regmap: af9033 demod driver regmap.
<<<<<<< HEAD
 * @dvb_frontend: af9033 demod driver DVB frontend.
=======
 * @fe: af9033 demod driver DVB frontend.
>>>>>>> upstream/android-13
 * @role: Chip role, single or dual configuration.
 */

struct it913x_platform_data {
	struct regmap *regmap;
	struct dvb_frontend *fe;
#define IT913X_ROLE_SINGLE         0
#define IT913X_ROLE_DUAL_MASTER    1
#define IT913X_ROLE_DUAL_SLAVE     2
	unsigned int role:2;
};

#endif
