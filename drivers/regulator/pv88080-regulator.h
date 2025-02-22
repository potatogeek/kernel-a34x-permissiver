<<<<<<< HEAD
/*
 * pv88080-regulator.h - Regulator definitions for PV88080
 * Copyright (C) 2016 Powerventure Semiconductor Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * pv88080-regulator.h - Regulator definitions for PV88080
 * Copyright (C) 2016 Powerventure Semiconductor Ltd.
>>>>>>> upstream/android-13
 */

#ifndef __PV88080_REGISTERS_H__
#define __PV88080_REGISTERS_H__

/* System Control and Event Registers */
#define	PV88080_REG_EVENT_A				0x04
#define	PV88080_REG_MASK_A				0x09
#define	PV88080_REG_MASK_B				0x0A
#define	PV88080_REG_MASK_C				0x0B

/* Regulator Registers - rev. AA */
#define PV88080AA_REG_HVBUCK_CONF1		0x2D
#define PV88080AA_REG_HVBUCK_CONF2		0x2E
#define	PV88080AA_REG_BUCK1_CONF0		0x27
#define	PV88080AA_REG_BUCK1_CONF1		0x28
#define	PV88080AA_REG_BUCK1_CONF2		0x59
#define	PV88080AA_REG_BUCK1_CONF5		0x5C
#define	PV88080AA_REG_BUCK2_CONF0		0x29
#define	PV88080AA_REG_BUCK2_CONF1		0x2A
#define	PV88080AA_REG_BUCK2_CONF2		0x61
#define	PV88080AA_REG_BUCK2_CONF5		0x64
#define	PV88080AA_REG_BUCK3_CONF0		0x2B
#define	PV88080AA_REG_BUCK3_CONF1		0x2C
#define	PV88080AA_REG_BUCK3_CONF2		0x69
#define	PV88080AA_REG_BUCK3_CONF5		0x6C

/* Regulator Registers - rev. BA */
#define	PV88080BA_REG_HVBUCK_CONF1		0x33
#define	PV88080BA_REG_HVBUCK_CONF2		0x34
#define	PV88080BA_REG_BUCK1_CONF0		0x2A
#define	PV88080BA_REG_BUCK1_CONF1		0x2C
#define	PV88080BA_REG_BUCK1_CONF2		0x5A
#define	PV88080BA_REG_BUCK1_CONF5		0x5D
#define	PV88080BA_REG_BUCK2_CONF0		0x2D
#define	PV88080BA_REG_BUCK2_CONF1		0x2F
#define	PV88080BA_REG_BUCK2_CONF2		0x63
#define	PV88080BA_REG_BUCK2_CONF5		0x66
#define	PV88080BA_REG_BUCK3_CONF0		0x30
#define	PV88080BA_REG_BUCK3_CONF1		0x32
#define	PV88080BA_REG_BUCK3_CONF2		0x6C
#define	PV88080BA_REG_BUCK3_CONF5		0x6F

/* PV88080_REG_EVENT_A (addr=0x04) */
#define	PV88080_E_VDD_FLT				0x01
#define	PV88080_E_OVER_TEMP				0x02

/* PV88080_REG_MASK_A (addr=0x09) */
#define	PV88080_M_VDD_FLT				0x01
#define	PV88080_M_OVER_TEMP				0x02

/* PV88080_REG_BUCK1_CONF0 (addr=0x27|0x2A) */
#define	PV88080_BUCK1_EN				0x80
#define PV88080_VBUCK1_MASK				0x7F

/* PV88080_REG_BUCK2_CONF0 (addr=0x29|0x2D) */
#define	PV88080_BUCK2_EN				0x80
#define PV88080_VBUCK2_MASK				0x7F

/* PV88080_REG_BUCK3_CONF0 (addr=0x2B|0x30) */
#define	PV88080_BUCK3_EN				0x80
#define PV88080_VBUCK3_MASK				0x7F

/* PV88080_REG_BUCK1_CONF1 (addr=0x28|0x2C) */
#define PV88080_BUCK1_ILIM_SHIFT		2
#define PV88080_BUCK1_ILIM_MASK			0x0C
#define PV88080_BUCK1_MODE_MASK			0x03

/* PV88080_REG_BUCK2_CONF1 (addr=0x2A|0x2F) */
#define PV88080_BUCK2_ILIM_SHIFT		2
#define PV88080_BUCK2_ILIM_MASK			0x0C
#define PV88080_BUCK2_MODE_MASK			0x03

/* PV88080_REG_BUCK3_CONF1 (addr=0x2C|0x32) */
#define PV88080_BUCK3_ILIM_SHIFT		2
#define PV88080_BUCK3_ILIM_MASK			0x0C
#define PV88080_BUCK3_MODE_MASK			0x03

#define	PV88080_BUCK_MODE_SLEEP			0x00
#define	PV88080_BUCK_MODE_AUTO			0x01
#define	PV88080_BUCK_MODE_SYNC			0x02

/* PV88080_REG_HVBUCK_CONF1 (addr=0x2D|0x33) */
#define PV88080_VHVBUCK_MASK			0xFF

/* PV88080_REG_HVBUCK_CONF1 (addr=0x2E|0x34) */
#define PV88080_HVBUCK_EN				0x01

/* PV88080_REG_BUCK2_CONF2 (addr=0x61|0x63) */
/* PV88080_REG_BUCK3_CONF2 (addr=0x69|0x6C) */
#define PV88080_BUCK_VDAC_RANGE_SHIFT	7
#define PV88080_BUCK_VDAC_RANGE_MASK	0x01

#define PV88080_BUCK_VDAC_RANGE_1		0x00
#define PV88080_BUCK_VDAC_RANGE_2		0x01

/* PV88080_REG_BUCK2_CONF5 (addr=0x64|0x66) */
/* PV88080_REG_BUCK3_CONF5 (addr=0x6C|0x6F) */
#define PV88080_BUCK_VRANGE_GAIN_SHIFT	0
#define PV88080_BUCK_VRANGE_GAIN_MASK	0x01

#define PV88080_BUCK_VRANGE_GAIN_1		0x00
#define PV88080_BUCK_VRANGE_GAIN_2		0x01

#endif	/* __PV88080_REGISTERS_H__ */
