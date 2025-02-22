<<<<<<< HEAD
/* -*- mode: c; c-basic-offset:8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * configfs_internal.h - Internal stuff for configfs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * configfs_internal.h - Internal stuff for configfs
 *
>>>>>>> upstream/android-13
 * Based on sysfs:
 * 	sysfs is Copyright (C) 2001, 2002, 2003 Patrick Mochel
 *
 * configfs Copyright (C) 2005 Oracle.  All rights reserved.
 */

#ifdef pr_fmt
#undef pr_fmt
#endif

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>

struct configfs_fragment {
	atomic_t frag_count;
	struct rw_semaphore frag_sem;
	bool frag_dead;
};

void put_fragment(struct configfs_fragment *);
struct configfs_fragment *get_fragment(struct configfs_fragment *);

struct configfs_dirent {
	atomic_t		s_count;
	int			s_dependent_count;
	struct list_head	s_sibling;
	struct list_head	s_children;
<<<<<<< HEAD
	struct list_head	s_links;
=======
	int			s_links;
>>>>>>> upstream/android-13
	void			* s_element;
	int			s_type;
	umode_t			s_mode;
	struct dentry		* s_dentry;
	struct iattr		* s_iattr;
#ifdef CONFIG_LOCKDEP
	int			s_depth;
#endif
	struct configfs_fragment *s_frag;
};

#define CONFIGFS_ROOT		0x0001
#define CONFIGFS_DIR		0x0002
#define CONFIGFS_ITEM_ATTR	0x0004
#define CONFIGFS_ITEM_BIN_ATTR	0x0008
#define CONFIGFS_ITEM_LINK	0x0020
#define CONFIGFS_USET_DIR	0x0040
#define CONFIGFS_USET_DEFAULT	0x0080
#define CONFIGFS_USET_DROPPING	0x0100
#define CONFIGFS_USET_IN_MKDIR	0x0200
#define CONFIGFS_USET_CREATING	0x0400
#define CONFIGFS_NOT_PINNED	(CONFIGFS_ITEM_ATTR | CONFIGFS_ITEM_BIN_ATTR)

extern struct mutex configfs_symlink_mutex;
extern spinlock_t configfs_dirent_lock;

extern struct kmem_cache *configfs_dir_cachep;

extern int configfs_is_root(struct config_item *item);

extern struct inode * configfs_new_inode(umode_t mode, struct configfs_dirent *, struct super_block *);
<<<<<<< HEAD
extern int configfs_create(struct dentry *, umode_t mode, void (*init)(struct inode *));
=======
extern struct inode *configfs_create(struct dentry *, umode_t mode);
>>>>>>> upstream/android-13

extern int configfs_create_file(struct config_item *, const struct configfs_attribute *);
extern int configfs_create_bin_file(struct config_item *,
				    const struct configfs_bin_attribute *);
extern int configfs_make_dirent(struct configfs_dirent *, struct dentry *,
				void *, umode_t, int, struct configfs_fragment *);
extern int configfs_dirent_is_ready(struct configfs_dirent *);

extern void configfs_hash_and_remove(struct dentry * dir, const char * name);

extern const unsigned char * configfs_get_name(struct configfs_dirent *sd);
extern void configfs_drop_dentry(struct configfs_dirent *sd, struct dentry *parent);
<<<<<<< HEAD
extern int configfs_setattr(struct dentry *dentry, struct iattr *iattr);
=======
extern int configfs_setattr(struct user_namespace *mnt_userns,
			    struct dentry *dentry, struct iattr *iattr);
>>>>>>> upstream/android-13

extern struct dentry *configfs_pin_fs(void);
extern void configfs_release_fs(void);

<<<<<<< HEAD
extern struct rw_semaphore configfs_rename_sem;
=======
>>>>>>> upstream/android-13
extern const struct file_operations configfs_dir_operations;
extern const struct file_operations configfs_file_operations;
extern const struct file_operations configfs_bin_file_operations;
extern const struct inode_operations configfs_dir_inode_operations;
extern const struct inode_operations configfs_root_inode_operations;
extern const struct inode_operations configfs_symlink_inode_operations;
extern const struct dentry_operations configfs_dentry_ops;

<<<<<<< HEAD
extern int configfs_symlink(struct inode *dir, struct dentry *dentry,
			    const char *symname);
extern int configfs_unlink(struct inode *dir, struct dentry *dentry);

struct configfs_symlink {
	struct list_head sl_list;
	struct config_item *sl_target;
};

extern int configfs_create_link(struct configfs_symlink *sl,
				struct dentry *parent,
				struct dentry *dentry);
=======
extern int configfs_symlink(struct user_namespace *mnt_userns,
			    struct inode *dir, struct dentry *dentry,
			    const char *symname);
extern int configfs_unlink(struct inode *dir, struct dentry *dentry);

int configfs_create_link(struct configfs_dirent *target, struct dentry *parent,
		struct dentry *dentry, char *body);
>>>>>>> upstream/android-13

static inline struct config_item * to_item(struct dentry * dentry)
{
	struct configfs_dirent * sd = dentry->d_fsdata;
	return ((struct config_item *) sd->s_element);
}

static inline struct configfs_attribute * to_attr(struct dentry * dentry)
{
	struct configfs_dirent * sd = dentry->d_fsdata;
	return ((struct configfs_attribute *) sd->s_element);
}

static inline struct configfs_bin_attribute *to_bin_attr(struct dentry *dentry)
{
	struct configfs_attribute *attr = to_attr(dentry);

	return container_of(attr, struct configfs_bin_attribute, cb_attr);
}

static inline struct config_item *configfs_get_config_item(struct dentry *dentry)
{
	struct config_item * item = NULL;

	spin_lock(&dentry->d_lock);
	if (!d_unhashed(dentry)) {
		struct configfs_dirent * sd = dentry->d_fsdata;
<<<<<<< HEAD
		if (sd->s_type & CONFIGFS_ITEM_LINK) {
			struct configfs_symlink * sl = sd->s_element;
			item = config_item_get(sl->sl_target);
		} else
			item = config_item_get(sd->s_element);
=======
		item = config_item_get(sd->s_element);
>>>>>>> upstream/android-13
	}
	spin_unlock(&dentry->d_lock);

	return item;
}

static inline void release_configfs_dirent(struct configfs_dirent * sd)
{
	if (!(sd->s_type & CONFIGFS_ROOT)) {
		kfree(sd->s_iattr);
		put_fragment(sd->s_frag);
		kmem_cache_free(configfs_dir_cachep, sd);
	}
}

static inline struct configfs_dirent * configfs_get(struct configfs_dirent * sd)
{
	if (sd) {
		WARN_ON(!atomic_read(&sd->s_count));
		atomic_inc(&sd->s_count);
	}
	return sd;
}

static inline void configfs_put(struct configfs_dirent * sd)
{
	WARN_ON(!atomic_read(&sd->s_count));
	if (atomic_dec_and_test(&sd->s_count))
		release_configfs_dirent(sd);
}

