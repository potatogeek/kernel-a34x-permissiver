// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include "perf.h"
#include "util/debug.h"
#include "util/event.h"
=======
#include "util/debug.h"
#include "util/dso.h"
#include "util/event.h"
#include "util/map.h"
>>>>>>> upstream/android-13
#include "util/symbol.h"
#include "util/sort.h"
#include "util/evsel.h"
#include "util/evlist.h"
#include "util/machine.h"
#include "util/thread.h"
#include "util/parse-events.h"
#include "tests/tests.h"
#include "tests/hists_common.h"
#include <linux/kernel.h>

struct sample {
	u32 cpu;
	u32 pid;
	u64 ip;
	struct thread *thread;
	struct map *map;
	struct symbol *sym;
};

/* For the numbers, see hists_common.c */
static struct sample fake_samples[] = {
	/* perf [kernel] schedule() */
	{ .cpu = 0, .pid = FAKE_PID_PERF1, .ip = FAKE_IP_KERNEL_SCHEDULE, },
	/* perf [perf]   main() */
	{ .cpu = 1, .pid = FAKE_PID_PERF1, .ip = FAKE_IP_PERF_MAIN, },
	/* perf [perf]   cmd_record() */
	{ .cpu = 1, .pid = FAKE_PID_PERF1, .ip = FAKE_IP_PERF_CMD_RECORD, },
	/* perf [libc]   malloc() */
	{ .cpu = 1, .pid = FAKE_PID_PERF1, .ip = FAKE_IP_LIBC_MALLOC, },
	/* perf [libc]   free() */
	{ .cpu = 2, .pid = FAKE_PID_PERF1, .ip = FAKE_IP_LIBC_FREE, },
	/* perf [perf]   main() */
	{ .cpu = 2, .pid = FAKE_PID_PERF2, .ip = FAKE_IP_PERF_MAIN, },
	/* perf [kernel] page_fault() */
	{ .cpu = 2, .pid = FAKE_PID_PERF2, .ip = FAKE_IP_KERNEL_PAGE_FAULT, },
	/* bash [bash]   main() */
	{ .cpu = 3, .pid = FAKE_PID_BASH,  .ip = FAKE_IP_BASH_MAIN, },
	/* bash [bash]   xmalloc() */
	{ .cpu = 0, .pid = FAKE_PID_BASH,  .ip = FAKE_IP_BASH_XMALLOC, },
	/* bash [kernel] page_fault() */
	{ .cpu = 1, .pid = FAKE_PID_BASH,  .ip = FAKE_IP_KERNEL_PAGE_FAULT, },
};

static int add_hist_entries(struct hists *hists, struct machine *machine)
{
	struct addr_location al;
<<<<<<< HEAD
	struct perf_evsel *evsel = hists_to_evsel(hists);
=======
	struct evsel *evsel = hists_to_evsel(hists);
>>>>>>> upstream/android-13
	struct perf_sample sample = { .period = 100, };
	size_t i;

	for (i = 0; i < ARRAY_SIZE(fake_samples); i++) {
		struct hist_entry_iter iter = {
			.evsel = evsel,
			.sample = &sample,
			.ops = &hist_iter_normal,
			.hide_unresolved = false,
		};

		sample.cpumode = PERF_RECORD_MISC_USER;
		sample.cpu = fake_samples[i].cpu;
		sample.pid = fake_samples[i].pid;
		sample.tid = fake_samples[i].pid;
		sample.ip = fake_samples[i].ip;

		if (machine__resolve(machine, &al, &sample) < 0)
			goto out;

		if (hist_entry_iter__add(&iter, &al, sysctl_perf_event_max_stack,
					 NULL) < 0) {
			addr_location__put(&al);
			goto out;
		}

		fake_samples[i].thread = al.thread;
		fake_samples[i].map = al.map;
		fake_samples[i].sym = al.sym;
	}

	return TEST_OK;

out:
	pr_debug("Not enough memory for adding a hist entry\n");
	return TEST_FAIL;
}

