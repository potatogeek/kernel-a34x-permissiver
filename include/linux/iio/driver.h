<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Industrial I/O in kernel access map interface.
 *
 * Copyright (c) 2011 Jonathan Cameron
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _IIO_INKERN_H_
#define _IIO_INKERN_H_

<<<<<<< HEAD
=======
struct iio_dev;
>>>>>>> upstream/android-13
struct iio_map;

/**
 * iio_map_array_register() - tell the core about inkernel consumers
 * @indio_dev:	provider device
 * @map:	array of mappings specifying association of channel with client
 */
int iio_map_array_register(struct iio_dev *indio_dev,
			   struct iio_map *map);

/**
 * iio_map_array_unregister() - tell the core to remove consumer mappings for
 *				the given provider device
 * @indio_dev:	provider device
 */
int iio_map_array_unregister(struct iio_dev *indio_dev);

#endif
