/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SECCOMP_H
#define _LINUX_SECCOMP_H

#include <uapi/linux/seccomp.h>

<<<<<<< HEAD
#define SECCOMP_FILTER_FLAG_MASK	(SECCOMP_FILTER_FLAG_TSYNC	| \
					 SECCOMP_FILTER_FLAG_LOG	| \
					 SECCOMP_FILTER_FLAG_SPEC_ALLOW)
=======
#define SECCOMP_FILTER_FLAG_MASK	(SECCOMP_FILTER_FLAG_TSYNC | \
					 SECCOMP_FILTER_FLAG_LOG | \
					 SECCOMP_FILTER_FLAG_SPEC_ALLOW | \
					 SECCOMP_FILTER_FLAG_NEW_LISTENER | \
					 SECCOMP_FILTER_FLAG_TSYNC_ESRCH)

/* sizeof() the first published struct seccomp_notif_addfd */
#define SECCOMP_NOTIFY_ADDFD_SIZE_VER0 24
#define SECCOMP_NOTIFY_ADDFD_SIZE_LATEST SECCOMP_NOTIFY_ADDFD_SIZE_VER0
>>>>>>> upstream/android-13

#ifdef CONFIG_SECCOMP

#include <linux/thread_info.h>
<<<<<<< HEAD
=======
#include <linux/atomic.h>
>>>>>>> upstream/android-13
#include <asm/seccomp.h>

struct seccomp_filter;
/**
 * struct seccomp - the state of a seccomp'ed process
 *
 * @mode:  indicates one of the valid values above for controlled
 *         system calls available to a process.
 * @filter: must always point to a valid seccomp-filter or NULL as it is
 *          accessed without locking during system call entry.
 *
 *          @filter must only be accessed from the context of current as there
 *          is no read locking.
 */
struct seccomp {
	int mode;
<<<<<<< HEAD
=======
	atomic_t filter_count;
>>>>>>> upstream/android-13
	struct seccomp_filter *filter;
};

#ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
extern int __secure_computing(const struct seccomp_data *sd);
<<<<<<< HEAD
static inline int secure_computing(const struct seccomp_data *sd)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		return  __secure_computing(sd);
=======
static inline int secure_computing(void)
{
	if (unlikely(test_syscall_work(SECCOMP)))
		return  __secure_computing(NULL);
>>>>>>> upstream/android-13
	return 0;
}
#else
extern void secure_computing_strict(int this_syscall);
#endif

extern long prctl_get_seccomp(void);
<<<<<<< HEAD
extern long prctl_set_seccomp(unsigned long, char __user *);
=======
extern long prctl_set_seccomp(unsigned long, void __user *);
>>>>>>> upstream/android-13

static inline int seccomp_mode(struct seccomp *s)
{
	return s->mode;
}

#else /* CONFIG_SECCOMP */

#include <linux/errno.h>

struct seccomp { };
struct seccomp_filter { };
<<<<<<< HEAD

#ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
static inline int secure_computing(struct seccomp_data *sd) { return 0; }
=======
struct seccomp_data;

#ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
static inline int secure_computing(void) { return 0; }
static inline int __secure_computing(const struct seccomp_data *sd) { return 0; }
>>>>>>> upstream/android-13
#else
static inline void secure_computing_strict(int this_syscall) { return; }
#endif

static inline long prctl_get_seccomp(void)
{
	return -EINVAL;
}

static inline long prctl_set_seccomp(unsigned long arg2, char __user *arg3)
{
	return -EINVAL;
}

static inline int seccomp_mode(struct seccomp *s)
{
	return SECCOMP_MODE_DISABLED;
}
#endif /* CONFIG_SECCOMP */

#ifdef CONFIG_SECCOMP_FILTER
<<<<<<< HEAD
extern void put_seccomp_filter(struct task_struct *tsk);
extern void get_seccomp_filter(struct task_struct *tsk);
#else  /* CONFIG_SECCOMP_FILTER */
static inline void put_seccomp_filter(struct task_struct *tsk)
=======
extern void seccomp_filter_release(struct task_struct *tsk);
extern void get_seccomp_filter(struct task_struct *tsk);
#else  /* CONFIG_SECCOMP_FILTER */
static inline void seccomp_filter_release(struct task_struct *tsk)
>>>>>>> upstream/android-13
{
	return;
}
static inline void get_seccomp_filter(struct task_struct *tsk)
{
	return;
}
#endif /* CONFIG_SECCOMP_FILTER */

#if defined(CONFIG_SECCOMP_FILTER) && defined(CONFIG_CHECKPOINT_RESTORE)
extern long seccomp_get_filter(struct task_struct *task,
			       unsigned long filter_off, void __user *data);
extern long seccomp_get_metadata(struct task_struct *task,
				 unsigned long filter_off, void __user *data);
#else
static inline long seccomp_get_filter(struct task_struct *task,
				      unsigned long n, void __user *data)
{
	return -EINVAL;
}
static inline long seccomp_get_metadata(struct task_struct *task,
					unsigned long filter_off,
					void __user *data)
{
	return -EINVAL;
}
#endif /* CONFIG_SECCOMP_FILTER && CONFIG_CHECKPOINT_RESTORE */
<<<<<<< HEAD
=======

#ifdef CONFIG_SECCOMP_CACHE_DEBUG
struct seq_file;

int proc_pid_seccomp_cache(struct seq_file *m, struct pid_namespace *ns,
			   struct pid *pid, struct task_struct *task);
#endif
>>>>>>> upstream/android-13
#endif /* _LINUX_SECCOMP_H */
