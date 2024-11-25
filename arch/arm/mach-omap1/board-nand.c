<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * linux/arch/arm/mach-omap1/board-nand.c
 *
 * Common OMAP1 board NAND code
 *
 * Copyright (C) 2004, 2012 Texas Instruments, Inc.
 * Copyright (C) 2002 MontaVista Software, Inc.
 * Copyright (C) 2001 RidgeRun, Inc.
 * Author: RidgeRun, Inc.
 *         Greg Lonnon (glonnon@ridgerun.com) or info@ridgerun.com
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>

#include "common.h"

<<<<<<< HEAD
void omap1_nand_cmd_ctl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd_to_nand(mtd);
=======
void omap1_nand_cmd_ctl(struct nand_chip *this, int cmd, unsigned int ctrl)
{
>>>>>>> upstream/android-13
	unsigned long mask;

	if (cmd == NAND_CMD_NONE)
		return;

	mask = (ctrl & NAND_CLE) ? 0x02 : 0;
	if (ctrl & NAND_ALE)
		mask |= 0x04;

<<<<<<< HEAD
	writeb(cmd, this->IO_ADDR_W + mask);
=======
	writeb(cmd, this->legacy.IO_ADDR_W + mask);
>>>>>>> upstream/android-13
}

