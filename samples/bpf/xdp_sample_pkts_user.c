// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/perf_event.h>
#include <linux/bpf.h>
#include <net/if.h>
#include <errno.h>
#include <assert.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <signal.h>
<<<<<<< HEAD
#include <libbpf.h>
#include <bpf/bpf.h>

#include "perf-sys.h"
#include "trace_helpers.h"

#define MAX_CPUS 128
static int pmu_fds[MAX_CPUS], if_idx;
static struct perf_event_mmap_page *headers[MAX_CPUS];
static char *if_name;

static int do_attach(int idx, int fd, const char *name)
{
	int err;

	err = bpf_set_link_xdp_fd(idx, fd, 0);
	if (err < 0)
		printf("ERROR: failed to attach program to %s\n", name);
=======
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <sys/resource.h>
#include <libgen.h>
#include <linux/if_link.h>

#include "perf-sys.h"

static int if_idx;
static char *if_name;
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
static __u32 prog_id;
static struct perf_buffer *pb = NULL;

static int do_attach(int idx, int fd, const char *name)
{
	struct bpf_prog_info info = {};
	__u32 info_len = sizeof(info);
	int err;

	err = bpf_set_link_xdp_fd(idx, fd, xdp_flags);
	if (err < 0) {
		printf("ERROR: failed to attach program to %s\n", name);
		return err;
	}

	err = bpf_obj_get_info_by_fd(fd, &info, &info_len);
	if (err) {
		printf("can't get prog info - %s\n", strerror(errno));
		return err;
	}
	prog_id = info.id;
>>>>>>> upstream/android-13

	return err;
}

static int do_detach(int idx, const char *name)
{
<<<<<<< HEAD
	int err;

	err = bpf_set_link_xdp_fd(idx, -1, 0);
	if (err < 0)
		printf("ERROR: failed to detach program from %s\n", name);
=======
	__u32 curr_prog_id = 0;
	int err = 0;

	err = bpf_get_link_xdp_id(idx, &curr_prog_id, xdp_flags);
	if (err) {
		printf("bpf_get_link_xdp_id failed\n");
		return err;
	}
	if (prog_id == curr_prog_id) {
		err = bpf_set_link_xdp_fd(idx, -1, xdp_flags);
		if (err < 0)
			printf("ERROR: failed to detach prog from %s\n", name);
	} else if (!curr_prog_id) {
		printf("couldn't find a prog id on a %s\n", name);
	} else {
		printf("program on interface changed, not removing\n");
	}
>>>>>>> upstream/android-13

	return err;
}

#define SAMPLE_SIZE 64

<<<<<<< HEAD
static int print_bpf_output(void *data, int size)
=======
static void print_bpf_output(void *ctx, int cpu, void *data, __u32 size)
>>>>>>> upstream/android-13
{
	struct {
		__u16 cookie;
		__u16 pkt_len;
		__u8  pkt_data[SAMPLE_SIZE];
	} __packed *e = data;
	int i;

	if (e->cookie != 0xdead) {
<<<<<<< HEAD
		printf("BUG cookie %x sized %d\n",
		       e->cookie, size);
		return LIBBPF_PERF_EVENT_ERROR;
=======
		printf("BUG cookie %x sized %d\n", e->cookie, size);
		return;
>>>>>>> upstream/android-13
	}

	printf("Pkt len: %-5d bytes. Ethernet hdr: ", e->pkt_len);
	for (i = 0; i < 14 && i < e->pkt_len; i++)
		printf("%02x ", e->pkt_data[i]);
	printf("\n");
<<<<<<< HEAD

	return LIBBPF_PERF_EVENT_CONT;
}

static void test_bpf_perf_event(int map_fd, int num)
{
	struct perf_event_attr attr = {
		.sample_type = PERF_SAMPLE_RAW,
		.type = PERF_TYPE_SOFTWARE,
		.config = PERF_COUNT_SW_BPF_OUTPUT,
		.wakeup_events = 1, /* get an fd notification for every event */
	};
	int i;

	for (i = 0; i < num; i++) {
		int key = i;

		pmu_fds[i] = sys_perf_event_open(&attr, -1/*pid*/, i/*cpu*/,
						 -1/*group_fd*/, 0);

		assert(pmu_fds[i] >= 0);
		assert(bpf_map_update_elem(map_fd, &key,
					   &pmu_fds[i], BPF_ANY) == 0);
		ioctl(pmu_fds[i], PERF_EVENT_IOC_ENABLE, 0);
	}
=======
>>>>>>> upstream/android-13
}

static void sig_handler(int signo)
{
	do_detach(if_idx, if_name);
<<<<<<< HEAD
	exit(0);
}

=======
	perf_buffer__free(pb);
	exit(0);
}

