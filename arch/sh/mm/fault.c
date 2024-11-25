/*
 * Page fault handler for SH with an MMU.
 *
 *  Copyright (C) 1999  Niibe Yutaka
 *  Copyright (C) 2003 - 2012  Paul Mundt
 *
 *  Based on linux/arch/i386/mm/fault.c:
 *   Copyright (C) 1995  Linus Torvalds
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/hardirq.h>
#include <linux/kprobes.h>
#include <linux/perf_event.h>
#include <linux/kdebug.h>
#include <linux/uaccess.h>
#include <asm/io_trapped.h>
#include <asm/mmu_context.h>
#include <asm/tlbflush.h>
#include <asm/traps.h>

<<<<<<< HEAD
static inline int notify_page_fault(struct pt_regs *regs, int trap)
{
	int ret = 0;

	if (kprobes_built_in() && !user_mode(regs)) {
		preempt_disable();
		if (kprobe_running() && kprobe_fault_handler(regs, trap))
			ret = 1;
		preempt_enable();
	}

	return ret;
}

static void
force_sig_info_fault(int si_signo, int si_code, unsigned long address,
		     struct task_struct *tsk)
{
	force_sig_fault(si_signo, si_code, (void __user *)address, tsk);
=======
static void
force_sig_info_fault(int si_signo, int si_code, unsigned long address)
{
	force_sig_fault(si_signo, si_code, (void __user *)address);
>>>>>>> upstream/android-13
}

/*
 * This is useful to dump out the page tables associated with
 * 'addr' in mm 'mm'.
 */
static void show_pte(struct mm_struct *mm, unsigned long addr)
{
	pgd_t *pgd;

	if (mm) {
		pgd = mm->pgd;
	} else {
		pgd = get_TTB();

		if (unlikely(!pgd))
			pgd = swapper_pg_dir;
	}

<<<<<<< HEAD
	printk(KERN_ALERT "pgd = %p\n", pgd);
	pgd += pgd_index(addr);
	printk(KERN_ALERT "[%08lx] *pgd=%0*Lx", addr,
	       (u32)(sizeof(*pgd) * 2), (u64)pgd_val(*pgd));

	do {
=======
	pr_alert("pgd = %p\n", pgd);
	pgd += pgd_index(addr);
	pr_alert("[%08lx] *pgd=%0*llx", addr, (u32)(sizeof(*pgd) * 2),
		 (u64)pgd_val(*pgd));

	do {
		p4d_t *p4d;
>>>>>>> upstream/android-13
		pud_t *pud;
		pmd_t *pmd;
		pte_t *pte;

		if (pgd_none(*pgd))
			break;

		if (pgd_bad(*pgd)) {
<<<<<<< HEAD
			printk("(bad)");
			break;
		}

		pud = pud_offset(pgd, addr);
		if (PTRS_PER_PUD != 1)
			printk(", *pud=%0*Lx", (u32)(sizeof(*pud) * 2),
			       (u64)pud_val(*pud));
=======
			pr_cont("(bad)");
			break;
		}

		p4d = p4d_offset(pgd, addr);
		if (PTRS_PER_P4D != 1)
			pr_cont(", *p4d=%0*Lx", (u32)(sizeof(*p4d) * 2),
			        (u64)p4d_val(*p4d));

		if (p4d_none(*p4d))
			break;

		if (p4d_bad(*p4d)) {
			pr_cont("(bad)");
			break;
		}

		pud = pud_offset(p4d, addr);
		if (PTRS_PER_PUD != 1)
			pr_cont(", *pud=%0*llx", (u32)(sizeof(*pud) * 2),
				(u64)pud_val(*pud));
>>>>>>> upstream/android-13

		if (pud_none(*pud))
			break;

		if (pud_bad(*pud)) {
<<<<<<< HEAD
			printk("(bad)");
=======
			pr_cont("(bad)");
>>>>>>> upstream/android-13
			break;
		}

		pmd = pmd_offset(pud, addr);
		if (PTRS_PER_PMD != 1)
<<<<<<< HEAD
			printk(", *pmd=%0*Lx", (u32)(sizeof(*pmd) * 2),
			       (u64)pmd_val(*pmd));
=======
			pr_cont(", *pmd=%0*llx", (u32)(sizeof(*pmd) * 2),
				(u64)pmd_val(*pmd));
>>>>>>> upstream/android-13

		if (pmd_none(*pmd))
			break;

		if (pmd_bad(*pmd)) {
<<<<<<< HEAD
			printk("(bad)");
=======
			pr_cont("(bad)");
>>>>>>> upstream/android-13
			break;
		}

		/* We must not map this if we have highmem enabled */
		if (PageHighMem(pfn_to_page(pmd_val(*pmd) >> PAGE_SHIFT)))
			break;

		pte = pte_offset_kernel(pmd, addr);
<<<<<<< HEAD
		printk(", *pte=%0*Lx", (u32)(sizeof(*pte) * 2),
		       (u64)pte_val(*pte));
	} while (0);

	printk("\n");
=======
		pr_cont(", *pte=%0*llx", (u32)(sizeof(*pte) * 2),
			(u64)pte_val(*pte));
	} while (0);

	pr_cont("\n");
