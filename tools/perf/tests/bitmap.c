// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
#include <linux/bitmap.h>
<<<<<<< HEAD
#include "tests.h"
#include "cpumap.h"
=======
#include <perf/cpumap.h>
#include <internal/cpumap.h>
#include "tests.h"
>>>>>>> upstream/android-13
#include "debug.h"

#define NBITS 100

static unsigned long *get_bitmap(const char *str, int nbits)
{
<<<<<<< HEAD
	struct cpu_map *map = cpu_map__new(str);
	unsigned long *bm = NULL;
	int i;

	bm = bitmap_alloc(nbits);
=======
	struct perf_cpu_map *map = perf_cpu_map__new(str);
	unsigned long *bm = NULL;
	int i;

	bm = bitmap_zalloc(nbits);
>>>>>>> upstream/android-13

	if (map && bm) {
		for (i = 0; i < map->nr; i++)
			set_bit(map->map[i], bm);
	}

	if (map)
<<<<<<< HEAD
		cpu_map__put(map);
=======
		perf_cpu_map__put(map);
>>>>>>> upstream/android-13
	return bm;
}

static int test_bitmap(const char *str)
{
	unsigned long *bm = get_bitmap(str, NBITS);
	char buf[100];
	int ret;

	bitmap_scnprintf(bm, NBITS, buf, sizeof(buf));
	pr_debug("bitmap: %s\n", buf);

	ret = !strcmp(buf, str);
	free(bm);
	return ret;
}

int test__bitmap_print(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("1"));
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("1,5"));
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("1,3,5,7,9,11,13,15,17,19,21-40"));
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("2-5"));
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("1,3-6,8-10,24,35-37"));
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("1,3-6,8-10,24,35-37"));
	TEST_ASSERT_VAL("failed to convert map", test_bitmap("1-10,12-20,22-30,32-40"));
	return 0;
}
