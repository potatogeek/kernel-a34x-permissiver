// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include <linux/mm.h>
#include <linux/gfp.h>
=======
/*
 * Copyright (c) 2017-2019 Borislav Petkov, SUSE Labs.
 */
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/ras.h>
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/workqueue.h>

#include <asm/mce.h>

#include "debugfs.h"

/*
 * RAS Correctable Errors Collector
 *
 * This is a simple gadget which collects correctable errors and counts their
 * occurrence per physical page address.
 *
 * We've opted for possibly the simplest data structure to collect those - an
 * array of the size of a memory page. It stores 512 u64's with the following
 * structure:
 *
 * [63 ... PFN ... 12 | 11 ... generation ... 10 | 9 ... count ... 0]
 *
 * The generation in the two highest order bits is two bits which are set to 11b
 * on every insertion. During the course of each entry's existence, the
 * generation field gets decremented during spring cleaning to 10b, then 01b and
 * then 00b.
 *
 * This way we're employing the natural numeric ordering to make sure that newly
 * inserted/touched elements have higher 12-bit counts (which we've manufactured)
 * and thus iterating over the array initially won't kick out those elements
 * which were inserted last.
 *
 * Spring cleaning is what we do when we reach a certain number CLEAN_ELEMS of
 * elements entered into the array, during which, we're decaying all elements.
 * If, after decay, an element gets inserted again, its generation is set to 11b
 * to make sure it has higher numerical count than other, older elements and
 * thus emulate an an LRU-like behavior when deleting elements to free up space
 * in the page.
 *
<<<<<<< HEAD
 * When an element reaches it's max count of count_threshold, we try to poison
 * it by assuming that errors triggered count_threshold times in a single page
 * are excessive and that page shouldn't be used anymore. count_threshold is
=======
 * When an element reaches it's max count of action_threshold, we try to poison
 * it by assuming that errors triggered action_threshold times in a single page
 * are excessive and that page shouldn't be used anymore. action_threshold is
>>>>>>> upstream/android-13
 * initialized to COUNT_MASK which is the maximum.
 *
 * That error event entry causes cec_add_elem() to return !0 value and thus
 * signal to its callers to log the error.
 *
 * To the question why we've chosen a page and moving elements around with
 * memmove(), it is because it is a very simple structure to handle and max data
 * movement is 4K which on highly optimized modern CPUs is almost unnoticeable.
 * We wanted to avoid the pointer traversal of more complex structures like a
 * linked list or some sort of a balancing search tree.
 *
 * Deleting an element takes O(n) but since it is only a single page, it should
 * be fast enough and it shouldn't happen all too often depending on error
 * patterns.
 */

#undef pr_fmt
#define pr_fmt(fmt) "RAS: " fmt

/*
 * We use DECAY_BITS bits of PAGE_SHIFT bits for counting decay, i.e., how long
 * elements have stayed in the array without having been accessed again.
 */
#define DECAY_BITS		2
#define DECAY_MASK		((1ULL << DECAY_BITS) - 1)
#define MAX_ELEMS		(PAGE_SIZE / sizeof(u64))

/*
 * Threshold amount of inserted elements after which we start spring
 * cleaning.
 */
#define CLEAN_ELEMS		(MAX_ELEMS >> DECAY_BITS)

/* Bits which count the number of errors happened in this 4K page. */
#define COUNT_BITS		(PAGE_SHIFT - DECAY_BITS)
#define COUNT_MASK		((1ULL << COUNT_BITS) - 1)
#define FULL_COUNT_MASK		(PAGE_SIZE - 1)

/*
 * u64: [ 63 ... 12 | DECAY_BITS | COUNT_BITS ]
 */

#define PFN(e)			((e) >> PAGE_SHIFT)
#define DECAY(e)		(((e) >> COUNT_BITS) & DECAY_MASK)
#define COUNT(e)		((unsigned int)(e) & COUNT_MASK)
#define FULL_COUNT(e)		((e) & (PAGE_SIZE - 1))

static struct ce_array {
	u64 *array;			/* container page */
	unsigned int n;			/* number of elements in the array */

	unsigned int decay_count;	/*
					 * number of element insertions/increments
					 * since the last spring cleaning.
					 */

	u64 pfns_poisoned;		/*
					 * number of PFNs which got poisoned.
					 */

