<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
	Mantis PCI bridge driver

	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

<<<<<<< HEAD
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> upstream/android-13
*/

#ifndef __MANTIS_DMA_H
#define __MANTIS_DMA_H

extern int mantis_dma_init(struct mantis_pci *mantis);
extern int mantis_dma_exit(struct mantis_pci *mantis);
extern void mantis_dma_start(struct mantis_pci *mantis);
extern void mantis_dma_stop(struct mantis_pci *mantis);
<<<<<<< HEAD
extern void mantis_dma_xfer(unsigned long data);
=======
extern void mantis_dma_xfer(struct tasklet_struct *t);
>>>>>>> upstream/android-13

#endif /* __MANTIS_DMA_H */
