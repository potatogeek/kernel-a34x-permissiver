// SPDX-License-Identifier: GPL-2.0
/*
 * builtin-inject.c
 *
 * Builtin inject command: Examine the live mode (stdin) event stream
 * and repipe it to stdout while optionally injecting additional
 * events into it.
 */
#include "builtin.h"

<<<<<<< HEAD
#include "perf.h"
#include "util/color.h"
#include "util/evlist.h"
#include "util/evsel.h"
=======
#include "util/color.h"
#include "util/dso.h"
#include "util/vdso.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/map.h"
>>>>>>> upstream/android-13
#include "util/session.h"
#include "util/tool.h"
#include "util/debug.h"
#include "util/build-id.h"
#include "util/data.h"
#include "util/auxtrace.h"
#include "util/jit.h"
<<<<<<< HEAD
#include "util/thread.h"

#include <subcmd/parse-options.h>

#include <linux/list.h>
=======
#include "util/symbol.h"
#include "util/synthetic-events.h"
#include "util/thread.h"
#include "util/namespaces.h"

#include <linux/err.h>
#include <subcmd/parse-options.h>
#include <uapi/linux/mman.h> /* To get things like MAP_HUGETLB even on older libc headers */

#include <linux/list.h>
#include <linux/string.h>
>>>>>>> upstream/android-13
#include <errno.h>
#include <signal.h>

struct perf_inject {
	struct perf_tool	tool;
	struct perf_session	*session;
	bool			build_ids;
<<<<<<< HEAD
=======
	bool			build_id_all;
>>>>>>> upstream/android-13
	bool			sched_stat;
	bool			have_auxtrace;
	bool			strip;
	bool			jit_mode;
<<<<<<< HEAD
=======
	bool			in_place_update;
	bool			in_place_update_dry_run;
	bool			is_pipe;
>>>>>>> upstream/android-13
	const char		*input_name;
	struct perf_data	output;
	u64			bytes_written;
	u64			aux_id;
	struct list_head	samples;
	struct itrace_synth_opts itrace_synth_opts;
<<<<<<< HEAD
=======
	char			event_copy[PERF_SAMPLE_MAX_SIZE];
>>>>>>> upstream/android-13
};

struct event_entry {
	struct list_head node;
	u32		 tid;
<<<<<<< HEAD
	union perf_event event[0];
};

=======
	union perf_event event[];
};

static int dso__inject_build_id(struct dso *dso, struct perf_tool *tool,
				struct machine *machine, u8 cpumode, u32 flags);

>>>>>>> upstream/android-13
static int output_bytes(struct perf_inject *inject, void *buf, size_t sz)
{
	ssize_t size;

	size = perf_data__write(&inject->output, buf, sz);
	if (size < 0)
		return -errno;

	inject->bytes_written += size;
	return 0;
}

static int perf_event__repipe_synth(struct perf_tool *tool,
				    union perf_event *event)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject,
						  tool);

	return output_bytes(inject, event, event->header.size);
}

static int perf_event__repipe_oe_synth(struct perf_tool *tool,
				       union perf_event *event,
				       struct ordered_events *oe __maybe_unused)
{
	return perf_event__repipe_synth(tool, event);
}

#ifdef HAVE_JITDUMP
static int perf_event__drop_oe(struct perf_tool *tool __maybe_unused,
			       union perf_event *event __maybe_unused,
			       struct ordered_events *oe __maybe_unused)
{
	return 0;
}
#endif

<<<<<<< HEAD
static int perf_event__repipe_op2_synth(struct perf_tool *tool,
					union perf_event *event,
					struct perf_session *session
					__maybe_unused)
{
	return perf_event__repipe_synth(tool, event);
=======
static int perf_event__repipe_op2_synth(struct perf_session *session,
					union perf_event *event)
{
	return perf_event__repipe_synth(session->tool, event);
}

static int perf_event__repipe_op4_synth(struct perf_session *session,
					union perf_event *event,
					u64 data __maybe_unused)
{
	return perf_event__repipe_synth(session->tool, event);
>>>>>>> upstream/android-13
}

static int perf_event__repipe_attr(struct perf_tool *tool,
				   union perf_event *event,
<<<<<<< HEAD
				   struct perf_evlist **pevlist)
=======
				   struct evlist **pevlist)
>>>>>>> upstream/android-13
{
	struct perf_inject *inject = container_of(tool, struct perf_inject,
						  tool);
	int ret;

	ret = perf_event__process_attr(tool, event, pevlist);
	if (ret)
		return ret;

<<<<<<< HEAD
	if (!inject->output.is_pipe)
=======
	if (!inject->is_pipe)
>>>>>>> upstream/android-13
		return 0;

	return perf_event__repipe_synth(tool, event);
}

<<<<<<< HEAD
=======
static int perf_event__repipe_event_update(struct perf_tool *tool,
					   union perf_event *event,
					   struct evlist **pevlist __maybe_unused)
{
	return perf_event__repipe_synth(tool, event);
}

>>>>>>> upstream/android-13
#ifdef HAVE_AUXTRACE_SUPPORT

static int copy_bytes(struct perf_inject *inject, int fd, off_t size)
{
	char buf[4096];
	ssize_t ssz;
	int ret;

	while (size > 0) {
		ssz = read(fd, buf, min(size, (off_t)sizeof(buf)));
		if (ssz < 0)
			return -errno;
		ret = output_bytes(inject, buf, ssz);
		if (ret)
			return ret;
		size -= ssz;
	}

	return 0;
}

