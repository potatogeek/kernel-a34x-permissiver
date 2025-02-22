// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <inttypes.h>
<<<<<<< HEAD
#include "util.h"
=======
>>>>>>> upstream/android-13
#include "string2.h"
#include <sys/param.h>
#include <sys/types.h>
#include <byteswap.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/compiler.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/bitops.h>
<<<<<<< HEAD
#include <linux/stringify.h>
=======
#include <linux/string.h>
#include <linux/stringify.h>
#include <linux/zalloc.h>
>>>>>>> upstream/android-13
#include <sys/stat.h>
#include <sys/utsname.h>
#include <linux/time64.h>
#include <dirent.h>
<<<<<<< HEAD

#include "evlist.h"
#include "evsel.h"
#include "header.h"
#include "memswap.h"
#include "../perf.h"
=======
#ifdef HAVE_LIBBPF_SUPPORT
#include <bpf/libbpf.h>
#endif
#include <perf/cpumap.h>

#include "dso.h"
#include "evlist.h"
#include "evsel.h"
#include "util/evsel_fprintf.h"
#include "header.h"
#include "memswap.h"
>>>>>>> upstream/android-13
#include "trace-event.h"
#include "session.h"
#include "symbol.h"
#include "debug.h"
#include "cpumap.h"
#include "pmu.h"
#include "vdso.h"
#include "strbuf.h"
#include "build-id.h"
#include "data.h"
#include <api/fs/fs.h>
#include "asm/bug.h"
#include "tool.h"
#include "time-utils.h"
#include "units.h"
<<<<<<< HEAD

#include "sane_ctype.h"
=======
#include "util/util.h" // perf_exe()
#include "cputopo.h"
#include "bpf-event.h"
#include "clockid.h"
#include "pmu-hybrid.h"

#include <linux/ctype.h>
#include <internal/lib.h>
>>>>>>> upstream/android-13

/*
 * magic2 = "PERFILE2"
 * must be a numerical value to let the endianness
 * determine the memory layout. That way we are able
 * to detect endianness when reading the perf.data file
 * back.
 *
 * we check for legacy (PERFFILE) format.
 */
static const char *__perf_magic1 = "PERFFILE";
static const u64 __perf_magic2    = 0x32454c4946524550ULL;
static const u64 __perf_magic2_sw = 0x50455246494c4532ULL;

#define PERF_MAGIC	__perf_magic2

const char perf_version_string[] = PERF_VERSION;

struct perf_file_attr {
	struct perf_event_attr	attr;
	struct perf_file_section	ids;
};

<<<<<<< HEAD
struct feat_fd {
	struct perf_header	*ph;
	int			fd;
	void			*buf;	/* Either buf != NULL or fd >= 0 */
	ssize_t			offset;
	size_t			size;
	struct perf_evsel	*events;
};

=======
>>>>>>> upstream/android-13
void perf_header__set_feat(struct perf_header *header, int feat)
{
	set_bit(feat, header->adds_features);
}

void perf_header__clear_feat(struct perf_header *header, int feat)
{
	clear_bit(feat, header->adds_features);
}

bool perf_header__has_feat(const struct perf_header *header, int feat)
{
	return test_bit(feat, header->adds_features);
}

static int __do_write_fd(struct feat_fd *ff, const void *buf, size_t size)
{
	ssize_t ret = writen(ff->fd, buf, size);

	if (ret != (ssize_t)size)
		return ret < 0 ? (int)ret : -1;
	return 0;
}

static int __do_write_buf(struct feat_fd *ff,  const void *buf, size_t size)
{
	/* struct perf_event_header::size is u16 */
	const size_t max_size = 0xffff - sizeof(struct perf_event_header);
	size_t new_size = ff->size;
	void *addr;

	if (size + ff->offset > max_size)
		return -E2BIG;

	while (size > (new_size - ff->offset))
		new_size <<= 1;
	new_size = min(max_size, new_size);

	if (ff->size < new_size) {
		addr = realloc(ff->buf, new_size);
		if (!addr)
			return -ENOMEM;
		ff->buf = addr;
		ff->size = new_size;
	}

	memcpy(ff->buf + ff->offset, buf, size);
	ff->offset += size;

	return 0;
}

<<<<<<< HEAD
/* Return: 0 if succeded, -ERR if failed. */
=======
/* Return: 0 if succeeded, -ERR if failed. */
>>>>>>> upstream/android-13
int do_write(struct feat_fd *ff, const void *buf, size_t size)
{
	if (!ff->buf)
		return __do_write_fd(ff, buf, size);
	return __do_write_buf(ff, buf, size);
}

<<<<<<< HEAD
/* Return: 0 if succeded, -ERR if failed. */
=======
/* Return: 0 if succeeded, -ERR if failed. */
>>>>>>> upstream/android-13
static int do_write_bitmap(struct feat_fd *ff, unsigned long *set, u64 size)
{
	u64 *p = (u64 *) set;
	int i, ret;

	ret = do_write(ff, &size, sizeof(size));
	if (ret < 0)
		return ret;

	for (i = 0; (u64) i < BITS_TO_U64(size); i++) {
		ret = do_write(ff, p + i, sizeof(*p));
		if (ret < 0)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
/* Return: 0 if succeded, -ERR if failed. */
=======
/* Return: 0 if succeeded, -ERR if failed. */
>>>>>>> upstream/android-13
int write_padded(struct feat_fd *ff, const void *bf,
		 size_t count, size_t count_aligned)
{
	static const char zero_buf[NAME_ALIGN];
	int err = do_write(ff, bf, count);

	if (!err)
		err = do_write(ff, zero_buf, count_aligned - count);

	return err;
}

#define string_size(str)						\
	(PERF_ALIGN((strlen(str) + 1), NAME_ALIGN) + sizeof(u32))

<<<<<<< HEAD
/* Return: 0 if succeded, -ERR if failed. */
=======
/* Return: 0 if succeeded, -ERR if failed. */
>>>>>>> upstream/android-13
static int do_write_string(struct feat_fd *ff, const char *str)
{
	u32 len, olen;
	int ret;

	olen = strlen(str) + 1;
	len = PERF_ALIGN(olen, NAME_ALIGN);

	/* write len, incl. \0 */
	ret = do_write(ff, &len, sizeof(len));
	if (ret < 0)
		return ret;

	return write_padded(ff, str, olen, len);
}

static int __do_read_fd(struct feat_fd *ff, void *addr, ssize_t size)
{
	ssize_t ret = readn(ff->fd, addr, size);

	if (ret != size)
		return ret < 0 ? (int)ret : -1;
	return 0;
}

static int __do_read_buf(struct feat_fd *ff, void *addr, ssize_t size)
{
	if (size > (ssize_t)ff->size - ff->offset)
		return -1;

	memcpy(addr, ff->buf + ff->offset, size);
	ff->offset += size;

	return 0;

}

static int __do_read(struct feat_fd *ff, void *addr, ssize_t size)
{
	if (!ff->buf)
		return __do_read_fd(ff, addr, size);
	return __do_read_buf(ff, addr, size);
}

static int do_read_u32(struct feat_fd *ff, u32 *addr)
{
	int ret;

	ret = __do_read(ff, addr, sizeof(*addr));
	if (ret)
		return ret;

	if (ff->ph->needs_swap)
		*addr = bswap_32(*addr);
	return 0;
}

static int do_read_u64(struct feat_fd *ff, u64 *addr)
{
	int ret;

	ret = __do_read(ff, addr, sizeof(*addr));
	if (ret)
		return ret;

	if (ff->ph->needs_swap)
		*addr = bswap_64(*addr);
	return 0;
}

static char *do_read_string(struct feat_fd *ff)
{
	u32 len;
	char *buf;

	if (do_read_u32(ff, &len))
		return NULL;

	buf = malloc(len);
	if (!buf)
		return NULL;

	if (!__do_read(ff, buf, len)) {
		/*
		 * strings are padded by zeroes
		 * thus the actual strlen of buf
		 * may be less than len
		 */
		return buf;
	}

	free(buf);
	return NULL;
}

<<<<<<< HEAD
/* Return: 0 if succeded, -ERR if failed. */
=======
/* Return: 0 if succeeded, -ERR if failed. */
>>>>>>> upstream/android-13
static int do_read_bitmap(struct feat_fd *ff, unsigned long **pset, u64 *psize)
{
	unsigned long *set;
	u64 size, *p;
	int i, ret;

	ret = do_read_u64(ff, &size);
	if (ret)
		return ret;

<<<<<<< HEAD
	set = bitmap_alloc(size);
=======
	set = bitmap_zalloc(size);
>>>>>>> upstream/android-13
	if (!set)
		return -ENOMEM;

	p = (u64 *) set;

	for (i = 0; (u64) i < BITS_TO_U64(size); i++) {
		ret = do_read_u64(ff, p + i);
		if (ret < 0) {
			free(set);
			return ret;
		}
	}

	*pset  = set;
	*psize = size;
	return 0;
}

static int write_tracing_data(struct feat_fd *ff,
<<<<<<< HEAD
			      struct perf_evlist *evlist)
=======
			      struct evlist *evlist)
>>>>>>> upstream/android-13
{
	if (WARN(ff->buf, "Error: calling %s in pipe-mode.\n", __func__))
		return -1;

<<<<<<< HEAD
	return read_tracing_data(ff->fd, &evlist->entries);
}

static int write_build_id(struct feat_fd *ff,
			  struct perf_evlist *evlist __maybe_unused)
=======
	return read_tracing_data(ff->fd, &evlist->core.entries);
}

static int write_build_id(struct feat_fd *ff,
			  struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	struct perf_session *session;
	int err;

	session = container_of(ff->ph, struct perf_session, header);

	if (!perf_session__read_build_ids(session, true))
		return -1;

	if (WARN(ff->buf, "Error: calling %s in pipe-mode.\n", __func__))
		return -1;

	err = perf_session__write_buildid_table(session, ff);
	if (err < 0) {
		pr_debug("failed to write buildid table\n");
		return err;
	}
	perf_session__cache_build_ids(session);

	return 0;
}

static int write_hostname(struct feat_fd *ff,
<<<<<<< HEAD
			  struct perf_evlist *evlist __maybe_unused)
=======
			  struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	struct utsname uts;
	int ret;

	ret = uname(&uts);
	if (ret < 0)
		return -1;

	return do_write_string(ff, uts.nodename);
}

static int write_osrelease(struct feat_fd *ff,
<<<<<<< HEAD
			   struct perf_evlist *evlist __maybe_unused)
=======
			   struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	struct utsname uts;
	int ret;

	ret = uname(&uts);
	if (ret < 0)
		return -1;

	return do_write_string(ff, uts.release);
}

static int write_arch(struct feat_fd *ff,
<<<<<<< HEAD
		      struct perf_evlist *evlist __maybe_unused)
=======
		      struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	struct utsname uts;
	int ret;

	ret = uname(&uts);
	if (ret < 0)
		return -1;

	return do_write_string(ff, uts.machine);
}

static int write_version(struct feat_fd *ff,
<<<<<<< HEAD
			 struct perf_evlist *evlist __maybe_unused)
=======
			 struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	return do_write_string(ff, perf_version_string);
}

static int __write_cpudesc(struct feat_fd *ff, const char *cpuinfo_proc)
{
	FILE *file;
	char *buf = NULL;
	char *s, *p;
	const char *search = cpuinfo_proc;
	size_t len = 0;
	int ret = -1;

	if (!search)
		return -1;

	file = fopen("/proc/cpuinfo", "r");
	if (!file)
		return -1;

	while (getline(&buf, &len, file) > 0) {
		ret = strncmp(buf, search, strlen(search));
		if (!ret)
			break;
	}

	if (ret) {
		ret = -1;
		goto done;
	}

	s = buf;

	p = strchr(buf, ':');
	if (p && *(p+1) == ' ' && *(p+2))
		s = p + 2;
	p = strchr(s, '\n');
	if (p)
		*p = '\0';

	/* squash extra space characters (branding string) */
	p = s;
	while (*p) {
		if (isspace(*p)) {
			char *r = p + 1;
<<<<<<< HEAD
			char *q = r;
			*p = ' ';
			while (*q && isspace(*q))
				q++;
=======
			char *q = skip_spaces(r);
			*p = ' ';
>>>>>>> upstream/android-13
			if (q != (p+1))
				while ((*r++ = *q++));
		}
		p++;
	}
	ret = do_write_string(ff, s);
done:
	free(buf);
	fclose(file);
	return ret;
}

static int write_cpudesc(struct feat_fd *ff,
<<<<<<< HEAD
		       struct perf_evlist *evlist __maybe_unused)
{
	const char *cpuinfo_procs[] = CPUINFO_PROC;
=======
		       struct evlist *evlist __maybe_unused)
{
#if defined(__powerpc__) || defined(__hppa__) || defined(__sparc__)
#define CPUINFO_PROC	{ "cpu", }
#elif defined(__s390__)
#define CPUINFO_PROC	{ "vendor_id", }
#elif defined(__sh__)
#define CPUINFO_PROC	{ "cpu type", }
#elif defined(__alpha__) || defined(__mips__)
#define CPUINFO_PROC	{ "cpu model", }
#elif defined(__arm__)
#define CPUINFO_PROC	{ "model name", "Processor", }
#elif defined(__arc__)
#define CPUINFO_PROC	{ "Processor", }
#elif defined(__xtensa__)
#define CPUINFO_PROC	{ "core ID", }
#else
#define CPUINFO_PROC	{ "model name", }
#endif
	const char *cpuinfo_procs[] = CPUINFO_PROC;
#undef CPUINFO_PROC
>>>>>>> upstream/android-13
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(cpuinfo_procs); i++) {
		int ret;
		ret = __write_cpudesc(ff, cpuinfo_procs[i]);
		if (ret >= 0)
			return ret;
	}
	return -1;
}


static int write_nrcpus(struct feat_fd *ff,
<<<<<<< HEAD
			struct perf_evlist *evlist __maybe_unused)
=======
			struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	long nr;
	u32 nrc, nra;
	int ret;

	nrc = cpu__max_present_cpu();

	nr = sysconf(_SC_NPROCESSORS_ONLN);
	if (nr < 0)
		return -1;

	nra = (u32)(nr & UINT_MAX);

	ret = do_write(ff, &nrc, sizeof(nrc));
	if (ret < 0)
		return ret;

	return do_write(ff, &nra, sizeof(nra));
}

static int write_event_desc(struct feat_fd *ff,
<<<<<<< HEAD
			    struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;
	u32 nre, nri, sz;
	int ret;

	nre = evlist->nr_entries;
=======
			    struct evlist *evlist)
{
	struct evsel *evsel;
	u32 nre, nri, sz;
	int ret;

	nre = evlist->core.nr_entries;
>>>>>>> upstream/android-13

	/*
	 * write number of events
	 */
	ret = do_write(ff, &nre, sizeof(nre));
	if (ret < 0)
		return ret;

	/*
	 * size of perf_event_attr struct
	 */
<<<<<<< HEAD
	sz = (u32)sizeof(evsel->attr);
=======
	sz = (u32)sizeof(evsel->core.attr);
>>>>>>> upstream/android-13
	ret = do_write(ff, &sz, sizeof(sz));
	if (ret < 0)
		return ret;

	evlist__for_each_entry(evlist, evsel) {
<<<<<<< HEAD
		ret = do_write(ff, &evsel->attr, sz);
=======
		ret = do_write(ff, &evsel->core.attr, sz);
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;
		/*
		 * write number of unique id per event
		 * there is one id per instance of an event
		 *
		 * copy into an nri to be independent of the
		 * type of ids,
		 */
<<<<<<< HEAD
		nri = evsel->ids;
=======
		nri = evsel->core.ids;
>>>>>>> upstream/android-13
		ret = do_write(ff, &nri, sizeof(nri));
		if (ret < 0)
			return ret;

		/*
		 * write event string as passed on cmdline
		 */
<<<<<<< HEAD
		ret = do_write_string(ff, perf_evsel__name(evsel));
=======
		ret = do_write_string(ff, evsel__name(evsel));
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;
		/*
		 * write unique ids for this event
		 */
<<<<<<< HEAD
		ret = do_write(ff, evsel->id, evsel->ids * sizeof(u64));
=======
		ret = do_write(ff, evsel->core.id, evsel->core.ids * sizeof(u64));
>>>>>>> upstream/android-13
		if (ret < 0)
			return ret;
	}
	return 0;
}

static int write_cmdline(struct feat_fd *ff,
<<<<<<< HEAD
			 struct perf_evlist *evlist __maybe_unused)
{
	char buf[MAXPATHLEN];
	u32 n;
	int i, ret;

	/* actual path to perf binary */
	ret = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (ret <= 0)
		return -1;

	/* readlink() does not add null termination */
	buf[ret] = '\0';
=======
			 struct evlist *evlist __maybe_unused)
{
	char pbuf[MAXPATHLEN], *buf;
	int i, ret, n;

	/* actual path to perf binary */
	buf = perf_exe(pbuf, MAXPATHLEN);
>>>>>>> upstream/android-13

	/* account for binary path */
	n = perf_env.nr_cmdline + 1;

	ret = do_write(ff, &n, sizeof(n));
	if (ret < 0)
		return ret;

	ret = do_write_string(ff, buf);
	if (ret < 0)
		return ret;

	for (i = 0 ; i < perf_env.nr_cmdline; i++) {
		ret = do_write_string(ff, perf_env.cmdline_argv[i]);
		if (ret < 0)
			return ret;
	}
	return 0;
}

<<<<<<< HEAD
#define CORE_SIB_FMT \
	"/sys/devices/system/cpu/cpu%d/topology/core_siblings_list"
#define THRD_SIB_FMT \
	"/sys/devices/system/cpu/cpu%d/topology/thread_siblings_list"

struct cpu_topo {
	u32 cpu_nr;
	u32 core_sib;
	u32 thread_sib;
	char **core_siblings;
	char **thread_siblings;
};

static int build_cpu_topo(struct cpu_topo *tp, int cpu)
{
	FILE *fp;
	char filename[MAXPATHLEN];
	char *buf = NULL, *p;
	size_t len = 0;
	ssize_t sret;
	u32 i = 0;
	int ret = -1;

	sprintf(filename, CORE_SIB_FMT, cpu);
	fp = fopen(filename, "r");
	if (!fp)
		goto try_threads;

	sret = getline(&buf, &len, fp);
	fclose(fp);
	if (sret <= 0)
		goto try_threads;

	p = strchr(buf, '\n');
	if (p)
		*p = '\0';

	for (i = 0; i < tp->core_sib; i++) {
		if (!strcmp(buf, tp->core_siblings[i]))
			break;
	}
	if (i == tp->core_sib) {
		tp->core_siblings[i] = buf;
		tp->core_sib++;
		buf = NULL;
		len = 0;
	}
	ret = 0;

try_threads:
	sprintf(filename, THRD_SIB_FMT, cpu);
	fp = fopen(filename, "r");
	if (!fp)
		goto done;

	if (getline(&buf, &len, fp) <= 0)
		goto done;

	p = strchr(buf, '\n');
	if (p)
		*p = '\0';

	for (i = 0; i < tp->thread_sib; i++) {
		if (!strcmp(buf, tp->thread_siblings[i]))
			break;
	}
	if (i == tp->thread_sib) {
		tp->thread_siblings[i] = buf;
		tp->thread_sib++;
		buf = NULL;
	}
	ret = 0;
done:
	if(fp)
		fclose(fp);
	free(buf);
	return ret;
}

static void free_cpu_topo(struct cpu_topo *tp)
{
	u32 i;

	if (!tp)
		return;

	for (i = 0 ; i < tp->core_sib; i++)
		zfree(&tp->core_siblings[i]);

	for (i = 0 ; i < tp->thread_sib; i++)
		zfree(&tp->thread_siblings[i]);

	free(tp);
}

