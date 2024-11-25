/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *
 *  Derived from "include/asm-i386/mmu_context.h"
 */

#ifndef __S390_MMU_CONTEXT_H
#define __S390_MMU_CONTEXT_H

#include <asm/pgalloc.h>
#include <linux/uaccess.h>
#include <linux/mm_types.h>
#include <asm/tlbflush.h>
#include <asm/ctl_reg.h>
#include <asm-generic/mm_hooks.h>

<<<<<<< HEAD
static inline int init_new_context(struct task_struct *tsk,
				   struct mm_struct *mm)
{
=======
#define init_new_context init_new_context
static inline int init_new_context(struct task_struct *tsk,
				   struct mm_struct *mm)
{
	unsigned long asce_type, init_entry;

>>>>>>> upstream/android-13
	spin_lock_init(&mm->context.lock);
	INIT_LIST_HEAD(&mm->context.pgtable_list);
	INIT_LIST_HEAD(&mm->context.gmap_list);
	cpumask_clear(&mm->context.cpu_attach_mask);
	atomic_set(&mm->context.flush_count, 0);
<<<<<<< HEAD
	mm->context.gmap_asce = 0;
	mm->context.flush_mm = 0;
	mm->context.compat_mm = test_thread_flag(TIF_31BIT);
=======
	atomic_set(&mm->context.is_protected, 0);
	mm->context.gmap_asce = 0;
	mm->context.flush_mm = 0;
>>>>>>> upstream/android-13
#ifdef CONFIG_PGSTE
	mm->context.alloc_pgste = page_table_allocate_pgste ||
		test_thread_flag(TIF_PGSTE) ||
		(current->mm && current->mm->context.alloc_pgste);
	mm->context.has_pgste = 0;
	mm->context.uses_skeys = 0;
	mm->context.uses_cmm = 0;
	mm->context.allow_gmap_hpage_1m = 0;
#endif
	switch (mm->context.asce_limit) {
<<<<<<< HEAD
	case _REGION2_SIZE:
		/*
		 * forked 3-level task, fall through to set new asce with new
		 * mm->pgd
		 */
	case 0:
		/* context created by exec, set asce limit to 4TB */
		mm->context.asce_limit = STACK_TOP_MAX;
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
				   _ASCE_USER_BITS | _ASCE_TYPE_REGION3;
		break;
	case -PAGE_SIZE:
		/* forked 5-level task, set new asce with new_mm->pgd */
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
			_ASCE_USER_BITS | _ASCE_TYPE_REGION1;
		break;
	case _REGION1_SIZE:
		/* forked 4-level task, set new asce with new mm->pgd */
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
				   _ASCE_USER_BITS | _ASCE_TYPE_REGION2;
		break;
	case _REGION3_SIZE:
		/* forked 2-level compat task, set new asce with new mm->pgd */
		mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
				   _ASCE_USER_BITS | _ASCE_TYPE_SEGMENT;
	}
	crst_table_init((unsigned long *) mm->pgd, pgd_entry_type(mm));
	return 0;
}

#define destroy_context(mm)             do { } while (0)

static inline void set_user_asce(struct mm_struct *mm)
{
	S390_lowcore.user_asce = mm->context.asce;
	__ctl_load(S390_lowcore.user_asce, 1, 1);
	clear_cpu_flag(CIF_ASCE_PRIMARY);
}

static inline void clear_user_asce(void)
{
	S390_lowcore.user_asce = S390_lowcore.kernel_asce;
	__ctl_load(S390_lowcore.kernel_asce, 1, 1);
	set_cpu_flag(CIF_ASCE_PRIMARY);
}

mm_segment_t enable_sacf_uaccess(void);
void disable_sacf_uaccess(mm_segment_t old_fs);
=======
	default:
		/*
		 * context created by exec, the value of asce_limit can
		 * only be zero in this case
		 */
		VM_BUG_ON(mm->context.asce_limit);
		/* continue as 3-level task */
		mm->context.asce_limit = _REGION2_SIZE;
		fallthrough;
	case _REGION2_SIZE:
		/* forked 3-level task */
		init_entry = _REGION3_ENTRY_EMPTY;
		asce_type = _ASCE_TYPE_REGION3;
		break;
	case TASK_SIZE_MAX:
		/* forked 5-level task */
		init_entry = _REGION1_ENTRY_EMPTY;
		asce_type = _ASCE_TYPE_REGION1;
		break;
	case _REGION1_SIZE:
		/* forked 4-level task */
		init_entry = _REGION2_ENTRY_EMPTY;
		asce_type = _ASCE_TYPE_REGION2;
		break;
	}
	mm->context.asce = __pa(mm->pgd) | _ASCE_TABLE_LENGTH |
			   _ASCE_USER_BITS | asce_type;
	crst_table_init((unsigned long *) mm->pgd, init_entry);
	return 0;
}

