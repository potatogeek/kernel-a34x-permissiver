// SPDX-License-Identifier: GPL-2.0
#include <linux/fault-inject.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "slab.h"

static struct {
	struct fault_attr attr;
	bool ignore_gfp_reclaim;
	bool cache_filter;
} failslab = {
	.attr = FAULT_ATTR_INITIALIZER,
	.ignore_gfp_reclaim = true,
	.cache_filter = false,
};

bool __should_failslab(struct kmem_cache *s, gfp_t gfpflags)
{
	/* No fault-injection for bootstrap cache */
	if (unlikely(s == kmem_cache))
		return false;

	if (gfpflags & __GFP_NOFAIL)
		return false;

<<<<<<< HEAD
	if (failslab.ignore_gfp_reclaim && (gfpflags & __GFP_RECLAIM))
=======
	if (failslab.ignore_gfp_reclaim &&
			(gfpflags & __GFP_DIRECT_RECLAIM))
>>>>>>> upstream/android-13
		return false;

	if (failslab.cache_filter && !(s->flags & SLAB_FAILSLAB))
		return false;

	return should_fail(&failslab.attr, s->object_size);
}

static int __init setup_failslab(char *str)
{
	return setup_fault_attr(&failslab.attr, str);
}
__setup("failslab=", setup_failslab);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS
static int __init failslab_debugfs_init(void)
{
	struct dentry *dir;
	umode_t mode = S_IFREG | 0600;

	dir = fault_create_debugfs_attr("failslab", NULL, &failslab.attr);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

<<<<<<< HEAD
	if (!debugfs_create_bool("ignore-gfp-wait", mode, dir,
				&failslab.ignore_gfp_reclaim))
		goto fail;
	if (!debugfs_create_bool("cache-filter", mode, dir,
				&failslab.cache_filter))
		goto fail;

	return 0;
fail:
	debugfs_remove_recursive(dir);

	return -ENOMEM;
=======
	debugfs_create_bool("ignore-gfp-wait", mode, dir,
			    &failslab.ignore_gfp_reclaim);
	debugfs_create_bool("cache-filter", mode, dir,
			    &failslab.cache_filter);

	return 0;
>>>>>>> upstream/android-13
}

late_initcall(failslab_debugfs_init);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */
