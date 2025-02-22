<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* MTD-based superblock management
 *
 * Copyright © 2001-2007 Red Hat, Inc. All Rights Reserved.
 * Copyright © 2001-2010 David Woodhouse <dwmw2@infradead.org>
 *
 * Written by:  David Howells <dhowells@redhat.com>
 *              David Woodhouse <dwmw2@infradead.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/mtd/super.h>
#include <linux/namei.h>
#include <linux/export.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/major.h>
#include <linux/backing-dev.h>
<<<<<<< HEAD
=======
#include <linux/fs_context.h>
#include "mtdcore.h"
>>>>>>> upstream/android-13

/*
 * compare superblocks to see if they're equivalent
 * - they are if the underlying MTD device is the same
 */
<<<<<<< HEAD
static int get_sb_mtd_compare(struct super_block *sb, void *_mtd)
{
	struct mtd_info *mtd = _mtd;

	if (sb->s_mtd == mtd) {
		pr_debug("MTDSB: Match on device %d (\"%s\")\n",
		      mtd->index, mtd->name);
=======
static int mtd_test_super(struct super_block *sb, struct fs_context *fc)
{
	struct mtd_info *mtd = fc->sget_key;

	if (sb->s_mtd == fc->sget_key) {
		pr_debug("MTDSB: Match on device %d (\"%s\")\n",
			 mtd->index, mtd->name);
>>>>>>> upstream/android-13
		return 1;
	}

	pr_debug("MTDSB: No match, device %d (\"%s\"), device %d (\"%s\")\n",
<<<<<<< HEAD
	      sb->s_mtd->index, sb->s_mtd->name, mtd->index, mtd->name);
	return 0;
}

extern struct backing_dev_info *mtd_bdi;

=======
		 sb->s_mtd->index, sb->s_mtd->name, mtd->index, mtd->name);
	return 0;
}

>>>>>>> upstream/android-13
/*
 * mark the superblock by the MTD device it is using
 * - set the device number to be the correct MTD block device for pesuperstence
 *   of NFS exports
 */
