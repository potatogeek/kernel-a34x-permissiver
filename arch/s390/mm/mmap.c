// SPDX-License-Identifier: GPL-2.0+
/*
 *  flexible mmap layout support
 *
 * Copyright 2003-2004 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * Started by Ingo Molnar <mingo@elte.hu>
 */

#include <linux/elf-randomize.h>
#include <linux/personality.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/sched/signal.h>
#include <linux/sched/mm.h>
#include <linux/random.h>
#include <linux/compat.h>
#include <linux/security.h>
<<<<<<< HEAD
#include <asm/pgalloc.h>
=======
>>>>>>> upstream/android-13
#include <asm/elf.h>

static unsigned long stack_maxrandom_size(void)
{
	if (!(current->flags & PF_RANDOMIZE))
		return 0;
<<<<<<< HEAD
	if (current->personality & ADDR_NO_RANDOMIZE)
		return 0;
	return STACK_RND_MASK << PAGE_SHIFT;
}

/*
 * Top of mmap area (just below the process stack).
 *
 * Leave at least a ~32 MB hole.
 */
#define MIN_GAP (32*1024*1024)
#define MAX_GAP (STACK_TOP/6*5)

=======
	return STACK_RND_MASK << PAGE_SHIFT;
}

>>>>>>> upstream/android-13
static inline int mmap_is_legacy(struct rlimit *rlim_stack)
{
	if (current->personality & ADDR_COMPAT_LAYOUT)
		return 1;
	if (rlim_stack->rlim_cur == RLIM_INFINITY)
		return 1;
	return sysctl_legacy_va_layout;
}

unsigned long arch_mmap_rnd(void)
{
	return (get_random_int() & MMAP_RND_MASK) << PAGE_SHIFT;
}

static unsigned long mmap_base_legacy(unsigned long rnd)
{
	return TASK_UNMAPPED_BASE + rnd;
}

static inline unsigned long mmap_base(unsigned long rnd,
				      struct rlimit *rlim_stack)
{
	unsigned long gap = rlim_stack->rlim_cur;
<<<<<<< HEAD

	if (gap < MIN_GAP)
		gap = MIN_GAP;
	else if (gap > MAX_GAP)
		gap = MAX_GAP;
	gap &= PAGE_MASK;
	return STACK_TOP - stack_maxrandom_size() - rnd - gap;
}

unsigned long
arch_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
=======
	unsigned long pad = stack_maxrandom_size() + stack_guard_gap;
	unsigned long gap_min, gap_max;

	/* Values close to RLIM_INFINITY can overflow. */
	if (gap + pad > gap)
		gap += pad;

	/*
	 * Top of mmap area (just below the process stack).
	 * Leave at least a ~32 MB hole.
	 */
	gap_min = 32 * 1024 * 1024UL;
	gap_max = (STACK_TOP / 6) * 5;

	if (gap < gap_min)
		gap = gap_min;
	else if (gap > gap_max)
		gap = gap_max;

	return PAGE_ALIGN(STACK_TOP - gap - rnd);
}

unsigned long arch_get_unmapped_area(struct file *filp, unsigned long addr,
				     unsigned long len, unsigned long pgoff,
				     unsigned long flags)
>>>>>>> upstream/android-13
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	struct vm_unmapped_area_info info;
<<<<<<< HEAD
	int rc;
=======
>>>>>>> upstream/android-13

	if (len > TASK_SIZE - mmap_min_addr)
		return -ENOMEM;

	if (flags & MAP_FIXED)
		goto check_asce_limit;

	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = find_vma(mm, addr);
		if (TASK_SIZE - len >= addr && addr >= mmap_min_addr &&
		    (!vma || addr + len <= vm_start_gap(vma)))
			goto check_asce_limit;
	}

	info.flags = 0;
	info.length = len;
	info.low_limit = mm->mmap_base;
	info.high_limit = TASK_SIZE;
	if (filp || (flags & MAP_SHARED))
		info.align_mask = MMAP_ALIGN_MASK << PAGE_SHIFT;
	else
		info.align_mask = 0;
	info.align_offset = pgoff << PAGE_SHIFT;
	addr = vm_unmapped_area(&info);
