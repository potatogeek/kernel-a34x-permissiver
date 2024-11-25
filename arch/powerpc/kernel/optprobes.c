<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Code for Kernel probes Jump optimization.
 *
 * Copyright 2017, Anju T, IBM Corp.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kprobes.h>
#include <linux/jump_label.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <asm/kprobes.h>
#include <asm/ptrace.h>
#include <asm/cacheflush.h>
#include <asm/code-patching.h>
#include <asm/sstep.h>
#include <asm/ppc-opcode.h>
<<<<<<< HEAD

#define TMPL_CALL_HDLR_IDX	\
	(optprobe_template_call_handler - optprobe_template_entry)
#define TMPL_EMULATE_IDX	\
	(optprobe_template_call_emulate - optprobe_template_entry)
#define TMPL_RET_IDX		\
	(optprobe_template_ret - optprobe_template_entry)
#define TMPL_OP_IDX		\
	(optprobe_template_op_address - optprobe_template_entry)
#define TMPL_INSN_IDX		\
	(optprobe_template_insn - optprobe_template_entry)
#define TMPL_END_IDX		\
	(optprobe_template_end - optprobe_template_entry)

DEFINE_INSN_CACHE_OPS(ppc_optinsn);

static bool insn_page_in_use;

static void *__ppc_alloc_insn_page(void)
=======
#include <asm/inst.h>

#define TMPL_CALL_HDLR_IDX	(optprobe_template_call_handler - optprobe_template_entry)
#define TMPL_EMULATE_IDX	(optprobe_template_call_emulate - optprobe_template_entry)
#define TMPL_RET_IDX		(optprobe_template_ret - optprobe_template_entry)
#define TMPL_OP_IDX		(optprobe_template_op_address - optprobe_template_entry)
#define TMPL_INSN_IDX		(optprobe_template_insn - optprobe_template_entry)
#define TMPL_END_IDX		(optprobe_template_end - optprobe_template_entry)

static bool insn_page_in_use;

void *alloc_optinsn_page(void)
>>>>>>> upstream/android-13
{
	if (insn_page_in_use)
		return NULL;
	insn_page_in_use = true;
	return &optinsn_slot;
}

<<<<<<< HEAD
static void __ppc_free_insn_page(void *page __maybe_unused)
=======
void free_optinsn_page(void *page)
>>>>>>> upstream/android-13
{
	insn_page_in_use = false;
}

<<<<<<< HEAD
struct kprobe_insn_cache kprobe_ppc_optinsn_slots = {
	.mutex = __MUTEX_INITIALIZER(kprobe_ppc_optinsn_slots.mutex),
	.pages = LIST_HEAD_INIT(kprobe_ppc_optinsn_slots.pages),
	/* insn_size initialized later */
	.alloc = __ppc_alloc_insn_page,
	.free = __ppc_free_insn_page,
	.nr_garbage = 0,
};

=======
>>>>>>> upstream/android-13
/*
 * Check if we can optimize this probe. Returns NIP post-emulation if this can
 * be optimized and 0 otherwise.
 */
