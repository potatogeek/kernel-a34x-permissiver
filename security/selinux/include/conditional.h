<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Interface to booleans in the security server. This is exported
 * for the selinuxfs.
 *
 * Author: Karl MacMillan <kmacmillan@tresys.com>
 *
 * Copyright (C) 2003 - 2004 Tresys Technology, LLC
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or modify
 *  	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
=======
>>>>>>> upstream/android-13
 */

#ifndef _SELINUX_CONDITIONAL_H_
#define _SELINUX_CONDITIONAL_H_

#include "security.h"

<<<<<<< HEAD
int security_get_bools(struct selinux_state *state,
		       int *len, char ***names, int **values);

int security_set_bools(struct selinux_state *state,
		       int len, int *values);

int security_get_bool_value(struct selinux_state *state,
			    int index);
=======
int security_get_bools(struct selinux_policy *policy,
		       u32 *len, char ***names, int **values);

int security_set_bools(struct selinux_state *state, u32 len, int *values);

int security_get_bool_value(struct selinux_state *state, u32 index);
>>>>>>> upstream/android-13

#endif
