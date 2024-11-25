<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  ALSA PCM interface for the Samsung SoC
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * ALSA PCM interface for the Samsung SoC
>>>>>>> upstream/android-13
 */

#ifndef _SAMSUNG_DMA_H
#define _SAMSUNG_DMA_H

#include <sound/dmaengine_pcm.h>

/*
 * @tx, @rx arguments can be NULL if the DMA channel names are "tx", "rx",
 * otherwise actual DMA channel names must be passed to this function.
 */
int samsung_asoc_dma_platform_register(struct device *dev, dma_filter_fn filter,
<<<<<<< HEAD
				       const char *tx, const char *rx);
=======
				       const char *tx, const char *rx,
				       struct device *dma_dev);
>>>>>>> upstream/android-13
#endif /* _SAMSUNG_DMA_H */
