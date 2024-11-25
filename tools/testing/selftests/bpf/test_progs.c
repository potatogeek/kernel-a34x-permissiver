<<<<<<< HEAD
/* Copyright (c) 2017 Facebook
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 */
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <linux/types.h>
typedef __u16 __sum16;
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/filter.h>
#include <linux/perf_event.h>
#include <linux/unistd.h>

#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <linux/bpf.h>
#include <linux/err.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#include "test_iptunnel_common.h"
#include "bpf_util.h"
#include "bpf_endian.h"
#include "bpf_rlimit.h"
#include "trace_helpers.h"

static int error_cnt, pass_cnt;
static bool jit_enabled;

#define MAGIC_BYTES 123

/* ipv4 test vector */
static struct {
	struct ethhdr eth;
	struct iphdr iph;
	struct tcphdr tcp;
} __packed pkt_v4 = {
	.eth.h_proto = __bpf_constant_htons(ETH_P_IP),
	.iph.ihl = 5,
	.iph.protocol = 6,
	.iph.tot_len = __bpf_constant_htons(MAGIC_BYTES),
	.tcp.urg_ptr = 123,
};

/* ipv6 test vector */
static struct {
	struct ethhdr eth;
	struct ipv6hdr iph;
	struct tcphdr tcp;
} __packed pkt_v6 = {
	.eth.h_proto = __bpf_constant_htons(ETH_P_IPV6),
	.iph.nexthdr = 6,
	.iph.payload_len = __bpf_constant_htons(MAGIC_BYTES),
	.tcp.urg_ptr = 123,
};

#define CHECK(condition, tag, format...) ({				\
	int __ret = !!(condition);					\
	if (__ret) {							\
		error_cnt++;						\
		printf("%s:FAIL:%s ", __func__, tag);			\
		printf(format);						\
	} else {							\
		pass_cnt++;						\
		printf("%s:PASS:%s %d nsec\n", __func__, tag, duration);\
	}								\
	__ret;								\
})

static int bpf_find_map(const char *test, struct bpf_object *obj,
			const char *name)
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2017 Facebook
 */
#define _GNU_SOURCE
#include "test_progs.h"
#include "cgroup_helpers.h"
#include "bpf_rlimit.h"
#include <argp.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <string.h>
#include <execinfo.h> /* backtrace */
#include <linux/membarrier.h>

/* Adapted from perf/util/string.c */
static bool glob_match(const char *str, const char *pat)
{
	while (*str && *pat && *pat != '*') {
		if (*str != *pat)
			return false;
		str++;
		pat++;
	}
	/* Check wild card */
	if (*pat == '*') {
		while (*pat == '*')
			pat++;
		if (!*pat) /* Tail wild card matches all */
			return true;
		while (*str)
			if (glob_match(str++, pat))
				return true;
	}
	return !*str && !*pat;
}

#define EXIT_NO_TEST		2
#define EXIT_ERR_SETUP_INFRA	3

/* defined in test_progs.h */
struct test_env env = {};

struct prog_test_def {
	const char *test_name;
	int test_num;
	void (*run_test)(void);
	bool force_log;
	int error_cnt;
	int skip_cnt;
	bool tested;
	bool need_cgroup_cleanup;

	char *subtest_name;
	int subtest_num;

	/* store counts before subtest started */
	int old_error_cnt;
};

/* Override C runtime library's usleep() implementation to ensure nanosleep()
 * is always called. Usleep is frequently used in selftests as a way to
 * trigger kprobe and tracepoints.
 */
int usleep(useconds_t usec)
{
	struct timespec ts = {
		.tv_sec = usec / 1000000,
		.tv_nsec = (usec % 1000000) * 1000,
	};

	return syscall(__NR_nanosleep, &ts, NULL);
}

static bool should_run(struct test_selector *sel, int num, const char *name)
{
	int i;

	for (i = 0; i < sel->blacklist.cnt; i++) {
		if (glob_match(name, sel->blacklist.strs[i]))
			return false;
	}

	for (i = 0; i < sel->whitelist.cnt; i++) {
		if (glob_match(name, sel->whitelist.strs[i]))
			return true;
	}

	if (!sel->whitelist.cnt && !sel->num_set)
		return true;

	return num < sel->num_set_len && sel->num_set[num];
}

static void dump_test_log(const struct prog_test_def *test, bool failed)
{
	if (stdout == env.stdout)
		return;

	fflush(stdout); /* exports env.log_buf & env.log_cnt */

	if (env.verbosity > VERBOSE_NONE || test->force_log || failed) {
		if (env.log_cnt) {
			env.log_buf[env.log_cnt] = '\0';
			fprintf(env.stdout, "%s", env.log_buf);
			if (env.log_buf[env.log_cnt - 1] != '\n')
				fprintf(env.stdout, "\n");
		}
	}

	fseeko(stdout, 0, SEEK_SET); /* rewind */
}

static void skip_account(void)
{
	if (env.test->skip_cnt) {
		env.skip_cnt++;
		env.test->skip_cnt = 0;
	}
}

static void stdio_restore(void);

/* A bunch of tests set custom affinity per-thread and/or per-process. Reset
 * it after each test/sub-test.
 */
static void reset_affinity() {

	cpu_set_t cpuset;
	int i, err;

	CPU_ZERO(&cpuset);
	for (i = 0; i < env.nr_cpus; i++)
		CPU_SET(i, &cpuset);

	err = sched_setaffinity(0, sizeof(cpuset), &cpuset);
	if (err < 0) {
		stdio_restore();
		fprintf(stderr, "Failed to reset process affinity: %d!\n", err);
		exit(EXIT_ERR_SETUP_INFRA);
	}
	err = pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
	if (err < 0) {
		stdio_restore();
		fprintf(stderr, "Failed to reset thread affinity: %d!\n", err);
		exit(EXIT_ERR_SETUP_INFRA);
	}
}

static void save_netns(void)
{
	env.saved_netns_fd = open("/proc/self/ns/net", O_RDONLY);
	if (env.saved_netns_fd == -1) {
		perror("open(/proc/self/ns/net)");
		exit(EXIT_ERR_SETUP_INFRA);
	}
}

static void restore_netns(void)
{
	if (setns(env.saved_netns_fd, CLONE_NEWNET) == -1) {
		stdio_restore();
		perror("setns(CLONE_NEWNS)");
		exit(EXIT_ERR_SETUP_INFRA);
	}
}

void test__end_subtest()
{
	struct prog_test_def *test = env.test;
	int sub_error_cnt = test->error_cnt - test->old_error_cnt;

	dump_test_log(test, sub_error_cnt);

	fprintf(env.stdout, "#%d/%d %s/%s:%s\n",
	       test->test_num, test->subtest_num, test->test_name, test->subtest_name,
	       sub_error_cnt ? "FAIL" : (test->skip_cnt ? "SKIP" : "OK"));

	if (sub_error_cnt)
		env.fail_cnt++;
	else if (test->skip_cnt == 0)
		env.sub_succ_cnt++;
	skip_account();

	free(test->subtest_name);
	test->subtest_name = NULL;
}

bool test__start_subtest(const char *name)
{
	struct prog_test_def *test = env.test;

	if (test->subtest_name)
		test__end_subtest();

	test->subtest_num++;

	if (!name || !name[0]) {
		fprintf(env.stderr,
			"Subtest #%d didn't provide sub-test name!\n",
			test->subtest_num);
		return false;
	}

	if (!should_run(&env.subtest_selector, test->subtest_num, name))
		return false;

	test->subtest_name = strdup(name);
	if (!test->subtest_name) {
		fprintf(env.stderr,
			"Subtest #%d: failed to copy subtest name!\n",
			test->subtest_num);
		return false;
	}
	env.test->old_error_cnt = env.test->error_cnt;

	return true;
}

void test__force_log() {
	env.test->force_log = true;
}

void test__skip(void)
{
	env.test->skip_cnt++;
}

void test__fail(void)
{
	env.test->error_cnt++;
}

int test__join_cgroup(const char *path)
{
	int fd;

	if (!env.test->need_cgroup_cleanup) {
		if (setup_cgroup_environment()) {
			fprintf(stderr,
				"#%d %s: Failed to setup cgroup environment\n",
				env.test->test_num, env.test->test_name);
			return -1;
		}

		env.test->need_cgroup_cleanup = true;
	}

	fd = create_and_get_cgroup(path);
	if (fd < 0) {
		fprintf(stderr,
			"#%d %s: Failed to create cgroup '%s' (errno=%d)\n",
			env.test->test_num, env.test->test_name, path, errno);
		return fd;
	}

	if (join_cgroup(path)) {
		fprintf(stderr,
			"#%d %s: Failed to join cgroup '%s' (errno=%d)\n",
			env.test->test_num, env.test->test_name, path, errno);
		return -1;
	}

	return fd;
}

int bpf_find_map(const char *test, struct bpf_object *obj, const char *name)
>>>>>>> upstream/android-13
{
	struct bpf_map *map;

	map = bpf_object__find_map_by_name(obj, name);
	if (!map) {
<<<<<<< HEAD
		printf("%s:FAIL:map '%s' not found\n", test, name);
		error_cnt++;
=======
		fprintf(stdout, "%s:FAIL:map '%s' not found\n", test, name);
		test__fail();
>>>>>>> upstream/android-13
		return -1;
	}
	return bpf_map__fd(map);
}

<<<<<<< HEAD
static void test_pkt_access(void)
{
	const char *file = "./test_pkt_access.o";
	struct bpf_object *obj;
	__u32 duration, retval;
	int err, prog_fd;

	err = bpf_prog_load(file, BPF_PROG_TYPE_SCHED_CLS, &obj, &prog_fd);
	if (err) {
		error_cnt++;
		return;
	}

	err = bpf_prog_test_run(prog_fd, 100000, &pkt_v4, sizeof(pkt_v4),
				NULL, NULL, &retval, &duration);
	CHECK(err || errno || retval, "ipv4",
	      "err %d errno %d retval %d duration %d\n",
	      err, errno, retval, duration);

	err = bpf_prog_test_run(prog_fd, 100000, &pkt_v6, sizeof(pkt_v6),
				NULL, NULL, &retval, &duration);
	CHECK(err || errno || retval, "ipv6",
	      "err %d errno %d retval %d duration %d\n",
	      err, errno, retval, duration);
	bpf_object__close(obj);
}

