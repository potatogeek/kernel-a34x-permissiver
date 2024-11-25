<<<<<<< HEAD
/* Copyright (c) 2016 Facebook
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2016 Facebook
>>>>>>> upstream/android-13
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
<<<<<<< HEAD
#include <linux/bpf.h>
#include <string.h>
#include <linux/perf_event.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/resource.h>
#include "libbpf.h"
#include "bpf_load.h"
=======
#include <linux/perf_event.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
>>>>>>> upstream/android-13
#include "trace_helpers.h"

#define PRINT_RAW_ADDR 0

<<<<<<< HEAD
=======
/* counts, stackmap */
static int map_fd[2];

>>>>>>> upstream/android-13
static void print_ksym(__u64 addr)
{
	struct ksym *sym;

	if (!addr)
		return;
	sym = ksym_search(addr);
<<<<<<< HEAD
=======
	if (!sym) {
		printf("ksym not found. Is kallsyms loaded?\n");
		return;
	}

>>>>>>> upstream/android-13
	if (PRINT_RAW_ADDR)
		printf("%s/%llx;", sym->name, addr);
	else
		printf("%s;", sym->name);
}

#define TASK_COMM_LEN 16

struct key_t {
	char waker[TASK_COMM_LEN];
	char target[TASK_COMM_LEN];
	__u32 wret;
	__u32 tret;
};

static void print_stack(struct key_t *key, __u64 count)
{
	__u64 ip[PERF_MAX_STACK_DEPTH] = {};
	static bool warned;
	int i;

	printf("%s;", key->target);
<<<<<<< HEAD
	if (bpf_map_lookup_elem(map_fd[3], &key->tret, ip) != 0) {
=======
	if (bpf_map_lookup_elem(map_fd[1], &key->tret, ip) != 0) {
>>>>>>> upstream/android-13
		printf("---;");
	} else {
		for (i = PERF_MAX_STACK_DEPTH - 1; i >= 0; i--)
			print_ksym(ip[i]);
	}
	printf("-;");
<<<<<<< HEAD
	if (bpf_map_lookup_elem(map_fd[3], &key->wret, ip) != 0) {
=======
	if (bpf_map_lookup_elem(map_fd[1], &key->wret, ip) != 0) {
>>>>>>> upstream/android-13
		printf("---;");
	} else {
		for (i = 0; i < PERF_MAX_STACK_DEPTH; i++)
			print_ksym(ip[i]);
	}
	printf(";%s %lld\n", key->waker, count);

	if ((key->tret == -EEXIST || key->wret == -EEXIST) && !warned) {
		printf("stackmap collisions seen. Consider increasing size\n");
		warned = true;
	} else if (((int)(key->tret) < 0 || (int)(key->wret) < 0)) {
		printf("err stackid %d %d\n", key->tret, key->wret);
	}
}

static void print_stacks(int fd)
{
	struct key_t key = {}, next_key;
	__u64 value;

	while (bpf_map_get_next_key(fd, &key, &next_key) == 0) {
		bpf_map_lookup_elem(fd, &next_key, &value);
		print_stack(&next_key, value);
		key = next_key;
	}
}

static void int_exit(int sig)
{
	print_stacks(map_fd[0]);
	exit(0);
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	char filename[256];
	int delay = 1;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	setrlimit(RLIMIT_MEMLOCK, &r);

	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);
=======
	struct bpf_object *obj = NULL;
	struct bpf_link *links[2];
	struct bpf_program *prog;
	int delay = 1, i = 0;
	char filename[256];
>>>>>>> upstream/android-13

	if (load_kallsyms()) {
		printf("failed to process /proc/kallsyms\n");
		return 2;
	}

<<<<<<< HEAD
	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
=======
	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	obj = bpf_object__open_file(filename, NULL);
	if (libbpf_get_error(obj)) {
		fprintf(stderr, "ERROR: opening BPF object file failed\n");
		obj = NULL;
		goto cleanup;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		fprintf(stderr, "ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	map_fd[0] = bpf_object__find_map_fd_by_name(obj, "counts");
	map_fd[1] = bpf_object__find_map_fd_by_name(obj, "stackmap");
	if (map_fd[0] < 0 || map_fd[1] < 0) {
		fprintf(stderr, "ERROR: finding a map in obj file failed\n");
		goto cleanup;
	}

	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);

	bpf_object__for_each_program(prog, obj) {
		links[i] = bpf_program__attach(prog);
		if (libbpf_get_error(links[i])) {
			fprintf(stderr, "ERROR: bpf_program__attach failed\n");
			links[i] = NULL;
			goto cleanup;
		}
		i++;
>>>>>>> upstream/android-13
	}

	if (argc > 1)
		delay = atoi(argv[1]);
	sleep(delay);
	print_stacks(map_fd[0]);

<<<<<<< HEAD
=======
cleanup:
	for (i--; i >= 0; i--)
		bpf_link__destroy(links[i]);

	bpf_object__close(obj);
>>>>>>> upstream/android-13
	return 0;
}
