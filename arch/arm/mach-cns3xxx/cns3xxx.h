<<<<<<< HEAD
/*
 * Copyright 2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2008 Cavium Networks
>>>>>>> upstream/android-13
 */

#ifndef __MACH_BOARD_CNS3XXXH
#define __MACH_BOARD_CNS3XXXH

/*
 * Memory map
 */
#define CNS3XXX_FLASH_BASE			0x10000000	/* Flash/SRAM Memory Bank 0 */
#define CNS3XXX_FLASH_SIZE			SZ_256M

#define CNS3XXX_DDR2SDRAM_BASE			0x20000000	/* DDR2 SDRAM Memory */

#define CNS3XXX_SPI_FLASH_BASE			0x60000000	/* SPI Serial Flash Memory */

#define CNS3XXX_SWITCH_BASE			0x70000000	/* Switch and HNAT Control */

#define CNS3XXX_PPE_BASE			0x70001000	/* HANT	*/

#define CNS3XXX_EMBEDDED_SRAM_BASE		0x70002000	/* HANT Embedded SRAM */

#define CNS3XXX_SSP_BASE			0x71000000	/* Synchronous Serial Port - SPI/PCM/I2C */

#define CNS3XXX_DMC_BASE			0x72000000	/* DMC Control (DDR2 SDRAM) */

#define CNS3XXX_SMC_BASE			0x73000000	/* SMC Control */

#define SMC_MEMC_STATUS_OFFSET			0x000
#define SMC_MEMIF_CFG_OFFSET			0x004
#define SMC_MEMC_CFG_SET_OFFSET			0x008
#define SMC_MEMC_CFG_CLR_OFFSET			0x00C
#define SMC_DIRECT_CMD_OFFSET			0x010
#define SMC_SET_CYCLES_OFFSET			0x014
#define SMC_SET_OPMODE_OFFSET			0x018
#define SMC_REFRESH_PERIOD_0_OFFSET		0x020
#define SMC_REFRESH_PERIOD_1_OFFSET		0x024
#define SMC_SRAM_CYCLES0_0_OFFSET		0x100
#define SMC_NAND_CYCLES0_0_OFFSET		0x100
#define SMC_OPMODE0_0_OFFSET			0x104
#define SMC_SRAM_CYCLES0_1_OFFSET		0x120
#define SMC_NAND_CYCLES0_1_OFFSET		0x120
#define SMC_OPMODE0_1_OFFSET			0x124
#define SMC_USER_STATUS_OFFSET			0x200
#define SMC_USER_CONFIG_OFFSET			0x204
#define SMC_ECC_STATUS_OFFSET			0x300
#define SMC_ECC_MEMCFG_OFFSET			0x304
#define SMC_ECC_MEMCOMMAND1_OFFSET		0x308
#define SMC_ECC_MEMCOMMAND2_OFFSET		0x30C
#define SMC_ECC_ADDR0_OFFSET			0x310
#define SMC_ECC_ADDR1_OFFSET			0x314
#define SMC_ECC_VALUE0_OFFSET			0x318
#define SMC_ECC_VALUE1_OFFSET			0x31C
#define SMC_ECC_VALUE2_OFFSET			0x320
#define SMC_ECC_VALUE3_OFFSET			0x324
#define SMC_PERIPH_ID_0_OFFSET			0xFE0
#define SMC_PERIPH_ID_1_OFFSET			0xFE4
#define SMC_PERIPH_ID_2_OFFSET			0xFE8
#define SMC_PERIPH_ID_3_OFFSET			0xFEC
#define SMC_PCELL_ID_0_OFFSET			0xFF0
#define SMC_PCELL_ID_1_OFFSET			0xFF4
#define SMC_PCELL_ID_2_OFFSET			0xFF8
#define SMC_PCELL_ID_3_OFFSET			0xFFC

#define CNS3XXX_GPIOA_BASE			0x74000000	/* GPIO port A */

#define CNS3XXX_GPIOB_BASE			0x74800000	/* GPIO port B */

#define CNS3XXX_RTC_BASE			0x75000000	/* Real Time Clock */

#define RTC_SEC_OFFSET				0x00
#define RTC_MIN_OFFSET				0x04
#define RTC_HOUR_OFFSET				0x08
#define RTC_DAY_OFFSET				0x0C
#define RTC_SEC_ALM_OFFSET			0x10
#define RTC_MIN_ALM_OFFSET			0x14
#define RTC_HOUR_ALM_OFFSET			0x18
#define RTC_REC_OFFSET				0x1C
#define RTC_CTRL_OFFSET				0x20
#define RTC_INTR_STS_OFFSET			0x34

#define CNS3XXX_MISC_BASE			0x76000000	/* Misc Control */
#define CNS3XXX_MISC_BASE_VIRT			0xFB000000	/* Misc Control */

#define CNS3XXX_PM_BASE				0x77000000	/* Power Management Control */
#define CNS3XXX_PM_BASE_VIRT			0xFB001000

#define PM_CLK_GATE_OFFSET			0x00
#define PM_SOFT_RST_OFFSET			0x04
#define PM_HS_CFG_OFFSET			0x08
#define PM_CACTIVE_STA_OFFSET			0x0C
#define PM_PWR_STA_OFFSET			0x10
#define PM_SYS_CLK_CTRL_OFFSET			0x14
#define PM_PLL_LCD_I2S_CTRL_OFFSET		0x18
#define PM_PLL_HM_PD_OFFSET			0x1C

