// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
#include <linux/memblock.h>
>>>>>>> upstream/android-13
#include <linux/string.h>
#include <asm/setup.h>


void * __ref zalloc_maybe_bootmem(size_t size, gfp_t mask)
{
	void *p;

	if (slab_is_available())
		p = kzalloc(size, mask);
	else {
<<<<<<< HEAD
		p = memblock_virt_alloc(size, 0);
=======
		p = memblock_alloc(size, SMP_CACHE_BYTES);
		if (!p)
			panic("%s: Failed to allocate %zu bytes\n", __func__,
			      size);
>>>>>>> upstream/android-13
	}
	return p;
}
