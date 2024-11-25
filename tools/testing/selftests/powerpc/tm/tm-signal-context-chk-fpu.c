<<<<<<< HEAD
/*
 * Copyright 2016, Cyril Bur, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 *
 * Test the kernel's signal frame code.
 *
 * The kernel sets up two sets of ucontexts if the signal was to be
 * delivered while the thread was in a transaction.
 * Expected behaviour is that the checkpointed state is in the user
 * context passed to the signal handler. The speculated state can be
 * accessed with the uc_link pointer.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2016, Cyril Bur, IBM Corp.
 *
 * Test the kernel's signal frame code.
 *
 * The kernel sets up two sets of ucontexts if the signal was to be
 * delivered while the thread was in a transaction (referred too as
 * first and second contexts).
 * Expected behaviour is that the checkpointed state is in the user
 * context passed to the signal handler (first context). The speculated
 * state can be accessed with the uc_link pointer (second context).
>>>>>>> upstream/android-13
 *
 * The rationale for this is that if TM unaware code (which linked
 * against TM libs) installs a signal handler it will not know of the
 * speculative nature of the 'live' registers and may infer the wrong
 * thing.
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <altivec.h>

#include "utils.h"
#include "tm.h"

#define MAX_ATTEMPT 500000

<<<<<<< HEAD
#define NV_FPU_REGS 18

long tm_signal_self_context_load(pid_t pid, long *gprs, double *fps, vector int *vms, vector int *vss);

/* Be sure there are 2x as many as there are NV FPU regs (2x18) */
static double fps[] = {
	 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18
};

static sig_atomic_t fail;
=======
#define NV_FPU_REGS 18 /* Number of non-volatile FP registers */
#define FPR14 14 /* First non-volatile FP register to check in f14-31 subset */

long tm_signal_self_context_load(pid_t pid, long *gprs, double *fps, vector int *vms, vector int *vss);

/* Test only non-volatile registers, i.e. 18 fpr registers from f14 to f31 */
static double fps[] = {
	/* First context will be set with these values, i.e. non-speculative */
	 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	/* Second context will be set with these values, i.e. speculative */
	-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18
};

static sig_atomic_t fail, broken;
>>>>>>> upstream/android-13

static void signal_usr1(int signum, siginfo_t *info, void *uc)
{
	int i;
	ucontext_t *ucp = uc;
	ucontext_t *tm_ucp = ucp->uc_link;

<<<<<<< HEAD
	for (i = 0; i < NV_FPU_REGS && !fail; i++) {
		fail = (ucp->uc_mcontext.fp_regs[i + 14] != fps[i]);
		fail |= (tm_ucp->uc_mcontext.fp_regs[i + 14] != fps[i + NV_FPU_REGS]);
		if (fail)
			printf("Failed on %d FP %g or %g\n", i, ucp->uc_mcontext.fp_regs[i + 14], tm_ucp->uc_mcontext.fp_regs[i + 14]);
=======
	for (i = 0; i < NV_FPU_REGS; i++) {
		/* Check first context. Print all mismatches. */
		fail = (ucp->uc_mcontext.fp_regs[FPR14 + i] != fps[i]);
		if (fail) {
			broken = 1;
			printf("FPR%d (1st context) == %g instead of %g (expected)\n",
				FPR14 + i, ucp->uc_mcontext.fp_regs[FPR14 + i], fps[i]);
		}
	}

	for (i = 0; i < NV_FPU_REGS; i++) {
		/* Check second context. Print all mismatches. */
		fail = (tm_ucp->uc_mcontext.fp_regs[FPR14 + i] != fps[NV_FPU_REGS + i]);
		if (fail) {
			broken = 1;
			printf("FPR%d (2nd context) == %g instead of %g (expected)\n",
				FPR14 + i, tm_ucp->uc_mcontext.fp_regs[FPR14 + i], fps[NV_FPU_REGS + i]);
		}
>>>>>>> upstream/android-13
	}
}

static int tm_signal_context_chk_fpu()
{
	struct sigaction act;
	int i;
	long rc;
	pid_t pid = getpid();

	SKIP_IF(!have_htm());
<<<<<<< HEAD
=======
	SKIP_IF(htm_is_synthetic());
>>>>>>> upstream/android-13

	act.sa_sigaction = signal_usr1;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &act, NULL) < 0) {
		perror("sigaction sigusr1");
		exit(1);
	}

	i = 0;
<<<<<<< HEAD
	while (i < MAX_ATTEMPT && !fail) {
=======
	while (i < MAX_ATTEMPT && !broken) {
		/*
		 * tm_signal_self_context_load will set both first and second
		 * contexts accordingly to the values passed through non-NULL
		 * array pointers to it, in that case 'fps', and invoke the
		 * signal handler installed for SIGUSR1.
		 */
>>>>>>> upstream/android-13
		rc = tm_signal_self_context_load(pid, NULL, fps, NULL, NULL);
		FAIL_IF(rc != pid);
		i++;
	}

<<<<<<< HEAD
	return fail;
=======
	return (broken);
>>>>>>> upstream/android-13
}

int main(void)
{
	return test_harness(tm_signal_context_chk_fpu, "tm_signal_context_chk_fpu");
}
