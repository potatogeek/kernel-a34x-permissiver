<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Network interface table.
 *
 * Network interfaces (devices) do not have a security field, so we
 * maintain a table associating each interface with a SID.
 *
 * Author: James Morris <jmorris@redhat.com>
 *
 * Copyright (C) 2003 Red Hat, Inc., James Morris <jmorris@redhat.com>
 * Copyright (C) 2007 Hewlett-Packard Development Company, L.P.
 *                    Paul Moore <paul@paul-moore.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef _SELINUX_NETIF_H_
#define _SELINUX_NETIF_H_

#include <net/net_namespace.h>

void sel_netif_flush(void);

int sel_netif_sid(struct net *ns, int ifindex, u32 *sid);

#endif	/* _SELINUX_NETIF_H_ */