<<<<<<< HEAD
static s64 perf_event__repipe_auxtrace(struct perf_tool *tool,
				       union perf_event *event,
				       struct perf_session *session)
{
=======
static s64 perf_event__repipe_auxtrace(struct perf_session *session,
				       union perf_event *event)
{
	struct perf_tool *tool = session->tool;
>>>>>>> upstream/android-13
	struct perf_inject *inject = container_of(tool, struct perf_inject,
						  tool);
	int ret;

	inject->have_auxtrace = true;

	if (!inject->output.is_pipe) {
		off_t offset;

		offset = lseek(inject->output.file.fd, 0, SEEK_CUR);
		if (offset == -1)
			return -errno;
		ret = auxtrace_index__auxtrace_event(&session->auxtrace_index,
						     event, offset);
		if (ret < 0)
			return ret;
	}

	if (perf_data__is_pipe(session->data) || !session->one_mmap) {
		ret = output_bytes(inject, event, event->header.size);
		if (ret < 0)
			return ret;
		ret = copy_bytes(inject, perf_data__fd(session->data),
				 event->auxtrace.size);
	} else {
		ret = output_bytes(inject, event,
				   event->header.size + event->auxtrace.size);
	}
	if (ret < 0)
		return ret;

	return event->auxtrace.size;
}

#else

static s64
<<<<<<< HEAD
perf_event__repipe_auxtrace(struct perf_tool *tool __maybe_unused,
			    union perf_event *event __maybe_unused,
			    struct perf_session *session __maybe_unused)
=======
perf_event__repipe_auxtrace(struct perf_session *session __maybe_unused,
			    union perf_event *event __maybe_unused)
>>>>>>> upstream/android-13
{
	pr_err("AUX area tracing not supported\n");
	return -EINVAL;
}

#endif

static int perf_event__repipe(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	return perf_event__repipe_synth(tool, event);
}

static int perf_event__drop(struct perf_tool *tool __maybe_unused,
			    union perf_event *event __maybe_unused,
			    struct perf_sample *sample __maybe_unused,
			    struct machine *machine __maybe_unused)
{
	return 0;
}

static int perf_event__drop_aux(struct perf_tool *tool,
				union perf_event *event __maybe_unused,
				struct perf_sample *sample,
				struct machine *machine __maybe_unused)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);

	if (!inject->aux_id)
		inject->aux_id = sample->id;

	return 0;
}

<<<<<<< HEAD
typedef int (*inject_handler)(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample,
			      struct perf_evsel *evsel,
=======
static union perf_event *
perf_inject__cut_auxtrace_sample(struct perf_inject *inject,
				 union perf_event *event,
				 struct perf_sample *sample)
{
	size_t sz1 = sample->aux_sample.data - (void *)event;
	size_t sz2 = event->header.size - sample->aux_sample.size - sz1;
	union perf_event *ev = (union perf_event *)inject->event_copy;

	if (sz1 > event->header.size || sz2 > event->header.size ||
	    sz1 + sz2 > event->header.size ||
	    sz1 < sizeof(struct perf_event_header) + sizeof(u64))
		return event;

	memcpy(ev, event, sz1);
	memcpy((void *)ev + sz1, (void *)event + event->header.size - sz2, sz2);
	ev->header.size = sz1 + sz2;
	((u64 *)((void *)ev + sz1))[-1] = 0;

	return ev;
}

typedef int (*inject_handler)(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample,
			      struct evsel *evsel,
>>>>>>> upstream/android-13
			      struct machine *machine);

static int perf_event__repipe_sample(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample,
<<<<<<< HEAD
				     struct perf_evsel *evsel,
				     struct machine *machine)
{
	if (evsel->handler) {
=======
				     struct evsel *evsel,
				     struct machine *machine)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject,
						  tool);

	if (evsel && evsel->handler) {
>>>>>>> upstream/android-13
		inject_handler f = evsel->handler;
		return f(tool, event, sample, evsel, machine);
	}

	build_id__mark_dso_hit(tool, event, sample, evsel, machine);

<<<<<<< HEAD
=======
	if (inject->itrace_synth_opts.set && sample->aux_sample.size)
		event = perf_inject__cut_auxtrace_sample(inject, event, sample);

>>>>>>> upstream/android-13
	return perf_event__repipe_synth(tool, event);
}

static int perf_event__repipe_mmap(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_mmap(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);

	return err;
}

#ifdef HAVE_JITDUMP
static int perf_event__jit_repipe_mmap(struct perf_tool *tool,
				       union perf_event *event,
				       struct perf_sample *sample,
				       struct machine *machine)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	u64 n = 0;
	int ret;

	/*
	 * if jit marker, then inject jit mmaps and generate ELF images
	 */
	ret = jit_process(inject->session, &inject->output, machine,
<<<<<<< HEAD
			  event->mmap.filename, sample->pid, &n);
=======
			  event->mmap.filename, event->mmap.pid, event->mmap.tid, &n);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;
	if (ret) {
		inject->bytes_written += n;
		return 0;
	}
	return perf_event__repipe_mmap(tool, event, sample, machine);
}
#endif

<<<<<<< HEAD
=======
static struct dso *findnew_dso(int pid, int tid, const char *filename,
			       struct dso_id *id, struct machine *machine)
{
	struct thread *thread;
	struct nsinfo *nsi = NULL;
	struct nsinfo *nnsi;
	struct dso *dso;
	bool vdso;

	thread = machine__findnew_thread(machine, pid, tid);
	if (thread == NULL) {
		pr_err("cannot find or create a task %d/%d.\n", tid, pid);
		return NULL;
	}

	vdso = is_vdso_map(filename);
	nsi = nsinfo__get(thread->nsinfo);

