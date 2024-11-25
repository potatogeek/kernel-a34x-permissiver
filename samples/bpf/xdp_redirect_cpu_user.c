<<<<<<< HEAD
/* GPLv2 Copyright(c) 2017 Jesper Dangaard Brouer, Red Hat, Inc.
 */
static const char *__doc__ =
	" XDP redirect with a CPU-map type \"BPF_MAP_TYPE_CPUMAP\"";
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright(c) 2017 Jesper Dangaard Brouer, Red Hat, Inc.
 */
static const char *__doc__ =
"XDP CPU redirect tool, using BPF_MAP_TYPE_CPUMAP\n"
"Usage: xdp_redirect_cpu -d <IFINDEX|IFNAME> -c 0 ... -c N\n"
"Valid specification for CPUMAP BPF program:\n"
"  --mprog-name/-e pass (use built-in XDP_PASS program)\n"
"  --mprog-name/-e drop (use built-in XDP_DROP program)\n"
"  --redirect-device/-r <ifindex|ifname> (use built-in DEVMAP redirect program)\n"
"  Custom CPUMAP BPF program:\n"
"    --mprog-filename/-f <filename> --mprog-name/-e <program>\n"
"    Optionally, also pass --redirect-map/-m and --redirect-device/-r together\n"
"    to configure DEVMAP in BPF object <filename>\n";
>>>>>>> upstream/android-13

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <sys/resource.h>
<<<<<<< HEAD
#include <getopt.h>
#include <net/if.h>
#include <time.h>

#include <arpa/inet.h>
#include <linux/if_link.h>

#define MAX_CPUS 64 /* WARNING - sync with _kern.c */

/* How many xdp_progs are defined in _kern.c */
#define MAX_PROG 6

/* Wanted to get rid of bpf_load.h and fake-"libbpf.h" (and instead
 * use bpf/libbpf.h), but cannot as (currently) needed for XDP
 * attaching to a device via bpf_set_link_xdp_fd()
 */
#include <bpf/bpf.h>
#include "bpf_load.h"

#include "bpf_util.h"

static int ifindex = -1;
static char ifname_buf[IF_NAMESIZE];
static char *ifname;

static __u32 xdp_flags;

/* Exit return codes */
#define EXIT_OK		0
#define EXIT_FAIL		1
#define EXIT_FAIL_OPTION	2
#define EXIT_FAIL_XDP		3
#define EXIT_FAIL_BPF		4
#define EXIT_FAIL_MEM		5

static const struct option long_options[] = {
	{"help",	no_argument,		NULL, 'h' },
	{"dev",		required_argument,	NULL, 'd' },
	{"skb-mode",	no_argument,		NULL, 'S' },
	{"debug",	no_argument,		NULL, 'D' },
	{"sec",		required_argument,	NULL, 's' },
	{"prognum",	required_argument,	NULL, 'p' },
	{"qsize",	required_argument,	NULL, 'q' },
	{"cpu",		required_argument,	NULL, 'c' },
	{"stress-mode", no_argument,		NULL, 'x' },
	{"no-separators", no_argument,		NULL, 'z' },
	{0, 0, NULL,  0 }
};

static void int_exit(int sig)
{
	fprintf(stderr,
		"Interrupted: Removing XDP program on ifindex:%d device:%s\n",
		ifindex, ifname);
	if (ifindex > -1)
		bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
	exit(EXIT_OK);
}

static void usage(char *argv[])
{
	int i;

	printf("\nDOCUMENTATION:\n%s\n", __doc__);
	printf("\n");
	printf(" Usage: %s (options-see-below)\n", argv[0]);
	printf(" Listing options:\n");
	for (i = 0; long_options[i].name != 0; i++) {
		printf(" --%-12s", long_options[i].name);
		if (long_options[i].flag != NULL)
			printf(" flag (internal value:%d)",
				*long_options[i].flag);
		else
			printf(" short-option: -%c",
				long_options[i].val);
		printf("\n");
	}
	printf("\n");
}

/* gettime returns the current time of day in nanoseconds.
 * Cost: clock_gettime (ns) => 26ns (CLOCK_MONOTONIC)
 *       clock_gettime (ns) =>  9ns (CLOCK_MONOTONIC_COARSE)
 */
#define NANOSEC_PER_SEC 1000000000 /* 10^9 */
static __u64 gettime(void)
{
	struct timespec t;
	int res;

	res = clock_gettime(CLOCK_MONOTONIC, &t);
	if (res < 0) {
		fprintf(stderr, "Error with gettimeofday! (%i)\n", res);
		exit(EXIT_FAIL);
	}
	return (__u64) t.tv_sec * NANOSEC_PER_SEC + t.tv_nsec;
}

/* Common stats data record shared with _kern.c */
struct datarec {
	__u64 processed;
	__u64 dropped;
	__u64 issue;
};
struct record {
	__u64 timestamp;
	struct datarec total;
	struct datarec *cpu;
};
struct stats_record {
	struct record rx_cnt;
	struct record redir_err;
	struct record kthread;
	struct record exception;
	struct record enq[MAX_CPUS];
};