static void del_hist_entries(struct hists *hists)
{
	struct hist_entry *he;
<<<<<<< HEAD
	struct rb_root *root_in;
	struct rb_root *root_out;
=======
	struct rb_root_cached *root_in;
	struct rb_root_cached *root_out;
>>>>>>> upstream/android-13
	struct rb_node *node;

	if (hists__has(hists, need_collapse))
		root_in = &hists->entries_collapsed;
	else
		root_in = hists->entries_in;

	root_out = &hists->entries;

<<<<<<< HEAD
	while (!RB_EMPTY_ROOT(root_out)) {
		node = rb_first(root_out);

		he = rb_entry(node, struct hist_entry, rb_node);
		rb_erase(node, root_out);
		rb_erase(&he->rb_node_in, root_in);
=======
	while (!RB_EMPTY_ROOT(&root_out->rb_root)) {
		node = rb_first_cached(root_out);

		he = rb_entry(node, struct hist_entry, rb_node);
		rb_erase_cached(node, root_out);
		rb_erase_cached(&he->rb_node_in, root_in);
>>>>>>> upstream/android-13
		hist_entry__delete(he);
	}
}

<<<<<<< HEAD
typedef int (*test_fn_t)(struct perf_evsel *, struct machine *);
=======
typedef int (*test_fn_t)(struct evsel *, struct machine *);
>>>>>>> upstream/android-13

#define COMM(he)  (thread__comm_str(he->thread))
#define DSO(he)   (he->ms.map->dso->short_name)
#define SYM(he)   (he->ms.sym->name)
#define CPU(he)   (he->cpu)
#define PID(he)   (he->thread->tid)

/* default sort keys (no field) */
<<<<<<< HEAD
static int test1(struct perf_evsel *evsel, struct machine *machine)
=======
static int test1(struct evsel *evsel, struct machine *machine)
>>>>>>> upstream/android-13
{
	int err;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he;
<<<<<<< HEAD
	struct rb_root *root;
=======
	struct rb_root_cached *root;
>>>>>>> upstream/android-13
	struct rb_node *node;

	field_order = NULL;
	sort_order = NULL; /* equivalent to sort_order = "comm,dso,sym" */

	setup_sorting(NULL);

	/*
	 * expected output:
	 *
	 * Overhead  Command  Shared Object          Symbol
	 * ========  =======  =============  ==============
	 *   20.00%     perf  perf           [.] main
	 *   10.00%     bash  [kernel]       [k] page_fault
	 *   10.00%     bash  bash           [.] main
	 *   10.00%     bash  bash           [.] xmalloc
	 *   10.00%     perf  [kernel]       [k] page_fault
	 *   10.00%     perf  [kernel]       [k] schedule
	 *   10.00%     perf  libc           [.] free
	 *   10.00%     perf  libc           [.] malloc
	 *   10.00%     perf  perf           [.] cmd_record
	 */
	err = add_hist_entries(hists, machine);
	if (err < 0)
		goto out;

	hists__collapse_resort(hists, NULL);
<<<<<<< HEAD
	perf_evsel__output_resort(evsel, NULL);
=======
	evsel__output_resort(evsel, NULL);
>>>>>>> upstream/android-13

	if (verbose > 2) {
		pr_info("[fields = %s, sort = %s]\n", field_order, sort_order);
		print_hists_out(hists);
	}

	root = &hists->entries;
<<<<<<< HEAD
	node = rb_first(root);
=======
	node = rb_first_cached(root);
>>>>>>> upstream/android-13
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "perf") &&
			!strcmp(SYM(he), "main") && he->stat.period == 200);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "[kernel]") &&
			!strcmp(SYM(he), "page_fault") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "bash") &&
			!strcmp(SYM(he), "main") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "bash") &&
			!strcmp(SYM(he), "xmalloc") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "[kernel]") &&
			!strcmp(SYM(he), "page_fault") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "[kernel]") &&
			!strcmp(SYM(he), "schedule") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "libc") &&
			!strcmp(SYM(he), "free") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "libc") &&
			!strcmp(SYM(he), "malloc") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "perf") &&
			!strcmp(SYM(he), "cmd_record") && he->stat.period == 100);

