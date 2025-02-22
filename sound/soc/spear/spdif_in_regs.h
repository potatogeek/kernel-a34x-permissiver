<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * SPEAr SPDIF IN controller header file
 *
 * Copyright (ST) 2011 Vipin Kumar (vipin.kumar@st.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> upstream/android-13
 */

#ifndef SPDIF_IN_REGS_H
#define SPDIF_IN_REGS_H

#define SPDIF_IN_CTRL		0x00
	#define SPDIF_IN_PRTYEN		(1 << 20)
	#define SPDIF_IN_STATEN		(1 << 19)
	#define SPDIF_IN_USREN		(1 << 18)
	#define SPDIF_IN_VALEN		(1 << 17)
	#define SPDIF_IN_BLKEN		(1 << 16)

	#define SPDIF_MODE_24BIT	(8 << 12)
	#define SPDIF_MODE_23BIT	(7 << 12)
	#define SPDIF_MODE_22BIT	(6 << 12)
	#define SPDIF_MODE_21BIT	(5 << 12)
	#define SPDIF_MODE_20BIT	(4 << 12)
	#define SPDIF_MODE_19BIT	(3 << 12)
	#define SPDIF_MODE_18BIT	(2 << 12)
	#define SPDIF_MODE_17BIT	(1 << 12)
	#define SPDIF_MODE_16BIT	(0 << 12)
	#define SPDIF_MODE_MASK		(0x0F << 12)

	#define SPDIF_IN_VALID		(1 << 11)
	#define SPDIF_IN_SAMPLE		(1 << 10)
	#define SPDIF_DATA_SWAP		(1 << 9)
	#define SPDIF_IN_ENB		(1 << 8)
	#define SPDIF_DATA_REVERT	(1 << 7)
	#define SPDIF_XTRACT_16BIT	(1 << 6)
	#define SPDIF_FIFO_THRES_16	(16 << 0)

#define SPDIF_IN_IRQ_MASK	0x04
#define SPDIF_IN_IRQ		0x08
	#define SPDIF_IRQ_FIFOWRITE	(1 << 0)
	#define SPDIF_IRQ_EMPTYFIFOREAD	(1 << 1)
	#define SPDIF_IRQ_FIFOFULL	(1 << 2)
	#define SPDIF_IRQ_OUTOFRANGE	(1 << 3)

#define SPDIF_IN_STA		0x0C
	#define SPDIF_IN_LOCK		(0x1 << 0)

#endif /* SPDIF_IN_REGS_H */