#define CNS3XXX_UART0_BASE			0x78000000	/* UART 0 */
#define CNS3XXX_UART0_BASE_VIRT			0xFB002000

#define CNS3XXX_UART1_BASE			0x78400000	/* UART 1 */

#define CNS3XXX_UART2_BASE			0x78800000	/* UART 2 */

#define CNS3XXX_DMAC_BASE			0x79000000	/* Generic DMA Control */

#define CNS3XXX_CORESIGHT_BASE			0x7A000000	/* CoreSight */

#define CNS3XXX_CRYPTO_BASE			0x7B000000	/* Crypto */

#define CNS3XXX_I2S_BASE			0x7C000000	/* I2S */

#define CNS3XXX_TIMER1_2_3_BASE			0x7C800000	/* Timer */
#define CNS3XXX_TIMER1_2_3_BASE_VIRT		0xFB003000

#define TIMER1_COUNTER_OFFSET			0x00
#define TIMER1_AUTO_RELOAD_OFFSET		0x04
#define TIMER1_MATCH_V1_OFFSET			0x08
#define TIMER1_MATCH_V2_OFFSET			0x0C

#define TIMER2_COUNTER_OFFSET			0x10
#define TIMER2_AUTO_RELOAD_OFFSET		0x14
#define TIMER2_MATCH_V1_OFFSET			0x18
#define TIMER2_MATCH_V2_OFFSET			0x1C

#define TIMER1_2_CONTROL_OFFSET			0x30
#define TIMER1_2_INTERRUPT_STATUS_OFFSET	0x34
#define TIMER1_2_INTERRUPT_MASK_OFFSET		0x38

#define TIMER_FREERUN_OFFSET			0x40
#define TIMER_FREERUN_CONTROL_OFFSET		0x44

#define CNS3XXX_HCIE_BASE			0x7D000000	/* HCIE Control */

#define CNS3XXX_RAID_BASE			0x7E000000	/* RAID Control */

#define CNS3XXX_AXI_IXC_BASE			0x7F000000	/* AXI IXC */

#define CNS3XXX_CLCD_BASE			0x80000000	/* LCD Control */

#define CNS3XXX_USBOTG_BASE			0x81000000	/* USB OTG Control */

#define CNS3XXX_USB_BASE			0x82000000	/* USB Host Control */

#define CNS3XXX_SATA2_BASE			0x83000000	/* SATA */
#define CNS3XXX_SATA2_SIZE			SZ_16M

#define CNS3XXX_CAMERA_BASE			0x84000000	/* Camera Interface */

#define CNS3XXX_SDIO_BASE			0x85000000	/* SDIO */

#define CNS3XXX_I2S_TDM_BASE			0x86000000	/* I2S TDM */

#define CNS3XXX_2DG_BASE			0x87000000	/* 2D Graphic Control */

#define CNS3XXX_USB_OHCI_BASE			0x88000000	/* USB OHCI */

#define CNS3XXX_L2C_BASE			0x92000000	/* L2 Cache Control */

#define CNS3XXX_PCIE0_MEM_BASE			0xA0000000	/* PCIe Port 0 IO/Memory Space */

#define CNS3XXX_PCIE0_HOST_BASE			0xAB000000	/* PCIe Port 0 RC Base */
#define CNS3XXX_PCIE0_HOST_BASE_VIRT		0xE1000000

#define CNS3XXX_PCIE0_IO_BASE			0xAC000000	/* PCIe Port 0 */

#define CNS3XXX_PCIE0_CFG0_BASE			0xAD000000	/* PCIe Port 0 CFG Type 0 */
#define CNS3XXX_PCIE0_CFG0_BASE_VIRT		0xE3000000

#define CNS3XXX_PCIE0_CFG1_BASE			0xAE000000	/* PCIe Port 0 CFG Type 1 */
#define CNS3XXX_PCIE0_CFG1_BASE_VIRT		0xE4000000

#define CNS3XXX_PCIE0_MSG_BASE			0xAF000000	/* PCIe Port 0 Message Space */

#define CNS3XXX_PCIE1_MEM_BASE			0xB0000000	/* PCIe Port 1 IO/Memory Space */

#define CNS3XXX_PCIE1_HOST_BASE			0xBB000000	/* PCIe Port 1 RC Base */
#define CNS3XXX_PCIE1_HOST_BASE_VIRT		0xE9000000

#define CNS3XXX_PCIE1_IO_BASE			0xBC000000	/* PCIe Port 1 */

#define CNS3XXX_PCIE1_CFG0_BASE			0xBD000000	/* PCIe Port 1 CFG Type 0 */
#define CNS3XXX_PCIE1_CFG0_BASE_VIRT		0xEB000000

#define CNS3XXX_PCIE1_CFG1_BASE			0xBE000000	/* PCIe Port 1 CFG Type 1 */
#define CNS3XXX_PCIE1_CFG1_BASE_VIRT		0xEC000000

#define CNS3XXX_PCIE1_MSG_BASE			0xBF000000	/* PCIe Port 1 Message Space */

/*
 * Testchip peripheral and fpga gic regions
 */
#define CNS3XXX_TC11MP_SCU_BASE			0x90000000	/* IRQ, Test chip */
#define CNS3XXX_TC11MP_SCU_BASE_VIRT		0xFB004000

