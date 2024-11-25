<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2011, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
 *
 * Parts came from builtin-{top,stat,record}.c, see those files for further
 * copyright notes.
<<<<<<< HEAD
 *
 * Released under the GPL v2. (and only v2, not any later version)
 */
#include "util.h"
=======
 */
>>>>>>> upstream/android-13
#include <api/fs/fs.h>
#include <errno.h>
#include <inttypes.h>
#include <poll.h>
#include "cpumap.h"
<<<<<<< HEAD
=======
#include "util/mmap.h"
>>>>>>> upstream/android-13
#include "thread_map.h"
#include "target.h"
#include "evlist.h"
#include "evsel.h"
#include "debug.h"
#include "units.h"
<<<<<<< HEAD
#include "asm/bug.h"
#include <signal.h>
#include <unistd.h>
=======
#include "bpf_counter.h"
#include <internal/lib.h> // page_size
#include "affinity.h"
#include "../perf.h"
#include "asm/bug.h"
#include "bpf-event.h"
#include "util/string2.h"
#include "util/perf_api_probe.h"
#include "util/evsel_fprintf.h"
#include "util/evlist-hybrid.h"
#include "util/pmu.h"
#include <signal.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
>>>>>>> upstream/android-13

#include "parse-events.h"
#include <subcmd/parse-options.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
<<<<<<< HEAD
=======
#include <sys/prctl.h>
>>>>>>> upstream/android-13

#include <linux/bitops.h>
#include <linux/hash.h>
#include <linux/log2.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/string.h>
#include <linux/zalloc.h>
#include <perf/evlist.h>
#include <perf/evsel.h>
#include <perf/cpumap.h>
#include <perf/mmap.h>

#include <internal/xyarray.h>
>>>>>>> upstream/android-13

#ifdef LACKS_SIGQUEUE_PROTOTYPE
int sigqueue(pid_t pid, int sig, const union sigval value);
#endif

<<<<<<< HEAD
#define FD(e, x, y) (*(int *)xyarray__entry(e->fd, x, y))
#define SID(e, x, y) xyarray__entry(e->sample_id, x, y)

void perf_evlist__init(struct perf_evlist *evlist, struct cpu_map *cpus,
		       struct thread_map *threads)
{
	int i;

	for (i = 0; i < PERF_EVLIST__HLIST_SIZE; ++i)
		INIT_HLIST_HEAD(&evlist->heads[i]);
	INIT_LIST_HEAD(&evlist->entries);
	perf_evlist__set_maps(evlist, cpus, threads);
	fdarray__init(&evlist->pollfd, 64);
	evlist->workload.pid = -1;
	evlist->bkw_mmap_state = BKW_MMAP_NOTREADY;
}

struct perf_evlist *perf_evlist__new(void)
{
	struct perf_evlist *evlist = zalloc(sizeof(*evlist));

	if (evlist != NULL)
		perf_evlist__init(evlist, NULL, NULL);
=======
#define FD(e, x, y) (*(int *)xyarray__entry(e->core.fd, x, y))
#define SID(e, x, y) xyarray__entry(e->core.sample_id, x, y)

void evlist__init(struct evlist *evlist, struct perf_cpu_map *cpus,
		  struct perf_thread_map *threads)
{
	perf_evlist__init(&evlist->core);
	perf_evlist__set_maps(&evlist->core, cpus, threads);
	evlist->workload.pid = -1;
	evlist->bkw_mmap_state = BKW_MMAP_NOTREADY;
	evlist->ctl_fd.fd = -1;
	evlist->ctl_fd.ack = -1;
	evlist->ctl_fd.pos = -1;
}

struct evlist *evlist__new(void)
{
	struct evlist *evlist = zalloc(sizeof(*evlist));

	if (evlist != NULL)
		evlist__init(evlist, NULL, NULL);
>>>>>>> upstream/android-13

	return evlist;
}

<<<<<<< HEAD
struct perf_evlist *perf_evlist__new_default(void)
{
	struct perf_evlist *evlist = perf_evlist__new();

	if (evlist && perf_evlist__add_default(evlist)) {
		perf_evlist__delete(evlist);
=======
struct evlist *evlist__new_default(void)
{
	struct evlist *evlist = evlist__new();

	if (evlist && evlist__add_default(evlist)) {
		evlist__delete(evlist);
>>>>>>> upstream/android-13
		evlist = NULL;
	}

	return evlist;
}

<<<<<<< HEAD
struct perf_evlist *perf_evlist__new_dummy(void)
{
	struct perf_evlist *evlist = perf_evlist__new();

	if (evlist && perf_evlist__add_dummy(evlist)) {
		perf_evlist__delete(evlist);
=======
struct evlist *evlist__new_dummy(void)
{
	struct evlist *evlist = evlist__new();

	if (evlist && evlist__add_dummy(evlist)) {
		evlist__delete(evlist);
>>>>>>> upstream/android-13
		evlist = NULL;
	}

	return evlist;
}

/**
<<<<<<< HEAD
 * perf_evlist__set_id_pos - set the positions of event ids.
=======
 * evlist__set_id_pos - set the positions of event ids.
>>>>>>> upstream/android-13
 * @evlist: selected event list
 *
 * Events with compatible sample types all have the same id_pos
 * and is_pos.  For convenience, put a copy on evlist.
 */
<<<<<<< HEAD
void perf_evlist__set_id_pos(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
=======
void evlist__set_id_pos(struct evlist *evlist)
{
	struct evsel *first = evlist__first(evlist);
>>>>>>> upstream/android-13

	evlist->id_pos = first->id_pos;
	evlist->is_pos = first->is_pos;
}

<<<<<<< HEAD
static void perf_evlist__update_id_pos(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel)
		perf_evsel__calc_id_pos(evsel);

	perf_evlist__set_id_pos(evlist);
}

static void perf_evlist__purge(struct perf_evlist *evlist)
{
	struct perf_evsel *pos, *n;

	evlist__for_each_entry_safe(evlist, n, pos) {
		list_del_init(&pos->node);
		pos->evlist = NULL;
		perf_evsel__delete(pos);
	}

	evlist->nr_entries = 0;
}

void perf_evlist__exit(struct perf_evlist *evlist)
{
	zfree(&evlist->mmap);
	zfree(&evlist->overwrite_mmap);
	fdarray__exit(&evlist->pollfd);
}

void perf_evlist__delete(struct perf_evlist *evlist)
=======
static void evlist__update_id_pos(struct evlist *evlist)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel)
		evsel__calc_id_pos(evsel);

	evlist__set_id_pos(evlist);
}

static void evlist__purge(struct evlist *evlist)
{
	struct evsel *pos, *n;

	evlist__for_each_entry_safe(evlist, n, pos) {
		list_del_init(&pos->core.node);
		pos->evlist = NULL;
		evsel__delete(pos);
	}

	evlist->core.nr_entries = 0;
}

void evlist__exit(struct evlist *evlist)
{
	zfree(&evlist->mmap);
	zfree(&evlist->overwrite_mmap);
	perf_evlist__exit(&evlist->core);
}

void evlist__delete(struct evlist *evlist)
>>>>>>> upstream/android-13
{
	if (evlist == NULL)
		return;

<<<<<<< HEAD
	perf_evlist__munmap(evlist);
	perf_evlist__close(evlist);
	cpu_map__put(evlist->cpus);
	thread_map__put(evlist->threads);
	evlist->cpus = NULL;
	evlist->threads = NULL;
	perf_evlist__purge(evlist);
	perf_evlist__exit(evlist);
	free(evlist);
}

static void __perf_evlist__propagate_maps(struct perf_evlist *evlist,
					  struct perf_evsel *evsel)
{
	/*
	 * We already have cpus for evsel (via PMU sysfs) so
	 * keep it, if there's no target cpu list defined.
	 */
	if (!evsel->own_cpus || evlist->has_user_cpus) {
		cpu_map__put(evsel->cpus);
		evsel->cpus = cpu_map__get(evlist->cpus);
	} else if (evsel->cpus != evsel->own_cpus) {
		cpu_map__put(evsel->cpus);
		evsel->cpus = cpu_map__get(evsel->own_cpus);
	}

	thread_map__put(evsel->threads);
	evsel->threads = thread_map__get(evlist->threads);
}

static void perf_evlist__propagate_maps(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel)
		__perf_evlist__propagate_maps(evlist, evsel);
}

void perf_evlist__add(struct perf_evlist *evlist, struct perf_evsel *entry)
{
	entry->evlist = evlist;
	list_add_tail(&entry->node, &evlist->entries);
	entry->idx = evlist->nr_entries;
	entry->tracking = !entry->idx;

	if (!evlist->nr_entries++)
		perf_evlist__set_id_pos(evlist);

	__perf_evlist__propagate_maps(evlist, entry);
}

void perf_evlist__remove(struct perf_evlist *evlist, struct perf_evsel *evsel)
{
	evsel->evlist = NULL;
	list_del_init(&evsel->node);
	evlist->nr_entries -= 1;
}

void perf_evlist__splice_list_tail(struct perf_evlist *evlist,
				   struct list_head *list)
{
	struct perf_evsel *evsel, *temp;

	__evlist__for_each_entry_safe(list, temp, evsel) {
		list_del_init(&evsel->node);
		perf_evlist__add(evlist, evsel);
	}
}

void __perf_evlist__set_leader(struct list_head *list)
{
	struct perf_evsel *evsel, *leader;

	leader = list_entry(list->next, struct perf_evsel, node);
	evsel = list_entry(list->prev, struct perf_evsel, node);

	leader->nr_members = evsel->idx - leader->idx + 1;

	__evlist__for_each_entry(list, evsel) {
		evsel->leader = leader;
	}
}

void perf_evlist__set_leader(struct perf_evlist *evlist)
{
	if (evlist->nr_entries) {
		evlist->nr_groups = evlist->nr_entries > 1 ? 1 : 0;
		__perf_evlist__set_leader(&evlist->entries);
	}
}

void perf_event_attr__set_max_precise_ip(struct perf_event_attr *attr)
{
	attr->precise_ip = 3;

	while (attr->precise_ip != 0) {
		int fd = sys_perf_event_open(attr, 0, -1, -1, 0);
		if (fd != -1) {
			close(fd);
			break;
		}
		--attr->precise_ip;
	}
}

int __perf_evlist__add_default(struct perf_evlist *evlist, bool precise)
{
	struct perf_evsel *evsel = perf_evsel__new_cycles(precise);

	if (evsel == NULL)
		return -ENOMEM;

	perf_evlist__add(evlist, evsel);
	return 0;
}

int perf_evlist__add_dummy(struct perf_evlist *evlist)
=======
	evlist__munmap(evlist);
	evlist__close(evlist);
	evlist__purge(evlist);
	evlist__exit(evlist);
	free(evlist);
}

void evlist__add(struct evlist *evlist, struct evsel *entry)
{
	perf_evlist__add(&evlist->core, &entry->core);
	entry->evlist = evlist;
	entry->tracking = !entry->core.idx;

	if (evlist->core.nr_entries == 1)
		evlist__set_id_pos(evlist);
}

void evlist__remove(struct evlist *evlist, struct evsel *evsel)
{
	evsel->evlist = NULL;
	perf_evlist__remove(&evlist->core, &evsel->core);
}

void evlist__splice_list_tail(struct evlist *evlist, struct list_head *list)
{
	while (!list_empty(list)) {
		struct evsel *evsel, *temp, *leader = NULL;

		__evlist__for_each_entry_safe(list, temp, evsel) {
			list_del_init(&evsel->core.node);
			evlist__add(evlist, evsel);
			leader = evsel;
			break;
		}

		__evlist__for_each_entry_safe(list, temp, evsel) {
			if (evsel__has_leader(evsel, leader)) {
				list_del_init(&evsel->core.node);
				evlist__add(evlist, evsel);
			}
		}
	}
}

int __evlist__set_tracepoints_handlers(struct evlist *evlist,
				       const struct evsel_str_handler *assocs, size_t nr_assocs)
{
	size_t i;
	int err;

	for (i = 0; i < nr_assocs; i++) {
		// Adding a handler for an event not in this evlist, just ignore it.
		struct evsel *evsel = evlist__find_tracepoint_by_name(evlist, assocs[i].name);
		if (evsel == NULL)
			continue;

		err = -EEXIST;
		if (evsel->handler != NULL)
			goto out;
		evsel->handler = assocs[i].handler;
	}

	err = 0;
out:
	return err;
}

void evlist__set_leader(struct evlist *evlist)
{
	perf_evlist__set_leader(&evlist->core);
}

int __evlist__add_default(struct evlist *evlist, bool precise)
{
	struct evsel *evsel;

	evsel = evsel__new_cycles(precise, PERF_TYPE_HARDWARE,
				  PERF_COUNT_HW_CPU_CYCLES);
	if (evsel == NULL)
		return -ENOMEM;

	evlist__add(evlist, evsel);
	return 0;
}

int evlist__add_dummy(struct evlist *evlist)
>>>>>>> upstream/android-13
{
	struct perf_event_attr attr = {
		.type	= PERF_TYPE_SOFTWARE,
		.config = PERF_COUNT_SW_DUMMY,
		.size	= sizeof(attr), /* to capture ABI version */
	};
<<<<<<< HEAD
	struct perf_evsel *evsel = perf_evsel__new_idx(&attr, evlist->nr_entries);
=======
	struct evsel *evsel = evsel__new_idx(&attr, evlist->core.nr_entries);
>>>>>>> upstream/android-13

	if (evsel == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	perf_evlist__add(evlist, evsel);
	return 0;
}

static int perf_evlist__add_attrs(struct perf_evlist *evlist,
				  struct perf_event_attr *attrs, size_t nr_attrs)
{
	struct perf_evsel *evsel, *n;
=======
	evlist__add(evlist, evsel);
	return 0;
}

static int evlist__add_attrs(struct evlist *evlist, struct perf_event_attr *attrs, size_t nr_attrs)
{
	struct evsel *evsel, *n;
>>>>>>> upstream/android-13
	LIST_HEAD(head);
	size_t i;

	for (i = 0; i < nr_attrs; i++) {
<<<<<<< HEAD
		evsel = perf_evsel__new_idx(attrs + i, evlist->nr_entries + i);
		if (evsel == NULL)
			goto out_delete_partial_list;
		list_add_tail(&evsel->node, &head);
	}

	perf_evlist__splice_list_tail(evlist, &head);
=======
		evsel = evsel__new_idx(attrs + i, evlist->core.nr_entries + i);
		if (evsel == NULL)
			goto out_delete_partial_list;
		list_add_tail(&evsel->core.node, &head);
	}

	evlist__splice_list_tail(evlist, &head);
>>>>>>> upstream/android-13

	return 0;

out_delete_partial_list:
	__evlist__for_each_entry_safe(&head, n, evsel)
<<<<<<< HEAD
		perf_evsel__delete(evsel);
	return -1;
}

int __perf_evlist__add_default_attrs(struct perf_evlist *evlist,
				     struct perf_event_attr *attrs, size_t nr_attrs)
=======
		evsel__delete(evsel);
	return -1;
}

