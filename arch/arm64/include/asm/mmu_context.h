<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/include/asm/mmu_context.h
 *
 * Copyright (C) 1996 Russell King.
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
#ifndef __ASM_MMU_CONTEXT_H
#define __ASM_MMU_CONTEXT_H

#ifndef __ASSEMBLY__

#include <linux/compiler.h>
#include <linux/sched.h>
#include <linux/sched/hotplug.h>
#include <linux/mm_types.h>
<<<<<<< HEAD
=======
#include <linux/pgtable.h>
>>>>>>> upstream/android-13

#include <asm/cacheflush.h>
#include <asm/cpufeature.h>
#include <asm/proc-fns.h>
#include <asm-generic/mm_hooks.h>
#include <asm/cputype.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include <asm/sysreg.h>
#include <asm/tlbflush.h>

=======
#include <asm/sysreg.h>
#include <asm/tlbflush.h>

extern bool rodata_full;

>>>>>>> upstream/android-13
static inline void contextidr_thread_switch(struct task_struct *next)
{
	if (!IS_ENABLED(CONFIG_PID_IN_CONTEXTIDR))
		return;

	write_sysreg(task_pid_nr(next), contextidr_el1);
	isb();
}

/*
<<<<<<< HEAD
 * Set TTBR0 to empty_zero_page. No translations will be possible via TTBR0.
 */