static struct cpu_topo *build_cpu_topology(void)
{
	struct cpu_topo *tp = NULL;
	void *addr;
	u32 nr, i;
	size_t sz;
	long ncpus;
	int ret = -1;
	struct cpu_map *map;

	ncpus = cpu__max_present_cpu();

	/* build online CPU map */
	map = cpu_map__new(NULL);
	if (map == NULL) {
		pr_debug("failed to get system cpumap\n");
		return NULL;
	}

	nr = (u32)(ncpus & UINT_MAX);

	sz = nr * sizeof(char *);
	addr = calloc(1, sizeof(*tp) + 2 * sz);
	if (!addr)
		goto out_free;

	tp = addr;
	tp->cpu_nr = nr;
	addr += sizeof(*tp);
	tp->core_siblings = addr;
	addr += sz;
	tp->thread_siblings = addr;

	for (i = 0; i < nr; i++) {
		if (!cpu_map__has(map, i))
			continue;

		ret = build_cpu_topo(tp, i);
		if (ret < 0)
			break;
	}

out_free:
	cpu_map__put(map);
	if (ret) {
		free_cpu_topo(tp);
		tp = NULL;
	}
	return tp;
}

static int write_cpu_topology(struct feat_fd *ff,
			      struct perf_evlist *evlist __maybe_unused)
{
	struct cpu_topo *tp;
	u32 i;
	int ret, j;

	tp = build_cpu_topology();
=======

static int write_cpu_topology(struct feat_fd *ff,
			      struct evlist *evlist __maybe_unused)
{
	struct cpu_topology *tp;
	u32 i;
	int ret, j;

	tp = cpu_topology__new();
>>>>>>> upstream/android-13
	if (!tp)
		return -1;

	ret = do_write(ff, &tp->core_sib, sizeof(tp->core_sib));
	if (ret < 0)
		goto done;

	for (i = 0; i < tp->core_sib; i++) {
		ret = do_write_string(ff, tp->core_siblings[i]);
		if (ret < 0)
			goto done;
	}
	ret = do_write(ff, &tp->thread_sib, sizeof(tp->thread_sib));
	if (ret < 0)
		goto done;

	for (i = 0; i < tp->thread_sib; i++) {
		ret = do_write_string(ff, tp->thread_siblings[i]);
		if (ret < 0)
			break;
	}

	ret = perf_env__read_cpu_topology_map(&perf_env);
	if (ret < 0)
		goto done;

	for (j = 0; j < perf_env.nr_cpus_avail; j++) {
		ret = do_write(ff, &perf_env.cpu[j].core_id,
			       sizeof(perf_env.cpu[j].core_id));
		if (ret < 0)
			return ret;
		ret = do_write(ff, &perf_env.cpu[j].socket_id,
			       sizeof(perf_env.cpu[j].socket_id));
		if (ret < 0)
			return ret;
	}
<<<<<<< HEAD
done:
	free_cpu_topo(tp);
=======

	if (!tp->die_sib)
		goto done;

	ret = do_write(ff, &tp->die_sib, sizeof(tp->die_sib));
	if (ret < 0)
		goto done;

	for (i = 0; i < tp->die_sib; i++) {
		ret = do_write_string(ff, tp->die_siblings[i]);
		if (ret < 0)
			goto done;
	}

	for (j = 0; j < perf_env.nr_cpus_avail; j++) {
		ret = do_write(ff, &perf_env.cpu[j].die_id,
			       sizeof(perf_env.cpu[j].die_id));
		if (ret < 0)
			return ret;
	}

done:
	cpu_topology__delete(tp);
>>>>>>> upstream/android-13
	return ret;
}



static int write_total_mem(struct feat_fd *ff,
<<<<<<< HEAD
			   struct perf_evlist *evlist __maybe_unused)
=======
			   struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	char *buf = NULL;
	FILE *fp;
	size_t len = 0;
	int ret = -1, n;
	uint64_t mem;

	fp = fopen("/proc/meminfo", "r");
	if (!fp)
		return -1;

	while (getline(&buf, &len, fp) > 0) {
		ret = strncmp(buf, "MemTotal:", 9);
		if (!ret)
			break;
	}
	if (!ret) {
		n = sscanf(buf, "%*s %"PRIu64, &mem);
		if (n == 1)
			ret = do_write(ff, &mem, sizeof(mem));
	} else
		ret = -1;
	free(buf);
	fclose(fp);
	return ret;
}

<<<<<<< HEAD
static int write_topo_node(struct feat_fd *ff, int node)
{
	char str[MAXPATHLEN];
	char field[32];
	char *buf = NULL, *p;
	size_t len = 0;
	FILE *fp;
	u64 mem_total, mem_free, mem;
	int ret = -1;

	sprintf(str, "/sys/devices/system/node/node%d/meminfo", node);
	fp = fopen(str, "r");
	if (!fp)
		return -1;

	while (getline(&buf, &len, fp) > 0) {
		/* skip over invalid lines */
		if (!strchr(buf, ':'))
			continue;
		if (sscanf(buf, "%*s %*d %31s %"PRIu64, field, &mem) != 2)
			goto done;
		if (!strcmp(field, "MemTotal:"))
			mem_total = mem;
		if (!strcmp(field, "MemFree:"))
			mem_free = mem;
	}

	fclose(fp);
	fp = NULL;

	ret = do_write(ff, &mem_total, sizeof(u64));
	if (ret)
		goto done;

	ret = do_write(ff, &mem_free, sizeof(u64));
	if (ret)
		goto done;

	ret = -1;
	sprintf(str, "/sys/devices/system/node/node%d/cpulist", node);

	fp = fopen(str, "r");
	if (!fp)
		goto done;

	if (getline(&buf, &len, fp) <= 0)
		goto done;

	p = strchr(buf, '\n');
	if (p)
		*p = '\0';

	ret = do_write_string(ff, buf);
done:
	free(buf);
	if (fp)
		fclose(fp);
	return ret;
}

static int write_numa_topology(struct feat_fd *ff,
			       struct perf_evlist *evlist __maybe_unused)
{
	char *buf = NULL;
	size_t len = 0;
	FILE *fp;
	struct cpu_map *node_map = NULL;
	char *c;
	u32 nr, i, j;
	int ret = -1;

	fp = fopen("/sys/devices/system/node/online", "r");
	if (!fp)
		return -1;

	if (getline(&buf, &len, fp) <= 0)
		goto done;

	c = strchr(buf, '\n');
	if (c)
		*c = '\0';

	node_map = cpu_map__new(buf);
	if (!node_map)
		goto done;

	nr = (u32)node_map->nr;

	ret = do_write(ff, &nr, sizeof(nr));
	if (ret < 0)
		goto done;

	for (i = 0; i < nr; i++) {
		j = (u32)node_map->map[i];
		ret = do_write(ff, &j, sizeof(j));
		if (ret < 0)
			break;

		ret = write_topo_node(ff, i);
		if (ret < 0)
			break;
	}
done:
	free(buf);
	fclose(fp);
	cpu_map__put(node_map);
=======
static int write_numa_topology(struct feat_fd *ff,
			       struct evlist *evlist __maybe_unused)
{
	struct numa_topology *tp;
	int ret = -1;
	u32 i;

	tp = numa_topology__new();
	if (!tp)
		return -ENOMEM;

	ret = do_write(ff, &tp->nr, sizeof(u32));
	if (ret < 0)
		goto err;

	for (i = 0; i < tp->nr; i++) {
		struct numa_topology_node *n = &tp->nodes[i];

		ret = do_write(ff, &n->node, sizeof(u32));
		if (ret < 0)
			goto err;

		ret = do_write(ff, &n->mem_total, sizeof(u64));
		if (ret)
			goto err;

		ret = do_write(ff, &n->mem_free, sizeof(u64));
		if (ret)
			goto err;

		ret = do_write_string(ff, n->cpus);
		if (ret < 0)
			goto err;
	}

	ret = 0;

err:
	numa_topology__delete(tp);
>>>>>>> upstream/android-13
	return ret;
}

/*
 * File format:
 *
 * struct pmu_mappings {
 *	u32	pmu_num;
 *	struct pmu_map {
 *		u32	type;
 *		char	name[];
 *	}[pmu_num];
 * };
 */

static int write_pmu_mappings(struct feat_fd *ff,
<<<<<<< HEAD
			      struct perf_evlist *evlist __maybe_unused)
=======
			      struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	struct perf_pmu *pmu = NULL;
	u32 pmu_num = 0;
	int ret;

	/*
	 * Do a first pass to count number of pmu to avoid lseek so this
	 * works in pipe mode as well.
	 */
	while ((pmu = perf_pmu__scan(pmu))) {
		if (!pmu->name)
			continue;
		pmu_num++;
	}

	ret = do_write(ff, &pmu_num, sizeof(pmu_num));
	if (ret < 0)
		return ret;

	while ((pmu = perf_pmu__scan(pmu))) {
		if (!pmu->name)
			continue;

		ret = do_write(ff, &pmu->type, sizeof(pmu->type));
		if (ret < 0)
			return ret;

		ret = do_write_string(ff, pmu->name);
		if (ret < 0)
			return ret;
	}

	return 0;
}

/*
 * File format:
 *
 * struct group_descs {
 *	u32	nr_groups;
 *	struct group_desc {
 *		char	name[];
 *		u32	leader_idx;
 *		u32	nr_members;
 *	}[nr_groups];
 * };
 */
static int write_group_desc(struct feat_fd *ff,
<<<<<<< HEAD
			    struct perf_evlist *evlist)
{
	u32 nr_groups = evlist->nr_groups;
	struct perf_evsel *evsel;
=======
			    struct evlist *evlist)
{
	u32 nr_groups = evlist->core.nr_groups;
	struct evsel *evsel;
>>>>>>> upstream/android-13
	int ret;

	ret = do_write(ff, &nr_groups, sizeof(nr_groups));
	if (ret < 0)
		return ret;

	evlist__for_each_entry(evlist, evsel) {
<<<<<<< HEAD
		if (perf_evsel__is_group_leader(evsel) &&
		    evsel->nr_members > 1) {
			const char *name = evsel->group_name ?: "{anon_group}";
			u32 leader_idx = evsel->idx;
			u32 nr_members = evsel->nr_members;
=======
		if (evsel__is_group_leader(evsel) && evsel->core.nr_members > 1) {
			const char *name = evsel->group_name ?: "{anon_group}";
			u32 leader_idx = evsel->core.idx;
			u32 nr_members = evsel->core.nr_members;
>>>>>>> upstream/android-13

			ret = do_write_string(ff, name);
			if (ret < 0)
				return ret;

			ret = do_write(ff, &leader_idx, sizeof(leader_idx));
			if (ret < 0)
				return ret;

			ret = do_write(ff, &nr_members, sizeof(nr_members));
			if (ret < 0)
				return ret;
		}
	}
	return 0;
}

/*
<<<<<<< HEAD
=======
 * Return the CPU id as a raw string.
 *
 * Each architecture should provide a more precise id string that
 * can be use to match the architecture's "mapfile".
 */
char * __weak get_cpuid_str(struct perf_pmu *pmu __maybe_unused)
{
	return NULL;
}

/* Return zero when the cpuid from the mapfile.csv matches the
 * cpuid string generated on this platform.
 * Otherwise return non-zero.
 */
int __weak strcmp_cpuid_str(const char *mapcpuid, const char *cpuid)
{
	regex_t re;
	regmatch_t pmatch[1];
	int match;

	if (regcomp(&re, mapcpuid, REG_EXTENDED) != 0) {
		/* Warn unable to generate match particular string. */
		pr_info("Invalid regular expression %s\n", mapcpuid);
		return 1;
	}

	match = !regexec(&re, cpuid, 1, pmatch, 0);
	regfree(&re);
	if (match) {
		size_t match_len = (pmatch[0].rm_eo - pmatch[0].rm_so);

		/* Verify the entire string matched. */
		if (match_len == strlen(cpuid))
			return 0;
	}
	return 1;
}

/*
>>>>>>> upstream/android-13
 * default get_cpuid(): nothing gets recorded
 * actual implementation must be in arch/$(SRCARCH)/util/header.c
 */
int __weak get_cpuid(char *buffer __maybe_unused, size_t sz __maybe_unused)
{
<<<<<<< HEAD
	return -1;
}

static int write_cpuid(struct feat_fd *ff,
		       struct perf_evlist *evlist __maybe_unused)
=======
	return ENOSYS; /* Not implemented */
}

static int write_cpuid(struct feat_fd *ff,
		       struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	char buffer[64];
	int ret;

	ret = get_cpuid(buffer, sizeof(buffer));
<<<<<<< HEAD
	if (!ret)
		goto write_it;

	return -1;
write_it:
=======
	if (ret)
		return -1;

>>>>>>> upstream/android-13
	return do_write_string(ff, buffer);
}

static int write_branch_stack(struct feat_fd *ff __maybe_unused,
<<<<<<< HEAD
			      struct perf_evlist *evlist __maybe_unused)
=======
			      struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	return 0;
}

static int write_auxtrace(struct feat_fd *ff,
<<<<<<< HEAD
			  struct perf_evlist *evlist __maybe_unused)
=======
			  struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	struct perf_session *session;
	int err;

	if (WARN(ff->buf, "Error: calling %s in pipe-mode.\n", __func__))
		return -1;

	session = container_of(ff->ph, struct perf_session, header);

	err = auxtrace_index__write(ff->fd, &session->auxtrace_index);
	if (err < 0)
		pr_err("Failed to write auxtrace index\n");
	return err;
}

<<<<<<< HEAD
=======
static int write_clockid(struct feat_fd *ff,
			 struct evlist *evlist __maybe_unused)
{
	return do_write(ff, &ff->ph->env.clock.clockid_res_ns,
			sizeof(ff->ph->env.clock.clockid_res_ns));
}

static int write_clock_data(struct feat_fd *ff,
			    struct evlist *evlist __maybe_unused)
{
	u64 *data64;
	u32 data32;
	int ret;

	/* version */
	data32 = 1;

	ret = do_write(ff, &data32, sizeof(data32));
	if (ret < 0)
		return ret;

	/* clockid */
	data32 = ff->ph->env.clock.clockid;

	ret = do_write(ff, &data32, sizeof(data32));
	if (ret < 0)
		return ret;

	/* TOD ref time */
	data64 = &ff->ph->env.clock.tod_ns;

	ret = do_write(ff, data64, sizeof(*data64));
	if (ret < 0)
		return ret;

	/* clockid ref time */
	data64 = &ff->ph->env.clock.clockid_ns;

	return do_write(ff, data64, sizeof(*data64));
}

static int write_hybrid_topology(struct feat_fd *ff,
				 struct evlist *evlist __maybe_unused)
{
	struct hybrid_topology *tp;
	int ret;
	u32 i;

	tp = hybrid_topology__new();
	if (!tp)
		return -ENOENT;

	ret = do_write(ff, &tp->nr, sizeof(u32));
	if (ret < 0)
		goto err;

	for (i = 0; i < tp->nr; i++) {
		struct hybrid_topology_node *n = &tp->nodes[i];

		ret = do_write_string(ff, n->pmu_name);
		if (ret < 0)
			goto err;

		ret = do_write_string(ff, n->cpus);
		if (ret < 0)
			goto err;
	}

	ret = 0;

err:
	hybrid_topology__delete(tp);
	return ret;
}

static int write_dir_format(struct feat_fd *ff,
			    struct evlist *evlist __maybe_unused)
{
	struct perf_session *session;
	struct perf_data *data;

	session = container_of(ff->ph, struct perf_session, header);
	data = session->data;

	if (WARN_ON(!perf_data__is_dir(data)))
		return -1;

	return do_write(ff, &data->dir.version, sizeof(data->dir.version));
}

#ifdef HAVE_LIBBPF_SUPPORT
static int write_bpf_prog_info(struct feat_fd *ff,
			       struct evlist *evlist __maybe_unused)
{
	struct perf_env *env = &ff->ph->env;
	struct rb_root *root;
	struct rb_node *next;
	int ret;

	down_read(&env->bpf_progs.lock);

	ret = do_write(ff, &env->bpf_progs.infos_cnt,
		       sizeof(env->bpf_progs.infos_cnt));
	if (ret < 0)
		goto out;

	root = &env->bpf_progs.infos;
	next = rb_first(root);
	while (next) {
		struct bpf_prog_info_node *node;
		size_t len;

		node = rb_entry(next, struct bpf_prog_info_node, rb_node);
		next = rb_next(&node->rb_node);
		len = sizeof(struct bpf_prog_info_linear) +
			node->info_linear->data_len;

		/* before writing to file, translate address to offset */
		bpf_program__bpil_addr_to_offs(node->info_linear);
		ret = do_write(ff, node->info_linear, len);
		/*
		 * translate back to address even when do_write() fails,
		 * so that this function never changes the data.
		 */
		bpf_program__bpil_offs_to_addr(node->info_linear);
		if (ret < 0)
			goto out;
	}
out:
	up_read(&env->bpf_progs.lock);
	return ret;
}

static int write_bpf_btf(struct feat_fd *ff,
			 struct evlist *evlist __maybe_unused)
{
	struct perf_env *env = &ff->ph->env;
	struct rb_root *root;
	struct rb_node *next;
	int ret;

	down_read(&env->bpf_progs.lock);

	ret = do_write(ff, &env->bpf_progs.btfs_cnt,
		       sizeof(env->bpf_progs.btfs_cnt));

	if (ret < 0)
		goto out;

	root = &env->bpf_progs.btfs;
	next = rb_first(root);
	while (next) {
		struct btf_node *node;

		node = rb_entry(next, struct btf_node, rb_node);
		next = rb_next(&node->rb_node);
		ret = do_write(ff, &node->id,
			       sizeof(u32) * 2 + node->data_size);
		if (ret < 0)
			goto out;
	}
out:
	up_read(&env->bpf_progs.lock);
	return ret;
}
#endif // HAVE_LIBBPF_SUPPORT

>>>>>>> upstream/android-13
static int cpu_cache_level__sort(const void *a, const void *b)
{
	struct cpu_cache_level *cache_a = (struct cpu_cache_level *)a;
	struct cpu_cache_level *cache_b = (struct cpu_cache_level *)b;

	return cache_a->level - cache_b->level;
}

static bool cpu_cache_level__cmp(struct cpu_cache_level *a, struct cpu_cache_level *b)
{
	if (a->level != b->level)
		return false;

	if (a->line_size != b->line_size)
		return false;

	if (a->sets != b->sets)
		return false;

	if (a->ways != b->ways)
		return false;

	if (strcmp(a->type, b->type))
		return false;

	if (strcmp(a->size, b->size))
		return false;

	if (strcmp(a->map, b->map))
		return false;

	return true;
}