#define CNS3XXX_TC11MP_GIC_CPU_BASE		0x90000100	/* Test chip interrupt controller CPU interface */
#define CNS3XXX_TC11MP_GIC_CPU_BASE_VIRT	(CNS3XXX_TC11MP_SCU_BASE_VIRT + 0x100)

#define CNS3XXX_TC11MP_TWD_BASE			0x90000600
#define CNS3XXX_TC11MP_TWD_BASE_VIRT		(CNS3XXX_TC11MP_SCU_BASE_VIRT + 0x600)

#define CNS3XXX_TC11MP_GIC_DIST_BASE		0x90001000	/* Test chip interrupt controller distributor */
#define CNS3XXX_TC11MP_GIC_DIST_BASE_VIRT	(CNS3XXX_TC11MP_SCU_BASE_VIRT + 0x1000)

#define CNS3XXX_TC11MP_L220_BASE		0x92002000	/* L220 registers */

/*
 * Misc block
 */
#define MISC_MEM_MAP(offs) (void __iomem *)(CNS3XXX_MISC_BASE_VIRT + (offs))

#define MISC_MEMORY_REMAP_REG			MISC_MEM_MAP(0x00)
#define MISC_CHIP_CONFIG_REG			MISC_MEM_MAP(0x04)
#define MISC_DEBUG_PROBE_DATA_REG		MISC_MEM_MAP(0x08)
#define MISC_DEBUG_PROBE_SELECTION_REG		MISC_MEM_MAP(0x0C)
#define MISC_IO_PIN_FUNC_SELECTION_REG		MISC_MEM_MAP(0x10)
#define MISC_GPIOA_PIN_ENABLE_REG		MISC_MEM_MAP(0x14)
#define MISC_GPIOB_PIN_ENABLE_REG		MISC_MEM_MAP(0x18)
#define MISC_IO_PAD_DRIVE_STRENGTH_CTRL_A	MISC_MEM_MAP(0x1C)
#define MISC_IO_PAD_DRIVE_STRENGTH_CTRL_B	MISC_MEM_MAP(0x20)
#define MISC_GPIOA_15_0_PULL_CTRL_REG		MISC_MEM_MAP(0x24)
#define MISC_GPIOA_16_31_PULL_CTRL_REG		MISC_MEM_MAP(0x28)
#define MISC_GPIOB_15_0_PULL_CTRL_REG		MISC_MEM_MAP(0x2C)
#define MISC_GPIOB_16_31_PULL_CTRL_REG		MISC_MEM_MAP(0x30)
#define MISC_IO_PULL_CTRL_REG			MISC_MEM_MAP(0x34)
#define MISC_E_FUSE_31_0_REG			MISC_MEM_MAP(0x40)
#define MISC_E_FUSE_63_32_REG			MISC_MEM_MAP(0x44)
#define MISC_E_FUSE_95_64_REG			MISC_MEM_MAP(0x48)
#define MISC_E_FUSE_127_96_REG			MISC_MEM_MAP(0x4C)
#define MISC_SOFTWARE_TEST_1_REG		MISC_MEM_MAP(0x50)
#define MISC_SOFTWARE_TEST_2_REG		MISC_MEM_MAP(0x54)

#define MISC_SATA_POWER_MODE			MISC_MEM_MAP(0x310)

#define MISC_USB_CFG_REG			MISC_MEM_MAP(0x800)
#define MISC_USB_STS_REG			MISC_MEM_MAP(0x804)
#define MISC_USBPHY00_CFG_REG			MISC_MEM_MAP(0x808)
#define MISC_USBPHY01_CFG_REG			MISC_MEM_MAP(0x80c)
#define MISC_USBPHY10_CFG_REG			MISC_MEM_MAP(0x810)
#define MISC_USBPHY11_CFG_REG			MISC_MEM_MAP(0x814)

#define MISC_PCIEPHY_CMCTL(x)			MISC_MEM_MAP(0x900 + (x) * 0x004)
#define MISC_PCIEPHY_CTL(x)			MISC_MEM_MAP(0x940 + (x) * 0x100)
#define MISC_PCIE_AXIS_AWMISC(x)		MISC_MEM_MAP(0x944 + (x) * 0x100)
#define MISC_PCIE_AXIS_ARMISC(x)		MISC_MEM_MAP(0x948 + (x) * 0x100)
#define MISC_PCIE_AXIS_RMISC(x)			MISC_MEM_MAP(0x94C + (x) * 0x100)
#define MISC_PCIE_AXIS_BMISC(x)			MISC_MEM_MAP(0x950 + (x) * 0x100)
#define MISC_PCIE_AXIM_RMISC(x)			MISC_MEM_MAP(0x954 + (x) * 0x100)
#define MISC_PCIE_AXIM_BMISC(x)			MISC_MEM_MAP(0x958 + (x) * 0x100)
#define MISC_PCIE_CTRL(x)			MISC_MEM_MAP(0x95C + (x) * 0x100)
#define MISC_PCIE_PM_DEBUG(x)			MISC_MEM_MAP(0x960 + (x) * 0x100)
#define MISC_PCIE_RFC_DEBUG(x)			MISC_MEM_MAP(0x964 + (x) * 0x100)
#define MISC_PCIE_CXPL_DEBUGL(x)		MISC_MEM_MAP(0x968 + (x) * 0x100)
#define MISC_PCIE_CXPL_DEBUGH(x)		MISC_MEM_MAP(0x96C + (x) * 0x100)
#define MISC_PCIE_DIAG_DEBUGH(x)		MISC_MEM_MAP(0x970 + (x) * 0x100)
#define MISC_PCIE_W1CLR(x)			MISC_MEM_MAP(0x974 + (x) * 0x100)
#define MISC_PCIE_INT_MASK(x)			MISC_MEM_MAP(0x978 + (x) * 0x100)
#define MISC_PCIE_INT_STATUS(x)			MISC_MEM_MAP(0x97C + (x) * 0x100)

