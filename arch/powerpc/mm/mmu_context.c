<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *  Common implementation of switch_mm_irqs_off
 *
 *  Copyright IBM Corp. 2017
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/mm.h>
#include <linux/cpu.h>
#include <linux/sched/mm.h>

#include <asm/mmu_context.h>
<<<<<<< HEAD
=======
#include <asm/pgalloc.h>
>>>>>>> upstream/android-13

#if defined(CONFIG_PPC32)
static inline void switch_mm_pgdir(struct task_struct *tsk,
				   struct mm_struct *mm)
{
	/* 32-bit keeps track of the current PGDIR in the thread struct */
	tsk->thread.pgdir = mm->pgd;
}
#elif defined(CONFIG_PPC_BOOK3E_64)
static inline void switch_mm_pgdir(struct task_struct *tsk,
				   struct mm_struct *mm)
{
	/* 64-bit Book3E keeps track of current PGD in the PACA */
	get_paca()->pgd = mm->pgd;
}
#else
static inline void switch_mm_pgdir(struct task_struct *tsk,
				   struct mm_struct *mm) { }
#endif

void switch_mm_irqs_off(struct mm_struct *prev, struct mm_struct *next,
			struct task_struct *tsk)
{
	bool new_on_cpu = false;

	/* Mark this context has been used on the new CPU */
	if (!cpumask_test_cpu(smp_processor_id(), mm_cpumask(next))) {
		cpumask_set_cpu(smp_processor_id(), mm_cpumask(next));
		inc_mm_active_cpus(next);

		/*
		 * This full barrier orders the store to the cpumask above vs
<<<<<<< HEAD
		 * a subsequent operation which allows this CPU to begin loading
		 * translations for next.
		 *
		 * When using the radix MMU that operation is the load of the
		 * MMU context id, which is then moved to SPRN_PID.
		 *
		 * For the hash MMU it is either the first load from slb_cache
		 * in switch_slb(), and/or the store of paca->mm_ctx_id in
		 * copy_mm_to_paca().
		 *
		 * On the other side, the barrier is in mm/tlb-radix.c for
		 * radix which orders earlier stores to clear the PTEs vs
		 * the load of mm_cpumask. And pte_xchg which does the same
		 * thing for hash.
		 *
		 * This full barrier is needed by membarrier when switching
		 * between processes after store to rq->curr, before user-space
		 * memory accesses.
=======
		 * a subsequent load which allows this CPU/MMU to begin loading
		 * translations for 'next' from page table PTEs into the TLB.
		 *
		 * When using the radix MMU, that operation is the load of the
		 * MMU context id, which is then moved to SPRN_PID.
		 *
		 * For the hash MMU it is either the first load from slb_cache
		 * in switch_slb() to preload the SLBs, or the load of
		 * get_user_context which loads the context for the VSID hash
		 * to insert a new SLB, in the SLB fault handler.
		 *
		 * On the other side, the barrier is in mm/tlb-radix.c for
		 * radix which orders earlier stores to clear the PTEs before
		 * the load of mm_cpumask to check which CPU TLBs should be
		 * flushed. For hash, pte_xchg to clear the PTE includes the
		 * barrier.
		 *
		 * This full barrier is also needed by membarrier when
		 * switching between processes after store to rq->curr, before
		 * user-space memory accesses.
>>>>>>> upstream/android-13
		 */
		smp_mb();

		new_on_cpu = true;
	}

	/* Some subarchs need to track the PGD elsewhere */
	switch_mm_pgdir(tsk, next);

	/* Nothing else to do if we aren't actually switching */
	if (prev == next)
		return;

	/*
	 * We must stop all altivec streams before changing the HW
	 * context
	 */
	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		asm volatile ("dssall");

<<<<<<< HEAD
	if (new_on_cpu)
		radix_kvm_prefetch_workaround(next);
	else
=======
	if (!new_on_cpu)
>>>>>>> upstream/android-13
		membarrier_arch_switch_mm(prev, next, tsk);

	/*
	 * The actual HW switching method differs between the various
	 * sub architectures. Out of line for now
	 */
	switch_mmu_context(prev, next, tsk);
}

<<<<<<< HEAD
=======
#ifndef CONFIG_PPC_BOOK3S_64
void arch_exit_mmap(struct mm_struct *mm)
{
	void *frag = pte_frag_get(&mm->context);

	if (frag)
		pte_frag_destroy(frag);
}
#endif
>>>>>>> upstream/android-13
