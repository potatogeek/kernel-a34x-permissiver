<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  hwmon-sysfs.h - hardware monitoring chip driver sysfs defines
 *
 *  Copyright (C) 2005 Yani Ioannou <yani.ioannou@gmail.com>
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
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
 */
#ifndef _LINUX_HWMON_SYSFS_H
#define _LINUX_HWMON_SYSFS_H

#include <linux/device.h>

struct sensor_device_attribute{
	struct device_attribute dev_attr;
	int index;
};
#define to_sensor_dev_attr(_dev_attr) \
	container_of(_dev_attr, struct sensor_device_attribute, dev_attr)

#define SENSOR_ATTR(_name, _mode, _show, _store, _index)	\
	{ .dev_attr = __ATTR(_name, _mode, _show, _store),	\
	  .index = _index }

<<<<<<< HEAD
=======
#define SENSOR_ATTR_RO(_name, _func, _index)			\
	SENSOR_ATTR(_name, 0444, _func##_show, NULL, _index)

#define SENSOR_ATTR_RW(_name, _func, _index)			\
	SENSOR_ATTR(_name, 0644, _func##_show, _func##_store, _index)

#define SENSOR_ATTR_WO(_name, _func, _index)			\
	SENSOR_ATTR(_name, 0200, NULL, _func##_store, _index)

>>>>>>> upstream/android-13
#define SENSOR_DEVICE_ATTR(_name, _mode, _show, _store, _index)	\
struct sensor_device_attribute sensor_dev_attr_##_name		\
	= SENSOR_ATTR(_name, _mode, _show, _store, _index)

<<<<<<< HEAD
=======
#define SENSOR_DEVICE_ATTR_RO(_name, _func, _index)		\
	SENSOR_DEVICE_ATTR(_name, 0444, _func##_show, NULL, _index)

#define SENSOR_DEVICE_ATTR_RW(_name, _func, _index)		\
	SENSOR_DEVICE_ATTR(_name, 0644, _func##_show, _func##_store, _index)

#define SENSOR_DEVICE_ATTR_WO(_name, _func, _index)		\
	SENSOR_DEVICE_ATTR(_name, 0200, NULL, _func##_store, _index)

>>>>>>> upstream/android-13
struct sensor_device_attribute_2 {
	struct device_attribute dev_attr;
	u8 index;
	u8 nr;
};
#define to_sensor_dev_attr_2(_dev_attr) \
	container_of(_dev_attr, struct sensor_device_attribute_2, dev_attr)

#define SENSOR_ATTR_2(_name, _mode, _show, _store, _nr, _index)	\
	{ .dev_attr = __ATTR(_name, _mode, _show, _store),	\
	  .index = _index,					\
	  .nr = _nr }

<<<<<<< HEAD
=======
#define SENSOR_ATTR_2_RO(_name, _func, _nr, _index)		\
	SENSOR_ATTR_2(_name, 0444, _func##_show, NULL, _nr, _index)

#define SENSOR_ATTR_2_RW(_name, _func, _nr, _index)		\
	SENSOR_ATTR_2(_name, 0644, _func##_show, _func##_store, _nr, _index)

#define SENSOR_ATTR_2_WO(_name, _func, _nr, _index)		\
	SENSOR_ATTR_2(_name, 0200, NULL, _func##_store, _nr, _index)

>>>>>>> upstream/android-13
#define SENSOR_DEVICE_ATTR_2(_name,_mode,_show,_store,_nr,_index)	\
struct sensor_device_attribute_2 sensor_dev_attr_##_name		\
	= SENSOR_ATTR_2(_name, _mode, _show, _store, _nr, _index)

<<<<<<< HEAD
=======
#define SENSOR_DEVICE_ATTR_2_RO(_name, _func, _nr, _index)		\
	SENSOR_DEVICE_ATTR_2(_name, 0444, _func##_show, NULL,		\
			     _nr, _index)

#define SENSOR_DEVICE_ATTR_2_RW(_name, _func, _nr, _index)		\
	SENSOR_DEVICE_ATTR_2(_name, 0644, _func##_show, _func##_store,	\
			     _nr, _index)

#define SENSOR_DEVICE_ATTR_2_WO(_name, _func, _nr, _index)		\
	SENSOR_DEVICE_ATTR_2(_name, 0200, NULL, _func##_store,		\
			     _nr, _index)

>>>>>>> upstream/android-13
#endif /* _LINUX_HWMON_SYSFS_H */
