/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_EVSEL_H
#define __PERF_EVSEL_H 1

#include <linux/list.h>
#include <stdbool.h>
<<<<<<< HEAD
#include <stddef.h>
#include <linux/perf_event.h>
#include <linux/types.h>
#include "xyarray.h"
#include "symbol.h"
#include "cpumap.h"
#include "counts.h"

struct perf_evsel;

/*
 * Per fd, to map back from PERF_SAMPLE_ID to evsel, only used when there are
 * more than one entry in the evlist.
 */
struct perf_sample_id {
	struct hlist_node 	node;
	u64		 	id;
	struct perf_evsel	*evsel;
	int			idx;
	int			cpu;
	pid_t			tid;

	/* Holds total ID period value for PERF_SAMPLE_READ processing. */
	u64			period;
};

struct cgroup;

/*
 * The 'struct perf_evsel_config_term' is used to pass event
 * specific configuration data to perf_evsel__config routine.
 * It is allocated within event parsing and attached to
 * perf_evsel::config_terms list head.
*/
enum term_type {
	PERF_EVSEL__CONFIG_TERM_PERIOD,
	PERF_EVSEL__CONFIG_TERM_FREQ,
	PERF_EVSEL__CONFIG_TERM_TIME,
	PERF_EVSEL__CONFIG_TERM_CALLGRAPH,
	PERF_EVSEL__CONFIG_TERM_STACK_USER,
	PERF_EVSEL__CONFIG_TERM_INHERIT,
	PERF_EVSEL__CONFIG_TERM_MAX_STACK,
	PERF_EVSEL__CONFIG_TERM_OVERWRITE,
	PERF_EVSEL__CONFIG_TERM_DRV_CFG,
	PERF_EVSEL__CONFIG_TERM_BRANCH,
};

struct perf_evsel_config_term {
	struct list_head	list;
	enum term_type	type;
	union {
		u64	period;
		u64	freq;
		bool	time;
		char	*callgraph;
		char	*drv_cfg;
		u64	stack_user;
		int	max_stack;
		bool	inherit;
		bool	overwrite;
		char	*branch;
	} val;
	bool weak;
};

struct perf_stat_evsel;

/** struct perf_evsel - event selector
 *
 * @evlist - evlist this evsel is in, if it is in one.
 * @node - To insert it into evlist->entries or in other list_heads, say in
 *         the event parsing routines.
=======
#include <sys/types.h>
#include <linux/perf_event.h>
#include <linux/types.h>
#include <internal/evsel.h>
#include <perf/evsel.h>
#include "symbol_conf.h"
#include <internal/cpumap.h>

struct bpf_object;
struct cgroup;
struct perf_counts;
struct perf_stat_evsel;
union perf_event;
struct bpf_counter_ops;
struct target;
struct hashmap;
struct bperf_leader_bpf;
struct bperf_follower_bpf;

typedef int (evsel__sb_cb_t)(union perf_event *event, void *data);

enum perf_tool_event {
	PERF_TOOL_NONE		= 0,
	PERF_TOOL_DURATION_TIME = 1,
};

/** struct evsel - event selector
 *
 * @evlist - evlist this evsel is in, if it is in one.
 * @core - libperf evsel object
>>>>>>> upstream/android-13
 * @name - Can be set to retain the original event name passed by the user,
 *         so that when showing results in tools such as 'perf stat', we
 *         show the name used, not some alias.
 * @id_pos: the position of the event id (PERF_SAMPLE_ID or
 *          PERF_SAMPLE_IDENTIFIER) in a sample event i.e. in the array of
<<<<<<< HEAD
 *          struct sample_event
=======
 *          struct perf_record_sample
>>>>>>> upstream/android-13
 * @is_pos: the position (counting backwards) of the event id (PERF_SAMPLE_ID or
 *          PERF_SAMPLE_IDENTIFIER) in a non-sample event i.e. if sample_id_all
 *          is used there is an id sample appended to non-sample events
 * @priv:   And what is in its containing unnamed union are tool specific
 */
