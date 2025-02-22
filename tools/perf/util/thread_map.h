/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_THREAD_MAP_H
#define __PERF_THREAD_MAP_H

#include <sys/types.h>
#include <stdio.h>
#include <linux/refcount.h>
<<<<<<< HEAD

struct thread_map_data {
	pid_t    pid;
	char	*comm;
};

struct thread_map {
	refcount_t refcnt;
	int nr;
	int err_thread;
	struct thread_map_data map[];
};

struct thread_map_event;

struct thread_map *thread_map__new_dummy(void);
struct thread_map *thread_map__new_by_pid(pid_t pid);
struct thread_map *thread_map__new_by_tid(pid_t tid);
struct thread_map *thread_map__new_by_uid(uid_t uid);
struct thread_map *thread_map__new_all_cpus(void);
struct thread_map *thread_map__new(pid_t pid, pid_t tid, uid_t uid);
struct thread_map *thread_map__new_event(struct thread_map_event *event);

struct thread_map *thread_map__get(struct thread_map *map);
void thread_map__put(struct thread_map *map);

struct thread_map *thread_map__new_str(const char *pid,
		const char *tid, uid_t uid, bool all_threads);

struct thread_map *thread_map__new_by_tid_str(const char *tid_str);

size_t thread_map__fprintf(struct thread_map *threads, FILE *fp);

static inline int thread_map__nr(struct thread_map *threads)
{
	return threads ? threads->nr : 1;
}

static inline pid_t thread_map__pid(struct thread_map *map, int thread)
{
	return map->map[thread].pid;
}

static inline void
thread_map__set_pid(struct thread_map *map, int thread, pid_t pid)
{
	map->map[thread].pid = pid;
}

static inline char *thread_map__comm(struct thread_map *map, int thread)
{
	return map->map[thread].comm;
}

void thread_map__read_comms(struct thread_map *threads);
bool thread_map__has(struct thread_map *threads, pid_t pid);
int thread_map__remove(struct thread_map *threads, int idx);
=======
#include <internal/threadmap.h>
#include <perf/threadmap.h>

struct perf_record_thread_map;

struct perf_thread_map *thread_map__new_dummy(void);
struct perf_thread_map *thread_map__new_by_pid(pid_t pid);
struct perf_thread_map *thread_map__new_by_tid(pid_t tid);
struct perf_thread_map *thread_map__new_by_uid(uid_t uid);
struct perf_thread_map *thread_map__new_all_cpus(void);
struct perf_thread_map *thread_map__new(pid_t pid, pid_t tid, uid_t uid);
struct perf_thread_map *thread_map__new_event(struct perf_record_thread_map *event);

struct perf_thread_map *thread_map__new_str(const char *pid,
		const char *tid, uid_t uid, bool all_threads);

struct perf_thread_map *thread_map__new_by_tid_str(const char *tid_str);

size_t thread_map__fprintf(struct perf_thread_map *threads, FILE *fp);

void thread_map__read_comms(struct perf_thread_map *threads);
bool thread_map__has(struct perf_thread_map *threads, pid_t pid);
int thread_map__remove(struct perf_thread_map *threads, int idx);
>>>>>>> upstream/android-13
#endif	/* __PERF_THREAD_MAP_H */
