<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/include/asm/system_misc.h
 *
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */
#ifndef __ASM_SYSTEM_MISC_H
#define __ASM_SYSTEM_MISC_H

#ifndef __ASSEMBLY__

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/irqflags.h>
#include <linux/signal.h>
#include <linux/ratelimit.h>
#include <linux/reboot.h>

struct pt_regs;

void die(const char *msg, struct pt_regs *regs, int err);

struct siginfo;
void arm64_notify_die(const char *str, struct pt_regs *regs,
<<<<<<< HEAD
		      struct siginfo *info, int err);
=======
		      int signo, int sicode, unsigned long far,
		      int err);
>>>>>>> upstream/android-13

void hook_debug_fault_code(int nr, int (*fn)(unsigned long, unsigned int,
					     struct pt_regs *),
			   int sig, int code, const char *name);

struct mm_struct;
<<<<<<< HEAD
extern void show_pte(unsigned long addr);
extern void __show_regs(struct pt_regs *);

extern void (*arm_pm_restart)(enum reboot_mode reboot_mode, const char *cmd);
extern char* (*arch_read_hardware_id)(void);

int handle_guest_sea(phys_addr_t addr, unsigned int esr);

=======
extern void __show_regs(struct pt_regs *);

>>>>>>> upstream/android-13
#endif	/* __ASSEMBLY__ */

#endif	/* __ASM_SYSTEM_MISC_H */
