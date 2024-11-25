// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/proc/root.c
 *
 *  Copyright (C) 1991, 1992 Linus Torvalds
 *
 *  proc root directory handling functions
 */

#include <linux/uaccess.h>

#include <linux/errno.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/stat.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/user_namespace.h>
<<<<<<< HEAD
#include <linux/mount.h>
#include <linux/pid_namespace.h>
#include <linux/parser.h>
#include <linux/cred.h>

#include "internal.h"

static int proc_test_super(struct super_block *sb, void *data)
{
	return sb->s_fs_info == data;
}

static int proc_set_super(struct super_block *sb, void *data)
{
	int err = set_anon_super(sb, NULL);
	if (!err) {
		struct pid_namespace *ns = (struct pid_namespace *)data;
		sb->s_fs_info = get_pid_ns(ns);
	}
	return err;
}

enum {
	Opt_gid, Opt_hidepid, Opt_err,
};

static const match_table_t tokens = {
	{Opt_hidepid, "hidepid=%u"},
	{Opt_gid, "gid=%u"},
	{Opt_err, NULL},
};

static int proc_parse_options(char *options, struct pid_namespace *pid)
{
	char *p;
	substring_t args[MAX_OPT_ARGS];
	int option;

	if (!options)
		return 1;

	while ((p = strsep(&options, ",")) != NULL) {
		int token;
		if (!*p)
			continue;

		args[0].to = args[0].from = NULL;
		token = match_token(p, tokens, args);
		switch (token) {
		case Opt_gid:
			if (match_int(&args[0], &option))
				return 0;
			pid->pid_gid = make_kgid(current_user_ns(), option);
			break;
		case Opt_hidepid:
			if (match_int(&args[0], &option))
				return 0;
			if (option < HIDEPID_OFF ||
			    option > HIDEPID_INVISIBLE) {
				pr_err("proc: hidepid value must be between 0 and 2.\n");
				return 0;
			}
			pid->hide_pid = option;
			break;
		default:
			pr_err("proc: unrecognized mount option \"%s\" "
			       "or missing value\n", p);
			return 0;
		}
	}

	return 1;
}

int proc_remount(struct super_block *sb, int *flags, char *data)
{
	struct pid_namespace *pid = sb->s_fs_info;

	sync_filesystem(sb);
	return !proc_parse_options(data, pid);
}

static struct dentry *proc_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	int err;
	struct super_block *sb;
	struct pid_namespace *ns;
	char *options;

	if (flags & SB_KERNMOUNT) {
		ns = (struct pid_namespace *)data;
		options = NULL;
	} else {
		ns = task_active_pid_ns(current);
		options = data;

		/* Does the mounter have privilege over the pid namespace? */
		if (!ns_capable(ns->user_ns, CAP_SYS_ADMIN))
			return ERR_PTR(-EPERM);
	}

	sb = sget(fs_type, proc_test_super, proc_set_super, flags, ns);
	if (IS_ERR(sb))
		return ERR_CAST(sb);

	if (!proc_parse_options(options, ns)) {
		deactivate_locked_super(sb);
		return ERR_PTR(-EINVAL);
	}

	if (!sb->s_root) {
		err = proc_fill_super(sb);
		if (err) {
			deactivate_locked_super(sb);
			return ERR_PTR(err);
		}

		sb->s_flags |= MS_ACTIVE;
		/* User space would break if executables appear on proc */
		sb->s_iflags |= SB_I_NOEXEC;
	}

	return dget(sb->s_root);
=======
#include <linux/fs_context.h>
#include <linux/mount.h>
#include <linux/pid_namespace.h>
#include <linux/fs_parser.h>
#include <linux/cred.h>
#include <linux/magic.h>
#include <linux/slab.h>

#include "internal.h"

struct proc_fs_context {
	struct pid_namespace	*pid_ns;
	unsigned int		mask;
	enum proc_hidepid	hidepid;
	int			gid;
	enum proc_pidonly	pidonly;
};

enum proc_param {
	Opt_gid,
	Opt_hidepid,
	Opt_subset,
};

