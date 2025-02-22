/*
 * arch/xtensa/kernel/platform.c
 *
 * Default platform functions.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 */

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/time.h>
#include <asm/platform.h>
#include <asm/timex.h>
#include <asm/param.h>		/* HZ */
=======
#include <linux/printk.h>
#include <linux/types.h>
#include <asm/platform.h>
#include <asm/timex.h>
>>>>>>> upstream/android-13

#define _F(r,f,a,b)							\
	r __platform_##f a b;                                   	\
	r platform_##f a __attribute__((weak, alias("__platform_"#f)))

/*
 * Default functions that are used if no platform specific function is defined.
 * (Please, refer to include/asm-xtensa/platform.h for more information)
 */

<<<<<<< HEAD
=======
_F(void, init, (bp_tag_t *first), { });
>>>>>>> upstream/android-13
_F(void, setup, (char** cmd), { });
_F(void, restart, (void), { while(1); });
_F(void, halt, (void), { while(1); });
_F(void, power_off, (void), { while(1); });
_F(void, idle, (void), { __asm__ __volatile__ ("waiti 0" ::: "memory"); });
_F(void, heartbeat, (void), { });
<<<<<<< HEAD
_F(int,  pcibios_fixup, (void), { return 0; });
_F(void, pcibios_init, (void), { });
=======
>>>>>>> upstream/android-13

#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
_F(void, calibrate_ccount, (void),
{
	pr_err("ERROR: Cannot calibrate cpu frequency! Assuming 10MHz.\n");
	ccount_freq = 10 * 1000000UL;
});
#endif
