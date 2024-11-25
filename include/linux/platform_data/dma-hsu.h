<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Driver for the High Speed UART DMA
 *
 * Copyright (C) 2015 Intel Corporation
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef _PLATFORM_DATA_DMA_HSU_H
#define _PLATFORM_DATA_DMA_HSU_H

#include <linux/device.h>

struct hsu_dma_slave {
	struct device	*dma_dev;
	int		chan_id;
};

#endif /* _PLATFORM_DATA_DMA_HSU_H */