int __evlist__add_default_attrs(struct evlist *evlist, struct perf_event_attr *attrs, size_t nr_attrs)
>>>>>>> upstream/android-13
{
	size_t i;

	for (i = 0; i < nr_attrs; i++)
		event_attr_init(attrs + i);

<<<<<<< HEAD
	return perf_evlist__add_attrs(evlist, attrs, nr_attrs);
}

struct perf_evsel *
perf_evlist__find_tracepoint_by_id(struct perf_evlist *evlist, int id)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->attr.type   == PERF_TYPE_TRACEPOINT &&
		    (int)evsel->attr.config == id)
=======
	return evlist__add_attrs(evlist, attrs, nr_attrs);
}

__weak int arch_evlist__add_default_attrs(struct evlist *evlist __maybe_unused)
{
	return 0;
}

struct evsel *evlist__find_tracepoint_by_id(struct evlist *evlist, int id)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->core.attr.type   == PERF_TYPE_TRACEPOINT &&
		    (int)evsel->core.attr.config == id)
>>>>>>> upstream/android-13
			return evsel;
	}

	return NULL;
}

<<<<<<< HEAD
struct perf_evsel *
perf_evlist__find_tracepoint_by_name(struct perf_evlist *evlist,
				     const char *name)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if ((evsel->attr.type == PERF_TYPE_TRACEPOINT) &&
=======
struct evsel *evlist__find_tracepoint_by_name(struct evlist *evlist, const char *name)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if ((evsel->core.attr.type == PERF_TYPE_TRACEPOINT) &&
>>>>>>> upstream/android-13
		    (strcmp(evsel->name, name) == 0))
			return evsel;
	}

	return NULL;
}

<<<<<<< HEAD
int perf_evlist__add_newtp(struct perf_evlist *evlist,
			   const char *sys, const char *name, void *handler)
{
	struct perf_evsel *evsel = perf_evsel__newtp(sys, name);
=======
int evlist__add_newtp(struct evlist *evlist, const char *sys, const char *name, void *handler)
{
	struct evsel *evsel = evsel__newtp(sys, name);
>>>>>>> upstream/android-13

	if (IS_ERR(evsel))
		return -1;

	evsel->handler = handler;
<<<<<<< HEAD
	perf_evlist__add(evlist, evsel);
	return 0;
}

static int perf_evlist__nr_threads(struct perf_evlist *evlist,
				   struct perf_evsel *evsel)
{
	if (evsel->system_wide)
		return 1;
	else
		return thread_map__nr(evlist->threads);
}

void perf_evlist__disable(struct perf_evlist *evlist)
{
	struct perf_evsel *pos;

	evlist__for_each_entry(evlist, pos) {
		if (!perf_evsel__is_group_leader(pos) || !pos->fd)
			continue;
		perf_evsel__disable(pos);
	}

	evlist->enabled = false;
}

void perf_evlist__enable(struct perf_evlist *evlist)
{
	struct perf_evsel *pos;

	evlist__for_each_entry(evlist, pos) {
		if (!perf_evsel__is_group_leader(pos) || !pos->fd)
			continue;
		perf_evsel__enable(pos);
	}

	evlist->enabled = true;
}

void perf_evlist__toggle_enable(struct perf_evlist *evlist)
{
	(evlist->enabled ? perf_evlist__disable : perf_evlist__enable)(evlist);
}

static int perf_evlist__enable_event_cpu(struct perf_evlist *evlist,
					 struct perf_evsel *evsel, int cpu)
{
	int thread;
	int nr_threads = perf_evlist__nr_threads(evlist, evsel);

	if (!evsel->fd)
=======
	evlist__add(evlist, evsel);
	return 0;
}

static int evlist__nr_threads(struct evlist *evlist, struct evsel *evsel)
{
	if (evsel->core.system_wide)
		return 1;
	else
		return perf_thread_map__nr(evlist->core.threads);
}

void evlist__cpu_iter_start(struct evlist *evlist)
{
	struct evsel *pos;

	/*
	 * Reset the per evsel cpu_iter. This is needed because
	 * each evsel's cpumap may have a different index space,
	 * and some operations need the index to modify
	 * the FD xyarray (e.g. open, close)
	 */
	evlist__for_each_entry(evlist, pos)
		pos->cpu_iter = 0;
}

bool evsel__cpu_iter_skip_no_inc(struct evsel *ev, int cpu)
{
	if (ev->cpu_iter >= ev->core.cpus->nr)
		return true;
	if (cpu >= 0 && ev->core.cpus->map[ev->cpu_iter] != cpu)
		return true;
	return false;
}

bool evsel__cpu_iter_skip(struct evsel *ev, int cpu)
{
	if (!evsel__cpu_iter_skip_no_inc(ev, cpu)) {
		ev->cpu_iter++;
		return false;
	}
	return true;
}

static int evsel__strcmp(struct evsel *pos, char *evsel_name)
{
	if (!evsel_name)
		return 0;
	if (evsel__is_dummy_event(pos))
		return 1;
	return strcmp(pos->name, evsel_name);
}

static int evlist__is_enabled(struct evlist *evlist)
{
	struct evsel *pos;

	evlist__for_each_entry(evlist, pos) {
		if (!evsel__is_group_leader(pos) || !pos->core.fd)
			continue;
		/* If at least one event is enabled, evlist is enabled. */
		if (!pos->disabled)
			return true;
	}
	return false;
}

static void __evlist__disable(struct evlist *evlist, char *evsel_name)
{
	struct evsel *pos;
	struct affinity affinity;
	int cpu, i, imm = 0;
	bool has_imm = false;

	if (affinity__setup(&affinity) < 0)
		return;

	/* Disable 'immediate' events last */
	for (imm = 0; imm <= 1; imm++) {
		evlist__for_each_cpu(evlist, i, cpu) {
			affinity__set(&affinity, cpu);

			evlist__for_each_entry(evlist, pos) {
				if (evsel__strcmp(pos, evsel_name))
					continue;
				if (evsel__cpu_iter_skip(pos, cpu))
					continue;
				if (pos->disabled || !evsel__is_group_leader(pos) || !pos->core.fd)
					continue;
				if (pos->immediate)
					has_imm = true;
				if (pos->immediate != imm)
					continue;
				evsel__disable_cpu(pos, pos->cpu_iter - 1);
			}
		}
		if (!has_imm)
			break;
	}

	affinity__cleanup(&affinity);
	evlist__for_each_entry(evlist, pos) {
		if (evsel__strcmp(pos, evsel_name))
			continue;
		if (!evsel__is_group_leader(pos) || !pos->core.fd)
			continue;
		pos->disabled = true;
	}

	/*
	 * If we disabled only single event, we need to check
	 * the enabled state of the evlist manually.
	 */
	if (evsel_name)
		evlist->enabled = evlist__is_enabled(evlist);
	else
		evlist->enabled = false;
}

void evlist__disable(struct evlist *evlist)
{
	__evlist__disable(evlist, NULL);
}

void evlist__disable_evsel(struct evlist *evlist, char *evsel_name)
{
	__evlist__disable(evlist, evsel_name);
}

static void __evlist__enable(struct evlist *evlist, char *evsel_name)
{
	struct evsel *pos;
	struct affinity affinity;
	int cpu, i;

	if (affinity__setup(&affinity) < 0)
		return;

	evlist__for_each_cpu(evlist, i, cpu) {
		affinity__set(&affinity, cpu);

		evlist__for_each_entry(evlist, pos) {
			if (evsel__strcmp(pos, evsel_name))
				continue;
			if (evsel__cpu_iter_skip(pos, cpu))
				continue;
			if (!evsel__is_group_leader(pos) || !pos->core.fd)
				continue;
			evsel__enable_cpu(pos, pos->cpu_iter - 1);
		}
	}
	affinity__cleanup(&affinity);
	evlist__for_each_entry(evlist, pos) {
		if (evsel__strcmp(pos, evsel_name))
			continue;
		if (!evsel__is_group_leader(pos) || !pos->core.fd)
			continue;
		pos->disabled = false;
	}

	/*
	 * Even single event sets the 'enabled' for evlist,
	 * so the toggle can work properly and toggle to
	 * 'disabled' state.
	 */
	evlist->enabled = true;
}

void evlist__enable(struct evlist *evlist)
{
	__evlist__enable(evlist, NULL);
}

void evlist__enable_evsel(struct evlist *evlist, char *evsel_name)
{
	__evlist__enable(evlist, evsel_name);
}

void evlist__toggle_enable(struct evlist *evlist)
{
	(evlist->enabled ? evlist__disable : evlist__enable)(evlist);
}

static int evlist__enable_event_cpu(struct evlist *evlist, struct evsel *evsel, int cpu)
{
	int thread;
	int nr_threads = evlist__nr_threads(evlist, evsel);

	if (!evsel->core.fd)
>>>>>>> upstream/android-13
		return -EINVAL;

	for (thread = 0; thread < nr_threads; thread++) {
		int err = ioctl(FD(evsel, cpu, thread), PERF_EVENT_IOC_ENABLE, 0);
		if (err)
			return err;
	}
	return 0;
}

<<<<<<< HEAD
static int perf_evlist__enable_event_thread(struct perf_evlist *evlist,
					    struct perf_evsel *evsel,
					    int thread)
{
	int cpu;
	int nr_cpus = cpu_map__nr(evlist->cpus);

	if (!evsel->fd)
=======
static int evlist__enable_event_thread(struct evlist *evlist, struct evsel *evsel, int thread)
{
	int cpu;
	int nr_cpus = perf_cpu_map__nr(evlist->core.cpus);

	if (!evsel->core.fd)
>>>>>>> upstream/android-13
		return -EINVAL;

	for (cpu = 0; cpu < nr_cpus; cpu++) {
		int err = ioctl(FD(evsel, cpu, thread), PERF_EVENT_IOC_ENABLE, 0);
		if (err)
			return err;
	}
	return 0;
}

<<<<<<< HEAD
int perf_evlist__enable_event_idx(struct perf_evlist *evlist,
				  struct perf_evsel *evsel, int idx)
{
	bool per_cpu_mmaps = !cpu_map__empty(evlist->cpus);

	if (per_cpu_mmaps)
		return perf_evlist__enable_event_cpu(evlist, evsel, idx);
	else
		return perf_evlist__enable_event_thread(evlist, evsel, idx);
}

int perf_evlist__alloc_pollfd(struct perf_evlist *evlist)
{
	int nr_cpus = cpu_map__nr(evlist->cpus);
	int nr_threads = thread_map__nr(evlist->threads);
	int nfds = 0;
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->system_wide)
			nfds += nr_cpus;
		else
			nfds += nr_cpus * nr_threads;
	}

	if (fdarray__available_entries(&evlist->pollfd) < nfds &&
	    fdarray__grow(&evlist->pollfd, nfds) < 0)
		return -ENOMEM;

	return 0;
}