	if (vdso) {
		/* The vdso maps are always on the host and not the
		 * container.  Ensure that we don't use setns to look
		 * them up.
		 */
		nnsi = nsinfo__copy(nsi);
		if (nnsi) {
			nsinfo__put(nsi);
			nnsi->need_setns = false;
			nsi = nnsi;
		}
		dso = machine__findnew_vdso(machine, thread);
	} else {
		dso = machine__findnew_dso_id(machine, filename, id);
	}

	if (dso) {
		nsinfo__put(dso->nsinfo);
		dso->nsinfo = nsi;
	} else
		nsinfo__put(nsi);

	thread__put(thread);
	return dso;
}

static int perf_event__repipe_buildid_mmap(struct perf_tool *tool,
					   union perf_event *event,
					   struct perf_sample *sample,
					   struct machine *machine)
{
	struct dso *dso;

	dso = findnew_dso(event->mmap.pid, event->mmap.tid,
			  event->mmap.filename, NULL, machine);

	if (dso && !dso->hit) {
		dso->hit = 1;
		dso__inject_build_id(dso, tool, machine, sample->cpumode, 0);
	}
	dso__put(dso);

	return perf_event__repipe(tool, event, sample, machine);
}

>>>>>>> upstream/android-13
static int perf_event__repipe_mmap2(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_mmap2(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);

<<<<<<< HEAD
=======
	if (event->header.misc & PERF_RECORD_MISC_MMAP_BUILD_ID) {
		struct dso *dso;

		dso = findnew_dso(event->mmap2.pid, event->mmap2.tid,
				  event->mmap2.filename, NULL, machine);
		if (dso) {
			/* mark it not to inject build-id */
			dso->hit = 1;
		}
		dso__put(dso);
	}

>>>>>>> upstream/android-13
	return err;
}

#ifdef HAVE_JITDUMP
static int perf_event__jit_repipe_mmap2(struct perf_tool *tool,
					union perf_event *event,
					struct perf_sample *sample,
					struct machine *machine)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	u64 n = 0;
	int ret;

	/*
	 * if jit marker, then inject jit mmaps and generate ELF images
	 */
	ret = jit_process(inject->session, &inject->output, machine,
<<<<<<< HEAD
			  event->mmap2.filename, sample->pid, &n);
=======
			  event->mmap2.filename, event->mmap2.pid, event->mmap2.tid, &n);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;
	if (ret) {
		inject->bytes_written += n;
		return 0;
	}
	return perf_event__repipe_mmap2(tool, event, sample, machine);
}
#endif

<<<<<<< HEAD
=======
static int perf_event__repipe_buildid_mmap2(struct perf_tool *tool,
					    union perf_event *event,
					    struct perf_sample *sample,
					    struct machine *machine)
{
	struct dso_id dso_id = {
		.maj = event->mmap2.maj,
		.min = event->mmap2.min,
		.ino = event->mmap2.ino,
		.ino_generation = event->mmap2.ino_generation,
	};
	struct dso *dso;

	if (event->header.misc & PERF_RECORD_MISC_MMAP_BUILD_ID) {
		/* cannot use dso_id since it'd have invalid info */
		dso = findnew_dso(event->mmap2.pid, event->mmap2.tid,
				  event->mmap2.filename, NULL, machine);
		if (dso) {
			/* mark it not to inject build-id */
			dso->hit = 1;
		}
		dso__put(dso);
		return 0;
	}

	dso = findnew_dso(event->mmap2.pid, event->mmap2.tid,
			  event->mmap2.filename, &dso_id, machine);

	if (dso && !dso->hit) {
		dso->hit = 1;
		dso__inject_build_id(dso, tool, machine, sample->cpumode,
				     event->mmap2.flags);
	}
	dso__put(dso);

	perf_event__repipe(tool, event, sample, machine);

	return 0;
}

>>>>>>> upstream/android-13
static int perf_event__repipe_fork(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_fork(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);

	return err;
}

static int perf_event__repipe_comm(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_comm(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);

	return err;
}

static int perf_event__repipe_namespaces(struct perf_tool *tool,
					 union perf_event *event,
					 struct perf_sample *sample,
					 struct machine *machine)
{
	int err = perf_event__process_namespaces(tool, event, sample, machine);

	perf_event__repipe(tool, event, sample, machine);

	return err;
}

static int perf_event__repipe_exit(struct perf_tool *tool,
				   union perf_event *event,
				   struct perf_sample *sample,
				   struct machine *machine)
{
	int err;

	err = perf_event__process_exit(tool, event, sample, machine);
	perf_event__repipe(tool, event, sample, machine);

	return err;
}

<<<<<<< HEAD
static int perf_event__repipe_tracing_data(struct perf_tool *tool,
					   union perf_event *event,
					   struct perf_session *session)
{
	int err;

	perf_event__repipe_synth(tool, event);
	err = perf_event__process_tracing_data(tool, event, session);

	return err;
}

static int perf_event__repipe_id_index(struct perf_tool *tool,
				       union perf_event *event,
				       struct perf_session *session)
{
	int err;

	perf_event__repipe_synth(tool, event);
	err = perf_event__process_id_index(tool, event, session);
=======
static int perf_event__repipe_tracing_data(struct perf_session *session,
					   union perf_event *event)
{
	int err;

	perf_event__repipe_synth(session->tool, event);
	err = perf_event__process_tracing_data(session, event);
>>>>>>> upstream/android-13

