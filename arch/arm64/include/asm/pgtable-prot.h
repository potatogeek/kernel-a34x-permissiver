<<<<<<< HEAD
/*
 * Copyright (C) 2016 ARM Ltd.
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2016 ARM Ltd.
>>>>>>> upstream/android-13
 */
#ifndef __ASM_PGTABLE_PROT_H
#define __ASM_PGTABLE_PROT_H

#include <asm/memory.h>
#include <asm/pgtable-hwdef.h>

#include <linux/const.h>

/*
 * Software defined PTE bits definition.
 */
<<<<<<< HEAD
#define PTE_VALID		(_AT(pteval_t, 1) << 0)
#define PTE_WRITE		(PTE_DBM)		 /* same as DBM (51) */
#define PTE_DIRTY		(_AT(pteval_t, 1) << 55)
#define PTE_SPECIAL		(_AT(pteval_t, 1) << 56)
#define PTE_PROT_NONE		(_AT(pteval_t, 1) << 58) /* only when !PTE_VALID */

#ifndef __ASSEMBLY__

#include <asm/pgtable-types.h>

#define _PROT_DEFAULT		(PTE_TYPE_PAGE | PTE_AF | PTE_SHARED)
#define _PROT_SECT_DEFAULT	(PMD_TYPE_SECT | PMD_SECT_AF | PMD_SECT_S)

#define PTE_MAYBE_NG		(arm64_kernel_unmapped_at_el0() ? PTE_NG : 0)
#define PMD_MAYBE_NG		(arm64_kernel_unmapped_at_el0() ? PMD_SECT_NG : 0)
=======
#define PTE_WRITE		(PTE_DBM)		 /* same as DBM (51) */
#define PTE_DIRTY		(_AT(pteval_t, 1) << 55)
#define PTE_SPECIAL		(_AT(pteval_t, 1) << 56)
#define PTE_DEVMAP		(_AT(pteval_t, 1) << 57)
#define PTE_PROT_NONE		(_AT(pteval_t, 1) << 58) /* only when !PTE_VALID */

/*
 * This bit indicates that the entry is present i.e. pmd_page()
 * still points to a valid huge page in memory even if the pmd
 * has been invalidated.
 */
#define PMD_PRESENT_INVALID	(_AT(pteval_t, 1) << 59) /* only when !PMD_SECT_VALID */

#ifndef __ASSEMBLY__

#include <asm/cpufeature.h>
#include <asm/pgtable-types.h>

extern bool arm64_use_ng_mappings;

#define _PROT_DEFAULT		(PTE_TYPE_PAGE | PTE_AF | PTE_SHARED)
#define _PROT_SECT_DEFAULT	(PMD_TYPE_SECT | PMD_SECT_AF | PMD_SECT_S)

#define PTE_MAYBE_NG		(arm64_use_ng_mappings ? PTE_NG : 0)
#define PMD_MAYBE_NG		(arm64_use_ng_mappings ? PMD_SECT_NG : 0)

/*
 * If we have userspace only BTI we don't want to mark kernel pages
 * guarded even if the system does support BTI.
 */
#ifdef CONFIG_ARM64_BTI_KERNEL
#define PTE_MAYBE_GP		(system_supports_bti() ? PTE_GP : 0)
#else
#define PTE_MAYBE_GP		0
#endif
>>>>>>> upstream/android-13

#define PROT_DEFAULT		(_PROT_DEFAULT | PTE_MAYBE_NG)
#define PROT_SECT_DEFAULT	(_PROT_SECT_DEFAULT | PMD_MAYBE_NG)

#define PROT_DEVICE_nGnRnE	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRnE))
#define PROT_DEVICE_nGnRE	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRE))
#define PROT_NORMAL_NC		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_NC))
<<<<<<< HEAD
#define PROT_NORMAL_WT		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_WT))
#define PROT_NORMAL		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL))
=======
#define PROT_NORMAL		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL))
#define PROT_NORMAL_TAGGED	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_TAGGED))
>>>>>>> upstream/android-13

#define PROT_SECT_DEVICE_nGnRE	(PROT_SECT_DEFAULT | PMD_SECT_PXN | PMD_SECT_UXN | PMD_ATTRINDX(MT_DEVICE_nGnRE))
#define PROT_SECT_NORMAL	(PROT_SECT_DEFAULT | PMD_SECT_PXN | PMD_SECT_UXN | PMD_ATTRINDX(MT_NORMAL))
#define PROT_SECT_NORMAL_EXEC	(PROT_SECT_DEFAULT | PMD_SECT_UXN | PMD_ATTRINDX(MT_NORMAL))

#define _PAGE_DEFAULT		(_PROT_DEFAULT | PTE_ATTRINDX(MT_NORMAL))
<<<<<<< HEAD
#define _HYP_PAGE_DEFAULT	_PAGE_DEFAULT
=======
>>>>>>> upstream/android-13

