<<<<<<< HEAD
/* Copyright (c) 2015 PLUMgrid, http://plumgrid.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2015 PLUMgrid, http://plumgrid.com
>>>>>>> upstream/android-13
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
<<<<<<< HEAD
#include <linux/bpf.h>
#include <sys/resource.h>

#include <bpf/bpf.h>
#include "bpf_load.h"
=======
#include <sys/resource.h>

#include <bpf/bpf.h>
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13

struct pair {
	long long val;
	__u64 ip;
};

static __u64 time_get_ns(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000000000ull + ts.tv_nsec;
}

static void print_old_objects(int fd)
{
	long long val = time_get_ns();
	__u64 key, next_key;
	struct pair v;

<<<<<<< HEAD
	key = write(1, "\e[1;1H\e[2J", 12); /* clear screen */

	key = -1;
	while (bpf_map_get_next_key(map_fd[0], &key, &next_key) == 0) {
		bpf_map_lookup_elem(map_fd[0], &next_key, &v);
=======
	key = write(1, "\e[1;1H\e[2J", 11); /* clear screen */

	key = -1;
	while (bpf_map_get_next_key(fd, &key, &next_key) == 0) {
		bpf_map_lookup_elem(fd, &next_key, &v);
>>>>>>> upstream/android-13
		key = next_key;
		if (val - v.val < 1000000000ll)
			/* object was allocated more then 1 sec ago */
			continue;
		printf("obj 0x%llx is %2lldsec old was allocated at ip %llx\n",
		       next_key, (val - v.val) / 1000000000ll, v.ip);
	}
}

int main(int ac, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	char filename[256];
	int i;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

	if (setrlimit(RLIMIT_MEMLOCK, &r)) {
		perror("setrlimit(RLIMIT_MEMLOCK, RLIM_INFINITY)");
		return 1;
	}

	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
	}

	for (i = 0; ; i++) {
		print_old_objects(map_fd[1]);
		sleep(1);
	}

=======
	struct bpf_link *links[2];
	struct bpf_program *prog;
	struct bpf_object *obj;
	char filename[256];
	int map_fd, i, j = 0;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	obj = bpf_object__open_file(filename, NULL);
	if (libbpf_get_error(obj)) {
		fprintf(stderr, "ERROR: opening BPF object file failed\n");
		return 0;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		fprintf(stderr, "ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	map_fd = bpf_object__find_map_fd_by_name(obj, "my_map");
	if (map_fd < 0) {
		fprintf(stderr, "ERROR: finding a map in obj file failed\n");
		goto cleanup;
	}

	bpf_object__for_each_program(prog, obj) {
		links[j] = bpf_program__attach(prog);
		if (libbpf_get_error(links[j])) {
			fprintf(stderr, "ERROR: bpf_program__attach failed\n");
			links[j] = NULL;
			goto cleanup;
		}
		j++;
	}

	for (i = 0; ; i++) {
		print_old_objects(map_fd);
		sleep(1);
	}

cleanup:
	for (j--; j >= 0; j--)
		bpf_link__destroy(links[j]);

	bpf_object__close(obj);
>>>>>>> upstream/android-13
	return 0;
}
