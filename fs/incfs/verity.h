/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright 2020 Google LLC
 */

#ifndef _INCFS_VERITY_H
#define _INCFS_VERITY_H

/* Arbitrary limit to bound the kmalloc() size.  Can be changed. */
#define FS_VERITY_MAX_SIGNATURE_SIZE	16128

#ifdef CONFIG_FS_VERITY

int incfs_ioctl_enable_verity(struct file *filp, const void __user *uarg);
int incfs_ioctl_measure_verity(struct file *filp, void __user *_uarg);

int incfs_fsverity_file_open(struct inode *inode, struct file *filp);
<<<<<<< HEAD
=======
int incfs_ioctl_read_verity_metadata(struct file *filp,
				     const void __user *uarg);
>>>>>>> upstream/android-13

#else /* !CONFIG_FS_VERITY */

static inline int incfs_ioctl_enable_verity(struct file *filp,
					    const void __user *uarg)
{
	return -EOPNOTSUPP;
}

static inline int incfs_ioctl_measure_verity(struct file *filp,
					     void __user *_uarg)
{
	return -EOPNOTSUPP;
}

static inline int incfs_fsverity_file_open(struct inode *inode,
					   struct file *filp)
{
	return -EOPNOTSUPP;
}

<<<<<<< HEAD
=======
static inline int incfs_ioctl_read_verity_metadata(struct file *filp,
						const void __user *uarg)
{
	return -EOPNOTSUPP;
}

>>>>>>> upstream/android-13
#endif /* !CONFIG_FS_VERITY */

#endif
