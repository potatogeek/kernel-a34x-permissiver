<<<<<<< HEAD
/*
 * KXCJK-1013 3-axis accelerometer Interface
 * Copyright (c) 2014, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * KXCJK-1013 3-axis accelerometer Interface
 * Copyright (c) 2014, Intel Corporation.
>>>>>>> upstream/android-13
 */

#ifndef __IIO_KXCJK_1013_H__
#define __IIO_KXCJK_1013_H__

<<<<<<< HEAD
struct kxcjk_1013_platform_data {
	bool active_high_intr;
=======
#include <linux/iio/iio.h>

struct kxcjk_1013_platform_data {
	bool active_high_intr;
	struct iio_mount_matrix orientation;
>>>>>>> upstream/android-13
};

#endif
