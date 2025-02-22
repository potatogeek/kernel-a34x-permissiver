<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Old U-boot compatibility for AmigaOne
 *
 * Author: Gerhard Pircher (gerhard_pircher@gmx.net)
 *
 *   Based on cuboot-83xx.c
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

#include "ppcboot.h"

static bd_t bd;

static void platform_fixups(void)
{
	dt_fixup_memory(bd.bi_memstart, bd.bi_memsize);
	dt_fixup_cpu_clocks(bd.bi_intfreq, bd.bi_busfreq / 4, bd.bi_busfreq);
}

void platform_init(unsigned long r3, unsigned long r4, unsigned long r5,
                   unsigned long r6, unsigned long r7)
{
	CUBOOT_INIT();
	fdt_init(_dtb_start);
	serial_console_init();
	platform_ops.fixups = platform_fixups;
}
