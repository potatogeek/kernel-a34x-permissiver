<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  linux/drivers/mmc/sdio_ops.c
 *
 *  Copyright 2006-2007 Pierre Ossman
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef _MMC_SDIO_OPS_H
#define _MMC_SDIO_OPS_H

#include <linux/types.h>
#include <linux/mmc/sdio.h>

struct mmc_host;
struct mmc_card;
struct work_struct;

int mmc_send_io_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr);
int mmc_io_rw_direct(struct mmc_card *card, int write, unsigned fn,
	unsigned addr, u8 in, u8* out);
int mmc_io_rw_extended(struct mmc_card *card, int write, unsigned fn,
	unsigned addr, int incr_addr, u8 *buf, unsigned blocks, unsigned blksz);
int sdio_reset(struct mmc_host *host);
<<<<<<< HEAD
unsigned int mmc_align_data_size(struct mmc_card *card, unsigned int sz);
=======
>>>>>>> upstream/android-13
void sdio_irq_work(struct work_struct *work);

static inline bool sdio_is_io_busy(u32 opcode, u32 arg)
{
	u32 addr;

	addr = (arg >> 9) & 0x1FFFF;

	return (opcode == SD_IO_RW_EXTENDED ||
		(opcode == SD_IO_RW_DIRECT &&
		!(addr == SDIO_CCCR_ABORT || addr == SDIO_CCCR_SUSPEND)));
}

#endif

