<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Hardware parameter area specific to Sharp SL series devices
 *
 * Copyright (c) 2005 Richard Purdie
 *
 * Based on Sharp's 2.4 kernel patches
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <asm/mach/sharpsl_param.h>
#include <asm/memory.h>

/*
 * Certain hardware parameters determined at the time of device manufacture,
 * typically including LCD parameters are loaded by the bootloader at the
 * address PARAM_BASE. As the kernel will overwrite them, we need to store
 * them early in the boot process, then pass them to the appropriate drivers.
 * Not all devices use all parameters but the format is common to all.
 */
#ifdef CONFIG_ARCH_SA1100
#define PARAM_BASE	0xe8ffc000
#define param_start(x)	(void *)(x)
#else
#define PARAM_BASE	0xa0000a00
#define param_start(x)	__va(x)
#endif
#define MAGIC_CHG(a,b,c,d) ( ( d << 24 ) | ( c << 16 )  | ( b << 8 ) | a )

#define COMADJ_MAGIC	MAGIC_CHG('C','M','A','D')
#define UUID_MAGIC	MAGIC_CHG('U','U','I','D')
#define TOUCH_MAGIC	MAGIC_CHG('T','U','C','H')
#define AD_MAGIC	MAGIC_CHG('B','V','A','D')
#define PHAD_MAGIC	MAGIC_CHG('P','H','A','D')

struct sharpsl_param_info sharpsl_param;
EXPORT_SYMBOL(sharpsl_param);

void sharpsl_save_param(void)
{
<<<<<<< HEAD
	memcpy(&sharpsl_param, param_start(PARAM_BASE), sizeof(struct sharpsl_param_info));
=======
	struct sharpsl_param_info *params = param_start(PARAM_BASE);

	memcpy(&sharpsl_param, params, sizeof(*params));
>>>>>>> upstream/android-13

	if (sharpsl_param.comadj_keyword != COMADJ_MAGIC)
		sharpsl_param.comadj=-1;

	if (sharpsl_param.phad_keyword != PHAD_MAGIC)
		sharpsl_param.phadadj=-1;

	if (sharpsl_param.uuid_keyword != UUID_MAGIC)
		sharpsl_param.uuid[0]=-1;

	if (sharpsl_param.touch_keyword != TOUCH_MAGIC)
		sharpsl_param.touch_xp=-1;

	if (sharpsl_param.adadj_keyword != AD_MAGIC)
		sharpsl_param.adadj=-1;
}


