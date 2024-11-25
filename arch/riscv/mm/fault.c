<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2009 Sunplus Core Technology Co., Ltd.
 *  Lennox Wu <lennox.wu@sunplusct.com>
 *  Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
=======
>>>>>>> upstream/android-13
 */


#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/perf_event.h>
#include <linux/signal.h>
#include <linux/uaccess.h>
<<<<<<< HEAD

#include <asm/pgalloc.h>
#include <asm/ptrace.h>
#include <asm/tlbflush.h>

=======
#include <linux/kprobes.h>
#include <linux/kfence.h>

#include <asm/ptrace.h>
#include <asm/tlbflush.h>

#include "../kernel/head.h"

static void die_kernel_fault(const char *msg, unsigned long addr,
		struct pt_regs *regs)
{
	bust_spinlocks(1);

	pr_alert("Unable to handle kernel %s at virtual address " REG_FMT "\n", msg,
		addr);

	bust_spinlocks(0);
	die(regs, "Oops");
	do_exit(SIGKILL);
}

static inline void no_context(struct pt_regs *regs, unsigned long addr)
{
	const char *msg;

	/* Are we prepared to handle this kernel fault? */
	if (fixup_exception(regs))
		return;

	/*
	 * Oops. The kernel tried to access some bad page. We'll have to
	 * terminate things with extreme prejudice.
	 */
	if (addr < PAGE_SIZE)
		msg = "NULL pointer dereference";
	else {
		if (kfence_handle_page_fault(addr, regs->cause == EXC_STORE_PAGE_FAULT, regs))
			return;

		msg = "paging request";
	}

	die_kernel_fault(msg, addr, regs);
}

static inline void mm_fault_error(struct pt_regs *regs, unsigned long addr, vm_fault_t fault)
{
	if (fault & VM_FAULT_OOM) {
		/*
		 * We ran out of memory, call the OOM killer, and return the userspace
		 * (which will retry the fault, or kill us if we got oom-killed).
		 */
		if (!user_mode(regs)) {
			no_context(regs, addr);
			return;
		}
		pagefault_out_of_memory();
		return;
	} else if (fault & VM_FAULT_SIGBUS) {
		/* Kernel mode? Handle exceptions or die */
		if (!user_mode(regs)) {
			no_context(regs, addr);
			return;
		}
		do_trap(regs, SIGBUS, BUS_ADRERR, addr);
		return;
	}
	BUG();
}

static inline void bad_area(struct pt_regs *regs, struct mm_struct *mm, int code, unsigned long addr)
{
	/*
	 * Something tried to access memory that isn't in our memory map.
	 * Fix it, but check if it's kernel or user first.
	 */
	mmap_read_unlock(mm);
	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs)) {
		do_trap(regs, SIGSEGV, code, addr);
		return;
	}

	no_context(regs, addr);
}

static inline void vmalloc_fault(struct pt_regs *regs, int code, unsigned long addr)
{
	pgd_t *pgd, *pgd_k;
	pud_t *pud, *pud_k;
	p4d_t *p4d, *p4d_k;
	pmd_t *pmd, *pmd_k;
	pte_t *pte_k;
	int index;
	unsigned long pfn;

	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs))
		return do_trap(regs, SIGSEGV, code, addr);

	/*
	 * Synchronize this task's top level page-table
	 * with the 'reference' page table.
	 *
	 * Do _not_ use "tsk->active_mm->pgd" here.
	 * We might be inside an interrupt in the middle
	 * of a task switch.
	 */
	index = pgd_index(addr);
	pfn = csr_read(CSR_SATP) & SATP_PPN;
	pgd = (pgd_t *)pfn_to_virt(pfn) + index;
	pgd_k = init_mm.pgd + index;

	if (!pgd_present(*pgd_k)) {
		no_context(regs, addr);
		return;
	}
	set_pgd(pgd, *pgd_k);

	p4d = p4d_offset(pgd, addr);
	p4d_k = p4d_offset(pgd_k, addr);
	if (!p4d_present(*p4d_k)) {
		no_context(regs, addr);
		return;
	}

	pud = pud_offset(p4d, addr);
	pud_k = pud_offset(p4d_k, addr);
	if (!pud_present(*pud_k)) {
		no_context(regs, addr);
		return;
	}

	/*
	 * Since the vmalloc area is global, it is unnecessary
	 * to copy individual PTEs
	 */
	pmd = pmd_offset(pud, addr);
	pmd_k = pmd_offset(pud_k, addr);
	if (!pmd_present(*pmd_k)) {
		no_context(regs, addr);
		return;
	}
	set_pmd(pmd, *pmd_k);

	/*
	 * Make sure the actual PTE exists as well to
	 * catch kernel vmalloc-area accesses to non-mapped
	 * addresses. If we don't do this, this will just
	 * silently loop forever.
	 */
	pte_k = pte_offset_kernel(pmd_k, addr);
	if (!pte_present(*pte_k)) {
		no_context(regs, addr);
		return;
	}

	/*
	 * The kernel assumes that TLBs don't cache invalid
	 * entries, but in RISC-V, SFENCE.VMA specifies an
	 * ordering constraint, not a cache flush; it is
	 * necessary even after writing invalid entries.
	 */
	local_flush_tlb_page(addr);
}

