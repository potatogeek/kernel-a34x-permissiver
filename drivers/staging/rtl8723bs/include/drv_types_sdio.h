/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __DRV_TYPES_SDIO_H__
#define __DRV_TYPES_SDIO_H__

/*  SDIO Header Files */
	#include <linux/mmc/sdio_func.h>
	#include <linux/mmc/sdio_ids.h>

<<<<<<< HEAD
#if defined(CONFIG_WOWLAN) || defined(CONFIG_AP_WOWLAN)
	#include <linux/mmc/host.h>
	#include <linux/mmc/card.h>
#endif

typedef struct sdio_data
{
=======
struct sdio_data {
>>>>>>> upstream/android-13
	u8  func_number;

	u8  tx_block_mode;
	u8  rx_block_mode;
	u32 block_transfer_len;

	struct sdio_func	 *func;
	void *sys_sdio_irq_thd;
<<<<<<< HEAD
} SDIO_DATA, *PSDIO_DATA;
=======
};
>>>>>>> upstream/android-13

#endif