static int __perf_evlist__add_pollfd(struct perf_evlist *evlist, int fd,
				     struct perf_mmap *map, short revent)
{
	int pos = fdarray__add(&evlist->pollfd, fd, revent | POLLERR | POLLHUP);
	/*
	 * Save the idx so that when we filter out fds POLLHUP'ed we can
	 * close the associated evlist->mmap[] entry.
	 */
	if (pos >= 0) {
		evlist->pollfd.priv[pos].ptr = map;

		fcntl(fd, F_SETFL, O_NONBLOCK);
	}

	return pos;
}

int perf_evlist__add_pollfd(struct perf_evlist *evlist, int fd)
{
	return __perf_evlist__add_pollfd(evlist, fd, NULL, POLLIN);
}

static void perf_evlist__munmap_filtered(struct fdarray *fda, int fd,
					 void *arg __maybe_unused)
{
	struct perf_mmap *map = fda->priv[fd].ptr;

	if (map)
		perf_mmap__put(map);
}

int perf_evlist__filter_pollfd(struct perf_evlist *evlist, short revents_and_mask)
{
	return fdarray__filter(&evlist->pollfd, revents_and_mask,
			       perf_evlist__munmap_filtered, NULL);
}

int perf_evlist__poll(struct perf_evlist *evlist, int timeout)
{
	return fdarray__poll(&evlist->pollfd, timeout);
}

static void perf_evlist__id_hash(struct perf_evlist *evlist,
				 struct perf_evsel *evsel,
				 int cpu, int thread, u64 id)
{
	int hash;
	struct perf_sample_id *sid = SID(evsel, cpu, thread);

	sid->id = id;
	sid->evsel = evsel;
	hash = hash_64(sid->id, PERF_EVLIST__HLIST_BITS);
	hlist_add_head(&sid->node, &evlist->heads[hash]);
}

void perf_evlist__id_add(struct perf_evlist *evlist, struct perf_evsel *evsel,
			 int cpu, int thread, u64 id)
{
	perf_evlist__id_hash(evlist, evsel, cpu, thread, id);
	evsel->id[evsel->ids++] = id;
}

int perf_evlist__id_add_fd(struct perf_evlist *evlist,
			   struct perf_evsel *evsel,
			   int cpu, int thread, int fd)
{
	u64 read_data[4] = { 0, };
	int id_idx = 1; /* The first entry is the counter value */
	u64 id;
	int ret;

	ret = ioctl(fd, PERF_EVENT_IOC_ID, &id);
	if (!ret)
		goto add;

	if (errno != ENOTTY)
		return -1;

	/* Legacy way to get event id.. All hail to old kernels! */

	/*
	 * This way does not work with group format read, so bail
	 * out in that case.
	 */
	if (perf_evlist__read_format(evlist) & PERF_FORMAT_GROUP)
		return -1;

	if (!(evsel->attr.read_format & PERF_FORMAT_ID) ||
	    read(fd, &read_data, sizeof(read_data)) == -1)
		return -1;

	if (evsel->attr.read_format & PERF_FORMAT_TOTAL_TIME_ENABLED)
		++id_idx;
	if (evsel->attr.read_format & PERF_FORMAT_TOTAL_TIME_RUNNING)
		++id_idx;

	id = read_data[id_idx];

 add:
	perf_evlist__id_add(evlist, evsel, cpu, thread, id);
	return 0;
}

static void perf_evlist__set_sid_idx(struct perf_evlist *evlist,
				     struct perf_evsel *evsel, int idx, int cpu,
				     int thread)
{
	struct perf_sample_id *sid = SID(evsel, cpu, thread);
	sid->idx = idx;
	if (evlist->cpus && cpu >= 0)
		sid->cpu = evlist->cpus->map[cpu];
	else
		sid->cpu = -1;
	if (!evsel->system_wide && evlist->threads && thread >= 0)
		sid->tid = thread_map__pid(evlist->threads, thread);
	else
		sid->tid = -1;
}

struct perf_sample_id *perf_evlist__id2sid(struct perf_evlist *evlist, u64 id)
=======
int evlist__enable_event_idx(struct evlist *evlist, struct evsel *evsel, int idx)
{
	bool per_cpu_mmaps = !perf_cpu_map__empty(evlist->core.cpus);

	if (per_cpu_mmaps)
		return evlist__enable_event_cpu(evlist, evsel, idx);

	return evlist__enable_event_thread(evlist, evsel, idx);
}

int evlist__add_pollfd(struct evlist *evlist, int fd)
{
	return perf_evlist__add_pollfd(&evlist->core, fd, NULL, POLLIN, fdarray_flag__default);
}

int evlist__filter_pollfd(struct evlist *evlist, short revents_and_mask)
{
	return perf_evlist__filter_pollfd(&evlist->core, revents_and_mask);
}

#ifdef HAVE_EVENTFD_SUPPORT
int evlist__add_wakeup_eventfd(struct evlist *evlist, int fd)
{
	return perf_evlist__add_pollfd(&evlist->core, fd, NULL, POLLIN,
				       fdarray_flag__nonfilterable);
}
#endif

int evlist__poll(struct evlist *evlist, int timeout)
{
	return perf_evlist__poll(&evlist->core, timeout);
}

struct perf_sample_id *evlist__id2sid(struct evlist *evlist, u64 id)
>>>>>>> upstream/android-13
{
	struct hlist_head *head;
	struct perf_sample_id *sid;
	int hash;

	hash = hash_64(id, PERF_EVLIST__HLIST_BITS);
<<<<<<< HEAD
	head = &evlist->heads[hash];
=======
	head = &evlist->core.heads[hash];
>>>>>>> upstream/android-13

	hlist_for_each_entry(sid, head, node)
		if (sid->id == id)
			return sid;

	return NULL;
}

<<<<<<< HEAD
struct perf_evsel *perf_evlist__id2evsel(struct perf_evlist *evlist, u64 id)
{
	struct perf_sample_id *sid;

	if (evlist->nr_entries == 1 || !id)
		return perf_evlist__first(evlist);

	sid = perf_evlist__id2sid(evlist, id);
	if (sid)
		return sid->evsel;

	if (!perf_evlist__sample_id_all(evlist))
		return perf_evlist__first(evlist);
=======
struct evsel *evlist__id2evsel(struct evlist *evlist, u64 id)
{
	struct perf_sample_id *sid;

	if (evlist->core.nr_entries == 1 || !id)
		return evlist__first(evlist);

	sid = evlist__id2sid(evlist, id);
	if (sid)
		return container_of(sid->evsel, struct evsel, core);

	if (!evlist__sample_id_all(evlist))
		return evlist__first(evlist);
>>>>>>> upstream/android-13

	return NULL;
}

<<<<<<< HEAD
struct perf_evsel *perf_evlist__id2evsel_strict(struct perf_evlist *evlist,
						u64 id)
=======
struct evsel *evlist__id2evsel_strict(struct evlist *evlist, u64 id)
>>>>>>> upstream/android-13
{
	struct perf_sample_id *sid;

	if (!id)
		return NULL;

<<<<<<< HEAD
	sid = perf_evlist__id2sid(evlist, id);
	if (sid)
		return sid->evsel;
=======
	sid = evlist__id2sid(evlist, id);
	if (sid)
		return container_of(sid->evsel, struct evsel, core);
>>>>>>> upstream/android-13

	return NULL;
}

<<<<<<< HEAD
static int perf_evlist__event2id(struct perf_evlist *evlist,
				 union perf_event *event, u64 *id)
{
	const u64 *array = event->sample.array;
=======
static int evlist__event2id(struct evlist *evlist, union perf_event *event, u64 *id)
{
	const __u64 *array = event->sample.array;
>>>>>>> upstream/android-13
	ssize_t n;

	n = (event->header.size - sizeof(event->header)) >> 3;

	if (event->header.type == PERF_RECORD_SAMPLE) {
		if (evlist->id_pos >= n)
			return -1;
		*id = array[evlist->id_pos];
	} else {
		if (evlist->is_pos > n)
			return -1;
		n -= evlist->is_pos;
		*id = array[n];
	}
	return 0;
}

<<<<<<< HEAD
struct perf_evsel *perf_evlist__event2evsel(struct perf_evlist *evlist,
					    union perf_event *event)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
=======
struct evsel *evlist__event2evsel(struct evlist *evlist, union perf_event *event)
{
	struct evsel *first = evlist__first(evlist);
>>>>>>> upstream/android-13
	struct hlist_head *head;
	struct perf_sample_id *sid;
	int hash;
	u64 id;

<<<<<<< HEAD
	if (evlist->nr_entries == 1)
		return first;

	if (!first->attr.sample_id_all &&
	    event->header.type != PERF_RECORD_SAMPLE)
		return first;

	if (perf_evlist__event2id(evlist, event, &id))
=======
	if (evlist->core.nr_entries == 1)
		return first;

	if (!first->core.attr.sample_id_all &&
	    event->header.type != PERF_RECORD_SAMPLE)
		return first;

	if (evlist__event2id(evlist, event, &id))
>>>>>>> upstream/android-13
		return NULL;

	/* Synthesized events have an id of zero */
	if (!id)
		return first;

	hash = hash_64(id, PERF_EVLIST__HLIST_BITS);
<<<<<<< HEAD
	head = &evlist->heads[hash];