static void test_xdp(void)
{
	struct vip key4 = {.protocol = 6, .family = AF_INET};
	struct vip key6 = {.protocol = 6, .family = AF_INET6};
	struct iptnl_info value4 = {.family = AF_INET};
	struct iptnl_info value6 = {.family = AF_INET6};
	const char *file = "./test_xdp.o";
	struct bpf_object *obj;
	char buf[128];
	struct ipv6hdr *iph6 = (void *)buf + sizeof(struct ethhdr);
	struct iphdr *iph = (void *)buf + sizeof(struct ethhdr);
	__u32 duration, retval, size;
	int err, prog_fd, map_fd;

	err = bpf_prog_load(file, BPF_PROG_TYPE_XDP, &obj, &prog_fd);
	if (err) {
		error_cnt++;
		return;
	}

	map_fd = bpf_find_map(__func__, obj, "vip2tnl");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &key4, &value4, 0);
	bpf_map_update_elem(map_fd, &key6, &value6, 0);

	err = bpf_prog_test_run(prog_fd, 1, &pkt_v4, sizeof(pkt_v4),
				buf, &size, &retval, &duration);

	CHECK(err || errno || retval != XDP_TX || size != 74 ||
	      iph->protocol != IPPROTO_IPIP, "ipv4",
	      "err %d errno %d retval %d size %d\n",
	      err, errno, retval, size);

	err = bpf_prog_test_run(prog_fd, 1, &pkt_v6, sizeof(pkt_v6),
				buf, &size, &retval, &duration);
	CHECK(err || errno || retval != XDP_TX || size != 114 ||
	      iph6->nexthdr != IPPROTO_IPV6, "ipv6",
	      "err %d errno %d retval %d size %d\n",
	      err, errno, retval, size);
out:
	bpf_object__close(obj);
}

static void test_xdp_adjust_tail(void)
{
	const char *file = "./test_adjust_tail.o";
	struct bpf_object *obj;
	char buf[128];
	__u32 duration, retval, size;
	int err, prog_fd;

	err = bpf_prog_load(file, BPF_PROG_TYPE_XDP, &obj, &prog_fd);
	if (err) {
		error_cnt++;
		return;
	}

	err = bpf_prog_test_run(prog_fd, 1, &pkt_v4, sizeof(pkt_v4),
				buf, &size, &retval, &duration);

	CHECK(err || errno || retval != XDP_DROP,
	      "ipv4", "err %d errno %d retval %d size %d\n",
	      err, errno, retval, size);

	err = bpf_prog_test_run(prog_fd, 1, &pkt_v6, sizeof(pkt_v6),
				buf, &size, &retval, &duration);
	CHECK(err || errno || retval != XDP_TX || size != 54,
	      "ipv6", "err %d errno %d retval %d size %d\n",
	      err, errno, retval, size);
	bpf_object__close(obj);
}



#define MAGIC_VAL 0x1234
#define NUM_ITER 100000
#define VIP_NUM 5

static void test_l4lb(const char *file)
{
	unsigned int nr_cpus = bpf_num_possible_cpus();
	struct vip key = {.protocol = 6};
	struct vip_meta {
		__u32 flags;
		__u32 vip_num;
	} value = {.vip_num = VIP_NUM};
	__u32 stats_key = VIP_NUM;
	struct vip_stats {
		__u64 bytes;
		__u64 pkts;
	} stats[nr_cpus];
	struct real_definition {
		union {
			__be32 dst;
			__be32 dstv6[4];
		};
		__u8 flags;
	} real_def = {.dst = MAGIC_VAL};
	__u32 ch_key = 11, real_num = 3;
	__u32 duration, retval, size;
	int err, i, prog_fd, map_fd;
	__u64 bytes = 0, pkts = 0;
	struct bpf_object *obj;
	char buf[128];
	u32 *magic = (u32 *)buf;

	err = bpf_prog_load(file, BPF_PROG_TYPE_SCHED_CLS, &obj, &prog_fd);
	if (err) {
		error_cnt++;
		return;
	}

	map_fd = bpf_find_map(__func__, obj, "vip_map");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &key, &value, 0);

	map_fd = bpf_find_map(__func__, obj, "ch_rings");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &ch_key, &real_num, 0);

	map_fd = bpf_find_map(__func__, obj, "reals");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &real_num, &real_def, 0);

	err = bpf_prog_test_run(prog_fd, NUM_ITER, &pkt_v4, sizeof(pkt_v4),
				buf, &size, &retval, &duration);
	CHECK(err || errno || retval != 7/*TC_ACT_REDIRECT*/ || size != 54 ||
	      *magic != MAGIC_VAL, "ipv4",
	      "err %d errno %d retval %d size %d magic %x\n",
	      err, errno, retval, size, *magic);

	err = bpf_prog_test_run(prog_fd, NUM_ITER, &pkt_v6, sizeof(pkt_v6),
				buf, &size, &retval, &duration);
	CHECK(err || errno || retval != 7/*TC_ACT_REDIRECT*/ || size != 74 ||
	      *magic != MAGIC_VAL, "ipv6",
	      "err %d errno %d retval %d size %d magic %x\n",
	      err, errno, retval, size, *magic);

	map_fd = bpf_find_map(__func__, obj, "stats");
	if (map_fd < 0)
		goto out;
	bpf_map_lookup_elem(map_fd, &stats_key, stats);
	for (i = 0; i < nr_cpus; i++) {
		bytes += stats[i].bytes;
		pkts += stats[i].pkts;
	}
	if (bytes != MAGIC_BYTES * NUM_ITER * 2 || pkts != NUM_ITER * 2) {
		error_cnt++;
		printf("test_l4lb:FAIL:stats %lld %lld\n", bytes, pkts);
	}
out:
	bpf_object__close(obj);
}

static void test_l4lb_all(void)
{
	const char *file1 = "./test_l4lb.o";
	const char *file2 = "./test_l4lb_noinline.o";

	test_l4lb(file1);
	test_l4lb(file2);
}

static void test_xdp_noinline(void)
{
	const char *file = "./test_xdp_noinline.o";
	unsigned int nr_cpus = bpf_num_possible_cpus();
	struct vip key = {.protocol = 6};
	struct vip_meta {
		__u32 flags;
		__u32 vip_num;
	} value = {.vip_num = VIP_NUM};
	__u32 stats_key = VIP_NUM;
	struct vip_stats {
		__u64 bytes;
		__u64 pkts;
	} stats[nr_cpus];
	struct real_definition {
		union {
			__be32 dst;
			__be32 dstv6[4];
		};
		__u8 flags;
	} real_def = {.dst = MAGIC_VAL};
	__u32 ch_key = 11, real_num = 3;
	__u32 duration, retval, size;
	int err, i, prog_fd, map_fd;
	__u64 bytes = 0, pkts = 0;
	struct bpf_object *obj;
	char buf[128];
	u32 *magic = (u32 *)buf;

	err = bpf_prog_load(file, BPF_PROG_TYPE_XDP, &obj, &prog_fd);
	if (err) {
		error_cnt++;
		return;
	}

	map_fd = bpf_find_map(__func__, obj, "vip_map");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &key, &value, 0);

	map_fd = bpf_find_map(__func__, obj, "ch_rings");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &ch_key, &real_num, 0);

	map_fd = bpf_find_map(__func__, obj, "reals");
	if (map_fd < 0)
		goto out;
	bpf_map_update_elem(map_fd, &real_num, &real_def, 0);

	err = bpf_prog_test_run(prog_fd, NUM_ITER, &pkt_v4, sizeof(pkt_v4),
				buf, &size, &retval, &duration);
	CHECK(err || errno || retval != 1 || size != 54 ||
	      *magic != MAGIC_VAL, "ipv4",
	      "err %d errno %d retval %d size %d magic %x\n",
	      err, errno, retval, size, *magic);

	err = bpf_prog_test_run(prog_fd, NUM_ITER, &pkt_v6, sizeof(pkt_v6),
				buf, &size, &retval, &duration);
	CHECK(err || errno || retval != 1 || size != 74 ||
	      *magic != MAGIC_VAL, "ipv6",
	      "err %d errno %d retval %d size %d magic %x\n",
	      err, errno, retval, size, *magic);

	map_fd = bpf_find_map(__func__, obj, "stats");
	if (map_fd < 0)
		goto out;
	bpf_map_lookup_elem(map_fd, &stats_key, stats);
	for (i = 0; i < nr_cpus; i++) {
		bytes += stats[i].bytes;
		pkts += stats[i].pkts;
	}
	if (bytes != MAGIC_BYTES * NUM_ITER * 2 || pkts != NUM_ITER * 2) {
		error_cnt++;
		printf("test_xdp_noinline:FAIL:stats %lld %lld\n", bytes, pkts);
	}
out:
	bpf_object__close(obj);
}

static void test_tcp_estats(void)
{
	const char *file = "./test_tcp_estats.o";
	int err, prog_fd;
	struct bpf_object *obj;
	__u32 duration = 0;

	err = bpf_prog_load(file, BPF_PROG_TYPE_TRACEPOINT, &obj, &prog_fd);
	CHECK(err, "", "err %d errno %d\n", err, errno);
	if (err) {
		error_cnt++;
		return;
	}

	bpf_object__close(obj);
}

static inline __u64 ptr_to_u64(const void *ptr)
{
	return (__u64) (unsigned long) ptr;
}

=======
>>>>>>> upstream/android-13
static bool is_jit_enabled(void)
{
	const char *jit_sysctl = "/proc/sys/net/core/bpf_jit_enable";
	bool enabled = false;
	int sysctl_fd;

	sysctl_fd = open(jit_sysctl, 0, O_RDONLY);
	if (sysctl_fd != -1) {
		char tmpc;

		if (read(sysctl_fd, &tmpc, sizeof(tmpc)) == 1)
			enabled = (tmpc != '0');
		close(sysctl_fd);
	}

	return enabled;
}

