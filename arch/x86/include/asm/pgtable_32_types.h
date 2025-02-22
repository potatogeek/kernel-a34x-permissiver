/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
#ifndef _ASM_X86_PGTABLE_32_DEFS_H
#define _ASM_X86_PGTABLE_32_DEFS_H
=======
#ifndef _ASM_X86_PGTABLE_32_TYPES_H
#define _ASM_X86_PGTABLE_32_TYPES_H
>>>>>>> upstream/android-13

/*
 * The Linux x86 paging architecture is 'compile-time dual-mode', it
 * implements both the traditional 2-level x86 page tables and the
 * newer 3-level PAE-mode page tables.
 */
#ifdef CONFIG_X86_PAE
# include <asm/pgtable-3level_types.h>
# define PMD_SIZE	(1UL << PMD_SHIFT)
# define PMD_MASK	(~(PMD_SIZE - 1))
#else
# include <asm/pgtable-2level_types.h>
#endif

#define pgtable_l5_enabled() 0

#define PGDIR_SIZE	(1UL << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE - 1))

<<<<<<< HEAD
/* Just any arbitrary offset to the start of the vmalloc VM area: the
 * current 8MB value just means that there will be a 8MB "hole" after the
 * physical memory until the kernel virtual memory starts.  That means that
 * any out-of-bounds memory accesses will hopefully be caught.
 * The vmalloc() routines leaves a hole of 4kB between each vmalloced
 * area for the same reason. ;)
 */
#define VMALLOC_OFFSET	(8 * 1024 * 1024)

#ifndef __ASSEMBLY__
extern bool __vmalloc_start_set; /* set once high_memory is set */
#endif

#define VMALLOC_START	((unsigned long)high_memory + VMALLOC_OFFSET)
#ifdef CONFIG_X86_PAE
#define LAST_PKMAP 512
#else
#define LAST_PKMAP 1024
#endif

/*
 * Define this here and validate with BUILD_BUG_ON() in pgtable_32.c
 * to avoid include recursion hell
 */
#define CPU_ENTRY_AREA_PAGES	(NR_CPUS * 40)

#define CPU_ENTRY_AREA_BASE						\
	((FIXADDR_TOT_START - PAGE_SIZE * (CPU_ENTRY_AREA_PAGES + 1))   \
	 & PMD_MASK)

#define LDT_BASE_ADDR		\
	((CPU_ENTRY_AREA_BASE - PAGE_SIZE) & PMD_MASK)

#define LDT_END_ADDR		(LDT_BASE_ADDR + PMD_SIZE)

#define PKMAP_BASE		\
	((LDT_BASE_ADDR - PAGE_SIZE) & PMD_MASK)

#ifdef CONFIG_HIGHMEM
# define VMALLOC_END	(PKMAP_BASE - 2 * PAGE_SIZE)
#else
# define VMALLOC_END	(LDT_BASE_ADDR - 2 * PAGE_SIZE)
#endif

#define MODULES_VADDR	VMALLOC_START
#define MODULES_END	VMALLOC_END
#define MODULES_LEN	(MODULES_VADDR - MODULES_END)

#define MAXMEM	(VMALLOC_END - PAGE_OFFSET - __VMALLOC_RESERVE)

#endif /* _ASM_X86_PGTABLE_32_DEFS_H */
=======
#endif /* _ASM_X86_PGTABLE_32_TYPES_H */
>>>>>>> upstream/android-13
