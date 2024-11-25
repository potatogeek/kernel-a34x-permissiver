// SPDX-License-Identifier: GPL-2.0
/* eBPF example program:
 *
 * - Loads eBPF program
 *
 *   The eBPF program loads a filter from file and attaches the
 *   program to a cgroup using BPF_PROG_ATTACH
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <linux/bpf.h>
#include <bpf/bpf.h>
<<<<<<< HEAD

#include "bpf_insn.h"
#include "bpf_load.h"
=======
#include <bpf/libbpf.h>

#include "bpf_insn.h"
>>>>>>> upstream/android-13

static int usage(const char *argv0)
{
	printf("Usage: %s cg-path filter-path [filter-id]\n", argv0);
	return EXIT_FAILURE;
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	int cg_fd, ret, filter_id = 0;
=======
	int cg_fd, err, ret = EXIT_FAILURE, filter_id = 0, prog_cnt = 0;
	const char *link_pin_path = "/sys/fs/bpf/test_cgrp2_sock2";
	struct bpf_link *link = NULL;
	struct bpf_program *progs[2];
	struct bpf_program *prog;
	struct bpf_object *obj;
>>>>>>> upstream/android-13

	if (argc < 3)
		return usage(argv[0]);

<<<<<<< HEAD
	cg_fd = open(argv[1], O_DIRECTORY | O_RDONLY);
	if (cg_fd < 0) {
		printf("Failed to open cgroup path: '%s'\n", strerror(errno));
		return EXIT_FAILURE;
	}

	if (load_bpf_file(argv[2]))
		return EXIT_FAILURE;

	printf("Output from kernel verifier:\n%s\n-------\n", bpf_log_buf);

	if (argc > 3)
		filter_id = atoi(argv[3]);

	if (filter_id >= prog_cnt) {
		printf("Invalid program id; program not found in file\n");
		return EXIT_FAILURE;
	}

	ret = bpf_prog_attach(prog_fd[filter_id], cg_fd,
			      BPF_CGROUP_INET_SOCK_CREATE, 0);
	if (ret < 0) {
		printf("Failed to attach prog to cgroup: '%s'\n",
		       strerror(errno));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
=======
	if (argc > 3)
		filter_id = atoi(argv[3]);

	cg_fd = open(argv[1], O_DIRECTORY | O_RDONLY);
	if (cg_fd < 0) {
		printf("Failed to open cgroup path: '%s'\n", strerror(errno));
		return ret;
	}

	obj = bpf_object__open_file(argv[2], NULL);
	if (libbpf_get_error(obj)) {
		printf("ERROR: opening BPF object file failed\n");
		return ret;
	}

	bpf_object__for_each_program(prog, obj) {
		progs[prog_cnt] = prog;
		prog_cnt++;
	}

	if (filter_id >= prog_cnt) {
		printf("Invalid program id; program not found in file\n");
		goto cleanup;
	}

	/* load BPF program */
	if (bpf_object__load(obj)) {
		printf("ERROR: loading BPF object file failed\n");
		goto cleanup;
	}

	link = bpf_program__attach_cgroup(progs[filter_id], cg_fd);
	if (libbpf_get_error(link)) {
		printf("ERROR: bpf_program__attach failed\n");
		link = NULL;
		goto cleanup;
	}

	err = bpf_link__pin(link, link_pin_path);
	if (err < 0) {
		printf("ERROR: bpf_link__pin failed: %d\n", err);
		goto cleanup;
	}

	ret = EXIT_SUCCESS;

cleanup:
	bpf_link__destroy(link);
	bpf_object__close(obj);
	return ret;
>>>>>>> upstream/android-13
}