static inline bool access_error(unsigned long cause, struct vm_area_struct *vma)
{
	switch (cause) {
	case EXC_INST_PAGE_FAULT:
		if (!(vma->vm_flags & VM_EXEC)) {
			return true;
		}
		break;
	case EXC_LOAD_PAGE_FAULT:
		if (!(vma->vm_flags & VM_READ)) {
			return true;
		}
		break;
	case EXC_STORE_PAGE_FAULT:
		if (!(vma->vm_flags & VM_WRITE)) {
			return true;
		}
		break;
	default:
		panic("%s: unhandled cause %lu", __func__, cause);
	}
	return false;
}

>>>>>>> upstream/android-13
/*
 * This routine handles page faults.  It determines the address and the
 * problem, and then passes it off to one of the appropriate routines.
 */
asmlinkage void do_page_fault(struct pt_regs *regs)
{
	struct task_struct *tsk;
	struct vm_area_struct *vma;
	struct mm_struct *mm;
	unsigned long addr, cause;
<<<<<<< HEAD
	unsigned int flags = FAULT_FLAG_ALLOW_RETRY | FAULT_FLAG_KILLABLE;
	int code = SEGV_MAPERR;
	vm_fault_t fault;

	cause = regs->scause;
	addr = regs->sbadaddr;
=======
	unsigned int flags = FAULT_FLAG_DEFAULT;
	int code = SEGV_MAPERR;
	vm_fault_t fault;

	cause = regs->cause;
	addr = regs->badaddr;
>>>>>>> upstream/android-13

	tsk = current;
	mm = tsk->mm;

<<<<<<< HEAD
=======
	if (kprobe_page_fault(regs, cause))
		return;

>>>>>>> upstream/android-13
	/*
	 * Fault-in kernel-space virtual memory on-demand.
	 * The 'reference' page table is init_mm.pgd.
	 *
	 * NOTE! We MUST NOT take any locks for this case. We may
	 * be in an interrupt or a critical region, and should
	 * only copy the information from the master page table,
	 * nothing more.
	 */
<<<<<<< HEAD
	if (unlikely((addr >= VMALLOC_START) && (addr <= VMALLOC_END)))
		goto vmalloc_fault;

	/* Enable interrupts if they were enabled in the parent context. */
	if (likely(regs->sstatus & SR_SPIE))
=======
	if (unlikely((addr >= VMALLOC_START) && (addr <= VMALLOC_END))) {
		vmalloc_fault(regs, code, addr);
		return;
	}

#ifdef CONFIG_64BIT
	/*
	 * Modules in 64bit kernels lie in their own virtual region which is not
	 * in the vmalloc region, but dealing with page faults in this region
	 * or the vmalloc region amounts to doing the same thing: checking that
	 * the mapping exists in init_mm.pgd and updating user page table, so
	 * just use vmalloc_fault.
	 */
	if (unlikely(addr >= MODULES_VADDR && addr < MODULES_END)) {
		vmalloc_fault(regs, code, addr);
		return;
	}
#endif
	/* Enable interrupts if they were enabled in the parent context. */
	if (likely(regs->status & SR_PIE))
>>>>>>> upstream/android-13
		local_irq_enable();

	/*
	 * If we're in an interrupt, have no user context, or are running
	 * in an atomic region, then we must not take the fault.
	 */
<<<<<<< HEAD
	if (unlikely(faulthandler_disabled() || !mm))
		goto no_context;
=======
	if (unlikely(faulthandler_disabled() || !mm)) {
		tsk->thread.bad_cause = cause;
		no_context(regs, addr);
		return;
	}
>>>>>>> upstream/android-13

	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;

<<<<<<< HEAD
	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, addr);