<<<<<<< HEAD
struct perf_evsel {
	struct list_head	node;
	struct perf_evlist	*evlist;
	struct perf_event_attr	attr;
	char			*filter;
	struct xyarray		*fd;
	struct xyarray		*sample_id;
	u64			*id;
	struct perf_counts	*counts;
	struct perf_counts	*prev_raw_counts;
	int			idx;
	u32			ids;
	char			*name;
	double			scale;
	const char		*unit;
	struct event_format	*tp_format;
	off_t			id_offset;
	struct perf_stat_evsel  *stats;
	void			*priv;
	u64			db_id;
	struct cgroup		*cgrp;
	void			*handler;
	struct cpu_map		*cpus;
	struct cpu_map		*own_cpus;
	struct thread_map	*threads;
	unsigned int		sample_size;
	int			id_pos;
	int			is_pos;
	bool			uniquified_name;
	bool			snapshot;
	bool 			supported;
	bool 			needs_swap;
	bool			no_aux_samples;
	bool			immediate;
	bool			system_wide;
	bool			tracking;
	bool			per_pkg;
	bool			precise_max;
	bool			ignore_missing_thread;
	bool			forced_leader;
	bool			use_uncore_alias;
	/* parse modifier helper */
	int			exclude_GH;
	int			nr_members;
	int			sample_read;
	unsigned long		*per_pkg_mask;
	struct perf_evsel	*leader;
	char			*group_name;
	bool			cmdline_group_boundary;
	struct list_head	config_terms;
	int			bpf_fd;
	bool			auto_merge_stats;
	bool			merged_stat;
	const char *		metric_expr;
	const char *		metric_name;
	struct perf_evsel	**metric_events;
	bool			collect_stat;
	bool			weak_group;
	const char		*pmu_name;
};

union u64_swap {
	u64 val64;
	u32 val32[2];
=======
struct evsel {
	struct perf_evsel	core;
	struct evlist		*evlist;
	off_t			id_offset;
	int			id_pos;
	int			is_pos;
	unsigned int		sample_size;

	/*
	 * These fields can be set in the parse-events code or similar.
	 * Please check evsel__clone() to copy them properly so that
	 * they can be released properly.
	 */
	struct {
		char			*name;
		char			*group_name;
		const char		*pmu_name;
		struct tep_event	*tp_format;
		char			*filter;
		unsigned long		max_events;
		double			scale;
		const char		*unit;
		struct cgroup		*cgrp;
		enum perf_tool_event	tool_event;
		/* parse modifier helper */
		int			exclude_GH;
		int			sample_read;
		bool			snapshot;
		bool			per_pkg;
		bool			percore;
		bool			precise_max;
		bool			use_uncore_alias;
		bool			is_libpfm_event;
		bool			auto_merge_stats;
		bool			collect_stat;
		bool			weak_group;
		bool			bpf_counter;
		bool			use_config_name;
		int			bpf_fd;
		struct bpf_object	*bpf_obj;
		struct list_head	config_terms;
	};

	/*
	 * metric fields are similar, but needs more care as they can have
	 * references to other metric (evsel).
	 */
	const char *		metric_expr;
	const char *		metric_name;
	struct evsel		**metric_events;
	struct evsel		*metric_leader;

	void			*handler;
	struct perf_counts	*counts;
	struct perf_counts	*prev_raw_counts;
	unsigned long		nr_events_printed;
	struct perf_stat_evsel  *stats;
	void			*priv;
	u64			db_id;
	bool			uniquified_name;
	bool 			supported;
	bool 			needs_swap;
	bool 			disabled;
	bool			no_aux_samples;
	bool			immediate;
	bool			tracking;
	bool			ignore_missing_thread;
	bool			forced_leader;
	bool			cmdline_group_boundary;
	bool			merged_stat;
	bool			reset_group;
	bool			errored;
	struct hashmap		*per_pkg_mask;
	int			err;
	int			cpu_iter;
	struct {
		evsel__sb_cb_t	*cb;
		void		*data;
	} side_band;
	/*
	 * For reporting purposes, an evsel sample can have a callchain
	 * synthesized from AUX area data. Keep track of synthesized sample
	 * types here. Note, the recorded sample_type cannot be changed because
	 * it is needed to continue to parse events.
	 * See also evsel__has_callchain().
	 */
	__u64			synth_sample_type;

	/*
	 * bpf_counter_ops serves two use cases:
	 *   1. perf-stat -b          counting events used byBPF programs
	 *   2. perf-stat --use-bpf   use BPF programs to aggregate counts
	 */
	struct bpf_counter_ops	*bpf_counter_ops;