	hlist_for_each_entry(sid, head, node) {
		if (sid->id == id)
			return sid->evsel;
=======
	head = &evlist->core.heads[hash];

	hlist_for_each_entry(sid, head, node) {
		if (sid->id == id)
			return container_of(sid->evsel, struct evsel, core);
>>>>>>> upstream/android-13
	}
	return NULL;
}

<<<<<<< HEAD
static int perf_evlist__set_paused(struct perf_evlist *evlist, bool value)
=======
static int evlist__set_paused(struct evlist *evlist, bool value)
>>>>>>> upstream/android-13
{
	int i;

	if (!evlist->overwrite_mmap)
		return 0;

<<<<<<< HEAD
	for (i = 0; i < evlist->nr_mmaps; i++) {
		int fd = evlist->overwrite_mmap[i].fd;
=======
	for (i = 0; i < evlist->core.nr_mmaps; i++) {
		int fd = evlist->overwrite_mmap[i].core.fd;
>>>>>>> upstream/android-13
		int err;

		if (fd < 0)
			continue;
		err = ioctl(fd, PERF_EVENT_IOC_PAUSE_OUTPUT, value ? 1 : 0);
		if (err)
			return err;
	}
	return 0;
}

<<<<<<< HEAD
static int perf_evlist__pause(struct perf_evlist *evlist)
{
	return perf_evlist__set_paused(evlist, true);
}

static int perf_evlist__resume(struct perf_evlist *evlist)
{
	return perf_evlist__set_paused(evlist, false);
}

static void perf_evlist__munmap_nofree(struct perf_evlist *evlist)
=======
static int evlist__pause(struct evlist *evlist)
{
	return evlist__set_paused(evlist, true);
}

static int evlist__resume(struct evlist *evlist)
{
	return evlist__set_paused(evlist, false);
}

static void evlist__munmap_nofree(struct evlist *evlist)
>>>>>>> upstream/android-13
{
	int i;

	if (evlist->mmap)
<<<<<<< HEAD
		for (i = 0; i < evlist->nr_mmaps; i++)
			perf_mmap__munmap(&evlist->mmap[i]);

	if (evlist->overwrite_mmap)
		for (i = 0; i < evlist->nr_mmaps; i++)
			perf_mmap__munmap(&evlist->overwrite_mmap[i]);
}

void perf_evlist__munmap(struct perf_evlist *evlist)
{
	perf_evlist__munmap_nofree(evlist);
=======
		for (i = 0; i < evlist->core.nr_mmaps; i++)
			perf_mmap__munmap(&evlist->mmap[i].core);

	if (evlist->overwrite_mmap)
		for (i = 0; i < evlist->core.nr_mmaps; i++)
			perf_mmap__munmap(&evlist->overwrite_mmap[i].core);
}

void evlist__munmap(struct evlist *evlist)
{
	evlist__munmap_nofree(evlist);
>>>>>>> upstream/android-13
	zfree(&evlist->mmap);
	zfree(&evlist->overwrite_mmap);
}

<<<<<<< HEAD
static struct perf_mmap *perf_evlist__alloc_mmap(struct perf_evlist *evlist,
						 bool overwrite)
{
	int i;
	struct perf_mmap *map;

	evlist->nr_mmaps = cpu_map__nr(evlist->cpus);
	if (cpu_map__empty(evlist->cpus))
		evlist->nr_mmaps = thread_map__nr(evlist->threads);
	map = zalloc(evlist->nr_mmaps * sizeof(struct perf_mmap));
	if (!map)
		return NULL;

	for (i = 0; i < evlist->nr_mmaps; i++) {
		map[i].fd = -1;
		map[i].overwrite = overwrite;
=======
static void perf_mmap__unmap_cb(struct perf_mmap *map)
{
	struct mmap *m = container_of(map, struct mmap, core);

	mmap__munmap(m);
}

static struct mmap *evlist__alloc_mmap(struct evlist *evlist,
				       bool overwrite)
{
	int i;
	struct mmap *map;

	map = zalloc(evlist->core.nr_mmaps * sizeof(struct mmap));
	if (!map)
		return NULL;

	for (i = 0; i < evlist->core.nr_mmaps; i++) {
		struct perf_mmap *prev = i ? &map[i - 1].core : NULL;

>>>>>>> upstream/android-13
		/*
		 * When the perf_mmap() call is made we grab one refcount, plus
		 * one extra to let perf_mmap__consume() get the last
		 * events after all real references (perf_mmap__get()) are
		 * dropped.
		 *
		 * Each PERF_EVENT_IOC_SET_OUTPUT points to this mmap and
		 * thus does perf_mmap__get() on it.
		 */
<<<<<<< HEAD
		refcount_set(&map[i].refcnt, 0);
	}
	return map;
}

static bool
perf_evlist__should_poll(struct perf_evlist *evlist __maybe_unused,
			 struct perf_evsel *evsel)
{
	if (evsel->attr.write_backward)
		return false;
	return true;
}

static int perf_evlist__mmap_per_evsel(struct perf_evlist *evlist, int idx,
				       struct mmap_params *mp, int cpu_idx,
				       int thread, int *_output, int *_output_overwrite)
{
	struct perf_evsel *evsel;
	int revent;
	int evlist_cpu = cpu_map__cpu(evlist->cpus, cpu_idx);

	evlist__for_each_entry(evlist, evsel) {
		struct perf_mmap *maps = evlist->mmap;
		int *output = _output;
		int fd;
		int cpu;

		mp->prot = PROT_READ | PROT_WRITE;
		if (evsel->attr.write_backward) {
			output = _output_overwrite;
			maps = evlist->overwrite_mmap;

			if (!maps) {
				maps = perf_evlist__alloc_mmap(evlist, true);
				if (!maps)
					return -1;
				evlist->overwrite_mmap = maps;
				if (evlist->bkw_mmap_state == BKW_MMAP_NOTREADY)
					perf_evlist__toggle_bkw_mmap(evlist, BKW_MMAP_RUNNING);
			}
			mp->prot &= ~PROT_WRITE;
		}

		if (evsel->system_wide && thread)
			continue;

		cpu = cpu_map__idx(evsel->cpus, evlist_cpu);
		if (cpu == -1)
			continue;

		fd = FD(evsel, cpu, thread);

		if (*output == -1) {
			*output = fd;

			if (perf_mmap__mmap(&maps[idx], mp, *output, evlist_cpu) < 0)
				return -1;
		} else {
			if (ioctl(fd, PERF_EVENT_IOC_SET_OUTPUT, *output) != 0)
				return -1;

			perf_mmap__get(&maps[idx]);
		}

		revent = perf_evlist__should_poll(evlist, evsel) ? POLLIN : 0;

		/*
		 * The system_wide flag causes a selected event to be opened
		 * always without a pid.  Consequently it will never get a
		 * POLLHUP, but it is used for tracking in combination with
		 * other events, so it should not need to be polled anyway.
		 * Therefore don't add it for polling.
		 */
		if (!evsel->system_wide &&
		    __perf_evlist__add_pollfd(evlist, fd, &maps[idx], revent) < 0) {
			perf_mmap__put(&maps[idx]);
			return -1;
		}

		if (evsel->attr.read_format & PERF_FORMAT_ID) {
			if (perf_evlist__id_add_fd(evlist, evsel, cpu, thread,
						   fd) < 0)
				return -1;
			perf_evlist__set_sid_idx(evlist, evsel, idx, cpu,
						 thread);
		}
	}

	return 0;
}

static int perf_evlist__mmap_per_cpu(struct perf_evlist *evlist,
				     struct mmap_params *mp)
{
	int cpu, thread;
	int nr_cpus = cpu_map__nr(evlist->cpus);
	int nr_threads = thread_map__nr(evlist->threads);

	pr_debug2("perf event ring buffer mmapped per cpu\n");
	for (cpu = 0; cpu < nr_cpus; cpu++) {
		int output = -1;
		int output_overwrite = -1;

		auxtrace_mmap_params__set_idx(&mp->auxtrace_mp, evlist, cpu,
					      true);

		for (thread = 0; thread < nr_threads; thread++) {
			if (perf_evlist__mmap_per_evsel(evlist, cpu, mp, cpu,
							thread, &output, &output_overwrite))
				goto out_unmap;
		}
	}

	return 0;

out_unmap:
	perf_evlist__munmap_nofree(evlist);
	return -1;
}

static int perf_evlist__mmap_per_thread(struct perf_evlist *evlist,
					struct mmap_params *mp)
{
	int thread;
	int nr_threads = thread_map__nr(evlist->threads);

	pr_debug2("perf event ring buffer mmapped per thread\n");
	for (thread = 0; thread < nr_threads; thread++) {
		int output = -1;
		int output_overwrite = -1;

		auxtrace_mmap_params__set_idx(&mp->auxtrace_mp, evlist, thread,
					      false);

		if (perf_evlist__mmap_per_evsel(evlist, thread, mp, 0, thread,
						&output, &output_overwrite))
			goto out_unmap;
	}

	return 0;

out_unmap:
	perf_evlist__munmap_nofree(evlist);
	return -1;
=======
		perf_mmap__init(&map[i].core, prev, overwrite, perf_mmap__unmap_cb);
	}

	return map;
}

static void
perf_evlist__mmap_cb_idx(struct perf_evlist *_evlist,
			 struct perf_mmap_param *_mp,
			 int idx, bool per_cpu)
{
	struct evlist *evlist = container_of(_evlist, struct evlist, core);
	struct mmap_params *mp = container_of(_mp, struct mmap_params, core);

	auxtrace_mmap_params__set_idx(&mp->auxtrace_mp, evlist, idx, per_cpu);
}

static struct perf_mmap*
perf_evlist__mmap_cb_get(struct perf_evlist *_evlist, bool overwrite, int idx)
{
	struct evlist *evlist = container_of(_evlist, struct evlist, core);
	struct mmap *maps;

	maps = overwrite ? evlist->overwrite_mmap : evlist->mmap;

	if (!maps) {
		maps = evlist__alloc_mmap(evlist, overwrite);
		if (!maps)
			return NULL;

		if (overwrite) {
			evlist->overwrite_mmap = maps;
			if (evlist->bkw_mmap_state == BKW_MMAP_NOTREADY)
				evlist__toggle_bkw_mmap(evlist, BKW_MMAP_RUNNING);
		} else {
			evlist->mmap = maps;
		}
	}

	return &maps[idx].core;
}

static int
perf_evlist__mmap_cb_mmap(struct perf_mmap *_map, struct perf_mmap_param *_mp,
			  int output, int cpu)
{
	struct mmap *map = container_of(_map, struct mmap, core);
	struct mmap_params *mp = container_of(_mp, struct mmap_params, core);

	return mmap__mmap(map, mp, output, cpu);
>>>>>>> upstream/android-13
}

unsigned long perf_event_mlock_kb_in_pages(void)
{
	unsigned long pages;
	int max;

	if (sysctl__read_int("kernel/perf_event_mlock_kb", &max) < 0) {
		/*
		 * Pick a once upon a time good value, i.e. things look
		 * strange since we can't read a sysctl value, but lets not
		 * die yet...
		 */
		max = 512;
	} else {
		max -= (page_size / 1024);
	}

	pages = (max * 1024) / page_size;
	if (!is_power_of_2(pages))
		pages = rounddown_pow_of_two(pages);

	return pages;
}

<<<<<<< HEAD
size_t perf_evlist__mmap_size(unsigned long pages)
=======
size_t evlist__mmap_size(unsigned long pages)
>>>>>>> upstream/android-13
{
	if (pages == UINT_MAX)
		pages = perf_event_mlock_kb_in_pages();
	else if (!is_power_of_2(pages))
		return 0;

	return (pages + 1) * page_size;
}

static long parse_pages_arg(const char *str, unsigned long min,
			    unsigned long max)
{
	unsigned long pages, val;
	static struct parse_tag tags[] = {
		{ .tag  = 'B', .mult = 1       },
		{ .tag  = 'K', .mult = 1 << 10 },
		{ .tag  = 'M', .mult = 1 << 20 },
		{ .tag  = 'G', .mult = 1 << 30 },
		{ .tag  = 0 },
	};

	if (str == NULL)
		return -EINVAL;

	val = parse_tag_value(str, tags);
	if (val != (unsigned long) -1) {
		/* we got file size value */
		pages = PERF_ALIGN(val, page_size) / page_size;
	} else {
		/* we got pages count value */
		char *eptr;
		pages = strtoul(str, &eptr, 10);
		if (*eptr != '\0')
			return -EINVAL;
	}

	if (pages == 0 && min == 0) {
		/* leave number of pages at 0 */
	} else if (!is_power_of_2(pages)) {
		char buf[100];

		/* round pages up to next power of 2 */
		pages = roundup_pow_of_two(pages);
		if (!pages)
			return -EINVAL;

		unit_number__scnprintf(buf, sizeof(buf), pages * page_size);
		pr_info("rounding mmap pages size to %s (%lu pages)\n",
			buf, pages);
	}

	if (pages > max)
		return -EINVAL;

	return pages;
}

<<<<<<< HEAD
int __perf_evlist__parse_mmap_pages(unsigned int *mmap_pages, const char *str)
=======
int __evlist__parse_mmap_pages(unsigned int *mmap_pages, const char *str)
>>>>>>> upstream/android-13
{
	unsigned long max = UINT_MAX;
	long pages;

	if (max > SIZE_MAX / page_size)
		max = SIZE_MAX / page_size;

	pages = parse_pages_arg(str, 1, max);
	if (pages < 0) {
		pr_err("Invalid argument for --mmap_pages/-m\n");
		return -1;
	}

	*mmap_pages = pages;
	return 0;
}

<<<<<<< HEAD
int perf_evlist__parse_mmap_pages(const struct option *opt, const char *str,
				  int unset __maybe_unused)
{
	return __perf_evlist__parse_mmap_pages(opt->value, str);
}

/**
 * perf_evlist__mmap_ex - Create mmaps to receive events.
=======
int evlist__parse_mmap_pages(const struct option *opt, const char *str, int unset __maybe_unused)
{
	return __evlist__parse_mmap_pages(opt->value, str);
}

/**
 * evlist__mmap_ex - Create mmaps to receive events.
>>>>>>> upstream/android-13
 * @evlist: list of events
 * @pages: map length in pages
 * @overwrite: overwrite older events?
 * @auxtrace_pages - auxtrace map length in pages
 * @auxtrace_overwrite - overwrite older auxtrace data?
 *
 * If @overwrite is %false the user needs to signal event consumption using
<<<<<<< HEAD
 * perf_mmap__write_tail().  Using perf_evlist__mmap_read() does this
=======
 * perf_mmap__write_tail().  Using evlist__mmap_read() does this
>>>>>>> upstream/android-13
 * automatically.
 *
 * Similarly, if @auxtrace_overwrite is %false the user needs to signal data
 * consumption using auxtrace_mmap__write_tail().
 *
 * Return: %0 on success, negative error code otherwise.
 */
<<<<<<< HEAD
int perf_evlist__mmap_ex(struct perf_evlist *evlist, unsigned int pages,
			 unsigned int auxtrace_pages,
			 bool auxtrace_overwrite)
{
	struct perf_evsel *evsel;
	const struct cpu_map *cpus = evlist->cpus;
	const struct thread_map *threads = evlist->threads;
=======
int evlist__mmap_ex(struct evlist *evlist, unsigned int pages,
			 unsigned int auxtrace_pages,
			 bool auxtrace_overwrite, int nr_cblocks, int affinity, int flush,
			 int comp_level)
{
>>>>>>> upstream/android-13
	/*
	 * Delay setting mp.prot: set it before calling perf_mmap__mmap.
	 * Its value is decided by evsel's write_backward.
	 * So &mp should not be passed through const pointer.
	 */
<<<<<<< HEAD
	struct mmap_params mp;

	if (!evlist->mmap)
		evlist->mmap = perf_evlist__alloc_mmap(evlist, false);
	if (!evlist->mmap)
		return -ENOMEM;

	if (evlist->pollfd.entries == NULL && perf_evlist__alloc_pollfd(evlist) < 0)
		return -ENOMEM;

	evlist->mmap_len = perf_evlist__mmap_size(pages);
	pr_debug("mmap size %zuB\n", evlist->mmap_len);
	mp.mask = evlist->mmap_len - page_size - 1;

	auxtrace_mmap_params__init(&mp.auxtrace_mp, evlist->mmap_len,
				   auxtrace_pages, auxtrace_overwrite);

	evlist__for_each_entry(evlist, evsel) {
		if ((evsel->attr.read_format & PERF_FORMAT_ID) &&
		    evsel->sample_id == NULL &&
		    perf_evsel__alloc_id(evsel, cpu_map__nr(cpus), threads->nr) < 0)
			return -ENOMEM;
	}

	if (cpu_map__empty(cpus))
		return perf_evlist__mmap_per_thread(evlist, &mp);

	return perf_evlist__mmap_per_cpu(evlist, &mp);
}

int perf_evlist__mmap(struct perf_evlist *evlist, unsigned int pages)
{
	return perf_evlist__mmap_ex(evlist, pages, 0, false);
}

int perf_evlist__create_maps(struct perf_evlist *evlist, struct target *target)
{
	bool all_threads = (target->per_thread && target->system_wide);
	struct cpu_map *cpus;
	struct thread_map *threads;
=======
	struct mmap_params mp = {
		.nr_cblocks	= nr_cblocks,
		.affinity	= affinity,
		.flush		= flush,
		.comp_level	= comp_level
	};
	struct perf_evlist_mmap_ops ops = {
		.idx  = perf_evlist__mmap_cb_idx,
		.get  = perf_evlist__mmap_cb_get,
		.mmap = perf_evlist__mmap_cb_mmap,
	};

	evlist->core.mmap_len = evlist__mmap_size(pages);
	pr_debug("mmap size %zuB\n", evlist->core.mmap_len);

	auxtrace_mmap_params__init(&mp.auxtrace_mp, evlist->core.mmap_len,
				   auxtrace_pages, auxtrace_overwrite);

	return perf_evlist__mmap_ops(&evlist->core, &ops, &mp.core);
}

int evlist__mmap(struct evlist *evlist, unsigned int pages)
{
	return evlist__mmap_ex(evlist, pages, 0, false, 0, PERF_AFFINITY_SYS, 1, 0);
}

int evlist__create_maps(struct evlist *evlist, struct target *target)
{
	bool all_threads = (target->per_thread && target->system_wide);
	struct perf_cpu_map *cpus;
	struct perf_thread_map *threads;
>>>>>>> upstream/android-13

	/*
	 * If specify '-a' and '--per-thread' to perf record, perf record
	 * will override '--per-thread'. target->per_thread = false and
	 * target->system_wide = true.
	 *
	 * If specify '--per-thread' only to perf record,
	 * target->per_thread = true and target->system_wide = false.
	 *
	 * So target->per_thread && target->system_wide is false.
	 * For perf record, thread_map__new_str doesn't call
	 * thread_map__new_all_cpus. That will keep perf record's
	 * current behavior.
	 *
	 * For perf stat, it allows the case that target->per_thread and
	 * target->system_wide are all true. It means to collect system-wide
	 * per-thread data. thread_map__new_str will call
	 * thread_map__new_all_cpus to enumerate all threads.
	 */
	threads = thread_map__new_str(target->pid, target->tid, target->uid,
				      all_threads);

	if (!threads)
		return -1;

	if (target__uses_dummy_map(target))
<<<<<<< HEAD
		cpus = cpu_map__dummy_new();
	else
		cpus = cpu_map__new(target->cpu_list);
=======
		cpus = perf_cpu_map__dummy_new();
	else
		cpus = perf_cpu_map__new(target->cpu_list);
>>>>>>> upstream/android-13

	if (!cpus)
		goto out_delete_threads;

<<<<<<< HEAD
	evlist->has_user_cpus = !!target->cpu_list;

	perf_evlist__set_maps(evlist, cpus, threads);
=======
	evlist->core.has_user_cpus = !!target->cpu_list && !target->hybrid;

	perf_evlist__set_maps(&evlist->core, cpus, threads);

	/* as evlist now has references, put count here */
	perf_cpu_map__put(cpus);
	perf_thread_map__put(threads);
>>>>>>> upstream/android-13

	return 0;

out_delete_threads:
<<<<<<< HEAD
	thread_map__put(threads);
	return -1;
}

void perf_evlist__set_maps(struct perf_evlist *evlist, struct cpu_map *cpus,
			   struct thread_map *threads)
{
	/*
	 * Allow for the possibility that one or another of the maps isn't being
	 * changed i.e. don't put it.  Note we are assuming the maps that are
	 * being applied are brand new and evlist is taking ownership of the
	 * original reference count of 1.  If that is not the case it is up to
	 * the caller to increase the reference count.
	 */
	if (cpus != evlist->cpus) {
		cpu_map__put(evlist->cpus);
		evlist->cpus = cpu_map__get(cpus);
	}

	if (threads != evlist->threads) {
		thread_map__put(evlist->threads);
		evlist->threads = thread_map__get(threads);
	}

	perf_evlist__propagate_maps(evlist);
}

void __perf_evlist__set_sample_bit(struct perf_evlist *evlist,
				   enum perf_event_sample_format bit)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel)
		__perf_evsel__set_sample_bit(evsel, bit);
}

void __perf_evlist__reset_sample_bit(struct perf_evlist *evlist,
				     enum perf_event_sample_format bit)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel)
		__perf_evsel__reset_sample_bit(evsel, bit);
}

int perf_evlist__apply_filters(struct perf_evlist *evlist, struct perf_evsel **err_evsel)
{
	struct perf_evsel *evsel;
=======
	perf_thread_map__put(threads);
	return -1;
}

int evlist__apply_filters(struct evlist *evlist, struct evsel **err_evsel)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13
	int err = 0;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->filter == NULL)
			continue;

