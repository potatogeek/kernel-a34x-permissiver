<<<<<<< HEAD
#define pr_fmt(fmt) "list_sort_test: " fmt
=======
// SPDX-License-Identifier: GPL-2.0-only
#include <kunit/test.h>
>>>>>>> upstream/android-13

#include <linux/kernel.h>
#include <linux/list_sort.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/random.h>

/*
 * The pattern of set bits in the list length determines which cases
 * are hit in list_sort().
 */
#define TEST_LIST_LEN (512+128+2) /* not including head */

#define TEST_POISON1 0xDEADBEEF
#define TEST_POISON2 0xA324354C

struct debug_el {
	unsigned int poison1;
	struct list_head list;
	unsigned int poison2;
	int value;
<<<<<<< HEAD
	unsigned serial;
};

/* Array, containing pointers to all elements in the test list */
static struct debug_el **elts __initdata;

static int __init check(struct debug_el *ela, struct debug_el *elb)
{
	if (ela->serial >= TEST_LIST_LEN) {
		pr_err("error: incorrect serial %d\n", ela->serial);
		return -EINVAL;
	}
	if (elb->serial >= TEST_LIST_LEN) {
		pr_err("error: incorrect serial %d\n", elb->serial);
		return -EINVAL;
	}
	if (elts[ela->serial] != ela || elts[elb->serial] != elb) {
		pr_err("error: phantom element\n");
		return -EINVAL;
	}
	if (ela->poison1 != TEST_POISON1 || ela->poison2 != TEST_POISON2) {
		pr_err("error: bad poison: %#x/%#x\n",
			ela->poison1, ela->poison2);
		return -EINVAL;
	}
	if (elb->poison1 != TEST_POISON1 || elb->poison2 != TEST_POISON2) {
		pr_err("error: bad poison: %#x/%#x\n",
			elb->poison1, elb->poison2);
		return -EINVAL;
	}
	return 0;
}

static int __init cmp(void *priv, struct list_head *a, struct list_head *b)
=======
	unsigned int serial;
};

static void check(struct kunit *test, struct debug_el *ela, struct debug_el *elb)
{
	struct debug_el **elts = test->priv;

	KUNIT_EXPECT_LT_MSG(test, ela->serial, (unsigned int)TEST_LIST_LEN, "incorrect serial");
	KUNIT_EXPECT_LT_MSG(test, elb->serial, (unsigned int)TEST_LIST_LEN, "incorrect serial");

	KUNIT_EXPECT_PTR_EQ_MSG(test, elts[ela->serial], ela, "phantom element");
	KUNIT_EXPECT_PTR_EQ_MSG(test, elts[elb->serial], elb, "phantom element");

	KUNIT_EXPECT_EQ_MSG(test, ela->poison1, TEST_POISON1, "bad poison");
	KUNIT_EXPECT_EQ_MSG(test, ela->poison2, TEST_POISON2, "bad poison");

	KUNIT_EXPECT_EQ_MSG(test, elb->poison1, TEST_POISON1, "bad poison");
	KUNIT_EXPECT_EQ_MSG(test, elb->poison2, TEST_POISON2, "bad poison");
}

/* `priv` is the test pointer so check() can fail the test if the list is invalid. */
static int cmp(void *priv, const struct list_head *a, const struct list_head *b)
>>>>>>> upstream/android-13
{
	struct debug_el *ela, *elb;

	ela = container_of(a, struct debug_el, list);
	elb = container_of(b, struct debug_el, list);

<<<<<<< HEAD
	check(ela, elb);
	return ela->value - elb->value;
}

