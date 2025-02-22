// SPDX-License-Identifier: GPL-2.0

#include <bpf.h>

struct bpf_map SEC("maps") __bpf_stdout__ = {
       .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
       .key_size = sizeof(int),
       .value_size = sizeof(u32),
       .max_entries = __NR_CPUS__,
};

<<<<<<< HEAD
static int (*perf_event_output)(void *, struct bpf_map *, int, void *, unsigned long) =
	(void *)BPF_FUNC_perf_event_output;

=======
>>>>>>> upstream/android-13
#define puts(from) \
	({ const int __len = sizeof(from); \
	   char __from[__len] = from; \
	   perf_event_output(args, &__bpf_stdout__, BPF_F_CURRENT_CPU, \
			  &__from, __len & (sizeof(from) - 1)); })