		/*
		 * filters only work for tracepoint event, which doesn't have cpu limit.
		 * So evlist and evsel should always be same.
		 */
<<<<<<< HEAD
		err = perf_evsel__apply_filter(evsel, evsel->filter);
=======
		err = perf_evsel__apply_filter(&evsel->core, evsel->filter);
>>>>>>> upstream/android-13
		if (err) {
			*err_evsel = evsel;
			break;
		}
	}

	return err;
}

<<<<<<< HEAD
int perf_evlist__set_filter(struct perf_evlist *evlist, const char *filter)
{
	struct perf_evsel *evsel;
	int err = 0;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->attr.type != PERF_TYPE_TRACEPOINT)
			continue;

		err = perf_evsel__set_filter(evsel, filter);
=======
int evlist__set_tp_filter(struct evlist *evlist, const char *filter)
{
	struct evsel *evsel;
	int err = 0;

	if (filter == NULL)
		return -1;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->core.attr.type != PERF_TYPE_TRACEPOINT)
			continue;

		err = evsel__set_filter(evsel, filter);
>>>>>>> upstream/android-13
		if (err)
			break;
	}

	return err;
}

<<<<<<< HEAD
int perf_evlist__set_filter_pids(struct perf_evlist *evlist, size_t npids, pid_t *pids)
{
	char *filter;
	int ret = -1;
=======
int evlist__append_tp_filter(struct evlist *evlist, const char *filter)
{
	struct evsel *evsel;
	int err = 0;

	if (filter == NULL)
		return -1;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->core.attr.type != PERF_TYPE_TRACEPOINT)
			continue;

		err = evsel__append_tp_filter(evsel, filter);
		if (err)
			break;
	}

	return err;
}

char *asprintf__tp_filter_pids(size_t npids, pid_t *pids)
{
	char *filter;
>>>>>>> upstream/android-13
	size_t i;

	for (i = 0; i < npids; ++i) {
		if (i == 0) {
			if (asprintf(&filter, "common_pid != %d", pids[i]) < 0)
<<<<<<< HEAD
				return -1;
=======
				return NULL;
>>>>>>> upstream/android-13
		} else {
			char *tmp;

			if (asprintf(&tmp, "%s && common_pid != %d", filter, pids[i]) < 0)
				goto out_free;

			free(filter);
			filter = tmp;
		}
	}

<<<<<<< HEAD
	ret = perf_evlist__set_filter(evlist, filter);
out_free:
	free(filter);
	return ret;
}

int perf_evlist__set_filter_pid(struct perf_evlist *evlist, pid_t pid)
{
	return perf_evlist__set_filter_pids(evlist, 1, &pid);
}

bool perf_evlist__valid_sample_type(struct perf_evlist *evlist)
{
	struct perf_evsel *pos;

	if (evlist->nr_entries == 1)
=======
	return filter;
out_free:
	free(filter);
	return NULL;
}

int evlist__set_tp_filter_pids(struct evlist *evlist, size_t npids, pid_t *pids)
{
	char *filter = asprintf__tp_filter_pids(npids, pids);
	int ret = evlist__set_tp_filter(evlist, filter);

	free(filter);
	return ret;
}

int evlist__set_tp_filter_pid(struct evlist *evlist, pid_t pid)
{
	return evlist__set_tp_filter_pids(evlist, 1, &pid);
}

int evlist__append_tp_filter_pids(struct evlist *evlist, size_t npids, pid_t *pids)
{
	char *filter = asprintf__tp_filter_pids(npids, pids);
	int ret = evlist__append_tp_filter(evlist, filter);

	free(filter);
	return ret;
}

int evlist__append_tp_filter_pid(struct evlist *evlist, pid_t pid)
{
	return evlist__append_tp_filter_pids(evlist, 1, &pid);
}

bool evlist__valid_sample_type(struct evlist *evlist)
{
	struct evsel *pos;

	if (evlist->core.nr_entries == 1)
>>>>>>> upstream/android-13
		return true;

	if (evlist->id_pos < 0 || evlist->is_pos < 0)
		return false;

	evlist__for_each_entry(evlist, pos) {
		if (pos->id_pos != evlist->id_pos ||
		    pos->is_pos != evlist->is_pos)
			return false;
	}

	return true;
}

<<<<<<< HEAD
u64 __perf_evlist__combined_sample_type(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
=======
u64 __evlist__combined_sample_type(struct evlist *evlist)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13

	if (evlist->combined_sample_type)
		return evlist->combined_sample_type;

	evlist__for_each_entry(evlist, evsel)
<<<<<<< HEAD
		evlist->combined_sample_type |= evsel->attr.sample_type;
=======
		evlist->combined_sample_type |= evsel->core.attr.sample_type;
>>>>>>> upstream/android-13

	return evlist->combined_sample_type;
}

