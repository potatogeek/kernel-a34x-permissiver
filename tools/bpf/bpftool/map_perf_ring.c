<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0-only
=======
// SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause)
>>>>>>> upstream/android-13
/* Copyright (C) 2018 Netronome Systems, Inc. */
/* This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 */
#include <errno.h>
#include <fcntl.h>
<<<<<<< HEAD
#include <libbpf.h>
=======
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13
#include <poll.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <linux/bpf.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>

<<<<<<< HEAD
#include <bpf.h>
=======
#include <bpf/bpf.h>
>>>>>>> upstream/android-13
#include <perf-sys.h>

#include "main.h"

#define MMAP_PAGE_CNT	16

<<<<<<< HEAD
static bool stop;
=======
static volatile bool stop;
>>>>>>> upstream/android-13

struct event_ring_info {
	int fd;
	int key;
	unsigned int cpu;
	void *mem;
};

struct perf_event_sample {
	struct perf_event_header header;
<<<<<<< HEAD
	u64 time;
=======
	__u64 time;
>>>>>>> upstream/android-13
	__u32 size;
	unsigned char data[];
};

<<<<<<< HEAD
=======
struct perf_event_lost {
	struct perf_event_header header;
	__u64 id;
	__u64 lost;
};

>>>>>>> upstream/android-13
static void int_exit(int signo)
{
	fprintf(stderr, "Stopping...\n");
	stop = true;
}

<<<<<<< HEAD
static enum bpf_perf_event_ret print_bpf_output(void *event, void *priv)
{
	struct event_ring_info *ring = priv;
	struct perf_event_sample *e = event;
	struct {
		struct perf_event_header header;
		__u64 id;
		__u64 lost;
	} *lost = event;
=======
struct event_pipe_ctx {
	bool all_cpus;
	int cpu;
	int idx;
};

static enum bpf_perf_event_ret
print_bpf_output(void *private_data, int cpu, struct perf_event_header *event)
{
	struct perf_event_sample *e = container_of(event,
						   struct perf_event_sample,
						   header);
	struct perf_event_lost *lost = container_of(event,
						    struct perf_event_lost,
						    header);
	struct event_pipe_ctx *ctx = private_data;
	int idx = ctx->all_cpus ? cpu : ctx->idx;
>>>>>>> upstream/android-13

	if (json_output) {
		jsonw_start_object(json_wtr);
		jsonw_name(json_wtr, "type");
		jsonw_uint(json_wtr, e->header.type);
		jsonw_name(json_wtr, "cpu");
<<<<<<< HEAD
		jsonw_uint(json_wtr, ring->cpu);
		jsonw_name(json_wtr, "index");
		jsonw_uint(json_wtr, ring->key);
=======
		jsonw_uint(json_wtr, cpu);
		jsonw_name(json_wtr, "index");
		jsonw_uint(json_wtr, idx);
>>>>>>> upstream/android-13
		if (e->header.type == PERF_RECORD_SAMPLE) {
			jsonw_name(json_wtr, "timestamp");
			jsonw_uint(json_wtr, e->time);
			jsonw_name(json_wtr, "data");
			print_data_json(e->data, e->size);
		} else if (e->header.type == PERF_RECORD_LOST) {
			jsonw_name(json_wtr, "lost");
			jsonw_start_object(json_wtr);
			jsonw_name(json_wtr, "id");
			jsonw_uint(json_wtr, lost->id);
			jsonw_name(json_wtr, "count");
			jsonw_uint(json_wtr, lost->lost);
			jsonw_end_object(json_wtr);
		}
		jsonw_end_object(json_wtr);
	} else {
		if (e->header.type == PERF_RECORD_SAMPLE) {
			printf("== @%lld.%09lld CPU: %d index: %d =====\n",
			       e->time / 1000000000ULL, e->time % 1000000000ULL,
<<<<<<< HEAD
			       ring->cpu, ring->key);
=======
			       cpu, idx);
>>>>>>> upstream/android-13
			fprint_hex(stdout, e->data, e->size, " ");
			printf("\n");
		} else if (e->header.type == PERF_RECORD_LOST) {
			printf("lost %lld events\n", lost->lost);
		} else {
			printf("unknown event type=%d size=%d\n",
			       e->header.type, e->header.size);
		}
	}

