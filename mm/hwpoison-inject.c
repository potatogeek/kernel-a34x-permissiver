<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* Inject a hwpoison memory failure on a arbitrary pfn */
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/pagemap.h>
#include <linux/hugetlb.h>
#include "internal.h"

static struct dentry *hwpoison_dir;

static int hwpoison_inject(void *data, u64 val)
{
	unsigned long pfn = val;
	struct page *p;
	struct page *hpage;
	int err;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (!pfn_valid(pfn))
		return -ENXIO;

	p = pfn_to_page(pfn);
	hpage = compound_head(p);
<<<<<<< HEAD
	/*
	 * This implies unable to support free buddy pages.
	 */
	if (!get_hwpoison_page(p))
		return 0;
=======
>>>>>>> upstream/android-13

	if (!hwpoison_filter_enable)
		goto inject;

<<<<<<< HEAD
	shake_page(hpage, 0);
=======
	shake_page(hpage);
>>>>>>> upstream/android-13
	/*
	 * This implies unable to support non-LRU pages.
	 */
	if (!PageLRU(hpage) && !PageHuge(p))
<<<<<<< HEAD
		goto put_out;

	/*
	 * do a racy check with elevated page count, to make sure PG_hwpoison
	 * will only be set for the targeted owner (or on a free page).
=======
		return 0;

	/*
	 * do a racy check to make sure PG_hwpoison will only be set for
	 * the targeted owner (or on a free page).
>>>>>>> upstream/android-13
	 * memory_failure() will redo the check reliably inside page lock.
	 */
	err = hwpoison_filter(hpage);
	if (err)
<<<<<<< HEAD
		goto put_out;

inject:
	pr_info("Injecting memory failure at pfn %#lx\n", pfn);
	return memory_failure(pfn, MF_COUNT_INCREASED);
put_out:
	put_hwpoison_page(p);
	return 0;
=======
		return 0;

inject:
	pr_info("Injecting memory failure at pfn %#lx\n", pfn);
	return memory_failure(pfn, 0);
>>>>>>> upstream/android-13
}

static int hwpoison_unpoison(void *data, u64 val)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	return unpoison_memory(val);
}

<<<<<<< HEAD
DEFINE_SIMPLE_ATTRIBUTE(hwpoison_fops, NULL, hwpoison_inject, "%lli\n");
DEFINE_SIMPLE_ATTRIBUTE(unpoison_fops, NULL, hwpoison_unpoison, "%lli\n");
=======
DEFINE_DEBUGFS_ATTRIBUTE(hwpoison_fops, NULL, hwpoison_inject, "%lli\n");
DEFINE_DEBUGFS_ATTRIBUTE(unpoison_fops, NULL, hwpoison_unpoison, "%lli\n");
>>>>>>> upstream/android-13

static void pfn_inject_exit(void)
{
	debugfs_remove_recursive(hwpoison_dir);
}

static int pfn_inject_init(void)
{
<<<<<<< HEAD
	struct dentry *dentry;

	hwpoison_dir = debugfs_create_dir("hwpoison", NULL);
	if (hwpoison_dir == NULL)
		return -ENOMEM;
=======
	hwpoison_dir = debugfs_create_dir("hwpoison", NULL);
>>>>>>> upstream/android-13

	/*
	 * Note that the below poison/unpoison interfaces do not involve
	 * hardware status change, hence do not require hardware support.
	 * They are mainly for testing hwpoison in software level.
	 */
<<<<<<< HEAD
	dentry = debugfs_create_file("corrupt-pfn", 0200, hwpoison_dir,
					  NULL, &hwpoison_fops);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_file("unpoison-pfn", 0200, hwpoison_dir,
				     NULL, &unpoison_fops);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u32("corrupt-filter-enable", 0600,
				    hwpoison_dir, &hwpoison_filter_enable);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u32("corrupt-filter-dev-major", 0600,
				    hwpoison_dir, &hwpoison_filter_dev_major);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u32("corrupt-filter-dev-minor", 0600,
				    hwpoison_dir, &hwpoison_filter_dev_minor);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u64("corrupt-filter-flags-mask", 0600,
				    hwpoison_dir, &hwpoison_filter_flags_mask);
	if (!dentry)
		goto fail;

	dentry = debugfs_create_u64("corrupt-filter-flags-value", 0600,
				    hwpoison_dir, &hwpoison_filter_flags_value);
	if (!dentry)
		goto fail;

#ifdef CONFIG_MEMCG
	dentry = debugfs_create_u64("corrupt-filter-memcg", 0600,
				    hwpoison_dir, &hwpoison_filter_memcg);
	if (!dentry)
		goto fail;
#endif

	return 0;
fail:
	pfn_inject_exit();
	return -ENOMEM;
=======
	debugfs_create_file("corrupt-pfn", 0200, hwpoison_dir, NULL,
			    &hwpoison_fops);

	debugfs_create_file("unpoison-pfn", 0200, hwpoison_dir, NULL,
			    &unpoison_fops);

	debugfs_create_u32("corrupt-filter-enable", 0600, hwpoison_dir,
			   &hwpoison_filter_enable);

	debugfs_create_u32("corrupt-filter-dev-major", 0600, hwpoison_dir,
			   &hwpoison_filter_dev_major);

	debugfs_create_u32("corrupt-filter-dev-minor", 0600, hwpoison_dir,
			   &hwpoison_filter_dev_minor);

	debugfs_create_u64("corrupt-filter-flags-mask", 0600, hwpoison_dir,
			   &hwpoison_filter_flags_mask);

	debugfs_create_u64("corrupt-filter-flags-value", 0600, hwpoison_dir,
			   &hwpoison_filter_flags_value);

#ifdef CONFIG_MEMCG
	debugfs_create_u64("corrupt-filter-memcg", 0600, hwpoison_dir,
			   &hwpoison_filter_memcg);
#endif

	return 0;
>>>>>>> upstream/android-13
}

module_init(pfn_inject_init);
module_exit(pfn_inject_exit);
MODULE_LICENSE("GPL");
