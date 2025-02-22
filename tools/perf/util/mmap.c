<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2011-2017, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
 *
 * Parts came from evlist.c builtin-{top,stat,record}.c, see those files for further
 * copyright notes.
<<<<<<< HEAD
 *
 * Released under the GPL v2. (and only v2, not any later version)
=======
>>>>>>> upstream/android-13
 */

#include <sys/mman.h>
#include <inttypes.h>
#include <asm/bug.h>
<<<<<<< HEAD
#include "debug.h"
#include "event.h"
#include "mmap.h"
#include "util.h" /* page_size */

size_t perf_mmap__mmap_len(struct perf_mmap *map)
{
	return map->mask + 1 + page_size;
}

/* When check_messup is true, 'end' must points to a good entry */
static union perf_event *perf_mmap__read(struct perf_mmap *map,
					 u64 *startp, u64 end)
{
	unsigned char *data = map->base + page_size;
	union perf_event *event = NULL;
	int diff = end - *startp;

	if (diff >= (int)sizeof(event->header)) {
		size_t size;

		event = (union perf_event *)&data[*startp & map->mask];
		size = event->header.size;

		if (size < sizeof(event->header) || diff < (int)size)
			return NULL;

		/*
		 * Event straddles the mmap boundary -- header should always
		 * be inside due to u64 alignment of output.
		 */
		if ((*startp & map->mask) + size != ((*startp + size) & map->mask)) {
			unsigned int offset = *startp;
			unsigned int len = min(sizeof(*event), size), cpy;
			void *dst = map->event_copy;

			do {
				cpy = min(map->mask + 1 - (offset & map->mask), len);
				memcpy(dst, &data[offset & map->mask], cpy);
				offset += cpy;
				dst += cpy;
				len -= cpy;
			} while (len);

			event = (union perf_event *)map->event_copy;
		}

		*startp += size;
	}

	return event;
}

/*
 * Read event from ring buffer one by one.
 * Return one event for each call.
 *
 * Usage:
 * perf_mmap__read_init()
 * while(event = perf_mmap__read_event()) {
 *	//process the event
 *	perf_mmap__consume()
 * }
 * perf_mmap__read_done()
 */
union perf_event *perf_mmap__read_event(struct perf_mmap *map)
{
	union perf_event *event;

	/*
	 * Check if event was unmapped due to a POLLHUP/POLLERR.
	 */
	if (!refcount_read(&map->refcnt))
		return NULL;

	/* non-overwirte doesn't pause the ringbuffer */
	if (!map->overwrite)
		map->end = perf_mmap__read_head(map);

	event = perf_mmap__read(map, &map->start, map->end);

	if (!map->overwrite)
		map->prev = map->start;

	return event;
}

static bool perf_mmap__empty(struct perf_mmap *map)
{
	return perf_mmap__read_head(map) == map->prev && !map->auxtrace_mmap.base;
}

void perf_mmap__get(struct perf_mmap *map)
{
	refcount_inc(&map->refcnt);
}

void perf_mmap__put(struct perf_mmap *map)
{
	BUG_ON(map->base && refcount_read(&map->refcnt) == 0);

	if (refcount_dec_and_test(&map->refcnt))
		perf_mmap__munmap(map);
}

