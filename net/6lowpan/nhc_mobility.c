<<<<<<< HEAD
/*
 *	6LoWPAN IPv6 Mobility Header compression according to RFC6282
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *	6LoWPAN IPv6 Mobility Header compression according to RFC6282
>>>>>>> upstream/android-13
 */

#include "nhc.h"

#define LOWPAN_NHC_MOBILITY_IDLEN	1
#define LOWPAN_NHC_MOBILITY_ID_0	0xe8
#define LOWPAN_NHC_MOBILITY_MASK_0	0xfe

static void mobility_nhid_setup(struct lowpan_nhc *nhc)
{
	nhc->id[0] = LOWPAN_NHC_MOBILITY_ID_0;
	nhc->idmask[0] = LOWPAN_NHC_MOBILITY_MASK_0;
}

LOWPAN_NHC(nhc_mobility, "RFC6282 Mobility", NEXTHDR_MOBILITY, 0,
	   mobility_nhid_setup, LOWPAN_NHC_MOBILITY_IDLEN, NULL, NULL);

module_lowpan_nhc(nhc_mobility);
MODULE_DESCRIPTION("6LoWPAN next header RFC6282 Mobility compression");
MODULE_LICENSE("GPL");
