<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * AppArmor security module
 *
 * This file contains AppArmor /proc/<pid>/attr/ interface function definitions.
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
=======
>>>>>>> upstream/android-13
 */

#ifndef __AA_PROCATTR_H
#define __AA_PROCATTR_H

int aa_getprocattr(struct aa_label *label, char **string);
int aa_setprocattr_changehat(char *args, size_t size, int flags);

#endif /* __AA_PROCATTR_H */
