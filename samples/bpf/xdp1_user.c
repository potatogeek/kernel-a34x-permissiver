<<<<<<< HEAD
/* Copyright (c) 2016 PLUMgrid
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2016 PLUMgrid
>>>>>>> upstream/android-13
 */
#include <linux/bpf.h>
#include <linux/if_link.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/resource.h>
<<<<<<< HEAD

#include "bpf_util.h"
#include "bpf/bpf.h"
#include "bpf/libbpf.h"

static int ifindex;
static __u32 xdp_flags;

static void int_exit(int sig)
{
	bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
=======
#include <net/if.h>

#include "bpf_util.h"
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

static int ifindex;
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
static __u32 prog_id;

static void int_exit(int sig)
{
	__u32 curr_prog_id = 0;

	if (bpf_get_link_xdp_id(ifindex, &curr_prog_id, xdp_flags)) {
		printf("bpf_get_link_xdp_id failed\n");
		exit(1);
	}
	if (prog_id == curr_prog_id)
		bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
	else if (!curr_prog_id)
		printf("couldn't find a prog id on a given interface\n");
	else
		printf("program on interface changed, not removing\n");
>>>>>>> upstream/android-13
	exit(0);
}

/* simple per-protocol drop counter
 */
static void poll_stats(int map_fd, int interval)
{
	unsigned int nr_cpus = bpf_num_possible_cpus();
<<<<<<< HEAD
	const unsigned int nr_keys = 256;
	__u64 values[nr_cpus], prev[nr_keys][nr_cpus];
	__u32 key;
	int i;

	memset(prev, 0, sizeof(prev));

	while (1) {
		sleep(interval);

		for (key = 0; key < nr_keys; key++) {
=======
	__u64 values[nr_cpus], prev[UINT8_MAX] = { 0 };
	int i;

	while (1) {
		__u32 key = UINT32_MAX;

		sleep(interval);

		while (bpf_map_get_next_key(map_fd, &key, &key) != -1) {
>>>>>>> upstream/android-13
			__u64 sum = 0;

			assert(bpf_map_lookup_elem(map_fd, &key, values) == 0);
			for (i = 0; i < nr_cpus; i++)
<<<<<<< HEAD
				sum += (values[i] - prev[key][i]);
			if (sum)
				printf("proto %u: %10llu pkt/s\n",
				       key, sum / interval);
			memcpy(prev[key], values, sizeof(values));
=======
				sum += values[i];
			if (sum > prev[key])
				printf("proto %u: %10llu pkt/s\n",
				       key, (sum - prev[key]) / interval);
			prev[key] = sum;
>>>>>>> upstream/android-13
		}
	}
}

static void usage(const char *prog)
{
	fprintf(stderr,
<<<<<<< HEAD
		"usage: %s [OPTS] IFINDEX\n\n"
		"OPTS:\n"
		"    -S    use skb-mode\n"
		"    -N    enforce native mode\n",
=======
		"usage: %s [OPTS] IFACE\n\n"
		"OPTS:\n"
		"    -S    use skb-mode\n"
		"    -N    enforce native mode\n"
		"    -F    force loading prog\n",
>>>>>>> upstream/android-13
		prog);
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	struct bpf_prog_load_attr prog_load_attr = {
		.prog_type	= BPF_PROG_TYPE_XDP,
	};
	const char *optstr = "SN";
=======
	struct bpf_prog_load_attr prog_load_attr = {
		.prog_type	= BPF_PROG_TYPE_XDP,
	};
	struct bpf_prog_info info = {};
	__u32 info_len = sizeof(info);
	const char *optstr = "FSN";
>>>>>>> upstream/android-13
	int prog_fd, map_fd, opt;
	struct bpf_object *obj;
	struct bpf_map *map;
	char filename[256];
<<<<<<< HEAD
=======
	int err;
>>>>>>> upstream/android-13

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch (opt) {
		case 'S':
			xdp_flags |= XDP_FLAGS_SKB_MODE;
			break;
		case 'N':
<<<<<<< HEAD
			xdp_flags |= XDP_FLAGS_DRV_MODE;
=======
			/* default, set below */
			break;
		case 'F':
			xdp_flags &= ~XDP_FLAGS_UPDATE_IF_NOEXIST;
>>>>>>> upstream/android-13
			break;
		default:
			usage(basename(argv[0]));
			return 1;
		}
	}

<<<<<<< HEAD
=======
	if (!(xdp_flags & XDP_FLAGS_SKB_MODE))
		xdp_flags |= XDP_FLAGS_DRV_MODE;

>>>>>>> upstream/android-13
	if (optind == argc) {
		usage(basename(argv[0]));
		return 1;
	}

<<<<<<< HEAD
	if (setrlimit(RLIMIT_MEMLOCK, &r)) {
		perror("setrlimit(RLIMIT_MEMLOCK)");
		return 1;
	}

	ifindex = strtoul(argv[optind], NULL, 0);

=======
	ifindex = if_nametoindex(argv[optind]);
	if (!ifindex) {
		perror("if_nametoindex");
		return 1;
	}

>>>>>>> upstream/android-13
	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	prog_load_attr.file = filename;

	if (bpf_prog_load_xattr(&prog_load_attr, &obj, &prog_fd))
		return 1;

	map = bpf_map__next(NULL, obj);
	if (!map) {
		printf("finding a map in obj file failed\n");
		return 1;
	}
	map_fd = bpf_map__fd(map);

	if (!prog_fd) {
<<<<<<< HEAD
		printf("load_bpf_file: %s\n", strerror(errno));
=======
		printf("bpf_prog_load_xattr: %s\n", strerror(errno));
>>>>>>> upstream/android-13
		return 1;
	}

	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);

	if (bpf_set_link_xdp_fd(ifindex, prog_fd, xdp_flags) < 0) {
		printf("link set xdp fd failed\n");
		return 1;
	}

<<<<<<< HEAD
=======
	err = bpf_obj_get_info_by_fd(prog_fd, &info, &info_len);
	if (err) {
		printf("can't get prog info - %s\n", strerror(errno));
		return err;
	}
	prog_id = info.id;

>>>>>>> upstream/android-13
	poll_stats(map_fd, 2);

	return 0;
}
