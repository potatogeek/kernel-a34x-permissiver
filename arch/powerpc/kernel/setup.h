<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Prototypes for functions that are shared between setup_(32|64|common).c
 *
 * Copyright 2016 Michael Ellerman, IBM Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef __ARCH_POWERPC_KERNEL_SETUP_H
#define __ARCH_POWERPC_KERNEL_SETUP_H

void initialize_cache_info(void);
void irqstack_early_init(void);

#ifdef CONFIG_PPC32
void setup_power_save(void);
#else
<<<<<<< HEAD
static inline void setup_power_save(void) { };
=======
static inline void setup_power_save(void) { }
>>>>>>> upstream/android-13
#endif

#if defined(CONFIG_PPC64) && defined(CONFIG_SMP)
void check_smt_enabled(void);
#else
<<<<<<< HEAD
static inline void check_smt_enabled(void) { };
=======
static inline void check_smt_enabled(void) { }
>>>>>>> upstream/android-13
#endif

#if defined(CONFIG_PPC_BOOK3E) && defined(CONFIG_SMP)
void setup_tlb_core_data(void);
#else
<<<<<<< HEAD
static inline void setup_tlb_core_data(void) { };
=======
static inline void setup_tlb_core_data(void) { }
>>>>>>> upstream/android-13
#endif

#if defined(CONFIG_PPC_BOOK3E) || defined(CONFIG_BOOKE) || defined(CONFIG_40x)
void exc_lvl_early_init(void);
#else
<<<<<<< HEAD
static inline void exc_lvl_early_init(void) { };
#endif

#ifdef CONFIG_PPC64
void emergency_stack_init(void);
#else
static inline void emergency_stack_init(void) { };
=======
static inline void exc_lvl_early_init(void) { }
#endif

#if defined(CONFIG_PPC64) || defined(CONFIG_VMAP_STACK)
void emergency_stack_init(void);
#else
static inline void emergency_stack_init(void) { }
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_PPC64
u64 ppc64_bolted_size(void);

/* Default SPR values from firmware/kexec */
extern unsigned long spr_default_dscr;
#endif

/*
 * Having this in kvm_ppc.h makes include dependencies too
 * tricky to solve for setup-common.c so have it here.
 */
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
void kvm_cma_reserve(void);
#else
<<<<<<< HEAD
static inline void kvm_cma_reserve(void) { };
=======
static inline void kvm_cma_reserve(void) { }
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_TAU
u32 cpu_temp(unsigned long cpu);
u32 cpu_temp_both(unsigned long cpu);
u32 tau_interrupts(unsigned long cpu);
#endif /* CONFIG_TAU */

#endif /* __ARCH_POWERPC_KERNEL_SETUP_H */
