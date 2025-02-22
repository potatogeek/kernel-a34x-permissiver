// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2015 Davidlohr Bueso.
 *
 * Block a bunch of threads and let parallel waker threads wakeup an
 * equal amount of them. The program output reflects the avg latency
 * for each individual thread to service its share of work. Ultimately
 * it can be used to measure futex_wake() changes.
 */
#include "bench.h"
#include <linux/compiler.h>
#include "../util/debug.h"

#ifndef HAVE_PTHREAD_BARRIER
int bench_futex_wake_parallel(int argc __maybe_unused, const char **argv __maybe_unused)
{
	pr_err("%s: pthread_barrier_t unavailable, disabling this test...\n", __func__);
	return 0;
}
#else /* HAVE_PTHREAD_BARRIER */
/* For the CLR_() macros */
#include <string.h>
#include <pthread.h>

#include <signal.h>
#include "../util/stat.h"
#include <subcmd/parse-options.h>
#include <linux/kernel.h>
#include <linux/time64.h>
#include <errno.h>
#include "futex.h"
<<<<<<< HEAD
#include "cpumap.h"
=======
#include <perf/cpumap.h>
>>>>>>> upstream/android-13

#include <err.h>
#include <stdlib.h>
#include <sys/time.h>
<<<<<<< HEAD
=======
#include <sys/mman.h>
>>>>>>> upstream/android-13

struct thread_data {
	pthread_t worker;
	unsigned int nwoken;
	struct timeval runtime;
};

static unsigned int nwakes = 1;

/* all threads will block on the same futex -- hash bucket chaos ;) */
static u_int32_t futex = 0;

static pthread_t *blocked_worker;
<<<<<<< HEAD
static bool done = false, silent = false, fshared = false;
static unsigned int nblocked_threads = 0, nwaking_threads = 0;
=======
static bool done = false;
>>>>>>> upstream/android-13
static pthread_mutex_t thread_lock;
static pthread_cond_t thread_parent, thread_worker;
static pthread_barrier_t barrier;
static struct stats waketime_stats, wakeup_stats;
static unsigned int threads_starting;
static int futex_flag = 0;

