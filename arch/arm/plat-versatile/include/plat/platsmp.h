<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 *  linux/arch/arm/plat-versatile/include/plat/platsmp.h
 *
 *  Copyright (C) 2011 ARM Ltd.
 *  All Rights Reserved
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
 */
extern volatile int versatile_cpu_release;
>>>>>>> upstream/android-13

extern void versatile_secondary_startup(void);
extern void versatile_secondary_init(unsigned int cpu);
extern int  versatile_boot_secondary(unsigned int cpu, struct task_struct *idle);
<<<<<<< HEAD
=======
void versatile_immitation_cpu_die(unsigned int cpu, unsigned int actrl_mask);
>>>>>>> upstream/android-13