>>>>>>> upstream/android-13
}

static inline pmd_t *vmalloc_sync_one(pgd_t *pgd, unsigned long address)
{
	unsigned index = pgd_index(address);
	pgd_t *pgd_k;
<<<<<<< HEAD
=======
	p4d_t *p4d, *p4d_k;
>>>>>>> upstream/android-13
	pud_t *pud, *pud_k;
	pmd_t *pmd, *pmd_k;

	pgd += index;
	pgd_k = init_mm.pgd + index;

	if (!pgd_present(*pgd_k))
		return NULL;

<<<<<<< HEAD
	pud = pud_offset(pgd, address);
	pud_k = pud_offset(pgd_k, address);
=======
	p4d = p4d_offset(pgd, address);
	p4d_k = p4d_offset(pgd_k, address);
	if (!p4d_present(*p4d_k))
		return NULL;

	pud = pud_offset(p4d, address);
	pud_k = pud_offset(p4d_k, address);
>>>>>>> upstream/android-13
	if (!pud_present(*pud_k))
		return NULL;

	if (!pud_present(*pud))
	    set_pud(pud, *pud_k);

	pmd = pmd_offset(pud, address);
	pmd_k = pmd_offset(pud_k, address);
	if (!pmd_present(*pmd_k))
		return NULL;

	if (!pmd_present(*pmd))
		set_pmd(pmd, *pmd_k);
	else {
		/*
		 * The page tables are fully synchronised so there must
		 * be another reason for the fault. Return NULL here to
		 * signal that we have not taken care of the fault.
		 */
		BUG_ON(pmd_page(*pmd) != pmd_page(*pmd_k));
		return NULL;
	}

	return pmd_k;
}

#ifdef CONFIG_SH_STORE_QUEUES
#define __FAULT_ADDR_LIMIT	P3_ADDR_MAX
#else
#define __FAULT_ADDR_LIMIT	VMALLOC_END
#endif

/*
 * Handle a fault on the vmalloc or module mapping area
 */
static noinline int vmalloc_fault(unsigned long address)
{
	pgd_t *pgd_k;
	pmd_t *pmd_k;
	pte_t *pte_k;

	/* Make sure we are in vmalloc/module/P3 area: */
	if (!(address >= VMALLOC_START && address < __FAULT_ADDR_LIMIT))
		return -1;

	/*
	 * Synchronize this task's top level page-table
	 * with the 'reference' page table.
	 *
	 * Do _not_ use "current" here. We might be inside
	 * an interrupt in the middle of a task switch..
	 */
	pgd_k = get_TTB();
	pmd_k = vmalloc_sync_one(pgd_k, address);
	if (!pmd_k)
		return -1;

	pte_k = pte_offset_kernel(pmd_k, address);
	if (!pte_present(*pte_k))
		return -1;

	return 0;
}

static void
show_fault_oops(struct pt_regs *regs, unsigned long address)
{
	if (!oops_may_print())
		return;

<<<<<<< HEAD
	printk(KERN_ALERT "BUG: unable to handle kernel ");
	if (address < PAGE_SIZE)
		printk(KERN_CONT "NULL pointer dereference");
	else
		printk(KERN_CONT "paging request");

	printk(KERN_CONT " at %08lx\n", address);
	printk(KERN_ALERT "PC:");
=======
	pr_alert("BUG: unable to handle kernel %s at %08lx\n",
		 address < PAGE_SIZE ? "NULL pointer dereference"
				     : "paging request",
		 address);
	pr_alert("PC:");
>>>>>>> upstream/android-13
	printk_address(regs->pc, 1);

	show_pte(NULL, address);
}

static noinline void
no_context(struct pt_regs *regs, unsigned long error_code,
	   unsigned long address)
{
	/* Are we prepared to handle this kernel fault?  */
	if (fixup_exception(regs))
		return;

	if (handle_trapped_io(regs, address))
		return;

	/*
	 * Oops. The kernel tried to access some bad page. We'll have to
	 * terminate things with extreme prejudice.
	 */
	bust_spinlocks(1);

	show_fault_oops(regs, address);

	die("Oops", regs, error_code);
	bust_spinlocks(0);
	do_exit(SIGKILL);
}

