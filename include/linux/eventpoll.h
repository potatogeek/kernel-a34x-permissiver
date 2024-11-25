<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 *  include/linux/eventpoll.h ( Efficient event polling implementation )
 *  Copyright (C) 2001,...,2006	 Davide Libenzi
 *
<<<<<<< HEAD
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Davide Libenzi <davidel@xmailserver.org>
 *
=======
 *  Davide Libenzi <davidel@xmailserver.org>
>>>>>>> upstream/android-13
 */
#ifndef _LINUX_EVENTPOLL_H
#define _LINUX_EVENTPOLL_H

#include <uapi/linux/eventpoll.h>
#include <uapi/linux/kcmp.h>


/* Forward declarations to avoid compiler errors */
struct file;


#ifdef CONFIG_EPOLL

<<<<<<< HEAD
#ifdef CONFIG_CHECKPOINT_RESTORE
struct file *get_epoll_tfile_raw_ptr(struct file *file, int tfd, unsigned long toff);
#endif

/* Used to initialize the epoll bits inside the "struct file" */
static inline void eventpoll_init_file(struct file *file)
{
	INIT_LIST_HEAD(&file->f_ep_links);
	INIT_LIST_HEAD(&file->f_tfile_llink);
}


=======
#ifdef CONFIG_KCMP
struct file *get_epoll_tfile_raw_ptr(struct file *file, int tfd, unsigned long toff);
#endif

>>>>>>> upstream/android-13
/* Used to release the epoll bits inside the "struct file" */
void eventpoll_release_file(struct file *file);

/*
 * This is called from inside fs/file_table.c:__fput() to unlink files
 * from the eventpoll interface. We need to have this facility to cleanup
 * correctly files that are closed without being removed from the eventpoll
 * interface.
 */
static inline void eventpoll_release(struct file *file)
{

	/*
	 * Fast check to avoid the get/release of the semaphore. Since
	 * we're doing this outside the semaphore lock, it might return
	 * false negatives, but we don't care. It'll help in 99.99% of cases
	 * to avoid the semaphore lock. False positives simply cannot happen
	 * because the file in on the way to be removed and nobody ( but
	 * eventpoll ) has still a reference to this file.
	 */
<<<<<<< HEAD
	if (likely(list_empty(&file->f_ep_links)))
=======
	if (likely(!file->f_ep))
>>>>>>> upstream/android-13
		return;

	/*
	 * The file is being closed while it is still linked to an epoll
	 * descriptor. We need to handle this by correctly unlinking it
	 * from its containers.
	 */
	eventpoll_release_file(file);
}

<<<<<<< HEAD
#else

static inline void eventpoll_init_file(struct file *file) {}
=======
int do_epoll_ctl(int epfd, int op, int fd, struct epoll_event *epds,
		 bool nonblock);

/* Tells if the epoll_ctl(2) operation needs an event copy from userspace */
static inline int ep_op_has_event(int op)
{
	return op != EPOLL_CTL_DEL;
}

#else

>>>>>>> upstream/android-13
static inline void eventpoll_release(struct file *file) {}

#endif

<<<<<<< HEAD
=======
#if defined(CONFIG_ARM) && defined(CONFIG_OABI_COMPAT)
/* ARM OABI has an incompatible struct layout and needs a special handler */
extern struct epoll_event __user *
epoll_put_uevent(__poll_t revents, __u64 data,
		 struct epoll_event __user *uevent);
#else
static inline struct epoll_event __user *
epoll_put_uevent(__poll_t revents, __u64 data,
		 struct epoll_event __user *uevent)
{
	if (__put_user(revents, &uevent->events) ||
	    __put_user(data, &uevent->data))
		return NULL;

	return uevent+1;
}
#endif

>>>>>>> upstream/android-13
#endif /* #ifndef _LINUX_EVENTPOLL_H */
