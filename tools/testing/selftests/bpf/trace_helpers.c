// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
<<<<<<< HEAD
=======
#include <fcntl.h>
>>>>>>> upstream/android-13
#include <poll.h>
#include <unistd.h>
#include <linux/perf_event.h>
#include <sys/mman.h>
#include "trace_helpers.h"

<<<<<<< HEAD
=======
#define DEBUGFS "/sys/kernel/debug/tracing/"

>>>>>>> upstream/android-13
#define MAX_SYMS 300000
static struct ksym syms[MAX_SYMS];
static int sym_cnt;

static int ksym_cmp(const void *p1, const void *p2)
{
	return ((struct ksym *)p1)->addr - ((struct ksym *)p2)->addr;
}

int load_kallsyms(void)
{
	FILE *f = fopen("/proc/kallsyms", "r");
	char func[256], buf[256];
	char symbol;
	void *addr;
	int i = 0;

	if (!f)
		return -ENOENT;

<<<<<<< HEAD
	while (!feof(f)) {
		if (!fgets(buf, sizeof(buf), f))
			break;
=======
	while (fgets(buf, sizeof(buf), f)) {
>>>>>>> upstream/android-13
		if (sscanf(buf, "%p %c %s", &addr, &symbol, func) != 3)
			break;
		if (!addr)
			continue;
		syms[i].addr = (long) addr;
		syms[i].name = strdup(func);
		i++;
	}
	fclose(f);
	sym_cnt = i;
	qsort(syms, sym_cnt, sizeof(struct ksym), ksym_cmp);
	return 0;
}

struct ksym *ksym_search(long key)
{
	int start = 0, end = sym_cnt;
	int result;

	/* kallsyms not loaded. return NULL */
	if (sym_cnt <= 0)
		return NULL;

	while (start < end) {
		size_t mid = start + (end - start) / 2;

		result = key - syms[mid].addr;
		if (result < 0)
			end = mid;
		else if (result > 0)
			start = mid + 1;
		else
			return &syms[mid];
	}

	if (start >= 1 && syms[start - 1].addr < key &&
	    key < syms[start].addr)
		/* valid ksym */
		return &syms[start - 1];

	/* out of range. return _stext */
	return &syms[0];
}

long ksym_get_addr(const char *name)
{
	int i;

	for (i = 0; i < sym_cnt; i++) {
		if (strcmp(syms[i].name, name) == 0)
			return syms[i].addr;
	}

	return 0;
}

<<<<<<< HEAD
static int page_size;
static int page_cnt = 8;
static struct perf_event_mmap_page *header;

int perf_event_mmap_header(int fd, struct perf_event_mmap_page **header)
{
	void *base;
	int mmap_size;

	page_size = getpagesize();
	mmap_size = page_size * (page_cnt + 1);

	base = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (base == MAP_FAILED) {
		printf("mmap err\n");
		return -1;
	}

	*header = base;
	return 0;
}

int perf_event_mmap(int fd)
{
	return perf_event_mmap_header(fd, &header);
}

static int perf_event_poll(int fd)
{
	struct pollfd pfd = { .fd = fd, .events = POLLIN };

	return poll(&pfd, 1, 1000);
}

struct perf_event_sample {
	struct perf_event_header header;
	__u32 size;
	char data[];
};

static enum bpf_perf_event_ret bpf_perf_event_print(void *event, void *priv)
{
	struct perf_event_sample *e = event;
	perf_event_print_fn fn = priv;
	int ret;

	if (e->header.type == PERF_RECORD_SAMPLE) {
		ret = fn(e->data, e->size);
		if (ret != LIBBPF_PERF_EVENT_CONT)
			return ret;
	} else if (e->header.type == PERF_RECORD_LOST) {
		struct {
			struct perf_event_header header;
			__u64 id;
			__u64 lost;
		} *lost = (void *) e;
		printf("lost %lld events\n", lost->lost);
	} else {
		printf("unknown event type=%d size=%d\n",
		       e->header.type, e->header.size);
	}

	return LIBBPF_PERF_EVENT_CONT;
}

int perf_event_poller(int fd, perf_event_print_fn output_fn)
{
	enum bpf_perf_event_ret ret;
	void *buf = NULL;
	size_t len = 0;

	for (;;) {
		perf_event_poll(fd);
		ret = bpf_perf_event_read_simple(header, page_cnt * page_size,
						 page_size, &buf, &len,
						 bpf_perf_event_print,
						 output_fn);
		if (ret != LIBBPF_PERF_EVENT_CONT)
			break;
	}
	free(buf);

	return ret;
}

int perf_event_poller_multi(int *fds, struct perf_event_mmap_page **headers,
			    int num_fds, perf_event_print_fn output_fn)
{
	enum bpf_perf_event_ret ret;
	struct pollfd *pfds;
	void *buf = NULL;
	size_t len = 0;
	int i;

	pfds = calloc(num_fds, sizeof(*pfds));
	if (!pfds)
		return LIBBPF_PERF_EVENT_ERROR;

	for (i = 0; i < num_fds; i++) {
		pfds[i].fd = fds[i];
		pfds[i].events = POLLIN;
	}

	for (;;) {
		poll(pfds, num_fds, 1000);
		for (i = 0; i < num_fds; i++) {
			if (!pfds[i].revents)
				continue;

			ret = bpf_perf_event_read_simple(headers[i],
							 page_cnt * page_size,
							 page_size, &buf, &len,
							 bpf_perf_event_print,
							 output_fn);
			if (ret != LIBBPF_PERF_EVENT_CONT)
				break;
		}
	}
	free(buf);
	free(pfds);

	return ret;
=======
/* open kallsyms and read symbol addresses on the fly. Without caching all symbols,
 * this is faster than load + find.
 */
int kallsyms_find(const char *sym, unsigned long long *addr)
{
	char type, name[500];
	unsigned long long value;
	int err = 0;
	FILE *f;

	f = fopen("/proc/kallsyms", "r");
	if (!f)
		return -EINVAL;

	while (fscanf(f, "%llx %c %499s%*[^\n]\n", &value, &type, name) > 0) {
		if (strcmp(name, sym) == 0) {
			*addr = value;
			goto out;
		}
	}
	err = -ENOENT;

out:
	fclose(f);
	return err;
}

void read_trace_pipe(void)
{
	int trace_fd;

	trace_fd = open(DEBUGFS "trace_pipe", O_RDONLY, 0);
	if (trace_fd < 0)
		return;

	while (1) {
		static char buf[4096];
		ssize_t sz;

		sz = read(trace_fd, buf, sizeof(buf) - 1);
		if (sz > 0) {
			buf[sz] = 0;
			puts(buf);
		}
	}
}

#if defined(__powerpc64__) && defined(_CALL_ELF) && _CALL_ELF == 2

#define OP_RT_RA_MASK   0xffff0000UL
#define LIS_R2          0x3c400000UL
#define ADDIS_R2_R12    0x3c4c0000UL
#define ADDI_R2_R2      0x38420000UL

ssize_t get_uprobe_offset(const void *addr, ssize_t base)
{
	u32 *insn = (u32 *)(uintptr_t)addr;

	/*
	 * A PPC64 ABIv2 function may have a local and a global entry
	 * point. We need to use the local entry point when patching
	 * functions, so identify and step over the global entry point
	 * sequence.
	 *
	 * The global entry point sequence is always of the form:
	 *
	 * addis r2,r12,XXXX
	 * addi  r2,r2,XXXX
	 *
	 * A linker optimisation may convert the addis to lis:
	 *
	 * lis   r2,XXXX
	 * addi  r2,r2,XXXX
	 */
	if ((((*insn & OP_RT_RA_MASK) == ADDIS_R2_R12) ||
	     ((*insn & OP_RT_RA_MASK) == LIS_R2)) &&
	    ((*(insn + 1) & OP_RT_RA_MASK) == ADDI_R2_R2))
		return (ssize_t)(insn + 2) - base;
	else
		return (uintptr_t)addr - base;
}

#else

ssize_t get_uprobe_offset(const void *addr, ssize_t base)
{
	return (uintptr_t)addr - base;
}

#endif

ssize_t get_base_addr(void)
{
	size_t start, offset;
	char buf[256];
	FILE *f;

	f = fopen("/proc/self/maps", "r");
	if (!f)
		return -errno;

	while (fscanf(f, "%zx-%*x %s %zx %*[^\n]\n",
		      &start, buf, &offset) == 3) {
		if (strcmp(buf, "r-xp") == 0) {
			fclose(f);
			return start - offset;
		}
	}

	fclose(f);
	return -EINVAL;
}

ssize_t get_rel_offset(uintptr_t addr)
{
	size_t start, end, offset;
	char buf[256];
	FILE *f;

	f = fopen("/proc/self/maps", "r");
	if (!f)
		return -errno;

	while (fscanf(f, "%zx-%zx %s %zx %*[^\n]\n", &start, &end, buf, &offset) == 4) {
		if (addr >= start && addr < end) {
			fclose(f);
			return (size_t)addr - start + offset;
		}
	}

	fclose(f);
	return -EINVAL;
>>>>>>> upstream/android-13
}