static bool map_collect_percpu(int fd, __u32 key, struct record *rec)
{
	/* For percpu maps, userspace gets a value per possible CPU */
	unsigned int nr_cpus = bpf_num_possible_cpus();
	struct datarec values[nr_cpus];
	__u64 sum_processed = 0;
	__u64 sum_dropped = 0;
	__u64 sum_issue = 0;
	int i;

	if ((bpf_map_lookup_elem(fd, &key, values)) != 0) {
		fprintf(stderr,
			"ERR: bpf_map_lookup_elem failed key:0x%X\n", key);
		return false;
	}
	/* Get time as close as possible to reading map contents */
	rec->timestamp = gettime();

	/* Record and sum values from each CPU */
	for (i = 0; i < nr_cpus; i++) {
		rec->cpu[i].processed = values[i].processed;
		sum_processed        += values[i].processed;
		rec->cpu[i].dropped = values[i].dropped;
		sum_dropped        += values[i].dropped;
		rec->cpu[i].issue = values[i].issue;
		sum_issue        += values[i].issue;
	}
	rec->total.processed = sum_processed;
	rec->total.dropped   = sum_dropped;
	rec->total.issue     = sum_issue;
	return true;
}

static struct datarec *alloc_record_per_cpu(void)
{
	unsigned int nr_cpus = bpf_num_possible_cpus();
	struct datarec *array;
	size_t size;

	size = sizeof(struct datarec) * nr_cpus;
	array = malloc(size);
	memset(array, 0, size);
	if (!array) {
		fprintf(stderr, "Mem alloc error (nr_cpus:%u)\n", nr_cpus);
		exit(EXIT_FAIL_MEM);
	}
	return array;
}

static struct stats_record *alloc_stats_record(void)
{
	struct stats_record *rec;
	int i;

	rec = malloc(sizeof(*rec));
	memset(rec, 0, sizeof(*rec));
	if (!rec) {
		fprintf(stderr, "Mem alloc error\n");
		exit(EXIT_FAIL_MEM);
	}
	rec->rx_cnt.cpu    = alloc_record_per_cpu();
	rec->redir_err.cpu = alloc_record_per_cpu();
	rec->kthread.cpu   = alloc_record_per_cpu();
	rec->exception.cpu = alloc_record_per_cpu();
	for (i = 0; i < MAX_CPUS; i++)
		rec->enq[i].cpu = alloc_record_per_cpu();

	return rec;
}

static void free_stats_record(struct stats_record *r)
{
	int i;

	for (i = 0; i < MAX_CPUS; i++)
		free(r->enq[i].cpu);
	free(r->exception.cpu);
	free(r->kthread.cpu);
	free(r->redir_err.cpu);
	free(r->rx_cnt.cpu);
	free(r);
}

static double calc_period(struct record *r, struct record *p)
{
	double period_ = 0;
	__u64 period = 0;

	period = r->timestamp - p->timestamp;
	if (period > 0)
		period_ = ((double) period / NANOSEC_PER_SEC);

	return period_;
}

static __u64 calc_pps(struct datarec *r, struct datarec *p, double period_)
{
	__u64 packets = 0;
	__u64 pps = 0;

	if (period_ > 0) {
		packets = r->processed - p->processed;
		pps = packets / period_;
	}
	return pps;
}

static __u64 calc_drop_pps(struct datarec *r, struct datarec *p, double period_)
{
	__u64 packets = 0;
	__u64 pps = 0;

	if (period_ > 0) {
		packets = r->dropped - p->dropped;
		pps = packets / period_;
	}
	return pps;
}

static __u64 calc_errs_pps(struct datarec *r,
			    struct datarec *p, double period_)
{
	__u64 packets = 0;
	__u64 pps = 0;

	if (period_ > 0) {
		packets = r->issue - p->issue;
		pps = packets / period_;
	}
	return pps;
}

