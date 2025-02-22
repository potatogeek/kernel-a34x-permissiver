/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __TRACE_HELPER_H
#define __TRACE_HELPER_H

<<<<<<< HEAD
#include <libbpf.h>
#include <linux/perf_event.h>
=======
#include <bpf/libbpf.h>
>>>>>>> upstream/android-13

struct ksym {
	long addr;
	char *name;
};

int load_kallsyms(void);
struct ksym *ksym_search(long key);
long ksym_get_addr(const char *name);

<<<<<<< HEAD
typedef enum bpf_perf_event_ret (*perf_event_print_fn)(void *data, int size);

int perf_event_mmap(int fd);
int perf_event_mmap_header(int fd, struct perf_event_mmap_page **header);
/* return LIBBPF_PERF_EVENT_DONE or LIBBPF_PERF_EVENT_ERROR */
int perf_event_poller(int fd, perf_event_print_fn output_fn);
int perf_event_poller_multi(int *fds, struct perf_event_mmap_page **headers,
			    int num_fds, perf_event_print_fn output_fn);
=======
/* open kallsyms and find addresses on the fly, faster than load + search. */
int kallsyms_find(const char *sym, unsigned long long *addr);

void read_trace_pipe(void);

ssize_t get_uprobe_offset(const void *addr, ssize_t base);
ssize_t get_base_addr(void);
ssize_t get_rel_offset(uintptr_t addr);

>>>>>>> upstream/android-13
#endif