/*
 * Power management and clock control
 */
#define PMU_MEM_MAP(offs) (void __iomem *)(CNS3XXX_PM_BASE_VIRT + (offs))

#define PM_CLK_GATE_REG					PMU_MEM_MAP(0x000)
#define PM_SOFT_RST_REG					PMU_MEM_MAP(0x004)
#define PM_HS_CFG_REG					PMU_MEM_MAP(0x008)
#define PM_CACTIVE_STA_REG				PMU_MEM_MAP(0x00C)
#define PM_PWR_STA_REG					PMU_MEM_MAP(0x010)
#define PM_CLK_CTRL_REG					PMU_MEM_MAP(0x014)
#define PM_PLL_LCD_I2S_CTRL_REG				PMU_MEM_MAP(0x018)
#define PM_PLL_HM_PD_CTRL_REG				PMU_MEM_MAP(0x01C)
#define PM_REGULAT_CTRL_REG				PMU_MEM_MAP(0x020)
#define PM_WDT_CTRL_REG					PMU_MEM_MAP(0x024)
#define PM_WU_CTRL0_REG					PMU_MEM_MAP(0x028)
#define PM_WU_CTRL1_REG					PMU_MEM_MAP(0x02C)
#define PM_CSR_REG					PMU_MEM_MAP(0x030)

/* PM_CLK_GATE_REG */
#define PM_CLK_GATE_REG_OFFSET_SDIO			(25)
#define PM_CLK_GATE_REG_OFFSET_GPU			(24)
#define PM_CLK_GATE_REG_OFFSET_CIM			(23)
#define PM_CLK_GATE_REG_OFFSET_LCDC			(22)
#define PM_CLK_GATE_REG_OFFSET_I2S			(21)
#define PM_CLK_GATE_REG_OFFSET_RAID			(20)
#define PM_CLK_GATE_REG_OFFSET_SATA			(19)
#define PM_CLK_GATE_REG_OFFSET_PCIE(x)			(17 + (x))
#define PM_CLK_GATE_REG_OFFSET_USB_HOST			(16)
#define PM_CLK_GATE_REG_OFFSET_USB_OTG			(15)
#define PM_CLK_GATE_REG_OFFSET_TIMER			(14)
#define PM_CLK_GATE_REG_OFFSET_CRYPTO			(13)
#define PM_CLK_GATE_REG_OFFSET_HCIE			(12)
#define PM_CLK_GATE_REG_OFFSET_SWITCH			(11)
#define PM_CLK_GATE_REG_OFFSET_GPIO			(10)
#define PM_CLK_GATE_REG_OFFSET_UART3			(9)
#define PM_CLK_GATE_REG_OFFSET_UART2			(8)
#define PM_CLK_GATE_REG_OFFSET_UART1			(7)
#define PM_CLK_GATE_REG_OFFSET_RTC			(5)
#define PM_CLK_GATE_REG_OFFSET_GDMA			(4)
#define PM_CLK_GATE_REG_OFFSET_SPI_PCM_I2C		(3)
#define PM_CLK_GATE_REG_OFFSET_SMC_NFI			(1)
#define PM_CLK_GATE_REG_MASK				(0x03FFFFBA)

/* PM_SOFT_RST_REG */
#define PM_SOFT_RST_REG_OFFST_WARM_RST_FLAG		(31)
#define PM_SOFT_RST_REG_OFFST_CPU1			(29)
#define PM_SOFT_RST_REG_OFFST_CPU0			(28)
#define PM_SOFT_RST_REG_OFFST_SDIO			(25)
#define PM_SOFT_RST_REG_OFFST_GPU			(24)
#define PM_SOFT_RST_REG_OFFST_CIM			(23)
#define PM_SOFT_RST_REG_OFFST_LCDC			(22)
#define PM_SOFT_RST_REG_OFFST_I2S			(21)
#define PM_SOFT_RST_REG_OFFST_RAID			(20)
#define PM_SOFT_RST_REG_OFFST_SATA			(19)
#define PM_SOFT_RST_REG_OFFST_PCIE(x)			(17 + (x))
#define PM_SOFT_RST_REG_OFFST_USB_HOST			(16)
#define PM_SOFT_RST_REG_OFFST_USB_OTG			(15)
#define PM_SOFT_RST_REG_OFFST_TIMER			(14)
#define PM_SOFT_RST_REG_OFFST_CRYPTO			(13)
#define PM_SOFT_RST_REG_OFFST_HCIE			(12)
#define PM_SOFT_RST_REG_OFFST_SWITCH			(11)
#define PM_SOFT_RST_REG_OFFST_GPIO			(10)
#define PM_SOFT_RST_REG_OFFST_UART3			(9)
#define PM_SOFT_RST_REG_OFFST_UART2			(8)
#define PM_SOFT_RST_REG_OFFST_UART1			(7)
#define PM_SOFT_RST_REG_OFFST_RTC			(5)
#define PM_SOFT_RST_REG_OFFST_GDMA			(4)
#define PM_SOFT_RST_REG_OFFST_SPI_PCM_I2C		(3)
#define PM_SOFT_RST_REG_OFFST_DMC			(2)
#define PM_SOFT_RST_REG_OFFST_SMC_NFI			(1)
#define PM_SOFT_RST_REG_OFFST_GLOBAL			(0)
#define PM_SOFT_RST_REG_MASK				(0xF3FFFFBF)