static void stats_print(struct stats_record *stats_rec,
			struct stats_record *stats_prev,
			int prog_num)
{
	unsigned int nr_cpus = bpf_num_possible_cpus();
	double pps = 0, drop = 0, err = 0;
	struct record *rec, *prev;
	int to_cpu;
	double t;
	int i;

	/* Header */
	printf("Running XDP/eBPF prog_num:%d\n", prog_num);
	printf("%-15s %-7s %-14s %-11s %-9s\n",
	       "XDP-cpumap", "CPU:to", "pps", "drop-pps", "extra-info");

	/* XDP rx_cnt */
	{
		char *fmt_rx = "%-15s %-7d %'-14.0f %'-11.0f %'-10.0f %s\n";
		char *fm2_rx = "%-15s %-7s %'-14.0f %'-11.0f\n";
		char *errstr = "";

		rec  = &stats_rec->rx_cnt;
		prev = &stats_prev->rx_cnt;
		t = calc_period(rec, prev);
		for (i = 0; i < nr_cpus; i++) {
			struct datarec *r = &rec->cpu[i];
			struct datarec *p = &prev->cpu[i];

			pps = calc_pps(r, p, t);
			drop = calc_drop_pps(r, p, t);
			err  = calc_errs_pps(r, p, t);
			if (err > 0)
				errstr = "cpu-dest/err";
			if (pps > 0)
				printf(fmt_rx, "XDP-RX",
					i, pps, drop, err, errstr);
		}
		pps  = calc_pps(&rec->total, &prev->total, t);
		drop = calc_drop_pps(&rec->total, &prev->total, t);
		err  = calc_errs_pps(&rec->total, &prev->total, t);
		printf(fm2_rx, "XDP-RX", "total", pps, drop);
	}

	/* cpumap enqueue stats */
	for (to_cpu = 0; to_cpu < MAX_CPUS; to_cpu++) {
		char *fmt = "%-15s %3d:%-3d %'-14.0f %'-11.0f %'-10.2f %s\n";
		char *fm2 = "%-15s %3s:%-3d %'-14.0f %'-11.0f %'-10.2f %s\n";
		char *errstr = "";

		rec  =  &stats_rec->enq[to_cpu];
		prev = &stats_prev->enq[to_cpu];
		t = calc_period(rec, prev);
		for (i = 0; i < nr_cpus; i++) {
			struct datarec *r = &rec->cpu[i];
			struct datarec *p = &prev->cpu[i];

			pps  = calc_pps(r, p, t);
			drop = calc_drop_pps(r, p, t);
			err  = calc_errs_pps(r, p, t);
			if (err > 0) {
				errstr = "bulk-average";
				err = pps / err; /* calc average bulk size */
			}
			if (pps > 0)
				printf(fmt, "cpumap-enqueue",
				       i, to_cpu, pps, drop, err, errstr);
		}
		pps = calc_pps(&rec->total, &prev->total, t);
		if (pps > 0) {
			drop = calc_drop_pps(&rec->total, &prev->total, t);
			err  = calc_errs_pps(&rec->total, &prev->total, t);
			if (err > 0) {
				errstr = "bulk-average";
				err = pps / err; /* calc average bulk size */
			}
			printf(fm2, "cpumap-enqueue",
			       "sum", to_cpu, pps, drop, err, errstr);
		}
	}

	/* cpumap kthread stats */
	{
		char *fmt_k = "%-15s %-7d %'-14.0f %'-11.0f %'-10.0f %s\n";
		char *fm2_k = "%-15s %-7s %'-14.0f %'-11.0f %'-10.0f %s\n";
		char *e_str = "";

		rec  = &stats_rec->kthread;
		prev = &stats_prev->kthread;
		t = calc_period(rec, prev);
		for (i = 0; i < nr_cpus; i++) {
			struct datarec *r = &rec->cpu[i];
			struct datarec *p = &prev->cpu[i];

			pps  = calc_pps(r, p, t);
			drop = calc_drop_pps(r, p, t);
			err  = calc_errs_pps(r, p, t);
			if (err > 0)
				e_str = "sched";
			if (pps > 0)
				printf(fmt_k, "cpumap_kthread",
				       i, pps, drop, err, e_str);
		}
		pps = calc_pps(&rec->total, &prev->total, t);
		drop = calc_drop_pps(&rec->total, &prev->total, t);
		err  = calc_errs_pps(&rec->total, &prev->total, t);
		if (err > 0)
			e_str = "sched-sum";
		printf(fm2_k, "cpumap_kthread", "total", pps, drop, err, e_str);
	}

	/* XDP redirect err tracepoints (very unlikely) */
	{
		char *fmt_err = "%-15s %-7d %'-14.0f %'-11.0f\n";
		char *fm2_err = "%-15s %-7s %'-14.0f %'-11.0f\n";

		rec  = &stats_rec->redir_err;
		prev = &stats_prev->redir_err;
		t = calc_period(rec, prev);
		for (i = 0; i < nr_cpus; i++) {
			struct datarec *r = &rec->cpu[i];
			struct datarec *p = &prev->cpu[i];

			pps  = calc_pps(r, p, t);
			drop = calc_drop_pps(r, p, t);
			if (pps > 0)
				printf(fmt_err, "redirect_err", i, pps, drop);
		}
		pps = calc_pps(&rec->total, &prev->total, t);
		drop = calc_drop_pps(&rec->total, &prev->total, t);
		printf(fm2_err, "redirect_err", "total", pps, drop);
	}

	/* XDP general exception tracepoints */
	{
		char *fmt_err = "%-15s %-7d %'-14.0f %'-11.0f\n";
		char *fm2_err = "%-15s %-7s %'-14.0f %'-11.0f\n";

		rec  = &stats_rec->exception;
		prev = &stats_prev->exception;
		t = calc_period(rec, prev);
		for (i = 0; i < nr_cpus; i++) {
			struct datarec *r = &rec->cpu[i];
			struct datarec *p = &prev->cpu[i];

			pps  = calc_pps(r, p, t);
			drop = calc_drop_pps(r, p, t);
			if (pps > 0)
				printf(fmt_err, "xdp_exception", i, pps, drop);
		}
		pps = calc_pps(&rec->total, &prev->total, t);
		drop = calc_drop_pps(&rec->total, &prev->total, t);
		printf(fm2_err, "xdp_exception", "total", pps, drop);
	}

	printf("\n");
	fflush(stdout);
}

static void stats_collect(struct stats_record *rec)
{
	int fd, i;

	fd = map_fd[1]; /* map: rx_cnt */
	map_collect_percpu(fd, 0, &rec->rx_cnt);

	fd = map_fd[2]; /* map: redirect_err_cnt */
	map_collect_percpu(fd, 1, &rec->redir_err);

	fd = map_fd[3]; /* map: cpumap_enqueue_cnt */
	for (i = 0; i < MAX_CPUS; i++)
		map_collect_percpu(fd, i, &rec->enq[i]);

	fd = map_fd[4]; /* map: cpumap_kthread_cnt */
	map_collect_percpu(fd, 0, &rec->kthread);

	fd = map_fd[8]; /* map: exception_cnt */
	map_collect_percpu(fd, 0, &rec->exception);
}