static void
__bad_area_nosemaphore(struct pt_regs *regs, unsigned long error_code,
		       unsigned long address, int si_code)
{
<<<<<<< HEAD
	struct task_struct *tsk = current;

=======
>>>>>>> upstream/android-13
	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs)) {
		/*
		 * It's possible to have interrupts off here:
		 */
		local_irq_enable();

<<<<<<< HEAD
		force_sig_info_fault(SIGSEGV, si_code, address, tsk);
=======
		force_sig_info_fault(SIGSEGV, si_code, address);
>>>>>>> upstream/android-13

		return;
	}

	no_context(regs, error_code, address);
}

static noinline void
bad_area_nosemaphore(struct pt_regs *regs, unsigned long error_code,
		     unsigned long address)
{
	__bad_area_nosemaphore(regs, error_code, address, SEGV_MAPERR);
}

static void
__bad_area(struct pt_regs *regs, unsigned long error_code,
	   unsigned long address, int si_code)
{
	struct mm_struct *mm = current->mm;

	/*
	 * Something tried to access memory that isn't in our memory map..
	 * Fix it, but check if it's kernel or user first..
	 */
<<<<<<< HEAD
	up_read(&mm->mmap_sem);
=======
	mmap_read_unlock(mm);
>>>>>>> upstream/android-13

	__bad_area_nosemaphore(regs, error_code, address, si_code);
}

static noinline void
bad_area(struct pt_regs *regs, unsigned long error_code, unsigned long address)
{
	__bad_area(regs, error_code, address, SEGV_MAPERR);
}

static noinline void
bad_area_access_error(struct pt_regs *regs, unsigned long error_code,
		      unsigned long address)
{
	__bad_area(regs, error_code, address, SEGV_ACCERR);
}

static void
do_sigbus(struct pt_regs *regs, unsigned long error_code, unsigned long address)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;

<<<<<<< HEAD
	up_read(&mm->mmap_sem);
=======
	mmap_read_unlock(mm);
>>>>>>> upstream/android-13

	/* Kernel mode? Handle exceptions or die: */
	if (!user_mode(regs))
		no_context(regs, error_code, address);

<<<<<<< HEAD
	force_sig_info_fault(SIGBUS, BUS_ADRERR, address, tsk);
=======
	force_sig_info_fault(SIGBUS, BUS_ADRERR, address);
>>>>>>> upstream/android-13
}

static noinline int
mm_fault_error(struct pt_regs *regs, unsigned long error_code,
	       unsigned long address, vm_fault_t fault)
{
	/*
	 * Pagefault was interrupted by SIGKILL. We have no reason to
	 * continue pagefault.
	 */
<<<<<<< HEAD
	if (fatal_signal_pending(current)) {
		if (!(fault & VM_FAULT_RETRY))
			up_read(&current->mm->mmap_sem);
=======
	if (fault_signal_pending(fault, regs)) {
>>>>>>> upstream/android-13
		if (!user_mode(regs))
			no_context(regs, error_code, address);
		return 1;
	}

<<<<<<< HEAD
=======
	/* Release mmap_lock first if necessary */
	if (!(fault & VM_FAULT_RETRY))
		mmap_read_unlock(current->mm);

>>>>>>> upstream/android-13
	if (!(fault & VM_FAULT_ERROR))
		return 0;

	if (fault & VM_FAULT_OOM) {
		/* Kernel mode? Handle exceptions or die: */
		if (!user_mode(regs)) {
<<<<<<< HEAD
			up_read(&current->mm->mmap_sem);
			no_context(regs, error_code, address);
			return 1;
		}
		up_read(&current->mm->mmap_sem);
=======
			no_context(regs, error_code, address);
			return 1;
		}
>>>>>>> upstream/android-13

		/*
		 * We ran out of memory, call the OOM killer, and return the
		 * userspace (which will retry the fault, or kill us if we got
		 * oom-killed):
		 */
		pagefault_out_of_memory();
	} else {
		if (fault & VM_FAULT_SIGBUS)
			do_sigbus(regs, error_code, address);
		else if (fault & VM_FAULT_SIGSEGV)
			bad_area(regs, error_code, address);
		else
			BUG();
	}

	return 1;
}

static inline int access_error(int error_code, struct vm_area_struct *vma)
{
	if (error_code & FAULT_CODE_WRITE) {
		/* write, present and write, not present: */
		if (unlikely(!(vma->vm_flags & VM_WRITE)))
			return 1;
		return 0;
	}

	/* ITLB miss on NX page */
	if (unlikely((error_code & FAULT_CODE_ITLB) &&
		     !(vma->vm_flags & VM_EXEC)))
		return 1;

	/* read, not present: */
<<<<<<< HEAD
	if (unlikely(!(vma->vm_flags & (VM_READ | VM_EXEC | VM_WRITE))))
=======
	if (unlikely(!vma_is_accessible(vma)))
>>>>>>> upstream/android-13
		return 1;

	return 0;
}

