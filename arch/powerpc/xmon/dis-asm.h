<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef _POWERPC_XMON_DIS_ASM_H
#define _POWERPC_XMON_DIS_ASM_H
/*
 * Copyright (C) 2006 Michael Ellerman, IBM Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

extern void print_address (unsigned long memaddr);

#ifdef CONFIG_XMON_DISASSEMBLY
extern int print_insn_powerpc(unsigned long insn, unsigned long memaddr);
extern int print_insn_spu(unsigned long insn, unsigned long memaddr);
#else
static inline int print_insn_powerpc(unsigned long insn, unsigned long memaddr)
{
<<<<<<< HEAD
	printf("%.8x", insn);
=======
	printf("%.8lx", insn);
>>>>>>> upstream/android-13
	return 0;
}

static inline int print_insn_spu(unsigned long insn, unsigned long memaddr)
{
<<<<<<< HEAD
	printf("%.8x", insn);
=======
	printf("%.8lx", insn);
>>>>>>> upstream/android-13
	return 0;
}
#endif

#endif /* _POWERPC_XMON_DIS_ASM_H */
