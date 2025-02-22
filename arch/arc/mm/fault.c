<<<<<<< HEAD
/* Page Fault Handling for ARC (TLB Miss / ProtV)
 *
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Page Fault Handling for ARC (TLB Miss / ProtV)
 *
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
>>>>>>> upstream/android-13
 */

#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <linux/uaccess.h>
#include <linux/kdebug.h>
#include <linux/perf_event.h>
#include <linux/mm_types.h>
<<<<<<< HEAD
#include <asm/pgalloc.h>
=======
>>>>>>> upstream/android-13
#include <asm/mmu.h>

/*
 * kernel virtual address is required to implement vmalloc/pkmap/fixmap
 * Refer to asm/processor.h for System Memory Map
 *
 * It simply copies the PMD entry (pointer to 2nd level page table or hugepage)
 * from swapper pgdir to task pgdir. The 2nd level table/page is thus shared
 */
noinline static int handle_kernel_vaddr_fault(unsigned long address)
{
	/*
	 * Synchronize this task's top level page-table
	 * with the 'reference' page table.
	 */
	pgd_t *pgd, *pgd_k;
<<<<<<< HEAD
	pud_t *pud, *pud_k;
	pmd_t *pmd, *pmd_k;

	pgd = pgd_offset_fast(current->active_mm, address);
	pgd_k = pgd_offset_k(address);

	if (!pgd_present(*pgd_k))
		goto bad_area;

	pud = pud_offset(pgd, address);
	pud_k = pud_offset(pgd_k, address);
	if (!pud_present(*pud_k))
		goto bad_area;

	pmd = pmd_offset(pud, address);
	pmd_k = pmd_offset(pud_k, address);
	if (!pmd_present(*pmd_k))
		goto bad_area;

	set_pmd(pmd, *pmd_k);
=======
	p4d_t *p4d, *p4d_k;
	pud_t *pud, *pud_k;
	pmd_t *pmd, *pmd_k;

	pgd = pgd_offset(current->active_mm, address);
	pgd_k = pgd_offset_k(address);

	if (pgd_none (*pgd_k))
		goto bad_area;
	if (!pgd_present(*pgd))
		set_pgd(pgd, *pgd_k);

	p4d = p4d_offset(pgd, address);
	p4d_k = p4d_offset(pgd_k, address);
	if (p4d_none(*p4d_k))
		goto bad_area;
	if (!p4d_present(*p4d))
		set_p4d(p4d, *p4d_k);

	pud = pud_offset(p4d, address);
	pud_k = pud_offset(p4d_k, address);
	if (pud_none(*pud_k))
		goto bad_area;
	if (!pud_present(*pud))
		set_pud(pud, *pud_k);

	pmd = pmd_offset(pud, address);
	pmd_k = pmd_offset(pud_k, address);
	if (pmd_none(*pmd_k))
		goto bad_area;
	if (!pmd_present(*pmd))
		set_pmd(pmd, *pmd_k);
>>>>>>> upstream/android-13

	/* XXX: create the TLB entry here */
	return 0;

bad_area:
	return 1;
}

