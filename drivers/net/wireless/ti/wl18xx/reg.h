<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * This file is part of wlcore
 *
 * Copyright (C) 2011 Texas Instruments Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef __REG_H__
#define __REG_H__

#define WL18XX_REGISTERS_BASE      0x00800000
#define WL18XX_CODE_BASE           0x00000000
#define WL18XX_DATA_BASE           0x00400000
#define WL18XX_DOUBLE_BUFFER_BASE  0x00600000
#define WL18XX_MCU_KEY_SEARCH_BASE 0x00700000
#define WL18XX_PHY_BASE            0x00900000
#define WL18XX_TOP_OCP_BASE        0x00A00000
#define WL18XX_PACKET_RAM_BASE     0x00B00000
#define WL18XX_HOST_BASE           0x00C00000

#define WL18XX_REGISTERS_DOWN_SIZE 0x0000B000

#define WL18XX_REG_BOOT_PART_START 0x00802000
#define WL18XX_REG_BOOT_PART_SIZE  0x00014578

#define WL18XX_PHY_INIT_MEM_ADDR   0x80926000
#define WL18XX_PHY_END_MEM_ADDR	   0x8093CA44
#define WL18XX_PHY_INIT_MEM_SIZE \
	(WL18XX_PHY_END_MEM_ADDR - WL18XX_PHY_INIT_MEM_ADDR)

#define WL18XX_SDIO_WSPI_BASE		(WL18XX_REGISTERS_BASE)
#define WL18XX_REG_CONFIG_BASE		(WL18XX_REGISTERS_BASE + 0x02000)
#define WL18XX_WGCM_REGS_BASE		(WL18XX_REGISTERS_BASE + 0x03000)
#define WL18XX_ENC_BASE			(WL18XX_REGISTERS_BASE + 0x04000)
#define WL18XX_INTERRUPT_BASE		(WL18XX_REGISTERS_BASE + 0x05000)
#define WL18XX_UART_BASE		(WL18XX_REGISTERS_BASE + 0x06000)
#define WL18XX_WELP_BASE		(WL18XX_REGISTERS_BASE + 0x07000)
#define WL18XX_TCP_CKSM_BASE		(WL18XX_REGISTERS_BASE + 0x08000)
#define WL18XX_FIFO_BASE		(WL18XX_REGISTERS_BASE + 0x09000)
#define WL18XX_OCP_BRIDGE_BASE		(WL18XX_REGISTERS_BASE + 0x0A000)
#define WL18XX_PMAC_RX_BASE		(WL18XX_REGISTERS_BASE + 0x14800)
#define WL18XX_PMAC_ACM_BASE		(WL18XX_REGISTERS_BASE + 0x14C00)
#define WL18XX_PMAC_TX_BASE		(WL18XX_REGISTERS_BASE + 0x15000)
#define WL18XX_PMAC_CSR_BASE		(WL18XX_REGISTERS_BASE + 0x15400)

#define WL18XX_REG_ECPU_CONTROL		(WL18XX_REGISTERS_BASE + 0x02004)
#define WL18XX_REG_INTERRUPT_NO_CLEAR	(WL18XX_REGISTERS_BASE + 0x050E8)
#define WL18XX_REG_INTERRUPT_ACK	(WL18XX_REGISTERS_BASE + 0x050F0)
#define WL18XX_REG_INTERRUPT_TRIG	(WL18XX_REGISTERS_BASE + 0x5074)
#define WL18XX_REG_INTERRUPT_TRIG_H	(WL18XX_REGISTERS_BASE + 0x5078)
#define WL18XX_REG_INTERRUPT_MASK	(WL18XX_REGISTERS_BASE + 0x0050DC)

#define WL18XX_REG_CHIP_ID_B		(WL18XX_REGISTERS_BASE + 0x01542C)

#define WL18XX_SLV_MEM_DATA		(WL18XX_HOST_BASE + 0x0018)
#define WL18XX_SLV_REG_DATA		(WL18XX_HOST_BASE + 0x0008)

