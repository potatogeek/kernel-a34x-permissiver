<<<<<<< HEAD
/*
 * Copyright (C) 2017-2018 Netronome Systems, Inc.
 *
 * This software is dual licensed under the GNU General License Version 2,
 * June 1991 as shown in the file COPYING in the top-level directory of this
 * source tree or the BSD 2-Clause License provided below.  You have the
 * option to license this software under the complete terms of either license.
 *
 * The BSD 2-Clause License:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      1. Redistributions of source code must retain the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer.
 *
 *      2. Redistributions in binary form must reproduce the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <bfd.h>
=======
// SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause)
/* Copyright (C) 2017-2018 Netronome Systems, Inc. */

>>>>>>> upstream/android-13
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
#include <bpf.h>
=======
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <bpf/btf.h>
>>>>>>> upstream/android-13

#include "main.h"

#define BATCH_LINE_LEN_MAX 65536
#define BATCH_ARG_NB_MAX 4096

const char *bin_name;
static int last_argc;
static char **last_argv;
static int (*last_do_help)(int argc, char **argv);
json_writer_t *json_wtr;
bool pretty_output;
bool json_output;
bool show_pinned;
<<<<<<< HEAD
struct pinned_obj_table prog_table;
struct pinned_obj_table map_table;
=======
bool block_mount;
bool verifier_logs;
bool relaxed_maps;
bool use_loader;
struct btf *base_btf;
struct pinned_obj_table prog_table;
struct pinned_obj_table map_table;
struct pinned_obj_table link_table;
struct obj_refs_table refs_table;
>>>>>>> upstream/android-13

static void __noreturn clean_and_exit(int i)
{
	if (json_output)
		jsonw_destroy(&json_wtr);

	exit(i);
}

void usage(void)
{
	last_do_help(last_argc - 1, last_argv + 1);

	clean_and_exit(-1);
}

static int do_help(int argc, char **argv)
{
	if (json_output) {
		jsonw_null(json_wtr);
		return 0;
	}

	fprintf(stderr,
		"Usage: %s [OPTIONS] OBJECT { COMMAND | help }\n"
		"       %s batch file FILE\n"
		"       %s version\n"
		"\n"
<<<<<<< HEAD
		"       OBJECT := { prog | map | cgroup | perf }\n"
		"       " HELP_SPEC_OPTIONS "\n"
=======
		"       OBJECT := { prog | map | link | cgroup | perf | net | feature | btf | gen | struct_ops | iter }\n"
		"       " HELP_SPEC_OPTIONS " |\n"
		"                    {-V|--version} }\n"
>>>>>>> upstream/android-13
		"",
		bin_name, bin_name, bin_name);

	return 0;
}

