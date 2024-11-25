<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * builtin-stat.c
 *
 * Builtin stat command: Give a precise performance counters summary
 * overview about any workload, CPU or specific PID.
 *
 * Sample output:

   $ perf stat ./hackbench 10

  Time: 0.118

  Performance counter stats for './hackbench 10':

       1708.761321 task-clock                #   11.037 CPUs utilized
            41,190 context-switches          #    0.024 M/sec
             6,735 CPU-migrations            #    0.004 M/sec
            17,318 page-faults               #    0.010 M/sec
     5,205,202,243 cycles                    #    3.046 GHz
     3,856,436,920 stalled-cycles-frontend   #   74.09% frontend cycles idle
     1,600,790,871 stalled-cycles-backend    #   30.75% backend  cycles idle
     2,603,501,247 instructions              #    0.50  insns per cycle
                                             #    1.48  stalled cycles per insn
       484,357,498 branches                  #  283.455 M/sec
         6,388,934 branch-misses             #    1.32% of all branches

        0.154822978  seconds time elapsed

 *
 * Copyright (C) 2008-2011, Red Hat Inc, Ingo Molnar <mingo@redhat.com>
 *
 * Improvements and fixes by:
 *
 *   Arjan van de Ven <arjan@linux.intel.com>
 *   Yanmin Zhang <yanmin.zhang@intel.com>
 *   Wu Fengguang <fengguang.wu@intel.com>
 *   Mike Galbraith <efault@gmx.de>
 *   Paul Mackerras <paulus@samba.org>
 *   Jaswinder Singh Rajput <jaswinder@kernel.org>
<<<<<<< HEAD
 *
 * Released under the GPL v2. (and only v2, not any later version)
 */

#include "perf.h"
#include "builtin.h"
#include "util/cgroup.h"
#include "util/util.h"
=======
 */

#include "builtin.h"
#include "perf.h"
#include "util/cgroup.h"
>>>>>>> upstream/android-13
#include <subcmd/parse-options.h>
#include "util/parse-events.h"
#include "util/pmu.h"
#include "util/event.h"
#include "util/evlist.h"
<<<<<<< HEAD
#include "util/evsel.h"
#include "util/debug.h"
#include "util/drv_configs.h"
=======
#include "util/evlist-hybrid.h"
#include "util/evsel.h"
#include "util/debug.h"
>>>>>>> upstream/android-13
#include "util/color.h"
#include "util/stat.h"
#include "util/header.h"
#include "util/cpumap.h"
<<<<<<< HEAD
#include "util/thread.h"
#include "util/thread_map.h"
#include "util/counts.h"
#include "util/group.h"
=======
#include "util/thread_map.h"
#include "util/counts.h"
#include "util/topdown.h"
>>>>>>> upstream/android-13
#include "util/session.h"
#include "util/tool.h"
#include "util/string2.h"
#include "util/metricgroup.h"
<<<<<<< HEAD
#include "util/top.h"
#include "asm/bug.h"

#include <linux/time64.h>
=======
#include "util/synthetic-events.h"
#include "util/target.h"
#include "util/time-utils.h"
#include "util/top.h"
#include "util/affinity.h"
#include "util/pfm.h"
#include "util/bpf_counter.h"
#include "util/iostat.h"
#include "util/pmu-hybrid.h"
#include "asm/bug.h"

#include <linux/time64.h>
#include <linux/zalloc.h>
>>>>>>> upstream/android-13
#include <api/fs/fs.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <inttypes.h>
#include <locale.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
<<<<<<< HEAD
#include <sys/wait.h>

#include "sane_ctype.h"

#define DEFAULT_SEPARATOR	" "
#define CNTR_NOT_SUPPORTED	"<not supported>"
#define CNTR_NOT_COUNTED	"<not counted>"
=======
#include <linux/err.h>

#include <linux/ctype.h>
#include <perf/evlist.h>

#define DEFAULT_SEPARATOR	" "
>>>>>>> upstream/android-13
#define FREEZE_ON_SMI_PATH	"devices/cpu/freeze_on_smi"

static void print_counters(struct timespec *ts, int argc, const char **argv);

/* Default events used for perf stat -T */
static const char *transaction_attrs = {
	"task-clock,"
	"{"
	"instructions,"
	"cycles,"
	"cpu/cycles-t/,"
	"cpu/tx-start/,"
	"cpu/el-start/,"
	"cpu/cycles-ct/"
	"}"
};

/* More limited version when the CPU does not have all events. */
static const char * transaction_limited_attrs = {
	"task-clock,"
	"{"
	"instructions,"
	"cycles,"
	"cpu/cycles-t/,"
	"cpu/tx-start/"
	"}"
};

static const char * topdown_attrs[] = {
	"topdown-total-slots",
	"topdown-slots-retired",
	"topdown-recovery-bubbles",
	"topdown-fetch-bubbles",
	"topdown-slots-issued",
	NULL,
};

<<<<<<< HEAD
=======
static const char *topdown_metric_attrs[] = {
	"slots",
	"topdown-retiring",
	"topdown-bad-spec",
	"topdown-fe-bound",
	"topdown-be-bound",
	NULL,
};

static const char *topdown_metric_L2_attrs[] = {
	"slots",
	"topdown-retiring",
	"topdown-bad-spec",
	"topdown-fe-bound",
	"topdown-be-bound",
	"topdown-heavy-ops",
	"topdown-br-mispredict",
	"topdown-fetch-lat",
	"topdown-mem-bound",
	NULL,
};

#define TOPDOWN_MAX_LEVEL			2

>>>>>>> upstream/android-13
static const char *smi_cost_attrs = {
	"{"
	"msr/aperf/,"
	"msr/smi/,"
	"cycles"
	"}"
};

<<<<<<< HEAD
static struct perf_evlist	*evsel_list;

static struct rblist		 metric_events;
=======
static struct evlist	*evsel_list;
static bool all_counters_use_bpf = true;
>>>>>>> upstream/android-13

static struct target target = {
	.uid	= UINT_MAX,
};

<<<<<<< HEAD
typedef int (*aggr_get_id_t)(struct cpu_map *m, int cpu);

#define METRIC_ONLY_LEN 20

static int			run_count			=  1;
static bool			no_inherit			= false;
static volatile pid_t		child_pid			= -1;
static bool			null_run			=  false;
=======
#define METRIC_ONLY_LEN 20

static volatile pid_t		child_pid			= -1;
>>>>>>> upstream/android-13
static int			detailed_run			=  0;
static bool			transaction_run;
static bool			topdown_run			= false;
static bool			smi_cost			= false;
static bool			smi_reset			= false;
<<<<<<< HEAD
static bool			big_num				=  true;
static int			big_num_opt			=  -1;
static const char		*csv_sep			= NULL;
static bool			csv_output			= false;
=======
static int			big_num_opt			=  -1;
>>>>>>> upstream/android-13
static bool			group				= false;
static const char		*pre_cmd			= NULL;
static const char		*post_cmd			= NULL;
static bool			sync_run			= false;
<<<<<<< HEAD
static unsigned int		initial_delay			= 0;
static unsigned int		unit_width			= 4; /* strlen("unit") */
static bool			forever				= false;
static bool			metric_only			= false;
static bool			force_metric_only		= false;
static bool			no_merge			= false;
static bool			walltime_run_table		= false;
static struct timespec		ref_time;
static struct cpu_map		*aggr_map;
static aggr_get_id_t		aggr_get_id;
static bool			append_file;
static bool			interval_count;
static bool			interval_clear;
static const char		*output_name;
static int			output_fd;
static int			print_free_counters_hint;
static int			print_mixed_hw_group_error;
static u64			*walltime_run;
static bool			ru_display			= false;
static struct rusage		ru_data;
static unsigned int		metric_only_len			= METRIC_ONLY_LEN;
=======
static bool			forever				= false;
static bool			force_metric_only		= false;
static struct timespec		ref_time;
static bool			append_file;
static bool			interval_count;
static const char		*output_name;
static int			output_fd;
>>>>>>> upstream/android-13

struct perf_stat {
	bool			 record;
	struct perf_data	 data;
	struct perf_session	*session;
	u64			 bytes_written;
	struct perf_tool	 tool;
	bool			 maps_allocated;
<<<<<<< HEAD
	struct cpu_map		*cpus;
	struct thread_map	*threads;
=======
	struct perf_cpu_map	*cpus;
	struct perf_thread_map *threads;
>>>>>>> upstream/android-13
	enum aggr_mode		 aggr_mode;
};

static struct perf_stat		perf_stat;
#define STAT_RECORD		perf_stat.record

static volatile int done = 0;

static struct perf_stat_config stat_config = {
<<<<<<< HEAD
	.aggr_mode	= AGGR_GLOBAL,
	.scale		= true,
};

static bool is_duration_time(struct perf_evsel *evsel)
{
	return !strcmp(evsel->name, "duration_time");
=======
	.aggr_mode		= AGGR_GLOBAL,
	.scale			= true,
	.unit_width		= 4, /* strlen("unit") */
	.run_count		= 1,
	.metric_only_len	= METRIC_ONLY_LEN,
	.walltime_nsecs_stats	= &walltime_nsecs_stats,
	.big_num		= true,
	.ctl_fd			= -1,
	.ctl_fd_ack		= -1,
	.iostat_run		= false,
};

static bool cpus_map_matched(struct evsel *a, struct evsel *b)
{
	if (!a->core.cpus && !b->core.cpus)
		return true;

	if (!a->core.cpus || !b->core.cpus)
		return false;

	if (a->core.cpus->nr != b->core.cpus->nr)
		return false;

	for (int i = 0; i < a->core.cpus->nr; i++) {
		if (a->core.cpus->map[i] != b->core.cpus->map[i])
			return false;
	}

	return true;
}

static void evlist__check_cpu_maps(struct evlist *evlist)
{
	struct evsel *evsel, *pos, *leader;
	char buf[1024];

	if (evlist__has_hybrid(evlist))
		evlist__warn_hybrid_group(evlist);

	evlist__for_each_entry(evlist, evsel) {
		leader = evsel__leader(evsel);

		/* Check that leader matches cpus with each member. */
		if (leader == evsel)
			continue;
		if (cpus_map_matched(leader, evsel))
			continue;

		/* If there's mismatch disable the group and warn user. */
		WARN_ONCE(1, "WARNING: grouped events cpus do not match, disabling group:\n");
		evsel__group_desc(leader, buf, sizeof(buf));
		pr_warning("  %s\n", buf);

		if (verbose) {
			cpu_map__snprint(leader->core.cpus, buf, sizeof(buf));
			pr_warning("     %s: %s\n", leader->name, buf);
			cpu_map__snprint(evsel->core.cpus, buf, sizeof(buf));
			pr_warning("     %s: %s\n", evsel->name, buf);
		}

		for_each_group_evsel(pos, leader) {
			evsel__set_leader(pos, pos);
			pos->core.nr_members = 0;
		}
		evsel->core.leader->nr_members = 0;
	}
>>>>>>> upstream/android-13
}

static inline void diff_timespec(struct timespec *r, struct timespec *a,
				 struct timespec *b)
{
	r->tv_sec = a->tv_sec - b->tv_sec;
	if (a->tv_nsec < b->tv_nsec) {
		r->tv_nsec = a->tv_nsec + NSEC_PER_SEC - b->tv_nsec;
		r->tv_sec--;
	} else {
		r->tv_nsec = a->tv_nsec - b->tv_nsec ;
	}
}

static void perf_stat__reset_stats(void)
{
	int i;

<<<<<<< HEAD
	perf_evlist__reset_stats(evsel_list);
=======
	evlist__reset_stats(evsel_list);
>>>>>>> upstream/android-13
	perf_stat__reset_shadow_stats();

	for (i = 0; i < stat_config.stats_num; i++)
		perf_stat__reset_shadow_per_stat(&stat_config.stats[i]);
}

<<<<<<< HEAD
static int create_perf_stat_counter(struct perf_evsel *evsel)
{
	struct perf_event_attr *attr = &evsel->attr;
	struct perf_evsel *leader = evsel->leader;

	if (stat_config.scale) {
		attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED |
				    PERF_FORMAT_TOTAL_TIME_RUNNING;
	}

	/*
	 * The event is part of non trivial group, let's enable
	 * the group read (for leader) and ID retrieval for all
	 * members.
	 */
	if (leader->nr_members > 1)
		attr->read_format |= PERF_FORMAT_ID|PERF_FORMAT_GROUP;

	attr->inherit = !no_inherit;

	/*
	 * Some events get initialized with sample_(period/type) set,
	 * like tracepoints. Clear it up for counting.
	 */
	attr->sample_period = 0;

	/*
	 * But set sample_type to PERF_SAMPLE_IDENTIFIER, which should be harmless
	 * while avoiding that older tools show confusing messages.
	 *
	 * However for pipe sessions we need to keep it zero,
	 * because script's perf_evsel__check_attr is triggered
	 * by attr->sample_type != 0, and we can't run it on
	 * stat sessions.
	 */
	if (!(STAT_RECORD && perf_stat.data.is_pipe))
		attr->sample_type = PERF_SAMPLE_IDENTIFIER;

	/*
	 * Disabling all counters initially, they will be enabled
	 * either manually by us or by kernel via enable_on_exec
	 * set later.
	 */
	if (perf_evsel__is_group_leader(evsel)) {
		attr->disabled = 1;

		/*
		 * In case of initial_delay we enable tracee
		 * events manually.
		 */
		if (target__none(&target) && !initial_delay)
			attr->enable_on_exec = 1;
	}

	if (target__has_cpu(&target) && !target__has_per_thread(&target))
		return perf_evsel__open_per_cpu(evsel, perf_evsel__cpus(evsel));

	return perf_evsel__open_per_thread(evsel, evsel_list->threads);
}

=======
>>>>>>> upstream/android-13
static int process_synthesized_event(struct perf_tool *tool __maybe_unused,
				     union perf_event *event,
				     struct perf_sample *sample __maybe_unused,
				     struct machine *machine __maybe_unused)
{
	if (perf_data__write(&perf_stat.data, event, event->header.size) < 0) {
		pr_err("failed to write perf data, error: %m\n");
		return -1;
	}

	perf_stat.bytes_written += event->header.size;
	return 0;
}

static int write_stat_round_event(u64 tm, u64 type)
{
	return perf_event__synthesize_stat_round(NULL, tm, type,
						 process_synthesized_event,
						 NULL);
}

