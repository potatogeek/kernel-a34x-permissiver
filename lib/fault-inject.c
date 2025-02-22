<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/random.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/export.h>
#include <linux/interrupt.h>
#include <linux/stacktrace.h>
#include <linux/fault-inject.h>

/*
 * setup_fault_attr() is a helper function for various __setup handlers, so it
 * returns 0 on error, because that is what __setup handlers do.
 */
int setup_fault_attr(struct fault_attr *attr, char *str)
{
	unsigned long probability;
	unsigned long interval;
	int times;
	int space;

	/* "<interval>,<probability>,<space>,<times>" */
	if (sscanf(str, "%lu,%lu,%d,%d",
			&interval, &probability, &space, &times) < 4) {
		printk(KERN_WARNING
			"FAULT_INJECTION: failed to parse arguments\n");
		return 0;
	}

	attr->probability = probability;
	attr->interval = interval;
	atomic_set(&attr->times, times);
	atomic_set(&attr->space, space);

	return 1;
}
EXPORT_SYMBOL_GPL(setup_fault_attr);

static void fail_dump(struct fault_attr *attr)
{
	if (attr->verbose > 0 && __ratelimit(&attr->ratelimit_state)) {
		printk(KERN_NOTICE "FAULT_INJECTION: forcing a failure.\n"
		       "name %pd, interval %lu, probability %lu, "
		       "space %d, times %d\n", attr->dname,
		       attr->interval, attr->probability,
		       atomic_read(&attr->space),
		       atomic_read(&attr->times));
		if (attr->verbose > 1)
			dump_stack();
	}
}

#define atomic_dec_not_zero(v)		atomic_add_unless((v), -1, 0)

static bool fail_task(struct fault_attr *attr, struct task_struct *task)
{
	return in_task() && task->make_it_fail;
}

#define MAX_STACK_TRACE_DEPTH 32

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER

