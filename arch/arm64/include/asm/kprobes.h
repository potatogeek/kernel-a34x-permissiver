<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * arch/arm64/include/asm/kprobes.h
 *
 * Copyright (C) 2013 Linaro Limited
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#ifndef _ARM_KPROBES_H
#define _ARM_KPROBES_H

#include <asm-generic/kprobes.h>

#ifdef CONFIG_KPROBES
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/percpu.h>

#define __ARCH_WANT_KPROBES_INSN_SLOT
<<<<<<< HEAD
#define MAX_INSN_SIZE			1
=======
#define MAX_INSN_SIZE			2
>>>>>>> upstream/android-13

#define flush_insn_slot(p)		do { } while (0)
#define kretprobe_blacklist_size	0

#include <asm/probes.h>

struct prev_kprobe {
	struct kprobe *kp;
	unsigned int status;
};

<<<<<<< HEAD
/* Single step context for kprobe */
struct kprobe_step_ctx {
	unsigned long ss_pending;
	unsigned long match_addr;
};

=======
>>>>>>> upstream/android-13
/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned int kprobe_status;
	unsigned long saved_irqflag;
	struct prev_kprobe prev_kprobe;
<<<<<<< HEAD
	struct kprobe_step_ctx ss_ctx;
=======
>>>>>>> upstream/android-13
};

void arch_remove_kprobe(struct kprobe *);
int kprobe_fault_handler(struct pt_regs *regs, unsigned int fsr);
int kprobe_exceptions_notify(struct notifier_block *self,
			     unsigned long val, void *data);
<<<<<<< HEAD
int kprobe_breakpoint_handler(struct pt_regs *regs, unsigned int esr);
int kprobe_single_step_handler(struct pt_regs *regs, unsigned int esr);
=======
>>>>>>> upstream/android-13
void kretprobe_trampoline(void);
void __kprobes *trampoline_probe_handler(struct pt_regs *regs);

#endif /* CONFIG_KPROBES */
#endif /* _ARM_KPROBES_H */
