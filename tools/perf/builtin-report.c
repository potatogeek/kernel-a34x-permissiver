// SPDX-License-Identifier: GPL-2.0
/*
 * builtin-report.c
 *
 * Builtin report command: Analyze the perf.data input file,
 * look up and read DSOs and symbol information and display
 * a histogram of results, along various sorting keys.
 */
#include "builtin.h"

<<<<<<< HEAD
#include "util/util.h"
=======
>>>>>>> upstream/android-13
#include "util/config.h"

#include "util/annotate.h"
#include "util/color.h"
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/err.h>
#include "util/symbol.h"
=======
#include "util/dso.h"
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/err.h>
#include <linux/zalloc.h>
#include "util/map.h"
#include "util/symbol.h"
#include "util/map_symbol.h"
#include "util/mem-events.h"
#include "util/branch.h"
>>>>>>> upstream/android-13
#include "util/callchain.h"
#include "util/values.h"

#include "perf.h"
#include "util/debug.h"
#include "util/evlist.h"
#include "util/evsel.h"
<<<<<<< HEAD
#include "util/header.h"
#include "util/session.h"
=======
#include "util/evswitch.h"
#include "util/header.h"
#include "util/session.h"
#include "util/srcline.h"
>>>>>>> upstream/android-13
#include "util/tool.h"

#include <subcmd/parse-options.h>
#include <subcmd/exec-cmd.h>
#include "util/parse-events.h"

#include "util/thread.h"
#include "util/sort.h"
#include "util/hist.h"
#include "util/data.h"
#include "arch/common.h"
#include "util/time-utils.h"
#include "util/auxtrace.h"
#include "util/units.h"
<<<<<<< HEAD
#include "util/branch.h"
=======
#include "util/util.h" // perf_tip()
#include "ui/ui.h"
#include "ui/progress.h"
#include "util/block-info.h"
>>>>>>> upstream/android-13

#include <dlfcn.h>
#include <errno.h>
#include <inttypes.h>
#include <regex.h>
<<<<<<< HEAD
#include <signal.h>
#include <linux/bitmap.h>
#include <linux/stringify.h>
=======
#include <linux/ctype.h>
#include <signal.h>
#include <linux/bitmap.h>
#include <linux/string.h>
#include <linux/stringify.h>
#include <linux/time64.h>
>>>>>>> upstream/android-13
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/mman.h>

struct report {
	struct perf_tool	tool;
	struct perf_session	*session;
<<<<<<< HEAD
=======
	struct evswitch		evswitch;
>>>>>>> upstream/android-13
	bool			use_tui, use_gtk, use_stdio;
	bool			show_full_info;
	bool			show_threads;
	bool			inverted_callchain;
	bool			mem_mode;
	bool			stats_mode;
	bool			tasks_mode;
	bool			mmaps_mode;
	bool			header;
	bool			header_only;
	bool			nonany_branch_mode;
	bool			group_set;
<<<<<<< HEAD
=======
	bool			stitch_lbr;
	bool			disable_order;
	bool			skip_empty;
>>>>>>> upstream/android-13
	int			max_stack;
	struct perf_read_values	show_threads_values;
	struct annotation_options annotation_opts;
	const char		*pretty_printing_style;
	const char		*cpu_list;
	const char		*symbol_filter_str;
	const char		*time_str;
	struct perf_time_interval *ptime_range;
	int			range_size;
	int			range_num;
	float			min_percent;
	u64			nr_entries;
	u64			queue_size;
<<<<<<< HEAD
	int			socket_filter;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
	struct branch_type_stat	brtype_stat;
=======
	u64			total_cycles;
	int			socket_filter;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
	struct branch_type_stat	brtype_stat;
	bool			symbol_ipc;
	bool			total_cycles_mode;
	struct block_report	*block_reports;
	int			nr_block_reports;
>>>>>>> upstream/android-13
};

static int report__config(const char *var, const char *value, void *cb)
{
	struct report *rep = cb;

	if (!strcmp(var, "report.group")) {
		symbol_conf.event_group = perf_config_bool(var, value);
		return 0;
	}
	if (!strcmp(var, "report.percent-limit")) {
		double pcnt = strtof(value, NULL);

		rep->min_percent = pcnt;
		callchain_param.min_percent = pcnt;
		return 0;
	}
	if (!strcmp(var, "report.children")) {
		symbol_conf.cumulate_callchain = perf_config_bool(var, value);
		return 0;
	}
	if (!strcmp(var, "report.queue-size"))
		return perf_config_u64(&rep->queue_size, var, value);

	if (!strcmp(var, "report.sort_order")) {
		default_sort_order = strdup(value);
		return 0;
	}

<<<<<<< HEAD
=======
	if (!strcmp(var, "report.skip-empty")) {
		rep->skip_empty = perf_config_bool(var, value);
		return 0;
	}

>>>>>>> upstream/android-13
	return 0;
}

static int hist_iter__report_callback(struct hist_entry_iter *iter,
				      struct addr_location *al, bool single,
				      void *arg)
{
	int err = 0;
	struct report *rep = arg;
	struct hist_entry *he = iter->he;
<<<<<<< HEAD
	struct perf_evsel *evsel = iter->evsel;
=======
	struct evsel *evsel = iter->evsel;
>>>>>>> upstream/android-13
	struct perf_sample *sample = iter->sample;
	struct mem_info *mi;
	struct branch_info *bi;

<<<<<<< HEAD
	if (!ui__has_annotation())
		return 0;

	hist__account_cycles(sample->branch_stack, al, sample,
			     rep->nonany_branch_mode);

=======
	if (!ui__has_annotation() && !rep->symbol_ipc)
		return 0;

>>>>>>> upstream/android-13
	if (sort__mode == SORT_MODE__BRANCH) {
		bi = he->branch_info;
		err = addr_map_symbol__inc_samples(&bi->from, sample, evsel);
		if (err)
			goto out;

		err = addr_map_symbol__inc_samples(&bi->to, sample, evsel);

	} else if (rep->mem_mode) {
		mi = he->mem_info;
		err = addr_map_symbol__inc_samples(&mi->daddr, sample, evsel);
		if (err)
			goto out;

		err = hist_entry__inc_addr_samples(he, sample, evsel, al->addr);

	} else if (symbol_conf.cumulate_callchain) {
		if (single)
			err = hist_entry__inc_addr_samples(he, sample, evsel, al->addr);
	} else {
		err = hist_entry__inc_addr_samples(he, sample, evsel, al->addr);
	}

out:
	return err;
}

static int hist_iter__branch_callback(struct hist_entry_iter *iter,
				      struct addr_location *al __maybe_unused,
				      bool single __maybe_unused,
				      void *arg)
{
	struct hist_entry *he = iter->he;
	struct report *rep = arg;
<<<<<<< HEAD
	struct branch_info *bi;
	struct perf_sample *sample = iter->sample;
	struct perf_evsel *evsel = iter->evsel;
	int err;

	if (!ui__has_annotation())
		return 0;

	hist__account_cycles(sample->branch_stack, al, sample,
			     rep->nonany_branch_mode);

	bi = he->branch_info;
=======
	struct branch_info *bi = he->branch_info;
	struct perf_sample *sample = iter->sample;
	struct evsel *evsel = iter->evsel;
	int err;

	branch_type_count(&rep->brtype_stat, &bi->flags,
			  bi->from.addr, bi->to.addr);

	if (!ui__has_annotation() && !rep->symbol_ipc)
		return 0;

>>>>>>> upstream/android-13
	err = addr_map_symbol__inc_samples(&bi->from, sample, evsel);
	if (err)
		goto out;

	err = addr_map_symbol__inc_samples(&bi->to, sample, evsel);

<<<<<<< HEAD
	branch_type_count(&rep->brtype_stat, &bi->flags,
			  bi->from.addr, bi->to.addr);

=======
>>>>>>> upstream/android-13
out:
	return err;
}

static void setup_forced_leader(struct report *report,
<<<<<<< HEAD
				struct perf_evlist *evlist)
{
	if (report->group_set)
		perf_evlist__force_leader(evlist);
}

static int process_feature_event(struct perf_tool *tool,
				 union perf_event *event,
				 struct perf_session *session __maybe_unused)
{
	struct report *rep = container_of(tool, struct report, tool);

	if (event->feat.feat_id < HEADER_LAST_FEATURE)
		return perf_event__process_feature(tool, event, session);

	if (event->feat.feat_id != HEADER_LAST_FEATURE) {
		pr_err("failed: wrong feature ID: %" PRIu64 "\n",
		       event->feat.feat_id);
		return -1;
=======
				struct evlist *evlist)
{
	if (report->group_set)
		evlist__force_leader(evlist);
}

static int process_feature_event(struct perf_session *session,
				 union perf_event *event)
{
	struct report *rep = container_of(session->tool, struct report, tool);

