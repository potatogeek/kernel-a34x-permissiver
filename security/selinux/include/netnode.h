<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Network node table
 *
 * SELinux must keep a mapping of network nodes to labels/SIDs.  This
 * mapping is maintained as part of the normal policy but a fast cache is
 * needed to reduce the lookup overhead since most of these queries happen on
 * a per-packet basis.
 *
 * Author: Paul Moore <paul@paul-moore.com>
<<<<<<< HEAD
 *
=======
>>>>>>> upstream/android-13
 */

/*
 * (c) Copyright Hewlett-Packard Development Company, L.P., 2007
<<<<<<< HEAD
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _SELINUX_NETNODE_H
#define _SELINUX_NETNODE_H

void sel_netnode_flush(void);

int sel_netnode_sid(void *addr, u16 family, u32 *sid);

#endif
