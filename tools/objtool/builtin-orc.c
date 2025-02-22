<<<<<<< HEAD
/*
 * Copyright (C) 2017 Josh Poimboeuf <jpoimboe@redhat.com>
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
 * Copyright (C) 2017 Josh Poimboeuf <jpoimboe@redhat.com>
>>>>>>> upstream/android-13
 */

/*
 * objtool orc:
 *
 * This command analyzes a .o file and adds .orc_unwind and .orc_unwind_ip
 * sections to it, which is used by the in-kernel ORC unwinder.
 *
 * This command is a superset of "objtool check".
 */

#include <string.h>
<<<<<<< HEAD
#include "builtin.h"
#include "check.h"

=======
#include <objtool/builtin.h>
#include <objtool/objtool.h>
>>>>>>> upstream/android-13

static const char *orc_usage[] = {
	"objtool orc generate [<options>] file.o",
	"objtool orc dump file.o",
	NULL,
};

int cmd_orc(int argc, const char **argv)
{
	const char *objname;

	argc--; argv++;
	if (argc <= 0)
		usage_with_options(orc_usage, check_options);

	if (!strncmp(argv[0], "gen", 3)) {
<<<<<<< HEAD
		argc = parse_options(argc, argv, check_options, orc_usage, 0);
		if (argc != 1)
			usage_with_options(orc_usage, check_options);

		objname = argv[0];

		return check(objname, true);
=======
		struct objtool_file *file;
		int ret;

		argc = cmd_parse_options(argc, argv, orc_usage);
		objname = argv[0];

		file = objtool_open_read(objname);
		if (!file)
			return 1;

		ret = check(file);
		if (ret)
			return ret;

		if (list_empty(&file->insn_list))
			return 0;

		ret = orc_create(file);
		if (ret)
			return ret;

		if (!file->elf->changed)
			return 0;

		return elf_write(file->elf);
>>>>>>> upstream/android-13
	}

	if (!strcmp(argv[0], "dump")) {
		if (argc != 2)
			usage_with_options(orc_usage, check_options);

		objname = argv[1];

		return orc_dump(objname);
	}

	usage_with_options(orc_usage, check_options);

	return 0;
}
