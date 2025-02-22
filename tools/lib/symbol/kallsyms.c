// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include <ctype.h>
#include "symbol/kallsyms.h"
#include <stdio.h>
#include <stdlib.h>
=======
#include "symbol/kallsyms.h"
#include "api/io.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
>>>>>>> upstream/android-13

u8 kallsyms2elf_type(char type)
{
	type = tolower(type);
	return (type == 't' || type == 'w') ? STT_FUNC : STT_OBJECT;
}

bool kallsyms__is_function(char symbol_type)
{
	symbol_type = toupper(symbol_type);
	return symbol_type == 'T' || symbol_type == 'W';
}

<<<<<<< HEAD
=======
static void read_to_eol(struct io *io)
{
	int ch;

	for (;;) {
		ch = io__get_char(io);
		if (ch < 0 || ch == '\n')
			return;
	}
}

>>>>>>> upstream/android-13
int kallsyms__parse(const char *filename, void *arg,
		    int (*process_symbol)(void *arg, const char *name,
					  char type, u64 start))
{
<<<<<<< HEAD
	char *line = NULL;
	size_t n;
	int err = -1;
	FILE *file = fopen(filename, "r");

	if (file == NULL)
		goto out_failure;

	err = 0;

	while (!feof(file)) {
		u64 start;
		int line_len, len;
		char symbol_type;
		char *symbol_name;

		line_len = getline(&line, &n, file);
		if (line_len < 0 || !line)
			break;

		line[--line_len] = '\0'; /* \n */

		len = hex2u64(line, &start);

		/* Skip the line if we failed to parse the address. */
		if (!len)
			continue;

		len++;
		if (len + 2 >= line_len)
			continue;

		symbol_type = line[len];
		len += 2;
		symbol_name = line + len;
		len = line_len - len;

		if (len >= KSYM_NAME_LEN) {
			err = -1;
			break;
		}
=======
	struct io io;
	char bf[BUFSIZ];
	int err;

	io.fd = open(filename, O_RDONLY, 0);

	if (io.fd < 0)
		return -1;

	io__init(&io, io.fd, bf, sizeof(bf));

	err = 0;
	while (!io.eof) {
		__u64 start;
		int ch;
		size_t i;
		char symbol_type;
		char symbol_name[KSYM_NAME_LEN + 1];

		if (io__get_hex(&io, &start) != ' ') {
			read_to_eol(&io);
			continue;
		}
		symbol_type = io__get_char(&io);
		if (io__get_char(&io) != ' ') {
			read_to_eol(&io);
			continue;
		}
		for (i = 0; i < sizeof(symbol_name); i++) {
			ch = io__get_char(&io);
			if (ch < 0 || ch == '\n')
				break;
			symbol_name[i]  = ch;
		}
		symbol_name[i]  = '\0';
>>>>>>> upstream/android-13

		err = process_symbol(arg, symbol_name, symbol_type, start);
		if (err)
			break;
	}

<<<<<<< HEAD
	free(line);
	fclose(file);
	return err;

out_failure:
	return -1;
=======
	close(io.fd);
	return err;
>>>>>>> upstream/android-13
}
