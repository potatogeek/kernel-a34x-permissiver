<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  MTK SPI bus driver definitions
 *
 * Copyright (c) 2015 MediaTek Inc.
 * Author: Leilk Liu <leilk.liu@mediatek.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#ifndef ____LINUX_PLATFORM_DATA_SPI_MTK_H
#define ____LINUX_PLATFORM_DATA_SPI_MTK_H

/* Board specific platform_data */
struct mtk_chip_config {
<<<<<<< HEAD
	u32 tx_mlsb;
	u32 rx_mlsb;
	u32 sample_sel;

	//patch the parameter as customized
	u32 cs_setuptime;
	u32 cs_holdtime;
	u32 cs_idletime;
	u32 deassert_mode;
=======
	u32 sample_sel;
>>>>>>> upstream/android-13
	u32 tick_delay;
};
#endif
