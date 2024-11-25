<<<<<<< HEAD
/*
 * HID Sensors Driver
 * Copyright (c) 2012, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * HID Sensors Driver
 * Copyright (c) 2012, Intel Corporation.
>>>>>>> upstream/android-13
 */
#ifndef _HID_SENSOR_TRIGGER_H
#define _HID_SENSOR_TRIGGER_H

#include <linux/pm.h>
#include <linux/pm_runtime.h>

<<<<<<< HEAD
=======
struct hid_sensor_common;
struct iio_dev;

>>>>>>> upstream/android-13
extern const struct dev_pm_ops hid_sensor_pm_ops;

int hid_sensor_setup_trigger(struct iio_dev *indio_dev, const char *name,
				struct hid_sensor_common *attrb);
<<<<<<< HEAD
void hid_sensor_remove_trigger(struct hid_sensor_common *attrb);
=======
void hid_sensor_remove_trigger(struct iio_dev *indio_dev,
			       struct hid_sensor_common *attrb);
>>>>>>> upstream/android-13
int hid_sensor_power_state(struct hid_sensor_common *st, bool state);

#endif
