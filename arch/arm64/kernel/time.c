<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/kernel/time.c
 *
 * Copyright (C) 1991, 1992, 1995  Linus Torvalds
 * Modifications for ARM (C) 1994-2001 Russell King
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clockchips.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/timex.h>
#include <linux/errno.h>
#include <linux/profile.h>
#include <linux/syscore_ops.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/clocksource.h>
<<<<<<< HEAD
#include <linux/clk-provider.h>
=======
#include <linux/of_clk.h>
>>>>>>> upstream/android-13
#include <linux/acpi.h>

#include <clocksource/arm_arch_timer.h>

#include <asm/thread_info.h>
#include <asm/stacktrace.h>
<<<<<<< HEAD
=======
#include <asm/paravirt.h>
>>>>>>> upstream/android-13

unsigned long profile_pc(struct pt_regs *regs)
{
	struct stackframe frame;

	if (!in_lock_functions(regs->pc))
		return regs->pc;

<<<<<<< HEAD
	frame.fp = regs->regs[29];
	frame.pc = regs->pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = current->curr_ret_stack;
#endif
=======
	start_backtrace(&frame, regs->regs[29], regs->pc);

>>>>>>> upstream/android-13
	do {
		int ret = unwind_frame(NULL, &frame);
		if (ret < 0)
			return 0;
	} while (in_lock_functions(frame.pc));

	return frame.pc;
}
EXPORT_SYMBOL(profile_pc);

void __init time_init(void)
{
	u32 arch_timer_rate;

	of_clk_init(NULL);
	timer_probe();

	tick_setup_hrtimer_broadcast();

	arch_timer_rate = arch_timer_get_rate();
	if (!arch_timer_rate)
		panic("Unable to initialise architected timer.\n");

	/* Calibrate the delay loop directly */
	lpj_fine = arch_timer_rate / HZ;
<<<<<<< HEAD
=======

	pv_time_init();
>>>>>>> upstream/android-13
}
