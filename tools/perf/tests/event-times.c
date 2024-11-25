// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
<<<<<<< HEAD
=======
#include <linux/string.h>
>>>>>>> upstream/android-13
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <sys/wait.h>
<<<<<<< HEAD
#include "tests.h"
#include "evlist.h"
#include "evsel.h"
#include "util.h"
#include "debug.h"
#include "thread_map.h"
#include "target.h"

static int attach__enable_on_exec(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(evlist);
=======
#include <perf/cpumap.h>
#include "tests.h"
#include "evlist.h"
#include "evsel.h"
#include "debug.h"
#include "parse-events.h"
#include "thread_map.h"
#include "target.h"

static int attach__enable_on_exec(struct evlist *evlist)
{
	struct evsel *evsel = evlist__last(evlist);
>>>>>>> upstream/android-13
	struct target target = {
		.uid = UINT_MAX,
	};
	const char *argv[] = { "true", NULL, };
	char sbuf[STRERR_BUFSIZE];
	int err;

	pr_debug("attaching to spawned child, enable on exec\n");

<<<<<<< HEAD
	err = perf_evlist__create_maps(evlist, &target);
=======
	err = evlist__create_maps(evlist, &target);
>>>>>>> upstream/android-13
	if (err < 0) {
		pr_debug("Not enough memory to create thread/cpu maps\n");
		return err;
	}

<<<<<<< HEAD
	err = perf_evlist__prepare_workload(evlist, &target, argv, false, NULL);
=======
	err = evlist__prepare_workload(evlist, &target, argv, false, NULL);
>>>>>>> upstream/android-13
	if (err < 0) {
		pr_debug("Couldn't run the workload!\n");
		return err;
	}

<<<<<<< HEAD
	evsel->attr.enable_on_exec = 1;

	err = perf_evlist__open(evlist);
=======
	evsel->core.attr.enable_on_exec = 1;

	err = evlist__open(evlist);
>>>>>>> upstream/android-13
	if (err < 0) {
		pr_debug("perf_evlist__open: %s\n",
			 str_error_r(errno, sbuf, sizeof(sbuf)));
		return err;
	}

<<<<<<< HEAD
	return perf_evlist__start_workload(evlist) == 1 ? TEST_OK : TEST_FAIL;
}

static int detach__enable_on_exec(struct perf_evlist *evlist)
=======
	return evlist__start_workload(evlist) == 1 ? TEST_OK : TEST_FAIL;
}

static int detach__enable_on_exec(struct evlist *evlist)
>>>>>>> upstream/android-13
{
	waitpid(evlist->workload.pid, NULL, 0);
	return 0;
}

