<<<<<<< HEAD
/*
 * Copyright (C) 2017 James.Bottomley@HansenPartnership.com
 *
 * GPLv2
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2017 James.Bottomley@HansenPartnership.com
>>>>>>> upstream/android-13
 */
#include <linux/slab.h>
#include "tpm-dev.h"

struct tpmrm_priv {
	struct file_priv priv;
	struct tpm_space space;
};

static int tpmrm_open(struct inode *inode, struct file *file)
{
	struct tpm_chip *chip;
	struct tpmrm_priv *priv;
	int rc;

	chip = container_of(inode->i_cdev, struct tpm_chip, cdevs);
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	rc = tpm2_init_space(&priv->space, TPM2_SPACE_BUFFER_SIZE);
	if (rc) {
		kfree(priv);
		return -ENOMEM;
	}

<<<<<<< HEAD
	tpm_common_open(file, chip, &priv->priv);
=======
	tpm_common_open(file, chip, &priv->priv, &priv->space);
>>>>>>> upstream/android-13

	return 0;
}

static int tpmrm_release(struct inode *inode, struct file *file)
{
	struct file_priv *fpriv = file->private_data;
	struct tpmrm_priv *priv = container_of(fpriv, struct tpmrm_priv, priv);

	tpm_common_release(file, fpriv);
	tpm2_del_space(fpriv->chip, &priv->space);
	kfree(priv);

	return 0;
}

<<<<<<< HEAD
static ssize_t tpmrm_write(struct file *file, const char __user *buf,
		   size_t size, loff_t *off)
{
	struct file_priv *fpriv = file->private_data;
	struct tpmrm_priv *priv = container_of(fpriv, struct tpmrm_priv, priv);

	return tpm_common_write(file, buf, size, off, &priv->space);
}

=======
>>>>>>> upstream/android-13
const struct file_operations tpmrm_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.open = tpmrm_open,
	.read = tpm_common_read,
<<<<<<< HEAD
	.write = tpmrm_write,
	.release = tpmrm_release,
};

=======
	.write = tpm_common_write,
	.poll = tpm_common_poll,
	.release = tpmrm_release,
};
>>>>>>> upstream/android-13