<<<<<<< HEAD
static int get_sb_mtd_set(struct super_block *sb, void *_mtd)
{
	struct mtd_info *mtd = _mtd;

	sb->s_mtd = mtd;
	sb->s_dev = MKDEV(MTD_BLOCK_MAJOR, mtd->index);
	sb->s_bdi = bdi_get(mtd_bdi);

=======
static int mtd_set_super(struct super_block *sb, struct fs_context *fc)
{
	sb->s_mtd = fc->sget_key;
	sb->s_dev = MKDEV(MTD_BLOCK_MAJOR, sb->s_mtd->index);
	sb->s_bdi = bdi_get(mtd_bdi);
>>>>>>> upstream/android-13
	return 0;
}

/*
 * get a superblock on an MTD-backed filesystem
 */
<<<<<<< HEAD
static struct dentry *mount_mtd_aux(struct file_system_type *fs_type, int flags,
			  const char *dev_name, void *data,
			  struct mtd_info *mtd,
			  int (*fill_super)(struct super_block *, void *, int))
=======
static int mtd_get_sb(struct fs_context *fc,
		      struct mtd_info *mtd,
		      int (*fill_super)(struct super_block *,
					struct fs_context *))
>>>>>>> upstream/android-13
{
	struct super_block *sb;
	int ret;

<<<<<<< HEAD
	sb = sget(fs_type, get_sb_mtd_compare, get_sb_mtd_set, flags, mtd);
	if (IS_ERR(sb))
		goto out_error;

	if (sb->s_root)
		goto already_mounted;

	/* fresh new superblock */
	pr_debug("MTDSB: New superblock for device %d (\"%s\")\n",
	      mtd->index, mtd->name);

	ret = fill_super(sb, data, flags & SB_SILENT ? 1 : 0);
	if (ret < 0) {
		deactivate_locked_super(sb);
		return ERR_PTR(ret);
	}

	/* go */
	sb->s_flags |= SB_ACTIVE;
	return dget(sb->s_root);

	/* new mountpoint for an already mounted superblock */
already_mounted:
	pr_debug("MTDSB: Device %d (\"%s\") is already mounted\n",
	      mtd->index, mtd->name);
	put_mtd_device(mtd);
	return dget(sb->s_root);

out_error:
	put_mtd_device(mtd);
	return ERR_CAST(sb);
=======
	fc->sget_key = mtd;
	sb = sget_fc(fc, mtd_test_super, mtd_set_super);
	if (IS_ERR(sb))
		return PTR_ERR(sb);

	if (sb->s_root) {
		/* new mountpoint for an already mounted superblock */
		pr_debug("MTDSB: Device %d (\"%s\") is already mounted\n",
			 mtd->index, mtd->name);
		put_mtd_device(mtd);
	} else {
		/* fresh new superblock */
		pr_debug("MTDSB: New superblock for device %d (\"%s\")\n",
			 mtd->index, mtd->name);

		ret = fill_super(sb, fc);
		if (ret < 0)
			goto error_sb;

		sb->s_flags |= SB_ACTIVE;
	}

	BUG_ON(fc->root);
	fc->root = dget(sb->s_root);
	return 0;

error_sb:
	deactivate_locked_super(sb);
	return ret;
>>>>>>> upstream/android-13
}

/*
 * get a superblock on an MTD-backed filesystem by MTD device number
 */
<<<<<<< HEAD
static struct dentry *mount_mtd_nr(struct file_system_type *fs_type, int flags,
			 const char *dev_name, void *data, int mtdnr,
			 int (*fill_super)(struct super_block *, void *, int))
=======
static int mtd_get_sb_by_nr(struct fs_context *fc, int mtdnr,
			    int (*fill_super)(struct super_block *,
					      struct fs_context *))
>>>>>>> upstream/android-13
{
	struct mtd_info *mtd;

	mtd = get_mtd_device(NULL, mtdnr);
	if (IS_ERR(mtd)) {
<<<<<<< HEAD
		pr_debug("MTDSB: Device #%u doesn't appear to exist\n", mtdnr);
		return ERR_CAST(mtd);
	}

	return mount_mtd_aux(fs_type, flags, dev_name, data, mtd, fill_super);
}

/*
 * set up an MTD-based superblock
 */
struct dentry *mount_mtd(struct file_system_type *fs_type, int flags,
	       const char *dev_name, void *data,
	       int (*fill_super)(struct super_block *, void *, int))
{
#ifdef CONFIG_BLOCK
	struct block_device *bdev;
	int ret, major;
#endif
	int mtdnr;

	if (!dev_name)
		return ERR_PTR(-EINVAL);

	pr_debug("MTDSB: dev_name \"%s\"\n", dev_name);
=======
		errorf(fc, "MTDSB: Device #%u doesn't appear to exist\n", mtdnr);
		return PTR_ERR(mtd);
	}

	return mtd_get_sb(fc, mtd, fill_super);
}

/**
 * get_tree_mtd - Get a superblock based on a single MTD device
 * @fc: The filesystem context holding the parameters
 * @fill_super: Helper to initialise a new superblock
 */
int get_tree_mtd(struct fs_context *fc,
	      int (*fill_super)(struct super_block *sb,
				struct fs_context *fc))
{
#ifdef CONFIG_BLOCK
	dev_t dev;
	int ret;
#endif
	int mtdnr;

	if (!fc->source)
		return invalf(fc, "No source specified");

	pr_debug("MTDSB: dev_name \"%s\"\n", fc->source);
>>>>>>> upstream/android-13

