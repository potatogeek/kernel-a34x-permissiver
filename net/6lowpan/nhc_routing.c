<<<<<<< HEAD
/*
 *	6LoWPAN IPv6 Routing Header compression according to RFC6282
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *	6LoWPAN IPv6 Routing Header compression according to RFC6282
>>>>>>> upstream/android-13
 */

#include "nhc.h"

#define LOWPAN_NHC_ROUTING_IDLEN	1
#define LOWPAN_NHC_ROUTING_ID_0		0xe2
#define LOWPAN_NHC_ROUTING_MASK_0	0xfe

static void routing_nhid_setup(struct lowpan_nhc *nhc)
{
	nhc->id[0] = LOWPAN_NHC_ROUTING_ID_0;
	nhc->idmask[0] = LOWPAN_NHC_ROUTING_MASK_0;
}

LOWPAN_NHC(nhc_routing, "RFC6282 Routing", NEXTHDR_ROUTING, 0,
	   routing_nhid_setup, LOWPAN_NHC_ROUTING_IDLEN, NULL, NULL);

module_lowpan_nhc(nhc_routing);
MODULE_DESCRIPTION("6LoWPAN next header RFC6282 Routing compression");
MODULE_LICENSE("GPL");
