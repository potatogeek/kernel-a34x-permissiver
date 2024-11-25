<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef _ASM_UPROBES_H
#define _ASM_UPROBES_H
/*
 * User-space Probes (UProbes) for powerpc
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
=======
>>>>>>> upstream/android-13
 * Copyright IBM Corporation, 2007-2012
 *
 * Adapted from the x86 port by Ananth N Mavinakayanahalli <ananth@in.ibm.com>
 */

#include <linux/notifier.h>
#include <asm/probes.h>
<<<<<<< HEAD

typedef ppc_opcode_t uprobe_opcode_t;

#define MAX_UINSN_BYTES		4
=======
#include <asm/inst.h>

typedef ppc_opcode_t uprobe_opcode_t;

#define MAX_UINSN_BYTES		8
>>>>>>> upstream/android-13
#define UPROBE_XOL_SLOT_BYTES	(MAX_UINSN_BYTES)

/* The following alias is needed for reference from arch-agnostic code */
#define UPROBE_SWBP_INSN	BREAKPOINT_INSTRUCTION
#define UPROBE_SWBP_INSN_SIZE	4 /* swbp insn size in bytes */

struct arch_uprobe {
	union {
<<<<<<< HEAD
		u32	insn;
		u32	ixol;
=======
		u32 insn[2];
		u32 ixol[2];
>>>>>>> upstream/android-13
	};
};

struct arch_uprobe_task {
	unsigned long	saved_trap_nr;
};

#endif	/* _ASM_UPROBES_H */
