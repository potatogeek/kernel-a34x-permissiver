// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2017-18 David Ahern <dsahern@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 */

#include <linux/bpf.h>
#include <linux/if_link.h>
#include <linux/limits.h>
#include <net/if.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libgen.h>

<<<<<<< HEAD
#include "bpf/libbpf.h"
#include <bpf/bpf.h>


static int do_attach(int idx, int fd, const char *name)
{
	int err;

	err = bpf_set_link_xdp_fd(idx, fd, 0);
	if (err < 0)
		printf("ERROR: failed to attach program to %s\n", name);
=======
#include <bpf/libbpf.h>
#include <bpf/bpf.h>

static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;

static int do_attach(int idx, int prog_fd, int map_fd, const char *name)
{
	int err;

	err = bpf_set_link_xdp_fd(idx, prog_fd, xdp_flags);
	if (err < 0) {
		printf("ERROR: failed to attach program to %s\n", name);
		return err;
	}

	/* Adding ifindex as a possible egress TX port */
	err = bpf_map_update_elem(map_fd, &idx, &idx, 0);
	if (err)
		printf("ERROR: failed using device %s as TX-port\n", name);
>>>>>>> upstream/android-13

	return err;
}

static int do_detach(int idx, const char *name)
{
	int err;

<<<<<<< HEAD
	err = bpf_set_link_xdp_fd(idx, -1, 0);
	if (err < 0)
		printf("ERROR: failed to detach program from %s\n", name);

=======
	err = bpf_set_link_xdp_fd(idx, -1, xdp_flags);
	if (err < 0)
		printf("ERROR: failed to detach program from %s\n", name);

	/* TODO: Remember to cleanup map, when adding use of shared map
	 *  bpf_map_delete_elem((map_fd, &idx);
	 */
>>>>>>> upstream/android-13
	return err;
}

static void usage(const char *prog)
{
	fprintf(stderr,
		"usage: %s [OPTS] interface-list\n"
		"\nOPTS:\n"
		"    -d    detach program\n"
<<<<<<< HEAD
=======
		"    -S    use skb-mode\n"
		"    -F    force loading prog\n"
>>>>>>> upstream/android-13
		"    -D    direct table lookups (skip fib rules)\n",
		prog);
}

int main(int argc, char **argv)
{
	struct bpf_prog_load_attr prog_load_attr = {
		.prog_type	= BPF_PROG_TYPE_XDP,
	};
	const char *prog_name = "xdp_fwd";
	struct bpf_program *prog;
<<<<<<< HEAD
	char filename[PATH_MAX];
	struct bpf_object *obj;
	int opt, i, idx, err;
	int prog_fd, map_fd;
	int attach = 1;
	int ret = 0;

	while ((opt = getopt(argc, argv, ":dD")) != -1) {
=======
	int prog_fd, map_fd = -1;
	char filename[PATH_MAX];
	struct bpf_object *obj;
	int opt, i, idx, err;
	int attach = 1;
	int ret = 0;

	while ((opt = getopt(argc, argv, ":dDSF")) != -1) {
>>>>>>> upstream/android-13
		switch (opt) {
		case 'd':
			attach = 0;
			break;
<<<<<<< HEAD
=======
		case 'S':
			xdp_flags |= XDP_FLAGS_SKB_MODE;
			break;
		case 'F':
			xdp_flags &= ~XDP_FLAGS_UPDATE_IF_NOEXIST;
			break;
>>>>>>> upstream/android-13
		case 'D':
			prog_name = "xdp_fwd_direct";
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

	if (attach) {
		snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
		prog_load_attr.file = filename;

		if (access(filename, O_RDONLY) < 0) {
			printf("error accessing file %s: %s\n",
				filename, strerror(errno));
			return 1;
		}

<<<<<<< HEAD
		if (bpf_prog_load_xattr(&prog_load_attr, &obj, &prog_fd))
			return 1;
=======
		err = bpf_prog_load_xattr(&prog_load_attr, &obj, &prog_fd);
		if (err) {
			printf("Does kernel support devmap lookup?\n");
			/* If not, the error message will be:
			 *  "cannot pass map_type 14 into func bpf_map_lookup_elem#1"
			 */
			return 1;
		}
>>>>>>> upstream/android-13

		prog = bpf_object__find_program_by_title(obj, prog_name);
		prog_fd = bpf_program__fd(prog);
		if (prog_fd < 0) {
			printf("program not found: %s\n", strerror(prog_fd));
			return 1;
		}
		map_fd = bpf_map__fd(bpf_object__find_map_by_name(obj,
<<<<<<< HEAD
								  "tx_port"));
=======
							"xdp_tx_ports"));
>>>>>>> upstream/android-13
		if (map_fd < 0) {
			printf("map not found: %s\n", strerror(map_fd));
			return 1;
		}
	}
<<<<<<< HEAD
	if (attach) {
		for (i = 1; i < 64; ++i)
			bpf_map_update_elem(map_fd, &i, &i, 0);
	}
=======
>>>>>>> upstream/android-13

	for (i = optind; i < argc; ++i) {
		idx = if_nametoindex(argv[i]);
		if (!idx)
			idx = strtoul(argv[i], NULL, 0);

		if (!idx) {
			fprintf(stderr, "Invalid arg\n");
			return 1;
		}
		if (!attach) {
			err = do_detach(idx, argv[i]);
			if (err)
				ret = err;
		} else {
<<<<<<< HEAD
			err = do_attach(idx, prog_fd, argv[i]);
=======
			err = do_attach(idx, prog_fd, map_fd, argv[i]);
>>>>>>> upstream/android-13
			if (err)
				ret = err;
		}
	}

	return ret;
}
