<<<<<<< HEAD
/*
 * Generic page table allocator for IOMMUs.
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
 *
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic page table allocator for IOMMUs.
 *
>>>>>>> upstream/android-13
 * Copyright (C) 2014 ARM Limited
 *
 * Author: Will Deacon <will.deacon@arm.com>
 */

<<<<<<< HEAD
#define pr_fmt(fmt)	"io-pgtable: " fmt

#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/iommu.h>
#include <linux/debugfs.h>
#include <linux/atomic.h>
#include <linux/module.h>
#include "io-pgtable.h"
=======
#include <linux/bug.h>
#include <linux/io-pgtable.h>
#include <linux/kernel.h>
#include <linux/types.h>
>>>>>>> upstream/android-13

static const struct io_pgtable_init_fns *
io_pgtable_init_table[IO_PGTABLE_NUM_FMTS] = {
#ifdef CONFIG_IOMMU_IO_PGTABLE_LPAE
	[ARM_32_LPAE_S1] = &io_pgtable_arm_32_lpae_s1_init_fns,
	[ARM_32_LPAE_S2] = &io_pgtable_arm_32_lpae_s2_init_fns,
	[ARM_64_LPAE_S1] = &io_pgtable_arm_64_lpae_s1_init_fns,
	[ARM_64_LPAE_S2] = &io_pgtable_arm_64_lpae_s2_init_fns,
<<<<<<< HEAD
=======
	[ARM_MALI_LPAE] = &io_pgtable_arm_mali_lpae_init_fns,
	[APPLE_DART] = &io_pgtable_apple_dart_init_fns,
>>>>>>> upstream/android-13
#endif
#ifdef CONFIG_IOMMU_IO_PGTABLE_ARMV7S
	[ARM_V7S] = &io_pgtable_arm_v7s_init_fns,
#endif
<<<<<<< HEAD
#ifdef CONFIG_IOMMU_IO_PGTABLE_FAST
	[ARM_V8L_FAST] = &io_pgtable_av8l_fast_init_fns,
#endif
};

static struct dentry *io_pgtable_top;

=======
#ifdef CONFIG_AMD_IOMMU
	[AMD_IOMMU_V1] = &io_pgtable_amd_iommu_v1_init_fns,
#endif
};

>>>>>>> upstream/android-13
struct io_pgtable_ops *alloc_io_pgtable_ops(enum io_pgtable_fmt fmt,
					    struct io_pgtable_cfg *cfg,
					    void *cookie)
{
	struct io_pgtable *iop;
	const struct io_pgtable_init_fns *fns;

<<<<<<< HEAD
	if (fmt >= IO_PGTABLE_NUM_FMTS) {
		pr_notice("%s, %d, err fmt:0x%x, IO_PGTABLE_NUM_FMTS:0x%x\n",
			__func__, __LINE__, fmt, IO_PGTABLE_NUM_FMTS);
		return NULL;
	}

	fns = io_pgtable_init_table[fmt];
	if (!fns) {
		pr_notice("%s, %d, err fns\n", __func__, __LINE__);
		return NULL;
	}

	iop = fns->alloc(cfg, cookie);
	if (!iop) {
		pr_notice("%s, %d, err iop\n", __func__, __LINE__);
		return NULL;
	}
=======
	if (fmt >= IO_PGTABLE_NUM_FMTS)
		return NULL;

	fns = io_pgtable_init_table[fmt];
	if (!fns)
		return NULL;

	iop = fns->alloc(cfg, cookie);
	if (!iop)
		return NULL;
>>>>>>> upstream/android-13

	iop->fmt	= fmt;
	iop->cookie	= cookie;
	iop->cfg	= *cfg;

	return &iop->ops;
}
EXPORT_SYMBOL_GPL(alloc_io_pgtable_ops);

/*
 * It is the IOMMU driver's responsibility to ensure that the page table
 * is no longer accessible to the walker by this point.
 */
void free_io_pgtable_ops(struct io_pgtable_ops *ops)
{
	struct io_pgtable *iop;

	if (!ops)
		return;

<<<<<<< HEAD
	iop = container_of(ops, struct io_pgtable, ops);
=======
	iop = io_pgtable_ops_to_pgtable(ops);
>>>>>>> upstream/android-13
	io_pgtable_tlb_flush_all(iop);
	io_pgtable_init_table[iop->fmt]->free(iop);
}
EXPORT_SYMBOL_GPL(free_io_pgtable_ops);
<<<<<<< HEAD

static atomic_t pages_allocated;

void *io_pgtable_alloc_pages_exact(struct io_pgtable_cfg *cfg, void *cookie,
				   size_t size, gfp_t gfp_mask)
{
	void *ret;

	if (cfg->tlb->alloc_pages_exact)
		ret = cfg->tlb->alloc_pages_exact(cookie, size, gfp_mask);
	else
		ret = alloc_pages_exact(size, gfp_mask);

	if (likely(ret))
		atomic_add(1 << get_order(size), &pages_allocated);

	return ret;
}

void io_pgtable_free_pages_exact(struct io_pgtable_cfg *cfg, void *cookie,
				 void *virt, size_t size)
{
	if (cfg->tlb->free_pages_exact)
		cfg->tlb->free_pages_exact(cookie, virt, size);
	else
		free_pages_exact(virt, size);

	atomic_sub(1 << get_order(size), &pages_allocated);
}

static int __init io_pgtable_init(void)
{
	static const char io_pgtable_str[] __initconst = "io-pgtable";
	static const char pages_str[] __initconst = "pages";

	io_pgtable_top = debugfs_create_dir(io_pgtable_str, iommu_debugfs_top);
	debugfs_create_atomic_t(pages_str, 0600, io_pgtable_top,
				&pages_allocated);
	return 0;
}

static void __exit io_pgtable_exit(void)
{
	debugfs_remove_recursive(io_pgtable_top);
}

module_init(io_pgtable_init);
module_exit(io_pgtable_exit);
=======
>>>>>>> upstream/android-13
