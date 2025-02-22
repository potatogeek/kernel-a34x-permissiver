<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * linux/drivers/mmc/core/sdio_cis.h
 *
 * Author:	Nicolas Pitre
 * Created:	June 11, 2007
 * Copyright:	MontaVista Software Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _MMC_SDIO_CIS_H
#define _MMC_SDIO_CIS_H

struct mmc_card;
struct sdio_func;

int sdio_read_common_cis(struct mmc_card *card);
void sdio_free_common_cis(struct mmc_card *card);

int sdio_read_func_cis(struct sdio_func *func);
void sdio_free_func_cis(struct sdio_func *func);

#endif