<<<<<<< HEAD
static const struct option options[] = {
	OPT_UINTEGER('t', "threads", &nblocked_threads, "Specify amount of threads"),
	OPT_UINTEGER('w', "nwakers", &nwaking_threads, "Specify amount of waking threads"),
	OPT_BOOLEAN( 's', "silent",  &silent,   "Silent mode: do not display data/details"),
	OPT_BOOLEAN( 'S', "shared",  &fshared,  "Use shared futexes instead of private ones"),
=======
static struct bench_futex_parameters params;

static const struct option options[] = {
	OPT_UINTEGER('t', "threads", &params.nthreads, "Specify amount of threads"),
	OPT_UINTEGER('w', "nwakers", &params.nwakes, "Specify amount of waking threads"),
	OPT_BOOLEAN( 's', "silent",  &params.silent, "Silent mode: do not display data/details"),
	OPT_BOOLEAN( 'S', "shared",  &params.fshared, "Use shared futexes instead of private ones"),
	OPT_BOOLEAN( 'm', "mlockall", &params.mlockall, "Lock all current and future memory"),

>>>>>>> upstream/android-13
	OPT_END()
};

static const char * const bench_futex_wake_parallel_usage[] = {
	"perf bench futex wake-parallel <options>",
	NULL
};

static void *waking_workerfn(void *arg)
{
	struct thread_data *waker = (struct thread_data *) arg;
	struct timeval start, end;

	pthread_barrier_wait(&barrier);

	gettimeofday(&start, NULL);

	waker->nwoken = futex_wake(&futex, nwakes, futex_flag);
	if (waker->nwoken != nwakes)
		warnx("couldn't wakeup all tasks (%d/%d)",
		      waker->nwoken, nwakes);

	gettimeofday(&end, NULL);
	timersub(&end, &start, &waker->runtime);

	pthread_exit(NULL);
	return NULL;
}

static void wakeup_threads(struct thread_data *td, pthread_attr_t thread_attr)
{
	unsigned int i;

	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

<<<<<<< HEAD
	pthread_barrier_init(&barrier, NULL, nwaking_threads + 1);

	/* create and block all threads */
	for (i = 0; i < nwaking_threads; i++) {
=======
	pthread_barrier_init(&barrier, NULL, params.nwakes + 1);

	/* create and block all threads */
	for (i = 0; i < params.nwakes; i++) {
>>>>>>> upstream/android-13
		/*
		 * Thread creation order will impact per-thread latency
		 * as it will affect the order to acquire the hb spinlock.
		 * For now let the scheduler decide.
		 */
		if (pthread_create(&td[i].worker, &thread_attr,
				   waking_workerfn, (void *)&td[i]))
			err(EXIT_FAILURE, "pthread_create");
	}

	pthread_barrier_wait(&barrier);

<<<<<<< HEAD
	for (i = 0; i < nwaking_threads; i++)
=======
	for (i = 0; i < params.nwakes; i++)
>>>>>>> upstream/android-13
		if (pthread_join(td[i].worker, NULL))
			err(EXIT_FAILURE, "pthread_join");

	pthread_barrier_destroy(&barrier);
}

static void *blocked_workerfn(void *arg __maybe_unused)
{
	pthread_mutex_lock(&thread_lock);
	threads_starting--;
	if (!threads_starting)
		pthread_cond_signal(&thread_parent);
	pthread_cond_wait(&thread_worker, &thread_lock);
	pthread_mutex_unlock(&thread_lock);

	while (1) { /* handle spurious wakeups */
		if (futex_wait(&futex, 0, NULL, futex_flag) != EINTR)
			break;
	}

	pthread_exit(NULL);
	return NULL;
}

static void block_threads(pthread_t *w, pthread_attr_t thread_attr,
<<<<<<< HEAD
			  struct cpu_map *cpu)
=======
			  struct perf_cpu_map *cpu)
>>>>>>> upstream/android-13
{
	cpu_set_t cpuset;
	unsigned int i;

<<<<<<< HEAD
	threads_starting = nblocked_threads;

	/* create and block all threads */
	for (i = 0; i < nblocked_threads; i++) {
=======
	threads_starting = params.nthreads;

	/* create and block all threads */
	for (i = 0; i < params.nthreads; i++) {
>>>>>>> upstream/android-13
		CPU_ZERO(&cpuset);
		CPU_SET(cpu->map[i % cpu->nr], &cpuset);

		if (pthread_attr_setaffinity_np(&thread_attr, sizeof(cpu_set_t), &cpuset))
			err(EXIT_FAILURE, "pthread_attr_setaffinity_np");

		if (pthread_create(&w[i], &thread_attr, blocked_workerfn, NULL))
			err(EXIT_FAILURE, "pthread_create");
	}
}

static void print_run(struct thread_data *waking_worker, unsigned int run_num)
{
	unsigned int i, wakeup_avg;
	double waketime_avg, waketime_stddev;
	struct stats __waketime_stats, __wakeup_stats;

	init_stats(&__wakeup_stats);
	init_stats(&__waketime_stats);

<<<<<<< HEAD
	for (i = 0; i < nwaking_threads; i++) {
=======
	for (i = 0; i < params.nwakes; i++) {
>>>>>>> upstream/android-13
		update_stats(&__waketime_stats, waking_worker[i].runtime.tv_usec);
		update_stats(&__wakeup_stats, waking_worker[i].nwoken);
	}

	waketime_avg = avg_stats(&__waketime_stats);
	waketime_stddev = stddev_stats(&__waketime_stats);
	wakeup_avg = avg_stats(&__wakeup_stats);

	printf("[Run %d]: Avg per-thread latency (waking %d/%d threads) "
	       "in %.4f ms (+-%.2f%%)\n", run_num + 1, wakeup_avg,
<<<<<<< HEAD
	       nblocked_threads, waketime_avg / USEC_PER_MSEC,
=======
	       params.nthreads, waketime_avg / USEC_PER_MSEC,
>>>>>>> upstream/android-13
	       rel_stddev_stats(waketime_stddev, waketime_avg));
}

static void print_summary(void)
{
	unsigned int wakeup_avg;
	double waketime_avg, waketime_stddev;

	waketime_avg = avg_stats(&waketime_stats);
	waketime_stddev = stddev_stats(&waketime_stats);
	wakeup_avg = avg_stats(&wakeup_stats);

	printf("Avg per-thread latency (waking %d/%d threads) in %.4f ms (+-%.2f%%)\n",
	       wakeup_avg,
<<<<<<< HEAD
	       nblocked_threads,
=======
	       params.nthreads,
>>>>>>> upstream/android-13
	       waketime_avg / USEC_PER_MSEC,
	       rel_stddev_stats(waketime_stddev, waketime_avg));
}


static void do_run_stats(struct thread_data *waking_worker)
{
	unsigned int i;

<<<<<<< HEAD
	for (i = 0; i < nwaking_threads; i++) {
=======
	for (i = 0; i < params.nwakes; i++) {
>>>>>>> upstream/android-13
		update_stats(&waketime_stats, waking_worker[i].runtime.tv_usec);
		update_stats(&wakeup_stats, waking_worker[i].nwoken);
	}

}

static void toggle_done(int sig __maybe_unused,
			siginfo_t *info __maybe_unused,
			void *uc __maybe_unused)
{
	done = true;
}

int bench_futex_wake_parallel(int argc, const char **argv)
{
	int ret = 0;
	unsigned int i, j;
	struct sigaction act;
	pthread_attr_t thread_attr;
	struct thread_data *waking_worker;
<<<<<<< HEAD
	struct cpu_map *cpu;
=======
	struct perf_cpu_map *cpu;
>>>>>>> upstream/android-13

	argc = parse_options(argc, argv, options,
			     bench_futex_wake_parallel_usage, 0);
	if (argc) {
		usage_with_options(bench_futex_wake_parallel_usage, options);
		exit(EXIT_FAILURE);
	}

<<<<<<< HEAD
=======
	memset(&act, 0, sizeof(act));
>>>>>>> upstream/android-13
	sigfillset(&act.sa_mask);
	act.sa_sigaction = toggle_done;
	sigaction(SIGINT, &act, NULL);

<<<<<<< HEAD
	cpu = cpu_map__new(NULL);
	if (!cpu)
		err(EXIT_FAILURE, "calloc");

	if (!nblocked_threads)
		nblocked_threads = cpu->nr;

	/* some sanity checks */
	if (nwaking_threads > nblocked_threads || !nwaking_threads)
		nwaking_threads = nblocked_threads;

	if (nblocked_threads % nwaking_threads)
=======
	if (params.mlockall) {
		if (mlockall(MCL_CURRENT | MCL_FUTURE))
			err(EXIT_FAILURE, "mlockall");
	}

	cpu = perf_cpu_map__new(NULL);
	if (!cpu)
		err(EXIT_FAILURE, "calloc");

	if (!params.nthreads)
		params.nthreads = cpu->nr;

	/* some sanity checks */
	if (params.nwakes > params.nthreads ||
	    !params.nwakes)
		params.nwakes = params.nthreads;

	if (params.nthreads % params.nwakes)
>>>>>>> upstream/android-13
		errx(EXIT_FAILURE, "Must be perfectly divisible");
	/*
	 * Each thread will wakeup nwakes tasks in
	 * a single futex_wait call.
	 */
<<<<<<< HEAD
	nwakes = nblocked_threads/nwaking_threads;

	blocked_worker = calloc(nblocked_threads, sizeof(*blocked_worker));
	if (!blocked_worker)
		err(EXIT_FAILURE, "calloc");

	if (!fshared)
=======
	nwakes = params.nthreads/params.nwakes;

	blocked_worker = calloc(params.nthreads, sizeof(*blocked_worker));
	if (!blocked_worker)
		err(EXIT_FAILURE, "calloc");

	if (!params.fshared)
>>>>>>> upstream/android-13
		futex_flag = FUTEX_PRIVATE_FLAG;

	printf("Run summary [PID %d]: blocking on %d threads (at [%s] "
	       "futex %p), %d threads waking up %d at a time.\n\n",
<<<<<<< HEAD
	       getpid(), nblocked_threads, fshared ? "shared":"private",
	       &futex, nwaking_threads, nwakes);
=======
	       getpid(), params.nthreads, params.fshared ? "shared":"private",
	       &futex, params.nwakes, nwakes);
>>>>>>> upstream/android-13

	init_stats(&wakeup_stats);
	init_stats(&waketime_stats);

	pthread_attr_init(&thread_attr);
	pthread_mutex_init(&thread_lock, NULL);
	pthread_cond_init(&thread_parent, NULL);
	pthread_cond_init(&thread_worker, NULL);

	for (j = 0; j < bench_repeat && !done; j++) {
<<<<<<< HEAD
		waking_worker = calloc(nwaking_threads, sizeof(*waking_worker));
=======
		waking_worker = calloc(params.nwakes, sizeof(*waking_worker));
>>>>>>> upstream/android-13
		if (!waking_worker)
			err(EXIT_FAILURE, "calloc");

		/* create, launch & block all threads */
		block_threads(blocked_worker, thread_attr, cpu);

		/* make sure all threads are already blocked */
		pthread_mutex_lock(&thread_lock);
		while (threads_starting)
			pthread_cond_wait(&thread_parent, &thread_lock);
		pthread_cond_broadcast(&thread_worker);
		pthread_mutex_unlock(&thread_lock);

		usleep(100000);

		/* Ok, all threads are patiently blocked, start waking folks up */
		wakeup_threads(waking_worker, thread_attr);

<<<<<<< HEAD
		for (i = 0; i < nblocked_threads; i++) {
=======
		for (i = 0; i < params.nthreads; i++) {
>>>>>>> upstream/android-13
			ret = pthread_join(blocked_worker[i], NULL);
			if (ret)
				err(EXIT_FAILURE, "pthread_join");
		}

		do_run_stats(waking_worker);
<<<<<<< HEAD
		if (!silent)
=======
		if (!params.silent)
>>>>>>> upstream/android-13
			print_run(waking_worker, j);

		free(waking_worker);
	}

	/* cleanup & report results */
	pthread_cond_destroy(&thread_parent);
	pthread_cond_destroy(&thread_worker);
	pthread_mutex_destroy(&thread_lock);
	pthread_attr_destroy(&thread_attr);

	print_summary();

	free(blocked_worker);
<<<<<<< HEAD
=======
	perf_cpu_map__put(cpu);
>>>>>>> upstream/android-13
	return ret;
}
#endif /* HAVE_PTHREAD_BARRIER */
