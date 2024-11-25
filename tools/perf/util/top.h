/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_TOP_H
#define __PERF_TOP_H 1

#include "tool.h"
<<<<<<< HEAD
#include "annotate.h"
=======
#include "evswitch.h"
#include "annotate.h"
#include "ordered-events.h"
#include "record.h"
>>>>>>> upstream/android-13
#include <linux/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/ioctl.h>

<<<<<<< HEAD
struct perf_evlist;
struct perf_evsel;
=======
struct evlist;
struct evsel;
>>>>>>> upstream/android-13
struct perf_session;

struct perf_top {
	struct perf_tool   tool;
<<<<<<< HEAD
	struct perf_evlist *evlist;
	struct record_opts record_opts;
	struct annotation_options annotation_opts;
=======
	struct evlist *evlist, *sb_evlist;
	struct record_opts record_opts;
	struct annotation_options annotation_opts;
	struct evswitch	   evswitch;
>>>>>>> upstream/android-13
	/*
	 * Symbols will be added here in perf_event__process_sample and will
	 * get out after decayed.
	 */
<<<<<<< HEAD
	u64		   samples;
=======
	u64		   samples, lost, lost_total, drop, drop_total;
>>>>>>> upstream/android-13
	u64		   kernel_samples, us_samples;
	u64		   exact_samples;
	u64		   guest_us_samples, guest_kernel_samples;
	int		   print_entries, count_filter, delay_secs;
	int		   max_stack;
	bool		   hide_kernel_symbols, hide_user_symbols, zero;
	bool		   use_tui, use_stdio;
	bool		   vmlinux_warned;
	bool		   dump_symtab;
<<<<<<< HEAD
	struct hist_entry  *sym_filter_entry;
	struct perf_evsel  *sym_evsel;
=======
	bool		   stitch_lbr;
	struct hist_entry  *sym_filter_entry;
	struct evsel 	   *sym_evsel;
>>>>>>> upstream/android-13
	struct perf_session *session;
	struct winsize	   winsize;
	int		   realtime_prio;
	const char	   *sym_filter;
	float		   min_percent;
	unsigned int	   nr_threads_synthesize;
<<<<<<< HEAD
=======

	struct {
		struct ordered_events	*in;
		struct ordered_events	 data[2];
		bool			 rotate;
		pthread_mutex_t		 mutex;
		pthread_cond_t		 cond;
	} qe;
>>>>>>> upstream/android-13
};

#define CONSOLE_CLEAR "[H[2J"

size_t perf_top__header_snprintf(struct perf_top *top, char *bf, size_t size);
void perf_top__reset_sample_counters(struct perf_top *top);
#endif /* __PERF_TOP_H */
