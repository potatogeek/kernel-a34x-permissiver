/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_CPUMAP_H
#define __PERF_CPUMAP_H

#include <stdio.h>
#include <stdbool.h>
<<<<<<< HEAD
#include <linux/refcount.h>

#include "perf.h"
#include "util/debug.h"

struct cpu_map {
	refcount_t refcnt;
	int nr;
	int map[];
};

struct cpu_map *cpu_map__new(const char *cpu_list);
struct cpu_map *cpu_map__empty_new(int nr);
struct cpu_map *cpu_map__dummy_new(void);
struct cpu_map *cpu_map__new_data(struct cpu_map_data *data);
struct cpu_map *cpu_map__read(FILE *file);
size_t cpu_map__snprint(struct cpu_map *map, char *buf, size_t size);
size_t cpu_map__snprint_mask(struct cpu_map *map, char *buf, size_t size);
size_t cpu_map__fprintf(struct cpu_map *map, FILE *fp);
int cpu_map__get_socket_id(int cpu);
int cpu_map__get_socket(struct cpu_map *map, int idx, void *data);
int cpu_map__get_core_id(int cpu);
int cpu_map__get_core(struct cpu_map *map, int idx, void *data);
int cpu_map__build_socket_map(struct cpu_map *cpus, struct cpu_map **sockp);
int cpu_map__build_core_map(struct cpu_map *cpus, struct cpu_map **corep);

struct cpu_map *cpu_map__get(struct cpu_map *map);
void cpu_map__put(struct cpu_map *map);

static inline int cpu_map__socket(struct cpu_map *sock, int s)
=======
#include <internal/cpumap.h>
#include <perf/cpumap.h>

struct aggr_cpu_id {
	int thread;
	int node;
	int socket;
	int die;
	int core;
};

struct cpu_aggr_map {
	refcount_t refcnt;
	int nr;
	struct aggr_cpu_id map[];
};

struct perf_record_cpu_map_data;

struct perf_cpu_map *perf_cpu_map__empty_new(int nr);
struct cpu_aggr_map *cpu_aggr_map__empty_new(int nr);

struct perf_cpu_map *cpu_map__new_data(struct perf_record_cpu_map_data *data);
size_t cpu_map__snprint(struct perf_cpu_map *map, char *buf, size_t size);
size_t cpu_map__snprint_mask(struct perf_cpu_map *map, char *buf, size_t size);
size_t cpu_map__fprintf(struct perf_cpu_map *map, FILE *fp);
int cpu_map__get_socket_id(int cpu);
struct aggr_cpu_id cpu_map__get_socket(struct perf_cpu_map *map, int idx, void *data);
int cpu_map__get_die_id(int cpu);
struct aggr_cpu_id cpu_map__get_die(struct perf_cpu_map *map, int idx, void *data);
int cpu_map__get_core_id(int cpu);
struct aggr_cpu_id cpu_map__get_core(struct perf_cpu_map *map, int idx, void *data);
int cpu_map__get_node_id(int cpu);
struct aggr_cpu_id  cpu_map__get_node(struct perf_cpu_map *map, int idx, void *data);
int cpu_map__build_socket_map(struct perf_cpu_map *cpus, struct cpu_aggr_map **sockp);
int cpu_map__build_die_map(struct perf_cpu_map *cpus, struct cpu_aggr_map **diep);
int cpu_map__build_core_map(struct perf_cpu_map *cpus, struct cpu_aggr_map **corep);
int cpu_map__build_node_map(struct perf_cpu_map *cpus, struct cpu_aggr_map **nodep);
const struct perf_cpu_map *cpu_map__online(void); /* thread unsafe */

static inline int cpu_map__socket(struct perf_cpu_map *sock, int s)
>>>>>>> upstream/android-13
{
	if (!sock || s > sock->nr || s < 0)
		return 0;
	return sock->map[s];
}

<<<<<<< HEAD
static inline int cpu_map__id_to_socket(int id)
{
	return id >> 16;
}

static inline int cpu_map__id_to_cpu(int id)
{
	return id & 0xffff;
}

static inline int cpu_map__nr(const struct cpu_map *map)
{
	return map ? map->nr : 1;
}

static inline bool cpu_map__empty(const struct cpu_map *map)
{
	return map ? map->map[0] == -1 : true;
}

=======
>>>>>>> upstream/android-13
int cpu__setup_cpunode_map(void);

int cpu__max_node(void);
int cpu__max_cpu(void);
int cpu__max_present_cpu(void);
int cpu__get_node(int cpu);

<<<<<<< HEAD
int cpu_map__build_map(struct cpu_map *cpus, struct cpu_map **res,
		       int (*f)(struct cpu_map *map, int cpu, void *data),
		       void *data);

int cpu_map__cpu(struct cpu_map *cpus, int idx);
bool cpu_map__has(struct cpu_map *cpus, int cpu);
int cpu_map__idx(struct cpu_map *cpus, int cpu);
=======
int cpu_map__build_map(struct perf_cpu_map *cpus, struct cpu_aggr_map **res,
		       struct aggr_cpu_id (*f)(struct perf_cpu_map *map, int cpu, void *data),
		       void *data);

int cpu_map__cpu(struct perf_cpu_map *cpus, int idx);
bool cpu_map__has(struct perf_cpu_map *cpus, int cpu);

bool cpu_map__compare_aggr_cpu_id(struct aggr_cpu_id a, struct aggr_cpu_id b);
bool cpu_map__aggr_cpu_id_is_empty(struct aggr_cpu_id a);
struct aggr_cpu_id cpu_map__empty_aggr_cpu_id(void);

>>>>>>> upstream/android-13
#endif /* __PERF_CPUMAP_H */
