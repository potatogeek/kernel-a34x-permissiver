<<<<<<< HEAD
 /*
 * Copyright (c) 2012 Analog Devices, Inc.
 *  Author: Lars-Peter Clausen <lars@metafoo.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
 /*
 * Copyright (c) 2012 Analog Devices, Inc.
 *  Author: Lars-Peter Clausen <lars@metafoo.de>
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/iio/iio.h>
#include <linux/iio/buffer.h>
<<<<<<< HEAD
=======
#include <linux/iio/buffer_impl.h>
>>>>>>> upstream/android-13
#include <linux/iio/kfifo_buf.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>

<<<<<<< HEAD
static const struct iio_buffer_setup_ops iio_triggered_buffer_setup_ops = {
	.postenable = &iio_triggered_buffer_postenable,
	.predisable = &iio_triggered_buffer_predisable,
};

/**
 * iio_triggered_buffer_setup() - Setup triggered buffer and pollfunc
=======
/**
 * iio_triggered_buffer_setup_ext() - Setup triggered buffer and pollfunc
>>>>>>> upstream/android-13
 * @indio_dev:		IIO device structure
 * @h:			Function which will be used as pollfunc top half
 * @thread:		Function which will be used as pollfunc bottom half
 * @setup_ops:		Buffer setup functions to use for this device.
 *			If NULL the default setup functions for triggered
 *			buffers will be used.
<<<<<<< HEAD
=======
 * @buffer_attrs:	Extra sysfs buffer attributes for this IIO buffer
>>>>>>> upstream/android-13
 *
 * This function combines some common tasks which will normally be performed
 * when setting up a triggered buffer. It will allocate the buffer and the
 * pollfunc.
 *
 * Before calling this function the indio_dev structure should already be
 * completely initialized, but not yet registered. In practice this means that
 * this function should be called right before iio_device_register().
 *
 * To free the resources allocated by this function call
 * iio_triggered_buffer_cleanup().
 */
<<<<<<< HEAD
int iio_triggered_buffer_setup(struct iio_dev *indio_dev,
	irqreturn_t (*h)(int irq, void *p),
	irqreturn_t (*thread)(int irq, void *p),
	const struct iio_buffer_setup_ops *setup_ops)
=======
int iio_triggered_buffer_setup_ext(struct iio_dev *indio_dev,
	irqreturn_t (*h)(int irq, void *p),
	irqreturn_t (*thread)(int irq, void *p),
	const struct iio_buffer_setup_ops *setup_ops,
	const struct attribute **buffer_attrs)
>>>>>>> upstream/android-13
{
	struct iio_buffer *buffer;
	int ret;

	buffer = iio_kfifo_allocate();
	if (!buffer) {
		ret = -ENOMEM;
		goto error_ret;
	}

<<<<<<< HEAD
	iio_device_attach_buffer(indio_dev, buffer);

=======
>>>>>>> upstream/android-13
	indio_dev->pollfunc = iio_alloc_pollfunc(h,
						 thread,
						 IRQF_ONESHOT,
						 indio_dev,
						 "%s_consumer%d",
						 indio_dev->name,
<<<<<<< HEAD
						 indio_dev->id);
=======
						 iio_device_id(indio_dev));
>>>>>>> upstream/android-13
	if (indio_dev->pollfunc == NULL) {
		ret = -ENOMEM;
		goto error_kfifo_free;
	}

	/* Ring buffer functions - here trigger setup related */
<<<<<<< HEAD
	if (setup_ops)
		indio_dev->setup_ops = setup_ops;
	else
		indio_dev->setup_ops = &iio_triggered_buffer_setup_ops;
=======
	indio_dev->setup_ops = setup_ops;
>>>>>>> upstream/android-13

	/* Flag that polled ring buffering is possible */
	indio_dev->modes |= INDIO_BUFFER_TRIGGERED;

<<<<<<< HEAD
	return 0;

error_kfifo_free:
	iio_kfifo_free(indio_dev->buffer);
error_ret:
	return ret;
}
EXPORT_SYMBOL(iio_triggered_buffer_setup);

/**
 * iio_triggered_buffer_cleanup() - Free resources allocated by iio_triggered_buffer_setup()
=======
	buffer->attrs = buffer_attrs;

	ret = iio_device_attach_buffer(indio_dev, buffer);
	if (ret < 0)
		goto error_dealloc_pollfunc;

	return 0;

error_dealloc_pollfunc:
	iio_dealloc_pollfunc(indio_dev->pollfunc);
error_kfifo_free:
	iio_kfifo_free(buffer);
error_ret:
	return ret;
}
EXPORT_SYMBOL(iio_triggered_buffer_setup_ext);

/**
 * iio_triggered_buffer_cleanup() - Free resources allocated by iio_triggered_buffer_setup_ext()
>>>>>>> upstream/android-13
 * @indio_dev: IIO device structure
 */
void iio_triggered_buffer_cleanup(struct iio_dev *indio_dev)
{
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	iio_kfifo_free(indio_dev->buffer);
}
EXPORT_SYMBOL(iio_triggered_buffer_cleanup);

<<<<<<< HEAD
static void devm_iio_triggered_buffer_clean(struct device *dev, void *res)
{
	iio_triggered_buffer_cleanup(*(struct iio_dev **)res);
}

int devm_iio_triggered_buffer_setup(struct device *dev,
				    struct iio_dev *indio_dev,
				    irqreturn_t (*h)(int irq, void *p),
				    irqreturn_t (*thread)(int irq, void *p),
				    const struct iio_buffer_setup_ops *ops)
{
	struct iio_dev **ptr;
	int ret;

	ptr = devres_alloc(devm_iio_triggered_buffer_clean, sizeof(*ptr),
			   GFP_KERNEL);
	if (!ptr)
		return -ENOMEM;

	*ptr = indio_dev;

	ret = iio_triggered_buffer_setup(indio_dev, h, thread, ops);
	if (!ret)
		devres_add(dev, ptr);
	else
		devres_free(ptr);

	return ret;
}
EXPORT_SYMBOL_GPL(devm_iio_triggered_buffer_setup);

void devm_iio_triggered_buffer_cleanup(struct device *dev,
				       struct iio_dev *indio_dev)
{
	int rc;

	rc = devres_release(dev, devm_iio_triggered_buffer_clean,
			    devm_iio_device_match, indio_dev);
	WARN_ON(rc);
}
EXPORT_SYMBOL_GPL(devm_iio_triggered_buffer_cleanup);
=======
static void devm_iio_triggered_buffer_clean(void *indio_dev)
{
	iio_triggered_buffer_cleanup(indio_dev);
}

int devm_iio_triggered_buffer_setup_ext(struct device *dev,
					struct iio_dev *indio_dev,
					irqreturn_t (*h)(int irq, void *p),
					irqreturn_t (*thread)(int irq, void *p),
					const struct iio_buffer_setup_ops *ops,
					const struct attribute **buffer_attrs)
{
	int ret;

	ret = iio_triggered_buffer_setup_ext(indio_dev, h, thread, ops,
					     buffer_attrs);
	if (ret)
		return ret;

	return devm_add_action_or_reset(dev, devm_iio_triggered_buffer_clean,
					indio_dev);
}
EXPORT_SYMBOL_GPL(devm_iio_triggered_buffer_setup_ext);
>>>>>>> upstream/android-13

MODULE_AUTHOR("Lars-Peter Clausen <lars@metafoo.de>");
MODULE_DESCRIPTION("IIO helper functions for setting up triggered buffers");
MODULE_LICENSE("GPL");
