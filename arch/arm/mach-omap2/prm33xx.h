/*
 * AM33XX PRM instance offset macros
 *
<<<<<<< HEAD
 * Copyright (C) 2011-2012 Texas Instruments Incorporated - http://www.ti.com/
=======
 * Copyright (C) 2011-2012 Texas Instruments Incorporated - https://www.ti.com/
>>>>>>> upstream/android-13
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_PRM33XX_H
#define __ARCH_ARM_MACH_OMAP2_PRM33XX_H

#include "prcm-common.h"
#include "prm.h"

#define AM33XX_PRM_BASE               0x44E00000

#define AM33XX_PRM_REGADDR(inst, reg)                         \
	AM33XX_L4_WK_IO_ADDRESS(AM33XX_PRM_BASE + (inst) + (reg))


/* PRM instances */
#define AM33XX_PRM_OCP_SOCKET_MOD	0x0B00
#define AM33XX_PRM_PER_MOD		0x0C00
#define AM33XX_PRM_WKUP_MOD		0x0D00
#define AM33XX_PRM_MPU_MOD		0x0E00
#define AM33XX_PRM_DEVICE_MOD		0x0F00
#define AM33XX_PRM_RTC_MOD		0x1000
#define AM33XX_PRM_GFX_MOD		0x1100
#define AM33XX_PRM_CEFUSE_MOD		0x1200

/* PRM */

/* PRM.OCP_SOCKET_PRM register offsets */
#define AM33XX_REVISION_PRM_OFFSET		0x0000
#define AM33XX_REVISION_PRM			AM33XX_PRM_REGADDR(AM33XX_PRM_OCP_SOCKET_MOD, 0x0000)
#define AM33XX_PRM_IRQSTATUS_MPU_OFFSET		0x0004
#define AM33XX_PRM_IRQSTATUS_MPU		AM33XX_PRM_REGADDR(AM33XX_PRM_OCP_SOCKET_MOD, 0x0004)
#define AM33XX_PRM_IRQENABLE_MPU_OFFSET		0x0008
#define AM33XX_PRM_IRQENABLE_MPU		AM33XX_PRM_REGADDR(AM33XX_PRM_OCP_SOCKET_MOD, 0x0008)
#define AM33XX_PRM_IRQSTATUS_M3_OFFSET		0x000c
#define AM33XX_PRM_IRQSTATUS_M3			AM33XX_PRM_REGADDR(AM33XX_PRM_OCP_SOCKET_MOD, 0x000c)
#define AM33XX_PRM_IRQENABLE_M3_OFFSET		0x0010
#define AM33XX_PRM_IRQENABLE_M3			AM33XX_PRM_REGADDR(AM33XX_PRM_OCP_SOCKET_MOD, 0x0010)

/* PRM.PER_PRM register offsets */
#define AM33XX_RM_PER_RSTCTRL_OFFSET		0x0000
#define AM33XX_RM_PER_RSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_PER_MOD, 0x0000)
#define AM33XX_PM_PER_PWRSTST_OFFSET		0x0008
#define AM33XX_PM_PER_PWRSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_PER_MOD, 0x0008)
#define AM33XX_PM_PER_PWRSTCTRL_OFFSET		0x000c
#define AM33XX_PM_PER_PWRSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_PER_MOD, 0x000c)

/* PRM.WKUP_PRM register offsets */
#define AM33XX_RM_WKUP_RSTCTRL_OFFSET		0x0000
#define AM33XX_RM_WKUP_RSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_WKUP_MOD, 0x0000)
#define AM33XX_PM_WKUP_PWRSTCTRL_OFFSET		0x0004
#define AM33XX_PM_WKUP_PWRSTCTRL		AM33XX_PRM_REGADDR(AM33XX_PRM_WKUP_MOD, 0x0004)
#define AM33XX_PM_WKUP_PWRSTST_OFFSET		0x0008
#define AM33XX_PM_WKUP_PWRSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_WKUP_MOD, 0x0008)
#define AM33XX_RM_WKUP_RSTST_OFFSET		0x000c
#define AM33XX_RM_WKUP_RSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_WKUP_MOD, 0x000c)

