// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
<<<<<<< HEAD
#include <unistd.h>
#include <sys/prctl.h>

#include "parse-events.h"
#include "evlist.h"
#include "evsel.h"
#include "thread_map.h"
#include "cpumap.h"
#include "tests.h"
=======
#include <limits.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <perf/cpumap.h>
#include <perf/evlist.h>
#include <perf/mmap.h>

#include "debug.h"
#include "parse-events.h"
#include "evlist.h"
#include "evsel.h"
#include "record.h"
#include "thread_map.h"
#include "tests.h"
#include "util/mmap.h"
>>>>>>> upstream/android-13

#define CHECK__(x) {				\
	while ((x) < 0) {			\
		pr_debug(#x " failed!\n");	\
		goto out_err;			\
	}					\
}

#define CHECK_NOT_NULL__(x) {			\
	while ((x) == NULL) {			\
		pr_debug(#x " failed!\n");	\
		goto out_err;			\
	}					\
}

<<<<<<< HEAD
static int find_comm(struct perf_evlist *evlist, const char *comm)
{
	union perf_event *event;
	struct perf_mmap *md;
	int i, found;

	found = 0;
	for (i = 0; i < evlist->nr_mmaps; i++) {
		md = &evlist->mmap[i];
		if (perf_mmap__read_init(md) < 0)
			continue;
		while ((event = perf_mmap__read_event(md)) != NULL) {
=======
static int find_comm(struct evlist *evlist, const char *comm)
{
	union perf_event *event;
	struct mmap *md;
	int i, found;

	found = 0;
	for (i = 0; i < evlist->core.nr_mmaps; i++) {
		md = &evlist->mmap[i];
		if (perf_mmap__read_init(&md->core) < 0)
			continue;
		while ((event = perf_mmap__read_event(&md->core)) != NULL) {
>>>>>>> upstream/android-13
			if (event->header.type == PERF_RECORD_COMM &&
			    (pid_t)event->comm.pid == getpid() &&
			    (pid_t)event->comm.tid == getpid() &&
			    strcmp(event->comm.comm, comm) == 0)
				found += 1;
<<<<<<< HEAD
			perf_mmap__consume(md);
		}
		perf_mmap__read_done(md);
=======
			perf_mmap__consume(&md->core);
		}
		perf_mmap__read_done(&md->core);
>>>>>>> upstream/android-13
	}
	return found;
}

/**
 * test__keep_tracking - test using a dummy software event to keep tracking.
 *
 * This function implements a test that checks that tracking events continue
 * when an event is disabled but a dummy software event is not disabled.  If the
 * test passes %0 is returned, otherwise %-1 is returned.
 */
int test__keep_tracking(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	struct record_opts opts = {
		.mmap_pages	     = UINT_MAX,
		.user_freq	     = UINT_MAX,
		.user_interval	     = ULLONG_MAX,
		.target		     = {
			.uses_mmap   = true,
		},
	};
<<<<<<< HEAD
	struct thread_map *threads = NULL;
	struct cpu_map *cpus = NULL;
	struct perf_evlist *evlist = NULL;
	struct perf_evsel *evsel = NULL;
=======
	struct perf_thread_map *threads = NULL;
	struct perf_cpu_map *cpus = NULL;
	struct evlist *evlist = NULL;
	struct evsel *evsel = NULL;
>>>>>>> upstream/android-13
	int found, err = -1;
	const char *comm;

	threads = thread_map__new(-1, getpid(), UINT_MAX);
	CHECK_NOT_NULL__(threads);

<<<<<<< HEAD
	cpus = cpu_map__new(NULL);
	CHECK_NOT_NULL__(cpus);

	evlist = perf_evlist__new();
	CHECK_NOT_NULL__(evlist);

	perf_evlist__set_maps(evlist, cpus, threads);
=======
	cpus = perf_cpu_map__new(NULL);
	CHECK_NOT_NULL__(cpus);

	evlist = evlist__new();
	CHECK_NOT_NULL__(evlist);

	perf_evlist__set_maps(&evlist->core, cpus, threads);
>>>>>>> upstream/android-13

	CHECK__(parse_events(evlist, "dummy:u", NULL));
	CHECK__(parse_events(evlist, "cycles:u", NULL));

<<<<<<< HEAD
	perf_evlist__config(evlist, &opts, NULL);

	evsel = perf_evlist__first(evlist);

	evsel->attr.comm = 1;
	evsel->attr.disabled = 1;
	evsel->attr.enable_on_exec = 0;

	if (perf_evlist__open(evlist) < 0) {
=======
	evlist__config(evlist, &opts, NULL);

	evsel = evlist__first(evlist);

	evsel->core.attr.comm = 1;
	evsel->core.attr.disabled = 1;
	evsel->core.attr.enable_on_exec = 0;

	if (evlist__open(evlist) < 0) {
>>>>>>> upstream/android-13
		pr_debug("Unable to open dummy and cycles event\n");
		err = TEST_SKIP;
		goto out_err;
	}

<<<<<<< HEAD
	CHECK__(perf_evlist__mmap(evlist, UINT_MAX));
=======
	CHECK__(evlist__mmap(evlist, UINT_MAX));
>>>>>>> upstream/android-13

	/*
	 * First, test that a 'comm' event can be found when the event is
	 * enabled.
	 */

<<<<<<< HEAD
	perf_evlist__enable(evlist);
=======
	evlist__enable(evlist);
>>>>>>> upstream/android-13

	comm = "Test COMM 1";
	CHECK__(prctl(PR_SET_NAME, (unsigned long)comm, 0, 0, 0));

<<<<<<< HEAD
	perf_evlist__disable(evlist);
=======
	evlist__disable(evlist);
>>>>>>> upstream/android-13

	found = find_comm(evlist, comm);
	if (found != 1) {
		pr_debug("First time, failed to find tracking event.\n");
		goto out_err;
	}

	/*
	 * Secondly, test that a 'comm' event can be found when the event is
	 * disabled with the dummy event still enabled.
	 */

<<<<<<< HEAD
	perf_evlist__enable(evlist);

	evsel = perf_evlist__last(evlist);

	CHECK__(perf_evsel__disable(evsel));
=======
	evlist__enable(evlist);

	evsel = evlist__last(evlist);

	CHECK__(evsel__disable(evsel));
>>>>>>> upstream/android-13

	comm = "Test COMM 2";
	CHECK__(prctl(PR_SET_NAME, (unsigned long)comm, 0, 0, 0));

<<<<<<< HEAD
	perf_evlist__disable(evlist);

	found = find_comm(evlist, comm);
	if (found != 1) {
		pr_debug("Seconf time, failed to find tracking event.\n");
=======
	evlist__disable(evlist);

	found = find_comm(evlist, comm);
	if (found != 1) {
		pr_debug("Second time, failed to find tracking event.\n");
>>>>>>> upstream/android-13
		goto out_err;
	}

	err = 0;

out_err:
	if (evlist) {
<<<<<<< HEAD
		perf_evlist__disable(evlist);
		perf_evlist__delete(evlist);
	} else {
		cpu_map__put(cpus);
		thread_map__put(threads);
	}
=======
		evlist__disable(evlist);
		evlist__delete(evlist);
	}
	perf_cpu_map__put(cpus);
	perf_thread_map__put(threads);
>>>>>>> upstream/android-13

	return err;
}
