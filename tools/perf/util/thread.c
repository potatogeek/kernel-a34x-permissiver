// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include "../perf.h"
=======
>>>>>>> upstream/android-13
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include "session.h"
#include "thread.h"
#include "thread-stack.h"
#include "util.h"
#include "debug.h"
#include "namespaces.h"
#include "comm.h"
#include "unwind.h"

#include <api/fs/fs.h>

int thread__init_map_groups(struct thread *thread, struct machine *machine)
=======
#include <linux/zalloc.h>
#include "dso.h"
#include "session.h"
#include "thread.h"
#include "thread-stack.h"
#include "debug.h"
#include "namespaces.h"
#include "comm.h"
#include "map.h"
#include "symbol.h"
#include "unwind.h"
#include "callchain.h"

#include <api/fs/fs.h>

int thread__init_maps(struct thread *thread, struct machine *machine)
>>>>>>> upstream/android-13
{
	pid_t pid = thread->pid_;

	if (pid == thread->tid || pid == -1) {
<<<<<<< HEAD
		thread->mg = map_groups__new(machine);
	} else {
		struct thread *leader = __machine__findnew_thread(machine, pid, pid);
		if (leader) {
			thread->mg = map_groups__get(leader->mg);
=======
		thread->maps = maps__new(machine);
	} else {
		struct thread *leader = __machine__findnew_thread(machine, pid, pid);
		if (leader) {
			thread->maps = maps__get(leader->maps);
>>>>>>> upstream/android-13
			thread__put(leader);
		}
	}

<<<<<<< HEAD
	return thread->mg ? 0 : -1;
=======
	return thread->maps ? 0 : -1;
>>>>>>> upstream/android-13
}

struct thread *thread__new(pid_t pid, pid_t tid)
{
	char *comm_str;
	struct comm *comm;
	struct thread *thread = zalloc(sizeof(*thread));

	if (thread != NULL) {
		thread->pid_ = pid;
		thread->tid = tid;
		thread->ppid = -1;
		thread->cpu = -1;
<<<<<<< HEAD
=======
		thread->lbr_stitch_enable = false;
>>>>>>> upstream/android-13
		INIT_LIST_HEAD(&thread->namespaces_list);
		INIT_LIST_HEAD(&thread->comm_list);
		init_rwsem(&thread->namespaces_lock);
		init_rwsem(&thread->comm_lock);

		comm_str = malloc(32);
		if (!comm_str)
			goto err_thread;

		snprintf(comm_str, 32, ":%d", tid);
		comm = comm__new(comm_str, 0, false);
		free(comm_str);
		if (!comm)
			goto err_thread;

		list_add(&comm->list, &thread->comm_list);
		refcount_set(&thread->refcnt, 1);
		RB_CLEAR_NODE(&thread->rb_node);
		/* Thread holds first ref to nsdata. */
		thread->nsinfo = nsinfo__new(pid);
<<<<<<< HEAD
=======
		srccode_state_init(&thread->srccode_state);
>>>>>>> upstream/android-13
	}

	return thread;

err_thread:
	free(thread);
	return NULL;
}

void thread__delete(struct thread *thread)
{
	struct namespaces *namespaces, *tmp_namespaces;
	struct comm *comm, *tmp_comm;

	BUG_ON(!RB_EMPTY_NODE(&thread->rb_node));

	thread_stack__free(thread);

<<<<<<< HEAD
	if (thread->mg) {
		map_groups__put(thread->mg);
		thread->mg = NULL;
=======
	if (thread->maps) {
		maps__put(thread->maps);
		thread->maps = NULL;
>>>>>>> upstream/android-13
	}
	down_write(&thread->namespaces_lock);
	list_for_each_entry_safe(namespaces, tmp_namespaces,
				 &thread->namespaces_list, list) {
<<<<<<< HEAD
		list_del(&namespaces->list);
=======
		list_del_init(&namespaces->list);
>>>>>>> upstream/android-13
		namespaces__free(namespaces);
	}
	up_write(&thread->namespaces_lock);

	down_write(&thread->comm_lock);
	list_for_each_entry_safe(comm, tmp_comm, &thread->comm_list, list) {
<<<<<<< HEAD
		list_del(&comm->list);
=======
		list_del_init(&comm->list);
>>>>>>> upstream/android-13
		comm__free(comm);
	}
	up_write(&thread->comm_lock);

<<<<<<< HEAD
	unwind__finish_access(thread);
	nsinfo__zput(thread->nsinfo);

	exit_rwsem(&thread->namespaces_lock);
	exit_rwsem(&thread->comm_lock);
=======
	nsinfo__zput(thread->nsinfo);
	srccode_state_free(&thread->srccode_state);

	exit_rwsem(&thread->namespaces_lock);
	exit_rwsem(&thread->comm_lock);
	thread__free_stitch_list(thread);
>>>>>>> upstream/android-13
	free(thread);
}

struct thread *thread__get(struct thread *thread)
{
	if (thread)
		refcount_inc(&thread->refcnt);
	return thread;
}

void thread__put(struct thread *thread)
{
	if (thread && refcount_dec_and_test(&thread->refcnt)) {
		/*
<<<<<<< HEAD
		 * Remove it from the dead_threads list, as last reference
		 * is gone.
		 */
		list_del_init(&thread->node);
=======
		 * Remove it from the dead threads list, as last reference is
		 * gone, if it is in a dead threads list.
		 *
		 * We may not be there anymore if say, the machine where it was
		 * stored was already deleted, so we already removed it from
		 * the dead threads and some other piece of code still keeps a
		 * reference.
		 *
		 * This is what 'perf sched' does and finally drops it in
		 * perf_sched__lat(), where it calls perf_sched__read_events(),
		 * that processes the events by creating a session and deleting
		 * it, which ends up destroying the list heads for the dead
		 * threads, but before it does that it removes all threads from
		 * it using list_del_init().
		 *
		 * So we need to check here if it is in a dead threads list and
		 * if so, remove it before finally deleting the thread, to avoid
		 * an use after free situation.
		 */
		if (!list_empty(&thread->node))
			list_del_init(&thread->node);
>>>>>>> upstream/android-13
		thread__delete(thread);
	}
}

static struct namespaces *__thread__namespaces(const struct thread *thread)
{
	if (list_empty(&thread->namespaces_list))
		return NULL;

	return list_first_entry(&thread->namespaces_list, struct namespaces, list);
}

<<<<<<< HEAD
struct namespaces *thread__namespaces(const struct thread *thread)
{
	struct namespaces *ns;

	down_read((struct rw_semaphore *)&thread->namespaces_lock);
	ns = __thread__namespaces(thread);
	up_read((struct rw_semaphore *)&thread->namespaces_lock);
=======
struct namespaces *thread__namespaces(struct thread *thread)
{
	struct namespaces *ns;

	down_read(&thread->namespaces_lock);
	ns = __thread__namespaces(thread);
	up_read(&thread->namespaces_lock);
>>>>>>> upstream/android-13

	return ns;
}

static int __thread__set_namespaces(struct thread *thread, u64 timestamp,
<<<<<<< HEAD
				    struct namespaces_event *event)
=======
				    struct perf_record_namespaces *event)
>>>>>>> upstream/android-13
{
	struct namespaces *new, *curr = __thread__namespaces(thread);

	new = namespaces__new(event);
	if (!new)
		return -ENOMEM;

	list_add(&new->list, &thread->namespaces_list);

	if (timestamp && curr) {
		/*
		 * setns syscall must have changed few or all the namespaces
		 * of this thread. Update end time for the namespaces
		 * previously used.
		 */
		curr = list_next_entry(new, list);
		curr->end_time = timestamp;
	}

	return 0;
}

int thread__set_namespaces(struct thread *thread, u64 timestamp,
<<<<<<< HEAD
			   struct namespaces_event *event)
=======
			   struct perf_record_namespaces *event)
>>>>>>> upstream/android-13
{
	int ret;

	down_write(&thread->namespaces_lock);
	ret = __thread__set_namespaces(thread, timestamp, event);
	up_write(&thread->namespaces_lock);
	return ret;
}

struct comm *thread__comm(const struct thread *thread)
{
	if (list_empty(&thread->comm_list))
		return NULL;

	return list_first_entry(&thread->comm_list, struct comm, list);
}

struct comm *thread__exec_comm(const struct thread *thread)
{
	struct comm *comm, *last = NULL, *second_last = NULL;

	list_for_each_entry(comm, &thread->comm_list, list) {
		if (comm->exec)
			return comm;
		second_last = last;
		last = comm;
	}

	/*
	 * 'last' with no start time might be the parent's comm of a synthesized
	 * thread (created by processing a synthesized fork event). For a main
	 * thread, that is very probably wrong. Prefer a later comm to avoid
	 * that case.
	 */
	if (second_last && !last->start && thread->pid_ == thread->tid)
		return second_last;

	return last;
}

static int ____thread__set_comm(struct thread *thread, const char *str,
				u64 timestamp, bool exec)
{
	struct comm *new, *curr = thread__comm(thread);

	/* Override the default :tid entry */
	if (!thread->comm_set) {
		int err = comm__override(curr, str, timestamp, exec);
		if (err)
			return err;
	} else {
		new = comm__new(str, timestamp, exec);
		if (!new)
			return -ENOMEM;
		list_add(&new->list, &thread->comm_list);

		if (exec)
<<<<<<< HEAD
			unwind__flush_access(thread);
=======
			unwind__flush_access(thread->maps);
>>>>>>> upstream/android-13
	}

	thread->comm_set = true;

	return 0;
}

int __thread__set_comm(struct thread *thread, const char *str, u64 timestamp,
		       bool exec)
{
	int ret;

	down_write(&thread->comm_lock);
	ret = ____thread__set_comm(thread, str, timestamp, exec);
	up_write(&thread->comm_lock);
	return ret;
}

int thread__set_comm_from_proc(struct thread *thread)
{
	char path[64];
	char *comm = NULL;
	size_t sz;
	int err = -1;

	if (!(snprintf(path, sizeof(path), "%d/task/%d/comm",
		       thread->pid_, thread->tid) >= (int)sizeof(path)) &&
	    procfs__read_str(path, &comm, &sz) == 0) {
		comm[sz - 1] = '\0';
		err = thread__set_comm(thread, comm, 0);
	}

	return err;
}

static const char *__thread__comm_str(const struct thread *thread)
{
	const struct comm *comm = thread__comm(thread);

	if (!comm)
		return NULL;

	return comm__str(comm);
}

<<<<<<< HEAD
const char *thread__comm_str(const struct thread *thread)
{
	const char *str;

	down_read((struct rw_semaphore *)&thread->comm_lock);
	str = __thread__comm_str(thread);
	up_read((struct rw_semaphore *)&thread->comm_lock);
=======
const char *thread__comm_str(struct thread *thread)
{
	const char *str;

	down_read(&thread->comm_lock);
	str = __thread__comm_str(thread);
	up_read(&thread->comm_lock);
>>>>>>> upstream/android-13

	return str;
}

/* CHECKME: it should probably better return the max comm len from its comm list */
int thread__comm_len(struct thread *thread)
{
	if (!thread->comm_len) {
		const char *comm = thread__comm_str(thread);
		if (!comm)
			return 0;
		thread->comm_len = strlen(comm);
	}

	return thread->comm_len;
}

size_t thread__fprintf(struct thread *thread, FILE *fp)
{
	return fprintf(fp, "Thread %d %s\n", thread->tid, thread__comm_str(thread)) +
<<<<<<< HEAD
	       map_groups__fprintf(thread->mg, fp);
=======
	       maps__fprintf(thread->maps, fp);
>>>>>>> upstream/android-13
}

int thread__insert_map(struct thread *thread, struct map *map)
{
	int ret;

<<<<<<< HEAD
	ret = unwind__prepare_access(thread, map, NULL);
	if (ret)
		return ret;

	map_groups__fixup_overlappings(thread->mg, map, stderr);
	map_groups__insert(thread->mg, map);
=======
	ret = unwind__prepare_access(thread->maps, map, NULL);
	if (ret)
		return ret;

	maps__fixup_overlappings(thread->maps, map, stderr);
	maps__insert(thread->maps, map);
>>>>>>> upstream/android-13

	return 0;
}

static int __thread__prepare_access(struct thread *thread)
{
	bool initialized = false;
	int err = 0;
<<<<<<< HEAD
	struct maps *maps = &thread->mg->maps;
=======
	struct maps *maps = thread->maps;
>>>>>>> upstream/android-13
	struct map *map;

	down_read(&maps->lock);

<<<<<<< HEAD
	for (map = maps__first(maps); map; map = map__next(map)) {
		err = unwind__prepare_access(thread, map, &initialized);
=======
	maps__for_each_entry(maps, map) {
		err = unwind__prepare_access(thread->maps, map, &initialized);
>>>>>>> upstream/android-13
		if (err || initialized)
			break;
	}

	up_read(&maps->lock);

	return err;
}

static int thread__prepare_access(struct thread *thread)
{
	int err = 0;

<<<<<<< HEAD
	if (symbol_conf.use_callchain)
=======
	if (dwarf_callchain_users)
>>>>>>> upstream/android-13
		err = __thread__prepare_access(thread);

	return err;
}

<<<<<<< HEAD
static int thread__clone_map_groups(struct thread *thread,
				    struct thread *parent)
=======
static int thread__clone_maps(struct thread *thread, struct thread *parent, bool do_maps_clone)
>>>>>>> upstream/android-13
{
	/* This is new thread, we share map groups for process. */
	if (thread->pid_ == parent->pid_)
		return thread__prepare_access(thread);

<<<<<<< HEAD
	if (thread->mg == parent->mg) {
=======
	if (thread->maps == parent->maps) {
>>>>>>> upstream/android-13
		pr_debug("broken map groups on thread %d/%d parent %d/%d\n",
			 thread->pid_, thread->tid, parent->pid_, parent->tid);
		return 0;
	}
<<<<<<< HEAD

	/* But this one is new process, copy maps. */
	if (map_groups__clone(thread, parent->mg) < 0)
		return -ENOMEM;

	return 0;
}

int thread__fork(struct thread *thread, struct thread *parent, u64 timestamp)
=======
	/* But this one is new process, copy maps. */
	return do_maps_clone ? maps__clone(thread, parent->maps) : 0;
}

int thread__fork(struct thread *thread, struct thread *parent, u64 timestamp, bool do_maps_clone)
>>>>>>> upstream/android-13
{
	if (parent->comm_set) {
		const char *comm = thread__comm_str(parent);
		int err;
		if (!comm)
			return -ENOMEM;
		err = thread__set_comm(thread, comm, timestamp);
		if (err)
			return err;
	}

	thread->ppid = parent->tid;
<<<<<<< HEAD
	return thread__clone_map_groups(thread, parent);
=======
	return thread__clone_maps(thread, parent, do_maps_clone);
>>>>>>> upstream/android-13
}

void thread__find_cpumode_addr_location(struct thread *thread, u64 addr,
					struct addr_location *al)
{
	size_t i;
	const u8 cpumodes[] = {
		PERF_RECORD_MISC_USER,
		PERF_RECORD_MISC_KERNEL,
		PERF_RECORD_MISC_GUEST_USER,
		PERF_RECORD_MISC_GUEST_KERNEL
	};

	for (i = 0; i < ARRAY_SIZE(cpumodes); i++) {
		thread__find_symbol(thread, cpumodes[i], addr, al);
		if (al->map)
			break;
	}
}

struct thread *thread__main_thread(struct machine *machine, struct thread *thread)
{
	if (thread->pid_ == thread->tid)
		return thread__get(thread);

	if (thread->pid_ == -1)
		return NULL;

	return machine__find_thread(machine, thread->pid_, thread->pid_);
}
<<<<<<< HEAD
=======

int thread__memcpy(struct thread *thread, struct machine *machine,
		   void *buf, u64 ip, int len, bool *is64bit)
{
       u8 cpumode = PERF_RECORD_MISC_USER;
       struct addr_location al;
       long offset;

       if (machine__kernel_ip(machine, ip))
               cpumode = PERF_RECORD_MISC_KERNEL;

       if (!thread__find_map(thread, cpumode, ip, &al) || !al.map->dso ||
	   al.map->dso->data.status == DSO_DATA_STATUS_ERROR ||
	   map__load(al.map) < 0)
               return -1;

       offset = al.map->map_ip(al.map, ip);
       if (is64bit)
               *is64bit = al.map->dso->is_64_bit;

       return dso__data_read_offset(al.map->dso, machine, offset, buf, len);
}

void thread__free_stitch_list(struct thread *thread)
{
	struct lbr_stitch *lbr_stitch = thread->lbr_stitch;
	struct stitch_list *pos, *tmp;

	if (!lbr_stitch)
		return;

	list_for_each_entry_safe(pos, tmp, &lbr_stitch->lists, node) {
		list_del_init(&pos->node);
		free(pos);
	}

	list_for_each_entry_safe(pos, tmp, &lbr_stitch->free_lists, node) {
		list_del_init(&pos->node);
		free(pos);
	}

	zfree(&lbr_stitch->prev_lbr_cursor);
	zfree(&thread->lbr_stitch);
}
>>>>>>> upstream/android-13