	return err;
}

static int dso__read_build_id(struct dso *dso)
{
<<<<<<< HEAD
	if (dso->has_build_id)
		return 0;

	if (filename__read_build_id(dso->long_name, dso->build_id,
				    sizeof(dso->build_id)) > 0) {
		dso->has_build_id = true;
		return 0;
	}

	return -1;
}

static int dso__inject_build_id(struct dso *dso, struct perf_tool *tool,
				struct machine *machine)
{
	u16 misc = PERF_RECORD_MISC_USER;
	int err;

=======
	struct nscookie nsc;

	if (dso->has_build_id)
		return 0;

	nsinfo__mountns_enter(dso->nsinfo, &nsc);
	if (filename__read_build_id(dso->long_name, &dso->bid) > 0)
		dso->has_build_id = true;
	nsinfo__mountns_exit(&nsc);

	return dso->has_build_id ? 0 : -1;
}

static int dso__inject_build_id(struct dso *dso, struct perf_tool *tool,
				struct machine *machine, u8 cpumode, u32 flags)
{
	int err;

	if (is_anon_memory(dso->long_name) || flags & MAP_HUGETLB)
		return 0;
	if (is_no_dso_memory(dso->long_name))
		return 0;

>>>>>>> upstream/android-13
	if (dso__read_build_id(dso) < 0) {
		pr_debug("no build_id found for %s\n", dso->long_name);
		return -1;
	}

<<<<<<< HEAD
	if (dso->kernel)
		misc = PERF_RECORD_MISC_KERNEL;

	err = perf_event__synthesize_build_id(tool, dso, misc, perf_event__repipe,
					      machine);
=======
	err = perf_event__synthesize_build_id(tool, dso, cpumode,
					      perf_event__repipe, machine);
>>>>>>> upstream/android-13
	if (err) {
		pr_err("Can't synthesize build_id event for %s\n", dso->long_name);
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
static int perf_event__inject_buildid(struct perf_tool *tool,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel __maybe_unused,
				      struct machine *machine)
=======
int perf_event__inject_buildid(struct perf_tool *tool, union perf_event *event,
			       struct perf_sample *sample,
			       struct evsel *evsel __maybe_unused,
			       struct machine *machine)
>>>>>>> upstream/android-13
{
	struct addr_location al;
	struct thread *thread;

	thread = machine__findnew_thread(machine, sample->pid, sample->tid);
	if (thread == NULL) {
		pr_err("problem processing %d event, skipping it.\n",
		       event->header.type);
		goto repipe;
	}

	if (thread__find_map(thread, sample->cpumode, sample->ip, &al)) {
		if (!al.map->dso->hit) {
			al.map->dso->hit = 1;
<<<<<<< HEAD
			if (map__load(al.map) >= 0) {
				dso__inject_build_id(al.map->dso, tool, machine);
				/*
				 * If this fails, too bad, let the other side
				 * account this as unresolved.
				 */
			} else {
#ifdef HAVE_LIBELF_SUPPORT
				pr_warning("no symbols found in %s, maybe "
					   "install a debug package?\n",
					   al.map->dso->long_name);
#endif
			}
=======
			dso__inject_build_id(al.map->dso, tool, machine,
					     sample->cpumode, al.map->flags);
>>>>>>> upstream/android-13
		}
	}

	thread__put(thread);
repipe:
	perf_event__repipe(tool, event, sample, machine);
	return 0;
}

static int perf_inject__sched_process_exit(struct perf_tool *tool,
					   union perf_event *event __maybe_unused,
					   struct perf_sample *sample,
<<<<<<< HEAD
					   struct perf_evsel *evsel __maybe_unused,
=======
					   struct evsel *evsel __maybe_unused,
>>>>>>> upstream/android-13
					   struct machine *machine __maybe_unused)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	struct event_entry *ent;

	list_for_each_entry(ent, &inject->samples, node) {
		if (sample->tid == ent->tid) {
			list_del_init(&ent->node);
			free(ent);
			break;
		}
	}

	return 0;
}

static int perf_inject__sched_switch(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample,
<<<<<<< HEAD
				     struct perf_evsel *evsel,
=======
				     struct evsel *evsel,
>>>>>>> upstream/android-13
				     struct machine *machine)
{
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
	struct event_entry *ent;

	perf_inject__sched_process_exit(tool, event, sample, evsel, machine);

	ent = malloc(event->header.size + sizeof(struct event_entry));
	if (ent == NULL) {
		color_fprintf(stderr, PERF_COLOR_RED,
			     "Not enough memory to process sched switch event!");
		return -1;
	}

	ent->tid = sample->tid;
	memcpy(&ent->event, event, event->header.size);
	list_add(&ent->node, &inject->samples);
	return 0;
}

static int perf_inject__sched_stat(struct perf_tool *tool,
				   union perf_event *event __maybe_unused,
				   struct perf_sample *sample,
<<<<<<< HEAD
				   struct perf_evsel *evsel,
=======
				   struct evsel *evsel,
>>>>>>> upstream/android-13
				   struct machine *machine)
{
	struct event_entry *ent;
	union perf_event *event_sw;
	struct perf_sample sample_sw;
	struct perf_inject *inject = container_of(tool, struct perf_inject, tool);
<<<<<<< HEAD
	u32 pid = perf_evsel__intval(evsel, sample, "pid");
=======
	u32 pid = evsel__intval(evsel, sample, "pid");
>>>>>>> upstream/android-13

	list_for_each_entry(ent, &inject->samples, node) {
		if (pid == ent->tid)
			goto found;
	}

	return 0;
found:
	event_sw = &ent->event[0];
<<<<<<< HEAD
	perf_evsel__parse_sample(evsel, event_sw, &sample_sw);

