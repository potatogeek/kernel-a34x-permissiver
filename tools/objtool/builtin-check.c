<<<<<<< HEAD
/*
 * Copyright (C) 2015-2017 Josh Poimboeuf <jpoimboe@redhat.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2015-2017 Josh Poimboeuf <jpoimboe@redhat.com>
>>>>>>> upstream/android-13
 */

/*
 * objtool check:
 *
 * This command analyzes every .o file and ensures the validity of its stack
 * trace metadata.  It enforces a set of rules on asm code and C inline
 * assembly code so that stack traces can be reliable.
 *
 * For more information, see tools/objtool/Documentation/stack-validation.txt.
 */

#include <subcmd/parse-options.h>
<<<<<<< HEAD
#include "builtin.h"
#include "check.h"

bool no_fp, no_unreachable, retpoline, module;
=======
#include <string.h>
#include <stdlib.h>
#include <objtool/builtin.h>
#include <objtool/objtool.h>

bool no_fp, no_unreachable, retpoline, module, backtrace, uaccess, stats,
     validate_dup, vmlinux, mcount, noinstr, backup, sls;
>>>>>>> upstream/android-13

static const char * const check_usage[] = {
	"objtool check [<options>] file.o",
	NULL,
};

<<<<<<< HEAD
=======
static const char * const env_usage[] = {
	"OBJTOOL_ARGS=\"<options>\"",
	NULL,
};

>>>>>>> upstream/android-13
const struct option check_options[] = {
	OPT_BOOLEAN('f', "no-fp", &no_fp, "Skip frame pointer validation"),
	OPT_BOOLEAN('u', "no-unreachable", &no_unreachable, "Skip 'unreachable instruction' warnings"),
	OPT_BOOLEAN('r', "retpoline", &retpoline, "Validate retpoline assumptions"),
	OPT_BOOLEAN('m', "module", &module, "Indicates the object will be part of a kernel module"),
<<<<<<< HEAD
	OPT_END(),
};

int cmd_check(int argc, const char **argv)
{
	const char *objname;

	argc = parse_options(argc, argv, check_options, check_usage, 0);

	if (argc != 1)
		usage_with_options(check_usage, check_options);

	objname = argv[0];

	return check(objname, false);
=======
	OPT_BOOLEAN('b', "backtrace", &backtrace, "unwind on error"),
	OPT_BOOLEAN('a', "uaccess", &uaccess, "enable uaccess checking"),
	OPT_BOOLEAN('s', "stats", &stats, "print statistics"),
	OPT_BOOLEAN('d', "duplicate", &validate_dup, "duplicate validation for vmlinux.o"),
	OPT_BOOLEAN('n', "noinstr", &noinstr, "noinstr validation for vmlinux.o"),
	OPT_BOOLEAN('l', "vmlinux", &vmlinux, "vmlinux.o validation"),
	OPT_BOOLEAN('M', "mcount", &mcount, "generate __mcount_loc"),
	OPT_BOOLEAN('B', "backup", &backup, "create .orig files before modification"),
	OPT_BOOLEAN('S', "sls", &sls, "validate straight-line-speculation"),
	OPT_END(),
};

int cmd_parse_options(int argc, const char **argv, const char * const usage[])
{
	const char *envv[16] = { };
	char *env;
	int envc;

	env = getenv("OBJTOOL_ARGS");
	if (env) {
		envv[0] = "OBJTOOL_ARGS";
		for (envc = 1; envc < ARRAY_SIZE(envv); ) {
			envv[envc++] = env;
			env = strchr(env, ' ');
			if (!env)
				break;
			*env = '\0';
			env++;
		}

		parse_options(envc, envv, check_options, env_usage, 0);
	}

	argc = parse_options(argc, argv, check_options, usage, 0);
	if (argc != 1)
		usage_with_options(usage, check_options);
	return argc;
}

int cmd_check(int argc, const char **argv)
{
	const char *objname;
	struct objtool_file *file;
	int ret;

	argc = cmd_parse_options(argc, argv, check_usage);
	objname = argv[0];

	file = objtool_open_read(objname);
	if (!file)
		return 1;

	ret = check(file);
	if (ret)
		return ret;

	if (file->elf->changed)
		return elf_write(file->elf);

	return 0;
>>>>>>> upstream/android-13
}
