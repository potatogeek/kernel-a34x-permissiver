/*
 *  Copyright (C) 2020, Samsung Electronics Co. Ltd. All Rights Reserved.
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
 */

#include "../sensormanager/shub_sensor.h"
#include "../sensormanager/shub_sensor_manager.h"
#include "../utility/shub_utility.h"

#include <linux/slab.h>

int init_significant_motion(bool en)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_SIGNIFICANT_MOTION);

	if (!sensor)
		return 0;

	if (en) {
<<<<<<< HEAD
		ret = init_default_func(sensor, "sig_motion_sensor", 1, 1, 1);
	} else {
		destroy_default_func(sensor);
	}

	return ret;
=======
		strcpy(sensor->name, "sig_motion_sensor");
		sensor->receive_event_size = 1;
		sensor->report_event_size = 1;
		sensor->event_buffer.value = kzalloc(sensor->receive_event_size, GFP_KERNEL);
		if (!sensor->event_buffer.value)
			goto err_no_mem;

	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;
	}

	return 0;

err_no_mem:
	kfree(sensor->event_buffer.value);
	sensor->event_buffer.value = NULL;

	return -ENOMEM;
>>>>>>> upstream/android-13
}

int init_tilt_detector(bool en)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_TILT_DETECTOR);

	if (!sensor)
		return 0;

	if (en) {
<<<<<<< HEAD
		ret = init_default_func(sensor, "tilt_detector", 1, 1, 1);
	} else {
		destroy_default_func(sensor);
	}

	return ret;
=======
		strcpy(sensor->name, "tilt_detector");
		sensor->receive_event_size = 1;
		sensor->report_event_size = 1;
		sensor->event_buffer.value = kzalloc(sensor->receive_event_size, GFP_KERNEL);
		if (!sensor->event_buffer.value)
			goto err_no_mem;
	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;
	}

	return 0;

err_no_mem:
	kfree(sensor->event_buffer.value);
	sensor->event_buffer.value = NULL;

	return -ENOMEM;
>>>>>>> upstream/android-13
}

int init_pick_up_gesture(bool en)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_PICK_UP_GESTURE);

	if (!sensor)
		return 0;

	if (en) {
<<<<<<< HEAD
		ret = init_default_func(sensor, "pickup_gesture", 1, 1, 1);
	} else {
		destroy_default_func(sensor);
	}

	return ret;
=======
		strcpy(sensor->name, "pickup_gesture");
		sensor->receive_event_size = 1;
		sensor->report_event_size = 1;
		sensor->event_buffer.value = kzalloc(sensor->receive_event_size, GFP_KERNEL);
		if (!sensor->event_buffer.value)
			goto err_no_mem;
	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;
	}
	return 0;

err_no_mem:
	kfree(sensor->event_buffer.value);
	sensor->event_buffer.value = NULL;

	return -ENOMEM;
>>>>>>> upstream/android-13
}

int init_call_gesture(bool en)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_CALL_GESTURE);

	if (!sensor)
		return 0;

	if (en) {
<<<<<<< HEAD
		ret = init_default_func(sensor, "call_gesture", 1, 1, 1);
	} else {
		destroy_default_func(sensor);
	}

	return ret;
=======
		strcpy(sensor->name, "call_gesture");
		sensor->receive_event_size = 1;
		sensor->report_event_size = 1;
		sensor->event_buffer.value = kzalloc(sensor->receive_event_size, GFP_KERNEL);
		if (!sensor->event_buffer.value)
			goto err_no_mem;
	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;
	}
	return 0;

err_no_mem:
	kfree(sensor->event_buffer.value);
	sensor->event_buffer.value = NULL;

	return -ENOMEM;
>>>>>>> upstream/android-13
}

int init_wake_up_motion(bool en)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_WAKE_UP_MOTION);

	if (!sensor)
		return 0;

	if (en) {
<<<<<<< HEAD
		ret = init_default_func(sensor, "wake_up_motion", 1, 1, 1);
	} else {
		destroy_default_func(sensor);
	}

	return ret;
=======
		strcpy(sensor->name, "wake_up_motion");
		sensor->receive_event_size = 1;
		sensor->report_event_size = 1;
		sensor->event_buffer.value = kzalloc(sensor->receive_event_size, GFP_KERNEL);
		if (!sensor->event_buffer.value)
			goto err_no_mem;
	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;
	}
	return 0;

err_no_mem:
	kfree(sensor->event_buffer.value);
	sensor->event_buffer.value = NULL;

	return -ENOMEM;
>>>>>>> upstream/android-13
}

int init_protos_motion(bool en)
{
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> upstream/android-13
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_PROTOS_MOTION);

	if (!sensor)
		return 0;

	if (en) {
<<<<<<< HEAD
		ret = init_default_func(sensor, "protos_motion", 1, 1, 1);
	} else {
		destroy_default_func(sensor);
	}

	return ret;
=======
		strcpy(sensor->name, "protos_motion");
		sensor->receive_event_size = 1;
		sensor->report_event_size = 1;
		sensor->event_buffer.value = kzalloc(sensor->receive_event_size, GFP_KERNEL);
		if (!sensor->event_buffer.value)
			goto err_no_mem;
	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;
	}
	return 0;

err_no_mem:
	kfree(sensor->event_buffer.value);
	sensor->event_buffer.value = NULL;

	return -ENOMEM;
>>>>>>> upstream/android-13
}
