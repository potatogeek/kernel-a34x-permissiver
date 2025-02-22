// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"
#include "debug-internal.h"

static int __base_pr(const char *format, ...)
{
	va_list args;
	int err;

	va_start(args, format);
	err = vfprintf(stderr, format, args);
	va_end(args);
	return err;
}

<<<<<<< HEAD
libapi_print_fn_t __pr_warning = __base_pr;
=======
libapi_print_fn_t __pr_warn    = __base_pr;
>>>>>>> upstream/android-13
libapi_print_fn_t __pr_info    = __base_pr;
libapi_print_fn_t __pr_debug;

void libapi_set_print(libapi_print_fn_t warn,
		      libapi_print_fn_t info,
		      libapi_print_fn_t debug)
{
<<<<<<< HEAD
	__pr_warning = warn;
=======
	__pr_warn    = warn;
>>>>>>> upstream/android-13
	__pr_info    = info;
	__pr_debug   = debug;
}
