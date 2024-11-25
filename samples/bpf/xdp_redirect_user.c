<<<<<<< HEAD
/* Copyright (c) 2016 John Fastabend <john.r.fastabend@intel.com>
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
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2016 John Fastabend <john.r.fastabend@intel.com>
 */
static const char *__doc__ =
"XDP redirect tool, using bpf_redirect helper\n"
"Usage: xdp_redirect <IFINDEX|IFNAME>_IN <IFINDEX|IFNAME>_OUT\n";

>>>>>>> upstream/android-13
#include <linux/bpf.h>
#include <linux/if_link.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
<<<<<<< HEAD
#include <unistd.h>
#include <libgen.h>
#include <sys/resource.h>

#include "bpf_load.h"
#include "bpf_util.h"
#include <bpf/bpf.h>

static int ifindex_in;
static int ifindex_out;
static bool ifindex_out_xdp_dummy_attached = true;

static __u32 xdp_flags;

static void int_exit(int sig)
{
	bpf_set_link_xdp_fd(ifindex_in, -1, xdp_flags);
	if (ifindex_out_xdp_dummy_attached)
		bpf_set_link_xdp_fd(ifindex_out, -1, xdp_flags);
	exit(0);
}

static void poll_stats(int interval, int ifindex)
{
	unsigned int nr_cpus = bpf_num_possible_cpus();
	__u64 values[nr_cpus], prev[nr_cpus];

	memset(prev, 0, sizeof(prev));

	while (1) {
		__u64 sum = 0;
		__u32 key = 0;
		int i;

		sleep(interval);
		assert(bpf_map_lookup_elem(map_fd[1], &key, values) == 0);
		for (i = 0; i < nr_cpus; i++)
			sum += (values[i] - prev[i]);
		if (sum)
			printf("ifindex %i: %10llu pkt/s\n",
			       ifindex, sum / interval);
		memcpy(prev, values, sizeof(values));
	}
}

static void usage(const char *prog)
{
	fprintf(stderr,
		"usage: %s [OPTS] IFINDEX_IN IFINDEX_OUT\n\n"
		"OPTS:\n"
		"    -S    use skb-mode\n"
		"    -N    enforce native mode\n",
		prog);
}


int main(int argc, char **argv)
{
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	const char *optstr = "SN";
	char filename[256];
	int ret, opt, key = 0;

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch (opt) {
		case 'S':
			xdp_flags |= XDP_FLAGS_SKB_MODE;
			break;
		case 'N':
			xdp_flags |= XDP_FLAGS_DRV_MODE;
			break;
		default:
			usage(basename(argv[0]));
			return 1;
		}
	}

	if (optind == argc) {
		printf("usage: %s IFINDEX_IN IFINDEX_OUT\n", argv[0]);
		return 1;
	}

	if (setrlimit(RLIMIT_MEMLOCK, &r)) {
		perror("setrlimit(RLIMIT_MEMLOCK)");
		return 1;
	}

	ifindex_in = strtoul(argv[optind], NULL, 0);
	ifindex_out = strtoul(argv[optind + 1], NULL, 0);
	printf("input: %d output: %d\n", ifindex_in, ifindex_out);

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
	}

	if (!prog_fd[0]) {
		printf("load_bpf_file: %s\n", strerror(errno));
		return 1;
	}

	if (bpf_set_link_xdp_fd(ifindex_in, prog_fd[0], xdp_flags) < 0) {
		printf("ERROR: link set xdp fd failed on %d\n", ifindex_in);
		return 1;
	}

	/* Loading dummy XDP prog on out-device */
	if (bpf_set_link_xdp_fd(ifindex_out, prog_fd[1],
			    (xdp_flags | XDP_FLAGS_UPDATE_IF_NOEXIST)) < 0) {
		printf("WARN: link set xdp fd failed on %d\n", ifindex_out);
		ifindex_out_xdp_dummy_attached = false;
	}

	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);

	/* bpf redirect port */
	ret = bpf_map_update_elem(map_fd[0], &key, &ifindex_out, 0);
	if (ret) {
		perror("bpf_update_elem");
		goto out;
	}

	poll_stats(2, ifindex_out);

out:
	return 0;
=======
#include <net/if.h>
#include <unistd.h>
#include <libgen.h>
#include <getopt.h>
#include <sys/resource.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include "bpf_util.h"
#include "xdp_sample_user.h"
#include "xdp_redirect.skel.h"

static int mask = SAMPLE_RX_CNT | SAMPLE_REDIRECT_ERR_CNT |
		  SAMPLE_EXCEPTION_CNT | SAMPLE_DEVMAP_XMIT_CNT_MULTI;

DEFINE_SAMPLE_INIT(xdp_redirect);

