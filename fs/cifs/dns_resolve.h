<<<<<<< HEAD
/*
 *   fs/cifs/dns_resolve.h -- DNS Resolver upcall management for CIFS DFS
 *                            Handles host name to IP address resolution
=======
/* SPDX-License-Identifier: LGPL-2.1 */
/*
 *   DNS Resolver upcall management for CIFS DFS
 *   Handles host name to IP address resolution
>>>>>>> upstream/android-13
 *
 *   Copyright (c) International Business Machines  Corp., 2008
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *
<<<<<<< HEAD
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#ifndef _DNS_RESOLVE_H
#define _DNS_RESOLVE_H

#ifdef __KERNEL__
<<<<<<< HEAD
extern int dns_resolve_server_name_to_ip(const char *unc, char **ip_addr);
=======
extern int dns_resolve_server_name_to_ip(const char *unc, char **ip_addr, time64_t *expiry);
>>>>>>> upstream/android-13
#endif /* KERNEL */

#endif /* _DNS_RESOLVE_H */
