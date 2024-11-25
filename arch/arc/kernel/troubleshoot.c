<<<<<<< HEAD
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
>>>>>>> upstream/android-13
 */

#include <linux/ptrace.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include <linux/sched/mm.h>
#include <linux/sched/debug.h>

#include <asm/arcregs.h>
#include <asm/irqflags.h>

#define ARC_PATH_MAX	256

<<<<<<< HEAD
/*
 * Common routine to print scratch regs (r0-r12) or callee regs (r13-r25)
 *   -Prints 3 regs per line and a CR.
 *   -To continue, callee regs right after scratch, special handling of CR
 */
static noinline void print_reg_file(long *reg_rev, int start_num)
{
	unsigned int i;
	char buf[512];
	int n = 0, len = sizeof(buf);

	for (i = start_num; i < start_num + 13; i++) {
		n += scnprintf(buf + n, len - n, "r%02u: 0x%08lx\t",
			       i, (unsigned long)*reg_rev);

		if (((i + 1) % 3) == 0)
			n += scnprintf(buf + n, len - n, "\n");

		/* because pt_regs has regs reversed: r12..r0, r25..r13 */
		if (is_isa_arcv2() && start_num == 0)
			reg_rev++;
		else
			reg_rev--;
	}

	if (start_num != 0)
		n += scnprintf(buf + n, len - n, "\n\n");

	/* To continue printing callee regs on same line as scratch regs */
	if (start_num == 0)
		pr_info("%s", buf);
	else
		pr_cont("%s\n", buf);
}

