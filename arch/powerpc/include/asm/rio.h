<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * RapidIO architecture support
 *
 * Copyright 2005 MontaVista Software, Inc.
 * Matt Porter <mporter@kernel.crashing.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> upstream/android-13
 */

#ifndef ASM_PPC_RIO_H
#define ASM_PPC_RIO_H

#ifdef CONFIG_FSL_RIO
extern int fsl_rio_mcheck_exception(struct pt_regs *);
#else
static inline int fsl_rio_mcheck_exception(struct pt_regs *regs) {return 0; }
#endif

#endif				/* ASM_PPC_RIO_H */