out:
	del_hist_entries(hists);
	reset_output_field();
	return err;
}

/* mixed fields and sort keys */
<<<<<<< HEAD
static int test2(struct perf_evsel *evsel, struct machine *machine)
=======
static int test2(struct evsel *evsel, struct machine *machine)
>>>>>>> upstream/android-13
{
	int err;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he;
<<<<<<< HEAD
	struct rb_root *root;
=======
	struct rb_root_cached *root;
>>>>>>> upstream/android-13
	struct rb_node *node;

	field_order = "overhead,cpu";
	sort_order = "pid";

	setup_sorting(NULL);

	/*
	 * expected output:
	 *
	 * Overhead  CPU  Command:  Pid
	 * ========  ===  =============
	 *   30.00%    1  perf   :  100
	 *   10.00%    0  perf   :  100
	 *   10.00%    2  perf   :  100
	 *   20.00%    2  perf   :  200
	 *   10.00%    0  bash   :  300
	 *   10.00%    1  bash   :  300
	 *   10.00%    3  bash   :  300
	 */
	err = add_hist_entries(hists, machine);
	if (err < 0)
		goto out;

	hists__collapse_resort(hists, NULL);
<<<<<<< HEAD
	perf_evsel__output_resort(evsel, NULL);
=======
	evsel__output_resort(evsel, NULL);
>>>>>>> upstream/android-13

	if (verbose > 2) {
		pr_info("[fields = %s, sort = %s]\n", field_order, sort_order);
		print_hists_out(hists);
	}

	root = &hists->entries;
<<<<<<< HEAD
	node = rb_first(root);
=======
	node = rb_first_cached(root);
>>>>>>> upstream/android-13
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 1 && PID(he) == 100 && he->stat.period == 300);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 0 && PID(he) == 100 && he->stat.period == 100);

out:
	del_hist_entries(hists);
	reset_output_field();
	return err;
}

/* fields only (no sort key) */
<<<<<<< HEAD
static int test3(struct perf_evsel *evsel, struct machine *machine)
=======
static int test3(struct evsel *evsel, struct machine *machine)
>>>>>>> upstream/android-13
{
	int err;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he;
<<<<<<< HEAD
	struct rb_root *root;
=======
	struct rb_root_cached *root;
>>>>>>> upstream/android-13
	struct rb_node *node;

	field_order = "comm,overhead,dso";
	sort_order = NULL;

	setup_sorting(NULL);

	/*
	 * expected output:
	 *
	 * Command  Overhead  Shared Object
	 * =======  ========  =============
	 *    bash    20.00%  bash
	 *    bash    10.00%  [kernel]
	 *    perf    30.00%  perf
	 *    perf    20.00%  [kernel]
	 *    perf    20.00%  libc
	 */
	err = add_hist_entries(hists, machine);
	if (err < 0)
		goto out;

	hists__collapse_resort(hists, NULL);
<<<<<<< HEAD
	perf_evsel__output_resort(evsel, NULL);
=======
	evsel__output_resort(evsel, NULL);
>>>>>>> upstream/android-13

	if (verbose > 2) {
		pr_info("[fields = %s, sort = %s]\n", field_order, sort_order);
		print_hists_out(hists);
	}

	root = &hists->entries;
<<<<<<< HEAD
	node = rb_first(root);
=======
	node = rb_first_cached(root);
>>>>>>> upstream/android-13
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "bash") &&
			he->stat.period == 200);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "[kernel]") &&
			he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "perf") &&
			he->stat.period == 300);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "[kernel]") &&
			he->stat.period == 200);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "libc") &&
			he->stat.period == 200);

out:
	del_hist_entries(hists);
	reset_output_field();
	return err;
}

