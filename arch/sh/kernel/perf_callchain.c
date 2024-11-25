<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Performance event callchain support - SuperH architecture code
 *
 * Copyright (C) 2009  Paul Mundt
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> upstream/android-13
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/perf_event.h>
#include <linux/percpu.h>
#include <asm/unwinder.h>
#include <asm/ptrace.h>

<<<<<<< HEAD
static int callchain_stack(void *data, char *name)
{
	return 0;
}

=======
>>>>>>> upstream/android-13
static void callchain_address(void *data, unsigned long addr, int reliable)
{
	struct perf_callchain_entry_ctx *entry = data;

	if (reliable)
		perf_callchain_store(entry, addr);
}

static const struct stacktrace_ops callchain_ops = {
<<<<<<< HEAD
	.stack		= callchain_stack,
=======
>>>>>>> upstream/android-13
	.address	= callchain_address,
};

void
perf_callchain_kernel(struct perf_callchain_entry_ctx *entry, struct pt_regs *regs)
{
	perf_callchain_store(entry, regs->pc);

	unwind_stack(NULL, regs, NULL, &callchain_ops, entry);
}
