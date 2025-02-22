<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * builtin-ftrace.c
 *
 * Copyright (c) 2013  LG Electronics,  Namhyung Kim <namhyung@kernel.org>
<<<<<<< HEAD
 *
 * Released under the GPL v2.
 */

#include "builtin.h"
#include "perf.h"
=======
 * Copyright (c) 2020  Changbin Du <changbin.du@gmail.com>, significant enhancement.
 */

#include "builtin.h"
>>>>>>> upstream/android-13

#include <errno.h>
#include <unistd.h>
#include <signal.h>
<<<<<<< HEAD
#include <fcntl.h>
#include <poll.h>

#include "debug.h"
=======
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <linux/capability.h>
#include <linux/string.h>

#include "debug.h"
#include <subcmd/pager.h>
>>>>>>> upstream/android-13
#include <subcmd/parse-options.h>
#include <api/fs/tracing_path.h>
#include "evlist.h"
#include "target.h"
#include "cpumap.h"
#include "thread_map.h"
<<<<<<< HEAD
#include "util/config.h"

=======
#include "strfilter.h"
#include "util/cap.h"
#include "util/config.h"
#include "util/units.h"
#include "util/parse-sublevel-options.h"
>>>>>>> upstream/android-13

#define DEFAULT_TRACER  "function_graph"

struct perf_ftrace {
<<<<<<< HEAD
	struct perf_evlist	*evlist;
=======
	struct evlist		*evlist;
>>>>>>> upstream/android-13
	struct target		target;
	const char		*tracer;
	struct list_head	filters;
	struct list_head	notrace;
	struct list_head	graph_funcs;
	struct list_head	nograph_funcs;
	int			graph_depth;
<<<<<<< HEAD
=======
	unsigned long		percpu_buffer_size;
	bool			inherit;
	int			func_stack_trace;
	int			func_irq_info;
	int			graph_nosleep_time;
	int			graph_noirqs;
	int			graph_verbose;
	int			graph_thresh;
	unsigned int		initial_delay;
>>>>>>> upstream/android-13
};

struct filter_entry {
	struct list_head	list;
	char			name[];
};

<<<<<<< HEAD
=======
static volatile int workload_exec_errno;
>>>>>>> upstream/android-13
static bool done;

static void sig_handler(int sig __maybe_unused)
{
	done = true;
}

/*
<<<<<<< HEAD
 * perf_evlist__prepare_workload will send a SIGUSR1 if the fork fails, since
=======
 * evlist__prepare_workload will send a SIGUSR1 if the fork fails, since
>>>>>>> upstream/android-13
 * we asked by setting its exec_error to the function below,
 * ftrace__workload_exec_failed_signal.
 *
 * XXX We need to handle this more appropriately, emitting an error, etc.
 */
static void ftrace__workload_exec_failed_signal(int signo __maybe_unused,
						siginfo_t *info __maybe_unused,
						void *ucontext __maybe_unused)
{
<<<<<<< HEAD
	/* workload_exec_errno = info->si_value.sival_int; */
=======
	workload_exec_errno = info->si_value.sival_int;
>>>>>>> upstream/android-13
	done = true;
}

static int __write_tracing_file(const char *name, const char *val, bool append)
{
	char *file;
	int fd, ret = -1;
	ssize_t size = strlen(val);
	int flags = O_WRONLY;
	char errbuf[512];
	char *val_copy;

	file = get_tracing_file(name);
	if (!file) {
		pr_debug("cannot get tracing file: %s\n", name);
		return -1;
	}

	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;

	fd = open(file, flags);
	if (fd < 0) {
		pr_debug("cannot open tracing file: %s: %s\n",
			 name, str_error_r(errno, errbuf, sizeof(errbuf)));
		goto out;
	}

	/*
	 * Copy the original value and append a '\n'. Without this,
	 * the kernel can hide possible errors.
	 */
	val_copy = strdup(val);
	if (!val_copy)
		goto out_close;
	val_copy[size] = '\n';

	if (write(fd, val_copy, size + 1) == size + 1)
		ret = 0;
	else
		pr_debug("write '%s' to tracing/%s failed: %s\n",
			 val, name, str_error_r(errno, errbuf, sizeof(errbuf)));

	free(val_copy);
out_close:
	close(fd);
out:
	put_tracing_file(file);
	return ret;
}