static const struct fs_parameter_spec proc_fs_parameters[] = {
	fsparam_u32("gid",	Opt_gid),
	fsparam_string("hidepid",	Opt_hidepid),
	fsparam_string("subset",	Opt_subset),
	{}
};

static inline int valid_hidepid(unsigned int value)
{
	return (value == HIDEPID_OFF ||
		value == HIDEPID_NO_ACCESS ||
		value == HIDEPID_INVISIBLE ||
		value == HIDEPID_NOT_PTRACEABLE);
}

static int proc_parse_hidepid_param(struct fs_context *fc, struct fs_parameter *param)
{
	struct proc_fs_context *ctx = fc->fs_private;
	struct fs_parameter_spec hidepid_u32_spec = fsparam_u32("hidepid", Opt_hidepid);
	struct fs_parse_result result;
	int base = (unsigned long)hidepid_u32_spec.data;

	if (param->type != fs_value_is_string)
		return invalf(fc, "proc: unexpected type of hidepid value\n");

	if (!kstrtouint(param->string, base, &result.uint_32)) {
		if (!valid_hidepid(result.uint_32))
			return invalf(fc, "proc: unknown value of hidepid - %s\n", param->string);
		ctx->hidepid = result.uint_32;
		return 0;
	}

	if (!strcmp(param->string, "off"))
		ctx->hidepid = HIDEPID_OFF;
	else if (!strcmp(param->string, "noaccess"))
		ctx->hidepid = HIDEPID_NO_ACCESS;
	else if (!strcmp(param->string, "invisible"))
		ctx->hidepid = HIDEPID_INVISIBLE;
	else if (!strcmp(param->string, "ptraceable"))
		ctx->hidepid = HIDEPID_NOT_PTRACEABLE;
	else
		return invalf(fc, "proc: unknown value of hidepid - %s\n", param->string);

	return 0;
}

static int proc_parse_subset_param(struct fs_context *fc, char *value)
{
	struct proc_fs_context *ctx = fc->fs_private;

	while (value) {
		char *ptr = strchr(value, ',');

		if (ptr != NULL)
			*ptr++ = '\0';

		if (*value != '\0') {
			if (!strcmp(value, "pid")) {
				ctx->pidonly = PROC_PIDONLY_ON;
			} else {
				return invalf(fc, "proc: unsupported subset option - %s\n", value);
			}
		}
		value = ptr;
	}

	return 0;
}

static int proc_parse_param(struct fs_context *fc, struct fs_parameter *param)
{
	struct proc_fs_context *ctx = fc->fs_private;
	struct fs_parse_result result;
	int opt;

	opt = fs_parse(fc, proc_fs_parameters, param, &result);
	if (opt < 0)
		return opt;

	switch (opt) {
	case Opt_gid:
		ctx->gid = result.uint_32;
		break;

	case Opt_hidepid:
		if (proc_parse_hidepid_param(fc, param))
			return -EINVAL;
		break;

	case Opt_subset:
		if (proc_parse_subset_param(fc, param->string) < 0)
			return -EINVAL;
		break;

	default:
		return -EINVAL;
	}

	ctx->mask |= 1 << opt;
	return 0;
}

static void proc_apply_options(struct proc_fs_info *fs_info,
			       struct fs_context *fc,
			       struct user_namespace *user_ns)
{
	struct proc_fs_context *ctx = fc->fs_private;

	if (ctx->mask & (1 << Opt_gid))
		fs_info->pid_gid = make_kgid(user_ns, ctx->gid);
	if (ctx->mask & (1 << Opt_hidepid))
		fs_info->hide_pid = ctx->hidepid;
	if (ctx->mask & (1 << Opt_subset))
		fs_info->pidonly = ctx->pidonly;
}

