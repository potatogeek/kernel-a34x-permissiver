<<<<<<< HEAD
/*
 * Dynamic Ftrace based Kprobes Optimization
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
 * Dynamic Ftrace based Kprobes Optimization
 *
>>>>>>> upstream/android-13
 * Copyright (C) Hitachi Ltd., 2012
 * Copyright 2016 Naveen N. Rao <naveen.n.rao@linux.vnet.ibm.com>
 *		  IBM Corporation
 */
#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <linux/hardirq.h>
#include <linux/preempt.h>
#include <linux/ftrace.h>

/* Ftrace callback handler for kprobes */
void kprobe_ftrace_handler(unsigned long nip, unsigned long parent_nip,
<<<<<<< HEAD
			   struct ftrace_ops *ops, struct pt_regs *regs)
{
	struct kprobe *p;
	struct kprobe_ctlblk *kcb;

	p = get_kprobe((kprobe_opcode_t *)nip);
	if (unlikely(!p) || kprobe_disabled(p))
		return;
=======
			   struct ftrace_ops *ops, struct ftrace_regs *fregs)
{
	struct kprobe *p;
	struct kprobe_ctlblk *kcb;
	struct pt_regs *regs;
	int bit;

	bit = ftrace_test_recursion_trylock(nip, parent_nip);
	if (bit < 0)
		return;

	regs = ftrace_get_regs(fregs);
	preempt_disable_notrace();
	p = get_kprobe((kprobe_opcode_t *)nip);
	if (unlikely(!p) || kprobe_disabled(p))
		goto out;
>>>>>>> upstream/android-13

	kcb = get_kprobe_ctlblk();
	if (kprobe_running()) {
		kprobes_inc_nmissed_count(p);
	} else {
		/*
		 * On powerpc, NIP is *before* this instruction for the
		 * pre handler
		 */
<<<<<<< HEAD
		regs->nip -= MCOUNT_INSN_SIZE;
=======
		regs_add_return_ip(regs, -MCOUNT_INSN_SIZE);
>>>>>>> upstream/android-13

		__this_cpu_write(current_kprobe, p);
		kcb->kprobe_status = KPROBE_HIT_ACTIVE;
		if (!p->pre_handler || !p->pre_handler(p, regs)) {
			/*
			 * Emulate singlestep (and also recover regs->nip)
			 * as if there is a nop
			 */
<<<<<<< HEAD
			regs->nip += MCOUNT_INSN_SIZE;
=======
			regs_add_return_ip(regs, MCOUNT_INSN_SIZE);
>>>>>>> upstream/android-13
			if (unlikely(p->post_handler)) {
				kcb->kprobe_status = KPROBE_HIT_SSDONE;
				p->post_handler(p, regs, 0);
			}
		}
		/*
		 * If pre_handler returns !0, it changes regs->nip. We have to
		 * skip emulating post_handler.
		 */
		__this_cpu_write(current_kprobe, NULL);
	}
<<<<<<< HEAD
=======
out:
	preempt_enable_notrace();
	ftrace_test_recursion_unlock(bit);
>>>>>>> upstream/android-13
}
NOKPROBE_SYMBOL(kprobe_ftrace_handler);

int arch_prepare_kprobe_ftrace(struct kprobe *p)
{
	p->ainsn.insn = NULL;
	p->ainsn.boostable = -1;
	return 0;
}
