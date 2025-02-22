<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * include/media/drv-intf/si476x.h -- Common definitions for si476x driver
 *
 * Copyright (C) 2012 Innovative Converged Devices(ICD)
 * Copyright (C) 2013 Andrey Smirnov
 *
 * Author: Andrey Smirnov <andrew.smirnov@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef SI476X_H
#define SI476X_H

#include <linux/types.h>
#include <linux/videodev2.h>

#include <linux/mfd/si476x-reports.h>

enum si476x_ctrl_id {
	V4L2_CID_SI476X_RSSI_THRESHOLD	= (V4L2_CID_USER_SI476X_BASE + 1),
	V4L2_CID_SI476X_SNR_THRESHOLD	= (V4L2_CID_USER_SI476X_BASE + 2),
	V4L2_CID_SI476X_MAX_TUNE_ERROR	= (V4L2_CID_USER_SI476X_BASE + 3),
	V4L2_CID_SI476X_HARMONICS_COUNT	= (V4L2_CID_USER_SI476X_BASE + 4),
	V4L2_CID_SI476X_DIVERSITY_MODE	= (V4L2_CID_USER_SI476X_BASE + 5),
	V4L2_CID_SI476X_INTERCHIP_LINK	= (V4L2_CID_USER_SI476X_BASE + 6),
};

#endif /* SI476X_H*/
