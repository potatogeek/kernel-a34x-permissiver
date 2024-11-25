// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
<<<<<<< HEAD
#include "evlist.h"
#include "evsel.h"
#include "machine.h"
=======
#include <perf/cpumap.h>
#include <string.h>
#include "cpumap.h"
#include "evlist.h"
#include "evsel.h"
#include "header.h"
#include "machine.h"
#include "util/synthetic-events.h"
#include "tool.h"
>>>>>>> upstream/android-13
#include "tests.h"
#include "debug.h"

static int process_event_unit(struct perf_tool *tool __maybe_unused,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct event_update_event *ev = (struct event_update_event *) event;
=======
	struct perf_record_event_update *ev = (struct perf_record_event_update *)event;
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("wrong id", ev->id == 123);
	TEST_ASSERT_VAL("wrong id", ev->type == PERF_EVENT_UPDATE__UNIT);
	TEST_ASSERT_VAL("wrong unit", !strcmp(ev->data, "KRAVA"));
	return 0;
}

static int process_event_scale(struct perf_tool *tool __maybe_unused,
			       union perf_event *event,
			       struct perf_sample *sample __maybe_unused,
			       struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct event_update_event *ev = (struct event_update_event *) event;
	struct event_update_event_scale *ev_data;

	ev_data = (struct event_update_event_scale *) ev->data;
=======
	struct perf_record_event_update *ev = (struct perf_record_event_update *)event;
	struct perf_record_event_update_scale *ev_data;

	ev_data = (struct perf_record_event_update_scale *)ev->data;
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("wrong id", ev->id == 123);
	TEST_ASSERT_VAL("wrong id", ev->type == PERF_EVENT_UPDATE__SCALE);
	TEST_ASSERT_VAL("wrong scale", ev_data->scale == 0.123);
	return 0;
}

struct event_name {
	struct perf_tool tool;
	const char *name;
};

static int process_event_name(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
	struct event_name *tmp = container_of(tool, struct event_name, tool);
<<<<<<< HEAD
	struct event_update_event *ev = (struct event_update_event*) event;
=======
	struct perf_record_event_update *ev = (struct perf_record_event_update *)event;
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("wrong id", ev->id == 123);
	TEST_ASSERT_VAL("wrong id", ev->type == PERF_EVENT_UPDATE__NAME);
	TEST_ASSERT_VAL("wrong name", !strcmp(ev->data, tmp->name));
	return 0;
}

static int process_event_cpus(struct perf_tool *tool __maybe_unused,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct event_update_event *ev = (struct event_update_event*) event;
	struct event_update_event_cpus *ev_data;
	struct cpu_map *map;

	ev_data = (struct event_update_event_cpus*) ev->data;
=======
	struct perf_record_event_update *ev = (struct perf_record_event_update *)event;
	struct perf_record_event_update_cpus *ev_data;
	struct perf_cpu_map *map;

	ev_data = (struct perf_record_event_update_cpus *) ev->data;
>>>>>>> upstream/android-13

	map = cpu_map__new_data(&ev_data->cpus);

	TEST_ASSERT_VAL("wrong id", ev->id == 123);
	TEST_ASSERT_VAL("wrong type", ev->type == PERF_EVENT_UPDATE__CPUS);
	TEST_ASSERT_VAL("wrong cpus", map->nr == 3);
	TEST_ASSERT_VAL("wrong cpus", map->map[0] == 1);
	TEST_ASSERT_VAL("wrong cpus", map->map[1] == 2);
	TEST_ASSERT_VAL("wrong cpus", map->map[2] == 3);
<<<<<<< HEAD
	cpu_map__put(map);
=======
	perf_cpu_map__put(map);
>>>>>>> upstream/android-13
	return 0;
}

int test__event_update(struct test *test __maybe_unused, int subtest __maybe_unused)
{
<<<<<<< HEAD
	struct perf_evlist *evlist;
	struct perf_evsel *evsel;
	struct event_name tmp;

	evlist = perf_evlist__new_default();
	TEST_ASSERT_VAL("failed to get evlist", evlist);

	evsel = perf_evlist__first(evlist);

	TEST_ASSERT_VAL("failed to allos ids",
			!perf_evsel__alloc_id(evsel, 1, 1));

	perf_evlist__id_add(evlist, evsel, 0, 0, 123);

	evsel->unit = strdup("KRAVA");
=======
	struct evsel *evsel;
	struct event_name tmp;
	struct evlist *evlist = evlist__new_default();
	char *unit = strdup("KRAVA");

	TEST_ASSERT_VAL("failed to get evlist", evlist);

	evsel = evlist__first(evlist);

	TEST_ASSERT_VAL("failed to allocate ids",
			!perf_evsel__alloc_id(&evsel->core, 1, 1));

	perf_evlist__id_add(&evlist->core, &evsel->core, 0, 0, 123);

	evsel->unit = unit;
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("failed to synthesize attr update unit",
			!perf_event__synthesize_event_update_unit(NULL, evsel, process_event_unit));

	evsel->scale = 0.123;

	TEST_ASSERT_VAL("failed to synthesize attr update scale",
			!perf_event__synthesize_event_update_scale(NULL, evsel, process_event_scale));

<<<<<<< HEAD
	tmp.name = perf_evsel__name(evsel);
=======
	tmp.name = evsel__name(evsel);
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("failed to synthesize attr update name",
			!perf_event__synthesize_event_update_name(&tmp.tool, evsel, process_event_name));

<<<<<<< HEAD
	evsel->own_cpus = cpu_map__new("1,2,3");
=======
	evsel->core.own_cpus = perf_cpu_map__new("1,2,3");
>>>>>>> upstream/android-13

	TEST_ASSERT_VAL("failed to synthesize attr update cpus",
			!perf_event__synthesize_event_update_cpus(&tmp.tool, evsel, process_event_cpus));

<<<<<<< HEAD
	cpu_map__put(evsel->own_cpus);
=======
	free(unit);
	evlist__delete(evlist);
>>>>>>> upstream/android-13
	return 0;
}
