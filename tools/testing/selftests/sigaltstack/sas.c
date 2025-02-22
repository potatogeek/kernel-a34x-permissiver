// SPDX-License-Identifier: GPL-2.0
/*
 * Stas Sergeev <stsp@users.sourceforge.net>
 *
 * test sigaltstack(SS_ONSTACK | SS_AUTODISARM)
 * If that succeeds, then swapcontext() can be used inside sighandler safely.
 *
 */

#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <alloca.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
<<<<<<< HEAD
=======
#include <sys/auxv.h>
>>>>>>> upstream/android-13

#include "../kselftest.h"

#ifndef SS_AUTODISARM
#define SS_AUTODISARM  (1U << 31)
#endif

<<<<<<< HEAD
=======
#ifndef AT_MINSIGSTKSZ
#define AT_MINSIGSTKSZ	51
#endif

static unsigned int stack_size;
>>>>>>> upstream/android-13
static void *sstack, *ustack;
static ucontext_t uc, sc;
static const char *msg = "[OK]\tStack preserved";
static const char *msg2 = "[FAIL]\tStack corrupted";
struct stk_data {
	char msg[128];
	int flag;
};

void my_usr1(int sig, siginfo_t *si, void *u)
{
	char *aa;
	int err;
	stack_t stk;
	struct stk_data *p;

#if __s390x__
	register unsigned long sp asm("%15");
#else
	register unsigned long sp asm("sp");
#endif

	if (sp < (unsigned long)sstack ||
<<<<<<< HEAD
			sp >= (unsigned long)sstack + SIGSTKSZ) {
=======
			sp >= (unsigned long)sstack + stack_size) {
>>>>>>> upstream/android-13
		ksft_exit_fail_msg("SP is not on sigaltstack\n");
	}
	/* put some data on stack. other sighandler will try to overwrite it */
	aa = alloca(1024);
	assert(aa);
	p = (struct stk_data *)(aa + 512);
	strcpy(p->msg, msg);
	p->flag = 1;
	ksft_print_msg("[RUN]\tsignal USR1\n");
	err = sigaltstack(NULL, &stk);
	if (err) {
		ksft_exit_fail_msg("sigaltstack() - %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (stk.ss_flags != SS_DISABLE)
		ksft_test_result_fail("tss_flags=%x, should be SS_DISABLE\n",
				stk.ss_flags);
	else
		ksft_test_result_pass(
				"sigaltstack is disabled in sighandler\n");
	swapcontext(&sc, &uc);
	ksft_print_msg("%s\n", p->msg);
	if (!p->flag) {
<<<<<<< HEAD
		ksft_exit_skip("[RUN]\tAborting\n");
=======
		ksft_exit_fail_msg("[RUN]\tAborting\n");
>>>>>>> upstream/android-13
		exit(EXIT_FAILURE);
	}
}

void my_usr2(int sig, siginfo_t *si, void *u)
{
	char *aa;
	struct stk_data *p;

	ksft_print_msg("[RUN]\tsignal USR2\n");
	aa = alloca(1024);
	/* dont run valgrind on this */
	/* try to find the data stored by previous sighandler */
	p = memmem(aa, 1024, msg, strlen(msg));
	if (p) {
		ksft_test_result_fail("sigaltstack re-used\n");
		/* corrupt the data */
		strcpy(p->msg, msg2);
		/* tell other sighandler that his data is corrupted */
		p->flag = 0;
	}
}

static void switch_fn(void)
{
	ksft_print_msg("[RUN]\tswitched to user ctx\n");
	raise(SIGUSR2);
	setcontext(&sc);
}

int main(void)
{
	struct sigaction act;
	stack_t stk;
	int err;

<<<<<<< HEAD
	ksft_print_header();
=======
	/* Make sure more than the required minimum. */
	stack_size = getauxval(AT_MINSIGSTKSZ) + SIGSTKSZ;
	ksft_print_msg("[NOTE]\tthe stack size is %lu\n", stack_size);

	ksft_print_header();
	ksft_set_plan(3);
>>>>>>> upstream/android-13

	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_ONSTACK | SA_SIGINFO;
	act.sa_sigaction = my_usr1;
	sigaction(SIGUSR1, &act, NULL);
	act.sa_sigaction = my_usr2;
	sigaction(SIGUSR2, &act, NULL);
<<<<<<< HEAD
	sstack = mmap(NULL, SIGSTKSZ, PROT_READ | PROT_WRITE,
=======
	sstack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE,
>>>>>>> upstream/android-13
		      MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if (sstack == MAP_FAILED) {
		ksft_exit_fail_msg("mmap() - %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	err = sigaltstack(NULL, &stk);
	if (err) {
		ksft_exit_fail_msg("sigaltstack() - %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (stk.ss_flags == SS_DISABLE) {
		ksft_test_result_pass(
				"Initial sigaltstack state was SS_DISABLE\n");
	} else {
		ksft_exit_fail_msg("Initial sigaltstack state was %x; "
		       "should have been SS_DISABLE\n", stk.ss_flags);
		return EXIT_FAILURE;
	}

	stk.ss_sp = sstack;
<<<<<<< HEAD
	stk.ss_size = SIGSTKSZ;
=======
	stk.ss_size = stack_size;
>>>>>>> upstream/android-13
	stk.ss_flags = SS_ONSTACK | SS_AUTODISARM;
	err = sigaltstack(&stk, NULL);
	if (err) {
		if (errno == EINVAL) {
<<<<<<< HEAD
			ksft_exit_skip(
=======
			ksft_test_result_skip(
>>>>>>> upstream/android-13
				"[NOTE]\tThe running kernel doesn't support SS_AUTODISARM\n");
			/*
			 * If test cases for the !SS_AUTODISARM variant were
			 * added, we could still run them.  We don't have any
			 * test cases like that yet, so just exit and report
			 * success.
			 */
			return 0;
		} else {
			ksft_exit_fail_msg(
				"sigaltstack(SS_ONSTACK | SS_AUTODISARM)  %s\n",
					strerror(errno));
			return EXIT_FAILURE;
		}
	}

<<<<<<< HEAD
	ustack = mmap(NULL, SIGSTKSZ, PROT_READ | PROT_WRITE,
=======
	ustack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE,
>>>>>>> upstream/android-13
		      MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if (ustack == MAP_FAILED) {
		ksft_exit_fail_msg("mmap() - %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	getcontext(&uc);
	uc.uc_link = NULL;
	uc.uc_stack.ss_sp = ustack;
<<<<<<< HEAD
	uc.uc_stack.ss_size = SIGSTKSZ;
=======
	uc.uc_stack.ss_size = stack_size;
>>>>>>> upstream/android-13
	makecontext(&uc, switch_fn, 0);
	raise(SIGUSR1);

	err = sigaltstack(NULL, &stk);
	if (err) {
		ksft_exit_fail_msg("sigaltstack() - %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (stk.ss_flags != SS_AUTODISARM) {
		ksft_exit_fail_msg("ss_flags=%x, should be SS_AUTODISARM\n",
				stk.ss_flags);
		exit(EXIT_FAILURE);
	}
	ksft_test_result_pass(
			"sigaltstack is still SS_AUTODISARM after signal\n");

	ksft_exit_pass();
	return 0;
}