	if (event->feat.feat_id < HEADER_LAST_FEATURE)
		return perf_event__process_feature(session, event);

	if (event->feat.feat_id != HEADER_LAST_FEATURE) {
		pr_err("failed: wrong feature ID: %" PRI_lu64 "\n",
		       event->feat.feat_id);
		return -1;
	} else if (rep->header_only) {
		session_done = 1;
>>>>>>> upstream/android-13
	}

	/*
	 * (feat_id = HEADER_LAST_FEATURE) is the end marker which
	 * means all features are received, now we can force the
	 * group if needed.
	 */
	setup_forced_leader(rep, session->evlist);
	return 0;
}

static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
<<<<<<< HEAD
				struct perf_evsel *evsel,
=======
				struct evsel *evsel,
>>>>>>> upstream/android-13
				struct machine *machine)
{
	struct report *rep = container_of(tool, struct report, tool);
	struct addr_location al;
	struct hist_entry_iter iter = {
		.evsel 			= evsel,
		.sample 		= sample,
		.hide_unresolved 	= symbol_conf.hide_unresolved,
		.add_entry_cb 		= hist_iter__report_callback,
	};
	int ret = 0;

	if (perf_time__ranges_skip_sample(rep->ptime_range, rep->range_num,
					  sample->time)) {
		return 0;
	}

<<<<<<< HEAD
=======
	if (evswitch__discard(&rep->evswitch, evsel))
		return 0;

>>>>>>> upstream/android-13
	if (machine__resolve(machine, &al, sample) < 0) {
		pr_debug("problem processing %d event, skipping it.\n",
			 event->header.type);
		return -1;
	}

<<<<<<< HEAD
=======
	if (rep->stitch_lbr)
		al.thread->lbr_stitch_enable = true;

>>>>>>> upstream/android-13
	if (symbol_conf.hide_unresolved && al.sym == NULL)
		goto out_put;

	if (rep->cpu_list && !test_bit(sample->cpu, rep->cpu_bitmap))
		goto out_put;

	if (sort__mode == SORT_MODE__BRANCH) {
		/*
		 * A non-synthesized event might not have a branch stack if
		 * branch stacks have been synthesized (using itrace options).
		 */
		if (!sample->branch_stack)
			goto out_put;

		iter.add_entry_cb = hist_iter__branch_callback;
		iter.ops = &hist_iter_branch;
	} else if (rep->mem_mode) {
		iter.ops = &hist_iter_mem;
	} else if (symbol_conf.cumulate_callchain) {
		iter.ops = &hist_iter_cumulative;
	} else {
		iter.ops = &hist_iter_normal;
	}

	if (al.map != NULL)
		al.map->dso->hit = 1;

<<<<<<< HEAD
=======
	if (ui__has_annotation() || rep->symbol_ipc || rep->total_cycles_mode) {
		hist__account_cycles(sample->branch_stack, &al, sample,
				     rep->nonany_branch_mode,
				     &rep->total_cycles);
	}

>>>>>>> upstream/android-13
	ret = hist_entry_iter__add(&iter, &al, rep->max_stack, rep);
	if (ret < 0)
		pr_debug("problem adding hist entry, skipping event\n");
out_put:
	addr_location__put(&al);
	return ret;
}

static int process_read_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
<<<<<<< HEAD
			      struct perf_evsel *evsel,
=======
			      struct evsel *evsel,
>>>>>>> upstream/android-13
			      struct machine *machine __maybe_unused)
{
	struct report *rep = container_of(tool, struct report, tool);

