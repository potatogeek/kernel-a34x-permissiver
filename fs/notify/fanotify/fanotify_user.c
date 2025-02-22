// SPDX-License-Identifier: GPL-2.0
#include <linux/fanotify.h>
#include <linux/fcntl.h>
<<<<<<< HEAD
=======
#include <linux/fdtable.h>
>>>>>>> upstream/android-13
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/anon_inodes.h>
#include <linux/fsnotify_backend.h>
#include <linux/init.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/poll.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include <linux/sched/signal.h>
#include <linux/memcontrol.h>
<<<<<<< HEAD
=======
#include <linux/statfs.h>
#include <linux/exportfs.h>
>>>>>>> upstream/android-13

#include <asm/ioctls.h>

#include "../../mount.h"
#include "../fdinfo.h"
#include "fanotify.h"

#define FANOTIFY_DEFAULT_MAX_EVENTS	16384
<<<<<<< HEAD
#define FANOTIFY_DEFAULT_MAX_MARKS	8192
#define FANOTIFY_DEFAULT_MAX_LISTENERS	128
=======
#define FANOTIFY_OLD_DEFAULT_MAX_MARKS	8192
#define FANOTIFY_DEFAULT_MAX_GROUPS	128

/*
 * Legacy fanotify marks limits (8192) is per group and we introduced a tunable
 * limit of marks per user, similar to inotify.  Effectively, the legacy limit
 * of fanotify marks per user is <max marks per group> * <max groups per user>.
 * This default limit (1M) also happens to match the increased limit of inotify
 * max_user_watches since v5.10.
 */
#define FANOTIFY_DEFAULT_MAX_USER_MARKS	\
	(FANOTIFY_OLD_DEFAULT_MAX_MARKS * FANOTIFY_DEFAULT_MAX_GROUPS)

/*
 * Most of the memory cost of adding an inode mark is pinning the marked inode.
 * The size of the filesystem inode struct is not uniform across filesystems,
 * so double the size of a VFS inode is used as a conservative approximation.
 */
#define INODE_MARK_COST	(2 * sizeof(struct inode))

/* configurable via /proc/sys/fs/fanotify/ */
static int fanotify_max_queued_events __read_mostly;

#ifdef CONFIG_SYSCTL

#include <linux/sysctl.h>

static long ft_zero = 0;
static long ft_int_max = INT_MAX;

struct ctl_table fanotify_table[] = {
	{
		.procname	= "max_user_groups",
		.data	= &init_user_ns.ucount_max[UCOUNT_FANOTIFY_GROUPS],
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= &ft_zero,
		.extra2		= &ft_int_max,
	},
	{
		.procname	= "max_user_marks",
		.data	= &init_user_ns.ucount_max[UCOUNT_FANOTIFY_MARKS],
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= &ft_zero,
		.extra2		= &ft_int_max,
	},
	{
		.procname	= "max_queued_events",
		.data		= &fanotify_max_queued_events,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO
	},
	{ }
};
#endif /* CONFIG_SYSCTL */
>>>>>>> upstream/android-13

/*
 * All flags that may be specified in parameter event_f_flags of fanotify_init.
 *
 * Internal and external open flags are stored together in field f_flags of
 * struct file. Only external open flags shall be allowed in event_f_flags.
 * Internal flags like FMODE_NONOTIFY, FMODE_EXEC, FMODE_NOCMTIME shall be
 * excluded.
 */
#define	FANOTIFY_INIT_ALL_EVENT_F_BITS				( \
		O_ACCMODE	| O_APPEND	| O_NONBLOCK	| \
		__O_SYNC	| O_DSYNC	| O_CLOEXEC     | \
		O_LARGEFILE	| O_NOATIME	)

extern const struct fsnotify_ops fanotify_fsnotify_ops;

struct kmem_cache *fanotify_mark_cache __read_mostly;
<<<<<<< HEAD
struct kmem_cache *fanotify_event_cachep __read_mostly;
struct kmem_cache *fanotify_perm_event_cachep __read_mostly;

/*
 * Get an fsnotify notification event if one exists and is small
 * enough to fit in "count". Return an error pointer if the count
 * is not large enough.
 *
 * Called with the group->notification_lock held.
 */
static struct fsnotify_event *get_one_event(struct fsnotify_group *group,
					    size_t count)
{
	assert_spin_locked(&group->notification_lock);

	pr_debug("%s: group=%p count=%zd\n", __func__, group, count);

	if (fsnotify_notify_queue_is_empty(group))
		return NULL;

	if (FAN_EVENT_METADATA_LEN > count)
		return ERR_PTR(-EINVAL);

	/* held the notification_lock the whole time, so this is the
	 * same event we peeked above */
	return fsnotify_remove_first_event(group);
}

