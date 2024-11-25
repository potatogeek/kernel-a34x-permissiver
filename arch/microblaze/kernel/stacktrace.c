/*
 * Stack trace support for Microblaze.
 *
 * Copyright (C) 2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2009 PetaLogix
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/export.h>
#include <linux/sched.h>
#include <linux/stacktrace.h>
#include <linux/thread_info.h>
#include <linux/ptrace.h>
#include <asm/unwind.h>

void save_stack_trace(struct stack_trace *trace)
{
	/* Exclude our helper functions from the trace*/
	trace->skip += 2;
<<<<<<< HEAD
	microblaze_unwind(NULL, trace);
=======
	microblaze_unwind(NULL, trace, "");
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
<<<<<<< HEAD
	microblaze_unwind(tsk, trace);
=======
	microblaze_unwind(tsk, trace, "");
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
