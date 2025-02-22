<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2004 IBM Corporation
 * Authors:
 * Leendert van Doorn <leendert@watson.ibm.com>
 * Dave Safford <safford@watson.ibm.com>
 * Reiner Sailer <sailer@watson.ibm.com>
 * Kylene Hall <kjhall@us.ibm.com>
 *
 * Copyright (C) 2013 Obsidian Research Corp
 * Jason Gunthorpe <jgunthorpe@obsidianresearch.com>
 *
 * Device file system interface to the TPM
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
=======
>>>>>>> upstream/android-13
 */
#include <linux/slab.h>
#include "tpm-dev.h"

static int tpm_open(struct inode *inode, struct file *file)
{
	struct tpm_chip *chip;
	struct file_priv *priv;

	chip = container_of(inode->i_cdev, struct tpm_chip, cdev);

	/* It's assured that the chip will be opened just once,
	 * by the check of is_open variable, which is protected
	 * by driver_lock. */
	if (test_and_set_bit(0, &chip->is_open)) {
		dev_dbg(&chip->dev, "Another process owns this TPM\n");
		return -EBUSY;
	}

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		goto out;

<<<<<<< HEAD
	tpm_common_open(file, chip, priv);
=======
	tpm_common_open(file, chip, priv, NULL);
>>>>>>> upstream/android-13

	return 0;

 out:
	clear_bit(0, &chip->is_open);
	return -ENOMEM;
}

<<<<<<< HEAD
static ssize_t tpm_write(struct file *file, const char __user *buf,
			 size_t size, loff_t *off)
{
	return tpm_common_write(file, buf, size, off, NULL);
}

=======
>>>>>>> upstream/android-13
/*
 * Called on file close
 */
static int tpm_release(struct inode *inode, struct file *file)
{
	struct file_priv *priv = file->private_data;

	tpm_common_release(file, priv);
	clear_bit(0, &priv->chip->is_open);
	kfree(priv);

	return 0;
}

const struct file_operations tpm_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.open = tpm_open,
	.read = tpm_common_read,
<<<<<<< HEAD
	.write = tpm_write,
=======
	.write = tpm_common_write,
	.poll = tpm_common_poll,
>>>>>>> upstream/android-13
	.release = tpm_release,
};