/* PRM.MPU_PRM register offsets */
#define AM33XX_PM_MPU_PWRSTCTRL_OFFSET		0x0000
#define AM33XX_PM_MPU_PWRSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_MPU_MOD, 0x0000)
#define AM33XX_PM_MPU_PWRSTST_OFFSET		0x0004
#define AM33XX_PM_MPU_PWRSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_MPU_MOD, 0x0004)
#define AM33XX_RM_MPU_RSTST_OFFSET		0x0008
#define AM33XX_RM_MPU_RSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_MPU_MOD, 0x0008)

/* PRM.DEVICE_PRM register offsets */
#define AM33XX_PRM_RSTCTRL_OFFSET		0x0000
#define AM33XX_PRM_RSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x0000)
#define AM33XX_PRM_RSTTIME_OFFSET		0x0004
#define AM33XX_PRM_RSTTIME			AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x0004)
#define AM33XX_PRM_RSTST_OFFSET			0x0008
#define AM33XX_PRM_RSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x0008)
#define AM33XX_PRM_SRAM_COUNT_OFFSET		0x000c
#define AM33XX_PRM_SRAM_COUNT			AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x000c)
#define AM33XX_PRM_LDO_SRAM_CORE_SETUP_OFFSET	0x0010
#define AM33XX_PRM_LDO_SRAM_CORE_SETUP		AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x0010)
#define AM33XX_PRM_LDO_SRAM_CORE_CTRL_OFFSET	0x0014
#define AM33XX_PRM_LDO_SRAM_CORE_CTRL		AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x0014)
#define AM33XX_PRM_LDO_SRAM_MPU_SETUP_OFFSET	0x0018
#define AM33XX_PRM_LDO_SRAM_MPU_SETUP		AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x0018)
#define AM33XX_PRM_LDO_SRAM_MPU_CTRL_OFFSET	0x001c
#define AM33XX_PRM_LDO_SRAM_MPU_CTRL		AM33XX_PRM_REGADDR(AM33XX_PRM_DEVICE_MOD, 0x001c)

/* PRM.RTC_PRM register offsets */
#define AM33XX_PM_RTC_PWRSTCTRL_OFFSET		0x0000
#define AM33XX_PM_RTC_PWRSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_RTC_MOD, 0x0000)
#define AM33XX_PM_RTC_PWRSTST_OFFSET		0x0004
#define AM33XX_PM_RTC_PWRSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_RTC_MOD, 0x0004)

/* PRM.GFX_PRM register offsets */
#define AM33XX_PM_GFX_PWRSTCTRL_OFFSET		0x0000
#define AM33XX_PM_GFX_PWRSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_GFX_MOD, 0x0000)
#define AM33XX_RM_GFX_RSTCTRL_OFFSET		0x0004
#define AM33XX_RM_GFX_RSTCTRL			AM33XX_PRM_REGADDR(AM33XX_PRM_GFX_MOD, 0x0004)
#define AM33XX_PM_GFX_PWRSTST_OFFSET		0x0010
#define AM33XX_PM_GFX_PWRSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_GFX_MOD, 0x0010)
#define AM33XX_RM_GFX_RSTST_OFFSET		0x0014
#define AM33XX_RM_GFX_RSTST			AM33XX_PRM_REGADDR(AM33XX_PRM_GFX_MOD, 0x0014)

/* PRM.CEFUSE_PRM register offsets */
#define AM33XX_PM_CEFUSE_PWRSTCTRL_OFFSET	0x0000
#define AM33XX_PM_CEFUSE_PWRSTCTRL		AM33XX_PRM_REGADDR(AM33XX_PRM_CEFUSE_MOD, 0x0000)
#define AM33XX_PM_CEFUSE_PWRSTST_OFFSET		0x0004
#define AM33XX_PM_CEFUSE_PWRSTST		AM33XX_PRM_REGADDR(AM33XX_PRM_CEFUSE_MOD, 0x0004)

#ifndef __ASSEMBLER__
int am33xx_prm_init(const struct omap_prcm_init_data *data);

#endif /* ASSEMBLER */
#endif
