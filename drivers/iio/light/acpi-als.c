<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * ACPI Ambient Light Sensor Driver
 *
 * Based on ALS driver:
 * Copyright (C) 2009 Zhang Rui <rui.zhang@intel.com>
 *
 * Rework for IIO subsystem:
 * Copyright (C) 2012-2013 Martin Liska <marxin.liska@gmail.com>
 *
 * Final cleanup and debugging:
 * Copyright (C) 2013-2014 Marek Vasut <marex@denx.de>
 * Copyright (C) 2015 Gabriele Mazzotta <gabriele.mzt@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/acpi.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/irq.h>
>>>>>>> upstream/android-13
#include <linux/mutex.h>

#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
<<<<<<< HEAD
#include <linux/iio/kfifo_buf.h>
=======
#include <linux/iio/trigger.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>
>>>>>>> upstream/android-13

#define ACPI_ALS_CLASS			"als"
#define ACPI_ALS_DEVICE_NAME		"acpi-als"
#define ACPI_ALS_NOTIFY_ILLUMINANCE	0x80

<<<<<<< HEAD
ACPI_MODULE_NAME("acpi-als");

=======
>>>>>>> upstream/android-13
/*
 * So far, there's only one channel in here, but the specification for
 * ACPI0008 says there can be more to what the block can report. Like
 * chromaticity and such. We are ready for incoming additions!
 */
static const struct iio_chan_spec acpi_als_channels[] = {
	{
		.type		= IIO_LIGHT,
		.scan_type	= {
			.sign		= 's',
			.realbits	= 32,
			.storagebits	= 32,
		},
		/* _RAW is here for backward ABI compatibility */
		.info_mask_separate	= BIT(IIO_CHAN_INFO_RAW) |
					  BIT(IIO_CHAN_INFO_PROCESSED),
	},
<<<<<<< HEAD
=======
	IIO_CHAN_SOFT_TIMESTAMP(1),
>>>>>>> upstream/android-13
};

/*
 * The event buffer contains timestamp and all the data from
 * the ACPI0008 block. There are multiple, but so far we only
<<<<<<< HEAD
 * support _ALI (illuminance). Once someone adds new channels
 * to acpi_als_channels[], the evt_buffer below will grow
 * automatically.
 */
#define ACPI_ALS_EVT_NR_SOURCES		ARRAY_SIZE(acpi_als_channels)
#define ACPI_ALS_EVT_BUFFER_SIZE		\
	(sizeof(s64) + (ACPI_ALS_EVT_NR_SOURCES * sizeof(s32)))
=======
 * support _ALI (illuminance): One channel, padding and timestamp.
 */
#define ACPI_ALS_EVT_BUFFER_SIZE		\
	(sizeof(s32) + sizeof(s32) + sizeof(s64))
>>>>>>> upstream/android-13

struct acpi_als {
	struct acpi_device	*device;
	struct mutex		lock;
<<<<<<< HEAD

	s32			evt_buffer[ACPI_ALS_EVT_BUFFER_SIZE];
=======
	struct iio_trigger	*trig;

	s32 evt_buffer[ACPI_ALS_EVT_BUFFER_SIZE / sizeof(s32)]  __aligned(8);
>>>>>>> upstream/android-13
};

/*
 * All types of properties the ACPI0008 block can report. The ALI, ALC, ALT
 * and ALP can all be handled by acpi_als_read_value() below, while the ALR is
 * special.
 *
 * The _ALR property returns tables that can be used to fine-tune the values
 * reported by the other props based on the particular hardware type and it's
 * location (it contains tables for "rainy", "bright inhouse lighting" etc.).
 *
 * So far, we support only ALI (illuminance).
 */
#define ACPI_ALS_ILLUMINANCE	"_ALI"
#define ACPI_ALS_CHROMATICITY	"_ALC"
#define ACPI_ALS_COLOR_TEMP	"_ALT"
#define ACPI_ALS_POLLING	"_ALP"
#define ACPI_ALS_TABLES		"_ALR"

static int acpi_als_read_value(struct acpi_als *als, char *prop, s32 *val)
{
	unsigned long long temp_val;
	acpi_status status;

	status = acpi_evaluate_integer(als->device->handle, prop, NULL,
				       &temp_val);

	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
		ACPI_EXCEPTION((AE_INFO, status, "Error reading ALS %s", prop));
=======
		acpi_evaluation_failure_warn(als->device->handle, prop, status);
>>>>>>> upstream/android-13
		return -EIO;
	}

	*val = temp_val;

	return 0;
}

static void acpi_als_notify(struct acpi_device *device, u32 event)
{
	struct iio_dev *indio_dev = acpi_driver_data(device);
	struct acpi_als *als = iio_priv(indio_dev);
<<<<<<< HEAD
	s32 *buffer = als->evt_buffer;
	s64 time_ns = iio_get_time_ns(indio_dev);
	s32 val;
	int ret;

	mutex_lock(&als->lock);

	memset(buffer, 0, ACPI_ALS_EVT_BUFFER_SIZE);

	switch (event) {
	case ACPI_ALS_NOTIFY_ILLUMINANCE:
		ret = acpi_als_read_value(als, ACPI_ALS_ILLUMINANCE, &val);
		if (ret < 0)
			goto out;
		*buffer++ = val;
		break;
	default:
		/* Unhandled event */
		dev_dbg(&device->dev, "Unhandled ACPI ALS event (%08x)!\n",
			event);
		goto out;
	}

	iio_push_to_buffers_with_timestamp(indio_dev, als->evt_buffer, time_ns);

out:
	mutex_unlock(&als->lock);
=======

	if (iio_buffer_enabled(indio_dev) && iio_trigger_using_own(indio_dev)) {
		switch (event) {
		case ACPI_ALS_NOTIFY_ILLUMINANCE:
			iio_trigger_poll_chained(als->trig);
			break;
		default:
			/* Unhandled event */
			dev_dbg(&device->dev,
				"Unhandled ACPI ALS event (%08x)!\n",
				event);
		}
	}
>>>>>>> upstream/android-13
}

