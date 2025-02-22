<<<<<<< HEAD
/*
 * Copyright (C) 2015 Regents of the University of California
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
 * Copyright (C) 2015 Regents of the University of California
>>>>>>> upstream/android-13
 */

#ifndef _ASM_RISCV_CACHEFLUSH_H
#define _ASM_RISCV_CACHEFLUSH_H

<<<<<<< HEAD
#include <asm-generic/cacheflush.h>

#undef flush_icache_range
#undef flush_icache_user_range
#undef flush_dcache_page
=======
#include <linux/mm.h>
>>>>>>> upstream/android-13

static inline void local_flush_icache_all(void)
{
	asm volatile ("fence.i" ::: "memory");
}

#define PG_dcache_clean PG_arch_1

static inline void flush_dcache_page(struct page *page)
{
	if (test_bit(PG_dcache_clean, &page->flags))
		clear_bit(PG_dcache_clean, &page->flags);
}
<<<<<<< HEAD
=======
#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
>>>>>>> upstream/android-13

/*
 * RISC-V doesn't have an instruction to flush parts of the instruction cache,
 * so instead we just flush the whole thing.
 */
#define flush_icache_range(start, end) flush_icache_all()
<<<<<<< HEAD
#define flush_icache_user_range(vma, pg, addr, len) flush_icache_all()
=======
#define flush_icache_user_page(vma, pg, addr, len) \
	flush_icache_mm(vma->vm_mm, 0)
>>>>>>> upstream/android-13

#ifndef CONFIG_SMP

#define flush_icache_all() local_flush_icache_all()
#define flush_icache_mm(mm, local) flush_icache_all()

#else /* CONFIG_SMP */

<<<<<<< HEAD
#define flush_icache_all() sbi_remote_fence_i(NULL)
=======
void flush_icache_all(void);
>>>>>>> upstream/android-13
void flush_icache_mm(struct mm_struct *mm, bool local);

#endif /* CONFIG_SMP */

/*
 * Bits in sys_riscv_flush_icache()'s flags argument.
 */
#define SYS_RISCV_FLUSH_ICACHE_LOCAL 1UL
#define SYS_RISCV_FLUSH_ICACHE_ALL   (SYS_RISCV_FLUSH_ICACHE_LOCAL)

<<<<<<< HEAD
=======
#include <asm-generic/cacheflush.h>

>>>>>>> upstream/android-13
#endif /* _ASM_RISCV_CACHEFLUSH_H */
