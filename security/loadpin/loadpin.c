<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Module and Firmware Pinning Security Module
 *
 * Copyright 2011-2016 Google Inc.
 *
 * Author: Kees Cook <keescook@chromium.org>
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#define pr_fmt(fmt) "LoadPin: " fmt

#include <linux/module.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/lsm_hooks.h>
#include <linux/mount.h>
=======
#include <linux/kernel_read_file.h>
#include <linux/lsm_hooks.h>
#include <linux/mount.h>
#include <linux/blkdev.h>
>>>>>>> upstream/android-13
#include <linux/path.h>
#include <linux/sched.h>	/* current */
#include <linux/string_helpers.h>

static void report_load(const char *origin, struct file *file, char *operation)
{
	char *cmdline, *pathname;

	pathname = kstrdup_quotable_file(file, GFP_KERNEL);
	cmdline = kstrdup_quotable_cmdline(current, GFP_KERNEL);

	pr_notice("%s %s obj=%s%s%s pid=%d cmdline=%s%s%s\n",
		  origin, operation,
		  (pathname && pathname[0] != '<') ? "\"" : "",
		  pathname,
		  (pathname && pathname[0] != '<') ? "\"" : "",
		  task_pid_nr(current),
		  cmdline ? "\"" : "", cmdline, cmdline ? "\"" : "");

	kfree(cmdline);
	kfree(pathname);
}

<<<<<<< HEAD
static int enabled = IS_ENABLED(CONFIG_SECURITY_LOADPIN_ENABLED);
=======
static int enforce = IS_ENABLED(CONFIG_SECURITY_LOADPIN_ENFORCE);
static char *exclude_read_files[READING_MAX_ID];
static int ignore_read_file_id[READING_MAX_ID] __ro_after_init;
>>>>>>> upstream/android-13
static struct super_block *pinned_root;
static DEFINE_SPINLOCK(pinned_root_spinlock);

#ifdef CONFIG_SYSCTL
<<<<<<< HEAD
static int zero;
static int one = 1;
=======
>>>>>>> upstream/android-13

static struct ctl_path loadpin_sysctl_path[] = {
	{ .procname = "kernel", },
	{ .procname = "loadpin", },
	{ }
};

static struct ctl_table loadpin_sysctl_table[] = {
	{
<<<<<<< HEAD
		.procname       = "enabled",
		.data           = &enabled,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = &zero,
		.extra2         = &one,
=======
		.procname       = "enforce",
		.data           = &enforce,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
>>>>>>> upstream/android-13
	},
	{ }
};

/*
 * This must be called after early kernel init, since then the rootdev
 * is available.
 */