retry:
	down_read(&mm->mmap_sem);
	vma = find_vma(mm, addr);
	if (unlikely(!vma))
		goto bad_area;
	if (likely(vma->vm_start <= addr))
		goto good_area;
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN)))
		goto bad_area;
	if (unlikely(expand_stack(vma, addr)))
		goto bad_area;
=======
	if (!user_mode(regs) && addr < TASK_SIZE &&
			unlikely(!(regs->status & SR_SUM)))
		die_kernel_fault("access to user memory without uaccess routines",
				addr, regs);

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, addr);

	if (cause == EXC_STORE_PAGE_FAULT)
		flags |= FAULT_FLAG_WRITE;
	else if (cause == EXC_INST_PAGE_FAULT)
		flags |= FAULT_FLAG_INSTRUCTION;
retry:
	mmap_read_lock(mm);
	vma = find_vma(mm, addr);
	if (unlikely(!vma)) {
		tsk->thread.bad_cause = cause;
		bad_area(regs, mm, code, addr);
		return;
	}
	if (likely(vma->vm_start <= addr))
		goto good_area;
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN))) {
		tsk->thread.bad_cause = cause;
		bad_area(regs, mm, code, addr);
		return;
	}
	if (unlikely(expand_stack(vma, addr))) {
		tsk->thread.bad_cause = cause;
		bad_area(regs, mm, code, addr);
		return;
	}
>>>>>>> upstream/android-13

	/*
	 * Ok, we have a good vm_area for this memory access, so
	 * we can handle it.
	 */
good_area:
	code = SEGV_ACCERR;

<<<<<<< HEAD
	switch (cause) {
	case EXC_INST_PAGE_FAULT:
		if (!(vma->vm_flags & VM_EXEC))
			goto bad_area;
		break;
	case EXC_LOAD_PAGE_FAULT:
		if (!(vma->vm_flags & VM_READ))
			goto bad_area;
		break;
	case EXC_STORE_PAGE_FAULT:
		if (!(vma->vm_flags & VM_WRITE))
			goto bad_area;
		flags |= FAULT_FLAG_WRITE;
		break;
	default:
		panic("%s: unhandled cause %lu", __func__, cause);
=======
	if (unlikely(access_error(cause, vma))) {
		tsk->thread.bad_cause = cause;
		bad_area(regs, mm, code, addr);
		return;
>>>>>>> upstream/android-13
	}

	/*
	 * If for any reason at all we could not handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */
<<<<<<< HEAD
	fault = handle_mm_fault(vma, addr, flags);

	/*
	 * If we need to retry but a fatal signal is pending, handle the
	 * signal first. We do not need to release the mmap_sem because it
	 * would already be released in __lock_page_or_retry in mm/filemap.c.
	 */
	if ((fault & VM_FAULT_RETRY) && fatal_signal_pending(tsk))
		return;

	if (unlikely(fault & VM_FAULT_ERROR)) {
		if (fault & VM_FAULT_OOM)
			goto out_of_memory;
		else if (fault & VM_FAULT_SIGBUS)
			goto do_sigbus;
		BUG();
	}

	/*
	 * Major/minor page fault accounting is only done on the
	 * initial attempt. If we go through a retry, it is extremely
	 * likely that the page will be found in page cache at that point.
	 */
	if (flags & FAULT_FLAG_ALLOW_RETRY) {
		if (fault & VM_FAULT_MAJOR) {
			tsk->maj_flt++;
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ,
				      1, regs, addr);
		} else {
			tsk->min_flt++;
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN,
				      1, regs, addr);
		}
		if (fault & VM_FAULT_RETRY) {
			/*
			 * Clear FAULT_FLAG_ALLOW_RETRY to avoid any risk
			 * of starvation.
			 */
			flags &= ~(FAULT_FLAG_ALLOW_RETRY);
			flags |= FAULT_FLAG_TRIED;

			/*
			 * No need to up_read(&mm->mmap_sem) as we would
			 * have already released it in __lock_page_or_retry
			 * in mm/filemap.c.
			 */
			goto retry;
		}
	}

	up_read(&mm->mmap_sem);
	return;

	/*
	 * Something tried to access memory that isn't in our memory map.
	 * Fix it, but check if it's kernel or user first.
	 */
bad_area:
	up_read(&mm->mmap_sem);
	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs)) {
		do_trap(regs, SIGSEGV, code, addr, tsk);
		return;
	}