/* PMHS_CFG_REG */
#define PM_HS_CFG_REG_OFFSET_SDIO			(25)
#define PM_HS_CFG_REG_OFFSET_GPU			(24)
#define PM_HS_CFG_REG_OFFSET_CIM			(23)
#define PM_HS_CFG_REG_OFFSET_LCDC			(22)
#define PM_HS_CFG_REG_OFFSET_I2S			(21)
#define PM_HS_CFG_REG_OFFSET_RAID			(20)
#define PM_HS_CFG_REG_OFFSET_SATA			(19)
#define PM_HS_CFG_REG_OFFSET_PCIE1			(18)
#define PM_HS_CFG_REG_OFFSET_PCIE0			(17)
#define PM_HS_CFG_REG_OFFSET_USB_HOST			(16)
#define PM_HS_CFG_REG_OFFSET_USB_OTG			(15)
#define PM_HS_CFG_REG_OFFSET_TIMER			(14)
#define PM_HS_CFG_REG_OFFSET_CRYPTO			(13)
#define PM_HS_CFG_REG_OFFSET_HCIE			(12)
#define PM_HS_CFG_REG_OFFSET_SWITCH			(11)
#define PM_HS_CFG_REG_OFFSET_GPIO			(10)
#define PM_HS_CFG_REG_OFFSET_UART3			(9)
#define PM_HS_CFG_REG_OFFSET_UART2			(8)
#define PM_HS_CFG_REG_OFFSET_UART1			(7)
#define PM_HS_CFG_REG_OFFSET_RTC			(5)
#define PM_HS_CFG_REG_OFFSET_GDMA			(4)
#define PM_HS_CFG_REG_OFFSET_SPI_PCM_I2S		(3)
#define PM_HS_CFG_REG_OFFSET_DMC			(2)
#define PM_HS_CFG_REG_OFFSET_SMC_NFI			(1)
#define PM_HS_CFG_REG_MASK				(0x03FFFFBE)
#define PM_HS_CFG_REG_MASK_SUPPORT			(0x01100806)

/* PM_CACTIVE_STA_REG */
#define PM_CACTIVE_STA_REG_OFFSET_SDIO			(25)
#define PM_CACTIVE_STA_REG_OFFSET_GPU			(24)
#define PM_CACTIVE_STA_REG_OFFSET_CIM			(23)
#define PM_CACTIVE_STA_REG_OFFSET_LCDC			(22)
#define PM_CACTIVE_STA_REG_OFFSET_I2S			(21)
#define PM_CACTIVE_STA_REG_OFFSET_RAID			(20)
#define PM_CACTIVE_STA_REG_OFFSET_SATA			(19)
#define PM_CACTIVE_STA_REG_OFFSET_PCIE1			(18)
#define PM_CACTIVE_STA_REG_OFFSET_PCIE0			(17)
#define PM_CACTIVE_STA_REG_OFFSET_USB_HOST		(16)
#define PM_CACTIVE_STA_REG_OFFSET_USB_OTG		(15)
#define PM_CACTIVE_STA_REG_OFFSET_TIMER			(14)
#define PM_CACTIVE_STA_REG_OFFSET_CRYPTO		(13)
#define PM_CACTIVE_STA_REG_OFFSET_HCIE			(12)
#define PM_CACTIVE_STA_REG_OFFSET_SWITCH		(11)
#define PM_CACTIVE_STA_REG_OFFSET_GPIO			(10)
#define PM_CACTIVE_STA_REG_OFFSET_UART3			(9)
#define PM_CACTIVE_STA_REG_OFFSET_UART2			(8)
#define PM_CACTIVE_STA_REG_OFFSET_UART1			(7)
#define PM_CACTIVE_STA_REG_OFFSET_RTC			(5)
#define PM_CACTIVE_STA_REG_OFFSET_GDMA			(4)
#define PM_CACTIVE_STA_REG_OFFSET_SPI_PCM_I2S		(3)
#define PM_CACTIVE_STA_REG_OFFSET_DMC			(2)
#define PM_CACTIVE_STA_REG_OFFSET_SMC_NFI		(1)
#define PM_CACTIVE_STA_REG_MASK				(0x03FFFFBE)