static inline void cpu_set_reserved_ttbr0(void)
{
	unsigned long ttbr = phys_to_ttbr(__pa_symbol(empty_zero_page));
=======
 * Set TTBR0 to reserved_pg_dir. No translations will be possible via TTBR0.
 */
static inline void cpu_set_reserved_ttbr0(void)
{
	unsigned long ttbr = phys_to_ttbr(__pa_symbol(reserved_pg_dir));
>>>>>>> upstream/android-13

	write_sysreg(ttbr, ttbr0_el1);
	isb();
}

<<<<<<< HEAD
=======
void cpu_do_switch_mm(phys_addr_t pgd_phys, struct mm_struct *mm);

>>>>>>> upstream/android-13
static inline void cpu_switch_mm(pgd_t *pgd, struct mm_struct *mm)
{
	BUG_ON(pgd == swapper_pg_dir);
	cpu_set_reserved_ttbr0();
	cpu_do_switch_mm(virt_to_phys(pgd),mm);
}

/*
 * TCR.T0SZ value to use when the ID map is active. Usually equals
 * TCR_T0SZ(VA_BITS), unless system RAM is positioned very high in
 * physical memory, in which case it will be smaller.
 */
extern u64 idmap_t0sz;
extern u64 idmap_ptrs_per_pgd;

<<<<<<< HEAD
static inline bool __cpu_uses_extended_idmap(void)
{
	return unlikely(idmap_t0sz != TCR_T0SZ(VA_BITS));
}

/*
 * True if the extended ID map requires an extra level of translation table
 * to be configured.
 */
static inline bool __cpu_uses_extended_idmap_level(void)
{
	return ARM64_HW_PGTABLE_LEVELS(64 - idmap_t0sz) > CONFIG_PGTABLE_LEVELS;
}

/*
 * Set TCR.T0SZ to its default value (based on VA_BITS)
 */
static inline void __cpu_set_tcr_t0sz(unsigned long t0sz)
{
	unsigned long tcr;

	if (!__cpu_uses_extended_idmap())
		return;

	tcr = read_sysreg(tcr_el1);
=======
/*
 * Ensure TCR.T0SZ is set to the provided value.
 */
static inline void __cpu_set_tcr_t0sz(unsigned long t0sz)
{
	unsigned long tcr = read_sysreg(tcr_el1);

	if ((tcr & TCR_T0SZ_MASK) >> TCR_T0SZ_OFFSET == t0sz)
		return;

>>>>>>> upstream/android-13
	tcr &= ~TCR_T0SZ_MASK;
	tcr |= t0sz << TCR_T0SZ_OFFSET;
	write_sysreg(tcr, tcr_el1);
	isb();
}

<<<<<<< HEAD
#define cpu_set_default_tcr_t0sz()	__cpu_set_tcr_t0sz(TCR_T0SZ(VA_BITS))
=======
#define cpu_set_default_tcr_t0sz()	__cpu_set_tcr_t0sz(TCR_T0SZ(vabits_actual))
>>>>>>> upstream/android-13
#define cpu_set_idmap_tcr_t0sz()	__cpu_set_tcr_t0sz(idmap_t0sz)

/*
 * Remove the idmap from TTBR0_EL1 and install the pgd of the active mm.
 *
 * The idmap lives in the same VA range as userspace, but uses global entries
 * and may use a different TCR_EL1.T0SZ. To avoid issues resulting from
 * speculative TLB fetches, we must temporarily install the reserved page
 * tables while we invalidate the TLBs and set up the correct TCR_EL1.T0SZ.
 *
 * If current is a not a user task, the mm covers the TTBR1_EL1 page tables,
 * which should not be installed in TTBR0_EL1. In this case we can leave the
 * reserved page tables in place.
 */
static inline void cpu_uninstall_idmap(void)
{
	struct mm_struct *mm = current->active_mm;

	cpu_set_reserved_ttbr0();
	local_flush_tlb_all();
	cpu_set_default_tcr_t0sz();

	if (mm != &init_mm && !system_uses_ttbr0_pan())
		cpu_switch_mm(mm->pgd, mm);
}

static inline void cpu_install_idmap(void)
{
	cpu_set_reserved_ttbr0();
	local_flush_tlb_all();
	cpu_set_idmap_tcr_t0sz();

	cpu_switch_mm(lm_alias(idmap_pg_dir), &init_mm);
}

/*
 * Atomically replaces the active TTBR1_EL1 PGD with a new VA-compatible PGD,
 * avoiding the possibility of conflicting TLB entries being allocated.
 */
static inline void __nocfi cpu_replace_ttbr1(pgd_t *pgdp)
{
	typedef void (ttbr_replace_func)(phys_addr_t);
	extern ttbr_replace_func idmap_cpu_replace_ttbr1;
	ttbr_replace_func *replace_phys;

<<<<<<< HEAD
	phys_addr_t pgd_phys = virt_to_phys(pgdp);

	replace_phys = (void *)__pa_function(idmap_cpu_replace_ttbr1);

	cpu_install_idmap();
	replace_phys(pgd_phys);
=======
	/* phys_to_ttbr() zeros lower 2 bits of ttbr with 52-bit PA */
	phys_addr_t ttbr1 = phys_to_ttbr(virt_to_phys(pgdp));

	if (system_supports_cnp() && !WARN_ON(pgdp != lm_alias(swapper_pg_dir))) {
		/*
		 * cpu_replace_ttbr1() is used when there's a boot CPU
		 * up (i.e. cpufeature framework is not up yet) and
		 * latter only when we enable CNP via cpufeature's
		 * enable() callback.
		 * Also we rely on the cpu_hwcap bit being set before
		 * calling the enable() function.
		 */
		ttbr1 |= TTBR_CNP_BIT;
	}

	replace_phys = (void *)__pa_symbol(function_nocfi(idmap_cpu_replace_ttbr1));

	cpu_install_idmap();
	replace_phys(ttbr1);
>>>>>>> upstream/android-13
	cpu_uninstall_idmap();
}

/*
 * It would be nice to return ASIDs back to the allocator, but unfortunately
 * that introduces a race with a generation rollover where we could erroneously
 * free an ASID allocated in a future generation. We could workaround this by
 * freeing the ASID from the context of the dying mm (e.g. in arch_exit_mmap),
 * but we'd then need to make sure that we didn't dirty any TLBs afterwards.
 * Setting a reserved TTBR0 or EPD0 would work, but it all gets ugly when you
 * take CPU migration into account.
 */
<<<<<<< HEAD
#define destroy_context(mm)		do { } while(0)
void check_and_switch_context(struct mm_struct *mm, unsigned int cpu);

#define init_new_context(tsk,mm)	({ atomic64_set(&(mm)->context.id, 0); 0; })
=======
void check_and_switch_context(struct mm_struct *mm);

#define init_new_context(tsk, mm) init_new_context(tsk, mm)
static inline int
init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	atomic64_set(&mm->context.id, 0);
	refcount_set(&mm->context.pinned, 0);
	return 0;
}
>>>>>>> upstream/android-13

#ifdef CONFIG_ARM64_SW_TTBR0_PAN
static inline void update_saved_ttbr0(struct task_struct *tsk,
				      struct mm_struct *mm)
{
	u64 ttbr;

	if (!system_uses_ttbr0_pan())
		return;

	if (mm == &init_mm)
<<<<<<< HEAD
		ttbr = __pa_symbol(empty_zero_page);
	else
		ttbr = virt_to_phys(mm->pgd) | ASID(mm) << 48;
=======
		ttbr = phys_to_ttbr(__pa_symbol(reserved_pg_dir));
	else
		ttbr = phys_to_ttbr(virt_to_phys(mm->pgd)) | ASID(mm) << 48;
>>>>>>> upstream/android-13

	WRITE_ONCE(task_thread_info(tsk)->ttbr0, ttbr);
}
#else
static inline void update_saved_ttbr0(struct task_struct *tsk,
				      struct mm_struct *mm)
{
}
#endif

<<<<<<< HEAD
=======
#define enter_lazy_tlb enter_lazy_tlb
>>>>>>> upstream/android-13
static inline void
enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
	/*
	 * We don't actually care about the ttbr0 mapping, so point it at the
	 * zero page.
	 */
	update_saved_ttbr0(tsk, &init_mm);
}