	return LIBBPF_PERF_EVENT_CONT;
}

<<<<<<< HEAD
static void
perf_event_read(struct event_ring_info *ring, void **buf, size_t *buf_len)
{
	enum bpf_perf_event_ret ret;

	ret = bpf_perf_event_read_simple(ring->mem,
					 MMAP_PAGE_CNT * get_page_size(),
					 get_page_size(), buf, buf_len,
					 print_bpf_output, ring);
	if (ret != LIBBPF_PERF_EVENT_CONT) {
		fprintf(stderr, "perf read loop failed with %d\n", ret);
		stop = true;
	}
}

static int perf_mmap_size(void)
{
	return get_page_size() * (MMAP_PAGE_CNT + 1);
}

static void *perf_event_mmap(int fd)
{
	int mmap_size = perf_mmap_size();
	void *base;

	base = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (base == MAP_FAILED) {
		p_err("event mmap failed: %s\n", strerror(errno));
		return NULL;
	}

	return base;
}

static void perf_event_unmap(void *mem)
{
	if (munmap(mem, perf_mmap_size()))
		fprintf(stderr, "Can't unmap ring memory!\n");
}

static int bpf_perf_event_open(int map_fd, int key, int cpu)
{
	struct perf_event_attr attr = {
		.sample_type = PERF_SAMPLE_RAW | PERF_SAMPLE_TIME,
		.type = PERF_TYPE_SOFTWARE,
		.config = PERF_COUNT_SW_BPF_OUTPUT,
	};
	int pmu_fd;

	pmu_fd = sys_perf_event_open(&attr, -1, cpu, -1, 0);
	if (pmu_fd < 0) {
		p_err("failed to open perf event %d for CPU %d", key, cpu);
		return -1;
	}

	if (bpf_map_update_elem(map_fd, &key, &pmu_fd, BPF_ANY)) {
		p_err("failed to update map for event %d for CPU %d", key, cpu);
		goto err_close;
	}
	if (ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0)) {
		p_err("failed to enable event %d for CPU %d", key, cpu);
		goto err_close;
	}

	return pmu_fd;

err_close:
	close(pmu_fd);
	return -1;
}