	u64 ces_entered;		/*
					 * The number of correctable errors
					 * entered into the collector.
					 */

	u64 decays_done;		/*
					 * Times we did spring cleaning.
					 */

	union {
		struct {
			__u32	disabled : 1,	/* cmdline disabled */
			__resv   : 31;
		};
		__u32 flags;
	};
} ce_arr;

static DEFINE_MUTEX(ce_mutex);
static u64 dfs_pfn;

/* Amount of errors after which we offline */
<<<<<<< HEAD
static unsigned int count_threshold = COUNT_MASK;
=======
static u64 action_threshold = COUNT_MASK;
>>>>>>> upstream/android-13

/* Each element "decays" each decay_interval which is 24hrs by default. */
#define CEC_DECAY_DEFAULT_INTERVAL	24 * 60 * 60	/* 24 hrs */
#define CEC_DECAY_MIN_INTERVAL		 1 * 60 * 60	/* 1h */
#define CEC_DECAY_MAX_INTERVAL	   30 *	24 * 60 * 60	/* one month */
static struct delayed_work cec_work;
static u64 decay_interval = CEC_DECAY_DEFAULT_INTERVAL;

/*
 * Decrement decay value. We're using DECAY_BITS bits to denote decay of an
 * element in the array. On insertion and any access, it gets reset to max.
 */
static void do_spring_cleaning(struct ce_array *ca)
{
	int i;

	for (i = 0; i < ca->n; i++) {
		u8 decay = DECAY(ca->array[i]);

		if (!decay)
			continue;

		decay--;

		ca->array[i] &= ~(DECAY_MASK << COUNT_BITS);
		ca->array[i] |= (decay << COUNT_BITS);
	}
	ca->decay_count = 0;
	ca->decays_done++;
}

/*
 * @interval in seconds
 */
static void cec_mod_work(unsigned long interval)
{
	unsigned long iv;

	iv = interval * HZ;
	mod_delayed_work(system_wq, &cec_work, round_jiffies(iv));
}

static void cec_work_fn(struct work_struct *work)
{
	mutex_lock(&ce_mutex);
	do_spring_cleaning(&ce_arr);
	mutex_unlock(&ce_mutex);

	cec_mod_work(decay_interval);
}

/*
 * @to: index of the smallest element which is >= then @pfn.
 *
 * Return the index of the pfn if found, otherwise negative value.
 */
static int __find_elem(struct ce_array *ca, u64 pfn, unsigned int *to)
{
	int min = 0, max = ca->n - 1;
	u64 this_pfn;

	while (min <= max) {
		int i = (min + max) >> 1;

		this_pfn = PFN(ca->array[i]);

		if (this_pfn < pfn)
			min = i + 1;
		else if (this_pfn > pfn)
			max = i - 1;
		else if (this_pfn == pfn) {
			if (to)
				*to = i;

			return i;
		}
	}

	/*
	 * When the loop terminates without finding @pfn, min has the index of
	 * the element slot where the new @pfn should be inserted. The loop
	 * terminates when min > max, which means the min index points to the
	 * bigger element while the max index to the smaller element, in-between
	 * which the new @pfn belongs to.
	 *
	 * For more details, see exercise 1, Section 6.2.1 in TAOCP, vol. 3.
	 */
	if (to)
		*to = min;

	return -ENOKEY;
}

static int find_elem(struct ce_array *ca, u64 pfn, unsigned int *to)
{
	WARN_ON(!to);

	if (!ca->n) {
		*to = 0;
		return -ENOKEY;
	}
	return __find_elem(ca, pfn, to);
}

static void del_elem(struct ce_array *ca, int idx)
{
	/* Save us a function call when deleting the last element. */
	if (ca->n - (idx + 1))
		memmove((void *)&ca->array[idx],
			(void *)&ca->array[idx + 1],
			(ca->n - (idx + 1)) * sizeof(u64));

	ca->n--;
}

static u64 del_lru_elem_unlocked(struct ce_array *ca)
{
	unsigned int min = FULL_COUNT_MASK;
	int i, min_idx = 0;

	for (i = 0; i < ca->n; i++) {
		unsigned int this = FULL_COUNT(ca->array[i]);

		if (min > this) {
			min = this;
			min_idx = i;
		}
	}

	del_elem(ca, min_idx);

	return PFN(ca->array[min_idx]);
}

