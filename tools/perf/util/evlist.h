/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_EVLIST_H
#define __PERF_EVLIST_H 1

#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/refcount.h>
#include <linux/list.h>
#include <api/fd/array.h>
<<<<<<< HEAD
#include <stdio.h>
#include "../perf.h"
#include "event.h"
#include "evsel.h"
#include "mmap.h"
#include "util.h"
=======
#include <internal/evlist.h>
#include <internal/evsel.h>
#include "events_stats.h"
#include "evsel.h"
#include <pthread.h>
>>>>>>> upstream/android-13
#include <signal.h>
#include <unistd.h>

struct pollfd;
struct thread_map;
<<<<<<< HEAD
struct cpu_map;
struct record_opts;

#define PERF_EVLIST__HLIST_BITS 8
#define PERF_EVLIST__HLIST_SIZE (1 << PERF_EVLIST__HLIST_BITS)

struct perf_evlist {
	struct list_head entries;
	struct hlist_head heads[PERF_EVLIST__HLIST_SIZE];
	int		 nr_entries;
	int		 nr_groups;
	int		 nr_mmaps;
	bool		 enabled;
	bool		 has_user_cpus;
	size_t		 mmap_len;
=======
struct perf_cpu_map;
struct record_opts;

/*
 * State machine of bkw_mmap_state:
 *
 *                     .________________(forbid)_____________.
 *                     |                                     V
 * NOTREADY --(0)--> RUNNING --(1)--> DATA_PENDING --(2)--> EMPTY
 *                     ^  ^              |   ^               |
 *                     |  |__(forbid)____/   |___(forbid)___/|
 *                     |                                     |
 *                      \_________________(3)_______________/
 *
 * NOTREADY     : Backward ring buffers are not ready
 * RUNNING      : Backward ring buffers are recording
 * DATA_PENDING : We are required to collect data from backward ring buffers
 * EMPTY        : We have collected data from backward ring buffers.
 *
 * (0): Setup backward ring buffer
 * (1): Pause ring buffers for reading
 * (2): Read from ring buffers
 * (3): Resume ring buffers for recording
 */
enum bkw_mmap_state {
	BKW_MMAP_NOTREADY,
	BKW_MMAP_RUNNING,
	BKW_MMAP_DATA_PENDING,
	BKW_MMAP_EMPTY,
};

struct evlist {
	struct perf_evlist core;
	bool		 enabled;
>>>>>>> upstream/android-13
	int		 id_pos;
	int		 is_pos;
	u64		 combined_sample_type;
	enum bkw_mmap_state bkw_mmap_state;
	struct {
		int	cork_fd;
		pid_t	pid;
	} workload;
<<<<<<< HEAD
	struct fdarray	 pollfd;
	struct perf_mmap *mmap;
	struct perf_mmap *overwrite_mmap;
	struct thread_map *threads;
	struct cpu_map	  *cpus;
	struct perf_evsel *selected;
	struct events_stats stats;
	struct perf_env	*env;
	u64		first_sample_time;
	u64		last_sample_time;
};

struct perf_evsel_str_handler {
=======
	struct mmap *mmap;
	struct mmap *overwrite_mmap;
	struct evsel *selected;
	struct events_stats stats;
	struct perf_env	*env;
	void (*trace_event_sample_raw)(struct evlist *evlist,
				       union perf_event *event,
				       struct perf_sample *sample);
	u64		first_sample_time;
	u64		last_sample_time;
	struct {
		pthread_t		th;
		volatile int		done;
	} thread;
	struct {
		int	fd;	/* control file descriptor */
		int	ack;	/* ack file descriptor for control commands */
		int	pos;	/* index at evlist core object to check signals */
	} ctl_fd;
};

struct evsel_str_handler {
>>>>>>> upstream/android-13
	const char *name;
	void	   *handler;
};

<<<<<<< HEAD
struct perf_evlist *perf_evlist__new(void);
struct perf_evlist *perf_evlist__new_default(void);
struct perf_evlist *perf_evlist__new_dummy(void);
void perf_evlist__init(struct perf_evlist *evlist, struct cpu_map *cpus,
		       struct thread_map *threads);
void perf_evlist__exit(struct perf_evlist *evlist);
void perf_evlist__delete(struct perf_evlist *evlist);

void perf_evlist__add(struct perf_evlist *evlist, struct perf_evsel *entry);
void perf_evlist__remove(struct perf_evlist *evlist, struct perf_evsel *evsel);

int __perf_evlist__add_default(struct perf_evlist *evlist, bool precise);

static inline int perf_evlist__add_default(struct perf_evlist *evlist)
{
	return __perf_evlist__add_default(evlist, true);
}

int __perf_evlist__add_default_attrs(struct perf_evlist *evlist,
				     struct perf_event_attr *attrs, size_t nr_attrs);

#define perf_evlist__add_default_attrs(evlist, array) \
	__perf_evlist__add_default_attrs(evlist, array, ARRAY_SIZE(array))

int perf_evlist__add_dummy(struct perf_evlist *evlist);

int perf_evlist__add_newtp(struct perf_evlist *evlist,
			   const char *sys, const char *name, void *handler);

void __perf_evlist__set_sample_bit(struct perf_evlist *evlist,
				   enum perf_event_sample_format bit);
void __perf_evlist__reset_sample_bit(struct perf_evlist *evlist,
				     enum perf_event_sample_format bit);

#define perf_evlist__set_sample_bit(evlist, bit) \
	__perf_evlist__set_sample_bit(evlist, PERF_SAMPLE_##bit)

#define perf_evlist__reset_sample_bit(evlist, bit) \
	__perf_evlist__reset_sample_bit(evlist, PERF_SAMPLE_##bit)

int perf_evlist__set_filter(struct perf_evlist *evlist, const char *filter);
int perf_evlist__set_filter_pid(struct perf_evlist *evlist, pid_t pid);
int perf_evlist__set_filter_pids(struct perf_evlist *evlist, size_t npids, pid_t *pids);

struct perf_evsel *
perf_evlist__find_tracepoint_by_id(struct perf_evlist *evlist, int id);

struct perf_evsel *
perf_evlist__find_tracepoint_by_name(struct perf_evlist *evlist,
				     const char *name);

void perf_evlist__id_add(struct perf_evlist *evlist, struct perf_evsel *evsel,
			 int cpu, int thread, u64 id);
int perf_evlist__id_add_fd(struct perf_evlist *evlist,
			   struct perf_evsel *evsel,
			   int cpu, int thread, int fd);

int perf_evlist__add_pollfd(struct perf_evlist *evlist, int fd);
int perf_evlist__alloc_pollfd(struct perf_evlist *evlist);
int perf_evlist__filter_pollfd(struct perf_evlist *evlist, short revents_and_mask);

int perf_evlist__poll(struct perf_evlist *evlist, int timeout);

struct perf_evsel *perf_evlist__id2evsel(struct perf_evlist *evlist, u64 id);
struct perf_evsel *perf_evlist__id2evsel_strict(struct perf_evlist *evlist,
						u64 id);

struct perf_sample_id *perf_evlist__id2sid(struct perf_evlist *evlist, u64 id);

void perf_evlist__toggle_bkw_mmap(struct perf_evlist *evlist, enum bkw_mmap_state state);

void perf_evlist__mmap_consume(struct perf_evlist *evlist, int idx);

int perf_evlist__open(struct perf_evlist *evlist);
void perf_evlist__close(struct perf_evlist *evlist);

struct callchain_param;

void perf_evlist__set_id_pos(struct perf_evlist *evlist);
bool perf_can_sample_identifier(void);
bool perf_can_record_switch_events(void);
bool perf_can_record_cpu_wide(void);
void perf_evlist__config(struct perf_evlist *evlist, struct record_opts *opts,
			 struct callchain_param *callchain);
int record_opts__config(struct record_opts *opts);

int perf_evlist__prepare_workload(struct perf_evlist *evlist,
				  struct target *target,
				  const char *argv[], bool pipe_output,
				  void (*exec_error)(int signo, siginfo_t *info,
						     void *ucontext));
int perf_evlist__start_workload(struct perf_evlist *evlist);

struct option;

int __perf_evlist__parse_mmap_pages(unsigned int *mmap_pages, const char *str);
int perf_evlist__parse_mmap_pages(const struct option *opt,
				  const char *str,
				  int unset);

unsigned long perf_event_mlock_kb_in_pages(void);

int perf_evlist__mmap_ex(struct perf_evlist *evlist, unsigned int pages,
			 unsigned int auxtrace_pages,
			 bool auxtrace_overwrite);
int perf_evlist__mmap(struct perf_evlist *evlist, unsigned int pages);
void perf_evlist__munmap(struct perf_evlist *evlist);

size_t perf_evlist__mmap_size(unsigned long pages);

void perf_evlist__disable(struct perf_evlist *evlist);
void perf_evlist__enable(struct perf_evlist *evlist);
void perf_evlist__toggle_enable(struct perf_evlist *evlist);

int perf_evlist__enable_event_idx(struct perf_evlist *evlist,
				  struct perf_evsel *evsel, int idx);

void perf_evlist__set_selected(struct perf_evlist *evlist,
			       struct perf_evsel *evsel);

void perf_evlist__set_maps(struct perf_evlist *evlist, struct cpu_map *cpus,
			   struct thread_map *threads);
int perf_evlist__create_maps(struct perf_evlist *evlist, struct target *target);
int perf_evlist__apply_filters(struct perf_evlist *evlist, struct perf_evsel **err_evsel);

void __perf_evlist__set_leader(struct list_head *list);
void perf_evlist__set_leader(struct perf_evlist *evlist);

u64 perf_evlist__read_format(struct perf_evlist *evlist);
u64 __perf_evlist__combined_sample_type(struct perf_evlist *evlist);
u64 perf_evlist__combined_sample_type(struct perf_evlist *evlist);
u64 perf_evlist__combined_branch_type(struct perf_evlist *evlist);
bool perf_evlist__sample_id_all(struct perf_evlist *evlist);
u16 perf_evlist__id_hdr_size(struct perf_evlist *evlist);

int perf_evlist__parse_sample(struct perf_evlist *evlist, union perf_event *event,
			      struct perf_sample *sample);

int perf_evlist__parse_sample_timestamp(struct perf_evlist *evlist,
					union perf_event *event,
					u64 *timestamp);

bool perf_evlist__valid_sample_type(struct perf_evlist *evlist);
bool perf_evlist__valid_sample_id_all(struct perf_evlist *evlist);
bool perf_evlist__valid_read_format(struct perf_evlist *evlist);

void perf_evlist__splice_list_tail(struct perf_evlist *evlist,
				   struct list_head *list);

static inline bool perf_evlist__empty(struct perf_evlist *evlist)
{
	return list_empty(&evlist->entries);
}

static inline struct perf_evsel *perf_evlist__first(struct perf_evlist *evlist)
{
	return list_entry(evlist->entries.next, struct perf_evsel, node);
}

static inline struct perf_evsel *perf_evlist__last(struct perf_evlist *evlist)
{
	return list_entry(evlist->entries.prev, struct perf_evsel, node);
}

size_t perf_evlist__fprintf(struct perf_evlist *evlist, FILE *fp);

int perf_evlist__strerror_open(struct perf_evlist *evlist, int err, char *buf, size_t size);
int perf_evlist__strerror_mmap(struct perf_evlist *evlist, int err, char *buf, size_t size);

bool perf_evlist__can_select_event(struct perf_evlist *evlist, const char *str);
void perf_evlist__to_front(struct perf_evlist *evlist,
			   struct perf_evsel *move_evsel);
=======
struct evlist *evlist__new(void);
struct evlist *evlist__new_default(void);
struct evlist *evlist__new_dummy(void);
void evlist__init(struct evlist *evlist, struct perf_cpu_map *cpus,
		  struct perf_thread_map *threads);
void evlist__exit(struct evlist *evlist);
void evlist__delete(struct evlist *evlist);

void evlist__add(struct evlist *evlist, struct evsel *entry);
void evlist__remove(struct evlist *evlist, struct evsel *evsel);

int __evlist__add_default(struct evlist *evlist, bool precise);

static inline int evlist__add_default(struct evlist *evlist)
{
	return __evlist__add_default(evlist, true);
}

int __evlist__add_default_attrs(struct evlist *evlist,
				     struct perf_event_attr *attrs, size_t nr_attrs);

#define evlist__add_default_attrs(evlist, array) \
	__evlist__add_default_attrs(evlist, array, ARRAY_SIZE(array))

int arch_evlist__add_default_attrs(struct evlist *evlist);

int evlist__add_dummy(struct evlist *evlist);

int evlist__add_sb_event(struct evlist *evlist, struct perf_event_attr *attr,
			 evsel__sb_cb_t cb, void *data);
void evlist__set_cb(struct evlist *evlist, evsel__sb_cb_t cb, void *data);
int evlist__start_sb_thread(struct evlist *evlist, struct target *target);
void evlist__stop_sb_thread(struct evlist *evlist);

int evlist__add_newtp(struct evlist *evlist, const char *sys, const char *name, void *handler);

int __evlist__set_tracepoints_handlers(struct evlist *evlist,
				       const struct evsel_str_handler *assocs,
				       size_t nr_assocs);

#define evlist__set_tracepoints_handlers(evlist, array) \
	__evlist__set_tracepoints_handlers(evlist, array, ARRAY_SIZE(array))

int evlist__set_tp_filter(struct evlist *evlist, const char *filter);
int evlist__set_tp_filter_pid(struct evlist *evlist, pid_t pid);
int evlist__set_tp_filter_pids(struct evlist *evlist, size_t npids, pid_t *pids);

int evlist__append_tp_filter(struct evlist *evlist, const char *filter);

int evlist__append_tp_filter_pid(struct evlist *evlist, pid_t pid);
int evlist__append_tp_filter_pids(struct evlist *evlist, size_t npids, pid_t *pids);

struct evsel *evlist__find_tracepoint_by_id(struct evlist *evlist, int id);
struct evsel *evlist__find_tracepoint_by_name(struct evlist *evlist, const char *name);

int evlist__add_pollfd(struct evlist *evlist, int fd);
int evlist__filter_pollfd(struct evlist *evlist, short revents_and_mask);

#ifdef HAVE_EVENTFD_SUPPORT
int evlist__add_wakeup_eventfd(struct evlist *evlist, int fd);
#endif

int evlist__poll(struct evlist *evlist, int timeout);

struct evsel *evlist__id2evsel(struct evlist *evlist, u64 id);
struct evsel *evlist__id2evsel_strict(struct evlist *evlist, u64 id);

struct perf_sample_id *evlist__id2sid(struct evlist *evlist, u64 id);

void evlist__toggle_bkw_mmap(struct evlist *evlist, enum bkw_mmap_state state);

void evlist__mmap_consume(struct evlist *evlist, int idx);

int evlist__open(struct evlist *evlist);
void evlist__close(struct evlist *evlist);

struct callchain_param;

void evlist__set_id_pos(struct evlist *evlist);
void evlist__config(struct evlist *evlist, struct record_opts *opts, struct callchain_param *callchain);
int record_opts__config(struct record_opts *opts);

int evlist__prepare_workload(struct evlist *evlist, struct target *target,
			     const char *argv[], bool pipe_output,
			     void (*exec_error)(int signo, siginfo_t *info, void *ucontext));
int evlist__start_workload(struct evlist *evlist);

struct option;

int __evlist__parse_mmap_pages(unsigned int *mmap_pages, const char *str);
int evlist__parse_mmap_pages(const struct option *opt, const char *str, int unset);

unsigned long perf_event_mlock_kb_in_pages(void);

int evlist__mmap_ex(struct evlist *evlist, unsigned int pages,
			 unsigned int auxtrace_pages,
			 bool auxtrace_overwrite, int nr_cblocks,
			 int affinity, int flush, int comp_level);
int evlist__mmap(struct evlist *evlist, unsigned int pages);
void evlist__munmap(struct evlist *evlist);

size_t evlist__mmap_size(unsigned long pages);

void evlist__disable(struct evlist *evlist);
void evlist__enable(struct evlist *evlist);
void evlist__toggle_enable(struct evlist *evlist);
void evlist__disable_evsel(struct evlist *evlist, char *evsel_name);
void evlist__enable_evsel(struct evlist *evlist, char *evsel_name);

int evlist__enable_event_idx(struct evlist *evlist, struct evsel *evsel, int idx);

void evlist__set_selected(struct evlist *evlist, struct evsel *evsel);

int evlist__create_maps(struct evlist *evlist, struct target *target);
int evlist__apply_filters(struct evlist *evlist, struct evsel **err_evsel);

void evlist__set_leader(struct evlist *evlist);

u64 __evlist__combined_sample_type(struct evlist *evlist);
u64 evlist__combined_sample_type(struct evlist *evlist);
u64 evlist__combined_branch_type(struct evlist *evlist);
bool evlist__sample_id_all(struct evlist *evlist);
u16 evlist__id_hdr_size(struct evlist *evlist);

int evlist__parse_sample(struct evlist *evlist, union perf_event *event, struct perf_sample *sample);
int evlist__parse_sample_timestamp(struct evlist *evlist, union perf_event *event, u64 *timestamp);

bool evlist__valid_sample_type(struct evlist *evlist);
bool evlist__valid_sample_id_all(struct evlist *evlist);
bool evlist__valid_read_format(struct evlist *evlist);

void evlist__splice_list_tail(struct evlist *evlist, struct list_head *list);

static inline bool evlist__empty(struct evlist *evlist)
{
	return list_empty(&evlist->core.entries);
}

static inline struct evsel *evlist__first(struct evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__first(&evlist->core);

	return container_of(evsel, struct evsel, core);
}

static inline struct evsel *evlist__last(struct evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(&evlist->core);

	return container_of(evsel, struct evsel, core);
}

int evlist__strerror_open(struct evlist *evlist, int err, char *buf, size_t size);
int evlist__strerror_mmap(struct evlist *evlist, int err, char *buf, size_t size);

bool evlist__can_select_event(struct evlist *evlist, const char *str);
void evlist__to_front(struct evlist *evlist, struct evsel *move_evsel);
>>>>>>> upstream/android-13

/**
 * __evlist__for_each_entry - iterate thru all the evsels
 * @list: list_head instance to iterate
 * @evsel: struct evsel iterator
 */
#define __evlist__for_each_entry(list, evsel) \
<<<<<<< HEAD
        list_for_each_entry(evsel, list, node)
=======
        list_for_each_entry(evsel, list, core.node)
>>>>>>> upstream/android-13

/**
 * evlist__for_each_entry - iterate thru all the evsels
 * @evlist: evlist instance to iterate
 * @evsel: struct evsel iterator
 */
#define evlist__for_each_entry(evlist, evsel) \
<<<<<<< HEAD
	__evlist__for_each_entry(&(evlist)->entries, evsel)
=======
	__evlist__for_each_entry(&(evlist)->core.entries, evsel)
>>>>>>> upstream/android-13

/**
 * __evlist__for_each_entry_continue - continue iteration thru all the evsels
 * @list: list_head instance to iterate
 * @evsel: struct evsel iterator
 */
#define __evlist__for_each_entry_continue(list, evsel) \
<<<<<<< HEAD
        list_for_each_entry_continue(evsel, list, node)
=======
        list_for_each_entry_continue(evsel, list, core.node)
>>>>>>> upstream/android-13

/**
 * evlist__for_each_entry_continue - continue iteration thru all the evsels
 * @evlist: evlist instance to iterate
 * @evsel: struct evsel iterator
 */
#define evlist__for_each_entry_continue(evlist, evsel) \
<<<<<<< HEAD
	__evlist__for_each_entry_continue(&(evlist)->entries, evsel)
=======
	__evlist__for_each_entry_continue(&(evlist)->core.entries, evsel)

/**
 * __evlist__for_each_entry_from - continue iteration from @evsel (included)
 * @list: list_head instance to iterate
 * @evsel: struct evsel iterator
 */
#define __evlist__for_each_entry_from(list, evsel) \
	list_for_each_entry_from(evsel, list, core.node)

/**
 * evlist__for_each_entry_from - continue iteration from @evsel (included)
 * @evlist: evlist instance to iterate
 * @evsel: struct evsel iterator
 */
#define evlist__for_each_entry_from(evlist, evsel) \
	__evlist__for_each_entry_from(&(evlist)->core.entries, evsel)
>>>>>>> upstream/android-13

/**
 * __evlist__for_each_entry_reverse - iterate thru all the evsels in reverse order
 * @list: list_head instance to iterate
 * @evsel: struct evsel iterator
 */
#define __evlist__for_each_entry_reverse(list, evsel) \
<<<<<<< HEAD
        list_for_each_entry_reverse(evsel, list, node)
=======
        list_for_each_entry_reverse(evsel, list, core.node)
>>>>>>> upstream/android-13

/**
 * evlist__for_each_entry_reverse - iterate thru all the evsels in reverse order
 * @evlist: evlist instance to iterate
 * @evsel: struct evsel iterator
 */
#define evlist__for_each_entry_reverse(evlist, evsel) \
<<<<<<< HEAD
	__evlist__for_each_entry_reverse(&(evlist)->entries, evsel)
=======
	__evlist__for_each_entry_reverse(&(evlist)->core.entries, evsel)
>>>>>>> upstream/android-13

/**
 * __evlist__for_each_entry_safe - safely iterate thru all the evsels
 * @list: list_head instance to iterate
 * @tmp: struct evsel temp iterator
 * @evsel: struct evsel iterator
 */
#define __evlist__for_each_entry_safe(list, tmp, evsel) \
<<<<<<< HEAD
        list_for_each_entry_safe(evsel, tmp, list, node)
=======
        list_for_each_entry_safe(evsel, tmp, list, core.node)
>>>>>>> upstream/android-13

/**
 * evlist__for_each_entry_safe - safely iterate thru all the evsels
 * @evlist: evlist instance to iterate
 * @evsel: struct evsel iterator
 * @tmp: struct evsel temp iterator
 */
#define evlist__for_each_entry_safe(evlist, tmp, evsel) \
<<<<<<< HEAD
	__evlist__for_each_entry_safe(&(evlist)->entries, tmp, evsel)

void perf_evlist__set_tracking_event(struct perf_evlist *evlist,
				     struct perf_evsel *tracking_evsel);

void perf_event_attr__set_max_precise_ip(struct perf_event_attr *attr);

struct perf_evsel *
perf_evlist__find_evsel_by_str(struct perf_evlist *evlist, const char *str);

struct perf_evsel *perf_evlist__event2evsel(struct perf_evlist *evlist,
					    union perf_event *event);

bool perf_evlist__exclude_kernel(struct perf_evlist *evlist);

void perf_evlist__force_leader(struct perf_evlist *evlist);

=======
	__evlist__for_each_entry_safe(&(evlist)->core.entries, tmp, evsel)

#define evlist__for_each_cpu(evlist, index, cpu)	\
	evlist__cpu_iter_start(evlist);			\
	perf_cpu_map__for_each_cpu (cpu, index, (evlist)->core.all_cpus)

struct evsel *evlist__get_tracking_event(struct evlist *evlist);
void evlist__set_tracking_event(struct evlist *evlist, struct evsel *tracking_evsel);

void evlist__cpu_iter_start(struct evlist *evlist);
bool evsel__cpu_iter_skip(struct evsel *ev, int cpu);
bool evsel__cpu_iter_skip_no_inc(struct evsel *ev, int cpu);

struct evsel *evlist__find_evsel_by_str(struct evlist *evlist, const char *str);

struct evsel *evlist__event2evsel(struct evlist *evlist, union perf_event *event);

bool evlist__exclude_kernel(struct evlist *evlist);

void evlist__force_leader(struct evlist *evlist);

struct evsel *evlist__reset_weak_group(struct evlist *evlist, struct evsel *evsel, bool close);

#define EVLIST_CTL_CMD_ENABLE_TAG  "enable"
#define EVLIST_CTL_CMD_DISABLE_TAG "disable"
#define EVLIST_CTL_CMD_ACK_TAG     "ack\n"
#define EVLIST_CTL_CMD_SNAPSHOT_TAG "snapshot"
#define EVLIST_CTL_CMD_EVLIST_TAG "evlist"
#define EVLIST_CTL_CMD_STOP_TAG "stop"
#define EVLIST_CTL_CMD_PING_TAG "ping"

#define EVLIST_CTL_CMD_MAX_LEN 64

enum evlist_ctl_cmd {
	EVLIST_CTL_CMD_UNSUPPORTED = 0,
	EVLIST_CTL_CMD_ENABLE,
	EVLIST_CTL_CMD_DISABLE,
	EVLIST_CTL_CMD_ACK,
	EVLIST_CTL_CMD_SNAPSHOT,
	EVLIST_CTL_CMD_EVLIST,
	EVLIST_CTL_CMD_STOP,
	EVLIST_CTL_CMD_PING,
};

int evlist__parse_control(const char *str, int *ctl_fd, int *ctl_fd_ack, bool *ctl_fd_close);
void evlist__close_control(int ctl_fd, int ctl_fd_ack, bool *ctl_fd_close);
int evlist__initialize_ctlfd(struct evlist *evlist, int ctl_fd, int ctl_fd_ack);
int evlist__finalize_ctlfd(struct evlist *evlist);
bool evlist__ctlfd_initialized(struct evlist *evlist);
int evlist__ctlfd_process(struct evlist *evlist, enum evlist_ctl_cmd *cmd);
int evlist__ctlfd_ack(struct evlist *evlist);

#define EVLIST_ENABLED_MSG "Events enabled\n"
#define EVLIST_DISABLED_MSG "Events disabled\n"

struct evsel *evlist__find_evsel(struct evlist *evlist, int idx);

int evlist__scnprintf_evsels(struct evlist *evlist, size_t size, char *bf);
void evlist__check_mem_load_aux(struct evlist *evlist);
>>>>>>> upstream/android-13
#endif /* __PERF_EVLIST_H */