static const struct option long_options[] = {
	{"help",	no_argument,		NULL, 'h' },
	{"skb-mode",	no_argument,		NULL, 'S' },
	{"force",	no_argument,		NULL, 'F' },
	{"stats",	no_argument,		NULL, 's' },
	{"interval",	required_argument,	NULL, 'i' },
	{"verbose",	no_argument,		NULL, 'v' },
	{}
};

int main(int argc, char **argv)
{
	int ifindex_in, ifindex_out, opt;
	char str[2 * IF_NAMESIZE + 1];
	char ifname_out[IF_NAMESIZE];
	char ifname_in[IF_NAMESIZE];
	int ret = EXIT_FAIL_OPTION;
	unsigned long interval = 2;
	struct xdp_redirect *skel;
	bool generic = false;
	bool force = false;
	bool error = true;

	while ((opt = getopt_long(argc, argv, "hSFi:vs",
				  long_options, NULL)) != -1) {
		switch (opt) {
		case 'S':
			generic = true;
			mask &= ~(SAMPLE_DEVMAP_XMIT_CNT |
				  SAMPLE_DEVMAP_XMIT_CNT_MULTI);
			break;
		case 'F':
			force = true;
			break;
		case 'i':
			interval = strtoul(optarg, NULL, 0);
			break;
		case 'v':
			sample_switch_mode();
			break;
		case 's':
			mask |= SAMPLE_REDIRECT_CNT;
			break;
		case 'h':
			error = false;
		default:
			sample_usage(argv, long_options, __doc__, mask, error);
			return ret;
		}
	}

	if (argc <= optind + 1) {
		sample_usage(argv, long_options, __doc__, mask, true);
		return ret;
	}

	ifindex_in = if_nametoindex(argv[optind]);
	if (!ifindex_in)
		ifindex_in = strtoul(argv[optind], NULL, 0);

	ifindex_out = if_nametoindex(argv[optind + 1]);
	if (!ifindex_out)
		ifindex_out = strtoul(argv[optind + 1], NULL, 0);

	if (!ifindex_in || !ifindex_out) {
		fprintf(stderr, "Bad interface index or name\n");
		sample_usage(argv, long_options, __doc__, mask, true);
		goto end;
	}

	skel = xdp_redirect__open();
	if (!skel) {
		fprintf(stderr, "Failed to xdp_redirect__open: %s\n", strerror(errno));
		ret = EXIT_FAIL_BPF;
		goto end;
	}

	ret = sample_init_pre_load(skel);
	if (ret < 0) {
		fprintf(stderr, "Failed to sample_init_pre_load: %s\n", strerror(-ret));
		ret = EXIT_FAIL_BPF;
		goto end_destroy;
	}

	skel->rodata->from_match[0] = ifindex_in;
	skel->rodata->to_match[0] = ifindex_out;
	skel->rodata->ifindex_out = ifindex_out;

	ret = xdp_redirect__load(skel);
	if (ret < 0) {
		fprintf(stderr, "Failed to xdp_redirect__load: %s\n", strerror(errno));
		ret = EXIT_FAIL_BPF;
		goto end_destroy;
	}

	ret = sample_init(skel, mask);
	if (ret < 0) {
		fprintf(stderr, "Failed to initialize sample: %s\n", strerror(-ret));
		ret = EXIT_FAIL;
		goto end_destroy;
	}

	ret = EXIT_FAIL_XDP;
	if (sample_install_xdp(skel->progs.xdp_redirect_prog, ifindex_in,
			       generic, force) < 0)
		goto end_destroy;

	/* Loading dummy XDP prog on out-device */
	sample_install_xdp(skel->progs.xdp_redirect_dummy_prog, ifindex_out,
			   generic, force);

	ret = EXIT_FAIL;
	if (!if_indextoname(ifindex_in, ifname_in)) {
		fprintf(stderr, "Failed to if_indextoname for %d: %s\n", ifindex_in,
			strerror(errno));
		goto end_destroy;
	}

	if (!if_indextoname(ifindex_out, ifname_out)) {
		fprintf(stderr, "Failed to if_indextoname for %d: %s\n", ifindex_out,
			strerror(errno));
		goto end_destroy;
	}

	safe_strncpy(str, get_driver_name(ifindex_in), sizeof(str));
	printf("Redirecting from %s (ifindex %d; driver %s) to %s (ifindex %d; driver %s)\n",
	       ifname_in, ifindex_in, str, ifname_out, ifindex_out, get_driver_name(ifindex_out));
	snprintf(str, sizeof(str), "%s->%s", ifname_in, ifname_out);

	ret = sample_run(interval, NULL, NULL);
	if (ret < 0) {
		fprintf(stderr, "Failed during sample run: %s\n", strerror(-ret));
		ret = EXIT_FAIL;
		goto end_destroy;
	}
	ret = EXIT_OK;
end_destroy:
	xdp_redirect__destroy(skel);
end:
	sample_exit(ret);
>>>>>>> upstream/android-13
}