static int do_version(int argc, char **argv)
{
<<<<<<< HEAD
	if (json_output) {
		jsonw_start_object(json_wtr);
		jsonw_name(json_wtr, "version");
		jsonw_printf(json_wtr, "\"%s\"", BPFTOOL_VERSION);
		jsonw_end_object(json_wtr);
	} else {
		printf("%s v%s\n", bin_name, BPFTOOL_VERSION);
=======
#ifdef HAVE_LIBBFD_SUPPORT
	const bool has_libbfd = true;
#else
	const bool has_libbfd = false;
#endif
#ifdef BPFTOOL_WITHOUT_SKELETONS
	const bool has_skeletons = false;
#else
	const bool has_skeletons = true;
#endif

	if (json_output) {
		jsonw_start_object(json_wtr);	/* root object */

		jsonw_name(json_wtr, "version");
		jsonw_printf(json_wtr, "\"%s\"", BPFTOOL_VERSION);

		jsonw_name(json_wtr, "features");
		jsonw_start_object(json_wtr);	/* features */
		jsonw_bool_field(json_wtr, "libbfd", has_libbfd);
		jsonw_bool_field(json_wtr, "skeletons", has_skeletons);
		jsonw_end_object(json_wtr);	/* features */

		jsonw_end_object(json_wtr);	/* root object */
	} else {
		unsigned int nb_features = 0;

		printf("%s v%s\n", bin_name, BPFTOOL_VERSION);
		printf("features:");
		if (has_libbfd) {
			printf(" libbfd");
			nb_features++;
		}
		if (has_skeletons)
			printf("%s skeletons", nb_features++ ? "," : "");
		printf("\n");
>>>>>>> upstream/android-13
	}
	return 0;
}

int cmd_select(const struct cmd *cmds, int argc, char **argv,
	       int (*help)(int argc, char **argv))
{
	unsigned int i;

	last_argc = argc;
	last_argv = argv;
	last_do_help = help;

	if (argc < 1 && cmds[0].func)
		return cmds[0].func(argc, argv);

<<<<<<< HEAD
	for (i = 0; cmds[i].func; i++)
		if (is_prefix(*argv, cmds[i].cmd))
			return cmds[i].func(argc - 1, argv + 1);
=======
	for (i = 0; cmds[i].cmd; i++) {
		if (is_prefix(*argv, cmds[i].cmd)) {
			if (!cmds[i].func) {
				p_err("command '%s' is not supported in bootstrap mode",
				      cmds[i].cmd);
				return -1;
			}
			return cmds[i].func(argc - 1, argv + 1);
		}
	}
>>>>>>> upstream/android-13

	help(argc - 1, argv + 1);

	return -1;
}

bool is_prefix(const char *pfx, const char *str)
{
	if (!pfx)
		return false;
	if (strlen(str) < strlen(pfx))
		return false;

	return !memcmp(str, pfx, strlen(pfx));
}

<<<<<<< HEAD
=======
/* Last argument MUST be NULL pointer */
int detect_common_prefix(const char *arg, ...)
{
	unsigned int count = 0;
	const char *ref;
	char msg[256];
	va_list ap;

	snprintf(msg, sizeof(msg), "ambiguous prefix: '%s' could be '", arg);
	va_start(ap, arg);
	while ((ref = va_arg(ap, const char *))) {
		if (!is_prefix(arg, ref))
			continue;
		count++;
		if (count > 1)
			strncat(msg, "' or '", sizeof(msg) - strlen(msg) - 1);
		strncat(msg, ref, sizeof(msg) - strlen(msg) - 1);
	}
	va_end(ap);
	strncat(msg, "'", sizeof(msg) - strlen(msg) - 1);

	if (count >= 2) {
		p_err("%s", msg);
		return -1;
	}

	return 0;
}

>>>>>>> upstream/android-13
void fprint_hex(FILE *f, void *arg, unsigned int n, const char *sep)
{
	unsigned char *data = arg;
	unsigned int i;

	for (i = 0; i < n; i++) {
		const char *pfx = "";

		if (!i)
			/* nothing */;
		else if (!(i % 16))
			fprintf(f, "\n");
		else if (!(i % 8))
			fprintf(f, "  ");
		else
			pfx = sep;

		fprintf(f, "%s%02hhx", i ? pfx : "", data[i]);
	}
}

/* Split command line into argument vector. */
static int make_args(char *line, char *n_argv[], int maxargs, int cmd_nb)
{
	static const char ws[] = " \t\r\n";
	char *cp = line;
	int n_argc = 0;

	while (*cp) {
		/* Skip leading whitespace. */
		cp += strspn(cp, ws);

		if (*cp == '\0')
			break;

		if (n_argc >= (maxargs - 1)) {
			p_err("too many arguments to command %d", cmd_nb);
			return -1;
		}

		/* Word begins with quote. */
		if (*cp == '\'' || *cp == '"') {
			char quote = *cp++;

			n_argv[n_argc++] = cp;
			/* Find ending quote. */
			cp = strchr(cp, quote);
			if (!cp) {
				p_err("unterminated quoted string in command %d",
				      cmd_nb);
				return -1;
			}
		} else {
			n_argv[n_argc++] = cp;

			/* Find end of word. */
			cp += strcspn(cp, ws);
			if (*cp == '\0')
				break;
		}

		/* Separate words. */
		*cp++ = 0;
	}
	n_argv[n_argc] = NULL;

	return n_argc;
}

static int do_batch(int argc, char **argv);

static const struct cmd cmds[] = {
	{ "help",	do_help },
	{ "batch",	do_batch },
	{ "prog",	do_prog },
	{ "map",	do_map },
<<<<<<< HEAD
	{ "cgroup",	do_cgroup },
	{ "perf",	do_perf },
=======
	{ "link",	do_link },
	{ "cgroup",	do_cgroup },
	{ "perf",	do_perf },
	{ "net",	do_net },
	{ "feature",	do_feature },
	{ "btf",	do_btf },
	{ "gen",	do_gen },
	{ "struct_ops",	do_struct_ops },
	{ "iter",	do_iter },
>>>>>>> upstream/android-13
	{ "version",	do_version },
	{ 0 }
};

static int do_batch(int argc, char **argv)
{
	char buf[BATCH_LINE_LEN_MAX], contline[BATCH_LINE_LEN_MAX];
	char *n_argv[BATCH_ARG_NB_MAX];
	unsigned int lines = 0;
	int n_argc;
	FILE *fp;
	char *cp;
<<<<<<< HEAD
	int err;
=======
	int err = 0;
>>>>>>> upstream/android-13
	int i;

	if (argc < 2) {
		p_err("too few parameters for batch");
		return -1;
	} else if (!is_prefix(*argv, "file")) {
		p_err("expected 'file', got: %s", *argv);
		return -1;
	} else if (argc > 2) {
		p_err("too many parameters for batch");
		return -1;
	}
	NEXT_ARG();

	if (!strcmp(*argv, "-"))
		fp = stdin;
	else
		fp = fopen(*argv, "r");
	if (!fp) {
		p_err("Can't open file (%s): %s", *argv, strerror(errno));
		return -1;
	}

	if (json_output)
		jsonw_start_array(json_wtr);
	while (fgets(buf, sizeof(buf), fp)) {
		cp = strchr(buf, '#');
		if (cp)
			*cp = '\0';

		if (strlen(buf) == sizeof(buf) - 1) {
			errno = E2BIG;
			break;
		}

		/* Append continuation lines if any (coming after a line ending
		 * with '\' in the batch file).
		 */
		while ((cp = strstr(buf, "\\\n")) != NULL) {
			if (!fgets(contline, sizeof(contline), fp) ||
			    strlen(contline) == 0) {
				p_err("missing continuation line on command %d",
				      lines);
				err = -1;
				goto err_close;
			}

			cp = strchr(contline, '#');
			if (cp)
				*cp = '\0';

			if (strlen(buf) + strlen(contline) + 1 > sizeof(buf)) {
				p_err("command %d is too long", lines);
				err = -1;
				goto err_close;
			}
			buf[strlen(buf) - 2] = '\0';
			strcat(buf, contline);
		}

		n_argc = make_args(buf, n_argv, BATCH_ARG_NB_MAX, lines);
		if (!n_argc)
			continue;
<<<<<<< HEAD
		if (n_argc < 0)
			goto err_close;
=======
		if (n_argc < 0) {
			err = n_argc;
			goto err_close;
		}
>>>>>>> upstream/android-13

		if (json_output) {
			jsonw_start_object(json_wtr);
			jsonw_name(json_wtr, "command");
			jsonw_start_array(json_wtr);
			for (i = 0; i < n_argc; i++)
				jsonw_string(json_wtr, n_argv[i]);
			jsonw_end_array(json_wtr);
			jsonw_name(json_wtr, "output");
		}

		err = cmd_select(cmds, n_argc, n_argv, do_help);

		if (json_output)
			jsonw_end_object(json_wtr);

		if (err)
			goto err_close;

		lines++;
	}

	if (errno && errno != ENOENT) {
		p_err("reading batch file failed: %s", strerror(errno));
		err = -1;
	} else {
<<<<<<< HEAD
		p_info("processed %d commands", lines);
		err = 0;
=======
		if (!json_output)
			printf("processed %d commands\n", lines);
>>>>>>> upstream/android-13
	}
err_close:
	if (fp != stdin)
		fclose(fp);

	if (json_output)
		jsonw_end_array(json_wtr);

	return err;
}

int main(int argc, char **argv)
{
	static const struct option options[] = {
		{ "json",	no_argument,	NULL,	'j' },
		{ "help",	no_argument,	NULL,	'h' },
		{ "pretty",	no_argument,	NULL,	'p' },
		{ "version",	no_argument,	NULL,	'V' },
		{ "bpffs",	no_argument,	NULL,	'f' },
<<<<<<< HEAD
=======
		{ "mapcompat",	no_argument,	NULL,	'm' },
		{ "nomount",	no_argument,	NULL,	'n' },
		{ "debug",	no_argument,	NULL,	'd' },
		{ "use-loader",	no_argument,	NULL,	'L' },
		{ "base-btf",	required_argument, NULL, 'B' },
>>>>>>> upstream/android-13
		{ 0 }
	};
	int opt, ret;

<<<<<<< HEAD
=======
	setlinebuf(stdout);

>>>>>>> upstream/android-13
	last_do_help = do_help;
	pretty_output = false;
	json_output = false;
	show_pinned = false;
<<<<<<< HEAD
=======
	block_mount = false;
>>>>>>> upstream/android-13
	bin_name = argv[0];

	hash_init(prog_table.table);
	hash_init(map_table.table);
<<<<<<< HEAD

	opterr = 0;
	while ((opt = getopt_long(argc, argv, "Vhpjf",
=======
	hash_init(link_table.table);

	opterr = 0;
	while ((opt = getopt_long(argc, argv, "VhpjfLmndB:",
>>>>>>> upstream/android-13
				  options, NULL)) >= 0) {
		switch (opt) {
		case 'V':
			return do_version(argc, argv);
		case 'h':
			return do_help(argc, argv);
		case 'p':
			pretty_output = true;
			/* fall through */
		case 'j':
			if (!json_output) {
				json_wtr = jsonw_new(stdout);
				if (!json_wtr) {
					p_err("failed to create JSON writer");
					return -1;
				}
				json_output = true;
			}
			jsonw_pretty(json_wtr, pretty_output);
			break;
		case 'f':
			show_pinned = true;
			break;
<<<<<<< HEAD
=======
		case 'm':
			relaxed_maps = true;
			break;
		case 'n':
			block_mount = true;
			break;
		case 'd':
			libbpf_set_print(print_all_levels);
			verifier_logs = true;
			break;
		case 'B':
			base_btf = btf__parse(optarg, NULL);
			if (libbpf_get_error(base_btf)) {
				p_err("failed to parse base BTF at '%s': %ld\n",
				      optarg, libbpf_get_error(base_btf));
				base_btf = NULL;
				return -1;
			}
			break;
		case 'L':
			use_loader = true;
			break;
>>>>>>> upstream/android-13
		default:
			p_err("unrecognized option '%s'", argv[optind - 1]);
			if (json_output)
				clean_and_exit(-1);
			else
				usage();
		}
	}

	argc -= optind;
	argv += optind;
	if (argc < 0)
		usage();

<<<<<<< HEAD
	bfd_init();

=======
>>>>>>> upstream/android-13
	ret = cmd_select(cmds, argc, argv, do_help);

	if (json_output)
		jsonw_destroy(&json_wtr);

	if (show_pinned) {
		delete_pinned_obj_table(&prog_table);
		delete_pinned_obj_table(&map_table);
<<<<<<< HEAD
	}
=======
		delete_pinned_obj_table(&link_table);
	}
	btf__free(base_btf);
>>>>>>> upstream/android-13

	return ret;
}
