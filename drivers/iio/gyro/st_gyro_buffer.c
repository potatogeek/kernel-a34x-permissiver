<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics gyroscopes driver
 *
 * Copyright 2012-2013 STMicroelectronics Inc.
 *
 * Denis Ciocca <denis.ciocca@st.com>
<<<<<<< HEAD
 *
 * Licensed under the GPL-2.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger_consumer.h>
=======
#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger.h>
>>>>>>> upstream/android-13
#include <linux/iio/triggered_buffer.h>

#include <linux/iio/common/st_sensors.h>
#include "st_gyro.h"

int st_gyro_trig_set_state(struct iio_trigger *trig, bool state)
{
	struct iio_dev *indio_dev = iio_trigger_get_drvdata(trig);

	return st_sensors_set_dataready_irq(indio_dev, state);
}

<<<<<<< HEAD
static int st_gyro_buffer_preenable(struct iio_dev *indio_dev)
{
	return st_sensors_set_enable(indio_dev, true);
}

static int st_gyro_buffer_postenable(struct iio_dev *indio_dev)
{
	int err;
	struct st_sensor_data *gdata = iio_priv(indio_dev);

	gdata->buffer_data = kmalloc(indio_dev->scan_bytes, GFP_KERNEL);
	if (gdata->buffer_data == NULL) {
		err = -ENOMEM;
		goto allocate_memory_error;
	}

	err = st_sensors_set_axis_enable(indio_dev,
					(u8)indio_dev->active_scan_mask[0]);
	if (err < 0)
		goto st_gyro_buffer_postenable_error;

	err = iio_triggered_buffer_postenable(indio_dev);
	if (err < 0)
		goto st_gyro_buffer_postenable_error;

	return err;

st_gyro_buffer_postenable_error:
	kfree(gdata->buffer_data);
allocate_memory_error:
=======
static int st_gyro_buffer_postenable(struct iio_dev *indio_dev)
{
	int err;

	err = st_sensors_set_axis_enable(indio_dev, indio_dev->active_scan_mask[0]);
	if (err < 0)
		return err;

	err = st_sensors_set_enable(indio_dev, true);
	if (err < 0)
		goto st_gyro_buffer_enable_all_axis;

	return 0;

st_gyro_buffer_enable_all_axis:
	st_sensors_set_axis_enable(indio_dev, ST_SENSORS_ENABLE_ALL_AXIS);
>>>>>>> upstream/android-13
	return err;
}

static int st_gyro_buffer_predisable(struct iio_dev *indio_dev)
{
	int err;
<<<<<<< HEAD
	struct st_sensor_data *gdata = iio_priv(indio_dev);

	err = iio_triggered_buffer_predisable(indio_dev);
	if (err < 0)
		goto st_gyro_buffer_predisable_error;

	err = st_sensors_set_axis_enable(indio_dev, ST_SENSORS_ENABLE_ALL_AXIS);
	if (err < 0)
		goto st_gyro_buffer_predisable_error;

	err = st_sensors_set_enable(indio_dev, false);

st_gyro_buffer_predisable_error:
	kfree(gdata->buffer_data);
	return err;
}

static const struct iio_buffer_setup_ops st_gyro_buffer_setup_ops = {
	.preenable = &st_gyro_buffer_preenable,
=======

	err = st_sensors_set_enable(indio_dev, false);
	if (err < 0)
		return err;

	return st_sensors_set_axis_enable(indio_dev, ST_SENSORS_ENABLE_ALL_AXIS);
}

static const struct iio_buffer_setup_ops st_gyro_buffer_setup_ops = {
>>>>>>> upstream/android-13
	.postenable = &st_gyro_buffer_postenable,
	.predisable = &st_gyro_buffer_predisable,
};

int st_gyro_allocate_ring(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
	return iio_triggered_buffer_setup(indio_dev, NULL,
		&st_sensors_trigger_handler, &st_gyro_buffer_setup_ops);
}

void st_gyro_deallocate_ring(struct iio_dev *indio_dev)
{
	iio_triggered_buffer_cleanup(indio_dev);
=======
	return devm_iio_triggered_buffer_setup(indio_dev->dev.parent, indio_dev,
		NULL, &st_sensors_trigger_handler, &st_gyro_buffer_setup_ops);
>>>>>>> upstream/android-13
}

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics gyroscopes buffer");
MODULE_LICENSE("GPL v2");
