<<<<<<< HEAD
/*
 * Copyright 2016, Cyril Bur, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2016, Cyril Bur, IBM Corp.
 *
>>>>>>> upstream/android-13
 * Syscalls can be performed provided the transactions are suspended.
 * The exec() class of syscall is unique as a new process is loaded.
 *
 * It makes little sense for after an exec() call for the previously
 * suspended transaction to still exist.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <libgen.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "tm.h"

static char *path;

static int test_exec(void)
{
	SKIP_IF(!have_htm());
<<<<<<< HEAD
=======
	SKIP_IF(htm_is_synthetic());
>>>>>>> upstream/android-13

	asm __volatile__(
		"tbegin.;"
		"blt    1f; "
		"tsuspend.;"
		"1: ;"
		: : : "memory");

	execl(path, "tm-exec", "--child", NULL);

	/* Shouldn't get here */
	perror("execl() failed");
	return 1;
}

static int after_exec(void)
{
	asm __volatile__(
		"tbegin.;"
		"blt    1f;"
		"tsuspend.;"
		"1: ;"
		: : : "memory");

	FAIL_IF(failure_is_nesting());
	return 0;
}

int main(int argc, char *argv[])
{
	path = argv[0];

	if (argc > 1 && strcmp(argv[1], "--child") == 0)
		return after_exec();

	return test_harness(test_exec, "tm_exec");
}
