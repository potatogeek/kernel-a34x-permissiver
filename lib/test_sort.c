<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only

#include <kunit/test.h>

>>>>>>> upstream/android-13
#include <linux/sort.h>
#include <linux/slab.h>
#include <linux/module.h>

/* a simple boot-time regression test */

#define TEST_LEN 1000

<<<<<<< HEAD
static int __init cmpint(const void *a, const void *b)
=======
static int cmpint(const void *a, const void *b)
>>>>>>> upstream/android-13
{
	return *(int *)a - *(int *)b;
}

<<<<<<< HEAD
static int __init test_sort_init(void)
{
	int *a, i, r = 1, err = -ENOMEM;

	a = kmalloc_array(TEST_LEN, sizeof(*a), GFP_KERNEL);
	if (!a)
		return err;
=======
static void test_sort(struct kunit *test)
{
	int *a, i, r = 1;

	a = kunit_kmalloc_array(test, TEST_LEN, sizeof(*a), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, a);
>>>>>>> upstream/android-13

	for (i = 0; i < TEST_LEN; i++) {
		r = (r * 725861) % 6599;
		a[i] = r;
	}

	sort(a, TEST_LEN, sizeof(*a), cmpint, NULL);

<<<<<<< HEAD
	err = -EINVAL;
	for (i = 0; i < TEST_LEN-1; i++)
		if (a[i] > a[i+1]) {
			pr_err("test has failed\n");
			goto exit;
		}
	err = 0;
	pr_info("test passed\n");
exit:
	kfree(a);
	return err;
}

static void __exit test_sort_exit(void)
{
}

module_init(test_sort_init);
module_exit(test_sort_exit);
=======
	for (i = 0; i < TEST_LEN-1; i++)
		KUNIT_ASSERT_LE(test, a[i], a[i + 1]);
}

static struct kunit_case sort_test_cases[] = {
	KUNIT_CASE(test_sort),
	{}
};

static struct kunit_suite sort_test_suite = {
	.name = "lib_sort",
	.test_cases = sort_test_cases,
};

kunit_test_suites(&sort_test_suite);
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL");