	sample_sw.period = sample->period;
	sample_sw.time	 = sample->time;
	perf_event__synthesize_sample(event_sw, evsel->attr.sample_type,
				      evsel->attr.read_format, &sample_sw);
=======
	evsel__parse_sample(evsel, event_sw, &sample_sw);

	sample_sw.period = sample->period;
	sample_sw.time	 = sample->time;
	perf_event__synthesize_sample(event_sw, evsel->core.attr.sample_type,
				      evsel->core.attr.read_format, &sample_sw);
>>>>>>> upstream/android-13
	build_id__mark_dso_hit(tool, event_sw, &sample_sw, evsel, machine);
	return perf_event__repipe(tool, event_sw, &sample_sw, machine);
}

static void sig_handler(int sig __maybe_unused)
{
	session_done = 1;
}

<<<<<<< HEAD
static int perf_evsel__check_stype(struct perf_evsel *evsel,
				   u64 sample_type, const char *sample_msg)
{
	struct perf_event_attr *attr = &evsel->attr;
	const char *name = perf_evsel__name(evsel);
=======
static int evsel__check_stype(struct evsel *evsel, u64 sample_type, const char *sample_msg)
{
	struct perf_event_attr *attr = &evsel->core.attr;
	const char *name = evsel__name(evsel);
>>>>>>> upstream/android-13

	if (!(attr->sample_type & sample_type)) {
		pr_err("Samples for %s event do not have %s attribute set.",
			name, sample_msg);
		return -EINVAL;
	}

	return 0;
}

static int drop_sample(struct perf_tool *tool __maybe_unused,
		       union perf_event *event __maybe_unused,
		       struct perf_sample *sample __maybe_unused,
<<<<<<< HEAD
		       struct perf_evsel *evsel __maybe_unused,
=======
		       struct evsel *evsel __maybe_unused,
>>>>>>> upstream/android-13
		       struct machine *machine __maybe_unused)
{
	return 0;
}

static void strip_init(struct perf_inject *inject)
{
<<<<<<< HEAD
	struct perf_evlist *evlist = inject->session->evlist;
	struct perf_evsel *evsel;
=======
	struct evlist *evlist = inject->session->evlist;
	struct evsel *evsel;
>>>>>>> upstream/android-13

	inject->tool.context_switch = perf_event__drop;

	evlist__for_each_entry(evlist, evsel)
		evsel->handler = drop_sample;
}

<<<<<<< HEAD
static bool has_tracking(struct perf_evsel *evsel)
{
	return evsel->attr.mmap || evsel->attr.mmap2 || evsel->attr.comm ||
	       evsel->attr.task;
}

#define COMPAT_MASK (PERF_SAMPLE_ID | PERF_SAMPLE_TID | PERF_SAMPLE_TIME | \
		     PERF_SAMPLE_ID | PERF_SAMPLE_CPU | PERF_SAMPLE_IDENTIFIER)

/*
 * In order that the perf.data file is parsable, tracking events like MMAP need
 * their selected event to exist, except if there is only 1 selected event left
 * and it has a compatible sample type.
 */
static bool ok_to_remove(struct perf_evlist *evlist,
			 struct perf_evsel *evsel_to_remove)
{
	struct perf_evsel *evsel;
	int cnt = 0;
	bool ok = false;

	if (!has_tracking(evsel_to_remove))
		return true;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->handler != drop_sample) {
			cnt += 1;
			if ((evsel->attr.sample_type & COMPAT_MASK) ==
			    (evsel_to_remove->attr.sample_type & COMPAT_MASK))
				ok = true;
		}
	}

	return ok && cnt == 1;
}