static int __init list_sort_test(void)
{
	int i, count = 1, err = -ENOMEM;
	struct debug_el *el;
	struct list_head *cur;
	LIST_HEAD(head);

	pr_debug("start testing list_sort()\n");

	elts = kcalloc(TEST_LIST_LEN, sizeof(*elts), GFP_KERNEL);
	if (!elts)
		return err;

	for (i = 0; i < TEST_LIST_LEN; i++) {
		el = kmalloc(sizeof(*el), GFP_KERNEL);
		if (!el)
			goto exit;
=======
	check(priv, ela, elb);
	return ela->value - elb->value;
}

static void list_sort_test(struct kunit *test)
{
	int i, count = 1;
	struct debug_el *el, **elts;
	struct list_head *cur;
	LIST_HEAD(head);

	elts = kunit_kcalloc(test, TEST_LIST_LEN, sizeof(*elts), GFP_KERNEL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, elts);
	test->priv = elts;

	for (i = 0; i < TEST_LIST_LEN; i++) {
		el = kunit_kmalloc(test, sizeof(*el), GFP_KERNEL);
		KUNIT_ASSERT_NOT_ERR_OR_NULL(test, el);
>>>>>>> upstream/android-13

		 /* force some equivalencies */
		el->value = prandom_u32() % (TEST_LIST_LEN / 3);
		el->serial = i;
		el->poison1 = TEST_POISON1;
		el->poison2 = TEST_POISON2;
		elts[i] = el;
		list_add_tail(&el->list, &head);
	}

<<<<<<< HEAD
	list_sort(NULL, &head, cmp);

	err = -EINVAL;
=======
	list_sort(test, &head, cmp);

>>>>>>> upstream/android-13
	for (cur = head.next; cur->next != &head; cur = cur->next) {
		struct debug_el *el1;
		int cmp_result;

<<<<<<< HEAD
		if (cur->next->prev != cur) {
			pr_err("error: list is corrupted\n");
			goto exit;
		}

		cmp_result = cmp(NULL, cur, cur->next);
		if (cmp_result > 0) {
			pr_err("error: list is not sorted\n");
			goto exit;
		}

		el = container_of(cur, struct debug_el, list);
		el1 = container_of(cur->next, struct debug_el, list);
		if (cmp_result == 0 && el->serial >= el1->serial) {
			pr_err("error: order of equivalent elements not "
				"preserved\n");
			goto exit;
		}

		if (check(el, el1)) {
			pr_err("error: element check failed\n");
			goto exit;
		}
		count++;
	}
	if (head.prev != cur) {
		pr_err("error: list is corrupted\n");
		goto exit;
	}


	if (count != TEST_LIST_LEN) {
		pr_err("error: bad list length %d", count);
		goto exit;
	}

	err = 0;
exit:
	for (i = 0; i < TEST_LIST_LEN; i++)
		kfree(elts[i]);
	kfree(elts);
	return err;
}
module_init(list_sort_test);
=======
		KUNIT_ASSERT_PTR_EQ_MSG(test, cur->next->prev, cur,
					"list is corrupted");

		cmp_result = cmp(test, cur, cur->next);
		KUNIT_ASSERT_LE_MSG(test, cmp_result, 0, "list is not sorted");

		el = container_of(cur, struct debug_el, list);
		el1 = container_of(cur->next, struct debug_el, list);
		if (cmp_result == 0) {
			KUNIT_ASSERT_LE_MSG(test, el->serial, el1->serial,
					    "order of equivalent elements not preserved");
		}

		check(test, el, el1);
		count++;
	}
	KUNIT_EXPECT_PTR_EQ_MSG(test, head.prev, cur, "list is corrupted");

	KUNIT_EXPECT_EQ_MSG(test, count, TEST_LIST_LEN,
			    "list length changed after sorting!");
}

static struct kunit_case list_sort_cases[] = {
	KUNIT_CASE(list_sort_test),
	{}
};

static struct kunit_suite list_sort_suite = {
	.name = "list_sort",
	.test_cases = list_sort_cases,
};

kunit_test_suites(&list_sort_suite);

>>>>>>> upstream/android-13
MODULE_LICENSE("GPL");