static int write_tracing_file(const char *name, const char *val)
{
	return __write_tracing_file(name, val, false);
}

static int append_tracing_file(const char *name, const char *val)
{
	return __write_tracing_file(name, val, true);
}

<<<<<<< HEAD
static int reset_tracing_cpu(void);
static void reset_tracing_filters(void);

=======
static int read_tracing_file_to_stdout(const char *name)
{
	char buf[4096];
	char *file;
	int fd;
	int ret = -1;

	file = get_tracing_file(name);
	if (!file) {
		pr_debug("cannot get tracing file: %s\n", name);
		return -1;
	}

	fd = open(file, O_RDONLY);
	if (fd < 0) {
		pr_debug("cannot open tracing file: %s: %s\n",
			 name, str_error_r(errno, buf, sizeof(buf)));
		goto out;
	}

	/* read contents to stdout */
	while (true) {
		int n = read(fd, buf, sizeof(buf));
		if (n == 0)
			break;
		else if (n < 0)
			goto out_close;

		if (fwrite(buf, n, 1, stdout) != 1)
			goto out_close;
	}
	ret = 0;

out_close:
	close(fd);
out:
	put_tracing_file(file);
	return ret;
}

static int read_tracing_file_by_line(const char *name,
				     void (*cb)(char *str, void *arg),
				     void *cb_arg)
{
	char *line = NULL;
	size_t len = 0;
	char *file;
	FILE *fp;

	file = get_tracing_file(name);
	if (!file) {
		pr_debug("cannot get tracing file: %s\n", name);
		return -1;
	}

	fp = fopen(file, "r");
	if (fp == NULL) {
		pr_debug("cannot open tracing file: %s\n", name);
		put_tracing_file(file);
		return -1;
	}

	while (getline(&line, &len, fp) != -1) {
		cb(line, cb_arg);
	}

	if (line)
		free(line);

	fclose(fp);
	put_tracing_file(file);
	return 0;
}

static int write_tracing_file_int(const char *name, int value)
{
	char buf[16];

	snprintf(buf, sizeof(buf), "%d", value);
	if (write_tracing_file(name, buf) < 0)
		return -1;

	return 0;
}

static int write_tracing_option_file(const char *name, const char *val)
{
	char *file;
	int ret;

	if (asprintf(&file, "options/%s", name) < 0)
		return -1;

	ret = __write_tracing_file(file, val, false);
	free(file);
	return ret;
}

static int reset_tracing_cpu(void);
static void reset_tracing_filters(void);

static void reset_tracing_options(struct perf_ftrace *ftrace __maybe_unused)
{
	write_tracing_option_file("function-fork", "0");
	write_tracing_option_file("func_stack_trace", "0");
	write_tracing_option_file("sleep-time", "1");
	write_tracing_option_file("funcgraph-irqs", "1");
	write_tracing_option_file("funcgraph-proc", "0");
	write_tracing_option_file("funcgraph-abstime", "0");
	write_tracing_option_file("latency-format", "0");
	write_tracing_option_file("irq-info", "0");
}

>>>>>>> upstream/android-13
static int reset_tracing_files(struct perf_ftrace *ftrace __maybe_unused)
{
	if (write_tracing_file("tracing_on", "0") < 0)
		return -1;

	if (write_tracing_file("current_tracer", "nop") < 0)
		return -1;

	if (write_tracing_file("set_ftrace_pid", " ") < 0)
		return -1;

	if (reset_tracing_cpu() < 0)
		return -1;

	if (write_tracing_file("max_graph_depth", "0") < 0)
		return -1;

<<<<<<< HEAD
	reset_tracing_filters();
=======
	if (write_tracing_file("tracing_thresh", "0") < 0)
		return -1;

	reset_tracing_filters();
	reset_tracing_options(ftrace);
>>>>>>> upstream/android-13
	return 0;
}

