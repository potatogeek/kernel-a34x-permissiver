// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

typedef void(*test_ubsan_fp)(void);

<<<<<<< HEAD
static void test_ubsan_add_overflow(void)
{
	volatile int val = INT_MAX;

	val += 2;
}

static void test_ubsan_sub_overflow(void)
{
	volatile int val = INT_MIN;
	volatile int val2 = 2;

	val -= val2;
}

static void test_ubsan_mul_overflow(void)
{
	volatile int val = INT_MAX / 2;

	val *= 3;
}

static void test_ubsan_negate_overflow(void)
{
	volatile int val = INT_MIN;

	val = -val;
}
=======
#define UBSAN_TEST(config, ...)	do {					\
		pr_info("%s " __VA_ARGS__ "%s(%s=%s)\n", __func__,	\
			sizeof(" " __VA_ARGS__) > 2 ? " " : "",		\
			#config, IS_ENABLED(config) ? "y" : "n");	\
	} while (0)
>>>>>>> upstream/android-13

static void test_ubsan_divrem_overflow(void)
{
	volatile int val = 16;
	volatile int val2 = 0;

<<<<<<< HEAD
	val /= val2;
}

static void test_ubsan_vla_bound_not_positive(void)
{
	volatile int size = -1;
	char buf[size];

	(void)buf;
}

static void test_ubsan_shift_out_of_bounds(void)
{
	volatile int val = -1;
	int val2 = 10;

	val2 <<= val;
=======
	UBSAN_TEST(CONFIG_UBSAN_DIV_ZERO);
	val /= val2;
}

static void test_ubsan_shift_out_of_bounds(void)
{
	volatile int neg = -1, wrap = 4;
	int val1 = 10;
	int val2 = INT_MAX;

	UBSAN_TEST(CONFIG_UBSAN_SHIFT, "negative exponent");
	val1 <<= neg;

	UBSAN_TEST(CONFIG_UBSAN_SHIFT, "left overflow");
	val2 <<= wrap;
>>>>>>> upstream/android-13
}

static void test_ubsan_out_of_bounds(void)
{
<<<<<<< HEAD
	volatile int i = 4, j = 5;
	volatile int arr[i];

	arr[j] = i;
}

=======
	volatile int i = 4, j = 5, k = -1;
	volatile char above[4] = { }; /* Protect surrounding memory. */
	volatile int arr[4];
	volatile char below[4] = { }; /* Protect surrounding memory. */

	above[0] = below[0];

	UBSAN_TEST(CONFIG_UBSAN_BOUNDS, "above");
	arr[j] = i;

	UBSAN_TEST(CONFIG_UBSAN_BOUNDS, "below");
	arr[k] = i;
}

enum ubsan_test_enum {
	UBSAN_TEST_ZERO = 0,
	UBSAN_TEST_ONE,
	UBSAN_TEST_MAX,
};

>>>>>>> upstream/android-13
static void test_ubsan_load_invalid_value(void)
{
	volatile char *dst, *src;
	bool val, val2, *ptr;
<<<<<<< HEAD
	char c = 4;

=======
	enum ubsan_test_enum eval, eval2, *eptr;
	unsigned char c = 0xff;

	UBSAN_TEST(CONFIG_UBSAN_BOOL, "bool");
>>>>>>> upstream/android-13
	dst = (char *)&val;
	src = &c;
	*dst = *src;

	ptr = &val2;
	val2 = val;
<<<<<<< HEAD
}

static void test_ubsan_null_ptr_deref(void)
{
	volatile int *ptr = NULL;
	int val;

	val = *ptr;
=======

	UBSAN_TEST(CONFIG_UBSAN_ENUM, "enum");
	dst = (char *)&eval;
	src = &c;
	*dst = *src;

	eptr = &eval2;
	eval2 = eval;
>>>>>>> upstream/android-13
}

static void test_ubsan_misaligned_access(void)
{
	volatile char arr[5] __aligned(4) = {1, 2, 3, 4, 5};
	volatile int *ptr, val = 6;

<<<<<<< HEAD
=======
	UBSAN_TEST(CONFIG_UBSAN_ALIGNMENT);
>>>>>>> upstream/android-13
	ptr = (int *)(arr + 1);
	*ptr = val;
}

<<<<<<< HEAD
static void test_ubsan_object_size_mismatch(void)
{
	/* "((aligned(8)))" helps this not into be misaligned for ptr-access. */
	volatile int val __aligned(8) = 4;
	volatile long long *ptr, val2;

	ptr = (long long *)&val;
	val2 = *ptr;
}

static const test_ubsan_fp test_ubsan_array[] = {
	test_ubsan_add_overflow,
	test_ubsan_sub_overflow,
	test_ubsan_mul_overflow,
	test_ubsan_negate_overflow,
	test_ubsan_divrem_overflow,
	test_ubsan_vla_bound_not_positive,
	test_ubsan_shift_out_of_bounds,
	test_ubsan_out_of_bounds,
	test_ubsan_load_invalid_value,
	//test_ubsan_null_ptr_deref, /* exclude it because there is a crash */
	test_ubsan_misaligned_access,
	test_ubsan_object_size_mismatch,
=======
static const test_ubsan_fp test_ubsan_array[] = {
	test_ubsan_shift_out_of_bounds,
	test_ubsan_out_of_bounds,
	test_ubsan_load_invalid_value,
	test_ubsan_misaligned_access,
};

/* Excluded because they Oops the module. */
static const test_ubsan_fp skip_ubsan_array[] = {
	test_ubsan_divrem_overflow,
>>>>>>> upstream/android-13
};

static int __init test_ubsan_init(void)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(test_ubsan_array); i++)
		test_ubsan_array[i]();

<<<<<<< HEAD
	(void)test_ubsan_null_ptr_deref; /* to avoid unsed-function warning */
=======
>>>>>>> upstream/android-13
	return 0;
}
module_init(test_ubsan_init);

static void __exit test_ubsan_exit(void)
{
	/* do nothing */
}
module_exit(test_ubsan_exit);

MODULE_AUTHOR("Jinbum Park <jinb.park7@gmail.com>");
MODULE_LICENSE("GPL v2");
