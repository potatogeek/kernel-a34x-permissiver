/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_USER_H
#define _LINUX_SCHED_USER_H

#include <linux/uidgid.h>
#include <linux/atomic.h>
<<<<<<< HEAD
=======
#include <linux/percpu_counter.h>
>>>>>>> upstream/android-13
#include <linux/refcount.h>
#include <linux/ratelimit.h>
#include <linux/android_kabi.h>

<<<<<<< HEAD
struct key;

=======
>>>>>>> upstream/android-13
/*
 * Some day this will be a full-fledged user tracking system..
 */
struct user_struct {
	refcount_t __count;	/* reference count */
<<<<<<< HEAD
	atomic_t processes;	/* How many processes does this user have? */
	atomic_t sigpending;	/* How many pending signals does this user have? */
#ifdef CONFIG_FANOTIFY
	atomic_t fanotify_listeners;
#endif
#ifdef CONFIG_EPOLL
	atomic_long_t epoll_watches; /* The number of file descriptors currently watched */
#endif
#ifdef CONFIG_POSIX_MQUEUE
	/* protected by mq_lock	*/
	unsigned long mq_bytes;	/* How many bytes can be allocated to mqueue? */
#endif
	unsigned long locked_shm; /* How many pages of mlocked shm ? */
	unsigned long unix_inflight;	/* How many files in flight in unix sockets */
	atomic_long_t pipe_bufs;  /* how many pages are allocated in pipe buffers */

#ifdef CONFIG_KEYS
	struct key *uid_keyring;	/* UID specific keyring */
	struct key *session_keyring;	/* UID's default session keyring */
#endif

=======
#ifdef CONFIG_EPOLL
	struct percpu_counter epoll_watches; /* The number of file descriptors currently watched */
#endif
	unsigned long unix_inflight;	/* How many files in flight in unix sockets */
	atomic_long_t pipe_bufs;  /* how many pages are allocated in pipe buffers */

>>>>>>> upstream/android-13
	/* Hash table maintenance information */
	struct hlist_node uidhash_node;
	kuid_t uid;

#if defined(CONFIG_PERF_EVENTS) || defined(CONFIG_BPF_SYSCALL) || \
<<<<<<< HEAD
    defined(CONFIG_NET)
	atomic_long_t locked_vm;
#endif
=======
    defined(CONFIG_NET) || defined(CONFIG_IO_URING)
	atomic_long_t locked_vm;
#endif
#ifdef CONFIG_WATCH_QUEUE
	atomic_t nr_watches;	/* The number of watches this user currently has */
#endif
>>>>>>> upstream/android-13

	/* Miscellaneous per-user rate limit */
	struct ratelimit_state ratelimit;

	ANDROID_KABI_RESERVE(1);
	ANDROID_KABI_RESERVE(2);
};

<<<<<<< HEAD
=======
struct ext_user_struct {
	struct user_struct user;
	ANDROID_OEM_DATA_ARRAY(1, 2);
};

>>>>>>> upstream/android-13
extern int uids_sysfs_init(void);

extern struct user_struct *find_user(kuid_t);

<<<<<<< HEAD
extern struct user_struct root_user;
#define INIT_USER (&root_user)

=======
extern struct ext_user_struct ext_root_user;
extern struct user_struct *root_user;
#define INIT_USER (&ext_root_user.user)
>>>>>>> upstream/android-13

/* per-UID process charging. */
extern struct user_struct * alloc_uid(kuid_t);
static inline struct user_struct *get_uid(struct user_struct *u)
{
	refcount_inc(&u->__count);
	return u;
}
extern void free_uid(struct user_struct *);

#endif /* _LINUX_SCHED_USER_H */
