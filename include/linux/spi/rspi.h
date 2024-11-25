<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Renesas SPI driver
 *
 * Copyright (C) 2012  Renesas Solutions Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_SPI_RENESAS_SPI_H__
#define __LINUX_SPI_RENESAS_SPI_H__

struct rspi_plat_data {
	unsigned int dma_tx_id;
	unsigned int dma_rx_id;

	u16 num_chipselect;
};

#endif
