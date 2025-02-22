<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
// Copyright (C) 2017 Arm Ltd.
#ifndef __ASM_VMAP_STACK_H
#define __ASM_VMAP_STACK_H

#include <linux/bug.h>
#include <linux/gfp.h>
#include <linux/kconfig.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
#include <asm/memory.h>
#include <asm/pgtable.h>
=======
#include <linux/pgtable.h>
#include <asm/memory.h>
>>>>>>> upstream/android-13
#include <asm/thread_info.h>

/*
 * To ensure that VMAP'd stack overflow detection works correctly, all VMAP'd
 * stacks need to have the same alignment.
 */
static inline unsigned long *arch_alloc_vmap_stack(size_t stack_size, int node)
{
<<<<<<< HEAD
	BUILD_BUG_ON(!IS_ENABLED(CONFIG_VMAP_STACK));

	return __vmalloc_node_range(stack_size, THREAD_ALIGN,
				    VMALLOC_START, VMALLOC_END,
				    THREADINFO_GFP, PAGE_KERNEL, 0, node,
				    __builtin_return_address(0));
=======
	void *p;

	BUILD_BUG_ON(!IS_ENABLED(CONFIG_VMAP_STACK));

	p = __vmalloc_node(stack_size, THREAD_ALIGN, THREADINFO_GFP, node,
			__builtin_return_address(0));
	return kasan_reset_tag(p);
>>>>>>> upstream/android-13
}

#endif /* __ASM_VMAP_STACK_H */