static unsigned long can_optimize(struct kprobe *p)
{
	struct pt_regs regs;
	struct instruction_op op;
	unsigned long nip = 0;
<<<<<<< HEAD
=======
	unsigned long addr = (unsigned long)p->addr;
>>>>>>> upstream/android-13

	/*
	 * kprobe placed for kretprobe during boot time
	 * has a 'nop' instruction, which can be emulated.
	 * So further checks can be skipped.
	 */
	if (p->addr == (kprobe_opcode_t *)&kretprobe_trampoline)
<<<<<<< HEAD
		return (unsigned long)p->addr + sizeof(kprobe_opcode_t);
=======
		return addr + sizeof(kprobe_opcode_t);
>>>>>>> upstream/android-13

	/*
	 * We only support optimizing kernel addresses, but not
	 * module addresses.
	 *
	 * FIXME: Optimize kprobes placed in module addresses.
	 */
<<<<<<< HEAD
	if (!is_kernel_addr((unsigned long)p->addr))
		return 0;

	memset(&regs, 0, sizeof(struct pt_regs));
	regs.nip = (unsigned long)p->addr;
=======
	if (!is_kernel_addr(addr))
		return 0;

	memset(&regs, 0, sizeof(struct pt_regs));
	regs.nip = addr;
>>>>>>> upstream/android-13
	regs.trap = 0x0;
	regs.msr = MSR_KERNEL;

	/*
	 * Kprobe placed in conditional branch instructions are
	 * not optimized, as we can't predict the nip prior with
	 * dummy pt_regs and can not ensure that the return branch
	 * from detour buffer falls in the range of address (i.e 32MB).
	 * A branch back from trampoline is set up in the detour buffer
	 * to the nip returned by the analyse_instr() here.
	 *
	 * Ensure that the instruction is not a conditional branch,
	 * and that can be emulated.
	 */
<<<<<<< HEAD
	if (!is_conditional_branch(*p->ainsn.insn) &&
			analyse_instr(&op, &regs, *p->ainsn.insn) == 1) {
=======
	if (!is_conditional_branch(ppc_inst_read(p->ainsn.insn)) &&
	    analyse_instr(&op, &regs, ppc_inst_read(p->ainsn.insn)) == 1) {
>>>>>>> upstream/android-13
		emulate_update_regs(&regs, &op);
		nip = regs.nip;
	}

	return nip;
}

static void optimized_callback(struct optimized_kprobe *op,
			       struct pt_regs *regs)
{
	/* This is possible if op is under delayed unoptimizing */
	if (kprobe_disabled(&op->kp))
		return;

	preempt_disable();

	if (kprobe_running()) {
		kprobes_inc_nmissed_count(&op->kp);
	} else {
		__this_cpu_write(current_kprobe, &op->kp);
<<<<<<< HEAD
		regs->nip = (unsigned long)op->kp.addr;
=======
		regs_set_return_ip(regs, (unsigned long)op->kp.addr);
>>>>>>> upstream/android-13
		get_kprobe_ctlblk()->kprobe_status = KPROBE_HIT_ACTIVE;
		opt_pre_handler(&op->kp, regs);
		__this_cpu_write(current_kprobe, NULL);
	}

	preempt_enable_no_resched();
}
NOKPROBE_SYMBOL(optimized_callback);

void arch_remove_optimized_kprobe(struct optimized_kprobe *op)
{
	if (op->optinsn.insn) {
<<<<<<< HEAD
		free_ppc_optinsn_slot(op->optinsn.insn, 1);
=======
		free_optinsn_slot(op->optinsn.insn, 1);
>>>>>>> upstream/android-13
		op->optinsn.insn = NULL;
	}
}

<<<<<<< HEAD
/*
 * emulate_step() requires insn to be emulated as
 * second parameter. Load register 'r4' with the
 * instruction.
 */
void patch_imm32_load_insns(unsigned int val, kprobe_opcode_t *addr)
{
	/* addis r4,0,(insn)@h */
	patch_instruction(addr, PPC_INST_ADDIS | ___PPC_RT(4) |
			  ((val >> 16) & 0xffff));
	addr++;

	/* ori r4,r4,(insn)@l */
	patch_instruction(addr, PPC_INST_ORI | ___PPC_RA(4) |
			  ___PPC_RS(4) | (val & 0xffff));
=======
static void patch_imm32_load_insns(unsigned long val, int reg, kprobe_opcode_t *addr)
{
	patch_instruction(addr++, ppc_inst(PPC_RAW_LIS(reg, PPC_HI(val))));
	patch_instruction(addr, ppc_inst(PPC_RAW_ORI(reg, reg, PPC_LO(val))));
>>>>>>> upstream/android-13
}

/*
 * Generate instructions to load provided immediate 64-bit value
<<<<<<< HEAD
 * to register 'r3' and patch these instructions at 'addr'.
 */
void patch_imm64_load_insns(unsigned long val, kprobe_opcode_t *addr)
{
	/* lis r3,(op)@highest */
	patch_instruction(addr, PPC_INST_ADDIS | ___PPC_RT(3) |
			  ((val >> 48) & 0xffff));
	addr++;

	/* ori r3,r3,(op)@higher */
	patch_instruction(addr, PPC_INST_ORI | ___PPC_RA(3) |
			  ___PPC_RS(3) | ((val >> 32) & 0xffff));
	addr++;

	/* rldicr r3,r3,32,31 */
	patch_instruction(addr, PPC_INST_RLDICR | ___PPC_RA(3) |
			  ___PPC_RS(3) | __PPC_SH64(32) | __PPC_ME64(31));
	addr++;

	/* oris r3,r3,(op)@h */
	patch_instruction(addr, PPC_INST_ORIS | ___PPC_RA(3) |
			  ___PPC_RS(3) | ((val >> 16) & 0xffff));
	addr++;

	/* ori r3,r3,(op)@l */
	patch_instruction(addr, PPC_INST_ORI | ___PPC_RA(3) |
			  ___PPC_RS(3) | (val & 0xffff));
=======
 * to register 'reg' and patch these instructions at 'addr'.
 */
static void patch_imm64_load_insns(unsigned long long val, int reg, kprobe_opcode_t *addr)
{
	patch_instruction(addr++, ppc_inst(PPC_RAW_LIS(reg, PPC_HIGHEST(val))));
	patch_instruction(addr++, ppc_inst(PPC_RAW_ORI(reg, reg, PPC_HIGHER(val))));
	patch_instruction(addr++, ppc_inst(PPC_RAW_SLDI(reg, reg, 32)));
	patch_instruction(addr++, ppc_inst(PPC_RAW_ORIS(reg, reg, PPC_HI(val))));
	patch_instruction(addr, ppc_inst(PPC_RAW_ORI(reg, reg, PPC_LO(val))));
}

static void patch_imm_load_insns(unsigned long val, int reg, kprobe_opcode_t *addr)
{
	if (IS_ENABLED(CONFIG_PPC64))
		patch_imm64_load_insns(val, reg, addr);
	else
		patch_imm32_load_insns(val, reg, addr);
>>>>>>> upstream/android-13
}

int arch_prepare_optimized_kprobe(struct optimized_kprobe *op, struct kprobe *p)
{
<<<<<<< HEAD
	kprobe_opcode_t *buff, branch_op_callback, branch_emulate_step;
	kprobe_opcode_t *op_callback_addr, *emulate_step_addr;
=======
	struct ppc_inst branch_op_callback, branch_emulate_step, temp;
	unsigned long op_callback_addr, emulate_step_addr;
	kprobe_opcode_t *buff;
>>>>>>> upstream/android-13
	long b_offset;
	unsigned long nip, size;
	int rc, i;

<<<<<<< HEAD
	kprobe_ppc_optinsn_slots.insn_size = MAX_OPTINSN_SIZE;

=======
>>>>>>> upstream/android-13
	nip = can_optimize(p);
	if (!nip)
		return -EILSEQ;

	/* Allocate instruction slot for detour buffer */
<<<<<<< HEAD
	buff = get_ppc_optinsn_slot();
=======
	buff = get_optinsn_slot();
>>>>>>> upstream/android-13
	if (!buff)
		return -ENOMEM;

	/*
	 * OPTPROBE uses 'b' instruction to branch to optinsn.insn.
	 *
	 * The target address has to be relatively nearby, to permit use
	 * of branch instruction in powerpc, because the address is specified
	 * in an immediate field in the instruction opcode itself, ie 24 bits
	 * in the opcode specify the address. Therefore the address should
	 * be within 32MB on either side of the current instruction.
	 */
	b_offset = (unsigned long)buff - (unsigned long)p->addr;
	if (!is_offset_in_branch_range(b_offset))
		goto error;

	/* Check if the return address is also within 32MB range */
<<<<<<< HEAD
	b_offset = (unsigned long)(buff + TMPL_RET_IDX) -
			(unsigned long)nip;
=======
	b_offset = (unsigned long)(buff + TMPL_RET_IDX) - nip;
>>>>>>> upstream/android-13
	if (!is_offset_in_branch_range(b_offset))
		goto error;

	/* Setup template */
	/* We can optimize this via patch_instruction_window later */
	size = (TMPL_END_IDX * sizeof(kprobe_opcode_t)) / sizeof(int);
	pr_devel("Copying template to %p, size %lu\n", buff, size);
	for (i = 0; i < size; i++) {
<<<<<<< HEAD
		rc = patch_instruction(buff + i, *(optprobe_template_entry + i));
=======
		rc = patch_instruction(buff + i, ppc_inst(*(optprobe_template_entry + i)));
>>>>>>> upstream/android-13
		if (rc < 0)
			goto error;
	}

	/*
	 * Fixup the template with instructions to:
	 * 1. load the address of the actual probepoint
	 */
<<<<<<< HEAD
	patch_imm64_load_insns((unsigned long)op, buff + TMPL_OP_IDX);
=======
	patch_imm_load_insns((unsigned long)op, 3, buff + TMPL_OP_IDX);
>>>>>>> upstream/android-13

	/*
	 * 2. branch to optimized_callback() and emulate_step()
	 */
<<<<<<< HEAD
	op_callback_addr = (kprobe_opcode_t *)ppc_kallsyms_lookup_name("optimized_callback");
	emulate_step_addr = (kprobe_opcode_t *)ppc_kallsyms_lookup_name("emulate_step");
=======
	op_callback_addr = ppc_kallsyms_lookup_name("optimized_callback");
	emulate_step_addr = ppc_kallsyms_lookup_name("emulate_step");
>>>>>>> upstream/android-13
	if (!op_callback_addr || !emulate_step_addr) {
		WARN(1, "Unable to lookup optimized_callback()/emulate_step()\n");
		goto error;
	}

<<<<<<< HEAD
	branch_op_callback = create_branch((unsigned int *)buff + TMPL_CALL_HDLR_IDX,
				(unsigned long)op_callback_addr,
				BRANCH_SET_LINK);

	branch_emulate_step = create_branch((unsigned int *)buff + TMPL_EMULATE_IDX,
				(unsigned long)emulate_step_addr,
				BRANCH_SET_LINK);

	if (!branch_op_callback || !branch_emulate_step)
=======
	rc = create_branch(&branch_op_callback, buff + TMPL_CALL_HDLR_IDX,
			   op_callback_addr, BRANCH_SET_LINK);

	rc |= create_branch(&branch_emulate_step, buff + TMPL_EMULATE_IDX,
			    emulate_step_addr, BRANCH_SET_LINK);

	if (rc)
>>>>>>> upstream/android-13
		goto error;

	patch_instruction(buff + TMPL_CALL_HDLR_IDX, branch_op_callback);
	patch_instruction(buff + TMPL_EMULATE_IDX, branch_emulate_step);

	/*
	 * 3. load instruction to be emulated into relevant register, and
	 */
<<<<<<< HEAD
	patch_imm32_load_insns(*p->ainsn.insn, buff + TMPL_INSN_IDX);
=======
	if (IS_ENABLED(CONFIG_PPC64)) {
		temp = ppc_inst_read(p->ainsn.insn);
		patch_imm_load_insns(ppc_inst_as_ulong(temp), 4, buff + TMPL_INSN_IDX);
	} else {
		patch_imm_load_insns((unsigned long)p->ainsn.insn, 4, buff + TMPL_INSN_IDX);
	}
>>>>>>> upstream/android-13

	/*
	 * 4. branch back from trampoline
	 */
<<<<<<< HEAD
	patch_branch(buff + TMPL_RET_IDX, (unsigned long)nip, 0);

	flush_icache_range((unsigned long)buff,
			   (unsigned long)(&buff[TMPL_END_IDX]));
=======
	patch_branch(buff + TMPL_RET_IDX, nip, 0);

	flush_icache_range((unsigned long)buff, (unsigned long)(&buff[TMPL_END_IDX]));
>>>>>>> upstream/android-13

	op->optinsn.insn = buff;

	return 0;

error:
<<<<<<< HEAD
	free_ppc_optinsn_slot(buff, 0);
=======
	free_optinsn_slot(buff, 0);
>>>>>>> upstream/android-13
	return -ERANGE;

}

int arch_prepared_optinsn(struct arch_optimized_insn *optinsn)
{
	return optinsn->insn != NULL;
}

/*
 * On powerpc, Optprobes always replaces one instruction (4 bytes
 * aligned and 4 bytes long). It is impossible to encounter another
 * kprobe in this address range. So always return 0.
 */
int arch_check_optimized_kprobe(struct optimized_kprobe *op)
{
	return 0;
}

void arch_optimize_kprobes(struct list_head *oplist)
{
<<<<<<< HEAD
=======
	struct ppc_inst instr;
>>>>>>> upstream/android-13
	struct optimized_kprobe *op;
	struct optimized_kprobe *tmp;

	list_for_each_entry_safe(op, tmp, oplist, list) {
		/*
		 * Backup instructions which will be replaced
		 * by jump address
		 */
<<<<<<< HEAD
		memcpy(op->optinsn.copied_insn, op->kp.addr,
					       RELATIVEJUMP_SIZE);
		patch_instruction(op->kp.addr,
			create_branch((unsigned int *)op->kp.addr,
				      (unsigned long)op->optinsn.insn, 0));
=======
		memcpy(op->optinsn.copied_insn, op->kp.addr, RELATIVEJUMP_SIZE);
		create_branch(&instr, op->kp.addr, (unsigned long)op->optinsn.insn, 0);
		patch_instruction(op->kp.addr, instr);
>>>>>>> upstream/android-13
		list_del_init(&op->list);
	}
}

void arch_unoptimize_kprobe(struct optimized_kprobe *op)
{
	arch_arm_kprobe(&op->kp);
}

<<<<<<< HEAD
void arch_unoptimize_kprobes(struct list_head *oplist,
			     struct list_head *done_list)
=======
void arch_unoptimize_kprobes(struct list_head *oplist, struct list_head *done_list)
>>>>>>> upstream/android-13
{
	struct optimized_kprobe *op;
	struct optimized_kprobe *tmp;

	list_for_each_entry_safe(op, tmp, oplist, list) {
		arch_unoptimize_kprobe(op);
		list_move(&op->list, done_list);
	}
}

<<<<<<< HEAD
int arch_within_optimized_kprobe(struct optimized_kprobe *op,
				 unsigned long addr)
=======
int arch_within_optimized_kprobe(struct optimized_kprobe *op, unsigned long addr)
>>>>>>> upstream/android-13
{
	return ((unsigned long)op->kp.addr <= addr &&
		(unsigned long)op->kp.addr + RELATIVEJUMP_SIZE > addr);
}