/*
 * We return the 0th pfn in the error case under the assumption that it cannot
 * be poisoned and excessive CEs in there are a serious deal anyway.
 */
static u64 __maybe_unused del_lru_elem(void)
{
	struct ce_array *ca = &ce_arr;
	u64 pfn;

	if (!ca->n)
		return 0;

	mutex_lock(&ce_mutex);
	pfn = del_lru_elem_unlocked(ca);
	mutex_unlock(&ce_mutex);

	return pfn;
}

<<<<<<< HEAD

int cec_add_elem(u64 pfn)
{
	struct ce_array *ca = &ce_arr;
	unsigned int to;
	int count, ret = 0;
=======
static bool sanity_check(struct ce_array *ca)
{
	bool ret = false;
	u64 prev = 0;
	int i;

	for (i = 0; i < ca->n; i++) {
		u64 this = PFN(ca->array[i]);

		if (WARN(prev > this, "prev: 0x%016llx <-> this: 0x%016llx\n", prev, this))
			ret = true;

		prev = this;
	}

	if (!ret)
		return ret;

	pr_info("Sanity check dump:\n{ n: %d\n", ca->n);
	for (i = 0; i < ca->n; i++) {
		u64 this = PFN(ca->array[i]);

		pr_info(" %03d: [%016llx|%03llx]\n", i, this, FULL_COUNT(ca->array[i]));
	}
	pr_info("}\n");

	return ret;
}

/**
 * cec_add_elem - Add an element to the CEC array.
 * @pfn:	page frame number to insert
 *
 * Return values:
 * - <0:	on error
 * -  0:	on success
 * - >0:	when the inserted pfn was offlined
 */
static int cec_add_elem(u64 pfn)
{
	struct ce_array *ca = &ce_arr;
	int count, err, ret = 0;
	unsigned int to = 0;
>>>>>>> upstream/android-13

	/*
	 * We can be called very early on the identify_cpu() path where we are
	 * not initialized yet. We ignore the error for simplicity.
	 */
	if (!ce_arr.array || ce_arr.disabled)
		return -ENODEV;

<<<<<<< HEAD
	ca->ces_entered++;

	mutex_lock(&ce_mutex);

	if (ca->n == MAX_ELEMS)
		WARN_ON(!del_lru_elem_unlocked(ca));

	ret = find_elem(ca, pfn, &to);
	if (ret < 0) {
=======
	mutex_lock(&ce_mutex);

	ca->ces_entered++;

	/* Array full, free the LRU slot. */
	if (ca->n == MAX_ELEMS)
		WARN_ON(!del_lru_elem_unlocked(ca));

	err = find_elem(ca, pfn, &to);
	if (err < 0) {
>>>>>>> upstream/android-13
		/*
		 * Shift range [to-end] to make room for one more element.
		 */
		memmove((void *)&ca->array[to + 1],
			(void *)&ca->array[to],
			(ca->n - to) * sizeof(u64));

<<<<<<< HEAD
		ca->array[to] = (pfn << PAGE_SHIFT) |
				(DECAY_MASK << COUNT_BITS) | 1;

		ca->n++;

		ret = 0;

		goto decay;
	}

	count = COUNT(ca->array[to]);

	if (count < count_threshold) {
		ca->array[to] |= (DECAY_MASK << COUNT_BITS);
		ca->array[to]++;

		ret = 0;
	} else {
=======
		ca->array[to] = pfn << PAGE_SHIFT;
		ca->n++;
	}

	/* Add/refresh element generation and increment count */
	ca->array[to] |= DECAY_MASK << COUNT_BITS;
	ca->array[to]++;

	/* Check action threshold and soft-offline, if reached. */
	count = COUNT(ca->array[to]);
	if (count >= action_threshold) {
>>>>>>> upstream/android-13
		u64 pfn = ca->array[to] >> PAGE_SHIFT;

		if (!pfn_valid(pfn)) {
			pr_warn("CEC: Invalid pfn: 0x%llx\n", pfn);
		} else {
			/* We have reached max count for this page, soft-offline it. */
			pr_err("Soft-offlining pfn: 0x%llx\n", pfn);
			memory_failure_queue(pfn, MF_SOFT_OFFLINE);
			ca->pfns_poisoned++;
		}

		del_elem(ca, to);

		/*
<<<<<<< HEAD
		 * Return a >0 value to denote that we've reached the offlining
		 * threshold.
=======
		 * Return a >0 value to callers, to denote that we've reached
		 * the offlining threshold.
>>>>>>> upstream/android-13
		 */
		ret = 1;

		goto unlock;
	}

<<<<<<< HEAD
decay:
=======
>>>>>>> upstream/android-13
	ca->decay_count++;

	if (ca->decay_count >= CLEAN_ELEMS)
		do_spring_cleaning(ca);

<<<<<<< HEAD
=======
	WARN_ON_ONCE(sanity_check(ca));

>>>>>>> upstream/android-13
unlock:
	mutex_unlock(&ce_mutex);

	return ret;
}

static int u64_get(void *data, u64 *val)
{
	*val = *(u64 *)data;

	return 0;
}

static int pfn_set(void *data, u64 val)
{
	*(u64 *)data = val;

	cec_add_elem(val);

	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(pfn_ops, u64_get, pfn_set, "0x%llx\n");

static int decay_interval_set(void *data, u64 val)
{
<<<<<<< HEAD
	*(u64 *)data = val;

=======
>>>>>>> upstream/android-13
	if (val < CEC_DECAY_MIN_INTERVAL)
		return -EINVAL;

	if (val > CEC_DECAY_MAX_INTERVAL)
		return -EINVAL;

<<<<<<< HEAD
	decay_interval = val;

	cec_mod_work(decay_interval);
=======
	*(u64 *)data   = val;
	decay_interval = val;

	cec_mod_work(decay_interval);

>>>>>>> upstream/android-13
	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(decay_interval_ops, u64_get, decay_interval_set, "%lld\n");

<<<<<<< HEAD
static int count_threshold_set(void *data, u64 val)
=======
static int action_threshold_set(void *data, u64 val)
>>>>>>> upstream/android-13
{
	*(u64 *)data = val;

	if (val > COUNT_MASK)
		val = COUNT_MASK;

<<<<<<< HEAD
	count_threshold = val;

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(count_threshold_ops, u64_get, count_threshold_set, "%lld\n");

static int array_dump(struct seq_file *m, void *v)
{
	struct ce_array *ca = &ce_arr;
	u64 prev = 0;
=======
	action_threshold = val;

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(action_threshold_ops, u64_get, action_threshold_set, "%lld\n");

static const char * const bins[] = { "00", "01", "10", "11" };

static int array_show(struct seq_file *m, void *v)
{
	struct ce_array *ca = &ce_arr;
>>>>>>> upstream/android-13
	int i;

	mutex_lock(&ce_mutex);

	seq_printf(m, "{ n: %d\n", ca->n);
	for (i = 0; i < ca->n; i++) {
		u64 this = PFN(ca->array[i]);

<<<<<<< HEAD
		seq_printf(m, " %03d: [%016llx|%03llx]\n", i, this, FULL_COUNT(ca->array[i]));

		WARN_ON(prev > this);

		prev = this;
=======
		seq_printf(m, " %3d: [%016llx|%s|%03llx]\n",
			   i, this, bins[DECAY(ca->array[i])], COUNT(ca->array[i]));
>>>>>>> upstream/android-13
	}

	seq_printf(m, "}\n");

	seq_printf(m, "Stats:\nCEs: %llu\nofflined pages: %llu\n",
		   ca->ces_entered, ca->pfns_poisoned);

	seq_printf(m, "Flags: 0x%x\n", ca->flags);

	seq_printf(m, "Decay interval: %lld seconds\n", decay_interval);
	seq_printf(m, "Decays: %lld\n", ca->decays_done);

<<<<<<< HEAD
	seq_printf(m, "Action threshold: %d\n", count_threshold);
=======
	seq_printf(m, "Action threshold: %lld\n", action_threshold);
>>>>>>> upstream/android-13

	mutex_unlock(&ce_mutex);

	return 0;
}

<<<<<<< HEAD
static int array_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, array_dump, NULL);
}

static const struct file_operations array_ops = {
	.owner	 = THIS_MODULE,
	.open	 = array_open,
	.read	 = seq_read,
	.llseek	 = seq_lseek,
	.release = single_release,
};
=======
DEFINE_SHOW_ATTRIBUTE(array);
>>>>>>> upstream/android-13

static int __init create_debugfs_nodes(void)
{
	struct dentry *d, *pfn, *decay, *count, *array;

	d = debugfs_create_dir("cec", ras_debugfs_dir);
	if (!d) {
		pr_warn("Error creating cec debugfs node!\n");
		return -1;
	}

<<<<<<< HEAD
	pfn = debugfs_create_file("pfn", S_IRUSR | S_IWUSR, d, &dfs_pfn, &pfn_ops);
	if (!pfn) {
		pr_warn("Error creating pfn debugfs node!\n");
		goto err;
	}

	array = debugfs_create_file("array", S_IRUSR, d, NULL, &array_ops);
	if (!array) {
		pr_warn("Error creating array debugfs node!\n");
		goto err;
	}

=======
>>>>>>> upstream/android-13
	decay = debugfs_create_file("decay_interval", S_IRUSR | S_IWUSR, d,
				    &decay_interval, &decay_interval_ops);
	if (!decay) {
		pr_warn("Error creating decay_interval debugfs node!\n");
		goto err;
	}

<<<<<<< HEAD
	count = debugfs_create_file("count_threshold", S_IRUSR | S_IWUSR, d,
				    &count_threshold, &count_threshold_ops);
	if (!count) {
		pr_warn("Error creating count_threshold debugfs node!\n");
		goto err;
	}

=======
	count = debugfs_create_file("action_threshold", S_IRUSR | S_IWUSR, d,
				    &action_threshold, &action_threshold_ops);
	if (!count) {
		pr_warn("Error creating action_threshold debugfs node!\n");
		goto err;
	}

	if (!IS_ENABLED(CONFIG_RAS_CEC_DEBUG))
		return 0;

	pfn = debugfs_create_file("pfn", S_IRUSR | S_IWUSR, d, &dfs_pfn, &pfn_ops);
	if (!pfn) {
		pr_warn("Error creating pfn debugfs node!\n");
		goto err;
	}

	array = debugfs_create_file("array", S_IRUSR, d, NULL, &array_fops);
	if (!array) {
		pr_warn("Error creating array debugfs node!\n");
		goto err;
	}
>>>>>>> upstream/android-13

	return 0;

err:
	debugfs_remove_recursive(d);

	return 1;
}

<<<<<<< HEAD
void __init cec_init(void)
{
	if (ce_arr.disabled)
		return;
=======
static int cec_notifier(struct notifier_block *nb, unsigned long val,
			void *data)
{
	struct mce *m = (struct mce *)data;

	if (!m)
		return NOTIFY_DONE;

	/* We eat only correctable DRAM errors with usable addresses. */
	if (mce_is_memory_error(m) &&
	    mce_is_correctable(m)  &&
	    mce_usable_address(m)) {
		if (!cec_add_elem(m->addr >> PAGE_SHIFT)) {
			m->kflags |= MCE_HANDLED_CEC;
			return NOTIFY_OK;
		}
	}

	return NOTIFY_DONE;
}

static struct notifier_block cec_nb = {
	.notifier_call	= cec_notifier,
	.priority	= MCE_PRIO_CEC,
};

static int __init cec_init(void)
{
	if (ce_arr.disabled)
		return -ENODEV;
>>>>>>> upstream/android-13

	ce_arr.array = (void *)get_zeroed_page(GFP_KERNEL);
	if (!ce_arr.array) {
		pr_err("Error allocating CE array page!\n");
<<<<<<< HEAD
		return;
	}

	if (create_debugfs_nodes())
		return;
=======
		return -ENOMEM;
	}

	if (create_debugfs_nodes()) {
		free_page((unsigned long)ce_arr.array);
		return -ENOMEM;
	}
>>>>>>> upstream/android-13

	INIT_DELAYED_WORK(&cec_work, cec_work_fn);
	schedule_delayed_work(&cec_work, CEC_DECAY_DEFAULT_INTERVAL);

<<<<<<< HEAD
	pr_info("Correctable Errors collector initialized.\n");
}
=======
	mce_register_decode_chain(&cec_nb);

	pr_info("Correctable Errors collector initialized.\n");
	return 0;
}
late_initcall(cec_init);
>>>>>>> upstream/android-13

int __init parse_cec_param(char *str)
{
	if (!str)
		return 0;

	if (*str == '=')
		str++;

	if (!strcmp(str, "cec_disable"))
		ce_arr.disabled = 1;
	else
		return 0;

	return 1;
}
