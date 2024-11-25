// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
<<<<<<< HEAD
=======
#include <stdio.h>
#include <string.h>
>>>>>>> upstream/android-13
#include "builtin.h"
#include "perf.h"
#include "debug.h"
#include <subcmd/parse-options.h>
#include "data-convert.h"
<<<<<<< HEAD
#include "data-convert-bt.h"
=======
>>>>>>> upstream/android-13

typedef int (*data_cmd_fn_t)(int argc, const char **argv);

struct data_cmd {
	const char	*name;
	const char	*summary;
	data_cmd_fn_t	fn;
};

static struct data_cmd data_cmds[];

#define for_each_cmd(cmd) \
	for (cmd = data_cmds; cmd && cmd->name; cmd++)

<<<<<<< HEAD
static const struct option data_options[] = {
	OPT_END()
};

static const char * const data_subcommands[] = { "convert", NULL };

static const char *data_usage[] = {
	"perf data [<common options>] <command> [<options>]",
	NULL
};

static void print_usage(void)
{
	struct data_cmd *cmd;

	printf("Usage:\n");
	printf("\t%s\n\n", data_usage[0]);
	printf("\tAvailable commands:\n");

	for_each_cmd(cmd) {
		printf("\t %s\t- %s\n", cmd->name, cmd->summary);
	}

	printf("\n");
}

static const char * const data_convert_usage[] = {
=======
static const char * const data_subcommands[] = { "convert", NULL };

static const char *data_usage[] = {
>>>>>>> upstream/android-13
	"perf data convert [<options>]",
	NULL
};

<<<<<<< HEAD
static int cmd_data_convert(int argc, const char **argv)
{
	const char *to_ctf     = NULL;
	struct perf_data_convert_opts opts = {
		.force = false,
		.all = false,
	};
	const struct option options[] = {
		OPT_INCR('v', "verbose", &verbose, "be more verbose"),
		OPT_STRING('i', "input", &input_name, "file", "input file name"),
#ifdef HAVE_LIBBABELTRACE_SUPPORT
		OPT_STRING(0, "to-ctf", &to_ctf, NULL, "Convert to CTF format"),
=======
const char *to_json;
const char *to_ctf;
struct perf_data_convert_opts opts = {
	.force = false,
	.all = false,
};

const struct option data_options[] = {
		OPT_INCR('v', "verbose", &verbose, "be more verbose"),
		OPT_STRING('i', "input", &input_name, "file", "input file name"),
		OPT_STRING(0, "to-json", &to_json, NULL, "Convert to JSON format"),
#ifdef HAVE_LIBBABELTRACE_SUPPORT
		OPT_STRING(0, "to-ctf", &to_ctf, NULL, "Convert to CTF format"),
		OPT_BOOLEAN(0, "tod", &opts.tod, "Convert time to wall clock time"),
>>>>>>> upstream/android-13
#endif
		OPT_BOOLEAN('f', "force", &opts.force, "don't complain, do it"),
		OPT_BOOLEAN(0, "all", &opts.all, "Convert all events"),
		OPT_END()
	};

<<<<<<< HEAD
#ifndef HAVE_LIBBABELTRACE_SUPPORT
	pr_err("No conversion support compiled in. perf should be compiled with environment variables LIBBABELTRACE=1 and LIBBABELTRACE_DIR=/path/to/libbabeltrace/\n");
	return -1;
#endif

	argc = parse_options(argc, argv, options,
			     data_convert_usage, 0);
	if (argc) {
		usage_with_options(data_convert_usage, options);
		return -1;
	}

=======
static int cmd_data_convert(int argc, const char **argv)
{

	argc = parse_options(argc, argv, data_options,
			     data_usage, 0);
	if (argc) {
		usage_with_options(data_usage, data_options);
		return -1;
	}

	if (to_json && to_ctf) {
		pr_err("You cannot specify both --to-ctf and --to-json.\n");
		return -1;
	}
	if (!to_json && !to_ctf) {
		pr_err("You must specify one of --to-ctf or --to-json.\n");
		return -1;
	}

	if (to_json)
		return bt_convert__perf2json(input_name, to_json, &opts);

>>>>>>> upstream/android-13
	if (to_ctf) {
#ifdef HAVE_LIBBABELTRACE_SUPPORT
		return bt_convert__perf2ctf(input_name, to_ctf, &opts);
#else
<<<<<<< HEAD
		pr_err("The libbabeltrace support is not compiled in.\n");
=======
		pr_err("The libbabeltrace support is not compiled in. perf should be "
		       "compiled with environment variables LIBBABELTRACE=1 and "
		       "LIBBABELTRACE_DIR=/path/to/libbabeltrace/\n");
>>>>>>> upstream/android-13
		return -1;
#endif
	}

	return 0;
}

static struct data_cmd data_cmds[] = {
	{ "convert", "converts data file between formats", cmd_data_convert },
	{ .name = NULL, },
};

int cmd_data(int argc, const char **argv)
{
	struct data_cmd *cmd;
	const char *cmdstr;

<<<<<<< HEAD
	/* No command specified. */
	if (argc < 2)
		goto usage;

	argc = parse_options_subcommand(argc, argv, data_options, data_subcommands, data_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	if (argc < 1)
		goto usage;
=======
	argc = parse_options_subcommand(argc, argv, data_options, data_subcommands, data_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	if (!argc) {
		usage_with_options(data_usage, data_options);
		return -1;
	}
>>>>>>> upstream/android-13

	cmdstr = argv[0];

	for_each_cmd(cmd) {
		if (strcmp(cmd->name, cmdstr))
			continue;

		return cmd->fn(argc, argv);
	}

	pr_err("Unknown command: %s\n", cmdstr);
<<<<<<< HEAD
usage:
	print_usage();
=======
	usage_with_options(data_usage, data_options);
>>>>>>> upstream/android-13
	return -1;
}