static int fault_in_kernel_space(unsigned long address)
{
	return address >= TASK_SIZE;
}

/*
 * This routine handles page faults.  It determines the address,
 * and the problem, and then passes it off to one of the appropriate
 * routines.
 */
asmlinkage void __kprobes do_page_fault(struct pt_regs *regs,
					unsigned long error_code,
					unsigned long address)
{
	unsigned long vec;
	struct task_struct *tsk;
	struct mm_struct *mm;
	struct vm_area_struct * vma;
	vm_fault_t fault;
<<<<<<< HEAD
	unsigned int flags = FAULT_FLAG_ALLOW_RETRY | FAULT_FLAG_KILLABLE;
=======
	unsigned int flags = FAULT_FLAG_DEFAULT;
>>>>>>> upstream/android-13

	tsk = current;
	mm = tsk->mm;
	vec = lookup_exception_vector();

	/*
	 * We fault-in kernel-space virtual memory on-demand. The
	 * 'reference' page table is init_mm.pgd.
	 *
	 * NOTE! We MUST NOT take any locks for this case. We may
	 * be in an interrupt or a critical region, and should
	 * only copy the information from the master page table,
	 * nothing more.
	 */
	if (unlikely(fault_in_kernel_space(address))) {
		if (vmalloc_fault(address) >= 0)
			return;
<<<<<<< HEAD
		if (notify_page_fault(regs, vec))
=======
		if (kprobe_page_fault(regs, vec))
>>>>>>> upstream/android-13
			return;

		bad_area_nosemaphore(regs, error_code, address);
		return;
	}

<<<<<<< HEAD
	if (unlikely(notify_page_fault(regs, vec)))
=======
	if (unlikely(kprobe_page_fault(regs, vec)))
>>>>>>> upstream/android-13
		return;

	/* Only enable interrupts if they were on before the fault */
	if ((regs->sr & SR_IMASK) != SR_IMASK)
		local_irq_enable();

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);

	/*
	 * If we're in an interrupt, have no user context or are running
	 * with pagefaults disabled then we must not take the fault:
	 */
	if (unlikely(faulthandler_disabled() || !mm)) {
		bad_area_nosemaphore(regs, error_code, address);
		return;
	}

retry:
<<<<<<< HEAD
	down_read(&mm->mmap_sem);
=======
	mmap_read_lock(mm);
>>>>>>> upstream/android-13

	vma = find_vma(mm, address);
	if (unlikely(!vma)) {
		bad_area(regs, error_code, address);
		return;
	}
	if (likely(vma->vm_start <= address))
		goto good_area;
	if (unlikely(!(vma->vm_flags & VM_GROWSDOWN))) {
		bad_area(regs, error_code, address);
		return;
	}
	if (unlikely(expand_stack(vma, address))) {
		bad_area(regs, error_code, address);
		return;
	}

	/*
	 * Ok, we have a good vm_area for this memory access, so
	 * we can handle it..
	 */
good_area:
	if (unlikely(access_error(error_code, vma))) {
		bad_area_access_error(regs, error_code, address);
		return;
	}

	set_thread_fault_code(error_code);

	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;
	if (error_code & FAULT_CODE_WRITE)
		flags |= FAULT_FLAG_WRITE;

	/*
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */
<<<<<<< HEAD
	fault = handle_mm_fault(vma, address, flags);
=======
	fault = handle_mm_fault(vma, address, flags, regs);
>>>>>>> upstream/android-13

	if (unlikely(fault & (VM_FAULT_RETRY | VM_FAULT_ERROR)))
		if (mm_fault_error(regs, error_code, address, fault))
			return;

	if (flags & FAULT_FLAG_ALLOW_RETRY) {
<<<<<<< HEAD
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

			/*
			 * No need to up_read(&mm->mmap_sem) as we would
=======
		if (fault & VM_FAULT_RETRY) {
			flags |= FAULT_FLAG_TRIED;

			/*
			 * No need to mmap_read_unlock(mm) as we would
>>>>>>> upstream/android-13
			 * have already released it in __lock_page_or_retry
			 * in mm/filemap.c.
			 */
			goto retry;
		}
	}

<<<<<<< HEAD
	up_read(&mm->mmap_sem);
=======
	mmap_read_unlock(mm);
>>>>>>> upstream/android-13
}
