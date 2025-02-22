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
#include <linux/bpf.h>
#include <linux/if_link.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
=======
#include <net/if.h>
>>>>>>> upstream/android-13
#include <sys/resource.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <time.h>
<<<<<<< HEAD
#include "bpf_load.h"
=======
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13
#include <bpf/bpf.h>
#include "bpf_util.h"
#include "xdp_tx_iptunnel_common.h"

#define STATS_INTERVAL_S 2U

static int ifindex = -1;
<<<<<<< HEAD
static __u32 xdp_flags = 0;

static void int_exit(int sig)
{
	if (ifindex > -1)
		bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
=======
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
static int rxcnt_map_fd;
static __u32 prog_id;

static void int_exit(int sig)
{
	__u32 curr_prog_id = 0;

	if (ifindex > -1) {
		if (bpf_get_link_xdp_id(ifindex, &curr_prog_id, xdp_flags)) {
			printf("bpf_get_link_xdp_id failed\n");
			exit(1);
		}
		if (prog_id == curr_prog_id)
			bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
		else if (!curr_prog_id)
			printf("couldn't find a prog id on a given iface\n");
		else
			printf("program on interface changed, not removing\n");
	}
>>>>>>> upstream/android-13
	exit(0);
}

/* simple per-protocol drop counter
 */
static void poll_stats(unsigned int kill_after_s)
{
	const unsigned int nr_protos = 256;
	unsigned int nr_cpus = bpf_num_possible_cpus();
	time_t started_at = time(NULL);
	__u64 values[nr_cpus], prev[nr_protos][nr_cpus];
	__u32 proto;
	int i;

	memset(prev, 0, sizeof(prev));

	while (!kill_after_s || time(NULL) - started_at <= kill_after_s) {
		sleep(STATS_INTERVAL_S);

		for (proto = 0; proto < nr_protos; proto++) {
			__u64 sum = 0;

<<<<<<< HEAD
			assert(bpf_map_lookup_elem(map_fd[0], &proto, values) == 0);
=======
			assert(bpf_map_lookup_elem(rxcnt_map_fd, &proto,
						   values) == 0);
>>>>>>> upstream/android-13
			for (i = 0; i < nr_cpus; i++)
				sum += (values[i] - prev[proto][i]);

			if (sum)
				printf("proto %u: sum:%10llu pkts, rate:%10llu pkts/s\n",
				       proto, sum, sum / STATS_INTERVAL_S);
			memcpy(prev[proto], values, sizeof(values));
		}
	}
}

static void usage(const char *cmd)
{
	printf("Start a XDP prog which encapsulates incoming packets\n"
	       "in an IPv4/v6 header and XDP_TX it out.  The dst <VIP:PORT>\n"
	       "is used to select packets to encapsulate\n\n");
	printf("Usage: %s [...]\n", cmd);
<<<<<<< HEAD
	printf("    -i <ifindex> Interface Index\n");
=======
	printf("    -i <ifname|ifindex> Interface\n");
>>>>>>> upstream/android-13
	printf("    -a <vip-service-address> IPv4 or IPv6\n");
	printf("    -p <vip-service-port> A port range (e.g. 433-444) is also allowed\n");
	printf("    -s <source-ip> Used in the IPTunnel header\n");
	printf("    -d <dest-ip> Used in the IPTunnel header\n");
	printf("    -m <dest-MAC> Used in sending the IP Tunneled pkt\n");
	printf("    -T <stop-after-X-seconds> Default: 0 (forever)\n");
	printf("    -P <IP-Protocol> Default is TCP\n");
	printf("    -S use skb-mode\n");
	printf("    -N enforce native mode\n");
<<<<<<< HEAD
=======
	printf("    -F Force loading the XDP prog\n");
>>>>>>> upstream/android-13
	printf("    -h Display this help\n");
}

static int parse_ipstr(const char *ipstr, unsigned int *addr)
{
	if (inet_pton(AF_INET6, ipstr, addr) == 1) {
		return AF_INET6;
	} else if (inet_pton(AF_INET, ipstr, addr) == 1) {
		addr[1] = addr[2] = addr[3] = 0;
		return AF_INET;
	}

	fprintf(stderr, "%s is an invalid IP\n", ipstr);
	return AF_UNSPEC;
}

static int parse_ports(const char *port_str, int *min_port, int *max_port)
{
	char *end;
	long tmp_min_port;
	long tmp_max_port;

	tmp_min_port = strtol(optarg, &end, 10);
	if (tmp_min_port < 1 || tmp_min_port > 65535) {
		fprintf(stderr, "Invalid port(s):%s\n", optarg);
		return 1;
	}

	if (*end == '-') {
		end++;
		tmp_max_port = strtol(end, NULL, 10);
		if (tmp_max_port < 1 || tmp_max_port > 65535) {
			fprintf(stderr, "Invalid port(s):%s\n", optarg);
			return 1;
		}
	} else {
		tmp_max_port = tmp_min_port;
	}

	if (tmp_min_port > tmp_max_port) {
		fprintf(stderr, "Invalid port(s):%s\n", optarg);
		return 1;
	}

	if (tmp_max_port - tmp_min_port + 1 > MAX_IPTNL_ENTRIES) {
		fprintf(stderr, "Port range (%s) is larger than %u\n",
			port_str, MAX_IPTNL_ENTRIES);
		return 1;
	}
	*min_port = tmp_min_port;
	*max_port = tmp_max_port;

	return 0;
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	unsigned char opt_flags[256] = {};
	unsigned int kill_after_s = 0;
	const char *optstr = "i:a:p:s:d:m:T:P:SNh";
	int min_port = 0, max_port = 0;
	struct iptnl_info tnl = {};
	struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
	struct vip vip = {};
	char filename[256];
	int opt;
	int i;
=======
	struct bpf_prog_load_attr prog_load_attr = {
		.prog_type	= BPF_PROG_TYPE_XDP,
	};
	int min_port = 0, max_port = 0, vip2tnl_map_fd;
	const char *optstr = "i:a:p:s:d:m:T:P:FSNh";
	unsigned char opt_flags[256] = {};
	struct bpf_prog_info info = {};
	__u32 info_len = sizeof(info);
	unsigned int kill_after_s = 0;
	struct iptnl_info tnl = {};
	struct bpf_object *obj;
	struct vip vip = {};
	char filename[256];
	int opt, prog_fd;
	int i, err;
>>>>>>> upstream/android-13

	tnl.family = AF_UNSPEC;
	vip.protocol = IPPROTO_TCP;

	for (i = 0; i < strlen(optstr); i++)
		if (optstr[i] != 'h' && 'a' <= optstr[i] && optstr[i] <= 'z')
			opt_flags[(unsigned char)optstr[i]] = 1;

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		unsigned short family;
		unsigned int *v6;

		switch (opt) {
		case 'i':
<<<<<<< HEAD
			ifindex = atoi(optarg);
=======
			ifindex = if_nametoindex(optarg);
			if (!ifindex)
				ifindex = atoi(optarg);
>>>>>>> upstream/android-13
			break;
		case 'a':
			vip.family = parse_ipstr(optarg, vip.daddr.v6);
			if (vip.family == AF_UNSPEC)
				return 1;
			break;
		case 'p':
			if (parse_ports(optarg, &min_port, &max_port))
				return 1;
			break;
		case 'P':
			vip.protocol = atoi(optarg);
			break;
		case 's':
		case 'd':
			if (opt == 's')
				v6 = tnl.saddr.v6;
			else
				v6 = tnl.daddr.v6;

			family = parse_ipstr(optarg, v6);
			if (family == AF_UNSPEC)
				return 1;
			if (tnl.family == AF_UNSPEC) {
				tnl.family = family;
			} else if (tnl.family != family) {
				fprintf(stderr,
					"The IP version of the src and dst addresses used in the IP encapsulation does not match\n");
				return 1;
			}
			break;
		case 'm':
			if (!ether_aton_r(optarg,
					  (struct ether_addr *)tnl.dmac)) {
				fprintf(stderr, "Invalid mac address:%s\n",
					optarg);
				return 1;
			}
			break;
		case 'T':
			kill_after_s = atoi(optarg);
			break;
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
			usage(argv[0]);
			return 1;
		}
		opt_flags[opt] = 0;
	}

<<<<<<< HEAD
=======
	if (!(xdp_flags & XDP_FLAGS_SKB_MODE))
		xdp_flags |= XDP_FLAGS_DRV_MODE;

>>>>>>> upstream/android-13
	for (i = 0; i < strlen(optstr); i++) {
		if (opt_flags[(unsigned int)optstr[i]]) {
			fprintf(stderr, "Missing argument -%c\n", optstr[i]);
			usage(argv[0]);
			return 1;
		}
	}

<<<<<<< HEAD
	if (setrlimit(RLIMIT_MEMLOCK, &r)) {
		perror("setrlimit(RLIMIT_MEMLOCK, RLIM_INFINITY)");
=======
	if (!ifindex) {
		fprintf(stderr, "Invalid ifname\n");
>>>>>>> upstream/android-13
		return 1;
	}

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
<<<<<<< HEAD

	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return 1;
	}

	if (!prog_fd[0]) {
		printf("load_bpf_file: %s\n", strerror(errno));
=======
	prog_load_attr.file = filename;

	if (bpf_prog_load_xattr(&prog_load_attr, &obj, &prog_fd))
		return 1;

	if (!prog_fd) {
		printf("bpf_prog_load_xattr: %s\n", strerror(errno));
		return 1;
	}

	rxcnt_map_fd = bpf_object__find_map_fd_by_name(obj, "rxcnt");
	vip2tnl_map_fd = bpf_object__find_map_fd_by_name(obj, "vip2tnl");
	if (vip2tnl_map_fd < 0 || rxcnt_map_fd < 0) {
		printf("bpf_object__find_map_fd_by_name failed\n");
>>>>>>> upstream/android-13
		return 1;
	}

	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);

	while (min_port <= max_port) {
		vip.dport = htons(min_port++);
<<<<<<< HEAD
		if (bpf_map_update_elem(map_fd[1], &vip, &tnl, BPF_NOEXIST)) {
=======
		if (bpf_map_update_elem(vip2tnl_map_fd, &vip, &tnl,
					BPF_NOEXIST)) {
>>>>>>> upstream/android-13
			perror("bpf_map_update_elem(&vip2tnl)");
			return 1;
		}
	}

<<<<<<< HEAD
	if (bpf_set_link_xdp_fd(ifindex, prog_fd[0], xdp_flags) < 0) {
=======
	if (bpf_set_link_xdp_fd(ifindex, prog_fd, xdp_flags) < 0) {
>>>>>>> upstream/android-13
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
	poll_stats(kill_after_s);

	bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);

	return 0;
}
