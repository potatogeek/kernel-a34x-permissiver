<<<<<<< HEAD
/*
 *  Kernel Probes Jump Optimization (Optprobes)
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Kernel Probes Jump Optimization (Optprobes)
 *
>>>>>>> upstream/android-13
 * Copyright (C) IBM Corporation, 2002, 2004
 * Copyright (C) Hitachi Ltd., 2012
 */
#include <linux/kprobes.h>
<<<<<<< HEAD
=======
#include <linux/perf_event.h>
>>>>>>> upstream/android-13
#include <linux/ptrace.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/hardirq.h>
#include <linux/preempt.h>
#include <linux/extable.h>
#include <linux/kdebug.h>
#include <linux/kallsyms.h>
#include <linux/ftrace.h>
<<<<<<< HEAD
#include <linux/frame.h>
=======
#include <linux/objtool.h>
#include <linux/pgtable.h>
#include <linux/static_call.h>
>>>>>>> upstream/android-13

#include <asm/text-patching.h>
#include <asm/cacheflush.h>
#include <asm/desc.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <linux/uaccess.h>
#include <asm/alternative.h>
#include <asm/insn.h>
#include <asm/debugreg.h>
#include <asm/set_memory.h>
#include <asm/sections.h>
#include <asm/nospec-branch.h>

#include "common.h"

