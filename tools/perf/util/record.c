// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include "evlist.h"
#include "evsel.h"
#include "cpumap.h"
#include "parse-events.h"
#include <errno.h>
#include <api/fs/fs.h>
#include <subcmd/parse-options.h>
#include "util.h"
#include "cloexec.h"

typedef void (*setup_probe_fn_t)(struct perf_evsel *evsel);

static int perf_do_probe_api(setup_probe_fn_t fn, int cpu, const char *str)
{
	struct perf_evlist *evlist;
	struct perf_evsel *evsel;
	unsigned long flags = perf_event_open_cloexec_flag();
	int err = -EAGAIN, fd;
	static pid_t pid = -1;

	evlist = perf_evlist__new();
	if (!evlist)
		return -ENOMEM;

	if (parse_events(evlist, str, NULL))
		goto out_delete;

	evsel = perf_evlist__first(evlist);

	while (1) {
		fd = sys_perf_event_open(&evsel->attr, pid, cpu, -1, flags);
		if (fd < 0) {
			if (pid == -1 && errno == EACCES) {
				pid = 0;
				continue;
			}
			goto out_delete;
		}
		break;
	}
	close(fd);

	fn(evsel);

	fd = sys_perf_event_open(&evsel->attr, pid, cpu, -1, flags);
	if (fd < 0) {
		if (errno == EINVAL)
			err = -EINVAL;
		goto out_delete;
	}
	close(fd);
	err = 0;

out_delete:
	perf_evlist__delete(evlist);
	return err;
}

static bool perf_probe_api(setup_probe_fn_t fn)
{
	const char *try[] = {"cycles:u", "instructions:u", "cpu-clock:u", NULL};
	struct cpu_map *cpus;
	int cpu, ret, i = 0;

	cpus = cpu_map__new(NULL);
	if (!cpus)
		return false;
	cpu = cpus->map[0];
	cpu_map__put(cpus);

	do {
		ret = perf_do_probe_api(fn, cpu, try[i++]);
		if (!ret)
			return true;
	} while (ret == -EAGAIN && try[i]);

	return false;
}

static void perf_probe_sample_identifier(struct perf_evsel *evsel)
{
	evsel->attr.sample_type |= PERF_SAMPLE_IDENTIFIER;
}

static void perf_probe_comm_exec(struct perf_evsel *evsel)
{
	evsel->attr.comm_exec = 1;
}

static void perf_probe_context_switch(struct perf_evsel *evsel)
{
	evsel->attr.context_switch = 1;
}

bool perf_can_sample_identifier(void)
{
	return perf_probe_api(perf_probe_sample_identifier);
}

static bool perf_can_comm_exec(void)
{
	return perf_probe_api(perf_probe_comm_exec);
}

bool perf_can_record_switch_events(void)
{
	return perf_probe_api(perf_probe_context_switch);
}

bool perf_can_record_cpu_wide(void)
{
	struct perf_event_attr attr = {
		.type = PERF_TYPE_SOFTWARE,
		.config = PERF_COUNT_SW_CPU_CLOCK,
		.exclude_kernel = 1,
	};
	struct cpu_map *cpus;
	int cpu, fd;

	cpus = cpu_map__new(NULL);
	if (!cpus)
		return false;
	cpu = cpus->map[0];
	cpu_map__put(cpus);

	fd = sys_perf_event_open(&attr, -1, cpu, -1, 0);
	if (fd < 0)
		return false;
	close(fd);

	return true;
}

void perf_evlist__config(struct perf_evlist *evlist, struct record_opts *opts,
			 struct callchain_param *callchain)
{
	struct perf_evsel *evsel;
=======
#include "debug.h"
#include "evlist.h"
#include "evsel.h"
#include "evsel_config.h"
#include "parse-events.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <api/fs/fs.h>
#include <subcmd/parse-options.h>
#include <perf/cpumap.h>
#include "cloexec.h"
#include "util/perf_api_probe.h"
#include "record.h"
#include "../perf-sys.h"
#include "topdown.h"
#include "map_symbol.h"
#include "mem-events.h"

/*
 * evsel__config_leader_sampling() uses special rules for leader sampling.
 * However, if the leader is an AUX area event, then assume the event to sample
 * is the next event.
 */
static struct evsel *evsel__read_sampler(struct evsel *evsel, struct evlist *evlist)
{
	struct evsel *leader = evsel__leader(evsel);

	if (evsel__is_aux_event(leader) || arch_topdown_sample_read(leader) ||
	    is_mem_loads_aux_event(leader)) {
		evlist__for_each_entry(evlist, evsel) {
			if (evsel__leader(evsel) == leader && evsel != evsel__leader(evsel))
				return evsel;
		}
	}

