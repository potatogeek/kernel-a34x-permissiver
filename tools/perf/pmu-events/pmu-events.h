/* SPDX-License-Identifier: GPL-2.0 */
#ifndef PMU_EVENTS_H
#define PMU_EVENTS_H

<<<<<<< HEAD
=======
enum aggr_mode_class {
	PerChip = 1,
	PerCore
};

>>>>>>> upstream/android-13
/*
 * Describe each PMU event. Each CPU has a table of PMU events.
 */
struct pmu_event {
	const char *name;
<<<<<<< HEAD
=======
	const char *compat;
>>>>>>> upstream/android-13
	const char *event;
	const char *desc;
	const char *topic;
	const char *long_desc;
	const char *pmu;
	const char *unit;
	const char *perpkg;
<<<<<<< HEAD
	const char *metric_expr;
	const char *metric_name;
	const char *metric_group;
=======
	const char *aggr_mode;
	const char *metric_expr;
	const char *metric_name;
	const char *metric_group;
	const char *deprecated;
	const char *metric_constraint;
>>>>>>> upstream/android-13
};

/*
 *
 * Map a CPU to its table of PMU events. The CPU is identified by the
 * cpuid field, which is an arch-specific identifier for the CPU.
 * The identifier specified in tools/perf/pmu-events/arch/xxx/mapfile
<<<<<<< HEAD
 * must match the get_cpustr() in tools/perf/arch/xxx/util/header.c)
=======
 * must match the get_cpuid_str() in tools/perf/arch/xxx/util/header.c)
>>>>>>> upstream/android-13
 *
 * The  cpuid can contain any character other than the comma.
 */
struct pmu_events_map {
	const char *cpuid;
	const char *version;
	const char *type;		/* core, uncore etc */
	struct pmu_event *table;
};

<<<<<<< HEAD
=======
struct pmu_sys_events {
	const char *name;
	struct pmu_event *table;
};

>>>>>>> upstream/android-13
/*
 * Global table mapping each known CPU for the architecture to its
 * table of PMU events.
 */
extern struct pmu_events_map pmu_events_map[];
<<<<<<< HEAD
=======
extern struct pmu_sys_events pmu_sys_event_tables[];
>>>>>>> upstream/android-13

#endif