<<<<<<< HEAD
static void test_bpf_obj_id(void)
{
	const __u64 array_magic_value = 0xfaceb00c;
	const __u32 array_key = 0;
	const int nr_iters = 2;
	const char *file = "./test_obj_id.o";
	const char *expected_prog_name = "test_obj_id";
	const char *expected_map_name = "test_map_id";
	const __u64 nsec_per_sec = 1000000000;

	struct bpf_object *objs[nr_iters];
	int prog_fds[nr_iters], map_fds[nr_iters];
	/* +1 to test for the info_len returned by kernel */
	struct bpf_prog_info prog_infos[nr_iters + 1];
	struct bpf_map_info map_infos[nr_iters + 1];
	/* Each prog only uses one map. +1 to test nr_map_ids
	 * returned by kernel.
	 */
	__u32 map_ids[nr_iters + 1];
	char jited_insns[128], xlated_insns[128], zeros[128];
	__u32 i, next_id, info_len, nr_id_found, duration = 0;
	struct timespec real_time_ts, boot_time_ts;
	int err = 0;
	__u64 array_value;
	uid_t my_uid = getuid();
	time_t now, load_time;

	err = bpf_prog_get_fd_by_id(0);
	CHECK(err >= 0 || errno != ENOENT,
	      "get-fd-by-notexist-prog-id", "err %d errno %d\n", err, errno);

	err = bpf_map_get_fd_by_id(0);
	CHECK(err >= 0 || errno != ENOENT,
	      "get-fd-by-notexist-map-id", "err %d errno %d\n", err, errno);

	for (i = 0; i < nr_iters; i++)
		objs[i] = NULL;

	/* Check bpf_obj_get_info_by_fd() */
	bzero(zeros, sizeof(zeros));
	for (i = 0; i < nr_iters; i++) {
		now = time(NULL);
		err = bpf_prog_load(file, BPF_PROG_TYPE_SOCKET_FILTER,
				    &objs[i], &prog_fds[i]);
		/* test_obj_id.o is a dumb prog. It should never fail
		 * to load.
		 */
		if (err)
			error_cnt++;
		assert(!err);

		/* Insert a magic value to the map */
		map_fds[i] = bpf_find_map(__func__, objs[i], "test_map_id");
		assert(map_fds[i] >= 0);
		err = bpf_map_update_elem(map_fds[i], &array_key,
					  &array_magic_value, 0);
		assert(!err);

		/* Check getting map info */
		info_len = sizeof(struct bpf_map_info) * 2;
		bzero(&map_infos[i], info_len);
		err = bpf_obj_get_info_by_fd(map_fds[i], &map_infos[i],
					     &info_len);
		if (CHECK(err ||
			  map_infos[i].type != BPF_MAP_TYPE_ARRAY ||
			  map_infos[i].key_size != sizeof(__u32) ||
			  map_infos[i].value_size != sizeof(__u64) ||
			  map_infos[i].max_entries != 1 ||
			  map_infos[i].map_flags != 0 ||
			  info_len != sizeof(struct bpf_map_info) ||
			  strcmp((char *)map_infos[i].name, expected_map_name),
			  "get-map-info(fd)",
			  "err %d errno %d type %d(%d) info_len %u(%Zu) key_size %u value_size %u max_entries %u map_flags %X name %s(%s)\n",
			  err, errno,
			  map_infos[i].type, BPF_MAP_TYPE_ARRAY,
			  info_len, sizeof(struct bpf_map_info),
			  map_infos[i].key_size,
			  map_infos[i].value_size,
			  map_infos[i].max_entries,
			  map_infos[i].map_flags,
			  map_infos[i].name, expected_map_name))
			goto done;

		/* Check getting prog info */
		info_len = sizeof(struct bpf_prog_info) * 2;
		bzero(&prog_infos[i], info_len);
		bzero(jited_insns, sizeof(jited_insns));
		bzero(xlated_insns, sizeof(xlated_insns));
		prog_infos[i].jited_prog_insns = ptr_to_u64(jited_insns);
		prog_infos[i].jited_prog_len = sizeof(jited_insns);
		prog_infos[i].xlated_prog_insns = ptr_to_u64(xlated_insns);
		prog_infos[i].xlated_prog_len = sizeof(xlated_insns);
		prog_infos[i].map_ids = ptr_to_u64(map_ids + i);
		prog_infos[i].nr_map_ids = 2;
		err = clock_gettime(CLOCK_REALTIME, &real_time_ts);
		assert(!err);
		err = clock_gettime(CLOCK_BOOTTIME, &boot_time_ts);
		assert(!err);
		err = bpf_obj_get_info_by_fd(prog_fds[i], &prog_infos[i],
					     &info_len);
		load_time = (real_time_ts.tv_sec - boot_time_ts.tv_sec)
			+ (prog_infos[i].load_time / nsec_per_sec);
		if (CHECK(err ||
			  prog_infos[i].type != BPF_PROG_TYPE_SOCKET_FILTER ||
			  info_len != sizeof(struct bpf_prog_info) ||
			  (jit_enabled && !prog_infos[i].jited_prog_len) ||
			  (jit_enabled &&
			   !memcmp(jited_insns, zeros, sizeof(zeros))) ||
			  !prog_infos[i].xlated_prog_len ||
			  !memcmp(xlated_insns, zeros, sizeof(zeros)) ||
			  load_time < now - 60 || load_time > now + 60 ||
			  prog_infos[i].created_by_uid != my_uid ||
			  prog_infos[i].nr_map_ids != 1 ||
			  *(int *)prog_infos[i].map_ids != map_infos[i].id ||
			  strcmp((char *)prog_infos[i].name, expected_prog_name),
			  "get-prog-info(fd)",
			  "err %d errno %d i %d type %d(%d) info_len %u(%Zu) jit_enabled %d jited_prog_len %u xlated_prog_len %u jited_prog %d xlated_prog %d load_time %lu(%lu) uid %u(%u) nr_map_ids %u(%u) map_id %u(%u) name %s(%s)\n",
			  err, errno, i,
			  prog_infos[i].type, BPF_PROG_TYPE_SOCKET_FILTER,
			  info_len, sizeof(struct bpf_prog_info),
			  jit_enabled,
			  prog_infos[i].jited_prog_len,
			  prog_infos[i].xlated_prog_len,
			  !!memcmp(jited_insns, zeros, sizeof(zeros)),
			  !!memcmp(xlated_insns, zeros, sizeof(zeros)),
			  load_time, now,
			  prog_infos[i].created_by_uid, my_uid,
			  prog_infos[i].nr_map_ids, 1,
			  *(int *)prog_infos[i].map_ids, map_infos[i].id,
			  prog_infos[i].name, expected_prog_name))
			goto done;
	}

	/* Check bpf_prog_get_next_id() */
	nr_id_found = 0;
	next_id = 0;
	while (!bpf_prog_get_next_id(next_id, &next_id)) {
		struct bpf_prog_info prog_info = {};
		__u32 saved_map_id;
		int prog_fd;

		info_len = sizeof(prog_info);

		prog_fd = bpf_prog_get_fd_by_id(next_id);
		if (prog_fd < 0 && errno == ENOENT)
			/* The bpf_prog is in the dead row */
			continue;
		if (CHECK(prog_fd < 0, "get-prog-fd(next_id)",
			  "prog_fd %d next_id %d errno %d\n",
			  prog_fd, next_id, errno))
			break;

		for (i = 0; i < nr_iters; i++)
			if (prog_infos[i].id == next_id)
				break;

		if (i == nr_iters)
			continue;

		nr_id_found++;

		/* Negative test:
		 * prog_info.nr_map_ids = 1
		 * prog_info.map_ids = NULL
		 */
		prog_info.nr_map_ids = 1;
		err = bpf_obj_get_info_by_fd(prog_fd, &prog_info, &info_len);
		if (CHECK(!err || errno != EFAULT,
			  "get-prog-fd-bad-nr-map-ids", "err %d errno %d(%d)",
			  err, errno, EFAULT))
			break;
		bzero(&prog_info, sizeof(prog_info));
		info_len = sizeof(prog_info);

		saved_map_id = *(int *)(prog_infos[i].map_ids);
		prog_info.map_ids = prog_infos[i].map_ids;
		prog_info.nr_map_ids = 2;
		err = bpf_obj_get_info_by_fd(prog_fd, &prog_info, &info_len);
		prog_infos[i].jited_prog_insns = 0;
		prog_infos[i].xlated_prog_insns = 0;
		CHECK(err || info_len != sizeof(struct bpf_prog_info) ||
		      memcmp(&prog_info, &prog_infos[i], info_len) ||
		      *(int *)prog_info.map_ids != saved_map_id,
		      "get-prog-info(next_id->fd)",
		      "err %d errno %d info_len %u(%Zu) memcmp %d map_id %u(%u)\n",
		      err, errno, info_len, sizeof(struct bpf_prog_info),
		      memcmp(&prog_info, &prog_infos[i], info_len),
		      *(int *)prog_info.map_ids, saved_map_id);
		close(prog_fd);
	}
	CHECK(nr_id_found != nr_iters,
	      "check total prog id found by get_next_id",
	      "nr_id_found %u(%u)\n",
	      nr_id_found, nr_iters);

	/* Check bpf_map_get_next_id() */
	nr_id_found = 0;
	next_id = 0;
	while (!bpf_map_get_next_id(next_id, &next_id)) {
		struct bpf_map_info map_info = {};
		int map_fd;

		info_len = sizeof(map_info);

		map_fd = bpf_map_get_fd_by_id(next_id);
		if (map_fd < 0 && errno == ENOENT)
			/* The bpf_map is in the dead row */
			continue;
		if (CHECK(map_fd < 0, "get-map-fd(next_id)",
			  "map_fd %d next_id %u errno %d\n",
			  map_fd, next_id, errno))
			break;

		for (i = 0; i < nr_iters; i++)
			if (map_infos[i].id == next_id)
				break;

		if (i == nr_iters)
			continue;

		nr_id_found++;

		err = bpf_map_lookup_elem(map_fd, &array_key, &array_value);
		assert(!err);

		err = bpf_obj_get_info_by_fd(map_fd, &map_info, &info_len);
		CHECK(err || info_len != sizeof(struct bpf_map_info) ||
		      memcmp(&map_info, &map_infos[i], info_len) ||
		      array_value != array_magic_value,
		      "check get-map-info(next_id->fd)",
		      "err %d errno %d info_len %u(%Zu) memcmp %d array_value %llu(%llu)\n",
		      err, errno, info_len, sizeof(struct bpf_map_info),
		      memcmp(&map_info, &map_infos[i], info_len),
		      array_value, array_magic_value);

		close(map_fd);
	}
	CHECK(nr_id_found != nr_iters,
	      "check total map id found by get_next_id",
	      "nr_id_found %u(%u)\n",
	      nr_id_found, nr_iters);

done:
	for (i = 0; i < nr_iters; i++)
		bpf_object__close(objs[i]);
}

static void test_pkt_md_access(void)
{
	const char *file = "./test_pkt_md_access.o";
	struct bpf_object *obj;
	__u32 duration, retval;
	int err, prog_fd;

	err = bpf_prog_load(file, BPF_PROG_TYPE_SCHED_CLS, &obj, &prog_fd);
	if (err) {
		error_cnt++;
		return;
	}

	err = bpf_prog_test_run(prog_fd, 10, &pkt_v4, sizeof(pkt_v4),
				NULL, NULL, &retval, &duration);
	CHECK(err || retval, "",
	      "err %d errno %d retval %d duration %d\n",
	      err, errno, retval, duration);

	bpf_object__close(obj);
}