static bool fail_stacktrace(struct fault_attr *attr)
{
<<<<<<< HEAD
	struct stack_trace trace;
	int depth = attr->stacktrace_depth;
	unsigned long entries[MAX_STACK_TRACE_DEPTH];
	int n;
=======
	int depth = attr->stacktrace_depth;
	unsigned long entries[MAX_STACK_TRACE_DEPTH];
	int n, nr_entries;
>>>>>>> upstream/android-13
	bool found = (attr->require_start == 0 && attr->require_end == ULONG_MAX);

	if (depth == 0)
		return found;

<<<<<<< HEAD
	trace.nr_entries = 0;
	trace.entries = entries;
	trace.max_entries = depth;
	trace.skip = 1;

	save_stack_trace(&trace);
	for (n = 0; n < trace.nr_entries; n++) {
=======
	nr_entries = stack_trace_save(entries, depth, 1);
	for (n = 0; n < nr_entries; n++) {
>>>>>>> upstream/android-13
		if (attr->reject_start <= entries[n] &&
			       entries[n] < attr->reject_end)
			return false;
		if (attr->require_start <= entries[n] &&
			       entries[n] < attr->require_end)
			found = true;
	}
	return found;
}

#else

static inline bool fail_stacktrace(struct fault_attr *attr)
{
	return true;
}

#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

/*
 * This code is stolen from failmalloc-1.0
 * http://www.nongnu.org/failmalloc/
 */

bool should_fail(struct fault_attr *attr, ssize_t size)
{
	if (in_task()) {
		unsigned int fail_nth = READ_ONCE(current->fail_nth);

		if (fail_nth) {
<<<<<<< HEAD
			if (!WRITE_ONCE(current->fail_nth, fail_nth - 1))
=======
			fail_nth--;
			WRITE_ONCE(current->fail_nth, fail_nth);
			if (!fail_nth)
>>>>>>> upstream/android-13
				goto fail;

			return false;
		}
	}

	/* No need to check any other properties if the probability is 0 */
	if (attr->probability == 0)
		return false;

	if (attr->task_filter && !fail_task(attr, current))
		return false;

	if (atomic_read(&attr->times) == 0)
		return false;

	if (atomic_read(&attr->space) > size) {
		atomic_sub(size, &attr->space);
		return false;
	}

	if (attr->interval > 1) {
		attr->count++;
		if (attr->count % attr->interval)
			return false;
	}

	if (attr->probability <= prandom_u32() % 100)
		return false;

	if (!fail_stacktrace(attr))
		return false;

fail:
	fail_dump(attr);

	if (atomic_read(&attr->times) != -1)
		atomic_dec_not_zero(&attr->times);

	return true;
}
EXPORT_SYMBOL_GPL(should_fail);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

static int debugfs_ul_set(void *data, u64 val)
{
	*(unsigned long *)data = val;
	return 0;
}

static int debugfs_ul_get(void *data, u64 *val)
{
	*val = *(unsigned long *)data;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_ul, debugfs_ul_get, debugfs_ul_set, "%llu\n");

<<<<<<< HEAD
static struct dentry *debugfs_create_ul(const char *name, umode_t mode,
				struct dentry *parent, unsigned long *value)
{
	return debugfs_create_file(name, mode, parent, value, &fops_ul);
=======
static void debugfs_create_ul(const char *name, umode_t mode,
			      struct dentry *parent, unsigned long *value)
{
	debugfs_create_file(name, mode, parent, value, &fops_ul);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER

static int debugfs_stacktrace_depth_set(void *data, u64 val)
{
	*(unsigned long *)data =
		min_t(unsigned long, val, MAX_STACK_TRACE_DEPTH);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_stacktrace_depth, debugfs_ul_get,
			debugfs_stacktrace_depth_set, "%llu\n");

<<<<<<< HEAD
static struct dentry *debugfs_create_stacktrace_depth(
	const char *name, umode_t mode,
	struct dentry *parent, unsigned long *value)
{
	return debugfs_create_file(name, mode, parent, value,
				   &fops_stacktrace_depth);
=======
static void debugfs_create_stacktrace_depth(const char *name, umode_t mode,
					    struct dentry *parent,
					    unsigned long *value)
{
	debugfs_create_file(name, mode, parent, value, &fops_stacktrace_depth);
>>>>>>> upstream/android-13
}

#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

struct dentry *fault_create_debugfs_attr(const char *name,
			struct dentry *parent, struct fault_attr *attr)
{
	umode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
	struct dentry *dir;

	dir = debugfs_create_dir(name, parent);
<<<<<<< HEAD
	if (!dir)
		return ERR_PTR(-ENOMEM);

	if (!debugfs_create_ul("probability", mode, dir, &attr->probability))
		goto fail;
	if (!debugfs_create_ul("interval", mode, dir, &attr->interval))
		goto fail;
	if (!debugfs_create_atomic_t("times", mode, dir, &attr->times))
		goto fail;
	if (!debugfs_create_atomic_t("space", mode, dir, &attr->space))
		goto fail;
	if (!debugfs_create_ul("verbose", mode, dir, &attr->verbose))
		goto fail;
	if (!debugfs_create_u32("verbose_ratelimit_interval_ms", mode, dir,
				&attr->ratelimit_state.interval))
		goto fail;
	if (!debugfs_create_u32("verbose_ratelimit_burst", mode, dir,
				&attr->ratelimit_state.burst))
		goto fail;
	if (!debugfs_create_bool("task-filter", mode, dir, &attr->task_filter))
		goto fail;

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER

	if (!debugfs_create_stacktrace_depth("stacktrace-depth", mode, dir,
				&attr->stacktrace_depth))
		goto fail;
	if (!debugfs_create_ul("require-start", mode, dir,
				&attr->require_start))
		goto fail;
	if (!debugfs_create_ul("require-end", mode, dir, &attr->require_end))
		goto fail;
	if (!debugfs_create_ul("reject-start", mode, dir, &attr->reject_start))
		goto fail;
	if (!debugfs_create_ul("reject-end", mode, dir, &attr->reject_end))
		goto fail;

=======
	if (IS_ERR(dir))
		return dir;

	debugfs_create_ul("probability", mode, dir, &attr->probability);
	debugfs_create_ul("interval", mode, dir, &attr->interval);
	debugfs_create_atomic_t("times", mode, dir, &attr->times);
	debugfs_create_atomic_t("space", mode, dir, &attr->space);
	debugfs_create_ul("verbose", mode, dir, &attr->verbose);
	debugfs_create_u32("verbose_ratelimit_interval_ms", mode, dir,
			   &attr->ratelimit_state.interval);
	debugfs_create_u32("verbose_ratelimit_burst", mode, dir,
			   &attr->ratelimit_state.burst);
	debugfs_create_bool("task-filter", mode, dir, &attr->task_filter);

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER
	debugfs_create_stacktrace_depth("stacktrace-depth", mode, dir,
					&attr->stacktrace_depth);
	debugfs_create_ul("require-start", mode, dir, &attr->require_start);
	debugfs_create_ul("require-end", mode, dir, &attr->require_end);
	debugfs_create_ul("reject-start", mode, dir, &attr->reject_start);
	debugfs_create_ul("reject-end", mode, dir, &attr->reject_end);
>>>>>>> upstream/android-13
#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

	attr->dname = dget(dir);
	return dir;
<<<<<<< HEAD
fail:
	debugfs_remove_recursive(dir);

	return ERR_PTR(-ENOMEM);
=======
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(fault_create_debugfs_attr);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */
