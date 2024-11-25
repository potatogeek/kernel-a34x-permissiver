<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2015 Naveen N. Rao, IBM Corporation
 */

#include <asm/trace_clock.h>
#include <asm/time.h>

u64 notrace trace_clock_ppc_tb(void)
{
	return get_tb();
}
