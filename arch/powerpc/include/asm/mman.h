<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> upstream/android-13
 */
#ifndef _ASM_POWERPC_MMAN_H
#define _ASM_POWERPC_MMAN_H

#include <uapi/asm/mman.h>

#ifdef CONFIG_PPC64

#include <asm/cputable.h>
#include <linux/mm.h>
#include <linux/pkeys.h>
#include <asm/cpu_has_feature.h>

<<<<<<< HEAD
/*
 * This file is included by linux/mman.h, so we can't use cacl_vm_prot_bits()
 * here.  How important is the optimization?
 */
=======
>>>>>>> upstream/android-13
static inline unsigned long arch_calc_vm_prot_bits(unsigned long prot,
		unsigned long pkey)
{
#ifdef CONFIG_PPC_MEM_KEYS
	return (((prot & PROT_SAO) ? VM_SAO : 0) | pkey_to_vmflag_bits(pkey));
#else
	return ((prot & PROT_SAO) ? VM_SAO : 0);
#endif
}
#define arch_calc_vm_prot_bits(prot, pkey) arch_calc_vm_prot_bits(prot, pkey)

static inline pgprot_t arch_vm_get_page_prot(unsigned long vm_flags)
{
#ifdef CONFIG_PPC_MEM_KEYS
	return (vm_flags & VM_SAO) ?
		__pgprot(_PAGE_SAO | vmflag_to_pte_pkey_bits(vm_flags)) :
		__pgprot(0 | vmflag_to_pte_pkey_bits(vm_flags));
#else
	return (vm_flags & VM_SAO) ? __pgprot(_PAGE_SAO) : __pgprot(0);
#endif
}
#define arch_vm_get_page_prot(vm_flags) arch_vm_get_page_prot(vm_flags)

static inline bool arch_validate_prot(unsigned long prot, unsigned long addr)
{
	if (prot & ~(PROT_READ | PROT_WRITE | PROT_EXEC | PROT_SEM | PROT_SAO))
		return false;
<<<<<<< HEAD
	if ((prot & PROT_SAO) && !cpu_has_feature(CPU_FTR_SAO))
		return false;
=======
	if (prot & PROT_SAO) {
		if (!cpu_has_feature(CPU_FTR_SAO))
			return false;
		if (firmware_has_feature(FW_FEATURE_LPAR) &&
		    !IS_ENABLED(CONFIG_PPC_PROT_SAO_LPAR))
			return false;
	}
>>>>>>> upstream/android-13
	return true;
}
#define arch_validate_prot arch_validate_prot

#endif /* CONFIG_PPC64 */
#endif	/* _ASM_POWERPC_MMAN_H */