/* handle duplicate 'dso' field */
<<<<<<< HEAD
static int test4(struct perf_evsel *evsel, struct machine *machine)
=======
static int test4(struct evsel *evsel, struct machine *machine)
>>>>>>> upstream/android-13
{
	int err;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he;
<<<<<<< HEAD
	struct rb_root *root;
=======
	struct rb_root_cached *root;
>>>>>>> upstream/android-13
	struct rb_node *node;

	field_order = "dso,sym,comm,overhead,dso";
	sort_order = "sym";

	setup_sorting(NULL);

	/*
	 * expected output:
	 *
	 * Shared Object          Symbol  Command  Overhead
	 * =============  ==============  =======  ========
	 *          perf  [.] cmd_record     perf    10.00%
	 *          libc  [.] free           perf    10.00%
	 *          bash  [.] main           bash    10.00%
	 *          perf  [.] main           perf    20.00%
	 *          libc  [.] malloc         perf    10.00%
	 *      [kernel]  [k] page_fault     bash    10.00%
	 *      [kernel]  [k] page_fault     perf    10.00%
	 *      [kernel]  [k] schedule       perf    10.00%
	 *          bash  [.] xmalloc        bash    10.00%
	 */
	err = add_hist_entries(hists, machine);
	if (err < 0)
		goto out;

	hists__collapse_resort(hists, NULL);
<<<<<<< HEAD
	perf_evsel__output_resort(evsel, NULL);
=======
	evsel__output_resort(evsel, NULL);
>>>>>>> upstream/android-13

	if (verbose > 2) {
		pr_info("[fields = %s, sort = %s]\n", field_order, sort_order);
		print_hists_out(hists);
	}

	root = &hists->entries;
<<<<<<< HEAD
	node = rb_first(root);
=======
	node = rb_first_cached(root);
>>>>>>> upstream/android-13
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "perf") && !strcmp(SYM(he), "cmd_record") &&
			!strcmp(COMM(he), "perf") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "libc") && !strcmp(SYM(he), "free") &&
			!strcmp(COMM(he), "perf") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "bash") && !strcmp(SYM(he), "main") &&
			!strcmp(COMM(he), "bash") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "perf") && !strcmp(SYM(he), "main") &&
			!strcmp(COMM(he), "perf") && he->stat.period == 200);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "libc") && !strcmp(SYM(he), "malloc") &&
			!strcmp(COMM(he), "perf") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "[kernel]") && !strcmp(SYM(he), "page_fault") &&
			!strcmp(COMM(he), "bash") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "[kernel]") && !strcmp(SYM(he), "page_fault") &&
			!strcmp(COMM(he), "perf") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "[kernel]") && !strcmp(SYM(he), "schedule") &&
			!strcmp(COMM(he), "perf") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			!strcmp(DSO(he), "bash") && !strcmp(SYM(he), "xmalloc") &&
			!strcmp(COMM(he), "bash") && he->stat.period == 100);

out:
	del_hist_entries(hists);
	reset_output_field();
	return err;
}