/* PM_PWR_STA_REG */
#define PM_PWR_STA_REG_REG_OFFSET_SDIO			(25)
#define PM_PWR_STA_REG_REG_OFFSET_GPU			(24)
#define PM_PWR_STA_REG_REG_OFFSET_CIM			(23)
#define PM_PWR_STA_REG_REG_OFFSET_LCDC			(22)
#define PM_PWR_STA_REG_REG_OFFSET_I2S			(21)
#define PM_PWR_STA_REG_REG_OFFSET_RAID			(20)
#define PM_PWR_STA_REG_REG_OFFSET_SATA			(19)
#define PM_PWR_STA_REG_REG_OFFSET_PCIE1			(18)
#define PM_PWR_STA_REG_REG_OFFSET_PCIE0			(17)
#define PM_PWR_STA_REG_REG_OFFSET_USB_HOST		(16)
#define PM_PWR_STA_REG_REG_OFFSET_USB_OTG		(15)
#define PM_PWR_STA_REG_REG_OFFSET_TIMER			(14)
#define PM_PWR_STA_REG_REG_OFFSET_CRYPTO		(13)
#define PM_PWR_STA_REG_REG_OFFSET_HCIE			(12)
#define PM_PWR_STA_REG_REG_OFFSET_SWITCH		(11)
#define PM_PWR_STA_REG_REG_OFFSET_GPIO			(10)
#define PM_PWR_STA_REG_REG_OFFSET_UART3			(9)
#define PM_PWR_STA_REG_REG_OFFSET_UART2			(8)
#define PM_PWR_STA_REG_REG_OFFSET_UART1			(7)
#define PM_PWR_STA_REG_REG_OFFSET_RTC			(5)
#define PM_PWR_STA_REG_REG_OFFSET_GDMA			(4)
#define PM_PWR_STA_REG_REG_OFFSET_SPI_PCM_I2S		(3)
#define PM_PWR_STA_REG_REG_OFFSET_DMC			(2)
#define PM_PWR_STA_REG_REG_OFFSET_SMC_NFI		(1)
#define PM_PWR_STA_REG_REG_MASK				(0x03FFFFBE)

/* PM_CLK_CTRL_REG */
#define PM_CLK_CTRL_REG_OFFSET_I2S_MCLK			(31)
#define PM_CLK_CTRL_REG_OFFSET_DDR2_CHG_EN		(30)
#define PM_CLK_CTRL_REG_OFFSET_PCIE_REF1_EN		(29)
#define PM_CLK_CTRL_REG_OFFSET_PCIE_REF0_EN		(28)
#define PM_CLK_CTRL_REG_OFFSET_TIMER_SIM_MODE		(27)
#define PM_CLK_CTRL_REG_OFFSET_I2SCLK_DIV		(24)
#define PM_CLK_CTRL_REG_OFFSET_I2SCLK_SEL		(22)
#define PM_CLK_CTRL_REG_OFFSET_CLKOUT_DIV		(20)
#define PM_CLK_CTRL_REG_OFFSET_CLKOUT_SEL		(16)
#define PM_CLK_CTRL_REG_OFFSET_MDC_DIV			(14)
#define PM_CLK_CTRL_REG_OFFSET_CRYPTO_CLK_SEL		(12)
#define PM_CLK_CTRL_REG_OFFSET_CPU_PWR_MODE		(9)
#define PM_CLK_CTRL_REG_OFFSET_PLL_DDR2_SEL		(7)
#define PM_CLK_CTRL_REG_OFFSET_DIV_IMMEDIATE		(6)
#define PM_CLK_CTRL_REG_OFFSET_CPU_CLK_DIV		(4)
#define PM_CLK_CTRL_REG_OFFSET_PLL_CPU_SEL		(0)

#define PM_CPU_CLK_DIV(DIV) { \
	PM_CLK_CTRL_REG &= ~((0x3) << PM_CLK_CTRL_REG_OFFSET_CPU_CLK_DIV); \
	PM_CLK_CTRL_REG |= (((DIV)&0x3) << PM_CLK_CTRL_REG_OFFSET_CPU_CLK_DIV); \
}

#define PM_PLL_CPU_SEL(CPU) { \
	PM_CLK_CTRL_REG &= ~((0xF) << PM_CLK_CTRL_REG_OFFSET_PLL_CPU_SEL); \
	PM_CLK_CTRL_REG |= (((CPU)&0xF) << PM_CLK_CTRL_REG_OFFSET_PLL_CPU_SEL); \
}

/* PM_PLL_LCD_I2S_CTRL_REG */
#define PM_PLL_LCD_I2S_CTRL_REG_OFFSET_MCLK_SMC_DIV	(22)
#define PM_PLL_LCD_I2S_CTRL_REG_OFFSET_R_SEL		(17)
#define PM_PLL_LCD_I2S_CTRL_REG_OFFSET_PLL_LCD_P	(11)
#define PM_PLL_LCD_I2S_CTRL_REG_OFFSET_PLL_LCD_M	(3)
#define PM_PLL_LCD_I2S_CTRL_REG_OFFSET_PLL_LCD_S	(0)

/* PM_PLL_HM_PD_CTRL_REG */
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_SATA_PHY1		(11)
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_SATA_PHY0		(10)
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_PLL_I2SCD		(6)
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_PLL_I2S		(5)
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_PLL_LCD		(4)
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_PLL_USB		(3)
#define PM_PLL_HM_PD_CTRL_REG_OFFSET_PLL_RGMII		(2)
#define PM_PLL_HM_PD_CTRL_REG_MASK			(0x00000C7C)

/* PM_WDT_CTRL_REG */
#define PM_WDT_CTRL_REG_OFFSET_RESET_CPU_ONLY		(0)