static void usage(const char *prog)
{
	fprintf(stderr,
		"%s: %s [OPTS] <ifname|ifindex>\n\n"
		"OPTS:\n"
		"    -F    force loading prog\n"
		"    -S    use skb-mode\n",
		__func__, prog);
}

>>>>>>> upstream/android-13
int main(int argc, char **argv)
{
	struct bpf_prog_load_attr prog_load_attr = {
		.prog_type	= BPF_PROG_TYPE_XDP,
	};
<<<<<<< HEAD
	struct bpf_object *obj;
	struct bpf_map *map;
	int prog_fd, map_fd;
	char filename[256];
	int ret, err, i;
	int numcpus;

	if (argc < 2) {
		printf("Usage: %s <ifname>\n", argv[0]);
		return 1;
	}

	numcpus = get_nprocs();
	if (numcpus > MAX_CPUS)
		numcpus = MAX_CPUS;
=======
	struct perf_buffer_opts pb_opts = {};
	const char *optstr = "FS";
	int prog_fd, map_fd, opt;
	struct bpf_object *obj;
	struct bpf_map *map;
	char filename[256];
	int ret, err;

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch (opt) {
		case 'F':
			xdp_flags &= ~XDP_FLAGS_UPDATE_IF_NOEXIST;
			break;
		case 'S':
			xdp_flags |= XDP_FLAGS_SKB_MODE;
			break;
		default:
			usage(basename(argv[0]));
			return 1;
		}
	}

	if (!(xdp_flags & XDP_FLAGS_SKB_MODE))
		xdp_flags |= XDP_FLAGS_DRV_MODE;

	if (optind == argc) {
		usage(basename(argv[0]));
		return 1;
	}
>>>>>>> upstream/android-13

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);
	prog_load_attr.file = filename;

	if (bpf_prog_load_xattr(&prog_load_attr, &obj, &prog_fd))
		return 1;

	if (!prog_fd) {
<<<<<<< HEAD
		printf("load_bpf_file: %s\n", strerror(errno));
=======
		printf("bpf_prog_load_xattr: %s\n", strerror(errno));
>>>>>>> upstream/android-13
		return 1;
	}

	map = bpf_map__next(NULL, obj);
	if (!map) {
		printf("finding a map in obj file failed\n");
		return 1;
	}
	map_fd = bpf_map__fd(map);

<<<<<<< HEAD
	if_idx = if_nametoindex(argv[1]);
	if (!if_idx)
		if_idx = strtoul(argv[1], NULL, 0);
=======
	if_idx = if_nametoindex(argv[optind]);
	if (!if_idx)
		if_idx = strtoul(argv[optind], NULL, 0);
>>>>>>> upstream/android-13

	if (!if_idx) {
		fprintf(stderr, "Invalid ifname\n");
		return 1;
	}
<<<<<<< HEAD
	if_name = argv[1];
	err = do_attach(if_idx, prog_fd, argv[1]);
=======
	if_name = argv[optind];
	err = do_attach(if_idx, prog_fd, if_name);
>>>>>>> upstream/android-13
	if (err)
		return err;

	if (signal(SIGINT, sig_handler) ||
	    signal(SIGHUP, sig_handler) ||
	    signal(SIGTERM, sig_handler)) {
		perror("signal");
		return 1;
	}

<<<<<<< HEAD
	test_bpf_perf_event(map_fd, numcpus);

	for (i = 0; i < numcpus; i++)
		if (perf_event_mmap_header(pmu_fds[i], &headers[i]) < 0)
			return 1;

	ret = perf_event_poller_multi(pmu_fds, headers, numcpus,
				      print_bpf_output);
=======
	pb_opts.sample_cb = print_bpf_output;
	pb = perf_buffer__new(map_fd, 8, &pb_opts);
	err = libbpf_get_error(pb);
	if (err) {
		perror("perf_buffer setup failed");
		return 1;
	}

	while ((ret = perf_buffer__poll(pb, 1000)) >= 0) {
	}

>>>>>>> upstream/android-13
	kill(0, SIGINT);
	return ret;
}
