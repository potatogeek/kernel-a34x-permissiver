<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * syscall_nt.c - checks syscalls with NT set
 * Copyright (c) 2014-2015 Andrew Lutomirski
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 * Some obscure user-space code requires the ability to make system calls
 * with FLAGS.NT set.  Make sure it works.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <err.h>
#include <sys/syscall.h>
<<<<<<< HEAD
#include <asm/processor-flags.h>

#ifdef __x86_64__
# define WIDTH "q"
#else
# define WIDTH "l"
#endif

static unsigned int nerrs;

static unsigned long get_eflags(void)
{
	unsigned long eflags;
	asm volatile ("pushf" WIDTH "\n\tpop" WIDTH " %0" : "=rm" (eflags));
	return eflags;
}

static void set_eflags(unsigned long eflags)
{
	asm volatile ("push" WIDTH " %0\n\tpopf" WIDTH
		      : : "rm" (eflags) : "flags");
}

=======

#include "helpers.h"

static unsigned int nerrs;

>>>>>>> upstream/android-13
static void sethandler(int sig, void (*handler)(int, siginfo_t *, void *),
		       int flags)
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO | flags;
	sigemptyset(&sa.sa_mask);
	if (sigaction(sig, &sa, 0))
		err(1, "sigaction");
}

static void sigtrap(int sig, siginfo_t *si, void *ctx_void)
{
}

static void do_it(unsigned long extraflags)
{
	unsigned long flags;

	set_eflags(get_eflags() | extraflags);
	syscall(SYS_getpid);
	flags = get_eflags();
	set_eflags(X86_EFLAGS_IF | X86_EFLAGS_FIXED);
	if ((flags & extraflags) == extraflags) {
		printf("[OK]\tThe syscall worked and flags are still set\n");
	} else {
		printf("[FAIL]\tThe syscall worked but flags were cleared (flags = 0x%lx but expected 0x%lx set)\n",
		       flags, extraflags);
		nerrs++;
	}
}

int main(void)
{
	printf("[RUN]\tSet NT and issue a syscall\n");
	do_it(X86_EFLAGS_NT);

<<<<<<< HEAD
=======
	printf("[RUN]\tSet AC and issue a syscall\n");
	do_it(X86_EFLAGS_AC);

	printf("[RUN]\tSet NT|AC and issue a syscall\n");
	do_it(X86_EFLAGS_NT | X86_EFLAGS_AC);

>>>>>>> upstream/android-13
	/*
	 * Now try it again with TF set -- TF forces returns via IRET in all
	 * cases except non-ptregs-using 64-bit full fast path syscalls.
	 */

	sethandler(SIGTRAP, sigtrap, 0);

<<<<<<< HEAD
	printf("[RUN]\tSet NT|TF and issue a syscall\n");
	do_it(X86_EFLAGS_NT | X86_EFLAGS_TF);

=======
	printf("[RUN]\tSet TF and issue a syscall\n");
	do_it(X86_EFLAGS_TF);

	printf("[RUN]\tSet NT|TF and issue a syscall\n");
	do_it(X86_EFLAGS_NT | X86_EFLAGS_TF);

	printf("[RUN]\tSet AC|TF and issue a syscall\n");
	do_it(X86_EFLAGS_AC | X86_EFLAGS_TF);

	printf("[RUN]\tSet NT|AC|TF and issue a syscall\n");
	do_it(X86_EFLAGS_NT | X86_EFLAGS_AC | X86_EFLAGS_TF);

	/*
	 * Now try DF.  This is evil and it's plausible that we will crash
	 * glibc, but glibc would have to do something rather surprising
	 * for this to happen.
	 */
	printf("[RUN]\tSet DF and issue a syscall\n");
	do_it(X86_EFLAGS_DF);

	printf("[RUN]\tSet TF|DF and issue a syscall\n");
	do_it(X86_EFLAGS_TF | X86_EFLAGS_DF);

>>>>>>> upstream/android-13
	return nerrs == 0 ? 0 : 1;
}
