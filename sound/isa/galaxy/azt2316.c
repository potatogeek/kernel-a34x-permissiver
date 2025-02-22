<<<<<<< HEAD
/*
 * Aztech AZT2316 Driver
 * Copyright (C) 2007,2010  Rene Herman
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Aztech AZT2316 Driver
 * Copyright (C) 2007,2010  Rene Herman
>>>>>>> upstream/android-13
 */

#define AZT2316

#define CRD_NAME "Aztech AZT2316"
#define DRV_NAME "AZT2316"
#define DEV_NAME "azt2316"

#define GALAXY_DSP_MAJOR		3
#define GALAXY_DSP_MINOR		1

#define GALAXY_CONFIG_SIZE		4

/*
 * 32-bit config register
 */

#define GALAXY_CONFIG_SBA_220		(0 << 0)
#define GALAXY_CONFIG_SBA_240		(1 << 0)
#define GALAXY_CONFIG_SBA_260		(2 << 0)
#define GALAXY_CONFIG_SBA_280		(3 << 0)
#define GALAXY_CONFIG_SBA_MASK		GALAXY_CONFIG_SBA_280

#define GALAXY_CONFIG_SBIRQ_2		(1 << 2)
#define GALAXY_CONFIG_SBIRQ_5		(1 << 3)
#define GALAXY_CONFIG_SBIRQ_7		(1 << 4)
#define GALAXY_CONFIG_SBIRQ_10		(1 << 5)

#define GALAXY_CONFIG_SBDMA_DISABLE	(0 << 6)
#define GALAXY_CONFIG_SBDMA_0		(1 << 6)
#define GALAXY_CONFIG_SBDMA_1		(2 << 6)
#define GALAXY_CONFIG_SBDMA_3		(3 << 6)

#define GALAXY_CONFIG_WSSA_530		(0 << 8)
#define GALAXY_CONFIG_WSSA_604		(1 << 8)
#define GALAXY_CONFIG_WSSA_E80		(2 << 8)
#define GALAXY_CONFIG_WSSA_F40		(3 << 8)

#define GALAXY_CONFIG_WSS_ENABLE	(1 << 10)

#define GALAXY_CONFIG_GAME_ENABLE	(1 << 11)

#define GALAXY_CONFIG_MPUA_300		(0 << 12)
#define GALAXY_CONFIG_MPUA_330		(1 << 12)

#define GALAXY_CONFIG_MPU_ENABLE	(1 << 13)

#define GALAXY_CONFIG_CDA_310		(0 << 14)
#define GALAXY_CONFIG_CDA_320		(1 << 14)
#define GALAXY_CONFIG_CDA_340		(2 << 14)
#define GALAXY_CONFIG_CDA_350		(3 << 14)
#define GALAXY_CONFIG_CDA_MASK		GALAXY_CONFIG_CDA_350

#define GALAXY_CONFIG_CD_DISABLE	(0 << 16)
#define GALAXY_CONFIG_CD_PANASONIC	(1 << 16)
#define GALAXY_CONFIG_CD_SONY		(2 << 16)
#define GALAXY_CONFIG_CD_MITSUMI	(3 << 16)
#define GALAXY_CONFIG_CD_AZTECH		(4 << 16)
#define GALAXY_CONFIG_CD_UNUSED_5	(5 << 16)
#define GALAXY_CONFIG_CD_UNUSED_6	(6 << 16)
#define GALAXY_CONFIG_CD_UNUSED_7	(7 << 16)
#define GALAXY_CONFIG_CD_MASK		GALAXY_CONFIG_CD_UNUSED_7

#define GALAXY_CONFIG_CDDMA8_DISABLE	(0 << 20)
#define GALAXY_CONFIG_CDDMA8_0		(1 << 20)
#define GALAXY_CONFIG_CDDMA8_1		(2 << 20)
#define GALAXY_CONFIG_CDDMA8_3		(3 << 20)
#define GALAXY_CONFIG_CDDMA8_MASK	GALAXY_CONFIG_CDDMA8_3

#define GALAXY_CONFIG_CDDMA16_DISABLE	(0 << 22)
#define GALAXY_CONFIG_CDDMA16_5		(1 << 22)
#define GALAXY_CONFIG_CDDMA16_6		(2 << 22)
#define GALAXY_CONFIG_CDDMA16_7		(3 << 22)
#define GALAXY_CONFIG_CDDMA16_MASK	GALAXY_CONFIG_CDDMA16_7

#define GALAXY_CONFIG_MPUIRQ_2		(1 << 24)
#define GALAXY_CONFIG_MPUIRQ_5		(1 << 25)
#define GALAXY_CONFIG_MPUIRQ_7		(1 << 26)
#define GALAXY_CONFIG_MPUIRQ_10		(1 << 27)

#define GALAXY_CONFIG_CDIRQ_5		(1 << 28)
#define GALAXY_CONFIG_CDIRQ_11		(1 << 29)
#define GALAXY_CONFIG_CDIRQ_12		(1 << 30)
#define GALAXY_CONFIG_CDIRQ_15		(1 << 31)
#define GALAXY_CONFIG_CDIRQ_MASK	(\
	GALAXY_CONFIG_CDIRQ_5 | GALAXY_CONFIG_CDIRQ_11 |\
	GALAXY_CONFIG_CDIRQ_12 | GALAXY_CONFIG_CDIRQ_15)

#define GALAXY_CONFIG_MASK		(\
	GALAXY_CONFIG_SBA_MASK | GALAXY_CONFIG_CDA_MASK |\
	GALAXY_CONFIG_CD_MASK | GALAXY_CONFIG_CDDMA16_MASK |\
	GALAXY_CONFIG_CDDMA8_MASK | GALAXY_CONFIG_CDIRQ_MASK)

#include "galaxy.c"
