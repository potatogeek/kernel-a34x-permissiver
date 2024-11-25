<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> upstream/android-13
 */

#ifndef __BCM47XX_SPROM_H
#define __BCM47XX_SPROM_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>

<<<<<<< HEAD
#ifdef CONFIG_BCM47XX_SPROM
int bcm47xx_sprom_register_fallbacks(void);
#else
=======
struct ssb_sprom;

#ifdef CONFIG_BCM47XX_SPROM
void bcm47xx_fill_sprom(struct ssb_sprom *sprom, const char *prefix,
			bool fallback);
int bcm47xx_sprom_register_fallbacks(void);
#else
static inline void bcm47xx_fill_sprom(struct ssb_sprom *sprom,
				      const char *prefix,
				      bool fallback)
{
}

>>>>>>> upstream/android-13
static inline int bcm47xx_sprom_register_fallbacks(void)
{
	return -ENOTSUPP;
};
#endif

#endif /* __BCM47XX_SPROM_H */
