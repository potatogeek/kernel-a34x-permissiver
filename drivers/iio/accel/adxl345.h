<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * ADXL345 3-Axis Digital Accelerometer
 *
 * Copyright (c) 2017 Eva Rachel Retuya <eraretuya@gmail.com>
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of version 2 of
 * the GNU General Public License. See the file COPYING in the main
 * directory of this archive for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ADXL345_H_
#define _ADXL345_H_

enum adxl345_device_type {
	ADXL345,
	ADXL375,
};

int adxl345_core_probe(struct device *dev, struct regmap *regmap,
		       enum adxl345_device_type type, const char *name);
<<<<<<< HEAD
int adxl345_core_remove(struct device *dev);
=======
>>>>>>> upstream/android-13

#endif /* _ADXL345_H_ */
