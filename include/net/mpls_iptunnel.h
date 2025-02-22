<<<<<<< HEAD
/*
 * Copyright (c) 2015 Cumulus Networks, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2015 Cumulus Networks, Inc.
>>>>>>> upstream/android-13
 */

#ifndef _NET_MPLS_IPTUNNEL_H
#define _NET_MPLS_IPTUNNEL_H 1

struct mpls_iptunnel_encap {
	u8	labels;
	u8	ttl_propagate;
	u8	default_ttl;
	u8	reserved1;
<<<<<<< HEAD
	u32	label[0];
=======
	u32	label[];
>>>>>>> upstream/android-13
};

static inline struct mpls_iptunnel_encap *mpls_lwtunnel_encap(struct lwtunnel_state *lwtstate)
{
	return (struct mpls_iptunnel_encap *)lwtstate->data;
}

#endif
