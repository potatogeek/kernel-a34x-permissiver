<<<<<<< HEAD
/* Copyright (c) 2017 Facebook
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2017 Facebook
>>>>>>> upstream/android-13
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <signal.h>
#include <linux/bpf.h>
#include <string.h>
#include <linux/perf_event.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <bpf/bpf.h>
#include "bpf_load.h"
=======
#include <string.h>
#include <linux/perf_event.h>
#include <errno.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
>>>>>>> upstream/android-13

/* This program verifies bpf attachment to tracepoint sys_enter_* and sys_exit_*.
 * This requires kernel CONFIG_FTRACE_SYSCALLS to be set.
 */

static void usage(const char *cmd)
{
	printf("USAGE: %s [-i num_progs] [-h]\n", cmd);
	printf("       -i num_progs      # number of progs of the test\n");
	printf("       -h                # help\n");
}

static void verify_map(int map_id)
{
	__u32 key = 0;
	__u32 val;

	if (bpf_map_lookup_elem(map_id, &key, &val) != 0) {
		fprintf(stderr, "map_lookup failed: %s\n", strerror(errno));
		return;
	}
	if (val == 0) {
		fprintf(stderr, "failed: map #%d returns value 0\n", map_id);
		return;
	}
	val = 0;
	if (bpf_map_update_elem(map_id, &key, &val, BPF_ANY) != 0) {
		fprintf(stderr, "map_update failed: %s\n", strerror(errno));
		return;
	}
}

static int test(char *filename, int num_progs)
{
<<<<<<< HEAD
	int i, fd, map0_fds[num_progs], map1_fds[num_progs];

	for (i = 0; i < num_progs; i++) {
		if (load_bpf_file(filename)) {
			fprintf(stderr, "%s", bpf_log_buf);
			return 1;
		}
		printf("prog #%d: map ids %d %d\n", i, map_fd[0], map_fd[1]);
		map0_fds[i] = map_fd[0];
		map1_fds[i] = map_fd[1];
=======
	int map0_fds[num_progs], map1_fds[num_progs], fd, i, j = 0;
	struct bpf_link *links[num_progs * 4];
	struct bpf_object *objs[num_progs];
	struct bpf_program *prog;

	for (i = 0; i < num_progs; i++) {
		objs[i] = bpf_object__open_file(filename, NULL);
		if (libbpf_get_error(objs[i])) {
			fprintf(stderr, "opening BPF object file failed\n");
			objs[i] = NULL;
			goto cleanup;
		}

		/* load BPF program */
		if (bpf_object__load(objs[i])) {
			fprintf(stderr, "loading BPF object file failed\n");
			goto cleanup;
		}

		map0_fds[i] = bpf_object__find_map_fd_by_name(objs[i],
							      "enter_open_map");
		map1_fds[i] = bpf_object__find_map_fd_by_name(objs[i],
							      "exit_open_map");
		if (map0_fds[i] < 0 || map1_fds[i] < 0) {
			fprintf(stderr, "finding a map in obj file failed\n");
			goto cleanup;
		}

		bpf_object__for_each_program(prog, objs[i]) {
			links[j] = bpf_program__attach(prog);
			if (libbpf_get_error(links[j])) {
				fprintf(stderr, "bpf_program__attach failed\n");
				links[j] = NULL;
				goto cleanup;
			}
			j++;
		}
		printf("prog #%d: map ids %d %d\n", i, map0_fds[i], map1_fds[i]);
>>>>>>> upstream/android-13
	}

	/* current load_bpf_file has perf_event_open default pid = -1
	 * and cpu = 0, which permits attached bpf execution on
	 * all cpus for all pid's. bpf program execution ignores
	 * cpu affinity.
	 */
	/* trigger some "open" operations */
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "open failed: %s\n", strerror(errno));
		return 1;
	}
	close(fd);

	/* verify the map */
	for (i = 0; i < num_progs; i++) {
		verify_map(map0_fds[i]);
		verify_map(map1_fds[i]);
	}

<<<<<<< HEAD
=======
cleanup:
	for (j--; j >= 0; j--)
		bpf_link__destroy(links[j]);

	for (i--; i >= 0; i--)
		bpf_object__close(objs[i]);
>>>>>>> upstream/android-13
	return 0;
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
=======
>>>>>>> upstream/android-13
	int opt, num_progs = 1;
	char filename[256];

	while ((opt = getopt(argc, argv, "i:h")) != -1) {
		switch (opt) {
		case 'i':
			num_progs = atoi(optarg);
			break;
		case 'h':
		default:
			usage(argv[0]);
			return 0;
		}
	}

<<<<<<< HEAD
	setrlimit(RLIMIT_MEMLOCK, &r);
=======
>>>>>>> upstream/android-13
	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

	return test(filename, num_progs);
}
