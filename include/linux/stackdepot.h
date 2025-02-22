<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * A generic stack depot implementation
 *
 * Author: Alexander Potapenko <glider@google.com>
 * Copyright (C) 2016 Google, Inc.
 *
 * Based on code by Dmitry Chernenkov.
<<<<<<< HEAD
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
=======
>>>>>>> upstream/android-13
 */

#ifndef _LINUX_STACKDEPOT_H
#define _LINUX_STACKDEPOT_H

<<<<<<< HEAD
typedef u32 depot_stack_handle_t;

struct stack_trace;

depot_stack_handle_t depot_save_stack(struct stack_trace *trace, gfp_t flags);

void depot_fetch_stack(depot_stack_handle_t handle, struct stack_trace *trace);
#ifdef CONFIG_PAGE_OWNER
void depot_hit_stack(depot_stack_handle_t handle, struct stack_trace *trace,
		int cnt);
void show_max_hit_page(void);
#endif
=======
#include <linux/gfp.h>

typedef u32 depot_stack_handle_t;

depot_stack_handle_t __stack_depot_save(unsigned long *entries,
					unsigned int nr_entries,
					gfp_t gfp_flags, bool can_alloc);

depot_stack_handle_t stack_depot_save(unsigned long *entries,
				      unsigned int nr_entries, gfp_t gfp_flags);

unsigned int stack_depot_fetch(depot_stack_handle_t handle,
			       unsigned long **entries);

#ifdef CONFIG_STACKDEPOT
int stack_depot_init(void);
#else
static inline int stack_depot_init(void)
{
	return 0;
}
#endif	/* CONFIG_STACKDEPOT */
>>>>>>> upstream/android-13

#endif
