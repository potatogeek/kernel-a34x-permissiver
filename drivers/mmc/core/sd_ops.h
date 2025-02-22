<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  linux/drivers/mmc/core/sd_ops.h
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

#ifndef _MMC_SD_OPS_H
#define _MMC_SD_OPS_H

#include <linux/types.h>

struct mmc_card;
struct mmc_host;
<<<<<<< HEAD
struct mmc_command;
=======
>>>>>>> upstream/android-13

int mmc_app_set_bus_width(struct mmc_card *card, int width);
int mmc_send_app_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr);
int mmc_send_if_cond(struct mmc_host *host, u32 ocr);
<<<<<<< HEAD
=======
int mmc_send_if_cond_pcie(struct mmc_host *host, u32 ocr);
>>>>>>> upstream/android-13
int mmc_send_relative_addr(struct mmc_host *host, unsigned int *rca);
int mmc_app_send_scr(struct mmc_card *card);
int mmc_sd_switch(struct mmc_card *card, int mode, int group,
	u8 value, u8 *resp);
int mmc_app_sd_status(struct mmc_card *card, void *ssr);
int mmc_app_cmd(struct mmc_host *host, struct mmc_card *card);
<<<<<<< HEAD
int mmc_wait_for_app_cmd(struct mmc_host *host, struct mmc_card *card,
	struct mmc_command *cmd, int retries);
=======
>>>>>>> upstream/android-13

#endif

