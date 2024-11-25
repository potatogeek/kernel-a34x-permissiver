/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_TSC_H
#define __PERF_TSC_H

#include <linux/types.h>

#include "event.h"

struct perf_tsc_conversion {
	u16 time_shift;
	u32 time_mult;
	u64 time_zero;
<<<<<<< HEAD
};
=======
	u64 time_cycles;
	u64 time_mask;

	bool cap_user_time_zero;
	bool cap_user_time_short;
};

>>>>>>> upstream/android-13
struct perf_event_mmap_page;

int perf_read_tsc_conversion(const struct perf_event_mmap_page *pc,
			     struct perf_tsc_conversion *tc);

u64 perf_time_to_tsc(u64 ns, struct perf_tsc_conversion *tc);
u64 tsc_to_perf_time(u64 cyc, struct perf_tsc_conversion *tc);
u64 rdtsc(void);

<<<<<<< HEAD
struct perf_event_mmap_page;
struct perf_tool;
struct machine;

int perf_event__synth_time_conv(const struct perf_event_mmap_page *pc,
				struct perf_tool *tool,
				perf_event__handler_t process,
				struct machine *machine);

#endif
=======
size_t perf_event__fprintf_time_conv(union perf_event *event, FILE *fp);

#endif // __PERF_TSC_H
>>>>>>> upstream/android-13
