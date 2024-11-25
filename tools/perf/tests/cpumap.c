// SPDX-License-Identifier: GPL-2.0
#include "tests.h"
#include <stdio.h>
#include "cpumap.h"
#include "event.h"
<<<<<<< HEAD
#include <string.h>
#include <linux/bitops.h>
=======
#include "util/synthetic-events.h"
#include <string.h>
#include <linux/bitops.h>
#include <perf/cpumap.h>
>>>>>>> upstream/android-13
#include "debug.h"

struct machine;

static int process_event_mask(struct perf_tool *tool __maybe_unused,
			 union perf_event *event,
			 struct perf_sample *sample __maybe_unused,
			 struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct cpu_map_event *map_event = &event->cpu_map;
	struct cpu_map_mask *mask;
	struct cpu_map_data *data;
	struct cpu_map *map;
=======
	struct perf_record_cpu_map *map_event = &event->cpu_map;
	struct perf_record_record_cpu_map *mask;
	struct perf_record_cpu_map_data *data;
	struct perf_cpu_map *map;
>>>>>>> upstream/android-13
	int i;

	data = &map_event->data;

	TEST_ASSERT_VAL("wrong type", data->type == PERF_CPU_MAP__MASK);

<<<<<<< HEAD
	mask = (struct cpu_map_mask *)data->data;
=======
	mask = (struct perf_record_record_cpu_map *)data->data;
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("wrong nr",   mask->nr == 1);

	for (i = 0; i < 20; i++) {
		TEST_ASSERT_VAL("wrong cpu", test_bit(i, mask->mask));
	}

	map = cpu_map__new_data(data);
	TEST_ASSERT_VAL("wrong nr",  map->nr == 20);

	for (i = 0; i < 20; i++) {
		TEST_ASSERT_VAL("wrong cpu", map->map[i] == i);
	}

<<<<<<< HEAD
	cpu_map__put(map);
=======
	perf_cpu_map__put(map);
>>>>>>> upstream/android-13
	return 0;
}

static int process_event_cpus(struct perf_tool *tool __maybe_unused,
			 union perf_event *event,
			 struct perf_sample *sample __maybe_unused,
			 struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct cpu_map_event *map_event = &event->cpu_map;
	struct cpu_map_entries *cpus;
	struct cpu_map_data *data;
	struct cpu_map *map;
=======
	struct perf_record_cpu_map *map_event = &event->cpu_map;
	struct cpu_map_entries *cpus;
	struct perf_record_cpu_map_data *data;
	struct perf_cpu_map *map;
>>>>>>> upstream/android-13

	data = &map_event->data;

	TEST_ASSERT_VAL("wrong type", data->type == PERF_CPU_MAP__CPUS);

	cpus = (struct cpu_map_entries *)data->data;

	TEST_ASSERT_VAL("wrong nr",   cpus->nr == 2);
	TEST_ASSERT_VAL("wrong cpu",  cpus->cpu[0] == 1);
	TEST_ASSERT_VAL("wrong cpu",  cpus->cpu[1] == 256);

	map = cpu_map__new_data(data);
	TEST_ASSERT_VAL("wrong nr",  map->nr == 2);
	TEST_ASSERT_VAL("wrong cpu", map->map[0] == 1);
	TEST_ASSERT_VAL("wrong cpu", map->map[1] == 256);
	TEST_ASSERT_VAL("wrong refcnt", refcount_read(&map->refcnt) == 1);
<<<<<<< HEAD
	cpu_map__put(map);
=======
	perf_cpu_map__put(map);
>>>>>>> upstream/android-13
	return 0;
}


int test__cpu_map_synthesize(struct test *test __maybe_unused, int subtest __maybe_unused)
{
<<<<<<< HEAD
	struct cpu_map *cpus;

	/* This one is better stores in mask. */
	cpus = cpu_map__new("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19");
=======
	struct perf_cpu_map *cpus;

	/* This one is better stores in mask. */
	cpus = perf_cpu_map__new("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19");
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("failed to synthesize map",
		!perf_event__synthesize_cpu_map(NULL, cpus, process_event_mask, NULL));

<<<<<<< HEAD
	cpu_map__put(cpus);

	/* This one is better stores in cpu values. */
	cpus = cpu_map__new("1,256");
=======
	perf_cpu_map__put(cpus);

	/* This one is better stores in cpu values. */
	cpus = perf_cpu_map__new("1,256");
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("failed to synthesize map",
		!perf_event__synthesize_cpu_map(NULL, cpus, process_event_cpus, NULL));

<<<<<<< HEAD
	cpu_map__put(cpus);
=======
	perf_cpu_map__put(cpus);
>>>>>>> upstream/android-13
	return 0;
}

static int cpu_map_print(const char *str)
{
<<<<<<< HEAD
	struct cpu_map *map = cpu_map__new(str);
=======
	struct perf_cpu_map *map = perf_cpu_map__new(str);
>>>>>>> upstream/android-13
	char buf[100];

	if (!map)
		return -1;

	cpu_map__snprint(map, buf, sizeof(buf));
<<<<<<< HEAD
=======
	perf_cpu_map__put(map);

>>>>>>> upstream/android-13
	return !strcmp(buf, str);
}

int test__cpu_map_print(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("1"));
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("1,5"));
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("1,3,5,7,9,11,13,15,17,19,21-40"));
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("2-5"));
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("1,3-6,8-10,24,35-37"));
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("1,3-6,8-10,24,35-37"));
	TEST_ASSERT_VAL("failed to convert map", cpu_map_print("1-10,12-20,22-30,32-40"));
	return 0;
}
<<<<<<< HEAD
=======

int test__cpu_map_merge(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	struct perf_cpu_map *a = perf_cpu_map__new("4,2,1");
	struct perf_cpu_map *b = perf_cpu_map__new("4,5,7");
	struct perf_cpu_map *c = perf_cpu_map__merge(a, b);
	char buf[100];

	TEST_ASSERT_VAL("failed to merge map: bad nr", c->nr == 5);
	cpu_map__snprint(c, buf, sizeof(buf));
	TEST_ASSERT_VAL("failed to merge map: bad result", !strcmp(buf, "1-2,4-5,7"));
	perf_cpu_map__put(b);
	perf_cpu_map__put(c);
	return 0;
}
>>>>>>> upstream/android-13
