/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RAMFS_H
#define _LINUX_RAMFS_H

<<<<<<< HEAD
struct inode *ramfs_get_inode(struct super_block *sb, const struct inode *dir,
	 umode_t mode, dev_t dev);
extern struct dentry *ramfs_mount(struct file_system_type *fs_type,
	 int flags, const char *dev_name, void *data);
=======
#include <linux/fs_parser.h> // bleh...

struct inode *ramfs_get_inode(struct super_block *sb, const struct inode *dir,
	 umode_t mode, dev_t dev);
extern int ramfs_init_fs_context(struct fs_context *fc);
>>>>>>> upstream/android-13

#ifdef CONFIG_MMU
static inline int
ramfs_nommu_expand_for_mapping(struct inode *inode, size_t newsize)
{
	return 0;
}
#else
extern int ramfs_nommu_expand_for_mapping(struct inode *inode, size_t newsize);
#endif

<<<<<<< HEAD
extern const struct file_operations ramfs_file_operations;
extern const struct vm_operations_struct generic_file_vm_ops;
extern int __init init_ramfs_fs(void);

int ramfs_fill_super(struct super_block *sb, void *data, int silent);
=======
extern const struct fs_parameter_spec ramfs_fs_parameters[];
extern const struct file_operations ramfs_file_operations;
extern const struct vm_operations_struct generic_file_vm_ops;
>>>>>>> upstream/android-13

#endif
