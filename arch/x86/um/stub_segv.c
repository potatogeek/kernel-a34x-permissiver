/*
 * Copyright (C) 2004 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <sysdep/stub.h>
#include <sysdep/faultinfo.h>
#include <sysdep/mcontext.h>
#include <sys/ucontext.h>

void __attribute__ ((__section__ (".__syscall_stub")))
stub_segv_handler(int sig, siginfo_t *info, void *p)
{
<<<<<<< HEAD
	ucontext_t *uc = p;

	GET_FAULTINFO_FROM_MC(*((struct faultinfo *) STUB_DATA),
			      &uc->uc_mcontext);
=======
	struct faultinfo *f = get_stub_page();
	ucontext_t *uc = p;

	GET_FAULTINFO_FROM_MC(*f, &uc->uc_mcontext);
>>>>>>> upstream/android-13
	trap_myself();
}