/* Scratch Pad registers*/
#define WL18XX_SCR_PAD0			(WL18XX_REGISTERS_BASE + 0x0154EC)
#define WL18XX_SCR_PAD1			(WL18XX_REGISTERS_BASE + 0x0154F0)
#define WL18XX_SCR_PAD2			(WL18XX_REGISTERS_BASE + 0x0154F4)
#define WL18XX_SCR_PAD3			(WL18XX_REGISTERS_BASE + 0x0154F8)
#define WL18XX_SCR_PAD4			(WL18XX_REGISTERS_BASE + 0x0154FC)
#define WL18XX_SCR_PAD4_SET		(WL18XX_REGISTERS_BASE + 0x015504)
#define WL18XX_SCR_PAD4_CLR		(WL18XX_REGISTERS_BASE + 0x015500)
#define WL18XX_SCR_PAD5			(WL18XX_REGISTERS_BASE + 0x015508)
#define WL18XX_SCR_PAD5_SET		(WL18XX_REGISTERS_BASE + 0x015510)
#define WL18XX_SCR_PAD5_CLR		(WL18XX_REGISTERS_BASE + 0x01550C)
#define WL18XX_SCR_PAD6			(WL18XX_REGISTERS_BASE + 0x015514)
#define WL18XX_SCR_PAD7			(WL18XX_REGISTERS_BASE + 0x015518)
#define WL18XX_SCR_PAD8			(WL18XX_REGISTERS_BASE + 0x01551C)
#define WL18XX_SCR_PAD9			(WL18XX_REGISTERS_BASE + 0x015520)

/* Spare registers*/
#define WL18XX_SPARE_A1			(WL18XX_REGISTERS_BASE + 0x002194)
#define WL18XX_SPARE_A2			(WL18XX_REGISTERS_BASE + 0x002198)
#define WL18XX_SPARE_A3			(WL18XX_REGISTERS_BASE + 0x00219C)
#define WL18XX_SPARE_A4			(WL18XX_REGISTERS_BASE + 0x0021A0)
#define WL18XX_SPARE_A5			(WL18XX_REGISTERS_BASE + 0x0021A4)
#define WL18XX_SPARE_A6			(WL18XX_REGISTERS_BASE + 0x0021A8)
#define WL18XX_SPARE_A7			(WL18XX_REGISTERS_BASE + 0x0021AC)
#define WL18XX_SPARE_A8			(WL18XX_REGISTERS_BASE + 0x0021B0)
#define WL18XX_SPARE_B1			(WL18XX_REGISTERS_BASE + 0x015524)
#define WL18XX_SPARE_B2			(WL18XX_REGISTERS_BASE + 0x015528)
#define WL18XX_SPARE_B3			(WL18XX_REGISTERS_BASE + 0x01552C)
#define WL18XX_SPARE_B4			(WL18XX_REGISTERS_BASE + 0x015530)
#define WL18XX_SPARE_B5			(WL18XX_REGISTERS_BASE + 0x015534)
#define WL18XX_SPARE_B6			(WL18XX_REGISTERS_BASE + 0x015538)
#define WL18XX_SPARE_B7			(WL18XX_REGISTERS_BASE + 0x01553C)
#define WL18XX_SPARE_B8			(WL18XX_REGISTERS_BASE + 0x015540)

#define WL18XX_REG_COMMAND_MAILBOX_PTR	(WL18XX_SCR_PAD0)
#define WL18XX_REG_EVENT_MAILBOX_PTR	(WL18XX_SCR_PAD1)
#define WL18XX_EEPROMLESS_IND		(WL18XX_SCR_PAD4)

#define WL18XX_WELP_ARM_COMMAND		(WL18XX_REGISTERS_BASE + 0x7100)
#define WL18XX_ENABLE			(WL18XX_REGISTERS_BASE + 0x01543C)
#define TOP_FN0_CCCR_REG_32		(WL18XX_TOP_OCP_BASE + 0x64)

/* PRCM registers */
#define PLATFORM_DETECTION		0xA0E3E0
#define OCS_EN				0xA02080
#define PRIMARY_CLK_DETECT		0xA020A6
#define PLLSH_COEX_PLL_N		0xA02384
#define PLLSH_COEX_PLL_M		0xA02382
#define PLLSH_COEX_PLL_SWALLOW_EN	0xA0238E
#define PLLSH_WL_PLL_SEL		0xA02398

#define PLLSH_WCS_PLL_N			0xA02362
#define PLLSH_WCS_PLL_M			0xA02360
#define PLLSH_WCS_PLL_Q_FACTOR_CFG_1	0xA02364
#define PLLSH_WCS_PLL_Q_FACTOR_CFG_2	0xA02366
#define PLLSH_WCS_PLL_P_FACTOR_CFG_1	0xA02368
#define PLLSH_WCS_PLL_P_FACTOR_CFG_2	0xA0236A
#define PLLSH_WCS_PLL_SWALLOW_EN	0xA0236C
#define PLLSH_WL_PLL_EN			0xA02392

#define PLLSH_WCS_PLL_Q_FACTOR_CFG_1_MASK	0xFFFF
#define PLLSH_WCS_PLL_Q_FACTOR_CFG_2_MASK	0x007F
#define PLLSH_WCS_PLL_P_FACTOR_CFG_1_MASK	0xFFFF
#define PLLSH_WCS_PLL_P_FACTOR_CFG_2_MASK	0x000F

