/* SPDX-License-Identifier: GPL-2.0 */

#ifndef BTRFS_SYSFS_H
#define BTRFS_SYSFS_H

<<<<<<< HEAD
/*
 * Data exported through sysfs
 */
extern u64 btrfs_debugfs_test;

enum btrfs_feature_set {
	FEAT_COMPAT = 0,
=======
#include <linux/kobject.h>

enum btrfs_feature_set {
	FEAT_COMPAT,
>>>>>>> upstream/android-13
	FEAT_COMPAT_RO,
	FEAT_INCOMPAT,
	FEAT_MAX
};

<<<<<<< HEAD
#define __INIT_KOBJ_ATTR(_name, _mode, _show, _store)			\
{									\
	.attr	= { .name = __stringify(_name), .mode = _mode },	\
	.show	= _show,						\
	.store	= _store,						\
}

#define BTRFS_ATTR_RW(_prefix, _name, _show, _store)			\
	static struct kobj_attribute btrfs_attr_##_prefix##_##_name =	\
			__INIT_KOBJ_ATTR(_name, 0644, _show, _store)

#define BTRFS_ATTR(_prefix, _name, _show)				\
	static struct kobj_attribute btrfs_attr_##_prefix##_##_name =	\
			__INIT_KOBJ_ATTR(_name, 0444, _show, NULL)

#define BTRFS_ATTR_PTR(_prefix, _name)					\
	(&btrfs_attr_##_prefix##_##_name.attr)


struct btrfs_feature_attr {
	struct kobj_attribute kobj_attr;
	enum btrfs_feature_set feature_set;
	u64 feature_bit;
};

#define BTRFS_FEAT_ATTR(_name, _feature_set, _feature_prefix, _feature_bit)  \
static struct btrfs_feature_attr btrfs_attr_features_##_name = {	     \
	.kobj_attr = __INIT_KOBJ_ATTR(_name, S_IRUGO,			     \
				      btrfs_feature_attr_show,		     \
				      btrfs_feature_attr_store),	     \
	.feature_set	= _feature_set,					     \
	.feature_bit	= _feature_prefix ##_## _feature_bit,		     \
}
#define BTRFS_FEAT_ATTR_PTR(_name)					     \
	(&btrfs_attr_features_##_name.kobj_attr.attr)

#define BTRFS_FEAT_ATTR_COMPAT(name, feature) \
	BTRFS_FEAT_ATTR(name, FEAT_COMPAT, BTRFS_FEATURE_COMPAT, feature)
#define BTRFS_FEAT_ATTR_COMPAT_RO(name, feature) \
	BTRFS_FEAT_ATTR(name, FEAT_COMPAT_RO, BTRFS_FEATURE_COMPAT_RO, feature)
#define BTRFS_FEAT_ATTR_INCOMPAT(name, feature) \
	BTRFS_FEAT_ATTR(name, FEAT_INCOMPAT, BTRFS_FEATURE_INCOMPAT, feature)

/* convert from attribute */
static inline struct btrfs_feature_attr *
to_btrfs_feature_attr(struct kobj_attribute *a)
{
	return container_of(a, struct btrfs_feature_attr, kobj_attr);
}

static inline struct kobj_attribute *attr_to_btrfs_attr(struct attribute *attr)
{
	return container_of(attr, struct kobj_attribute, attr);
}

static inline struct btrfs_feature_attr *
attr_to_btrfs_feature_attr(struct attribute *attr)
{
	return to_btrfs_feature_attr(attr_to_btrfs_attr(attr));
}

char *btrfs_printable_features(enum btrfs_feature_set set, u64 flags);
extern const char * const btrfs_feature_set_names[FEAT_MAX];
extern struct kobj_type space_info_ktype;
extern struct kobj_type btrfs_raid_ktype;
int btrfs_sysfs_add_device_link(struct btrfs_fs_devices *fs_devices,
		struct btrfs_device *one_device);
int btrfs_sysfs_rm_device_link(struct btrfs_fs_devices *fs_devices,
                struct btrfs_device *one_device);
int btrfs_sysfs_add_fsid(struct btrfs_fs_devices *fs_devs,
				struct kobject *parent);
int btrfs_sysfs_add_device(struct btrfs_fs_devices *fs_devs);
void btrfs_sysfs_remove_fsid(struct btrfs_fs_devices *fs_devs);
void btrfs_sysfs_feature_update(struct btrfs_fs_info *fs_info,
		u64 bit, enum btrfs_feature_set set);
=======
char *btrfs_printable_features(enum btrfs_feature_set set, u64 flags);
const char *btrfs_feature_set_name(enum btrfs_feature_set set);
int btrfs_sysfs_add_device(struct btrfs_device *device);
void btrfs_sysfs_remove_device(struct btrfs_device *device);
int btrfs_sysfs_add_fsid(struct btrfs_fs_devices *fs_devs);
void btrfs_sysfs_remove_fsid(struct btrfs_fs_devices *fs_devs);
void btrfs_sysfs_update_sprout_fsid(struct btrfs_fs_devices *fs_devices);
void btrfs_sysfs_feature_update(struct btrfs_fs_info *fs_info,
		u64 bit, enum btrfs_feature_set set);
void btrfs_kobject_uevent(struct block_device *bdev, enum kobject_action action);

int __init btrfs_init_sysfs(void);
void __cold btrfs_exit_sysfs(void);
int btrfs_sysfs_add_mounted(struct btrfs_fs_info *fs_info);
void btrfs_sysfs_remove_mounted(struct btrfs_fs_info *fs_info);
void btrfs_sysfs_add_block_group_type(struct btrfs_block_group *cache);
int btrfs_sysfs_add_space_info_type(struct btrfs_fs_info *fs_info,
				    struct btrfs_space_info *space_info);
void btrfs_sysfs_remove_space_info(struct btrfs_space_info *space_info);
void btrfs_sysfs_update_devid(struct btrfs_device *device);

int btrfs_sysfs_add_one_qgroup(struct btrfs_fs_info *fs_info,
				struct btrfs_qgroup *qgroup);
void btrfs_sysfs_del_qgroups(struct btrfs_fs_info *fs_info);
int btrfs_sysfs_add_qgroups(struct btrfs_fs_info *fs_info);
void btrfs_sysfs_del_one_qgroup(struct btrfs_fs_info *fs_info,
				struct btrfs_qgroup *qgroup);
>>>>>>> upstream/android-13

#endif