/* PM_CSR_REG - Clock Scaling Register*/
#define PM_CSR_REG_OFFSET_CSR_EN			(30)
#define PM_CSR_REG_OFFSET_CSR_NUM			(0)

#define CNS3XXX_PWR_CLK_EN(BLOCK) (0x1<<PM_CLK_GATE_REG_OFFSET_##BLOCK)

/* Software reset*/
#define CNS3XXX_PWR_SOFTWARE_RST(BLOCK) (0x1<<PM_SOFT_RST_REG_OFFST_##BLOCK)

/*
 * CNS3XXX support several power saving mode as following,
 * DFS, IDLE, HALT, DOZE, SLEEP, Hibernate
 */
#define CNS3XXX_PWR_CPU_MODE_DFS			(0)
#define CNS3XXX_PWR_CPU_MODE_IDLE			(1)
#define CNS3XXX_PWR_CPU_MODE_HALT			(2)
#define CNS3XXX_PWR_CPU_MODE_DOZE			(3)
#define CNS3XXX_PWR_CPU_MODE_SLEEP			(4)
#define CNS3XXX_PWR_CPU_MODE_HIBERNATE			(5)

#define CNS3XXX_PWR_PLL(BLOCK)	(0x1<<PM_PLL_HM_PD_CTRL_REG_OFFSET_##BLOCK)
#define CNS3XXX_PWR_PLL_ALL	PM_PLL_HM_PD_CTRL_REG_MASK

/* Change CPU frequency and divider */
#define CNS3XXX_PWR_PLL_CPU_300MHZ			(0)
#define CNS3XXX_PWR_PLL_CPU_333MHZ			(1)
#define CNS3XXX_PWR_PLL_CPU_366MHZ			(2)
#define CNS3XXX_PWR_PLL_CPU_400MHZ			(3)
#define CNS3XXX_PWR_PLL_CPU_433MHZ			(4)
#define CNS3XXX_PWR_PLL_CPU_466MHZ			(5)
#define CNS3XXX_PWR_PLL_CPU_500MHZ			(6)
#define CNS3XXX_PWR_PLL_CPU_533MHZ			(7)
#define CNS3XXX_PWR_PLL_CPU_566MHZ			(8)
#define CNS3XXX_PWR_PLL_CPU_600MHZ			(9)
#define CNS3XXX_PWR_PLL_CPU_633MHZ			(10)
#define CNS3XXX_PWR_PLL_CPU_666MHZ			(11)
#define CNS3XXX_PWR_PLL_CPU_700MHZ			(12)

#define CNS3XXX_PWR_CPU_CLK_DIV_BY1			(0)
#define CNS3XXX_PWR_CPU_CLK_DIV_BY2			(1)
#define CNS3XXX_PWR_CPU_CLK_DIV_BY4			(2)

/* Change DDR2 frequency */
#define CNS3XXX_PWR_PLL_DDR2_200MHZ			(0)
#define CNS3XXX_PWR_PLL_DDR2_266MHZ			(1)
#define CNS3XXX_PWR_PLL_DDR2_333MHZ			(2)
#define CNS3XXX_PWR_PLL_DDR2_400MHZ			(3)

void cns3xxx_pwr_soft_rst(unsigned int block);
void cns3xxx_pwr_clk_en(unsigned int block);
int cns3xxx_cpu_clock(void);

/*
 * ARM11 MPCore interrupt sources (primary GIC)
 */
#define IRQ_TC11MP_GIC_START	32

#define IRQ_CNS3XXX_PMU			(IRQ_TC11MP_GIC_START + 0)
#define IRQ_CNS3XXX_SDIO		(IRQ_TC11MP_GIC_START + 1)
#define IRQ_CNS3XXX_L2CC		(IRQ_TC11MP_GIC_START + 2)
#define IRQ_CNS3XXX_RTC			(IRQ_TC11MP_GIC_START + 3)
#define IRQ_CNS3XXX_I2S			(IRQ_TC11MP_GIC_START + 4)
#define IRQ_CNS3XXX_PCM			(IRQ_TC11MP_GIC_START + 5)
#define IRQ_CNS3XXX_SPI			(IRQ_TC11MP_GIC_START + 6)
#define IRQ_CNS3XXX_I2C			(IRQ_TC11MP_GIC_START + 7)
#define IRQ_CNS3XXX_CIM			(IRQ_TC11MP_GIC_START + 8)
#define IRQ_CNS3XXX_GPU			(IRQ_TC11MP_GIC_START + 9)
#define IRQ_CNS3XXX_LCD			(IRQ_TC11MP_GIC_START + 10)
#define IRQ_CNS3XXX_GPIOA		(IRQ_TC11MP_GIC_START + 11)
#define IRQ_CNS3XXX_GPIOB		(IRQ_TC11MP_GIC_START + 12)
#define IRQ_CNS3XXX_UART0		(IRQ_TC11MP_GIC_START + 13)
#define IRQ_CNS3XXX_UART1		(IRQ_TC11MP_GIC_START + 14)
#define IRQ_CNS3XXX_UART2		(IRQ_TC11MP_GIC_START + 15)
#define IRQ_CNS3XXX_ARM11		(IRQ_TC11MP_GIC_START + 16)