#define PLLSH_WL_PLL_EN_VAL1		0x7
#define PLLSH_WL_PLL_EN_VAL2		0x2
#define PLLSH_COEX_PLL_SWALLOW_EN_VAL1	0x2
#define PLLSH_COEX_PLL_SWALLOW_EN_VAL2	0x11

#define PLLSH_WCS_PLL_SWALLOW_EN_VAL1	0x1
#define PLLSH_WCS_PLL_SWALLOW_EN_VAL2	0x12

#define PLLSH_WL_PLL_SEL_WCS_PLL	0x0
#define PLLSH_WL_PLL_SEL_COEX_PLL	0x1

#define WL18XX_REG_FUSE_DATA_1_3	0xA0260C
#define WL18XX_PG_VER_MASK		0x70
#define WL18XX_PG_VER_OFFSET		4
#define WL18XX_ROM_VER_MASK		0x3e00
#define WL18XX_ROM_VER_OFFSET		9
#define WL18XX_METAL_VER_MASK		0xC
#define WL18XX_METAL_VER_OFFSET		2
#define WL18XX_NEW_METAL_VER_MASK	0x180
#define WL18XX_NEW_METAL_VER_OFFSET	7

#define WL18XX_PACKAGE_TYPE_OFFSET	13
#define WL18XX_PACKAGE_TYPE_WSP		0

#define WL18XX_REG_FUSE_DATA_2_3	0xA02614
#define WL18XX_RDL_VER_MASK		0x1f00
#define WL18XX_RDL_VER_OFFSET		8

#define WL18XX_REG_FUSE_BD_ADDR_1	0xA02602
#define WL18XX_REG_FUSE_BD_ADDR_2	0xA02606

#define WL18XX_CMD_MBOX_ADDRESS		0xB007B4

#define WL18XX_FW_STATUS_ADDR		0x50F8

#define CHIP_ID_185x_PG10              (0x06030101)
#define CHIP_ID_185x_PG20              (0x06030111)

/*
 * Host Command Interrupt. Setting this bit masks
 * the interrupt that the host issues to inform
 * the FW that it has sent a command
 * to the Wlan hardware Command Mailbox.
 */
#define WL18XX_INTR_TRIG_CMD       BIT(28)

/*
 * Host Event Acknowlegde Interrupt. The host
 * sets this bit to acknowledge that it received
 * the unsolicited information from the event
 * mailbox.
 */
#define WL18XX_INTR_TRIG_EVENT_ACK BIT(29)

/*
 * To boot the firmware in PLT mode we need to write this value in
 * SCR_PAD8 before starting.
 */
#define WL18XX_SCR_PAD8_PLT	0xBABABEBE

enum {
	COMPONENT_NO_SWITCH	= 0x0,
	COMPONENT_2_WAY_SWITCH	= 0x1,
	COMPONENT_3_WAY_SWITCH	= 0x2,
	COMPONENT_MATCHING	= 0x3,
};

enum {
	FEM_NONE	= 0x0,
	FEM_VENDOR_1	= 0x1,
	FEM_VENDOR_2	= 0x2,
	FEM_VENDOR_3	= 0x3,
};

enum {
	BOARD_TYPE_EVB_18XX     = 0,
	BOARD_TYPE_DVP_18XX     = 1,
	BOARD_TYPE_HDK_18XX     = 2,
	BOARD_TYPE_FPGA_18XX    = 3,
	BOARD_TYPE_COM8_18XX    = 4,

	NUM_BOARD_TYPES,
};

enum wl18xx_rdl_num {
	RDL_NONE	= 0,
	RDL_1_HP	= 1,
	RDL_2_SP	= 2,
	RDL_3_HP	= 3,
	RDL_4_SP	= 4,
	RDL_5_SP	= 0x11,
	RDL_6_SP	= 0x12,
	RDL_7_SP	= 0x13,
	RDL_8_SP	= 0x14,

	_RDL_LAST,
	RDL_MAX = _RDL_LAST - 1,
};


/* FPGA_SPARE_1 register - used to change the PHY ATPG clock at boot time */
#define WL18XX_PHY_FPGA_SPARE_1		0x8093CA40

/* command to disable FDSP clock */
#define MEM_FDSP_CLK_120_DISABLE        0x80000000

/* command to set ATPG clock toward FDSP Code RAM rather than its own clock */
#define MEM_FDSP_CODERAM_FUNC_CLK_SEL	0xC0000000

/* command to re-enable FDSP clock */
#define MEM_FDSP_CLK_120_ENABLE		0x40000000

#endif /* __REG_H__ */