void perf_mmap__consume(struct perf_mmap *map)
{
	if (!map->overwrite) {
		u64 old = map->prev;

		perf_mmap__write_tail(map, old);
	}

	if (refcount_read(&map->refcnt) == 1 && perf_mmap__empty(map))
		perf_mmap__put(map);
=======
#include <linux/zalloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sysconf()
#include <perf/mmap.h>
#ifdef HAVE_LIBNUMA_SUPPORT
#include <numaif.h>
#endif
#include "cpumap.h"
#include "debug.h"
#include "event.h"
#include "mmap.h"
#include "../perf.h"
#include <internal/lib.h> /* page_size */
#include <linux/bitmap.h>

#define MASK_SIZE 1023
void mmap_cpu_mask__scnprintf(struct mmap_cpu_mask *mask, const char *tag)
{
	char buf[MASK_SIZE + 1];
	size_t len;

	len = bitmap_scnprintf(mask->bits, mask->nbits, buf, MASK_SIZE);
	buf[len] = '\0';
	pr_debug("%p: %s mask[%zd]: %s\n", mask, tag, mask->nbits, buf);
}

size_t mmap__mmap_len(struct mmap *map)
{
	return perf_mmap__mmap_len(&map->core);
>>>>>>> upstream/android-13
}

int __weak auxtrace_mmap__mmap(struct auxtrace_mmap *mm __maybe_unused,
			       struct auxtrace_mmap_params *mp __maybe_unused,
			       void *userpg __maybe_unused,
			       int fd __maybe_unused)
{
	return 0;
}

void __weak auxtrace_mmap__munmap(struct auxtrace_mmap *mm __maybe_unused)
{
}

void __weak auxtrace_mmap_params__init(struct auxtrace_mmap_params *mp __maybe_unused,
				       off_t auxtrace_offset __maybe_unused,
				       unsigned int auxtrace_pages __maybe_unused,
				       bool auxtrace_overwrite __maybe_unused)
{
}

void __weak auxtrace_mmap_params__set_idx(struct auxtrace_mmap_params *mp __maybe_unused,
<<<<<<< HEAD
					  struct perf_evlist *evlist __maybe_unused,
=======
					  struct evlist *evlist __maybe_unused,
>>>>>>> upstream/android-13
					  int idx __maybe_unused,
					  bool per_cpu __maybe_unused)
{
}

<<<<<<< HEAD
void perf_mmap__munmap(struct perf_mmap *map)
{
	if (map->base != NULL) {
		munmap(map->base, perf_mmap__mmap_len(map));
		map->base = NULL;
		map->fd = -1;
		refcount_set(&map->refcnt, 0);
=======
#ifdef HAVE_AIO_SUPPORT
static int perf_mmap__aio_enabled(struct mmap *map)
{
	return map->aio.nr_cblocks > 0;
}

#ifdef HAVE_LIBNUMA_SUPPORT
static int perf_mmap__aio_alloc(struct mmap *map, int idx)
{
	map->aio.data[idx] = mmap(NULL, mmap__mmap_len(map), PROT_READ|PROT_WRITE,
				  MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
	if (map->aio.data[idx] == MAP_FAILED) {
		map->aio.data[idx] = NULL;
		return -1;
	}

	return 0;
}

static void perf_mmap__aio_free(struct mmap *map, int idx)
{
	if (map->aio.data[idx]) {
		munmap(map->aio.data[idx], mmap__mmap_len(map));
		map->aio.data[idx] = NULL;
	}
}

static int perf_mmap__aio_bind(struct mmap *map, int idx, int cpu, int affinity)
{
	void *data;
	size_t mmap_len;
	unsigned long *node_mask;
	unsigned long node_index;
	int err = 0;

	if (affinity != PERF_AFFINITY_SYS && cpu__max_node() > 1) {
		data = map->aio.data[idx];
		mmap_len = mmap__mmap_len(map);
		node_index = cpu__get_node(cpu);
		node_mask = bitmap_zalloc(node_index + 1);
		if (!node_mask) {
			pr_err("Failed to allocate node mask for mbind: error %m\n");
			return -1;
		}
		set_bit(node_index, node_mask);
		if (mbind(data, mmap_len, MPOL_BIND, node_mask, node_index + 1 + 1, 0)) {
			pr_err("Failed to bind [%p-%p] AIO buffer to node %lu: error %m\n",
				data, data + mmap_len, node_index);
			err = -1;
		}
		bitmap_free(node_mask);
	}

	return err;
}
#else /* !HAVE_LIBNUMA_SUPPORT */
static int perf_mmap__aio_alloc(struct mmap *map, int idx)
{
	map->aio.data[idx] = malloc(mmap__mmap_len(map));
	if (map->aio.data[idx] == NULL)
		return -1;

	return 0;
}

static void perf_mmap__aio_free(struct mmap *map, int idx)
{
	zfree(&(map->aio.data[idx]));
}

static int perf_mmap__aio_bind(struct mmap *map __maybe_unused, int idx __maybe_unused,
		int cpu __maybe_unused, int affinity __maybe_unused)
{
	return 0;
}
#endif

static int perf_mmap__aio_mmap(struct mmap *map, struct mmap_params *mp)
{
	int delta_max, i, prio, ret;

	map->aio.nr_cblocks = mp->nr_cblocks;
	if (map->aio.nr_cblocks) {
		map->aio.aiocb = calloc(map->aio.nr_cblocks, sizeof(struct aiocb *));
		if (!map->aio.aiocb) {
			pr_debug2("failed to allocate aiocb for data buffer, error %m\n");
			return -1;
		}
		map->aio.cblocks = calloc(map->aio.nr_cblocks, sizeof(struct aiocb));
		if (!map->aio.cblocks) {
			pr_debug2("failed to allocate cblocks for data buffer, error %m\n");
			return -1;
		}
		map->aio.data = calloc(map->aio.nr_cblocks, sizeof(void *));
		if (!map->aio.data) {
			pr_debug2("failed to allocate data buffer, error %m\n");
			return -1;
		}
		delta_max = sysconf(_SC_AIO_PRIO_DELTA_MAX);
		for (i = 0; i < map->aio.nr_cblocks; ++i) {
			ret = perf_mmap__aio_alloc(map, i);
			if (ret == -1) {
				pr_debug2("failed to allocate data buffer area, error %m");
				return -1;
			}
			ret = perf_mmap__aio_bind(map, i, map->core.cpu, mp->affinity);
			if (ret == -1)
				return -1;
			/*
			 * Use cblock.aio_fildes value different from -1
			 * to denote started aio write operation on the
			 * cblock so it requires explicit record__aio_sync()
			 * call prior the cblock may be reused again.
			 */
			map->aio.cblocks[i].aio_fildes = -1;
			/*
			 * Allocate cblocks with priority delta to have
			 * faster aio write system calls because queued requests
			 * are kept in separate per-prio queues and adding
			 * a new request will iterate thru shorter per-prio
			 * list. Blocks with numbers higher than
			 *  _SC_AIO_PRIO_DELTA_MAX go with priority 0.
			 */
			prio = delta_max - i;
			map->aio.cblocks[i].aio_reqprio = prio >= 0 ? prio : 0;
		}
	}

	return 0;
}

static void perf_mmap__aio_munmap(struct mmap *map)
{
	int i;

	for (i = 0; i < map->aio.nr_cblocks; ++i)
		perf_mmap__aio_free(map, i);
	if (map->aio.data)
		zfree(&map->aio.data);
	zfree(&map->aio.cblocks);
	zfree(&map->aio.aiocb);
}
#else /* !HAVE_AIO_SUPPORT */
static int perf_mmap__aio_enabled(struct mmap *map __maybe_unused)
{
	return 0;
}

static int perf_mmap__aio_mmap(struct mmap *map __maybe_unused,
			       struct mmap_params *mp __maybe_unused)
{
	return 0;
}

static void perf_mmap__aio_munmap(struct mmap *map __maybe_unused)
{
}
#endif

void mmap__munmap(struct mmap *map)
{
	bitmap_free(map->affinity_mask.bits);

	perf_mmap__aio_munmap(map);
	if (map->data != NULL) {
		munmap(map->data, mmap__mmap_len(map));
		map->data = NULL;
>>>>>>> upstream/android-13
	}
	auxtrace_mmap__munmap(&map->auxtrace_mmap);
}

<<<<<<< HEAD
int perf_mmap__mmap(struct perf_mmap *map, struct mmap_params *mp, int fd, int cpu)
{
	/*
	 * The last one will be done at perf_mmap__consume(), so that we
	 * make sure we don't prevent tools from consuming every last event in
	 * the ring buffer.
	 *
	 * I.e. we can get the POLLHUP meaning that the fd doesn't exist
	 * anymore, but the last events for it are still in the ring buffer,
	 * waiting to be consumed.
	 *
	 * Tools can chose to ignore this at their own discretion, but the
	 * evlist layer can't just drop it when filtering events in
	 * perf_evlist__filter_pollfd().
	 */
	refcount_set(&map->refcnt, 2);
	map->prev = 0;
	map->mask = mp->mask;
	map->base = mmap(NULL, perf_mmap__mmap_len(map), mp->prot,
			 MAP_SHARED, fd, 0);
	if (map->base == MAP_FAILED) {
		pr_debug2("failed to mmap perf event ring buffer, error %d\n",
			  errno);
		map->base = NULL;
		return -1;
	}
	map->fd = fd;
	map->cpu = cpu;

	if (auxtrace_mmap__mmap(&map->auxtrace_mmap,
				&mp->auxtrace_mp, map->base, fd))
		return -1;

	return 0;
}

static int overwrite_rb_find_range(void *buf, int mask, u64 *start, u64 *end)
{
	struct perf_event_header *pheader;
	u64 evt_head = *start;
	int size = mask + 1;

	pr_debug2("%s: buf=%p, start=%"PRIx64"\n", __func__, buf, *start);
	pheader = (struct perf_event_header *)(buf + (*start & mask));
	while (true) {
		if (evt_head - *start >= (unsigned int)size) {
			pr_debug("Finished reading overwrite ring buffer: rewind\n");
			if (evt_head - *start > (unsigned int)size)
				evt_head -= pheader->size;
			*end = evt_head;
			return 0;
		}

		pheader = (struct perf_event_header *)(buf + (evt_head & mask));

		if (pheader->size == 0) {
			pr_debug("Finished reading overwrite ring buffer: get start\n");
			*end = evt_head;
			return 0;
		}

		evt_head += pheader->size;
		pr_debug3("move evt_head: %"PRIx64"\n", evt_head);
	}
	WARN_ONCE(1, "Shouldn't get here\n");
	return -1;
}

/*
 * Report the start and end of the available data in ringbuffer
 */
static int __perf_mmap__read_init(struct perf_mmap *md)
{
	u64 head = perf_mmap__read_head(md);
	u64 old = md->prev;
	unsigned char *data = md->base + page_size;
	unsigned long size;

	md->start = md->overwrite ? head : old;
	md->end = md->overwrite ? old : head;

	if (md->start == md->end)
		return -EAGAIN;

	size = md->end - md->start;
	if (size > (unsigned long)(md->mask) + 1) {
		if (!md->overwrite) {
			WARN_ONCE(1, "failed to keep up with mmap data. (warn only once)\n");

			md->prev = head;
			perf_mmap__consume(md);
			return -EAGAIN;
		}

		/*
		 * Backward ring buffer is full. We still have a chance to read
		 * most of data from it.
		 */
		if (overwrite_rb_find_range(data, md->mask, &md->start, &md->end))
			return -EINVAL;
	}

	return 0;
}

int perf_mmap__read_init(struct perf_mmap *map)
{
	/*
	 * Check if event was unmapped due to a POLLHUP/POLLERR.
	 */
	if (!refcount_read(&map->refcnt))
		return -ENOENT;

	return __perf_mmap__read_init(map);
}

int perf_mmap__push(struct perf_mmap *md, void *to,
		    int push(void *to, void *buf, size_t size))
{
	u64 head = perf_mmap__read_head(md);
	unsigned char *data = md->base + page_size;
=======
static void build_node_mask(int node, struct mmap_cpu_mask *mask)
{
	int c, cpu, nr_cpus;
	const struct perf_cpu_map *cpu_map = NULL;

	cpu_map = cpu_map__online();
	if (!cpu_map)
		return;

	nr_cpus = perf_cpu_map__nr(cpu_map);
	for (c = 0; c < nr_cpus; c++) {
		cpu = cpu_map->map[c]; /* map c index to online cpu index */
		if (cpu__get_node(cpu) == node)
			set_bit(cpu, mask->bits);
	}
}

static int perf_mmap__setup_affinity_mask(struct mmap *map, struct mmap_params *mp)
{
	map->affinity_mask.nbits = cpu__max_cpu();
	map->affinity_mask.bits = bitmap_zalloc(map->affinity_mask.nbits);
	if (!map->affinity_mask.bits)
		return -1;

	if (mp->affinity == PERF_AFFINITY_NODE && cpu__max_node() > 1)
		build_node_mask(cpu__get_node(map->core.cpu), &map->affinity_mask);
	else if (mp->affinity == PERF_AFFINITY_CPU)
		set_bit(map->core.cpu, map->affinity_mask.bits);

	return 0;
}

int mmap__mmap(struct mmap *map, struct mmap_params *mp, int fd, int cpu)
{
	if (perf_mmap__mmap(&map->core, &mp->core, fd, cpu)) {
		pr_debug2("failed to mmap perf event ring buffer, error %d\n",
			  errno);
		return -1;
	}

	if (mp->affinity != PERF_AFFINITY_SYS &&
		perf_mmap__setup_affinity_mask(map, mp)) {
		pr_debug2("failed to alloc mmap affinity mask, error %d\n",
			  errno);
		return -1;
	}

	if (verbose == 2)
		mmap_cpu_mask__scnprintf(&map->affinity_mask, "mmap");

	map->core.flush = mp->flush;

	map->comp_level = mp->comp_level;

	if (map->comp_level && !perf_mmap__aio_enabled(map)) {
		map->data = mmap(NULL, mmap__mmap_len(map), PROT_READ|PROT_WRITE,
				 MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
		if (map->data == MAP_FAILED) {
			pr_debug2("failed to mmap data buffer, error %d\n",
					errno);
			map->data = NULL;
			return -1;
		}
	}

	if (auxtrace_mmap__mmap(&map->auxtrace_mmap,
				&mp->auxtrace_mp, map->core.base, fd))
		return -1;

	return perf_mmap__aio_mmap(map, mp);
}

int perf_mmap__push(struct mmap *md, void *to,
		    int push(struct mmap *map, void *to, void *buf, size_t size))
{
	u64 head = perf_mmap__read_head(&md->core);
	unsigned char *data = md->core.base + page_size;
>>>>>>> upstream/android-13
	unsigned long size;
	void *buf;
	int rc = 0;

<<<<<<< HEAD
	rc = perf_mmap__read_init(md);
	if (rc < 0)
		return (rc == -EAGAIN) ? 0 : -1;

	size = md->end - md->start;

	if ((md->start & md->mask) + size != (md->end & md->mask)) {
		buf = &data[md->start & md->mask];
		size = md->mask + 1 - (md->start & md->mask);
		md->start += size;

		if (push(to, buf, size) < 0) {
=======
	rc = perf_mmap__read_init(&md->core);
	if (rc < 0)
		return (rc == -EAGAIN) ? 1 : -1;

	size = md->core.end - md->core.start;

	if ((md->core.start & md->core.mask) + size != (md->core.end & md->core.mask)) {
		buf = &data[md->core.start & md->core.mask];
		size = md->core.mask + 1 - (md->core.start & md->core.mask);
		md->core.start += size;

		if (push(md, to, buf, size) < 0) {
>>>>>>> upstream/android-13
			rc = -1;
			goto out;
		}
	}

<<<<<<< HEAD
	buf = &data[md->start & md->mask];
	size = md->end - md->start;
	md->start += size;

	if (push(to, buf, size) < 0) {
=======
	buf = &data[md->core.start & md->core.mask];
	size = md->core.end - md->core.start;
	md->core.start += size;

	if (push(md, to, buf, size) < 0) {
>>>>>>> upstream/android-13
		rc = -1;
		goto out;
	}

<<<<<<< HEAD
	md->prev = head;
	perf_mmap__consume(md);
out:
	return rc;
}

/*
 * Mandatory for overwrite mode
 * The direction of overwrite mode is backward.
 * The last perf_mmap__read() will set tail to map->prev.
 * Need to correct the map->prev to head which is the end of next read.
 */
void perf_mmap__read_done(struct perf_mmap *map)
{
	/*
	 * Check if event was unmapped due to a POLLHUP/POLLERR.
	 */
	if (!refcount_read(&map->refcnt))
		return;

	map->prev = perf_mmap__read_head(map);
}
=======
	md->core.prev = head;
	perf_mmap__consume(&md->core);
out:
	return rc;
}
>>>>>>> upstream/android-13
