<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2013 Citrix Systems
 *
 * Author: Stefano Stabellini <stefano.stabellini@eu.citrix.com>
 */

#include <linux/export.h>
#include <linux/jump_label.h>
#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/static_call.h>
>>>>>>> upstream/android-13
#include <asm/paravirt.h>

struct static_key paravirt_steal_enabled;
struct static_key paravirt_steal_rq_enabled;

<<<<<<< HEAD
struct pv_time_ops pv_time_ops;
EXPORT_SYMBOL_GPL(pv_time_ops);
=======
static u64 native_steal_clock(int cpu)
{
	return 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
>>>>>>> upstream/android-13
