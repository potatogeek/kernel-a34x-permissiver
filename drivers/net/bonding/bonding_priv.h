/*
 * Bond several ethernet interfaces into a Cisco, running 'Etherchannel'.
 *
 * Portions are (c) Copyright 1995 Simon "Guru Aleph-Null" Janes
 * NCM: Network and Communications Management, Inc.
 *
 * BUT, I'm the one who modified it for ethernet, so:
 * (c) Copyright 1999, Thomas Davis, tadavis@lbl.gov
 *
 *	This software may be used and distributed according to the terms
 *	of the GNU Public License, incorporated herein by reference.
 *
 */

#ifndef _BONDING_PRIV_H
#define _BONDING_PRIV_H
<<<<<<< HEAD

#define DRV_VERSION	"3.7.1"
#define DRV_RELDATE	"April 27, 2011"
#define DRV_NAME	"bonding"
#define DRV_DESCRIPTION	"Ethernet Channel Bonding Driver"

#define bond_version DRV_DESCRIPTION ": v" DRV_VERSION " (" DRV_RELDATE ")\n"
=======
#include <generated/utsrelease.h>

#define DRV_NAME	"bonding"
#define DRV_DESCRIPTION	"Ethernet Channel Bonding Driver"

#define bond_version DRV_DESCRIPTION ": v" UTS_RELEASE "\n"
>>>>>>> upstream/android-13

#endif
