<<<<<<< HEAD
/*
 *  Copyright (C) 2005 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 *  Copyright (C) 2005 Russell King.
>>>>>>> upstream/android-13
 */
#ifndef __MFD_MCP_SA11X0_H
#define __MFD_MCP_SA11X0_H

#include <linux/types.h>

struct mcp_plat_data {
	u32 mccr0;
	u32 mccr1;
	unsigned int sclk_rate;
	void *codec_pdata;
};

#endif