no_context:
	/* Are we prepared to handle this kernel fault? */
	if (fixup_exception(regs))
		return;

	/*
	 * Oops. The kernel tried to access some bad page. We'll have to
	 * terminate things with extreme prejudice.
	 */
	bust_spinlocks(1);
	pr_alert("Unable to handle kernel %s at virtual address " REG_FMT "\n",
		(addr < PAGE_SIZE) ? "NULL pointer dereference" :
		"paging request", addr);
	die(regs, "Oops");
	do_exit(SIGKILL);

	/*
	 * We ran out of memory, call the OOM killer, and return the userspace
	 * (which will retry the fault, or kill us if we got oom-killed).
	 */
out_of_memory:
	up_read(&mm->mmap_sem);
	if (!user_mode(regs))
		goto no_context;
	pagefault_out_of_memory();
	return;

do_sigbus:
	up_read(&mm->mmap_sem);
	/* Kernel mode? Handle exceptions or die */
	if (!user_mode(regs))
		goto no_context;
	do_trap(regs, SIGBUS, BUS_ADRERR, addr, tsk);
	return;

vmalloc_fault:
	{
		pgd_t *pgd, *pgd_k;
		pud_t *pud, *pud_k;
		p4d_t *p4d, *p4d_k;
		pmd_t *pmd, *pmd_k;
		pte_t *pte_k;
		int index;

		if (user_mode(regs))
			goto bad_area;

		/*
		 * Synchronize this task's top level page-table
		 * with the 'reference' page table.
		 *
		 * Do _not_ use "tsk->active_mm->pgd" here.
		 * We might be inside an interrupt in the middle
		 * of a task switch.
		 *
		 * Note: Use the old spbtr name instead of using the current
		 * satp name to support binutils 2.29 which doesn't know about
		 * the privileged ISA 1.10 yet.
		 */
		index = pgd_index(addr);
		pgd = (pgd_t *)pfn_to_virt(csr_read(sptbr)) + index;
		pgd_k = init_mm.pgd + index;

		if (!pgd_present(*pgd_k))
			goto no_context;
		set_pgd(pgd, *pgd_k);

		p4d = p4d_offset(pgd, addr);
		p4d_k = p4d_offset(pgd_k, addr);
		if (!p4d_present(*p4d_k))
			goto no_context;

		pud = pud_offset(p4d, addr);
		pud_k = pud_offset(p4d_k, addr);
		if (!pud_present(*pud_k))
			goto no_context;

		/*
		 * Since the vmalloc area is global, it is unnecessary
		 * to copy individual PTEs
		 */
		pmd = pmd_offset(pud, addr);
		pmd_k = pmd_offset(pud_k, addr);
		if (!pmd_present(*pmd_k))
			goto no_context;
		set_pmd(pmd, *pmd_k);

		/*
		 * Make sure the actual PTE exists as well to
		 * catch kernel vmalloc-area accesses to non-mapped
		 * addresses. If we don't do this, this will just
		 * silently loop forever.
		 */
		pte_k = pte_offset_kernel(pmd_k, addr);
		if (!pte_present(*pte_k))
			goto no_context;

		/*
		 * The kernel assumes that TLBs don't cache invalid
		 * entries, but in RISC-V, SFENCE.VMA specifies an
		 * ordering constraint, not a cache flush; it is
		 * necessary even after writing invalid entries.
		 * Relying on flush_tlb_fix_spurious_fault would
		 * suffice, but the extra traps reduce
		 * performance. So, eagerly SFENCE.VMA.
		 */
		local_flush_tlb_page(addr);

		return;
	}
}
=======
	fault = handle_mm_fault(vma, addr, flags, regs);

	/*
	 * If we need to retry but a fatal signal is pending, handle the
	 * signal first. We do not need to release the mmap_lock because it
	 * would already be released in __lock_page_or_retry in mm/filemap.c.
	 */
	if (fault_signal_pending(fault, regs))
		return;

	if (unlikely((fault & VM_FAULT_RETRY) && (flags & FAULT_FLAG_ALLOW_RETRY))) {
		flags |= FAULT_FLAG_TRIED;

		/*
		 * No need to mmap_read_unlock(mm) as we would
		 * have already released it in __lock_page_or_retry
		 * in mm/filemap.c.
		 */
		goto retry;
	}

	mmap_read_unlock(mm);

	if (unlikely(fault & VM_FAULT_ERROR)) {
		tsk->thread.bad_cause = cause;
		mm_fault_error(regs, addr, fault);
		return;
	}
	return;
}
NOKPROBE_SYMBOL(do_page_fault);
>>>>>>> upstream/android-13