static inline void switch_mm_irqs_off(struct mm_struct *prev, struct mm_struct *next,
				      struct task_struct *tsk)
{
	int cpu = smp_processor_id();

	if (next == &init_mm)
		S390_lowcore.user_asce = s390_invalid_asce;
	else
		S390_lowcore.user_asce = next->context.asce;
	cpumask_set_cpu(cpu, &next->context.cpu_attach_mask);
	/* Clear previous user-ASCE from CR7 */
	__ctl_load(s390_invalid_asce, 7, 7);
	if (prev != next)
		cpumask_clear_cpu(cpu, &prev->context.cpu_attach_mask);
}
#define switch_mm_irqs_off switch_mm_irqs_off
>>>>>>> upstream/android-13

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next,
			     struct task_struct *tsk)
{
<<<<<<< HEAD
	int cpu = smp_processor_id();

	S390_lowcore.user_asce = next->context.asce;
	cpumask_set_cpu(cpu, &next->context.cpu_attach_mask);
	/* Clear previous user-ASCE from CR1 and CR7 */
	if (!test_cpu_flag(CIF_ASCE_PRIMARY)) {
		__ctl_load(S390_lowcore.kernel_asce, 1, 1);
		set_cpu_flag(CIF_ASCE_PRIMARY);
	}
	if (test_cpu_flag(CIF_ASCE_SECONDARY)) {
		__ctl_load(S390_lowcore.vdso_asce, 7, 7);
		clear_cpu_flag(CIF_ASCE_SECONDARY);
	}
	if (prev != next)
		cpumask_clear_cpu(cpu, &prev->context.cpu_attach_mask);
=======
	unsigned long flags;

	local_irq_save(flags);
	switch_mm_irqs_off(prev, next, tsk);
	local_irq_restore(flags);
>>>>>>> upstream/android-13
}

#define finish_arch_post_lock_switch finish_arch_post_lock_switch
static inline void finish_arch_post_lock_switch(void)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;

	if (mm) {
		preempt_disable();
		while (atomic_read(&mm->context.flush_count))
			cpu_relax();
		cpumask_set_cpu(smp_processor_id(), mm_cpumask(mm));
		__tlb_flush_mm_lazy(mm);
		preempt_enable();
	}
<<<<<<< HEAD
	set_fs(current->thread.mm_segment);
}

#define enter_lazy_tlb(mm,tsk)	do { } while (0)
#define deactivate_mm(tsk,mm)	do { } while (0)

=======
	__ctl_load(S390_lowcore.user_asce, 7, 7);
}

#define activate_mm activate_mm
>>>>>>> upstream/android-13
static inline void activate_mm(struct mm_struct *prev,
                               struct mm_struct *next)
{
	switch_mm(prev, next, current);
	cpumask_set_cpu(smp_processor_id(), mm_cpumask(next));
<<<<<<< HEAD
	set_user_asce(next);
}

=======
	__ctl_load(S390_lowcore.user_asce, 7, 7);
}

#include <asm-generic/mmu_context.h>

>>>>>>> upstream/android-13
#endif /* __S390_MMU_CONTEXT_H */
