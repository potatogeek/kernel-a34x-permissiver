<<<<<<< HEAD
/**
 * ipoctal.h
 *
 * driver for the IPOCTAL boards

 * Copyright (C) 2009-2012 CERN (www.cern.ch)
 * Author: Nicolas Serafini, EIC2 SA
 * Author: Samuel Iglesias Gonsalvez <siglesias@igalia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; version 2 of the License.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * driver for the IPOCTAL boards
 *
 * Copyright (C) 2009-2012 CERN (www.cern.ch)
 * Author: Nicolas Serafini, EIC2 SA
 * Author: Samuel Iglesias Gonsalvez <siglesias@igalia.com>
>>>>>>> upstream/android-13
 */

#ifndef _IPOCTAL_H_
#define _IPOCTAL_H_

#define NR_CHANNELS		8
#define IPOCTAL_MAX_BOARDS	16
#define MAX_DEVICES		(NR_CHANNELS * IPOCTAL_MAX_BOARDS)
<<<<<<< HEAD
#define RELEVANT_IFLAG(iflag) ((iflag) & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))
=======
>>>>>>> upstream/android-13

/**
 * struct ipoctal_stats -- Stats since last reset
 *
 * @tx: Number of transmitted bytes
 * @rx: Number of received bytes
 * @overrun: Number of overrun errors
 * @parity_err: Number of parity errors
 * @framing_err: Number of framing errors
 * @rcv_break: Number of break received
 */
struct ipoctal_stats {
	unsigned long tx;
	unsigned long rx;
	unsigned long overrun_err;
	unsigned long parity_err;
	unsigned long framing_err;
	unsigned long rcv_break;
};

#endif /* _IPOCTAL_H_ */
