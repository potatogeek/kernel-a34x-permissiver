// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <inttypes.h>
#include <api/fs/tracing_path.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/string.h>
>>>>>>> upstream/android-13
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "thread_map.h"
#include "evsel.h"
#include "debug.h"
#include "tests.h"
<<<<<<< HEAD
=======
#include "util/counts.h"
>>>>>>> upstream/android-13

int test__openat_syscall_event(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	int err = -1, fd;
<<<<<<< HEAD
	struct perf_evsel *evsel;
	unsigned int nr_openat_calls = 111, i;
	struct thread_map *threads = thread_map__new(-1, getpid(), UINT_MAX);
=======
	struct evsel *evsel;
	unsigned int nr_openat_calls = 111, i;
	struct perf_thread_map *threads = thread_map__new(-1, getpid(), UINT_MAX);
>>>>>>> upstream/android-13
	char sbuf[STRERR_BUFSIZE];
	char errbuf[BUFSIZ];

	if (threads == NULL) {
		pr_debug("thread_map__new\n");
		return -1;
	}

<<<<<<< HEAD
	evsel = perf_evsel__newtp("syscalls", "sys_enter_openat");
=======
	evsel = evsel__newtp("syscalls", "sys_enter_openat");
>>>>>>> upstream/android-13
	if (IS_ERR(evsel)) {
		tracing_path__strerror_open_tp(errno, errbuf, sizeof(errbuf), "syscalls", "sys_enter_openat");
		pr_debug("%s\n", errbuf);
		goto out_thread_map_delete;
	}

<<<<<<< HEAD
	if (perf_evsel__open_per_thread(evsel, threads) < 0) {
=======
	if (evsel__open_per_thread(evsel, threads) < 0) {
>>>>>>> upstream/android-13
		pr_debug("failed to open counter: %s, "
			 "tweak /proc/sys/kernel/perf_event_paranoid?\n",
			 str_error_r(errno, sbuf, sizeof(sbuf)));
		goto out_evsel_delete;
	}

	for (i = 0; i < nr_openat_calls; ++i) {
		fd = openat(0, "/etc/passwd", O_RDONLY);
		close(fd);
	}

<<<<<<< HEAD
	if (perf_evsel__read_on_cpu(evsel, 0, 0) < 0) {
		pr_debug("perf_evsel__read_on_cpu\n");
=======
	if (evsel__read_on_cpu(evsel, 0, 0) < 0) {
		pr_debug("evsel__read_on_cpu\n");
>>>>>>> upstream/android-13
		goto out_close_fd;
	}

	if (perf_counts(evsel->counts, 0, 0)->val != nr_openat_calls) {
<<<<<<< HEAD
		pr_debug("perf_evsel__read_on_cpu: expected to intercept %d calls, got %" PRIu64 "\n",
=======
		pr_debug("evsel__read_on_cpu: expected to intercept %d calls, got %" PRIu64 "\n",
>>>>>>> upstream/android-13
			 nr_openat_calls, perf_counts(evsel->counts, 0, 0)->val);
		goto out_close_fd;
	}

	err = 0;
out_close_fd:
<<<<<<< HEAD
	perf_evsel__close_fd(evsel);
out_evsel_delete:
	perf_evsel__delete(evsel);
out_thread_map_delete:
	thread_map__put(threads);
=======
	perf_evsel__close_fd(&evsel->core);
out_evsel_delete:
	evsel__delete(evsel);
out_thread_map_delete:
	perf_thread_map__put(threads);
>>>>>>> upstream/android-13
	return err;
}