void do_page_fault(unsigned long address, struct pt_regs *regs)
{
	struct vm_area_struct *vma = NULL;
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;
<<<<<<< HEAD
	int si_code = SEGV_MAPERR;
	int ret;
	vm_fault_t fault;
	int write = regs->ecr_cause & ECR_C_PROTV_STORE;  /* ST/EX */
	unsigned int flags = FAULT_FLAG_ALLOW_RETRY | FAULT_FLAG_KILLABLE;

	/*
	 * We fault-in kernel-space virtual memory on-demand. The
	 * 'reference' page table is init_mm.pgd.
	 *
=======
	int sig, si_code = SEGV_MAPERR;
	unsigned int write = 0, exec = 0, mask;
	vm_fault_t fault = VM_FAULT_SIGSEGV;	/* handle_mm_fault() output */
	unsigned int flags;			/* handle_mm_fault() input */

	/*
>>>>>>> upstream/android-13
	 * NOTE! We MUST NOT take any locks for this case. We may
	 * be in an interrupt or a critical region, and should
	 * only copy the information from the master page table,
	 * nothing more.
	 */
	if (address >= VMALLOC_START && !user_mode(regs)) {
<<<<<<< HEAD
		ret = handle_kernel_vaddr_fault(address);
		if (unlikely(ret))
=======
		if (unlikely(handle_kernel_vaddr_fault(address)))
>>>>>>> upstream/android-13
			goto no_context;
		else
			return;
	}

	/*
	 * If we're in an interrupt or have no user
	 * context, we must not take the fault..
	 */
	if (faulthandler_disabled() || !mm)
		goto no_context;

<<<<<<< HEAD
	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;
retry:
	down_read(&mm->mmap_sem);
	vma = find_vma(mm, address);
	if (!vma)
		goto bad_area;
	if (vma->vm_start <= address)
		goto good_area;
	if (!(vma->vm_flags & VM_GROWSDOWN))
		goto bad_area;
	if (expand_stack(vma, address))
		goto bad_area;

	/*
	 * Ok, we have a good vm_area for this memory access, so
	 * we can handle it..
	 */
good_area:
	si_code = SEGV_ACCERR;

	/* Handle protection violation, execute on heap or stack */

	if ((regs->ecr_vec == ECR_V_PROTV) &&
	    (regs->ecr_cause == ECR_C_PROTV_INST_FETCH))
		goto bad_area;

	if (write) {
		if (!(vma->vm_flags & VM_WRITE))
			goto bad_area;
		flags |= FAULT_FLAG_WRITE;
	} else {
		if (!(vma->vm_flags & (VM_READ | VM_EXEC)))
=======
	if (regs->ecr_cause & ECR_C_PROTV_STORE)	/* ST/EX */
		write = 1;
	else if ((regs->ecr_vec == ECR_V_PROTV) &&
	         (regs->ecr_cause == ECR_C_PROTV_INST_FETCH))
		exec = 1;

	flags = FAULT_FLAG_DEFAULT;
	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;
	if (write)
		flags |= FAULT_FLAG_WRITE;

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
retry:
	mmap_read_lock(mm);

	vma = find_vma(mm, address);
	if (!vma)
		goto bad_area;
	if (unlikely(address < vma->vm_start)) {
		if (!(vma->vm_flags & VM_GROWSDOWN) || expand_stack(vma, address))
>>>>>>> upstream/android-13
			goto bad_area;
	}

	/*
<<<<<<< HEAD
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */
	fault = handle_mm_fault(vma, address, flags);

	if (unlikely(fatal_signal_pending(current))) {

		/*
		 * if fault retry, mmap_sem already relinquished by core mm
		 * so OK to return to user mode (with signal handled first)
		 */
		if (fault & VM_FAULT_RETRY) {
			if (!user_mode(regs))
				goto no_context;
			return;
		}
	}

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);

	if (likely(!(fault & VM_FAULT_ERROR))) {
		if (flags & FAULT_FLAG_ALLOW_RETRY) {
			/* To avoid updating stats twice for retry case */
			if (fault & VM_FAULT_MAJOR) {
				tsk->maj_flt++;
				perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1,
					      regs, address);
			} else {
				tsk->min_flt++;
				perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1,
					      regs, address);
			}

			if (fault & VM_FAULT_RETRY) {
				flags &= ~FAULT_FLAG_ALLOW_RETRY;
				flags |= FAULT_FLAG_TRIED;
				goto retry;
			}
		}

		/* Fault Handled Gracefully */
		up_read(&mm->mmap_sem);
		return;
	}

	if (fault & VM_FAULT_OOM)
		goto out_of_memory;
	else if (fault & VM_FAULT_SIGSEGV)
		goto bad_area;
	else if (fault & VM_FAULT_SIGBUS)
		goto do_sigbus;

	/* no man's land */
	BUG();

	/*
	 * Something tried to access memory that isn't in our memory map..
	 * Fix it, but check if it's kernel or user first..
	 */
bad_area:
	up_read(&mm->mmap_sem);

	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs)) {
		tsk->thread.fault_address = address;
		force_sig_fault(SIGSEGV, si_code, (void __user *)address, tsk);
		return;
	}

no_context:
	/* Are we prepared to handle this kernel fault?
	 *
	 * (The kernel has valid exception-points in the source
	 *  when it accesses user-memory. When it fails in one
	 *  of those points, we find it in a table and do a jump
	 *  to some fixup code that loads an appropriate error
	 *  code)
	 */
	if (fixup_exception(regs))
		return;

	die("Oops", regs, address);

out_of_memory:
	up_read(&mm->mmap_sem);

	if (user_mode(regs)) {
		pagefault_out_of_memory();
		return;
	}

	goto no_context;

do_sigbus:
	up_read(&mm->mmap_sem);

	if (!user_mode(regs))
		goto no_context;

	tsk->thread.fault_address = address;
	force_sig_fault(SIGBUS, BUS_ADRERR, (void __user *)address, tsk);
=======
	 * vm_area is good, now check permissions for this memory access
	 */
	mask = VM_READ;
	if (write)
		mask = VM_WRITE;
	if (exec)
		mask = VM_EXEC;

	if (!(vma->vm_flags & mask)) {
		si_code = SEGV_ACCERR;
		goto bad_area;
	}

	fault = handle_mm_fault(vma, address, flags, regs);

	/* Quick path to respond to signals */
	if (fault_signal_pending(fault, regs)) {
		if (!user_mode(regs))
			goto no_context;
		return;
	}

	/*
	 * Fault retry nuances, mmap_lock already relinquished by core mm
	 */
	if (unlikely((fault & VM_FAULT_RETRY) &&
		     (flags & FAULT_FLAG_ALLOW_RETRY))) {
		flags |= FAULT_FLAG_TRIED;
		goto retry;
	}

bad_area:
	mmap_read_unlock(mm);

	/*
	 * Major/minor page fault accounting
	 * (in case of retry we only land here once)
	 */
	if (likely(!(fault & VM_FAULT_ERROR)))
		/* Normal return path: fault Handled Gracefully */
		return;

	if (!user_mode(regs))
		goto no_context;

	if (fault & VM_FAULT_OOM) {
		pagefault_out_of_memory();
		return;
	}

	if (fault & VM_FAULT_SIGBUS) {
		sig = SIGBUS;
		si_code = BUS_ADRERR;
	}
	else {
		sig = SIGSEGV;
	}

	tsk->thread.fault_address = address;
	force_sig_fault(sig, si_code, (void __user *)address);
	return;

no_context:
	if (fixup_exception(regs))
		return;

	die("Oops", regs, address);
>>>>>>> upstream/android-13
}