static int acpi_als_read_raw(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan, int *val,
			     int *val2, long mask)
{
	struct acpi_als *als = iio_priv(indio_dev);
	s32 temp_val;
	int ret;

	if ((mask != IIO_CHAN_INFO_PROCESSED) && (mask != IIO_CHAN_INFO_RAW))
		return -EINVAL;

	/* we support only illumination (_ALI) so far. */
	if (chan->type != IIO_LIGHT)
		return -EINVAL;

	ret = acpi_als_read_value(als, ACPI_ALS_ILLUMINANCE, &temp_val);
	if (ret < 0)
		return ret;

	*val = temp_val;

	return IIO_VAL_INT;
}

static const struct iio_info acpi_als_info = {
	.read_raw		= acpi_als_read_raw,
};

<<<<<<< HEAD
static int acpi_als_add(struct acpi_device *device)
{
	struct acpi_als *als;
	struct iio_dev *indio_dev;
	struct iio_buffer *buffer;

	indio_dev = devm_iio_device_alloc(&device->dev, sizeof(*als));
=======
static irqreturn_t acpi_als_trigger_handler(int irq, void *p)
{
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct acpi_als *als = iio_priv(indio_dev);
	s32 *buffer = als->evt_buffer;
	s32 val;
	int ret;

	mutex_lock(&als->lock);

	ret = acpi_als_read_value(als, ACPI_ALS_ILLUMINANCE, &val);
	if (ret < 0)
		goto out;
	*buffer = val;

	/*
	 * When coming from own trigger via polls, set polling function
	 * timestamp here. Given ACPI notifier is already in a thread and call
	 * function directly, there is no need to set the timestamp in the
	 * notify function.
	 *
	 * If the timestamp was actually 0, the timestamp is set one more time.
	 */
	if (!pf->timestamp)
		pf->timestamp = iio_get_time_ns(indio_dev);

	iio_push_to_buffers_with_timestamp(indio_dev, buffer, pf->timestamp);
out:
	mutex_unlock(&als->lock);
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}

static int acpi_als_add(struct acpi_device *device)
{
	struct device *dev = &device->dev;
	struct iio_dev *indio_dev;
	struct acpi_als *als;
	int ret;

	indio_dev = devm_iio_device_alloc(dev, sizeof(*als));
>>>>>>> upstream/android-13
	if (!indio_dev)
		return -ENOMEM;

	als = iio_priv(indio_dev);

	device->driver_data = indio_dev;
	als->device = device;
	mutex_init(&als->lock);

	indio_dev->name = ACPI_ALS_DEVICE_NAME;
<<<<<<< HEAD
	indio_dev->dev.parent = &device->dev;
	indio_dev->info = &acpi_als_info;
	indio_dev->modes = INDIO_BUFFER_SOFTWARE;
	indio_dev->channels = acpi_als_channels;
	indio_dev->num_channels = ARRAY_SIZE(acpi_als_channels);

	buffer = devm_iio_kfifo_allocate(&device->dev);
	if (!buffer)
		return -ENOMEM;

	iio_device_attach_buffer(indio_dev, buffer);

	return devm_iio_device_register(&device->dev, indio_dev);
=======
	indio_dev->info = &acpi_als_info;
	indio_dev->channels = acpi_als_channels;
	indio_dev->num_channels = ARRAY_SIZE(acpi_als_channels);

	als->trig = devm_iio_trigger_alloc(dev, "%s-dev%d", indio_dev->name,
					   iio_device_id(indio_dev));
	if (!als->trig)
		return -ENOMEM;

	ret = devm_iio_trigger_register(dev, als->trig);
	if (ret)
		return ret;
	/*
	 * Set hardware trigger by default to let events flow when
	 * BIOS support notification.
	 */
	indio_dev->trig = iio_trigger_get(als->trig);

	ret = devm_iio_triggered_buffer_setup(dev, indio_dev,
					      iio_pollfunc_store_time,
					      acpi_als_trigger_handler,
					      NULL);
	if (ret)
		return ret;

	return devm_iio_device_register(dev, indio_dev);
>>>>>>> upstream/android-13
}

static const struct acpi_device_id acpi_als_device_ids[] = {
	{"ACPI0008", 0},
	{},
};

MODULE_DEVICE_TABLE(acpi, acpi_als_device_ids);

static struct acpi_driver acpi_als_driver = {
	.name	= "acpi_als",
	.class	= ACPI_ALS_CLASS,
	.ids	= acpi_als_device_ids,
	.ops = {
		.add	= acpi_als_add,
		.notify	= acpi_als_notify,
	},
};

module_acpi_driver(acpi_als_driver);

MODULE_AUTHOR("Zhang Rui <rui.zhang@intel.com>");
MODULE_AUTHOR("Martin Liska <marxin.liska@gmail.com>");
MODULE_AUTHOR("Marek Vasut <marex@denx.de>");
MODULE_DESCRIPTION("ACPI Ambient Light Sensor Driver");
MODULE_LICENSE("GPL");