static void test_obj_name(void)
{
	struct {
		const char *name;
		int success;
		int expected_errno;
	} tests[] = {
		{ "", 1, 0 },
		{ "_123456789ABCDE", 1, 0 },
		{ "_123456789ABCDEF", 0, EINVAL },
		{ "_123456789ABCD\n", 0, EINVAL },
	};
	struct bpf_insn prog[] = {
		BPF_ALU64_IMM(BPF_MOV, BPF_REG_0, 0),
		BPF_EXIT_INSN(),
	};
	__u32 duration = 0;
	int i;

	for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
		size_t name_len = strlen(tests[i].name) + 1;
		union bpf_attr attr;
		size_t ncopy;
		int fd;

		/* test different attr.prog_name during BPF_PROG_LOAD */
		ncopy = name_len < sizeof(attr.prog_name) ?
			name_len : sizeof(attr.prog_name);
		bzero(&attr, sizeof(attr));
		attr.prog_type = BPF_PROG_TYPE_SCHED_CLS;
		attr.insn_cnt = 2;
		attr.insns = ptr_to_u64(prog);
		attr.license = ptr_to_u64("");
		memcpy(attr.prog_name, tests[i].name, ncopy);

		fd = syscall(__NR_bpf, BPF_PROG_LOAD, &attr, sizeof(attr));
		CHECK((tests[i].success && fd < 0) ||
		      (!tests[i].success && fd != -1) ||
		      (!tests[i].success && errno != tests[i].expected_errno),
		      "check-bpf-prog-name",
		      "fd %d(%d) errno %d(%d)\n",
		       fd, tests[i].success, errno, tests[i].expected_errno);

		if (fd != -1)
			close(fd);

		/* test different attr.map_name during BPF_MAP_CREATE */
		ncopy = name_len < sizeof(attr.map_name) ?
			name_len : sizeof(attr.map_name);
		bzero(&attr, sizeof(attr));
		attr.map_type = BPF_MAP_TYPE_ARRAY;
		attr.key_size = 4;
		attr.value_size = 4;
		attr.max_entries = 1;
		attr.map_flags = 0;
		memcpy(attr.map_name, tests[i].name, ncopy);
		fd = syscall(__NR_bpf, BPF_MAP_CREATE, &attr, sizeof(attr));
		CHECK((tests[i].success && fd < 0) ||
		      (!tests[i].success && fd != -1) ||
		      (!tests[i].success && errno != tests[i].expected_errno),
		      "check-bpf-map-name",
		      "fd %d(%d) errno %d(%d)\n",
		      fd, tests[i].success, errno, tests[i].expected_errno);

		if (fd != -1)
			close(fd);
	}
}

static void test_tp_attach_query(void)
{
	const int num_progs = 3;
	int i, j, bytes, efd, err, prog_fd[num_progs], pmu_fd[num_progs];
	__u32 duration = 0, info_len, saved_prog_ids[num_progs];
	const char *file = "./test_tracepoint.o";
	struct perf_event_query_bpf *query;
	struct perf_event_attr attr = {};
	struct bpf_object *obj[num_progs];
	struct bpf_prog_info prog_info;
	char buf[256];

	snprintf(buf, sizeof(buf),
		 "/sys/kernel/debug/tracing/events/sched/sched_switch/id");
	efd = open(buf, O_RDONLY, 0);
	if (CHECK(efd < 0, "open", "err %d errno %d\n", efd, errno))
		return;
	bytes = read(efd, buf, sizeof(buf));
	close(efd);
	if (CHECK(bytes <= 0 || bytes >= sizeof(buf),
		  "read", "bytes %d errno %d\n", bytes, errno))
		return;

	attr.config = strtol(buf, NULL, 0);
	attr.type = PERF_TYPE_TRACEPOINT;
	attr.sample_type = PERF_SAMPLE_RAW | PERF_SAMPLE_CALLCHAIN;
	attr.sample_period = 1;
	attr.wakeup_events = 1;

	query = malloc(sizeof(*query) + sizeof(__u32) * num_progs);
	for (i = 0; i < num_progs; i++) {
		err = bpf_prog_load(file, BPF_PROG_TYPE_TRACEPOINT, &obj[i],
				    &prog_fd[i]);
		if (CHECK(err, "prog_load", "err %d errno %d\n", err, errno))
			goto cleanup1;

		bzero(&prog_info, sizeof(prog_info));
		prog_info.jited_prog_len = 0;
		prog_info.xlated_prog_len = 0;
		prog_info.nr_map_ids = 0;
		info_len = sizeof(prog_info);
		err = bpf_obj_get_info_by_fd(prog_fd[i], &prog_info, &info_len);
		if (CHECK(err, "bpf_obj_get_info_by_fd", "err %d errno %d\n",
			  err, errno))
			goto cleanup1;
		saved_prog_ids[i] = prog_info.id;

		pmu_fd[i] = syscall(__NR_perf_event_open, &attr, -1 /* pid */,
				    0 /* cpu 0 */, -1 /* group id */,
				    0 /* flags */);
		if (CHECK(pmu_fd[i] < 0, "perf_event_open", "err %d errno %d\n",
			  pmu_fd[i], errno))
			goto cleanup2;
		err = ioctl(pmu_fd[i], PERF_EVENT_IOC_ENABLE, 0);
		if (CHECK(err, "perf_event_ioc_enable", "err %d errno %d\n",
			  err, errno))
			goto cleanup3;

		if (i == 0) {
			/* check NULL prog array query */
			query->ids_len = num_progs;
			err = ioctl(pmu_fd[i], PERF_EVENT_IOC_QUERY_BPF, query);
			if (CHECK(err || query->prog_cnt != 0,
				  "perf_event_ioc_query_bpf",
				  "err %d errno %d query->prog_cnt %u\n",
				  err, errno, query->prog_cnt))
				goto cleanup3;
		}

		err = ioctl(pmu_fd[i], PERF_EVENT_IOC_SET_BPF, prog_fd[i]);
		if (CHECK(err, "perf_event_ioc_set_bpf", "err %d errno %d\n",
			  err, errno))
			goto cleanup3;

		if (i == 1) {
			/* try to get # of programs only */
			query->ids_len = 0;
			err = ioctl(pmu_fd[i], PERF_EVENT_IOC_QUERY_BPF, query);
			if (CHECK(err || query->prog_cnt != 2,
				  "perf_event_ioc_query_bpf",
				  "err %d errno %d query->prog_cnt %u\n",
				  err, errno, query->prog_cnt))
				goto cleanup3;

			/* try a few negative tests */
			/* invalid query pointer */
			err = ioctl(pmu_fd[i], PERF_EVENT_IOC_QUERY_BPF,
				    (struct perf_event_query_bpf *)0x1);
			if (CHECK(!err || errno != EFAULT,
				  "perf_event_ioc_query_bpf",
				  "err %d errno %d\n", err, errno))
				goto cleanup3;

			/* no enough space */
			query->ids_len = 1;
			err = ioctl(pmu_fd[i], PERF_EVENT_IOC_QUERY_BPF, query);
			if (CHECK(!err || errno != ENOSPC || query->prog_cnt != 2,
				  "perf_event_ioc_query_bpf",
				  "err %d errno %d query->prog_cnt %u\n",
				  err, errno, query->prog_cnt))
				goto cleanup3;
		}

		query->ids_len = num_progs;
		err = ioctl(pmu_fd[i], PERF_EVENT_IOC_QUERY_BPF, query);
		if (CHECK(err || query->prog_cnt != (i + 1),
			  "perf_event_ioc_query_bpf",
			  "err %d errno %d query->prog_cnt %u\n",
			  err, errno, query->prog_cnt))
			goto cleanup3;
		for (j = 0; j < i + 1; j++)
			if (CHECK(saved_prog_ids[j] != query->ids[j],
				  "perf_event_ioc_query_bpf",
				  "#%d saved_prog_id %x query prog_id %x\n",
				  j, saved_prog_ids[j], query->ids[j]))
				goto cleanup3;
	}

	i = num_progs - 1;
	for (; i >= 0; i--) {
 cleanup3:
		ioctl(pmu_fd[i], PERF_EVENT_IOC_DISABLE);
 cleanup2:
		close(pmu_fd[i]);
 cleanup1:
		bpf_object__close(obj[i]);
	}
	free(query);
}

static int compare_map_keys(int map1_fd, int map2_fd)
=======
int compare_map_keys(int map1_fd, int map2_fd)
>>>>>>> upstream/android-13
{
	__u32 key, next_key;
	char val_buf[PERF_MAX_STACK_DEPTH *
		     sizeof(struct bpf_stack_build_id)];
	int err;

	err = bpf_map_get_next_key(map1_fd, NULL, &key);
	if (err)
		return err;
	err = bpf_map_lookup_elem(map2_fd, &key, val_buf);
	if (err)
		return err;

	while (bpf_map_get_next_key(map1_fd, &key, &next_key) == 0) {
		err = bpf_map_lookup_elem(map2_fd, &next_key, val_buf);
		if (err)
			return err;

		key = next_key;
	}
	if (errno != ENOENT)
		return -1;

	return 0;
}

<<<<<<< HEAD
static int compare_stack_ips(int smap_fd, int amap_fd, int stack_trace_len)
=======
int compare_stack_ips(int smap_fd, int amap_fd, int stack_trace_len)
>>>>>>> upstream/android-13
{
	__u32 key, next_key, *cur_key_p, *next_key_p;
	char *val_buf1, *val_buf2;
	int i, err = 0;

	val_buf1 = malloc(stack_trace_len);
	val_buf2 = malloc(stack_trace_len);
	cur_key_p = NULL;
	next_key_p = &key;
	while (bpf_map_get_next_key(smap_fd, cur_key_p, next_key_p) == 0) {
		err = bpf_map_lookup_elem(smap_fd, next_key_p, val_buf1);
		if (err)
			goto out;
		err = bpf_map_lookup_elem(amap_fd, next_key_p, val_buf2);
		if (err)
			goto out;
		for (i = 0; i < stack_trace_len; i++) {
			if (val_buf1[i] != val_buf2[i]) {
				err = -1;
				goto out;
			}
		}
		key = *next_key_p;
		cur_key_p = &key;
		next_key_p = &next_key;
	}
	if (errno != ENOENT)
		err = -1;

out:
	free(val_buf1);
	free(val_buf2);
	return err;
}

