<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
#ifndef _ASM_X86_KPROBES_H
#define _ASM_X86_KPROBES_H
/*
 *  Kernel Probes (KProbes)
 *
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 * Copyright (C) IBM Corporation, 2002, 2004
 *
 * See arch/x86/kernel/kprobes.c for x86 kprobes history.
 */

#include <asm-generic/kprobes.h>

<<<<<<< HEAD
#define BREAKPOINT_INSTRUCTION	0xcc

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_KPROBES
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/percpu.h>
<<<<<<< HEAD
=======
#include <asm/text-patching.h>
>>>>>>> upstream/android-13
#include <asm/insn.h>

#define  __ARCH_WANT_KPROBES_INSN_SLOT

struct pt_regs;
struct kprobe;

typedef u8 kprobe_opcode_t;
<<<<<<< HEAD
#define RELATIVEJUMP_OPCODE 0xe9
#define RELATIVEJUMP_SIZE 5
#define RELATIVECALL_OPCODE 0xe8
#define RELATIVE_ADDR_SIZE 4
=======

>>>>>>> upstream/android-13
#define MAX_STACK_SIZE 64
#define CUR_STACK_SIZE(ADDR) \
	(current_top_of_stack() - (unsigned long)(ADDR))
#define MIN_STACK_SIZE(ADDR)				\
	(MAX_STACK_SIZE < CUR_STACK_SIZE(ADDR) ?	\
	 MAX_STACK_SIZE : CUR_STACK_SIZE(ADDR))

#define flush_insn_slot(p)	do { } while (0)

/* optinsn template addresses */
extern __visible kprobe_opcode_t optprobe_template_entry[];
<<<<<<< HEAD
extern __visible kprobe_opcode_t optprobe_template_val[];
extern __visible kprobe_opcode_t optprobe_template_call[];
extern __visible kprobe_opcode_t optprobe_template_end[];
#define MAX_OPTIMIZED_LENGTH (MAX_INSN_SIZE + RELATIVE_ADDR_SIZE)
#define MAX_OPTINSN_SIZE 				\
	(((unsigned long)optprobe_template_end -	\
	  (unsigned long)optprobe_template_entry) +	\
	 MAX_OPTIMIZED_LENGTH + RELATIVEJUMP_SIZE)
=======
extern __visible kprobe_opcode_t optprobe_template_clac[];
extern __visible kprobe_opcode_t optprobe_template_val[];
extern __visible kprobe_opcode_t optprobe_template_call[];
extern __visible kprobe_opcode_t optprobe_template_end[];
#define MAX_OPTIMIZED_LENGTH (MAX_INSN_SIZE + DISP32_SIZE)
#define MAX_OPTINSN_SIZE 				\
	(((unsigned long)optprobe_template_end -	\
	  (unsigned long)optprobe_template_entry) +	\
	 MAX_OPTIMIZED_LENGTH + JMP32_INSN_SIZE)
>>>>>>> upstream/android-13

extern const int kretprobe_blacklist_size;

void arch_remove_kprobe(struct kprobe *p);
asmlinkage void kretprobe_trampoline(void);

extern void arch_kprobe_override_function(struct pt_regs *regs);

/* Architecture specific copy of original instruction*/
struct arch_specific_insn {
	/* copy of the original instruction */
	kprobe_opcode_t *insn;
	/*
<<<<<<< HEAD
	 * boostable = false: This instruction type is not boostable.
	 * boostable = true: This instruction has been boosted: we have
=======
	 * boostable = 0: This instruction type is not boostable.
	 * boostable = 1: This instruction has been boosted: we have
>>>>>>> upstream/android-13
	 * added a relative jump after the instruction copy in insn,
	 * so no single-step and fixup are needed (unless there's
	 * a post_handler).
	 */
<<<<<<< HEAD
	bool boostable;
	bool if_modifier;
=======
	unsigned boostable:1;
	unsigned char size;	/* The size of insn */
	union {
		unsigned char opcode;
		struct {
			unsigned char type;
		} jcc;
		struct {
			unsigned char type;
			unsigned char asize;
		} loop;
		struct {
			unsigned char reg;
		} indirect;
	};
	s32 rel32;	/* relative offset must be s32, s16, or s8 */
	void (*emulate_op)(struct kprobe *p, struct pt_regs *regs);
	/* Number of bytes of text poked */
	int tp_len;
>>>>>>> upstream/android-13
};

struct arch_optimized_insn {
	/* copy of the original instructions */
<<<<<<< HEAD
	kprobe_opcode_t copied_insn[RELATIVE_ADDR_SIZE];
=======
	kprobe_opcode_t copied_insn[DISP32_SIZE];
>>>>>>> upstream/android-13
	/* detour code buffer */
	kprobe_opcode_t *insn;
	/* the size of instructions copied to detour code buffer */
	size_t size;
};

/* Return true (!0) if optinsn is prepared for optimization. */
static inline int arch_prepared_optinsn(struct arch_optimized_insn *optinsn)
{
	return optinsn->size;
}

struct prev_kprobe {
	struct kprobe *kp;
	unsigned long status;
	unsigned long old_flags;
	unsigned long saved_flags;
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned long kprobe_status;
	unsigned long kprobe_old_flags;
	unsigned long kprobe_saved_flags;
	struct prev_kprobe prev_kprobe;
};

extern int kprobe_fault_handler(struct pt_regs *regs, int trapnr);
extern int kprobe_exceptions_notify(struct notifier_block *self,
				    unsigned long val, void *data);
extern int kprobe_int3_handler(struct pt_regs *regs);
<<<<<<< HEAD
extern int kprobe_debug_handler(struct pt_regs *regs);
=======

#else

static inline int kprobe_debug_handler(struct pt_regs *regs) { return 0; }
>>>>>>> upstream/android-13

#endif /* CONFIG_KPROBES */
#endif /* _ASM_X86_KPROBES_H */
