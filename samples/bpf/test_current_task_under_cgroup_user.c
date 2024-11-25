<<<<<<< HEAD
/* Copyright (c) 2016 Sargun Dhillon <sargun@sargun.me>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2016 Sargun Dhillon <sargun@sargun.me>
>>>>>>> upstream/android-13
 */

#define _GNU_SOURCE
#include <stdio.h>
<<<<<<< HEAD
#include <linux/bpf.h>
#include <unistd.h>
#include <bpf/bpf.h>
#include "bpf_load.h"
#include <linux/bpf.h>
=======
#include <unistd.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13
#include "cgroup_helpers.h"

#define CGROUP_PATH		"/my-cgroup"

int main(int argc, char **argv)
{
	pid_t remote_pid, local_pid = getpid();
<<<<<<< HEAD
	int cg2, idx = 0, rc = 0;
	char filename[256];

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
=======
	struct bpf_link *link = NULL;
	struct bpf_program *prog;
	int cg2, idx = 0, rc = 1;
	struct bpf_object *obj;
	char filename[256];
	int map_fd[2];

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	obj = bpf_object__open_file(filename, NULL);
	if (libbpf_get_error(obj)) {
		fprintf(stderr, "ERROR: opening BPF object file failed\n");
		return 0;
	}

	prog = bpf_object__find_program_by_name(obj, "bpf_prog1");
	if (!prog) {
		printf("finding a prog in obj file failed\n");
		goto cleanup;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		fprintf(stderr, "ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	map_fd[0] = bpf_object__find_map_fd_by_name(obj, "cgroup_map");
	map_fd[1] = bpf_object__find_map_fd_by_name(obj, "perf_map");
	if (map_fd[0] < 0 || map_fd[1] < 0) {
		fprintf(stderr, "ERROR: finding a map in obj file failed\n");
		goto cleanup;
	}

	link = bpf_program__attach(prog);
	if (libbpf_get_error(link)) {
		fprintf(stderr, "ERROR: bpf_program__attach failed\n");
		link = NULL;
		goto cleanup;
>>>>>>> upstream/android-13
	}

	if (setup_cgroup_environment())
		goto err;

	cg2 = create_and_get_cgroup(CGROUP_PATH);

<<<<<<< HEAD
	if (!cg2)
=======
	if (cg2 < 0)
>>>>>>> upstream/android-13
		goto err;

	if (bpf_map_update_elem(map_fd[0], &idx, &cg2, BPF_ANY)) {
		log_err("Adding target cgroup to map");
		goto err;
	}

	if (join_cgroup(CGROUP_PATH))
		goto err;

	/*
	 * The installed helper program catched the sync call, and should
	 * write it to the map.
	 */

	sync();
	bpf_map_lookup_elem(map_fd[1], &idx, &remote_pid);

	if (local_pid != remote_pid) {
		fprintf(stderr,
			"BPF Helper didn't write correct PID to map, but: %d\n",
			remote_pid);
		goto err;
	}

	/* Verify the negative scenario; leave the cgroup */
	if (join_cgroup("/"))
		goto err;

	remote_pid = 0;
	bpf_map_update_elem(map_fd[1], &idx, &remote_pid, BPF_ANY);

	sync();
	bpf_map_lookup_elem(map_fd[1], &idx, &remote_pid);

	if (local_pid == remote_pid) {
		fprintf(stderr, "BPF cgroup negative test did not work\n");
		goto err;
	}

<<<<<<< HEAD
	goto out;
err:
	rc = 1;

out:
	close(cg2);
	cleanup_cgroup_environment();
=======
	rc = 0;

err:
	close(cg2);
	cleanup_cgroup_environment();

cleanup:
	bpf_link__destroy(link);
	bpf_object__close(obj);
>>>>>>> upstream/android-13
	return rc;
}
