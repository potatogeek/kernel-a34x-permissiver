<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ASM_MICROBLAZE_SETUP_H
#define _ASM_MICROBLAZE_SETUP_H

#include <uapi/asm/setup.h>

# ifndef __ASSEMBLY__
<<<<<<< HEAD
extern unsigned int boot_cpuid; /* move to smp.h */

=======
>>>>>>> upstream/android-13
extern char cmd_line[COMMAND_LINE_SIZE];

extern char *klimit;

<<<<<<< HEAD
#   ifdef CONFIG_MMU
extern void mmu_reset(void);
#   endif /* CONFIG_MMU */
=======
extern void mmu_reset(void);
>>>>>>> upstream/android-13

void time_init(void);
void init_IRQ(void);
void machine_early_init(const char *cmdline, unsigned int ram,
		unsigned int fdt, unsigned int msr, unsigned int tlb0,
		unsigned int tlb1);

void machine_restart(char *cmd);
void machine_shutdown(void);
void machine_halt(void);
void machine_power_off(void);

extern void *zalloc_maybe_bootmem(size_t size, gfp_t mask);

# endif /* __ASSEMBLY__ */
#endif /* _ASM_MICROBLAZE_SETUP_H */
