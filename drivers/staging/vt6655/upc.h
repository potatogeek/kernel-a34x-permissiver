<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0+
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
<<<<<<< HEAD
 * File: upc.h
 *
=======
>>>>>>> upstream/android-13
 * Purpose: Macros to access device
 *
 * Author: Tevin Chen
 *
 * Date: Mar 17, 1997
 *
 */

#ifndef __UPC_H__
#define __UPC_H__

#include "device.h"

/*---------------------  Export Definitions -------------------------*/

<<<<<<< HEAD

/* For memory mapped IO */


=======
/* For memory mapped IO */

>>>>>>> upstream/android-13
#define VNSvInPortB(dwIOAddress, pbyData) \
	(*(pbyData) = ioread8(dwIOAddress))

#define VNSvInPortW(dwIOAddress, pwData) \
	(*(pwData) = ioread16(dwIOAddress))

#define VNSvInPortD(dwIOAddress, pdwData) \
	(*(pdwData) = ioread32(dwIOAddress))

#define VNSvOutPortB(dwIOAddress, byData) \
	iowrite8((u8)(byData), dwIOAddress)

#define VNSvOutPortW(dwIOAddress, wData) \
	iowrite16((u16)(wData), dwIOAddress)

#define VNSvOutPortD(dwIOAddress, dwData) \
	iowrite32((u32)(dwData), dwIOAddress)

#define PCAvDelayByIO(uDelayUnit)				\
do {								\
<<<<<<< HEAD
	unsigned char byData;					\
=======
	unsigned char __maybe_unused byData;			\
>>>>>>> upstream/android-13
	unsigned long ii;					\
								\
	if (uDelayUnit <= 50) {					\
		udelay(uDelayUnit);				\
	} else {						\
		for (ii = 0; ii < (uDelayUnit); ii++)		\
			byData = inb(0x61);			\
	}							\
} while (0)

/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

#endif /* __UPC_H__ */