static int create_fd(struct fsnotify_group *group,
		     struct fanotify_event_info *event,
=======
struct kmem_cache *fanotify_fid_event_cachep __read_mostly;
struct kmem_cache *fanotify_path_event_cachep __read_mostly;
struct kmem_cache *fanotify_perm_event_cachep __read_mostly;

#define FANOTIFY_EVENT_ALIGN 4
#define FANOTIFY_FID_INFO_HDR_LEN \
	(sizeof(struct fanotify_event_info_fid) + sizeof(struct file_handle))
#define FANOTIFY_PIDFD_INFO_HDR_LEN \
	sizeof(struct fanotify_event_info_pidfd)

static int fanotify_fid_info_len(int fh_len, int name_len)
{
	int info_len = fh_len;

	if (name_len)
		info_len += name_len + 1;

	return roundup(FANOTIFY_FID_INFO_HDR_LEN + info_len,
		       FANOTIFY_EVENT_ALIGN);
}

static int fanotify_event_info_len(unsigned int info_mode,
				   struct fanotify_event *event)
{
	struct fanotify_info *info = fanotify_event_info(event);
	int dir_fh_len = fanotify_event_dir_fh_len(event);
	int fh_len = fanotify_event_object_fh_len(event);
	int info_len = 0;
	int dot_len = 0;

	if (dir_fh_len) {
		info_len += fanotify_fid_info_len(dir_fh_len, info->name_len);
	} else if ((info_mode & FAN_REPORT_NAME) &&
		   (event->mask & FAN_ONDIR)) {
		/*
		 * With group flag FAN_REPORT_NAME, if name was not recorded in
		 * event on a directory, we will report the name ".".
		 */
		dot_len = 1;
	}

	if (info_mode & FAN_REPORT_PIDFD)
		info_len += FANOTIFY_PIDFD_INFO_HDR_LEN;

	if (fh_len)
		info_len += fanotify_fid_info_len(fh_len, dot_len);

	return info_len;
}

/*
 * Remove an hashed event from merge hash table.
 */
static void fanotify_unhash_event(struct fsnotify_group *group,
				  struct fanotify_event *event)
{
	assert_spin_locked(&group->notification_lock);

	pr_debug("%s: group=%p event=%p bucket=%u\n", __func__,
		 group, event, fanotify_event_hash_bucket(group, event));

	if (WARN_ON_ONCE(hlist_unhashed(&event->merge_list)))
		return;

	hlist_del_init(&event->merge_list);
}

/*
 * Get an fanotify notification event if one exists and is small
 * enough to fit in "count". Return an error pointer if the count
 * is not large enough. When permission event is dequeued, its state is
 * updated accordingly.
 */
static struct fanotify_event *get_one_event(struct fsnotify_group *group,
					    size_t count)
{
	size_t event_size = FAN_EVENT_METADATA_LEN;
	struct fanotify_event *event = NULL;
	struct fsnotify_event *fsn_event;
	unsigned int info_mode = FAN_GROUP_FLAG(group, FANOTIFY_INFO_MODES);

	pr_debug("%s: group=%p count=%zd\n", __func__, group, count);

	spin_lock(&group->notification_lock);
	fsn_event = fsnotify_peek_first_event(group);
	if (!fsn_event)
		goto out;

	event = FANOTIFY_E(fsn_event);
	if (info_mode)
		event_size += fanotify_event_info_len(info_mode, event);

	if (event_size > count) {
		event = ERR_PTR(-EINVAL);
		goto out;
	}

	/*
	 * Held the notification_lock the whole time, so this is the
	 * same event we peeked above.
	 */
	fsnotify_remove_first_event(group);
	if (fanotify_is_perm_event(event->mask))
		FANOTIFY_PERM(event)->state = FAN_EVENT_REPORTED;
	if (fanotify_is_hashed_event(event->mask))
		fanotify_unhash_event(group, event);
out:
	spin_unlock(&group->notification_lock);
	return event;
}

static int create_fd(struct fsnotify_group *group, struct path *path,
>>>>>>> upstream/android-13
		     struct file **file)
{
	int client_fd;
	struct file *new_file;

<<<<<<< HEAD
	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

=======
>>>>>>> upstream/android-13
	client_fd = get_unused_fd_flags(group->fanotify_data.f_flags);
	if (client_fd < 0)
		return client_fd;

	/*
	 * we need a new file handle for the userspace program so it can read even if it was
	 * originally opened O_WRONLY.
	 */
<<<<<<< HEAD
	/* it's possible this event was an overflow event.  in that case dentry and mnt
	 * are NULL;  That's fine, just don't call dentry open */
	if (event->path.dentry && event->path.mnt)
		new_file = dentry_open(&event->path,
				       group->fanotify_data.f_flags | FMODE_NONOTIFY,
				       current_cred());
	else
		new_file = ERR_PTR(-EOVERFLOW);
=======
	new_file = dentry_open(path,
			       group->fanotify_data.f_flags | FMODE_NONOTIFY,
			       current_cred());
>>>>>>> upstream/android-13
	if (IS_ERR(new_file)) {
		/*
		 * we still send an event even if we can't open the file.  this
		 * can happen when say tasks are gone and we try to open their
		 * /proc files or we try to open a WRONLY file like in sysfs
		 * we just send the errno to userspace since there isn't much
		 * else we can do.
		 */
		put_unused_fd(client_fd);
		client_fd = PTR_ERR(new_file);
	} else {
		*file = new_file;
	}

	return client_fd;
}

<<<<<<< HEAD
static int fill_event_metadata(struct fsnotify_group *group,
			       struct fanotify_event_metadata *metadata,
			       struct fsnotify_event *fsn_event,
			       struct file **file)
{
	int ret = 0;
	struct fanotify_event_info *event;

	pr_debug("%s: group=%p metadata=%p event=%p\n", __func__,
		 group, metadata, fsn_event);

	*file = NULL;
	event = container_of(fsn_event, struct fanotify_event_info, fse);
	metadata->event_len = FAN_EVENT_METADATA_LEN;
	metadata->metadata_len = FAN_EVENT_METADATA_LEN;
	metadata->vers = FANOTIFY_METADATA_VERSION;
	metadata->reserved = 0;
	metadata->mask = fsn_event->mask & FAN_ALL_OUTGOING_EVENTS;
	metadata->pid = pid_vnr(event->tgid);
	if (unlikely(fsn_event->mask & FAN_Q_OVERFLOW))
		metadata->fd = FAN_NOFD;
	else {
		metadata->fd = create_fd(group, event, file);
		if (metadata->fd < 0)
			ret = metadata->fd;
	}

	return ret;
}

static struct fanotify_perm_event_info *dequeue_event(
				struct fsnotify_group *group, int fd)
{
	struct fanotify_perm_event_info *event, *return_e = NULL;

	spin_lock(&group->notification_lock);
	list_for_each_entry(event, &group->fanotify_data.access_list,
			    fae.fse.list) {
		if (event->fd != fd)
			continue;

		list_del_init(&event->fae.fse.list);
		return_e = event;
		break;
	}
	spin_unlock(&group->notification_lock);

	pr_debug("%s: found return_re=%p\n", __func__, return_e);

	return return_e;
=======
/*
 * Finish processing of permission event by setting it to ANSWERED state and
 * drop group->notification_lock.
 */
static void finish_permission_event(struct fsnotify_group *group,
				    struct fanotify_perm_event *event,
				    unsigned int response)
				    __releases(&group->notification_lock)
{
	bool destroy = false;

	assert_spin_locked(&group->notification_lock);
	event->response = response;
	if (event->state == FAN_EVENT_CANCELED)
		destroy = true;
	else
		event->state = FAN_EVENT_ANSWERED;
	spin_unlock(&group->notification_lock);
	if (destroy)
		fsnotify_destroy_event(group, &event->fae.fse);
>>>>>>> upstream/android-13
}

static int process_access_response(struct fsnotify_group *group,
				   struct fanotify_response *response_struct)
{
<<<<<<< HEAD
	struct fanotify_perm_event_info *event;
=======
	struct fanotify_perm_event *event;
>>>>>>> upstream/android-13
	int fd = response_struct->fd;
	int response = response_struct->response;

	pr_debug("%s: group=%p fd=%d response=%d\n", __func__, group,
		 fd, response);
	/*
	 * make sure the response is valid, if invalid we do nothing and either
	 * userspace can send a valid response or we will clean it up after the
	 * timeout
	 */
	switch (response & ~FAN_AUDIT) {
	case FAN_ALLOW:
	case FAN_DENY:
		break;
	default:
		return -EINVAL;
	}

	if (fd < 0)
		return -EINVAL;

<<<<<<< HEAD
	if ((response & FAN_AUDIT) && !group->fanotify_data.audit)
		return -EINVAL;

	event = dequeue_event(group, fd);
	if (!event)
		return -ENOENT;

	event->response = response;
	wake_up(&group->fanotify_data.access_waitq);

	return 0;
}

static ssize_t copy_event_to_user(struct fsnotify_group *group,
				  struct fsnotify_event *event,
				  char __user *buf)
{
	struct fanotify_event_metadata fanotify_event_metadata;
	struct file *f;
	int fd, ret;

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	ret = fill_event_metadata(group, &fanotify_event_metadata, event, &f);
	if (ret < 0)
		return ret;

	fd = fanotify_event_metadata.fd;
	ret = -EFAULT;
	if (copy_to_user(buf, &fanotify_event_metadata,
			 fanotify_event_metadata.event_len))
		goto out_close_fd;

	if (fanotify_is_perm_event(event->mask))
		FANOTIFY_PE(event)->fd = fd;

	if (fd != FAN_NOFD)
		fd_install(fd, f);
	return fanotify_event_metadata.event_len;
=======
	if ((response & FAN_AUDIT) && !FAN_GROUP_FLAG(group, FAN_ENABLE_AUDIT))
		return -EINVAL;

	spin_lock(&group->notification_lock);
	list_for_each_entry(event, &group->fanotify_data.access_list,
			    fae.fse.list) {
		if (event->fd != fd)
			continue;

		list_del_init(&event->fae.fse.list);
		finish_permission_event(group, event, response);
		wake_up(&group->fanotify_data.access_waitq);
		return 0;
	}
	spin_unlock(&group->notification_lock);

	return -ENOENT;
}

static int copy_fid_info_to_user(__kernel_fsid_t *fsid, struct fanotify_fh *fh,
				 int info_type, const char *name,
				 size_t name_len,
				 char __user *buf, size_t count)
{
	struct fanotify_event_info_fid info = { };
	struct file_handle handle = { };
	unsigned char bounce[FANOTIFY_INLINE_FH_LEN], *fh_buf;
	size_t fh_len = fh ? fh->len : 0;
	size_t info_len = fanotify_fid_info_len(fh_len, name_len);
	size_t len = info_len;

	pr_debug("%s: fh_len=%zu name_len=%zu, info_len=%zu, count=%zu\n",
		 __func__, fh_len, name_len, info_len, count);

	if (!fh_len)
		return 0;

	if (WARN_ON_ONCE(len < sizeof(info) || len > count))
		return -EFAULT;

	/*
	 * Copy event info fid header followed by variable sized file handle
	 * and optionally followed by variable sized filename.
	 */
	switch (info_type) {
	case FAN_EVENT_INFO_TYPE_FID:
	case FAN_EVENT_INFO_TYPE_DFID:
		if (WARN_ON_ONCE(name_len))
			return -EFAULT;
		break;
	case FAN_EVENT_INFO_TYPE_DFID_NAME:
		if (WARN_ON_ONCE(!name || !name_len))
			return -EFAULT;
		break;
	default:
		return -EFAULT;
	}

	info.hdr.info_type = info_type;
	info.hdr.len = len;
	info.fsid = *fsid;
	if (copy_to_user(buf, &info, sizeof(info)))
		return -EFAULT;

	buf += sizeof(info);
	len -= sizeof(info);
	if (WARN_ON_ONCE(len < sizeof(handle)))
		return -EFAULT;

	handle.handle_type = fh->type;
	handle.handle_bytes = fh_len;
	if (copy_to_user(buf, &handle, sizeof(handle)))
		return -EFAULT;

	buf += sizeof(handle);
	len -= sizeof(handle);
	if (WARN_ON_ONCE(len < fh_len))
		return -EFAULT;

	/*
	 * For an inline fh and inline file name, copy through stack to exclude
	 * the copy from usercopy hardening protections.
	 */
	fh_buf = fanotify_fh_buf(fh);
	if (fh_len <= FANOTIFY_INLINE_FH_LEN) {
		memcpy(bounce, fh_buf, fh_len);
		fh_buf = bounce;
	}
	if (copy_to_user(buf, fh_buf, fh_len))
		return -EFAULT;

	buf += fh_len;
	len -= fh_len;

	if (name_len) {
		/* Copy the filename with terminating null */
		name_len++;
		if (WARN_ON_ONCE(len < name_len))
			return -EFAULT;

		if (copy_to_user(buf, name, name_len))
			return -EFAULT;

		buf += name_len;
		len -= name_len;
	}

	/* Pad with 0's */
	WARN_ON_ONCE(len < 0 || len >= FANOTIFY_EVENT_ALIGN);
	if (len > 0 && clear_user(buf, len))
		return -EFAULT;

	return info_len;
}

static int copy_pidfd_info_to_user(int pidfd,
				   char __user *buf,
				   size_t count)
{
	struct fanotify_event_info_pidfd info = { };
	size_t info_len = FANOTIFY_PIDFD_INFO_HDR_LEN;

	if (WARN_ON_ONCE(info_len > count))
		return -EFAULT;

	info.hdr.info_type = FAN_EVENT_INFO_TYPE_PIDFD;
	info.hdr.len = info_len;
	info.pidfd = pidfd;

	if (copy_to_user(buf, &info, info_len))
		return -EFAULT;

	return info_len;
}

static int copy_info_records_to_user(struct fanotify_event *event,
				     struct fanotify_info *info,
				     unsigned int info_mode, int pidfd,
				     char __user *buf, size_t count)
{
	int ret, total_bytes = 0, info_type = 0;
	unsigned int fid_mode = info_mode & FANOTIFY_FID_BITS;
	unsigned int pidfd_mode = info_mode & FAN_REPORT_PIDFD;

	/*
	 * Event info records order is as follows: dir fid + name, child fid.
	 */
	if (fanotify_event_dir_fh_len(event)) {
		info_type = info->name_len ? FAN_EVENT_INFO_TYPE_DFID_NAME :
					     FAN_EVENT_INFO_TYPE_DFID;
		ret = copy_fid_info_to_user(fanotify_event_fsid(event),
					    fanotify_info_dir_fh(info),
					    info_type,
					    fanotify_info_name(info),
					    info->name_len, buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	if (fanotify_event_object_fh_len(event)) {
		const char *dot = NULL;
		int dot_len = 0;

		if (fid_mode == FAN_REPORT_FID || info_type) {
			/*
			 * With only group flag FAN_REPORT_FID only type FID is
			 * reported. Second info record type is always FID.
			 */
			info_type = FAN_EVENT_INFO_TYPE_FID;
		} else if ((fid_mode & FAN_REPORT_NAME) &&
			   (event->mask & FAN_ONDIR)) {
			/*
			 * With group flag FAN_REPORT_NAME, if name was not
			 * recorded in an event on a directory, report the name
			 * "." with info type DFID_NAME.
			 */
			info_type = FAN_EVENT_INFO_TYPE_DFID_NAME;
			dot = ".";
			dot_len = 1;
		} else if ((event->mask & ALL_FSNOTIFY_DIRENT_EVENTS) ||
			   (event->mask & FAN_ONDIR)) {
			/*
			 * With group flag FAN_REPORT_DIR_FID, a single info
			 * record has type DFID for directory entry modification
			 * event and for event on a directory.
			 */
			info_type = FAN_EVENT_INFO_TYPE_DFID;
		} else {
			/*
			 * With group flags FAN_REPORT_DIR_FID|FAN_REPORT_FID,
			 * a single info record has type FID for event on a
			 * non-directory, when there is no directory to report.
			 * For example, on FAN_DELETE_SELF event.
			 */
			info_type = FAN_EVENT_INFO_TYPE_FID;
		}

		ret = copy_fid_info_to_user(fanotify_event_fsid(event),
					    fanotify_event_object_fh(event),
					    info_type, dot, dot_len,
					    buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	if (pidfd_mode) {
		ret = copy_pidfd_info_to_user(pidfd, buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	return total_bytes;
}

static ssize_t copy_event_to_user(struct fsnotify_group *group,
				  struct fanotify_event *event,
				  char __user *buf, size_t count)
{
	struct fanotify_event_metadata metadata;
	struct path *path = fanotify_event_path(event);
	struct fanotify_info *info = fanotify_event_info(event);
	unsigned int info_mode = FAN_GROUP_FLAG(group, FANOTIFY_INFO_MODES);
	unsigned int pidfd_mode = info_mode & FAN_REPORT_PIDFD;
	struct file *f = NULL;
	int ret, pidfd = FAN_NOPIDFD, fd = FAN_NOFD;

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	metadata.event_len = FAN_EVENT_METADATA_LEN +
				fanotify_event_info_len(info_mode, event);
	metadata.metadata_len = FAN_EVENT_METADATA_LEN;
	metadata.vers = FANOTIFY_METADATA_VERSION;
	metadata.reserved = 0;
	metadata.mask = event->mask & FANOTIFY_OUTGOING_EVENTS;
	metadata.pid = pid_vnr(event->pid);
	/*
	 * For an unprivileged listener, event->pid can be used to identify the
	 * events generated by the listener process itself, without disclosing
	 * the pids of other processes.
	 */
	if (FAN_GROUP_FLAG(group, FANOTIFY_UNPRIV) &&
	    task_tgid(current) != event->pid)
		metadata.pid = 0;

	/*
	 * For now, fid mode is required for an unprivileged listener and
	 * fid mode does not report fd in events.  Keep this check anyway
	 * for safety in case fid mode requirement is relaxed in the future
	 * to allow unprivileged listener to get events with no fd and no fid.
	 */
	if (!FAN_GROUP_FLAG(group, FANOTIFY_UNPRIV) &&
	    path && path->mnt && path->dentry) {
		fd = create_fd(group, path, &f);
		if (fd < 0)
			return fd;
	}
	metadata.fd = fd;

	if (pidfd_mode) {
		/*
		 * Complain if the FAN_REPORT_PIDFD and FAN_REPORT_TID mutual
		 * exclusion is ever lifted. At the time of incoporating pidfd
		 * support within fanotify, the pidfd API only supported the
		 * creation of pidfds for thread-group leaders.
		 */
		WARN_ON_ONCE(FAN_GROUP_FLAG(group, FAN_REPORT_TID));

		/*
		 * The PIDTYPE_TGID check for an event->pid is performed
		 * preemptively in an attempt to catch out cases where the event
		 * listener reads events after the event generating process has
		 * already terminated. Report FAN_NOPIDFD to the event listener
		 * in those cases, with all other pidfd creation errors being
		 * reported as FAN_EPIDFD.
		 */
		if (metadata.pid == 0 ||
		    !pid_has_task(event->pid, PIDTYPE_TGID)) {
			pidfd = FAN_NOPIDFD;
		} else {
			pidfd = pidfd_create(event->pid, 0);
			if (pidfd < 0)
				pidfd = FAN_EPIDFD;
		}
	}

	ret = -EFAULT;
	/*
	 * Sanity check copy size in case get_one_event() and
	 * event_len sizes ever get out of sync.
	 */
	if (WARN_ON_ONCE(metadata.event_len > count))
		goto out_close_fd;

	if (copy_to_user(buf, &metadata, FAN_EVENT_METADATA_LEN))
		goto out_close_fd;

	buf += FAN_EVENT_METADATA_LEN;
	count -= FAN_EVENT_METADATA_LEN;

	if (fanotify_is_perm_event(event->mask))
		FANOTIFY_PERM(event)->fd = fd;

	if (info_mode) {
		ret = copy_info_records_to_user(event, info, info_mode, pidfd,
						buf, count);
		if (ret < 0)
			goto out_close_fd;
	}

	if (f)
		fd_install(fd, f);

	return metadata.event_len;
>>>>>>> upstream/android-13

out_close_fd:
	if (fd != FAN_NOFD) {
		put_unused_fd(fd);
		fput(f);
	}
<<<<<<< HEAD
=======

	if (pidfd >= 0)
		close_fd(pidfd);

>>>>>>> upstream/android-13
	return ret;
}

/* intofiy userspace file descriptor functions */
static __poll_t fanotify_poll(struct file *file, poll_table *wait)
{
	struct fsnotify_group *group = file->private_data;
	__poll_t ret = 0;

	poll_wait(file, &group->notification_waitq, wait);
	spin_lock(&group->notification_lock);
	if (!fsnotify_notify_queue_is_empty(group))
		ret = EPOLLIN | EPOLLRDNORM;
	spin_unlock(&group->notification_lock);

	return ret;
}

static ssize_t fanotify_read(struct file *file, char __user *buf,
			     size_t count, loff_t *pos)
{
	struct fsnotify_group *group;
<<<<<<< HEAD
	struct fsnotify_event *kevent;
=======
	struct fanotify_event *event;
>>>>>>> upstream/android-13
	char __user *start;
	int ret;
	DEFINE_WAIT_FUNC(wait, woken_wake_function);

	start = buf;
	group = file->private_data;

	pr_debug("%s: group=%p\n", __func__, group);

	add_wait_queue(&group->notification_waitq, &wait);
	while (1) {
<<<<<<< HEAD
		spin_lock(&group->notification_lock);
		kevent = get_one_event(group, count);
		spin_unlock(&group->notification_lock);

		if (IS_ERR(kevent)) {
			ret = PTR_ERR(kevent);
			break;
		}

		if (!kevent) {
=======
		/*
		 * User can supply arbitrarily large buffer. Avoid softlockups
		 * in case there are lots of available events.
		 */
		cond_resched();
		event = get_one_event(group, count);
		if (IS_ERR(event)) {
			ret = PTR_ERR(event);
			break;
		}

		if (!event) {
>>>>>>> upstream/android-13
			ret = -EAGAIN;
			if (file->f_flags & O_NONBLOCK)
				break;

			ret = -ERESTARTSYS;
			if (signal_pending(current))
				break;

			if (start != buf)
				break;

			wait_woken(&wait, TASK_INTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
			continue;
		}

<<<<<<< HEAD
		ret = copy_event_to_user(group, kevent, buf);
=======
		ret = copy_event_to_user(group, event, buf, count);
>>>>>>> upstream/android-13
		if (unlikely(ret == -EOPENSTALE)) {
			/*
			 * We cannot report events with stale fd so drop it.
			 * Setting ret to 0 will continue the event loop and
			 * do the right thing if there are no more events to
			 * read (i.e. return bytes read, -EAGAIN or wait).
			 */
			ret = 0;
		}

		/*
		 * Permission events get queued to wait for response.  Other
		 * events can be destroyed now.
		 */
<<<<<<< HEAD
		if (!fanotify_is_perm_event(kevent->mask)) {
			fsnotify_destroy_event(group, kevent);
		} else {
			if (ret <= 0) {
				FANOTIFY_PE(kevent)->response = FAN_DENY;
				wake_up(&group->fanotify_data.access_waitq);
			} else {
				spin_lock(&group->notification_lock);
				list_add_tail(&kevent->list,
=======
		if (!fanotify_is_perm_event(event->mask)) {
			fsnotify_destroy_event(group, &event->fse);
		} else {
			if (ret <= 0) {
				spin_lock(&group->notification_lock);
				finish_permission_event(group,
					FANOTIFY_PERM(event), FAN_DENY);
				wake_up(&group->fanotify_data.access_waitq);
			} else {
				spin_lock(&group->notification_lock);
				list_add_tail(&event->fse.list,
>>>>>>> upstream/android-13
					&group->fanotify_data.access_list);
				spin_unlock(&group->notification_lock);
			}
		}
		if (ret < 0)
			break;
		buf += ret;
		count -= ret;
	}
	remove_wait_queue(&group->notification_waitq, &wait);

	if (start != buf && ret != -EFAULT)
		ret = buf - start;
	return ret;
}

static ssize_t fanotify_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
	struct fanotify_response response = { .fd = -1, .response = -1 };
	struct fsnotify_group *group;
	int ret;

	if (!IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS))
		return -EINVAL;

	group = file->private_data;

<<<<<<< HEAD
	if (count > sizeof(response))
		count = sizeof(response);
=======
	if (count < sizeof(response))
		return -EINVAL;

	count = sizeof(response);
>>>>>>> upstream/android-13

	pr_debug("%s: group=%p count=%zu\n", __func__, group, count);

	if (copy_from_user(&response, buf, count))
		return -EFAULT;

	ret = process_access_response(group, &response);
	if (ret < 0)
		count = ret;

	return count;
}

static int fanotify_release(struct inode *ignored, struct file *file)
{
	struct fsnotify_group *group = file->private_data;
<<<<<<< HEAD
	struct fanotify_perm_event_info *event, *next;
=======
>>>>>>> upstream/android-13
	struct fsnotify_event *fsn_event;

	/*
	 * Stop new events from arriving in the notification queue. since
	 * userspace cannot use fanotify fd anymore, no event can enter or
	 * leave access_list by now either.
	 */
	fsnotify_group_stop_queueing(group);

	/*
	 * Process all permission events on access_list and notification queue
	 * and simulate reply from userspace.
	 */
	spin_lock(&group->notification_lock);
<<<<<<< HEAD
	list_for_each_entry_safe(event, next, &group->fanotify_data.access_list,
				 fae.fse.list) {
		pr_debug("%s: found group=%p event=%p\n", __func__, group,
			 event);

		list_del_init(&event->fae.fse.list);
		event->response = FAN_ALLOW;
=======
	while (!list_empty(&group->fanotify_data.access_list)) {
		struct fanotify_perm_event *event;

		event = list_first_entry(&group->fanotify_data.access_list,
				struct fanotify_perm_event, fae.fse.list);
		list_del_init(&event->fae.fse.list);
		finish_permission_event(group, event, FAN_ALLOW);
		spin_lock(&group->notification_lock);
>>>>>>> upstream/android-13
	}

	/*
	 * Destroy all non-permission events. For permission events just
	 * dequeue them and set the response. They will be freed once the
	 * response is consumed and fanotify_get_response() returns.
	 */
<<<<<<< HEAD
	while (!fsnotify_notify_queue_is_empty(group)) {
		fsn_event = fsnotify_remove_first_event(group);
		if (!(fsn_event->mask & FAN_ALL_PERM_EVENTS)) {
			spin_unlock(&group->notification_lock);
			fsnotify_destroy_event(group, fsn_event);
			spin_lock(&group->notification_lock);
		} else {
			FANOTIFY_PE(fsn_event)->response = FAN_ALLOW;
		}
=======
	while ((fsn_event = fsnotify_remove_first_event(group))) {
		struct fanotify_event *event = FANOTIFY_E(fsn_event);

		if (!(event->mask & FANOTIFY_PERM_EVENTS)) {
			spin_unlock(&group->notification_lock);
			fsnotify_destroy_event(group, fsn_event);
		} else {
			finish_permission_event(group, FANOTIFY_PERM(event),
						FAN_ALLOW);
		}
		spin_lock(&group->notification_lock);
>>>>>>> upstream/android-13
	}
	spin_unlock(&group->notification_lock);

	/* Response for all permission events it set, wakeup waiters */
	wake_up(&group->fanotify_data.access_waitq);

	/* matches the fanotify_init->fsnotify_alloc_group */
	fsnotify_destroy_group(group);

	return 0;
}

static long fanotify_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct fsnotify_group *group;
	struct fsnotify_event *fsn_event;
	void __user *p;
	int ret = -ENOTTY;
	size_t send_len = 0;

	group = file->private_data;

	p = (void __user *) arg;

	switch (cmd) {
	case FIONREAD:
		spin_lock(&group->notification_lock);
		list_for_each_entry(fsn_event, &group->notification_list, list)
			send_len += FAN_EVENT_METADATA_LEN;
		spin_unlock(&group->notification_lock);
		ret = put_user(send_len, (int __user *) p);
		break;
	}

	return ret;
}

static const struct file_operations fanotify_fops = {
	.show_fdinfo	= fanotify_show_fdinfo,
	.poll		= fanotify_poll,
	.read		= fanotify_read,
	.write		= fanotify_write,
	.fasync		= NULL,
	.release	= fanotify_release,
	.unlocked_ioctl	= fanotify_ioctl,
<<<<<<< HEAD
	.compat_ioctl	= fanotify_ioctl,
=======
	.compat_ioctl	= compat_ptr_ioctl,
>>>>>>> upstream/android-13
	.llseek		= noop_llseek,
};

static int fanotify_find_path(int dfd, const char __user *filename,
<<<<<<< HEAD
			      struct path *path, unsigned int flags)
=======
			      struct path *path, unsigned int flags, __u64 mask,
			      unsigned int obj_type)
>>>>>>> upstream/android-13
{
	int ret;

	pr_debug("%s: dfd=%d filename=%p flags=%x\n", __func__,
		 dfd, filename, flags);

	if (filename == NULL) {
		struct fd f = fdget(dfd);

		ret = -EBADF;
		if (!f.file)
			goto out;

		ret = -ENOTDIR;
		if ((flags & FAN_MARK_ONLYDIR) &&
		    !(S_ISDIR(file_inode(f.file)->i_mode))) {
			fdput(f);
			goto out;
		}

		*path = f.file->f_path;
		path_get(path);
		fdput(f);
	} else {
		unsigned int lookup_flags = 0;

		if (!(flags & FAN_MARK_DONT_FOLLOW))
			lookup_flags |= LOOKUP_FOLLOW;
		if (flags & FAN_MARK_ONLYDIR)
			lookup_flags |= LOOKUP_DIRECTORY;

		ret = user_path_at(dfd, filename, lookup_flags, path);
		if (ret)
			goto out;
	}

	/* you can only watch an inode if you have read permissions on it */
<<<<<<< HEAD
	ret = inode_permission2(path->mnt, path->dentry->d_inode, MAY_READ);
	if (ret)
		path_put(path);
=======
	ret = path_permission(path, MAY_READ);
	if (ret) {
		path_put(path);
		goto out;
	}

	ret = security_path_notify(path, mask, obj_type);
	if (ret)
		path_put(path);

>>>>>>> upstream/android-13
out:
	return ret;
}

static __u32 fanotify_mark_remove_from_mask(struct fsnotify_mark *fsn_mark,
<<<<<<< HEAD
					    __u32 mask,
					    unsigned int flags,
					    int *destroy)
{
	__u32 oldmask = 0;

	spin_lock(&fsn_mark->lock);
	if (!(flags & FAN_MARK_IGNORED_MASK)) {
		__u32 tmask = fsn_mark->mask & ~mask;

		if (flags & FAN_MARK_ONDIR)
			tmask &= ~FAN_ONDIR;

		oldmask = fsn_mark->mask;
		fsn_mark->mask = tmask;
	} else {
		__u32 tmask = fsn_mark->ignored_mask & ~mask;
		if (flags & FAN_MARK_ONDIR)
			tmask &= ~FAN_ONDIR;
		fsn_mark->ignored_mask = tmask;
	}
	*destroy = !(fsn_mark->mask | fsn_mark->ignored_mask);
=======
					    __u32 mask, unsigned int flags,
					    __u32 umask, int *destroy)
{
	__u32 oldmask = 0;

	/* umask bits cannot be removed by user */
	mask &= ~umask;
	spin_lock(&fsn_mark->lock);
	if (!(flags & FAN_MARK_IGNORED_MASK)) {
		oldmask = fsn_mark->mask;
		fsn_mark->mask &= ~mask;
	} else {
		fsn_mark->ignored_mask &= ~mask;
	}
	/*
	 * We need to keep the mark around even if remaining mask cannot
	 * result in any events (e.g. mask == FAN_ONDIR) to support incremenal
	 * changes to the mask.
	 * Destroy mark when only umask bits remain.
	 */
	*destroy = !((fsn_mark->mask | fsn_mark->ignored_mask) & ~umask);
>>>>>>> upstream/android-13
	spin_unlock(&fsn_mark->lock);

	return mask & oldmask;
}

static int fanotify_remove_mark(struct fsnotify_group *group,
				fsnotify_connp_t *connp, __u32 mask,
<<<<<<< HEAD
				unsigned int flags)
=======
				unsigned int flags, __u32 umask)
>>>>>>> upstream/android-13
{
	struct fsnotify_mark *fsn_mark = NULL;
	__u32 removed;
	int destroy_mark;

	mutex_lock(&group->mark_mutex);
	fsn_mark = fsnotify_find_mark(connp, group);
	if (!fsn_mark) {
		mutex_unlock(&group->mark_mutex);
		return -ENOENT;
	}

	removed = fanotify_mark_remove_from_mask(fsn_mark, mask, flags,
<<<<<<< HEAD
						 &destroy_mark);
=======
						 umask, &destroy_mark);
>>>>>>> upstream/android-13
	if (removed & fsnotify_conn_mask(fsn_mark->connector))
		fsnotify_recalc_mask(fsn_mark->connector);
	if (destroy_mark)
		fsnotify_detach_mark(fsn_mark);
	mutex_unlock(&group->mark_mutex);
	if (destroy_mark)
		fsnotify_free_mark(fsn_mark);

	/* matches the fsnotify_find_mark() */
	fsnotify_put_mark(fsn_mark);
	return 0;
}

static int fanotify_remove_vfsmount_mark(struct fsnotify_group *group,
					 struct vfsmount *mnt, __u32 mask,
<<<<<<< HEAD
					 unsigned int flags)
{
	return fanotify_remove_mark(group, &real_mount(mnt)->mnt_fsnotify_marks,
				    mask, flags);
=======
					 unsigned int flags, __u32 umask)
{
	return fanotify_remove_mark(group, &real_mount(mnt)->mnt_fsnotify_marks,
				    mask, flags, umask);
}

static int fanotify_remove_sb_mark(struct fsnotify_group *group,
				   struct super_block *sb, __u32 mask,
				   unsigned int flags, __u32 umask)
{
	return fanotify_remove_mark(group, &sb->s_fsnotify_marks, mask,
				    flags, umask);
>>>>>>> upstream/android-13
}

static int fanotify_remove_inode_mark(struct fsnotify_group *group,
				      struct inode *inode, __u32 mask,
<<<<<<< HEAD
				      unsigned int flags)
{
	return fanotify_remove_mark(group, &inode->i_fsnotify_marks, mask,
				    flags);
=======
				      unsigned int flags, __u32 umask)
{
	return fanotify_remove_mark(group, &inode->i_fsnotify_marks, mask,
				    flags, umask);
>>>>>>> upstream/android-13
}

static __u32 fanotify_mark_add_to_mask(struct fsnotify_mark *fsn_mark,
				       __u32 mask,
				       unsigned int flags)
{
	__u32 oldmask = -1;

	spin_lock(&fsn_mark->lock);
	if (!(flags & FAN_MARK_IGNORED_MASK)) {
<<<<<<< HEAD
		__u32 tmask = fsn_mark->mask | mask;

		if (flags & FAN_MARK_ONDIR)
			tmask |= FAN_ONDIR;

		oldmask = fsn_mark->mask;
		fsn_mark->mask = tmask;
	} else {
		__u32 tmask = fsn_mark->ignored_mask | mask;
		if (flags & FAN_MARK_ONDIR)
			tmask |= FAN_ONDIR;

		fsn_mark->ignored_mask = tmask;
=======
		oldmask = fsn_mark->mask;
		fsn_mark->mask |= mask;
	} else {
		fsn_mark->ignored_mask |= mask;
>>>>>>> upstream/android-13
		if (flags & FAN_MARK_IGNORED_SURV_MODIFY)
			fsn_mark->flags |= FSNOTIFY_MARK_FLAG_IGNORED_SURV_MODIFY;
	}
	spin_unlock(&fsn_mark->lock);

	return mask & ~oldmask;
}

static struct fsnotify_mark *fanotify_add_new_mark(struct fsnotify_group *group,
						   fsnotify_connp_t *connp,
<<<<<<< HEAD
						   unsigned int type)
{
	struct fsnotify_mark *mark;
	int ret;

	if (atomic_read(&group->num_marks) > group->fanotify_data.max_marks)
		return ERR_PTR(-ENOSPC);

	mark = kmem_cache_alloc(fanotify_mark_cache, GFP_KERNEL);
	if (!mark)
		return ERR_PTR(-ENOMEM);

	fsnotify_init_mark(mark, group);
	ret = fsnotify_add_mark_locked(mark, connp, type, 0);
	if (ret) {
		fsnotify_put_mark(mark);
		return ERR_PTR(ret);
	}

	return mark;
=======
						   unsigned int type,
						   __kernel_fsid_t *fsid)
{
	struct ucounts *ucounts = group->fanotify_data.ucounts;
	struct fsnotify_mark *mark;
	int ret;

	/*
	 * Enforce per user marks limits per user in all containing user ns.
	 * A group with FAN_UNLIMITED_MARKS does not contribute to mark count
	 * in the limited groups account.
	 */
	if (!FAN_GROUP_FLAG(group, FAN_UNLIMITED_MARKS) &&
	    !inc_ucount(ucounts->ns, ucounts->uid, UCOUNT_FANOTIFY_MARKS))
		return ERR_PTR(-ENOSPC);

	mark = kmem_cache_alloc(fanotify_mark_cache, GFP_KERNEL);
	if (!mark) {
		ret = -ENOMEM;
		goto out_dec_ucounts;
	}

	fsnotify_init_mark(mark, group);
	ret = fsnotify_add_mark_locked(mark, connp, type, 0, fsid);
	if (ret) {
		fsnotify_put_mark(mark);
		goto out_dec_ucounts;
	}

	return mark;

out_dec_ucounts:
	if (!FAN_GROUP_FLAG(group, FAN_UNLIMITED_MARKS))
		dec_ucount(ucounts, UCOUNT_FANOTIFY_MARKS);
	return ERR_PTR(ret);
>>>>>>> upstream/android-13
}


static int fanotify_add_mark(struct fsnotify_group *group,
			     fsnotify_connp_t *connp, unsigned int type,
<<<<<<< HEAD
			     __u32 mask, unsigned int flags)
=======
			     __u32 mask, unsigned int flags,
			     __kernel_fsid_t *fsid)
>>>>>>> upstream/android-13
{
	struct fsnotify_mark *fsn_mark;
	__u32 added;

	mutex_lock(&group->mark_mutex);
	fsn_mark = fsnotify_find_mark(connp, group);
	if (!fsn_mark) {
<<<<<<< HEAD
		fsn_mark = fanotify_add_new_mark(group, connp, type);
=======
		fsn_mark = fanotify_add_new_mark(group, connp, type, fsid);
>>>>>>> upstream/android-13
		if (IS_ERR(fsn_mark)) {
			mutex_unlock(&group->mark_mutex);
			return PTR_ERR(fsn_mark);
		}
	}
	added = fanotify_mark_add_to_mask(fsn_mark, mask, flags);
	if (added & ~fsnotify_conn_mask(fsn_mark->connector))
		fsnotify_recalc_mask(fsn_mark->connector);
	mutex_unlock(&group->mark_mutex);

	fsnotify_put_mark(fsn_mark);
	return 0;
}

static int fanotify_add_vfsmount_mark(struct fsnotify_group *group,
				      struct vfsmount *mnt, __u32 mask,
<<<<<<< HEAD
				      unsigned int flags)
{
	return fanotify_add_mark(group, &real_mount(mnt)->mnt_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_VFSMOUNT, mask, flags);
=======
				      unsigned int flags, __kernel_fsid_t *fsid)
{
	return fanotify_add_mark(group, &real_mount(mnt)->mnt_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_VFSMOUNT, mask, flags, fsid);
}

static int fanotify_add_sb_mark(struct fsnotify_group *group,
				struct super_block *sb, __u32 mask,
				unsigned int flags, __kernel_fsid_t *fsid)
{
	return fanotify_add_mark(group, &sb->s_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_SB, mask, flags, fsid);
>>>>>>> upstream/android-13
}

static int fanotify_add_inode_mark(struct fsnotify_group *group,
				   struct inode *inode, __u32 mask,
<<<<<<< HEAD
				   unsigned int flags)
=======
				   unsigned int flags, __kernel_fsid_t *fsid)
>>>>>>> upstream/android-13
{
	pr_debug("%s: group=%p inode=%p\n", __func__, group, inode);

	/*
	 * If some other task has this inode open for write we should not add
	 * an ignored mark, unless that ignored mark is supposed to survive
	 * modification changes anyway.
	 */
	if ((flags & FAN_MARK_IGNORED_MASK) &&
	    !(flags & FAN_MARK_IGNORED_SURV_MODIFY) &&
<<<<<<< HEAD
	    (atomic_read(&inode->i_writecount) > 0))
		return 0;

	return fanotify_add_mark(group, &inode->i_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_INODE, mask, flags);
=======
	    inode_is_open_for_write(inode))
		return 0;

	return fanotify_add_mark(group, &inode->i_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_INODE, mask, flags, fsid);
}

static struct fsnotify_event *fanotify_alloc_overflow_event(void)
{
	struct fanotify_event *oevent;

	oevent = kmalloc(sizeof(*oevent), GFP_KERNEL_ACCOUNT);
	if (!oevent)
		return NULL;

	fanotify_init_event(oevent, 0, FS_Q_OVERFLOW);
	oevent->type = FANOTIFY_EVENT_TYPE_OVERFLOW;

	return &oevent->fse;
}

static struct hlist_head *fanotify_alloc_merge_hash(void)
{
	struct hlist_head *hash;

	hash = kmalloc(sizeof(struct hlist_head) << FANOTIFY_HTABLE_BITS,
		       GFP_KERNEL_ACCOUNT);
	if (!hash)
		return NULL;

	__hash_init(hash, FANOTIFY_HTABLE_SIZE);

	return hash;
>>>>>>> upstream/android-13
}

/* fanotify syscalls */
SYSCALL_DEFINE2(fanotify_init, unsigned int, flags, unsigned int, event_f_flags)
{
	struct fsnotify_group *group;
	int f_flags, fd;
<<<<<<< HEAD
	struct user_struct *user;
	struct fanotify_event_info *oevent;

	pr_debug("%s: flags=%d event_f_flags=%d\n",
		__func__, flags, event_f_flags);

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

#ifdef CONFIG_AUDITSYSCALL
	if (flags & ~(FAN_ALL_INIT_FLAGS | FAN_ENABLE_AUDIT))
#else
	if (flags & ~FAN_ALL_INIT_FLAGS)
#endif
		return -EINVAL;

=======
	unsigned int fid_mode = flags & FANOTIFY_FID_BITS;
	unsigned int class = flags & FANOTIFY_CLASS_BITS;
	unsigned int internal_flags = 0;

	pr_debug("%s: flags=%x event_f_flags=%x\n",
		 __func__, flags, event_f_flags);

	if (!capable(CAP_SYS_ADMIN)) {
		/*
		 * An unprivileged user can setup an fanotify group with
		 * limited functionality - an unprivileged group is limited to
		 * notification events with file handles and it cannot use
		 * unlimited queue/marks.
		 */
		if ((flags & FANOTIFY_ADMIN_INIT_FLAGS) || !fid_mode)
			return -EPERM;

		/*
		 * Setting the internal flag FANOTIFY_UNPRIV on the group
		 * prevents setting mount/filesystem marks on this group and
		 * prevents reporting pid and open fd in events.
		 */
		internal_flags |= FANOTIFY_UNPRIV;
	}

#ifdef CONFIG_AUDITSYSCALL
	if (flags & ~(FANOTIFY_INIT_FLAGS | FAN_ENABLE_AUDIT))
#else
	if (flags & ~FANOTIFY_INIT_FLAGS)
#endif
		return -EINVAL;

	/*
	 * A pidfd can only be returned for a thread-group leader; thus
	 * FAN_REPORT_PIDFD and FAN_REPORT_TID need to remain mutually
	 * exclusive.
	 */
	if ((flags & FAN_REPORT_PIDFD) && (flags & FAN_REPORT_TID))
		return -EINVAL;

>>>>>>> upstream/android-13
	if (event_f_flags & ~FANOTIFY_INIT_ALL_EVENT_F_BITS)
		return -EINVAL;

	switch (event_f_flags & O_ACCMODE) {
	case O_RDONLY:
	case O_RDWR:
	case O_WRONLY:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	user = get_current_user();
	if (atomic_read(&user->fanotify_listeners) > FANOTIFY_DEFAULT_MAX_LISTENERS) {
		free_uid(user);
		return -EMFILE;
	}
=======
	if (fid_mode && class != FAN_CLASS_NOTIF)
		return -EINVAL;

	/*
	 * Child name is reported with parent fid so requires dir fid.
	 * We can report both child fid and dir fid with or without name.
	 */
	if ((fid_mode & FAN_REPORT_NAME) && !(fid_mode & FAN_REPORT_DIR_FID))
		return -EINVAL;
>>>>>>> upstream/android-13

	f_flags = O_RDWR | FMODE_NONOTIFY;
	if (flags & FAN_CLOEXEC)
		f_flags |= O_CLOEXEC;
	if (flags & FAN_NONBLOCK)
		f_flags |= O_NONBLOCK;

	/* fsnotify_alloc_group takes a ref.  Dropped in fanotify_release */
<<<<<<< HEAD
	group = fsnotify_alloc_group(&fanotify_fsnotify_ops);
	if (IS_ERR(group)) {
		free_uid(user);
		return PTR_ERR(group);
	}

	group->fanotify_data.user = user;
	atomic_inc(&user->fanotify_listeners);
	group->memcg = get_mem_cgroup_from_mm(current->mm);

	oevent = fanotify_alloc_event(group, NULL, FS_Q_OVERFLOW, NULL);
	if (unlikely(!oevent)) {
		fd = -ENOMEM;
		goto out_destroy_group;
	}
	group->overflow_event = &oevent->fse;
=======
	group = fsnotify_alloc_user_group(&fanotify_fsnotify_ops);
	if (IS_ERR(group)) {
		return PTR_ERR(group);
	}

	/* Enforce groups limits per user in all containing user ns */
	group->fanotify_data.ucounts = inc_ucount(current_user_ns(),
						  current_euid(),
						  UCOUNT_FANOTIFY_GROUPS);
	if (!group->fanotify_data.ucounts) {
		fd = -EMFILE;
		goto out_destroy_group;
	}

	group->fanotify_data.flags = flags | internal_flags;
	group->memcg = get_mem_cgroup_from_mm(current->mm);

	group->fanotify_data.merge_hash = fanotify_alloc_merge_hash();
	if (!group->fanotify_data.merge_hash) {
		fd = -ENOMEM;
		goto out_destroy_group;
	}

	group->overflow_event = fanotify_alloc_overflow_event();
	if (unlikely(!group->overflow_event)) {
		fd = -ENOMEM;
		goto out_destroy_group;
	}
>>>>>>> upstream/android-13

	if (force_o_largefile())
		event_f_flags |= O_LARGEFILE;
	group->fanotify_data.f_flags = event_f_flags;
	init_waitqueue_head(&group->fanotify_data.access_waitq);
	INIT_LIST_HEAD(&group->fanotify_data.access_list);
<<<<<<< HEAD
	switch (flags & FAN_ALL_CLASS_BITS) {
=======
	switch (class) {
>>>>>>> upstream/android-13
	case FAN_CLASS_NOTIF:
		group->priority = FS_PRIO_0;
		break;
	case FAN_CLASS_CONTENT:
		group->priority = FS_PRIO_1;
		break;
	case FAN_CLASS_PRE_CONTENT:
		group->priority = FS_PRIO_2;
		break;
	default:
		fd = -EINVAL;
		goto out_destroy_group;
	}

	if (flags & FAN_UNLIMITED_QUEUE) {
		fd = -EPERM;
		if (!capable(CAP_SYS_ADMIN))
			goto out_destroy_group;
		group->max_events = UINT_MAX;
	} else {
<<<<<<< HEAD
		group->max_events = FANOTIFY_DEFAULT_MAX_EVENTS;
=======
		group->max_events = fanotify_max_queued_events;
>>>>>>> upstream/android-13
	}

	if (flags & FAN_UNLIMITED_MARKS) {
		fd = -EPERM;
		if (!capable(CAP_SYS_ADMIN))
			goto out_destroy_group;
<<<<<<< HEAD
		group->fanotify_data.max_marks = UINT_MAX;
	} else {
		group->fanotify_data.max_marks = FANOTIFY_DEFAULT_MAX_MARKS;
=======
>>>>>>> upstream/android-13
	}

	if (flags & FAN_ENABLE_AUDIT) {
		fd = -EPERM;
		if (!capable(CAP_AUDIT_WRITE))
			goto out_destroy_group;
<<<<<<< HEAD
		group->fanotify_data.audit = true;
=======
>>>>>>> upstream/android-13
	}

	fd = anon_inode_getfd("[fanotify]", &fanotify_fops, group, f_flags);
	if (fd < 0)
		goto out_destroy_group;

	return fd;

out_destroy_group:
	fsnotify_destroy_group(group);
	return fd;
}

<<<<<<< HEAD
=======
/* Check if filesystem can encode a unique fid */
static int fanotify_test_fid(struct path *path, __kernel_fsid_t *fsid)
{
	__kernel_fsid_t root_fsid;
	int err;

	/*
	 * Make sure path is not in filesystem with zero fsid (e.g. tmpfs).
	 */
	err = vfs_get_fsid(path->dentry, fsid);
	if (err)
		return err;

	if (!fsid->val[0] && !fsid->val[1])
		return -ENODEV;

	/*
	 * Make sure path is not inside a filesystem subvolume (e.g. btrfs)
	 * which uses a different fsid than sb root.
	 */
	err = vfs_get_fsid(path->dentry->d_sb->s_root, &root_fsid);
	if (err)
		return err;

	if (root_fsid.val[0] != fsid->val[0] ||
	    root_fsid.val[1] != fsid->val[1])
		return -EXDEV;

	/*
	 * We need to make sure that the file system supports at least
	 * encoding a file handle so user can use name_to_handle_at() to
	 * compare fid returned with event to the file handle of watched
	 * objects. However, name_to_handle_at() requires that the
	 * filesystem also supports decoding file handles.
	 */
	if (!path->dentry->d_sb->s_export_op ||
	    !path->dentry->d_sb->s_export_op->fh_to_dentry)
		return -EOPNOTSUPP;

	return 0;
}

static int fanotify_events_supported(struct path *path, __u64 mask)
{
	/*
	 * Some filesystems such as 'proc' acquire unusual locks when opening
	 * files. For them fanotify permission events have high chances of
	 * deadlocking the system - open done when reporting fanotify event
	 * blocks on this "unusual" lock while another process holding the lock
	 * waits for fanotify permission event to be answered. Just disallow
	 * permission events for such filesystems.
	 */
	if (mask & FANOTIFY_PERM_EVENTS &&
	    path->mnt->mnt_sb->s_type->fs_flags & FS_DISALLOW_NOTIFY_PERM)
		return -EINVAL;
	return 0;
}

>>>>>>> upstream/android-13
static int do_fanotify_mark(int fanotify_fd, unsigned int flags, __u64 mask,
			    int dfd, const char  __user *pathname)
{
	struct inode *inode = NULL;
	struct vfsmount *mnt = NULL;
	struct fsnotify_group *group;
	struct fd f;
	struct path path;
<<<<<<< HEAD
	u32 valid_mask = FAN_ALL_EVENTS | FAN_EVENT_ON_CHILD;
=======
	__kernel_fsid_t __fsid, *fsid = NULL;
	u32 valid_mask = FANOTIFY_EVENTS | FANOTIFY_EVENT_FLAGS;
	unsigned int mark_type = flags & FANOTIFY_MARK_TYPE_BITS;
	bool ignored = flags & FAN_MARK_IGNORED_MASK;
	unsigned int obj_type, fid_mode;
	u32 umask = 0;
>>>>>>> upstream/android-13
	int ret;

	pr_debug("%s: fanotify_fd=%d flags=%x dfd=%d pathname=%p mask=%llx\n",
		 __func__, fanotify_fd, flags, dfd, pathname, mask);

	/* we only use the lower 32 bits as of right now. */
<<<<<<< HEAD
	if (mask & ((__u64)0xffffffff << 32))
		return -EINVAL;

	if (flags & ~FAN_ALL_MARK_FLAGS)
		return -EINVAL;
	switch (flags & (FAN_MARK_ADD | FAN_MARK_REMOVE | FAN_MARK_FLUSH)) {
	case FAN_MARK_ADD:		/* fallthrough */
=======
	if (upper_32_bits(mask))
		return -EINVAL;

	if (flags & ~FANOTIFY_MARK_FLAGS)
		return -EINVAL;

	switch (mark_type) {
	case FAN_MARK_INODE:
		obj_type = FSNOTIFY_OBJ_TYPE_INODE;
		break;
	case FAN_MARK_MOUNT:
		obj_type = FSNOTIFY_OBJ_TYPE_VFSMOUNT;
		break;
	case FAN_MARK_FILESYSTEM:
		obj_type = FSNOTIFY_OBJ_TYPE_SB;
		break;
	default:
		return -EINVAL;
	}

	switch (flags & (FAN_MARK_ADD | FAN_MARK_REMOVE | FAN_MARK_FLUSH)) {
	case FAN_MARK_ADD:
>>>>>>> upstream/android-13
	case FAN_MARK_REMOVE:
		if (!mask)
			return -EINVAL;
		break;
	case FAN_MARK_FLUSH:
<<<<<<< HEAD
		if (flags & ~(FAN_MARK_MOUNT | FAN_MARK_FLUSH))
=======
		if (flags & ~(FANOTIFY_MARK_TYPE_BITS | FAN_MARK_FLUSH))
>>>>>>> upstream/android-13
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	if (mask & FAN_ONDIR) {
		flags |= FAN_MARK_ONDIR;
		mask &= ~FAN_ONDIR;
	}

	if (IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS))
		valid_mask |= FAN_ALL_PERM_EVENTS;
=======
	if (IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS))
		valid_mask |= FANOTIFY_PERM_EVENTS;
>>>>>>> upstream/android-13

	if (mask & ~valid_mask)
		return -EINVAL;

<<<<<<< HEAD
=======
	/* Event flags (ONDIR, ON_CHILD) are meaningless in ignored mask */
	if (ignored)
		mask &= ~FANOTIFY_EVENT_FLAGS;

>>>>>>> upstream/android-13
	f = fdget(fanotify_fd);
	if (unlikely(!f.file))
		return -EBADF;

	/* verify that this is indeed an fanotify instance */
	ret = -EINVAL;
	if (unlikely(f.file->f_op != &fanotify_fops))
		goto fput_and_out;
	group = f.file->private_data;

	/*
<<<<<<< HEAD
=======
	 * An unprivileged user is not allowed to setup mount nor filesystem
	 * marks.  This also includes setting up such marks by a group that
	 * was initialized by an unprivileged user.
	 */
	ret = -EPERM;
	if ((!capable(CAP_SYS_ADMIN) ||
	     FAN_GROUP_FLAG(group, FANOTIFY_UNPRIV)) &&
	    mark_type != FAN_MARK_INODE)
		goto fput_and_out;

	/*
>>>>>>> upstream/android-13
	 * group->priority == FS_PRIO_0 == FAN_CLASS_NOTIF.  These are not
	 * allowed to set permissions events.
	 */
	ret = -EINVAL;
<<<<<<< HEAD
	if (mask & FAN_ALL_PERM_EVENTS &&
	    group->priority == FS_PRIO_0)
		goto fput_and_out;

	if (flags & FAN_MARK_FLUSH) {
		ret = 0;
		if (flags & FAN_MARK_MOUNT)
			fsnotify_clear_vfsmount_marks_by_group(group);
=======
	if (mask & FANOTIFY_PERM_EVENTS &&
	    group->priority == FS_PRIO_0)
		goto fput_and_out;

	/*
	 * Events with data type inode do not carry enough information to report
	 * event->fd, so we do not allow setting a mask for inode events unless
	 * group supports reporting fid.
	 * inode events are not supported on a mount mark, because they do not
	 * carry enough information (i.e. path) to be filtered by mount point.
	 */
	fid_mode = FAN_GROUP_FLAG(group, FANOTIFY_FID_BITS);
	if (mask & FANOTIFY_INODE_EVENTS &&
	    (!fid_mode || mark_type == FAN_MARK_MOUNT))
		goto fput_and_out;

	if (flags & FAN_MARK_FLUSH) {
		ret = 0;
		if (mark_type == FAN_MARK_MOUNT)
			fsnotify_clear_vfsmount_marks_by_group(group);
		else if (mark_type == FAN_MARK_FILESYSTEM)
			fsnotify_clear_sb_marks_by_group(group);
>>>>>>> upstream/android-13
		else
			fsnotify_clear_inode_marks_by_group(group);
		goto fput_and_out;
	}

<<<<<<< HEAD
	ret = fanotify_find_path(dfd, pathname, &path, flags);
	if (ret)
		goto fput_and_out;

	/* inode held in place by reference to path; group by fget on fd */
	if (!(flags & FAN_MARK_MOUNT))
=======
	ret = fanotify_find_path(dfd, pathname, &path, flags,
			(mask & ALL_FSNOTIFY_EVENTS), obj_type);
	if (ret)
		goto fput_and_out;

	if (flags & FAN_MARK_ADD) {
		ret = fanotify_events_supported(&path, mask);
		if (ret)
			goto path_put_and_out;
	}

	if (fid_mode) {
		ret = fanotify_test_fid(&path, &__fsid);
		if (ret)
			goto path_put_and_out;

		fsid = &__fsid;
	}

	/* inode held in place by reference to path; group by fget on fd */
	if (mark_type == FAN_MARK_INODE)
>>>>>>> upstream/android-13
		inode = path.dentry->d_inode;
	else
		mnt = path.mnt;

<<<<<<< HEAD
	/* create/update an inode mark */
	switch (flags & (FAN_MARK_ADD | FAN_MARK_REMOVE)) {
	case FAN_MARK_ADD:
		if (flags & FAN_MARK_MOUNT)
			ret = fanotify_add_vfsmount_mark(group, mnt, mask, flags);
		else
			ret = fanotify_add_inode_mark(group, inode, mask, flags);
		break;
	case FAN_MARK_REMOVE:
		if (flags & FAN_MARK_MOUNT)
			ret = fanotify_remove_vfsmount_mark(group, mnt, mask, flags);
		else
			ret = fanotify_remove_inode_mark(group, inode, mask, flags);
=======
	/* Mask out FAN_EVENT_ON_CHILD flag for sb/mount/non-dir marks */
	if (mnt || !S_ISDIR(inode->i_mode)) {
		mask &= ~FAN_EVENT_ON_CHILD;
		umask = FAN_EVENT_ON_CHILD;
		/*
		 * If group needs to report parent fid, register for getting
		 * events with parent/name info for non-directory.
		 */
		if ((fid_mode & FAN_REPORT_DIR_FID) &&
		    (flags & FAN_MARK_ADD) && !ignored)
			mask |= FAN_EVENT_ON_CHILD;
	}

	/* create/update an inode mark */
	switch (flags & (FAN_MARK_ADD | FAN_MARK_REMOVE)) {
	case FAN_MARK_ADD:
		if (mark_type == FAN_MARK_MOUNT)
			ret = fanotify_add_vfsmount_mark(group, mnt, mask,
							 flags, fsid);
		else if (mark_type == FAN_MARK_FILESYSTEM)
			ret = fanotify_add_sb_mark(group, mnt->mnt_sb, mask,
						   flags, fsid);
		else
			ret = fanotify_add_inode_mark(group, inode, mask,
						      flags, fsid);
		break;
	case FAN_MARK_REMOVE:
		if (mark_type == FAN_MARK_MOUNT)
			ret = fanotify_remove_vfsmount_mark(group, mnt, mask,
							    flags, umask);
		else if (mark_type == FAN_MARK_FILESYSTEM)
			ret = fanotify_remove_sb_mark(group, mnt->mnt_sb, mask,
						      flags, umask);
		else
			ret = fanotify_remove_inode_mark(group, inode, mask,
							 flags, umask);
>>>>>>> upstream/android-13
		break;
	default:
		ret = -EINVAL;
	}

<<<<<<< HEAD
=======
path_put_and_out:
>>>>>>> upstream/android-13
	path_put(&path);
fput_and_out:
	fdput(f);
	return ret;
}

<<<<<<< HEAD
=======
#ifndef CONFIG_ARCH_SPLIT_ARG64
>>>>>>> upstream/android-13
SYSCALL_DEFINE5(fanotify_mark, int, fanotify_fd, unsigned int, flags,
			      __u64, mask, int, dfd,
			      const char  __user *, pathname)
{
	return do_fanotify_mark(fanotify_fd, flags, mask, dfd, pathname);
}
<<<<<<< HEAD

#ifdef CONFIG_COMPAT
COMPAT_SYSCALL_DEFINE6(fanotify_mark,
				int, fanotify_fd, unsigned int, flags,
				__u32, mask0, __u32, mask1, int, dfd,
				const char  __user *, pathname)
{
	return do_fanotify_mark(fanotify_fd, flags,
#ifdef __BIG_ENDIAN
				((__u64)mask0 << 32) | mask1,
#else
				((__u64)mask1 << 32) | mask0,
#endif
				 dfd, pathname);
=======
#endif

#if defined(CONFIG_ARCH_SPLIT_ARG64) || defined(CONFIG_COMPAT)
SYSCALL32_DEFINE6(fanotify_mark,
				int, fanotify_fd, unsigned int, flags,
				SC_ARG64(mask), int, dfd,
				const char  __user *, pathname)
{
	return do_fanotify_mark(fanotify_fd, flags, SC_VAL64(__u64, mask),
				dfd, pathname);
>>>>>>> upstream/android-13
}
#endif

/*
 * fanotify_user_setup - Our initialization function.  Note that we cannot return
 * error because we have compiled-in VFS hooks.  So an (unlikely) failure here
 * must result in panic().
 */
static int __init fanotify_user_setup(void)
{
<<<<<<< HEAD
	fanotify_mark_cache = KMEM_CACHE(fsnotify_mark,
					 SLAB_PANIC|SLAB_ACCOUNT);
	fanotify_event_cachep = KMEM_CACHE(fanotify_event_info, SLAB_PANIC);
	if (IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS)) {
		fanotify_perm_event_cachep =
			KMEM_CACHE(fanotify_perm_event_info, SLAB_PANIC);
	}

=======
	struct sysinfo si;
	int max_marks;

	si_meminfo(&si);
	/*
	 * Allow up to 1% of addressable memory to be accounted for per user
	 * marks limited to the range [8192, 1048576]. mount and sb marks are
	 * a lot cheaper than inode marks, but there is no reason for a user
	 * to have many of those, so calculate by the cost of inode marks.
	 */
	max_marks = (((si.totalram - si.totalhigh) / 100) << PAGE_SHIFT) /
		    INODE_MARK_COST;
	max_marks = clamp(max_marks, FANOTIFY_OLD_DEFAULT_MAX_MARKS,
				     FANOTIFY_DEFAULT_MAX_USER_MARKS);

	BUILD_BUG_ON(FANOTIFY_INIT_FLAGS & FANOTIFY_INTERNAL_GROUP_FLAGS);
	BUILD_BUG_ON(HWEIGHT32(FANOTIFY_INIT_FLAGS) != 11);
	BUILD_BUG_ON(HWEIGHT32(FANOTIFY_MARK_FLAGS) != 9);

	fanotify_mark_cache = KMEM_CACHE(fsnotify_mark,
					 SLAB_PANIC|SLAB_ACCOUNT);
	fanotify_fid_event_cachep = KMEM_CACHE(fanotify_fid_event,
					       SLAB_PANIC);
	fanotify_path_event_cachep = KMEM_CACHE(fanotify_path_event,
						SLAB_PANIC);
	if (IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS)) {
		fanotify_perm_event_cachep =
			KMEM_CACHE(fanotify_perm_event, SLAB_PANIC);
	}

	fanotify_max_queued_events = FANOTIFY_DEFAULT_MAX_EVENTS;
	init_user_ns.ucount_max[UCOUNT_FANOTIFY_GROUPS] =
					FANOTIFY_DEFAULT_MAX_GROUPS;
	init_user_ns.ucount_max[UCOUNT_FANOTIFY_MARKS] = max_marks;

>>>>>>> upstream/android-13
	return 0;
}
device_initcall(fanotify_user_setup);