<<<<<<< HEAD
u64 perf_evlist__combined_sample_type(struct perf_evlist *evlist)
{
	evlist->combined_sample_type = 0;
	return __perf_evlist__combined_sample_type(evlist);
}

u64 perf_evlist__combined_branch_type(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
	u64 branch_type = 0;

	evlist__for_each_entry(evlist, evsel)
		branch_type |= evsel->attr.branch_sample_type;
	return branch_type;
}

bool perf_evlist__valid_read_format(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist), *pos = first;
	u64 read_format = first->attr.read_format;
	u64 sample_type = first->attr.sample_type;

	evlist__for_each_entry(evlist, pos) {
		if (read_format != pos->attr.read_format)
			return false;
	}

	/* PERF_SAMPLE_READ imples PERF_FORMAT_ID. */
=======
u64 evlist__combined_sample_type(struct evlist *evlist)
{
	evlist->combined_sample_type = 0;
	return __evlist__combined_sample_type(evlist);
}

u64 evlist__combined_branch_type(struct evlist *evlist)
{
	struct evsel *evsel;
	u64 branch_type = 0;

	evlist__for_each_entry(evlist, evsel)
		branch_type |= evsel->core.attr.branch_sample_type;
	return branch_type;
}

bool evlist__valid_read_format(struct evlist *evlist)
{
	struct evsel *first = evlist__first(evlist), *pos = first;
	u64 read_format = first->core.attr.read_format;
	u64 sample_type = first->core.attr.sample_type;

	evlist__for_each_entry(evlist, pos) {
		if (read_format != pos->core.attr.read_format) {
			pr_debug("Read format differs %#" PRIx64 " vs %#" PRIx64 "\n",
				 read_format, (u64)pos->core.attr.read_format);
		}
	}

	/* PERF_SAMPLE_READ implies PERF_FORMAT_ID. */
>>>>>>> upstream/android-13
	if ((sample_type & PERF_SAMPLE_READ) &&
	    !(read_format & PERF_FORMAT_ID)) {
		return false;
	}

	return true;
}

<<<<<<< HEAD
u64 perf_evlist__read_format(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
	return first->attr.read_format;
}

