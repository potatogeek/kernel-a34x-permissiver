<<<<<<< HEAD

/*
 * da9210-regulator.h - Regulator definitions for DA9210
 * Copyright (C) 2013  Dialog Semiconductor Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * da9210-regulator.h - Regulator definitions for DA9210
 * Copyright (C) 2013  Dialog Semiconductor Ltd.
>>>>>>> upstream/android-13
 */

#ifndef __DA9210_REGISTERS_H__
#define __DA9210_REGISTERS_H__

struct da9210_pdata {
	struct regulator_init_data da9210_constraints;
};

/* Page selection */
#define	DA9210_REG_PAGE_CON			0x00

/* System Control and Event Registers */
#define	DA9210_REG_STATUS_A			0x50
#define	DA9210_REG_STATUS_B			0x51
#define	DA9210_REG_EVENT_A			0x52
#define	DA9210_REG_EVENT_B			0x53
#define	DA9210_REG_MASK_A			0x54
#define	DA9210_REG_MASK_B			0x55
#define	DA9210_REG_CONTROL_A			0x56

/* GPIO Control Registers */
#define	DA9210_REG_GPIO_0_1			0x58
#define	DA9210_REG_GPIO_2_3			0x59
#define	DA9210_REG_GPIO_4_5			0x5A
#define	DA9210_REG_GPIO_6			0x5B

/* Regulator Registers */
#define	DA9210_REG_BUCK_CONT			0x5D
#define	DA9210_REG_BUCK_ILIM			0xD0
#define	DA9210_REG_BUCK_CONF1			0xD1
#define	DA9210_REG_BUCK_CONF2			0xD2
#define DA9210_REG_VBACK_AUTO			0xD4
#define DA9210_REG_VBACK_BASE			0xD5
#define DA9210_REG_VBACK_MAX_DVC_IF		0xD6
#define DA9210_REG_VBACK_DVC			0xD7
#define	DA9210_REG_VBUCK_A			0xD8
#define	DA9210_REG_VBUCK_B			0xD9

/* I2C Interface Settings */
#define DA9210_REG_INTERFACE			0x105

/* OTP */
#define	DA9210_REG_OPT_COUNT			0x140
#define	DA9210_REG_OPT_ADDR			0x141
#define	DA9210_REG_OPT_DATA			0x142

/* Customer Trim and Configuration */
#define	DA9210_REG_CONFIG_A			0x143
#define	DA9210_REG_CONFIG_B			0x144
#define	DA9210_REG_CONFIG_C			0x145
#define	DA9210_REG_CONFIG_D			0x146
#define	DA9210_REG_CONFIG_E			0x147


/*
 * Registers bits
 */
/* DA9210_REG_PAGE_CON (addr=0x00) */
#define	DA9210_PEG_PAGE_SHIFT			0
#define	DA9210_REG_PAGE_MASK			0x0F
/* On I2C registers 0x00 - 0xFF */
#define	DA9210_REG_PAGE0			0
/* On I2C registers 0x100 - 0x1FF */
#define	DA9210_REG_PAGE2			2
#define	DA9210_PAGE_WRITE_MODE			0x00
#define	DA9210_REPEAT_WRITE_MODE		0x40
#define	DA9210_PAGE_REVERT			0x80

/* DA9210_REG_STATUS_A (addr=0x50) */
#define	DA9210_GPI0				0x01
#define	DA9210_GPI1				0x02
#define	DA9210_GPI2				0x04
#define	DA9210_GPI3				0x08
#define	DA9210_GPI4				0x10
#define	DA9210_GPI5				0x20
#define	DA9210_GPI6				0x40

/* DA9210_REG_EVENT_A (addr=0x52) */
#define	DA9210_E_GPI0				0x01
#define	DA9210_E_GPI1				0x02
#define	DA9210_E_GPI2				0x04
#define	DA9210_E_GPI3				0x08
#define	DA9210_E_GPI4				0x10
#define	DA9210_E_GPI5				0x20
#define	DA9210_E_GPI6				0x40

/* DA9210_REG_EVENT_B (addr=0x53) */
#define	DA9210_E_OVCURR				0x01
#define	DA9210_E_NPWRGOOD			0x02
#define	DA9210_E_TEMP_WARN			0x04
#define	DA9210_E_TEMP_CRIT			0x08
#define	DA9210_E_VMAX				0x10

/* DA9210_REG_MASK_A (addr=0x54) */
#define	DA9210_M_GPI0				0x01
#define	DA9210_M_GPI1				0x02
#define	DA9210_M_GPI2				0x04
#define	DA9210_M_GPI3				0x08
#define	DA9210_M_GPI4				0x10
#define	DA9210_M_GPI5				0x20
#define	DA9210_M_GPI6				0x40