static inline void __switch_mm(struct mm_struct *next)
{
<<<<<<< HEAD
	unsigned int cpu = smp_processor_id();

=======
>>>>>>> upstream/android-13
	/*
	 * init_mm.pgd does not contain any user mappings and it is always
	 * active for kernel addresses in TTBR1. Just set the reserved TTBR0.
	 */
	if (next == &init_mm) {
		cpu_set_reserved_ttbr0();
		return;
	}

<<<<<<< HEAD
	check_and_switch_context(next, cpu);
=======
	check_and_switch_context(next);
>>>>>>> upstream/android-13
}

static inline void
switch_mm(struct mm_struct *prev, struct mm_struct *next,
	  struct task_struct *tsk)
{
	if (prev != next)
		__switch_mm(next);

	/*
	 * Update the saved TTBR0_EL1 of the scheduled-in task as the previous
	 * value may have not been initialised yet (activate_mm caller) or the
	 * ASID has changed since the last run (following the context switch
	 * of another thread of the same process).
	 */
	update_saved_ttbr0(tsk, next);
}

<<<<<<< HEAD
#define deactivate_mm(tsk,mm)	do { } while (0)
#define activate_mm(prev,next)	switch_mm(prev, next, current)
=======
static inline const struct cpumask *
task_cpu_possible_mask(struct task_struct *p)
{
	if (!static_branch_unlikely(&arm64_mismatched_32bit_el0))
		return cpu_possible_mask;

	if (!is_compat_thread(task_thread_info(p)))
		return cpu_possible_mask;

	return system_32bit_el0_cpumask();
}
#define task_cpu_possible_mask	task_cpu_possible_mask
>>>>>>> upstream/android-13

void verify_cpu_asid_bits(void);
void post_ttbr_update_workaround(void);

<<<<<<< HEAD
=======
unsigned long arm64_mm_context_get(struct mm_struct *mm);
void arm64_mm_context_put(struct mm_struct *mm);

#include <asm-generic/mmu_context.h>

>>>>>>> upstream/android-13
#endif /* !__ASSEMBLY__ */

#endif /* !__ASM_MMU_CONTEXT_H */