	return leader;
}

static u64 evsel__config_term_mask(struct evsel *evsel)
{
	struct evsel_config_term *term;
	struct list_head *config_terms = &evsel->config_terms;
	u64 term_types = 0;

	list_for_each_entry(term, config_terms, list) {
		term_types |= 1 << term->type;
	}
	return term_types;
}

static void evsel__config_leader_sampling(struct evsel *evsel, struct evlist *evlist)
{
	struct perf_event_attr *attr = &evsel->core.attr;
	struct evsel *leader = evsel__leader(evsel);
	struct evsel *read_sampler;
	u64 term_types, freq_mask;

	if (!leader->sample_read)
		return;

	read_sampler = evsel__read_sampler(evsel, evlist);

	if (evsel == read_sampler)
		return;

	term_types = evsel__config_term_mask(evsel);
	/*
	 * Disable sampling for all group members except those with explicit
	 * config terms or the leader. In the case of an AUX area event, the 2nd
	 * event in the group is the one that 'leads' the sampling.
	 */
	freq_mask = (1 << EVSEL__CONFIG_TERM_FREQ) | (1 << EVSEL__CONFIG_TERM_PERIOD);
	if ((term_types & freq_mask) == 0) {
		attr->freq           = 0;
		attr->sample_freq    = 0;
		attr->sample_period  = 0;
	}
	if ((term_types & (1 << EVSEL__CONFIG_TERM_OVERWRITE)) == 0)
		attr->write_backward = 0;

	/*
	 * We don't get a sample for slave events, we make them when delivering
	 * the group leader sample. Set the slave event to follow the master
	 * sample_type to ease up reporting.
	 * An AUX area event also has sample_type requirements, so also include
	 * the sample type bits from the leader's sample_type to cover that
	 * case.
	 */
	attr->sample_type = read_sampler->core.attr.sample_type |
			    leader->core.attr.sample_type;
}

void evlist__config(struct evlist *evlist, struct record_opts *opts, struct callchain_param *callchain)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13
	bool use_sample_identifier = false;
	bool use_comm_exec;
	bool sample_id = opts->sample_id;

	/*
	 * Set the evsel leader links before we configure attributes,
	 * since some might depend on this info.
	 */
	if (opts->group)
<<<<<<< HEAD
		perf_evlist__set_leader(evlist);

	if (evlist->cpus->map[0] < 0)
=======
		evlist__set_leader(evlist);

	if (evlist->core.cpus->map[0] < 0)
>>>>>>> upstream/android-13
		opts->no_inherit = true;

	use_comm_exec = perf_can_comm_exec();

	evlist__for_each_entry(evlist, evsel) {
<<<<<<< HEAD
		perf_evsel__config(evsel, opts, callchain);
		if (evsel->tracking && use_comm_exec)
			evsel->attr.comm_exec = 1;
	}

=======
		evsel__config(evsel, opts, callchain);
		if (evsel->tracking && use_comm_exec)
			evsel->core.attr.comm_exec = 1;
	}

	/* Configure leader sampling here now that the sample type is known */
	evlist__for_each_entry(evlist, evsel)
		evsel__config_leader_sampling(evsel, evlist);

>>>>>>> upstream/android-13
	if (opts->full_auxtrace) {
		/*
		 * Need to be able to synthesize and parse selected events with
		 * arbitrary sample types, which requires always being able to
		 * match the id.
		 */
		use_sample_identifier = perf_can_sample_identifier();
		sample_id = true;
<<<<<<< HEAD
	} else if (evlist->nr_entries > 1) {
		struct perf_evsel *first = perf_evlist__first(evlist);

		evlist__for_each_entry(evlist, evsel) {
			if (evsel->attr.sample_type == first->attr.sample_type)
=======
	} else if (evlist->core.nr_entries > 1) {
		struct evsel *first = evlist__first(evlist);

		evlist__for_each_entry(evlist, evsel) {
			if (evsel->core.attr.sample_type == first->core.attr.sample_type)
>>>>>>> upstream/android-13
				continue;
			use_sample_identifier = perf_can_sample_identifier();
			break;
		}
		sample_id = true;
	}

	if (sample_id) {
		evlist__for_each_entry(evlist, evsel)
<<<<<<< HEAD
			perf_evsel__set_sample_id(evsel, use_sample_identifier);
	}

	perf_evlist__set_id_pos(evlist);
=======
			evsel__set_sample_id(evsel, use_sample_identifier);
	}

	evlist__set_id_pos(evlist);
>>>>>>> upstream/android-13
}

static int get_max_rate(unsigned int *rate)
{
	return sysctl__read_int("kernel/perf_event_max_sample_rate", (int *)rate);
}

