<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
>>>>>>> upstream/android-13
 */


#ifndef _ASM_RISCV_MMU_H
#define _ASM_RISCV_MMU_H

#ifndef __ASSEMBLY__

typedef struct {
<<<<<<< HEAD
=======
#ifndef CONFIG_MMU
	unsigned long	end_brk;
#else
	atomic_long_t id;
#endif
>>>>>>> upstream/android-13
	void *vdso;
#ifdef CONFIG_SMP
	/* A local icache flush is needed before user execution can resume. */
	cpumask_t icache_stale_mask;
#endif
} mm_context_t;

<<<<<<< HEAD
=======
void __init create_pgd_mapping(pgd_t *pgdp, uintptr_t va, phys_addr_t pa,
			       phys_addr_t sz, pgprot_t prot);
>>>>>>> upstream/android-13
#endif /* __ASSEMBLY__ */

#endif /* _ASM_RISCV_MMU_H */