int do_event_pipe(int argc, char **argv)
{
	int i, nfds, map_fd, index = -1, cpu = -1;
	struct bpf_map_info map_info = {};
	struct event_ring_info *rings;
	size_t tmp_buf_sz = 0;
	void *tmp_buf = NULL;
	struct pollfd *pfds;
	__u32 map_info_len;
	bool do_all = true;
=======
int do_event_pipe(int argc, char **argv)
{
	struct perf_event_attr perf_attr = {
		.sample_type = PERF_SAMPLE_RAW | PERF_SAMPLE_TIME,
		.type = PERF_TYPE_SOFTWARE,
		.config = PERF_COUNT_SW_BPF_OUTPUT,
		.sample_period = 1,
		.wakeup_events = 1,
	};
	struct bpf_map_info map_info = {};
	struct perf_buffer_raw_opts opts = {};
	struct event_pipe_ctx ctx = {
		.all_cpus = true,
		.cpu = -1,
		.idx = -1,
	};
	struct perf_buffer *pb;
	__u32 map_info_len;
	int err, map_fd;
>>>>>>> upstream/android-13

	map_info_len = sizeof(map_info);
	map_fd = map_parse_fd_and_info(&argc, &argv, &map_info, &map_info_len);
	if (map_fd < 0)
		return -1;

	if (map_info.type != BPF_MAP_TYPE_PERF_EVENT_ARRAY) {
		p_err("map is not a perf event array");
		goto err_close_map;
	}

	while (argc) {
		if (argc < 2) {
			BAD_ARG();
			goto err_close_map;
		}

		if (is_prefix(*argv, "cpu")) {
			char *endptr;

			NEXT_ARG();
<<<<<<< HEAD
			cpu = strtoul(*argv, &endptr, 0);
=======
			ctx.cpu = strtoul(*argv, &endptr, 0);
>>>>>>> upstream/android-13
			if (*endptr) {
				p_err("can't parse %s as CPU ID", *argv);
				goto err_close_map;
			}

			NEXT_ARG();
		} else if (is_prefix(*argv, "index")) {
			char *endptr;

			NEXT_ARG();
<<<<<<< HEAD
			index = strtoul(*argv, &endptr, 0);
=======
			ctx.idx = strtoul(*argv, &endptr, 0);
>>>>>>> upstream/android-13
			if (*endptr) {
				p_err("can't parse %s as index", *argv);
				goto err_close_map;
			}

			NEXT_ARG();
		} else {
			BAD_ARG();
			goto err_close_map;
		}

<<<<<<< HEAD
		do_all = false;
	}

	if (!do_all) {
		if (index == -1 || cpu == -1) {
			p_err("cpu and index must be specified together");
			goto err_close_map;
		}

		nfds = 1;
	} else {
		nfds = min(get_possible_cpus(), map_info.max_entries);
		cpu = 0;
		index = 0;
	}

	rings = calloc(nfds, sizeof(rings[0]));
	if (!rings)
		goto err_close_map;

	pfds = calloc(nfds, sizeof(pfds[0]));
	if (!pfds)
		goto err_free_rings;

	for (i = 0; i < nfds; i++) {
		rings[i].cpu = cpu + i;
		rings[i].key = index + i;

		rings[i].fd = bpf_perf_event_open(map_fd, rings[i].key,
						  rings[i].cpu);
		if (rings[i].fd < 0)
			goto err_close_fds_prev;

		rings[i].mem = perf_event_mmap(rings[i].fd);
		if (!rings[i].mem)
			goto err_close_fds_current;

		pfds[i].fd = rings[i].fd;
		pfds[i].events = POLLIN;
=======
		ctx.all_cpus = false;
	}

	if (!ctx.all_cpus) {
		if (ctx.idx == -1 || ctx.cpu == -1) {
			p_err("cpu and index must be specified together");
			goto err_close_map;
		}
	} else {
		ctx.cpu = 0;
		ctx.idx = 0;
	}

	opts.attr = &perf_attr;
	opts.event_cb = print_bpf_output;
	opts.ctx = &ctx;
	opts.cpu_cnt = ctx.all_cpus ? 0 : 1;
	opts.cpus = &ctx.cpu;
	opts.map_keys = &ctx.idx;

	pb = perf_buffer__new_raw(map_fd, MMAP_PAGE_CNT, &opts);
	err = libbpf_get_error(pb);
	if (err) {
		p_err("failed to create perf buffer: %s (%d)",
		      strerror(err), err);
		goto err_close_map;
>>>>>>> upstream/android-13
	}

	signal(SIGINT, int_exit);
	signal(SIGHUP, int_exit);
	signal(SIGTERM, int_exit);

	if (json_output)
		jsonw_start_array(json_wtr);

	while (!stop) {
<<<<<<< HEAD
		poll(pfds, nfds, 200);
		for (i = 0; i < nfds; i++)
			perf_event_read(&rings[i], &tmp_buf, &tmp_buf_sz);
	}
	free(tmp_buf);
=======
		err = perf_buffer__poll(pb, 200);
		if (err < 0 && err != -EINTR) {
			p_err("perf buffer polling failed: %s (%d)",
			      strerror(err), err);
			goto err_close_pb;
		}
	}
>>>>>>> upstream/android-13

	if (json_output)
		jsonw_end_array(json_wtr);

<<<<<<< HEAD
	for (i = 0; i < nfds; i++) {
		perf_event_unmap(rings[i].mem);
		close(rings[i].fd);
	}
	free(pfds);
	free(rings);
=======
	perf_buffer__free(pb);
>>>>>>> upstream/android-13
	close(map_fd);

	return 0;

<<<<<<< HEAD
err_close_fds_prev:
	while (i--) {
		perf_event_unmap(rings[i].mem);
err_close_fds_current:
		close(rings[i].fd);
	}
	free(pfds);
err_free_rings:
	free(rings);
=======
err_close_pb:
	perf_buffer__free(pb);
>>>>>>> upstream/android-13
err_close_map:
	close(map_fd);
	return -1;
}
