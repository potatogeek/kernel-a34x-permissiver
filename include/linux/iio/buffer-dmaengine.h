<<<<<<< HEAD
/*
 * Copyright 2014-2015 Analog Devices Inc.
 *  Author: Lars-Peter Clausen <lars@metafoo.de>
 *
 * Licensed under the GPL-2 or later.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2014-2015 Analog Devices Inc.
 *  Author: Lars-Peter Clausen <lars@metafoo.de>
>>>>>>> upstream/android-13
 */

#ifndef __IIO_DMAENGINE_H__
#define __IIO_DMAENGINE_H__

<<<<<<< HEAD
struct iio_buffer;
struct device;

struct iio_buffer *iio_dmaengine_buffer_alloc(struct device *dev,
	const char *channel);
void iio_dmaengine_buffer_free(struct iio_buffer *buffer);
=======
struct iio_dev;
struct device;

int devm_iio_dmaengine_buffer_setup(struct device *dev,
				    struct iio_dev *indio_dev,
				    const char *channel);
>>>>>>> upstream/android-13

#endif
