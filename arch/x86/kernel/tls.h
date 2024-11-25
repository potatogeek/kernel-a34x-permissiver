<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Internal declarations for x86 TLS implementation functions.
 *
 * Copyright (C) 2007 Red Hat, Inc.  All rights reserved.
 *
<<<<<<< HEAD
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
=======
>>>>>>> upstream/android-13
 * Red Hat Author: Roland McGrath.
 */

#ifndef _ARCH_X86_KERNEL_TLS_H

#include <linux/regset.h>

extern user_regset_active_fn regset_tls_active;
<<<<<<< HEAD
extern user_regset_get_fn regset_tls_get;
=======
extern user_regset_get2_fn regset_tls_get;
>>>>>>> upstream/android-13
extern user_regset_set_fn regset_tls_set;

#endif	/* _ARCH_X86_KERNEL_TLS_H */