#define WRITE_STAT_ROUND_EVENT(time, interval) \
	write_stat_round_event(time, PERF_STAT_ROUND_TYPE__ ## interval)

<<<<<<< HEAD
#define SID(e, x, y) xyarray__entry(e->sample_id, x, y)

static int
perf_evsel__write_stat_event(struct perf_evsel *counter, u32 cpu, u32 thread,
			     struct perf_counts_values *count)
=======
#define SID(e, x, y) xyarray__entry(e->core.sample_id, x, y)

static int evsel__write_stat_event(struct evsel *counter, u32 cpu, u32 thread,
				   struct perf_counts_values *count)
>>>>>>> upstream/android-13
{
	struct perf_sample_id *sid = SID(counter, cpu, thread);

	return perf_event__synthesize_stat(NULL, cpu, thread, sid->id, count,
					   process_synthesized_event, NULL);
}

<<<<<<< HEAD
=======
static int read_single_counter(struct evsel *counter, int cpu,
			       int thread, struct timespec *rs)
{
	if (counter->tool_event == PERF_TOOL_DURATION_TIME) {
		u64 val = rs->tv_nsec + rs->tv_sec*1000000000ULL;
		struct perf_counts_values *count =
			perf_counts(counter->counts, cpu, thread);
		count->ena = count->run = val;
		count->val = val;
		return 0;
	}
	return evsel__read_counter(counter, cpu, thread);
}

>>>>>>> upstream/android-13
/*
 * Read out the results of a single counter:
 * do not aggregate counts across CPUs in system-wide mode
 */
<<<<<<< HEAD
static int read_counter(struct perf_evsel *counter)
{
	int nthreads = thread_map__nr(evsel_list->threads);
	int ncpus, cpu, thread;

	if (target__has_cpu(&target) && !target__has_per_thread(&target))
		ncpus = perf_evsel__nr_cpus(counter);
	else
		ncpus = 1;
=======
static int read_counter_cpu(struct evsel *counter, struct timespec *rs, int cpu)
{
	int nthreads = perf_thread_map__nr(evsel_list->core.threads);
	int thread;
>>>>>>> upstream/android-13

	if (!counter->supported)
		return -ENOENT;

<<<<<<< HEAD
	if (counter->system_wide)
		nthreads = 1;

	for (thread = 0; thread < nthreads; thread++) {
		for (cpu = 0; cpu < ncpus; cpu++) {
			struct perf_counts_values *count;

			count = perf_counts(counter->counts, cpu, thread);

			/*
			 * The leader's group read loads data into its group members
			 * (via perf_evsel__read_counter) and sets threir count->loaded.
			 */
			if (!count->loaded &&
			    perf_evsel__read_counter(counter, cpu, thread)) {
				counter->counts->scaled = -1;
				perf_counts(counter->counts, cpu, thread)->ena = 0;
				perf_counts(counter->counts, cpu, thread)->run = 0;
				return -1;
			}

			count->loaded = false;

			if (STAT_RECORD) {
				if (perf_evsel__write_stat_event(counter, cpu, thread, count)) {
					pr_err("failed to write stat event\n");
					return -1;
				}
			}

			if (verbose > 1) {
				fprintf(stat_config.output,
					"%s: %d: %" PRIu64 " %" PRIu64 " %" PRIu64 "\n",
						perf_evsel__name(counter),
						cpu,
						count->val, count->ena, count->run);
			}
=======
	if (counter->core.system_wide)
		nthreads = 1;

	for (thread = 0; thread < nthreads; thread++) {
		struct perf_counts_values *count;

		count = perf_counts(counter->counts, cpu, thread);

		/*
		 * The leader's group read loads data into its group members
		 * (via evsel__read_counter()) and sets their count->loaded.
		 */
		if (!perf_counts__is_loaded(counter->counts, cpu, thread) &&
		    read_single_counter(counter, cpu, thread, rs)) {
			counter->counts->scaled = -1;
			perf_counts(counter->counts, cpu, thread)->ena = 0;
			perf_counts(counter->counts, cpu, thread)->run = 0;
			return -1;
		}

		perf_counts__set_loaded(counter->counts, cpu, thread, false);

		if (STAT_RECORD) {
			if (evsel__write_stat_event(counter, cpu, thread, count)) {
				pr_err("failed to write stat event\n");
				return -1;
			}
		}

		if (verbose > 1) {
			fprintf(stat_config.output,
				"%s: %d: %" PRIu64 " %" PRIu64 " %" PRIu64 "\n",
					evsel__name(counter),
					cpu,
					count->val, count->ena, count->run);
>>>>>>> upstream/android-13
		}
	}

	return 0;
}

<<<<<<< HEAD
static void read_counters(void)
{
	struct perf_evsel *counter;
	int ret;

	evlist__for_each_entry(evsel_list, counter) {
		ret = read_counter(counter);
		if (ret)
			pr_debug("failed to read counter %s\n", counter->name);

		if (ret == 0 && perf_stat_process_counter(&stat_config, counter))
			pr_warning("failed to process counter %s\n", counter->name);
	}
=======
static int read_affinity_counters(struct timespec *rs)
{
	struct evsel *counter;
	struct affinity affinity;
	int i, ncpus, cpu;

	if (all_counters_use_bpf)
		return 0;

	if (affinity__setup(&affinity) < 0)
		return -1;

	ncpus = perf_cpu_map__nr(evsel_list->core.all_cpus);
	if (!target__has_cpu(&target) || target__has_per_thread(&target))
		ncpus = 1;
	evlist__for_each_cpu(evsel_list, i, cpu) {
		if (i >= ncpus)
			break;
		affinity__set(&affinity, cpu);

		evlist__for_each_entry(evsel_list, counter) {
			if (evsel__cpu_iter_skip(counter, cpu))
				continue;
			if (evsel__is_bpf(counter))
				continue;
			if (!counter->err) {
				counter->err = read_counter_cpu(counter, rs,
								counter->cpu_iter - 1);
			}
		}
	}
	affinity__cleanup(&affinity);
	return 0;
}

static int read_bpf_map_counters(void)
{
	struct evsel *counter;
	int err;

	evlist__for_each_entry(evsel_list, counter) {
		if (!evsel__is_bpf(counter))
			continue;

		err = bpf_counter__read(counter);
		if (err)
			return err;
	}
	return 0;
}

static void read_counters(struct timespec *rs)
{
	struct evsel *counter;

	if (!stat_config.stop_read_counter) {
		if (read_bpf_map_counters() ||
		    read_affinity_counters(rs))
			return;
	}

	evlist__for_each_entry(evsel_list, counter) {
		if (counter->err)
			pr_debug("failed to read counter %s\n", counter->name);
		if (counter->err == 0 && perf_stat_process_counter(&stat_config, counter))
			pr_warning("failed to process counter %s\n", counter->name);
		counter->err = 0;
	}
}

static int runtime_stat_new(struct perf_stat_config *config, int nthreads)
{
	int i;

	config->stats = calloc(nthreads, sizeof(struct runtime_stat));
	if (!config->stats)
		return -1;

	config->stats_num = nthreads;

	for (i = 0; i < nthreads; i++)
		runtime_stat__init(&config->stats[i]);

	return 0;
}

static void runtime_stat_delete(struct perf_stat_config *config)
{
	int i;

	if (!config->stats)
		return;

	for (i = 0; i < config->stats_num; i++)
		runtime_stat__exit(&config->stats[i]);

	zfree(&config->stats);
}

static void runtime_stat_reset(struct perf_stat_config *config)
{
	int i;

	if (!config->stats)
		return;

	for (i = 0; i < config->stats_num; i++)
		perf_stat__reset_shadow_per_stat(&config->stats[i]);
>>>>>>> upstream/android-13
}

static void process_interval(void)
{
	struct timespec ts, rs;

<<<<<<< HEAD
	read_counters();

	clock_gettime(CLOCK_MONOTONIC, &ts);
	diff_timespec(&rs, &ts, &ref_time);

=======
	clock_gettime(CLOCK_MONOTONIC, &ts);
	diff_timespec(&rs, &ts, &ref_time);

	perf_stat__reset_shadow_per_stat(&rt_stat);
	runtime_stat_reset(&stat_config);
	read_counters(&rs);

>>>>>>> upstream/android-13
	if (STAT_RECORD) {
		if (WRITE_STAT_ROUND_EVENT(rs.tv_sec * NSEC_PER_SEC + rs.tv_nsec, INTERVAL))
			pr_err("failed to write stat round event\n");
	}

	init_stats(&walltime_nsecs_stats);
	update_stats(&walltime_nsecs_stats, stat_config.interval * 1000000ULL);
	print_counters(&rs, 0, NULL);
}

<<<<<<< HEAD
static void enable_counters(void)
{
	if (initial_delay)
		usleep(initial_delay * USEC_PER_MSEC);
=======
static bool handle_interval(unsigned int interval, int *times)
{
	if (interval) {
		process_interval();
		if (interval_count && !(--(*times)))
			return true;
	}
	return false;
}

static int enable_counters(void)
{
	struct evsel *evsel;
	int err;

	evlist__for_each_entry(evsel_list, evsel) {
		if (!evsel__is_bpf(evsel))
			continue;

		err = bpf_counter__enable(evsel);
		if (err)
			return err;
	}

	if (stat_config.initial_delay < 0) {
		pr_info(EVLIST_DISABLED_MSG);
		return 0;
	}

	if (stat_config.initial_delay > 0) {
		pr_info(EVLIST_DISABLED_MSG);
		usleep(stat_config.initial_delay * USEC_PER_MSEC);
	}
>>>>>>> upstream/android-13

	/*
	 * We need to enable counters only if:
	 * - we don't have tracee (attaching to task or cpu)
	 * - we have initial delay configured
	 */
<<<<<<< HEAD
	if (!target__none(&target) || initial_delay)
		perf_evlist__enable(evsel_list);
=======
	if (!target__none(&target) || stat_config.initial_delay) {
		if (!all_counters_use_bpf)
			evlist__enable(evsel_list);
		if (stat_config.initial_delay > 0)
			pr_info(EVLIST_ENABLED_MSG);
	}
	return 0;
>>>>>>> upstream/android-13
}

static void disable_counters(void)
{
<<<<<<< HEAD
=======
	struct evsel *counter;

>>>>>>> upstream/android-13
	/*
	 * If we don't have tracee (attaching to task or cpu), counters may
	 * still be running. To get accurate group ratios, we must stop groups
	 * from counting before reading their constituent counters.
	 */
<<<<<<< HEAD
	if (!target__none(&target))
		perf_evlist__disable(evsel_list);
=======
	if (!target__none(&target)) {
		evlist__for_each_entry(evsel_list, counter)
			bpf_counter__disable(counter);
		if (!all_counters_use_bpf)
			evlist__disable(evsel_list);
	}
>>>>>>> upstream/android-13
}

static volatile int workload_exec_errno;

/*
<<<<<<< HEAD
 * perf_evlist__prepare_workload will send a SIGUSR1
=======
 * evlist__prepare_workload will send a SIGUSR1
>>>>>>> upstream/android-13
 * if the fork fails, since we asked by setting its
 * want_signal to true.
 */
static void workload_exec_failed_signal(int signo __maybe_unused, siginfo_t *info,
					void *ucontext __maybe_unused)
{
	workload_exec_errno = info->si_value.sival_int;
}

<<<<<<< HEAD
static int perf_stat_synthesize_config(bool is_pipe)
{
	int err;

	if (is_pipe) {
		err = perf_event__synthesize_attrs(NULL, perf_stat.session,
						   process_synthesized_event);
		if (err < 0) {
			pr_err("Couldn't synthesize attrs.\n");
			return err;
		}
	}

	err = perf_event__synthesize_extra_attr(NULL,
						evsel_list,
						process_synthesized_event,
						is_pipe);

	err = perf_event__synthesize_thread_map2(NULL, evsel_list->threads,
						process_synthesized_event,
						NULL);
	if (err < 0) {
		pr_err("Couldn't synthesize thread map.\n");
		return err;
	}

	err = perf_event__synthesize_cpu_map(NULL, evsel_list->cpus,
					     process_synthesized_event, NULL);
	if (err < 0) {
		pr_err("Couldn't synthesize thread map.\n");
		return err;
	}

	err = perf_event__synthesize_stat_config(NULL, &stat_config,
						 process_synthesized_event, NULL);
	if (err < 0) {
		pr_err("Couldn't synthesize config.\n");
		return err;
	}

	return 0;
}

#define FD(e, x, y) (*(int *)xyarray__entry(e->fd, x, y))

static int __store_counter_ids(struct perf_evsel *counter)
{
	int cpu, thread;

	for (cpu = 0; cpu < xyarray__max_x(counter->fd); cpu++) {
		for (thread = 0; thread < xyarray__max_y(counter->fd);
		     thread++) {
			int fd = FD(counter, cpu, thread);

			if (perf_evlist__id_add_fd(evsel_list, counter,
						   cpu, thread, fd) < 0)
				return -1;
		}
	}

	return 0;
}

static int store_counter_ids(struct perf_evsel *counter)
{
	struct cpu_map *cpus = counter->cpus;
	struct thread_map *threads = counter->threads;

	if (perf_evsel__alloc_id(counter, cpus->nr, threads->nr))
		return -ENOMEM;

	return __store_counter_ids(counter);
}

static bool perf_evsel__should_store_id(struct perf_evsel *counter)
{
	return STAT_RECORD || counter->attr.read_format & PERF_FORMAT_ID;
}

static struct perf_evsel *perf_evsel__reset_weak_group(struct perf_evsel *evsel)
{
	struct perf_evsel *c2, *leader;
	bool is_open = true;

	leader = evsel->leader;
	pr_debug("Weak group for %s/%d failed\n",
			leader->name, leader->nr_members);

	/*
	 * for_each_group_member doesn't work here because it doesn't
	 * include the first entry.
	 */
	evlist__for_each_entry(evsel_list, c2) {
		if (c2 == evsel)
			is_open = false;
		if (c2->leader == leader) {
			if (is_open)
				perf_evsel__close(c2);
			c2->leader = c2;
			c2->nr_members = 0;
		}
	}
	return leader;
=======
static bool evsel__should_store_id(struct evsel *counter)
{
	return STAT_RECORD || counter->core.attr.read_format & PERF_FORMAT_ID;
}

static bool is_target_alive(struct target *_target,
			    struct perf_thread_map *threads)
{
	struct stat st;
	int i;

	if (!target__has_task(_target))
		return true;

	for (i = 0; i < threads->nr; i++) {
		char path[PATH_MAX];

		scnprintf(path, PATH_MAX, "%s/%d", procfs__mountpoint(),
			  threads->map[i].pid);

		if (!stat(path, &st))
			return true;
	}

	return false;
}

static void process_evlist(struct evlist *evlist, unsigned int interval)
{
	enum evlist_ctl_cmd cmd = EVLIST_CTL_CMD_UNSUPPORTED;

	if (evlist__ctlfd_process(evlist, &cmd) > 0) {
		switch (cmd) {
		case EVLIST_CTL_CMD_ENABLE:
			if (interval)
				process_interval();
			break;
		case EVLIST_CTL_CMD_DISABLE:
			if (interval)
				process_interval();
			break;
		case EVLIST_CTL_CMD_SNAPSHOT:
		case EVLIST_CTL_CMD_ACK:
		case EVLIST_CTL_CMD_UNSUPPORTED:
		case EVLIST_CTL_CMD_EVLIST:
		case EVLIST_CTL_CMD_STOP:
		case EVLIST_CTL_CMD_PING:
		default:
			break;
		}
	}
}

static void compute_tts(struct timespec *time_start, struct timespec *time_stop,
			int *time_to_sleep)
{
	int tts = *time_to_sleep;
	struct timespec time_diff;

	diff_timespec(&time_diff, time_stop, time_start);

	tts -= time_diff.tv_sec * MSEC_PER_SEC +
	       time_diff.tv_nsec / NSEC_PER_MSEC;

	if (tts < 0)
		tts = 0;

	*time_to_sleep = tts;
}

static int dispatch_events(bool forks, int timeout, int interval, int *times)
{
	int child_exited = 0, status = 0;
	int time_to_sleep, sleep_time;
	struct timespec time_start, time_stop;

	if (interval)
		sleep_time = interval;
	else if (timeout)
		sleep_time = timeout;
	else
		sleep_time = 1000;

	time_to_sleep = sleep_time;

	while (!done) {
		if (forks)
			child_exited = waitpid(child_pid, &status, WNOHANG);
		else
			child_exited = !is_target_alive(&target, evsel_list->core.threads) ? 1 : 0;

		if (child_exited)
			break;

		clock_gettime(CLOCK_MONOTONIC, &time_start);
		if (!(evlist__poll(evsel_list, time_to_sleep) > 0)) { /* poll timeout or EINTR */
			if (timeout || handle_interval(interval, times))
				break;
			time_to_sleep = sleep_time;
		} else { /* fd revent */
			process_evlist(evsel_list, interval);
			clock_gettime(CLOCK_MONOTONIC, &time_stop);
			compute_tts(&time_start, &time_stop, &time_to_sleep);
		}
	}

	return status;
}

enum counter_recovery {
	COUNTER_SKIP,
	COUNTER_RETRY,
	COUNTER_FATAL,
};

static enum counter_recovery stat_handle_error(struct evsel *counter)
{
	char msg[BUFSIZ];
	/*
	 * PPC returns ENXIO for HW counters until 2.6.37
	 * (behavior changed with commit b0a873e).
	 */
	if (errno == EINVAL || errno == ENOSYS ||
	    errno == ENOENT || errno == EOPNOTSUPP ||
	    errno == ENXIO) {
		if (verbose > 0)
			ui__warning("%s event is not supported by the kernel.\n",
				    evsel__name(counter));
		counter->supported = false;
		/*
		 * errored is a sticky flag that means one of the counter's
		 * cpu event had a problem and needs to be reexamined.
		 */
		counter->errored = true;

		if ((evsel__leader(counter) != counter) ||
		    !(counter->core.leader->nr_members > 1))
			return COUNTER_SKIP;
	} else if (evsel__fallback(counter, errno, msg, sizeof(msg))) {
		if (verbose > 0)
			ui__warning("%s\n", msg);
		return COUNTER_RETRY;
	} else if (target__has_per_thread(&target) &&
		   evsel_list->core.threads &&
		   evsel_list->core.threads->err_thread != -1) {
		/*
		 * For global --per-thread case, skip current
		 * error thread.
		 */
		if (!thread_map__remove(evsel_list->core.threads,
					evsel_list->core.threads->err_thread)) {
			evsel_list->core.threads->err_thread = -1;
			return COUNTER_RETRY;
		}
	}

	evsel__open_strerror(counter, &target, errno, msg, sizeof(msg));
	ui__error("%s\n", msg);

	if (child_pid != -1)
		kill(child_pid, SIGTERM);
	return COUNTER_FATAL;
>>>>>>> upstream/android-13
}

static int __run_perf_stat(int argc, const char **argv, int run_idx)
{
	int interval = stat_config.interval;
	int times = stat_config.times;
	int timeout = stat_config.timeout;
	char msg[BUFSIZ];
	unsigned long long t0, t1;
<<<<<<< HEAD
	struct perf_evsel *counter;
	struct timespec ts;
=======
	struct evsel *counter;
>>>>>>> upstream/android-13
	size_t l;
	int status = 0;
	const bool forks = (argc > 0);
	bool is_pipe = STAT_RECORD ? perf_stat.data.is_pipe : false;
<<<<<<< HEAD
	struct perf_evsel_config_term *err_term;

	if (interval) {
		ts.tv_sec  = interval / USEC_PER_MSEC;
		ts.tv_nsec = (interval % USEC_PER_MSEC) * NSEC_PER_MSEC;
	} else if (timeout) {
		ts.tv_sec  = timeout / USEC_PER_MSEC;
		ts.tv_nsec = (timeout % USEC_PER_MSEC) * NSEC_PER_MSEC;
	} else {
		ts.tv_sec  = 1;
		ts.tv_nsec = 0;
	}

	if (forks) {
		if (perf_evlist__prepare_workload(evsel_list, &target, argv, is_pipe,
						  workload_exec_failed_signal) < 0) {
=======
	struct affinity affinity;
	int i, cpu, err;
	bool second_pass = false;

	if (forks) {
		if (evlist__prepare_workload(evsel_list, &target, argv, is_pipe, workload_exec_failed_signal) < 0) {
>>>>>>> upstream/android-13
			perror("failed to prepare workload");
			return -1;
		}
		child_pid = evsel_list->workload.pid;
	}

	if (group)
<<<<<<< HEAD
		perf_evlist__set_leader(evsel_list);

	evlist__for_each_entry(evsel_list, counter) {
try_again:
		if (create_perf_stat_counter(counter) < 0) {

			/* Weak group failed. Reset the group. */
			if ((errno == EINVAL || errno == EBADF) &&
			    counter->leader != counter &&
			    counter->weak_group) {
				counter = perf_evsel__reset_weak_group(counter);
				goto try_again;
			}

			/*
			 * PPC returns ENXIO for HW counters until 2.6.37
			 * (behavior changed with commit b0a873e).
			 */
			if (errno == EINVAL || errno == ENOSYS ||
			    errno == ENOENT || errno == EOPNOTSUPP ||
			    errno == ENXIO) {
				if (verbose > 0)
					ui__warning("%s event is not supported by the kernel.\n",
						    perf_evsel__name(counter));
				counter->supported = false;

				if ((counter->leader != counter) ||
				    !(counter->leader->nr_members > 1))
					continue;
			} else if (perf_evsel__fallback(counter, errno, msg, sizeof(msg))) {
                                if (verbose > 0)
                                        ui__warning("%s\n", msg);
                                goto try_again;
			} else if (target__has_per_thread(&target) &&
				   evsel_list->threads &&
				   evsel_list->threads->err_thread != -1) {
				/*
				 * For global --per-thread case, skip current
				 * error thread.
				 */
				if (!thread_map__remove(evsel_list->threads,
							evsel_list->threads->err_thread)) {
					evsel_list->threads->err_thread = -1;
					goto try_again;
				}
			}

			perf_evsel__open_strerror(counter, &target,
						  errno, msg, sizeof(msg));
			ui__error("%s\n", msg);

			if (child_pid != -1)
				kill(child_pid, SIGTERM);

			return -1;
		}
		counter->supported = true;

		l = strlen(counter->unit);
		if (l > unit_width)
			unit_width = l;

		if (perf_evsel__should_store_id(counter) &&
		    store_counter_ids(counter))
			return -1;
	}

	if (perf_evlist__apply_filters(evsel_list, &counter)) {
		pr_err("failed to set filter \"%s\" on event %s with %d (%s)\n",
			counter->filter, perf_evsel__name(counter), errno,
=======
		evlist__set_leader(evsel_list);

	if (affinity__setup(&affinity) < 0)
		return -1;

	evlist__for_each_entry(evsel_list, counter) {
		if (bpf_counter__load(counter, &target))
			return -1;
		if (!evsel__is_bpf(counter))
			all_counters_use_bpf = false;
	}

	evlist__for_each_cpu (evsel_list, i, cpu) {
		/*
		 * bperf calls evsel__open_per_cpu() in bperf__load(), so
		 * no need to call it again here.
		 */
		if (target.use_bpf)
			break;
		affinity__set(&affinity, cpu);

		evlist__for_each_entry(evsel_list, counter) {
			if (evsel__cpu_iter_skip(counter, cpu))
				continue;
			if (counter->reset_group || counter->errored)
				continue;
			if (evsel__is_bpf(counter))
				continue;
try_again:
			if (create_perf_stat_counter(counter, &stat_config, &target,
						     counter->cpu_iter - 1) < 0) {

				/*
				 * Weak group failed. We cannot just undo this here
				 * because earlier CPUs might be in group mode, and the kernel
				 * doesn't support mixing group and non group reads. Defer
				 * it to later.
				 * Don't close here because we're in the wrong affinity.
				 */
				if ((errno == EINVAL || errno == EBADF) &&
				    evsel__leader(counter) != counter &&
				    counter->weak_group) {
					evlist__reset_weak_group(evsel_list, counter, false);
					assert(counter->reset_group);
					second_pass = true;
					continue;
				}

				switch (stat_handle_error(counter)) {
				case COUNTER_FATAL:
					return -1;
				case COUNTER_RETRY:
					goto try_again;
				case COUNTER_SKIP:
					continue;
				default:
					break;
				}

			}
			counter->supported = true;
		}
	}

	if (second_pass) {
		/*
		 * Now redo all the weak group after closing them,
		 * and also close errored counters.
		 */

		evlist__for_each_cpu(evsel_list, i, cpu) {
			affinity__set(&affinity, cpu);
			/* First close errored or weak retry */
			evlist__for_each_entry(evsel_list, counter) {
				if (!counter->reset_group && !counter->errored)
					continue;
				if (evsel__cpu_iter_skip_no_inc(counter, cpu))
					continue;
				perf_evsel__close_cpu(&counter->core, counter->cpu_iter);
			}
			/* Now reopen weak */
			evlist__for_each_entry(evsel_list, counter) {
				if (!counter->reset_group && !counter->errored)
					continue;
				if (evsel__cpu_iter_skip(counter, cpu))
					continue;
				if (!counter->reset_group)
					continue;
try_again_reset:
				pr_debug2("reopening weak %s\n", evsel__name(counter));
				if (create_perf_stat_counter(counter, &stat_config, &target,
							     counter->cpu_iter - 1) < 0) {

					switch (stat_handle_error(counter)) {
					case COUNTER_FATAL:
						return -1;
					case COUNTER_RETRY:
						goto try_again_reset;
					case COUNTER_SKIP:
						continue;
					default:
						break;
					}
				}
				counter->supported = true;
			}
		}
	}
	affinity__cleanup(&affinity);

	evlist__for_each_entry(evsel_list, counter) {
		if (!counter->supported) {
			perf_evsel__free_fd(&counter->core);
			continue;
		}

		l = strlen(counter->unit);
		if (l > stat_config.unit_width)
			stat_config.unit_width = l;

		if (evsel__should_store_id(counter) &&
		    evsel__store_ids(counter, evsel_list))
			return -1;
	}

	if (evlist__apply_filters(evsel_list, &counter)) {
		pr_err("failed to set filter \"%s\" on event %s with %d (%s)\n",
			counter->filter, evsel__name(counter), errno,
>>>>>>> upstream/android-13
			str_error_r(errno, msg, sizeof(msg)));
		return -1;
	}

<<<<<<< HEAD
	if (perf_evlist__apply_drv_configs(evsel_list, &counter, &err_term)) {
		pr_err("failed to set config \"%s\" on event %s with %d (%s)\n",
		      err_term->val.drv_cfg, perf_evsel__name(counter), errno,
		      str_error_r(errno, msg, sizeof(msg)));
		return -1;
	}

	if (STAT_RECORD) {
		int err, fd = perf_data__fd(&perf_stat.data);
=======
	if (STAT_RECORD) {
		int fd = perf_data__fd(&perf_stat.data);
>>>>>>> upstream/android-13

		if (is_pipe) {
			err = perf_header__write_pipe(perf_data__fd(&perf_stat.data));
		} else {
			err = perf_session__write_header(perf_stat.session, evsel_list,
							 fd, false);
		}

		if (err < 0)
			return err;

<<<<<<< HEAD
		err = perf_stat_synthesize_config(is_pipe);
=======
		err = perf_event__synthesize_stat_events(&stat_config, NULL, evsel_list,
							 process_synthesized_event, is_pipe);
>>>>>>> upstream/android-13
		if (err < 0)
			return err;
	}

	/*
	 * Enable counters and exec the command:
	 */
<<<<<<< HEAD
	t0 = rdclock();
	clock_gettime(CLOCK_MONOTONIC, &ref_time);

	if (forks) {
		perf_evlist__start_workload(evsel_list);
		enable_counters();

		if (interval || timeout) {
			while (!waitpid(child_pid, &status, WNOHANG)) {
				nanosleep(&ts, NULL);
				if (timeout)
					break;
				process_interval();
				if (interval_count && !(--times))
					break;
			}
		}
		wait4(child_pid, &status, 0, &ru_data);
=======
	if (forks) {
		err = enable_counters();
		if (err)
			return -1;
		evlist__start_workload(evsel_list);

		t0 = rdclock();
		clock_gettime(CLOCK_MONOTONIC, &ref_time);

		if (interval || timeout || evlist__ctlfd_initialized(evsel_list))
			status = dispatch_events(forks, timeout, interval, &times);
		if (child_pid != -1) {
			if (timeout)
				kill(child_pid, SIGTERM);
			wait4(child_pid, &status, 0, &stat_config.ru_data);
		}
>>>>>>> upstream/android-13

		if (workload_exec_errno) {
			const char *emsg = str_error_r(workload_exec_errno, msg, sizeof(msg));
			pr_err("Workload failed: %s\n", emsg);
			return -1;
		}

		if (WIFSIGNALED(status))
			psignal(WTERMSIG(status), argv[0]);
	} else {
<<<<<<< HEAD
		enable_counters();
		while (!done) {
			nanosleep(&ts, NULL);
			if (timeout)
				break;
			if (interval) {
				process_interval();
				if (interval_count && !(--times))
					break;
			}
		}
=======
		err = enable_counters();
		if (err)
			return -1;

		t0 = rdclock();
		clock_gettime(CLOCK_MONOTONIC, &ref_time);

		status = dispatch_events(forks, timeout, interval, &times);
>>>>>>> upstream/android-13
	}

	disable_counters();

	t1 = rdclock();

<<<<<<< HEAD
	if (walltime_run_table)
		walltime_run[run_idx] = t1 - t0;

	update_stats(&walltime_nsecs_stats, t1 - t0);
=======
	if (stat_config.walltime_run_table)
		stat_config.walltime_run[run_idx] = t1 - t0;

	if (interval && stat_config.summary) {
		stat_config.interval = 0;
		stat_config.stop_read_counter = true;
		init_stats(&walltime_nsecs_stats);
		update_stats(&walltime_nsecs_stats, t1 - t0);

		if (stat_config.aggr_mode == AGGR_GLOBAL)
			evlist__save_aggr_prev_raw_counts(evsel_list);

		evlist__copy_prev_raw_counts(evsel_list);
		evlist__reset_prev_raw_counts(evsel_list);
		runtime_stat_reset(&stat_config);
		perf_stat__reset_shadow_per_stat(&rt_stat);
	} else
		update_stats(&walltime_nsecs_stats, t1 - t0);
>>>>>>> upstream/android-13

	/*
	 * Closing a group leader splits the group, and as we only disable
	 * group leaders, results in remaining events becoming enabled. To
	 * avoid arbitrary skew, we must read all counters before closing any
	 * group leaders.
	 */
<<<<<<< HEAD
	read_counters();
	perf_evlist__close(evsel_list);
=======
	read_counters(&(struct timespec) { .tv_nsec = t1-t0 });

	/*
	 * We need to keep evsel_list alive, because it's processed
	 * later the evsel_list will be closed after.
	 */
	if (!STAT_RECORD)
		evlist__close(evsel_list);
>>>>>>> upstream/android-13

	return WEXITSTATUS(status);
}

static int run_perf_stat(int argc, const char **argv, int run_idx)
{
	int ret;

	if (pre_cmd) {
		ret = system(pre_cmd);
		if (ret)
			return ret;
	}

	if (sync_run)
		sync();

	ret = __run_perf_stat(argc, argv, run_idx);
	if (ret)
		return ret;

	if (post_cmd) {
		ret = system(post_cmd);
		if (ret)
			return ret;
	}

	return ret;
}

<<<<<<< HEAD
static void print_running(u64 run, u64 ena)
{
	if (csv_output) {
		fprintf(stat_config.output, "%s%" PRIu64 "%s%.2f",
					csv_sep,
					run,
					csv_sep,
					ena ? 100.0 * run / ena : 100.0);
	} else if (run != ena) {
		fprintf(stat_config.output, "  (%.2f%%)", 100.0 * run / ena);
	}
}

static void print_noise_pct(double total, double avg)
{
	double pct = rel_stddev_stats(total, avg);

	if (csv_output)
		fprintf(stat_config.output, "%s%.2f%%", csv_sep, pct);
	else if (pct)
		fprintf(stat_config.output, "  ( +-%6.2f%% )", pct);
}

static void print_noise(struct perf_evsel *evsel, double avg)
{
	struct perf_stat_evsel *ps;

	if (run_count == 1)
		return;

	ps = evsel->stats;
	print_noise_pct(stddev_stats(&ps->res_stats[0]), avg);
}

static void aggr_printout(struct perf_evsel *evsel, int id, int nr)
{
	switch (stat_config.aggr_mode) {
	case AGGR_CORE:
		fprintf(stat_config.output, "S%d-C%*d%s%*d%s",
			cpu_map__id_to_socket(id),
			csv_output ? 0 : -8,
			cpu_map__id_to_cpu(id),
			csv_sep,
			csv_output ? 0 : 4,
			nr,
			csv_sep);
		break;
	case AGGR_SOCKET:
		fprintf(stat_config.output, "S%*d%s%*d%s",
			csv_output ? 0 : -5,
			id,
			csv_sep,
			csv_output ? 0 : 4,
			nr,
			csv_sep);
			break;
	case AGGR_NONE:
		fprintf(stat_config.output, "CPU%*d%s",
			csv_output ? 0 : -4,
			perf_evsel__cpus(evsel)->map[id], csv_sep);
		break;
	case AGGR_THREAD:
		fprintf(stat_config.output, "%*s-%*d%s",
			csv_output ? 0 : 16,
			thread_map__comm(evsel->threads, id),
			csv_output ? 0 : -8,
			thread_map__pid(evsel->threads, id),
			csv_sep);
		break;
	case AGGR_GLOBAL:
	case AGGR_UNSET:
	default:
		break;
	}
}

struct outstate {
	FILE *fh;
	bool newline;
	const char *prefix;
	int  nfields;
	int  id, nr;
	struct perf_evsel *evsel;
};

#define METRIC_LEN  35

static void new_line_std(void *ctx)
{
	struct outstate *os = ctx;

	os->newline = true;
}

static void do_new_line_std(struct outstate *os)
{
	fputc('\n', os->fh);
	fputs(os->prefix, os->fh);
	aggr_printout(os->evsel, os->id, os->nr);
	if (stat_config.aggr_mode == AGGR_NONE)
		fprintf(os->fh, "        ");
	fprintf(os->fh, "                                                 ");
}

static void print_metric_std(void *ctx, const char *color, const char *fmt,
			     const char *unit, double val)
{
	struct outstate *os = ctx;
	FILE *out = os->fh;
	int n;
	bool newline = os->newline;

	os->newline = false;

	if (unit == NULL || fmt == NULL) {
		fprintf(out, "%-*s", METRIC_LEN, "");
		return;
	}

	if (newline)
		do_new_line_std(os);

	n = fprintf(out, " # ");
	if (color)
		n += color_fprintf(out, color, fmt, val);
	else
		n += fprintf(out, fmt, val);
	fprintf(out, " %-*s", METRIC_LEN - n - 1, unit);
}

static void new_line_csv(void *ctx)
{
	struct outstate *os = ctx;
	int i;

	fputc('\n', os->fh);
	if (os->prefix)
		fprintf(os->fh, "%s%s", os->prefix, csv_sep);
	aggr_printout(os->evsel, os->id, os->nr);
	for (i = 0; i < os->nfields; i++)
		fputs(csv_sep, os->fh);
}

static void print_metric_csv(void *ctx,
			     const char *color __maybe_unused,
			     const char *fmt, const char *unit, double val)
{
	struct outstate *os = ctx;
	FILE *out = os->fh;
	char buf[64], *vals, *ends;

	if (unit == NULL || fmt == NULL) {
		fprintf(out, "%s%s", csv_sep, csv_sep);
		return;
	}
	snprintf(buf, sizeof(buf), fmt, val);
	ends = vals = ltrim(buf);
	while (isdigit(*ends) || *ends == '.')
		ends++;
	*ends = 0;
	while (isspace(*unit))
		unit++;
	fprintf(out, "%s%s%s%s", csv_sep, vals, csv_sep, unit);
}

/* Filter out some columns that don't work well in metrics only mode */

static bool valid_only_metric(const char *unit)
{
	if (!unit)
		return false;
	if (strstr(unit, "/sec") ||
	    strstr(unit, "hz") ||
	    strstr(unit, "Hz") ||
	    strstr(unit, "CPUs utilized"))
		return false;
	return true;
}

static const char *fixunit(char *buf, struct perf_evsel *evsel,
			   const char *unit)
{
	if (!strncmp(unit, "of all", 6)) {
		snprintf(buf, 1024, "%s %s", perf_evsel__name(evsel),
			 unit);
		return buf;
	}
	return unit;
}

static void print_metric_only(void *ctx, const char *color, const char *fmt,
			      const char *unit, double val)
{
	struct outstate *os = ctx;
	FILE *out = os->fh;
	char buf[1024], str[1024];
	unsigned mlen = metric_only_len;

	if (!valid_only_metric(unit))
		return;
	unit = fixunit(buf, os->evsel, unit);
	if (mlen < strlen(unit))
		mlen = strlen(unit) + 1;

	if (color)
		mlen += strlen(color) + sizeof(PERF_COLOR_RESET) - 1;

	color_snprintf(str, sizeof(str), color ?: "", fmt, val);
	fprintf(out, "%*s ", mlen, str);
}

static void print_metric_only_csv(void *ctx, const char *color __maybe_unused,
				  const char *fmt,
				  const char *unit, double val)
{
	struct outstate *os = ctx;
	FILE *out = os->fh;
	char buf[64], *vals, *ends;
	char tbuf[1024];

	if (!valid_only_metric(unit))
		return;
	unit = fixunit(tbuf, os->evsel, unit);
	snprintf(buf, sizeof buf, fmt, val);
	ends = vals = ltrim(buf);
	while (isdigit(*ends) || *ends == '.')
		ends++;
	*ends = 0;
	fprintf(out, "%s%s", vals, csv_sep);
}

static void new_line_metric(void *ctx __maybe_unused)
{
}

static void print_metric_header(void *ctx, const char *color __maybe_unused,
				const char *fmt __maybe_unused,
				const char *unit, double val __maybe_unused)
{
	struct outstate *os = ctx;
	char tbuf[1024];

	if (!valid_only_metric(unit))
		return;
	unit = fixunit(tbuf, os->evsel, unit);
	if (csv_output)
		fprintf(os->fh, "%s%s", unit, csv_sep);
	else
		fprintf(os->fh, "%*s ", metric_only_len, unit);
}

static int first_shadow_cpu(struct perf_evsel *evsel, int id)
{
	int i;

	if (!aggr_get_id)
		return 0;

	if (stat_config.aggr_mode == AGGR_NONE)
		return id;

	if (stat_config.aggr_mode == AGGR_GLOBAL)
		return 0;

	for (i = 0; i < perf_evsel__nr_cpus(evsel); i++) {
		int cpu2 = perf_evsel__cpus(evsel)->map[i];

		if (aggr_get_id(evsel_list->cpus, cpu2) == id)
			return cpu2;
	}
	return 0;
}

static void abs_printout(int id, int nr, struct perf_evsel *evsel, double avg)
{
	FILE *output = stat_config.output;
	double sc =  evsel->scale;
	const char *fmt;

	if (csv_output) {
		fmt = floor(sc) != sc ?  "%.2f%s" : "%.0f%s";
	} else {
		if (big_num)
			fmt = floor(sc) != sc ? "%'18.2f%s" : "%'18.0f%s";
		else
			fmt = floor(sc) != sc ? "%18.2f%s" : "%18.0f%s";
	}

	aggr_printout(evsel, id, nr);

	fprintf(output, fmt, avg, csv_sep);

	if (evsel->unit)
		fprintf(output, "%-*s%s",
			csv_output ? 0 : unit_width,
			evsel->unit, csv_sep);

	fprintf(output, "%-*s", csv_output ? 0 : 25, perf_evsel__name(evsel));

	if (evsel->cgrp)
		fprintf(output, "%s%s", csv_sep, evsel->cgrp->name);
}

static bool is_mixed_hw_group(struct perf_evsel *counter)
{
	struct perf_evlist *evlist = counter->evlist;
	u32 pmu_type = counter->attr.type;
	struct perf_evsel *pos;

	if (counter->nr_members < 2)
		return false;

	evlist__for_each_entry(evlist, pos) {
		/* software events can be part of any hardware group */
		if (pos->attr.type == PERF_TYPE_SOFTWARE)
			continue;
		if (pmu_type == PERF_TYPE_SOFTWARE) {
			pmu_type = pos->attr.type;
			continue;
		}
		if (pmu_type != pos->attr.type)
			return true;
	}

	return false;
}

static void printout(int id, int nr, struct perf_evsel *counter, double uval,
		     char *prefix, u64 run, u64 ena, double noise,
		     struct runtime_stat *st)
{
	struct perf_stat_output_ctx out;
	struct outstate os = {
		.fh = stat_config.output,
		.prefix = prefix ? prefix : "",
		.id = id,
		.nr = nr,
		.evsel = counter,
	};
	print_metric_t pm = print_metric_std;
	void (*nl)(void *);

	if (metric_only) {
		nl = new_line_metric;
		if (csv_output)
			pm = print_metric_only_csv;
		else
			pm = print_metric_only;
	} else
		nl = new_line_std;

	if (csv_output && !metric_only) {
		static int aggr_fields[] = {
			[AGGR_GLOBAL] = 0,
			[AGGR_THREAD] = 1,
			[AGGR_NONE] = 1,
			[AGGR_SOCKET] = 2,
			[AGGR_CORE] = 2,
		};

		pm = print_metric_csv;
		nl = new_line_csv;
		os.nfields = 3;
		os.nfields += aggr_fields[stat_config.aggr_mode];
		if (counter->cgrp)
			os.nfields++;
	}
	if (run == 0 || ena == 0 || counter->counts->scaled == -1) {
		if (metric_only) {
			pm(&os, NULL, "", "", 0);
			return;
		}
		aggr_printout(counter, id, nr);

		fprintf(stat_config.output, "%*s%s",
			csv_output ? 0 : 18,
			counter->supported ? CNTR_NOT_COUNTED : CNTR_NOT_SUPPORTED,
			csv_sep);

		if (counter->supported) {
			print_free_counters_hint = 1;
			if (is_mixed_hw_group(counter))
				print_mixed_hw_group_error = 1;
		}

		fprintf(stat_config.output, "%-*s%s",
			csv_output ? 0 : unit_width,
			counter->unit, csv_sep);

		fprintf(stat_config.output, "%*s",
			csv_output ? 0 : -25,
			perf_evsel__name(counter));

		if (counter->cgrp)
			fprintf(stat_config.output, "%s%s",
				csv_sep, counter->cgrp->name);

		if (!csv_output)
			pm(&os, NULL, NULL, "", 0);
		print_noise(counter, noise);
		print_running(run, ena);
		if (csv_output)
			pm(&os, NULL, NULL, "", 0);
		return;
	}

	if (!metric_only)
		abs_printout(id, nr, counter, uval);

	out.print_metric = pm;
	out.new_line = nl;
	out.ctx = &os;
	out.force_header = false;

	if (csv_output && !metric_only) {
		print_noise(counter, noise);
		print_running(run, ena);
	}

	perf_stat__print_shadow_stats(counter, uval,
				first_shadow_cpu(counter, id),
				&out, &metric_events, st);
	if (!csv_output && !metric_only) {
		print_noise(counter, noise);
		print_running(run, ena);
	}
}

static void aggr_update_shadow(void)
{
	int cpu, s2, id, s;
	u64 val;
	struct perf_evsel *counter;

	for (s = 0; s < aggr_map->nr; s++) {
		id = aggr_map->map[s];
		evlist__for_each_entry(evsel_list, counter) {
			val = 0;
			for (cpu = 0; cpu < perf_evsel__nr_cpus(counter); cpu++) {
				s2 = aggr_get_id(evsel_list->cpus, cpu);
				if (s2 != id)
					continue;
				val += perf_counts(counter->counts, cpu, 0)->val;
			}
			perf_stat__update_shadow_stats(counter, val,
					first_shadow_cpu(counter, id),
					&rt_stat);
		}
	}
}

static void uniquify_event_name(struct perf_evsel *counter)
{
	char *new_name;
	char *config;

	if (counter->uniquified_name ||
	    !counter->pmu_name || !strncmp(counter->name, counter->pmu_name,
					   strlen(counter->pmu_name)))
		return;

	config = strchr(counter->name, '/');
	if (config) {
		if (asprintf(&new_name,
			     "%s%s", counter->pmu_name, config) > 0) {
			free(counter->name);
			counter->name = new_name;
		}
	} else {
		if (asprintf(&new_name,
			     "%s [%s]", counter->name, counter->pmu_name) > 0) {
			free(counter->name);
			counter->name = new_name;
		}
	}

	counter->uniquified_name = true;
}

static void collect_all_aliases(struct perf_evsel *counter,
			    void (*cb)(struct perf_evsel *counter, void *data,
				       bool first),
			    void *data)
{
	struct perf_evsel *alias;

	alias = list_prepare_entry(counter, &(evsel_list->entries), node);
	list_for_each_entry_continue (alias, &evsel_list->entries, node) {
		if (strcmp(perf_evsel__name(alias), perf_evsel__name(counter)) ||
		    alias->scale != counter->scale ||
		    alias->cgrp != counter->cgrp ||
		    strcmp(alias->unit, counter->unit) ||
		    perf_evsel__is_clock(alias) != perf_evsel__is_clock(counter))
			break;
		alias->merged_stat = true;
		cb(alias, data, false);
	}
}

static bool collect_data(struct perf_evsel *counter,
			    void (*cb)(struct perf_evsel *counter, void *data,
				       bool first),
			    void *data)
{
	if (counter->merged_stat)
		return false;
	cb(counter, data, true);
	if (no_merge)
		uniquify_event_name(counter);
	else if (counter->auto_merge_stats)
		collect_all_aliases(counter, cb, data);
	return true;
}

struct aggr_data {
	u64 ena, run, val;
	int id;
	int nr;
	int cpu;
};

static void aggr_cb(struct perf_evsel *counter, void *data, bool first)
{
	struct aggr_data *ad = data;
	int cpu, s2;

	for (cpu = 0; cpu < perf_evsel__nr_cpus(counter); cpu++) {
		struct perf_counts_values *counts;

		s2 = aggr_get_id(perf_evsel__cpus(counter), cpu);
		if (s2 != ad->id)
			continue;
		if (first)
			ad->nr++;
		counts = perf_counts(counter->counts, cpu, 0);
		/*
		 * When any result is bad, make them all to give
		 * consistent output in interval mode.
		 */
		if (counts->ena == 0 || counts->run == 0 ||
		    counter->counts->scaled == -1) {
			ad->ena = 0;
			ad->run = 0;
			break;
		}
		ad->val += counts->val;
		ad->ena += counts->ena;
		ad->run += counts->run;
	}
}

static void print_aggr(char *prefix)
{
	FILE *output = stat_config.output;
	struct perf_evsel *counter;
	int s, id, nr;
	double uval;
	u64 ena, run, val;
	bool first;

	if (!(aggr_map || aggr_get_id))
		return;

	aggr_update_shadow();

	/*
	 * With metric_only everything is on a single line.
	 * Without each counter has its own line.
	 */
	for (s = 0; s < aggr_map->nr; s++) {
		struct aggr_data ad;
		if (prefix && metric_only)
			fprintf(output, "%s", prefix);

		ad.id = id = aggr_map->map[s];
		first = true;
		evlist__for_each_entry(evsel_list, counter) {
			if (is_duration_time(counter))
				continue;

			ad.val = ad.ena = ad.run = 0;
			ad.nr = 0;
			if (!collect_data(counter, aggr_cb, &ad))
				continue;
			nr = ad.nr;
			ena = ad.ena;
			run = ad.run;
			val = ad.val;
			if (first && metric_only) {
				first = false;
				aggr_printout(counter, id, nr);
			}
			if (prefix && !metric_only)
				fprintf(output, "%s", prefix);

			uval = val * counter->scale;
			printout(id, nr, counter, uval, prefix, run, ena, 1.0,
				 &rt_stat);
			if (!metric_only)
				fputc('\n', output);
		}
		if (metric_only)
			fputc('\n', output);
	}
}

static int cmp_val(const void *a, const void *b)
{
	return ((struct perf_aggr_thread_value *)b)->val -
		((struct perf_aggr_thread_value *)a)->val;
}

static struct perf_aggr_thread_value *sort_aggr_thread(
					struct perf_evsel *counter,
					int nthreads, int ncpus,
					int *ret)
{
	int cpu, thread, i = 0;
	double uval;
	struct perf_aggr_thread_value *buf;

	buf = calloc(nthreads, sizeof(struct perf_aggr_thread_value));
	if (!buf)
		return NULL;

	for (thread = 0; thread < nthreads; thread++) {
		u64 ena = 0, run = 0, val = 0;

		for (cpu = 0; cpu < ncpus; cpu++) {
			val += perf_counts(counter->counts, cpu, thread)->val;
			ena += perf_counts(counter->counts, cpu, thread)->ena;
			run += perf_counts(counter->counts, cpu, thread)->run;
		}

		uval = val * counter->scale;

		/*
		 * Skip value 0 when enabling --per-thread globally,
		 * otherwise too many 0 output.
		 */
		if (uval == 0.0 && target__has_per_thread(&target))
			continue;

		buf[i].counter = counter;
		buf[i].id = thread;
		buf[i].uval = uval;
		buf[i].val = val;
		buf[i].run = run;
		buf[i].ena = ena;
		i++;
	}

	qsort(buf, i, sizeof(struct perf_aggr_thread_value), cmp_val);

	if (ret)
		*ret = i;

	return buf;
}

static void print_aggr_thread(struct perf_evsel *counter, char *prefix)
{
	FILE *output = stat_config.output;
	int nthreads = thread_map__nr(counter->threads);
	int ncpus = cpu_map__nr(counter->cpus);
	int thread, sorted_threads, id;
	struct perf_aggr_thread_value *buf;

	buf = sort_aggr_thread(counter, nthreads, ncpus, &sorted_threads);
	if (!buf) {
		perror("cannot sort aggr thread");
		return;
	}

	for (thread = 0; thread < sorted_threads; thread++) {
		if (prefix)
			fprintf(output, "%s", prefix);

		id = buf[thread].id;
		if (stat_config.stats)
			printout(id, 0, buf[thread].counter, buf[thread].uval,
				 prefix, buf[thread].run, buf[thread].ena, 1.0,
				 &stat_config.stats[id]);
		else
			printout(id, 0, buf[thread].counter, buf[thread].uval,
				 prefix, buf[thread].run, buf[thread].ena, 1.0,
				 &rt_stat);
		fputc('\n', output);
	}

	free(buf);
}

struct caggr_data {
	double avg, avg_enabled, avg_running;
};

static void counter_aggr_cb(struct perf_evsel *counter, void *data,
			    bool first __maybe_unused)
{
	struct caggr_data *cd = data;
	struct perf_stat_evsel *ps = counter->stats;

	cd->avg += avg_stats(&ps->res_stats[0]);
	cd->avg_enabled += avg_stats(&ps->res_stats[1]);
	cd->avg_running += avg_stats(&ps->res_stats[2]);
}

/*
 * Print out the results of a single counter:
 * aggregated counts in system-wide mode
 */
static void print_counter_aggr(struct perf_evsel *counter, char *prefix)
{
	FILE *output = stat_config.output;
	double uval;
	struct caggr_data cd = { .avg = 0.0 };

	if (!collect_data(counter, counter_aggr_cb, &cd))
		return;

	if (prefix && !metric_only)
		fprintf(output, "%s", prefix);

	uval = cd.avg * counter->scale;
	printout(-1, 0, counter, uval, prefix, cd.avg_running, cd.avg_enabled,
		 cd.avg, &rt_stat);
	if (!metric_only)
		fprintf(output, "\n");
}

static void counter_cb(struct perf_evsel *counter, void *data,
		       bool first __maybe_unused)
{
	struct aggr_data *ad = data;

	ad->val += perf_counts(counter->counts, ad->cpu, 0)->val;
	ad->ena += perf_counts(counter->counts, ad->cpu, 0)->ena;
	ad->run += perf_counts(counter->counts, ad->cpu, 0)->run;
}

/*
 * Print out the results of a single counter:
 * does not use aggregated count in system-wide
 */
static void print_counter(struct perf_evsel *counter, char *prefix)
{
	FILE *output = stat_config.output;
	u64 ena, run, val;
	double uval;
	int cpu;

	for (cpu = 0; cpu < perf_evsel__nr_cpus(counter); cpu++) {
		struct aggr_data ad = { .cpu = cpu };

		if (!collect_data(counter, counter_cb, &ad))
			return;
		val = ad.val;
		ena = ad.ena;
		run = ad.run;

		if (prefix)
			fprintf(output, "%s", prefix);

		uval = val * counter->scale;
		printout(cpu, 0, counter, uval, prefix, run, ena, 1.0,
			 &rt_stat);

		fputc('\n', output);
	}
}

static void print_no_aggr_metric(char *prefix)
{
	int cpu;
	int nrcpus = 0;
	struct perf_evsel *counter;
	u64 ena, run, val;
	double uval;

	nrcpus = evsel_list->cpus->nr;
	for (cpu = 0; cpu < nrcpus; cpu++) {
		bool first = true;

		if (prefix)
			fputs(prefix, stat_config.output);
		evlist__for_each_entry(evsel_list, counter) {
			if (is_duration_time(counter))
				continue;
			if (first) {
				aggr_printout(counter, cpu, 0);
				first = false;
			}
			val = perf_counts(counter->counts, cpu, 0)->val;
			ena = perf_counts(counter->counts, cpu, 0)->ena;
			run = perf_counts(counter->counts, cpu, 0)->run;

			uval = val * counter->scale;
			printout(cpu, 0, counter, uval, prefix, run, ena, 1.0,
				 &rt_stat);
		}
		fputc('\n', stat_config.output);
	}
}

static int aggr_header_lens[] = {
	[AGGR_CORE] = 18,
	[AGGR_SOCKET] = 12,
	[AGGR_NONE] = 6,
	[AGGR_THREAD] = 24,
	[AGGR_GLOBAL] = 0,
};

static const char *aggr_header_csv[] = {
	[AGGR_CORE] 	= 	"core,cpus,",
	[AGGR_SOCKET] 	= 	"socket,cpus",
	[AGGR_NONE] 	= 	"cpu,",
	[AGGR_THREAD] 	= 	"comm-pid,",
	[AGGR_GLOBAL] 	=	""
};

static void print_metric_headers(const char *prefix, bool no_indent)
{
	struct perf_stat_output_ctx out;
	struct perf_evsel *counter;
	struct outstate os = {
		.fh = stat_config.output
	};

	if (prefix)
		fprintf(stat_config.output, "%s", prefix);

	if (!csv_output && !no_indent)
		fprintf(stat_config.output, "%*s",
			aggr_header_lens[stat_config.aggr_mode], "");
	if (csv_output) {
		if (stat_config.interval)
			fputs("time,", stat_config.output);
		fputs(aggr_header_csv[stat_config.aggr_mode],
			stat_config.output);
	}

	/* Print metrics headers only */
	evlist__for_each_entry(evsel_list, counter) {
		if (is_duration_time(counter))
			continue;
		os.evsel = counter;
		out.ctx = &os;
		out.print_metric = print_metric_header;
		out.new_line = new_line_metric;
		out.force_header = true;
		os.evsel = counter;
		perf_stat__print_shadow_stats(counter, 0,
					      0,
					      &out,
					      &metric_events,
					      &rt_stat);
	}
	fputc('\n', stat_config.output);
}

static void print_interval(char *prefix, struct timespec *ts)
{
	FILE *output = stat_config.output;
	static int num_print_interval;

	if (interval_clear)
		puts(CONSOLE_CLEAR);

	sprintf(prefix, "%6lu.%09lu%s", ts->tv_sec, ts->tv_nsec, csv_sep);

	if ((num_print_interval == 0 && !csv_output) || interval_clear) {
		switch (stat_config.aggr_mode) {
		case AGGR_SOCKET:
			fprintf(output, "#           time socket cpus");
			if (!metric_only)
				fprintf(output, "             counts %*s events\n", unit_width, "unit");
			break;
		case AGGR_CORE:
			fprintf(output, "#           time core         cpus");
			if (!metric_only)
				fprintf(output, "             counts %*s events\n", unit_width, "unit");
			break;
		case AGGR_NONE:
			fprintf(output, "#           time CPU    ");
			if (!metric_only)
				fprintf(output, "                counts %*s events\n", unit_width, "unit");
			break;
		case AGGR_THREAD:
			fprintf(output, "#           time             comm-pid");
			if (!metric_only)
				fprintf(output, "                  counts %*s events\n", unit_width, "unit");
			break;
		case AGGR_GLOBAL:
		default:
			fprintf(output, "#           time");
			if (!metric_only)
				fprintf(output, "             counts %*s events\n", unit_width, "unit");
		case AGGR_UNSET:
			break;
		}
	}

	if ((num_print_interval == 0 || interval_clear) && metric_only)
		print_metric_headers(" ", true);
	if (++num_print_interval == 25)
		num_print_interval = 0;
}

static void print_header(int argc, const char **argv)
{
	FILE *output = stat_config.output;
	int i;

	fflush(stdout);

	if (!csv_output) {
		fprintf(output, "\n");
		fprintf(output, " Performance counter stats for ");
		if (target.system_wide)
			fprintf(output, "\'system wide");
		else if (target.cpu_list)
			fprintf(output, "\'CPU(s) %s", target.cpu_list);
		else if (!target__has_task(&target)) {
			fprintf(output, "\'%s", argv ? argv[0] : "pipe");
			for (i = 1; argv && (i < argc); i++)
				fprintf(output, " %s", argv[i]);
		} else if (target.pid)
			fprintf(output, "process id \'%s", target.pid);
		else
			fprintf(output, "thread id \'%s", target.tid);

		fprintf(output, "\'");
		if (run_count > 1)
			fprintf(output, " (%d runs)", run_count);
		fprintf(output, ":\n\n");
	}
}

static int get_precision(double num)
{
	if (num > 1)
		return 0;

	return lround(ceil(-log10(num)));
}

static void print_table(FILE *output, int precision, double avg)
{
	char tmp[64];
	int idx, indent = 0;

	scnprintf(tmp, 64, " %17.*f", precision, avg);
	while (tmp[indent] == ' ')
		indent++;

	fprintf(output, "%*s# Table of individual measurements:\n", indent, "");

	for (idx = 0; idx < run_count; idx++) {
		double run = (double) walltime_run[idx] / NSEC_PER_SEC;
		int h, n = 1 + abs((int) (100.0 * (run - avg)/run) / 5);

		fprintf(output, " %17.*f (%+.*f) ",
			precision, run, precision, run - avg);

		for (h = 0; h < n; h++)
			fprintf(output, "#");

		fprintf(output, "\n");
	}

	fprintf(output, "\n%*s# Final result:\n", indent, "");
}

static double timeval2double(struct timeval *t)
{
	return t->tv_sec + (double) t->tv_usec/USEC_PER_SEC;
}

static void print_footer(void)
{
	double avg = avg_stats(&walltime_nsecs_stats) / NSEC_PER_SEC;
	FILE *output = stat_config.output;
	int n;

	if (!null_run)
		fprintf(output, "\n");

	if (run_count == 1) {
		fprintf(output, " %17.9f seconds time elapsed", avg);

		if (ru_display) {
			double ru_utime = timeval2double(&ru_data.ru_utime);
			double ru_stime = timeval2double(&ru_data.ru_stime);

			fprintf(output, "\n\n");
			fprintf(output, " %17.9f seconds user\n", ru_utime);
			fprintf(output, " %17.9f seconds sys\n", ru_stime);
		}
	} else {
		double sd = stddev_stats(&walltime_nsecs_stats) / NSEC_PER_SEC;
		/*
		 * Display at most 2 more significant
		 * digits than the stddev inaccuracy.
		 */
		int precision = get_precision(sd) + 2;

		if (walltime_run_table)
			print_table(output, precision, avg);

		fprintf(output, " %17.*f +- %.*f seconds time elapsed",
			precision, avg, precision, sd);

		print_noise_pct(sd, avg);
	}
	fprintf(output, "\n\n");

	if (print_free_counters_hint &&
	    sysctl__read_int("kernel/nmi_watchdog", &n) >= 0 &&
	    n > 0)
		fprintf(output,
"Some events weren't counted. Try disabling the NMI watchdog:\n"
"	echo 0 > /proc/sys/kernel/nmi_watchdog\n"
"	perf stat ...\n"
"	echo 1 > /proc/sys/kernel/nmi_watchdog\n");

	if (print_mixed_hw_group_error)
		fprintf(output,
			"The events in group usually have to be from "
			"the same PMU. Try reorganizing the group.\n");
}

static void print_counters(struct timespec *ts, int argc, const char **argv)
{
	int interval = stat_config.interval;
	struct perf_evsel *counter;
	char buf[64], *prefix = NULL;

	/* Do not print anything if we record to the pipe. */
	if (STAT_RECORD && perf_stat.data.is_pipe)
		return;

	if (interval)
		print_interval(prefix = buf, ts);
	else
		print_header(argc, argv);

	if (metric_only) {
		static int num_print_iv;

		if (num_print_iv == 0 && !interval)
			print_metric_headers(prefix, false);
		if (num_print_iv++ == 25)
			num_print_iv = 0;
		if (stat_config.aggr_mode == AGGR_GLOBAL && prefix)
			fprintf(stat_config.output, "%s", prefix);
	}

	switch (stat_config.aggr_mode) {
	case AGGR_CORE:
	case AGGR_SOCKET:
		print_aggr(prefix);
		break;
	case AGGR_THREAD:
		evlist__for_each_entry(evsel_list, counter) {
			if (is_duration_time(counter))
				continue;
			print_aggr_thread(counter, prefix);
		}
		break;
	case AGGR_GLOBAL:
		evlist__for_each_entry(evsel_list, counter) {
			if (is_duration_time(counter))
				continue;
			print_counter_aggr(counter, prefix);
		}
		if (metric_only)
			fputc('\n', stat_config.output);
		break;
	case AGGR_NONE:
		if (metric_only)
			print_no_aggr_metric(prefix);
		else {
			evlist__for_each_entry(evsel_list, counter) {
				if (is_duration_time(counter))
					continue;
				print_counter(counter, prefix);
			}
		}
		break;
	case AGGR_UNSET:
	default:
		break;
	}

	if (!interval && !csv_output)
		print_footer();

	fflush(stat_config.output);
=======
static void print_counters(struct timespec *ts, int argc, const char **argv)
{
	/* Do not print anything if we record to the pipe. */
	if (STAT_RECORD && perf_stat.data.is_pipe)
		return;
	if (stat_config.quiet)
		return;

	evlist__print_counters(evsel_list, &stat_config, &target, ts, argc, argv);
>>>>>>> upstream/android-13
}

static volatile int signr = -1;

static void skip_signal(int signo)
{
	if ((child_pid == -1) || stat_config.interval)
		done = 1;

	signr = signo;
	/*
	 * render child_pid harmless
	 * won't send SIGTERM to a random
	 * process in case of race condition
	 * and fast PID recycling
	 */
	child_pid = -1;
}

static void sig_atexit(void)
{
	sigset_t set, oset;

	/*
	 * avoid race condition with SIGCHLD handler
	 * in skip_signal() which is modifying child_pid
	 * goal is to avoid send SIGTERM to a random
	 * process
	 */
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, &oset);

	if (child_pid != -1)
		kill(child_pid, SIGTERM);

	sigprocmask(SIG_SETMASK, &oset, NULL);

	if (signr == -1)
		return;

	signal(signr, SIG_DFL);
	kill(getpid(), signr);
}

<<<<<<< HEAD
=======
void perf_stat__set_big_num(int set)
{
	stat_config.big_num = (set != 0);
}

void perf_stat__set_no_csv_summary(int set)
{
	stat_config.no_csv_summary = (set != 0);
}

>>>>>>> upstream/android-13
static int stat__set_big_num(const struct option *opt __maybe_unused,
			     const char *s __maybe_unused, int unset)
{
	big_num_opt = unset ? 0 : 1;
<<<<<<< HEAD
=======
	perf_stat__set_big_num(!unset);
>>>>>>> upstream/android-13
	return 0;
}

static int enable_metric_only(const struct option *opt __maybe_unused,
			      const char *s __maybe_unused, int unset)
{
	force_metric_only = true;
<<<<<<< HEAD
	metric_only = !unset;
=======
	stat_config.metric_only = !unset;
>>>>>>> upstream/android-13
	return 0;
}

static int parse_metric_groups(const struct option *opt,
			       const char *str,
			       int unset __maybe_unused)
{
<<<<<<< HEAD
	return metricgroup__parse_groups(opt, str, &metric_events);
=======
	return metricgroup__parse_groups(opt, str,
					 stat_config.metric_no_group,
					 stat_config.metric_no_merge,
					 &stat_config.metric_events);
}

static int parse_control_option(const struct option *opt,
				const char *str,
				int unset __maybe_unused)
{
	struct perf_stat_config *config = opt->value;

	return evlist__parse_control(str, &config->ctl_fd, &config->ctl_fd_ack, &config->ctl_fd_close);
}

static int parse_stat_cgroups(const struct option *opt,
			      const char *str, int unset)
{
	if (stat_config.cgroup_list) {
		pr_err("--cgroup and --for-each-cgroup cannot be used together\n");
		return -1;
	}

	return parse_cgroups(opt, str, unset);
>>>>>>> upstream/android-13
}

static struct option stat_options[] = {
	OPT_BOOLEAN('T', "transaction", &transaction_run,
		    "hardware transaction statistics"),
	OPT_CALLBACK('e', "event", &evsel_list, "event",
		     "event selector. use 'perf list' to list available events",
		     parse_events_option),
	OPT_CALLBACK(0, "filter", &evsel_list, "filter",
		     "event filter", parse_filter),
<<<<<<< HEAD
	OPT_BOOLEAN('i', "no-inherit", &no_inherit,
=======
	OPT_BOOLEAN('i', "no-inherit", &stat_config.no_inherit,
>>>>>>> upstream/android-13
		    "child tasks do not inherit counters"),
	OPT_STRING('p', "pid", &target.pid, "pid",
		   "stat events on existing process id"),
	OPT_STRING('t', "tid", &target.tid, "tid",
		   "stat events on existing thread id"),
<<<<<<< HEAD
=======
#ifdef HAVE_BPF_SKEL
	OPT_STRING('b', "bpf-prog", &target.bpf_str, "bpf-prog-id",
		   "stat events on existing bpf program id"),
	OPT_BOOLEAN(0, "bpf-counters", &target.use_bpf,
		    "use bpf program to count events"),
	OPT_STRING(0, "bpf-attr-map", &target.attr_map, "attr-map-path",
		   "path to perf_event_attr map"),
#endif
>>>>>>> upstream/android-13
	OPT_BOOLEAN('a', "all-cpus", &target.system_wide,
		    "system-wide collection from all CPUs"),
	OPT_BOOLEAN('g', "group", &group,
		    "put the counters into a counter group"),
<<<<<<< HEAD
	OPT_BOOLEAN('c', "scale", &stat_config.scale, "scale/normalize counters"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show counter open errors, etc)"),
	OPT_INTEGER('r', "repeat", &run_count,
		    "repeat command and print average + stddev (max: 100, forever: 0)"),
	OPT_BOOLEAN(0, "table", &walltime_run_table,
		    "display details about each run (only with -r option)"),
	OPT_BOOLEAN('n', "null", &null_run,
=======
	OPT_BOOLEAN(0, "scale", &stat_config.scale,
		    "Use --no-scale to disable counter scaling for multiplexing"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show counter open errors, etc)"),
	OPT_INTEGER('r', "repeat", &stat_config.run_count,
		    "repeat command and print average + stddev (max: 100, forever: 0)"),
	OPT_BOOLEAN(0, "table", &stat_config.walltime_run_table,
		    "display details about each run (only with -r option)"),
	OPT_BOOLEAN('n', "null", &stat_config.null_run,
>>>>>>> upstream/android-13
		    "null run - dont start any counters"),
	OPT_INCR('d', "detailed", &detailed_run,
		    "detailed run - start a lot of events"),
	OPT_BOOLEAN('S', "sync", &sync_run,
		    "call sync() before starting a run"),
	OPT_CALLBACK_NOOPT('B', "big-num", NULL, NULL,
			   "print large numbers with thousands\' separators",
			   stat__set_big_num),
	OPT_STRING('C', "cpu", &target.cpu_list, "cpu",
		    "list of cpus to monitor in system-wide"),
	OPT_SET_UINT('A', "no-aggr", &stat_config.aggr_mode,
		    "disable CPU count aggregation", AGGR_NONE),
<<<<<<< HEAD
	OPT_BOOLEAN(0, "no-merge", &no_merge, "Do not merge identical named events"),
	OPT_STRING('x', "field-separator", &csv_sep, "separator",
		   "print counts with custom separator"),
	OPT_CALLBACK('G', "cgroup", &evsel_list, "name",
		     "monitor event in cgroup name only", parse_cgroups),
=======
	OPT_BOOLEAN(0, "no-merge", &stat_config.no_merge, "Do not merge identical named events"),
	OPT_STRING('x', "field-separator", &stat_config.csv_sep, "separator",
		   "print counts with custom separator"),
	OPT_CALLBACK('G', "cgroup", &evsel_list, "name",
		     "monitor event in cgroup name only", parse_stat_cgroups),
	OPT_STRING(0, "for-each-cgroup", &stat_config.cgroup_list, "name",
		    "expand events for each cgroup"),
>>>>>>> upstream/android-13
	OPT_STRING('o', "output", &output_name, "file", "output file name"),
	OPT_BOOLEAN(0, "append", &append_file, "append to the output file"),
	OPT_INTEGER(0, "log-fd", &output_fd,
		    "log output to fd, instead of stderr"),
	OPT_STRING(0, "pre", &pre_cmd, "command",
			"command to run prior to the measured command"),
	OPT_STRING(0, "post", &post_cmd, "command",
			"command to run after to the measured command"),
	OPT_UINTEGER('I', "interval-print", &stat_config.interval,
		    "print counts at regular interval in ms "
		    "(overhead is possible for values <= 100ms)"),
	OPT_INTEGER(0, "interval-count", &stat_config.times,
		    "print counts for fixed number of times"),
<<<<<<< HEAD
	OPT_BOOLEAN(0, "interval-clear", &interval_clear,
=======
	OPT_BOOLEAN(0, "interval-clear", &stat_config.interval_clear,
>>>>>>> upstream/android-13
		    "clear screen in between new interval"),
	OPT_UINTEGER(0, "timeout", &stat_config.timeout,
		    "stop workload and print counts after a timeout period in ms (>= 10ms)"),
	OPT_SET_UINT(0, "per-socket", &stat_config.aggr_mode,
		     "aggregate counts per processor socket", AGGR_SOCKET),
<<<<<<< HEAD
=======
	OPT_SET_UINT(0, "per-die", &stat_config.aggr_mode,
		     "aggregate counts per processor die", AGGR_DIE),
>>>>>>> upstream/android-13
	OPT_SET_UINT(0, "per-core", &stat_config.aggr_mode,
		     "aggregate counts per physical processor core", AGGR_CORE),
	OPT_SET_UINT(0, "per-thread", &stat_config.aggr_mode,
		     "aggregate counts per thread", AGGR_THREAD),
<<<<<<< HEAD
	OPT_UINTEGER('D', "delay", &initial_delay,
		     "ms to wait before starting measurement after program start"),
	OPT_CALLBACK_NOOPT(0, "metric-only", &metric_only, NULL,
			"Only print computed metrics. No raw values", enable_metric_only),
	OPT_BOOLEAN(0, "topdown", &topdown_run,
			"measure topdown level 1 statistics"),
=======
	OPT_SET_UINT(0, "per-node", &stat_config.aggr_mode,
		     "aggregate counts per numa node", AGGR_NODE),
	OPT_INTEGER('D', "delay", &stat_config.initial_delay,
		    "ms to wait before starting measurement after program start (-1: start with events disabled)"),
	OPT_CALLBACK_NOOPT(0, "metric-only", &stat_config.metric_only, NULL,
			"Only print computed metrics. No raw values", enable_metric_only),
	OPT_BOOLEAN(0, "metric-no-group", &stat_config.metric_no_group,
		       "don't group metric events, impacts multiplexing"),
	OPT_BOOLEAN(0, "metric-no-merge", &stat_config.metric_no_merge,
		       "don't try to share events between metrics in a group"),
	OPT_BOOLEAN(0, "topdown", &topdown_run,
			"measure top-down statistics"),
	OPT_UINTEGER(0, "td-level", &stat_config.topdown_level,
			"Set the metrics level for the top-down statistics (0: max level)"),
>>>>>>> upstream/android-13
	OPT_BOOLEAN(0, "smi-cost", &smi_cost,
			"measure SMI cost"),
	OPT_CALLBACK('M', "metrics", &evsel_list, "metric/metric group list",
		     "monitor specified metrics or metric groups (separated by ,)",
		     parse_metric_groups),
<<<<<<< HEAD
	OPT_END()
};

static int perf_stat__get_socket(struct cpu_map *map, int cpu)
=======
	OPT_BOOLEAN_FLAG(0, "all-kernel", &stat_config.all_kernel,
			 "Configure all used events to run in kernel space.",
			 PARSE_OPT_EXCLUSIVE),
	OPT_BOOLEAN_FLAG(0, "all-user", &stat_config.all_user,
			 "Configure all used events to run in user space.",
			 PARSE_OPT_EXCLUSIVE),
	OPT_BOOLEAN(0, "percore-show-thread", &stat_config.percore_show_thread,
		    "Use with 'percore' event qualifier to show the event "
		    "counts of one hardware thread by sum up total hardware "
		    "threads of same physical core"),
	OPT_BOOLEAN(0, "summary", &stat_config.summary,
		       "print summary for interval mode"),
	OPT_BOOLEAN(0, "no-csv-summary", &stat_config.no_csv_summary,
		       "don't print 'summary' for CSV summary output"),
	OPT_BOOLEAN(0, "quiet", &stat_config.quiet,
			"don't print output (useful with record)"),
#ifdef HAVE_LIBPFM
	OPT_CALLBACK(0, "pfm-events", &evsel_list, "event",
		"libpfm4 event selector. use 'perf list' to list available events",
		parse_libpfm_events_option),
#endif
	OPT_CALLBACK(0, "control", &stat_config, "fd:ctl-fd[,ack-fd] or fifo:ctl-fifo[,ack-fifo]",
		     "Listen on ctl-fd descriptor for command to control measurement ('enable': enable events, 'disable': disable events).\n"
		     "\t\t\t  Optionally send control command completion ('ack\\n') to ack-fd descriptor.\n"
		     "\t\t\t  Alternatively, ctl-fifo / ack-fifo will be opened and used as ctl-fd / ack-fd.",
		      parse_control_option),
	OPT_CALLBACK_OPTARG(0, "iostat", &evsel_list, &stat_config, "default",
			    "measure I/O performance metrics provided by arch/platform",
			    iostat_parse),
	OPT_END()
};

static struct aggr_cpu_id perf_stat__get_socket(struct perf_stat_config *config __maybe_unused,
				 struct perf_cpu_map *map, int cpu)
>>>>>>> upstream/android-13
{
	return cpu_map__get_socket(map, cpu, NULL);
}

<<<<<<< HEAD
static int perf_stat__get_core(struct cpu_map *map, int cpu)
=======
static struct aggr_cpu_id perf_stat__get_die(struct perf_stat_config *config __maybe_unused,
			      struct perf_cpu_map *map, int cpu)
{
	return cpu_map__get_die(map, cpu, NULL);
}

static struct aggr_cpu_id perf_stat__get_core(struct perf_stat_config *config __maybe_unused,
			       struct perf_cpu_map *map, int cpu)
>>>>>>> upstream/android-13
{
	return cpu_map__get_core(map, cpu, NULL);
}

<<<<<<< HEAD
static int cpu_map__get_max(struct cpu_map *map)
{
	int i, max = -1;

	for (i = 0; i < map->nr; i++) {
		if (map->map[i] > max)
			max = map->map[i];
	}

	return max;
}

static struct cpu_map *cpus_aggr_map;

static int perf_stat__get_aggr(aggr_get_id_t get_id, struct cpu_map *map, int idx)
{
	int cpu;

	if (idx >= map->nr)
		return -1;

	cpu = map->map[idx];

	if (cpus_aggr_map->map[cpu] == -1)
		cpus_aggr_map->map[cpu] = get_id(map, idx);

	return cpus_aggr_map->map[cpu];
}

static int perf_stat__get_socket_cached(struct cpu_map *map, int idx)
{
	return perf_stat__get_aggr(perf_stat__get_socket, map, idx);
}

static int perf_stat__get_core_cached(struct cpu_map *map, int idx)
{
	return perf_stat__get_aggr(perf_stat__get_core, map, idx);
=======
static struct aggr_cpu_id perf_stat__get_node(struct perf_stat_config *config __maybe_unused,
			       struct perf_cpu_map *map, int cpu)
{
	return cpu_map__get_node(map, cpu, NULL);
}

static struct aggr_cpu_id perf_stat__get_aggr(struct perf_stat_config *config,
			       aggr_get_id_t get_id, struct perf_cpu_map *map, int idx)
{
	int cpu;
	struct aggr_cpu_id id = cpu_map__empty_aggr_cpu_id();

	if (idx >= map->nr)
		return id;

	cpu = map->map[idx];

	if (cpu_map__aggr_cpu_id_is_empty(config->cpus_aggr_map->map[cpu]))
		config->cpus_aggr_map->map[cpu] = get_id(config, map, idx);

	id = config->cpus_aggr_map->map[cpu];
	return id;
}

static struct aggr_cpu_id perf_stat__get_socket_cached(struct perf_stat_config *config,
					struct perf_cpu_map *map, int idx)
{
	return perf_stat__get_aggr(config, perf_stat__get_socket, map, idx);
}

static struct aggr_cpu_id perf_stat__get_die_cached(struct perf_stat_config *config,
					struct perf_cpu_map *map, int idx)
{
	return perf_stat__get_aggr(config, perf_stat__get_die, map, idx);
}

static struct aggr_cpu_id perf_stat__get_core_cached(struct perf_stat_config *config,
				      struct perf_cpu_map *map, int idx)
{
	return perf_stat__get_aggr(config, perf_stat__get_core, map, idx);
}

static struct aggr_cpu_id perf_stat__get_node_cached(struct perf_stat_config *config,
				      struct perf_cpu_map *map, int idx)
{
	return perf_stat__get_aggr(config, perf_stat__get_node, map, idx);
}

static bool term_percore_set(void)
{
	struct evsel *counter;

	evlist__for_each_entry(evsel_list, counter) {
		if (counter->percore)
			return true;
	}

	return false;
>>>>>>> upstream/android-13
}

static int perf_stat_init_aggr_mode(void)
{
	int nr;

	switch (stat_config.aggr_mode) {
	case AGGR_SOCKET:
<<<<<<< HEAD
		if (cpu_map__build_socket_map(evsel_list->cpus, &aggr_map)) {
			perror("cannot build socket map");
			return -1;
		}
		aggr_get_id = perf_stat__get_socket_cached;
		break;
	case AGGR_CORE:
		if (cpu_map__build_core_map(evsel_list->cpus, &aggr_map)) {
			perror("cannot build core map");
			return -1;
		}
		aggr_get_id = perf_stat__get_core_cached;
		break;
	case AGGR_NONE:
=======
		if (cpu_map__build_socket_map(evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build socket map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_socket_cached;
		break;
	case AGGR_DIE:
		if (cpu_map__build_die_map(evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build die map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_die_cached;
		break;
	case AGGR_CORE:
		if (cpu_map__build_core_map(evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build core map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_core_cached;
		break;
	case AGGR_NODE:
		if (cpu_map__build_node_map(evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build core map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_node_cached;
		break;
	case AGGR_NONE:
		if (term_percore_set()) {
			if (cpu_map__build_core_map(evsel_list->core.cpus,
						    &stat_config.aggr_map)) {
				perror("cannot build core map");
				return -1;
			}
			stat_config.aggr_get_id = perf_stat__get_core_cached;
		}
		break;
>>>>>>> upstream/android-13
	case AGGR_GLOBAL:
	case AGGR_THREAD:
	case AGGR_UNSET:
	default:
		break;
	}

	/*
	 * The evsel_list->cpus is the base we operate on,
	 * taking the highest cpu number to be the size of
	 * the aggregation translate cpumap.
	 */
<<<<<<< HEAD
	nr = cpu_map__get_max(evsel_list->cpus);
	cpus_aggr_map = cpu_map__empty_new(nr + 1);
	return cpus_aggr_map ? 0 : -ENOMEM;
=======
	nr = perf_cpu_map__max(evsel_list->core.cpus);
	stat_config.cpus_aggr_map = cpu_aggr_map__empty_new(nr + 1);
	return stat_config.cpus_aggr_map ? 0 : -ENOMEM;
}

static void cpu_aggr_map__delete(struct cpu_aggr_map *map)
{
	if (map) {
		WARN_ONCE(refcount_read(&map->refcnt) != 0,
			  "cpu_aggr_map refcnt unbalanced\n");
		free(map);
	}
}

static void cpu_aggr_map__put(struct cpu_aggr_map *map)
{
	if (map && refcount_dec_and_test(&map->refcnt))
		cpu_aggr_map__delete(map);
>>>>>>> upstream/android-13
}

static void perf_stat__exit_aggr_mode(void)
{
<<<<<<< HEAD
	cpu_map__put(aggr_map);
	cpu_map__put(cpus_aggr_map);
	aggr_map = NULL;
	cpus_aggr_map = NULL;
}

static inline int perf_env__get_cpu(struct perf_env *env, struct cpu_map *map, int idx)
=======
	cpu_aggr_map__put(stat_config.aggr_map);
	cpu_aggr_map__put(stat_config.cpus_aggr_map);
	stat_config.aggr_map = NULL;
	stat_config.cpus_aggr_map = NULL;
}

static inline int perf_env__get_cpu(struct perf_env *env, struct perf_cpu_map *map, int idx)
>>>>>>> upstream/android-13
{
	int cpu;

	if (idx > map->nr)
		return -1;

	cpu = map->map[idx];

	if (cpu >= env->nr_cpus_avail)
		return -1;

	return cpu;
}

<<<<<<< HEAD
static int perf_env__get_socket(struct cpu_map *map, int idx, void *data)
{
	struct perf_env *env = data;
	int cpu = perf_env__get_cpu(env, map, idx);

	return cpu == -1 ? -1 : env->cpu[cpu].socket_id;
}

static int perf_env__get_core(struct cpu_map *map, int idx, void *data)
{
	struct perf_env *env = data;
	int core = -1, cpu = perf_env__get_cpu(env, map, idx);

	if (cpu != -1) {
		int socket_id = env->cpu[cpu].socket_id;

		/*
		 * Encode socket in upper 16 bits
		 * core_id is relative to socket, and
		 * we need a global id. So we combine
		 * socket + core id.
		 */
		core = (socket_id << 16) | (env->cpu[cpu].core_id & 0xffff);
	}

	return core;
}

static int perf_env__build_socket_map(struct perf_env *env, struct cpu_map *cpus,
				      struct cpu_map **sockp)
=======
static struct aggr_cpu_id perf_env__get_socket(struct perf_cpu_map *map, int idx, void *data)
{
	struct perf_env *env = data;
	int cpu = perf_env__get_cpu(env, map, idx);
	struct aggr_cpu_id id = cpu_map__empty_aggr_cpu_id();

	if (cpu != -1)
		id.socket = env->cpu[cpu].socket_id;

	return id;
}

static struct aggr_cpu_id perf_env__get_die(struct perf_cpu_map *map, int idx, void *data)
{
	struct perf_env *env = data;
	struct aggr_cpu_id id = cpu_map__empty_aggr_cpu_id();
	int cpu = perf_env__get_cpu(env, map, idx);

	if (cpu != -1) {
		/*
		 * die_id is relative to socket, so start
		 * with the socket ID and then add die to
		 * make a unique ID.
		 */
		id.socket = env->cpu[cpu].socket_id;
		id.die = env->cpu[cpu].die_id;
	}

	return id;
}

static struct aggr_cpu_id perf_env__get_core(struct perf_cpu_map *map, int idx, void *data)
{
	struct perf_env *env = data;
	struct aggr_cpu_id id = cpu_map__empty_aggr_cpu_id();
	int cpu = perf_env__get_cpu(env, map, idx);

	if (cpu != -1) {
		/*
		 * core_id is relative to socket and die,
		 * we need a global id. So we set
		 * socket, die id and core id
		 */
		id.socket = env->cpu[cpu].socket_id;
		id.die = env->cpu[cpu].die_id;
		id.core = env->cpu[cpu].core_id;
	}

	return id;
}

static struct aggr_cpu_id perf_env__get_node(struct perf_cpu_map *map, int idx, void *data)
{
	int cpu = perf_env__get_cpu(data, map, idx);
	struct aggr_cpu_id id = cpu_map__empty_aggr_cpu_id();

	id.node = perf_env__numa_node(data, cpu);
	return id;
}

static int perf_env__build_socket_map(struct perf_env *env, struct perf_cpu_map *cpus,
				      struct cpu_aggr_map **sockp)
>>>>>>> upstream/android-13
{
	return cpu_map__build_map(cpus, sockp, perf_env__get_socket, env);
}

<<<<<<< HEAD
static int perf_env__build_core_map(struct perf_env *env, struct cpu_map *cpus,
				    struct cpu_map **corep)
=======
static int perf_env__build_die_map(struct perf_env *env, struct perf_cpu_map *cpus,
				   struct cpu_aggr_map **diep)
{
	return cpu_map__build_map(cpus, diep, perf_env__get_die, env);
}

static int perf_env__build_core_map(struct perf_env *env, struct perf_cpu_map *cpus,
				    struct cpu_aggr_map **corep)
>>>>>>> upstream/android-13
{
	return cpu_map__build_map(cpus, corep, perf_env__get_core, env);
}

<<<<<<< HEAD
static int perf_stat__get_socket_file(struct cpu_map *map, int idx)
{
	return perf_env__get_socket(map, idx, &perf_stat.session->header.env);
}

static int perf_stat__get_core_file(struct cpu_map *map, int idx)
=======
static int perf_env__build_node_map(struct perf_env *env, struct perf_cpu_map *cpus,
				    struct cpu_aggr_map **nodep)
{
	return cpu_map__build_map(cpus, nodep, perf_env__get_node, env);
}

static struct aggr_cpu_id perf_stat__get_socket_file(struct perf_stat_config *config __maybe_unused,
				      struct perf_cpu_map *map, int idx)
{
	return perf_env__get_socket(map, idx, &perf_stat.session->header.env);
}
static struct aggr_cpu_id perf_stat__get_die_file(struct perf_stat_config *config __maybe_unused,
				   struct perf_cpu_map *map, int idx)
{
	return perf_env__get_die(map, idx, &perf_stat.session->header.env);
}

static struct aggr_cpu_id perf_stat__get_core_file(struct perf_stat_config *config __maybe_unused,
				    struct perf_cpu_map *map, int idx)
>>>>>>> upstream/android-13
{
	return perf_env__get_core(map, idx, &perf_stat.session->header.env);
}

<<<<<<< HEAD
=======
static struct aggr_cpu_id perf_stat__get_node_file(struct perf_stat_config *config __maybe_unused,
				    struct perf_cpu_map *map, int idx)
{
	return perf_env__get_node(map, idx, &perf_stat.session->header.env);
}

>>>>>>> upstream/android-13
static int perf_stat_init_aggr_mode_file(struct perf_stat *st)
{
	struct perf_env *env = &st->session->header.env;

	switch (stat_config.aggr_mode) {
	case AGGR_SOCKET:
<<<<<<< HEAD
		if (perf_env__build_socket_map(env, evsel_list->cpus, &aggr_map)) {
			perror("cannot build socket map");
			return -1;
		}
		aggr_get_id = perf_stat__get_socket_file;
		break;
	case AGGR_CORE:
		if (perf_env__build_core_map(env, evsel_list->cpus, &aggr_map)) {
			perror("cannot build core map");
			return -1;
		}
		aggr_get_id = perf_stat__get_core_file;
=======
		if (perf_env__build_socket_map(env, evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build socket map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_socket_file;
		break;
	case AGGR_DIE:
		if (perf_env__build_die_map(env, evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build die map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_die_file;
		break;
	case AGGR_CORE:
		if (perf_env__build_core_map(env, evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build core map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_core_file;
		break;
	case AGGR_NODE:
		if (perf_env__build_node_map(env, evsel_list->core.cpus, &stat_config.aggr_map)) {
			perror("cannot build core map");
			return -1;
		}
		stat_config.aggr_get_id = perf_stat__get_node_file;
>>>>>>> upstream/android-13
		break;
	case AGGR_NONE:
	case AGGR_GLOBAL:
	case AGGR_THREAD:
	case AGGR_UNSET:
	default:
		break;
	}

	return 0;
}

<<<<<<< HEAD
static int topdown_filter_events(const char **attr, char **str, bool use_group)
{
	int off = 0;
	int i;
	int len = 0;
	char *s;

	for (i = 0; attr[i]; i++) {
		if (pmu_have_event("cpu", attr[i])) {
			len += strlen(attr[i]) + 1;
			attr[i - off] = attr[i];
		} else
			off++;
	}
	attr[i - off] = NULL;

	*str = malloc(len + 1 + 2);
	if (!*str)
		return -1;
	s = *str;
	if (i - off == 0) {
		*s = 0;
		return 0;
	}
	if (use_group)
		*s++ = '{';
	for (i = 0; attr[i]; i++) {
		strcpy(s, attr[i]);
		s += strlen(s);
		*s++ = ',';
	}
	if (use_group) {
		s[-1] = '}';
		*s = 0;
	} else
		s[-1] = 0;
	return 0;
}

__weak bool arch_topdown_check_group(bool *warn)
{
	*warn = false;
	return false;
}

__weak void arch_topdown_group_warn(void)
{
}

=======
>>>>>>> upstream/android-13
/*
 * Add default attributes, if there were no attributes specified or
 * if -d/--detailed, -d -d or -d -d -d is used:
 */
static int add_default_attributes(void)
{
	int err;
	struct perf_event_attr default_attrs0[] = {

  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_TASK_CLOCK		},
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CONTEXT_SWITCHES	},
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_MIGRATIONS		},
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_PAGE_FAULTS		},

  { .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_CPU_CYCLES		},
};
	struct perf_event_attr frontend_attrs[] = {
  { .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND	},
};
	struct perf_event_attr backend_attrs[] = {
  { .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND	},
};
	struct perf_event_attr default_attrs1[] = {
  { .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_INSTRUCTIONS		},
  { .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS	},
  { .type = PERF_TYPE_HARDWARE, .config = PERF_COUNT_HW_BRANCH_MISSES		},

};
<<<<<<< HEAD
=======
	struct perf_event_attr default_sw_attrs[] = {
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_TASK_CLOCK		},
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CONTEXT_SWITCHES	},
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_CPU_MIGRATIONS		},
  { .type = PERF_TYPE_SOFTWARE, .config = PERF_COUNT_SW_PAGE_FAULTS		},
};
>>>>>>> upstream/android-13

/*
 * Detailed stats (-d), covering the L1 and last level data caches:
 */
	struct perf_event_attr detailed_attrs[] = {

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_LL			<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_LL			<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
};

/*
 * Very detailed stats (-d -d), covering the instruction cache and the TLB caches:
 */
	struct perf_event_attr very_detailed_attrs[] = {

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1I		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1I		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_DTLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_DTLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_ITLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_ITLB		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_READ		<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},

};

/*
 * Very, very detailed stats (-d -d -d), adding prefetch events:
 */
	struct perf_event_attr very_very_detailed_attrs[] = {

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_PREFETCH	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_ACCESS	<< 16)				},

  { .type = PERF_TYPE_HW_CACHE,
    .config =
	 PERF_COUNT_HW_CACHE_L1D		<<  0  |
	(PERF_COUNT_HW_CACHE_OP_PREFETCH	<<  8) |
	(PERF_COUNT_HW_CACHE_RESULT_MISS	<< 16)				},
};
	struct parse_events_error errinfo;

	/* Set attrs if no event is selected and !null_run: */
<<<<<<< HEAD
	if (null_run)
		return 0;

	if (transaction_run) {
		/* Handle -T as -M transaction. Once platform specific metrics
		 * support has been added to the json files, all archictures
=======
	if (stat_config.null_run)
		return 0;

	bzero(&errinfo, sizeof(errinfo));
	if (transaction_run) {
		/* Handle -T as -M transaction. Once platform specific metrics
		 * support has been added to the json files, all architectures
>>>>>>> upstream/android-13
		 * will use this approach. To determine transaction support
		 * on an architecture test for such a metric name.
		 */
		if (metricgroup__has_metric("transaction")) {
			struct option opt = { .value = &evsel_list };

			return metricgroup__parse_groups(&opt, "transaction",
<<<<<<< HEAD
							 &metric_events);
=======
							 stat_config.metric_no_group,
							stat_config.metric_no_merge,
							 &stat_config.metric_events);
>>>>>>> upstream/android-13
		}

		if (pmu_have_event("cpu", "cycles-ct") &&
		    pmu_have_event("cpu", "el-start"))
			err = parse_events(evsel_list, transaction_attrs,
					   &errinfo);
		else
			err = parse_events(evsel_list,
					   transaction_limited_attrs,
					   &errinfo);
		if (err) {
			fprintf(stderr, "Cannot set up transaction events\n");
			parse_events_print_error(&errinfo, transaction_attrs);
			return -1;
		}
		return 0;
	}

	if (smi_cost) {
		int smi;

		if (sysfs__read_int(FREEZE_ON_SMI_PATH, &smi) < 0) {
			fprintf(stderr, "freeze_on_smi is not supported.\n");
			return -1;
		}

		if (!smi) {
			if (sysfs__write_int(FREEZE_ON_SMI_PATH, 1) < 0) {
				fprintf(stderr, "Failed to set freeze_on_smi.\n");
				return -1;
			}
			smi_reset = true;
		}

		if (pmu_have_event("msr", "aperf") &&
		    pmu_have_event("msr", "smi")) {
			if (!force_metric_only)
<<<<<<< HEAD
				metric_only = true;
=======
				stat_config.metric_only = true;
>>>>>>> upstream/android-13
			err = parse_events(evsel_list, smi_cost_attrs, &errinfo);
		} else {
			fprintf(stderr, "To measure SMI cost, it needs "
				"msr/aperf/, msr/smi/ and cpu/cycles/ support\n");
			parse_events_print_error(&errinfo, smi_cost_attrs);
			return -1;
		}
		if (err) {
<<<<<<< HEAD
=======
			parse_events_print_error(&errinfo, smi_cost_attrs);
>>>>>>> upstream/android-13
			fprintf(stderr, "Cannot set up SMI cost events\n");
			return -1;
		}
		return 0;
	}

	if (topdown_run) {
<<<<<<< HEAD
		char *str = NULL;
		bool warn = false;

=======
		const char **metric_attrs = topdown_metric_attrs;
		unsigned int max_level = 1;
		char *str = NULL;
		bool warn = false;

		if (!force_metric_only)
			stat_config.metric_only = true;

		if (pmu_have_event("cpu", topdown_metric_L2_attrs[5])) {
			metric_attrs = topdown_metric_L2_attrs;
			max_level = 2;
		}

		if (stat_config.topdown_level > max_level) {
			pr_err("Invalid top-down metrics level. The max level is %u.\n", max_level);
			return -1;
		} else if (!stat_config.topdown_level)
			stat_config.topdown_level = max_level;

		if (topdown_filter_events(metric_attrs, &str, 1) < 0) {
			pr_err("Out of memory\n");
			return -1;
		}
		if (metric_attrs[0] && str) {
			if (!stat_config.interval && !stat_config.metric_only) {
				fprintf(stat_config.output,
					"Topdown accuracy may decrease when measuring long periods.\n"
					"Please print the result regularly, e.g. -I1000\n");
			}
			goto setup_metrics;
		}

		zfree(&str);

>>>>>>> upstream/android-13
		if (stat_config.aggr_mode != AGGR_GLOBAL &&
		    stat_config.aggr_mode != AGGR_CORE) {
			pr_err("top down event configuration requires --per-core mode\n");
			return -1;
		}
		stat_config.aggr_mode = AGGR_CORE;
		if (nr_cgroups || !target__has_cpu(&target)) {
			pr_err("top down event configuration requires system-wide mode (-a)\n");
			return -1;
		}

<<<<<<< HEAD
		if (!force_metric_only)
			metric_only = true;
=======
>>>>>>> upstream/android-13
		if (topdown_filter_events(topdown_attrs, &str,
				arch_topdown_check_group(&warn)) < 0) {
			pr_err("Out of memory\n");
			return -1;
		}
		if (topdown_attrs[0] && str) {
			if (warn)
				arch_topdown_group_warn();
<<<<<<< HEAD
=======
setup_metrics:
>>>>>>> upstream/android-13
			err = parse_events(evsel_list, str, &errinfo);
			if (err) {
				fprintf(stderr,
					"Cannot set up top down events %s: %d\n",
					str, err);
				parse_events_print_error(&errinfo, str);
				free(str);
				return -1;
			}
		} else {
			fprintf(stderr, "System does not support topdown\n");
			return -1;
		}
		free(str);
	}

<<<<<<< HEAD
	if (!evsel_list->nr_entries) {
		if (target__has_cpu(&target))
			default_attrs0[0].config = PERF_COUNT_SW_CPU_CLOCK;

		if (perf_evlist__add_default_attrs(evsel_list, default_attrs0) < 0)
			return -1;
		if (pmu_have_event("cpu", "stalled-cycles-frontend")) {
			if (perf_evlist__add_default_attrs(evsel_list,
						frontend_attrs) < 0)
				return -1;
		}
		if (pmu_have_event("cpu", "stalled-cycles-backend")) {
			if (perf_evlist__add_default_attrs(evsel_list,
						backend_attrs) < 0)
				return -1;
		}
		if (perf_evlist__add_default_attrs(evsel_list, default_attrs1) < 0)
=======
	if (!evsel_list->core.nr_entries) {
		if (perf_pmu__has_hybrid()) {
			const char *hybrid_str = "cycles,instructions,branches,branch-misses";

			if (target__has_cpu(&target))
				default_sw_attrs[0].config = PERF_COUNT_SW_CPU_CLOCK;

			if (evlist__add_default_attrs(evsel_list,
						      default_sw_attrs) < 0) {
				return -1;
			}

			err = parse_events(evsel_list, hybrid_str, &errinfo);
			if (err) {
				fprintf(stderr,
					"Cannot set up hybrid events %s: %d\n",
					hybrid_str, err);
				parse_events_print_error(&errinfo, hybrid_str);
				return -1;
			}
			return err;
		}

		if (target__has_cpu(&target))
			default_attrs0[0].config = PERF_COUNT_SW_CPU_CLOCK;

		if (evlist__add_default_attrs(evsel_list, default_attrs0) < 0)
			return -1;
		if (pmu_have_event("cpu", "stalled-cycles-frontend")) {
			if (evlist__add_default_attrs(evsel_list, frontend_attrs) < 0)
				return -1;
		}
		if (pmu_have_event("cpu", "stalled-cycles-backend")) {
			if (evlist__add_default_attrs(evsel_list, backend_attrs) < 0)
				return -1;
		}
		if (evlist__add_default_attrs(evsel_list, default_attrs1) < 0)
			return -1;

		stat_config.topdown_level = TOPDOWN_MAX_LEVEL;
		if (arch_evlist__add_default_attrs(evsel_list) < 0)
>>>>>>> upstream/android-13
			return -1;
	}

	/* Detailed events get appended to the event list: */

	if (detailed_run <  1)
		return 0;

	/* Append detailed run extra attributes: */
<<<<<<< HEAD
	if (perf_evlist__add_default_attrs(evsel_list, detailed_attrs) < 0)
=======
	if (evlist__add_default_attrs(evsel_list, detailed_attrs) < 0)
>>>>>>> upstream/android-13
		return -1;

	if (detailed_run < 2)
		return 0;

	/* Append very detailed run extra attributes: */
<<<<<<< HEAD
	if (perf_evlist__add_default_attrs(evsel_list, very_detailed_attrs) < 0)
=======
	if (evlist__add_default_attrs(evsel_list, very_detailed_attrs) < 0)
>>>>>>> upstream/android-13
		return -1;

	if (detailed_run < 3)
		return 0;

	/* Append very, very detailed run extra attributes: */
<<<<<<< HEAD
	return perf_evlist__add_default_attrs(evsel_list, very_very_detailed_attrs);
=======
	return evlist__add_default_attrs(evsel_list, very_very_detailed_attrs);
>>>>>>> upstream/android-13
}

static const char * const stat_record_usage[] = {
	"perf stat record [<options>]",
	NULL,
};

static void init_features(struct perf_session *session)
{
	int feat;

	for (feat = HEADER_FIRST_FEATURE; feat < HEADER_LAST_FEATURE; feat++)
		perf_header__set_feat(&session->header, feat);

<<<<<<< HEAD
=======
	perf_header__clear_feat(&session->header, HEADER_DIR_FORMAT);
>>>>>>> upstream/android-13
	perf_header__clear_feat(&session->header, HEADER_BUILD_ID);
	perf_header__clear_feat(&session->header, HEADER_TRACING_DATA);
	perf_header__clear_feat(&session->header, HEADER_BRANCH_STACK);
	perf_header__clear_feat(&session->header, HEADER_AUXTRACE);
}

static int __cmd_record(int argc, const char **argv)
{
	struct perf_session *session;
	struct perf_data *data = &perf_stat.data;

	argc = parse_options(argc, argv, stat_options, stat_record_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	if (output_name)
<<<<<<< HEAD
		data->file.path = output_name;

	if (run_count != 1 || forever) {
=======
		data->path = output_name;

	if (stat_config.run_count != 1 || forever) {
>>>>>>> upstream/android-13
		pr_err("Cannot use -r option with perf stat record.\n");
		return -1;
	}

<<<<<<< HEAD
	session = perf_session__new(data, false, NULL);
	if (session == NULL) {
		pr_err("Perf session creation failed.\n");
		return -1;
=======
	session = perf_session__new(data, NULL);
	if (IS_ERR(session)) {
		pr_err("Perf session creation failed\n");
		return PTR_ERR(session);
>>>>>>> upstream/android-13
	}

	init_features(session);

	session->evlist   = evsel_list;
	perf_stat.session = session;
	perf_stat.record  = true;
	return argc;
}

<<<<<<< HEAD
static int process_stat_round_event(struct perf_tool *tool __maybe_unused,
				    union perf_event *event,
				    struct perf_session *session)
{
	struct stat_round_event *stat_round = &event->stat_round;
	struct perf_evsel *counter;
=======
static int process_stat_round_event(struct perf_session *session,
				    union perf_event *event)
{
	struct perf_record_stat_round *stat_round = &event->stat_round;
	struct evsel *counter;
>>>>>>> upstream/android-13
	struct timespec tsh, *ts = NULL;
	const char **argv = session->header.env.cmdline_argv;
	int argc = session->header.env.nr_cmdline;

	evlist__for_each_entry(evsel_list, counter)
		perf_stat_process_counter(&stat_config, counter);

	if (stat_round->type == PERF_STAT_ROUND_TYPE__FINAL)
		update_stats(&walltime_nsecs_stats, stat_round->time);

	if (stat_config.interval && stat_round->time) {
		tsh.tv_sec  = stat_round->time / NSEC_PER_SEC;
		tsh.tv_nsec = stat_round->time % NSEC_PER_SEC;
		ts = &tsh;
	}

	print_counters(ts, argc, argv);
	return 0;
}

static
<<<<<<< HEAD
int process_stat_config_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_session *session __maybe_unused)
{
=======
int process_stat_config_event(struct perf_session *session,
			      union perf_event *event)
{
	struct perf_tool *tool = session->tool;
>>>>>>> upstream/android-13
	struct perf_stat *st = container_of(tool, struct perf_stat, tool);

	perf_event__read_stat_config(&stat_config, &event->stat_config);

<<<<<<< HEAD
	if (cpu_map__empty(st->cpus)) {
=======
	if (perf_cpu_map__empty(st->cpus)) {
>>>>>>> upstream/android-13
		if (st->aggr_mode != AGGR_UNSET)
			pr_warning("warning: processing task data, aggregation mode not set\n");
		return 0;
	}

	if (st->aggr_mode != AGGR_UNSET)
		stat_config.aggr_mode = st->aggr_mode;

	if (perf_stat.data.is_pipe)
		perf_stat_init_aggr_mode();
	else
		perf_stat_init_aggr_mode_file(st);

	return 0;
}

static int set_maps(struct perf_stat *st)
{
	if (!st->cpus || !st->threads)
		return 0;

	if (WARN_ONCE(st->maps_allocated, "stats double allocation\n"))
		return -EINVAL;

<<<<<<< HEAD
	perf_evlist__set_maps(evsel_list, st->cpus, st->threads);

	if (perf_evlist__alloc_stats(evsel_list, true))
=======
	perf_evlist__set_maps(&evsel_list->core, st->cpus, st->threads);

	if (evlist__alloc_stats(evsel_list, true))
>>>>>>> upstream/android-13
		return -ENOMEM;

	st->maps_allocated = true;
	return 0;
}

static
<<<<<<< HEAD
int process_thread_map_event(struct perf_tool *tool,
			     union perf_event *event,
			     struct perf_session *session __maybe_unused)
{
=======
int process_thread_map_event(struct perf_session *session,
			     union perf_event *event)
{
	struct perf_tool *tool = session->tool;
>>>>>>> upstream/android-13
	struct perf_stat *st = container_of(tool, struct perf_stat, tool);

	if (st->threads) {
		pr_warning("Extra thread map event, ignoring.\n");
		return 0;
	}

	st->threads = thread_map__new_event(&event->thread_map);
	if (!st->threads)
		return -ENOMEM;

	return set_maps(st);
}

static
<<<<<<< HEAD
int process_cpu_map_event(struct perf_tool *tool,
			  union perf_event *event,
			  struct perf_session *session __maybe_unused)
{
	struct perf_stat *st = container_of(tool, struct perf_stat, tool);
	struct cpu_map *cpus;
=======
int process_cpu_map_event(struct perf_session *session,
			  union perf_event *event)
{
	struct perf_tool *tool = session->tool;
	struct perf_stat *st = container_of(tool, struct perf_stat, tool);
	struct perf_cpu_map *cpus;
>>>>>>> upstream/android-13

	if (st->cpus) {
		pr_warning("Extra cpu map event, ignoring.\n");
		return 0;
	}

	cpus = cpu_map__new_data(&event->cpu_map.data);
	if (!cpus)
		return -ENOMEM;

	st->cpus = cpus;
	return set_maps(st);
}

<<<<<<< HEAD
static int runtime_stat_new(struct perf_stat_config *config, int nthreads)
{
	int i;

	config->stats = calloc(nthreads, sizeof(struct runtime_stat));
	if (!config->stats)
		return -1;

	config->stats_num = nthreads;

	for (i = 0; i < nthreads; i++)
		runtime_stat__init(&config->stats[i]);

	return 0;
}

static void runtime_stat_delete(struct perf_stat_config *config)
{
	int i;

	if (!config->stats)
		return;

	for (i = 0; i < config->stats_num; i++)
		runtime_stat__exit(&config->stats[i]);

	free(config->stats);
}

=======
>>>>>>> upstream/android-13
static const char * const stat_report_usage[] = {
	"perf stat report [<options>]",
	NULL,
};

static struct perf_stat perf_stat = {
	.tool = {
		.attr		= perf_event__process_attr,
		.event_update	= perf_event__process_event_update,
		.thread_map	= process_thread_map_event,
		.cpu_map	= process_cpu_map_event,
		.stat_config	= process_stat_config_event,
		.stat		= perf_event__process_stat_event,
		.stat_round	= process_stat_round_event,
	},
	.aggr_mode = AGGR_UNSET,
};

static int __cmd_report(int argc, const char **argv)
{
	struct perf_session *session;
	const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file", "input file name"),
	OPT_SET_UINT(0, "per-socket", &perf_stat.aggr_mode,
		     "aggregate counts per processor socket", AGGR_SOCKET),
<<<<<<< HEAD
	OPT_SET_UINT(0, "per-core", &perf_stat.aggr_mode,
		     "aggregate counts per physical processor core", AGGR_CORE),
=======
	OPT_SET_UINT(0, "per-die", &perf_stat.aggr_mode,
		     "aggregate counts per processor die", AGGR_DIE),
	OPT_SET_UINT(0, "per-core", &perf_stat.aggr_mode,
		     "aggregate counts per physical processor core", AGGR_CORE),
	OPT_SET_UINT(0, "per-node", &perf_stat.aggr_mode,
		     "aggregate counts per numa node", AGGR_NODE),
>>>>>>> upstream/android-13
	OPT_SET_UINT('A', "no-aggr", &perf_stat.aggr_mode,
		     "disable CPU count aggregation", AGGR_NONE),
	OPT_END()
	};
	struct stat st;
	int ret;

	argc = parse_options(argc, argv, options, stat_report_usage, 0);

	if (!input_name || !strlen(input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			input_name = "-";
		else
			input_name = "perf.data";
	}

<<<<<<< HEAD
	perf_stat.data.file.path = input_name;
	perf_stat.data.mode      = PERF_DATA_MODE_READ;

	session = perf_session__new(&perf_stat.data, false, &perf_stat.tool);
	if (session == NULL)
		return -1;
=======
	perf_stat.data.path = input_name;
	perf_stat.data.mode = PERF_DATA_MODE_READ;

	session = perf_session__new(&perf_stat.data, &perf_stat.tool);
	if (IS_ERR(session))
		return PTR_ERR(session);
>>>>>>> upstream/android-13

	perf_stat.session  = session;
	stat_config.output = stderr;
	evsel_list         = session->evlist;

	ret = perf_session__process_events(session);
	if (ret)
		return ret;

	perf_session__delete(session);
	return 0;
}

static void setup_system_wide(int forks)
{
	/*
	 * Make system wide (-a) the default target if
	 * no target was specified and one of following
	 * conditions is met:
	 *
	 *   - there's no workload specified
	 *   - there is workload specified but all requested
	 *     events are system wide events
	 */
	if (!target__none(&target))
		return;

	if (!forks)
		target.system_wide = true;
	else {
<<<<<<< HEAD
		struct perf_evsel *counter;

		evlist__for_each_entry(evsel_list, counter) {
			if (!counter->system_wide)
				return;
		}

		if (evsel_list->nr_entries)
=======
		struct evsel *counter;

		evlist__for_each_entry(evsel_list, counter) {
			if (!counter->core.system_wide &&
			    strcmp(counter->name, "duration_time")) {
				return;
			}
		}

		if (evsel_list->core.nr_entries)
>>>>>>> upstream/android-13
			target.system_wide = true;
	}
}

int cmd_stat(int argc, const char **argv)
{
	const char * const stat_usage[] = {
		"perf stat [<options>] [<command>]",
		NULL
	};
<<<<<<< HEAD
	int status = -EINVAL, run_idx;
=======
	int status = -EINVAL, run_idx, err;
>>>>>>> upstream/android-13
	const char *mode;
	FILE *output = stderr;
	unsigned int interval, timeout;
	const char * const stat_subcommands[] = { "record", "report" };
<<<<<<< HEAD

	setlocale(LC_ALL, "");

	evsel_list = perf_evlist__new();
=======
	char errbuf[BUFSIZ];

	setlocale(LC_ALL, "");

	evsel_list = evlist__new();
>>>>>>> upstream/android-13
	if (evsel_list == NULL)
		return -ENOMEM;

	parse_events__shrink_config_terms();

	/* String-parsing callback-based options would segfault when negated */
	set_option_flag(stat_options, 'e', "event", PARSE_OPT_NONEG);
	set_option_flag(stat_options, 'M', "metrics", PARSE_OPT_NONEG);
	set_option_flag(stat_options, 'G', "cgroup", PARSE_OPT_NONEG);

	argc = parse_options_subcommand(argc, argv, stat_options, stat_subcommands,
					(const char **) stat_usage,
					PARSE_OPT_STOP_AT_NON_OPTION);
	perf_stat__collect_metric_expr(evsel_list);
	perf_stat__init_shadow_stats();

<<<<<<< HEAD
	if (csv_sep) {
		csv_output = true;
		if (!strcmp(csv_sep, "\\t"))
			csv_sep = "\t";
	} else
		csv_sep = DEFAULT_SEPARATOR;
=======
	if (stat_config.csv_sep) {
		stat_config.csv_output = true;
		if (!strcmp(stat_config.csv_sep, "\\t"))
			stat_config.csv_sep = "\t";
	} else
		stat_config.csv_sep = DEFAULT_SEPARATOR;
>>>>>>> upstream/android-13

	if (argc && !strncmp(argv[0], "rec", 3)) {
		argc = __cmd_record(argc, argv);
		if (argc < 0)
			return -1;
	} else if (argc && !strncmp(argv[0], "rep", 3))
		return __cmd_report(argc, argv);

	interval = stat_config.interval;
	timeout = stat_config.timeout;

	/*
	 * For record command the -o is already taken care of.
	 */
	if (!STAT_RECORD && output_name && strcmp(output_name, "-"))
		output = NULL;

	if (output_name && output_fd) {
		fprintf(stderr, "cannot use both --output and --log-fd\n");
		parse_options_usage(stat_usage, stat_options, "o", 1);
		parse_options_usage(NULL, stat_options, "log-fd", 0);
		goto out;
	}

<<<<<<< HEAD
	if (metric_only && stat_config.aggr_mode == AGGR_THREAD) {
=======
	if (stat_config.metric_only && stat_config.aggr_mode == AGGR_THREAD) {
>>>>>>> upstream/android-13
		fprintf(stderr, "--metric-only is not supported with --per-thread\n");
		goto out;
	}

<<<<<<< HEAD
	if (metric_only && run_count > 1) {
=======
	if (stat_config.metric_only && stat_config.run_count > 1) {
>>>>>>> upstream/android-13
		fprintf(stderr, "--metric-only is not supported with -r\n");
		goto out;
	}

<<<<<<< HEAD
	if (walltime_run_table && run_count <= 1) {
=======
	if (stat_config.walltime_run_table && stat_config.run_count <= 1) {
>>>>>>> upstream/android-13
		fprintf(stderr, "--table is only supported with -r\n");
		parse_options_usage(stat_usage, stat_options, "r", 1);
		parse_options_usage(NULL, stat_options, "table", 0);
		goto out;
	}

	if (output_fd < 0) {
		fprintf(stderr, "argument to --log-fd must be a > 0\n");
		parse_options_usage(stat_usage, stat_options, "log-fd", 0);
		goto out;
	}

<<<<<<< HEAD
	if (!output) {
=======
	if (!output && !stat_config.quiet) {
>>>>>>> upstream/android-13
		struct timespec tm;
		mode = append_file ? "a" : "w";

		output = fopen(output_name, mode);
		if (!output) {
			perror("failed to create output file");
			return -1;
		}
		clock_gettime(CLOCK_REALTIME, &tm);
		fprintf(output, "# started on %s\n", ctime(&tm.tv_sec));
	} else if (output_fd > 0) {
		mode = append_file ? "a" : "w";
		output = fdopen(output_fd, mode);
		if (!output) {
			perror("Failed opening logfd");
			return -errno;
		}
	}

	stat_config.output = output;

	/*
	 * let the spreadsheet do the pretty-printing
	 */
<<<<<<< HEAD
	if (csv_output) {
=======
	if (stat_config.csv_output) {
>>>>>>> upstream/android-13
		/* User explicitly passed -B? */
		if (big_num_opt == 1) {
			fprintf(stderr, "-B option not supported with -x\n");
			parse_options_usage(stat_usage, stat_options, "B", 1);
			parse_options_usage(NULL, stat_options, "x", 1);
			goto out;
		} else /* Nope, so disable big number formatting */
<<<<<<< HEAD
			big_num = false;
	} else if (big_num_opt == 0) /* User passed --no-big-num */
		big_num = false;
=======
			stat_config.big_num = false;
	} else if (big_num_opt == 0) /* User passed --no-big-num */
		stat_config.big_num = false;

	err = target__validate(&target);
	if (err) {
		target__strerror(&target, err, errbuf, BUFSIZ);
		pr_warning("%s\n", errbuf);
	}
>>>>>>> upstream/android-13

	setup_system_wide(argc);

	/*
	 * Display user/system times only for single
	 * run and when there's specified tracee.
	 */
<<<<<<< HEAD
	if ((run_count == 1) && target__none(&target))
		ru_display = true;

	if (run_count < 0) {
		pr_err("Run count must be a positive number\n");
		parse_options_usage(stat_usage, stat_options, "r", 1);
		goto out;
	} else if (run_count == 0) {
		forever = true;
		run_count = 1;
	}

	if (walltime_run_table) {
		walltime_run = zalloc(run_count * sizeof(walltime_run[0]));
		if (!walltime_run) {
=======
	if ((stat_config.run_count == 1) && target__none(&target))
		stat_config.ru_display = true;

	if (stat_config.run_count < 0) {
		pr_err("Run count must be a positive number\n");
		parse_options_usage(stat_usage, stat_options, "r", 1);
		goto out;
	} else if (stat_config.run_count == 0) {
		forever = true;
		stat_config.run_count = 1;
	}

	if (stat_config.walltime_run_table) {
		stat_config.walltime_run = zalloc(stat_config.run_count * sizeof(stat_config.walltime_run[0]));
		if (!stat_config.walltime_run) {
>>>>>>> upstream/android-13
			pr_err("failed to setup -r option");
			goto out;
		}
	}

	if ((stat_config.aggr_mode == AGGR_THREAD) &&
		!target__has_task(&target)) {
		if (!target.system_wide || target.cpu_list) {
			fprintf(stderr, "The --per-thread option is only "
				"available when monitoring via -p -t -a "
				"options or only --per-thread.\n");
			parse_options_usage(NULL, stat_options, "p", 1);
			parse_options_usage(NULL, stat_options, "t", 1);
			goto out;
		}
	}

	/*
	 * no_aggr, cgroup are for system-wide only
	 * --per-thread is aggregated per thread, we dont mix it with cpu mode
	 */
	if (((stat_config.aggr_mode != AGGR_GLOBAL &&
<<<<<<< HEAD
	      stat_config.aggr_mode != AGGR_THREAD) || nr_cgroups) &&
=======
	      stat_config.aggr_mode != AGGR_THREAD) ||
	     (nr_cgroups || stat_config.cgroup_list)) &&
>>>>>>> upstream/android-13
	    !target__has_cpu(&target)) {
		fprintf(stderr, "both cgroup and no-aggregation "
			"modes only available in system-wide mode\n");

		parse_options_usage(stat_usage, stat_options, "G", 1);
		parse_options_usage(NULL, stat_options, "A", 1);
		parse_options_usage(NULL, stat_options, "a", 1);
<<<<<<< HEAD
		goto out;
	}

	if (add_default_attributes())
		goto out;

	target__validate(&target);
=======
		parse_options_usage(NULL, stat_options, "for-each-cgroup", 0);
		goto out;
	}

	if (stat_config.iostat_run) {
		status = iostat_prepare(evsel_list, &stat_config);
		if (status)
			goto out;
		if (iostat_mode == IOSTAT_LIST) {
			iostat_list(evsel_list, &stat_config);
			goto out;
		} else if (verbose)
			iostat_list(evsel_list, &stat_config);
		if (iostat_mode == IOSTAT_RUN && !target__has_cpu(&target))
			target.system_wide = true;
	}

	if (add_default_attributes())
		goto out;

	if (stat_config.cgroup_list) {
		if (nr_cgroups > 0) {
			pr_err("--cgroup and --for-each-cgroup cannot be used together\n");
			parse_options_usage(stat_usage, stat_options, "G", 1);
			parse_options_usage(NULL, stat_options, "for-each-cgroup", 0);
			goto out;
		}

		if (evlist__expand_cgroup(evsel_list, stat_config.cgroup_list,
					  &stat_config.metric_events, true) < 0) {
			parse_options_usage(stat_usage, stat_options,
					    "for-each-cgroup", 0);
			goto out;
		}
	}
>>>>>>> upstream/android-13

	if ((stat_config.aggr_mode == AGGR_THREAD) && (target.system_wide))
		target.per_thread = true;

<<<<<<< HEAD
	if (perf_evlist__create_maps(evsel_list, &target) < 0) {
=======
	if (evlist__fix_hybrid_cpus(evsel_list, target.cpu_list)) {
		pr_err("failed to use cpu list %s\n", target.cpu_list);
		goto out;
	}

	target.hybrid = perf_pmu__has_hybrid();
	if (evlist__create_maps(evsel_list, &target) < 0) {
>>>>>>> upstream/android-13
		if (target__has_task(&target)) {
			pr_err("Problems finding threads of monitor\n");
			parse_options_usage(stat_usage, stat_options, "p", 1);
			parse_options_usage(NULL, stat_options, "t", 1);
		} else if (target__has_cpu(&target)) {
			perror("failed to parse CPUs map");
			parse_options_usage(stat_usage, stat_options, "C", 1);
			parse_options_usage(NULL, stat_options, "a", 1);
		}
		goto out;
	}

<<<<<<< HEAD
=======
	evlist__check_cpu_maps(evsel_list);

>>>>>>> upstream/android-13
	/*
	 * Initialize thread_map with comm names,
	 * so we could print it out on output.
	 */
	if (stat_config.aggr_mode == AGGR_THREAD) {
<<<<<<< HEAD
		thread_map__read_comms(evsel_list->threads);
		if (target.system_wide) {
			if (runtime_stat_new(&stat_config,
				thread_map__nr(evsel_list->threads))) {
=======
		thread_map__read_comms(evsel_list->core.threads);
		if (target.system_wide) {
			if (runtime_stat_new(&stat_config,
				perf_thread_map__nr(evsel_list->core.threads))) {
>>>>>>> upstream/android-13
				goto out;
			}
		}
	}

<<<<<<< HEAD
=======
	if (stat_config.aggr_mode == AGGR_NODE)
		cpu__setup_cpunode_map();

>>>>>>> upstream/android-13
	if (stat_config.times && interval)
		interval_count = true;
	else if (stat_config.times && !interval) {
		pr_err("interval-count option should be used together with "
				"interval-print.\n");
		parse_options_usage(stat_usage, stat_options, "interval-count", 0);
		parse_options_usage(stat_usage, stat_options, "I", 1);
		goto out;
	}

	if (timeout && timeout < 100) {
		if (timeout < 10) {
			pr_err("timeout must be >= 10ms.\n");
			parse_options_usage(stat_usage, stat_options, "timeout", 0);
			goto out;
		} else
			pr_warning("timeout < 100ms. "
				   "The overhead percentage could be high in some cases. "
				   "Please proceed with caution.\n");
	}
	if (timeout && interval) {
		pr_err("timeout option is not supported with interval-print.\n");
		parse_options_usage(stat_usage, stat_options, "timeout", 0);
		parse_options_usage(stat_usage, stat_options, "I", 1);
		goto out;
	}

<<<<<<< HEAD
	if (perf_evlist__alloc_stats(evsel_list, interval))
=======
	if (evlist__alloc_stats(evsel_list, interval))
>>>>>>> upstream/android-13
		goto out;

	if (perf_stat_init_aggr_mode())
		goto out;

	/*
<<<<<<< HEAD
=======
	 * Set sample_type to PERF_SAMPLE_IDENTIFIER, which should be harmless
	 * while avoiding that older tools show confusing messages.
	 *
	 * However for pipe sessions we need to keep it zero,
	 * because script's perf_evsel__check_attr is triggered
	 * by attr->sample_type != 0, and we can't run it on
	 * stat sessions.
	 */
	stat_config.identifier = !(STAT_RECORD && perf_stat.data.is_pipe);

	/*
>>>>>>> upstream/android-13
	 * We dont want to block the signals - that would cause
	 * child tasks to inherit that and Ctrl-C would not work.
	 * What we want is for Ctrl-C to work in the exec()-ed
	 * task, but being ignored by perf stat itself:
	 */
	atexit(sig_atexit);
	if (!forever)
		signal(SIGINT,  skip_signal);
	signal(SIGCHLD, skip_signal);
	signal(SIGALRM, skip_signal);
	signal(SIGABRT, skip_signal);

<<<<<<< HEAD
	status = 0;
	for (run_idx = 0; forever || run_idx < run_count; run_idx++) {
		if (run_count != 1 && verbose > 0)
=======
	if (evlist__initialize_ctlfd(evsel_list, stat_config.ctl_fd, stat_config.ctl_fd_ack))
		goto out;

	status = 0;
	for (run_idx = 0; forever || run_idx < stat_config.run_count; run_idx++) {
		if (stat_config.run_count != 1 && verbose > 0)
>>>>>>> upstream/android-13
			fprintf(output, "[ perf stat: executing run #%d ... ]\n",
				run_idx + 1);

		if (run_idx != 0)
<<<<<<< HEAD
			perf_evlist__reset_prev_raw_counts(evsel_list);
=======
			evlist__reset_prev_raw_counts(evsel_list);
>>>>>>> upstream/android-13

		status = run_perf_stat(argc, argv, run_idx);
		if (forever && status != -1 && !interval) {
			print_counters(NULL, argc, argv);
			perf_stat__reset_stats();
		}
	}

<<<<<<< HEAD
	if (!forever && status != -1 && !interval)
		print_counters(NULL, argc, argv);

=======
	if (!forever && status != -1 && (!interval || stat_config.summary))
		print_counters(NULL, argc, argv);

	evlist__finalize_ctlfd(evsel_list);

>>>>>>> upstream/android-13
	if (STAT_RECORD) {
		/*
		 * We synthesize the kernel mmap record just so that older tools
		 * don't emit warnings about not being able to resolve symbols
<<<<<<< HEAD
		 * due to /proc/sys/kernel/kptr_restrict settings and instear provide
=======
		 * due to /proc/sys/kernel/kptr_restrict settings and instead provide
>>>>>>> upstream/android-13
		 * a saner message about no samples being in the perf.data file.
		 *
		 * This also serves to suppress a warning about f_header.data.size == 0
		 * in header.c at the moment 'perf stat record' gets introduced, which
		 * is not really needed once we start adding the stat specific PERF_RECORD_
		 * records, but the need to suppress the kptr_restrict messages in older
		 * tools remain  -acme
		 */
		int fd = perf_data__fd(&perf_stat.data);
<<<<<<< HEAD
		int err = perf_event__synthesize_kernel_mmap((void *)&perf_stat,
							     process_synthesized_event,
							     &perf_stat.session->machines.host);
=======

		err = perf_event__synthesize_kernel_mmap((void *)&perf_stat,
							 process_synthesized_event,
							 &perf_stat.session->machines.host);
>>>>>>> upstream/android-13
		if (err) {
			pr_warning("Couldn't synthesize the kernel mmap record, harmless, "
				   "older tools may produce warnings about this file\n.");
		}

		if (!interval) {
			if (WRITE_STAT_ROUND_EVENT(walltime_nsecs_stats.max, FINAL))
				pr_err("failed to write stat round event\n");
		}

		if (!perf_stat.data.is_pipe) {
			perf_stat.session->header.data_size += perf_stat.bytes_written;
			perf_session__write_header(perf_stat.session, evsel_list, fd, true);
		}

<<<<<<< HEAD
=======
		evlist__close(evsel_list);
>>>>>>> upstream/android-13
		perf_session__delete(perf_stat.session);
	}

	perf_stat__exit_aggr_mode();
<<<<<<< HEAD
	perf_evlist__free_stats(evsel_list);
out:
	free(walltime_run);
=======
	evlist__free_stats(evsel_list);
out:
	if (stat_config.iostat_run)
		iostat_release(evsel_list);

	zfree(&stat_config.walltime_run);
>>>>>>> upstream/android-13

	if (smi_cost && smi_reset)
		sysfs__write_int(FREEZE_ON_SMI_PATH, 0);

<<<<<<< HEAD
	perf_evlist__delete(evsel_list);

	runtime_stat_delete(&stat_config);
=======
	evlist__delete(evsel_list);

	metricgroup__rblist_exit(&stat_config.metric_events);
	runtime_stat_delete(&stat_config);
	evlist__close_control(stat_config.ctl_fd, stat_config.ctl_fd_ack, &stat_config.ctl_fd_close);
>>>>>>> upstream/android-13

	return status;
}
