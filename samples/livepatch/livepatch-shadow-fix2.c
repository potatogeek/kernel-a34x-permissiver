<<<<<<< HEAD
/*
 * Copyright (C) 2017 Joe Lawrence <joe.lawrence@redhat.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2017 Joe Lawrence <joe.lawrence@redhat.com>
>>>>>>> upstream/android-13
 */

/*
 * livepatch-shadow-fix2.c - Shadow variables, livepatch demo
 *
 * Purpose
 * -------
 *
 * Adds functionality to livepatch-shadow-mod's in-flight data
 * structures through a shadow variable.  The livepatch patches a
 * routine that periodically inspects data structures, incrementing a
 * per-data-structure counter, creating the counter if needed.
 *
 *
 * Usage
 * -----
 *
 * This module is not intended to be standalone.  See the "Usage"
 * section of livepatch-shadow-mod.c.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/livepatch.h>
#include <linux/slab.h>

/* Shadow variable enums */
#define SV_LEAK		1
#define SV_COUNTER	2

struct dummy {
	struct list_head list;
	unsigned long jiffies_expire;
};

<<<<<<< HEAD
bool livepatch_fix2_dummy_check(struct dummy *d, unsigned long jiffies)
=======
static bool livepatch_fix2_dummy_check(struct dummy *d, unsigned long jiffies)
>>>>>>> upstream/android-13
{
	int *shadow_count;

	/*
	 * Patch: handle in-flight dummy structures, if they do not
	 * already have a SV_COUNTER shadow variable, then attach a
	 * new one.
	 */
	shadow_count = klp_shadow_get_or_alloc(d, SV_COUNTER,
				sizeof(*shadow_count), GFP_NOWAIT,
				NULL, NULL);
	if (shadow_count)
		*shadow_count += 1;

	return time_after(jiffies, d->jiffies_expire);
}

static void livepatch_fix2_dummy_leak_dtor(void *obj, void *shadow_data)
{
	void *d = obj;
<<<<<<< HEAD
	void **shadow_leak = shadow_data;
=======
	int **shadow_leak = shadow_data;
>>>>>>> upstream/android-13

	kfree(*shadow_leak);
	pr_info("%s: dummy @ %p, prevented leak @ %p\n",
			 __func__, d, *shadow_leak);
}

<<<<<<< HEAD
void livepatch_fix2_dummy_free(struct dummy *d)
{
	void **shadow_leak;
=======
static void livepatch_fix2_dummy_free(struct dummy *d)
{
	int **shadow_leak;
>>>>>>> upstream/android-13
	int *shadow_count;

	/* Patch: copy the memory leak patch from the fix1 module. */
	shadow_leak = klp_shadow_get(d, SV_LEAK);
	if (shadow_leak)
		klp_shadow_free(d, SV_LEAK, livepatch_fix2_dummy_leak_dtor);
	else
		pr_info("%s: dummy @ %p leaked!\n", __func__, d);

	/*
	 * Patch: fetch the SV_COUNTER shadow variable and display
	 * the final count.  Detach the shadow variable.
	 */
	shadow_count = klp_shadow_get(d, SV_COUNTER);
	if (shadow_count) {
		pr_info("%s: dummy @ %p, check counter = %d\n",
			__func__, d, *shadow_count);
		klp_shadow_free(d, SV_COUNTER, NULL);
	}

	kfree(d);
}

static struct klp_func funcs[] = {
	{
		.old_name = "dummy_check",
		.new_func = livepatch_fix2_dummy_check,
	},
	{
		.old_name = "dummy_free",
		.new_func = livepatch_fix2_dummy_free,
	}, { }
};

static struct klp_object objs[] = {
	{
		.name = "livepatch_shadow_mod",
		.funcs = funcs,
	}, { }
};

static struct klp_patch patch = {
	.mod = THIS_MODULE,
	.objs = objs,
};

static int livepatch_shadow_fix2_init(void)
{
<<<<<<< HEAD
	int ret;

	ret = klp_register_patch(&patch);
	if (ret)
		return ret;
	ret = klp_enable_patch(&patch);
	if (ret) {
		WARN_ON(klp_unregister_patch(&patch));
		return ret;
	}
	return 0;
=======
	return klp_enable_patch(&patch);
>>>>>>> upstream/android-13
}

static void livepatch_shadow_fix2_exit(void)
{
	/* Cleanup any existing SV_COUNTER shadow variables */
	klp_shadow_free_all(SV_COUNTER, NULL);
<<<<<<< HEAD

	WARN_ON(klp_unregister_patch(&patch));
=======
>>>>>>> upstream/android-13
}

module_init(livepatch_shadow_fix2_init);
module_exit(livepatch_shadow_fix2_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");