u16 perf_evlist__id_hdr_size(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
=======
u16 evlist__id_hdr_size(struct evlist *evlist)
{
	struct evsel *first = evlist__first(evlist);
>>>>>>> upstream/android-13
	struct perf_sample *data;
	u64 sample_type;
	u16 size = 0;

<<<<<<< HEAD
	if (!first->attr.sample_id_all)
		goto out;

	sample_type = first->attr.sample_type;
=======
	if (!first->core.attr.sample_id_all)
		goto out;

	sample_type = first->core.attr.sample_type;
>>>>>>> upstream/android-13

	if (sample_type & PERF_SAMPLE_TID)
		size += sizeof(data->tid) * 2;

       if (sample_type & PERF_SAMPLE_TIME)
		size += sizeof(data->time);

	if (sample_type & PERF_SAMPLE_ID)
		size += sizeof(data->id);

	if (sample_type & PERF_SAMPLE_STREAM_ID)
		size += sizeof(data->stream_id);

	if (sample_type & PERF_SAMPLE_CPU)
		size += sizeof(data->cpu) * 2;

	if (sample_type & PERF_SAMPLE_IDENTIFIER)
		size += sizeof(data->id);
out:
	return size;
}

<<<<<<< HEAD
bool perf_evlist__valid_sample_id_all(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist), *pos = first;

	evlist__for_each_entry_continue(evlist, pos) {
		if (first->attr.sample_id_all != pos->attr.sample_id_all)
=======
bool evlist__valid_sample_id_all(struct evlist *evlist)
{
	struct evsel *first = evlist__first(evlist), *pos = first;

	evlist__for_each_entry_continue(evlist, pos) {
		if (first->core.attr.sample_id_all != pos->core.attr.sample_id_all)
>>>>>>> upstream/android-13
			return false;
	}

	return true;
}

<<<<<<< HEAD
bool perf_evlist__sample_id_all(struct perf_evlist *evlist)
{
	struct perf_evsel *first = perf_evlist__first(evlist);
	return first->attr.sample_id_all;
}

void perf_evlist__set_selected(struct perf_evlist *evlist,
			       struct perf_evsel *evsel)
=======
bool evlist__sample_id_all(struct evlist *evlist)
{
	struct evsel *first = evlist__first(evlist);
	return first->core.attr.sample_id_all;
}

void evlist__set_selected(struct evlist *evlist, struct evsel *evsel)
>>>>>>> upstream/android-13
{
	evlist->selected = evsel;
}

<<<<<<< HEAD
void perf_evlist__close(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry_reverse(evlist, evsel)
		perf_evsel__close(evsel);
}

static int perf_evlist__create_syswide_maps(struct perf_evlist *evlist)
{
	struct cpu_map	  *cpus;
	struct thread_map *threads;
=======
void evlist__close(struct evlist *evlist)
{
	struct evsel *evsel;
	struct affinity affinity;
	int cpu, i;

	/*
	 * With perf record core.cpus is usually NULL.
	 * Use the old method to handle this for now.
	 */
	if (!evlist->core.cpus) {
		evlist__for_each_entry_reverse(evlist, evsel)
			evsel__close(evsel);
		return;
	}

	if (affinity__setup(&affinity) < 0)
		return;
	evlist__for_each_cpu(evlist, i, cpu) {
		affinity__set(&affinity, cpu);

		evlist__for_each_entry_reverse(evlist, evsel) {
			if (evsel__cpu_iter_skip(evsel, cpu))
			    continue;
			perf_evsel__close_cpu(&evsel->core, evsel->cpu_iter - 1);
		}
	}
	affinity__cleanup(&affinity);
	evlist__for_each_entry_reverse(evlist, evsel) {
		perf_evsel__free_fd(&evsel->core);
		perf_evsel__free_id(&evsel->core);
	}
	perf_evlist__reset_id_hash(&evlist->core);
}

static int evlist__create_syswide_maps(struct evlist *evlist)
{
	struct perf_cpu_map *cpus;
	struct perf_thread_map *threads;
>>>>>>> upstream/android-13
	int err = -ENOMEM;

	/*
	 * Try reading /sys/devices/system/cpu/online to get
	 * an all cpus map.
	 *
	 * FIXME: -ENOMEM is the best we can do here, the cpu_map
	 * code needs an overhaul to properly forward the
	 * error, and we may not want to do that fallback to a
	 * default cpu identity map :-\
	 */
<<<<<<< HEAD
	cpus = cpu_map__new(NULL);
	if (!cpus)
		goto out;

	threads = thread_map__new_dummy();
	if (!threads)
		goto out_put;

	perf_evlist__set_maps(evlist, cpus, threads);
out:
	return err;
out_put:
	cpu_map__put(cpus);
	goto out;
}

int perf_evlist__open(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
=======
	cpus = perf_cpu_map__new(NULL);
	if (!cpus)
		goto out;

	threads = perf_thread_map__new_dummy();
	if (!threads)
		goto out_put;

	perf_evlist__set_maps(&evlist->core, cpus, threads);

	perf_thread_map__put(threads);
out_put:
	perf_cpu_map__put(cpus);
out:
	return err;
}

int evlist__open(struct evlist *evlist)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13
	int err;

	/*
	 * Default: one fd per CPU, all threads, aka systemwide
	 * as sys_perf_event_open(cpu = -1, thread = -1) is EINVAL
	 */
<<<<<<< HEAD
	if (evlist->threads == NULL && evlist->cpus == NULL) {
		err = perf_evlist__create_syswide_maps(evlist);
=======
	if (evlist->core.threads == NULL && evlist->core.cpus == NULL) {
		err = evlist__create_syswide_maps(evlist);
>>>>>>> upstream/android-13
		if (err < 0)
			goto out_err;
	}

<<<<<<< HEAD
	perf_evlist__update_id_pos(evlist);

	evlist__for_each_entry(evlist, evsel) {
		err = perf_evsel__open(evsel, evsel->cpus, evsel->threads);
=======
	evlist__update_id_pos(evlist);

	evlist__for_each_entry(evlist, evsel) {
		err = evsel__open(evsel, evsel->core.cpus, evsel->core.threads);
>>>>>>> upstream/android-13
		if (err < 0)
			goto out_err;
	}

	return 0;
out_err:
<<<<<<< HEAD
	perf_evlist__close(evlist);
=======
	evlist__close(evlist);
>>>>>>> upstream/android-13
	errno = -err;
	return err;
}

<<<<<<< HEAD
int perf_evlist__prepare_workload(struct perf_evlist *evlist, struct target *target,
				  const char *argv[], bool pipe_output,
				  void (*exec_error)(int signo, siginfo_t *info, void *ucontext))
=======
int evlist__prepare_workload(struct evlist *evlist, struct target *target, const char *argv[],
			     bool pipe_output, void (*exec_error)(int signo, siginfo_t *info, void *ucontext))
>>>>>>> upstream/android-13
{
	int child_ready_pipe[2], go_pipe[2];
	char bf;

	if (pipe(child_ready_pipe) < 0) {
		perror("failed to create 'ready' pipe");
		return -1;
	}

	if (pipe(go_pipe) < 0) {
		perror("failed to create 'go' pipe");
		goto out_close_ready_pipe;
	}

	evlist->workload.pid = fork();
	if (evlist->workload.pid < 0) {
		perror("failed to fork");
		goto out_close_pipes;
	}

	if (!evlist->workload.pid) {
		int ret;

		if (pipe_output)
			dup2(2, 1);

		signal(SIGTERM, SIG_DFL);

		close(child_ready_pipe[0]);
		close(go_pipe[1]);
		fcntl(go_pipe[0], F_SETFD, FD_CLOEXEC);

		/*
<<<<<<< HEAD
=======
		 * Change the name of this process not to confuse --exclude-perf users
		 * that sees 'perf' in the window up to the execvp() and thinks that
		 * perf samples are not being excluded.
		 */
		prctl(PR_SET_NAME, "perf-exec");

		/*
>>>>>>> upstream/android-13
		 * Tell the parent we're ready to go
		 */
		close(child_ready_pipe[1]);

		/*
		 * Wait until the parent tells us to go.
		 */
		ret = read(go_pipe[0], &bf, 1);
		/*
		 * The parent will ask for the execvp() to be performed by
		 * writing exactly one byte, in workload.cork_fd, usually via
<<<<<<< HEAD
		 * perf_evlist__start_workload().
=======
		 * evlist__start_workload().
>>>>>>> upstream/android-13
		 *
		 * For cancelling the workload without actually running it,
		 * the parent will just close workload.cork_fd, without writing
		 * anything, i.e. read will return zero and we just exit()
		 * here.
		 */
		if (ret != 1) {
			if (ret == -1)
				perror("unable to read pipe");
			exit(ret);
		}

		execvp(argv[0], (char **)argv);

		if (exec_error) {
			union sigval val;

			val.sival_int = errno;
			if (sigqueue(getppid(), SIGUSR1, val))
				perror(argv[0]);
		} else
			perror(argv[0]);
		exit(-1);
	}

	if (exec_error) {
		struct sigaction act = {
			.sa_flags     = SA_SIGINFO,
			.sa_sigaction = exec_error,
		};
		sigaction(SIGUSR1, &act, NULL);
	}

	if (target__none(target)) {
<<<<<<< HEAD
		if (evlist->threads == NULL) {
=======
		if (evlist->core.threads == NULL) {
>>>>>>> upstream/android-13
			fprintf(stderr, "FATAL: evlist->threads need to be set at this point (%s:%d).\n",
				__func__, __LINE__);
			goto out_close_pipes;
		}
<<<<<<< HEAD
		thread_map__set_pid(evlist->threads, 0, evlist->workload.pid);
=======
		perf_thread_map__set_pid(evlist->core.threads, 0, evlist->workload.pid);
>>>>>>> upstream/android-13
	}

	close(child_ready_pipe[1]);
	close(go_pipe[0]);
	/*
	 * wait for child to settle
	 */
	if (read(child_ready_pipe[0], &bf, 1) == -1) {
		perror("unable to read pipe");
		goto out_close_pipes;
	}

	fcntl(go_pipe[1], F_SETFD, FD_CLOEXEC);
	evlist->workload.cork_fd = go_pipe[1];
	close(child_ready_pipe[0]);
	return 0;

out_close_pipes:
	close(go_pipe[0]);
	close(go_pipe[1]);
out_close_ready_pipe:
	close(child_ready_pipe[0]);
	close(child_ready_pipe[1]);
	return -1;
}

<<<<<<< HEAD
int perf_evlist__start_workload(struct perf_evlist *evlist)
=======
int evlist__start_workload(struct evlist *evlist)
>>>>>>> upstream/android-13
{
	if (evlist->workload.cork_fd > 0) {
		char bf = 0;
		int ret;
		/*
		 * Remove the cork, let it rip!
		 */
		ret = write(evlist->workload.cork_fd, &bf, 1);
		if (ret < 0)
			perror("unable to write to pipe");

		close(evlist->workload.cork_fd);
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
int perf_evlist__parse_sample(struct perf_evlist *evlist, union perf_event *event,
			      struct perf_sample *sample)
{
	struct perf_evsel *evsel = perf_evlist__event2evsel(evlist, event);

	if (!evsel)
		return -EFAULT;
	return perf_evsel__parse_sample(evsel, event, sample);
}

int perf_evlist__parse_sample_timestamp(struct perf_evlist *evlist,
					union perf_event *event,
					u64 *timestamp)
{
	struct perf_evsel *evsel = perf_evlist__event2evsel(evlist, event);

	if (!evsel)
		return -EFAULT;
	return perf_evsel__parse_sample_timestamp(evsel, event, timestamp);
}

size_t perf_evlist__fprintf(struct perf_evlist *evlist, FILE *fp)
{
	struct perf_evsel *evsel;
	size_t printed = 0;

	evlist__for_each_entry(evlist, evsel) {
		printed += fprintf(fp, "%s%s", evsel->idx ? ", " : "",
				   perf_evsel__name(evsel));
	}

	return printed + fprintf(fp, "\n");
}

int perf_evlist__strerror_open(struct perf_evlist *evlist,
			       int err, char *buf, size_t size)
=======
int evlist__parse_sample(struct evlist *evlist, union perf_event *event, struct perf_sample *sample)
{
	struct evsel *evsel = evlist__event2evsel(evlist, event);

	if (!evsel)
		return -EFAULT;
	return evsel__parse_sample(evsel, event, sample);
}

int evlist__parse_sample_timestamp(struct evlist *evlist, union perf_event *event, u64 *timestamp)
{
	struct evsel *evsel = evlist__event2evsel(evlist, event);

	if (!evsel)
		return -EFAULT;
	return evsel__parse_sample_timestamp(evsel, event, timestamp);
}

int evlist__strerror_open(struct evlist *evlist, int err, char *buf, size_t size)
>>>>>>> upstream/android-13
{
	int printed, value;
	char sbuf[STRERR_BUFSIZE], *emsg = str_error_r(err, sbuf, sizeof(sbuf));

	switch (err) {
	case EACCES:
	case EPERM:
		printed = scnprintf(buf, size,
				    "Error:\t%s.\n"
				    "Hint:\tCheck /proc/sys/kernel/perf_event_paranoid setting.", emsg);

		value = perf_event_paranoid();

		printed += scnprintf(buf + printed, size - printed, "\nHint:\t");

		if (value >= 2) {
			printed += scnprintf(buf + printed, size - printed,
					     "For your workloads it needs to be <= 1\nHint:\t");
		}
		printed += scnprintf(buf + printed, size - printed,
				     "For system wide tracing it needs to be set to -1.\n");

		printed += scnprintf(buf + printed, size - printed,
				    "Hint:\tTry: 'sudo sh -c \"echo -1 > /proc/sys/kernel/perf_event_paranoid\"'\n"
				    "Hint:\tThe current value is %d.", value);
		break;
	case EINVAL: {
<<<<<<< HEAD
		struct perf_evsel *first = perf_evlist__first(evlist);
=======
		struct evsel *first = evlist__first(evlist);
>>>>>>> upstream/android-13
		int max_freq;

		if (sysctl__read_int("kernel/perf_event_max_sample_rate", &max_freq) < 0)
			goto out_default;

<<<<<<< HEAD
		if (first->attr.sample_freq < (u64)max_freq)
=======
		if (first->core.attr.sample_freq < (u64)max_freq)
>>>>>>> upstream/android-13
			goto out_default;

		printed = scnprintf(buf, size,
				    "Error:\t%s.\n"
				    "Hint:\tCheck /proc/sys/kernel/perf_event_max_sample_rate.\n"
				    "Hint:\tThe current value is %d and %" PRIu64 " is being requested.",
<<<<<<< HEAD
				    emsg, max_freq, first->attr.sample_freq);
=======
				    emsg, max_freq, first->core.attr.sample_freq);
>>>>>>> upstream/android-13
		break;
	}
	default:
out_default:
		scnprintf(buf, size, "%s", emsg);
		break;
	}

	return 0;
}

<<<<<<< HEAD
int perf_evlist__strerror_mmap(struct perf_evlist *evlist, int err, char *buf, size_t size)
{
	char sbuf[STRERR_BUFSIZE], *emsg = str_error_r(err, sbuf, sizeof(sbuf));
	int pages_attempted = evlist->mmap_len / 1024, pages_max_per_user, printed = 0;
=======
int evlist__strerror_mmap(struct evlist *evlist, int err, char *buf, size_t size)
{
	char sbuf[STRERR_BUFSIZE], *emsg = str_error_r(err, sbuf, sizeof(sbuf));
	int pages_attempted = evlist->core.mmap_len / 1024, pages_max_per_user, printed = 0;
>>>>>>> upstream/android-13

	switch (err) {
	case EPERM:
		sysctl__read_int("kernel/perf_event_mlock_kb", &pages_max_per_user);
		printed += scnprintf(buf + printed, size - printed,
				     "Error:\t%s.\n"
				     "Hint:\tCheck /proc/sys/kernel/perf_event_mlock_kb (%d kB) setting.\n"
				     "Hint:\tTried using %zd kB.\n",
				     emsg, pages_max_per_user, pages_attempted);

		if (pages_attempted >= pages_max_per_user) {
			printed += scnprintf(buf + printed, size - printed,
					     "Hint:\tTry 'sudo sh -c \"echo %d > /proc/sys/kernel/perf_event_mlock_kb\"', or\n",
					     pages_max_per_user + pages_attempted);
		}

		printed += scnprintf(buf + printed, size - printed,
				     "Hint:\tTry using a smaller -m/--mmap-pages value.");
		break;
	default:
		scnprintf(buf, size, "%s", emsg);
		break;
	}

	return 0;
}

<<<<<<< HEAD
void perf_evlist__to_front(struct perf_evlist *evlist,
			   struct perf_evsel *move_evsel)
{
	struct perf_evsel *evsel, *n;
	LIST_HEAD(move);

	if (move_evsel == perf_evlist__first(evlist))
		return;

	evlist__for_each_entry_safe(evlist, n, evsel) {
		if (evsel->leader == move_evsel->leader)
			list_move_tail(&evsel->node, &move);
	}

	list_splice(&move, &evlist->entries);
}

void perf_evlist__set_tracking_event(struct perf_evlist *evlist,
				     struct perf_evsel *tracking_evsel)
{
	struct perf_evsel *evsel;
=======
void evlist__to_front(struct evlist *evlist, struct evsel *move_evsel)
{
	struct evsel *evsel, *n;
	LIST_HEAD(move);

	if (move_evsel == evlist__first(evlist))
		return;

	evlist__for_each_entry_safe(evlist, n, evsel) {
		if (evsel__leader(evsel) == evsel__leader(move_evsel))
			list_move_tail(&evsel->core.node, &move);
	}

	list_splice(&move, &evlist->core.entries);
}

struct evsel *evlist__get_tracking_event(struct evlist *evlist)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->tracking)
			return evsel;
	}

	return evlist__first(evlist);
}

void evlist__set_tracking_event(struct evlist *evlist, struct evsel *tracking_evsel)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13

	if (tracking_evsel->tracking)
		return;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel != tracking_evsel)
			evsel->tracking = false;
	}

	tracking_evsel->tracking = true;
}

<<<<<<< HEAD
struct perf_evsel *
perf_evlist__find_evsel_by_str(struct perf_evlist *evlist,
			       const char *str)
{
	struct perf_evsel *evsel;
=======
struct evsel *evlist__find_evsel_by_str(struct evlist *evlist, const char *str)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13

	evlist__for_each_entry(evlist, evsel) {
		if (!evsel->name)
			continue;
		if (strcmp(str, evsel->name) == 0)
			return evsel;
	}

	return NULL;
}

<<<<<<< HEAD
void perf_evlist__toggle_bkw_mmap(struct perf_evlist *evlist,
				  enum bkw_mmap_state state)
=======
void evlist__toggle_bkw_mmap(struct evlist *evlist, enum bkw_mmap_state state)
>>>>>>> upstream/android-13
{
	enum bkw_mmap_state old_state = evlist->bkw_mmap_state;
	enum action {
		NONE,
		PAUSE,
		RESUME,
	} action = NONE;

	if (!evlist->overwrite_mmap)
		return;

	switch (old_state) {
	case BKW_MMAP_NOTREADY: {
		if (state != BKW_MMAP_RUNNING)
			goto state_err;
		break;
	}
	case BKW_MMAP_RUNNING: {
		if (state != BKW_MMAP_DATA_PENDING)
			goto state_err;
		action = PAUSE;
		break;
	}
	case BKW_MMAP_DATA_PENDING: {
		if (state != BKW_MMAP_EMPTY)
			goto state_err;
		break;
	}
	case BKW_MMAP_EMPTY: {
		if (state != BKW_MMAP_RUNNING)
			goto state_err;
		action = RESUME;
		break;
	}
	default:
		WARN_ONCE(1, "Shouldn't get there\n");
	}

	evlist->bkw_mmap_state = state;

	switch (action) {
	case PAUSE:
<<<<<<< HEAD
		perf_evlist__pause(evlist);
		break;
	case RESUME:
		perf_evlist__resume(evlist);
=======
		evlist__pause(evlist);
		break;
	case RESUME:
		evlist__resume(evlist);
>>>>>>> upstream/android-13
		break;
	case NONE:
	default:
		break;
	}

state_err:
	return;
}

<<<<<<< HEAD
bool perf_evlist__exclude_kernel(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (!evsel->attr.exclude_kernel)
=======
bool evlist__exclude_kernel(struct evlist *evlist)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (!evsel->core.attr.exclude_kernel)
>>>>>>> upstream/android-13
			return false;
	}

	return true;
}

/*
 * Events in data file are not collect in groups, but we still want
 * the group display. Set the artificial group and set the leader's
 * forced_leader flag to notify the display code.
 */
<<<<<<< HEAD
void perf_evlist__force_leader(struct perf_evlist *evlist)
{
	if (!evlist->nr_groups) {
		struct perf_evsel *leader = perf_evlist__first(evlist);

		perf_evlist__set_leader(evlist);
		leader->forced_leader = true;
	}
}
=======
void evlist__force_leader(struct evlist *evlist)
{
	if (!evlist->core.nr_groups) {
		struct evsel *leader = evlist__first(evlist);

		evlist__set_leader(evlist);
		leader->forced_leader = true;
	}
}

struct evsel *evlist__reset_weak_group(struct evlist *evsel_list, struct evsel *evsel, bool close)
{
	struct evsel *c2, *leader;
	bool is_open = true;

	leader = evsel__leader(evsel);

	pr_debug("Weak group for %s/%d failed\n",
			leader->name, leader->core.nr_members);

	/*
	 * for_each_group_member doesn't work here because it doesn't
	 * include the first entry.
	 */
	evlist__for_each_entry(evsel_list, c2) {
		if (c2 == evsel)
			is_open = false;
		if (evsel__has_leader(c2, leader)) {
			if (is_open && close)
				perf_evsel__close(&c2->core);
			evsel__set_leader(c2, c2);
			c2->core.nr_members = 0;
			/*
			 * Set this for all former members of the group
			 * to indicate they get reopened.
			 */
			c2->reset_group = true;
		}
	}
	return leader;
}

static int evlist__parse_control_fifo(const char *str, int *ctl_fd, int *ctl_fd_ack, bool *ctl_fd_close)
{
	char *s, *p;
	int ret = 0, fd;

	if (strncmp(str, "fifo:", 5))
		return -EINVAL;

	str += 5;
	if (!*str || *str == ',')
		return -EINVAL;

	s = strdup(str);
	if (!s)
		return -ENOMEM;

	p = strchr(s, ',');
	if (p)
		*p = '\0';

	/*
	 * O_RDWR avoids POLLHUPs which is necessary to allow the other
	 * end of a FIFO to be repeatedly opened and closed.
	 */
	fd = open(s, O_RDWR | O_NONBLOCK | O_CLOEXEC);
	if (fd < 0) {
		pr_err("Failed to open '%s'\n", s);
		ret = -errno;
		goto out_free;
	}
	*ctl_fd = fd;
	*ctl_fd_close = true;

	if (p && *++p) {
		/* O_RDWR | O_NONBLOCK means the other end need not be open */
		fd = open(p, O_RDWR | O_NONBLOCK | O_CLOEXEC);
		if (fd < 0) {
			pr_err("Failed to open '%s'\n", p);
			ret = -errno;
			goto out_free;
		}
		*ctl_fd_ack = fd;
	}

out_free:
	free(s);
	return ret;
}

int evlist__parse_control(const char *str, int *ctl_fd, int *ctl_fd_ack, bool *ctl_fd_close)
{
	char *comma = NULL, *endptr = NULL;

	*ctl_fd_close = false;

	if (strncmp(str, "fd:", 3))
		return evlist__parse_control_fifo(str, ctl_fd, ctl_fd_ack, ctl_fd_close);

	*ctl_fd = strtoul(&str[3], &endptr, 0);
	if (endptr == &str[3])
		return -EINVAL;

	comma = strchr(str, ',');
	if (comma) {
		if (endptr != comma)
			return -EINVAL;

		*ctl_fd_ack = strtoul(comma + 1, &endptr, 0);
		if (endptr == comma + 1 || *endptr != '\0')
			return -EINVAL;
	}

	return 0;
}

void evlist__close_control(int ctl_fd, int ctl_fd_ack, bool *ctl_fd_close)
{
	if (*ctl_fd_close) {
		*ctl_fd_close = false;
		close(ctl_fd);
		if (ctl_fd_ack >= 0)
			close(ctl_fd_ack);
	}
}

int evlist__initialize_ctlfd(struct evlist *evlist, int fd, int ack)
{
	if (fd == -1) {
		pr_debug("Control descriptor is not initialized\n");
		return 0;
	}

	evlist->ctl_fd.pos = perf_evlist__add_pollfd(&evlist->core, fd, NULL, POLLIN,
						     fdarray_flag__nonfilterable);
	if (evlist->ctl_fd.pos < 0) {
		evlist->ctl_fd.pos = -1;
		pr_err("Failed to add ctl fd entry: %m\n");
		return -1;
	}

	evlist->ctl_fd.fd = fd;
	evlist->ctl_fd.ack = ack;

	return 0;
}

bool evlist__ctlfd_initialized(struct evlist *evlist)
{
	return evlist->ctl_fd.pos >= 0;
}

int evlist__finalize_ctlfd(struct evlist *evlist)
{
	struct pollfd *entries = evlist->core.pollfd.entries;

	if (!evlist__ctlfd_initialized(evlist))
		return 0;

	entries[evlist->ctl_fd.pos].fd = -1;
	entries[evlist->ctl_fd.pos].events = 0;
	entries[evlist->ctl_fd.pos].revents = 0;

	evlist->ctl_fd.pos = -1;
	evlist->ctl_fd.ack = -1;
	evlist->ctl_fd.fd = -1;

	return 0;
}

static int evlist__ctlfd_recv(struct evlist *evlist, enum evlist_ctl_cmd *cmd,
			      char *cmd_data, size_t data_size)
{
	int err;
	char c;
	size_t bytes_read = 0;

	*cmd = EVLIST_CTL_CMD_UNSUPPORTED;
	memset(cmd_data, 0, data_size);
	data_size--;

	do {
		err = read(evlist->ctl_fd.fd, &c, 1);
		if (err > 0) {
			if (c == '\n' || c == '\0')
				break;
			cmd_data[bytes_read++] = c;
			if (bytes_read == data_size)
				break;
			continue;
		} else if (err == -1) {
			if (errno == EINTR)
				continue;
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				err = 0;
			else
				pr_err("Failed to read from ctlfd %d: %m\n", evlist->ctl_fd.fd);
		}
		break;
	} while (1);

	pr_debug("Message from ctl_fd: \"%s%s\"\n", cmd_data,
		 bytes_read == data_size ? "" : c == '\n' ? "\\n" : "\\0");

	if (bytes_read > 0) {
		if (!strncmp(cmd_data, EVLIST_CTL_CMD_ENABLE_TAG,
			     (sizeof(EVLIST_CTL_CMD_ENABLE_TAG)-1))) {
			*cmd = EVLIST_CTL_CMD_ENABLE;
		} else if (!strncmp(cmd_data, EVLIST_CTL_CMD_DISABLE_TAG,
				    (sizeof(EVLIST_CTL_CMD_DISABLE_TAG)-1))) {
			*cmd = EVLIST_CTL_CMD_DISABLE;
		} else if (!strncmp(cmd_data, EVLIST_CTL_CMD_SNAPSHOT_TAG,
				    (sizeof(EVLIST_CTL_CMD_SNAPSHOT_TAG)-1))) {
			*cmd = EVLIST_CTL_CMD_SNAPSHOT;
			pr_debug("is snapshot\n");
		} else if (!strncmp(cmd_data, EVLIST_CTL_CMD_EVLIST_TAG,
				    (sizeof(EVLIST_CTL_CMD_EVLIST_TAG)-1))) {
			*cmd = EVLIST_CTL_CMD_EVLIST;
		} else if (!strncmp(cmd_data, EVLIST_CTL_CMD_STOP_TAG,
				    (sizeof(EVLIST_CTL_CMD_STOP_TAG)-1))) {
			*cmd = EVLIST_CTL_CMD_STOP;
		} else if (!strncmp(cmd_data, EVLIST_CTL_CMD_PING_TAG,
				    (sizeof(EVLIST_CTL_CMD_PING_TAG)-1))) {
			*cmd = EVLIST_CTL_CMD_PING;
		}
	}

	return bytes_read ? (int)bytes_read : err;
}

int evlist__ctlfd_ack(struct evlist *evlist)
{
	int err;

	if (evlist->ctl_fd.ack == -1)
		return 0;

	err = write(evlist->ctl_fd.ack, EVLIST_CTL_CMD_ACK_TAG,
		    sizeof(EVLIST_CTL_CMD_ACK_TAG));
	if (err == -1)
		pr_err("failed to write to ctl_ack_fd %d: %m\n", evlist->ctl_fd.ack);

	return err;
}

static int get_cmd_arg(char *cmd_data, size_t cmd_size, char **arg)
{
	char *data = cmd_data + cmd_size;

	/* no argument */
	if (!*data)
		return 0;

	/* there's argument */
	if (*data == ' ') {
		*arg = data + 1;
		return 1;
	}

	/* malformed */
	return -1;
}

static int evlist__ctlfd_enable(struct evlist *evlist, char *cmd_data, bool enable)
{
	struct evsel *evsel;
	char *name;
	int err;

	err = get_cmd_arg(cmd_data,
			  enable ? sizeof(EVLIST_CTL_CMD_ENABLE_TAG) - 1 :
				   sizeof(EVLIST_CTL_CMD_DISABLE_TAG) - 1,
			  &name);
	if (err < 0) {
		pr_info("failed: wrong command\n");
		return -1;
	}

	if (err) {
		evsel = evlist__find_evsel_by_str(evlist, name);
		if (evsel) {
			if (enable)
				evlist__enable_evsel(evlist, name);
			else
				evlist__disable_evsel(evlist, name);
			pr_info("Event %s %s\n", evsel->name,
				enable ? "enabled" : "disabled");
		} else {
			pr_info("failed: can't find '%s' event\n", name);
		}
	} else {
		if (enable) {
			evlist__enable(evlist);
			pr_info(EVLIST_ENABLED_MSG);
		} else {
			evlist__disable(evlist);
			pr_info(EVLIST_DISABLED_MSG);
		}
	}

	return 0;
}

static int evlist__ctlfd_list(struct evlist *evlist, char *cmd_data)
{
	struct perf_attr_details details = { .verbose = false, };
	struct evsel *evsel;
	char *arg;
	int err;

	err = get_cmd_arg(cmd_data,
			  sizeof(EVLIST_CTL_CMD_EVLIST_TAG) - 1,
			  &arg);
	if (err < 0) {
		pr_info("failed: wrong command\n");
		return -1;
	}

	if (err) {
		if (!strcmp(arg, "-v")) {
			details.verbose = true;
		} else if (!strcmp(arg, "-g")) {
			details.event_group = true;
		} else if (!strcmp(arg, "-F")) {
			details.freq = true;
		} else {
			pr_info("failed: wrong command\n");
			return -1;
		}
	}

	evlist__for_each_entry(evlist, evsel)
		evsel__fprintf(evsel, &details, stderr);

	return 0;
}

int evlist__ctlfd_process(struct evlist *evlist, enum evlist_ctl_cmd *cmd)
{
	int err = 0;
	char cmd_data[EVLIST_CTL_CMD_MAX_LEN];
	int ctlfd_pos = evlist->ctl_fd.pos;
	struct pollfd *entries = evlist->core.pollfd.entries;

	if (!evlist__ctlfd_initialized(evlist) || !entries[ctlfd_pos].revents)
		return 0;

	if (entries[ctlfd_pos].revents & POLLIN) {
		err = evlist__ctlfd_recv(evlist, cmd, cmd_data,
					 EVLIST_CTL_CMD_MAX_LEN);
		if (err > 0) {
			switch (*cmd) {
			case EVLIST_CTL_CMD_ENABLE:
			case EVLIST_CTL_CMD_DISABLE:
				err = evlist__ctlfd_enable(evlist, cmd_data,
							   *cmd == EVLIST_CTL_CMD_ENABLE);
				break;
			case EVLIST_CTL_CMD_EVLIST:
				err = evlist__ctlfd_list(evlist, cmd_data);
				break;
			case EVLIST_CTL_CMD_SNAPSHOT:
			case EVLIST_CTL_CMD_STOP:
			case EVLIST_CTL_CMD_PING:
				break;
			case EVLIST_CTL_CMD_ACK:
			case EVLIST_CTL_CMD_UNSUPPORTED:
			default:
				pr_debug("ctlfd: unsupported %d\n", *cmd);
				break;
			}
			if (!(*cmd == EVLIST_CTL_CMD_ACK || *cmd == EVLIST_CTL_CMD_UNSUPPORTED ||
			      *cmd == EVLIST_CTL_CMD_SNAPSHOT))
				evlist__ctlfd_ack(evlist);
		}
	}

	if (entries[ctlfd_pos].revents & (POLLHUP | POLLERR))
		evlist__finalize_ctlfd(evlist);
	else
		entries[ctlfd_pos].revents = 0;

	return err;
}

struct evsel *evlist__find_evsel(struct evlist *evlist, int idx)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->core.idx == idx)
			return evsel;
	}
	return NULL;
}