static int cpu_cache_level__read(struct cpu_cache_level *cache, u32 cpu, u16 level)
{
	char path[PATH_MAX], file[PATH_MAX];
	struct stat st;
	size_t len;

	scnprintf(path, PATH_MAX, "devices/system/cpu/cpu%d/cache/index%d/", cpu, level);
	scnprintf(file, PATH_MAX, "%s/%s", sysfs__mountpoint(), path);

	if (stat(file, &st))
		return 1;

	scnprintf(file, PATH_MAX, "%s/level", path);
	if (sysfs__read_int(file, (int *) &cache->level))
		return -1;

	scnprintf(file, PATH_MAX, "%s/coherency_line_size", path);
	if (sysfs__read_int(file, (int *) &cache->line_size))
		return -1;

	scnprintf(file, PATH_MAX, "%s/number_of_sets", path);
	if (sysfs__read_int(file, (int *) &cache->sets))
		return -1;

	scnprintf(file, PATH_MAX, "%s/ways_of_associativity", path);
	if (sysfs__read_int(file, (int *) &cache->ways))
		return -1;

	scnprintf(file, PATH_MAX, "%s/type", path);
	if (sysfs__read_str(file, &cache->type, &len))
		return -1;

	cache->type[len] = 0;
<<<<<<< HEAD
	cache->type = rtrim(cache->type);

	scnprintf(file, PATH_MAX, "%s/size", path);
	if (sysfs__read_str(file, &cache->size, &len)) {
		free(cache->type);
=======
	cache->type = strim(cache->type);

	scnprintf(file, PATH_MAX, "%s/size", path);
	if (sysfs__read_str(file, &cache->size, &len)) {
		zfree(&cache->type);
>>>>>>> upstream/android-13
		return -1;
	}

	cache->size[len] = 0;
<<<<<<< HEAD
	cache->size = rtrim(cache->size);

	scnprintf(file, PATH_MAX, "%s/shared_cpu_list", path);
	if (sysfs__read_str(file, &cache->map, &len)) {
		free(cache->size);
		free(cache->type);
=======
	cache->size = strim(cache->size);

	scnprintf(file, PATH_MAX, "%s/shared_cpu_list", path);
	if (sysfs__read_str(file, &cache->map, &len)) {
		zfree(&cache->size);
		zfree(&cache->type);
>>>>>>> upstream/android-13
		return -1;
	}

	cache->map[len] = 0;
<<<<<<< HEAD
	cache->map = rtrim(cache->map);
=======
	cache->map = strim(cache->map);
>>>>>>> upstream/android-13
	return 0;
}

static void cpu_cache_level__fprintf(FILE *out, struct cpu_cache_level *c)
{
	fprintf(out, "L%d %-15s %8s [%s]\n", c->level, c->type, c->size, c->map);
}

<<<<<<< HEAD
static int build_caches(struct cpu_cache_level caches[], u32 size, u32 *cntp)
{
	u32 i, cnt = 0;
	long ncpus;
	u32 nr, cpu;
	u16 level;

	ncpus = sysconf(_SC_NPROCESSORS_CONF);
	if (ncpus < 0)
		return -1;

	nr = (u32)(ncpus & UINT_MAX);

	for (cpu = 0; cpu < nr; cpu++) {
		for (level = 0; level < 10; level++) {
=======
#define MAX_CACHE_LVL 4

static int build_caches(struct cpu_cache_level caches[], u32 *cntp)
{
	u32 i, cnt = 0;
	u32 nr, cpu;
	u16 level;

	nr = cpu__max_cpu();

	for (cpu = 0; cpu < nr; cpu++) {
		for (level = 0; level < MAX_CACHE_LVL; level++) {
>>>>>>> upstream/android-13
			struct cpu_cache_level c;
			int err;

			err = cpu_cache_level__read(&c, cpu, level);
			if (err < 0)
				return err;

			if (err == 1)
				break;

			for (i = 0; i < cnt; i++) {
				if (cpu_cache_level__cmp(&c, &caches[i]))
					break;
			}

			if (i == cnt)
				caches[cnt++] = c;
			else
				cpu_cache_level__free(&c);
<<<<<<< HEAD

			if (WARN_ONCE(cnt == size, "way too many cpu caches.."))
				goto out;
		}
	}
 out:
=======
		}
	}
>>>>>>> upstream/android-13
	*cntp = cnt;
	return 0;
}

<<<<<<< HEAD
#define MAX_CACHES (MAX_NR_CPUS * 4)

static int write_cache(struct feat_fd *ff,
		       struct perf_evlist *evlist __maybe_unused)
{
	struct cpu_cache_level caches[MAX_CACHES];
	u32 cnt = 0, i, version = 1;
	int ret;

	ret = build_caches(caches, MAX_CACHES, &cnt);
=======
static int write_cache(struct feat_fd *ff,
		       struct evlist *evlist __maybe_unused)
{
	u32 max_caches = cpu__max_cpu() * MAX_CACHE_LVL;
	struct cpu_cache_level caches[max_caches];
	u32 cnt = 0, i, version = 1;
	int ret;

	ret = build_caches(caches, &cnt);
>>>>>>> upstream/android-13
	if (ret)
		goto out;

	qsort(&caches, cnt, sizeof(struct cpu_cache_level), cpu_cache_level__sort);

	ret = do_write(ff, &version, sizeof(u32));
	if (ret < 0)
		goto out;

	ret = do_write(ff, &cnt, sizeof(u32));
	if (ret < 0)
		goto out;

	for (i = 0; i < cnt; i++) {
		struct cpu_cache_level *c = &caches[i];

		#define _W(v)					\
			ret = do_write(ff, &c->v, sizeof(u32));	\
			if (ret < 0)				\
				goto out;

		_W(level)
		_W(line_size)
		_W(sets)
		_W(ways)
		#undef _W

		#define _W(v)						\
			ret = do_write_string(ff, (const char *) c->v);	\
			if (ret < 0)					\
				goto out;

		_W(type)
		_W(size)
		_W(map)
		#undef _W
	}

out:
	for (i = 0; i < cnt; i++)
		cpu_cache_level__free(&caches[i]);
	return ret;
}

static int write_stat(struct feat_fd *ff __maybe_unused,
<<<<<<< HEAD
		      struct perf_evlist *evlist __maybe_unused)
=======
		      struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	return 0;
}

static int write_sample_time(struct feat_fd *ff,
<<<<<<< HEAD
			     struct perf_evlist *evlist)
=======
			     struct evlist *evlist)
>>>>>>> upstream/android-13
{
	int ret;

	ret = do_write(ff, &evlist->first_sample_time,
		       sizeof(evlist->first_sample_time));
	if (ret < 0)
		return ret;

	return do_write(ff, &evlist->last_sample_time,
			sizeof(evlist->last_sample_time));
}


static int memory_node__read(struct memory_node *n, unsigned long idx)
{
	unsigned int phys, size = 0;
	char path[PATH_MAX];
	struct dirent *ent;
	DIR *dir;

#define for_each_memory(mem, dir)					\
	while ((ent = readdir(dir)))					\
		if (strcmp(ent->d_name, ".") &&				\
		    strcmp(ent->d_name, "..") &&			\
		    sscanf(ent->d_name, "memory%u", &mem) == 1)

	scnprintf(path, PATH_MAX,
		  "%s/devices/system/node/node%lu",
		  sysfs__mountpoint(), idx);

	dir = opendir(path);
	if (!dir) {
<<<<<<< HEAD
		pr_warning("failed: cant' open memory sysfs data\n");
=======
		pr_warning("failed: can't open memory sysfs data\n");
>>>>>>> upstream/android-13
		return -1;
	}

	for_each_memory(phys, dir) {
		size = max(phys, size);
	}

	size++;

<<<<<<< HEAD
	n->set = bitmap_alloc(size);
=======
	n->set = bitmap_zalloc(size);
>>>>>>> upstream/android-13
	if (!n->set) {
		closedir(dir);
		return -ENOMEM;
	}

	n->node = idx;
	n->size = size;

	rewinddir(dir);

	for_each_memory(phys, dir) {
		set_bit(phys, n->set);
	}

	closedir(dir);
	return 0;
}

static int memory_node__sort(const void *a, const void *b)
{
	const struct memory_node *na = a;
	const struct memory_node *nb = b;

	return na->node - nb->node;
}

static int build_mem_topology(struct memory_node *nodes, u64 size, u64 *cntp)
{
	char path[PATH_MAX];
	struct dirent *ent;
	DIR *dir;
	u64 cnt = 0;
	int ret = 0;

	scnprintf(path, PATH_MAX, "%s/devices/system/node/",
		  sysfs__mountpoint());

	dir = opendir(path);
	if (!dir) {
		pr_debug2("%s: could't read %s, does this arch have topology information?\n",
			  __func__, path);
		return -1;
	}

	while (!ret && (ent = readdir(dir))) {
		unsigned int idx;
		int r;

		if (!strcmp(ent->d_name, ".") ||
		    !strcmp(ent->d_name, ".."))
			continue;

		r = sscanf(ent->d_name, "node%u", &idx);
		if (r != 1)
			continue;

		if (WARN_ONCE(cnt >= size,
<<<<<<< HEAD
			      "failed to write MEM_TOPOLOGY, way too many nodes\n"))
			return -1;
=======
			"failed to write MEM_TOPOLOGY, way too many nodes\n")) {
			closedir(dir);
			return -1;
		}
>>>>>>> upstream/android-13

		ret = memory_node__read(&nodes[cnt++], idx);
	}

	*cntp = cnt;
	closedir(dir);

	if (!ret)
		qsort(nodes, cnt, sizeof(nodes[0]), memory_node__sort);

	return ret;
}

#define MAX_MEMORY_NODES 2000

/*
 * The MEM_TOPOLOGY holds physical memory map for every
 * node in system. The format of data is as follows:
 *
 *  0 - version          | for future changes
 *  8 - block_size_bytes | /sys/devices/system/memory/block_size_bytes
 * 16 - count            | number of nodes
 *
 * For each node we store map of physical indexes for
 * each node:
 *
 * 32 - node id          | node index
 * 40 - size             | size of bitmap
 * 48 - bitmap           | bitmap of memory indexes that belongs to node
 */
static int write_mem_topology(struct feat_fd *ff __maybe_unused,
<<<<<<< HEAD
			      struct perf_evlist *evlist __maybe_unused)
=======
			      struct evlist *evlist __maybe_unused)
>>>>>>> upstream/android-13
{
	static struct memory_node nodes[MAX_MEMORY_NODES];
	u64 bsize, version = 1, i, nr;
	int ret;

	ret = sysfs__read_xll("devices/system/memory/block_size_bytes",
			      (unsigned long long *) &bsize);
	if (ret)
		return ret;

	ret = build_mem_topology(&nodes[0], MAX_MEMORY_NODES, &nr);
	if (ret)
		return ret;

	ret = do_write(ff, &version, sizeof(version));
	if (ret < 0)
		goto out;

	ret = do_write(ff, &bsize, sizeof(bsize));
	if (ret < 0)
		goto out;

	ret = do_write(ff, &nr, sizeof(nr));
	if (ret < 0)
		goto out;

	for (i = 0; i < nr; i++) {
		struct memory_node *n = &nodes[i];

		#define _W(v)						\
			ret = do_write(ff, &n->v, sizeof(n->v));	\
			if (ret < 0)					\
				goto out;

		_W(node)
		_W(size)

		#undef _W

		ret = do_write_bitmap(ff, n->set, n->size);
		if (ret < 0)
			goto out;
	}

out:
	return ret;
}

<<<<<<< HEAD
=======
static int write_compressed(struct feat_fd *ff __maybe_unused,
			    struct evlist *evlist __maybe_unused)
{
	int ret;

	ret = do_write(ff, &(ff->ph->env.comp_ver), sizeof(ff->ph->env.comp_ver));
	if (ret)
		return ret;

	ret = do_write(ff, &(ff->ph->env.comp_type), sizeof(ff->ph->env.comp_type));
	if (ret)
		return ret;

	ret = do_write(ff, &(ff->ph->env.comp_level), sizeof(ff->ph->env.comp_level));
	if (ret)
		return ret;

	ret = do_write(ff, &(ff->ph->env.comp_ratio), sizeof(ff->ph->env.comp_ratio));
	if (ret)
		return ret;

	return do_write(ff, &(ff->ph->env.comp_mmap_len), sizeof(ff->ph->env.comp_mmap_len));
}

static int write_per_cpu_pmu_caps(struct feat_fd *ff, struct perf_pmu *pmu,
				  bool write_pmu)
{
	struct perf_pmu_caps *caps = NULL;
	int nr_caps;
	int ret;

	nr_caps = perf_pmu__caps_parse(pmu);
	if (nr_caps < 0)
		return nr_caps;

	ret = do_write(ff, &nr_caps, sizeof(nr_caps));
	if (ret < 0)
		return ret;

	list_for_each_entry(caps, &pmu->caps, list) {
		ret = do_write_string(ff, caps->name);
		if (ret < 0)
			return ret;

		ret = do_write_string(ff, caps->value);
		if (ret < 0)
			return ret;
	}

	if (write_pmu) {
		ret = do_write_string(ff, pmu->name);
		if (ret < 0)
			return ret;
	}

	return ret;
}

static int write_cpu_pmu_caps(struct feat_fd *ff,
			      struct evlist *evlist __maybe_unused)
{
	struct perf_pmu *cpu_pmu = perf_pmu__find("cpu");

	if (!cpu_pmu)
		return -ENOENT;

	return write_per_cpu_pmu_caps(ff, cpu_pmu, false);
}

static int write_hybrid_cpu_pmu_caps(struct feat_fd *ff,
				     struct evlist *evlist __maybe_unused)
{
	struct perf_pmu *pmu;
	u32 nr_pmu = perf_pmu__hybrid_pmu_num();
	int ret;

	if (nr_pmu == 0)
		return -ENOENT;

	ret = do_write(ff, &nr_pmu, sizeof(nr_pmu));
	if (ret < 0)
		return ret;

	perf_pmu__for_each_hybrid_pmu(pmu) {
		ret = write_per_cpu_pmu_caps(ff, pmu, true);
		if (ret < 0)
			return ret;
	}

	return 0;
}

>>>>>>> upstream/android-13
static void print_hostname(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# hostname : %s\n", ff->ph->env.hostname);
}

static void print_osrelease(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# os release : %s\n", ff->ph->env.os_release);
}

static void print_arch(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# arch : %s\n", ff->ph->env.arch);
}

static void print_cpudesc(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# cpudesc : %s\n", ff->ph->env.cpu_desc);
}

static void print_nrcpus(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# nrcpus online : %u\n", ff->ph->env.nr_cpus_online);
	fprintf(fp, "# nrcpus avail : %u\n", ff->ph->env.nr_cpus_avail);
}

static void print_version(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# perf version : %s\n", ff->ph->env.version);
}

static void print_cmdline(struct feat_fd *ff, FILE *fp)
{
	int nr, i;

	nr = ff->ph->env.nr_cmdline;

	fprintf(fp, "# cmdline : ");

	for (i = 0; i < nr; i++) {
		char *argv_i = strdup(ff->ph->env.cmdline_argv[i]);
		if (!argv_i) {
			fprintf(fp, "%s ", ff->ph->env.cmdline_argv[i]);
		} else {
			char *mem = argv_i;
			do {
				char *quote = strchr(argv_i, '\'');
				if (!quote)
					break;
				*quote++ = '\0';
				fprintf(fp, "%s\\\'", argv_i);
				argv_i = quote;
			} while (1);
			fprintf(fp, "%s ", argv_i);
			free(mem);
		}
	}
	fputc('\n', fp);
}

static void print_cpu_topology(struct feat_fd *ff, FILE *fp)
{
	struct perf_header *ph = ff->ph;
	int cpu_nr = ph->env.nr_cpus_avail;
	int nr, i;
	char *str;

	nr = ph->env.nr_sibling_cores;
	str = ph->env.sibling_cores;

	for (i = 0; i < nr; i++) {
<<<<<<< HEAD
		fprintf(fp, "# sibling cores   : %s\n", str);
		str += strlen(str) + 1;
	}

=======
		fprintf(fp, "# sibling sockets : %s\n", str);
		str += strlen(str) + 1;
	}

	if (ph->env.nr_sibling_dies) {
		nr = ph->env.nr_sibling_dies;
		str = ph->env.sibling_dies;

		for (i = 0; i < nr; i++) {
			fprintf(fp, "# sibling dies    : %s\n", str);
			str += strlen(str) + 1;
		}
	}

>>>>>>> upstream/android-13
	nr = ph->env.nr_sibling_threads;
	str = ph->env.sibling_threads;

	for (i = 0; i < nr; i++) {
		fprintf(fp, "# sibling threads : %s\n", str);
		str += strlen(str) + 1;
	}

<<<<<<< HEAD
	if (ph->env.cpu != NULL) {
		for (i = 0; i < cpu_nr; i++)
			fprintf(fp, "# CPU %d: Core ID %d, Socket ID %d\n", i,
				ph->env.cpu[i].core_id, ph->env.cpu[i].socket_id);
	} else
		fprintf(fp, "# Core ID and Socket ID information is not available\n");
}

static void free_event_desc(struct perf_evsel *events)
{
	struct perf_evsel *evsel;
=======
	if (ph->env.nr_sibling_dies) {
		if (ph->env.cpu != NULL) {
			for (i = 0; i < cpu_nr; i++)
				fprintf(fp, "# CPU %d: Core ID %d, "
					    "Die ID %d, Socket ID %d\n",
					    i, ph->env.cpu[i].core_id,
					    ph->env.cpu[i].die_id,
					    ph->env.cpu[i].socket_id);
		} else
			fprintf(fp, "# Core ID, Die ID and Socket ID "
				    "information is not available\n");
	} else {
		if (ph->env.cpu != NULL) {
			for (i = 0; i < cpu_nr; i++)
				fprintf(fp, "# CPU %d: Core ID %d, "
					    "Socket ID %d\n",
					    i, ph->env.cpu[i].core_id,
					    ph->env.cpu[i].socket_id);
		} else
			fprintf(fp, "# Core ID and Socket ID "
				    "information is not available\n");
	}
}

static void print_clockid(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# clockid frequency: %"PRIu64" MHz\n",
		ff->ph->env.clock.clockid_res_ns * 1000);
}

static void print_clock_data(struct feat_fd *ff, FILE *fp)
{
	struct timespec clockid_ns;
	char tstr[64], date[64];
	struct timeval tod_ns;
	clockid_t clockid;
	struct tm ltime;
	u64 ref;

	if (!ff->ph->env.clock.enabled) {
		fprintf(fp, "# reference time disabled\n");
		return;
	}

	/* Compute TOD time. */
	ref = ff->ph->env.clock.tod_ns;
	tod_ns.tv_sec = ref / NSEC_PER_SEC;
	ref -= tod_ns.tv_sec * NSEC_PER_SEC;
	tod_ns.tv_usec = ref / NSEC_PER_USEC;

	/* Compute clockid time. */
	ref = ff->ph->env.clock.clockid_ns;
	clockid_ns.tv_sec = ref / NSEC_PER_SEC;
	ref -= clockid_ns.tv_sec * NSEC_PER_SEC;
	clockid_ns.tv_nsec = ref;

	clockid = ff->ph->env.clock.clockid;

	if (localtime_r(&tod_ns.tv_sec, &ltime) == NULL)
		snprintf(tstr, sizeof(tstr), "<error>");
	else {
		strftime(date, sizeof(date), "%F %T", &ltime);
		scnprintf(tstr, sizeof(tstr), "%s.%06d",
			  date, (int) tod_ns.tv_usec);
	}

	fprintf(fp, "# clockid: %s (%u)\n", clockid_name(clockid), clockid);
	fprintf(fp, "# reference time: %s = %ld.%06d (TOD) = %ld.%09ld (%s)\n",
		    tstr, (long) tod_ns.tv_sec, (int) tod_ns.tv_usec,
		    (long) clockid_ns.tv_sec, clockid_ns.tv_nsec,
		    clockid_name(clockid));
}

static void print_hybrid_topology(struct feat_fd *ff, FILE *fp)
{
	int i;
	struct hybrid_node *n;

	fprintf(fp, "# hybrid cpu system:\n");
	for (i = 0; i < ff->ph->env.nr_hybrid_nodes; i++) {
		n = &ff->ph->env.hybrid_nodes[i];
		fprintf(fp, "# %s cpu list : %s\n", n->pmu_name, n->cpus);
	}
}

static void print_dir_format(struct feat_fd *ff, FILE *fp)
{
	struct perf_session *session;
	struct perf_data *data;

	session = container_of(ff->ph, struct perf_session, header);
	data = session->data;

	fprintf(fp, "# directory data version : %"PRIu64"\n", data->dir.version);
}

