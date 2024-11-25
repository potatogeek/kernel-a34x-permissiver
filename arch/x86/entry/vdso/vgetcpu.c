<<<<<<< HEAD
/*
 * Copyright 2006 Andi Kleen, SUSE Labs.
 * Subject to the GNU Public License, v.2
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 2006 Andi Kleen, SUSE Labs.
>>>>>>> upstream/android-13
 *
 * Fast user context implementation of getcpu()
 */

#include <linux/kernel.h>
#include <linux/getcpu.h>
#include <linux/time.h>
#include <asm/vgtod.h>

notrace long
__vdso_getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *unused)
{
	vdso_read_cpunode(cpu, node);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	return 0;
}

long getcpu(unsigned *cpu, unsigned *node, struct getcpu_cache *tcache)
	__attribute__((weak, alias("__vdso_getcpu")));