static int set_tracing_pid(struct perf_ftrace *ftrace)
{
	int i;
	char buf[16];

	if (target__has_cpu(&ftrace->target))
		return 0;

<<<<<<< HEAD
	for (i = 0; i < thread_map__nr(ftrace->evlist->threads); i++) {
		scnprintf(buf, sizeof(buf), "%d",
			  ftrace->evlist->threads->map[i]);
=======
	for (i = 0; i < perf_thread_map__nr(ftrace->evlist->core.threads); i++) {
		scnprintf(buf, sizeof(buf), "%d",
			  perf_thread_map__pid(ftrace->evlist->core.threads, i));
>>>>>>> upstream/android-13
		if (append_tracing_file("set_ftrace_pid", buf) < 0)
			return -1;
	}
	return 0;
}

<<<<<<< HEAD
static int set_tracing_cpumask(struct cpu_map *cpumap)
=======
static int set_tracing_cpumask(struct perf_cpu_map *cpumap)
>>>>>>> upstream/android-13
{
	char *cpumask;
	size_t mask_size;
	int ret;
	int last_cpu;

	last_cpu = cpu_map__cpu(cpumap, cpumap->nr - 1);
	mask_size = last_cpu / 4 + 2; /* one more byte for EOS */
	mask_size += last_cpu / 32; /* ',' is needed for every 32th cpus */

	cpumask = malloc(mask_size);
	if (cpumask == NULL) {
		pr_debug("failed to allocate cpu mask\n");
		return -1;
	}

	cpu_map__snprint_mask(cpumap, cpumask, mask_size);

	ret = write_tracing_file("tracing_cpumask", cpumask);

	free(cpumask);
	return ret;
}

static int set_tracing_cpu(struct perf_ftrace *ftrace)
{
<<<<<<< HEAD
	struct cpu_map *cpumap = ftrace->evlist->cpus;
=======
	struct perf_cpu_map *cpumap = ftrace->evlist->core.cpus;
>>>>>>> upstream/android-13

	if (!target__has_cpu(&ftrace->target))
		return 0;

	return set_tracing_cpumask(cpumap);
}

<<<<<<< HEAD
static int reset_tracing_cpu(void)
{
	struct cpu_map *cpumap = cpu_map__new(NULL);
	int ret;

	ret = set_tracing_cpumask(cpumap);
	cpu_map__put(cpumap);
=======
static int set_tracing_func_stack_trace(struct perf_ftrace *ftrace)
{
	if (!ftrace->func_stack_trace)
		return 0;

	if (write_tracing_option_file("func_stack_trace", "1") < 0)
		return -1;

	return 0;
}

static int set_tracing_func_irqinfo(struct perf_ftrace *ftrace)
{
	if (!ftrace->func_irq_info)
		return 0;

	if (write_tracing_option_file("irq-info", "1") < 0)
		return -1;

	return 0;
}

static int reset_tracing_cpu(void)
{
	struct perf_cpu_map *cpumap = perf_cpu_map__new(NULL);
	int ret;

	ret = set_tracing_cpumask(cpumap);
	perf_cpu_map__put(cpumap);
>>>>>>> upstream/android-13
	return ret;
}

static int __set_tracing_filter(const char *filter_file, struct list_head *funcs)
{
	struct filter_entry *pos;

	list_for_each_entry(pos, funcs, list) {
		if (append_tracing_file(filter_file, pos->name) < 0)
			return -1;
	}

	return 0;
}

static int set_tracing_filters(struct perf_ftrace *ftrace)
{
	int ret;

	ret = __set_tracing_filter("set_ftrace_filter", &ftrace->filters);
	if (ret < 0)
		return ret;

	ret = __set_tracing_filter("set_ftrace_notrace", &ftrace->notrace);
	if (ret < 0)
		return ret;

	ret = __set_tracing_filter("set_graph_function", &ftrace->graph_funcs);
	if (ret < 0)
		return ret;

	/* old kernels do not have this filter */
	__set_tracing_filter("set_graph_notrace", &ftrace->nograph_funcs);

	return ret;
}

static void reset_tracing_filters(void)
{
	write_tracing_file("set_ftrace_filter", " ");
	write_tracing_file("set_ftrace_notrace", " ");
	write_tracing_file("set_graph_function", " ");
	write_tracing_file("set_graph_notrace", " ");
}

static int set_tracing_depth(struct perf_ftrace *ftrace)
{
<<<<<<< HEAD
	char buf[16];

=======
>>>>>>> upstream/android-13
	if (ftrace->graph_depth == 0)
		return 0;

	if (ftrace->graph_depth < 0) {
		pr_err("invalid graph depth: %d\n", ftrace->graph_depth);
		return -1;
	}

<<<<<<< HEAD
	snprintf(buf, sizeof(buf), "%d", ftrace->graph_depth);

	if (write_tracing_file("max_graph_depth", buf) < 0)
=======
	if (write_tracing_file_int("max_graph_depth", ftrace->graph_depth) < 0)
>>>>>>> upstream/android-13
		return -1;

	return 0;
}

<<<<<<< HEAD
=======
static int set_tracing_percpu_buffer_size(struct perf_ftrace *ftrace)
{
	int ret;

	if (ftrace->percpu_buffer_size == 0)
		return 0;

	ret = write_tracing_file_int("buffer_size_kb",
				     ftrace->percpu_buffer_size / 1024);
	if (ret < 0)
		return ret;

	return 0;
}

static int set_tracing_trace_inherit(struct perf_ftrace *ftrace)
{
	if (!ftrace->inherit)
		return 0;

	if (write_tracing_option_file("function-fork", "1") < 0)
		return -1;

	return 0;
}

static int set_tracing_sleep_time(struct perf_ftrace *ftrace)
{
	if (!ftrace->graph_nosleep_time)
		return 0;

	if (write_tracing_option_file("sleep-time", "0") < 0)
		return -1;

	return 0;
}

static int set_tracing_funcgraph_irqs(struct perf_ftrace *ftrace)
{
	if (!ftrace->graph_noirqs)
		return 0;

	if (write_tracing_option_file("funcgraph-irqs", "0") < 0)
		return -1;

	return 0;
}

static int set_tracing_funcgraph_verbose(struct perf_ftrace *ftrace)
{
	if (!ftrace->graph_verbose)
		return 0;

	if (write_tracing_option_file("funcgraph-proc", "1") < 0)
		return -1;

	if (write_tracing_option_file("funcgraph-abstime", "1") < 0)
		return -1;

	if (write_tracing_option_file("latency-format", "1") < 0)
		return -1;

	return 0;
}

static int set_tracing_thresh(struct perf_ftrace *ftrace)
{
	int ret;

	if (ftrace->graph_thresh == 0)
		return 0;

	ret = write_tracing_file_int("tracing_thresh", ftrace->graph_thresh);
	if (ret < 0)
		return ret;

	return 0;
}

static int set_tracing_options(struct perf_ftrace *ftrace)
{
	if (set_tracing_pid(ftrace) < 0) {
		pr_err("failed to set ftrace pid\n");
		return -1;
	}

	if (set_tracing_cpu(ftrace) < 0) {
		pr_err("failed to set tracing cpumask\n");
		return -1;
	}

	if (set_tracing_func_stack_trace(ftrace) < 0) {
		pr_err("failed to set tracing option func_stack_trace\n");
		return -1;
	}

	if (set_tracing_func_irqinfo(ftrace) < 0) {
		pr_err("failed to set tracing option irq-info\n");
		return -1;
	}

	if (set_tracing_filters(ftrace) < 0) {
		pr_err("failed to set tracing filters\n");
		return -1;
	}

	if (set_tracing_depth(ftrace) < 0) {
		pr_err("failed to set graph depth\n");
		return -1;
	}

	if (set_tracing_percpu_buffer_size(ftrace) < 0) {
		pr_err("failed to set tracing per-cpu buffer size\n");
		return -1;
	}

	if (set_tracing_trace_inherit(ftrace) < 0) {
		pr_err("failed to set tracing option function-fork\n");
		return -1;
	}

	if (set_tracing_sleep_time(ftrace) < 0) {
		pr_err("failed to set tracing option sleep-time\n");
		return -1;
	}

	if (set_tracing_funcgraph_irqs(ftrace) < 0) {
		pr_err("failed to set tracing option funcgraph-irqs\n");
		return -1;
	}

	if (set_tracing_funcgraph_verbose(ftrace) < 0) {
		pr_err("failed to set tracing option funcgraph-proc/funcgraph-abstime\n");
		return -1;
	}

	if (set_tracing_thresh(ftrace) < 0) {
		pr_err("failed to set tracing thresh\n");
		return -1;
	}

	return 0;
}

>>>>>>> upstream/android-13
static int __cmd_ftrace(struct perf_ftrace *ftrace, int argc, const char **argv)
{
	char *trace_file;
	int trace_fd;
	char buf[4096];
	struct pollfd pollfd = {
		.events = POLLIN,
	};

<<<<<<< HEAD
	if (geteuid() != 0) {
		pr_err("ftrace only works for root!\n");
=======
	if (!(perf_cap__capable(CAP_PERFMON) ||
	      perf_cap__capable(CAP_SYS_ADMIN))) {
		pr_err("ftrace only works for %s!\n",
#ifdef HAVE_LIBCAP_SUPPORT
		"users with the CAP_PERFMON or CAP_SYS_ADMIN capability"
#else
		"root"
#endif
		);
>>>>>>> upstream/android-13
		return -1;
	}

	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGCHLD, sig_handler);
	signal(SIGPIPE, sig_handler);

	if (reset_tracing_files(ftrace) < 0) {
		pr_err("failed to reset ftrace\n");
		goto out;
	}

	/* reset ftrace buffer */
	if (write_tracing_file("trace", "0") < 0)
		goto out;

<<<<<<< HEAD
	if (argc && perf_evlist__prepare_workload(ftrace->evlist,
				&ftrace->target, argv, false,
				ftrace__workload_exec_failed_signal) < 0) {
		goto out;
	}

	if (set_tracing_pid(ftrace) < 0) {
		pr_err("failed to set ftrace pid\n");
		goto out_reset;
	}

	if (set_tracing_cpu(ftrace) < 0) {
		pr_err("failed to set tracing cpumask\n");
		goto out_reset;
	}

	if (set_tracing_filters(ftrace) < 0) {
		pr_err("failed to set tracing filters\n");
		goto out_reset;
	}

	if (set_tracing_depth(ftrace) < 0) {
		pr_err("failed to set graph depth\n");
		goto out_reset;
	}
=======
	if (argc && evlist__prepare_workload(ftrace->evlist, &ftrace->target, argv, false,
					     ftrace__workload_exec_failed_signal) < 0) {
		goto out;
	}

	if (set_tracing_options(ftrace) < 0)
		goto out_reset;
>>>>>>> upstream/android-13

	if (write_tracing_file("current_tracer", ftrace->tracer) < 0) {
		pr_err("failed to set current_tracer to %s\n", ftrace->tracer);
		goto out_reset;
	}

	setup_pager();

	trace_file = get_tracing_file("trace_pipe");
	if (!trace_file) {
		pr_err("failed to open trace_pipe\n");
		goto out_reset;
	}

	trace_fd = open(trace_file, O_RDONLY);

	put_tracing_file(trace_file);

	if (trace_fd < 0) {
		pr_err("failed to open trace_pipe\n");
		goto out_reset;
	}

	fcntl(trace_fd, F_SETFL, O_NONBLOCK);
	pollfd.fd = trace_fd;

<<<<<<< HEAD
	if (write_tracing_file("tracing_on", "1") < 0) {
		pr_err("can't enable tracing\n");
		goto out_close_fd;
	}

	perf_evlist__start_workload(ftrace->evlist);
=======
	/* display column headers */
	read_tracing_file_to_stdout("trace");

	if (!ftrace->initial_delay) {
		if (write_tracing_file("tracing_on", "1") < 0) {
			pr_err("can't enable tracing\n");
			goto out_close_fd;
		}
	}

	evlist__start_workload(ftrace->evlist);

	if (ftrace->initial_delay) {
		usleep(ftrace->initial_delay * 1000);
		if (write_tracing_file("tracing_on", "1") < 0) {
			pr_err("can't enable tracing\n");
			goto out_close_fd;
		}
	}
>>>>>>> upstream/android-13

	while (!done) {
		if (poll(&pollfd, 1, -1) < 0)
			break;

		if (pollfd.revents & POLLIN) {
			int n = read(trace_fd, buf, sizeof(buf));
			if (n < 0)
				break;
			if (fwrite(buf, n, 1, stdout) != 1)
				break;
		}
	}

	write_tracing_file("tracing_on", "0");

<<<<<<< HEAD
=======
	if (workload_exec_errno) {
		const char *emsg = str_error_r(workload_exec_errno, buf, sizeof(buf));
		/* flush stdout first so below error msg appears at the end. */
		fflush(stdout);
		pr_err("workload failed: %s\n", emsg);
		goto out_close_fd;
	}

>>>>>>> upstream/android-13
	/* read remaining buffer contents */
	while (true) {
		int n = read(trace_fd, buf, sizeof(buf));
		if (n <= 0)
			break;
		if (fwrite(buf, n, 1, stdout) != 1)
			break;
	}

out_close_fd:
	close(trace_fd);
out_reset:
	reset_tracing_files(ftrace);
out:
<<<<<<< HEAD
	return done ? 0 : -1;
=======
	return (done && !workload_exec_errno) ? 0 : -1;
>>>>>>> upstream/android-13
}

static int perf_ftrace_config(const char *var, const char *value, void *cb)
{
	struct perf_ftrace *ftrace = cb;

	if (!strstarts(var, "ftrace."))
		return 0;

	if (strcmp(var, "ftrace.tracer"))
		return -1;

	if (!strcmp(value, "function_graph") ||
	    !strcmp(value, "function")) {
		ftrace->tracer = value;
		return 0;
	}

	pr_err("Please select \"function_graph\" (default) or \"function\"\n");
	return -1;
}

<<<<<<< HEAD
=======
static void list_function_cb(char *str, void *arg)
{
	struct strfilter *filter = (struct strfilter *)arg;

	if (strfilter__compare(filter, str))
		printf("%s", str);
}

static int opt_list_avail_functions(const struct option *opt __maybe_unused,
				    const char *str, int unset)
{
	struct strfilter *filter;
	const char *err = NULL;
	int ret;

	if (unset || !str)
		return -1;

	filter = strfilter__new(str, &err);
	if (!filter)
		return err ? -EINVAL : -ENOMEM;

	ret = strfilter__or(filter, str, &err);
	if (ret == -EINVAL) {
		pr_err("Filter parse error at %td.\n", err - str + 1);
		pr_err("Source: \"%s\"\n", str);
		pr_err("         %*c\n", (int)(err - str + 1), '^');
		strfilter__delete(filter);
		return ret;
	}

	ret = read_tracing_file_by_line("available_filter_functions",
					list_function_cb, filter);
	strfilter__delete(filter);
	if (ret < 0)
		return ret;

	exit(0);
}

>>>>>>> upstream/android-13
static int parse_filter_func(const struct option *opt, const char *str,
			     int unset __maybe_unused)
{
	struct list_head *head = opt->value;
	struct filter_entry *entry;

	entry = malloc(sizeof(*entry) + strlen(str) + 1);
	if (entry == NULL)
		return -ENOMEM;

	strcpy(entry->name, str);
	list_add_tail(&entry->list, head);

	return 0;
}

static void delete_filter_func(struct list_head *head)
{
	struct filter_entry *pos, *tmp;

	list_for_each_entry_safe(pos, tmp, head, list) {
<<<<<<< HEAD
		list_del(&pos->list);
=======
		list_del_init(&pos->list);
>>>>>>> upstream/android-13
		free(pos);
	}
}

<<<<<<< HEAD
=======
static int parse_buffer_size(const struct option *opt,
			     const char *str, int unset)
{
	unsigned long *s = (unsigned long *)opt->value;
	static struct parse_tag tags_size[] = {
		{ .tag  = 'B', .mult = 1       },
		{ .tag  = 'K', .mult = 1 << 10 },
		{ .tag  = 'M', .mult = 1 << 20 },
		{ .tag  = 'G', .mult = 1 << 30 },
		{ .tag  = 0 },
	};
	unsigned long val;

	if (unset) {
		*s = 0;
		return 0;
	}

	val = parse_tag_value(str, tags_size);
	if (val != (unsigned long) -1) {
		if (val < 1024) {
			pr_err("buffer size too small, must larger than 1KB.");
			return -1;
		}
		*s = val;
		return 0;
	}

	return -1;
}

static int parse_func_tracer_opts(const struct option *opt,
				  const char *str, int unset)
{
	int ret;
	struct perf_ftrace *ftrace = (struct perf_ftrace *) opt->value;
	struct sublevel_option func_tracer_opts[] = {
		{ .name = "call-graph",	.value_ptr = &ftrace->func_stack_trace },
		{ .name = "irq-info",	.value_ptr = &ftrace->func_irq_info },
		{ .name = NULL, }
	};

	if (unset)
		return 0;

	ret = perf_parse_sublevel_options(str, func_tracer_opts);
	if (ret)
		return ret;

	return 0;
}

static int parse_graph_tracer_opts(const struct option *opt,
				  const char *str, int unset)
{
	int ret;
	struct perf_ftrace *ftrace = (struct perf_ftrace *) opt->value;
	struct sublevel_option graph_tracer_opts[] = {
		{ .name = "nosleep-time",	.value_ptr = &ftrace->graph_nosleep_time },
		{ .name = "noirqs",		.value_ptr = &ftrace->graph_noirqs },
		{ .name = "verbose",		.value_ptr = &ftrace->graph_verbose },
		{ .name = "thresh",		.value_ptr = &ftrace->graph_thresh },
		{ .name = "depth",		.value_ptr = &ftrace->graph_depth },
		{ .name = NULL, }
	};

	if (unset)
		return 0;

	ret = perf_parse_sublevel_options(str, graph_tracer_opts);
	if (ret)
		return ret;

	return 0;
}

static void select_tracer(struct perf_ftrace *ftrace)
{
	bool graph = !list_empty(&ftrace->graph_funcs) ||
		     !list_empty(&ftrace->nograph_funcs);
	bool func = !list_empty(&ftrace->filters) ||
		    !list_empty(&ftrace->notrace);

	/* The function_graph has priority over function tracer. */
	if (graph)
		ftrace->tracer = "function_graph";
	else if (func)
		ftrace->tracer = "function";
	/* Otherwise, the default tracer is used. */

	pr_debug("%s tracer is used\n", ftrace->tracer);
}

>>>>>>> upstream/android-13
int cmd_ftrace(int argc, const char **argv)
{
	int ret;
	struct perf_ftrace ftrace = {
		.tracer = DEFAULT_TRACER,
		.target = { .uid = UINT_MAX, },
	};
	const char * const ftrace_usage[] = {
		"perf ftrace [<options>] [<command>]",
		"perf ftrace [<options>] -- <command> [<options>]",
		NULL
	};
	const struct option ftrace_options[] = {
	OPT_STRING('t', "tracer", &ftrace.tracer, "tracer",
<<<<<<< HEAD
		   "tracer to use: function_graph(default) or function"),
	OPT_STRING('p', "pid", &ftrace.target.pid, "pid",
		   "trace on existing process id"),
	OPT_INCR('v', "verbose", &verbose,
		 "be more verbose"),
	OPT_BOOLEAN('a', "all-cpus", &ftrace.target.system_wide,
		    "system-wide collection from all CPUs"),
	OPT_STRING('C', "cpu", &ftrace.target.cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_CALLBACK('T', "trace-funcs", &ftrace.filters, "func",
		     "trace given functions only", parse_filter_func),
	OPT_CALLBACK('N', "notrace-funcs", &ftrace.notrace, "func",
		     "do not trace given functions", parse_filter_func),
	OPT_CALLBACK('G', "graph-funcs", &ftrace.graph_funcs, "func",
		     "Set graph filter on given functions", parse_filter_func),
	OPT_CALLBACK('g', "nograph-funcs", &ftrace.nograph_funcs, "func",
		     "Set nograph filter on given functions", parse_filter_func),
	OPT_INTEGER('D', "graph-depth", &ftrace.graph_depth,
		    "Max depth for function graph tracer"),
=======
		   "Tracer to use: function_graph(default) or function"),
	OPT_CALLBACK_DEFAULT('F', "funcs", NULL, "[FILTER]",
			     "Show available functions to filter",
			     opt_list_avail_functions, "*"),
	OPT_STRING('p', "pid", &ftrace.target.pid, "pid",
		   "Trace on existing process id"),
	/* TODO: Add short option -t after -t/--tracer can be removed. */
	OPT_STRING(0, "tid", &ftrace.target.tid, "tid",
		   "Trace on existing thread id (exclusive to --pid)"),
	OPT_INCR('v', "verbose", &verbose,
		 "Be more verbose"),
	OPT_BOOLEAN('a', "all-cpus", &ftrace.target.system_wide,
		    "System-wide collection from all CPUs"),
	OPT_STRING('C', "cpu", &ftrace.target.cpu_list, "cpu",
		    "List of cpus to monitor"),
	OPT_CALLBACK('T', "trace-funcs", &ftrace.filters, "func",
		     "Trace given functions using function tracer",
		     parse_filter_func),
	OPT_CALLBACK('N', "notrace-funcs", &ftrace.notrace, "func",
		     "Do not trace given functions", parse_filter_func),
	OPT_CALLBACK(0, "func-opts", &ftrace, "options",
		     "Function tracer options, available options: call-graph,irq-info",
		     parse_func_tracer_opts),
	OPT_CALLBACK('G', "graph-funcs", &ftrace.graph_funcs, "func",
		     "Trace given functions using function_graph tracer",
		     parse_filter_func),
	OPT_CALLBACK('g', "nograph-funcs", &ftrace.nograph_funcs, "func",
		     "Set nograph filter on given functions", parse_filter_func),
	OPT_CALLBACK(0, "graph-opts", &ftrace, "options",
		     "Graph tracer options, available options: nosleep-time,noirqs,verbose,thresh=<n>,depth=<n>",
		     parse_graph_tracer_opts),
	OPT_CALLBACK('m', "buffer-size", &ftrace.percpu_buffer_size, "size",
		     "Size of per cpu buffer, needs to use a B, K, M or G suffix.", parse_buffer_size),
	OPT_BOOLEAN(0, "inherit", &ftrace.inherit,
		    "Trace children processes"),
	OPT_UINTEGER('D', "delay", &ftrace.initial_delay,
		     "Number of milliseconds to wait before starting tracing after program start"),
>>>>>>> upstream/android-13
	OPT_END()
	};

	INIT_LIST_HEAD(&ftrace.filters);
	INIT_LIST_HEAD(&ftrace.notrace);
	INIT_LIST_HEAD(&ftrace.graph_funcs);
	INIT_LIST_HEAD(&ftrace.nograph_funcs);

	ret = perf_config(perf_ftrace_config, &ftrace);
	if (ret < 0)
		return -1;

	argc = parse_options(argc, argv, ftrace_options, ftrace_usage,
			    PARSE_OPT_STOP_AT_NON_OPTION);
	if (!argc && target__none(&ftrace.target))
<<<<<<< HEAD
		usage_with_options(ftrace_usage, ftrace_options);
=======
		ftrace.target.system_wide = true;

	select_tracer(&ftrace);
>>>>>>> upstream/android-13

	ret = target__validate(&ftrace.target);
	if (ret) {
		char errbuf[512];

		target__strerror(&ftrace.target, ret, errbuf, 512);
		pr_err("%s\n", errbuf);
		goto out_delete_filters;
	}

<<<<<<< HEAD
	ftrace.evlist = perf_evlist__new();
=======
	ftrace.evlist = evlist__new();
>>>>>>> upstream/android-13
	if (ftrace.evlist == NULL) {
		ret = -ENOMEM;
		goto out_delete_filters;
	}

<<<<<<< HEAD
	ret = perf_evlist__create_maps(ftrace.evlist, &ftrace.target);
=======
	ret = evlist__create_maps(ftrace.evlist, &ftrace.target);
>>>>>>> upstream/android-13
	if (ret < 0)
		goto out_delete_evlist;

	ret = __cmd_ftrace(&ftrace, argc, argv);

out_delete_evlist:
<<<<<<< HEAD
	perf_evlist__delete(ftrace.evlist);
=======
	evlist__delete(ftrace.evlist);
>>>>>>> upstream/android-13

out_delete_filters:
	delete_filter_func(&ftrace.filters);
	delete_filter_func(&ftrace.notrace);
	delete_filter_func(&ftrace.graph_funcs);
	delete_filter_func(&ftrace.nograph_funcs);

	return ret;
}
