<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * arch/arm64/kernel/return_address.c
 *
 * Copyright (C) 2013 Linaro Limited
 * Author: AKASHI Takahiro <takahiro.akashi@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <linux/export.h>
#include <linux/ftrace.h>
#include <linux/kprobes.h>

#include <asm/stack_pointer.h>
#include <asm/stacktrace.h>

struct return_address_data {
	unsigned int level;
	void *addr;
};

<<<<<<< HEAD
static int save_return_addr(struct stackframe *frame, void *d)
=======
static bool save_return_addr(void *d, unsigned long pc)
>>>>>>> upstream/android-13
{
	struct return_address_data *data = d;

	if (!data->level) {
<<<<<<< HEAD
		data->addr = (void *)frame->pc;
		return 1;
	} else {
		--data->level;
		return 0;
=======
		data->addr = (void *)pc;
		return false;
	} else {
		--data->level;
		return true;
>>>>>>> upstream/android-13
	}
}
NOKPROBE_SYMBOL(save_return_addr);

void *return_address(unsigned int level)
{
	struct return_address_data data;
	struct stackframe frame;

	data.level = level + 2;
	data.addr = NULL;

<<<<<<< HEAD
	frame.fp = (unsigned long)__builtin_frame_address(0);
	frame.pc = (unsigned long)return_address; /* dummy */
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = current->curr_ret_stack;
#endif

=======
	start_backtrace(&frame,
			(unsigned long)__builtin_frame_address(0),
			(unsigned long)return_address);
>>>>>>> upstream/android-13
	walk_stackframe(current, &frame, save_return_addr, &data);

	if (!data.level)
		return data.addr;
	else
		return NULL;
}
EXPORT_SYMBOL_GPL(return_address);
NOKPROBE_SYMBOL(return_address);
