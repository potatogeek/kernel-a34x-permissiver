<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
>>>>>>> upstream/android-13
 *
 * Copyright (C) 2017 Hari Bathini, IBM Corporation
 */

#ifndef __PERF_NAMESPACES_H
#define __PERF_NAMESPACES_H

#include <sys/types.h>
<<<<<<< HEAD
=======
#include <sys/stat.h>
>>>>>>> upstream/android-13
#include <linux/stddef.h>
#include <linux/perf_event.h>
#include <linux/refcount.h>
#include <linux/types.h>

<<<<<<< HEAD
struct namespaces_event;
=======
#ifndef HAVE_SETNS_SUPPORT
int setns(int fd, int nstype);
#endif

struct perf_record_namespaces;
>>>>>>> upstream/android-13

struct namespaces {
	struct list_head list;
	u64 end_time;
	struct perf_ns_link_info link_info[];
};

<<<<<<< HEAD
struct namespaces *namespaces__new(struct namespaces_event *event);
=======
struct namespaces *namespaces__new(struct perf_record_namespaces *event);
>>>>>>> upstream/android-13
void namespaces__free(struct namespaces *namespaces);

struct nsinfo {
	pid_t			pid;
	pid_t			tgid;
	pid_t			nstgid;
	bool			need_setns;
<<<<<<< HEAD
=======
	bool			in_pidns;
>>>>>>> upstream/android-13
	char			*mntns_path;
	refcount_t		refcnt;
};

struct nscookie {
	int			oldns;
	int			newns;
	char			*oldcwd;
};

int nsinfo__init(struct nsinfo *nsi);
struct nsinfo *nsinfo__new(pid_t pid);
struct nsinfo *nsinfo__copy(struct nsinfo *nsi);
void nsinfo__delete(struct nsinfo *nsi);

struct nsinfo *nsinfo__get(struct nsinfo *nsi);
void nsinfo__put(struct nsinfo *nsi);

void nsinfo__mountns_enter(struct nsinfo *nsi, struct nscookie *nc);
void nsinfo__mountns_exit(struct nscookie *nc);

char *nsinfo__realpath(const char *path, struct nsinfo *nsi);
<<<<<<< HEAD
=======
int nsinfo__stat(const char *filename, struct stat *st, struct nsinfo *nsi);
>>>>>>> upstream/android-13

static inline void __nsinfo__zput(struct nsinfo **nsip)
{
	if (nsip) {
		nsinfo__put(*nsip);
		*nsip = NULL;
	}
}

#define nsinfo__zput(nsi) __nsinfo__zput(&nsi)

<<<<<<< HEAD
=======
const char *perf_ns__name(unsigned int id);

>>>>>>> upstream/android-13
#endif  /* __PERF_NAMESPACES_H */