	/* for perf-stat -b */
	struct list_head	bpf_counter_list;

	/* for perf-stat --use-bpf */
	int			bperf_leader_prog_fd;
	int			bperf_leader_link_fd;
	union {
		struct bperf_leader_bpf *leader_skel;
		struct bperf_follower_bpf *follower_skel;
	};
	unsigned long		open_flags;
	int			precise_ip_original;
>>>>>>> upstream/android-13
};

struct perf_missing_features {
	bool sample_id_all;
	bool exclude_guest;
	bool mmap2;
	bool cloexec;
	bool clockid;
	bool clockid_wrong;
	bool lbr_flags;
	bool write_backward;
	bool group_read;
<<<<<<< HEAD
=======
	bool ksymbol;
	bool bpf;
	bool aux_output;
	bool branch_hw_idx;
	bool cgroup;
	bool data_page_size;
	bool code_page_size;
	bool weight_struct;
>>>>>>> upstream/android-13
};

extern struct perf_missing_features perf_missing_features;

<<<<<<< HEAD
struct cpu_map;
struct target;
struct thread_map;
struct record_opts;

static inline struct cpu_map *perf_evsel__cpus(struct perf_evsel *evsel)
{
	return evsel->cpus;
}

static inline int perf_evsel__nr_cpus(struct perf_evsel *evsel)
{
	return perf_evsel__cpus(evsel)->nr;
=======
struct perf_cpu_map;
struct thread_map;
struct record_opts;

static inline struct perf_cpu_map *evsel__cpus(struct evsel *evsel)
{
	return perf_evsel__cpus(&evsel->core);
}

static inline int evsel__nr_cpus(struct evsel *evsel)
{
	return evsel__cpus(evsel)->nr;
>>>>>>> upstream/android-13
}

void perf_counts_values__scale(struct perf_counts_values *count,
			       bool scale, s8 *pscaled);

<<<<<<< HEAD
void perf_evsel__compute_deltas(struct perf_evsel *evsel, int cpu, int thread,
				struct perf_counts_values *count);

int perf_evsel__object_config(size_t object_size,
			      int (*init)(struct perf_evsel *evsel),
			      void (*fini)(struct perf_evsel *evsel));

struct perf_evsel *perf_evsel__new_idx(struct perf_event_attr *attr, int idx);

static inline struct perf_evsel *perf_evsel__new(struct perf_event_attr *attr)
{
	return perf_evsel__new_idx(attr, 0);
}

struct perf_evsel *perf_evsel__newtp_idx(const char *sys, const char *name, int idx);
=======
void evsel__compute_deltas(struct evsel *evsel, int cpu, int thread,
			   struct perf_counts_values *count);

int evsel__object_config(size_t object_size,
			 int (*init)(struct evsel *evsel),
			 void (*fini)(struct evsel *evsel));

struct perf_pmu *evsel__find_pmu(struct evsel *evsel);
bool evsel__is_aux_event(struct evsel *evsel);

struct evsel *evsel__new_idx(struct perf_event_attr *attr, int idx);

static inline struct evsel *evsel__new(struct perf_event_attr *attr)
{
	return evsel__new_idx(attr, 0);
}

struct evsel *evsel__clone(struct evsel *orig);
struct evsel *evsel__newtp_idx(const char *sys, const char *name, int idx);

int copy_config_terms(struct list_head *dst, struct list_head *src);
void free_config_terms(struct list_head *config_terms);
>>>>>>> upstream/android-13

/*
 * Returns pointer with encoded error via <linux/err.h> interface.
 */
<<<<<<< HEAD
static inline struct perf_evsel *perf_evsel__newtp(const char *sys, const char *name)
{
	return perf_evsel__newtp_idx(sys, name, 0);
}

struct perf_evsel *perf_evsel__new_cycles(bool precise);

struct event_format *event_format__new(const char *sys, const char *name);

void perf_evsel__init(struct perf_evsel *evsel,
		      struct perf_event_attr *attr, int idx);
void perf_evsel__exit(struct perf_evsel *evsel);
void perf_evsel__delete(struct perf_evsel *evsel);

struct callchain_param;

void perf_evsel__config(struct perf_evsel *evsel,
			struct record_opts *opts,
			struct callchain_param *callchain);
void perf_evsel__config_callchain(struct perf_evsel *evsel,
				  struct record_opts *opts,
				  struct callchain_param *callchain);

int __perf_evsel__sample_size(u64 sample_type);
void perf_evsel__calc_id_pos(struct perf_evsel *evsel);

bool perf_evsel__is_cache_op_valid(u8 type, u8 op);

#define PERF_EVSEL__MAX_ALIASES 8

extern const char *perf_evsel__hw_cache[PERF_COUNT_HW_CACHE_MAX]
				       [PERF_EVSEL__MAX_ALIASES];
extern const char *perf_evsel__hw_cache_op[PERF_COUNT_HW_CACHE_OP_MAX]
					  [PERF_EVSEL__MAX_ALIASES];
extern const char *perf_evsel__hw_cache_result[PERF_COUNT_HW_CACHE_RESULT_MAX]
					      [PERF_EVSEL__MAX_ALIASES];
extern const char *perf_evsel__hw_names[PERF_COUNT_HW_MAX];
extern const char *perf_evsel__sw_names[PERF_COUNT_SW_MAX];
int __perf_evsel__hw_cache_type_op_res_name(u8 type, u8 op, u8 result,
					    char *bf, size_t size);
const char *perf_evsel__name(struct perf_evsel *evsel);

const char *perf_evsel__group_name(struct perf_evsel *evsel);
int perf_evsel__group_desc(struct perf_evsel *evsel, char *buf, size_t size);

int perf_evsel__alloc_id(struct perf_evsel *evsel, int ncpus, int nthreads);
void perf_evsel__close_fd(struct perf_evsel *evsel);

void __perf_evsel__set_sample_bit(struct perf_evsel *evsel,
				  enum perf_event_sample_format bit);
void __perf_evsel__reset_sample_bit(struct perf_evsel *evsel,
				    enum perf_event_sample_format bit);

#define perf_evsel__set_sample_bit(evsel, bit) \
	__perf_evsel__set_sample_bit(evsel, PERF_SAMPLE_##bit)

#define perf_evsel__reset_sample_bit(evsel, bit) \
	__perf_evsel__reset_sample_bit(evsel, PERF_SAMPLE_##bit)

void perf_evsel__set_sample_id(struct perf_evsel *evsel,
			       bool use_sample_identifier);

int perf_evsel__set_filter(struct perf_evsel *evsel, const char *filter);
int perf_evsel__append_tp_filter(struct perf_evsel *evsel, const char *filter);
int perf_evsel__append_addr_filter(struct perf_evsel *evsel,
				   const char *filter);
int perf_evsel__apply_filter(struct perf_evsel *evsel, const char *filter);
int perf_evsel__enable(struct perf_evsel *evsel);
int perf_evsel__disable(struct perf_evsel *evsel);

int perf_evsel__open_per_cpu(struct perf_evsel *evsel,
			     struct cpu_map *cpus);
int perf_evsel__open_per_thread(struct perf_evsel *evsel,
				struct thread_map *threads);
int perf_evsel__open(struct perf_evsel *evsel, struct cpu_map *cpus,
		     struct thread_map *threads);
void perf_evsel__close(struct perf_evsel *evsel);

struct perf_sample;

void *perf_evsel__rawptr(struct perf_evsel *evsel, struct perf_sample *sample,
			 const char *name);
u64 perf_evsel__intval(struct perf_evsel *evsel, struct perf_sample *sample,
		       const char *name);

static inline char *perf_evsel__strval(struct perf_evsel *evsel,
				       struct perf_sample *sample,
				       const char *name)
{
	return perf_evsel__rawptr(evsel, sample, name);
}

struct format_field;

u64 format_field__intval(struct format_field *field, struct perf_sample *sample, bool needs_swap);

struct format_field *perf_evsel__field(struct perf_evsel *evsel, const char *name);

#define perf_evsel__match(evsel, t, c)		\
	(evsel->attr.type == PERF_TYPE_##t &&	\
	 evsel->attr.config == PERF_COUNT_##c)

static inline bool perf_evsel__match2(struct perf_evsel *e1,
				      struct perf_evsel *e2)
{
	return (e1->attr.type == e2->attr.type) &&
	       (e1->attr.config == e2->attr.config);
}

#define perf_evsel__cmp(a, b)			\
	((a) &&					\
	 (b) &&					\
	 (a)->attr.type == (b)->attr.type &&	\
	 (a)->attr.config == (b)->attr.config)

int perf_evsel__read(struct perf_evsel *evsel, int cpu, int thread,
		     struct perf_counts_values *count);

int perf_evsel__read_counter(struct perf_evsel *evsel, int cpu, int thread);

int __perf_evsel__read_on_cpu(struct perf_evsel *evsel,
			      int cpu, int thread, bool scale);

/**
 * perf_evsel__read_on_cpu - Read out the results on a CPU and thread
=======
static inline struct evsel *evsel__newtp(const char *sys, const char *name)
{
	return evsel__newtp_idx(sys, name, 0);
}

struct evsel *evsel__new_cycles(bool precise, __u32 type, __u64 config);

struct tep_event *event_format__new(const char *sys, const char *name);

void evsel__init(struct evsel *evsel, struct perf_event_attr *attr, int idx);
void evsel__exit(struct evsel *evsel);
void evsel__delete(struct evsel *evsel);

struct callchain_param;

void evsel__config(struct evsel *evsel, struct record_opts *opts,
		   struct callchain_param *callchain);
void evsel__config_callchain(struct evsel *evsel, struct record_opts *opts,
			     struct callchain_param *callchain);

int __evsel__sample_size(u64 sample_type);
void evsel__calc_id_pos(struct evsel *evsel);

bool evsel__is_cache_op_valid(u8 type, u8 op);

static inline bool evsel__is_bpf(struct evsel *evsel)
{
	return evsel->bpf_counter_ops != NULL;
}

#define EVSEL__MAX_ALIASES 8

extern const char *evsel__hw_cache[PERF_COUNT_HW_CACHE_MAX][EVSEL__MAX_ALIASES];
extern const char *evsel__hw_cache_op[PERF_COUNT_HW_CACHE_OP_MAX][EVSEL__MAX_ALIASES];
extern const char *evsel__hw_cache_result[PERF_COUNT_HW_CACHE_RESULT_MAX][EVSEL__MAX_ALIASES];
extern const char *evsel__hw_names[PERF_COUNT_HW_MAX];
extern const char *evsel__sw_names[PERF_COUNT_SW_MAX];
extern char *evsel__bpf_counter_events;
bool evsel__match_bpf_counter_events(const char *name);

int __evsel__hw_cache_type_op_res_name(u8 type, u8 op, u8 result, char *bf, size_t size);
const char *evsel__name(struct evsel *evsel);

const char *evsel__group_name(struct evsel *evsel);
int evsel__group_desc(struct evsel *evsel, char *buf, size_t size);

void __evsel__set_sample_bit(struct evsel *evsel, enum perf_event_sample_format bit);
void __evsel__reset_sample_bit(struct evsel *evsel, enum perf_event_sample_format bit);

#define evsel__set_sample_bit(evsel, bit) \
	__evsel__set_sample_bit(evsel, PERF_SAMPLE_##bit)

#define evsel__reset_sample_bit(evsel, bit) \
	__evsel__reset_sample_bit(evsel, PERF_SAMPLE_##bit)

void evsel__set_sample_id(struct evsel *evsel, bool use_sample_identifier);

void arch_evsel__set_sample_weight(struct evsel *evsel);

int evsel__set_filter(struct evsel *evsel, const char *filter);
int evsel__append_tp_filter(struct evsel *evsel, const char *filter);
int evsel__append_addr_filter(struct evsel *evsel, const char *filter);
int evsel__enable_cpu(struct evsel *evsel, int cpu);
int evsel__enable(struct evsel *evsel);
int evsel__disable(struct evsel *evsel);
int evsel__disable_cpu(struct evsel *evsel, int cpu);

int evsel__open_per_cpu(struct evsel *evsel, struct perf_cpu_map *cpus, int cpu);
int evsel__open_per_thread(struct evsel *evsel, struct perf_thread_map *threads);
int evsel__open(struct evsel *evsel, struct perf_cpu_map *cpus,
		struct perf_thread_map *threads);
void evsel__close(struct evsel *evsel);
int evsel__prepare_open(struct evsel *evsel, struct perf_cpu_map *cpus,
		struct perf_thread_map *threads);
bool evsel__detect_missing_features(struct evsel *evsel);

enum rlimit_action { NO_CHANGE, SET_TO_MAX, INCREASED_MAX };
bool evsel__increase_rlimit(enum rlimit_action *set_rlimit);

bool evsel__ignore_missing_thread(struct evsel *evsel,
				  int nr_cpus, int cpu,
				  struct perf_thread_map *threads,
				  int thread, int err);
bool evsel__precise_ip_fallback(struct evsel *evsel);

struct perf_sample;

void *evsel__rawptr(struct evsel *evsel, struct perf_sample *sample, const char *name);
u64 evsel__intval(struct evsel *evsel, struct perf_sample *sample, const char *name);

static inline char *evsel__strval(struct evsel *evsel, struct perf_sample *sample, const char *name)
{
	return evsel__rawptr(evsel, sample, name);
}

struct tep_format_field;

u64 format_field__intval(struct tep_format_field *field, struct perf_sample *sample, bool needs_swap);

struct tep_format_field *evsel__field(struct evsel *evsel, const char *name);

#define evsel__match(evsel, t, c)		\
	(evsel->core.attr.type == PERF_TYPE_##t &&	\
	 evsel->core.attr.config == PERF_COUNT_##c)

static inline bool evsel__match2(struct evsel *e1, struct evsel *e2)
{
	return (e1->core.attr.type == e2->core.attr.type) &&
	       (e1->core.attr.config == e2->core.attr.config);
}

int evsel__read_counter(struct evsel *evsel, int cpu, int thread);

int __evsel__read_on_cpu(struct evsel *evsel, int cpu, int thread, bool scale);

/**
 * evsel__read_on_cpu - Read out the results on a CPU and thread
>>>>>>> upstream/android-13
 *
 * @evsel - event selector to read value
 * @cpu - CPU of interest
 * @thread - thread of interest
 */
<<<<<<< HEAD
static inline int perf_evsel__read_on_cpu(struct perf_evsel *evsel,
					  int cpu, int thread)
{
	return __perf_evsel__read_on_cpu(evsel, cpu, thread, false);
}

/**
 * perf_evsel__read_on_cpu_scaled - Read out the results on a CPU and thread, scaled
=======
static inline int evsel__read_on_cpu(struct evsel *evsel, int cpu, int thread)
{
	return __evsel__read_on_cpu(evsel, cpu, thread, false);
}

/**
 * evsel__read_on_cpu_scaled - Read out the results on a CPU and thread, scaled
>>>>>>> upstream/android-13
 *
 * @evsel - event selector to read value
 * @cpu - CPU of interest
 * @thread - thread of interest
 */
<<<<<<< HEAD
static inline int perf_evsel__read_on_cpu_scaled(struct perf_evsel *evsel,
						 int cpu, int thread)
{
	return __perf_evsel__read_on_cpu(evsel, cpu, thread, true);
}

int perf_evsel__parse_sample(struct perf_evsel *evsel, union perf_event *event,
			     struct perf_sample *sample);

int perf_evsel__parse_sample_timestamp(struct perf_evsel *evsel,
				       union perf_event *event,
				       u64 *timestamp);

static inline struct perf_evsel *perf_evsel__next(struct perf_evsel *evsel)
{
	return list_entry(evsel->node.next, struct perf_evsel, node);
}

static inline struct perf_evsel *perf_evsel__prev(struct perf_evsel *evsel)
{
	return list_entry(evsel->node.prev, struct perf_evsel, node);
}

/**
 * perf_evsel__is_group_leader - Return whether given evsel is a leader event
=======
static inline int evsel__read_on_cpu_scaled(struct evsel *evsel, int cpu, int thread)
{
	return __evsel__read_on_cpu(evsel, cpu, thread, true);
}

int evsel__parse_sample(struct evsel *evsel, union perf_event *event,
			struct perf_sample *sample);

int evsel__parse_sample_timestamp(struct evsel *evsel, union perf_event *event,
				  u64 *timestamp);

static inline struct evsel *evsel__next(struct evsel *evsel)
{
	return list_entry(evsel->core.node.next, struct evsel, core.node);
}

static inline struct evsel *evsel__prev(struct evsel *evsel)
{
	return list_entry(evsel->core.node.prev, struct evsel, core.node);
}

/**
 * evsel__is_group_leader - Return whether given evsel is a leader event
>>>>>>> upstream/android-13
 *
 * @evsel - evsel selector to be tested
 *
 * Return %true if @evsel is a group leader or a stand-alone event
 */
<<<<<<< HEAD
static inline bool perf_evsel__is_group_leader(const struct perf_evsel *evsel)
{
	return evsel->leader == evsel;
}

/**
 * perf_evsel__is_group_event - Return whether given evsel is a group event
=======
static inline bool evsel__is_group_leader(const struct evsel *evsel)
{
	return evsel->core.leader == &evsel->core;
}

/**
 * evsel__is_group_event - Return whether given evsel is a group event
>>>>>>> upstream/android-13
 *
 * @evsel - evsel selector to be tested
 *
 * Return %true iff event group view is enabled and @evsel is a actual group
 * leader which has other members in the group
 */
<<<<<<< HEAD
static inline bool perf_evsel__is_group_event(struct perf_evsel *evsel)
=======
static inline bool evsel__is_group_event(struct evsel *evsel)
>>>>>>> upstream/android-13
{
	if (!symbol_conf.event_group)
		return false;

<<<<<<< HEAD
	return perf_evsel__is_group_leader(evsel) && evsel->nr_members > 1;
}

bool perf_evsel__is_function_event(struct perf_evsel *evsel);

static inline bool perf_evsel__is_bpf_output(struct perf_evsel *evsel)
{
	return perf_evsel__match(evsel, SOFTWARE, SW_BPF_OUTPUT);
}

static inline bool perf_evsel__is_clock(struct perf_evsel *evsel)
{
	return perf_evsel__match(evsel, SOFTWARE, SW_CPU_CLOCK) ||
	       perf_evsel__match(evsel, SOFTWARE, SW_TASK_CLOCK);
}

struct perf_attr_details {
	bool freq;
	bool verbose;
	bool event_group;
	bool force;
	bool trace_fields;
};

int perf_evsel__fprintf(struct perf_evsel *evsel,
			struct perf_attr_details *details, FILE *fp);

#define EVSEL__PRINT_IP			(1<<0)
#define EVSEL__PRINT_SYM		(1<<1)
#define EVSEL__PRINT_DSO		(1<<2)
#define EVSEL__PRINT_SYMOFFSET		(1<<3)
#define EVSEL__PRINT_ONELINE		(1<<4)
#define EVSEL__PRINT_SRCLINE		(1<<5)
#define EVSEL__PRINT_UNKNOWN_AS_ADDR	(1<<6)
#define EVSEL__PRINT_CALLCHAIN_ARROW	(1<<7)
#define EVSEL__PRINT_SKIP_IGNORED	(1<<8)

struct callchain_cursor;

int sample__fprintf_callchain(struct perf_sample *sample, int left_alignment,
			      unsigned int print_opts,
			      struct callchain_cursor *cursor, FILE *fp);

int sample__fprintf_sym(struct perf_sample *sample, struct addr_location *al,
			int left_alignment, unsigned int print_opts,
			struct callchain_cursor *cursor, FILE *fp);

bool perf_evsel__fallback(struct perf_evsel *evsel, int err,
			  char *msg, size_t msgsize);
int perf_evsel__open_strerror(struct perf_evsel *evsel, struct target *target,
			      int err, char *msg, size_t size);

static inline int perf_evsel__group_idx(struct perf_evsel *evsel)
{
	return evsel->idx - evsel->leader->idx;
=======
	return evsel__is_group_leader(evsel) && evsel->core.nr_members > 1;
}

bool evsel__is_function_event(struct evsel *evsel);

static inline bool evsel__is_bpf_output(struct evsel *evsel)
{
	return evsel__match(evsel, SOFTWARE, SW_BPF_OUTPUT);
}

static inline bool evsel__is_clock(struct evsel *evsel)
{
	return evsel__match(evsel, SOFTWARE, SW_CPU_CLOCK) ||
	       evsel__match(evsel, SOFTWARE, SW_TASK_CLOCK);
}

bool evsel__fallback(struct evsel *evsel, int err, char *msg, size_t msgsize);
int evsel__open_strerror(struct evsel *evsel, struct target *target,
			 int err, char *msg, size_t size);

static inline int evsel__group_idx(struct evsel *evsel)
{
	return evsel->core.idx - evsel->core.leader->idx;
>>>>>>> upstream/android-13
}

/* Iterates group WITHOUT the leader. */
#define for_each_group_member(_evsel, _leader) 					\
<<<<<<< HEAD
for ((_evsel) = list_entry((_leader)->node.next, struct perf_evsel, node); 	\
     (_evsel) && (_evsel)->leader == (_leader);					\
     (_evsel) = list_entry((_evsel)->node.next, struct perf_evsel, node))
=======
for ((_evsel) = list_entry((_leader)->core.node.next, struct evsel, core.node); \
     (_evsel) && (_evsel)->core.leader == (&_leader->core);					\
     (_evsel) = list_entry((_evsel)->core.node.next, struct evsel, core.node))
>>>>>>> upstream/android-13

/* Iterates group WITH the leader. */
#define for_each_group_evsel(_evsel, _leader) 					\
for ((_evsel) = _leader; 							\
<<<<<<< HEAD
     (_evsel) && (_evsel)->leader == (_leader);					\
     (_evsel) = list_entry((_evsel)->node.next, struct perf_evsel, node))

static inline bool perf_evsel__has_branch_callstack(const struct perf_evsel *evsel)
{
	return evsel->attr.branch_sample_type & PERF_SAMPLE_BRANCH_CALL_STACK;
}

static inline bool evsel__has_callchain(const struct perf_evsel *evsel)
{
	return (evsel->attr.sample_type & PERF_SAMPLE_CALLCHAIN) != 0;
}

typedef int (*attr__fprintf_f)(FILE *, const char *, const char *, void *);

int perf_event_attr__fprintf(FILE *fp, struct perf_event_attr *attr,
			     attr__fprintf_f attr__fprintf, void *priv);

struct perf_env *perf_evsel__env(struct perf_evsel *evsel);

=======
     (_evsel) && (_evsel)->core.leader == (&_leader->core);					\
     (_evsel) = list_entry((_evsel)->core.node.next, struct evsel, core.node))

static inline bool evsel__has_branch_callstack(const struct evsel *evsel)
{
	return evsel->core.attr.branch_sample_type & PERF_SAMPLE_BRANCH_CALL_STACK;
}

static inline bool evsel__has_branch_hw_idx(const struct evsel *evsel)
{
	return evsel->core.attr.branch_sample_type & PERF_SAMPLE_BRANCH_HW_INDEX;
}

static inline bool evsel__has_callchain(const struct evsel *evsel)
{
	/*
	 * For reporting purposes, an evsel sample can have a recorded callchain
	 * or a callchain synthesized from AUX area data.
	 */
	return evsel->core.attr.sample_type & PERF_SAMPLE_CALLCHAIN ||
	       evsel->synth_sample_type & PERF_SAMPLE_CALLCHAIN;
}

static inline bool evsel__has_br_stack(const struct evsel *evsel)
{
	/*
	 * For reporting purposes, an evsel sample can have a recorded branch
	 * stack or a branch stack synthesized from AUX area data.
	 */
	return evsel->core.attr.sample_type & PERF_SAMPLE_BRANCH_STACK ||
	       evsel->synth_sample_type & PERF_SAMPLE_BRANCH_STACK;
}

static inline bool evsel__is_dummy_event(struct evsel *evsel)
{
	return (evsel->core.attr.type == PERF_TYPE_SOFTWARE) &&
	       (evsel->core.attr.config == PERF_COUNT_SW_DUMMY);
}

struct perf_env *evsel__env(struct evsel *evsel);

int evsel__store_ids(struct evsel *evsel, struct evlist *evlist);

void evsel__zero_per_pkg(struct evsel *evsel);
bool evsel__is_hybrid(struct evsel *evsel);
struct evsel *evsel__leader(struct evsel *evsel);
bool evsel__has_leader(struct evsel *evsel, struct evsel *leader);
bool evsel__is_leader(struct evsel *evsel);
void evsel__set_leader(struct evsel *evsel, struct evsel *leader);
>>>>>>> upstream/android-13
#endif /* __PERF_EVSEL_H */