unsigned long __recover_optprobed_insn(kprobe_opcode_t *buf, unsigned long addr)
{
	struct optimized_kprobe *op;
	struct kprobe *kp;
	long offs;
	int i;

<<<<<<< HEAD
	for (i = 0; i < RELATIVEJUMP_SIZE; i++) {
=======
	for (i = 0; i < JMP32_INSN_SIZE; i++) {
>>>>>>> upstream/android-13
		kp = get_kprobe((void *)addr - i);
		/* This function only handles jump-optimized kprobe */
		if (kp && kprobe_optimized(kp)) {
			op = container_of(kp, struct optimized_kprobe, kp);
			/* If op->list is not empty, op is under optimizing */
			if (list_empty(&op->list))
				goto found;
		}
	}

	return addr;
found:
	/*
	 * If the kprobe can be optimized, original bytes which can be
	 * overwritten by jump destination address. In this case, original
	 * bytes must be recovered from op->optinsn.copied_insn buffer.
	 */
<<<<<<< HEAD
	if (probe_kernel_read(buf, (void *)addr,
=======
	if (copy_from_kernel_nofault(buf, (void *)addr,
>>>>>>> upstream/android-13
		MAX_INSN_SIZE * sizeof(kprobe_opcode_t)))
		return 0UL;

	if (addr == (unsigned long)kp->addr) {
		buf[0] = kp->opcode;
<<<<<<< HEAD
		memcpy(buf + 1, op->optinsn.copied_insn, RELATIVE_ADDR_SIZE);
	} else {
		offs = addr - (unsigned long)kp->addr - 1;
		memcpy(buf, op->optinsn.copied_insn + offs, RELATIVE_ADDR_SIZE - offs);
=======
		memcpy(buf + 1, op->optinsn.copied_insn, DISP32_SIZE);
	} else {
		offs = addr - (unsigned long)kp->addr - 1;
		memcpy(buf, op->optinsn.copied_insn + offs, DISP32_SIZE - offs);
>>>>>>> upstream/android-13
	}

	return (unsigned long)buf;
}

<<<<<<< HEAD
=======
static void synthesize_clac(kprobe_opcode_t *addr)
{
	/*
	 * Can't be static_cpu_has() due to how objtool treats this feature bit.
	 * This isn't a fast path anyway.
	 */
	if (!boot_cpu_has(X86_FEATURE_SMAP))
		return;

	/* Replace the NOP3 with CLAC */
	addr[0] = 0x0f;
	addr[1] = 0x01;
	addr[2] = 0xca;
}

>>>>>>> upstream/android-13
/* Insert a move instruction which sets a pointer to eax/rdi (1st arg). */
static void synthesize_set_arg1(kprobe_opcode_t *addr, unsigned long val)
{
#ifdef CONFIG_X86_64
	*addr++ = 0x48;
	*addr++ = 0xbf;
#else
	*addr++ = 0xb8;
#endif
	*(unsigned long *)addr = val;
}

asm (
<<<<<<< HEAD
=======
			".pushsection .rodata\n"
>>>>>>> upstream/android-13
			"optprobe_template_func:\n"
			".global optprobe_template_entry\n"
			"optprobe_template_entry:\n"
#ifdef CONFIG_X86_64
			/* We don't bother saving the ss register */
			"	pushq %rsp\n"
			"	pushfq\n"
<<<<<<< HEAD
=======
			".global optprobe_template_clac\n"
			"optprobe_template_clac:\n"
			ASM_NOP3
>>>>>>> upstream/android-13
			SAVE_REGS_STRING
			"	movq %rsp, %rsi\n"
			".global optprobe_template_val\n"
			"optprobe_template_val:\n"
			ASM_NOP5
			ASM_NOP5
			".global optprobe_template_call\n"
			"optprobe_template_call:\n"
			ASM_NOP5
			/* Move flags to rsp */
<<<<<<< HEAD
			"	movq 144(%rsp), %rdx\n"
			"	movq %rdx, 152(%rsp)\n"
=======
			"	movq 18*8(%rsp), %rdx\n"
			"	movq %rdx, 19*8(%rsp)\n"
>>>>>>> upstream/android-13
			RESTORE_REGS_STRING
			/* Skip flags entry */
			"	addq $8, %rsp\n"
			"	popfq\n"
#else /* CONFIG_X86_32 */
<<<<<<< HEAD
			"	pushf\n"
=======
			"	pushl %esp\n"
			"	pushfl\n"
			".global optprobe_template_clac\n"
			"optprobe_template_clac:\n"
			ASM_NOP3
>>>>>>> upstream/android-13
			SAVE_REGS_STRING
			"	movl %esp, %edx\n"
			".global optprobe_template_val\n"
			"optprobe_template_val:\n"
			ASM_NOP5
			".global optprobe_template_call\n"
			"optprobe_template_call:\n"
			ASM_NOP5
<<<<<<< HEAD
			RESTORE_REGS_STRING
			"	addl $4, %esp\n"	/* skip cs */
			"	popf\n"
#endif
			".global optprobe_template_end\n"
			"optprobe_template_end:\n"
			".type optprobe_template_func, @function\n"
			".size optprobe_template_func, .-optprobe_template_func\n");

void optprobe_template_func(void);
STACK_FRAME_NON_STANDARD(optprobe_template_func);
NOKPROBE_SYMBOL(optprobe_template_func);
NOKPROBE_SYMBOL(optprobe_template_entry);
NOKPROBE_SYMBOL(optprobe_template_val);
NOKPROBE_SYMBOL(optprobe_template_call);
NOKPROBE_SYMBOL(optprobe_template_end);

=======
			/* Move flags into esp */
			"	movl 14*4(%esp), %edx\n"
			"	movl %edx, 15*4(%esp)\n"
			RESTORE_REGS_STRING
			/* Skip flags entry */
			"	addl $4, %esp\n"
			"	popfl\n"
#endif
			".global optprobe_template_end\n"
			"optprobe_template_end:\n"
			".popsection\n");

void optprobe_template_func(void);
STACK_FRAME_NON_STANDARD(optprobe_template_func);

#define TMPL_CLAC_IDX \
	((long)optprobe_template_clac - (long)optprobe_template_entry)
>>>>>>> upstream/android-13
#define TMPL_MOVE_IDX \
	((long)optprobe_template_val - (long)optprobe_template_entry)
#define TMPL_CALL_IDX \
	((long)optprobe_template_call - (long)optprobe_template_entry)
#define TMPL_END_IDX \
	((long)optprobe_template_end - (long)optprobe_template_entry)

<<<<<<< HEAD
#define INT3_SIZE sizeof(kprobe_opcode_t)

=======
>>>>>>> upstream/android-13
/* Optimized kprobe call back function: called from optinsn */
static void
optimized_callback(struct optimized_kprobe *op, struct pt_regs *regs)
{
	/* This is possible if op is under delayed unoptimizing */
	if (kprobe_disabled(&op->kp))
		return;

	preempt_disable();
	if (kprobe_running()) {
		kprobes_inc_nmissed_count(&op->kp);
	} else {
		struct kprobe_ctlblk *kcb = get_kprobe_ctlblk();
		/* Save skipped registers */
<<<<<<< HEAD
#ifdef CONFIG_X86_64
		regs->cs = __KERNEL_CS;
#else
		regs->cs = __KERNEL_CS | get_kernel_rpl();
		regs->gs = 0;
#endif
		regs->ip = (unsigned long)op->kp.addr + INT3_SIZE;
=======
		regs->cs = __KERNEL_CS;
#ifdef CONFIG_X86_32
		regs->gs = 0;
#endif
		regs->ip = (unsigned long)op->kp.addr + INT3_INSN_SIZE;
>>>>>>> upstream/android-13
		regs->orig_ax = ~0UL;

		__this_cpu_write(current_kprobe, &op->kp);
		kcb->kprobe_status = KPROBE_HIT_ACTIVE;
		opt_pre_handler(&op->kp, regs);
		__this_cpu_write(current_kprobe, NULL);
	}
	preempt_enable();
}
NOKPROBE_SYMBOL(optimized_callback);

static int copy_optimized_instructions(u8 *dest, u8 *src, u8 *real)
{
	struct insn insn;
	int len = 0, ret;

<<<<<<< HEAD
	while (len < RELATIVEJUMP_SIZE) {
=======
	while (len < JMP32_INSN_SIZE) {
>>>>>>> upstream/android-13
		ret = __copy_instruction(dest + len, src + len, real + len, &insn);
		if (!ret || !can_boost(&insn, src + len))
			return -EINVAL;
		len += ret;
	}
	/* Check whether the address range is reserved */
	if (ftrace_text_reserved(src, src + len - 1) ||
	    alternatives_text_reserved(src, src + len - 1) ||
<<<<<<< HEAD
	    jump_label_text_reserved(src, src + len - 1))
=======
	    jump_label_text_reserved(src, src + len - 1) ||
	    static_call_text_reserved(src, src + len - 1))
>>>>>>> upstream/android-13
		return -EBUSY;

	return len;
}

/* Check whether insn is indirect jump */
static int __insn_is_indirect_jump(struct insn *insn)
{
	return ((insn->opcode.bytes[0] == 0xff &&
		(X86_MODRM_REG(insn->modrm.value) & 6) == 4) || /* Jump */
		insn->opcode.bytes[0] == 0xea);	/* Segment based jump */
}

/* Check whether insn jumps into specified address range */
static int insn_jump_into_range(struct insn *insn, unsigned long start, int len)
{
	unsigned long target = 0;

	switch (insn->opcode.bytes[0]) {
	case 0xe0:	/* loopne */
	case 0xe1:	/* loope */
	case 0xe2:	/* loop */
	case 0xe3:	/* jcxz */
	case 0xe9:	/* near relative jump */
	case 0xeb:	/* short relative jump */
		break;
	case 0x0f:
		if ((insn->opcode.bytes[1] & 0xf0) == 0x80) /* jcc near */
			break;
		return 0;
	default:
		if ((insn->opcode.bytes[0] & 0xf0) == 0x70) /* jcc short */
			break;
		return 0;
	}
	target = (unsigned long)insn->next_byte + insn->immediate.value;

	return (start <= target && target <= start + len);
}

static int insn_is_indirect_jump(struct insn *insn)
{
	int ret = __insn_is_indirect_jump(insn);

#ifdef CONFIG_RETPOLINE
	/*
	 * Jump to x86_indirect_thunk_* is treated as an indirect jump.
	 * Note that even with CONFIG_RETPOLINE=y, the kernel compiled with
	 * older gcc may use indirect jump. So we add this check instead of
	 * replace indirect-jump check.
	 */
	if (!ret)
		ret = insn_jump_into_range(insn,
				(unsigned long)__indirect_thunk_start,
				(unsigned long)__indirect_thunk_end -
				(unsigned long)__indirect_thunk_start);
#endif
	return ret;
}

<<<<<<< HEAD
=======
static bool is_padding_int3(unsigned long addr, unsigned long eaddr)
{
	unsigned char ops;

	for (; addr < eaddr; addr++) {
		if (get_kernel_nofault(ops, (void *)addr) < 0 ||
		    ops != INT3_INSN_OPCODE)
			return false;
	}

	return true;
}

>>>>>>> upstream/android-13
/* Decode whole function to ensure any instructions don't jump into target */
static int can_optimize(unsigned long paddr)
{
	unsigned long addr, size = 0, offset = 0;
	struct insn insn;
	kprobe_opcode_t buf[MAX_INSN_SIZE];

	/* Lookup symbol including addr */
	if (!kallsyms_lookup_size_offset(paddr, &size, &offset))
		return 0;

	/*
	 * Do not optimize in the entry code due to the unstable
	 * stack handling and registers setup.
	 */
	if (((paddr >= (unsigned long)__entry_text_start) &&
<<<<<<< HEAD
	     (paddr <  (unsigned long)__entry_text_end)) ||
	    ((paddr >= (unsigned long)__irqentry_text_start) &&
	     (paddr <  (unsigned long)__irqentry_text_end)))
		return 0;

	/* Check there is enough space for a relative jump. */
	if (size - offset < RELATIVEJUMP_SIZE)
=======
	     (paddr <  (unsigned long)__entry_text_end)))
		return 0;

	/* Check there is enough space for a relative jump. */
	if (size - offset < JMP32_INSN_SIZE)
>>>>>>> upstream/android-13
		return 0;

	/* Decode instructions */
	addr = paddr - offset;
	while (addr < paddr - offset + size) { /* Decode until function end */
		unsigned long recovered_insn;
<<<<<<< HEAD
=======
		int ret;

>>>>>>> upstream/android-13
		if (search_exception_tables(addr))
			/*
			 * Since some fixup code will jumps into this function,
			 * we can't optimize kprobe in this function.
			 */
			return 0;
		recovered_insn = recover_probed_instruction(buf, addr);
		if (!recovered_insn)
			return 0;
<<<<<<< HEAD
		kernel_insn_init(&insn, (void *)recovered_insn, MAX_INSN_SIZE);
		insn_get_length(&insn);
		/* Another subsystem puts a breakpoint */
		if (insn.opcode.bytes[0] == BREAKPOINT_INSTRUCTION)
			return 0;
=======

		ret = insn_decode_kernel(&insn, (void *)recovered_insn);
		if (ret < 0)
			return 0;

		/*
		 * In the case of detecting unknown breakpoint, this could be
		 * a padding INT3 between functions. Let's check that all the
		 * rest of the bytes are also INT3.
		 */
		if (insn.opcode.bytes[0] == INT3_INSN_OPCODE)
			return is_padding_int3(addr, paddr - offset + size) ? 1 : 0;

>>>>>>> upstream/android-13
		/* Recover address */
		insn.kaddr = (void *)addr;
		insn.next_byte = (void *)(addr + insn.length);
		/* Check any instructions don't jump into target */
		if (insn_is_indirect_jump(&insn) ||
<<<<<<< HEAD
		    insn_jump_into_range(&insn, paddr + INT3_SIZE,
					 RELATIVE_ADDR_SIZE))
=======
		    insn_jump_into_range(&insn, paddr + INT3_INSN_SIZE,
					 DISP32_SIZE))
>>>>>>> upstream/android-13
			return 0;
		addr += insn.length;
	}

	return 1;
}

