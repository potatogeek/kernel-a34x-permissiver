// SPDX-License-Identifier: GPL-2.0
#include <linux/mm_types.h>
#include <linux/rbtree.h>
#include <linux/rwsem.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/cpumask.h>
<<<<<<< HEAD

#include <linux/atomic.h>
#include <linux/user_namespace.h>
#include <asm/pgtable.h>
=======
#include <linux/mman.h>
#include <linux/pgtable.h>

#include <linux/atomic.h>
#include <linux/user_namespace.h>
>>>>>>> upstream/android-13
#include <asm/mmu.h>

#ifndef INIT_MM_CONTEXT
#define INIT_MM_CONTEXT(name)
#endif

/*
 * For dynamically allocated mm_structs, there is a dynamically sized cpumask
 * at the end of the structure, the size of which depends on the maximum CPU
 * number the system can see. That way we allocate only as much memory for
 * mm_cpumask() as needed for the hundreds, or thousands of processes that
 * a system typically runs.
 *
 * Since there is only one init_mm in the entire system, keep it simple
 * and size this cpu_bitmask to NR_CPUS.
 */
struct mm_struct init_mm = {
	.mm_rb		= RB_ROOT,
<<<<<<< HEAD
#ifdef CONFIG_SPECULATIVE_PAGE_FAULT
	.mm_rb_lock	= __RW_LOCK_UNLOCKED(init_mm.mm_rb_lock),
#endif
	.pgd		= swapper_pg_dir,
	.mm_users	= ATOMIC_INIT(2),
	.mm_count	= ATOMIC_INIT(1),
	.mmap_sem	= __RWSEM_INITIALIZER(init_mm.mmap_sem),
=======
	.pgd		= swapper_pg_dir,
	.mm_users	= ATOMIC_INIT(2),
	.mm_count	= ATOMIC_INIT(1),
	.write_protect_seq = SEQCNT_ZERO(init_mm.write_protect_seq),
	MMAP_LOCK_INITIALIZER(init_mm)
>>>>>>> upstream/android-13
	.page_table_lock =  __SPIN_LOCK_UNLOCKED(init_mm.page_table_lock),
	.arg_lock	=  __SPIN_LOCK_UNLOCKED(init_mm.arg_lock),
	.mmlist		= LIST_HEAD_INIT(init_mm.mmlist),
	.user_ns	= &init_user_ns,
<<<<<<< HEAD
	.cpu_bitmap	= { [BITS_TO_LONGS(NR_CPUS)] = 0},
	INIT_MM_CONTEXT(init_mm)
};
=======
	.cpu_bitmap	= CPU_BITS_NONE,
	INIT_MM_CONTEXT(init_mm)
};

void setup_initial_init_mm(void *start_code, void *end_code,
			   void *end_data, void *brk)
{
	init_mm.start_code = (unsigned long)start_code;
	init_mm.end_code = (unsigned long)end_code;
	init_mm.end_data = (unsigned long)end_data;
	init_mm.brk = (unsigned long)brk;
}
>>>>>>> upstream/android-13