static void strip_fini(struct perf_inject *inject)
{
	struct perf_evlist *evlist = inject->session->evlist;
	struct perf_evsel *evsel, *tmp;

	/* Remove non-synthesized evsels if possible */
	evlist__for_each_entry_safe(evlist, tmp, evsel) {
		if (evsel->handler == drop_sample &&
		    ok_to_remove(evlist, evsel)) {
			pr_debug("Deleting %s\n", perf_evsel__name(evsel));
			perf_evlist__remove(evlist, evsel);
			perf_evsel__delete(evsel);
		}
	}
=======
static int parse_vm_time_correlation(const struct option *opt, const char *str, int unset)
{
	struct perf_inject *inject = opt->value;
	const char *args;
	char *dry_run;

	if (unset)
		return 0;

	inject->itrace_synth_opts.set = true;
	inject->itrace_synth_opts.vm_time_correlation = true;
	inject->in_place_update = true;

	if (!str)
		return 0;

	dry_run = skip_spaces(str);
	if (!strncmp(dry_run, "dry-run", strlen("dry-run"))) {
		inject->itrace_synth_opts.vm_tm_corr_dry_run = true;
		inject->in_place_update_dry_run = true;
		args = dry_run + strlen("dry-run");
	} else {
		args = str;
	}

	inject->itrace_synth_opts.vm_tm_corr_args = strdup(args);

	return inject->itrace_synth_opts.vm_tm_corr_args ? 0 : -ENOMEM;
}

static int output_fd(struct perf_inject *inject)
{
	return inject->in_place_update ? -1 : perf_data__fd(&inject->output);
>>>>>>> upstream/android-13
}

static int __cmd_inject(struct perf_inject *inject)
{
	int ret = -EINVAL;
	struct perf_session *session = inject->session;
<<<<<<< HEAD
	struct perf_data *data_out = &inject->output;
	int fd = perf_data__fd(data_out);
=======
	int fd = output_fd(inject);
>>>>>>> upstream/android-13
	u64 output_data_offset;

	signal(SIGINT, sig_handler);

	if (inject->build_ids || inject->sched_stat ||
<<<<<<< HEAD
	    inject->itrace_synth_opts.set) {
=======
	    inject->itrace_synth_opts.set || inject->build_id_all) {
>>>>>>> upstream/android-13
		inject->tool.mmap	  = perf_event__repipe_mmap;
		inject->tool.mmap2	  = perf_event__repipe_mmap2;
		inject->tool.fork	  = perf_event__repipe_fork;
		inject->tool.tracing_data = perf_event__repipe_tracing_data;
	}

	output_data_offset = session->header.data_offset;

<<<<<<< HEAD
	if (inject->build_ids) {
		inject->tool.sample = perf_event__inject_buildid;
	} else if (inject->sched_stat) {
		struct perf_evsel *evsel;

		evlist__for_each_entry(session->evlist, evsel) {
			const char *name = perf_evsel__name(evsel);

			if (!strcmp(name, "sched:sched_switch")) {
				if (perf_evsel__check_stype(evsel, PERF_SAMPLE_TID, "TID"))
=======
	if (inject->build_id_all) {
		inject->tool.mmap	  = perf_event__repipe_buildid_mmap;
		inject->tool.mmap2	  = perf_event__repipe_buildid_mmap2;
	} else if (inject->build_ids) {
		inject->tool.sample = perf_event__inject_buildid;
	} else if (inject->sched_stat) {
		struct evsel *evsel;

		evlist__for_each_entry(session->evlist, evsel) {
			const char *name = evsel__name(evsel);

			if (!strcmp(name, "sched:sched_switch")) {
				if (evsel__check_stype(evsel, PERF_SAMPLE_TID, "TID"))
>>>>>>> upstream/android-13
					return -EINVAL;

				evsel->handler = perf_inject__sched_switch;
			} else if (!strcmp(name, "sched:sched_process_exit"))
				evsel->handler = perf_inject__sched_process_exit;
			else if (!strncmp(name, "sched:sched_stat_", 17))
				evsel->handler = perf_inject__sched_stat;
		}
<<<<<<< HEAD
=======
	} else if (inject->itrace_synth_opts.vm_time_correlation) {
		session->itrace_synth_opts = &inject->itrace_synth_opts;
		memset(&inject->tool, 0, sizeof(inject->tool));
		inject->tool.id_index	    = perf_event__process_id_index;
		inject->tool.auxtrace_info  = perf_event__process_auxtrace_info;
		inject->tool.auxtrace	    = perf_event__process_auxtrace;
		inject->tool.auxtrace_error = perf_event__process_auxtrace_error;
		inject->tool.ordered_events = true;
		inject->tool.ordering_requires_timestamps = true;
>>>>>>> upstream/android-13
	} else if (inject->itrace_synth_opts.set) {
		session->itrace_synth_opts = &inject->itrace_synth_opts;
		inject->itrace_synth_opts.inject = true;
		inject->tool.comm	    = perf_event__repipe_comm;
		inject->tool.namespaces	    = perf_event__repipe_namespaces;
		inject->tool.exit	    = perf_event__repipe_exit;
<<<<<<< HEAD
		inject->tool.id_index	    = perf_event__repipe_id_index;
=======
		inject->tool.id_index	    = perf_event__process_id_index;
>>>>>>> upstream/android-13
		inject->tool.auxtrace_info  = perf_event__process_auxtrace_info;
		inject->tool.auxtrace	    = perf_event__process_auxtrace;
		inject->tool.aux	    = perf_event__drop_aux;
		inject->tool.itrace_start   = perf_event__drop_aux,
		inject->tool.ordered_events = true;
		inject->tool.ordering_requires_timestamps = true;
		/* Allow space in the header for new attributes */
<<<<<<< HEAD
		output_data_offset = 4096;
=======
		output_data_offset = roundup(8192 + session->header.data_offset, 4096);
>>>>>>> upstream/android-13
		if (inject->strip)
			strip_init(inject);
	}

	if (!inject->itrace_synth_opts.set)
		auxtrace_index__free(&session->auxtrace_index);

<<<<<<< HEAD
	if (!data_out->is_pipe)
=======
	if (!inject->is_pipe && !inject->in_place_update)
>>>>>>> upstream/android-13
		lseek(fd, output_data_offset, SEEK_SET);

	ret = perf_session__process_events(session);
	if (ret)
		return ret;

<<<<<<< HEAD
	if (!data_out->is_pipe) {
=======
	if (!inject->is_pipe && !inject->in_place_update) {
>>>>>>> upstream/android-13
		if (inject->build_ids)
			perf_header__set_feat(&session->header,
					      HEADER_BUILD_ID);
		/*
		 * Keep all buildids when there is unprocessed AUX data because
		 * it is not known which ones the AUX trace hits.
		 */
		if (perf_header__has_feat(&session->header, HEADER_BUILD_ID) &&
		    inject->have_auxtrace && !inject->itrace_synth_opts.set)
			dsos__hit_all(session);
		/*
		 * The AUX areas have been removed and replaced with
<<<<<<< HEAD
		 * synthesized hardware events, so clear the feature flag and
		 * remove the evsel.
		 */
		if (inject->itrace_synth_opts.set) {
			struct perf_evsel *evsel;

			perf_header__clear_feat(&session->header,
						HEADER_AUXTRACE);
			if (inject->itrace_synth_opts.last_branch)
				perf_header__set_feat(&session->header,
						      HEADER_BRANCH_STACK);
			evsel = perf_evlist__id2evsel_strict(session->evlist,
							     inject->aux_id);
			if (evsel) {
				pr_debug("Deleting %s\n",
					 perf_evsel__name(evsel));
				perf_evlist__remove(session->evlist, evsel);
				perf_evsel__delete(evsel);
			}
			if (inject->strip)
				strip_fini(inject);
=======
		 * synthesized hardware events, so clear the feature flag.
		 */
		if (inject->itrace_synth_opts.set) {
			perf_header__clear_feat(&session->header,
						HEADER_AUXTRACE);
			if (inject->itrace_synth_opts.last_branch ||
			    inject->itrace_synth_opts.add_last_branch)
				perf_header__set_feat(&session->header,
						      HEADER_BRANCH_STACK);
>>>>>>> upstream/android-13
		}
		session->header.data_offset = output_data_offset;
		session->header.data_size = inject->bytes_written;
		perf_session__write_header(session, session->evlist, fd, true);
	}

	return ret;
}

int cmd_inject(int argc, const char **argv)
{
	struct perf_inject inject = {
		.tool = {
			.sample		= perf_event__repipe_sample,
<<<<<<< HEAD
			.mmap		= perf_event__repipe,
			.mmap2		= perf_event__repipe,
			.comm		= perf_event__repipe,
=======
			.read		= perf_event__repipe_sample,
			.mmap		= perf_event__repipe,
			.mmap2		= perf_event__repipe,
			.comm		= perf_event__repipe,
			.namespaces	= perf_event__repipe,
			.cgroup		= perf_event__repipe,
>>>>>>> upstream/android-13
			.fork		= perf_event__repipe,
			.exit		= perf_event__repipe,
			.lost		= perf_event__repipe,
			.lost_samples	= perf_event__repipe,
			.aux		= perf_event__repipe,
			.itrace_start	= perf_event__repipe,
			.context_switch	= perf_event__repipe,
<<<<<<< HEAD
			.read		= perf_event__repipe_sample,
			.throttle	= perf_event__repipe,
			.unthrottle	= perf_event__repipe,
			.attr		= perf_event__repipe_attr,
			.tracing_data	= perf_event__repipe_op2_synth,
			.auxtrace_info	= perf_event__repipe_op2_synth,
			.auxtrace	= perf_event__repipe_auxtrace,
			.auxtrace_error	= perf_event__repipe_op2_synth,
			.time_conv	= perf_event__repipe_op2_synth,
			.finished_round	= perf_event__repipe_oe_synth,
			.build_id	= perf_event__repipe_op2_synth,
			.id_index	= perf_event__repipe_op2_synth,
			.feature	= perf_event__repipe_op2_synth,
=======
			.throttle	= perf_event__repipe,
			.unthrottle	= perf_event__repipe,
			.ksymbol	= perf_event__repipe,
			.bpf		= perf_event__repipe,
			.text_poke	= perf_event__repipe,
			.attr		= perf_event__repipe_attr,
			.event_update	= perf_event__repipe_event_update,
			.tracing_data	= perf_event__repipe_op2_synth,
			.finished_round	= perf_event__repipe_oe_synth,
			.build_id	= perf_event__repipe_op2_synth,
			.id_index	= perf_event__repipe_op2_synth,
			.auxtrace_info	= perf_event__repipe_op2_synth,
			.auxtrace_error	= perf_event__repipe_op2_synth,
			.time_conv	= perf_event__repipe_op2_synth,
			.thread_map	= perf_event__repipe_op2_synth,
			.cpu_map	= perf_event__repipe_op2_synth,
			.stat_config	= perf_event__repipe_op2_synth,
			.stat		= perf_event__repipe_op2_synth,
			.stat_round	= perf_event__repipe_op2_synth,
			.feature	= perf_event__repipe_op2_synth,
			.compressed	= perf_event__repipe_op4_synth,
			.auxtrace	= perf_event__repipe_auxtrace,
>>>>>>> upstream/android-13
		},
		.input_name  = "-",
		.samples = LIST_HEAD_INIT(inject.samples),
		.output = {
<<<<<<< HEAD
			.file      = {
				.path = "-",
			},
			.mode      = PERF_DATA_MODE_WRITE,
=======
			.path = "-",
			.mode = PERF_DATA_MODE_WRITE,
			.use_stdio = true,
>>>>>>> upstream/android-13
		},
	};
	struct perf_data data = {
		.mode = PERF_DATA_MODE_READ,
<<<<<<< HEAD
	};
	int ret;
=======
		.use_stdio = true,
	};
	int ret;
	bool repipe = true;
>>>>>>> upstream/android-13

	struct option options[] = {
		OPT_BOOLEAN('b', "build-ids", &inject.build_ids,
			    "Inject build-ids into the output stream"),
<<<<<<< HEAD
		OPT_STRING('i', "input", &inject.input_name, "file",
			   "input file name"),
		OPT_STRING('o', "output", &inject.output.file.path, "file",
=======
		OPT_BOOLEAN(0, "buildid-all", &inject.build_id_all,
			    "Inject build-ids of all DSOs into the output stream"),
		OPT_STRING('i', "input", &inject.input_name, "file",
			   "input file name"),
		OPT_STRING('o', "output", &inject.output.path, "file",
>>>>>>> upstream/android-13
			   "output file name"),
		OPT_BOOLEAN('s', "sched-stat", &inject.sched_stat,
			    "Merge sched-stat and sched-switch for getting events "
			    "where and how long tasks slept"),
#ifdef HAVE_JITDUMP
		OPT_BOOLEAN('j', "jit", &inject.jit_mode, "merge jitdump files into perf.data file"),
#endif
		OPT_INCR('v', "verbose", &verbose,
			 "be more verbose (show build ids, etc)"),
		OPT_STRING(0, "kallsyms", &symbol_conf.kallsyms_name, "file",
			   "kallsyms pathname"),
		OPT_BOOLEAN('f', "force", &data.force, "don't complain, do it"),
		OPT_CALLBACK_OPTARG(0, "itrace", &inject.itrace_synth_opts,
<<<<<<< HEAD
				    NULL, "opts", "Instruction Tracing options",
				    itrace_parse_synth_opts),
		OPT_BOOLEAN(0, "strip", &inject.strip,
			    "strip non-synthesized events (use with --itrace)"),
=======
				    NULL, "opts", "Instruction Tracing options\n"
				    ITRACE_HELP,
				    itrace_parse_synth_opts),
		OPT_BOOLEAN(0, "strip", &inject.strip,
			    "strip non-synthesized events (use with --itrace)"),
		OPT_CALLBACK_OPTARG(0, "vm-time-correlation", &inject, NULL, "opts",
				    "correlate time between VM guests and the host",
				    parse_vm_time_correlation),
>>>>>>> upstream/android-13
		OPT_END()
	};
	const char * const inject_usage[] = {
		"perf inject [<options>]",
		NULL
	};
#ifndef HAVE_JITDUMP
	set_option_nobuild(options, 'j', "jit", "NO_LIBELF=1", true);
#endif
	argc = parse_options(argc, argv, options, inject_usage, 0);

	/*
	 * Any (unrecognized) arguments left?
	 */
	if (argc)
		usage_with_options(inject_usage, options);

	if (inject.strip && !inject.itrace_synth_opts.set) {
		pr_err("--strip option requires --itrace option\n");
		return -1;
	}

<<<<<<< HEAD
	if (perf_data__open(&inject.output)) {
=======
	if (inject.in_place_update) {
		if (!strcmp(inject.input_name, "-")) {
			pr_err("Input file name required for in-place updating\n");
			return -1;
		}
		if (strcmp(inject.output.path, "-")) {
			pr_err("Output file name must not be specified for in-place updating\n");
			return -1;
		}
		if (!data.force && !inject.in_place_update_dry_run) {
			pr_err("The input file would be updated in place, "
				"the --force option is required.\n");
			return -1;
		}
		if (!inject.in_place_update_dry_run)
			data.in_place_update = true;
	} else if (perf_data__open(&inject.output)) {
>>>>>>> upstream/android-13
		perror("failed to create output file");
		return -1;
	}

<<<<<<< HEAD
	inject.tool.ordered_events = inject.sched_stat;

	data.file.path = inject.input_name;
	inject.session = perf_session__new(&data, true, &inject.tool);
	if (inject.session == NULL)
		return -1;

	if (inject.build_ids) {
=======
	data.path = inject.input_name;
	if (!strcmp(inject.input_name, "-") || inject.output.is_pipe) {
		inject.is_pipe = true;
		/*
		 * Do not repipe header when input is a regular file
		 * since either it can rewrite the header at the end
		 * or write a new pipe header.
		 */
		if (strcmp(inject.input_name, "-"))
			repipe = false;
	}

	inject.session = __perf_session__new(&data, repipe,
					     output_fd(&inject),
					     &inject.tool);
	if (IS_ERR(inject.session)) {
		ret = PTR_ERR(inject.session);
		goto out_close_output;
	}

	if (zstd_init(&(inject.session->zstd_data), 0) < 0)
		pr_warning("Decompression initialization failed.\n");

	if (!data.is_pipe && inject.output.is_pipe) {
		ret = perf_header__write_pipe(perf_data__fd(&inject.output));
		if (ret < 0) {
			pr_err("Couldn't write a new pipe header.\n");
			goto out_delete;
		}

		ret = perf_event__synthesize_for_pipe(&inject.tool,
						      inject.session,
						      &inject.output,
						      perf_event__repipe);
		if (ret < 0)
			goto out_delete;
	}

	if (inject.build_ids && !inject.build_id_all) {
>>>>>>> upstream/android-13
		/*
		 * to make sure the mmap records are ordered correctly
		 * and so that the correct especially due to jitted code
		 * mmaps. We cannot generate the buildid hit list and
		 * inject the jit mmaps at the same time for now.
		 */
		inject.tool.ordered_events = true;
		inject.tool.ordering_requires_timestamps = true;
	}
<<<<<<< HEAD
=======

	if (inject.sched_stat) {
		inject.tool.ordered_events = true;
	}

>>>>>>> upstream/android-13
#ifdef HAVE_JITDUMP
	if (inject.jit_mode) {
		inject.tool.mmap2	   = perf_event__jit_repipe_mmap2;
		inject.tool.mmap	   = perf_event__jit_repipe_mmap;
		inject.tool.ordered_events = true;
		inject.tool.ordering_requires_timestamps = true;
		/*
		 * JIT MMAP injection injects all MMAP events in one go, so it
		 * does not obey finished_round semantics.
		 */
		inject.tool.finished_round = perf_event__drop_oe;
	}
#endif
	ret = symbol__init(&inject.session->header.env);
	if (ret < 0)
		goto out_delete;

	ret = __cmd_inject(&inject);

out_delete:
<<<<<<< HEAD
	perf_session__delete(inject.session);
=======
	zstd_fini(&(inject.session->zstd_data));
	perf_session__delete(inject.session);
out_close_output:
	if (!inject.in_place_update)
		perf_data__close(&inject.output);
	free(inject.itrace_synth_opts.vm_tm_corr_args);
>>>>>>> upstream/android-13
	return ret;
}
