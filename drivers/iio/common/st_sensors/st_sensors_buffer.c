<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * STMicroelectronics sensors buffer library driver
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

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/slab.h>
=======
>>>>>>> upstream/android-13
#include <linux/iio/iio.h>
#include <linux/iio/trigger.h>
#include <linux/interrupt.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger_consumer.h>
<<<<<<< HEAD
#include <linux/iio/triggered_buffer.h>
#include <linux/irqreturn.h>
=======
#include <linux/irqreturn.h>
#include <linux/regmap.h>
>>>>>>> upstream/android-13

#include <linux/iio/common/st_sensors.h>


static int st_sensors_get_buffer_element(struct iio_dev *indio_dev, u8 *buf)
{
<<<<<<< HEAD
	int i;
	struct st_sensor_data *sdata = iio_priv(indio_dev);
	unsigned int num_data_channels = sdata->num_data_channels;
=======
	struct st_sensor_data *sdata = iio_priv(indio_dev);
	unsigned int num_data_channels = sdata->num_data_channels;
	int i;
>>>>>>> upstream/android-13

	for_each_set_bit(i, indio_dev->active_scan_mask, num_data_channels) {
		const struct iio_chan_spec *channel = &indio_dev->channels[i];
		unsigned int bytes_to_read =
			DIV_ROUND_UP(channel->scan_type.realbits +
				     channel->scan_type.shift, 8);
		unsigned int storage_bytes =
			channel->scan_type.storagebits >> 3;

		buf = PTR_ALIGN(buf, storage_bytes);
<<<<<<< HEAD
		if (sdata->tf->read_multiple_byte(&sdata->tb, sdata->dev,
						  channel->address,
						  bytes_to_read, buf,
						  sdata->multiread_bit) <
		    bytes_to_read)
=======
		if (regmap_bulk_read(sdata->regmap, channel->address,
				     buf, bytes_to_read) < 0)
>>>>>>> upstream/android-13
			return -EIO;

		/* Advance the buffer pointer */
		buf += storage_bytes;
	}

	return 0;
}

irqreturn_t st_sensors_trigger_handler(int irq, void *p)
{
	int len;
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct st_sensor_data *sdata = iio_priv(indio_dev);
	s64 timestamp;

	/*
<<<<<<< HEAD
	 * If we do timetamping here, do it before reading the values, because
=======
	 * If we do timestamping here, do it before reading the values, because
>>>>>>> upstream/android-13
	 * once we've read the values, new interrupts can occur (when using
	 * the hardware trigger) and the hw_timestamp may get updated.
	 * By storing it in a local variable first, we are safe.
	 */
	if (iio_trigger_using_own(indio_dev))
		timestamp = sdata->hw_timestamp;
	else
		timestamp = iio_get_time_ns(indio_dev);

	len = st_sensors_get_buffer_element(indio_dev, sdata->buffer_data);
	if (len < 0)
		goto st_sensors_get_buffer_element_error;

	iio_push_to_buffers_with_timestamp(indio_dev, sdata->buffer_data,
					   timestamp);

st_sensors_get_buffer_element_error:
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL(st_sensors_trigger_handler);

MODULE_AUTHOR("Denis Ciocca <denis.ciocca@st.com>");
MODULE_DESCRIPTION("STMicroelectronics ST-sensors buffer");
MODULE_LICENSE("GPL v2");
