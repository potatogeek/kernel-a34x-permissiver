<<<<<<< HEAD
/*
 * Copyright (C) 2015 Josh Poimboeuf <jpoimboe@redhat.com>
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
 * Copyright (C) 2015 Josh Poimboeuf <jpoimboe@redhat.com>
>>>>>>> upstream/android-13
 */

/*
 * objtool:
 *
 * The 'check' subcmd analyzes every .o file and ensures the validity of its
 * stack trace metadata.  It enforces a set of rules on asm code and C inline
 * assembly code so that stack traces can be reliable.
 *
 * For more information, see tools/objtool/Documentation/stack-validation.txt.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
<<<<<<< HEAD
=======
#include <unistd.h>
>>>>>>> upstream/android-13
#include <subcmd/exec-cmd.h>
#include <subcmd/pager.h>
#include <linux/kernel.h>

<<<<<<< HEAD
#include "builtin.h"
=======
#include <objtool/builtin.h>
#include <objtool/objtool.h>
#include <objtool/warn.h>
>>>>>>> upstream/android-13

struct cmd_struct {
	const char *name;
	int (*fn)(int, const char **);
	const char *help;
};

static const char objtool_usage_string[] =
	"objtool COMMAND [ARGS]";

static struct cmd_struct objtool_cmds[] = {
	{"check",	cmd_check,	"Perform stack metadata validation on an object file" },
	{"orc",		cmd_orc,	"Generate in-place ORC unwind tables for an object file" },
};

bool help;

<<<<<<< HEAD
=======
const char *objname;
static struct objtool_file file;

static bool objtool_create_backup(const char *_objname)
{
	int len = strlen(_objname);
	char *buf, *base, *name = malloc(len+6);
	int s, d, l, t;

	if (!name) {
		perror("failed backup name malloc");
		return false;
	}

	strcpy(name, _objname);
	strcpy(name + len, ".orig");

	d = open(name, O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if (d < 0) {
		perror("failed to create backup file");
		return false;
	}

	s = open(_objname, O_RDONLY);
	if (s < 0) {
		perror("failed to open orig file");
		return false;
	}

	buf = malloc(4096);
	if (!buf) {
		perror("failed backup data malloc");
		return false;
	}

	while ((l = read(s, buf, 4096)) > 0) {
		base = buf;
		do {
			t = write(d, base, l);
			if (t < 0) {
				perror("failed backup write");
				return false;
			}
			base += t;
			l -= t;
		} while (l);
	}

	if (l < 0) {
		perror("failed backup read");
		return false;
	}

	free(name);
	free(buf);
	close(d);
	close(s);

	return true;
}

struct objtool_file *objtool_open_read(const char *_objname)
{
	if (objname) {
		if (strcmp(objname, _objname)) {
			WARN("won't handle more than one file at a time");
			return NULL;
		}
		return &file;
	}
	objname = _objname;

	file.elf = elf_open_read(objname, O_RDWR);
	if (!file.elf)
		return NULL;

	if (backup && !objtool_create_backup(objname)) {
		WARN("can't create backup file");
		return NULL;
	}

	INIT_LIST_HEAD(&file.insn_list);
	hash_init(file.insn_hash);
	INIT_LIST_HEAD(&file.retpoline_call_list);
	INIT_LIST_HEAD(&file.static_call_list);
	INIT_LIST_HEAD(&file.mcount_loc_list);
	file.c_file = !vmlinux && find_section_by_name(file.elf, ".comment");
	file.ignore_unreachables = no_unreachable;
	file.hints = false;

	return &file;
}

>>>>>>> upstream/android-13
static void cmd_usage(void)
{
	unsigned int i, longest = 0;

	printf("\n usage: %s\n\n", objtool_usage_string);

	for (i = 0; i < ARRAY_SIZE(objtool_cmds); i++) {
		if (longest < strlen(objtool_cmds[i].name))
			longest = strlen(objtool_cmds[i].name);
	}

	puts(" Commands:");
	for (i = 0; i < ARRAY_SIZE(objtool_cmds); i++) {
		printf("   %-*s   ", longest, objtool_cmds[i].name);
		puts(objtool_cmds[i].help);
	}

	printf("\n");

<<<<<<< HEAD
	exit(129);
=======
	if (!help)
		exit(129);
	exit(0);
>>>>>>> upstream/android-13
}

static void handle_options(int *argc, const char ***argv)
{
	while (*argc > 0) {
		const char *cmd = (*argv)[0];

		if (cmd[0] != '-')
			break;

		if (!strcmp(cmd, "--help") || !strcmp(cmd, "-h")) {
			help = true;
			break;
		} else {
			fprintf(stderr, "Unknown option: %s\n", cmd);
			cmd_usage();
		}

		(*argv)++;
		(*argc)--;
	}
}

static void handle_internal_command(int argc, const char **argv)
{
	const char *cmd = argv[0];
	unsigned int i, ret;

	for (i = 0; i < ARRAY_SIZE(objtool_cmds); i++) {
		struct cmd_struct *p = objtool_cmds+i;

		if (strcmp(p->name, cmd))
			continue;

		ret = p->fn(argc, argv);

		exit(ret);
	}

	cmd_usage();
}

int main(int argc, const char **argv)
{
	static const char *UNUSED = "OBJTOOL_NOT_IMPLEMENTED";

	/* libsubcmd init */
	exec_cmd_init("objtool", UNUSED, UNUSED, UNUSED);
	pager_init(UNUSED);

	argv++;
	argc--;
	handle_options(&argc, &argv);

	if (!argc || help)
		cmd_usage();

	handle_internal_command(argc, argv);

	return 0;
}
