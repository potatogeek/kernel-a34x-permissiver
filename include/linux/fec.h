<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/* include/linux/fec.h
 *
 * Copyright (c) 2009 Orex Computed Radiography
 *   Baruch Siach <baruch@tkos.co.il>
 *
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * Header file for the FEC platform data
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __LINUX_FEC_H__
#define __LINUX_FEC_H__

#include <linux/phy.h>

struct fec_platform_data {
	phy_interface_t phy;
	unsigned char mac[ETH_ALEN];
	void (*sleep_mode_enable)(int enabled);
};

#endif