<<<<<<< HEAD
static void test_stacktrace_map()
{
	int control_map_fd, stackid_hmap_fd, stackmap_fd, stack_amap_fd;
	const char *file = "./test_stacktrace_map.o";
	int bytes, efd, err, pmu_fd, prog_fd, stack_trace_len;
	struct perf_event_attr attr = {};
	__u32 key, val, duration = 0;
	struct bpf_object *obj;
	char buf[256];

	err = bpf_prog_load(file, BPF_PROG_TYPE_TRACEPOINT, &obj, &prog_fd);
	if (CHECK(err, "prog_load", "err %d errno %d\n", err, errno))
		return;

	/* Get the ID for the sched/sched_switch tracepoint */
	snprintf(buf, sizeof(buf),
		 "/sys/kernel/debug/tracing/events/sched/sched_switch/id");
	efd = open(buf, O_RDONLY, 0);
	if (CHECK(efd < 0, "open", "err %d errno %d\n", efd, errno))
		goto close_prog;

	bytes = read(efd, buf, sizeof(buf));
	close(efd);
	if (bytes <= 0 || bytes >= sizeof(buf))
		goto close_prog;

	/* Open the perf event and attach bpf progrram */
	attr.config = strtol(buf, NULL, 0);
	attr.type = PERF_TYPE_TRACEPOINT;
	attr.sample_type = PERF_SAMPLE_RAW | PERF_SAMPLE_CALLCHAIN;
	attr.sample_period = 1;
	attr.wakeup_events = 1;
	pmu_fd = syscall(__NR_perf_event_open, &attr, -1 /* pid */,
			 0 /* cpu 0 */, -1 /* group id */,
			 0 /* flags */);
	if (CHECK(pmu_fd < 0, "perf_event_open", "err %d errno %d\n",
		  pmu_fd, errno))
		goto close_prog;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0);
	if (err)
		goto disable_pmu;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_SET_BPF, prog_fd);
	if (err)
		goto disable_pmu;

	/* find map fds */
	control_map_fd = bpf_find_map(__func__, obj, "control_map");
	if (control_map_fd < 0)
		goto disable_pmu;

	stackid_hmap_fd = bpf_find_map(__func__, obj, "stackid_hmap");
	if (stackid_hmap_fd < 0)
		goto disable_pmu;

	stackmap_fd = bpf_find_map(__func__, obj, "stackmap");
	if (stackmap_fd < 0)
		goto disable_pmu;

	stack_amap_fd = bpf_find_map(__func__, obj, "stack_amap");
	if (stack_amap_fd < 0)
		goto disable_pmu;

	/* give some time for bpf program run */
	sleep(1);

	/* disable stack trace collection */
	key = 0;
	val = 1;
	bpf_map_update_elem(control_map_fd, &key, &val, 0);

	/* for every element in stackid_hmap, we can find a corresponding one
	 * in stackmap, and vise versa.
	 */
	err = compare_map_keys(stackid_hmap_fd, stackmap_fd);
	if (CHECK(err, "compare_map_keys stackid_hmap vs. stackmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu_noerr;

	err = compare_map_keys(stackmap_fd, stackid_hmap_fd);
	if (CHECK(err, "compare_map_keys stackmap vs. stackid_hmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu_noerr;

	stack_trace_len = PERF_MAX_STACK_DEPTH * sizeof(__u64);
	err = compare_stack_ips(stackmap_fd, stack_amap_fd, stack_trace_len);
	if (CHECK(err, "compare_stack_ips stackmap vs. stack_amap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu_noerr;

	goto disable_pmu_noerr;
disable_pmu:
	error_cnt++;
disable_pmu_noerr:
	ioctl(pmu_fd, PERF_EVENT_IOC_DISABLE);
	close(pmu_fd);
close_prog:
	bpf_object__close(obj);
}

static void test_stacktrace_map_raw_tp()
{
	int control_map_fd, stackid_hmap_fd, stackmap_fd;
	const char *file = "./test_stacktrace_map.o";
	int efd, err, prog_fd;
	__u32 key, val, duration = 0;
	struct bpf_object *obj;

	err = bpf_prog_load(file, BPF_PROG_TYPE_RAW_TRACEPOINT, &obj, &prog_fd);
	if (CHECK(err, "prog_load raw tp", "err %d errno %d\n", err, errno))
		return;

	efd = bpf_raw_tracepoint_open("sched_switch", prog_fd);
	if (CHECK(efd < 0, "raw_tp_open", "err %d errno %d\n", efd, errno))
		goto close_prog;

	/* find map fds */
	control_map_fd = bpf_find_map(__func__, obj, "control_map");
	if (control_map_fd < 0)
		goto close_prog;

	stackid_hmap_fd = bpf_find_map(__func__, obj, "stackid_hmap");
	if (stackid_hmap_fd < 0)
		goto close_prog;

	stackmap_fd = bpf_find_map(__func__, obj, "stackmap");
	if (stackmap_fd < 0)
		goto close_prog;

	/* give some time for bpf program run */
	sleep(1);

	/* disable stack trace collection */
	key = 0;
	val = 1;
	bpf_map_update_elem(control_map_fd, &key, &val, 0);

	/* for every element in stackid_hmap, we can find a corresponding one
	 * in stackmap, and vise versa.
	 */
	err = compare_map_keys(stackid_hmap_fd, stackmap_fd);
	if (CHECK(err, "compare_map_keys stackid_hmap vs. stackmap",
		  "err %d errno %d\n", err, errno))
		goto close_prog;

	err = compare_map_keys(stackmap_fd, stackid_hmap_fd);
	if (CHECK(err, "compare_map_keys stackmap vs. stackid_hmap",
		  "err %d errno %d\n", err, errno))
		goto close_prog;

	goto close_prog_noerr;
close_prog:
	error_cnt++;
close_prog_noerr:
	bpf_object__close(obj);
}

static int extract_build_id(char *build_id, size_t size)
=======
int extract_build_id(char *build_id, size_t size)
>>>>>>> upstream/android-13
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;

	fp = popen("readelf -n ./urandom_read | grep 'Build ID'", "r");
	if (fp == NULL)
		return -1;

	if (getline(&line, &len, fp) == -1)
		goto err;
<<<<<<< HEAD
	fclose(fp);
=======
	pclose(fp);
>>>>>>> upstream/android-13

	if (len > size)
		len = size;
	memcpy(build_id, line, len);
	build_id[len] = '\0';
	free(line);
	return 0;
err:
<<<<<<< HEAD
	fclose(fp);
	return -1;
}

static void test_stacktrace_build_id(void)
{
	int control_map_fd, stackid_hmap_fd, stackmap_fd, stack_amap_fd;
	const char *file = "./test_stacktrace_build_id.o";
	int bytes, efd, err, pmu_fd, prog_fd, stack_trace_len;
	struct perf_event_attr attr = {};
	__u32 key, previous_key, val, duration = 0;
	struct bpf_object *obj;
	char buf[256];
	int i, j;
	struct bpf_stack_build_id id_offs[PERF_MAX_STACK_DEPTH];
	int build_id_matches = 0;
	int retry = 1;

retry:
	err = bpf_prog_load(file, BPF_PROG_TYPE_TRACEPOINT, &obj, &prog_fd);
	if (CHECK(err, "prog_load", "err %d errno %d\n", err, errno))
		goto out;

	/* Get the ID for the sched/sched_switch tracepoint */
	snprintf(buf, sizeof(buf),
		 "/sys/kernel/debug/tracing/events/random/urandom_read/id");
	efd = open(buf, O_RDONLY, 0);
	if (CHECK(efd < 0, "open", "err %d errno %d\n", efd, errno))
		goto close_prog;

	bytes = read(efd, buf, sizeof(buf));
	close(efd);
	if (CHECK(bytes <= 0 || bytes >= sizeof(buf),
		  "read", "bytes %d errno %d\n", bytes, errno))
		goto close_prog;

	/* Open the perf event and attach bpf progrram */
	attr.config = strtol(buf, NULL, 0);
	attr.type = PERF_TYPE_TRACEPOINT;
	attr.sample_type = PERF_SAMPLE_RAW | PERF_SAMPLE_CALLCHAIN;
	attr.sample_period = 1;
	attr.wakeup_events = 1;
	pmu_fd = syscall(__NR_perf_event_open, &attr, -1 /* pid */,
			 0 /* cpu 0 */, -1 /* group id */,
			 0 /* flags */);
	if (CHECK(pmu_fd < 0, "perf_event_open", "err %d errno %d\n",
		  pmu_fd, errno))
		goto close_prog;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0);
	if (CHECK(err, "perf_event_ioc_enable", "err %d errno %d\n",
		  err, errno))
		goto close_pmu;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_SET_BPF, prog_fd);
	if (CHECK(err, "perf_event_ioc_set_bpf", "err %d errno %d\n",
		  err, errno))
		goto disable_pmu;

	/* find map fds */
	control_map_fd = bpf_find_map(__func__, obj, "control_map");
	if (CHECK(control_map_fd < 0, "bpf_find_map control_map",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	stackid_hmap_fd = bpf_find_map(__func__, obj, "stackid_hmap");
	if (CHECK(stackid_hmap_fd < 0, "bpf_find_map stackid_hmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	stackmap_fd = bpf_find_map(__func__, obj, "stackmap");
	if (CHECK(stackmap_fd < 0, "bpf_find_map stackmap", "err %d errno %d\n",
		  err, errno))
		goto disable_pmu;

	stack_amap_fd = bpf_find_map(__func__, obj, "stack_amap");
	if (CHECK(stack_amap_fd < 0, "bpf_find_map stack_amap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	assert(system("dd if=/dev/urandom of=/dev/zero count=4 2> /dev/null")
	       == 0);
	assert(system("./urandom_read") == 0);
	/* disable stack trace collection */
	key = 0;
	val = 1;
	bpf_map_update_elem(control_map_fd, &key, &val, 0);

	/* for every element in stackid_hmap, we can find a corresponding one
	 * in stackmap, and vise versa.
	 */
	err = compare_map_keys(stackid_hmap_fd, stackmap_fd);
	if (CHECK(err, "compare_map_keys stackid_hmap vs. stackmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	err = compare_map_keys(stackmap_fd, stackid_hmap_fd);
	if (CHECK(err, "compare_map_keys stackmap vs. stackid_hmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	err = extract_build_id(buf, 256);

	if (CHECK(err, "get build_id with readelf",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	err = bpf_map_get_next_key(stackmap_fd, NULL, &key);
	if (CHECK(err, "get_next_key from stackmap",
		  "err %d, errno %d\n", err, errno))
		goto disable_pmu;

	do {
		char build_id[64];

		err = bpf_map_lookup_elem(stackmap_fd, &key, id_offs);
		if (CHECK(err, "lookup_elem from stackmap",
			  "err %d, errno %d\n", err, errno))
			goto disable_pmu;
		for (i = 0; i < PERF_MAX_STACK_DEPTH; ++i)
			if (id_offs[i].status == BPF_STACK_BUILD_ID_VALID &&
			    id_offs[i].offset != 0) {
				for (j = 0; j < 20; ++j)
					sprintf(build_id + 2 * j, "%02x",
						id_offs[i].build_id[j] & 0xff);
				if (strstr(buf, build_id) != NULL)
					build_id_matches = 1;
			}
		previous_key = key;
	} while (bpf_map_get_next_key(stackmap_fd, &previous_key, &key) == 0);

	/* stack_map_get_build_id_offset() is racy and sometimes can return
	 * BPF_STACK_BUILD_ID_IP instead of BPF_STACK_BUILD_ID_VALID;
	 * try it one more time.
	 */
	if (build_id_matches < 1 && retry--) {
		ioctl(pmu_fd, PERF_EVENT_IOC_DISABLE);
		close(pmu_fd);
		bpf_object__close(obj);
		printf("%s:WARN:Didn't find expected build ID from the map, retrying\n",
		       __func__);
		goto retry;
	}

	if (CHECK(build_id_matches < 1, "build id match",
		  "Didn't find expected build ID from the map\n"))
		goto disable_pmu;

	stack_trace_len = PERF_MAX_STACK_DEPTH
		* sizeof(struct bpf_stack_build_id);
	err = compare_stack_ips(stackmap_fd, stack_amap_fd, stack_trace_len);
	CHECK(err, "compare_stack_ips stackmap vs. stack_amap",
	      "err %d errno %d\n", err, errno);

disable_pmu:
	ioctl(pmu_fd, PERF_EVENT_IOC_DISABLE);

close_pmu:
	close(pmu_fd);

close_prog:
	bpf_object__close(obj);

out:
	return;
}

static void test_stacktrace_build_id_nmi(void)
{
	int control_map_fd, stackid_hmap_fd, stackmap_fd, stack_amap_fd;
	const char *file = "./test_stacktrace_build_id.o";
	int err, pmu_fd, prog_fd;
	struct perf_event_attr attr = {
		.sample_freq = 5000,
		.freq = 1,
		.type = PERF_TYPE_HARDWARE,
		.config = PERF_COUNT_HW_CPU_CYCLES,
	};
	__u32 key, previous_key, val, duration = 0;
	struct bpf_object *obj;
	char buf[256];
	int i, j;
	struct bpf_stack_build_id id_offs[PERF_MAX_STACK_DEPTH];
	int build_id_matches = 0;
	int retry = 1;

retry:
	err = bpf_prog_load(file, BPF_PROG_TYPE_PERF_EVENT, &obj, &prog_fd);
	if (CHECK(err, "prog_load", "err %d errno %d\n", err, errno))
		return;

	pmu_fd = syscall(__NR_perf_event_open, &attr, -1 /* pid */,
			 0 /* cpu 0 */, -1 /* group id */,
			 0 /* flags */);
	if (CHECK(pmu_fd < 0, "perf_event_open",
		  "err %d errno %d. Does the test host support PERF_COUNT_HW_CPU_CYCLES?\n",
		  pmu_fd, errno))
		goto close_prog;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0);
	if (CHECK(err, "perf_event_ioc_enable", "err %d errno %d\n",
		  err, errno))
		goto close_pmu;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_SET_BPF, prog_fd);
	if (CHECK(err, "perf_event_ioc_set_bpf", "err %d errno %d\n",
		  err, errno))
		goto disable_pmu;

	/* find map fds */
	control_map_fd = bpf_find_map(__func__, obj, "control_map");
	if (CHECK(control_map_fd < 0, "bpf_find_map control_map",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	stackid_hmap_fd = bpf_find_map(__func__, obj, "stackid_hmap");
	if (CHECK(stackid_hmap_fd < 0, "bpf_find_map stackid_hmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	stackmap_fd = bpf_find_map(__func__, obj, "stackmap");
	if (CHECK(stackmap_fd < 0, "bpf_find_map stackmap", "err %d errno %d\n",
		  err, errno))
		goto disable_pmu;

	stack_amap_fd = bpf_find_map(__func__, obj, "stack_amap");
	if (CHECK(stack_amap_fd < 0, "bpf_find_map stack_amap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	assert(system("dd if=/dev/urandom of=/dev/zero count=4 2> /dev/null")
	       == 0);
	assert(system("taskset 0x1 ./urandom_read 100000") == 0);
	/* disable stack trace collection */
	key = 0;
	val = 1;
	bpf_map_update_elem(control_map_fd, &key, &val, 0);

	/* for every element in stackid_hmap, we can find a corresponding one
	 * in stackmap, and vise versa.
	 */
	err = compare_map_keys(stackid_hmap_fd, stackmap_fd);
	if (CHECK(err, "compare_map_keys stackid_hmap vs. stackmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	err = compare_map_keys(stackmap_fd, stackid_hmap_fd);
	if (CHECK(err, "compare_map_keys stackmap vs. stackid_hmap",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	err = extract_build_id(buf, 256);

	if (CHECK(err, "get build_id with readelf",
		  "err %d errno %d\n", err, errno))
		goto disable_pmu;

	err = bpf_map_get_next_key(stackmap_fd, NULL, &key);
	if (CHECK(err, "get_next_key from stackmap",
		  "err %d, errno %d\n", err, errno))
		goto disable_pmu;

	do {
		char build_id[64];

		err = bpf_map_lookup_elem(stackmap_fd, &key, id_offs);
		if (CHECK(err, "lookup_elem from stackmap",
			  "err %d, errno %d\n", err, errno))
			goto disable_pmu;
		for (i = 0; i < PERF_MAX_STACK_DEPTH; ++i)
			if (id_offs[i].status == BPF_STACK_BUILD_ID_VALID &&
			    id_offs[i].offset != 0) {
				for (j = 0; j < 20; ++j)
					sprintf(build_id + 2 * j, "%02x",
						id_offs[i].build_id[j] & 0xff);
				if (strstr(buf, build_id) != NULL)
					build_id_matches = 1;
			}
		previous_key = key;
	} while (bpf_map_get_next_key(stackmap_fd, &previous_key, &key) == 0);

	/* stack_map_get_build_id_offset() is racy and sometimes can return
	 * BPF_STACK_BUILD_ID_IP instead of BPF_STACK_BUILD_ID_VALID;
	 * try it one more time.
	 */
	if (build_id_matches < 1 && retry--) {
		ioctl(pmu_fd, PERF_EVENT_IOC_DISABLE);
		close(pmu_fd);
		bpf_object__close(obj);
		printf("%s:WARN:Didn't find expected build ID from the map, retrying\n",
		       __func__);
		goto retry;
	}

	if (CHECK(build_id_matches < 1, "build id match",
		  "Didn't find expected build ID from the map\n"))
		goto disable_pmu;

	/*
	 * We intentionally skip compare_stack_ips(). This is because we
	 * only support one in_nmi() ips-to-build_id translation per cpu
	 * at any time, thus stack_amap here will always fallback to
	 * BPF_STACK_BUILD_ID_IP;
	 */

disable_pmu:
	ioctl(pmu_fd, PERF_EVENT_IOC_DISABLE);

close_pmu:
	close(pmu_fd);

close_prog:
	bpf_object__close(obj);
}

#define MAX_CNT_RAWTP	10ull
#define MAX_STACK_RAWTP	100
struct get_stack_trace_t {
	int pid;
	int kern_stack_size;
	int user_stack_size;
	int user_stack_buildid_size;
	__u64 kern_stack[MAX_STACK_RAWTP];
	__u64 user_stack[MAX_STACK_RAWTP];
	struct bpf_stack_build_id user_stack_buildid[MAX_STACK_RAWTP];
};

static int get_stack_print_output(void *data, int size)
{
	bool good_kern_stack = false, good_user_stack = false;
	const char *nonjit_func = "___bpf_prog_run";
	struct get_stack_trace_t *e = data;
	int i, num_stack;
	static __u64 cnt;
	struct ksym *ks;

	cnt++;

	if (size < sizeof(struct get_stack_trace_t)) {
		__u64 *raw_data = data;
		bool found = false;

		num_stack = size / sizeof(__u64);
		/* If jit is enabled, we do not have a good way to
		 * verify the sanity of the kernel stack. So we
		 * just assume it is good if the stack is not empty.
		 * This could be improved in the future.
		 */
		if (jit_enabled) {
			found = num_stack > 0;
		} else {
			for (i = 0; i < num_stack; i++) {
				ks = ksym_search(raw_data[i]);
				if (strcmp(ks->name, nonjit_func) == 0) {
					found = true;
					break;
				}
			}
		}
		if (found) {
			good_kern_stack = true;
			good_user_stack = true;
		}
	} else {
		num_stack = e->kern_stack_size / sizeof(__u64);
		if (jit_enabled) {
			good_kern_stack = num_stack > 0;
		} else {
			for (i = 0; i < num_stack; i++) {
				ks = ksym_search(e->kern_stack[i]);
				if (strcmp(ks->name, nonjit_func) == 0) {
					good_kern_stack = true;
					break;
				}
			}
		}
		if (e->user_stack_size > 0 && e->user_stack_buildid_size > 0)
			good_user_stack = true;
	}
	if (!good_kern_stack || !good_user_stack)
		return LIBBPF_PERF_EVENT_ERROR;

	if (cnt == MAX_CNT_RAWTP)
		return LIBBPF_PERF_EVENT_DONE;

	return LIBBPF_PERF_EVENT_CONT;
}

static void test_get_stack_raw_tp(void)
{
	const char *file = "./test_get_stack_rawtp.o";
	int i, efd, err, prog_fd, pmu_fd, perfmap_fd;
	struct perf_event_attr attr = {};
	struct timespec tv = {0, 10};
	__u32 key = 0, duration = 0;
	struct bpf_object *obj;

	err = bpf_prog_load(file, BPF_PROG_TYPE_RAW_TRACEPOINT, &obj, &prog_fd);
	if (CHECK(err, "prog_load raw tp", "err %d errno %d\n", err, errno))
		return;

	efd = bpf_raw_tracepoint_open("sys_enter", prog_fd);
	if (CHECK(efd < 0, "raw_tp_open", "err %d errno %d\n", efd, errno))
		goto close_prog;

	perfmap_fd = bpf_find_map(__func__, obj, "perfmap");
	if (CHECK(perfmap_fd < 0, "bpf_find_map", "err %d errno %d\n",
		  perfmap_fd, errno))
		goto close_prog;

	err = load_kallsyms();
	if (CHECK(err < 0, "load_kallsyms", "err %d errno %d\n", err, errno))
		goto close_prog;

	attr.sample_type = PERF_SAMPLE_RAW;
	attr.type = PERF_TYPE_SOFTWARE;
	attr.config = PERF_COUNT_SW_BPF_OUTPUT;
	pmu_fd = syscall(__NR_perf_event_open, &attr, getpid()/*pid*/, -1/*cpu*/,
			 -1/*group_fd*/, 0);
	if (CHECK(pmu_fd < 0, "perf_event_open", "err %d errno %d\n", pmu_fd,
		  errno))
		goto close_prog;

	err = bpf_map_update_elem(perfmap_fd, &key, &pmu_fd, BPF_ANY);
	if (CHECK(err < 0, "bpf_map_update_elem", "err %d errno %d\n", err,
		  errno))
		goto close_prog;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0);
	if (CHECK(err < 0, "ioctl PERF_EVENT_IOC_ENABLE", "err %d errno %d\n",
		  err, errno))
		goto close_prog;

	err = perf_event_mmap(pmu_fd);
	if (CHECK(err < 0, "perf_event_mmap", "err %d errno %d\n", err, errno))
		goto close_prog;

	/* trigger some syscall action */
	for (i = 0; i < MAX_CNT_RAWTP; i++)
		nanosleep(&tv, NULL);

	err = perf_event_poller(pmu_fd, get_stack_print_output);
	if (CHECK(err < 0, "perf_event_poller", "err %d errno %d\n", err, errno))
		goto close_prog;

	goto close_prog_noerr;
close_prog:
	error_cnt++;
close_prog_noerr:
	bpf_object__close(obj);
}

static void test_task_fd_query_rawtp(void)
{
	const char *file = "./test_get_stack_rawtp.o";
	__u64 probe_offset, probe_addr;
	__u32 len, prog_id, fd_type;
	struct bpf_object *obj;
	int efd, err, prog_fd;
	__u32 duration = 0;
	char buf[256];

	err = bpf_prog_load(file, BPF_PROG_TYPE_RAW_TRACEPOINT, &obj, &prog_fd);
	if (CHECK(err, "prog_load raw tp", "err %d errno %d\n", err, errno))
		return;

	efd = bpf_raw_tracepoint_open("sys_enter", prog_fd);
	if (CHECK(efd < 0, "raw_tp_open", "err %d errno %d\n", efd, errno))
		goto close_prog;

	/* query (getpid(), efd) */
	len = sizeof(buf);
	err = bpf_task_fd_query(getpid(), efd, 0, buf, &len, &prog_id,
				&fd_type, &probe_offset, &probe_addr);
	if (CHECK(err < 0, "bpf_task_fd_query", "err %d errno %d\n", err,
		  errno))
		goto close_prog;

	err = fd_type == BPF_FD_TYPE_RAW_TRACEPOINT &&
	      strcmp(buf, "sys_enter") == 0;
	if (CHECK(!err, "check_results", "fd_type %d tp_name %s\n",
		  fd_type, buf))
		goto close_prog;

	/* test zero len */
	len = 0;
	err = bpf_task_fd_query(getpid(), efd, 0, buf, &len, &prog_id,
				&fd_type, &probe_offset, &probe_addr);
	if (CHECK(err < 0, "bpf_task_fd_query (len = 0)", "err %d errno %d\n",
		  err, errno))
		goto close_prog;
	err = fd_type == BPF_FD_TYPE_RAW_TRACEPOINT &&
	      len == strlen("sys_enter");
	if (CHECK(!err, "check_results", "fd_type %d len %u\n", fd_type, len))
		goto close_prog;

	/* test empty buffer */
	len = sizeof(buf);
	err = bpf_task_fd_query(getpid(), efd, 0, 0, &len, &prog_id,
				&fd_type, &probe_offset, &probe_addr);
	if (CHECK(err < 0, "bpf_task_fd_query (buf = 0)", "err %d errno %d\n",
		  err, errno))
		goto close_prog;
	err = fd_type == BPF_FD_TYPE_RAW_TRACEPOINT &&
	      len == strlen("sys_enter");
	if (CHECK(!err, "check_results", "fd_type %d len %u\n", fd_type, len))
		goto close_prog;

	/* test smaller buffer */
	len = 3;
	err = bpf_task_fd_query(getpid(), efd, 0, buf, &len, &prog_id,
				&fd_type, &probe_offset, &probe_addr);
	if (CHECK(err >= 0 || errno != ENOSPC, "bpf_task_fd_query (len = 3)",
		  "err %d errno %d\n", err, errno))
		goto close_prog;
	err = fd_type == BPF_FD_TYPE_RAW_TRACEPOINT &&
	      len == strlen("sys_enter") &&
	      strcmp(buf, "sy") == 0;
	if (CHECK(!err, "check_results", "fd_type %d len %u\n", fd_type, len))
		goto close_prog;

	goto close_prog_noerr;
close_prog:
	error_cnt++;
close_prog_noerr:
	bpf_object__close(obj);
}

static void test_task_fd_query_tp_core(const char *probe_name,
				       const char *tp_name)
{
	const char *file = "./test_tracepoint.o";
	int err, bytes, efd, prog_fd, pmu_fd;
	struct perf_event_attr attr = {};
	__u64 probe_offset, probe_addr;
	__u32 len, prog_id, fd_type;
	struct bpf_object *obj;
	__u32 duration = 0;
	char buf[256];

	err = bpf_prog_load(file, BPF_PROG_TYPE_TRACEPOINT, &obj, &prog_fd);
	if (CHECK(err, "bpf_prog_load", "err %d errno %d\n", err, errno))
		goto close_prog;

	snprintf(buf, sizeof(buf),
		 "/sys/kernel/debug/tracing/events/%s/id", probe_name);
	efd = open(buf, O_RDONLY, 0);
	if (CHECK(efd < 0, "open", "err %d errno %d\n", efd, errno))
		goto close_prog;
	bytes = read(efd, buf, sizeof(buf));
	close(efd);
	if (CHECK(bytes <= 0 || bytes >= sizeof(buf), "read",
		  "bytes %d errno %d\n", bytes, errno))
		goto close_prog;

	attr.config = strtol(buf, NULL, 0);
	attr.type = PERF_TYPE_TRACEPOINT;
	attr.sample_type = PERF_SAMPLE_RAW;
	attr.sample_period = 1;
	attr.wakeup_events = 1;
	pmu_fd = syscall(__NR_perf_event_open, &attr, -1 /* pid */,
			 0 /* cpu 0 */, -1 /* group id */,
			 0 /* flags */);
	if (CHECK(err, "perf_event_open", "err %d errno %d\n", err, errno))
		goto close_pmu;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0);
	if (CHECK(err, "perf_event_ioc_enable", "err %d errno %d\n", err,
		  errno))
		goto close_pmu;

	err = ioctl(pmu_fd, PERF_EVENT_IOC_SET_BPF, prog_fd);
	if (CHECK(err, "perf_event_ioc_set_bpf", "err %d errno %d\n", err,
		  errno))
		goto close_pmu;

	/* query (getpid(), pmu_fd) */
	len = sizeof(buf);
	err = bpf_task_fd_query(getpid(), pmu_fd, 0, buf, &len, &prog_id,
				&fd_type, &probe_offset, &probe_addr);
	if (CHECK(err < 0, "bpf_task_fd_query", "err %d errno %d\n", err,
		  errno))
		goto close_pmu;

	err = (fd_type == BPF_FD_TYPE_TRACEPOINT) && !strcmp(buf, tp_name);
	if (CHECK(!err, "check_results", "fd_type %d tp_name %s\n",
		  fd_type, buf))
		goto close_pmu;

	close(pmu_fd);
	goto close_prog_noerr;

close_pmu:
	close(pmu_fd);
close_prog:
	error_cnt++;
close_prog_noerr:
	bpf_object__close(obj);
}

static void test_task_fd_query_tp(void)
{
	test_task_fd_query_tp_core("sched/sched_switch",
				   "sched_switch");
	test_task_fd_query_tp_core("syscalls/sys_enter_read",
				   "sys_enter_read");
}

int main(void)
{
	jit_enabled = is_jit_enabled();

	test_pkt_access();
	test_xdp();
	test_xdp_adjust_tail();
	test_l4lb_all();
	test_xdp_noinline();
	test_tcp_estats();
	test_bpf_obj_id();
	test_pkt_md_access();
	test_obj_name();
	test_tp_attach_query();
	test_stacktrace_map();
	test_stacktrace_build_id();
	test_stacktrace_build_id_nmi();
	test_stacktrace_map_raw_tp();
	test_get_stack_raw_tp();
	test_task_fd_query_rawtp();
	test_task_fd_query_tp();

	printf("Summary: %d PASSED, %d FAILED\n", pass_cnt, error_cnt);
	return error_cnt ? EXIT_FAILURE : EXIT_SUCCESS;
=======
	pclose(fp);
	return -1;
}

static int finit_module(int fd, const char *param_values, int flags)
{
	return syscall(__NR_finit_module, fd, param_values, flags);
}

static int delete_module(const char *name, int flags)
{
	return syscall(__NR_delete_module, name, flags);
}

/*
 * Trigger synchronize_rcu() in kernel.
 */
int kern_sync_rcu(void)
{
	return syscall(__NR_membarrier, MEMBARRIER_CMD_SHARED, 0, 0);
}

static void unload_bpf_testmod(void)
{
	if (kern_sync_rcu())
		fprintf(env.stderr, "Failed to trigger kernel-side RCU sync!\n");
	if (delete_module("bpf_testmod", 0)) {
		if (errno == ENOENT) {
			if (env.verbosity > VERBOSE_NONE)
				fprintf(stdout, "bpf_testmod.ko is already unloaded.\n");
			return;
		}
		fprintf(env.stderr, "Failed to unload bpf_testmod.ko from kernel: %d\n", -errno);
		return;
	}
	if (env.verbosity > VERBOSE_NONE)
		fprintf(stdout, "Successfully unloaded bpf_testmod.ko.\n");
}

static int load_bpf_testmod(void)
{
	int fd;

	/* ensure previous instance of the module is unloaded */
	unload_bpf_testmod();

	if (env.verbosity > VERBOSE_NONE)
		fprintf(stdout, "Loading bpf_testmod.ko...\n");

	fd = open("bpf_testmod.ko", O_RDONLY);
	if (fd < 0) {
		fprintf(env.stderr, "Can't find bpf_testmod.ko kernel module: %d\n", -errno);
		return -ENOENT;
	}
	if (finit_module(fd, "", 0)) {
		fprintf(env.stderr, "Failed to load bpf_testmod.ko into the kernel: %d\n", -errno);
		close(fd);
		return -EINVAL;
	}
	close(fd);

	if (env.verbosity > VERBOSE_NONE)
		fprintf(stdout, "Successfully loaded bpf_testmod.ko.\n");
	return 0;
}

/* extern declarations for test funcs */
#define DEFINE_TEST(name) extern void test_##name(void);
#include <prog_tests/tests.h>
#undef DEFINE_TEST

static struct prog_test_def prog_test_defs[] = {
#define DEFINE_TEST(name) {		\
	.test_name = #name,		\
	.run_test = &test_##name,	\
},
#include <prog_tests/tests.h>
#undef DEFINE_TEST
};
const int prog_test_cnt = ARRAY_SIZE(prog_test_defs);

const char *argp_program_version = "test_progs 0.1";
const char *argp_program_bug_address = "<bpf@vger.kernel.org>";
const char argp_program_doc[] = "BPF selftests test runner";

enum ARG_KEYS {
	ARG_TEST_NUM = 'n',
	ARG_TEST_NAME = 't',
	ARG_TEST_NAME_BLACKLIST = 'b',
	ARG_VERIFIER_STATS = 's',
	ARG_VERBOSE = 'v',
	ARG_GET_TEST_CNT = 'c',
	ARG_LIST_TEST_NAMES = 'l',
	ARG_TEST_NAME_GLOB_ALLOWLIST = 'a',
	ARG_TEST_NAME_GLOB_DENYLIST = 'd',
};

static const struct argp_option opts[] = {
	{ "num", ARG_TEST_NUM, "NUM", 0,
	  "Run test number NUM only " },
	{ "name", ARG_TEST_NAME, "NAMES", 0,
	  "Run tests with names containing any string from NAMES list" },
	{ "name-blacklist", ARG_TEST_NAME_BLACKLIST, "NAMES", 0,
	  "Don't run tests with names containing any string from NAMES list" },
	{ "verifier-stats", ARG_VERIFIER_STATS, NULL, 0,
	  "Output verifier statistics", },
	{ "verbose", ARG_VERBOSE, "LEVEL", OPTION_ARG_OPTIONAL,
	  "Verbose output (use -vv or -vvv for progressively verbose output)" },
	{ "count", ARG_GET_TEST_CNT, NULL, 0,
	  "Get number of selected top-level tests " },
	{ "list", ARG_LIST_TEST_NAMES, NULL, 0,
	  "List test names that would run (without running them) " },
	{ "allow", ARG_TEST_NAME_GLOB_ALLOWLIST, "NAMES", 0,
	  "Run tests with name matching the pattern (supports '*' wildcard)." },
	{ "deny", ARG_TEST_NAME_GLOB_DENYLIST, "NAMES", 0,
	  "Don't run tests with name matching the pattern (supports '*' wildcard)." },
	{},
};

static int libbpf_print_fn(enum libbpf_print_level level,
			   const char *format, va_list args)
{
	if (env.verbosity < VERBOSE_VERY && level == LIBBPF_DEBUG)
		return 0;
	vfprintf(stdout, format, args);
	return 0;
}

static void free_str_set(const struct str_set *set)
{
	int i;

	if (!set)
		return;

	for (i = 0; i < set->cnt; i++)
		free((void *)set->strs[i]);
	free(set->strs);
}

static int parse_str_list(const char *s, struct str_set *set, bool is_glob_pattern)
{
	char *input, *state = NULL, *next, **tmp, **strs = NULL;
	int i, cnt = 0;

	input = strdup(s);
	if (!input)
		return -ENOMEM;

	while ((next = strtok_r(state ? NULL : input, ",", &state))) {
		tmp = realloc(strs, sizeof(*strs) * (cnt + 1));
		if (!tmp)
			goto err;
		strs = tmp;

		if (is_glob_pattern) {
			strs[cnt] = strdup(next);
			if (!strs[cnt])
				goto err;
		} else {
			strs[cnt] = malloc(strlen(next) + 2 + 1);
			if (!strs[cnt])
				goto err;
			sprintf(strs[cnt], "*%s*", next);
		}

		cnt++;
	}

	tmp = realloc(set->strs, sizeof(*strs) * (cnt + set->cnt));
	if (!tmp)
		goto err;
	memcpy(tmp + set->cnt, strs, sizeof(*strs) * cnt);
	set->strs = (const char **)tmp;
	set->cnt += cnt;

	free(input);
	free(strs);
	return 0;
err:
	for (i = 0; i < cnt; i++)
		free(strs[i]);
	free(strs);
	free(input);
	return -ENOMEM;
}

extern int extra_prog_load_log_flags;

static error_t parse_arg(int key, char *arg, struct argp_state *state)
{
	struct test_env *env = state->input;

	switch (key) {
	case ARG_TEST_NUM: {
		char *subtest_str = strchr(arg, '/');

		if (subtest_str) {
			*subtest_str = '\0';
			if (parse_num_list(subtest_str + 1,
					   &env->subtest_selector.num_set,
					   &env->subtest_selector.num_set_len)) {
				fprintf(stderr,
					"Failed to parse subtest numbers.\n");
				return -EINVAL;
			}
		}
		if (parse_num_list(arg, &env->test_selector.num_set,
				   &env->test_selector.num_set_len)) {
			fprintf(stderr, "Failed to parse test numbers.\n");
			return -EINVAL;
		}
		break;
	}
	case ARG_TEST_NAME_GLOB_ALLOWLIST:
	case ARG_TEST_NAME: {
		char *subtest_str = strchr(arg, '/');

		if (subtest_str) {
			*subtest_str = '\0';
			if (parse_str_list(subtest_str + 1,
					   &env->subtest_selector.whitelist,
					   key == ARG_TEST_NAME_GLOB_ALLOWLIST))
				return -ENOMEM;
		}
		if (parse_str_list(arg, &env->test_selector.whitelist,
				   key == ARG_TEST_NAME_GLOB_ALLOWLIST))
			return -ENOMEM;
		break;
	}
	case ARG_TEST_NAME_GLOB_DENYLIST:
	case ARG_TEST_NAME_BLACKLIST: {
		char *subtest_str = strchr(arg, '/');

		if (subtest_str) {
			*subtest_str = '\0';
			if (parse_str_list(subtest_str + 1,
					   &env->subtest_selector.blacklist,
					   key == ARG_TEST_NAME_GLOB_DENYLIST))
				return -ENOMEM;
		}
		if (parse_str_list(arg, &env->test_selector.blacklist,
				   key == ARG_TEST_NAME_GLOB_DENYLIST))
			return -ENOMEM;
		break;
	}
	case ARG_VERIFIER_STATS:
		env->verifier_stats = true;
		break;
	case ARG_VERBOSE:
		env->verbosity = VERBOSE_NORMAL;
		if (arg) {
			if (strcmp(arg, "v") == 0) {
				env->verbosity = VERBOSE_VERY;
				extra_prog_load_log_flags = 1;
			} else if (strcmp(arg, "vv") == 0) {
				env->verbosity = VERBOSE_SUPER;
				extra_prog_load_log_flags = 2;
			} else {
				fprintf(stderr,
					"Unrecognized verbosity setting ('%s'), only -v and -vv are supported\n",
					arg);
				return -EINVAL;
			}
		}

		if (env->verbosity > VERBOSE_NONE) {
			if (setenv("SELFTESTS_VERBOSE", "1", 1) == -1) {
				fprintf(stderr,
					"Unable to setenv SELFTESTS_VERBOSE=1 (errno=%d)",
					errno);
				return -1;
			}
		}

		break;
	case ARG_GET_TEST_CNT:
		env->get_test_cnt = true;
		break;
	case ARG_LIST_TEST_NAMES:
		env->list_test_names = true;
		break;
	case ARGP_KEY_ARG:
		argp_usage(state);
		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static void stdio_hijack(void)
{
#ifdef __GLIBC__
	env.stdout = stdout;
	env.stderr = stderr;

	if (env.verbosity > VERBOSE_NONE) {
		/* nothing to do, output to stdout by default */
		return;
	}

	/* stdout and stderr -> buffer */
	fflush(stdout);

	stdout = open_memstream(&env.log_buf, &env.log_cnt);
	if (!stdout) {
		stdout = env.stdout;
		perror("open_memstream");
		return;
	}

	stderr = stdout;
#endif
}

static void stdio_restore(void)
{
#ifdef __GLIBC__
	if (stdout == env.stdout)
		return;

	fclose(stdout);
	free(env.log_buf);

	env.log_buf = NULL;
	env.log_cnt = 0;

	stdout = env.stdout;
	stderr = env.stderr;
#endif
}

/*
 * Determine if test_progs is running as a "flavored" test runner and switch
 * into corresponding sub-directory to load correct BPF objects.
 *
 * This is done by looking at executable name. If it contains "-flavor"
 * suffix, then we are running as a flavored test runner.
 */
int cd_flavor_subdir(const char *exec_name)
{
	/* General form of argv[0] passed here is:
	 * some/path/to/test_progs[-flavor], where -flavor part is optional.
	 * First cut out "test_progs[-flavor]" part, then extract "flavor"
	 * part, if it's there.
	 */
	const char *flavor = strrchr(exec_name, '/');

	if (!flavor)
		return 0;
	flavor++;
	flavor = strrchr(flavor, '-');
	if (!flavor)
		return 0;
	flavor++;
	if (env.verbosity > VERBOSE_NONE)
		fprintf(stdout,	"Switching to flavor '%s' subdirectory...\n", flavor);

	return chdir(flavor);
}

#define MAX_BACKTRACE_SZ 128
void crash_handler(int signum)
{
	void *bt[MAX_BACKTRACE_SZ];
	size_t sz;

	sz = backtrace(bt, ARRAY_SIZE(bt));

	if (env.test)
		dump_test_log(env.test, true);
	if (env.stdout)
		stdio_restore();

	fprintf(stderr, "Caught signal #%d!\nStack trace:\n", signum);
	backtrace_symbols_fd(bt, sz, STDERR_FILENO);
}

int main(int argc, char **argv)
{
	static const struct argp argp = {
		.options = opts,
		.parser = parse_arg,
		.doc = argp_program_doc,
	};
	struct sigaction sigact = {
		.sa_handler = crash_handler,
		.sa_flags = SA_RESETHAND,
	};
	int err, i;

	sigaction(SIGSEGV, &sigact, NULL);

	err = argp_parse(&argp, argc, argv, 0, NULL, &env);
	if (err)
		return err;

	err = cd_flavor_subdir(argv[0]);
	if (err)
		return err;

	/* Use libbpf 1.0 API mode */
	libbpf_set_strict_mode(LIBBPF_STRICT_ALL);

	libbpf_set_print(libbpf_print_fn);

	srand(time(NULL));

	env.jit_enabled = is_jit_enabled();
	env.nr_cpus = libbpf_num_possible_cpus();
	if (env.nr_cpus < 0) {
		fprintf(stderr, "Failed to get number of CPUs: %d!\n",
			env.nr_cpus);
		return -1;
	}

	save_netns();
	stdio_hijack();
	env.has_testmod = true;
	if (!env.list_test_names && load_bpf_testmod()) {
		fprintf(env.stderr, "WARNING! Selftests relying on bpf_testmod.ko will be skipped.\n");
		env.has_testmod = false;
	}
	for (i = 0; i < prog_test_cnt; i++) {
		struct prog_test_def *test = &prog_test_defs[i];

		env.test = test;
		test->test_num = i + 1;

		if (!should_run(&env.test_selector,
				test->test_num, test->test_name))
			continue;

		if (env.get_test_cnt) {
			env.succ_cnt++;
			continue;
		}

		if (env.list_test_names) {
			fprintf(env.stdout, "%s\n", test->test_name);
			env.succ_cnt++;
			continue;
		}

		test->run_test();
		/* ensure last sub-test is finalized properly */
		if (test->subtest_name)
			test__end_subtest();

		test->tested = true;

		dump_test_log(test, test->error_cnt);

		fprintf(env.stdout, "#%d %s:%s\n",
			test->test_num, test->test_name,
			test->error_cnt ? "FAIL" : (test->skip_cnt ? "SKIP" : "OK"));

		if (test->error_cnt)
			env.fail_cnt++;
		else
			env.succ_cnt++;
		skip_account();

		reset_affinity();
		restore_netns();
		if (test->need_cgroup_cleanup)
			cleanup_cgroup_environment();
	}
	if (!env.list_test_names && env.has_testmod)
		unload_bpf_testmod();
	stdio_restore();

	if (env.get_test_cnt) {
		printf("%d\n", env.succ_cnt);
		goto out;
	}

	if (env.list_test_names)
		goto out;

	fprintf(stdout, "Summary: %d/%d PASSED, %d SKIPPED, %d FAILED\n",
		env.succ_cnt, env.sub_succ_cnt, env.skip_cnt, env.fail_cnt);

out:
	free_str_set(&env.test_selector.blacklist);
	free_str_set(&env.test_selector.whitelist);
	free(env.test_selector.num_set);
	free_str_set(&env.subtest_selector.blacklist);
	free_str_set(&env.subtest_selector.whitelist);
	free(env.subtest_selector.num_set);
	close(env.saved_netns_fd);

	if (env.succ_cnt + env.fail_cnt + env.skip_cnt == 0)
		return EXIT_NO_TEST;

	return env.fail_cnt ? EXIT_FAILURE : EXIT_SUCCESS;
>>>>>>> upstream/android-13
}