static int record_opts__config_freq(struct record_opts *opts)
{
	bool user_freq = opts->user_freq != UINT_MAX;
<<<<<<< HEAD
	unsigned int max_rate;

	if (opts->user_interval != ULLONG_MAX)
=======
	bool user_interval = opts->user_interval != ULLONG_MAX;
	unsigned int max_rate;

	if (user_interval && user_freq) {
		pr_err("cannot set frequency and period at the same time\n");
		return -1;
	}

	if (user_interval)
>>>>>>> upstream/android-13
		opts->default_interval = opts->user_interval;
	if (user_freq)
		opts->freq = opts->user_freq;

	/*
	 * User specified count overrides default frequency.
	 */
	if (opts->default_interval)
		opts->freq = 0;
	else if (opts->freq) {
		opts->default_interval = opts->freq;
	} else {
		pr_err("frequency and count are zero, aborting\n");
		return -1;
	}

	if (get_max_rate(&max_rate))
		return 0;

	/*
	 * User specified frequency is over current maximum.
	 */
	if (user_freq && (max_rate < opts->freq)) {
		if (opts->strict_freq) {
			pr_err("error: Maximum frequency rate (%'u Hz) exceeded.\n"
			       "       Please use -F freq option with a lower value or consider\n"
			       "       tweaking /proc/sys/kernel/perf_event_max_sample_rate.\n",
			       max_rate);
			return -1;
		} else {
			pr_warning("warning: Maximum frequency rate (%'u Hz) exceeded, throttling from %'u Hz to %'u Hz.\n"
				   "         The limit can be raised via /proc/sys/kernel/perf_event_max_sample_rate.\n"
				   "         The kernel will lower it when perf's interrupts take too long.\n"
				   "         Use --strict-freq to disable this throttling, refusing to record.\n",
				   max_rate, opts->freq, max_rate);

			opts->freq = max_rate;
		}
	}

	/*
	 * Default frequency is over current maximum.
	 */
	if (max_rate < opts->freq) {
<<<<<<< HEAD
		pr_warning("Lowering default frequency rate to %u.\n"
			   "Please consider tweaking "
			   "/proc/sys/kernel/perf_event_max_sample_rate.\n",
			   max_rate);
=======
		pr_warning("Lowering default frequency rate from %u to %u.\n"
			   "Please consider tweaking "
			   "/proc/sys/kernel/perf_event_max_sample_rate.\n",
			   opts->freq, max_rate);
>>>>>>> upstream/android-13
		opts->freq = max_rate;
	}

	return 0;
}

int record_opts__config(struct record_opts *opts)
{
	return record_opts__config_freq(opts);
}

<<<<<<< HEAD
bool perf_evlist__can_select_event(struct perf_evlist *evlist, const char *str)
{
	struct perf_evlist *temp_evlist;
	struct perf_evsel *evsel;
=======
bool evlist__can_select_event(struct evlist *evlist, const char *str)
{
	struct evlist *temp_evlist;
	struct evsel *evsel;
>>>>>>> upstream/android-13
	int err, fd, cpu;
	bool ret = false;
	pid_t pid = -1;

<<<<<<< HEAD
	temp_evlist = perf_evlist__new();
=======
	temp_evlist = evlist__new();
>>>>>>> upstream/android-13
	if (!temp_evlist)
		return false;

	err = parse_events(temp_evlist, str, NULL);
	if (err)
		goto out_delete;

<<<<<<< HEAD
	evsel = perf_evlist__last(temp_evlist);

	if (!evlist || cpu_map__empty(evlist->cpus)) {
		struct cpu_map *cpus = cpu_map__new(NULL);

		cpu =  cpus ? cpus->map[0] : 0;
		cpu_map__put(cpus);
	} else {
		cpu = evlist->cpus->map[0];
	}

	while (1) {
		fd = sys_perf_event_open(&evsel->attr, pid, cpu, -1,
=======
	evsel = evlist__last(temp_evlist);

	if (!evlist || perf_cpu_map__empty(evlist->core.cpus)) {
		struct perf_cpu_map *cpus = perf_cpu_map__new(NULL);

		cpu =  cpus ? cpus->map[0] : 0;
		perf_cpu_map__put(cpus);
	} else {
		cpu = evlist->core.cpus->map[0];
	}

	while (1) {
		fd = sys_perf_event_open(&evsel->core.attr, pid, cpu, -1,
>>>>>>> upstream/android-13
					 perf_event_open_cloexec_flag());
		if (fd < 0) {
			if (pid == -1 && errno == EACCES) {
				pid = 0;
				continue;
			}
			goto out_delete;
		}
		break;
	}
	close(fd);
	ret = true;

out_delete:
<<<<<<< HEAD
	perf_evlist__delete(temp_evlist);
=======
	evlist__delete(temp_evlist);
>>>>>>> upstream/android-13
	return ret;
}

int record__parse_freq(const struct option *opt, const char *str, int unset __maybe_unused)
{
	unsigned int freq;
	struct record_opts *opts = opt->value;

	if (!str)
		return -EINVAL;

	if (strcasecmp(str, "max") == 0) {
		if (get_max_rate(&freq)) {
			pr_err("couldn't read /proc/sys/kernel/perf_event_max_sample_rate\n");
			return -1;
		}
		pr_info("info: Using a maximum frequency rate of %'d Hz\n", freq);
	} else {
		freq = atoi(str);
	}

	opts->user_freq = freq;
	return 0;
}