/* DA9210_REG_MASK_B (addr=0x55) */
#define	DA9210_M_OVCURR				0x01
#define	DA9210_M_NPWRGOOD			0x02
#define	DA9210_M_TEMP_WARN			0x04
#define	DA9210_M_TEMP_CRIT			0x08
#define	DA9210_M_VMAX				0x10

/* DA9210_REG_CONTROL_A (addr=0x56) */
#define	DA9210_DEBOUNCING_SHIFT			0
#define	DA9210_DEBOUNCING_MASK			0x07
#define	DA9210_SLEW_RATE_SHIFT			3
#define	DA9210_SLEW_RATE_MASK			0x18
#define	DA9210_V_LOCK				0x20

/* DA9210_REG_GPIO_0_1 (addr=0x58) */
#define	DA9210_GPIO0_PIN_SHIFT			0
#define	DA9210_GPIO0_PIN_MASK			0x03
#define		DA9210_GPIO0_PIN_GPI		0x00
#define		DA9210_GPIO0_PIN_GPO_OD		0x02
#define		DA9210_GPIO0_PIN_GPO		0x03
#define	DA9210_GPIO0_TYPE			0x04
#define		DA9210_GPIO0_TYPE_GPI		0x00
#define		DA9210_GPIO0_TYPE_GPO		0x04
#define	DA9210_GPIO0_MODE			0x08
#define	DA9210_GPIO1_PIN_SHIFT			4
#define	DA9210_GPIO1_PIN_MASK			0x30
#define		DA9210_GPIO1_PIN_GPI		0x00
#define		DA9210_GPIO1_PIN_VERROR		0x10
#define		DA9210_GPIO1_PIN_GPO_OD		0x20
#define		DA9210_GPIO1_PIN_GPO		0x30
#define	DA9210_GPIO1_TYPE_SHIFT			0x40
#define		DA9210_GPIO1_TYPE_GPI		0x00
#define		DA9210_GPIO1_TYPE_GPO		0x40
#define	DA9210_GPIO1_MODE			0x80

/* DA9210_REG_GPIO_2_3 (addr=0x59) */
#define	DA9210_GPIO2_PIN_SHIFT			0
#define	DA9210_GPIO2_PIN_MASK			0x03
#define		DA9210_GPIO2_PIN_GPI		0x00
#define		DA9210_GPIO5_PIN_BUCK_CLK	0x10
#define		DA9210_GPIO2_PIN_GPO_OD		0x02
#define		DA9210_GPIO2_PIN_GPO		0x03
#define	DA9210_GPIO2_TYPE			0x04
#define		DA9210_GPIO2_TYPE_GPI		0x00
#define		DA9210_GPIO2_TYPE_GPO		0x04
#define	DA9210_GPIO2_MODE			0x08
#define	DA9210_GPIO3_PIN_SHIFT			4
#define	DA9210_GPIO3_PIN_MASK			0x30
#define		DA9210_GPIO3_PIN_GPI		0x00
#define		DA9210_GPIO3_PIN_IERROR		0x10
#define		DA9210_GPIO3_PIN_GPO_OD		0x20
#define		DA9210_GPIO3_PIN_GPO		0x30
#define	DA9210_GPIO3_TYPE_SHIFT			0x40
#define		DA9210_GPIO3_TYPE_GPI		0x00
#define		DA9210_GPIO3_TYPE_GPO		0x40
#define	DA9210_GPIO3_MODE			0x80

/* DA9210_REG_GPIO_4_5 (addr=0x5A) */
#define	DA9210_GPIO4_PIN_SHIFT			0
#define	DA9210_GPIO4_PIN_MASK			0x03
#define		DA9210_GPIO4_PIN_GPI		0x00
#define		DA9210_GPIO4_PIN_GPO_OD		0x02
#define		DA9210_GPIO4_PIN_GPO		0x03
#define	DA9210_GPIO4_TYPE			0x04
#define		DA9210_GPIO4_TYPE_GPI		0x00
#define		DA9210_GPIO4_TYPE_GPO		0x04
#define	DA9210_GPIO4_MODE			0x08
#define	DA9210_GPIO5_PIN_SHIFT			4
#define	DA9210_GPIO5_PIN_MASK			0x30
#define		DA9210_GPIO5_PIN_GPI		0x00
#define		DA9210_GPIO5_PIN_INTERFACE	0x01
#define		DA9210_GPIO5_PIN_GPO_OD		0x20
#define		DA9210_GPIO5_PIN_GPO		0x30
#define	DA9210_GPIO5_TYPE_SHIFT			0x40
#define		DA9210_GPIO5_TYPE_GPI		0x00
#define		DA9210_GPIO5_TYPE_GPO		0x40
#define	DA9210_GPIO5_MODE			0x80