#define PAGE_KERNEL		__pgprot(PROT_NORMAL)
#define PAGE_KERNEL_RO		__pgprot((PROT_NORMAL & ~PTE_WRITE) | PTE_RDONLY)
#define PAGE_KERNEL_ROX		__pgprot((PROT_NORMAL & ~(PTE_WRITE | PTE_PXN)) | PTE_RDONLY)
#define PAGE_KERNEL_EXEC	__pgprot(PROT_NORMAL & ~PTE_PXN)
#define PAGE_KERNEL_EXEC_CONT	__pgprot((PROT_NORMAL & ~PTE_PXN) | PTE_CONT)

<<<<<<< HEAD
#define PAGE_HYP		__pgprot(_HYP_PAGE_DEFAULT | PTE_HYP | PTE_HYP_XN)
#define PAGE_HYP_EXEC		__pgprot(_HYP_PAGE_DEFAULT | PTE_HYP | PTE_RDONLY)
#define PAGE_HYP_RO		__pgprot(_HYP_PAGE_DEFAULT | PTE_HYP | PTE_RDONLY | PTE_HYP_XN)
#define PAGE_HYP_DEVICE		__pgprot(_PROT_DEFAULT | PTE_ATTRINDX(MT_DEVICE_nGnRE) | PTE_HYP | PTE_HYP_XN)

#define PAGE_S2_MEMATTR(attr)						\
	({								\
		u64 __val;						\
		if (cpus_have_const_cap(ARM64_HAS_STAGE2_FWB))		\
=======
#define PAGE_S2_MEMATTR(attr, has_fwb)					\
	({								\
		u64 __val;						\
		if (has_fwb)						\
>>>>>>> upstream/android-13
			__val = PTE_S2_MEMATTR(MT_S2_FWB_ ## attr);	\
		else							\
			__val = PTE_S2_MEMATTR(MT_S2_ ## attr);		\
		__val;							\
	 })

<<<<<<< HEAD
#define PAGE_S2_XN							\
	({								\
		u64 __val;						\
		if (cpus_have_const_cap(ARM64_HAS_CACHE_DIC))		\
			__val = 0;					\
		else							\
			__val = PTE_S2_XN;				\
		__val;							\
	})

#define PAGE_S2			__pgprot(_PROT_DEFAULT | PAGE_S2_MEMATTR(NORMAL) | PTE_S2_RDONLY | PAGE_S2_XN)
#define PAGE_S2_DEVICE		__pgprot(_PROT_DEFAULT | PAGE_S2_MEMATTR(DEVICE_nGnRE) | PTE_S2_RDONLY | PAGE_S2_XN)

=======
>>>>>>> upstream/android-13
#define PAGE_NONE		__pgprot(((_PAGE_DEFAULT) & ~PTE_VALID) | PTE_PROT_NONE | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_UXN)
/* shared+writable pages are clean by default, hence PTE_RDONLY|PTE_WRITE */
#define PAGE_SHARED		__pgprot(_PAGE_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_UXN | PTE_WRITE)
#define PAGE_SHARED_EXEC	__pgprot(_PAGE_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_WRITE)
#define PAGE_READONLY		__pgprot(_PAGE_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN | PTE_UXN)
#define PAGE_READONLY_EXEC	__pgprot(_PAGE_DEFAULT | PTE_USER | PTE_RDONLY | PTE_NG | PTE_PXN)
<<<<<<< HEAD
=======
#define PAGE_EXECONLY		__pgprot(_PAGE_DEFAULT | PTE_RDONLY | PTE_NG | PTE_PXN)
>>>>>>> upstream/android-13

#define __P000  PAGE_NONE
#define __P001  PAGE_READONLY
#define __P010  PAGE_READONLY
#define __P011  PAGE_READONLY
<<<<<<< HEAD
#define __P100  PAGE_READONLY_EXEC
=======
#define __P100  PAGE_READONLY_EXEC	/* PAGE_EXECONLY if Enhanced PAN */
>>>>>>> upstream/android-13
#define __P101  PAGE_READONLY_EXEC
#define __P110  PAGE_READONLY_EXEC
#define __P111  PAGE_READONLY_EXEC

#define __S000  PAGE_NONE
#define __S001  PAGE_READONLY
#define __S010  PAGE_SHARED
#define __S011  PAGE_SHARED
<<<<<<< HEAD
#define __S100  PAGE_READONLY_EXEC
=======
#define __S100  PAGE_READONLY_EXEC	/* PAGE_EXECONLY if Enhanced PAN */
>>>>>>> upstream/android-13
#define __S101  PAGE_READONLY_EXEC
#define __S110  PAGE_SHARED_EXEC
#define __S111  PAGE_SHARED_EXEC

#endif /* __ASSEMBLY__ */

#endif /* __ASM_PGTABLE_PROT_H */