/* Check optimized_kprobe can actually be optimized. */
int arch_check_optimized_kprobe(struct optimized_kprobe *op)
{
	int i;
	struct kprobe *p;

	for (i = 1; i < op->optinsn.size; i++) {
		p = get_kprobe(op->kp.addr + i);
		if (p && !kprobe_disabled(p))
			return -EEXIST;
	}

	return 0;
}

/* Check the addr is within the optimized instructions. */
int arch_within_optimized_kprobe(struct optimized_kprobe *op,
				 unsigned long addr)
{
	return ((unsigned long)op->kp.addr <= addr &&
		(unsigned long)op->kp.addr + op->optinsn.size > addr);
}

/* Free optimized instruction slot */
static
void __arch_remove_optimized_kprobe(struct optimized_kprobe *op, int dirty)
{
<<<<<<< HEAD
	if (op->optinsn.insn) {
		free_optinsn_slot(op->optinsn.insn, dirty);
=======
	u8 *slot = op->optinsn.insn;
	if (slot) {
		int len = TMPL_END_IDX + op->optinsn.size + JMP32_INSN_SIZE;

		/* Record the perf event before freeing the slot */
		if (dirty)
			perf_event_text_poke(slot, slot, len, NULL, 0);

		free_optinsn_slot(slot, dirty);
>>>>>>> upstream/android-13
		op->optinsn.insn = NULL;
		op->optinsn.size = 0;
	}
}

void arch_remove_optimized_kprobe(struct optimized_kprobe *op)
{
	__arch_remove_optimized_kprobe(op, 1);
}

/*
 * Copy replacing target instructions
 * Target instructions MUST be relocatable (checked inside)
 * This is called when new aggr(opt)probe is allocated or reused.
 */
int arch_prepare_optimized_kprobe(struct optimized_kprobe *op,
				  struct kprobe *__unused)
{
	u8 *buf = NULL, *slot;
	int ret, len;
	long rel;

	if (!can_optimize((unsigned long)op->kp.addr))
		return -EILSEQ;

	buf = kzalloc(MAX_OPTINSN_SIZE, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	op->optinsn.insn = slot = get_optinsn_slot();
	if (!slot) {
		ret = -ENOMEM;
		goto out;
	}

	/*
	 * Verify if the address gap is in 2GB range, because this uses
	 * a relative jump.
	 */
<<<<<<< HEAD
	rel = (long)slot - (long)op->kp.addr + RELATIVEJUMP_SIZE;
=======
	rel = (long)slot - (long)op->kp.addr + JMP32_INSN_SIZE;
>>>>>>> upstream/android-13
	if (abs(rel) > 0x7fffffff) {
		ret = -ERANGE;
		goto err;
	}

	/* Copy arch-dep-instance from template */
	memcpy(buf, optprobe_template_entry, TMPL_END_IDX);

	/* Copy instructions into the out-of-line buffer */
	ret = copy_optimized_instructions(buf + TMPL_END_IDX, op->kp.addr,
					  slot + TMPL_END_IDX);
	if (ret < 0)
		goto err;
	op->optinsn.size = ret;
	len = TMPL_END_IDX + op->optinsn.size;

<<<<<<< HEAD
=======
	synthesize_clac(buf + TMPL_CLAC_IDX);

>>>>>>> upstream/android-13
	/* Set probe information */
	synthesize_set_arg1(buf + TMPL_MOVE_IDX, (unsigned long)op);

	/* Set probe function call */
	synthesize_relcall(buf + TMPL_CALL_IDX,
			   slot + TMPL_CALL_IDX, optimized_callback);

	/* Set returning jmp instruction at the tail of out-of-line buffer */
	synthesize_reljump(buf + len, slot + len,
			   (u8 *)op->kp.addr + op->optinsn.size);
<<<<<<< HEAD
	len += RELATIVEJUMP_SIZE;

	/* We have to use text_poke for instuction buffer because it is RO */
	text_poke(slot, buf, len);
=======
	len += JMP32_INSN_SIZE;

	/*
	 * Note	len = TMPL_END_IDX + op->optinsn.size + JMP32_INSN_SIZE is also
	 * used in __arch_remove_optimized_kprobe().
	 */

	/* We have to use text_poke() for instruction buffer because it is RO */
	perf_event_text_poke(slot, NULL, 0, buf, len);
	text_poke(slot, buf, len);

>>>>>>> upstream/android-13
	ret = 0;
out:
	kfree(buf);
	return ret;

err:
	__arch_remove_optimized_kprobe(op, 0);
	goto out;
}

/*
<<<<<<< HEAD
 * Replace breakpoints (int3) with relative jumps.
 * Caller must call with locking kprobe_mutex and text_mutex.
=======
 * Replace breakpoints (INT3) with relative jumps (JMP.d32).
 * Caller must call with locking kprobe_mutex and text_mutex.
 *
 * The caller will have installed a regular kprobe and after that issued
 * syncrhonize_rcu_tasks(), this ensures that the instruction(s) that live in
 * the 4 bytes after the INT3 are unused and can now be overwritten.
>>>>>>> upstream/android-13
 */
void arch_optimize_kprobes(struct list_head *oplist)
{
	struct optimized_kprobe *op, *tmp;
<<<<<<< HEAD
	u8 insn_buf[RELATIVEJUMP_SIZE];

	list_for_each_entry_safe(op, tmp, oplist, list) {
		s32 rel = (s32)((long)op->optinsn.insn -
			((long)op->kp.addr + RELATIVEJUMP_SIZE));
=======
	u8 insn_buff[JMP32_INSN_SIZE];

	list_for_each_entry_safe(op, tmp, oplist, list) {
		s32 rel = (s32)((long)op->optinsn.insn -
			((long)op->kp.addr + JMP32_INSN_SIZE));
>>>>>>> upstream/android-13

		WARN_ON(kprobe_disabled(&op->kp));

		/* Backup instructions which will be replaced by jump address */
<<<<<<< HEAD
		memcpy(op->optinsn.copied_insn, op->kp.addr + INT3_SIZE,
		       RELATIVE_ADDR_SIZE);

		insn_buf[0] = RELATIVEJUMP_OPCODE;
		*(s32 *)(&insn_buf[1]) = rel;

		text_poke_bp(op->kp.addr, insn_buf, RELATIVEJUMP_SIZE,
			     op->optinsn.insn);
=======
		memcpy(op->optinsn.copied_insn, op->kp.addr + INT3_INSN_SIZE,
		       DISP32_SIZE);

		insn_buff[0] = JMP32_INSN_OPCODE;
		*(s32 *)(&insn_buff[1]) = rel;

		text_poke_bp(op->kp.addr, insn_buff, JMP32_INSN_SIZE, NULL);
>>>>>>> upstream/android-13

		list_del_init(&op->list);
	}
}

<<<<<<< HEAD
/* Replace a relative jump with a breakpoint (int3).  */
void arch_unoptimize_kprobe(struct optimized_kprobe *op)
{
	u8 insn_buf[RELATIVEJUMP_SIZE];

	/* Set int3 to first byte for kprobes */
	insn_buf[0] = BREAKPOINT_INSTRUCTION;
	memcpy(insn_buf + 1, op->optinsn.copied_insn, RELATIVE_ADDR_SIZE);
	text_poke_bp(op->kp.addr, insn_buf, RELATIVEJUMP_SIZE,
		     op->optinsn.insn);
=======
/*
 * Replace a relative jump (JMP.d32) with a breakpoint (INT3).
 *
 * After that, we can restore the 4 bytes after the INT3 to undo what
 * arch_optimize_kprobes() scribbled. This is safe since those bytes will be
 * unused once the INT3 lands.
 */
void arch_unoptimize_kprobe(struct optimized_kprobe *op)
{
	u8 new[JMP32_INSN_SIZE] = { INT3_INSN_OPCODE, };
	u8 old[JMP32_INSN_SIZE];
	u8 *addr = op->kp.addr;

	memcpy(old, op->kp.addr, JMP32_INSN_SIZE);
	memcpy(new + INT3_INSN_SIZE,
	       op->optinsn.copied_insn,
	       JMP32_INSN_SIZE - INT3_INSN_SIZE);

	text_poke(addr, new, INT3_INSN_SIZE);
	text_poke_sync();
	text_poke(addr + INT3_INSN_SIZE,
		  new + INT3_INSN_SIZE,
		  JMP32_INSN_SIZE - INT3_INSN_SIZE);
	text_poke_sync();

	perf_event_text_poke(op->kp.addr, old, JMP32_INSN_SIZE, new, JMP32_INSN_SIZE);
>>>>>>> upstream/android-13
}

/*
 * Recover original instructions and breakpoints from relative jumps.
 * Caller must call with locking kprobe_mutex.
 */
extern void arch_unoptimize_kprobes(struct list_head *oplist,
				    struct list_head *done_list)
{
	struct optimized_kprobe *op, *tmp;

	list_for_each_entry_safe(op, tmp, oplist, list) {
		arch_unoptimize_kprobe(op);
		list_move(&op->list, done_list);
	}
}

int setup_detour_execution(struct kprobe *p, struct pt_regs *regs, int reenter)
{
	struct optimized_kprobe *op;

	if (p->flags & KPROBE_FLAG_OPTIMIZED) {
		/* This kprobe is really able to run optimized path. */
		op = container_of(p, struct optimized_kprobe, kp);
		/* Detour through copied instructions */
		regs->ip = (unsigned long)op->optinsn.insn + TMPL_END_IDX;
		if (!reenter)
			reset_current_kprobe();
		return 1;
	}
	return 0;
}
NOKPROBE_SYMBOL(setup_detour_execution);
