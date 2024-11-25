<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13

/* The industrial I/O core, trigger consumer handling functions
 *
 * Copyright (c) 2008 Jonathan Cameron
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifdef CONFIG_IIO_TRIGGER
/**
 * iio_device_register_trigger_consumer() - set up an iio_dev to use triggers
 * @indio_dev: iio_dev associated with the device that will consume the trigger
<<<<<<< HEAD
 **/
void iio_device_register_trigger_consumer(struct iio_dev *indio_dev);
=======
 *
 * Return 0 if successful, negative otherwise
 **/
int iio_device_register_trigger_consumer(struct iio_dev *indio_dev);
>>>>>>> upstream/android-13

/**
 * iio_device_unregister_trigger_consumer() - reverse the registration process
 * @indio_dev: iio_dev associated with the device that consumed the trigger
 **/
void iio_device_unregister_trigger_consumer(struct iio_dev *indio_dev);

<<<<<<< HEAD
=======

int iio_trigger_attach_poll_func(struct iio_trigger *trig,
				 struct iio_poll_func *pf);
int iio_trigger_detach_poll_func(struct iio_trigger *trig,
				 struct iio_poll_func *pf);

>>>>>>> upstream/android-13
#else

/**
 * iio_device_register_trigger_consumer() - set up an iio_dev to use triggers
 * @indio_dev: iio_dev associated with the device that will consume the trigger
 **/
<<<<<<< HEAD
static int iio_device_register_trigger_consumer(struct iio_dev *indio_dev)
=======
static inline int iio_device_register_trigger_consumer(struct iio_dev *indio_dev)
>>>>>>> upstream/android-13
{
	return 0;
}

/**
 * iio_device_unregister_trigger_consumer() - reverse the registration process
 * @indio_dev: iio_dev associated with the device that consumed the trigger
 **/
<<<<<<< HEAD
static void iio_device_unregister_trigger_consumer(struct iio_dev *indio_dev)
{
}

=======
static inline void iio_device_unregister_trigger_consumer(struct iio_dev *indio_dev)
{
}

static inline int iio_trigger_attach_poll_func(struct iio_trigger *trig,
					       struct iio_poll_func *pf)
{
	return 0;
}
static inline int iio_trigger_detach_poll_func(struct iio_trigger *trig,
					       struct iio_poll_func *pf)
{
	return 0;
}

>>>>>>> upstream/android-13
#endif /* CONFIG_TRIGGER_CONSUMER */