	/* the preferred way of mounting in future; especially when
	 * CONFIG_BLOCK=n - we specify the underlying MTD device by number or
	 * by name, so that we don't require block device support to be present
<<<<<<< HEAD
	 * in the kernel. */
	if (dev_name[0] == 'm' && dev_name[1] == 't' && dev_name[2] == 'd') {
		if (dev_name[3] == ':') {
=======
	 * in the kernel.
	 */
	if (fc->source[0] == 'm' &&
	    fc->source[1] == 't' &&
	    fc->source[2] == 'd') {
		if (fc->source[3] == ':') {
>>>>>>> upstream/android-13
			struct mtd_info *mtd;

			/* mount by MTD device name */
			pr_debug("MTDSB: mtd:%%s, name \"%s\"\n",
<<<<<<< HEAD
			      dev_name + 4);

			mtd = get_mtd_device_nm(dev_name + 4);
			if (!IS_ERR(mtd))
				return mount_mtd_aux(
					fs_type, flags,
					dev_name, data, mtd,
					fill_super);

			printk(KERN_NOTICE "MTD:"
			       " MTD device with name \"%s\" not found.\n",
			       dev_name + 4);

		} else if (isdigit(dev_name[3])) {
			/* mount by MTD device number name */
			char *endptr;

			mtdnr = simple_strtoul(dev_name + 3, &endptr, 0);
			if (!*endptr) {
				/* It was a valid number */
				pr_debug("MTDSB: mtd%%d, mtdnr %d\n",
				      mtdnr);
				return mount_mtd_nr(fs_type, flags,
						     dev_name, data,
						     mtdnr, fill_super);
=======
				 fc->source + 4);

			mtd = get_mtd_device_nm(fc->source + 4);
			if (!IS_ERR(mtd))
				return mtd_get_sb(fc, mtd, fill_super);

			errorf(fc, "MTD: MTD device with name \"%s\" not found",
			       fc->source + 4);

		} else if (isdigit(fc->source[3])) {
			/* mount by MTD device number name */
			char *endptr;

			mtdnr = simple_strtoul(fc->source + 3, &endptr, 0);
			if (!*endptr) {
				/* It was a valid number */
				pr_debug("MTDSB: mtd%%d, mtdnr %d\n", mtdnr);
				return mtd_get_sb_by_nr(fc, mtdnr, fill_super);
>>>>>>> upstream/android-13
			}
		}
	}

#ifdef CONFIG_BLOCK
	/* try the old way - the hack where we allowed users to mount
	 * /dev/mtdblock$(n) but didn't actually _use_ the blockdev
	 */
<<<<<<< HEAD
	bdev = lookup_bdev(dev_name);
	if (IS_ERR(bdev)) {
		ret = PTR_ERR(bdev);
		pr_debug("MTDSB: lookup_bdev() returned %d\n", ret);
		return ERR_PTR(ret);
	}
	pr_debug("MTDSB: lookup_bdev() returned 0\n");

	ret = -EINVAL;

	major = MAJOR(bdev->bd_dev);
	mtdnr = MINOR(bdev->bd_dev);
	bdput(bdev);

	if (major != MTD_BLOCK_MAJOR)
		goto not_an_MTD_device;

	return mount_mtd_nr(fs_type, flags, dev_name, data, mtdnr, fill_super);

not_an_MTD_device:
#endif /* CONFIG_BLOCK */

	if (!(flags & SB_SILENT))
		printk(KERN_NOTICE
		       "MTD: Attempt to mount non-MTD device \"%s\"\n",
		       dev_name);
	return ERR_PTR(-EINVAL);
}

EXPORT_SYMBOL_GPL(mount_mtd);
=======
	ret = lookup_bdev(fc->source, &dev);
	if (ret) {
		errorf(fc, "MTD: Couldn't look up '%s': %d", fc->source, ret);
		return ret;
	}
	pr_debug("MTDSB: lookup_bdev() returned 0\n");

	if (MAJOR(dev) == MTD_BLOCK_MAJOR)
		return mtd_get_sb_by_nr(fc, MINOR(dev), fill_super);

#endif /* CONFIG_BLOCK */

	if (!(fc->sb_flags & SB_SILENT))
		errorf(fc, "MTD: Attempt to mount non-MTD device \"%s\"",
		       fc->source);
	return -EINVAL;
}
EXPORT_SYMBOL_GPL(get_tree_mtd);
>>>>>>> upstream/android-13

/*
 * destroy an MTD-based superblock
 */
void kill_mtd_super(struct super_block *sb)
{
	generic_shutdown_super(sb);
	put_mtd_device(sb->s_mtd);
	sb->s_mtd = NULL;
}

EXPORT_SYMBOL_GPL(kill_mtd_super);