<<<<<<< HEAD
static int attach__current_disabled(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(evlist);
	struct thread_map *threads;
=======
static int attach__current_disabled(struct evlist *evlist)
{
	struct evsel *evsel = evlist__last(evlist);
	struct perf_thread_map *threads;
>>>>>>> upstream/android-13
	int err;

	pr_debug("attaching to current thread as disabled\n");

	threads = thread_map__new(-1, getpid(), UINT_MAX);
	if (threads == NULL) {
		pr_debug("thread_map__new\n");
		return -1;
	}

<<<<<<< HEAD
	evsel->attr.disabled = 1;

	err = perf_evsel__open_per_thread(evsel, threads);
=======
	evsel->core.attr.disabled = 1;

	err = evsel__open_per_thread(evsel, threads);
>>>>>>> upstream/android-13
	if (err) {
		pr_debug("Failed to open event cpu-clock:u\n");
		return err;
	}

<<<<<<< HEAD
	thread_map__put(threads);
	return perf_evsel__enable(evsel) == 0 ? TEST_OK : TEST_FAIL;
}

static int attach__current_enabled(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(evlist);
	struct thread_map *threads;
=======
	perf_thread_map__put(threads);
	return evsel__enable(evsel) == 0 ? TEST_OK : TEST_FAIL;
}

static int attach__current_enabled(struct evlist *evlist)
{
	struct evsel *evsel = evlist__last(evlist);
	struct perf_thread_map *threads;
>>>>>>> upstream/android-13
	int err;

	pr_debug("attaching to current thread as enabled\n");

	threads = thread_map__new(-1, getpid(), UINT_MAX);
	if (threads == NULL) {
		pr_debug("failed to call thread_map__new\n");
		return -1;
	}

<<<<<<< HEAD
	err = perf_evsel__open_per_thread(evsel, threads);

	thread_map__put(threads);
	return err == 0 ? TEST_OK : TEST_FAIL;
}

static int detach__disable(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(evlist);

	return perf_evsel__enable(evsel);
}

static int attach__cpu_disabled(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(evlist);
	struct cpu_map *cpus;
=======
	err = evsel__open_per_thread(evsel, threads);

	perf_thread_map__put(threads);
	return err == 0 ? TEST_OK : TEST_FAIL;
}

static int detach__disable(struct evlist *evlist)
{
	struct evsel *evsel = evlist__last(evlist);

	return evsel__enable(evsel);
}

static int attach__cpu_disabled(struct evlist *evlist)
{
	struct evsel *evsel = evlist__last(evlist);
	struct perf_cpu_map *cpus;
>>>>>>> upstream/android-13
	int err;

	pr_debug("attaching to CPU 0 as enabled\n");

<<<<<<< HEAD
	cpus = cpu_map__new("0");
	if (cpus == NULL) {
		pr_debug("failed to call cpu_map__new\n");
		return -1;
	}

	evsel->attr.disabled = 1;

	err = perf_evsel__open_per_cpu(evsel, cpus);
=======
	cpus = perf_cpu_map__new("0");
	if (cpus == NULL) {
		pr_debug("failed to call perf_cpu_map__new\n");
		return -1;
	}

	evsel->core.attr.disabled = 1;

	err = evsel__open_per_cpu(evsel, cpus, -1);
>>>>>>> upstream/android-13
	if (err) {
		if (err == -EACCES)
			return TEST_SKIP;

		pr_debug("Failed to open event cpu-clock:u\n");
		return err;
	}

<<<<<<< HEAD
	cpu_map__put(cpus);
	return perf_evsel__enable(evsel);
}

static int attach__cpu_enabled(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel = perf_evlist__last(evlist);
	struct cpu_map *cpus;
=======
	perf_cpu_map__put(cpus);
	return evsel__enable(evsel);
}

static int attach__cpu_enabled(struct evlist *evlist)
{
	struct evsel *evsel = evlist__last(evlist);
	struct perf_cpu_map *cpus;
>>>>>>> upstream/android-13
	int err;

	pr_debug("attaching to CPU 0 as enabled\n");

<<<<<<< HEAD
	cpus = cpu_map__new("0");
	if (cpus == NULL) {
		pr_debug("failed to call cpu_map__new\n");
		return -1;
	}

	err = perf_evsel__open_per_cpu(evsel, cpus);
	if (err == -EACCES)
		return TEST_SKIP;

	cpu_map__put(cpus);
	return err ? TEST_FAIL : TEST_OK;
}

static int test_times(int (attach)(struct perf_evlist *),
		      int (detach)(struct perf_evlist *))
{
	struct perf_counts_values count;
	struct perf_evlist *evlist = NULL;
	struct perf_evsel *evsel;
	int err = -1, i;

	evlist = perf_evlist__new();
=======
	cpus = perf_cpu_map__new("0");
	if (cpus == NULL) {
		pr_debug("failed to call perf_cpu_map__new\n");
		return -1;
	}

	err = evsel__open_per_cpu(evsel, cpus, -1);
	if (err == -EACCES)
		return TEST_SKIP;

	perf_cpu_map__put(cpus);
	return err ? TEST_FAIL : TEST_OK;
}

static int test_times(int (attach)(struct evlist *),
		      int (detach)(struct evlist *))
{
	struct perf_counts_values count;
	struct evlist *evlist = NULL;
	struct evsel *evsel;
	int err = -1, i;

	evlist = evlist__new();
>>>>>>> upstream/android-13
	if (!evlist) {
		pr_debug("failed to create event list\n");
		goto out_err;
	}

	err = parse_events(evlist, "cpu-clock:u", NULL);
	if (err) {
		pr_debug("failed to parse event cpu-clock:u\n");
		goto out_err;
	}

<<<<<<< HEAD
	evsel = perf_evlist__last(evlist);
	evsel->attr.read_format |=
=======
	evsel = evlist__last(evlist);
	evsel->core.attr.read_format |=
>>>>>>> upstream/android-13
		PERF_FORMAT_TOTAL_TIME_ENABLED |
		PERF_FORMAT_TOTAL_TIME_RUNNING;

	err = attach(evlist);
	if (err == TEST_SKIP) {
		pr_debug("  SKIP  : not enough rights\n");
		return err;
	}

	TEST_ASSERT_VAL("failed to attach", !err);

	for (i = 0; i < 100000000; i++) { }

	TEST_ASSERT_VAL("failed to detach", !detach(evlist));

<<<<<<< HEAD
	perf_evsel__read(evsel, 0, 0, &count);
=======
	perf_evsel__read(&evsel->core, 0, 0, &count);
>>>>>>> upstream/android-13

	err = !(count.ena == count.run);

	pr_debug("  %s: ena %" PRIu64", run %" PRIu64"\n",
		 !err ? "OK    " : "FAILED",
		 count.ena, count.run);

out_err:
<<<<<<< HEAD
	perf_evlist__delete(evlist);
=======
	evlist__delete(evlist);
>>>>>>> upstream/android-13
	return !err ? TEST_OK : TEST_FAIL;
}

/*
 * This test creates software event 'cpu-clock'
 * attaches it in several ways (explained below)
 * and checks that enabled and running times
 * match.
 */
int test__event_times(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	int err, ret = 0;

#define _T(attach, detach)			\
	err = test_times(attach, detach);	\
	if (err && (ret == TEST_OK || ret == TEST_SKIP))	\
		ret = err;

	/* attach on newly spawned process after exec */
	_T(attach__enable_on_exec,   detach__enable_on_exec)
	/* attach on current process as enabled */
	_T(attach__current_enabled,  detach__disable)
	/* attach on current process as disabled */
	_T(attach__current_disabled, detach__disable)
	/* attach on cpu as disabled */
	_T(attach__cpu_disabled,     detach__disable)
	/* attach on cpu as enabled */
	_T(attach__cpu_enabled,      detach__disable)

#undef _T
	return ret;
}