#ifdef HAVE_LIBBPF_SUPPORT
static void print_bpf_prog_info(struct feat_fd *ff, FILE *fp)
{
	struct perf_env *env = &ff->ph->env;
	struct rb_root *root;
	struct rb_node *next;

	down_read(&env->bpf_progs.lock);

	root = &env->bpf_progs.infos;
	next = rb_first(root);

	while (next) {
		struct bpf_prog_info_node *node;

		node = rb_entry(next, struct bpf_prog_info_node, rb_node);
		next = rb_next(&node->rb_node);

		bpf_event__print_bpf_prog_info(&node->info_linear->info,
					       env, fp);
	}

	up_read(&env->bpf_progs.lock);
}

static void print_bpf_btf(struct feat_fd *ff, FILE *fp)
{
	struct perf_env *env = &ff->ph->env;
	struct rb_root *root;
	struct rb_node *next;

	down_read(&env->bpf_progs.lock);

	root = &env->bpf_progs.btfs;
	next = rb_first(root);

	while (next) {
		struct btf_node *node;

		node = rb_entry(next, struct btf_node, rb_node);
		next = rb_next(&node->rb_node);
		fprintf(fp, "# btf info of id %u\n", node->id);
	}

	up_read(&env->bpf_progs.lock);
}
#endif // HAVE_LIBBPF_SUPPORT

static void free_event_desc(struct evsel *events)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13

	if (!events)
		return;

<<<<<<< HEAD
	for (evsel = events; evsel->attr.size; evsel++) {
		zfree(&evsel->name);
		zfree(&evsel->id);
=======
	for (evsel = events; evsel->core.attr.size; evsel++) {
		zfree(&evsel->name);
		zfree(&evsel->core.id);
>>>>>>> upstream/android-13
	}

	free(events);
}

<<<<<<< HEAD
static struct perf_evsel *read_event_desc(struct feat_fd *ff)
{
	struct perf_evsel *evsel, *events = NULL;
=======
static bool perf_attr_check(struct perf_event_attr *attr)
{
	if (attr->__reserved_1 || attr->__reserved_2 || attr->__reserved_3) {
		pr_warning("Reserved bits are set unexpectedly. "
			   "Please update perf tool.\n");
		return false;
	}

	if (attr->sample_type & ~(PERF_SAMPLE_MAX-1)) {
		pr_warning("Unknown sample type (0x%llx) is detected. "
			   "Please update perf tool.\n",
			   attr->sample_type);
		return false;
	}

	if (attr->read_format & ~(PERF_FORMAT_MAX-1)) {
		pr_warning("Unknown read format (0x%llx) is detected. "
			   "Please update perf tool.\n",
			   attr->read_format);
		return false;
	}

	if ((attr->sample_type & PERF_SAMPLE_BRANCH_STACK) &&
	    (attr->branch_sample_type & ~(PERF_SAMPLE_BRANCH_MAX-1))) {
		pr_warning("Unknown branch sample type (0x%llx) is detected. "
			   "Please update perf tool.\n",
			   attr->branch_sample_type);

		return false;
	}

	return true;
}

static struct evsel *read_event_desc(struct feat_fd *ff)
{
	struct evsel *evsel, *events = NULL;
>>>>>>> upstream/android-13
	u64 *id;
	void *buf = NULL;
	u32 nre, sz, nr, i, j;
	size_t msz;

	/* number of events */
	if (do_read_u32(ff, &nre))
		goto error;

	if (do_read_u32(ff, &sz))
		goto error;

	/* buffer to hold on file attr struct */
	buf = malloc(sz);
	if (!buf)
		goto error;

<<<<<<< HEAD
	/* the last event terminates with evsel->attr.size == 0: */
=======
	/* the last event terminates with evsel->core.attr.size == 0: */
>>>>>>> upstream/android-13
	events = calloc(nre + 1, sizeof(*events));
	if (!events)
		goto error;

<<<<<<< HEAD
	msz = sizeof(evsel->attr);
=======
	msz = sizeof(evsel->core.attr);
>>>>>>> upstream/android-13
	if (sz < msz)
		msz = sz;