#define IRQ_CNS3XXX_SW_STATUS		(IRQ_TC11MP_GIC_START + 17)
#define IRQ_CNS3XXX_SW_R0TXC		(IRQ_TC11MP_GIC_START + 18)
#define IRQ_CNS3XXX_SW_R0RXC		(IRQ_TC11MP_GIC_START + 19)
#define IRQ_CNS3XXX_SW_R0QE		(IRQ_TC11MP_GIC_START + 20)
#define IRQ_CNS3XXX_SW_R0QF		(IRQ_TC11MP_GIC_START + 21)
#define IRQ_CNS3XXX_SW_R1TXC		(IRQ_TC11MP_GIC_START + 22)
#define IRQ_CNS3XXX_SW_R1RXC		(IRQ_TC11MP_GIC_START + 23)
#define IRQ_CNS3XXX_SW_R1QE		(IRQ_TC11MP_GIC_START + 24)
#define IRQ_CNS3XXX_SW_R1QF		(IRQ_TC11MP_GIC_START + 25)
#define IRQ_CNS3XXX_SW_PPE		(IRQ_TC11MP_GIC_START + 26)

#define IRQ_CNS3XXX_CRYPTO		(IRQ_TC11MP_GIC_START + 27)
#define IRQ_CNS3XXX_HCIE		(IRQ_TC11MP_GIC_START + 28)
#define IRQ_CNS3XXX_PCIE0_DEVICE	(IRQ_TC11MP_GIC_START + 29)
#define IRQ_CNS3XXX_PCIE1_DEVICE	(IRQ_TC11MP_GIC_START + 30)
#define IRQ_CNS3XXX_USB_OTG		(IRQ_TC11MP_GIC_START + 31)
#define IRQ_CNS3XXX_USB_EHCI		(IRQ_TC11MP_GIC_START + 32)
#define IRQ_CNS3XXX_SATA		(IRQ_TC11MP_GIC_START + 33)
#define IRQ_CNS3XXX_RAID		(IRQ_TC11MP_GIC_START + 34)
#define IRQ_CNS3XXX_SMC			(IRQ_TC11MP_GIC_START + 35)

#define IRQ_CNS3XXX_DMAC_ABORT		(IRQ_TC11MP_GIC_START + 36)
#define IRQ_CNS3XXX_DMAC0		(IRQ_TC11MP_GIC_START + 37)
#define IRQ_CNS3XXX_DMAC1		(IRQ_TC11MP_GIC_START + 38)
#define IRQ_CNS3XXX_DMAC2		(IRQ_TC11MP_GIC_START + 39)
#define IRQ_CNS3XXX_DMAC3		(IRQ_TC11MP_GIC_START + 40)
#define IRQ_CNS3XXX_DMAC4		(IRQ_TC11MP_GIC_START + 41)
#define IRQ_CNS3XXX_DMAC5		(IRQ_TC11MP_GIC_START + 42)
#define IRQ_CNS3XXX_DMAC6		(IRQ_TC11MP_GIC_START + 43)
#define IRQ_CNS3XXX_DMAC7		(IRQ_TC11MP_GIC_START + 44)
#define IRQ_CNS3XXX_DMAC8		(IRQ_TC11MP_GIC_START + 45)
#define IRQ_CNS3XXX_DMAC9		(IRQ_TC11MP_GIC_START + 46)
#define IRQ_CNS3XXX_DMAC10		(IRQ_TC11MP_GIC_START + 47)
#define IRQ_CNS3XXX_DMAC11		(IRQ_TC11MP_GIC_START + 48)
#define IRQ_CNS3XXX_DMAC12		(IRQ_TC11MP_GIC_START + 49)
#define IRQ_CNS3XXX_DMAC13		(IRQ_TC11MP_GIC_START + 50)
#define IRQ_CNS3XXX_DMAC14		(IRQ_TC11MP_GIC_START + 51)
#define IRQ_CNS3XXX_DMAC15		(IRQ_TC11MP_GIC_START + 52)
#define IRQ_CNS3XXX_DMAC16		(IRQ_TC11MP_GIC_START + 53)
#define IRQ_CNS3XXX_DMAC17		(IRQ_TC11MP_GIC_START + 54)

#define IRQ_CNS3XXX_PCIE0_RC		(IRQ_TC11MP_GIC_START + 55)
#define IRQ_CNS3XXX_PCIE1_RC		(IRQ_TC11MP_GIC_START + 56)
#define IRQ_CNS3XXX_TIMER0		(IRQ_TC11MP_GIC_START + 57)
#define IRQ_CNS3XXX_TIMER1		(IRQ_TC11MP_GIC_START + 58)
#define IRQ_CNS3XXX_USB_OHCI		(IRQ_TC11MP_GIC_START + 59)
#define IRQ_CNS3XXX_TIMER2		(IRQ_TC11MP_GIC_START + 60)
#define IRQ_CNS3XXX_EXTERNAL_PIN0	(IRQ_TC11MP_GIC_START + 61)
#define IRQ_CNS3XXX_EXTERNAL_PIN1	(IRQ_TC11MP_GIC_START + 62)
#define IRQ_CNS3XXX_EXTERNAL_PIN2	(IRQ_TC11MP_GIC_START + 63)

#define NR_IRQS_CNS3XXX			(IRQ_TC11MP_GIC_START + 64)

#endif	/* __MACH_BOARD_CNS3XXX_H */