/* Pointer swap trick */
static inline void swap(struct stats_record **a, struct stats_record **b)
{
	struct stats_record *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int create_cpu_entry(__u32 cpu, __u32 queue_size,
=======
#include <sys/sysinfo.h>
#include <getopt.h>
#include <net/if.h>
#include <time.h>
#include <linux/limits.h>
#include <arpa/inet.h>
#include <linux/if_link.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include "bpf_util.h"
#include "xdp_sample_user.h"
#include "xdp_redirect_cpu.skel.h"

static int map_fd;
static int avail_fd;
static int count_fd;

static int mask = SAMPLE_RX_CNT | SAMPLE_REDIRECT_ERR_MAP_CNT |
		  SAMPLE_CPUMAP_ENQUEUE_CNT | SAMPLE_CPUMAP_KTHREAD_CNT |
		  SAMPLE_EXCEPTION_CNT;

DEFINE_SAMPLE_INIT(xdp_redirect_cpu);

static const struct option long_options[] = {
	{ "help", no_argument, NULL, 'h' },
	{ "dev", required_argument, NULL, 'd' },
	{ "skb-mode", no_argument, NULL, 'S' },
	{ "progname", required_argument, NULL, 'p' },
	{ "qsize", required_argument, NULL, 'q' },
	{ "cpu", required_argument, NULL, 'c' },
	{ "stress-mode", no_argument, NULL, 'x' },
	{ "force", no_argument, NULL, 'F' },
	{ "interval", required_argument, NULL, 'i' },
	{ "verbose", no_argument, NULL, 'v' },
	{ "stats", no_argument, NULL, 's' },
	{ "mprog-name", required_argument, NULL, 'e' },
	{ "mprog-filename", required_argument, NULL, 'f' },
	{ "redirect-device", required_argument, NULL, 'r' },
	{ "redirect-map", required_argument, NULL, 'm' },
	{}
};

static void print_avail_progs(struct bpf_object *obj)
{
	struct bpf_program *pos;

	printf(" Programs to be used for -p/--progname:\n");
	bpf_object__for_each_program(pos, obj) {
		if (bpf_program__is_xdp(pos)) {
			if (!strncmp(bpf_program__name(pos), "xdp_prognum",
				     sizeof("xdp_prognum") - 1))
				printf(" %s\n", bpf_program__name(pos));
		}
	}
}

static void usage(char *argv[], const struct option *long_options,
		  const char *doc, int mask, bool error, struct bpf_object *obj)
{
	sample_usage(argv, long_options, doc, mask, error);
	print_avail_progs(obj);
}

static int create_cpu_entry(__u32 cpu, struct bpf_cpumap_val *value,
>>>>>>> upstream/android-13
			    __u32 avail_idx, bool new)
{
	__u32 curr_cpus_count = 0;
	__u32 key = 0;
	int ret;

	/* Add a CPU entry to cpumap, as this allocate a cpu entry in
	 * the kernel for the cpu.
	 */
<<<<<<< HEAD
	ret = bpf_map_update_elem(map_fd[0], &cpu, &queue_size, 0);
	if (ret) {
		fprintf(stderr, "Create CPU entry failed (err:%d)\n", ret);
		exit(EXIT_FAIL_BPF);
=======
	ret = bpf_map_update_elem(map_fd, &cpu, value, 0);
	if (ret < 0) {
		fprintf(stderr, "Create CPU entry failed: %s\n", strerror(errno));
		return ret;
>>>>>>> upstream/android-13
	}

	/* Inform bpf_prog's that a new CPU is available to select
	 * from via some control maps.
	 */
<<<<<<< HEAD
	/* map_fd[5] = cpus_available */
	ret = bpf_map_update_elem(map_fd[5], &avail_idx, &cpu, 0);
	if (ret) {
		fprintf(stderr, "Add to avail CPUs failed\n");
		exit(EXIT_FAIL_BPF);
	}

	/* When not replacing/updating existing entry, bump the count */
	/* map_fd[6] = cpus_count */
	ret = bpf_map_lookup_elem(map_fd[6], &key, &curr_cpus_count);
	if (ret) {
		fprintf(stderr, "Failed reading curr cpus_count\n");
		exit(EXIT_FAIL_BPF);
	}
	if (new) {
		curr_cpus_count++;
		ret = bpf_map_update_elem(map_fd[6], &key, &curr_cpus_count, 0);
		if (ret) {
			fprintf(stderr, "Failed write curr cpus_count\n");
			exit(EXIT_FAIL_BPF);
		}
	}
	/* map_fd[7] = cpus_iterator */
	printf("%s CPU:%u as idx:%u queue_size:%d (total cpus_count:%u)\n",
	       new ? "Add-new":"Replace", cpu, avail_idx,
	       queue_size, curr_cpus_count);
=======
	ret = bpf_map_update_elem(avail_fd, &avail_idx, &cpu, 0);
	if (ret < 0) {
		fprintf(stderr, "Add to avail CPUs failed: %s\n", strerror(errno));
		return ret;
	}

	/* When not replacing/updating existing entry, bump the count */
	ret = bpf_map_lookup_elem(count_fd, &key, &curr_cpus_count);
	if (ret < 0) {
		fprintf(stderr, "Failed reading curr cpus_count: %s\n",
			strerror(errno));
		return ret;
	}
	if (new) {
		curr_cpus_count++;
		ret = bpf_map_update_elem(count_fd, &key,
					  &curr_cpus_count, 0);
		if (ret < 0) {
			fprintf(stderr, "Failed write curr cpus_count: %s\n",
				strerror(errno));
			return ret;
		}
	}

	printf("%s CPU: %u as idx: %u qsize: %d cpumap_prog_fd: %d (cpus_count: %u)\n",
	       new ? "Add new" : "Replace", cpu, avail_idx,
	       value->qsize, value->bpf_prog.fd, curr_cpus_count);
>>>>>>> upstream/android-13

	return 0;
}

/* CPUs are zero-indexed. Thus, add a special sentinel default value
 * in map cpus_available to mark CPU index'es not configured
 */
<<<<<<< HEAD
static void mark_cpus_unavailable(void)
{
	__u32 invalid_cpu = MAX_CPUS;
	int ret, i;

	for (i = 0; i < MAX_CPUS; i++) {
		/* map_fd[5] = cpus_available */
		ret = bpf_map_update_elem(map_fd[5], &i, &invalid_cpu, 0);
		if (ret) {
			fprintf(stderr, "Failed marking CPU unavailable\n");
			exit(EXIT_FAIL_BPF);
		}
	}
}

/* Stress cpumap management code by concurrently changing underlying cpumap */
static void stress_cpumap(void)
{
=======
static int mark_cpus_unavailable(void)
{
	int ret, i, n_cpus = libbpf_num_possible_cpus();
	__u32 invalid_cpu = n_cpus;

	for (i = 0; i < n_cpus; i++) {
		ret = bpf_map_update_elem(avail_fd, &i,
					  &invalid_cpu, 0);
		if (ret < 0) {
			fprintf(stderr, "Failed marking CPU unavailable: %s\n",
				strerror(errno));
			return ret;
		}
	}

	return 0;
}

/* Stress cpumap management code by concurrently changing underlying cpumap */
static void stress_cpumap(void *ctx)
{
	struct bpf_cpumap_val *value = ctx;

>>>>>>> upstream/android-13
	/* Changing qsize will cause kernel to free and alloc a new
	 * bpf_cpu_map_entry, with an associated/complicated tear-down
	 * procedure.
	 */
<<<<<<< HEAD
	create_cpu_entry(1,  1024, 0, false);
	create_cpu_entry(1,     8, 0, false);
	create_cpu_entry(1, 16000, 0, false);
}

static void stats_poll(int interval, bool use_separators, int prog_num,
		       bool stress_mode)
{
	struct stats_record *record, *prev;

	record = alloc_stats_record();
	prev   = alloc_stats_record();
	stats_collect(record);

	/* Trick to pretty printf with thousands separators use %' */
	if (use_separators)
		setlocale(LC_NUMERIC, "en_US");

	while (1) {
		swap(&prev, &record);
		stats_collect(record);
		stats_print(record, prev, prog_num);
		sleep(interval);
		if (stress_mode)
			stress_cpumap();
	}

	free_stats_record(record);
	free_stats_record(prev);
=======
	value->qsize = 1024;
	create_cpu_entry(1, value, 0, false);
	value->qsize = 8;
	create_cpu_entry(1, value, 0, false);
	value->qsize = 16000;
	create_cpu_entry(1, value, 0, false);
}

static int set_cpumap_prog(struct xdp_redirect_cpu *skel,
			   const char *redir_interface, const char *redir_map,
			   const char *mprog_filename, const char *mprog_name)
{
	if (mprog_filename) {
		struct bpf_program *prog;
		struct bpf_object *obj;
		int ret;

		if (!mprog_name) {
			fprintf(stderr, "BPF program not specified for file %s\n",
				mprog_filename);
			goto end;
		}
		if ((redir_interface && !redir_map) || (!redir_interface && redir_map)) {
			fprintf(stderr, "--redirect-%s specified but --redirect-%s not specified\n",
				redir_interface ? "device" : "map", redir_interface ? "map" : "device");
			goto end;
		}

		/* Custom BPF program */
		obj = bpf_object__open_file(mprog_filename, NULL);
		if (!obj) {
			ret = -errno;
			fprintf(stderr, "Failed to bpf_prog_load_xattr: %s\n",
				strerror(errno));
			return ret;
		}

		ret = bpf_object__load(obj);
		if (ret < 0) {
			ret = -errno;
			fprintf(stderr, "Failed to bpf_object__load: %s\n",
				strerror(errno));
			return ret;
		}

		if (redir_map) {
			int err, redir_map_fd, ifindex_out, key = 0;

			redir_map_fd = bpf_object__find_map_fd_by_name(obj, redir_map);
			if (redir_map_fd < 0) {
				fprintf(stderr, "Failed to bpf_object__find_map_fd_by_name: %s\n",
					strerror(errno));
				return redir_map_fd;
			}

			ifindex_out = if_nametoindex(redir_interface);
			if (!ifindex_out)
				ifindex_out = strtoul(redir_interface, NULL, 0);
			if (!ifindex_out) {
				fprintf(stderr, "Bad interface name or index\n");
				return -EINVAL;
			}

			err = bpf_map_update_elem(redir_map_fd, &key, &ifindex_out, 0);
			if (err < 0)
				return err;
		}

		prog = bpf_object__find_program_by_name(obj, mprog_name);
		if (!prog) {
			ret = -errno;
			fprintf(stderr, "Failed to bpf_object__find_program_by_name: %s\n",
				strerror(errno));
			return ret;
		}

		return bpf_program__fd(prog);
	} else {
		if (mprog_name) {
			if (redir_interface || redir_map) {
				fprintf(stderr, "Need to specify --mprog-filename/-f\n");
				goto end;
			}
			if (!strcmp(mprog_name, "pass") || !strcmp(mprog_name, "drop")) {
				/* Use built-in pass/drop programs */
				return *mprog_name == 'p' ? bpf_program__fd(skel->progs.xdp_redirect_cpu_pass)
					: bpf_program__fd(skel->progs.xdp_redirect_cpu_drop);
			} else {
				fprintf(stderr, "Unknown name \"%s\" for built-in BPF program\n",
					mprog_name);
				goto end;
			}
		} else {
			if (redir_map) {
				fprintf(stderr, "Need to specify --mprog-filename, --mprog-name and"
					" --redirect-device with --redirect-map\n");
				goto end;
			}
			if (redir_interface) {
				/* Use built-in devmap redirect */
				struct bpf_devmap_val val = {};
				int ifindex_out, err;
				__u32 key = 0;

				if (!redir_interface)
					return 0;

				ifindex_out = if_nametoindex(redir_interface);
				if (!ifindex_out)
					ifindex_out = strtoul(redir_interface, NULL, 0);
				if (!ifindex_out) {
					fprintf(stderr, "Bad interface name or index\n");
					return -EINVAL;
				}

				if (get_mac_addr(ifindex_out, skel->bss->tx_mac_addr) < 0) {
					printf("Get interface %d mac failed\n", ifindex_out);
					return -EINVAL;
				}

				val.ifindex = ifindex_out;
				val.bpf_prog.fd = bpf_program__fd(skel->progs.xdp_redirect_egress_prog);
				err = bpf_map_update_elem(bpf_map__fd(skel->maps.tx_port), &key, &val, 0);
				if (err < 0)
					return -errno;

				return bpf_program__fd(skel->progs.xdp_redirect_cpu_devmap);
			}
		}
	}

	/* Disabled */
	return 0;
end:
	fprintf(stderr, "Invalid options for CPUMAP BPF program\n");
	return -EINVAL;
>>>>>>> upstream/android-13
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
	struct rlimit r = {10 * 1024 * 1024, RLIM_INFINITY};
	bool use_separators = true;
	bool stress_mode = false;
	char filename[256];
	bool debug = false;
	int added_cpus = 0;
	int longindex = 0;
	int interval = 2;
	int prog_num = 5;
	int add_cpu = -1;
	__u32 qsize;
	int opt;

	/* Notice: choosing he queue size is very important with the
	 * ixgbe driver, because it's driver page recycling trick is
	 * dependend on pages being returned quickly.  The number of
	 * out-standing packets in the system must be less-than 2x
	 * RX-ring size.
	 */
	qsize = 128+64;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

	if (setrlimit(RLIMIT_MEMLOCK, &r)) {
		perror("setrlimit(RLIMIT_MEMLOCK)");
		return 1;
	}

	if (load_bpf_file(filename)) {
		fprintf(stderr, "ERR in load_bpf_file(): %s", bpf_log_buf);
		return EXIT_FAIL;
	}

	if (!prog_fd[0]) {
		fprintf(stderr, "ERR: load_bpf_file: %s\n", strerror(errno));
		return EXIT_FAIL;
	}

	mark_cpus_unavailable();

	/* Parse commands line args */
	while ((opt = getopt_long(argc, argv, "hSd:",
=======
	const char *redir_interface = NULL, *redir_map = NULL;
	const char *mprog_filename = NULL, *mprog_name = NULL;
	struct xdp_redirect_cpu *skel;
	struct bpf_map_info info = {};
	struct bpf_cpumap_val value;
	__u32 infosz = sizeof(info);
	int ret = EXIT_FAIL_OPTION;
	unsigned long interval = 2;
	bool stress_mode = false;
	struct bpf_program *prog;
	const char *prog_name;
	bool generic = false;
	bool force = false;
	int added_cpus = 0;
	bool error = true;
	int longindex = 0;
	int add_cpu = -1;
	int ifindex = -1;
	int *cpu, i, opt;
	__u32 qsize;
	int n_cpus;

	n_cpus = libbpf_num_possible_cpus();

	/* Notice: Choosing the queue size is very important when CPU is
	 * configured with power-saving states.
	 *
	 * If deepest state take 133 usec to wakeup from (133/10^6). When link
	 * speed is 10Gbit/s ((10*10^9/8) in bytes/sec). How many bytes can
	 * arrive with in 133 usec at this speed: (10*10^9/8)*(133/10^6) =
	 * 166250 bytes. With MTU size packets this is 110 packets, and with
	 * minimum Ethernet (MAC-preamble + intergap) 84 bytes is 1979 packets.
	 *
	 * Setting default cpumap queue to 2048 as worst-case (small packet)
	 * should be +64 packet due kthread wakeup call (due to xdp_do_flush)
	 * worst-case is 2043 packets.
	 *
	 * Sysadm can configured system to avoid deep-sleep via:
	 *   tuned-adm profile network-latency
	 */
	qsize = 2048;

	skel = xdp_redirect_cpu__open();
	if (!skel) {
		fprintf(stderr, "Failed to xdp_redirect_cpu__open: %s\n",
			strerror(errno));
		ret = EXIT_FAIL_BPF;
		goto end;
	}

	ret = sample_init_pre_load(skel);
	if (ret < 0) {
		fprintf(stderr, "Failed to sample_init_pre_load: %s\n", strerror(-ret));
		ret = EXIT_FAIL_BPF;
		goto end_destroy;
	}

	if (bpf_map__set_max_entries(skel->maps.cpu_map, n_cpus) < 0) {
		fprintf(stderr, "Failed to set max entries for cpu_map map: %s",
			strerror(errno));
		ret = EXIT_FAIL_BPF;
		goto end_destroy;
	}

	if (bpf_map__set_max_entries(skel->maps.cpus_available, n_cpus) < 0) {
		fprintf(stderr, "Failed to set max entries for cpus_available map: %s",
			strerror(errno));
		ret = EXIT_FAIL_BPF;
		goto end_destroy;
	}

	cpu = calloc(n_cpus, sizeof(int));
	if (!cpu) {
		fprintf(stderr, "Failed to allocate cpu array\n");
		goto end_destroy;
	}

	prog = skel->progs.xdp_prognum5_lb_hash_ip_pairs;
	while ((opt = getopt_long(argc, argv, "d:si:Sxp:f:e:r:m:c:q:Fvh",
>>>>>>> upstream/android-13
				  long_options, &longindex)) != -1) {
		switch (opt) {
		case 'd':
			if (strlen(optarg) >= IF_NAMESIZE) {
<<<<<<< HEAD
				fprintf(stderr, "ERR: --dev name too long\n");
				goto error;
			}
			ifname = (char *)&ifname_buf;
			strncpy(ifname, optarg, IF_NAMESIZE);
			ifindex = if_nametoindex(ifname);
			if (ifindex == 0) {
				fprintf(stderr,
					"ERR: --dev name unknown err(%d):%s\n",
					errno, strerror(errno));
				goto error;
			}
			break;
		case 's':
			interval = atoi(optarg);
			break;
		case 'S':
			xdp_flags |= XDP_FLAGS_SKB_MODE;
			break;
		case 'D':
			debug = true;
=======
				fprintf(stderr, "-d/--dev name too long\n");
				usage(argv, long_options, __doc__, mask, true, skel->obj);
				goto end_cpu;
			}
			ifindex = if_nametoindex(optarg);
			if (!ifindex)
				ifindex = strtoul(optarg, NULL, 0);
			if (!ifindex) {
				fprintf(stderr, "Bad interface index or name (%d): %s\n",
					errno, strerror(errno));
				usage(argv, long_options, __doc__, mask, true, skel->obj);
				goto end_cpu;
			}
			break;
		case 's':
			mask |= SAMPLE_REDIRECT_MAP_CNT;
			break;
		case 'i':
			interval = strtoul(optarg, NULL, 0);
			break;
		case 'S':
			generic = true;
>>>>>>> upstream/android-13
			break;
		case 'x':
			stress_mode = true;
			break;
<<<<<<< HEAD
		case 'z':
			use_separators = false;
			break;
		case 'p':
			/* Selecting eBPF prog to load */
			prog_num = atoi(optarg);
			if (prog_num < 0 || prog_num >= MAX_PROG) {
				fprintf(stderr,
					"--prognum too large err(%d):%s\n",
					errno, strerror(errno));
				goto error;
			}
			break;
		case 'c':
			/* Add multiple CPUs */
			add_cpu = strtoul(optarg, NULL, 0);
			if (add_cpu >= MAX_CPUS) {
				fprintf(stderr,
				"--cpu nr too large for cpumap err(%d):%s\n",
					errno, strerror(errno));
				goto error;
			}
			create_cpu_entry(add_cpu, qsize, added_cpus, true);
			added_cpus++;
			break;
		case 'q':
			qsize = atoi(optarg);
			break;
		case 'h':
		error:
		default:
			usage(argv);
			return EXIT_FAIL_OPTION;
		}
	}
	/* Required option */
	if (ifindex == -1) {
		fprintf(stderr, "ERR: required option --dev missing\n");
		usage(argv);
		return EXIT_FAIL_OPTION;
	}
	/* Required option */
	if (add_cpu == -1) {
		fprintf(stderr, "ERR: required option --cpu missing\n");
		fprintf(stderr, " Specify multiple --cpu option to add more\n");
		usage(argv);
		return EXIT_FAIL_OPTION;
	}

	/* Remove XDP program when program is interrupted or killed */
	signal(SIGINT, int_exit);
	signal(SIGTERM, int_exit);

	if (bpf_set_link_xdp_fd(ifindex, prog_fd[prog_num], xdp_flags) < 0) {
		fprintf(stderr, "link set xdp fd failed\n");
		return EXIT_FAIL_XDP;
	}

	if (debug) {
		printf("Debug-mode reading trace pipe (fix #define DEBUG)\n");
		read_trace_pipe();
	}

	stats_poll(interval, use_separators, prog_num, stress_mode);
	return EXIT_OK;
=======
		case 'p':
			/* Selecting eBPF prog to load */
			prog_name = optarg;
			prog = bpf_object__find_program_by_name(skel->obj,
								prog_name);
			if (!prog) {
				fprintf(stderr,
					"Failed to find program %s specified by"
					" option -p/--progname\n",
					prog_name);
				print_avail_progs(skel->obj);
				goto end_cpu;
			}
			break;
		case 'f':
			mprog_filename = optarg;
			break;
		case 'e':
			mprog_name = optarg;
			break;
		case 'r':
			redir_interface = optarg;
			mask |= SAMPLE_DEVMAP_XMIT_CNT_MULTI;
			break;
		case 'm':
			redir_map = optarg;
			break;
		case 'c':
			/* Add multiple CPUs */
			add_cpu = strtoul(optarg, NULL, 0);
			if (add_cpu >= n_cpus) {
				fprintf(stderr,
				"--cpu nr too large for cpumap err (%d):%s\n",
					errno, strerror(errno));
				usage(argv, long_options, __doc__, mask, true, skel->obj);
				goto end_cpu;
			}
			cpu[added_cpus++] = add_cpu;
			break;
		case 'q':
			qsize = strtoul(optarg, NULL, 0);
			break;
		case 'F':
			force = true;
			break;
		case 'v':
			sample_switch_mode();
			break;
		case 'h':
			error = false;
		default:
			usage(argv, long_options, __doc__, mask, error, skel->obj);
			goto end_cpu;
		}
	}

	ret = EXIT_FAIL_OPTION;
	if (ifindex == -1) {
		fprintf(stderr, "Required option --dev missing\n");
		usage(argv, long_options, __doc__, mask, true, skel->obj);
		goto end_cpu;
	}

	if (add_cpu == -1) {
		fprintf(stderr, "Required option --cpu missing\n"
				"Specify multiple --cpu option to add more\n");
		usage(argv, long_options, __doc__, mask, true, skel->obj);
		goto end_cpu;
	}

	skel->rodata->from_match[0] = ifindex;
	if (redir_interface)
		skel->rodata->to_match[0] = if_nametoindex(redir_interface);

	ret = xdp_redirect_cpu__load(skel);
	if (ret < 0) {
		fprintf(stderr, "Failed to xdp_redirect_cpu__load: %s\n",
			strerror(errno));
		goto end_cpu;
	}

	ret = bpf_obj_get_info_by_fd(bpf_map__fd(skel->maps.cpu_map), &info, &infosz);
	if (ret < 0) {
		fprintf(stderr, "Failed bpf_obj_get_info_by_fd for cpumap: %s\n",
			strerror(errno));
		goto end_cpu;
	}

	skel->bss->cpumap_map_id = info.id;

	map_fd = bpf_map__fd(skel->maps.cpu_map);
	avail_fd = bpf_map__fd(skel->maps.cpus_available);
	count_fd = bpf_map__fd(skel->maps.cpus_count);

	ret = mark_cpus_unavailable();
	if (ret < 0) {
		fprintf(stderr, "Unable to mark CPUs as unavailable\n");
		goto end_cpu;
	}

	ret = sample_init(skel, mask);
	if (ret < 0) {
		fprintf(stderr, "Failed to initialize sample: %s\n", strerror(-ret));
		ret = EXIT_FAIL;
		goto end_cpu;
	}

	value.bpf_prog.fd = set_cpumap_prog(skel, redir_interface, redir_map,
					    mprog_filename, mprog_name);
	if (value.bpf_prog.fd < 0) {
		fprintf(stderr, "Failed to set CPUMAP BPF program: %s\n",
			strerror(-value.bpf_prog.fd));
		usage(argv, long_options, __doc__, mask, true, skel->obj);
		ret = EXIT_FAIL_BPF;
		goto end_cpu;
	}
	value.qsize = qsize;

	for (i = 0; i < added_cpus; i++) {
		if (create_cpu_entry(cpu[i], &value, i, true) < 0) {
			fprintf(stderr, "Cannot proceed, exiting\n");
			usage(argv, long_options, __doc__, mask, true, skel->obj);
			goto end_cpu;
		}
	}

	ret = EXIT_FAIL_XDP;
	if (sample_install_xdp(prog, ifindex, generic, force) < 0)
		goto end_cpu;

	ret = sample_run(interval, stress_mode ? stress_cpumap : NULL, &value);
	if (ret < 0) {
		fprintf(stderr, "Failed during sample run: %s\n", strerror(-ret));
		ret = EXIT_FAIL;
		goto end_cpu;
	}
	ret = EXIT_OK;
end_cpu:
	free(cpu);
end_destroy:
	xdp_redirect_cpu__destroy(skel);
end:
	sample_exit(ret);
>>>>>>> upstream/android-13
}