static void show_callee_regs(struct callee_regs *cregs)
{
	print_reg_file(&(cregs->r13), 13);
=======
static noinline void print_regs_scratch(struct pt_regs *regs)
{
	pr_cont("BTA: 0x%08lx\n SP: 0x%08lx  FP: 0x%08lx BLK: %pS\n",
		regs->bta, regs->sp, regs->fp, (void *)regs->blink);
	pr_cont("LPS: 0x%08lx\tLPE: 0x%08lx\tLPC: 0x%08lx\n",
		regs->lp_start, regs->lp_end, regs->lp_count);

	pr_info("r00: 0x%08lx\tr01: 0x%08lx\tr02: 0x%08lx\n"	\
		"r03: 0x%08lx\tr04: 0x%08lx\tr05: 0x%08lx\n"	\
		"r06: 0x%08lx\tr07: 0x%08lx\tr08: 0x%08lx\n"	\
		"r09: 0x%08lx\tr10: 0x%08lx\tr11: 0x%08lx\n"	\
		"r12: 0x%08lx\t",
		regs->r0, regs->r1, regs->r2,
		regs->r3, regs->r4, regs->r5,
		regs->r6, regs->r7, regs->r8,
		regs->r9, regs->r10, regs->r11,
		regs->r12);
}

static void print_regs_callee(struct callee_regs *regs)
{
	pr_cont("r13: 0x%08lx\tr14: 0x%08lx\n"			\
		"r15: 0x%08lx\tr16: 0x%08lx\tr17: 0x%08lx\n"	\
		"r18: 0x%08lx\tr19: 0x%08lx\tr20: 0x%08lx\n"	\
		"r21: 0x%08lx\tr22: 0x%08lx\tr23: 0x%08lx\n"	\
		"r24: 0x%08lx\tr25: 0x%08lx\n",
		regs->r13, regs->r14,
		regs->r15, regs->r16, regs->r17,
		regs->r18, regs->r19, regs->r20,
		regs->r21, regs->r22, regs->r23,
		regs->r24, regs->r25);
>>>>>>> upstream/android-13
}

static void print_task_path_n_nm(struct task_struct *tsk)
{
	char *path_nm = NULL;
	struct mm_struct *mm;
	struct file *exe_file;
	char buf[ARC_PATH_MAX];

	mm = get_task_mm(tsk);
	if (!mm)
		goto done;

	exe_file = get_mm_exe_file(mm);
	mmput(mm);

	if (exe_file) {
		path_nm = file_path(exe_file, buf, ARC_PATH_MAX-1);
		fput(exe_file);
	}

done:
	pr_info("Path: %s\n", !IS_ERR(path_nm) ? path_nm : "?");
}

static void show_faulting_vma(unsigned long address)
{
	struct vm_area_struct *vma;
	struct mm_struct *active_mm = current->active_mm;

	/* can't use print_vma_addr() yet as it doesn't check for
	 * non-inclusive vma
	 */
<<<<<<< HEAD
	down_read(&active_mm->mmap_sem);
	vma = find_vma(active_mm, address);

	/* check against the find_vma( ) behaviour which returns the next VMA
	 * if the container VMA is not found
	 */
	if (vma && (vma->vm_start <= address)) {
=======
	mmap_read_lock(active_mm);
	vma = vma_lookup(active_mm, address);

	/* Lookup the vma at the address and report if the container VMA is not
	 * found
	 */
	if (vma) {
>>>>>>> upstream/android-13
		char buf[ARC_PATH_MAX];
		char *nm = "?";

		if (vma->vm_file) {
			nm = file_path(vma->vm_file, buf, ARC_PATH_MAX-1);
			if (IS_ERR(nm))
				nm = "?";
		}
<<<<<<< HEAD
		pr_info("    @off 0x%lx in [%s]\n"
			"    VMA: 0x%08lx to 0x%08lx\n",
=======
		pr_info("  @off 0x%lx in [%s]  VMA: 0x%08lx to 0x%08lx\n",
>>>>>>> upstream/android-13
			vma->vm_start < TASK_UNMAPPED_BASE ?
				address : address - vma->vm_start,
			nm, vma->vm_start, vma->vm_end);
	} else
		pr_info("    @No matching VMA found\n");

<<<<<<< HEAD
	up_read(&active_mm->mmap_sem);
=======
	mmap_read_unlock(active_mm);
>>>>>>> upstream/android-13
}

static void show_ecr_verbose(struct pt_regs *regs)
{
	unsigned int vec, cause_code;
	unsigned long address;

<<<<<<< HEAD
	pr_info("\n[ECR   ]: 0x%08lx => ", regs->event);

=======
>>>>>>> upstream/android-13
	/* For Data fault, this is data address not instruction addr */
	address = current->thread.fault_address;

	vec = regs->ecr_vec;
	cause_code = regs->ecr_cause;

	/* For DTLB Miss or ProtV, display the memory involved too */
	if (vec == ECR_V_DTLB_MISS) {
<<<<<<< HEAD
		pr_cont("Invalid %s @ 0x%08lx by insn @ 0x%08lx\n",
		       (cause_code == 0x01) ? "Read" :
		       ((cause_code == 0x02) ? "Write" : "EX"),
		       address, regs->ret);
=======
		pr_cont("Invalid %s @ 0x%08lx by insn @ %pS\n",
		       (cause_code == 0x01) ? "Read" :
		       ((cause_code == 0x02) ? "Write" : "EX"),
		       address, (void *)regs->ret);
>>>>>>> upstream/android-13
	} else if (vec == ECR_V_ITLB_MISS) {
		pr_cont("Insn could not be fetched\n");
	} else if (vec == ECR_V_MACH_CHK) {
		pr_cont("Machine Check (%s)\n", (cause_code == 0x0) ?
					"Double Fault" : "Other Fatal Err");

	} else if (vec == ECR_V_PROTV) {
		if (cause_code == ECR_C_PROTV_INST_FETCH)
			pr_cont("Execute from Non-exec Page\n");
<<<<<<< HEAD
		else if (cause_code == ECR_C_PROTV_MISALIG_DATA)
=======
		else if (cause_code == ECR_C_PROTV_MISALIG_DATA &&
		         IS_ENABLED(CONFIG_ISA_ARCOMPACT))
>>>>>>> upstream/android-13
			pr_cont("Misaligned r/w from 0x%08lx\n", address);
		else
			pr_cont("%s access not allowed on page\n",
				(cause_code == 0x01) ? "Read" :
				((cause_code == 0x02) ? "Write" : "EX"));
	} else if (vec == ECR_V_INSN_ERR) {
		pr_cont("Illegal Insn\n");
#ifdef CONFIG_ISA_ARCV2
	} else if (vec == ECR_V_MEM_ERR) {
		if (cause_code == 0x00)
			pr_cont("Bus Error from Insn Mem\n");
		else if (cause_code == 0x10)
			pr_cont("Bus Error from Data Mem\n");
		else
			pr_cont("Bus Error, check PRM\n");
<<<<<<< HEAD
=======
	} else if (vec == ECR_V_MISALIGN) {
		pr_cont("Misaligned r/w from 0x%08lx\n", address);
>>>>>>> upstream/android-13
#endif
	} else if (vec == ECR_V_TRAP) {
		if (regs->ecr_param == 5)
			pr_cont("gcc generated __builtin_trap\n");
	} else {
		pr_cont("Check Programmer's Manual\n");
	}
}

/************************************************************************
 *  API called by rest of kernel
 ***********************************************************************/

void show_regs(struct pt_regs *regs)
{
	struct task_struct *tsk = current;
<<<<<<< HEAD
	struct callee_regs *cregs;
=======
	struct callee_regs *cregs = (struct callee_regs *)tsk->thread.callee_reg;
>>>>>>> upstream/android-13

	/*
	 * generic code calls us with preemption disabled, but some calls
	 * here could sleep, so re-enable to avoid lockdep splat
	 */
	preempt_enable();

	print_task_path_n_nm(tsk);
	show_regs_print_info(KERN_INFO);

	show_ecr_verbose(regs);

<<<<<<< HEAD
	pr_info("[EFA   ]: 0x%08lx\n[BLINK ]: %pS\n[ERET  ]: %pS\n",
		current->thread.fault_address,
		(void *)regs->blink, (void *)regs->ret);

	if (user_mode(regs))
		show_faulting_vma(regs->ret); /* faulting code, not data */

	pr_info("[STAT32]: 0x%08lx", regs->status32);
=======
	if (user_mode(regs))
		show_faulting_vma(regs->ret); /* faulting code, not data */

	pr_info("ECR: 0x%08lx EFA: 0x%08lx ERET: 0x%08lx\nSTAT: 0x%08lx",
		regs->event, current->thread.fault_address, regs->ret,
		regs->status32);
>>>>>>> upstream/android-13

#define STS_BIT(r, bit)	r->status32 & STATUS_##bit##_MASK ? #bit" " : ""

#ifdef CONFIG_ISA_ARCOMPACT
<<<<<<< HEAD
	pr_cont(" : %2s%2s%2s%2s%2s%2s%2s\n",
=======
	pr_cont(" [%2s%2s%2s%2s%2s%2s%2s]",
>>>>>>> upstream/android-13
			(regs->status32 & STATUS_U_MASK) ? "U " : "K ",
			STS_BIT(regs, DE), STS_BIT(regs, AE),
			STS_BIT(regs, A2), STS_BIT(regs, A1),
			STS_BIT(regs, E2), STS_BIT(regs, E1));
#else
<<<<<<< HEAD
	pr_cont(" : %2s%2s%2s%2s\n",
=======
	pr_cont(" [%2s%2s%2s%2s]   ",
>>>>>>> upstream/android-13
			STS_BIT(regs, IE),
			(regs->status32 & STATUS_U_MASK) ? "U " : "K ",
			STS_BIT(regs, DE), STS_BIT(regs, AE));
#endif
<<<<<<< HEAD
	pr_info("BTA: 0x%08lx\t SP: 0x%08lx\t FP: 0x%08lx\n",
		regs->bta, regs->sp, regs->fp);
	pr_info("LPS: 0x%08lx\tLPE: 0x%08lx\tLPC: 0x%08lx\n",
	       regs->lp_start, regs->lp_end, regs->lp_count);

	/* print regs->r0 thru regs->r12
	 * Sequential printing was generating horrible code
	 */
	print_reg_file(&(regs->r0), 0);

	/* If Callee regs were saved, display them too */
	cregs = (struct callee_regs *)current->thread.callee_reg;
	if (cregs)
		show_callee_regs(cregs);
=======

	print_regs_scratch(regs);
	if (cregs)
		print_regs_callee(cregs);
>>>>>>> upstream/android-13

	preempt_disable();
}

void show_kernel_fault_diag(const char *str, struct pt_regs *regs,
			    unsigned long address)
{
	current->thread.fault_address = address;

	/* Show fault description */
	pr_info("\n%s\n", str);

	/* Caller and Callee regs */
	show_regs(regs);

	/* Show stack trace if this Fatality happened in kernel mode */
	if (!user_mode(regs))
<<<<<<< HEAD
		show_stacktrace(current, regs);
=======
		show_stacktrace(current, regs, KERN_DEFAULT);
>>>>>>> upstream/android-13
}