static int proc_fill_super(struct super_block *s, struct fs_context *fc)
{
	struct proc_fs_context *ctx = fc->fs_private;
	struct inode *root_inode;
	struct proc_fs_info *fs_info;
	int ret;

	fs_info = kzalloc(sizeof(*fs_info), GFP_KERNEL);
	if (!fs_info)
		return -ENOMEM;

	fs_info->pid_ns = get_pid_ns(ctx->pid_ns);
	proc_apply_options(fs_info, fc, current_user_ns());

	/* User space would break if executables or devices appear on proc */
	s->s_iflags |= SB_I_USERNS_VISIBLE | SB_I_NOEXEC | SB_I_NODEV;
	s->s_flags |= SB_NODIRATIME | SB_NOSUID | SB_NOEXEC;
	s->s_blocksize = 1024;
	s->s_blocksize_bits = 10;
	s->s_magic = PROC_SUPER_MAGIC;
	s->s_op = &proc_sops;
	s->s_time_gran = 1;
	s->s_fs_info = fs_info;

	/*
	 * procfs isn't actually a stacking filesystem; however, there is
	 * too much magic going on inside it to permit stacking things on
	 * top of it
	 */
	s->s_stack_depth = FILESYSTEM_MAX_STACK_DEPTH;

	/* procfs dentries and inodes don't require IO to create */
	s->s_shrink.seeks = 0;

	pde_get(&proc_root);
	root_inode = proc_get_inode(s, &proc_root);
	if (!root_inode) {
		pr_err("proc_fill_super: get root inode failed\n");
		return -ENOMEM;
	}

	s->s_root = d_make_root(root_inode);
	if (!s->s_root) {
		pr_err("proc_fill_super: allocate dentry failed\n");
		return -ENOMEM;
	}

	ret = proc_setup_self(s);
	if (ret) {
		return ret;
	}
	return proc_setup_thread_self(s);
}

static int proc_reconfigure(struct fs_context *fc)
{
	struct super_block *sb = fc->root->d_sb;
	struct proc_fs_info *fs_info = proc_sb_info(sb);

	sync_filesystem(sb);

	proc_apply_options(fs_info, fc, current_user_ns());
	return 0;
}

static int proc_get_tree(struct fs_context *fc)
{
	return get_tree_nodev(fc, proc_fill_super);
}

static void proc_fs_context_free(struct fs_context *fc)
{
	struct proc_fs_context *ctx = fc->fs_private;

	put_pid_ns(ctx->pid_ns);
	kfree(ctx);
}

static const struct fs_context_operations proc_fs_context_ops = {
	.free		= proc_fs_context_free,
	.parse_param	= proc_parse_param,
	.get_tree	= proc_get_tree,
	.reconfigure	= proc_reconfigure,
};

