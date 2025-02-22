<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Regulator Haptic Platform Data
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 * Author: Jaewon Kim <jaewon02.kim@samsung.com>
 * Author: Hyunhee Kim <hyunhee.kim@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _REGULATOR_HAPTIC_H
#define _REGULATOR_HAPTIC_H

/*
 * struct regulator_haptic_data - Platform device data
 *
 * @max_volt: maximum voltage value supplied to the haptic motor.
 *		<The unit of the voltage is a micro>
 * @min_volt: minimum voltage value supplied to the haptic motor.
 *		<The unit of the voltage is a micro>
 */
struct regulator_haptic_data {
	unsigned int max_volt;
	unsigned int min_volt;
};

#endif /* _REGULATOR_HAPTIC_H */