<<<<<<< HEAD
	if (addr & ~PAGE_MASK)
		return addr;

check_asce_limit:
	if (addr + len > current->mm->context.asce_limit &&
	    addr + len <= TASK_SIZE) {
		rc = crst_table_upgrade(mm, addr + len);
		if (rc)
			return (unsigned long) rc;
	}

	return addr;
}

unsigned long
arch_get_unmapped_area_topdown(struct file *filp, const unsigned long addr0,
			  const unsigned long len, const unsigned long pgoff,
			  const unsigned long flags)
{
	struct vm_area_struct *vma;
	struct mm_struct *mm = current->mm;
	unsigned long addr = addr0;
	struct vm_unmapped_area_info info;
	int rc;
=======
	if (offset_in_page(addr))
		return addr;

check_asce_limit:
	return check_asce_limit(mm, addr, len);
}

unsigned long arch_get_unmapped_area_topdown(struct file *filp, unsigned long addr,
					     unsigned long len, unsigned long pgoff,
					     unsigned long flags)
{
	struct vm_area_struct *vma;
	struct mm_struct *mm = current->mm;
	struct vm_unmapped_area_info info;
>>>>>>> upstream/android-13

	/* requested length too big for entire address space */
	if (len > TASK_SIZE - mmap_min_addr)
		return -ENOMEM;

	if (flags & MAP_FIXED)
		goto check_asce_limit;

	/* requesting a specific address */
	if (addr) {
		addr = PAGE_ALIGN(addr);
		vma = find_vma(mm, addr);
		if (TASK_SIZE - len >= addr && addr >= mmap_min_addr &&
				(!vma || addr + len <= vm_start_gap(vma)))
			goto check_asce_limit;
	}

	info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	info.length = len;
	info.low_limit = max(PAGE_SIZE, mmap_min_addr);
	info.high_limit = mm->mmap_base;
	if (filp || (flags & MAP_SHARED))
		info.align_mask = MMAP_ALIGN_MASK << PAGE_SHIFT;
	else
		info.align_mask = 0;
	info.align_offset = pgoff << PAGE_SHIFT;
	addr = vm_unmapped_area(&info);

	/*
	 * A failed mmap() very likely causes application failure,
	 * so fall back to the bottom-up function here. This scenario
	 * can happen with large stack limits and large mmap()
	 * allocations.
	 */
<<<<<<< HEAD
	if (addr & ~PAGE_MASK) {
=======
	if (offset_in_page(addr)) {
>>>>>>> upstream/android-13
		VM_BUG_ON(addr != -ENOMEM);
		info.flags = 0;
		info.low_limit = TASK_UNMAPPED_BASE;
		info.high_limit = TASK_SIZE;
		addr = vm_unmapped_area(&info);
<<<<<<< HEAD
		if (addr & ~PAGE_MASK)
=======
		if (offset_in_page(addr))
>>>>>>> upstream/android-13
			return addr;
	}

check_asce_limit:
<<<<<<< HEAD
	if (addr + len > current->mm->context.asce_limit &&
	    addr + len <= TASK_SIZE) {
		rc = crst_table_upgrade(mm, addr + len);
		if (rc)
			return (unsigned long) rc;
	}

	return addr;
=======
	return check_asce_limit(mm, addr, len);
>>>>>>> upstream/android-13
}

/*
 * This function, called very early during the creation of a new
 * process VM image, sets up which VM layout function to use:
 */
void arch_pick_mmap_layout(struct mm_struct *mm, struct rlimit *rlim_stack)
{
	unsigned long random_factor = 0UL;

	if (current->flags & PF_RANDOMIZE)
		random_factor = arch_mmap_rnd();

	/*
	 * Fall back to the standard layout if the personality
	 * bit is set, or if the expected stack growth is unlimited:
	 */
	if (mmap_is_legacy(rlim_stack)) {
		mm->mmap_base = mmap_base_legacy(random_factor);
		mm->get_unmapped_area = arch_get_unmapped_area;
	} else {
		mm->mmap_base = mmap_base(random_factor, rlim_stack);
		mm->get_unmapped_area = arch_get_unmapped_area_topdown;
	}
}
