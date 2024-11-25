/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_TOOL_H
#define __PERF_TOOL_H

#include <stdbool.h>

#include <linux/types.h>

struct perf_session;
union perf_event;
<<<<<<< HEAD
struct perf_evlist;
struct perf_evsel;
=======
struct evlist;
struct evsel;
>>>>>>> upstream/android-13
struct perf_sample;
struct perf_tool;
struct machine;
struct ordered_events;

typedef int (*event_sample)(struct perf_tool *tool, union perf_event *event,
			    struct perf_sample *sample,
<<<<<<< HEAD
			    struct perf_evsel *evsel, struct machine *machine);
=======
			    struct evsel *evsel, struct machine *machine);
>>>>>>> upstream/android-13

typedef int (*event_op)(struct perf_tool *tool, union perf_event *event,
			struct perf_sample *sample, struct machine *machine);

typedef int (*event_attr_op)(struct perf_tool *tool,
			     union perf_event *event,
<<<<<<< HEAD
			     struct perf_evlist **pevlist);

typedef int (*event_op2)(struct perf_tool *tool, union perf_event *event,
			 struct perf_session *session);
=======
			     struct evlist **pevlist);

typedef int (*event_op2)(struct perf_session *session, union perf_event *event);
typedef s64 (*event_op3)(struct perf_session *session, union perf_event *event);
typedef int (*event_op4)(struct perf_session *session, union perf_event *event, u64 data);
>>>>>>> upstream/android-13

typedef int (*event_oe)(struct perf_tool *tool, union perf_event *event,
			struct ordered_events *oe);

<<<<<<< HEAD
typedef s64 (*event_op3)(struct perf_tool *tool, union perf_event *event,
			 struct perf_session *session);

=======
>>>>>>> upstream/android-13
enum show_feature_header {
	SHOW_FEAT_NO_HEADER = 0,
	SHOW_FEAT_HEADER,
	SHOW_FEAT_HEADER_FULL_INFO,
};

struct perf_tool {
	event_sample	sample,
			read;
	event_op	mmap,
			mmap2,
			comm,
			namespaces,
<<<<<<< HEAD
=======
			cgroup,
>>>>>>> upstream/android-13
			fork,
			exit,
			lost,
			lost_samples,
			aux,
			itrace_start,
			context_switch,
			throttle,
<<<<<<< HEAD
			unthrottle;
=======
			unthrottle,
			ksymbol,
			bpf,
			text_poke;

>>>>>>> upstream/android-13
	event_attr_op	attr;
	event_attr_op	event_update;
	event_op2	tracing_data;
	event_oe	finished_round;
	event_op2	build_id,
			id_index,
			auxtrace_info,
			auxtrace_error,
			time_conv,
			thread_map,
			cpu_map,
			stat_config,
			stat,
			stat_round,
			feature;
<<<<<<< HEAD
=======
	event_op4	compressed;
>>>>>>> upstream/android-13
	event_op3	auxtrace;
	bool		ordered_events;
	bool		ordering_requires_timestamps;
	bool		namespace_events;
<<<<<<< HEAD
=======
	bool		cgroup_events;
>>>>>>> upstream/android-13
	bool		no_warn;
	enum show_feature_header show_feat_hdr;
};

#endif /* __PERF_TOOL_H */