	for (i = 0, evsel = events; i < nre; evsel++, i++) {
<<<<<<< HEAD
		evsel->idx = i;
=======
		evsel->core.idx = i;
>>>>>>> upstream/android-13

		/*
		 * must read entire on-file attr struct to
		 * sync up with layout.
		 */
		if (__do_read(ff, buf, sz))
			goto error;

		if (ff->ph->needs_swap)
			perf_event__attr_swap(buf);

<<<<<<< HEAD
		memcpy(&evsel->attr, buf, msz);
=======
		memcpy(&evsel->core.attr, buf, msz);

		if (!perf_attr_check(&evsel->core.attr))
			goto error;
>>>>>>> upstream/android-13

		if (do_read_u32(ff, &nr))
			goto error;

		if (ff->ph->needs_swap)
			evsel->needs_swap = true;

		evsel->name = do_read_string(ff);
		if (!evsel->name)
			goto error;

		if (!nr)
			continue;

		id = calloc(nr, sizeof(*id));
		if (!id)
			goto error;
<<<<<<< HEAD
		evsel->ids = nr;
		evsel->id = id;
=======
		evsel->core.ids = nr;
		evsel->core.id = id;
>>>>>>> upstream/android-13

		for (j = 0 ; j < nr; j++) {
			if (do_read_u64(ff, id))
				goto error;
			id++;
		}
	}
out:
	free(buf);
	return events;
error:
	free_event_desc(events);
	events = NULL;
	goto out;
}

static int __desc_attr__fprintf(FILE *fp, const char *name, const char *val,
				void *priv __maybe_unused)
{
	return fprintf(fp, ", %s = %s", name, val);
}

static void print_event_desc(struct feat_fd *ff, FILE *fp)
{
<<<<<<< HEAD
	struct perf_evsel *evsel, *events;
=======
	struct evsel *evsel, *events;
>>>>>>> upstream/android-13
	u32 j;
	u64 *id;

	if (ff->events)
		events = ff->events;
	else
		events = read_event_desc(ff);

	if (!events) {
		fprintf(fp, "# event desc: not available or unable to read\n");
		return;
	}

<<<<<<< HEAD
	for (evsel = events; evsel->attr.size; evsel++) {
		fprintf(fp, "# event : name = %s, ", evsel->name);

		if (evsel->ids) {
			fprintf(fp, ", id = {");
			for (j = 0, id = evsel->id; j < evsel->ids; j++, id++) {
=======
	for (evsel = events; evsel->core.attr.size; evsel++) {
		fprintf(fp, "# event : name = %s, ", evsel->name);

		if (evsel->core.ids) {
			fprintf(fp, ", id = {");
			for (j = 0, id = evsel->core.id; j < evsel->core.ids; j++, id++) {
>>>>>>> upstream/android-13
				if (j)
					fputc(',', fp);
				fprintf(fp, " %"PRIu64, *id);
			}
			fprintf(fp, " }");
		}

<<<<<<< HEAD
		perf_event_attr__fprintf(fp, &evsel->attr, __desc_attr__fprintf, NULL);
=======
		perf_event_attr__fprintf(fp, &evsel->core.attr, __desc_attr__fprintf, NULL);
>>>>>>> upstream/android-13

		fputc('\n', fp);
	}

	free_event_desc(events);
	ff->events = NULL;
}

static void print_total_mem(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# total memory : %llu kB\n", ff->ph->env.total_mem);
}

static void print_numa_topology(struct feat_fd *ff, FILE *fp)
{
	int i;
	struct numa_node *n;

	for (i = 0; i < ff->ph->env.nr_numa_nodes; i++) {
		n = &ff->ph->env.numa_nodes[i];

		fprintf(fp, "# node%u meminfo  : total = %"PRIu64" kB,"
			    " free = %"PRIu64" kB\n",
			n->node, n->mem_total, n->mem_free);

		fprintf(fp, "# node%u cpu list : ", n->node);
		cpu_map__fprintf(n->map, fp);
	}
}

static void print_cpuid(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# cpuid : %s\n", ff->ph->env.cpuid);
}

static void print_branch_stack(struct feat_fd *ff __maybe_unused, FILE *fp)
{
	fprintf(fp, "# contains samples with branch stack\n");
}

static void print_auxtrace(struct feat_fd *ff __maybe_unused, FILE *fp)
{
	fprintf(fp, "# contains AUX area data (e.g. instruction trace)\n");
}

static void print_stat(struct feat_fd *ff __maybe_unused, FILE *fp)
{
	fprintf(fp, "# contains stat data\n");
}

static void print_cache(struct feat_fd *ff, FILE *fp __maybe_unused)
{
	int i;

	fprintf(fp, "# CPU cache info:\n");
	for (i = 0; i < ff->ph->env.caches_cnt; i++) {
		fprintf(fp, "#  ");
		cpu_cache_level__fprintf(fp, &ff->ph->env.caches[i]);
	}
}

<<<<<<< HEAD
=======
static void print_compressed(struct feat_fd *ff, FILE *fp)
{
	fprintf(fp, "# compressed : %s, level = %d, ratio = %d\n",
		ff->ph->env.comp_type == PERF_COMP_ZSTD ? "Zstd" : "Unknown",
		ff->ph->env.comp_level, ff->ph->env.comp_ratio);
}

static void print_per_cpu_pmu_caps(FILE *fp, int nr_caps, char *cpu_pmu_caps,
				   char *pmu_name)
{
	const char *delimiter;
	char *str, buf[128];

	if (!nr_caps) {
		if (!pmu_name)
			fprintf(fp, "# cpu pmu capabilities: not available\n");
		else
			fprintf(fp, "# %s pmu capabilities: not available\n", pmu_name);
		return;
	}

	if (!pmu_name)
		scnprintf(buf, sizeof(buf), "# cpu pmu capabilities: ");
	else
		scnprintf(buf, sizeof(buf), "# %s pmu capabilities: ", pmu_name);

	delimiter = buf;

	str = cpu_pmu_caps;
	while (nr_caps--) {
		fprintf(fp, "%s%s", delimiter, str);
		delimiter = ", ";
		str += strlen(str) + 1;
	}

	fprintf(fp, "\n");
}

static void print_cpu_pmu_caps(struct feat_fd *ff, FILE *fp)
{
	print_per_cpu_pmu_caps(fp, ff->ph->env.nr_cpu_pmu_caps,
			       ff->ph->env.cpu_pmu_caps, NULL);
}

static void print_hybrid_cpu_pmu_caps(struct feat_fd *ff, FILE *fp)
{
	struct hybrid_cpc_node *n;

	for (int i = 0; i < ff->ph->env.nr_hybrid_cpc_nodes; i++) {
		n = &ff->ph->env.hybrid_cpc_nodes[i];
		print_per_cpu_pmu_caps(fp, n->nr_cpu_pmu_caps,
				       n->cpu_pmu_caps,
				       n->pmu_name);
	}
}

>>>>>>> upstream/android-13
static void print_pmu_mappings(struct feat_fd *ff, FILE *fp)
{
	const char *delimiter = "# pmu mappings: ";
	char *str, *tmp;
	u32 pmu_num;
	u32 type;

	pmu_num = ff->ph->env.nr_pmu_mappings;
	if (!pmu_num) {
		fprintf(fp, "# pmu mappings: not available\n");
		return;
	}

	str = ff->ph->env.pmu_mappings;

	while (pmu_num) {
		type = strtoul(str, &tmp, 0);
		if (*tmp != ':')
			goto error;

		str = tmp + 1;
		fprintf(fp, "%s%s = %" PRIu32, delimiter, str, type);

		delimiter = ", ";
		str += strlen(str) + 1;
		pmu_num--;
	}

	fprintf(fp, "\n");

	if (!pmu_num)
		return;
error:
	fprintf(fp, "# pmu mappings: unable to read\n");
}

static void print_group_desc(struct feat_fd *ff, FILE *fp)
{
	struct perf_session *session;
<<<<<<< HEAD
	struct perf_evsel *evsel;
=======
	struct evsel *evsel;
>>>>>>> upstream/android-13
	u32 nr = 0;

	session = container_of(ff->ph, struct perf_session, header);

	evlist__for_each_entry(session->evlist, evsel) {
<<<<<<< HEAD
		if (perf_evsel__is_group_leader(evsel) &&
		    evsel->nr_members > 1) {
			fprintf(fp, "# group: %s{%s", evsel->group_name ?: "",
				perf_evsel__name(evsel));

			nr = evsel->nr_members - 1;
		} else if (nr) {
			fprintf(fp, ",%s", perf_evsel__name(evsel));
=======
		if (evsel__is_group_leader(evsel) && evsel->core.nr_members > 1) {
			fprintf(fp, "# group: %s{%s", evsel->group_name ?: "", evsel__name(evsel));

			nr = evsel->core.nr_members - 1;
		} else if (nr) {
			fprintf(fp, ",%s", evsel__name(evsel));
>>>>>>> upstream/android-13

			if (--nr == 0)
				fprintf(fp, "}\n");
		}
	}
}

static void print_sample_time(struct feat_fd *ff, FILE *fp)
{
	struct perf_session *session;
	char time_buf[32];
	double d;

	session = container_of(ff->ph, struct perf_session, header);

	timestamp__scnprintf_usec(session->evlist->first_sample_time,
				  time_buf, sizeof(time_buf));
	fprintf(fp, "# time of first sample : %s\n", time_buf);

	timestamp__scnprintf_usec(session->evlist->last_sample_time,
				  time_buf, sizeof(time_buf));
	fprintf(fp, "# time of last sample : %s\n", time_buf);

	d = (double)(session->evlist->last_sample_time -
		session->evlist->first_sample_time) / NSEC_PER_MSEC;

	fprintf(fp, "# sample duration : %10.3f ms\n", d);
}

static void memory_node__fprintf(struct memory_node *n,
				 unsigned long long bsize, FILE *fp)
{
	char buf_map[100], buf_size[50];
	unsigned long long size;

	size = bsize * bitmap_weight(n->set, n->size);
	unit_number__scnprintf(buf_size, 50, size);

	bitmap_scnprintf(n->set, n->size, buf_map, 100);
	fprintf(fp, "#  %3" PRIu64 " [%s]: %s\n", n->node, buf_size, buf_map);
}

static void print_mem_topology(struct feat_fd *ff, FILE *fp)
{
	struct memory_node *nodes;
	int i, nr;

	nodes = ff->ph->env.memory_nodes;
	nr    = ff->ph->env.nr_memory_nodes;

	fprintf(fp, "# memory nodes (nr %d, block size 0x%llx):\n",
		nr, ff->ph->env.memory_bsize);

	for (i = 0; i < nr; i++) {
		memory_node__fprintf(&nodes[i], ff->ph->env.memory_bsize, fp);
	}
}

<<<<<<< HEAD
static int __event_process_build_id(struct build_id_event *bev,
=======
static int __event_process_build_id(struct perf_record_header_build_id *bev,
>>>>>>> upstream/android-13
				    char *filename,
				    struct perf_session *session)
{
	int err = -1;
	struct machine *machine;
	u16 cpumode;
	struct dso *dso;
<<<<<<< HEAD
	enum dso_kernel_type dso_type;
=======
	enum dso_space_type dso_space;
>>>>>>> upstream/android-13

	machine = perf_session__findnew_machine(session, bev->pid);
	if (!machine)
		goto out;

	cpumode = bev->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;

	switch (cpumode) {
	case PERF_RECORD_MISC_KERNEL:
<<<<<<< HEAD
		dso_type = DSO_TYPE_KERNEL;
		break;
	case PERF_RECORD_MISC_GUEST_KERNEL:
		dso_type = DSO_TYPE_GUEST_KERNEL;
		break;
	case PERF_RECORD_MISC_USER:
	case PERF_RECORD_MISC_GUEST_USER:
		dso_type = DSO_TYPE_USER;
=======
		dso_space = DSO_SPACE__KERNEL;
		break;
	case PERF_RECORD_MISC_GUEST_KERNEL:
		dso_space = DSO_SPACE__KERNEL_GUEST;
		break;
	case PERF_RECORD_MISC_USER:
	case PERF_RECORD_MISC_GUEST_USER:
		dso_space = DSO_SPACE__USER;
>>>>>>> upstream/android-13
		break;
	default:
		goto out;
	}

	dso = machine__findnew_dso(machine, filename);
	if (dso != NULL) {
		char sbuild_id[SBUILD_ID_SIZE];
<<<<<<< HEAD

		dso__set_build_id(dso, &bev->build_id);

		if (dso_type != DSO_TYPE_USER) {
=======
		struct build_id bid;
		size_t size = BUILD_ID_SIZE;

		if (bev->header.misc & PERF_RECORD_MISC_BUILD_ID_SIZE)
			size = bev->size;

		build_id__init(&bid, bev->data, size);
		dso__set_build_id(dso, &bid);

		if (dso_space != DSO_SPACE__USER) {
>>>>>>> upstream/android-13
			struct kmod_path m = { .name = NULL, };

			if (!kmod_path__parse_name(&m, filename) && m.kmod)
				dso__set_module_info(dso, &m, machine);
<<<<<<< HEAD
			else
				dso->kernel = dso_type;

			free(m.name);
		}

		build_id__sprintf(dso->build_id, sizeof(dso->build_id),
				  sbuild_id);
		pr_debug("build id event received for %s: %s\n",
			 dso->long_name, sbuild_id);
=======

			dso->kernel = dso_space;
			free(m.name);
		}

		build_id__sprintf(&dso->bid, sbuild_id);
		pr_debug("build id event received for %s: %s [%zu]\n",
			 dso->long_name, sbuild_id, size);
>>>>>>> upstream/android-13
		dso__put(dso);
	}

	err = 0;
out:
	return err;
}

static int perf_header__read_build_ids_abi_quirk(struct perf_header *header,
						 int input, u64 offset, u64 size)
{
	struct perf_session *session = container_of(header, struct perf_session, header);
	struct {
		struct perf_event_header   header;
		u8			   build_id[PERF_ALIGN(BUILD_ID_SIZE, sizeof(u64))];
		char			   filename[0];
	} old_bev;
<<<<<<< HEAD
	struct build_id_event bev;
=======
	struct perf_record_header_build_id bev;
>>>>>>> upstream/android-13
	char filename[PATH_MAX];
	u64 limit = offset + size;

	while (offset < limit) {
		ssize_t len;

		if (readn(input, &old_bev, sizeof(old_bev)) != sizeof(old_bev))
			return -1;

		if (header->needs_swap)
			perf_event_header__bswap(&old_bev.header);

		len = old_bev.header.size - sizeof(old_bev);
		if (readn(input, filename, len) != len)
			return -1;

		bev.header = old_bev.header;

		/*
		 * As the pid is the missing value, we need to fill
		 * it properly. The header.misc value give us nice hint.
		 */
		bev.pid	= HOST_KERNEL_ID;
		if (bev.header.misc == PERF_RECORD_MISC_GUEST_USER ||
		    bev.header.misc == PERF_RECORD_MISC_GUEST_KERNEL)
			bev.pid	= DEFAULT_GUEST_KERNEL_ID;

		memcpy(bev.build_id, old_bev.build_id, sizeof(bev.build_id));
		__event_process_build_id(&bev, filename, session);

		offset += bev.header.size;
	}

	return 0;
}

static int perf_header__read_build_ids(struct perf_header *header,
				       int input, u64 offset, u64 size)
{
	struct perf_session *session = container_of(header, struct perf_session, header);
<<<<<<< HEAD
	struct build_id_event bev;
=======
	struct perf_record_header_build_id bev;
>>>>>>> upstream/android-13
	char filename[PATH_MAX];
	u64 limit = offset + size, orig_offset = offset;
	int err = -1;

	while (offset < limit) {
		ssize_t len;

		if (readn(input, &bev, sizeof(bev)) != sizeof(bev))
			goto out;

		if (header->needs_swap)
			perf_event_header__bswap(&bev.header);

		len = bev.header.size - sizeof(bev);
		if (readn(input, filename, len) != len)
			goto out;
		/*
		 * The a1645ce1 changeset:
		 *
		 * "perf: 'perf kvm' tool for monitoring guest performance from host"
		 *
<<<<<<< HEAD
		 * Added a field to struct build_id_event that broke the file
=======
		 * Added a field to struct perf_record_header_build_id that broke the file
>>>>>>> upstream/android-13
		 * format.
		 *
		 * Since the kernel build-id is the first entry, process the
		 * table using the old format if the well known
		 * '[kernel.kallsyms]' string for the kernel build-id has the
		 * first 4 characters chopped off (where the pid_t sits).
		 */
		if (memcmp(filename, "nel.kallsyms]", 13) == 0) {
			if (lseek(input, orig_offset, SEEK_SET) == (off_t)-1)
				return -1;
			return perf_header__read_build_ids_abi_quirk(header, input, offset, size);
		}

		__event_process_build_id(&bev, filename, session);

		offset += bev.header.size;
	}
	err = 0;
out:
	return err;
}

/* Macro for features that simply need to read and store a string. */
#define FEAT_PROCESS_STR_FUN(__feat, __feat_env) \
static int process_##__feat(struct feat_fd *ff, void *data __maybe_unused) \
{\
	ff->ph->env.__feat_env = do_read_string(ff); \
	return ff->ph->env.__feat_env ? 0 : -ENOMEM; \
}

FEAT_PROCESS_STR_FUN(hostname, hostname);
FEAT_PROCESS_STR_FUN(osrelease, os_release);
FEAT_PROCESS_STR_FUN(version, version);
FEAT_PROCESS_STR_FUN(arch, arch);
FEAT_PROCESS_STR_FUN(cpudesc, cpu_desc);
FEAT_PROCESS_STR_FUN(cpuid, cpuid);

static int process_tracing_data(struct feat_fd *ff, void *data)
{
	ssize_t ret = trace_report(ff->fd, data, false);

	return ret < 0 ? -1 : 0;
}

static int process_build_id(struct feat_fd *ff, void *data __maybe_unused)
{
	if (perf_header__read_build_ids(ff->ph, ff->fd, ff->offset, ff->size))
		pr_debug("Failed to read buildids, continuing...\n");
	return 0;
}

static int process_nrcpus(struct feat_fd *ff, void *data __maybe_unused)
{
	int ret;
	u32 nr_cpus_avail, nr_cpus_online;

	ret = do_read_u32(ff, &nr_cpus_avail);
	if (ret)
		return ret;

	ret = do_read_u32(ff, &nr_cpus_online);
	if (ret)
		return ret;
	ff->ph->env.nr_cpus_avail = (int)nr_cpus_avail;
	ff->ph->env.nr_cpus_online = (int)nr_cpus_online;
	return 0;
}

static int process_total_mem(struct feat_fd *ff, void *data __maybe_unused)
{
	u64 total_mem;
	int ret;

	ret = do_read_u64(ff, &total_mem);
	if (ret)
		return -1;
	ff->ph->env.total_mem = (unsigned long long)total_mem;
	return 0;
}

<<<<<<< HEAD
static struct perf_evsel *
perf_evlist__find_by_index(struct perf_evlist *evlist, int idx)
{
	struct perf_evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->idx == idx)
=======
static struct evsel *evlist__find_by_index(struct evlist *evlist, int idx)
{
	struct evsel *evsel;

	evlist__for_each_entry(evlist, evsel) {
		if (evsel->core.idx == idx)
>>>>>>> upstream/android-13
			return evsel;
	}

	return NULL;
}

<<<<<<< HEAD
static void
perf_evlist__set_event_name(struct perf_evlist *evlist,
			    struct perf_evsel *event)
{
	struct perf_evsel *evsel;
=======
static void evlist__set_event_name(struct evlist *evlist, struct evsel *event)
{
	struct evsel *evsel;
>>>>>>> upstream/android-13

	if (!event->name)
		return;

<<<<<<< HEAD
	evsel = perf_evlist__find_by_index(evlist, event->idx);
=======
	evsel = evlist__find_by_index(evlist, event->core.idx);
>>>>>>> upstream/android-13
	if (!evsel)
		return;

	if (evsel->name)
		return;

	evsel->name = strdup(event->name);
}

static int
process_event_desc(struct feat_fd *ff, void *data __maybe_unused)
{
	struct perf_session *session;
<<<<<<< HEAD
	struct perf_evsel *evsel, *events = read_event_desc(ff);
=======
	struct evsel *evsel, *events = read_event_desc(ff);
>>>>>>> upstream/android-13

	if (!events)
		return 0;

	session = container_of(ff->ph, struct perf_session, header);

	if (session->data->is_pipe) {
		/* Save events for reading later by print_event_desc,
		 * since they can't be read again in pipe mode. */
		ff->events = events;
	}

<<<<<<< HEAD
	for (evsel = events; evsel->attr.size; evsel++)
		perf_evlist__set_event_name(session->evlist, evsel);
=======
	for (evsel = events; evsel->core.attr.size; evsel++)
		evlist__set_event_name(session->evlist, evsel);
>>>>>>> upstream/android-13

	if (!session->data->is_pipe)
		free_event_desc(events);

	return 0;
}

static int process_cmdline(struct feat_fd *ff, void *data __maybe_unused)
{
	char *str, *cmdline = NULL, **argv = NULL;
	u32 nr, i, len = 0;

	if (do_read_u32(ff, &nr))
		return -1;

	ff->ph->env.nr_cmdline = nr;

	cmdline = zalloc(ff->size + nr + 1);
	if (!cmdline)
		return -1;

	argv = zalloc(sizeof(char *) * (nr + 1));
	if (!argv)
		goto error;

	for (i = 0; i < nr; i++) {
		str = do_read_string(ff);
		if (!str)
			goto error;

		argv[i] = cmdline + len;
		memcpy(argv[i], str, strlen(str) + 1);
		len += strlen(str) + 1;
		free(str);
	}
	ff->ph->env.cmdline = cmdline;
	ff->ph->env.cmdline_argv = (const char **) argv;
	return 0;

error:
	free(argv);
	free(cmdline);
	return -1;
}

static int process_cpu_topology(struct feat_fd *ff, void *data __maybe_unused)
{
	u32 nr, i;
	char *str;
	struct strbuf sb;
	int cpu_nr = ff->ph->env.nr_cpus_avail;
	u64 size = 0;
	struct perf_header *ph = ff->ph;
	bool do_core_id_test = true;

	ph->env.cpu = calloc(cpu_nr, sizeof(*ph->env.cpu));
	if (!ph->env.cpu)
		return -1;

	if (do_read_u32(ff, &nr))
		goto free_cpu;

	ph->env.nr_sibling_cores = nr;
	size += sizeof(u32);
	if (strbuf_init(&sb, 128) < 0)
		goto free_cpu;

	for (i = 0; i < nr; i++) {
		str = do_read_string(ff);
		if (!str)
			goto error;

		/* include a NULL character at the end */
		if (strbuf_add(&sb, str, strlen(str) + 1) < 0)
			goto error;
		size += string_size(str);
		free(str);
	}
	ph->env.sibling_cores = strbuf_detach(&sb, NULL);

	if (do_read_u32(ff, &nr))
		return -1;

	ph->env.nr_sibling_threads = nr;
	size += sizeof(u32);

	for (i = 0; i < nr; i++) {
		str = do_read_string(ff);
		if (!str)
			goto error;

		/* include a NULL character at the end */
		if (strbuf_add(&sb, str, strlen(str) + 1) < 0)
			goto error;
		size += string_size(str);
		free(str);
	}
	ph->env.sibling_threads = strbuf_detach(&sb, NULL);

	/*
	 * The header may be from old perf,
	 * which doesn't include core id and socket id information.
	 */
	if (ff->size <= size) {
		zfree(&ph->env.cpu);
		return 0;
	}

	/* On s390 the socket_id number is not related to the numbers of cpus.
	 * The socket_id number might be higher than the numbers of cpus.
	 * This depends on the configuration.
	 * AArch64 is the same.
	 */
	if (ph->env.arch && (!strncmp(ph->env.arch, "s390", 4)
			  || !strncmp(ph->env.arch, "aarch64", 7)))
		do_core_id_test = false;

	for (i = 0; i < (u32)cpu_nr; i++) {
		if (do_read_u32(ff, &nr))
			goto free_cpu;

		ph->env.cpu[i].core_id = nr;
<<<<<<< HEAD
=======
		size += sizeof(u32);
>>>>>>> upstream/android-13

		if (do_read_u32(ff, &nr))
			goto free_cpu;

		if (do_core_id_test && nr != (u32)-1 && nr > (u32)cpu_nr) {
			pr_debug("socket_id number is too big."
				 "You may need to upgrade the perf tool.\n");
			goto free_cpu;
		}

		ph->env.cpu[i].socket_id = nr;
<<<<<<< HEAD
=======
		size += sizeof(u32);
	}

	/*
	 * The header may be from old perf,
	 * which doesn't include die information.
	 */
	if (ff->size <= size)
		return 0;

	if (do_read_u32(ff, &nr))
		return -1;

	ph->env.nr_sibling_dies = nr;
	size += sizeof(u32);

	for (i = 0; i < nr; i++) {
		str = do_read_string(ff);
		if (!str)
			goto error;

		/* include a NULL character at the end */
		if (strbuf_add(&sb, str, strlen(str) + 1) < 0)
			goto error;
		size += string_size(str);
		free(str);
	}
	ph->env.sibling_dies = strbuf_detach(&sb, NULL);

	for (i = 0; i < (u32)cpu_nr; i++) {
		if (do_read_u32(ff, &nr))
			goto free_cpu;

		ph->env.cpu[i].die_id = nr;
>>>>>>> upstream/android-13
	}

	return 0;

error:
	strbuf_release(&sb);
free_cpu:
	zfree(&ph->env.cpu);
	return -1;
}

static int process_numa_topology(struct feat_fd *ff, void *data __maybe_unused)
{
	struct numa_node *nodes, *n;
	u32 nr, i;
	char *str;

	/* nr nodes */
	if (do_read_u32(ff, &nr))
		return -1;

	nodes = zalloc(sizeof(*nodes) * nr);
	if (!nodes)
		return -ENOMEM;

	for (i = 0; i < nr; i++) {
		n = &nodes[i];

		/* node number */
		if (do_read_u32(ff, &n->node))
			goto error;

		if (do_read_u64(ff, &n->mem_total))
			goto error;

		if (do_read_u64(ff, &n->mem_free))
			goto error;

		str = do_read_string(ff);
		if (!str)
			goto error;

<<<<<<< HEAD
		n->map = cpu_map__new(str);
=======
		n->map = perf_cpu_map__new(str);
>>>>>>> upstream/android-13
		if (!n->map)
			goto error;

		free(str);
	}
	ff->ph->env.nr_numa_nodes = nr;
	ff->ph->env.numa_nodes = nodes;
	return 0;

error:
	free(nodes);
	return -1;
}

static int process_pmu_mappings(struct feat_fd *ff, void *data __maybe_unused)
{
	char *name;
	u32 pmu_num;
	u32 type;
	struct strbuf sb;

	if (do_read_u32(ff, &pmu_num))
		return -1;

	if (!pmu_num) {
		pr_debug("pmu mappings not available\n");
		return 0;
	}

	ff->ph->env.nr_pmu_mappings = pmu_num;
	if (strbuf_init(&sb, 128) < 0)
		return -1;

	while (pmu_num) {
		if (do_read_u32(ff, &type))
			goto error;

		name = do_read_string(ff);
		if (!name)
			goto error;

		if (strbuf_addf(&sb, "%u:%s", type, name) < 0)
			goto error;
		/* include a NULL character at the end */
		if (strbuf_add(&sb, "", 1) < 0)
			goto error;

		if (!strcmp(name, "msr"))
			ff->ph->env.msr_pmu_type = type;

		free(name);
		pmu_num--;
	}
	ff->ph->env.pmu_mappings = strbuf_detach(&sb, NULL);
	return 0;

error:
	strbuf_release(&sb);
	return -1;
}

static int process_group_desc(struct feat_fd *ff, void *data __maybe_unused)
{
	size_t ret = -1;
	u32 i, nr, nr_groups;
	struct perf_session *session;
<<<<<<< HEAD
	struct perf_evsel *evsel, *leader = NULL;
=======
	struct evsel *evsel, *leader = NULL;
>>>>>>> upstream/android-13
	struct group_desc {
		char *name;
		u32 leader_idx;
		u32 nr_members;
	} *desc;

	if (do_read_u32(ff, &nr_groups))
		return -1;

	ff->ph->env.nr_groups = nr_groups;
	if (!nr_groups) {
		pr_debug("group desc not available\n");
		return 0;
	}

	desc = calloc(nr_groups, sizeof(*desc));
	if (!desc)
		return -1;

	for (i = 0; i < nr_groups; i++) {
		desc[i].name = do_read_string(ff);
		if (!desc[i].name)
			goto out_free;

		if (do_read_u32(ff, &desc[i].leader_idx))
			goto out_free;

		if (do_read_u32(ff, &desc[i].nr_members))
			goto out_free;
	}

	/*
	 * Rebuild group relationship based on the group_desc
	 */
	session = container_of(ff->ph, struct perf_session, header);
<<<<<<< HEAD
	session->evlist->nr_groups = nr_groups;

	i = nr = 0;
	evlist__for_each_entry(session->evlist, evsel) {
		if (evsel->idx == (int) desc[i].leader_idx) {
			evsel->leader = evsel;
=======
	session->evlist->core.nr_groups = nr_groups;

	i = nr = 0;
	evlist__for_each_entry(session->evlist, evsel) {
		if (evsel->core.idx == (int) desc[i].leader_idx) {
			evsel__set_leader(evsel, evsel);
>>>>>>> upstream/android-13
			/* {anon_group} is a dummy name */
			if (strcmp(desc[i].name, "{anon_group}")) {
				evsel->group_name = desc[i].name;
				desc[i].name = NULL;
			}
<<<<<<< HEAD
			evsel->nr_members = desc[i].nr_members;
=======
			evsel->core.nr_members = desc[i].nr_members;
>>>>>>> upstream/android-13

			if (i >= nr_groups || nr > 0) {
				pr_debug("invalid group desc\n");
				goto out_free;
			}

			leader = evsel;
<<<<<<< HEAD
			nr = evsel->nr_members - 1;
			i++;
		} else if (nr) {
			/* This is a group member */
			evsel->leader = leader;
=======
			nr = evsel->core.nr_members - 1;
			i++;
		} else if (nr) {
			/* This is a group member */
			evsel__set_leader(evsel, leader);
>>>>>>> upstream/android-13

			nr--;
		}
	}

	if (i != nr_groups || nr != 0) {
		pr_debug("invalid group desc\n");
		goto out_free;
	}

	ret = 0;
out_free:
	for (i = 0; i < nr_groups; i++)
		zfree(&desc[i].name);
	free(desc);

	return ret;
}

static int process_auxtrace(struct feat_fd *ff, void *data __maybe_unused)
{
	struct perf_session *session;
	int err;

	session = container_of(ff->ph, struct perf_session, header);

	err = auxtrace_index__process(ff->fd, ff->size, session,
				      ff->ph->needs_swap);
	if (err < 0)
		pr_err("Failed to process auxtrace index\n");
	return err;
}

static int process_cache(struct feat_fd *ff, void *data __maybe_unused)
{
	struct cpu_cache_level *caches;
	u32 cnt, i, version;

	if (do_read_u32(ff, &version))
		return -1;

	if (version != 1)
		return -1;

	if (do_read_u32(ff, &cnt))
		return -1;

	caches = zalloc(sizeof(*caches) * cnt);
	if (!caches)
		return -1;

	for (i = 0; i < cnt; i++) {
		struct cpu_cache_level c;

		#define _R(v)						\
			if (do_read_u32(ff, &c.v))\
				goto out_free_caches;			\

		_R(level)
		_R(line_size)
		_R(sets)
		_R(ways)
		#undef _R

		#define _R(v)					\
			c.v = do_read_string(ff);		\
			if (!c.v)				\
				goto out_free_caches;

		_R(type)
		_R(size)
		_R(map)
		#undef _R

		caches[i] = c;
	}

	ff->ph->env.caches = caches;
	ff->ph->env.caches_cnt = cnt;
	return 0;
out_free_caches:
	free(caches);
	return -1;
}

static int process_sample_time(struct feat_fd *ff, void *data __maybe_unused)
{
	struct perf_session *session;
	u64 first_sample_time, last_sample_time;
	int ret;

	session = container_of(ff->ph, struct perf_session, header);

	ret = do_read_u64(ff, &first_sample_time);
	if (ret)
		return -1;

	ret = do_read_u64(ff, &last_sample_time);
	if (ret)
		return -1;

	session->evlist->first_sample_time = first_sample_time;
	session->evlist->last_sample_time = last_sample_time;
	return 0;
}

static int process_mem_topology(struct feat_fd *ff,
				void *data __maybe_unused)
{
	struct memory_node *nodes;
	u64 version, i, nr, bsize;
	int ret = -1;

	if (do_read_u64(ff, &version))
		return -1;

	if (version != 1)
		return -1;

	if (do_read_u64(ff, &bsize))
		return -1;

	if (do_read_u64(ff, &nr))
		return -1;

	nodes = zalloc(sizeof(*nodes) * nr);
	if (!nodes)
		return -1;

	for (i = 0; i < nr; i++) {
		struct memory_node n;

		#define _R(v)				\
			if (do_read_u64(ff, &n.v))	\
				goto out;		\

		_R(node)
		_R(size)

		#undef _R

		if (do_read_bitmap(ff, &n.set, &n.size))
			goto out;

		nodes[i] = n;
	}

	ff->ph->env.memory_bsize    = bsize;
	ff->ph->env.memory_nodes    = nodes;
	ff->ph->env.nr_memory_nodes = nr;
	ret = 0;

out:
	if (ret)
		free(nodes);
	return ret;
}

<<<<<<< HEAD
struct feature_ops {
	int (*write)(struct feat_fd *ff, struct perf_evlist *evlist);
	void (*print)(struct feat_fd *ff, FILE *fp);
	int (*process)(struct feat_fd *ff, void *data);
	const char *name;
	bool full_only;
	bool synthesize;
};
=======
static int process_clockid(struct feat_fd *ff,
			   void *data __maybe_unused)
{
	if (do_read_u64(ff, &ff->ph->env.clock.clockid_res_ns))
		return -1;

	return 0;
}

static int process_clock_data(struct feat_fd *ff,
			      void *_data __maybe_unused)
{
	u32 data32;
	u64 data64;

	/* version */
	if (do_read_u32(ff, &data32))
		return -1;

	if (data32 != 1)
		return -1;

	/* clockid */
	if (do_read_u32(ff, &data32))
		return -1;

	ff->ph->env.clock.clockid = data32;

	/* TOD ref time */
	if (do_read_u64(ff, &data64))
		return -1;

	ff->ph->env.clock.tod_ns = data64;

	/* clockid ref time */
	if (do_read_u64(ff, &data64))
		return -1;

	ff->ph->env.clock.clockid_ns = data64;
	ff->ph->env.clock.enabled = true;
	return 0;
}

static int process_hybrid_topology(struct feat_fd *ff,
				   void *data __maybe_unused)
{
	struct hybrid_node *nodes, *n;
	u32 nr, i;

	/* nr nodes */
	if (do_read_u32(ff, &nr))
		return -1;

	nodes = zalloc(sizeof(*nodes) * nr);
	if (!nodes)
		return -ENOMEM;

	for (i = 0; i < nr; i++) {
		n = &nodes[i];

		n->pmu_name = do_read_string(ff);
		if (!n->pmu_name)
			goto error;

		n->cpus = do_read_string(ff);
		if (!n->cpus)
			goto error;
	}

	ff->ph->env.nr_hybrid_nodes = nr;
	ff->ph->env.hybrid_nodes = nodes;
	return 0;

error:
	for (i = 0; i < nr; i++) {
		free(nodes[i].pmu_name);
		free(nodes[i].cpus);
	}

	free(nodes);
	return -1;
}

static int process_dir_format(struct feat_fd *ff,
			      void *_data __maybe_unused)
{
	struct perf_session *session;
	struct perf_data *data;

	session = container_of(ff->ph, struct perf_session, header);
	data = session->data;

	if (WARN_ON(!perf_data__is_dir(data)))
		return -1;

	return do_read_u64(ff, &data->dir.version);
}

#ifdef HAVE_LIBBPF_SUPPORT
static int process_bpf_prog_info(struct feat_fd *ff, void *data __maybe_unused)
{
	struct bpf_prog_info_linear *info_linear;
	struct bpf_prog_info_node *info_node;
	struct perf_env *env = &ff->ph->env;
	u32 count, i;
	int err = -1;

	if (ff->ph->needs_swap) {
		pr_warning("interpreting bpf_prog_info from systems with endianness is not yet supported\n");
		return 0;
	}

	if (do_read_u32(ff, &count))
		return -1;

	down_write(&env->bpf_progs.lock);

	for (i = 0; i < count; ++i) {
		u32 info_len, data_len;

		info_linear = NULL;
		info_node = NULL;
		if (do_read_u32(ff, &info_len))
			goto out;
		if (do_read_u32(ff, &data_len))
			goto out;

		if (info_len > sizeof(struct bpf_prog_info)) {
			pr_warning("detected invalid bpf_prog_info\n");
			goto out;
		}

		info_linear = malloc(sizeof(struct bpf_prog_info_linear) +
				     data_len);
		if (!info_linear)
			goto out;
		info_linear->info_len = sizeof(struct bpf_prog_info);
		info_linear->data_len = data_len;
		if (do_read_u64(ff, (u64 *)(&info_linear->arrays)))
			goto out;
		if (__do_read(ff, &info_linear->info, info_len))
			goto out;
		if (info_len < sizeof(struct bpf_prog_info))
			memset(((void *)(&info_linear->info)) + info_len, 0,
			       sizeof(struct bpf_prog_info) - info_len);

		if (__do_read(ff, info_linear->data, data_len))
			goto out;

		info_node = malloc(sizeof(struct bpf_prog_info_node));
		if (!info_node)
			goto out;

		/* after reading from file, translate offset to address */
		bpf_program__bpil_offs_to_addr(info_linear);
		info_node->info_linear = info_linear;
		perf_env__insert_bpf_prog_info(env, info_node);
	}

	up_write(&env->bpf_progs.lock);
	return 0;
out:
	free(info_linear);
	free(info_node);
	up_write(&env->bpf_progs.lock);
	return err;
}

static int process_bpf_btf(struct feat_fd *ff, void *data __maybe_unused)
{
	struct perf_env *env = &ff->ph->env;
	struct btf_node *node = NULL;
	u32 count, i;
	int err = -1;

	if (ff->ph->needs_swap) {
		pr_warning("interpreting btf from systems with endianness is not yet supported\n");
		return 0;
	}

	if (do_read_u32(ff, &count))
		return -1;

	down_write(&env->bpf_progs.lock);

	for (i = 0; i < count; ++i) {
		u32 id, data_size;

		if (do_read_u32(ff, &id))
			goto out;
		if (do_read_u32(ff, &data_size))
			goto out;

		node = malloc(sizeof(struct btf_node) + data_size);
		if (!node)
			goto out;

		node->id = id;
		node->data_size = data_size;

		if (__do_read(ff, node->data, data_size))
			goto out;

		perf_env__insert_btf(env, node);
		node = NULL;
	}

	err = 0;
out:
	up_write(&env->bpf_progs.lock);
	free(node);
	return err;
}
#endif // HAVE_LIBBPF_SUPPORT

static int process_compressed(struct feat_fd *ff,
			      void *data __maybe_unused)
{
	if (do_read_u32(ff, &(ff->ph->env.comp_ver)))
		return -1;

	if (do_read_u32(ff, &(ff->ph->env.comp_type)))
		return -1;

	if (do_read_u32(ff, &(ff->ph->env.comp_level)))
		return -1;

	if (do_read_u32(ff, &(ff->ph->env.comp_ratio)))
		return -1;

	if (do_read_u32(ff, &(ff->ph->env.comp_mmap_len)))
		return -1;

	return 0;
}

static int process_per_cpu_pmu_caps(struct feat_fd *ff, int *nr_cpu_pmu_caps,
				    char **cpu_pmu_caps,
				    unsigned int *max_branches)
{
	char *name, *value;
	struct strbuf sb;
	u32 nr_caps;

	if (do_read_u32(ff, &nr_caps))
		return -1;

	if (!nr_caps) {
		pr_debug("cpu pmu capabilities not available\n");
		return 0;
	}

	*nr_cpu_pmu_caps = nr_caps;

	if (strbuf_init(&sb, 128) < 0)
		return -1;

	while (nr_caps--) {
		name = do_read_string(ff);
		if (!name)
			goto error;

		value = do_read_string(ff);
		if (!value)
			goto free_name;

		if (strbuf_addf(&sb, "%s=%s", name, value) < 0)
			goto free_value;

		/* include a NULL character at the end */
		if (strbuf_add(&sb, "", 1) < 0)
			goto free_value;

		if (!strcmp(name, "branches"))
			*max_branches = atoi(value);

		free(value);
		free(name);
	}
	*cpu_pmu_caps = strbuf_detach(&sb, NULL);
	return 0;

free_value:
	free(value);
free_name:
	free(name);
error:
	strbuf_release(&sb);
	return -1;
}

static int process_cpu_pmu_caps(struct feat_fd *ff,
				void *data __maybe_unused)
{
	return process_per_cpu_pmu_caps(ff, &ff->ph->env.nr_cpu_pmu_caps,
					&ff->ph->env.cpu_pmu_caps,
					&ff->ph->env.max_branches);
}

static int process_hybrid_cpu_pmu_caps(struct feat_fd *ff,
				       void *data __maybe_unused)
{
	struct hybrid_cpc_node *nodes;
	u32 nr_pmu, i;
	int ret;

	if (do_read_u32(ff, &nr_pmu))
		return -1;

	if (!nr_pmu) {
		pr_debug("hybrid cpu pmu capabilities not available\n");
		return 0;
	}

	nodes = zalloc(sizeof(*nodes) * nr_pmu);
	if (!nodes)
		return -ENOMEM;

	for (i = 0; i < nr_pmu; i++) {
		struct hybrid_cpc_node *n = &nodes[i];

		ret = process_per_cpu_pmu_caps(ff, &n->nr_cpu_pmu_caps,
					       &n->cpu_pmu_caps,
					       &n->max_branches);
		if (ret)
			goto err;

		n->pmu_name = do_read_string(ff);
		if (!n->pmu_name) {
			ret = -1;
			goto err;
		}
	}

	ff->ph->env.nr_hybrid_cpc_nodes = nr_pmu;
	ff->ph->env.hybrid_cpc_nodes = nodes;
	return 0;

err:
	for (i = 0; i < nr_pmu; i++) {
		free(nodes[i].cpu_pmu_caps);
		free(nodes[i].pmu_name);
	}

	free(nodes);
	return ret;
}
>>>>>>> upstream/android-13

#define FEAT_OPR(n, func, __full_only) \
	[HEADER_##n] = {					\
		.name	    = __stringify(n),			\
		.write	    = write_##func,			\
		.print	    = print_##func,			\
		.full_only  = __full_only,			\
		.process    = process_##func,			\
		.synthesize = true				\
	}

#define FEAT_OPN(n, func, __full_only) \
	[HEADER_##n] = {					\
		.name	    = __stringify(n),			\
		.write	    = write_##func,			\
		.print	    = print_##func,			\
		.full_only  = __full_only,			\
		.process    = process_##func			\
	}

/* feature_ops not implemented: */
#define print_tracing_data	NULL
#define print_build_id		NULL

#define process_branch_stack	NULL
#define process_stat		NULL

<<<<<<< HEAD

static const struct feature_ops feat_ops[HEADER_LAST_FEATURE] = {
=======
// Only used in util/synthetic-events.c
const struct perf_header_feature_ops feat_ops[HEADER_LAST_FEATURE];

const struct perf_header_feature_ops feat_ops[HEADER_LAST_FEATURE] = {
>>>>>>> upstream/android-13
	FEAT_OPN(TRACING_DATA,	tracing_data,	false),
	FEAT_OPN(BUILD_ID,	build_id,	false),
	FEAT_OPR(HOSTNAME,	hostname,	false),
	FEAT_OPR(OSRELEASE,	osrelease,	false),
	FEAT_OPR(VERSION,	version,	false),
	FEAT_OPR(ARCH,		arch,		false),
	FEAT_OPR(NRCPUS,	nrcpus,		false),
	FEAT_OPR(CPUDESC,	cpudesc,	false),
	FEAT_OPR(CPUID,		cpuid,		false),
	FEAT_OPR(TOTAL_MEM,	total_mem,	false),
	FEAT_OPR(EVENT_DESC,	event_desc,	false),
	FEAT_OPR(CMDLINE,	cmdline,	false),
	FEAT_OPR(CPU_TOPOLOGY,	cpu_topology,	true),
	FEAT_OPR(NUMA_TOPOLOGY,	numa_topology,	true),
	FEAT_OPN(BRANCH_STACK,	branch_stack,	false),
	FEAT_OPR(PMU_MAPPINGS,	pmu_mappings,	false),
	FEAT_OPR(GROUP_DESC,	group_desc,	false),
	FEAT_OPN(AUXTRACE,	auxtrace,	false),
	FEAT_OPN(STAT,		stat,		false),
	FEAT_OPN(CACHE,		cache,		true),
	FEAT_OPR(SAMPLE_TIME,	sample_time,	false),
	FEAT_OPR(MEM_TOPOLOGY,	mem_topology,	true),
<<<<<<< HEAD
=======
	FEAT_OPR(CLOCKID,	clockid,	false),
	FEAT_OPN(DIR_FORMAT,	dir_format,	false),
#ifdef HAVE_LIBBPF_SUPPORT
	FEAT_OPR(BPF_PROG_INFO, bpf_prog_info,  false),
	FEAT_OPR(BPF_BTF,       bpf_btf,        false),
#endif
	FEAT_OPR(COMPRESSED,	compressed,	false),
	FEAT_OPR(CPU_PMU_CAPS,	cpu_pmu_caps,	false),
	FEAT_OPR(CLOCK_DATA,	clock_data,	false),
	FEAT_OPN(HYBRID_TOPOLOGY,	hybrid_topology,	true),
	FEAT_OPR(HYBRID_CPU_PMU_CAPS,	hybrid_cpu_pmu_caps,	false),
>>>>>>> upstream/android-13
};

struct header_print_data {
	FILE *fp;
	bool full; /* extended list of headers */
};

static int perf_file_section__fprintf_info(struct perf_file_section *section,
					   struct perf_header *ph,
					   int feat, int fd, void *data)
{
	struct header_print_data *hd = data;
	struct feat_fd ff;

	if (lseek(fd, section->offset, SEEK_SET) == (off_t)-1) {
		pr_debug("Failed to lseek to %" PRIu64 " offset for feature "
				"%d, continuing...\n", section->offset, feat);
		return 0;
	}
	if (feat >= HEADER_LAST_FEATURE) {
		pr_warning("unknown feature %d\n", feat);
		return 0;
	}
	if (!feat_ops[feat].print)
		return 0;

	ff = (struct  feat_fd) {
		.fd = fd,
		.ph = ph,
	};

	if (!feat_ops[feat].full_only || hd->full)
		feat_ops[feat].print(&ff, hd->fp);
	else
		fprintf(hd->fp, "# %s info available, use -I to display\n",
			feat_ops[feat].name);

	return 0;
}

int perf_header__fprintf_info(struct perf_session *session, FILE *fp, bool full)
{
	struct header_print_data hd;
	struct perf_header *header = &session->header;
	int fd = perf_data__fd(session->data);
	struct stat st;
	time_t stctime;
	int ret, bit;

	hd.fp = fp;
	hd.full = full;

	ret = fstat(fd, &st);
	if (ret == -1)
		return -1;

<<<<<<< HEAD
	stctime = st.st_ctime;
=======
	stctime = st.st_mtime;
>>>>>>> upstream/android-13
	fprintf(fp, "# captured on    : %s", ctime(&stctime));

	fprintf(fp, "# header version : %u\n", header->version);
	fprintf(fp, "# data offset    : %" PRIu64 "\n", header->data_offset);
	fprintf(fp, "# data size      : %" PRIu64 "\n", header->data_size);
	fprintf(fp, "# feat offset    : %" PRIu64 "\n", header->feat_offset);

	perf_header__process_sections(header, fd, &hd,
				      perf_file_section__fprintf_info);

	if (session->data->is_pipe)
		return 0;

	fprintf(fp, "# missing features: ");
	for_each_clear_bit(bit, header->adds_features, HEADER_LAST_FEATURE) {
		if (bit)
			fprintf(fp, "%s ", feat_ops[bit].name);
	}

	fprintf(fp, "\n");
	return 0;
}

static int do_write_feat(struct feat_fd *ff, int type,
			 struct perf_file_section **p,
<<<<<<< HEAD
			 struct perf_evlist *evlist)
=======
			 struct evlist *evlist)
>>>>>>> upstream/android-13
{
	int err;
	int ret = 0;

	if (perf_header__has_feat(ff->ph, type)) {
		if (!feat_ops[type].write)
			return -1;

		if (WARN(ff->buf, "Error: calling %s in pipe-mode.\n", __func__))
			return -1;

		(*p)->offset = lseek(ff->fd, 0, SEEK_CUR);

		err = feat_ops[type].write(ff, evlist);
		if (err < 0) {
			pr_debug("failed to write feature %s\n", feat_ops[type].name);

			/* undo anything written */
			lseek(ff->fd, (*p)->offset, SEEK_SET);

			return -1;
		}
		(*p)->size = lseek(ff->fd, 0, SEEK_CUR) - (*p)->offset;
		(*p)++;
	}
	return ret;
}

static int perf_header__adds_write(struct perf_header *header,
<<<<<<< HEAD
				   struct perf_evlist *evlist, int fd)
=======
				   struct evlist *evlist, int fd)
>>>>>>> upstream/android-13
{
	int nr_sections;
	struct feat_fd ff;
	struct perf_file_section *feat_sec, *p;
	int sec_size;
	u64 sec_start;
	int feat;
	int err;

	ff = (struct feat_fd){
		.fd  = fd,
		.ph = header,
	};

	nr_sections = bitmap_weight(header->adds_features, HEADER_FEAT_BITS);
	if (!nr_sections)
		return 0;

	feat_sec = p = calloc(nr_sections, sizeof(*feat_sec));
	if (feat_sec == NULL)
		return -ENOMEM;

	sec_size = sizeof(*feat_sec) * nr_sections;

	sec_start = header->feat_offset;
	lseek(fd, sec_start + sec_size, SEEK_SET);

	for_each_set_bit(feat, header->adds_features, HEADER_FEAT_BITS) {
		if (do_write_feat(&ff, feat, &p, evlist))
			perf_header__clear_feat(header, feat);
	}

	lseek(fd, sec_start, SEEK_SET);
	/*
	 * may write more than needed due to dropped feature, but
<<<<<<< HEAD
	 * this is okay, reader will skip the mising entries
=======
	 * this is okay, reader will skip the missing entries
>>>>>>> upstream/android-13
	 */
	err = do_write(&ff, feat_sec, sec_size);
	if (err < 0)
		pr_debug("failed to write feature section\n");
	free(feat_sec);
	return err;
}

int perf_header__write_pipe(int fd)
{
	struct perf_pipe_file_header f_header;
	struct feat_fd ff;
	int err;

	ff = (struct feat_fd){ .fd = fd };

	f_header = (struct perf_pipe_file_header){
		.magic	   = PERF_MAGIC,
		.size	   = sizeof(f_header),
	};

	err = do_write(&ff, &f_header, sizeof(f_header));
	if (err < 0) {
		pr_debug("failed to write perf pipe header\n");
		return err;
	}

	return 0;
}

int perf_session__write_header(struct perf_session *session,
<<<<<<< HEAD
			       struct perf_evlist *evlist,
=======
			       struct evlist *evlist,
>>>>>>> upstream/android-13
			       int fd, bool at_exit)
{
	struct perf_file_header f_header;
	struct perf_file_attr   f_attr;
	struct perf_header *header = &session->header;
<<<<<<< HEAD
	struct perf_evsel *evsel;
=======
	struct evsel *evsel;
>>>>>>> upstream/android-13
	struct feat_fd ff;
	u64 attr_offset;
	int err;

	ff = (struct feat_fd){ .fd = fd};
	lseek(fd, sizeof(f_header), SEEK_SET);

	evlist__for_each_entry(session->evlist, evsel) {
		evsel->id_offset = lseek(fd, 0, SEEK_CUR);
<<<<<<< HEAD
		err = do_write(&ff, evsel->id, evsel->ids * sizeof(u64));
=======
		err = do_write(&ff, evsel->core.id, evsel->core.ids * sizeof(u64));
>>>>>>> upstream/android-13
		if (err < 0) {
			pr_debug("failed to write perf header\n");
			return err;
		}
	}

	attr_offset = lseek(ff.fd, 0, SEEK_CUR);

	evlist__for_each_entry(evlist, evsel) {
<<<<<<< HEAD
		f_attr = (struct perf_file_attr){
			.attr = evsel->attr,
			.ids  = {
				.offset = evsel->id_offset,
				.size   = evsel->ids * sizeof(u64),
=======
		if (evsel->core.attr.size < sizeof(evsel->core.attr)) {
			/*
			 * We are likely in "perf inject" and have read
			 * from an older file. Update attr size so that
			 * reader gets the right offset to the ids.
			 */
			evsel->core.attr.size = sizeof(evsel->core.attr);
		}
		f_attr = (struct perf_file_attr){
			.attr = evsel->core.attr,
			.ids  = {
				.offset = evsel->id_offset,
				.size   = evsel->core.ids * sizeof(u64),
>>>>>>> upstream/android-13
			}
		};
		err = do_write(&ff, &f_attr, sizeof(f_attr));
		if (err < 0) {
			pr_debug("failed to write perf header attribute\n");
			return err;
		}
	}

	if (!header->data_offset)
		header->data_offset = lseek(fd, 0, SEEK_CUR);
	header->feat_offset = header->data_offset + header->data_size;

	if (at_exit) {
		err = perf_header__adds_write(header, evlist, fd);
		if (err < 0)
			return err;
	}

	f_header = (struct perf_file_header){
		.magic	   = PERF_MAGIC,
		.size	   = sizeof(f_header),
		.attr_size = sizeof(f_attr),
		.attrs = {
			.offset = attr_offset,
<<<<<<< HEAD
			.size   = evlist->nr_entries * sizeof(f_attr),
=======
			.size   = evlist->core.nr_entries * sizeof(f_attr),
>>>>>>> upstream/android-13
		},
		.data = {
			.offset = header->data_offset,
			.size	= header->data_size,
		},
		/* event_types is ignored, store zeros */
	};

	memcpy(&f_header.adds_features, &header->adds_features, sizeof(header->adds_features));

	lseek(fd, 0, SEEK_SET);
	err = do_write(&ff, &f_header, sizeof(f_header));
	if (err < 0) {
		pr_debug("failed to write perf header\n");
		return err;
	}
	lseek(fd, header->data_offset + header->data_size, SEEK_SET);

	return 0;
}

static int perf_header__getbuffer64(struct perf_header *header,
				    int fd, void *buf, size_t size)
{
	if (readn(fd, buf, size) <= 0)
		return -1;

	if (header->needs_swap)
		mem_bswap_64(buf, size);

	return 0;
}

int perf_header__process_sections(struct perf_header *header, int fd,
				  void *data,
				  int (*process)(struct perf_file_section *section,
						 struct perf_header *ph,
						 int feat, int fd, void *data))
{
	struct perf_file_section *feat_sec, *sec;
	int nr_sections;
	int sec_size;
	int feat;
	int err;

	nr_sections = bitmap_weight(header->adds_features, HEADER_FEAT_BITS);
	if (!nr_sections)
		return 0;

	feat_sec = sec = calloc(nr_sections, sizeof(*feat_sec));
	if (!feat_sec)
		return -1;

	sec_size = sizeof(*feat_sec) * nr_sections;

	lseek(fd, header->feat_offset, SEEK_SET);

	err = perf_header__getbuffer64(header, fd, feat_sec, sec_size);
	if (err < 0)
		goto out_free;

	for_each_set_bit(feat, header->adds_features, HEADER_LAST_FEATURE) {
		err = process(sec++, header, feat, fd, data);
		if (err < 0)
			goto out_free;
	}
	err = 0;
out_free:
	free(feat_sec);
	return err;
}

static const int attr_file_abi_sizes[] = {
	[0] = PERF_ATTR_SIZE_VER0,
	[1] = PERF_ATTR_SIZE_VER1,
	[2] = PERF_ATTR_SIZE_VER2,
	[3] = PERF_ATTR_SIZE_VER3,
	[4] = PERF_ATTR_SIZE_VER4,
	0,
};

/*
 * In the legacy file format, the magic number is not used to encode endianness.
 * hdr_sz was used to encode endianness. But given that hdr_sz can vary based
 * on ABI revisions, we need to try all combinations for all endianness to
 * detect the endianness.
 */
static int try_all_file_abis(uint64_t hdr_sz, struct perf_header *ph)
{
	uint64_t ref_size, attr_size;
	int i;

	for (i = 0 ; attr_file_abi_sizes[i]; i++) {
		ref_size = attr_file_abi_sizes[i]
			 + sizeof(struct perf_file_section);
		if (hdr_sz != ref_size) {
			attr_size = bswap_64(hdr_sz);
			if (attr_size != ref_size)
				continue;

			ph->needs_swap = true;
		}
		pr_debug("ABI%d perf.data file detected, need_swap=%d\n",
			 i,
			 ph->needs_swap);
		return 0;
	}
	/* could not determine endianness */
	return -1;
}

#define PERF_PIPE_HDR_VER0	16

static const size_t attr_pipe_abi_sizes[] = {
	[0] = PERF_PIPE_HDR_VER0,
	0,
};

/*
<<<<<<< HEAD
 * In the legacy pipe format, there is an implicit assumption that endiannesss
 * between host recording the samples, and host parsing the samples is the
 * same. This is not always the case given that the pipe output may always be
 * redirected into a file and analyzed on a different machine with possibly a
 * different endianness and perf_event ABI revsions in the perf tool itself.
=======
 * In the legacy pipe format, there is an implicit assumption that endianness
 * between host recording the samples, and host parsing the samples is the
 * same. This is not always the case given that the pipe output may always be
 * redirected into a file and analyzed on a different machine with possibly a
 * different endianness and perf_event ABI revisions in the perf tool itself.
>>>>>>> upstream/android-13
 */
static int try_all_pipe_abis(uint64_t hdr_sz, struct perf_header *ph)
{
	u64 attr_size;
	int i;

	for (i = 0 ; attr_pipe_abi_sizes[i]; i++) {
		if (hdr_sz != attr_pipe_abi_sizes[i]) {
			attr_size = bswap_64(hdr_sz);
			if (attr_size != hdr_sz)
				continue;

			ph->needs_swap = true;
		}
		pr_debug("Pipe ABI%d perf.data file detected\n", i);
		return 0;
	}
	return -1;
}

bool is_perf_magic(u64 magic)
{
	if (!memcmp(&magic, __perf_magic1, sizeof(magic))
		|| magic == __perf_magic2
		|| magic == __perf_magic2_sw)
		return true;

	return false;
}

static int check_magic_endian(u64 magic, uint64_t hdr_sz,
			      bool is_pipe, struct perf_header *ph)
{
	int ret;

	/* check for legacy format */
	ret = memcmp(&magic, __perf_magic1, sizeof(magic));
	if (ret == 0) {
		ph->version = PERF_HEADER_VERSION_1;
		pr_debug("legacy perf.data format\n");
		if (is_pipe)
			return try_all_pipe_abis(hdr_sz, ph);

		return try_all_file_abis(hdr_sz, ph);
	}
	/*
	 * the new magic number serves two purposes:
	 * - unique number to identify actual perf.data files
	 * - encode endianness of file
	 */
	ph->version = PERF_HEADER_VERSION_2;

	/* check magic number with one endianness */
	if (magic == __perf_magic2)
		return 0;

	/* check magic number with opposite endianness */
	if (magic != __perf_magic2_sw)
		return -1;

	ph->needs_swap = true;

	return 0;
}

int perf_file_header__read(struct perf_file_header *header,
			   struct perf_header *ph, int fd)
{
	ssize_t ret;

	lseek(fd, 0, SEEK_SET);

	ret = readn(fd, header, sizeof(*header));
	if (ret <= 0)
		return -1;

	if (check_magic_endian(header->magic,
			       header->attr_size, false, ph) < 0) {
		pr_debug("magic/endian check failed\n");
		return -1;
	}

	if (ph->needs_swap) {
		mem_bswap_64(header, offsetof(struct perf_file_header,
			     adds_features));
	}

	if (header->size != sizeof(*header)) {
		/* Support the previous format */
		if (header->size == offsetof(typeof(*header), adds_features))
			bitmap_zero(header->adds_features, HEADER_FEAT_BITS);
		else
			return -1;
	} else if (ph->needs_swap) {
		/*
		 * feature bitmap is declared as an array of unsigned longs --
		 * not good since its size can differ between the host that
		 * generated the data file and the host analyzing the file.
		 *
		 * We need to handle endianness, but we don't know the size of
		 * the unsigned long where the file was generated. Take a best
		 * guess at determining it: try 64-bit swap first (ie., file
		 * created on a 64-bit host), and check if the hostname feature
		 * bit is set (this feature bit is forced on as of fbe96f2).
		 * If the bit is not, undo the 64-bit swap and try a 32-bit
		 * swap. If the hostname bit is still not set (e.g., older data
		 * file), punt and fallback to the original behavior --
		 * clearing all feature bits and setting buildid.
		 */
		mem_bswap_64(&header->adds_features,
			    BITS_TO_U64(HEADER_FEAT_BITS));

		if (!test_bit(HEADER_HOSTNAME, header->adds_features)) {
			/* unswap as u64 */
			mem_bswap_64(&header->adds_features,
				    BITS_TO_U64(HEADER_FEAT_BITS));

			/* unswap as u32 */
			mem_bswap_32(&header->adds_features,
				    BITS_TO_U32(HEADER_FEAT_BITS));
		}

		if (!test_bit(HEADER_HOSTNAME, header->adds_features)) {
			bitmap_zero(header->adds_features, HEADER_FEAT_BITS);
			set_bit(HEADER_BUILD_ID, header->adds_features);
		}
	}

	memcpy(&ph->adds_features, &header->adds_features,
	       sizeof(ph->adds_features));

	ph->data_offset  = header->data.offset;
	ph->data_size	 = header->data.size;
	ph->feat_offset  = header->data.offset + header->data.size;
	return 0;
}

static int perf_file_section__process(struct perf_file_section *section,
				      struct perf_header *ph,
				      int feat, int fd, void *data)
{
	struct feat_fd fdd = {
		.fd	= fd,
		.ph	= ph,
		.size	= section->size,
		.offset	= section->offset,
	};

	if (lseek(fd, section->offset, SEEK_SET) == (off_t)-1) {
		pr_debug("Failed to lseek to %" PRIu64 " offset for feature "
			  "%d, continuing...\n", section->offset, feat);
		return 0;
	}

	if (feat >= HEADER_LAST_FEATURE) {
		pr_debug("unknown feature %d, continuing...\n", feat);
		return 0;
	}

	if (!feat_ops[feat].process)
		return 0;

	return feat_ops[feat].process(&fdd, data);
}

static int perf_file_header__read_pipe(struct perf_pipe_file_header *header,
<<<<<<< HEAD
				       struct perf_header *ph, int fd,
				       bool repipe)
{
	struct feat_fd ff = {
		.fd = STDOUT_FILENO,
=======
				       struct perf_header *ph,
				       struct perf_data* data,
				       bool repipe, int repipe_fd)
{
	struct feat_fd ff = {
		.fd = repipe_fd,
>>>>>>> upstream/android-13
		.ph = ph,
	};
	ssize_t ret;

<<<<<<< HEAD
	ret = readn(fd, header, sizeof(*header));
=======
	ret = perf_data__read(data, header, sizeof(*header));
>>>>>>> upstream/android-13
	if (ret <= 0)
		return -1;

	if (check_magic_endian(header->magic, header->size, true, ph) < 0) {
		pr_debug("endian/magic failed\n");
		return -1;
	}

	if (ph->needs_swap)
		header->size = bswap_64(header->size);

	if (repipe && do_write(&ff, header, sizeof(*header)) < 0)
		return -1;

	return 0;
}

<<<<<<< HEAD
static int perf_header__read_pipe(struct perf_session *session)
=======
static int perf_header__read_pipe(struct perf_session *session, int repipe_fd)
>>>>>>> upstream/android-13
{
	struct perf_header *header = &session->header;
	struct perf_pipe_file_header f_header;

<<<<<<< HEAD
	if (perf_file_header__read_pipe(&f_header, header,
					perf_data__fd(session->data),
					session->repipe) < 0) {
=======
	if (perf_file_header__read_pipe(&f_header, header, session->data,
					session->repipe, repipe_fd) < 0) {
>>>>>>> upstream/android-13
		pr_debug("incompatible file format\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	return 0;
=======
	return f_header.size == sizeof(f_header) ? 0 : -1;
>>>>>>> upstream/android-13
}

static int read_attr(int fd, struct perf_header *ph,
		     struct perf_file_attr *f_attr)
{
	struct perf_event_attr *attr = &f_attr->attr;
	size_t sz, left;
	size_t our_sz = sizeof(f_attr->attr);
	ssize_t ret;

	memset(f_attr, 0, sizeof(*f_attr));

	/* read minimal guaranteed structure */
	ret = readn(fd, attr, PERF_ATTR_SIZE_VER0);
	if (ret <= 0) {
		pr_debug("cannot read %d bytes of header attr\n",
			 PERF_ATTR_SIZE_VER0);
		return -1;
	}

	/* on file perf_event_attr size */
	sz = attr->size;

	if (ph->needs_swap)
		sz = bswap_32(sz);

	if (sz == 0) {
		/* assume ABI0 */
		sz =  PERF_ATTR_SIZE_VER0;
	} else if (sz > our_sz) {
		pr_debug("file uses a more recent and unsupported ABI"
			 " (%zu bytes extra)\n", sz - our_sz);
		return -1;
	}
	/* what we have not yet read and that we know about */
	left = sz - PERF_ATTR_SIZE_VER0;
	if (left) {
		void *ptr = attr;
		ptr += PERF_ATTR_SIZE_VER0;

		ret = readn(fd, ptr, left);
	}
	/* read perf_file_section, ids are read in caller */
	ret = readn(fd, &f_attr->ids, sizeof(f_attr->ids));

	return ret <= 0 ? -1 : 0;
}

<<<<<<< HEAD
static int perf_evsel__prepare_tracepoint_event(struct perf_evsel *evsel,
						struct tep_handle *pevent)
{
	struct event_format *event;
=======
static int evsel__prepare_tracepoint_event(struct evsel *evsel, struct tep_handle *pevent)
{
	struct tep_event *event;
>>>>>>> upstream/android-13
	char bf[128];

	/* already prepared */
	if (evsel->tp_format)
		return 0;

	if (pevent == NULL) {
		pr_debug("broken or missing trace data\n");
		return -1;
	}

<<<<<<< HEAD
	event = tep_find_event(pevent, evsel->attr.config);
	if (event == NULL) {
		pr_debug("cannot find event format for %d\n", (int)evsel->attr.config);
=======
	event = tep_find_event(pevent, evsel->core.attr.config);
	if (event == NULL) {
		pr_debug("cannot find event format for %d\n", (int)evsel->core.attr.config);
>>>>>>> upstream/android-13
		return -1;
	}

	if (!evsel->name) {
		snprintf(bf, sizeof(bf), "%s:%s", event->system, event->name);
		evsel->name = strdup(bf);
		if (evsel->name == NULL)
			return -1;
	}

	evsel->tp_format = event;
	return 0;
}

<<<<<<< HEAD
static int perf_evlist__prepare_tracepoint_events(struct perf_evlist *evlist,
						  struct tep_handle *pevent)
{
	struct perf_evsel *pos;

	evlist__for_each_entry(evlist, pos) {
		if (pos->attr.type == PERF_TYPE_TRACEPOINT &&
		    perf_evsel__prepare_tracepoint_event(pos, pevent))
=======
static int evlist__prepare_tracepoint_events(struct evlist *evlist, struct tep_handle *pevent)
{
	struct evsel *pos;

	evlist__for_each_entry(evlist, pos) {
		if (pos->core.attr.type == PERF_TYPE_TRACEPOINT &&
		    evsel__prepare_tracepoint_event(pos, pevent))
>>>>>>> upstream/android-13
			return -1;
	}

	return 0;
}

<<<<<<< HEAD
int perf_session__read_header(struct perf_session *session)
=======
int perf_session__read_header(struct perf_session *session, int repipe_fd)
>>>>>>> upstream/android-13
{
	struct perf_data *data = session->data;
	struct perf_header *header = &session->header;
	struct perf_file_header	f_header;
	struct perf_file_attr	f_attr;
	u64			f_id;
<<<<<<< HEAD
	int nr_attrs, nr_ids, i, j;
	int fd = perf_data__fd(data);

	session->evlist = perf_evlist__new();
=======
	int nr_attrs, nr_ids, i, j, err;
	int fd = perf_data__fd(data);

	session->evlist = evlist__new();
>>>>>>> upstream/android-13
	if (session->evlist == NULL)
		return -ENOMEM;

	session->evlist->env = &header->env;
	session->machines.host.env = &header->env;
<<<<<<< HEAD
	if (perf_data__is_pipe(data))
		return perf_header__read_pipe(session);
=======

	/*
	 * We can read 'pipe' data event from regular file,
	 * check for the pipe header regardless of source.
	 */
	err = perf_header__read_pipe(session, repipe_fd);
	if (!err || perf_data__is_pipe(data)) {
		data->is_pipe = true;
		return err;
	}
>>>>>>> upstream/android-13

	if (perf_file_header__read(&f_header, header, fd) < 0)
		return -EINVAL;

<<<<<<< HEAD
=======
	if (header->needs_swap && data->in_place_update) {
		pr_err("In-place update not supported when byte-swapping is required\n");
		return -EINVAL;
	}

>>>>>>> upstream/android-13
	/*
	 * Sanity check that perf.data was written cleanly; data size is
	 * initialized to 0 and updated only if the on_exit function is run.
	 * If data size is still 0 then the file contains only partial
	 * information.  Just warn user and process it as much as it can.
	 */
	if (f_header.data.size == 0) {
		pr_warning("WARNING: The %s file's data size field is 0 which is unexpected.\n"
			   "Was the 'perf record' command properly terminated?\n",
			   data->file.path);
	}

	if (f_header.attr_size == 0) {
		pr_err("ERROR: The %s file's attr size field is 0 which is unexpected.\n"
		       "Was the 'perf record' command properly terminated?\n",
		       data->file.path);
		return -EINVAL;
	}

	nr_attrs = f_header.attrs.size / f_header.attr_size;
	lseek(fd, f_header.attrs.offset, SEEK_SET);

	for (i = 0; i < nr_attrs; i++) {
<<<<<<< HEAD
		struct perf_evsel *evsel;
=======
		struct evsel *evsel;
>>>>>>> upstream/android-13
		off_t tmp;

		if (read_attr(fd, header, &f_attr) < 0)
			goto out_errno;

		if (header->needs_swap) {
			f_attr.ids.size   = bswap_64(f_attr.ids.size);
			f_attr.ids.offset = bswap_64(f_attr.ids.offset);
			perf_event__attr_swap(&f_attr.attr);
		}

		tmp = lseek(fd, 0, SEEK_CUR);
<<<<<<< HEAD
		evsel = perf_evsel__new(&f_attr.attr);
=======
		evsel = evsel__new(&f_attr.attr);
>>>>>>> upstream/android-13

		if (evsel == NULL)
			goto out_delete_evlist;

		evsel->needs_swap = header->needs_swap;
		/*
		 * Do it before so that if perf_evsel__alloc_id fails, this
<<<<<<< HEAD
		 * entry gets purged too at perf_evlist__delete().
		 */
		perf_evlist__add(session->evlist, evsel);
=======
		 * entry gets purged too at evlist__delete().
		 */
		evlist__add(session->evlist, evsel);
>>>>>>> upstream/android-13

		nr_ids = f_attr.ids.size / sizeof(u64);
		/*
		 * We don't have the cpu and thread maps on the header, so
		 * for allocating the perf_sample_id table we fake 1 cpu and
		 * hattr->ids threads.
		 */
<<<<<<< HEAD
		if (perf_evsel__alloc_id(evsel, 1, nr_ids))
=======
		if (perf_evsel__alloc_id(&evsel->core, 1, nr_ids))
>>>>>>> upstream/android-13
			goto out_delete_evlist;

		lseek(fd, f_attr.ids.offset, SEEK_SET);

		for (j = 0; j < nr_ids; j++) {
			if (perf_header__getbuffer64(header, fd, &f_id, sizeof(f_id)))
				goto out_errno;

<<<<<<< HEAD
			perf_evlist__id_add(session->evlist, evsel, 0, j, f_id);
=======
			perf_evlist__id_add(&session->evlist->core, &evsel->core, 0, j, f_id);
>>>>>>> upstream/android-13
		}

		lseek(fd, tmp, SEEK_SET);
	}

	perf_header__process_sections(header, fd, &session->tevent,
				      perf_file_section__process);

<<<<<<< HEAD
	if (perf_evlist__prepare_tracepoint_events(session->evlist,
						   session->tevent.pevent))
=======
	if (evlist__prepare_tracepoint_events(session->evlist, session->tevent.pevent))
>>>>>>> upstream/android-13
		goto out_delete_evlist;

	return 0;
out_errno:
	return -errno;

out_delete_evlist:
<<<<<<< HEAD
	perf_evlist__delete(session->evlist);
=======
	evlist__delete(session->evlist);
>>>>>>> upstream/android-13
	session->evlist = NULL;
	return -ENOMEM;
}

<<<<<<< HEAD
int perf_event__synthesize_attr(struct perf_tool *tool,
				struct perf_event_attr *attr, u32 ids, u64 *id,
				perf_event__handler_t process)
{
	union perf_event *ev;
	size_t size;
	int err;

	size = sizeof(struct perf_event_attr);
	size = PERF_ALIGN(size, sizeof(u64));
	size += sizeof(struct perf_event_header);
	size += ids * sizeof(u64);

	ev = zalloc(size);

	if (ev == NULL)
		return -ENOMEM;

	ev->attr.attr = *attr;
	memcpy(ev->attr.id, id, ids * sizeof(u64));

	ev->attr.header.type = PERF_RECORD_HEADER_ATTR;
	ev->attr.header.size = (u16)size;

	if (ev->attr.header.size == size)
		err = process(tool, ev, NULL, NULL);
	else
		err = -E2BIG;

	free(ev);

	return err;
}

int perf_event__synthesize_features(struct perf_tool *tool,
				    struct perf_session *session,
				    struct perf_evlist *evlist,
				    perf_event__handler_t process)
{
	struct perf_header *header = &session->header;
	struct feat_fd ff;
	struct feature_event *fe;
	size_t sz, sz_hdr;
	int feat, ret;

	sz_hdr = sizeof(fe->header);
	sz = sizeof(union perf_event);
	/* get a nice alignment */
	sz = PERF_ALIGN(sz, page_size);

	memset(&ff, 0, sizeof(ff));

	ff.buf = malloc(sz);
	if (!ff.buf)
		return -ENOMEM;

	ff.size = sz - sz_hdr;

	for_each_set_bit(feat, header->adds_features, HEADER_FEAT_BITS) {
		if (!feat_ops[feat].synthesize) {
			pr_debug("No record header feature for header :%d\n", feat);
			continue;
		}

		ff.offset = sizeof(*fe);

		ret = feat_ops[feat].write(&ff, evlist);
		if (ret || ff.offset <= (ssize_t)sizeof(*fe)) {
			pr_debug("Error writing feature\n");
			continue;
		}
		/* ff.buf may have changed due to realloc in do_write() */
		fe = ff.buf;
		memset(fe, 0, sizeof(*fe));

		fe->feat_id = feat;
		fe->header.type = PERF_RECORD_HEADER_FEATURE;
		fe->header.size = ff.offset;

		ret = process(tool, ff.buf, NULL, NULL);
		if (ret) {
			free(ff.buf);
			return ret;
		}
	}

	/* Send HEADER_LAST_FEATURE mark. */
	fe = ff.buf;
	fe->feat_id     = HEADER_LAST_FEATURE;
	fe->header.type = PERF_RECORD_HEADER_FEATURE;
	fe->header.size = sizeof(*fe);

	ret = process(tool, ff.buf, NULL, NULL);

	free(ff.buf);
	return ret;
}

int perf_event__process_feature(struct perf_tool *tool,
				union perf_event *event,
				struct perf_session *session __maybe_unused)
{
	struct feat_fd ff = { .fd = 0 };
	struct feature_event *fe = (struct feature_event *)event;
=======
int perf_event__process_feature(struct perf_session *session,
				union perf_event *event)
{
	struct perf_tool *tool = session->tool;
	struct feat_fd ff = { .fd = 0 };
	struct perf_record_header_feature *fe = (struct perf_record_header_feature *)event;
>>>>>>> upstream/android-13
	int type = fe->header.type;
	u64 feat = fe->feat_id;

	if (type < 0 || type >= PERF_RECORD_HEADER_MAX) {
		pr_warning("invalid record type %d in pipe-mode\n", type);
		return 0;
	}
	if (feat == HEADER_RESERVED || feat >= HEADER_LAST_FEATURE) {
		pr_warning("invalid record type %d in pipe-mode\n", type);
		return -1;
	}

	if (!feat_ops[feat].process)
		return 0;

	ff.buf  = (void *)fe->data;
	ff.size = event->header.size - sizeof(*fe);
	ff.ph = &session->header;

	if (feat_ops[feat].process(&ff, NULL))
		return -1;

	if (!feat_ops[feat].print || !tool->show_feat_hdr)
		return 0;

	if (!feat_ops[feat].full_only ||
	    tool->show_feat_hdr >= SHOW_FEAT_HEADER_FULL_INFO) {
		feat_ops[feat].print(&ff, stdout);
	} else {
		fprintf(stdout, "# %s info available, use -I to display\n",
			feat_ops[feat].name);
	}

	return 0;
}

<<<<<<< HEAD
static struct event_update_event *
event_update_event__new(size_t size, u64 type, u64 id)
{
	struct event_update_event *ev;

	size += sizeof(*ev);
	size  = PERF_ALIGN(size, sizeof(u64));

	ev = zalloc(size);
	if (ev) {
		ev->header.type = PERF_RECORD_EVENT_UPDATE;
		ev->header.size = (u16)size;
		ev->type = type;
		ev->id = id;
	}
	return ev;
}

int
perf_event__synthesize_event_update_unit(struct perf_tool *tool,
					 struct perf_evsel *evsel,
					 perf_event__handler_t process)
{
	struct event_update_event *ev;
	size_t size = strlen(evsel->unit);
	int err;

	ev = event_update_event__new(size + 1, PERF_EVENT_UPDATE__UNIT, evsel->id[0]);
	if (ev == NULL)
		return -ENOMEM;

	strlcpy(ev->data, evsel->unit, size + 1);
	err = process(tool, (union perf_event *)ev, NULL, NULL);
	free(ev);
	return err;
}

int
perf_event__synthesize_event_update_scale(struct perf_tool *tool,
					  struct perf_evsel *evsel,
					  perf_event__handler_t process)
{
	struct event_update_event *ev;
	struct event_update_event_scale *ev_data;
	int err;

	ev = event_update_event__new(sizeof(*ev_data), PERF_EVENT_UPDATE__SCALE, evsel->id[0]);
	if (ev == NULL)
		return -ENOMEM;

	ev_data = (struct event_update_event_scale *) ev->data;
	ev_data->scale = evsel->scale;
	err = process(tool, (union perf_event*) ev, NULL, NULL);
	free(ev);
	return err;
}

int
perf_event__synthesize_event_update_name(struct perf_tool *tool,
					 struct perf_evsel *evsel,
					 perf_event__handler_t process)
{
	struct event_update_event *ev;
	size_t len = strlen(evsel->name);
	int err;

	ev = event_update_event__new(len + 1, PERF_EVENT_UPDATE__NAME, evsel->id[0]);
	if (ev == NULL)
		return -ENOMEM;

	strlcpy(ev->data, evsel->name, len + 1);
	err = process(tool, (union perf_event*) ev, NULL, NULL);
	free(ev);
	return err;
}

int
perf_event__synthesize_event_update_cpus(struct perf_tool *tool,
					struct perf_evsel *evsel,
					perf_event__handler_t process)
{
	size_t size = sizeof(struct event_update_event);
	struct event_update_event *ev;
	int max, err;
	u16 type;

	if (!evsel->own_cpus)
		return 0;

	ev = cpu_map_data__alloc(evsel->own_cpus, &size, &type, &max);
	if (!ev)
		return -ENOMEM;

	ev->header.type = PERF_RECORD_EVENT_UPDATE;
	ev->header.size = (u16)size;
	ev->type = PERF_EVENT_UPDATE__CPUS;
	ev->id   = evsel->id[0];

	cpu_map_data__synthesize((struct cpu_map_data *) ev->data,
				 evsel->own_cpus,
				 type, max);

	err = process(tool, (union perf_event*) ev, NULL, NULL);
	free(ev);
	return err;
}

size_t perf_event__fprintf_event_update(union perf_event *event, FILE *fp)
{
	struct event_update_event *ev = &event->event_update;
	struct event_update_event_scale *ev_scale;
	struct event_update_event_cpus *ev_cpus;
	struct cpu_map *map;
	size_t ret;

	ret = fprintf(fp, "\n... id:    %" PRIu64 "\n", ev->id);

	switch (ev->type) {
	case PERF_EVENT_UPDATE__SCALE:
		ev_scale = (struct event_update_event_scale *) ev->data;
=======
size_t perf_event__fprintf_event_update(union perf_event *event, FILE *fp)
{
	struct perf_record_event_update *ev = &event->event_update;
	struct perf_record_event_update_scale *ev_scale;
	struct perf_record_event_update_cpus *ev_cpus;
	struct perf_cpu_map *map;
	size_t ret;

	ret = fprintf(fp, "\n... id:    %" PRI_lu64 "\n", ev->id);

	switch (ev->type) {
	case PERF_EVENT_UPDATE__SCALE:
		ev_scale = (struct perf_record_event_update_scale *)ev->data;
>>>>>>> upstream/android-13
		ret += fprintf(fp, "... scale: %f\n", ev_scale->scale);
		break;
	case PERF_EVENT_UPDATE__UNIT:
		ret += fprintf(fp, "... unit:  %s\n", ev->data);
		break;
	case PERF_EVENT_UPDATE__NAME:
		ret += fprintf(fp, "... name:  %s\n", ev->data);
		break;
	case PERF_EVENT_UPDATE__CPUS:
<<<<<<< HEAD
		ev_cpus = (struct event_update_event_cpus *) ev->data;
=======
		ev_cpus = (struct perf_record_event_update_cpus *)ev->data;
>>>>>>> upstream/android-13
		ret += fprintf(fp, "... ");

		map = cpu_map__new_data(&ev_cpus->cpus);
		if (map)
			ret += cpu_map__fprintf(map, fp);
		else
			ret += fprintf(fp, "failed to get cpus\n");
		break;
	default:
		ret += fprintf(fp, "... unknown type\n");
		break;
	}

	return ret;
}

<<<<<<< HEAD
int perf_event__synthesize_attrs(struct perf_tool *tool,
				   struct perf_session *session,
				   perf_event__handler_t process)
{
	struct perf_evsel *evsel;
	int err = 0;

	evlist__for_each_entry(session->evlist, evsel) {
		err = perf_event__synthesize_attr(tool, &evsel->attr, evsel->ids,
						  evsel->id, process);
		if (err) {
			pr_debug("failed to create perf header attribute\n");
			return err;
		}
	}

	return err;
}

static bool has_unit(struct perf_evsel *counter)
{
	return counter->unit && *counter->unit;
}

static bool has_scale(struct perf_evsel *counter)
{
	return counter->scale != 1;
}

int perf_event__synthesize_extra_attr(struct perf_tool *tool,
				      struct perf_evlist *evsel_list,
				      perf_event__handler_t process,
				      bool is_pipe)
{
	struct perf_evsel *counter;
	int err;

	/*
	 * Synthesize other events stuff not carried within
	 * attr event - unit, scale, name
	 */
	evlist__for_each_entry(evsel_list, counter) {
		if (!counter->supported)
			continue;

		/*
		 * Synthesize unit and scale only if it's defined.
		 */
		if (has_unit(counter)) {
			err = perf_event__synthesize_event_update_unit(tool, counter, process);
			if (err < 0) {
				pr_err("Couldn't synthesize evsel unit.\n");
				return err;
			}
		}

		if (has_scale(counter)) {
			err = perf_event__synthesize_event_update_scale(tool, counter, process);
			if (err < 0) {
				pr_err("Couldn't synthesize evsel counter.\n");
				return err;
			}
		}

		if (counter->own_cpus) {
			err = perf_event__synthesize_event_update_cpus(tool, counter, process);
			if (err < 0) {
				pr_err("Couldn't synthesize evsel cpus.\n");
				return err;
			}
		}

		/*
		 * Name is needed only for pipe output,
		 * perf.data carries event names.
		 */
		if (is_pipe) {
			err = perf_event__synthesize_event_update_name(tool, counter, process);
			if (err < 0) {
				pr_err("Couldn't synthesize evsel name.\n");
				return err;
			}
		}
	}
	return 0;
}

int perf_event__process_attr(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct perf_evlist **pevlist)
{
	u32 i, ids, n_ids;
	struct perf_evsel *evsel;
	struct perf_evlist *evlist = *pevlist;

	if (evlist == NULL) {
		*pevlist = evlist = perf_evlist__new();
=======
int perf_event__process_attr(struct perf_tool *tool __maybe_unused,
			     union perf_event *event,
			     struct evlist **pevlist)
{
	u32 i, ids, n_ids;
	struct evsel *evsel;
	struct evlist *evlist = *pevlist;

	if (evlist == NULL) {
		*pevlist = evlist = evlist__new();
>>>>>>> upstream/android-13
		if (evlist == NULL)
			return -ENOMEM;
	}

<<<<<<< HEAD
	evsel = perf_evsel__new(&event->attr.attr);
	if (evsel == NULL)
		return -ENOMEM;

	perf_evlist__add(evlist, evsel);
=======
	evsel = evsel__new(&event->attr.attr);
	if (evsel == NULL)
		return -ENOMEM;

	evlist__add(evlist, evsel);
>>>>>>> upstream/android-13

	ids = event->header.size;
	ids -= (void *)&event->attr.id - (void *)event;
	n_ids = ids / sizeof(u64);
	/*
	 * We don't have the cpu and thread maps on the header, so
	 * for allocating the perf_sample_id table we fake 1 cpu and
	 * hattr->ids threads.
	 */
<<<<<<< HEAD
	if (perf_evsel__alloc_id(evsel, 1, n_ids))
		return -ENOMEM;

	for (i = 0; i < n_ids; i++) {
		perf_evlist__id_add(evlist, evsel, 0, i, event->attr.id[i]);
=======
	if (perf_evsel__alloc_id(&evsel->core, 1, n_ids))
		return -ENOMEM;

	for (i = 0; i < n_ids; i++) {
		perf_evlist__id_add(&evlist->core, &evsel->core, 0, i, event->attr.id[i]);
>>>>>>> upstream/android-13
	}

	return 0;
}

int perf_event__process_event_update(struct perf_tool *tool __maybe_unused,
				     union perf_event *event,
<<<<<<< HEAD
				     struct perf_evlist **pevlist)
{
	struct event_update_event *ev = &event->event_update;
	struct event_update_event_scale *ev_scale;
	struct event_update_event_cpus *ev_cpus;
	struct perf_evlist *evlist;
	struct perf_evsel *evsel;
	struct cpu_map *map;
=======
				     struct evlist **pevlist)
{
	struct perf_record_event_update *ev = &event->event_update;
	struct perf_record_event_update_scale *ev_scale;
	struct perf_record_event_update_cpus *ev_cpus;
	struct evlist *evlist;
	struct evsel *evsel;
	struct perf_cpu_map *map;
>>>>>>> upstream/android-13

	if (!pevlist || *pevlist == NULL)
		return -EINVAL;

	evlist = *pevlist;

<<<<<<< HEAD
	evsel = perf_evlist__id2evsel(evlist, ev->id);
=======
	evsel = evlist__id2evsel(evlist, ev->id);
>>>>>>> upstream/android-13
	if (evsel == NULL)
		return -EINVAL;

	switch (ev->type) {
	case PERF_EVENT_UPDATE__UNIT:
		evsel->unit = strdup(ev->data);
		break;
	case PERF_EVENT_UPDATE__NAME:
		evsel->name = strdup(ev->data);
		break;
	case PERF_EVENT_UPDATE__SCALE:
<<<<<<< HEAD
		ev_scale = (struct event_update_event_scale *) ev->data;
		evsel->scale = ev_scale->scale;
		break;
	case PERF_EVENT_UPDATE__CPUS:
		ev_cpus = (struct event_update_event_cpus *) ev->data;

		map = cpu_map__new_data(&ev_cpus->cpus);
		if (map)
			evsel->own_cpus = map;
=======
		ev_scale = (struct perf_record_event_update_scale *)ev->data;
		evsel->scale = ev_scale->scale;
		break;
	case PERF_EVENT_UPDATE__CPUS:
		ev_cpus = (struct perf_record_event_update_cpus *)ev->data;

		map = cpu_map__new_data(&ev_cpus->cpus);
		if (map)
			evsel->core.own_cpus = map;
>>>>>>> upstream/android-13
		else
			pr_err("failed to get event_update cpus\n");
	default:
		break;
	}

	return 0;
}

<<<<<<< HEAD
int perf_event__synthesize_tracing_data(struct perf_tool *tool, int fd,
					struct perf_evlist *evlist,
					perf_event__handler_t process)
{
	union perf_event ev;
	struct tracing_data *tdata;
	ssize_t size = 0, aligned_size = 0, padding;
	struct feat_fd ff;
	int err __maybe_unused = 0;

	/*
	 * We are going to store the size of the data followed
	 * by the data contents. Since the fd descriptor is a pipe,
	 * we cannot seek back to store the size of the data once
	 * we know it. Instead we:
	 *
	 * - write the tracing data to the temp file
	 * - get/write the data size to pipe
	 * - write the tracing data from the temp file
	 *   to the pipe
	 */
	tdata = tracing_data_get(&evlist->entries, fd, true);
	if (!tdata)
		return -1;

	memset(&ev, 0, sizeof(ev));

	ev.tracing_data.header.type = PERF_RECORD_HEADER_TRACING_DATA;
	size = tdata->size;
	aligned_size = PERF_ALIGN(size, sizeof(u64));
	padding = aligned_size - size;
	ev.tracing_data.header.size = sizeof(ev.tracing_data);
	ev.tracing_data.size = aligned_size;

	process(tool, &ev, NULL, NULL);

	/*
	 * The put function will copy all the tracing data
	 * stored in temp file to the pipe.
	 */
	tracing_data_put(tdata);

	ff = (struct feat_fd){ .fd = fd };
	if (write_padded(&ff, NULL, 0, padding))
		return -1;

	return aligned_size;
}

int perf_event__process_tracing_data(struct perf_tool *tool __maybe_unused,
				     union perf_event *event,
				     struct perf_session *session)
{
	ssize_t size_read, padding, size = event->tracing_data.size;
	int fd = perf_data__fd(session->data);
	off_t offset = lseek(fd, 0, SEEK_CUR);
	char buf[BUFSIZ];

	/* setup for reading amidst mmap */
	lseek(fd, offset + sizeof(struct tracing_data_event),
	      SEEK_SET);
=======
int perf_event__process_tracing_data(struct perf_session *session,
				     union perf_event *event)
{
	ssize_t size_read, padding, size = event->tracing_data.size;
	int fd = perf_data__fd(session->data);
	char buf[BUFSIZ];

	/*
	 * The pipe fd is already in proper place and in any case
	 * we can't move it, and we'd screw the case where we read
	 * 'pipe' data from regular file. The trace_report reads
	 * data from 'fd' so we need to set it directly behind the
	 * event, where the tracing data starts.
	 */
	if (!perf_data__is_pipe(session->data)) {
		off_t offset = lseek(fd, 0, SEEK_CUR);

		/* setup for reading amidst mmap */
		lseek(fd, offset + sizeof(struct perf_record_header_tracing_data),
		      SEEK_SET);
	}
>>>>>>> upstream/android-13

	size_read = trace_report(fd, &session->tevent,
				 session->repipe);
	padding = PERF_ALIGN(size_read, sizeof(u64)) - size_read;

	if (readn(fd, buf, padding) < 0) {
		pr_err("%s: reading input file", __func__);
		return -1;
	}
	if (session->repipe) {
		int retw = write(STDOUT_FILENO, buf, padding);
		if (retw <= 0 || retw != padding) {
			pr_err("%s: repiping tracing data padding", __func__);
			return -1;
		}
	}

	if (size_read + padding != size) {
		pr_err("%s: tracing data size mismatch", __func__);
		return -1;
	}

<<<<<<< HEAD
	perf_evlist__prepare_tracepoint_events(session->evlist,
					       session->tevent.pevent);
=======
	evlist__prepare_tracepoint_events(session->evlist, session->tevent.pevent);
>>>>>>> upstream/android-13

	return size_read + padding;
}

<<<<<<< HEAD
int perf_event__synthesize_build_id(struct perf_tool *tool,
				    struct dso *pos, u16 misc,
				    perf_event__handler_t process,
				    struct machine *machine)
{
	union perf_event ev;
	size_t len;
	int err = 0;

	if (!pos->hit)
		return err;

	memset(&ev, 0, sizeof(ev));

	len = pos->long_name_len + 1;
	len = PERF_ALIGN(len, NAME_ALIGN);
	memcpy(&ev.build_id.build_id, pos->build_id, sizeof(pos->build_id));
	ev.build_id.header.type = PERF_RECORD_HEADER_BUILD_ID;
	ev.build_id.header.misc = misc;
	ev.build_id.pid = machine->pid;
	ev.build_id.header.size = sizeof(ev.build_id) + len;
	memcpy(&ev.build_id.filename, pos->long_name, pos->long_name_len);

	err = process(tool, &ev, NULL, machine);

	return err;
}

int perf_event__process_build_id(struct perf_tool *tool __maybe_unused,
				 union perf_event *event,
				 struct perf_session *session)
=======
int perf_event__process_build_id(struct perf_session *session,
				 union perf_event *event)
>>>>>>> upstream/android-13
{
	__event_process_build_id(&event->build_id,
				 event->build_id.filename,
				 session);
	return 0;
}
