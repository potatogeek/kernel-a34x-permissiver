<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Platform data for LPC32xx SoC SLC NAND controller
 *
 * Copyright © 2012 Roland Stigge
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef __LINUX_MTD_LPC32XX_SLC_H
#define __LINUX_MTD_LPC32XX_SLC_H

#include <linux/dmaengine.h>

struct lpc32xx_slc_platform_data {
	bool (*dma_filter)(struct dma_chan *chan, void *filter_param);
};

#endif  /* __LINUX_MTD_LPC32XX_SLC_H */