static void check_pinning_enforcement(struct super_block *mnt_sb)
{
	bool ro = false;

	/*
	 * If load pinning is not enforced via a read-only block
	 * device, allow sysctl to change modes for testing.
	 */
	if (mnt_sb->s_bdev) {
<<<<<<< HEAD
		ro = bdev_read_only(mnt_sb->s_bdev);
		pr_info("dev(%u,%u): %s\n",
=======
		char bdev[BDEVNAME_SIZE];

		ro = bdev_read_only(mnt_sb->s_bdev);
		bdevname(mnt_sb->s_bdev, bdev);
		pr_info("%s (%u:%u): %s\n", bdev,
>>>>>>> upstream/android-13
			MAJOR(mnt_sb->s_bdev->bd_dev),
			MINOR(mnt_sb->s_bdev->bd_dev),
			ro ? "read-only" : "writable");
	} else
		pr_info("mnt_sb lacks block device, treating as: writable\n");

	if (!ro) {
		if (!register_sysctl_paths(loadpin_sysctl_path,
					   loadpin_sysctl_table))
			pr_notice("sysctl registration failed!\n");
		else
<<<<<<< HEAD
			pr_info("load pinning can be disabled.\n");
=======
			pr_info("enforcement can be disabled.\n");
>>>>>>> upstream/android-13
	} else
		pr_info("load pinning engaged.\n");
}
#else
static void check_pinning_enforcement(struct super_block *mnt_sb)
{
	pr_info("load pinning engaged.\n");
}
#endif

static void loadpin_sb_free_security(struct super_block *mnt_sb)
{
	/*
	 * When unmounting the filesystem we were using for load
	 * pinning, we acknowledge the superblock release, but make sure
	 * no other modules or firmware can be loaded.
	 */
	if (!IS_ERR_OR_NULL(pinned_root) && mnt_sb == pinned_root) {
		pinned_root = ERR_PTR(-EIO);
		pr_info("umount pinned fs: refusing further loads\n");
	}
}

<<<<<<< HEAD
static int loadpin_read_file(struct file *file, enum kernel_read_file_id id)
=======
static int loadpin_read_file(struct file *file, enum kernel_read_file_id id,
			     bool contents)
>>>>>>> upstream/android-13
{
	struct super_block *load_root;
	const char *origin = kernel_read_file_id_str(id);

<<<<<<< HEAD
	/* This handles the older init_module API that has a NULL file. */
	if (!file) {
		if (!enabled) {
=======
	/*
	 * If we will not know that we'll be seeing the full contents
	 * then we cannot trust a load will be complete and unchanged
	 * off disk. Treat all contents=false hooks as if there were
	 * no associated file struct.
	 */
	if (!contents)
		file = NULL;

	/* If the file id is excluded, ignore the pinning. */
	if ((unsigned int)id < ARRAY_SIZE(ignore_read_file_id) &&
	    ignore_read_file_id[id]) {
		report_load(origin, file, "pinning-excluded");
		return 0;
	}

	/* This handles the older init_module API that has a NULL file. */
	if (!file) {
		if (!enforce) {
>>>>>>> upstream/android-13
			report_load(origin, NULL, "old-api-pinning-ignored");
			return 0;
		}

		report_load(origin, NULL, "old-api-denied");
		return -EPERM;
	}

	load_root = file->f_path.mnt->mnt_sb;

	/* First loaded module/firmware defines the root for all others. */
	spin_lock(&pinned_root_spinlock);
	/*
	 * pinned_root is only NULL at startup. Otherwise, it is either
	 * a valid reference, or an ERR_PTR.
	 */
	if (!pinned_root) {
		pinned_root = load_root;
		/*
		 * Unlock now since it's only pinned_root we care about.
		 * In the worst case, we will (correctly) report pinning
		 * failures before we have announced that pinning is
<<<<<<< HEAD
		 * enabled. This would be purely cosmetic.
=======
		 * enforcing. This would be purely cosmetic.
>>>>>>> upstream/android-13
		 */
		spin_unlock(&pinned_root_spinlock);
		check_pinning_enforcement(pinned_root);
		report_load(origin, file, "pinned");
	} else {
		spin_unlock(&pinned_root_spinlock);
	}

	if (IS_ERR_OR_NULL(pinned_root) || load_root != pinned_root) {
<<<<<<< HEAD
		if (unlikely(!enabled)) {
=======
		if (unlikely(!enforce)) {
>>>>>>> upstream/android-13
			report_load(origin, file, "pinning-ignored");
			return 0;
		}

		report_load(origin, file, "denied");
		return -EPERM;
	}

	return 0;
}

<<<<<<< HEAD
static int loadpin_load_data(enum kernel_load_data_id id)
{
	return loadpin_read_file(NULL, (enum kernel_read_file_id) id);
=======
static int loadpin_load_data(enum kernel_load_data_id id, bool contents)
{
	return loadpin_read_file(NULL, (enum kernel_read_file_id) id, contents);
>>>>>>> upstream/android-13
}

static struct security_hook_list loadpin_hooks[] __lsm_ro_after_init = {
	LSM_HOOK_INIT(sb_free_security, loadpin_sb_free_security),
	LSM_HOOK_INIT(kernel_read_file, loadpin_read_file),
	LSM_HOOK_INIT(kernel_load_data, loadpin_load_data),
};

<<<<<<< HEAD
void __init loadpin_add_hooks(void)
{
	pr_info("ready to pin (currently %sabled)", enabled ? "en" : "dis");
	security_add_hooks(loadpin_hooks, ARRAY_SIZE(loadpin_hooks), "loadpin");
}

/* Should not be mutable after boot, so not listed in sysfs (perm == 0). */
module_param(enabled, int, 0);
MODULE_PARM_DESC(enabled, "Pin module/firmware loading (default: true)");
=======
static void __init parse_exclude(void)
{
	int i, j;
	char *cur;

	/*
	 * Make sure all the arrays stay within expected sizes. This
	 * is slightly weird because kernel_read_file_str[] includes
	 * READING_MAX_ID, which isn't actually meaningful here.
	 */
	BUILD_BUG_ON(ARRAY_SIZE(exclude_read_files) !=
		     ARRAY_SIZE(ignore_read_file_id));
	BUILD_BUG_ON(ARRAY_SIZE(kernel_read_file_str) <
		     ARRAY_SIZE(ignore_read_file_id));

	for (i = 0; i < ARRAY_SIZE(exclude_read_files); i++) {
		cur = exclude_read_files[i];
		if (!cur)
			break;
		if (*cur == '\0')
			continue;

		for (j = 0; j < ARRAY_SIZE(ignore_read_file_id); j++) {
			if (strcmp(cur, kernel_read_file_str[j]) == 0) {
				pr_info("excluding: %s\n",
					kernel_read_file_str[j]);
				ignore_read_file_id[j] = 1;
				/*
				 * Can not break, because one read_file_str
				 * may map to more than on read_file_id.
				 */
			}
		}
	}
}

static int __init loadpin_init(void)
{
	pr_info("ready to pin (currently %senforcing)\n",
		enforce ? "" : "not ");
	parse_exclude();
	security_add_hooks(loadpin_hooks, ARRAY_SIZE(loadpin_hooks), "loadpin");
	return 0;
}

DEFINE_LSM(loadpin) = {
	.name = "loadpin",
	.init = loadpin_init,
};

/* Should not be mutable after boot, so not listed in sysfs (perm == 0). */
module_param(enforce, int, 0);
MODULE_PARM_DESC(enforce, "Enforce module/firmware pinning");
module_param_array_named(exclude, exclude_read_files, charp, NULL, 0);
MODULE_PARM_DESC(exclude, "Exclude pinning specific read file types");
>>>>>>> upstream/android-13
