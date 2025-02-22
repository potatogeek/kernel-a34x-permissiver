<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Old U-boot compatibility for Taishan
 *
 * Author: Hugh Blemings <hugh@au.ibm.com>
 *
 * Copyright 2007 Hugh Blemings, IBM Corporation.
 *   Based on cuboot-ebony.c which is:
 * Copyright 2007 David Gibson, IBM Corporation.
 *   Based on cuboot-83xx.c, which is:
 * Copyright (c) 2007 Freescale Semiconductor, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include "ops.h"
#include "stdio.h"
#include "cuboot.h"
#include "reg.h"
#include "dcr.h"
#include "4xx.h"

#define TARGET_4xx
#define TARGET_44x
#define TARGET_440GX
#include "ppcboot.h"

static bd_t bd;

BSS_STACK(4096);

static void taishan_fixups(void)
{
	/* FIXME: sysclk should be derived by reading the FPGA
	   registers */
	unsigned long sysclk = 33000000;

	ibm440gx_fixup_clocks(sysclk, 6 * 1843200, 25000000);

	ibm4xx_sdram_fixup_memsize();

	dt_fixup_mac_address_by_alias("ethernet0", bd.bi_enetaddr);
	dt_fixup_mac_address_by_alias("ethernet1", bd.bi_enet1addr);

	ibm4xx_fixup_ebc_ranges("/plb/opb/ebc");
}

void platform_init(unsigned long r3, unsigned long r4, unsigned long r5,
		   unsigned long r6, unsigned long r7)
{
	CUBOOT_INIT();

	platform_ops.fixups = taishan_fixups;
	fdt_init(_dtb_start);
	serial_console_init();
}
