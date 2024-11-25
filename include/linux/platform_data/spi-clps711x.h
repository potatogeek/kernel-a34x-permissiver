<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  CLPS711X SPI bus driver definitions
 *
 *  Copyright (C) 2012 Alexander Shiyan <shc_work@mail.ru>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef ____LINUX_PLATFORM_DATA_SPI_CLPS711X_H
#define ____LINUX_PLATFORM_DATA_SPI_CLPS711X_H

/* Board specific platform_data */
struct spi_clps711x_pdata {
	int *chipselect;	/* Array of GPIO-numbers */
	int num_chipselect;	/* Total count of GPIOs */
};

#endif
