/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_COUNTS_H
#define __PERF_COUNTS_H

<<<<<<< HEAD
#include "xyarray.h"

struct perf_counts_values {
	union {
		struct {
			u64 val;
			u64 ena;
			u64 run;
		};
		u64 values[3];
	};
	bool	loaded;
};
=======
#include <linux/types.h>
#include <internal/xyarray.h>
#include <perf/evsel.h>
#include <stdbool.h>

struct evsel;
>>>>>>> upstream/android-13

struct perf_counts {
	s8			  scaled;
	struct perf_counts_values aggr;
	struct xyarray		  *values;
<<<<<<< HEAD
=======
	struct xyarray		  *loaded;
>>>>>>> upstream/android-13
};


static inline struct perf_counts_values*
perf_counts(struct perf_counts *counts, int cpu, int thread)
{
	return xyarray__entry(counts->values, cpu, thread);
}

<<<<<<< HEAD
struct perf_counts *perf_counts__new(int ncpus, int nthreads);
void perf_counts__delete(struct perf_counts *counts);

void perf_evsel__reset_counts(struct perf_evsel *evsel);
int perf_evsel__alloc_counts(struct perf_evsel *evsel, int ncpus, int nthreads);
void perf_evsel__free_counts(struct perf_evsel *evsel);
=======
static inline bool
perf_counts__is_loaded(struct perf_counts *counts, int cpu, int thread)
{
	return *((bool *) xyarray__entry(counts->loaded, cpu, thread));
}

static inline void
perf_counts__set_loaded(struct perf_counts *counts, int cpu, int thread, bool loaded)
{
	*((bool *) xyarray__entry(counts->loaded, cpu, thread)) = loaded;
}

struct perf_counts *perf_counts__new(int ncpus, int nthreads);
void perf_counts__delete(struct perf_counts *counts);
void perf_counts__reset(struct perf_counts *counts);

void evsel__reset_counts(struct evsel *evsel);
int evsel__alloc_counts(struct evsel *evsel, int ncpus, int nthreads);
void evsel__free_counts(struct evsel *evsel);
>>>>>>> upstream/android-13

#endif /* __PERF_COUNTS_H */