	if (rep->show_threads) {
<<<<<<< HEAD
		const char *name = evsel ? perf_evsel__name(evsel) : "unknown";
		int err = perf_read_values_add_value(&rep->show_threads_values,
					   event->read.pid, event->read.tid,
					   evsel->idx,
=======
		const char *name = evsel__name(evsel);
		int err = perf_read_values_add_value(&rep->show_threads_values,
					   event->read.pid, event->read.tid,
					   evsel->core.idx,
>>>>>>> upstream/android-13
					   name,
					   event->read.value);

		if (err)
			return err;
	}

	return 0;
}

/* For pipe mode, sample_type is not currently set */
static int report__setup_sample_type(struct report *rep)
{
	struct perf_session *session = rep->session;
<<<<<<< HEAD
	u64 sample_type = perf_evlist__combined_sample_type(session->evlist);
	bool is_pipe = perf_data__is_pipe(session->data);

	if (session->itrace_synth_opts->callchain ||
=======
	u64 sample_type = evlist__combined_sample_type(session->evlist);
	bool is_pipe = perf_data__is_pipe(session->data);
	struct evsel *evsel;

	if (session->itrace_synth_opts->callchain ||
	    session->itrace_synth_opts->add_callchain ||
>>>>>>> upstream/android-13
	    (!is_pipe &&
	     perf_header__has_feat(&session->header, HEADER_AUXTRACE) &&
	     !session->itrace_synth_opts->set))
		sample_type |= PERF_SAMPLE_CALLCHAIN;

<<<<<<< HEAD
	if (session->itrace_synth_opts->last_branch)
=======
	if (session->itrace_synth_opts->last_branch ||
	    session->itrace_synth_opts->add_last_branch)
>>>>>>> upstream/android-13
		sample_type |= PERF_SAMPLE_BRANCH_STACK;

	if (!is_pipe && !(sample_type & PERF_SAMPLE_CALLCHAIN)) {
		if (perf_hpp_list.parent) {
			ui__error("Selected --sort parent, but no "
				    "callchain data. Did you call "
				    "'perf record' without -g?\n");
			return -EINVAL;
		}
		if (symbol_conf.use_callchain &&
			!symbol_conf.show_branchflag_count) {
			ui__error("Selected -g or --branch-history.\n"
				  "But no callchain or branch data.\n"
				  "Did you call 'perf record' without -g or -b?\n");
			return -1;
		}
	} else if (!callchain_param.enabled &&
		   callchain_param.mode != CHAIN_NONE &&
		   !symbol_conf.use_callchain) {
			symbol_conf.use_callchain = true;
			if (callchain_register_param(&callchain_param) < 0) {
				ui__error("Can't register callchain params.\n");
				return -EINVAL;
			}
	}

	if (symbol_conf.cumulate_callchain) {
		/* Silently ignore if callchain is missing */
		if (!(sample_type & PERF_SAMPLE_CALLCHAIN)) {
			symbol_conf.cumulate_callchain = false;
			perf_hpp__cancel_cumulate();
		}
	}

	if (sort__mode == SORT_MODE__BRANCH) {
		if (!is_pipe &&
		    !(sample_type & PERF_SAMPLE_BRANCH_STACK)) {
			ui__error("Selected -b but no branch data. "
				  "Did you call perf record without -b?\n");
			return -1;
		}
	}

<<<<<<< HEAD
	if (symbol_conf.use_callchain || symbol_conf.cumulate_callchain) {
		if ((sample_type & PERF_SAMPLE_REGS_USER) &&
		    (sample_type & PERF_SAMPLE_STACK_USER)) {
			callchain_param.record_mode = CALLCHAIN_DWARF;
			dwarf_callchain_users = true;
		} else if (sample_type & PERF_SAMPLE_BRANCH_STACK)
			callchain_param.record_mode = CALLCHAIN_LBR;
		else
			callchain_param.record_mode = CALLCHAIN_FP;
	}

	/* ??? handle more cases than just ANY? */
	if (!(perf_evlist__combined_branch_type(session->evlist) &
				PERF_SAMPLE_BRANCH_ANY))
=======
	if (sort__mode == SORT_MODE__MEMORY) {
		/*
		 * FIXUP: prior to kernel 5.18, Arm SPE missed to set
		 * PERF_SAMPLE_DATA_SRC bit in sample type.  For backward
		 * compatibility, set the bit if it's an old perf data file.
		 */
		evlist__for_each_entry(session->evlist, evsel) {
			if (strstr(evsel->name, "arm_spe") &&
				!(sample_type & PERF_SAMPLE_DATA_SRC)) {
				evsel->core.attr.sample_type |= PERF_SAMPLE_DATA_SRC;
				sample_type |= PERF_SAMPLE_DATA_SRC;
			}
		}

		if (!is_pipe && !(sample_type & PERF_SAMPLE_DATA_SRC)) {
			ui__error("Selected --mem-mode but no mem data. "
				  "Did you call perf record without -d?\n");
			return -1;
		}
	}

	callchain_param_setup(sample_type);

	if (rep->stitch_lbr && (callchain_param.record_mode != CALLCHAIN_LBR)) {
		ui__warning("Can't find LBR callchain. Switch off --stitch-lbr.\n"
			    "Please apply --call-graph lbr when recording.\n");
		rep->stitch_lbr = false;
	}

	/* ??? handle more cases than just ANY? */
	if (!(evlist__combined_branch_type(session->evlist) & PERF_SAMPLE_BRANCH_ANY))
>>>>>>> upstream/android-13
		rep->nonany_branch_mode = true;

#if !defined(HAVE_LIBUNWIND_SUPPORT) && !defined(HAVE_DWARF_SUPPORT)
	if (dwarf_callchain_users) {
		ui__warning("Please install libunwind or libdw "
			    "development packages during the perf build.\n");
	}
#endif

	return 0;
}

static void sig_handler(int sig __maybe_unused)
{
	session_done = 1;
}

static size_t hists__fprintf_nr_sample_events(struct hists *hists, struct report *rep,
					      const char *evname, FILE *fp)
{
	size_t ret;
	char unit;
<<<<<<< HEAD
	unsigned long nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];
	u64 nr_events = hists->stats.total_period;
	struct perf_evsel *evsel = hists_to_evsel(hists);
=======
	unsigned long nr_samples = hists->stats.nr_samples;
	u64 nr_events = hists->stats.total_period;
	struct evsel *evsel = hists_to_evsel(hists);
>>>>>>> upstream/android-13
	char buf[512];
	size_t size = sizeof(buf);
	int socked_id = hists->socket_filter;

	if (quiet)
		return 0;

	if (symbol_conf.filter_relative) {
		nr_samples = hists->stats.nr_non_filtered_samples;
		nr_events = hists->stats.total_non_filtered_period;
	}

<<<<<<< HEAD
	if (perf_evsel__is_group_event(evsel)) {
		struct perf_evsel *pos;

		perf_evsel__group_desc(evsel, buf, size);
=======
	if (evsel__is_group_event(evsel)) {
		struct evsel *pos;

		evsel__group_desc(evsel, buf, size);
>>>>>>> upstream/android-13
		evname = buf;

		for_each_group_member(pos, evsel) {
			const struct hists *pos_hists = evsel__hists(pos);

			if (symbol_conf.filter_relative) {
				nr_samples += pos_hists->stats.nr_non_filtered_samples;
				nr_events += pos_hists->stats.total_non_filtered_period;
			} else {
<<<<<<< HEAD
				nr_samples += pos_hists->stats.nr_events[PERF_RECORD_SAMPLE];
=======
				nr_samples += pos_hists->stats.nr_samples;
>>>>>>> upstream/android-13
				nr_events += pos_hists->stats.total_period;
			}
		}
	}

	nr_samples = convert_unit(nr_samples, &unit);
	ret = fprintf(fp, "# Samples: %lu%c", nr_samples, unit);
	if (evname != NULL) {
		ret += fprintf(fp, " of event%s '%s'",
<<<<<<< HEAD
			       evsel->nr_members > 1 ? "s" : "", evname);
=======
			       evsel->core.nr_members > 1 ? "s" : "", evname);
>>>>>>> upstream/android-13
	}

	if (rep->time_str)
		ret += fprintf(fp, " (time slices: %s)", rep->time_str);

	if (symbol_conf.show_ref_callgraph && evname && strstr(evname, "call-graph=no")) {
		ret += fprintf(fp, ", show reference callgraph");
	}

	if (rep->mem_mode) {
		ret += fprintf(fp, "\n# Total weight : %" PRIu64, nr_events);
		ret += fprintf(fp, "\n# Sort order   : %s", sort_order ? : default_mem_sort_order);
	} else
		ret += fprintf(fp, "\n# Event count (approx.): %" PRIu64, nr_events);

	if (socked_id > -1)
		ret += fprintf(fp, "\n# Processor Socket: %d", socked_id);

	return ret + fprintf(fp, "\n#\n");
}

<<<<<<< HEAD
static int perf_evlist__tty_browse_hists(struct perf_evlist *evlist,
					 struct report *rep,
					 const char *help)
{
	struct perf_evsel *pos;
=======
static int evlist__tui_block_hists_browse(struct evlist *evlist, struct report *rep)
{
	struct evsel *pos;
	int i = 0, ret;

	evlist__for_each_entry(evlist, pos) {
		ret = report__browse_block_hists(&rep->block_reports[i++].hist,
						 rep->min_percent, pos,
						 &rep->session->header.env,
						 &rep->annotation_opts);
		if (ret != 0)
			return ret;
	}

	return 0;
}

static int evlist__tty_browse_hists(struct evlist *evlist, struct report *rep, const char *help)
{
	struct evsel *pos;
	int i = 0;
>>>>>>> upstream/android-13

	if (!quiet) {
		fprintf(stdout, "#\n# Total Lost Samples: %" PRIu64 "\n#\n",
			evlist->stats.total_lost_samples);
	}

	evlist__for_each_entry(evlist, pos) {
		struct hists *hists = evsel__hists(pos);
<<<<<<< HEAD
		const char *evname = perf_evsel__name(pos);

		if (symbol_conf.event_group &&
		    !perf_evsel__is_group_leader(pos))
			continue;

		hists__fprintf_nr_sample_events(hists, rep, evname, stdout);
=======
		const char *evname = evsel__name(pos);

		if (symbol_conf.event_group && !evsel__is_group_leader(pos))
			continue;

		if (rep->skip_empty && !hists->stats.nr_samples)
			continue;

		hists__fprintf_nr_sample_events(hists, rep, evname, stdout);

		if (rep->total_cycles_mode) {
			report__browse_block_hists(&rep->block_reports[i++].hist,
						   rep->min_percent, pos,
						   NULL, NULL);
			continue;
		}

>>>>>>> upstream/android-13
		hists__fprintf(hists, !quiet, 0, 0, rep->min_percent, stdout,
			       !(symbol_conf.use_callchain ||
			         symbol_conf.show_branchflag_count));
		fprintf(stdout, "\n\n");
	}

	if (!quiet)
		fprintf(stdout, "#\n# (%s)\n#\n", help);

	if (rep->show_threads) {
		bool style = !strcmp(rep->pretty_printing_style, "raw");
		perf_read_values_display(stdout, &rep->show_threads_values,
					 style);
		perf_read_values_destroy(&rep->show_threads_values);
	}

	if (sort__mode == SORT_MODE__BRANCH)
		branch_type_stat_display(stdout, &rep->brtype_stat);

	return 0;
}

static void report__warn_kptr_restrict(const struct report *rep)
{
	struct map *kernel_map = machine__kernel_map(&rep->session->machines.host);
	struct kmap *kernel_kmap = kernel_map ? map__kmap(kernel_map) : NULL;

<<<<<<< HEAD
	if (perf_evlist__exclude_kernel(rep->session->evlist))
=======
	if (evlist__exclude_kernel(rep->session->evlist))
>>>>>>> upstream/android-13
		return;

	if (kernel_map == NULL ||
	    (kernel_map->dso->hit &&
	     (kernel_kmap->ref_reloc_sym == NULL ||
	      kernel_kmap->ref_reloc_sym->addr == 0))) {
		const char *desc =
		    "As no suitable kallsyms nor vmlinux was found, kernel samples\n"
		    "can't be resolved.";

		if (kernel_map && map__has_symbols(kernel_map)) {
			desc = "If some relocation was applied (e.g. "
			       "kexec) symbols may be misresolved.";
		}

		ui__warning(
"Kernel address maps (/proc/{kallsyms,modules}) were restricted.\n\n"
"Check /proc/sys/kernel/kptr_restrict before running 'perf record'.\n\n%s\n\n"
"Samples in kernel modules can't be resolved as well.\n\n",
		desc);
	}
}

static int report__gtk_browse_hists(struct report *rep, const char *help)
{
<<<<<<< HEAD
	int (*hist_browser)(struct perf_evlist *evlist, const char *help,
			    struct hist_browser_timer *timer, float min_pcnt);

	hist_browser = dlsym(perf_gtk_handle, "perf_evlist__gtk_browse_hists");
=======
	int (*hist_browser)(struct evlist *evlist, const char *help,
			    struct hist_browser_timer *timer, float min_pcnt);

	hist_browser = dlsym(perf_gtk_handle, "evlist__gtk_browse_hists");
>>>>>>> upstream/android-13

	if (hist_browser == NULL) {
		ui__error("GTK browser not found!\n");
		return -1;
	}

	return hist_browser(rep->session->evlist, help, NULL, rep->min_percent);
}

static int report__browse_hists(struct report *rep)
{
	int ret;
	struct perf_session *session = rep->session;
<<<<<<< HEAD
	struct perf_evlist *evlist = session->evlist;
	const char *help = perf_tip(system_path(TIPDIR));

	if (help == NULL) {
		/* fallback for people who don't install perf ;-) */
		help = perf_tip(DOCDIR);
		if (help == NULL)
			help = "Cannot load tips.txt file, please install perf!";
	}

	switch (use_browser) {
	case 1:
		ret = perf_evlist__tui_browse_hists(evlist, help, NULL,
						    rep->min_percent,
						    &session->header.env,
						    true, &rep->annotation_opts);
=======
	struct evlist *evlist = session->evlist;
	char *help = NULL, *path = NULL;

	path = system_path(TIPDIR);
	if (perf_tip(&help, path) || help == NULL) {
		/* fallback for people who don't install perf ;-) */
		free(path);
		path = system_path(DOCDIR);
		if (perf_tip(&help, path) || help == NULL)
			help = strdup("Cannot load tips.txt file, please install perf!");
	}
	free(path);

	switch (use_browser) {
	case 1:
		if (rep->total_cycles_mode) {
			ret = evlist__tui_block_hists_browse(evlist, rep);
			break;
		}

		ret = evlist__tui_browse_hists(evlist, help, NULL, rep->min_percent,
					       &session->header.env, true, &rep->annotation_opts);
>>>>>>> upstream/android-13
		/*
		 * Usually "ret" is the last pressed key, and we only
		 * care if the key notifies us to switch data file.
		 */
<<<<<<< HEAD
		if (ret != K_SWITCH_INPUT_DATA)
=======
		if (ret != K_SWITCH_INPUT_DATA && ret != K_RELOAD)
>>>>>>> upstream/android-13
			ret = 0;
		break;
	case 2:
		ret = report__gtk_browse_hists(rep, help);
		break;
	default:
<<<<<<< HEAD
		ret = perf_evlist__tty_browse_hists(evlist, rep, help);
		break;
	}

=======
		ret = evlist__tty_browse_hists(evlist, rep, help);
		break;
	}
	free(help);
>>>>>>> upstream/android-13
	return ret;
}

static int report__collapse_hists(struct report *rep)
{
	struct ui_progress prog;
<<<<<<< HEAD
	struct perf_evsel *pos;
=======
	struct evsel *pos;
>>>>>>> upstream/android-13
	int ret = 0;

	ui_progress__init(&prog, rep->nr_entries, "Merging related events...");

	evlist__for_each_entry(rep->session->evlist, pos) {
		struct hists *hists = evsel__hists(pos);

<<<<<<< HEAD
		if (pos->idx == 0)
=======
		if (pos->core.idx == 0)
>>>>>>> upstream/android-13
			hists->symbol_filter_str = rep->symbol_filter_str;

		hists->socket_filter = rep->socket_filter;

		ret = hists__collapse_resort(hists, &prog);
		if (ret < 0)
			break;

		/* Non-group events are considered as leader */
<<<<<<< HEAD
		if (symbol_conf.event_group &&
		    !perf_evsel__is_group_leader(pos)) {
			struct hists *leader_hists = evsel__hists(pos->leader);
=======
		if (symbol_conf.event_group && !evsel__is_group_leader(pos)) {
			struct hists *leader_hists = evsel__hists(evsel__leader(pos));
>>>>>>> upstream/android-13

			hists__match(leader_hists, hists);
			hists__link(leader_hists, hists);
		}
	}

	ui_progress__finish();
	return ret;
}

<<<<<<< HEAD
static void report__output_resort(struct report *rep)
{
	struct ui_progress prog;
	struct perf_evsel *pos;

	ui_progress__init(&prog, rep->nr_entries, "Sorting events for output...");

	evlist__for_each_entry(rep->session->evlist, pos)
		perf_evsel__output_resort(pos, &prog);
=======
static int hists__resort_cb(struct hist_entry *he, void *arg)
{
	struct report *rep = arg;
	struct symbol *sym = he->ms.sym;

	if (rep->symbol_ipc && sym && !sym->annotate2) {
		struct evsel *evsel = hists_to_evsel(he->hists);

		symbol__annotate2(&he->ms, evsel,
				  &annotation__default_options, NULL);
	}

	return 0;
}

static void report__output_resort(struct report *rep)
{
	struct ui_progress prog;
	struct evsel *pos;

	ui_progress__init(&prog, rep->nr_entries, "Sorting events for output...");

	evlist__for_each_entry(rep->session->evlist, pos) {
		evsel__output_resort_cb(pos, &prog, hists__resort_cb, rep);
	}
>>>>>>> upstream/android-13

	ui_progress__finish();
}

<<<<<<< HEAD
static void stats_setup(struct report *rep)
{
	memset(&rep->tool, 0, sizeof(rep->tool));
=======
static int count_sample_event(struct perf_tool *tool __maybe_unused,
			      union perf_event *event __maybe_unused,
			      struct perf_sample *sample __maybe_unused,
			      struct evsel *evsel,
			      struct machine *machine __maybe_unused)
{
	struct hists *hists = evsel__hists(evsel);

	hists__inc_nr_events(hists);
	return 0;
}

static int process_attr(struct perf_tool *tool __maybe_unused,
			union perf_event *event,
			struct evlist **pevlist);

static void stats_setup(struct report *rep)
{
	memset(&rep->tool, 0, sizeof(rep->tool));
	rep->tool.attr = process_attr;
	rep->tool.sample = count_sample_event;
>>>>>>> upstream/android-13
	rep->tool.no_warn = true;
}

static int stats_print(struct report *rep)
{
	struct perf_session *session = rep->session;

<<<<<<< HEAD
	perf_session__fprintf_nr_events(session, stdout);
=======
	perf_session__fprintf_nr_events(session, stdout, rep->skip_empty);
	evlist__fprintf_nr_events(session->evlist, stdout, rep->skip_empty);
>>>>>>> upstream/android-13
	return 0;
}

static void tasks_setup(struct report *rep)
{
	memset(&rep->tool, 0, sizeof(rep->tool));
	rep->tool.ordered_events = true;
	if (rep->mmaps_mode) {
		rep->tool.mmap = perf_event__process_mmap;
		rep->tool.mmap2 = perf_event__process_mmap2;
	}
<<<<<<< HEAD
=======
	rep->tool.attr = process_attr;
>>>>>>> upstream/android-13
	rep->tool.comm = perf_event__process_comm;
	rep->tool.exit = perf_event__process_exit;
	rep->tool.fork = perf_event__process_fork;
	rep->tool.no_warn = true;
}

struct task {
	struct thread		*thread;
	struct list_head	 list;
	struct list_head	 children;
};

static struct task *tasks_list(struct task *task, struct machine *machine)
{
	struct thread *parent_thread, *thread = task->thread;
	struct task   *parent_task;

	/* Already listed. */
	if (!list_empty(&task->list))
		return NULL;

	/* Last one in the chain. */
	if (thread->ppid == -1)
		return task;

	parent_thread = machine__find_thread(machine, -1, thread->ppid);
	if (!parent_thread)
		return ERR_PTR(-ENOENT);

	parent_task = thread__priv(parent_thread);
	list_add_tail(&task->list, &parent_task->children);
	return tasks_list(parent_task, machine);
}

static size_t maps__fprintf_task(struct maps *maps, int indent, FILE *fp)
{
	size_t printed = 0;
<<<<<<< HEAD
	struct rb_node *nd;

	for (nd = rb_first(&maps->entries); nd; nd = rb_next(nd)) {
		struct map *map = rb_entry(nd, struct map, rb_node);

=======
	struct map *map;

	maps__for_each_entry(maps, map) {
>>>>>>> upstream/android-13
		printed += fprintf(fp, "%*s  %" PRIx64 "-%" PRIx64 " %c%c%c%c %08" PRIx64 " %" PRIu64 " %s\n",
				   indent, "", map->start, map->end,
				   map->prot & PROT_READ ? 'r' : '-',
				   map->prot & PROT_WRITE ? 'w' : '-',
				   map->prot & PROT_EXEC ? 'x' : '-',
				   map->flags & MAP_SHARED ? 's' : 'p',
				   map->pgoff,
<<<<<<< HEAD
				   map->ino, map->dso->name);
=======
				   map->dso->id.ino, map->dso->name);
>>>>>>> upstream/android-13
	}

	return printed;
}

<<<<<<< HEAD
static int map_groups__fprintf_task(struct map_groups *mg, int indent, FILE *fp)
{
	return maps__fprintf_task(&mg->maps, indent, fp);
}

=======
>>>>>>> upstream/android-13
static void task__print_level(struct task *task, FILE *fp, int level)
{
	struct thread *thread = task->thread;
	struct task *child;
	int comm_indent = fprintf(fp, "  %8d %8d %8d |%*s",
				  thread->pid_, thread->tid, thread->ppid,
				  level, "");

	fprintf(fp, "%s\n", thread__comm_str(thread));

<<<<<<< HEAD
	map_groups__fprintf_task(thread->mg, comm_indent, fp);
=======
	maps__fprintf_task(thread->maps, comm_indent, fp);
>>>>>>> upstream/android-13

	if (!list_empty(&task->children)) {
		list_for_each_entry(child, &task->children, list)
			task__print_level(child, fp, level + 1);
	}
}

static int tasks_print(struct report *rep, FILE *fp)
{
	struct perf_session *session = rep->session;
	struct machine      *machine = &session->machines.host;
	struct task *tasks, *task;
	unsigned int nr = 0, itask = 0, i;
	struct rb_node *nd;
	LIST_HEAD(list);

	/*
	 * No locking needed while accessing machine->threads,
	 * because --tasks is single threaded command.
	 */

	/* Count all the threads. */
	for (i = 0; i < THREADS__TABLE_SIZE; i++)
		nr += machine->threads[i].nr;

	tasks = malloc(sizeof(*tasks) * nr);
	if (!tasks)
		return -ENOMEM;

	for (i = 0; i < THREADS__TABLE_SIZE; i++) {
		struct threads *threads = &machine->threads[i];

<<<<<<< HEAD
		for (nd = rb_first(&threads->entries); nd; nd = rb_next(nd)) {
=======
		for (nd = rb_first_cached(&threads->entries); nd;
		     nd = rb_next(nd)) {
>>>>>>> upstream/android-13
			task = tasks + itask++;

			task->thread = rb_entry(nd, struct thread, rb_node);
			INIT_LIST_HEAD(&task->children);
			INIT_LIST_HEAD(&task->list);
			thread__set_priv(task->thread, task);
		}
	}

	/*
	 * Iterate every task down to the unprocessed parent
	 * and link all in task children list. Task with no
	 * parent is added into 'list'.
	 */
	for (itask = 0; itask < nr; itask++) {
		task = tasks + itask;

		if (!list_empty(&task->list))
			continue;

		task = tasks_list(task, machine);
		if (IS_ERR(task)) {
			pr_err("Error: failed to process tasks\n");
			free(tasks);
			return PTR_ERR(task);
		}

		if (task)
			list_add_tail(&task->list, &list);
	}

	fprintf(fp, "# %8s %8s %8s  %s\n", "pid", "tid", "ppid", "comm");

	list_for_each_entry(task, &list, list)
		task__print_level(task, fp, 0);

	free(tasks);
	return 0;
}

static int __cmd_report(struct report *rep)
{
	int ret;
	struct perf_session *session = rep->session;
<<<<<<< HEAD
	struct perf_evsel *pos;
=======
	struct evsel *pos;
>>>>>>> upstream/android-13
	struct perf_data *data = session->data;

	signal(SIGINT, sig_handler);

	if (rep->cpu_list) {
		ret = perf_session__cpu_bitmap(session, rep->cpu_list,
					       rep->cpu_bitmap);
		if (ret) {
			ui__error("failed to set cpu bitmap\n");
			return ret;
		}
		session->itrace_synth_opts->cpu_bitmap = rep->cpu_bitmap;
	}

	if (rep->show_threads) {
		ret = perf_read_values_init(&rep->show_threads_values);
		if (ret)
			return ret;
	}

	ret = report__setup_sample_type(rep);
	if (ret) {
		/* report__setup_sample_type() already showed error message */
		return ret;
	}

	if (rep->stats_mode)
		stats_setup(rep);

	if (rep->tasks_mode)
		tasks_setup(rep);

	ret = perf_session__process_events(session);
	if (ret) {
		ui__error("failed to process sample\n");
		return ret;
	}

<<<<<<< HEAD
=======
	evlist__check_mem_load_aux(session->evlist);

>>>>>>> upstream/android-13
	if (rep->stats_mode)
		return stats_print(rep);

	if (rep->tasks_mode)
		return tasks_print(rep, stdout);

	report__warn_kptr_restrict(rep);

	evlist__for_each_entry(session->evlist, pos)
		rep->nr_entries += evsel__hists(pos)->nr_entries;

	if (use_browser == 0) {
		if (verbose > 3)
			perf_session__fprintf(session, stdout);

		if (verbose > 2)
			perf_session__fprintf_dsos(session, stdout);

		if (dump_trace) {
<<<<<<< HEAD
			perf_session__fprintf_nr_events(session, stdout);
			perf_evlist__fprintf_nr_events(session->evlist, stdout);
=======
			perf_session__fprintf_nr_events(session, stdout,
							rep->skip_empty);
			evlist__fprintf_nr_events(session->evlist, stdout,
						  rep->skip_empty);
>>>>>>> upstream/android-13
			return 0;
		}
	}

	ret = report__collapse_hists(rep);
	if (ret) {
		ui__error("failed to process hist entry\n");
		return ret;
	}

	if (session_done())
		return 0;

	/*
	 * recalculate number of entries after collapsing since it
	 * might be changed during the collapse phase.
	 */
	rep->nr_entries = 0;
	evlist__for_each_entry(session->evlist, pos)
		rep->nr_entries += evsel__hists(pos)->nr_entries;

	if (rep->nr_entries == 0) {
<<<<<<< HEAD
		ui__error("The %s file has no samples!\n", data->file.path);
=======
		ui__error("The %s data has no samples!\n", data->path);
>>>>>>> upstream/android-13
		return 0;
	}

	report__output_resort(rep);

<<<<<<< HEAD
=======
	if (rep->total_cycles_mode) {
		int block_hpps[6] = {
			PERF_HPP_REPORT__BLOCK_TOTAL_CYCLES_PCT,
			PERF_HPP_REPORT__BLOCK_LBR_CYCLES,
			PERF_HPP_REPORT__BLOCK_CYCLES_PCT,
			PERF_HPP_REPORT__BLOCK_AVG_CYCLES,
			PERF_HPP_REPORT__BLOCK_RANGE,
			PERF_HPP_REPORT__BLOCK_DSO,
		};

		rep->block_reports = block_info__create_report(session->evlist,
							       rep->total_cycles,
							       block_hpps, 6,
							       &rep->nr_block_reports);
		if (!rep->block_reports)
			return -1;
	}

>>>>>>> upstream/android-13
	return report__browse_hists(rep);
}

static int
report_parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	struct callchain_param *callchain = opt->value;

	callchain->enabled = !unset;
	/*
	 * --no-call-graph
	 */
	if (unset) {
		symbol_conf.use_callchain = false;
		callchain->mode = CHAIN_NONE;
		return 0;
	}

	return parse_callchain_report_opt(arg);
}

<<<<<<< HEAD
=======
static int
parse_time_quantum(const struct option *opt, const char *arg,
		   int unset __maybe_unused)
{
	unsigned long *time_q = opt->value;
	char *end;

	*time_q = strtoul(arg, &end, 0);
	if (end == arg)
		goto parse_err;
	if (*time_q == 0) {
		pr_err("time quantum cannot be 0");
		return -1;
	}
	end = skip_spaces(end);
	if (*end == 0)
		return 0;
	if (!strcmp(end, "s")) {
		*time_q *= NSEC_PER_SEC;
		return 0;
	}
	if (!strcmp(end, "ms")) {
		*time_q *= NSEC_PER_MSEC;
		return 0;
	}
	if (!strcmp(end, "us")) {
		*time_q *= NSEC_PER_USEC;
		return 0;
	}
	if (!strcmp(end, "ns"))
		return 0;
parse_err:
	pr_err("Cannot parse time quantum `%s'\n", arg);
	return -1;
}

>>>>>>> upstream/android-13
int
report_parse_ignore_callees_opt(const struct option *opt __maybe_unused,
				const char *arg, int unset __maybe_unused)
{
	if (arg) {
		int err = regcomp(&ignore_callees_regex, arg, REG_EXTENDED);
		if (err) {
			char buf[BUFSIZ];
			regerror(err, &ignore_callees_regex, buf, sizeof(buf));
			pr_err("Invalid --ignore-callees regex: %s\n%s", arg, buf);
			return -1;
		}
		have_ignore_callees = 1;
	}

	return 0;
}

static int
parse_branch_mode(const struct option *opt,
		  const char *str __maybe_unused, int unset)
{
	int *branch_mode = opt->value;

	*branch_mode = !unset;
	return 0;
}

static int
parse_percent_limit(const struct option *opt, const char *str,
		    int unset __maybe_unused)
{
	struct report *rep = opt->value;
	double pcnt = strtof(str, NULL);

	rep->min_percent = pcnt;
	callchain_param.min_percent = pcnt;
	return 0;
}

<<<<<<< HEAD
=======
static int process_attr(struct perf_tool *tool __maybe_unused,
			union perf_event *event,
			struct evlist **pevlist)
{
	u64 sample_type;
	int err;

	err = perf_event__process_attr(tool, event, pevlist);
	if (err)
		return err;

	/*
	 * Check if we need to enable callchains based
	 * on events sample_type.
	 */
	sample_type = evlist__combined_sample_type(*pevlist);
	callchain_param_setup(sample_type);
	return 0;
}

>>>>>>> upstream/android-13
int cmd_report(int argc, const char **argv)
{
	struct perf_session *session;
	struct itrace_synth_opts itrace_synth_opts = { .set = 0, };
	struct stat st;
	bool has_br_stack = false;
	int branch_mode = -1;
	int last_key = 0;
	bool branch_call_mode = false;
#define CALLCHAIN_DEFAULT_OPT  "graph,0.5,caller,function,percent"
<<<<<<< HEAD
	const char report_callchain_help[] = "Display call graph (stack chain/backtrace):\n\n"
					     CALLCHAIN_REPORT_HELP
					     "\n\t\t\t\tDefault: " CALLCHAIN_DEFAULT_OPT;
=======
	static const char report_callchain_help[] = "Display call graph (stack chain/backtrace):\n\n"
						    CALLCHAIN_REPORT_HELP
						    "\n\t\t\t\tDefault: " CALLCHAIN_DEFAULT_OPT;
>>>>>>> upstream/android-13
	char callchain_default_opt[] = CALLCHAIN_DEFAULT_OPT;
	const char * const report_usage[] = {
		"perf report [<options>]",
		NULL
	};
	struct report report = {
		.tool = {
			.sample		 = process_sample_event,
			.mmap		 = perf_event__process_mmap,
			.mmap2		 = perf_event__process_mmap2,
			.comm		 = perf_event__process_comm,
			.namespaces	 = perf_event__process_namespaces,
<<<<<<< HEAD
=======
			.cgroup		 = perf_event__process_cgroup,
>>>>>>> upstream/android-13
			.exit		 = perf_event__process_exit,
			.fork		 = perf_event__process_fork,
			.lost		 = perf_event__process_lost,
			.read		 = process_read_event,
<<<<<<< HEAD
			.attr		 = perf_event__process_attr,
=======
			.attr		 = process_attr,
>>>>>>> upstream/android-13
			.tracing_data	 = perf_event__process_tracing_data,
			.build_id	 = perf_event__process_build_id,
			.id_index	 = perf_event__process_id_index,
			.auxtrace_info	 = perf_event__process_auxtrace_info,
			.auxtrace	 = perf_event__process_auxtrace,
			.event_update	 = perf_event__process_event_update,
			.feature	 = process_feature_event,
			.ordered_events	 = true,
			.ordering_requires_timestamps = true,
		},
		.max_stack		 = PERF_MAX_STACK_DEPTH,
		.pretty_printing_style	 = "normal",
		.socket_filter		 = -1,
		.annotation_opts	 = annotation__default_options,
<<<<<<< HEAD
	};
=======
		.skip_empty		 = true,
	};
	char *sort_order_help = sort_help("sort by key(s):");
	char *field_order_help = sort_help("output field(s): overhead period sample ");
>>>>>>> upstream/android-13
	const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('q', "quiet", &quiet, "Do not show any message"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_BOOLEAN(0, "stats", &report.stats_mode, "Display event stats"),
	OPT_BOOLEAN(0, "tasks", &report.tasks_mode, "Display recorded tasks"),
	OPT_BOOLEAN(0, "mmaps", &report.mmaps_mode, "Display recorded tasks memory maps"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_BOOLEAN(0, "ignore-vmlinux", &symbol_conf.ignore_vmlinux,
                    "don't load vmlinux even if found"),
	OPT_STRING(0, "kallsyms", &symbol_conf.kallsyms_name,
		   "file", "kallsyms pathname"),
	OPT_BOOLEAN('f', "force", &symbol_conf.force, "don't complain, do it"),
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
	OPT_BOOLEAN('n', "show-nr-samples", &symbol_conf.show_nr_samples,
		    "Show a column with the number of samples"),
	OPT_BOOLEAN('T', "threads", &report.show_threads,
		    "Show per-thread event counters"),
	OPT_STRING(0, "pretty", &report.pretty_printing_style, "key",
		   "pretty printing style key: normal raw"),
	OPT_BOOLEAN(0, "tui", &report.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "gtk", &report.use_gtk, "Use the GTK2 interface"),
	OPT_BOOLEAN(0, "stdio", &report.use_stdio,
		    "Use the stdio interface"),
	OPT_BOOLEAN(0, "header", &report.header, "Show data header."),
	OPT_BOOLEAN(0, "header-only", &report.header_only,
		    "Show only data header."),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
<<<<<<< HEAD
		   "sort by key(s): pid, comm, dso, symbol, parent, cpu, srcline, ..."
		   " Please refer the man page for the complete list."),
	OPT_STRING('F', "fields", &field_order, "key[,keys...]",
		   "output field(s): overhead, period, sample plus all of sort keys"),
=======
		   sort_order_help),
	OPT_STRING('F', "fields", &field_order, "key[,keys...]",
		   field_order_help),
>>>>>>> upstream/android-13
	OPT_BOOLEAN(0, "show-cpu-utilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes"),
	OPT_BOOLEAN_FLAG(0, "showcpuutilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes", PARSE_OPT_HIDDEN),
	OPT_STRING('p', "parent", &parent_pattern, "regex",
		   "regex filter to identify parent, see: '--sort parent'"),
	OPT_BOOLEAN('x', "exclude-other", &symbol_conf.exclude_other,
		    "Only display entries with parent-match"),
	OPT_CALLBACK_DEFAULT('g', "call-graph", &callchain_param,
			     "print_type,threshold[,print_limit],order,sort_key[,branch],value",
			     report_callchain_help, &report_parse_callchain_opt,
			     callchain_default_opt),
	OPT_BOOLEAN(0, "children", &symbol_conf.cumulate_callchain,
<<<<<<< HEAD
		    "Accumulate callchains of children and show total overhead as well"),
=======
		    "Accumulate callchains of children and show total overhead as well. "
		    "Enabled by default, use --no-children to disable."),
>>>>>>> upstream/android-13
	OPT_INTEGER(0, "max-stack", &report.max_stack,
		    "Set the maximum stack depth when parsing the callchain, "
		    "anything beyond the specified depth will be ignored. "
		    "Default: kernel.perf_event_max_stack or " __stringify(PERF_MAX_STACK_DEPTH)),
	OPT_BOOLEAN('G', "inverted", &report.inverted_callchain,
		    "alias for inverted call graph"),
	OPT_CALLBACK(0, "ignore-callees", NULL, "regex",
		   "ignore callees of these functions in call graphs",
		   report_parse_ignore_callees_opt),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('c', "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
	OPT_STRING(0, "pid", &symbol_conf.pid_list_str, "pid[,pid...]",
		   "only consider symbols in these pids"),
	OPT_STRING(0, "tid", &symbol_conf.tid_list_str, "tid[,tid...]",
		   "only consider symbols in these tids"),
	OPT_STRING('S', "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
	OPT_STRING(0, "symbol-filter", &report.symbol_filter_str, "filter",
		   "only show symbols that (partially) match with this filter"),
	OPT_STRING('w', "column-widths", &symbol_conf.col_width_list_str,
		   "width[,width...]",
		   "don't try to adjust column width, use these fixed values"),
	OPT_STRING_NOEMPTY('t', "field-separator", &symbol_conf.field_sep, "separator",
		   "separator for columns, no spaces will be added between "
		   "columns '.' is reserved."),
	OPT_BOOLEAN('U', "hide-unresolved", &symbol_conf.hide_unresolved,
		    "Only display entries resolved to a symbol"),
	OPT_CALLBACK(0, "symfs", NULL, "directory",
		     "Look for files with symbols relative to this directory",
		     symbol__config_symfs),
	OPT_STRING('C', "cpu", &report.cpu_list, "cpu",
		   "list of cpus to profile"),
	OPT_BOOLEAN('I', "show-info", &report.show_full_info,
		    "Display extended information about perf.data file"),
	OPT_BOOLEAN(0, "source", &report.annotation_opts.annotate_src,
		    "Interleave source code with assembly code (default)"),
	OPT_BOOLEAN(0, "asm-raw", &report.annotation_opts.show_asm_raw,
		    "Display raw encoding of assembly instructions (default)"),
	OPT_STRING('M', "disassembler-style", &report.annotation_opts.disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
<<<<<<< HEAD
=======
	OPT_STRING(0, "prefix", &report.annotation_opts.prefix, "prefix",
		    "Add prefix to source file path names in programs (with --prefix-strip)"),
	OPT_STRING(0, "prefix-strip", &report.annotation_opts.prefix_strip, "N",
		    "Strip first N entries of source file path name in programs (with --prefix)"),
>>>>>>> upstream/android-13
	OPT_BOOLEAN(0, "show-total-period", &symbol_conf.show_total_period,
		    "Show a column with the sum of periods"),
	OPT_BOOLEAN_SET(0, "group", &symbol_conf.event_group, &report.group_set,
		    "Show event group information together"),
<<<<<<< HEAD
=======
	OPT_INTEGER(0, "group-sort-idx", &symbol_conf.group_sort_idx,
		    "Sort the output by the event at the index n in group. "
		    "If n is invalid, sort by the first event. "
		    "WARNING: should be used on grouped events."),
>>>>>>> upstream/android-13
	OPT_CALLBACK_NOOPT('b', "branch-stack", &branch_mode, "",
		    "use branch records for per branch histogram filling",
		    parse_branch_mode),
	OPT_BOOLEAN(0, "branch-history", &branch_call_mode,
		    "add last branch records to call history"),
	OPT_STRING(0, "objdump", &report.annotation_opts.objdump_path, "path",
		   "objdump binary to use for disassembly and annotations"),
	OPT_BOOLEAN(0, "demangle", &symbol_conf.demangle,
		    "Disable symbol demangling"),
	OPT_BOOLEAN(0, "demangle-kernel", &symbol_conf.demangle_kernel,
		    "Enable kernel symbol demangling"),
	OPT_BOOLEAN(0, "mem-mode", &report.mem_mode, "mem access profile"),
<<<<<<< HEAD
=======
	OPT_INTEGER(0, "samples", &symbol_conf.res_sample,
		    "Number of samples to save per histogram entry for individual browsing"),
>>>>>>> upstream/android-13
	OPT_CALLBACK(0, "percent-limit", &report, "percent",
		     "Don't show entries under that percent", parse_percent_limit),
	OPT_CALLBACK(0, "percentage", NULL, "relative|absolute",
		     "how to display percentage of filtered entries", parse_filter_percentage),
	OPT_CALLBACK_OPTARG(0, "itrace", &itrace_synth_opts, NULL, "opts",
<<<<<<< HEAD
			    "Instruction Tracing options",
=======
			    "Instruction Tracing options\n" ITRACE_HELP,
>>>>>>> upstream/android-13
			    itrace_parse_synth_opts),
	OPT_BOOLEAN(0, "full-source-path", &srcline_full_filename,
			"Show full source file name path for source lines"),
	OPT_BOOLEAN(0, "show-ref-call-graph", &symbol_conf.show_ref_callgraph,
		    "Show callgraph from reference event"),
<<<<<<< HEAD
=======
	OPT_BOOLEAN(0, "stitch-lbr", &report.stitch_lbr,
		    "Enable LBR callgraph stitching approach"),
>>>>>>> upstream/android-13
	OPT_INTEGER(0, "socket-filter", &report.socket_filter,
		    "only show processor socket that match with this filter"),
	OPT_BOOLEAN(0, "raw-trace", &symbol_conf.raw_trace,
		    "Show raw trace event output (do not use print fmt or plugins)"),
	OPT_BOOLEAN(0, "hierarchy", &symbol_conf.report_hierarchy,
		    "Show entries in a hierarchy"),
	OPT_CALLBACK_DEFAULT(0, "stdio-color", NULL, "mode",
			     "'always' (default), 'never' or 'auto' only applicable to --stdio mode",
			     stdio__config_color, "always"),
	OPT_STRING(0, "time", &report.time_str, "str",
		   "Time span of interest (start,stop)"),
	OPT_BOOLEAN(0, "inline", &symbol_conf.inline_name,
		    "Show inline function"),
	OPT_CALLBACK(0, "percent-type", &report.annotation_opts, "local-period",
		     "Set percent type local/global-period/hits",
		     annotate_parse_percent_type),
<<<<<<< HEAD
=======
	OPT_BOOLEAN(0, "ns", &symbol_conf.nanosecs, "Show times in nanosecs"),
	OPT_CALLBACK(0, "time-quantum", &symbol_conf.time_quantum, "time (ms|us|ns|s)",
		     "Set time quantum for time sort key (default 100ms)",
		     parse_time_quantum),
	OPTS_EVSWITCH(&report.evswitch),
	OPT_BOOLEAN(0, "total-cycles", &report.total_cycles_mode,
		    "Sort all blocks by 'Sampled Cycles%'"),
	OPT_BOOLEAN(0, "disable-order", &report.disable_order,
		    "Disable raw trace ordering"),
	OPT_BOOLEAN(0, "skip-empty", &report.skip_empty,
		    "Do not display empty (or dummy) events in the output"),
>>>>>>> upstream/android-13
	OPT_END()
	};
	struct perf_data data = {
		.mode  = PERF_DATA_MODE_READ,
	};
	int ret = hists__init();
<<<<<<< HEAD

	if (ret < 0)
		return ret;

	ret = perf_config(report__config, &report);
	if (ret)
		return ret;
=======
	char sort_tmp[128];

	if (ret < 0)
		goto exit;

	ret = perf_config(report__config, &report);
	if (ret)
		goto exit;
>>>>>>> upstream/android-13

	argc = parse_options(argc, argv, options, report_usage, 0);
	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(report_usage, options);

		report.symbol_filter_str = argv[0];
	}

<<<<<<< HEAD
	if (report.mmaps_mode)
		report.tasks_mode = true;

=======
	if (annotate_check_args(&report.annotation_opts) < 0) {
		ret = -EINVAL;
		goto exit;
	}

	if (report.mmaps_mode)
		report.tasks_mode = true;

	if (dump_trace && report.disable_order)
		report.tool.ordered_events = false;

>>>>>>> upstream/android-13
	if (quiet)
		perf_quiet_option();

	if (symbol_conf.vmlinux_name &&
	    access(symbol_conf.vmlinux_name, R_OK)) {
		pr_err("Invalid file: %s\n", symbol_conf.vmlinux_name);
<<<<<<< HEAD
		return -EINVAL;
=======
		ret = -EINVAL;
		goto exit;
>>>>>>> upstream/android-13
	}
	if (symbol_conf.kallsyms_name &&
	    access(symbol_conf.kallsyms_name, R_OK)) {
		pr_err("Invalid file: %s\n", symbol_conf.kallsyms_name);
<<<<<<< HEAD
		return -EINVAL;
=======
		ret = -EINVAL;
		goto exit;
>>>>>>> upstream/android-13
	}

	if (report.inverted_callchain)
		callchain_param.order = ORDER_CALLER;
	if (symbol_conf.cumulate_callchain && !callchain_param.order_set)
		callchain_param.order = ORDER_CALLER;

<<<<<<< HEAD
	if (itrace_synth_opts.callchain &&
=======
	if ((itrace_synth_opts.callchain || itrace_synth_opts.add_callchain) &&
>>>>>>> upstream/android-13
	    (int)itrace_synth_opts.callchain_sz > report.max_stack)
		report.max_stack = itrace_synth_opts.callchain_sz;

	if (!input_name || !strlen(input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			input_name = "-";
		else
			input_name = "perf.data";
	}

<<<<<<< HEAD
	data.file.path = input_name;
	data.force     = symbol_conf.force;

repeat:
	session = perf_session__new(&data, false, &report.tool);
	if (session == NULL)
		return -1;
=======
	data.path  = input_name;
	data.force = symbol_conf.force;

repeat:
	session = perf_session__new(&data, &report.tool);
	if (IS_ERR(session)) {
		ret = PTR_ERR(session);
		goto exit;
	}

	ret = evswitch__init(&report.evswitch, session->evlist, stderr);
	if (ret)
		goto exit;

	if (zstd_init(&(session->zstd_data), 0) < 0)
		pr_warning("Decompression initialization failed. Reported data may be incomplete.\n");
>>>>>>> upstream/android-13

	if (report.queue_size) {
		ordered_events__set_alloc_size(&session->ordered_events,
					       report.queue_size);
	}

	session->itrace_synth_opts = &itrace_synth_opts;

	report.session = session;

	has_br_stack = perf_header__has_feat(&session->header,
					     HEADER_BRANCH_STACK);
<<<<<<< HEAD

	setup_forced_leader(&report, session->evlist);

	if (itrace_synth_opts.last_branch)
=======
	if (evlist__combined_sample_type(session->evlist) & PERF_SAMPLE_STACK_USER)
		has_br_stack = false;

	setup_forced_leader(&report, session->evlist);

	if (symbol_conf.group_sort_idx && !session->evlist->core.nr_groups) {
		parse_options_usage(NULL, options, "group-sort-idx", 0);
		ret = -EINVAL;
		goto error;
	}

	if (itrace_synth_opts.last_branch || itrace_synth_opts.add_last_branch)
>>>>>>> upstream/android-13
		has_br_stack = true;

	if (has_br_stack && branch_call_mode)
		symbol_conf.show_branchflag_count = true;

	memset(&report.brtype_stat, 0, sizeof(struct branch_type_stat));

	/*
	 * Branch mode is a tristate:
	 * -1 means default, so decide based on the file having branch data.
	 * 0/1 means the user chose a mode.
	 */
	if (((branch_mode == -1 && has_br_stack) || branch_mode == 1) &&
	    !branch_call_mode) {
		sort__mode = SORT_MODE__BRANCH;
		symbol_conf.cumulate_callchain = false;
	}
	if (branch_call_mode) {
		callchain_param.key = CCKEY_ADDRESS;
<<<<<<< HEAD
		callchain_param.branch_callstack = 1;
=======
		callchain_param.branch_callstack = true;
>>>>>>> upstream/android-13
		symbol_conf.use_callchain = true;
		callchain_register_param(&callchain_param);
		if (sort_order == NULL)
			sort_order = "srcline,symbol,dso";
	}

	if (report.mem_mode) {
		if (sort__mode == SORT_MODE__BRANCH) {
			pr_err("branch and mem mode incompatible\n");
			goto error;
		}
		sort__mode = SORT_MODE__MEMORY;
		symbol_conf.cumulate_callchain = false;
	}

	if (symbol_conf.report_hierarchy) {
		/* disable incompatible options */
		symbol_conf.cumulate_callchain = false;

		if (field_order) {
			pr_err("Error: --hierarchy and --fields options cannot be used together\n");
			parse_options_usage(report_usage, options, "F", 1);
			parse_options_usage(NULL, options, "hierarchy", 0);
			goto error;
		}

		perf_hpp_list.need_collapse = true;
	}

	if (report.use_stdio)
		use_browser = 0;
	else if (report.use_tui)
		use_browser = 1;
	else if (report.use_gtk)
		use_browser = 2;

	/* Force tty output for header output and per-thread stat. */
	if (report.header || report.header_only || report.show_threads)
		use_browser = 0;
	if (report.header || report.header_only)
		report.tool.show_feat_hdr = SHOW_FEAT_HEADER;
	if (report.show_full_info)
		report.tool.show_feat_hdr = SHOW_FEAT_HEADER_FULL_INFO;
	if (report.stats_mode || report.tasks_mode)
		use_browser = 0;
	if (report.stats_mode && report.tasks_mode) {
		pr_err("Error: --tasks and --mmaps can't be used together with --stats\n");
		goto error;
	}

<<<<<<< HEAD
=======
	if (report.total_cycles_mode) {
		if (sort__mode != SORT_MODE__BRANCH)
			report.total_cycles_mode = false;
		else
			sort_order = NULL;
	}

>>>>>>> upstream/android-13
	if (strcmp(input_name, "-") != 0)
		setup_browser(true);
	else
		use_browser = 0;

<<<<<<< HEAD
	if ((last_key != K_SWITCH_INPUT_DATA) &&
=======
	if (sort_order && strstr(sort_order, "ipc")) {
		parse_options_usage(report_usage, options, "s", 1);
		goto error;
	}

	if (sort_order && strstr(sort_order, "symbol")) {
		if (sort__mode == SORT_MODE__BRANCH) {
			snprintf(sort_tmp, sizeof(sort_tmp), "%s,%s",
				 sort_order, "ipc_lbr");
			report.symbol_ipc = true;
		} else {
			snprintf(sort_tmp, sizeof(sort_tmp), "%s,%s",
				 sort_order, "ipc_null");
		}

		sort_order = sort_tmp;
	}

	if ((last_key != K_SWITCH_INPUT_DATA && last_key != K_RELOAD) &&
>>>>>>> upstream/android-13
	    (setup_sorting(session->evlist) < 0)) {
		if (sort_order)
			parse_options_usage(report_usage, options, "s", 1);
		if (field_order)
			parse_options_usage(sort_order ? NULL : report_usage,
					    options, "F", 1);
		goto error;
	}

	if ((report.header || report.header_only) && !quiet) {
		perf_session__fprintf_info(session, stdout,
					   report.show_full_info);
		if (report.header_only) {
<<<<<<< HEAD
=======
			if (data.is_pipe) {
				/*
				 * we need to process first few records
				 * which contains PERF_RECORD_HEADER_FEATURE.
				 */
				perf_session__process_events(session);
			}
>>>>>>> upstream/android-13
			ret = 0;
			goto error;
		}
	} else if (use_browser == 0 && !quiet &&
		   !report.stats_mode && !report.tasks_mode) {
		fputs("# To display the perf.data header info, please use --header/--header-only options.\n#\n",
		      stdout);
	}

	/*
	 * Only in the TUI browser we are doing integrated annotation,
	 * so don't allocate extra space that won't be used in the stdio
	 * implementation.
	 */
<<<<<<< HEAD
	if (ui__has_annotation()) {
=======
	if (ui__has_annotation() || report.symbol_ipc ||
	    report.total_cycles_mode) {
>>>>>>> upstream/android-13
		ret = symbol__annotation_init();
		if (ret < 0)
			goto error;
		/*
 		 * For searching by name on the "Browse map details".
 		 * providing it only in verbose mode not to bloat too
 		 * much struct symbol.
 		 */
		if (verbose > 0) {
			/*
			 * XXX: Need to provide a less kludgy way to ask for
			 * more space per symbol, the u32 is for the index on
			 * the ui browser.
			 * See symbol__browser_index.
			 */
			symbol_conf.priv_size += sizeof(u32);
			symbol_conf.sort_by_name = true;
		}
<<<<<<< HEAD
		annotation_config__init();
=======
		annotation_config__init(&report.annotation_opts);
>>>>>>> upstream/android-13
	}

	if (symbol__init(&session->header.env) < 0)
		goto error;

<<<<<<< HEAD
	report.ptime_range = perf_time__range_alloc(report.time_str,
						    &report.range_size);
	if (!report.ptime_range) {
		ret = -ENOMEM;
		goto error;
	}

	if (perf_time__parse_str(report.ptime_range, report.time_str) != 0) {
		if (session->evlist->first_sample_time == 0 &&
		    session->evlist->last_sample_time == 0) {
			pr_err("HINT: no first/last sample time found in perf data.\n"
			       "Please use latest perf binary to execute 'perf record'\n"
			       "(if '--buildid-all' is enabled, please set '--timestamp-boundary').\n");
			ret = -EINVAL;
			goto error;
		}

		report.range_num = perf_time__percent_parse_str(
					report.ptime_range, report.range_size,
					report.time_str,
					session->evlist->first_sample_time,
					session->evlist->last_sample_time);

		if (report.range_num < 0) {
			pr_err("Invalid time string\n");
			ret = -EINVAL;
			goto error;
		}
	} else {
		report.range_num = 1;
=======
	if (report.time_str) {
		ret = perf_time__parse_for_ranges(report.time_str, session,
						  &report.ptime_range,
						  &report.range_size,
						  &report.range_num);
		if (ret < 0)
			goto error;

		itrace_synth_opts__set_time_range(&itrace_synth_opts,
						  report.ptime_range,
						  report.range_num);
>>>>>>> upstream/android-13
	}

	if (session->tevent.pevent &&
	    tep_set_function_resolver(session->tevent.pevent,
				      machine__resolve_kernel_addr,
				      &session->machines.host) < 0) {
		pr_err("%s: failed to set libtraceevent function resolver\n",
		       __func__);
		return -1;
	}

	sort__setup_elide(stdout);

	ret = __cmd_report(&report);
<<<<<<< HEAD
	if (ret == K_SWITCH_INPUT_DATA) {
=======
	if (ret == K_SWITCH_INPUT_DATA || ret == K_RELOAD) {
>>>>>>> upstream/android-13
		perf_session__delete(session);
		last_key = K_SWITCH_INPUT_DATA;
		goto repeat;
	} else
		ret = 0;

error:
<<<<<<< HEAD
	zfree(&report.ptime_range);

	perf_session__delete(session);
=======
	if (report.ptime_range) {
		itrace_synth_opts__clear_time_range(&itrace_synth_opts);
		zfree(&report.ptime_range);
	}

	if (report.block_reports) {
		block_info__free_report(report.block_reports,
					report.nr_block_reports);
		report.block_reports = NULL;
	}

	zstd_fini(&(session->zstd_data));
	perf_session__delete(session);
exit:
	free(sort_order_help);
	free(field_order_help);
>>>>>>> upstream/android-13
	return ret;
}