/* full sort keys w/o overhead field */
<<<<<<< HEAD
static int test5(struct perf_evsel *evsel, struct machine *machine)
=======
static int test5(struct evsel *evsel, struct machine *machine)
>>>>>>> upstream/android-13
{
	int err;
	struct hists *hists = evsel__hists(evsel);
	struct hist_entry *he;
<<<<<<< HEAD
	struct rb_root *root;
=======
	struct rb_root_cached *root;
>>>>>>> upstream/android-13
	struct rb_node *node;

	field_order = "cpu,pid,comm,dso,sym";
	sort_order = "dso,pid";

	setup_sorting(NULL);

	/*
	 * expected output:
	 *
	 * CPU  Command:  Pid  Command  Shared Object          Symbol
	 * ===  =============  =======  =============  ==============
	 *   0     perf:  100     perf       [kernel]  [k] schedule
	 *   2     perf:  200     perf       [kernel]  [k] page_fault
	 *   1     bash:  300     bash       [kernel]  [k] page_fault
	 *   0     bash:  300     bash           bash  [.] xmalloc
	 *   3     bash:  300     bash           bash  [.] main
	 *   1     perf:  100     perf           libc  [.] malloc
	 *   2     perf:  100     perf           libc  [.] free
	 *   1     perf:  100     perf           perf  [.] cmd_record
	 *   1     perf:  100     perf           perf  [.] main
	 *   2     perf:  200     perf           perf  [.] main
	 */
	err = add_hist_entries(hists, machine);
	if (err < 0)
		goto out;

	hists__collapse_resort(hists, NULL);
<<<<<<< HEAD
	perf_evsel__output_resort(evsel, NULL);
=======
	evsel__output_resort(evsel, NULL);
>>>>>>> upstream/android-13

	if (verbose > 2) {
		pr_info("[fields = %s, sort = %s]\n", field_order, sort_order);
		print_hists_out(hists);
	}

	root = &hists->entries;
<<<<<<< HEAD
	node = rb_first(root);
=======
	node = rb_first_cached(root);
>>>>>>> upstream/android-13
	he = rb_entry(node, struct hist_entry, rb_node);

	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 0 && PID(he) == 100 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "[kernel]") &&
			!strcmp(SYM(he), "schedule") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 2 && PID(he) == 200 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "[kernel]") &&
			!strcmp(SYM(he), "page_fault") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 1 && PID(he) == 300 &&
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "[kernel]") &&
			!strcmp(SYM(he), "page_fault") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 0 && PID(he) == 300 &&
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "bash") &&
			!strcmp(SYM(he), "xmalloc") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 3 && PID(he) == 300 &&
			!strcmp(COMM(he), "bash") && !strcmp(DSO(he), "bash") &&
			!strcmp(SYM(he), "main") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 1 && PID(he) == 100 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "libc") &&
			!strcmp(SYM(he), "malloc") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 2 && PID(he) == 100 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "libc") &&
			!strcmp(SYM(he), "free") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 1 && PID(he) == 100 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "perf") &&
			!strcmp(SYM(he), "cmd_record") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 1 && PID(he) == 100 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "perf") &&
			!strcmp(SYM(he), "main") && he->stat.period == 100);

	node = rb_next(node);
	he = rb_entry(node, struct hist_entry, rb_node);
	TEST_ASSERT_VAL("Invalid hist entry",
			CPU(he) == 2 && PID(he) == 200 &&
			!strcmp(COMM(he), "perf") && !strcmp(DSO(he), "perf") &&
			!strcmp(SYM(he), "main") && he->stat.period == 100);

out:
	del_hist_entries(hists);
	reset_output_field();
	return err;
}

int test__hists_output(struct test *test __maybe_unused, int subtest __maybe_unused)
{
	int err = TEST_FAIL;
	struct machines machines;
	struct machine *machine;
<<<<<<< HEAD
	struct perf_evsel *evsel;
	struct perf_evlist *evlist = perf_evlist__new();
=======
	struct evsel *evsel;
	struct evlist *evlist = evlist__new();
>>>>>>> upstream/android-13
	size_t i;
	test_fn_t testcases[] = {
		test1,
		test2,
		test3,
		test4,
		test5,
	};

	TEST_ASSERT_VAL("No memory", evlist);

	err = parse_events(evlist, "cpu-clock", NULL);
	if (err)
		goto out;
	err = TEST_FAIL;

	machines__init(&machines);

	/* setup threads/dso/map/symbols also */
	machine = setup_fake_machine(&machines);
	if (!machine)
		goto out;

	if (verbose > 1)
		machine__fprintf(machine, stderr);

<<<<<<< HEAD
	evsel = perf_evlist__first(evlist);
=======
	evsel = evlist__first(evlist);
>>>>>>> upstream/android-13

	for (i = 0; i < ARRAY_SIZE(testcases); i++) {
		err = testcases[i](evsel, machine);
		if (err < 0)
			break;
	}

out:
	/* tear down everything */
<<<<<<< HEAD
	perf_evlist__delete(evlist);
=======
	evlist__delete(evlist);
>>>>>>> upstream/android-13
	machines__exit(&machines);

	return err;
}
