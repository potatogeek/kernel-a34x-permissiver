/* Broadcom NetXtreme-C/E network driver.
 *
 * Copyright (c) 2017-2018 Broadcom Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/pci.h>
#include "bnxt_hsi.h"
<<<<<<< HEAD
#include <linux/net_dim.h>
=======
#include <linux/dim.h>
>>>>>>> upstream/android-13
#include "bnxt.h"
#include "bnxt_debugfs.h"

static struct dentry *bnxt_debug_mnt;

static ssize_t debugfs_dim_read(struct file *filep,
				char __user *buffer,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct net_dim *dim = filep->private_data;
=======
	struct dim *dim = filep->private_data;
>>>>>>> upstream/android-13
	int len;
	char *buf;

	if (*ppos)
		return 0;
	if (!dim)
		return -ENODEV;
	buf = kasprintf(GFP_KERNEL,
			"state = %d\n" \
			"profile_ix = %d\n" \
			"mode = %d\n" \
			"tune_state = %d\n" \
			"steps_right = %d\n" \
			"steps_left = %d\n" \
			"tired = %d\n",
			dim->state,
			dim->profile_ix,
			dim->mode,
			dim->tune_state,
			dim->steps_right,
			dim->steps_left,
			dim->tired);
	if (!buf)
		return -ENOMEM;
	if (count < strlen(buf)) {
		kfree(buf);
		return -ENOSPC;
	}
	len = simple_read_from_buffer(buffer, count, ppos, buf, strlen(buf));
	kfree(buf);
	return len;
}

static const struct file_operations debugfs_dim_fops = {
	.owner = THIS_MODULE,
	.open = simple_open,
	.read = debugfs_dim_read,
};

<<<<<<< HEAD
static struct dentry *debugfs_dim_ring_init(struct net_dim *dim, int ring_idx,
					    struct dentry *dd)
=======
static void debugfs_dim_ring_init(struct dim *dim, int ring_idx,
				  struct dentry *dd)
>>>>>>> upstream/android-13
{
	static char qname[16];

	snprintf(qname, 10, "%d", ring_idx);
<<<<<<< HEAD
	return debugfs_create_file(qname, 0600, dd,
				   dim, &debugfs_dim_fops);
=======
	debugfs_create_file(qname, 0600, dd, dim, &debugfs_dim_fops);
>>>>>>> upstream/android-13
}

void bnxt_debug_dev_init(struct bnxt *bp)
{
	const char *pname = pci_name(bp->pdev);
<<<<<<< HEAD
	struct dentry *pdevf;
	int i;

	bp->debugfs_pdev = debugfs_create_dir(pname, bnxt_debug_mnt);
	if (bp->debugfs_pdev) {
		pdevf = debugfs_create_dir("dim", bp->debugfs_pdev);
		if (!pdevf) {
			pr_err("failed to create debugfs entry %s/dim\n",
			       pname);
			return;
		}
		bp->debugfs_dim = pdevf;
		/* create files for each rx ring */
		for (i = 0; i < bp->cp_nr_rings; i++) {
			struct bnxt_cp_ring_info *cpr = &bp->bnapi[i]->cp_ring;

			if (cpr && bp->bnapi[i]->rx_ring) {
				pdevf = debugfs_dim_ring_init(&cpr->dim, i,
							      bp->debugfs_dim);
				if (!pdevf)
					pr_err("failed to create debugfs entry %s/dim/%d\n",
					       pname, i);
			}
		}
	} else {
		pr_err("failed to create debugfs entry %s\n", pname);
=======
	struct dentry *dir;
	int i;

	bp->debugfs_pdev = debugfs_create_dir(pname, bnxt_debug_mnt);
	dir = debugfs_create_dir("dim", bp->debugfs_pdev);

	/* create files for each rx ring */
	for (i = 0; i < bp->cp_nr_rings; i++) {
		struct bnxt_cp_ring_info *cpr = &bp->bnapi[i]->cp_ring;

		if (cpr && bp->bnapi[i]->rx_ring)
			debugfs_dim_ring_init(&cpr->dim, i, dir);
>>>>>>> upstream/android-13
	}
}

void bnxt_debug_dev_exit(struct bnxt *bp)
{
	if (bp) {
		debugfs_remove_recursive(bp->debugfs_pdev);
		bp->debugfs_pdev = NULL;
	}
}

void bnxt_debug_init(void)
{
	bnxt_debug_mnt = debugfs_create_dir("bnxt_en", NULL);
<<<<<<< HEAD
	if (!bnxt_debug_mnt)
		pr_err("failed to init bnxt_en debugfs\n");
=======
>>>>>>> upstream/android-13
}

void bnxt_debug_exit(void)
{
	debugfs_remove_recursive(bnxt_debug_mnt);
}