/* DA9210_REG_GPIO_6 (addr=0x5B) */
#define	DA9210_GPIO6_PIN_SHIFT			0
#define	DA9210_GPIO6_PIN_MASK			0x03
#define		DA9210_GPIO6_PIN_GPI		0x00
#define		DA9210_GPIO6_PIN_INTERFACE	0x01
#define		DA9210_GPIO6_PIN_GPO_OD		0x02
#define		DA9210_GPIO6_PIN_GPO		0x03
#define	DA9210_GPIO6_TYPE			0x04
#define		DA9210_GPIO6_TYPE_GPI		0x00
#define		DA9210_GPIO6_TYPE_GPO		0x04
#define	DA9210_GPIO6_MODE			0x08

/* DA9210_REG_BUCK_CONT (addr=0x5D) */
#define	DA9210_BUCK_EN				0x01
#define	DA9210_BUCK_GPI_SHIFT			1
#define DA9210_BUCK_GPI_MASK			0x06
#define		DA9210_BUCK_GPI_OFF		0x00
#define		DA9210_BUCK_GPI_GPIO0		0x02
#define		DA9210_BUCK_GPI_GPIO3		0x04
#define		DA9210_BUCK_GPI_GPIO4		0x06
#define	DA9210_BUCK_PD_DIS			0x08
#define	DA9210_VBUCK_SEL			0x10
#define		DA9210_VBUCK_SEL_A		0x00
#define		DA9210_VBUCK_SEL_B		0x10
#define	DA9210_VBUCK_GPI_SHIFT			5
#define	DA9210_VBUCK_GPI_MASK			0x60
#define		DA9210_VBUCK_GPI_OFF		0x00
#define		DA9210_VBUCK_GPI_GPIO0		0x20
#define		DA9210_VBUCK_GPI_GPIO3		0x40
#define		DA9210_VBUCK_GPI_GPIO4		0x60
#define	DA9210_DVC_CTRL_EN			0x80

/* DA9210_REG_BUCK_ILIM (addr=0xD0) */
#define DA9210_BUCK_ILIM_SHIFT			0
#define DA9210_BUCK_ILIM_MASK			0x0F
#define DA9210_BUCK_IALARM			0x10

/* DA9210_REG_BUCK_CONF1 (addr=0xD1) */
#define DA9210_BUCK_MODE_SHIFT			0
#define DA9210_BUCK_MODE_MASK			0x03
#define		DA9210_BUCK_MODE_MANUAL		0x00
#define		DA9210_BUCK_MODE_SLEEP		0x01
#define		DA9210_BUCK_MODE_SYNC		0x02
#define		DA9210_BUCK_MODE_AUTO		0x03
#define DA9210_STARTUP_CTRL_SHIFT		2
#define DA9210_STARTUP_CTRL_MASK		0x1C
#define DA9210_PWR_DOWN_CTRL_SHIFT		5
#define DA9210_PWR_DOWN_CTRL_MASK		0xE0

/* DA9210_REG_BUCK_CONF2 (addr=0xD2) */
#define DA9210_PHASE_SEL_SHIFT			0
#define DA9210_PHASE_SEL_MASK			0x03
#define DA9210_FREQ_SEL				0x40

/* DA9210_REG_BUCK_AUTO (addr=0xD4) */
#define DA9210_VBUCK_AUTO_SHIFT			0
#define DA9210_VBUCK_AUTO_MASK			0x7F

/* DA9210_REG_BUCK_BASE (addr=0xD5) */
#define DA9210_VBUCK_BASE_SHIFT			0
#define DA9210_VBUCK_BASE_MASK			0x7F

/* DA9210_REG_VBUCK_MAX_DVC_IF (addr=0xD6) */
#define DA9210_VBUCK_MAX_SHIFT			0
#define DA9210_VBUCK_MAX_MASK			0x7F
#define DA9210_DVC_STEP_SIZE			0x80
#define		DA9210_DVC_STEP_SIZE_10MV	0x00
#define		DA9210_DVC_STEP_SIZE_20MV	0x80

/* DA9210_REG_VBUCK_DVC (addr=0xD7) */
#define DA9210_VBUCK_DVC_SHIFT			0
#define DA9210_VBUCK_DVC_MASK			0x7F

/* DA9210_REG_VBUCK_A/B (addr=0xD8/0xD9) */
#define DA9210_VBUCK_SHIFT			0
#define DA9210_VBUCK_MASK			0x7F
#define DA9210_VBUCK_BIAS			0
#define DA9210_BUCK_SL				0x80

/* DA9210_REG_INTERFACE (addr=0x105) */
#define DA9210_IF_BASE_ADDR_SHIFT		4
#define DA9210_IF_BASE_ADDR_MASK		0xF0

/* DA9210_REG_CONFIG_E (addr=0x147) */
#define DA9210_STAND_ALONE			0x01

#endif	/* __DA9210_REGISTERS_H__ */