int evlist__scnprintf_evsels(struct evlist *evlist, size_t size, char *bf)
{
	struct evsel *evsel;
	int printed = 0;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel__is_dummy_event(evsel))
			continue;
		if (size > (strlen(evsel__name(evsel)) + (printed ? 2 : 1))) {
			printed += scnprintf(bf + printed, size - printed, "%s%s", printed ? "," : "", evsel__name(evsel));
		} else {
			printed += scnprintf(bf + printed, size - printed, "%s...", printed ? "," : "");
			break;
		}
	}

	return printed;
}

void evlist__check_mem_load_aux(struct evlist *evlist)
{
	struct evsel *leader, *evsel, *pos;

	/*
	 * For some platforms, the 'mem-loads' event is required to use
	 * together with 'mem-loads-aux' within a group and 'mem-loads-aux'
	 * must be the group leader. Now we disable this group before reporting
	 * because 'mem-loads-aux' is just an auxiliary event. It doesn't carry
	 * any valid memory load information.
	 */
	evlist__for_each_entry(evlist, evsel) {
		leader = evsel__leader(evsel);
		if (leader == evsel)
			continue;

		if (leader->name && strstr(leader->name, "mem-loads-aux")) {
			for_each_group_evsel(pos, leader) {
				evsel__set_leader(pos, pos);
				pos->core.nr_members = 0;
			}
		}
	}
}
>>>>>>> upstream/android-13