static int proc_init_fs_context(struct fs_context *fc)
{
	struct proc_fs_context *ctx;

	ctx = kzalloc(sizeof(struct proc_fs_context), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->pid_ns = get_pid_ns(task_active_pid_ns(current));
	put_user_ns(fc->user_ns);
	fc->user_ns = get_user_ns(ctx->pid_ns->user_ns);
	fc->fs_private = ctx;
	fc->ops = &proc_fs_context_ops;
	return 0;
>>>>>>> upstream/android-13
}

static void proc_kill_sb(struct super_block *sb)
{
<<<<<<< HEAD
	struct pid_namespace *ns;

	ns = (struct pid_namespace *)sb->s_fs_info;
	if (ns->proc_self)
		dput(ns->proc_self);
	if (ns->proc_thread_self)
		dput(ns->proc_thread_self);
	kill_anon_super(sb);
	put_pid_ns(ns);
}

static struct file_system_type proc_fs_type = {
	.name		= "proc",
	.mount		= proc_mount,
	.kill_sb	= proc_kill_sb,
	.fs_flags	= FS_USERNS_MOUNT,
=======
	struct proc_fs_info *fs_info = proc_sb_info(sb);

	if (!fs_info) {
		kill_anon_super(sb);
		return;
	}

	dput(fs_info->proc_self);
	dput(fs_info->proc_thread_self);

	kill_anon_super(sb);
	put_pid_ns(fs_info->pid_ns);
	kfree(fs_info);
}

static struct file_system_type proc_fs_type = {
	.name			= "proc",
	.init_fs_context	= proc_init_fs_context,
	.parameters		= proc_fs_parameters,
	.kill_sb		= proc_kill_sb,
	.fs_flags		= FS_USERNS_MOUNT | FS_DISALLOW_NOTIFY_PERM,
>>>>>>> upstream/android-13
};

void __init proc_root_init(void)
{
	proc_init_kmemcache();
	set_proc_pid_nlink();
	proc_self_init();
	proc_thread_self_init();
	proc_symlink("mounts", NULL, "self/mounts");

	proc_net_init();
<<<<<<< HEAD
	proc_uid_init();
=======
>>>>>>> upstream/android-13
	proc_mkdir("fs", NULL);
	proc_mkdir("driver", NULL);
	proc_create_mount_point("fs/nfsd"); /* somewhere for the nfsd filesystem to be mounted */
#if defined(CONFIG_SUN_OPENPROMFS) || defined(CONFIG_SUN_OPENPROMFS_MODULE)
	/* just give it a mountpoint */
	proc_create_mount_point("openprom");
#endif
	proc_tty_init();
	proc_mkdir("bus", NULL);
	proc_sys_init();

	register_filesystem(&proc_fs_type);
}

<<<<<<< HEAD
static int proc_root_getattr(const struct path *path, struct kstat *stat,
			     u32 request_mask, unsigned int query_flags)
{
	generic_fillattr(d_inode(path->dentry), stat);
=======
static int proc_root_getattr(struct user_namespace *mnt_userns,
			     const struct path *path, struct kstat *stat,
			     u32 request_mask, unsigned int query_flags)
{
	generic_fillattr(&init_user_ns, d_inode(path->dentry), stat);
>>>>>>> upstream/android-13
	stat->nlink = proc_root.nlink + nr_processes();
	return 0;
}

static struct dentry *proc_root_lookup(struct inode * dir, struct dentry * dentry, unsigned int flags)
{
<<<<<<< HEAD
	if (!proc_pid_lookup(dir, dentry, flags))
		return NULL;
	
=======
	if (!proc_pid_lookup(dentry, flags))
		return NULL;

>>>>>>> upstream/android-13
	return proc_lookup(dir, dentry, flags);
}

static int proc_root_readdir(struct file *file, struct dir_context *ctx)
{
	if (ctx->pos < FIRST_PROCESS_ENTRY) {
		int error = proc_readdir(file, ctx);
		if (unlikely(error <= 0))
			return error;
		ctx->pos = FIRST_PROCESS_ENTRY;
	}

	return proc_pid_readdir(file, ctx);
}

/*
 * The root /proc directory is special, as it has the
 * <pid> directories. Thus we don't use the generic
 * directory handling functions for that..
 */
static const struct file_operations proc_root_operations = {
	.read		 = generic_read_dir,
	.iterate_shared	 = proc_root_readdir,
	.llseek		= generic_file_llseek,
};

/*
 * proc root can do almost nothing..
 */
static const struct inode_operations proc_root_inode_operations = {
	.lookup		= proc_root_lookup,
	.getattr	= proc_root_getattr,
};

/*
 * This is the root "inode" in the /proc tree..
 */
struct proc_dir_entry proc_root = {
	.low_ino	= PROC_ROOT_INO, 
	.namelen	= 5, 
	.mode		= S_IFDIR | S_IRUGO | S_IXUGO, 
	.nlink		= 2, 
	.refcnt		= REFCOUNT_INIT(1),
	.proc_iops	= &proc_root_inode_operations, 
<<<<<<< HEAD
	.proc_fops	= &proc_root_operations,
=======
	.proc_dir_ops	= &proc_root_operations,
>>>>>>> upstream/android-13
	.parent		= &proc_root,
	.subdir		= RB_ROOT,
	.name		= "/proc",
};
<<<<<<< HEAD

int pid_ns_prepare_proc(struct pid_namespace *ns)
{
	struct vfsmount *mnt;

	mnt = kern_mount_data(&proc_fs_type, ns);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	ns->proc_mnt = mnt;
	return 0;
}

void pid_ns_release_proc(struct pid_namespace *ns)
{
	kern_unmount(ns->proc_mnt);
}
=======
>>>>>>> upstream/android-13
