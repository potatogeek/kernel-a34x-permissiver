<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * sampleip: sample instruction pointer and frequency count in a BPF map.
 *
 * Copyright 2016 Netflix, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
=======
 */
#include <stdio.h>
#include <stdlib.h>
>>>>>>> upstream/android-13
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
<<<<<<< HEAD
#include <assert.h>
#include <linux/perf_event.h>
#include <linux/ptrace.h>
#include <linux/bpf.h>
#include <sys/ioctl.h>
#include "libbpf.h"
#include "bpf_load.h"
=======
#include <linux/perf_event.h>
#include <linux/ptrace.h>
#include <linux/bpf.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13
#include "perf-sys.h"
#include "trace_helpers.h"

#define DEFAULT_FREQ	99
#define DEFAULT_SECS	5
#define MAX_IPS		8192
#define PAGE_OFFSET	0xffff880000000000

<<<<<<< HEAD
=======
static int map_fd;
>>>>>>> upstream/android-13
static int nr_cpus;

static void usage(void)
{
	printf("USAGE: sampleip [-F freq] [duration]\n");
	printf("       -F freq    # sample frequency (Hertz), default 99\n");
	printf("       duration   # sampling duration (seconds), default 5\n");
}

<<<<<<< HEAD
static int sampling_start(int *pmu_fd, int freq)
{
	int i;
=======
static int sampling_start(int freq, struct bpf_program *prog,
			  struct bpf_link *links[])
{
	int i, pmu_fd;
>>>>>>> upstream/android-13

	struct perf_event_attr pe_sample_attr = {
		.type = PERF_TYPE_SOFTWARE,
		.freq = 1,
		.sample_period = freq,
		.config = PERF_COUNT_SW_CPU_CLOCK,
		.inherit = 1,
	};

	for (i = 0; i < nr_cpus; i++) {
<<<<<<< HEAD
		pmu_fd[i] = sys_perf_event_open(&pe_sample_attr, -1 /* pid */, i,
					    -1 /* group_fd */, 0 /* flags */);
		if (pmu_fd[i] < 0) {
			fprintf(stderr, "ERROR: Initializing perf sampling\n");
			return 1;
		}
		assert(ioctl(pmu_fd[i], PERF_EVENT_IOC_SET_BPF,
			     prog_fd[0]) == 0);
		assert(ioctl(pmu_fd[i], PERF_EVENT_IOC_ENABLE, 0) == 0);
=======
		pmu_fd = sys_perf_event_open(&pe_sample_attr, -1 /* pid */, i,
					    -1 /* group_fd */, 0 /* flags */);
		if (pmu_fd < 0) {
			fprintf(stderr, "ERROR: Initializing perf sampling\n");
			return 1;
		}
		links[i] = bpf_program__attach_perf_event(prog, pmu_fd);
		if (libbpf_get_error(links[i])) {
			fprintf(stderr, "ERROR: Attach perf event\n");
			links[i] = NULL;
			close(pmu_fd);
			return 1;
		}
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD
static void sampling_end(int *pmu_fd)
=======
static void sampling_end(struct bpf_link *links[])
>>>>>>> upstream/android-13
{
	int i;

	for (i = 0; i < nr_cpus; i++)
<<<<<<< HEAD
		close(pmu_fd[i]);
=======
		bpf_link__destroy(links[i]);
>>>>>>> upstream/android-13
}

struct ipcount {
	__u64 ip;
	__u32 count;
};

/* used for sorting */
struct ipcount counts[MAX_IPS];

static int count_cmp(const void *p1, const void *p2)
{
	return ((struct ipcount *)p1)->count - ((struct ipcount *)p2)->count;
}

static void print_ip_map(int fd)
{
	struct ksym *sym;
	__u64 key, next_key;
	__u32 value;
	int i, max;

	printf("%-19s %-32s %s\n", "ADDR", "KSYM", "COUNT");

	/* fetch IPs and counts */
	key = 0, i = 0;
	while (bpf_map_get_next_key(fd, &key, &next_key) == 0) {
		bpf_map_lookup_elem(fd, &next_key, &value);
		counts[i].ip = next_key;
		counts[i++].count = value;
		key = next_key;
	}
	max = i;

	/* sort and print */
	qsort(counts, max, sizeof(struct ipcount), count_cmp);
	for (i = 0; i < max; i++) {
		if (counts[i].ip > PAGE_OFFSET) {
			sym = ksym_search(counts[i].ip);
<<<<<<< HEAD
=======
			if (!sym) {
				printf("ksym not found. Is kallsyms loaded?\n");
				continue;
			}

>>>>>>> upstream/android-13
			printf("0x%-17llx %-32s %u\n", counts[i].ip, sym->name,
			       counts[i].count);
		} else {
			printf("0x%-17llx %-32s %u\n", counts[i].ip, "(user)",
			       counts[i].count);
		}
	}

	if (max == MAX_IPS) {
		printf("WARNING: IP hash was full (max %d entries); ", max);
		printf("may have dropped samples\n");
	}
}

static void int_exit(int sig)
{
	printf("\n");
<<<<<<< HEAD
	print_ip_map(map_fd[0]);
=======
	print_ip_map(map_fd);
>>>>>>> upstream/android-13
	exit(0);
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	char filename[256];
	int *pmu_fd, opt, freq = DEFAULT_FREQ, secs = DEFAULT_SECS;
=======
	int opt, freq = DEFAULT_FREQ, secs = DEFAULT_SECS, error = 1;
	struct bpf_object *obj = NULL;
	struct bpf_program *prog;
	struct bpf_link **links;
	char filename[256];
>>>>>>> upstream/android-13

	/* process arguments */
	while ((opt = getopt(argc, argv, "F:h")) != -1) {
		switch (opt) {
		case 'F':
			freq = atoi(optarg);
			break;
		case 'h':
		default:
			usage();
			return 0;
		}
	}
	if (argc - optind == 1)
		secs = atoi(argv[optind]);
	if (freq == 0 || secs == 0) {
		usage();
		return 1;
	}

	/* initialize kernel symbol translation */
	if (load_kallsyms()) {
		fprintf(stderr, "ERROR: loading /proc/kallsyms\n");
		return 2;
	}

	/* create perf FDs for each CPU */
<<<<<<< HEAD
	nr_cpus = sysconf(_SC_NPROCESSORS_CONF);
	pmu_fd = malloc(nr_cpus * sizeof(int));
	if (pmu_fd == NULL) {
		fprintf(stderr, "ERROR: malloc of pmu_fd\n");
		return 1;
	}

	/* load BPF program */
	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	if (load_bpf_file(filename)) {
		fprintf(stderr, "ERROR: loading BPF program (errno %d):\n",
			errno);
		if (strcmp(bpf_log_buf, "") == 0)
			fprintf(stderr, "Try: ulimit -l unlimited\n");
		else
			fprintf(stderr, "%s", bpf_log_buf);
		return 1;
	}
=======
	nr_cpus = sysconf(_SC_NPROCESSORS_ONLN);
	links = calloc(nr_cpus, sizeof(struct bpf_link *));
	if (!links) {
		fprintf(stderr, "ERROR: malloc of links\n");
		goto cleanup;
	}

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	obj = bpf_object__open_file(filename, NULL);
	if (libbpf_get_error(obj)) {
		fprintf(stderr, "ERROR: opening BPF object file failed\n");
		obj = NULL;
		goto cleanup;
	}

	prog = bpf_object__find_program_by_name(obj, "do_sample");
	if (!prog) {
		fprintf(stderr, "ERROR: finding a prog in obj file failed\n");
		goto cleanup;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		fprintf(stderr, "ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	map_fd = bpf_object__find_map_fd_by_name(obj, "ip_map");
	if (map_fd < 0) {
		fprintf(stderr, "ERROR: finding a map in obj file failed\n");
		goto cleanup;
	}

>>>>>>> upstream/android-13
	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);

	/* do sampling */
	printf("Sampling at %d Hertz for %d seconds. Ctrl-C also ends.\n",
	       freq, secs);
<<<<<<< HEAD
	if (sampling_start(pmu_fd, freq) != 0)
		return 1;
	sleep(secs);
	sampling_end(pmu_fd);
	free(pmu_fd);

	/* output sample counts */
	print_ip_map(map_fd[0]);

	return 0;
=======
	if (sampling_start(freq, prog, links) != 0)
		goto cleanup;

	sleep(secs);
	error = 0;

cleanup:
	sampling_end(links);
	/* output sample counts */
	if (!error)
		print_ip_map(map_fd);

	free(links);
	bpf_object__close(obj);
	return error;
>>>>>>> upstream/android-13
}
