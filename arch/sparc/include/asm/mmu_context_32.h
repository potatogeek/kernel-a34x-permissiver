/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SPARC_MMU_CONTEXT_H
#define __SPARC_MMU_CONTEXT_H

#ifndef __ASSEMBLY__

#include <asm-generic/mm_hooks.h>

<<<<<<< HEAD
static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
}

/* Initialize a new mmu context.  This is invoked when a new
 * address space instance (unique or shared) is instantiated.
 */
=======
/* Initialize a new mmu context.  This is invoked when a new
 * address space instance (unique or shared) is instantiated.
 */
#define init_new_context init_new_context
>>>>>>> upstream/android-13
int init_new_context(struct task_struct *tsk, struct mm_struct *mm);

/* Destroy a dead context.  This occurs when mmput drops the
 * mm_users count to zero, the mmaps have been released, and
 * all the page tables have been flushed.  Our job is to destroy
 * any remaining processor-specific state.
 */
<<<<<<< HEAD
=======
#define destroy_context destroy_context
>>>>>>> upstream/android-13
void destroy_context(struct mm_struct *mm);

/* Switch the current MM context. */
void switch_mm(struct mm_struct *old_mm, struct mm_struct *mm,
	       struct task_struct *tsk);

<<<<<<< HEAD
#define deactivate_mm(tsk,mm)	do { } while (0)

/* Activate a new MM instance for the current task. */
#define activate_mm(active_mm, mm) switch_mm((active_mm), (mm), NULL)

=======
/* Activate a new MM instance for the current task. */
#define activate_mm(active_mm, mm) switch_mm((active_mm), (mm), NULL)

#include <asm-generic/mmu_context.h>

>>>>>>> upstream/android-13
#endif /* !(__ASSEMBLY__) */

#endif /* !(__SPARC_MMU_CONTEXT_H) */
