<<<<<<< HEAD
/*
 * CAN bus driver for Bosch M_CAN controller
 *
 * Copyright (C) 2014 Freescale Semiconductor, Inc.
 *	Dong Aisheng <b29396@freescale.com>
 *
 * Bosch M_CAN user manual can be obtained from:
 * http://www.bosch-semiconductors.de/media/pdf_1/ipmodules_1/m_can/
 * mcan_users_manual_v302.pdf
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/clk.h>
#include <linux/delay.h>
=======
// SPDX-License-Identifier: GPL-2.0
// CAN bus driver for Bosch M_CAN controller
// Copyright (C) 2014 Freescale Semiconductor, Inc.
//      Dong Aisheng <b29396@freescale.com>
// Copyright (C) 2018-19 Texas Instruments Incorporated - http://www.ti.com/

/* Bosch M_CAN user manual can be obtained from:
 * https://github.com/linux-can/can-doc/tree/master/m_can
 */

#include <linux/bitfield.h>
>>>>>>> upstream/android-13
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/iopoll.h>
#include <linux/can/dev.h>
#include <linux/pinctrl/consumer.h>
<<<<<<< HEAD

/* napi related */
#define M_CAN_NAPI_WEIGHT	64

/* message ram configuration data length */
#define MRAM_CFG_LEN	8
=======
#include <linux/phy/phy.h>

#include "m_can.h"
>>>>>>> upstream/android-13

/* registers definition */
enum m_can_reg {
	M_CAN_CREL	= 0x0,
	M_CAN_ENDN	= 0x4,
	M_CAN_CUST	= 0x8,
	M_CAN_DBTP	= 0xc,
	M_CAN_TEST	= 0x10,
	M_CAN_RWD	= 0x14,
	M_CAN_CCCR	= 0x18,
	M_CAN_NBTP	= 0x1c,
	M_CAN_TSCC	= 0x20,
	M_CAN_TSCV	= 0x24,
	M_CAN_TOCC	= 0x28,
	M_CAN_TOCV	= 0x2c,
	M_CAN_ECR	= 0x40,
	M_CAN_PSR	= 0x44,
<<<<<<< HEAD
/* TDCR Register only available for version >=3.1.x */
=======
	/* TDCR Register only available for version >=3.1.x */
>>>>>>> upstream/android-13
	M_CAN_TDCR	= 0x48,
	M_CAN_IR	= 0x50,
	M_CAN_IE	= 0x54,
	M_CAN_ILS	= 0x58,
	M_CAN_ILE	= 0x5c,
	M_CAN_GFC	= 0x80,
	M_CAN_SIDFC	= 0x84,
	M_CAN_XIDFC	= 0x88,
	M_CAN_XIDAM	= 0x90,
	M_CAN_HPMS	= 0x94,
	M_CAN_NDAT1	= 0x98,
	M_CAN_NDAT2	= 0x9c,
	M_CAN_RXF0C	= 0xa0,
	M_CAN_RXF0S	= 0xa4,
	M_CAN_RXF0A	= 0xa8,
	M_CAN_RXBC	= 0xac,
	M_CAN_RXF1C	= 0xb0,
	M_CAN_RXF1S	= 0xb4,
	M_CAN_RXF1A	= 0xb8,
	M_CAN_RXESC	= 0xbc,
	M_CAN_TXBC	= 0xc0,
	M_CAN_TXFQS	= 0xc4,
	M_CAN_TXESC	= 0xc8,
	M_CAN_TXBRP	= 0xcc,
	M_CAN_TXBAR	= 0xd0,
	M_CAN_TXBCR	= 0xd4,
	M_CAN_TXBTO	= 0xd8,
	M_CAN_TXBCF	= 0xdc,
	M_CAN_TXBTIE	= 0xe0,
	M_CAN_TXBCIE	= 0xe4,
	M_CAN_TXEFC	= 0xf0,
	M_CAN_TXEFS	= 0xf4,
	M_CAN_TXEFA	= 0xf8,
};

<<<<<<< HEAD
/* m_can lec values */
enum m_can_lec_type {
	LEC_NO_ERROR = 0,
	LEC_STUFF_ERROR,
	LEC_FORM_ERROR,
	LEC_ACK_ERROR,
	LEC_BIT1_ERROR,
	LEC_BIT0_ERROR,
	LEC_CRC_ERROR,
	LEC_UNUSED,
};

enum m_can_mram_cfg {
	MRAM_SIDF = 0,
	MRAM_XIDF,
	MRAM_RXF0,
	MRAM_RXF1,
	MRAM_RXB,
	MRAM_TXE,
	MRAM_TXB,
	MRAM_CFG_NUM,
};

/* Core Release Register (CREL) */
#define CREL_REL_SHIFT		28
#define CREL_REL_MASK		(0xF << CREL_REL_SHIFT)
#define CREL_STEP_SHIFT		24
#define CREL_STEP_MASK		(0xF << CREL_STEP_SHIFT)
#define CREL_SUBSTEP_SHIFT	20
#define CREL_SUBSTEP_MASK	(0xF << CREL_SUBSTEP_SHIFT)

/* Data Bit Timing & Prescaler Register (DBTP) */
#define DBTP_TDC		BIT(23)
#define DBTP_DBRP_SHIFT		16
#define DBTP_DBRP_MASK		(0x1f << DBTP_DBRP_SHIFT)
#define DBTP_DTSEG1_SHIFT	8
#define DBTP_DTSEG1_MASK	(0x1f << DBTP_DTSEG1_SHIFT)
#define DBTP_DTSEG2_SHIFT	4
#define DBTP_DTSEG2_MASK	(0xf << DBTP_DTSEG2_SHIFT)
#define DBTP_DSJW_SHIFT		0
#define DBTP_DSJW_MASK		(0xf << DBTP_DSJW_SHIFT)

/* Transmitter Delay Compensation Register (TDCR) */
#define TDCR_TDCO_SHIFT		8
#define TDCR_TDCO_MASK		(0x7F << TDCR_TDCO_SHIFT)
#define TDCR_TDCF_SHIFT		0
#define TDCR_TDCF_MASK		(0x7F << TDCR_TDCF_SHIFT)
=======
/* napi related */
#define M_CAN_NAPI_WEIGHT	64

/* message ram configuration data length */
#define MRAM_CFG_LEN	8

/* Core Release Register (CREL) */
#define CREL_REL_MASK		GENMASK(31, 28)
#define CREL_STEP_MASK		GENMASK(27, 24)
#define CREL_SUBSTEP_MASK	GENMASK(23, 20)

/* Data Bit Timing & Prescaler Register (DBTP) */
#define DBTP_TDC		BIT(23)
#define DBTP_DBRP_MASK		GENMASK(20, 16)
#define DBTP_DTSEG1_MASK	GENMASK(12, 8)
#define DBTP_DTSEG2_MASK	GENMASK(7, 4)
#define DBTP_DSJW_MASK		GENMASK(3, 0)

/* Transmitter Delay Compensation Register (TDCR) */
#define TDCR_TDCO_MASK		GENMASK(14, 8)
#define TDCR_TDCF_MASK		GENMASK(6, 0)
>>>>>>> upstream/android-13

/* Test Register (TEST) */
#define TEST_LBCK		BIT(4)

<<<<<<< HEAD
/* CC Control Register(CCCR) */
#define CCCR_CMR_MASK		0x3
#define CCCR_CMR_SHIFT		10
#define CCCR_CMR_CANFD		0x1
#define CCCR_CMR_CANFD_BRS	0x2
#define CCCR_CMR_CAN		0x3
#define CCCR_CME_MASK		0x3
#define CCCR_CME_SHIFT		8
#define CCCR_CME_CAN		0
#define CCCR_CME_CANFD		0x1
#define CCCR_CME_CANFD_BRS	0x2
#define CCCR_TXP		BIT(14)
#define CCCR_TEST		BIT(7)
=======
/* CC Control Register (CCCR) */
#define CCCR_TXP		BIT(14)
#define CCCR_TEST		BIT(7)
#define CCCR_DAR		BIT(6)
>>>>>>> upstream/android-13
#define CCCR_MON		BIT(5)
#define CCCR_CSR		BIT(4)
#define CCCR_CSA		BIT(3)
#define CCCR_ASM		BIT(2)
#define CCCR_CCE		BIT(1)
#define CCCR_INIT		BIT(0)
<<<<<<< HEAD
#define CCCR_CANFD		0x10
=======
/* for version 3.0.x */
#define CCCR_CMR_MASK		GENMASK(11, 10)
#define CCCR_CMR_CANFD		0x1
#define CCCR_CMR_CANFD_BRS	0x2
#define CCCR_CMR_CAN		0x3
#define CCCR_CME_MASK		GENMASK(9, 8)
#define CCCR_CME_CAN		0
#define CCCR_CME_CANFD		0x1
#define CCCR_CME_CANFD_BRS	0x2
>>>>>>> upstream/android-13
/* for version >=3.1.x */
#define CCCR_EFBI		BIT(13)
#define CCCR_PXHD		BIT(12)
#define CCCR_BRSE		BIT(9)
#define CCCR_FDOE		BIT(8)
<<<<<<< HEAD
/* only for version >=3.2.x */
#define CCCR_NISO		BIT(15)

/* Nominal Bit Timing & Prescaler Register (NBTP) */
#define NBTP_NSJW_SHIFT		25
#define NBTP_NSJW_MASK		(0x7f << NBTP_NSJW_SHIFT)
#define NBTP_NBRP_SHIFT		16
#define NBTP_NBRP_MASK		(0x1ff << NBTP_NBRP_SHIFT)
#define NBTP_NTSEG1_SHIFT	8
#define NBTP_NTSEG1_MASK	(0xff << NBTP_NTSEG1_SHIFT)
#define NBTP_NTSEG2_SHIFT	0
#define NBTP_NTSEG2_MASK	(0x7f << NBTP_NTSEG2_SHIFT)

/* Error Counter Register(ECR) */
#define ECR_RP			BIT(15)
#define ECR_REC_SHIFT		8
#define ECR_REC_MASK		(0x7f << ECR_REC_SHIFT)
#define ECR_TEC_SHIFT		0
#define ECR_TEC_MASK		0xff

/* Protocol Status Register(PSR) */
#define PSR_BO		BIT(7)
#define PSR_EW		BIT(6)
#define PSR_EP		BIT(5)
#define PSR_LEC_MASK	0x7

/* Interrupt Register(IR) */
=======
/* for version >=3.2.x */
#define CCCR_NISO		BIT(15)
/* for version >=3.3.x */
#define CCCR_WMM		BIT(11)
#define CCCR_UTSU		BIT(10)

/* Nominal Bit Timing & Prescaler Register (NBTP) */
#define NBTP_NSJW_MASK		GENMASK(31, 25)
#define NBTP_NBRP_MASK		GENMASK(24, 16)
#define NBTP_NTSEG1_MASK	GENMASK(15, 8)
#define NBTP_NTSEG2_MASK	GENMASK(6, 0)

/* Timestamp Counter Configuration Register (TSCC) */
#define TSCC_TCP_MASK		GENMASK(19, 16)
#define TSCC_TSS_MASK		GENMASK(1, 0)
#define TSCC_TSS_DISABLE	0x0
#define TSCC_TSS_INTERNAL	0x1
#define TSCC_TSS_EXTERNAL	0x2

/* Timestamp Counter Value Register (TSCV) */
#define TSCV_TSC_MASK		GENMASK(15, 0)

/* Error Counter Register (ECR) */
#define ECR_RP			BIT(15)
#define ECR_REC_MASK		GENMASK(14, 8)
#define ECR_TEC_MASK		GENMASK(7, 0)

/* Protocol Status Register (PSR) */
#define PSR_BO		BIT(7)
#define PSR_EW		BIT(6)
#define PSR_EP		BIT(5)
#define PSR_LEC_MASK	GENMASK(2, 0)

/* Interrupt Register (IR) */
>>>>>>> upstream/android-13
#define IR_ALL_INT	0xffffffff

/* Renamed bits for versions > 3.1.x */
#define IR_ARA		BIT(29)
#define IR_PED		BIT(28)
#define IR_PEA		BIT(27)

/* Bits for version 3.0.x */
#define IR_STE		BIT(31)
#define IR_FOE		BIT(30)
#define IR_ACKE		BIT(29)
#define IR_BE		BIT(28)
#define IR_CRCE		BIT(27)
#define IR_WDI		BIT(26)
#define IR_BO		BIT(25)
#define IR_EW		BIT(24)
#define IR_EP		BIT(23)
#define IR_ELO		BIT(22)
#define IR_BEU		BIT(21)
#define IR_BEC		BIT(20)
#define IR_DRX		BIT(19)
#define IR_TOO		BIT(18)
#define IR_MRAF		BIT(17)
#define IR_TSW		BIT(16)
#define IR_TEFL		BIT(15)
#define IR_TEFF		BIT(14)
#define IR_TEFW		BIT(13)
#define IR_TEFN		BIT(12)
#define IR_TFE		BIT(11)
#define IR_TCF		BIT(10)
#define IR_TC		BIT(9)
#define IR_HPM		BIT(8)
#define IR_RF1L		BIT(7)
#define IR_RF1F		BIT(6)
#define IR_RF1W		BIT(5)
#define IR_RF1N		BIT(4)
#define IR_RF0L		BIT(3)
#define IR_RF0F		BIT(2)
#define IR_RF0W		BIT(1)
#define IR_RF0N		BIT(0)
#define IR_ERR_STATE	(IR_BO | IR_EW | IR_EP)

/* Interrupts for version 3.0.x */
#define IR_ERR_LEC_30X	(IR_STE	| IR_FOE | IR_ACKE | IR_BE | IR_CRCE)
<<<<<<< HEAD
#define IR_ERR_BUS_30X	(IR_ERR_LEC_30X | IR_WDI | IR_ELO | IR_BEU | \
			 IR_BEC | IR_TOO | IR_MRAF | IR_TSW | IR_TEFL | \
			 IR_RF1L | IR_RF0L)
#define IR_ERR_ALL_30X	(IR_ERR_STATE | IR_ERR_BUS_30X)
/* Interrupts for version >= 3.1.x */
#define IR_ERR_LEC_31X	(IR_PED | IR_PEA)
#define IR_ERR_BUS_31X      (IR_ERR_LEC_31X | IR_WDI | IR_ELO | IR_BEU | \
			 IR_BEC | IR_TOO | IR_MRAF | IR_TSW | IR_TEFL | \
			 IR_RF1L | IR_RF0L)
=======
#define IR_ERR_BUS_30X	(IR_ERR_LEC_30X | IR_WDI | IR_BEU | IR_BEC | \
			 IR_TOO | IR_MRAF | IR_TSW | IR_TEFL | IR_RF1L | \
			 IR_RF0L)
#define IR_ERR_ALL_30X	(IR_ERR_STATE | IR_ERR_BUS_30X)

/* Interrupts for version >= 3.1.x */
#define IR_ERR_LEC_31X	(IR_PED | IR_PEA)
#define IR_ERR_BUS_31X      (IR_ERR_LEC_31X | IR_WDI | IR_BEU | IR_BEC | \
			 IR_TOO | IR_MRAF | IR_TSW | IR_TEFL | IR_RF1L | \
			 IR_RF0L)
>>>>>>> upstream/android-13
#define IR_ERR_ALL_31X	(IR_ERR_STATE | IR_ERR_BUS_31X)

/* Interrupt Line Select (ILS) */
#define ILS_ALL_INT0	0x0
#define ILS_ALL_INT1	0xFFFFFFFF

/* Interrupt Line Enable (ILE) */
#define ILE_EINT1	BIT(1)
#define ILE_EINT0	BIT(0)

/* Rx FIFO 0/1 Configuration (RXF0C/RXF1C) */
<<<<<<< HEAD
#define RXFC_FWM_SHIFT	24
#define RXFC_FWM_MASK	(0x7f << RXFC_FWM_SHIFT)
#define RXFC_FS_SHIFT	16
#define RXFC_FS_MASK	(0x7f << RXFC_FS_SHIFT)
=======
#define RXFC_FWM_MASK	GENMASK(30, 24)
#define RXFC_FS_MASK	GENMASK(22, 16)
>>>>>>> upstream/android-13

/* Rx FIFO 0/1 Status (RXF0S/RXF1S) */
#define RXFS_RFL	BIT(25)
#define RXFS_FF		BIT(24)
<<<<<<< HEAD
#define RXFS_FPI_SHIFT	16
#define RXFS_FPI_MASK	0x3f0000
#define RXFS_FGI_SHIFT	8
#define RXFS_FGI_MASK	0x3f00
#define RXFS_FFL_MASK	0x7f

/* Rx Buffer / FIFO Element Size Configuration (RXESC) */
#define M_CAN_RXESC_8BYTES	0x0
#define M_CAN_RXESC_64BYTES	0x777

/* Tx Buffer Configuration(TXBC) */
#define TXBC_NDTB_SHIFT		16
#define TXBC_NDTB_MASK		(0x3f << TXBC_NDTB_SHIFT)
#define TXBC_TFQS_SHIFT		24
#define TXBC_TFQS_MASK		(0x3f << TXBC_TFQS_SHIFT)

/* Tx FIFO/Queue Status (TXFQS) */
#define TXFQS_TFQF		BIT(21)
#define TXFQS_TFQPI_SHIFT	16
#define TXFQS_TFQPI_MASK	(0x1f << TXFQS_TFQPI_SHIFT)
#define TXFQS_TFGI_SHIFT	8
#define TXFQS_TFGI_MASK		(0x1f << TXFQS_TFGI_SHIFT)
#define TXFQS_TFFL_SHIFT	0
#define TXFQS_TFFL_MASK		(0x3f << TXFQS_TFFL_SHIFT)

/* Tx Buffer Element Size Configuration(TXESC) */
#define TXESC_TBDS_8BYTES	0x0
#define TXESC_TBDS_64BYTES	0x7

/* Tx Event FIFO Configuration (TXEFC) */
#define TXEFC_EFS_SHIFT		16
#define TXEFC_EFS_MASK		(0x3f << TXEFC_EFS_SHIFT)
=======
#define RXFS_FPI_MASK	GENMASK(21, 16)
#define RXFS_FGI_MASK	GENMASK(13, 8)
#define RXFS_FFL_MASK	GENMASK(6, 0)

/* Rx Buffer / FIFO Element Size Configuration (RXESC) */
#define RXESC_RBDS_MASK		GENMASK(10, 8)
#define RXESC_F1DS_MASK		GENMASK(6, 4)
#define RXESC_F0DS_MASK		GENMASK(2, 0)
#define RXESC_64B		0x7

/* Tx Buffer Configuration (TXBC) */
#define TXBC_TFQS_MASK		GENMASK(29, 24)
#define TXBC_NDTB_MASK		GENMASK(21, 16)

/* Tx FIFO/Queue Status (TXFQS) */
#define TXFQS_TFQF		BIT(21)
#define TXFQS_TFQPI_MASK	GENMASK(20, 16)
#define TXFQS_TFGI_MASK		GENMASK(12, 8)
#define TXFQS_TFFL_MASK		GENMASK(5, 0)

/* Tx Buffer Element Size Configuration (TXESC) */
#define TXESC_TBDS_MASK		GENMASK(2, 0)
#define TXESC_TBDS_64B		0x7

/* Tx Event FIFO Configuration (TXEFC) */
#define TXEFC_EFS_MASK		GENMASK(21, 16)
>>>>>>> upstream/android-13

/* Tx Event FIFO Status (TXEFS) */
#define TXEFS_TEFL		BIT(25)
#define TXEFS_EFF		BIT(24)
<<<<<<< HEAD
#define TXEFS_EFGI_SHIFT	8
#define	TXEFS_EFGI_MASK		(0x1f << TXEFS_EFGI_SHIFT)
#define TXEFS_EFFL_SHIFT	0
#define TXEFS_EFFL_MASK		(0x3f << TXEFS_EFFL_SHIFT)

/* Tx Event FIFO Acknowledge (TXEFA) */
#define TXEFA_EFAI_SHIFT	0
#define TXEFA_EFAI_MASK		(0x1f << TXEFA_EFAI_SHIFT)
=======
#define TXEFS_EFGI_MASK		GENMASK(12, 8)
#define TXEFS_EFFL_MASK		GENMASK(5, 0)

/* Tx Event FIFO Acknowledge (TXEFA) */
#define TXEFA_EFAI_MASK		GENMASK(4, 0)
>>>>>>> upstream/android-13

/* Message RAM Configuration (in bytes) */
#define SIDF_ELEMENT_SIZE	4
#define XIDF_ELEMENT_SIZE	8
#define RXF0_ELEMENT_SIZE	72
#define RXF1_ELEMENT_SIZE	72
#define RXB_ELEMENT_SIZE	72
#define TXE_ELEMENT_SIZE	8
#define TXB_ELEMENT_SIZE	72

/* Message RAM Elements */
#define M_CAN_FIFO_ID		0x0
#define M_CAN_FIFO_DLC		0x4
<<<<<<< HEAD
#define M_CAN_FIFO_DATA(n)	(0x8 + ((n) << 2))
=======
#define M_CAN_FIFO_DATA		0x8
>>>>>>> upstream/android-13

/* Rx Buffer Element */
/* R0 */
#define RX_BUF_ESI		BIT(31)
#define RX_BUF_XTD		BIT(30)
#define RX_BUF_RTR		BIT(29)
/* R1 */
#define RX_BUF_ANMF		BIT(31)
#define RX_BUF_FDF		BIT(21)
#define RX_BUF_BRS		BIT(20)
<<<<<<< HEAD
=======
#define RX_BUF_RXTS_MASK	GENMASK(15, 0)
>>>>>>> upstream/android-13

/* Tx Buffer Element */
/* T0 */
#define TX_BUF_ESI		BIT(31)
#define TX_BUF_XTD		BIT(30)
#define TX_BUF_RTR		BIT(29)
/* T1 */
#define TX_BUF_EFC		BIT(23)
#define TX_BUF_FDF		BIT(21)
#define TX_BUF_BRS		BIT(20)
<<<<<<< HEAD
#define TX_BUF_MM_SHIFT		24
#define TX_BUF_MM_MASK		(0xff << TX_BUF_MM_SHIFT)

/* Tx event FIFO Element */
/* E1 */
#define TX_EVENT_MM_SHIFT	TX_BUF_MM_SHIFT
#define TX_EVENT_MM_MASK	(0xff << TX_EVENT_MM_SHIFT)

/* address offset and element number for each FIFO/Buffer in the Message RAM */
struct mram_cfg {
	u16 off;
	u8  num;
};

/* m_can private data structure */
struct m_can_priv {
	struct can_priv can;	/* must be the first member */
	struct napi_struct napi;
	struct net_device *dev;
	struct device *device;
	struct clk *hclk;
	struct clk *cclk;
	void __iomem *base;
	u32 irqstatus;
	int version;

	/* message ram configuration */
	void __iomem *mram_base;
	struct mram_cfg mcfg[MRAM_CFG_NUM];
};

static inline u32 m_can_read(const struct m_can_priv *priv, enum m_can_reg reg)
{
	return readl(priv->base + reg);
}

static inline void m_can_write(const struct m_can_priv *priv,
			       enum m_can_reg reg, u32 val)
{
	writel(val, priv->base + reg);
}

static inline u32 m_can_fifo_read(const struct m_can_priv *priv,
				  u32 fgi, unsigned int offset)
{
	return readl(priv->mram_base + priv->mcfg[MRAM_RXF0].off +
		     fgi * RXF0_ELEMENT_SIZE + offset);
}

static inline void m_can_fifo_write(const struct m_can_priv *priv,
				    u32 fpi, unsigned int offset, u32 val)
{
	writel(val, priv->mram_base + priv->mcfg[MRAM_TXB].off +
	       fpi * TXB_ELEMENT_SIZE + offset);
}

static inline u32 m_can_txe_fifo_read(const struct m_can_priv *priv,
				      u32 fgi,
				      u32 offset) {
	return readl(priv->mram_base + priv->mcfg[MRAM_TXE].off +
			fgi * TXE_ELEMENT_SIZE + offset);
}

static inline bool m_can_tx_fifo_full(const struct m_can_priv *priv)
{
		return !!(m_can_read(priv, M_CAN_TXFQS) & TXFQS_TFQF);
}

static inline void m_can_config_endisable(const struct m_can_priv *priv,
					  bool enable)
{
	u32 cccr = m_can_read(priv, M_CAN_CCCR);
	u32 timeout = 10;
	u32 val = 0;

	if (enable) {
		/* enable m_can configuration */
		m_can_write(priv, M_CAN_CCCR, cccr | CCCR_INIT);
		udelay(5);
		/* CCCR.CCE can only be set/reset while CCCR.INIT = '1' */
		m_can_write(priv, M_CAN_CCCR, cccr | CCCR_INIT | CCCR_CCE);
	} else {
		m_can_write(priv, M_CAN_CCCR, cccr & ~(CCCR_INIT | CCCR_CCE));
=======
#define TX_BUF_MM_MASK		GENMASK(31, 24)
#define TX_BUF_DLC_MASK		GENMASK(19, 16)

/* Tx event FIFO Element */
/* E1 */
#define TX_EVENT_MM_MASK	GENMASK(31, 24)
#define TX_EVENT_TXTS_MASK	GENMASK(15, 0)

/* The ID and DLC registers are adjacent in M_CAN FIFO memory,
 * and we can save a (potentially slow) bus round trip by combining
 * reads and writes to them.
 */
struct id_and_dlc {
	u32 id;
	u32 dlc;
};

static inline u32 m_can_read(struct m_can_classdev *cdev, enum m_can_reg reg)
{
	return cdev->ops->read_reg(cdev, reg);
}

static inline void m_can_write(struct m_can_classdev *cdev, enum m_can_reg reg,
			       u32 val)
{
	cdev->ops->write_reg(cdev, reg, val);
}

static int
m_can_fifo_read(struct m_can_classdev *cdev,
		u32 fgi, unsigned int offset, void *val, size_t val_count)
{
	u32 addr_offset = cdev->mcfg[MRAM_RXF0].off + fgi * RXF0_ELEMENT_SIZE +
		offset;

	if (val_count == 0)
		return 0;

	return cdev->ops->read_fifo(cdev, addr_offset, val, val_count);
}

static int
m_can_fifo_write(struct m_can_classdev *cdev,
		 u32 fpi, unsigned int offset, const void *val, size_t val_count)
{
	u32 addr_offset = cdev->mcfg[MRAM_TXB].off + fpi * TXB_ELEMENT_SIZE +
		offset;

	if (val_count == 0)
		return 0;

	return cdev->ops->write_fifo(cdev, addr_offset, val, val_count);
}

static inline int m_can_fifo_write_no_off(struct m_can_classdev *cdev,
					  u32 fpi, u32 val)
{
	return cdev->ops->write_fifo(cdev, fpi, &val, 1);
}

static int
m_can_txe_fifo_read(struct m_can_classdev *cdev, u32 fgi, u32 offset, u32 *val)
{
	u32 addr_offset = cdev->mcfg[MRAM_TXE].off + fgi * TXE_ELEMENT_SIZE +
		offset;

	return cdev->ops->read_fifo(cdev, addr_offset, val, 1);
}

static inline bool m_can_tx_fifo_full(struct m_can_classdev *cdev)
{
	return !!(m_can_read(cdev, M_CAN_TXFQS) & TXFQS_TFQF);
}

static void m_can_config_endisable(struct m_can_classdev *cdev, bool enable)
{
	u32 cccr = m_can_read(cdev, M_CAN_CCCR);
	u32 timeout = 10;
	u32 val = 0;

	/* Clear the Clock stop request if it was set */
	if (cccr & CCCR_CSR)
		cccr &= ~CCCR_CSR;

	if (enable) {
		/* enable m_can configuration */
		m_can_write(cdev, M_CAN_CCCR, cccr | CCCR_INIT);
		udelay(5);
		/* CCCR.CCE can only be set/reset while CCCR.INIT = '1' */
		m_can_write(cdev, M_CAN_CCCR, cccr | CCCR_INIT | CCCR_CCE);
	} else {
		m_can_write(cdev, M_CAN_CCCR, cccr & ~(CCCR_INIT | CCCR_CCE));
>>>>>>> upstream/android-13
	}

	/* there's a delay for module initialization */
	if (enable)
		val = CCCR_INIT | CCCR_CCE;

<<<<<<< HEAD
	while ((m_can_read(priv, M_CAN_CCCR) & (CCCR_INIT | CCCR_CCE)) != val) {
		if (timeout == 0) {
			netdev_warn(priv->dev, "Failed to init module\n");
=======
	while ((m_can_read(cdev, M_CAN_CCCR) & (CCCR_INIT | CCCR_CCE)) != val) {
		if (timeout == 0) {
			netdev_warn(cdev->net, "Failed to init module\n");
>>>>>>> upstream/android-13
			return;
		}
		timeout--;
		udelay(1);
	}
}

<<<<<<< HEAD
static inline void m_can_enable_all_interrupts(const struct m_can_priv *priv)
{
	/* Only interrupt line 0 is used in this driver */
	m_can_write(priv, M_CAN_ILE, ILE_EINT0);
}

static inline void m_can_disable_all_interrupts(const struct m_can_priv *priv)
{
	m_can_write(priv, M_CAN_ILE, 0x0);
}

static void m_can_read_fifo(struct net_device *dev, u32 rxfs)
{
	struct net_device_stats *stats = &dev->stats;
	struct m_can_priv *priv = netdev_priv(dev);
	struct canfd_frame *cf;
	struct sk_buff *skb;
	u32 id, fgi, dlc;
	int i;

	/* calculate the fifo get index for where to read data */
	fgi = (rxfs & RXFS_FGI_MASK) >> RXFS_FGI_SHIFT;
	dlc = m_can_fifo_read(priv, fgi, M_CAN_FIFO_DLC);
	if (dlc & RX_BUF_FDF)
=======
static inline void m_can_enable_all_interrupts(struct m_can_classdev *cdev)
{
	/* Only interrupt line 0 is used in this driver */
	m_can_write(cdev, M_CAN_ILE, ILE_EINT0);
}

static inline void m_can_disable_all_interrupts(struct m_can_classdev *cdev)
{
	m_can_write(cdev, M_CAN_ILE, 0x0);
}

/* Retrieve internal timestamp counter from TSCV.TSC, and shift it to 32-bit
 * width.
 */
static u32 m_can_get_timestamp(struct m_can_classdev *cdev)
{
	u32 tscv;
	u32 tsc;

	tscv = m_can_read(cdev, M_CAN_TSCV);
	tsc = FIELD_GET(TSCV_TSC_MASK, tscv);

	return (tsc << 16);
}

static void m_can_clean(struct net_device *net)
{
	struct m_can_classdev *cdev = netdev_priv(net);

	if (cdev->tx_skb) {
		int putidx = 0;

		net->stats.tx_errors++;
		if (cdev->version > 30)
			putidx = FIELD_GET(TXFQS_TFQPI_MASK,
					   m_can_read(cdev, M_CAN_TXFQS));

		can_free_echo_skb(cdev->net, putidx, NULL);
		cdev->tx_skb = NULL;
	}
}

/* For peripherals, pass skb to rx-offload, which will push skb from
 * napi. For non-peripherals, RX is done in napi already, so push
 * directly. timestamp is used to ensure good skb ordering in
 * rx-offload and is ignored for non-peripherals.
 */
static void m_can_receive_skb(struct m_can_classdev *cdev,
			      struct sk_buff *skb,
			      u32 timestamp)
{
	if (cdev->is_peripheral) {
		struct net_device_stats *stats = &cdev->net->stats;
		int err;

		err = can_rx_offload_queue_sorted(&cdev->offload, skb,
						  timestamp);
		if (err)
			stats->rx_fifo_errors++;
	} else {
		netif_receive_skb(skb);
	}
}

static int m_can_read_fifo(struct net_device *dev, u32 rxfs)
{
	struct net_device_stats *stats = &dev->stats;
	struct m_can_classdev *cdev = netdev_priv(dev);
	struct canfd_frame *cf;
	struct sk_buff *skb;
	struct id_and_dlc fifo_header;
	u32 fgi;
	u32 timestamp = 0;
	int err;

	/* calculate the fifo get index for where to read data */
	fgi = FIELD_GET(RXFS_FGI_MASK, rxfs);
	err = m_can_fifo_read(cdev, fgi, M_CAN_FIFO_ID, &fifo_header, 2);
	if (err)
		goto out_fail;

	if (fifo_header.dlc & RX_BUF_FDF)
>>>>>>> upstream/android-13
		skb = alloc_canfd_skb(dev, &cf);
	else
		skb = alloc_can_skb(dev, (struct can_frame **)&cf);
	if (!skb) {
		stats->rx_dropped++;
<<<<<<< HEAD
		return;
	}

	if (dlc & RX_BUF_FDF)
		cf->len = can_dlc2len((dlc >> 16) & 0x0F);
	else
		cf->len = get_can_dlc((dlc >> 16) & 0x0F);

	id = m_can_fifo_read(priv, fgi, M_CAN_FIFO_ID);
	if (id & RX_BUF_XTD)
		cf->can_id = (id & CAN_EFF_MASK) | CAN_EFF_FLAG;
	else
		cf->can_id = (id >> 18) & CAN_SFF_MASK;

	if (id & RX_BUF_ESI) {
=======
		return 0;
	}

	if (fifo_header.dlc & RX_BUF_FDF)
		cf->len = can_fd_dlc2len((fifo_header.dlc >> 16) & 0x0F);
	else
		cf->len = can_cc_dlc2len((fifo_header.dlc >> 16) & 0x0F);

	if (fifo_header.id & RX_BUF_XTD)
		cf->can_id = (fifo_header.id & CAN_EFF_MASK) | CAN_EFF_FLAG;
	else
		cf->can_id = (fifo_header.id >> 18) & CAN_SFF_MASK;

	if (fifo_header.id & RX_BUF_ESI) {
>>>>>>> upstream/android-13
		cf->flags |= CANFD_ESI;
		netdev_dbg(dev, "ESI Error\n");
	}

<<<<<<< HEAD
	if (!(dlc & RX_BUF_FDF) && (id & RX_BUF_RTR)) {
		cf->can_id |= CAN_RTR_FLAG;
	} else {
		if (dlc & RX_BUF_BRS)
			cf->flags |= CANFD_BRS;

		for (i = 0; i < cf->len; i += 4)
			*(u32 *)(cf->data + i) =
				m_can_fifo_read(priv, fgi,
						M_CAN_FIFO_DATA(i / 4));
	}

	/* acknowledge rx fifo 0 */
	m_can_write(priv, M_CAN_RXF0A, fgi);
=======
	if (!(fifo_header.dlc & RX_BUF_FDF) && (fifo_header.id & RX_BUF_RTR)) {
		cf->can_id |= CAN_RTR_FLAG;
	} else {
		if (fifo_header.dlc & RX_BUF_BRS)
			cf->flags |= CANFD_BRS;

		err = m_can_fifo_read(cdev, fgi, M_CAN_FIFO_DATA,
				      cf->data, DIV_ROUND_UP(cf->len, 4));
		if (err)
			goto out_free_skb;
	}

	/* acknowledge rx fifo 0 */
	m_can_write(cdev, M_CAN_RXF0A, fgi);
>>>>>>> upstream/android-13

	stats->rx_packets++;
	stats->rx_bytes += cf->len;

<<<<<<< HEAD
	netif_receive_skb(skb);
=======
	timestamp = FIELD_GET(RX_BUF_RXTS_MASK, fifo_header.dlc);

	m_can_receive_skb(cdev, skb, timestamp);

	return 0;

out_free_skb:
	kfree_skb(skb);
out_fail:
	netdev_err(dev, "FIFO read returned %d\n", err);
	return err;
>>>>>>> upstream/android-13
}

static int m_can_do_rx_poll(struct net_device *dev, int quota)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	u32 pkts = 0;
	u32 rxfs;

	rxfs = m_can_read(priv, M_CAN_RXF0S);
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	u32 pkts = 0;
	u32 rxfs;
	int err;

	rxfs = m_can_read(cdev, M_CAN_RXF0S);
>>>>>>> upstream/android-13
	if (!(rxfs & RXFS_FFL_MASK)) {
		netdev_dbg(dev, "no messages in fifo0\n");
		return 0;
	}

	while ((rxfs & RXFS_FFL_MASK) && (quota > 0)) {
<<<<<<< HEAD
		m_can_read_fifo(dev, rxfs);

		quota--;
		pkts++;
		rxfs = m_can_read(priv, M_CAN_RXF0S);
=======
		err = m_can_read_fifo(dev, rxfs);
		if (err)
			return err;

		quota--;
		pkts++;
		rxfs = m_can_read(cdev, M_CAN_RXF0S);
>>>>>>> upstream/android-13
	}

	if (pkts)
		can_led_event(dev, CAN_LED_EVENT_RX);

	return pkts;
}

static int m_can_handle_lost_msg(struct net_device *dev)
{
<<<<<<< HEAD
	struct net_device_stats *stats = &dev->stats;
	struct sk_buff *skb;
	struct can_frame *frame;
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct sk_buff *skb;
	struct can_frame *frame;
	u32 timestamp = 0;
>>>>>>> upstream/android-13

	netdev_err(dev, "msg lost in rxf0\n");

	stats->rx_errors++;
	stats->rx_over_errors++;

	skb = alloc_can_err_skb(dev, &frame);
	if (unlikely(!skb))
		return 0;

	frame->can_id |= CAN_ERR_CRTL;
	frame->data[1] = CAN_ERR_CRTL_RX_OVERFLOW;

<<<<<<< HEAD
	netif_receive_skb(skb);
=======
	if (cdev->is_peripheral)
		timestamp = m_can_get_timestamp(cdev);

	m_can_receive_skb(cdev, skb, timestamp);
>>>>>>> upstream/android-13

	return 1;
}

static int m_can_handle_lec_err(struct net_device *dev,
				enum m_can_lec_type lec_type)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;

	priv->can.can_stats.bus_error++;
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 timestamp = 0;

	cdev->can.can_stats.bus_error++;
>>>>>>> upstream/android-13
	stats->rx_errors++;

	/* propagate the error condition to the CAN stack */
	skb = alloc_can_err_skb(dev, &cf);
	if (unlikely(!skb))
		return 0;

	/* check for 'last error code' which tells us the
	 * type of the last error to occur on the CAN bus
	 */
	cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;

	switch (lec_type) {
	case LEC_STUFF_ERROR:
		netdev_dbg(dev, "stuff error\n");
		cf->data[2] |= CAN_ERR_PROT_STUFF;
		break;
	case LEC_FORM_ERROR:
		netdev_dbg(dev, "form error\n");
		cf->data[2] |= CAN_ERR_PROT_FORM;
		break;
	case LEC_ACK_ERROR:
		netdev_dbg(dev, "ack error\n");
		cf->data[3] = CAN_ERR_PROT_LOC_ACK;
		break;
	case LEC_BIT1_ERROR:
		netdev_dbg(dev, "bit1 error\n");
		cf->data[2] |= CAN_ERR_PROT_BIT1;
		break;
	case LEC_BIT0_ERROR:
		netdev_dbg(dev, "bit0 error\n");
		cf->data[2] |= CAN_ERR_PROT_BIT0;
		break;
	case LEC_CRC_ERROR:
		netdev_dbg(dev, "CRC error\n");
		cf->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
		break;
	default:
		break;
	}

	stats->rx_packets++;
<<<<<<< HEAD
	stats->rx_bytes += cf->can_dlc;
	netif_receive_skb(skb);
=======
	stats->rx_bytes += cf->len;

	if (cdev->is_peripheral)
		timestamp = m_can_get_timestamp(cdev);

	m_can_receive_skb(cdev, skb, timestamp);
>>>>>>> upstream/android-13

	return 1;
}

static int __m_can_get_berr_counter(const struct net_device *dev,
				    struct can_berr_counter *bec)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	unsigned int ecr;

	ecr = m_can_read(priv, M_CAN_ECR);
	bec->rxerr = (ecr & ECR_REC_MASK) >> ECR_REC_SHIFT;
	bec->txerr = (ecr & ECR_TEC_MASK) >> ECR_TEC_SHIFT;
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	unsigned int ecr;

	ecr = m_can_read(cdev, M_CAN_ECR);
	bec->rxerr = FIELD_GET(ECR_REC_MASK, ecr);
	bec->txerr = FIELD_GET(ECR_TEC_MASK, ecr);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int m_can_clk_start(struct m_can_priv *priv)
{
	int err;

	err = pm_runtime_get_sync(priv->device);
	if (err < 0) {
		pm_runtime_put_noidle(priv->device);
		return err;
	}

	return 0;
}

static void m_can_clk_stop(struct m_can_priv *priv)
{
	pm_runtime_put_sync(priv->device);
=======
static int m_can_clk_start(struct m_can_classdev *cdev)
{
	if (cdev->pm_clock_support == 0)
		return 0;

	return pm_runtime_resume_and_get(cdev->dev);
}

static void m_can_clk_stop(struct m_can_classdev *cdev)
{
	if (cdev->pm_clock_support)
		pm_runtime_put_sync(cdev->dev);
>>>>>>> upstream/android-13
}

static int m_can_get_berr_counter(const struct net_device *dev,
				  struct can_berr_counter *bec)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	int err;

	err = m_can_clk_start(priv);
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	int err;

	err = m_can_clk_start(cdev);
>>>>>>> upstream/android-13
	if (err)
		return err;

	__m_can_get_berr_counter(dev, bec);

<<<<<<< HEAD
	m_can_clk_stop(priv);
=======
	m_can_clk_stop(cdev);
>>>>>>> upstream/android-13

	return 0;
}

static int m_can_handle_state_change(struct net_device *dev,
				     enum can_state new_state)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
>>>>>>> upstream/android-13
	struct net_device_stats *stats = &dev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	struct can_berr_counter bec;
	unsigned int ecr;
<<<<<<< HEAD
=======
	u32 timestamp = 0;
>>>>>>> upstream/android-13

	switch (new_state) {
	case CAN_STATE_ERROR_WARNING:
		/* error warning state */
<<<<<<< HEAD
		priv->can.can_stats.error_warning++;
		priv->can.state = CAN_STATE_ERROR_WARNING;
		break;
	case CAN_STATE_ERROR_PASSIVE:
		/* error passive state */
		priv->can.can_stats.error_passive++;
		priv->can.state = CAN_STATE_ERROR_PASSIVE;
		break;
	case CAN_STATE_BUS_OFF:
		/* bus-off state */
		priv->can.state = CAN_STATE_BUS_OFF;
		m_can_disable_all_interrupts(priv);
		priv->can.can_stats.bus_off++;
=======
		cdev->can.can_stats.error_warning++;
		cdev->can.state = CAN_STATE_ERROR_WARNING;
		break;
	case CAN_STATE_ERROR_PASSIVE:
		/* error passive state */
		cdev->can.can_stats.error_passive++;
		cdev->can.state = CAN_STATE_ERROR_PASSIVE;
		break;
	case CAN_STATE_BUS_OFF:
		/* bus-off state */
		cdev->can.state = CAN_STATE_BUS_OFF;
		m_can_disable_all_interrupts(cdev);
		cdev->can.can_stats.bus_off++;
>>>>>>> upstream/android-13
		can_bus_off(dev);
		break;
	default:
		break;
	}

	/* propagate the error condition to the CAN stack */
	skb = alloc_can_err_skb(dev, &cf);
	if (unlikely(!skb))
		return 0;

	__m_can_get_berr_counter(dev, &bec);

	switch (new_state) {
	case CAN_STATE_ERROR_WARNING:
		/* error warning state */
		cf->can_id |= CAN_ERR_CRTL;
		cf->data[1] = (bec.txerr > bec.rxerr) ?
			CAN_ERR_CRTL_TX_WARNING :
			CAN_ERR_CRTL_RX_WARNING;
		cf->data[6] = bec.txerr;
		cf->data[7] = bec.rxerr;
		break;
	case CAN_STATE_ERROR_PASSIVE:
		/* error passive state */
		cf->can_id |= CAN_ERR_CRTL;
<<<<<<< HEAD
		ecr = m_can_read(priv, M_CAN_ECR);
=======
		ecr = m_can_read(cdev, M_CAN_ECR);
>>>>>>> upstream/android-13
		if (ecr & ECR_RP)
			cf->data[1] |= CAN_ERR_CRTL_RX_PASSIVE;
		if (bec.txerr > 127)
			cf->data[1] |= CAN_ERR_CRTL_TX_PASSIVE;
		cf->data[6] = bec.txerr;
		cf->data[7] = bec.rxerr;
		break;
	case CAN_STATE_BUS_OFF:
		/* bus-off state */
		cf->can_id |= CAN_ERR_BUSOFF;
		break;
	default:
		break;
	}

	stats->rx_packets++;
<<<<<<< HEAD
	stats->rx_bytes += cf->can_dlc;
	netif_receive_skb(skb);
=======
	stats->rx_bytes += cf->len;

	if (cdev->is_peripheral)
		timestamp = m_can_get_timestamp(cdev);

	m_can_receive_skb(cdev, skb, timestamp);
>>>>>>> upstream/android-13

	return 1;
}

static int m_can_handle_state_errors(struct net_device *dev, u32 psr)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	int work_done = 0;

	if ((psr & PSR_EW) &&
	    (priv->can.state != CAN_STATE_ERROR_WARNING)) {
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	int work_done = 0;

	if (psr & PSR_EW && cdev->can.state != CAN_STATE_ERROR_WARNING) {
>>>>>>> upstream/android-13
		netdev_dbg(dev, "entered error warning state\n");
		work_done += m_can_handle_state_change(dev,
						       CAN_STATE_ERROR_WARNING);
	}

<<<<<<< HEAD
	if ((psr & PSR_EP) &&
	    (priv->can.state != CAN_STATE_ERROR_PASSIVE)) {
=======
	if (psr & PSR_EP && cdev->can.state != CAN_STATE_ERROR_PASSIVE) {
>>>>>>> upstream/android-13
		netdev_dbg(dev, "entered error passive state\n");
		work_done += m_can_handle_state_change(dev,
						       CAN_STATE_ERROR_PASSIVE);
	}

<<<<<<< HEAD
	if ((psr & PSR_BO) &&
	    (priv->can.state != CAN_STATE_BUS_OFF)) {
=======
	if (psr & PSR_BO && cdev->can.state != CAN_STATE_BUS_OFF) {
>>>>>>> upstream/android-13
		netdev_dbg(dev, "entered error bus off state\n");
		work_done += m_can_handle_state_change(dev,
						       CAN_STATE_BUS_OFF);
	}

	return work_done;
}

static void m_can_handle_other_err(struct net_device *dev, u32 irqstatus)
{
	if (irqstatus & IR_WDI)
		netdev_err(dev, "Message RAM Watchdog event due to missing READY\n");
<<<<<<< HEAD
	if (irqstatus & IR_ELO)
		netdev_err(dev, "Error Logging Overflow\n");
=======
>>>>>>> upstream/android-13
	if (irqstatus & IR_BEU)
		netdev_err(dev, "Bit Error Uncorrected\n");
	if (irqstatus & IR_BEC)
		netdev_err(dev, "Bit Error Corrected\n");
	if (irqstatus & IR_TOO)
		netdev_err(dev, "Timeout reached\n");
	if (irqstatus & IR_MRAF)
		netdev_err(dev, "Message RAM access failure occurred\n");
}

static inline bool is_lec_err(u32 psr)
{
	psr &= LEC_UNUSED;

	return psr && (psr != LEC_UNUSED);
}

<<<<<<< HEAD
static int m_can_handle_bus_errors(struct net_device *dev, u32 irqstatus,
				   u32 psr)
{
	struct m_can_priv *priv = netdev_priv(dev);
=======
static inline bool m_can_is_protocol_err(u32 irqstatus)
{
	return irqstatus & IR_ERR_LEC_31X;
}

static int m_can_handle_protocol_error(struct net_device *dev, u32 irqstatus)
{
	struct net_device_stats *stats = &dev->stats;
	struct m_can_classdev *cdev = netdev_priv(dev);
	struct can_frame *cf;
	struct sk_buff *skb;
	u32 timestamp = 0;

	/* propagate the error condition to the CAN stack */
	skb = alloc_can_err_skb(dev, &cf);

	/* update tx error stats since there is protocol error */
	stats->tx_errors++;

	/* update arbitration lost status */
	if (cdev->version >= 31 && (irqstatus & IR_PEA)) {
		netdev_dbg(dev, "Protocol error in Arbitration fail\n");
		cdev->can.can_stats.arbitration_lost++;
		if (skb) {
			cf->can_id |= CAN_ERR_LOSTARB;
			cf->data[0] |= CAN_ERR_LOSTARB_UNSPEC;
		}
	}

	if (unlikely(!skb)) {
		netdev_dbg(dev, "allocation of skb failed\n");
		return 0;
	}

	if (cdev->is_peripheral)
		timestamp = m_can_get_timestamp(cdev);

	m_can_receive_skb(cdev, skb, timestamp);

	return 1;
}

static int m_can_handle_bus_errors(struct net_device *dev, u32 irqstatus,
				   u32 psr)
{
	struct m_can_classdev *cdev = netdev_priv(dev);
>>>>>>> upstream/android-13
	int work_done = 0;

	if (irqstatus & IR_RF0L)
		work_done += m_can_handle_lost_msg(dev);

	/* handle lec errors on the bus */
<<<<<<< HEAD
	if ((priv->can.ctrlmode & CAN_CTRLMODE_BERR_REPORTING) &&
	    is_lec_err(psr))
		work_done += m_can_handle_lec_err(dev, psr & LEC_UNUSED);

=======
	if ((cdev->can.ctrlmode & CAN_CTRLMODE_BERR_REPORTING) &&
	    is_lec_err(psr))
		work_done += m_can_handle_lec_err(dev, psr & LEC_UNUSED);

	/* handle protocol errors in arbitration phase */
	if ((cdev->can.ctrlmode & CAN_CTRLMODE_BERR_REPORTING) &&
	    m_can_is_protocol_err(irqstatus))
		work_done += m_can_handle_protocol_error(dev, irqstatus);

>>>>>>> upstream/android-13
	/* other unproccessed error interrupts */
	m_can_handle_other_err(dev, irqstatus);

	return work_done;
}

<<<<<<< HEAD
static int m_can_poll(struct napi_struct *napi, int quota)
{
	struct net_device *dev = napi->dev;
	struct m_can_priv *priv = netdev_priv(dev);
	int work_done = 0;
	u32 irqstatus, psr;

	irqstatus = priv->irqstatus | m_can_read(priv, M_CAN_IR);
=======
static int m_can_rx_handler(struct net_device *dev, int quota)
{
	struct m_can_classdev *cdev = netdev_priv(dev);
	int rx_work_or_err;
	int work_done = 0;
	u32 irqstatus, psr;

	irqstatus = cdev->irqstatus | m_can_read(cdev, M_CAN_IR);
>>>>>>> upstream/android-13
	if (!irqstatus)
		goto end;

	/* Errata workaround for issue "Needless activation of MRAF irq"
	 * During frame reception while the MCAN is in Error Passive state
	 * and the Receive Error Counter has the value MCAN_ECR.REC = 127,
	 * it may happen that MCAN_IR.MRAF is set although there was no
	 * Message RAM access failure.
	 * If MCAN_IR.MRAF is enabled, an interrupt to the Host CPU is generated
	 * The Message RAM Access Failure interrupt routine needs to check
	 * whether MCAN_ECR.RP = ’1’ and MCAN_ECR.REC = 127.
	 * In this case, reset MCAN_IR.MRAF. No further action is required.
	 */
<<<<<<< HEAD
	if ((priv->version <= 31) && (irqstatus & IR_MRAF) &&
	    (m_can_read(priv, M_CAN_ECR) & ECR_RP)) {
=======
	if (cdev->version <= 31 && irqstatus & IR_MRAF &&
	    m_can_read(cdev, M_CAN_ECR) & ECR_RP) {
>>>>>>> upstream/android-13
		struct can_berr_counter bec;

		__m_can_get_berr_counter(dev, &bec);
		if (bec.rxerr == 127) {
<<<<<<< HEAD
			m_can_write(priv, M_CAN_IR, IR_MRAF);
=======
			m_can_write(cdev, M_CAN_IR, IR_MRAF);
>>>>>>> upstream/android-13
			irqstatus &= ~IR_MRAF;
		}
	}

<<<<<<< HEAD
	psr = m_can_read(priv, M_CAN_PSR);
=======
	psr = m_can_read(cdev, M_CAN_PSR);

>>>>>>> upstream/android-13
	if (irqstatus & IR_ERR_STATE)
		work_done += m_can_handle_state_errors(dev, psr);

	if (irqstatus & IR_ERR_BUS_30X)
		work_done += m_can_handle_bus_errors(dev, irqstatus, psr);

<<<<<<< HEAD
	if (irqstatus & IR_RF0N)
		work_done += m_can_do_rx_poll(dev, (quota - work_done));

	if (work_done < quota) {
		napi_complete_done(napi, work_done);
		m_can_enable_all_interrupts(priv);
	}

=======
	if (irqstatus & IR_RF0N) {
		rx_work_or_err = m_can_do_rx_poll(dev, (quota - work_done));
		if (rx_work_or_err < 0)
			return rx_work_or_err;

		work_done += rx_work_or_err;
	}
>>>>>>> upstream/android-13
end:
	return work_done;
}

<<<<<<< HEAD
static void m_can_echo_tx_event(struct net_device *dev)
=======
static int m_can_rx_peripheral(struct net_device *dev)
{
	struct m_can_classdev *cdev = netdev_priv(dev);
	int work_done;

	work_done = m_can_rx_handler(dev, M_CAN_NAPI_WEIGHT);

	/* Don't re-enable interrupts if the driver had a fatal error
	 * (e.g., FIFO read failure).
	 */
	if (work_done >= 0)
		m_can_enable_all_interrupts(cdev);

	return work_done;
}

static int m_can_poll(struct napi_struct *napi, int quota)
{
	struct net_device *dev = napi->dev;
	struct m_can_classdev *cdev = netdev_priv(dev);
	int work_done;

	work_done = m_can_rx_handler(dev, quota);

	/* Don't re-enable interrupts if the driver had a fatal error
	 * (e.g., FIFO read failure).
	 */
	if (work_done >= 0 && work_done < quota) {
		napi_complete_done(napi, work_done);
		m_can_enable_all_interrupts(cdev);
	}

	return work_done;
}

/* Echo tx skb and update net stats. Peripherals use rx-offload for
 * echo. timestamp is used for peripherals to ensure correct ordering
 * by rx-offload, and is ignored for non-peripherals.
 */
static void m_can_tx_update_stats(struct m_can_classdev *cdev,
				  unsigned int msg_mark,
				  u32 timestamp)
{
	struct net_device *dev = cdev->net;
	struct net_device_stats *stats = &dev->stats;

	if (cdev->is_peripheral)
		stats->tx_bytes +=
			can_rx_offload_get_echo_skb(&cdev->offload,
						    msg_mark,
						    timestamp,
						    NULL);
	else
		stats->tx_bytes += can_get_echo_skb(dev, msg_mark, NULL);

	stats->tx_packets++;
}

static int m_can_echo_tx_event(struct net_device *dev)
>>>>>>> upstream/android-13
{
	u32 txe_count = 0;
	u32 m_can_txefs;
	u32 fgi = 0;
	int i = 0;
	unsigned int msg_mark;

<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;

	/* read tx event fifo status */
	m_can_txefs = m_can_read(priv, M_CAN_TXEFS);

	/* Get Tx Event fifo element count */
	txe_count = (m_can_txefs & TXEFS_EFFL_MASK)
			>> TXEFS_EFFL_SHIFT;

	/* Get and process all sent elements */
	for (i = 0; i < txe_count; i++) {
		/* retrieve get index */
		fgi = (m_can_read(priv, M_CAN_TXEFS) & TXEFS_EFGI_MASK)
			>> TXEFS_EFGI_SHIFT;

		/* get message marker */
		msg_mark = (m_can_txe_fifo_read(priv, fgi, 4) &
			    TX_EVENT_MM_MASK) >> TX_EVENT_MM_SHIFT;

		/* ack txe element */
		m_can_write(priv, M_CAN_TXEFA, (TXEFA_EFAI_MASK &
						(fgi << TXEFA_EFAI_SHIFT)));

		/* update stats */
		stats->tx_bytes += can_get_echo_skb(dev, msg_mark);
		stats->tx_packets++;
	}
=======
	struct m_can_classdev *cdev = netdev_priv(dev);

	/* read tx event fifo status */
	m_can_txefs = m_can_read(cdev, M_CAN_TXEFS);

	/* Get Tx Event fifo element count */
	txe_count = FIELD_GET(TXEFS_EFFL_MASK, m_can_txefs);

	/* Get and process all sent elements */
	for (i = 0; i < txe_count; i++) {
		u32 txe, timestamp = 0;
		int err;

		/* retrieve get index */
		fgi = FIELD_GET(TXEFS_EFGI_MASK, m_can_read(cdev, M_CAN_TXEFS));

		/* get message marker, timestamp */
		err = m_can_txe_fifo_read(cdev, fgi, 4, &txe);
		if (err) {
			netdev_err(dev, "TXE FIFO read returned %d\n", err);
			return err;
		}

		msg_mark = FIELD_GET(TX_EVENT_MM_MASK, txe);
		timestamp = FIELD_GET(TX_EVENT_TXTS_MASK, txe);

		/* ack txe element */
		m_can_write(cdev, M_CAN_TXEFA, FIELD_PREP(TXEFA_EFAI_MASK,
							  fgi));

		/* update stats */
		m_can_tx_update_stats(cdev, msg_mark, timestamp);
	}

	return 0;
>>>>>>> upstream/android-13
}

static irqreturn_t m_can_isr(int irq, void *dev_id)
{
	struct net_device *dev = (struct net_device *)dev_id;
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	u32 ir;

	ir = m_can_read(priv, M_CAN_IR);
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	u32 ir;

	if (pm_runtime_suspended(cdev->dev))
		return IRQ_NONE;
	ir = m_can_read(cdev, M_CAN_IR);
>>>>>>> upstream/android-13
	if (!ir)
		return IRQ_NONE;

	/* ACK all irqs */
	if (ir & IR_ALL_INT)
<<<<<<< HEAD
		m_can_write(priv, M_CAN_IR, ir);
=======
		m_can_write(cdev, M_CAN_IR, ir);

	if (cdev->ops->clear_interrupts)
		cdev->ops->clear_interrupts(cdev);
>>>>>>> upstream/android-13

	/* schedule NAPI in case of
	 * - rx IRQ
	 * - state change IRQ
	 * - bus error IRQ and bus error reporting
	 */
	if ((ir & IR_RF0N) || (ir & IR_ERR_ALL_30X)) {
<<<<<<< HEAD
		priv->irqstatus = ir;
		m_can_disable_all_interrupts(priv);
		napi_schedule(&priv->napi);
	}

	if (priv->version == 30) {
		if (ir & IR_TC) {
			/* Transmission Complete Interrupt*/
			stats->tx_bytes += can_get_echo_skb(dev, 0);
			stats->tx_packets++;
=======
		cdev->irqstatus = ir;
		m_can_disable_all_interrupts(cdev);
		if (!cdev->is_peripheral)
			napi_schedule(&cdev->napi);
		else if (m_can_rx_peripheral(dev) < 0)
			goto out_fail;
	}

	if (cdev->version == 30) {
		if (ir & IR_TC) {
			/* Transmission Complete Interrupt*/
			u32 timestamp = 0;

			if (cdev->is_peripheral)
				timestamp = m_can_get_timestamp(cdev);
			m_can_tx_update_stats(cdev, 0, timestamp);

>>>>>>> upstream/android-13
			can_led_event(dev, CAN_LED_EVENT_TX);
			netif_wake_queue(dev);
		}
	} else  {
		if (ir & IR_TEFN) {
			/* New TX FIFO Element arrived */
<<<<<<< HEAD
			m_can_echo_tx_event(dev);
			can_led_event(dev, CAN_LED_EVENT_TX);
			if (netif_queue_stopped(dev) &&
			    !m_can_tx_fifo_full(priv))
=======
			if (m_can_echo_tx_event(dev) != 0)
				goto out_fail;

			can_led_event(dev, CAN_LED_EVENT_TX);
			if (netif_queue_stopped(dev) &&
			    !m_can_tx_fifo_full(cdev))
>>>>>>> upstream/android-13
				netif_wake_queue(dev);
		}
	}

<<<<<<< HEAD
=======
	if (cdev->is_peripheral)
		can_rx_offload_threaded_irq_finish(&cdev->offload);

	return IRQ_HANDLED;

out_fail:
	m_can_disable_all_interrupts(cdev);
>>>>>>> upstream/android-13
	return IRQ_HANDLED;
}

static const struct can_bittiming_const m_can_bittiming_const_30X = {
	.name = KBUILD_MODNAME,
	.tseg1_min = 2,		/* Time segment 1 = prop_seg + phase_seg1 */
	.tseg1_max = 64,
	.tseg2_min = 1,		/* Time segment 2 = phase_seg2 */
	.tseg2_max = 16,
	.sjw_max = 16,
	.brp_min = 1,
	.brp_max = 1024,
	.brp_inc = 1,
};

static const struct can_bittiming_const m_can_data_bittiming_const_30X = {
	.name = KBUILD_MODNAME,
	.tseg1_min = 2,		/* Time segment 1 = prop_seg + phase_seg1 */
	.tseg1_max = 16,
	.tseg2_min = 1,		/* Time segment 2 = phase_seg2 */
	.tseg2_max = 8,
	.sjw_max = 4,
	.brp_min = 1,
	.brp_max = 32,
	.brp_inc = 1,
};

static const struct can_bittiming_const m_can_bittiming_const_31X = {
	.name = KBUILD_MODNAME,
	.tseg1_min = 2,		/* Time segment 1 = prop_seg + phase_seg1 */
	.tseg1_max = 256,
	.tseg2_min = 2,		/* Time segment 2 = phase_seg2 */
	.tseg2_max = 128,
	.sjw_max = 128,
	.brp_min = 1,
	.brp_max = 512,
	.brp_inc = 1,
};

static const struct can_bittiming_const m_can_data_bittiming_const_31X = {
	.name = KBUILD_MODNAME,
	.tseg1_min = 1,		/* Time segment 1 = prop_seg + phase_seg1 */
	.tseg1_max = 32,
	.tseg2_min = 1,		/* Time segment 2 = phase_seg2 */
	.tseg2_max = 16,
	.sjw_max = 16,
	.brp_min = 1,
	.brp_max = 32,
	.brp_inc = 1,
};

static int m_can_set_bittiming(struct net_device *dev)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	const struct can_bittiming *bt = &priv->can.bittiming;
	const struct can_bittiming *dbt = &priv->can.data_bittiming;
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	const struct can_bittiming *bt = &cdev->can.bittiming;
	const struct can_bittiming *dbt = &cdev->can.data_bittiming;
>>>>>>> upstream/android-13
	u16 brp, sjw, tseg1, tseg2;
	u32 reg_btp;

	brp = bt->brp - 1;
	sjw = bt->sjw - 1;
	tseg1 = bt->prop_seg + bt->phase_seg1 - 1;
	tseg2 = bt->phase_seg2 - 1;
<<<<<<< HEAD
	reg_btp = (brp << NBTP_NBRP_SHIFT) | (sjw << NBTP_NSJW_SHIFT) |
		(tseg1 << NBTP_NTSEG1_SHIFT) | (tseg2 << NBTP_NTSEG2_SHIFT);
	m_can_write(priv, M_CAN_NBTP, reg_btp);

	if (priv->can.ctrlmode & CAN_CTRLMODE_FD) {
=======
	reg_btp = FIELD_PREP(NBTP_NBRP_MASK, brp) |
		  FIELD_PREP(NBTP_NSJW_MASK, sjw) |
		  FIELD_PREP(NBTP_NTSEG1_MASK, tseg1) |
		  FIELD_PREP(NBTP_NTSEG2_MASK, tseg2);
	m_can_write(cdev, M_CAN_NBTP, reg_btp);

	if (cdev->can.ctrlmode & CAN_CTRLMODE_FD) {
>>>>>>> upstream/android-13
		reg_btp = 0;
		brp = dbt->brp - 1;
		sjw = dbt->sjw - 1;
		tseg1 = dbt->prop_seg + dbt->phase_seg1 - 1;
		tseg2 = dbt->phase_seg2 - 1;

		/* TDC is only needed for bitrates beyond 2.5 MBit/s.
		 * This is mentioned in the "Bit Time Requirements for CAN FD"
		 * paper presented at the International CAN Conference 2013
		 */
		if (dbt->bitrate > 2500000) {
			u32 tdco, ssp;

			/* Use the same value of secondary sampling point
			 * as the data sampling point
			 */
			ssp = dbt->sample_point;

			/* Equation based on Bosch's M_CAN User Manual's
			 * Transmitter Delay Compensation Section
			 */
<<<<<<< HEAD
			tdco = (priv->can.clock.freq / 1000) *
			       ssp / dbt->bitrate;
=======
			tdco = (cdev->can.clock.freq / 1000) *
				ssp / dbt->bitrate;
>>>>>>> upstream/android-13

			/* Max valid TDCO value is 127 */
			if (tdco > 127) {
				netdev_warn(dev, "TDCO value of %u is beyond maximum. Using maximum possible value\n",
					    tdco);
				tdco = 127;
			}

			reg_btp |= DBTP_TDC;
<<<<<<< HEAD
			m_can_write(priv, M_CAN_TDCR,
				    tdco << TDCR_TDCO_SHIFT);
		}

		reg_btp |= (brp << DBTP_DBRP_SHIFT) |
			   (sjw << DBTP_DSJW_SHIFT) |
			   (tseg1 << DBTP_DTSEG1_SHIFT) |
			   (tseg2 << DBTP_DTSEG2_SHIFT);

		m_can_write(priv, M_CAN_DBTP, reg_btp);
=======
			m_can_write(cdev, M_CAN_TDCR,
				    FIELD_PREP(TDCR_TDCO_MASK, tdco));
		}

		reg_btp |= FIELD_PREP(DBTP_DBRP_MASK, brp) |
			FIELD_PREP(DBTP_DSJW_MASK, sjw) |
			FIELD_PREP(DBTP_DTSEG1_MASK, tseg1) |
			FIELD_PREP(DBTP_DTSEG2_MASK, tseg2);

		m_can_write(cdev, M_CAN_DBTP, reg_btp);
>>>>>>> upstream/android-13
	}

	return 0;
}

/* Configure M_CAN chip:
 * - set rx buffer/fifo element size
 * - configure rx fifo
 * - accept non-matching frame into fifo 0
 * - configure tx buffer
 *		- >= v3.1.x: TX FIFO is used
 * - configure mode
 * - setup bittiming
<<<<<<< HEAD
 */
static void m_can_chip_config(struct net_device *dev)
{
	struct m_can_priv *priv = netdev_priv(dev);
	u32 cccr, test;

	m_can_config_endisable(priv, true);

	/* RX Buffer/FIFO Element Size 64 bytes data field */
	m_can_write(priv, M_CAN_RXESC, M_CAN_RXESC_64BYTES);

	/* Accept Non-matching Frames Into FIFO 0 */
	m_can_write(priv, M_CAN_GFC, 0x0);

	if (priv->version == 30) {
		/* only support one Tx Buffer currently */
		m_can_write(priv, M_CAN_TXBC, (1 << TXBC_NDTB_SHIFT) |
				priv->mcfg[MRAM_TXB].off);
	} else {
		/* TX FIFO is used for newer IP Core versions */
		m_can_write(priv, M_CAN_TXBC,
			    (priv->mcfg[MRAM_TXB].num << TXBC_TFQS_SHIFT) |
			    (priv->mcfg[MRAM_TXB].off));
	}

	/* support 64 bytes payload */
	m_can_write(priv, M_CAN_TXESC, TXESC_TBDS_64BYTES);

	/* TX Event FIFO */
	if (priv->version == 30) {
		m_can_write(priv, M_CAN_TXEFC, (1 << TXEFC_EFS_SHIFT) |
				priv->mcfg[MRAM_TXE].off);
	} else {
		/* Full TX Event FIFO is used */
		m_can_write(priv, M_CAN_TXEFC,
			    ((priv->mcfg[MRAM_TXE].num << TXEFC_EFS_SHIFT)
			     & TXEFC_EFS_MASK) |
			    priv->mcfg[MRAM_TXE].off);
	}

	/* rx fifo configuration, blocking mode, fifo size 1 */
	m_can_write(priv, M_CAN_RXF0C,
		    (priv->mcfg[MRAM_RXF0].num << RXFC_FS_SHIFT) |
		     priv->mcfg[MRAM_RXF0].off);

	m_can_write(priv, M_CAN_RXF1C,
		    (priv->mcfg[MRAM_RXF1].num << RXFC_FS_SHIFT) |
		     priv->mcfg[MRAM_RXF1].off);

	cccr = m_can_read(priv, M_CAN_CCCR);
	test = m_can_read(priv, M_CAN_TEST);
	test &= ~TEST_LBCK;
	if (priv->version == 30) {
	/* Version 3.0.x */

		cccr &= ~(CCCR_TEST | CCCR_MON |
			(CCCR_CMR_MASK << CCCR_CMR_SHIFT) |
			(CCCR_CME_MASK << CCCR_CME_SHIFT));

		if (priv->can.ctrlmode & CAN_CTRLMODE_FD)
			cccr |= CCCR_CME_CANFD_BRS << CCCR_CME_SHIFT;

	} else {
	/* Version 3.1.x or 3.2.x */
		cccr &= ~(CCCR_TEST | CCCR_MON | CCCR_BRSE | CCCR_FDOE |
			  CCCR_NISO);

		/* Only 3.2.x has NISO Bit implemented */
		if (priv->can.ctrlmode & CAN_CTRLMODE_FD_NON_ISO)
			cccr |= CCCR_NISO;

		if (priv->can.ctrlmode & CAN_CTRLMODE_FD)
=======
 * - configure timestamp generation
 */
static void m_can_chip_config(struct net_device *dev)
{
	struct m_can_classdev *cdev = netdev_priv(dev);
	u32 cccr, test;

	m_can_config_endisable(cdev, true);

	/* RX Buffer/FIFO Element Size 64 bytes data field */
	m_can_write(cdev, M_CAN_RXESC,
		    FIELD_PREP(RXESC_RBDS_MASK, RXESC_64B) |
		    FIELD_PREP(RXESC_F1DS_MASK, RXESC_64B) |
		    FIELD_PREP(RXESC_F0DS_MASK, RXESC_64B));

	/* Accept Non-matching Frames Into FIFO 0 */
	m_can_write(cdev, M_CAN_GFC, 0x0);

	if (cdev->version == 30) {
		/* only support one Tx Buffer currently */
		m_can_write(cdev, M_CAN_TXBC, FIELD_PREP(TXBC_NDTB_MASK, 1) |
			    cdev->mcfg[MRAM_TXB].off);
	} else {
		/* TX FIFO is used for newer IP Core versions */
		m_can_write(cdev, M_CAN_TXBC,
			    FIELD_PREP(TXBC_TFQS_MASK,
				       cdev->mcfg[MRAM_TXB].num) |
			    cdev->mcfg[MRAM_TXB].off);
	}

	/* support 64 bytes payload */
	m_can_write(cdev, M_CAN_TXESC,
		    FIELD_PREP(TXESC_TBDS_MASK, TXESC_TBDS_64B));

	/* TX Event FIFO */
	if (cdev->version == 30) {
		m_can_write(cdev, M_CAN_TXEFC,
			    FIELD_PREP(TXEFC_EFS_MASK, 1) |
			    cdev->mcfg[MRAM_TXE].off);
	} else {
		/* Full TX Event FIFO is used */
		m_can_write(cdev, M_CAN_TXEFC,
			    FIELD_PREP(TXEFC_EFS_MASK,
				       cdev->mcfg[MRAM_TXE].num) |
			    cdev->mcfg[MRAM_TXE].off);
	}

	/* rx fifo configuration, blocking mode, fifo size 1 */
	m_can_write(cdev, M_CAN_RXF0C,
		    FIELD_PREP(RXFC_FS_MASK, cdev->mcfg[MRAM_RXF0].num) |
		    cdev->mcfg[MRAM_RXF0].off);

	m_can_write(cdev, M_CAN_RXF1C,
		    FIELD_PREP(RXFC_FS_MASK, cdev->mcfg[MRAM_RXF1].num) |
		    cdev->mcfg[MRAM_RXF1].off);

	cccr = m_can_read(cdev, M_CAN_CCCR);
	test = m_can_read(cdev, M_CAN_TEST);
	test &= ~TEST_LBCK;
	if (cdev->version == 30) {
		/* Version 3.0.x */

		cccr &= ~(CCCR_TEST | CCCR_MON | CCCR_DAR |
			  FIELD_PREP(CCCR_CMR_MASK, FIELD_MAX(CCCR_CMR_MASK)) |
			  FIELD_PREP(CCCR_CME_MASK, FIELD_MAX(CCCR_CME_MASK)));

		if (cdev->can.ctrlmode & CAN_CTRLMODE_FD)
			cccr |= FIELD_PREP(CCCR_CME_MASK, CCCR_CME_CANFD_BRS);

	} else {
		/* Version 3.1.x or 3.2.x */
		cccr &= ~(CCCR_TEST | CCCR_MON | CCCR_BRSE | CCCR_FDOE |
			  CCCR_NISO | CCCR_DAR);

		/* Only 3.2.x has NISO Bit implemented */
		if (cdev->can.ctrlmode & CAN_CTRLMODE_FD_NON_ISO)
			cccr |= CCCR_NISO;

		if (cdev->can.ctrlmode & CAN_CTRLMODE_FD)
>>>>>>> upstream/android-13
			cccr |= (CCCR_BRSE | CCCR_FDOE);
	}

	/* Loopback Mode */
<<<<<<< HEAD
	if (priv->can.ctrlmode & CAN_CTRLMODE_LOOPBACK) {
=======
	if (cdev->can.ctrlmode & CAN_CTRLMODE_LOOPBACK) {
>>>>>>> upstream/android-13
		cccr |= CCCR_TEST | CCCR_MON;
		test |= TEST_LBCK;
	}

	/* Enable Monitoring (all versions) */
<<<<<<< HEAD
	if (priv->can.ctrlmode & CAN_CTRLMODE_LISTENONLY)
		cccr |= CCCR_MON;

	/* Write config */
	m_can_write(priv, M_CAN_CCCR, cccr);
	m_can_write(priv, M_CAN_TEST, test);

	/* Enable interrupts */
	m_can_write(priv, M_CAN_IR, IR_ALL_INT);
	if (!(priv->can.ctrlmode & CAN_CTRLMODE_BERR_REPORTING))
		if (priv->version == 30)
			m_can_write(priv, M_CAN_IE, IR_ALL_INT &
				    ~(IR_ERR_LEC_30X));
		else
			m_can_write(priv, M_CAN_IE, IR_ALL_INT &
				    ~(IR_ERR_LEC_31X));
	else
		m_can_write(priv, M_CAN_IE, IR_ALL_INT);

	/* route all interrupts to INT0 */
	m_can_write(priv, M_CAN_ILS, ILS_ALL_INT0);
=======
	if (cdev->can.ctrlmode & CAN_CTRLMODE_LISTENONLY)
		cccr |= CCCR_MON;

	/* Disable Auto Retransmission (all versions) */
	if (cdev->can.ctrlmode & CAN_CTRLMODE_ONE_SHOT)
		cccr |= CCCR_DAR;

	/* Write config */
	m_can_write(cdev, M_CAN_CCCR, cccr);
	m_can_write(cdev, M_CAN_TEST, test);

	/* Enable interrupts */
	m_can_write(cdev, M_CAN_IR, IR_ALL_INT);
	if (!(cdev->can.ctrlmode & CAN_CTRLMODE_BERR_REPORTING))
		if (cdev->version == 30)
			m_can_write(cdev, M_CAN_IE, IR_ALL_INT &
				    ~(IR_ERR_LEC_30X));
		else
			m_can_write(cdev, M_CAN_IE, IR_ALL_INT &
				    ~(IR_ERR_LEC_31X));
	else
		m_can_write(cdev, M_CAN_IE, IR_ALL_INT);

	/* route all interrupts to INT0 */
	m_can_write(cdev, M_CAN_ILS, ILS_ALL_INT0);
>>>>>>> upstream/android-13

	/* set bittiming params */
	m_can_set_bittiming(dev);

<<<<<<< HEAD
	m_can_config_endisable(priv, false);
=======
	/* enable internal timestamp generation, with a prescalar of 16. The
	 * prescalar is applied to the nominal bit timing
	 */
	m_can_write(cdev, M_CAN_TSCC, FIELD_PREP(TSCC_TCP_MASK, 0xf));

	m_can_config_endisable(cdev, false);

	if (cdev->ops->init)
		cdev->ops->init(cdev);
>>>>>>> upstream/android-13
}

static void m_can_start(struct net_device *dev)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
>>>>>>> upstream/android-13

	/* basic m_can configuration */
	m_can_chip_config(dev);

<<<<<<< HEAD
	priv->can.state = CAN_STATE_ERROR_ACTIVE;

	m_can_enable_all_interrupts(priv);
=======
	cdev->can.state = CAN_STATE_ERROR_ACTIVE;

	m_can_enable_all_interrupts(cdev);
>>>>>>> upstream/android-13
}

static int m_can_set_mode(struct net_device *dev, enum can_mode mode)
{
	switch (mode) {
	case CAN_MODE_START:
<<<<<<< HEAD
=======
		m_can_clean(dev);
>>>>>>> upstream/android-13
		m_can_start(dev);
		netif_wake_queue(dev);
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

/* Checks core release number of M_CAN
 * returns 0 if an unsupported device is detected
 * else it returns the release and step coded as:
 * return value = 10 * <release> + 1 * <step>
 */
<<<<<<< HEAD
static int m_can_check_core_release(void __iomem *m_can_base)
=======
static int m_can_check_core_release(struct m_can_classdev *cdev)
>>>>>>> upstream/android-13
{
	u32 crel_reg;
	u8 rel;
	u8 step;
	int res;
<<<<<<< HEAD
	struct m_can_priv temp_priv = {
		.base = m_can_base
	};
=======
>>>>>>> upstream/android-13

	/* Read Core Release Version and split into version number
	 * Example: Version 3.2.1 => rel = 3; step = 2; substep = 1;
	 */
<<<<<<< HEAD
	crel_reg = m_can_read(&temp_priv, M_CAN_CREL);
	rel = (u8)((crel_reg & CREL_REL_MASK) >> CREL_REL_SHIFT);
	step = (u8)((crel_reg & CREL_STEP_MASK) >> CREL_STEP_SHIFT);
=======
	crel_reg = m_can_read(cdev, M_CAN_CREL);
	rel = (u8)FIELD_GET(CREL_REL_MASK, crel_reg);
	step = (u8)FIELD_GET(CREL_STEP_MASK, crel_reg);
>>>>>>> upstream/android-13

	if (rel == 3) {
		/* M_CAN v3.x.y: create return value */
		res = 30 + step;
	} else {
		/* Unsupported M_CAN version */
		res = 0;
	}

	return res;
}

/* Selectable Non ISO support only in version 3.2.x
 * This function checks if the bit is writable.
 */
<<<<<<< HEAD
static bool m_can_niso_supported(const struct m_can_priv *priv)
{
	u32 cccr_reg, cccr_poll;
	int niso_timeout;

	m_can_config_endisable(priv, true);
	cccr_reg = m_can_read(priv, M_CAN_CCCR);
	cccr_reg |= CCCR_NISO;
	m_can_write(priv, M_CAN_CCCR, cccr_reg);

	niso_timeout = readl_poll_timeout((priv->base + M_CAN_CCCR), cccr_poll,
					  (cccr_poll == cccr_reg), 0, 10);

	/* Clear NISO */
	cccr_reg &= ~(CCCR_NISO);
	m_can_write(priv, M_CAN_CCCR, cccr_reg);

	m_can_config_endisable(priv, false);
=======
static bool m_can_niso_supported(struct m_can_classdev *cdev)
{
	u32 cccr_reg, cccr_poll = 0;
	int niso_timeout = -ETIMEDOUT;
	int i;

	m_can_config_endisable(cdev, true);
	cccr_reg = m_can_read(cdev, M_CAN_CCCR);
	cccr_reg |= CCCR_NISO;
	m_can_write(cdev, M_CAN_CCCR, cccr_reg);

	for (i = 0; i <= 10; i++) {
		cccr_poll = m_can_read(cdev, M_CAN_CCCR);
		if (cccr_poll == cccr_reg) {
			niso_timeout = 0;
			break;
		}

		usleep_range(1, 5);
	}

	/* Clear NISO */
	cccr_reg &= ~(CCCR_NISO);
	m_can_write(cdev, M_CAN_CCCR, cccr_reg);

	m_can_config_endisable(cdev, false);
>>>>>>> upstream/android-13

	/* return false if time out (-ETIMEDOUT), else return true */
	return !niso_timeout;
}

<<<<<<< HEAD
static int m_can_dev_setup(struct platform_device *pdev, struct net_device *dev,
			   void __iomem *addr)
{
	struct m_can_priv *priv;
	int m_can_version;

	m_can_version = m_can_check_core_release(addr);
	/* return if unsupported version */
	if (!m_can_version) {
		dev_err(&pdev->dev, "Unsupported version number: %2d",
=======
static int m_can_dev_setup(struct m_can_classdev *cdev)
{
	struct net_device *dev = cdev->net;
	int m_can_version;

	m_can_version = m_can_check_core_release(cdev);
	/* return if unsupported version */
	if (!m_can_version) {
		dev_err(cdev->dev, "Unsupported version number: %2d",
>>>>>>> upstream/android-13
			m_can_version);
		return -EINVAL;
	}

<<<<<<< HEAD
	priv = netdev_priv(dev);
	netif_napi_add(dev, &priv->napi, m_can_poll, M_CAN_NAPI_WEIGHT);

	/* Shared properties of all M_CAN versions */
	priv->version = m_can_version;
	priv->dev = dev;
	priv->base = addr;
	priv->can.do_set_mode = m_can_set_mode;
	priv->can.do_get_berr_counter = m_can_get_berr_counter;

	/* Set M_CAN supported operations */
	priv->can.ctrlmode_supported = CAN_CTRLMODE_LOOPBACK |
					CAN_CTRLMODE_LISTENONLY |
					CAN_CTRLMODE_BERR_REPORTING |
					CAN_CTRLMODE_FD;

	/* Set properties depending on M_CAN version */
	switch (priv->version) {
	case 30:
		/* CAN_CTRLMODE_FD_NON_ISO is fixed with M_CAN IP v3.0.x */
		can_set_static_ctrlmode(dev, CAN_CTRLMODE_FD_NON_ISO);
		priv->can.bittiming_const = &m_can_bittiming_const_30X;
		priv->can.data_bittiming_const =
				&m_can_data_bittiming_const_30X;
=======
	if (!cdev->is_peripheral)
		netif_napi_add(dev, &cdev->napi,
			       m_can_poll, M_CAN_NAPI_WEIGHT);

	/* Shared properties of all M_CAN versions */
	cdev->version = m_can_version;
	cdev->can.do_set_mode = m_can_set_mode;
	cdev->can.do_get_berr_counter = m_can_get_berr_counter;

	/* Set M_CAN supported operations */
	cdev->can.ctrlmode_supported = CAN_CTRLMODE_LOOPBACK |
		CAN_CTRLMODE_LISTENONLY |
		CAN_CTRLMODE_BERR_REPORTING |
		CAN_CTRLMODE_FD |
		CAN_CTRLMODE_ONE_SHOT;

	/* Set properties depending on M_CAN version */
	switch (cdev->version) {
	case 30:
		/* CAN_CTRLMODE_FD_NON_ISO is fixed with M_CAN IP v3.0.x */
		can_set_static_ctrlmode(dev, CAN_CTRLMODE_FD_NON_ISO);
		cdev->can.bittiming_const = cdev->bit_timing ?
			cdev->bit_timing : &m_can_bittiming_const_30X;

		cdev->can.data_bittiming_const = cdev->data_timing ?
			cdev->data_timing :
			&m_can_data_bittiming_const_30X;
>>>>>>> upstream/android-13
		break;
	case 31:
		/* CAN_CTRLMODE_FD_NON_ISO is fixed with M_CAN IP v3.1.x */
		can_set_static_ctrlmode(dev, CAN_CTRLMODE_FD_NON_ISO);
<<<<<<< HEAD
		priv->can.bittiming_const = &m_can_bittiming_const_31X;
		priv->can.data_bittiming_const =
				&m_can_data_bittiming_const_31X;
		break;
	case 32:
		priv->can.bittiming_const = &m_can_bittiming_const_31X;
		priv->can.data_bittiming_const =
				&m_can_data_bittiming_const_31X;
		priv->can.ctrlmode_supported |= (m_can_niso_supported(priv)
						? CAN_CTRLMODE_FD_NON_ISO
						: 0);
		break;
	default:
		dev_err(&pdev->dev, "Unsupported version number: %2d",
			priv->version);
		return -EINVAL;
	}

	return 0;
}

static int m_can_open(struct net_device *dev)
{
	struct m_can_priv *priv = netdev_priv(dev);
	int err;

	err = m_can_clk_start(priv);
	if (err)
		return err;

	/* open the can device */
	err = open_candev(dev);
	if (err) {
		netdev_err(dev, "failed to open can device\n");
		goto exit_disable_clks;
	}

	/* register interrupt handler */
	err = request_irq(dev->irq, m_can_isr, IRQF_SHARED, dev->name,
			  dev);
	if (err < 0) {
		netdev_err(dev, "failed to request interrupt\n");
		goto exit_irq_fail;
	}

	/* start the m_can controller */
	m_can_start(dev);

	can_led_event(dev, CAN_LED_EVENT_OPEN);
	napi_enable(&priv->napi);
	netif_start_queue(dev);

	return 0;

exit_irq_fail:
	close_candev(dev);
exit_disable_clks:
	m_can_clk_stop(priv);
	return err;
=======
		cdev->can.bittiming_const = cdev->bit_timing ?
			cdev->bit_timing : &m_can_bittiming_const_31X;

		cdev->can.data_bittiming_const = cdev->data_timing ?
			cdev->data_timing :
			&m_can_data_bittiming_const_31X;
		break;
	case 32:
	case 33:
		/* Support both MCAN version v3.2.x and v3.3.0 */
		cdev->can.bittiming_const = cdev->bit_timing ?
			cdev->bit_timing : &m_can_bittiming_const_31X;

		cdev->can.data_bittiming_const = cdev->data_timing ?
			cdev->data_timing :
			&m_can_data_bittiming_const_31X;

		cdev->can.ctrlmode_supported |=
			(m_can_niso_supported(cdev) ?
			 CAN_CTRLMODE_FD_NON_ISO : 0);
		break;
	default:
		dev_err(cdev->dev, "Unsupported version number: %2d",
			cdev->version);
		return -EINVAL;
	}

	if (cdev->ops->init)
		cdev->ops->init(cdev);

	return 0;
>>>>>>> upstream/android-13
}

static void m_can_stop(struct net_device *dev)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);

	/* disable all interrupts */
	m_can_disable_all_interrupts(priv);

	/* set the state as STOPPED */
	priv->can.state = CAN_STATE_STOPPED;
=======
	struct m_can_classdev *cdev = netdev_priv(dev);

	/* disable all interrupts */
	m_can_disable_all_interrupts(cdev);

	/* Set init mode to disengage from the network */
	m_can_config_endisable(cdev, true);

	/* set the state as STOPPED */
	cdev->can.state = CAN_STATE_STOPPED;
>>>>>>> upstream/android-13
}

static int m_can_close(struct net_device *dev)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);

	netif_stop_queue(dev);
	napi_disable(&priv->napi);
	m_can_stop(dev);
	m_can_clk_stop(priv);
	free_irq(dev->irq, dev);
	close_candev(dev);
	can_led_event(dev, CAN_LED_EVENT_STOP);

=======
	struct m_can_classdev *cdev = netdev_priv(dev);

	netif_stop_queue(dev);

	if (!cdev->is_peripheral)
		napi_disable(&cdev->napi);

	m_can_stop(dev);
	m_can_clk_stop(cdev);
	free_irq(dev->irq, dev);

	if (cdev->is_peripheral) {
		cdev->tx_skb = NULL;
		destroy_workqueue(cdev->tx_wq);
		cdev->tx_wq = NULL;
	}

	if (cdev->is_peripheral)
		can_rx_offload_disable(&cdev->offload);

	close_candev(dev);
	can_led_event(dev, CAN_LED_EVENT_STOP);

	phy_power_off(cdev->transceiver);

>>>>>>> upstream/android-13
	return 0;
}

static int m_can_next_echo_skb_occupied(struct net_device *dev, int putidx)
{
<<<<<<< HEAD
	struct m_can_priv *priv = netdev_priv(dev);
	/*get wrap around for loopback skb index */
	unsigned int wrap = priv->can.echo_skb_max;
=======
	struct m_can_classdev *cdev = netdev_priv(dev);
	/*get wrap around for loopback skb index */
	unsigned int wrap = cdev->can.echo_skb_max;
>>>>>>> upstream/android-13
	int next_idx;

	/* calculate next index */
	next_idx = (++putidx >= wrap ? 0 : putidx);

	/* check if occupied */
<<<<<<< HEAD
	return !!priv->can.echo_skb[next_idx];
}

static netdev_tx_t m_can_start_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct m_can_priv *priv = netdev_priv(dev);
	struct canfd_frame *cf = (struct canfd_frame *)skb->data;
	u32 id, cccr, fdflags;
	int i;
	int putidx;

	if (can_dropped_invalid_skb(dev, skb))
		return NETDEV_TX_OK;
=======
	return !!cdev->can.echo_skb[next_idx];
}

static netdev_tx_t m_can_tx_handler(struct m_can_classdev *cdev)
{
	struct canfd_frame *cf = (struct canfd_frame *)cdev->tx_skb->data;
	struct net_device *dev = cdev->net;
	struct sk_buff *skb = cdev->tx_skb;
	struct id_and_dlc fifo_header;
	u32 cccr, fdflags;
	int err;
	int putidx;

	cdev->tx_skb = NULL;
>>>>>>> upstream/android-13

	/* Generate ID field for TX buffer Element */
	/* Common to all supported M_CAN versions */
	if (cf->can_id & CAN_EFF_FLAG) {
<<<<<<< HEAD
		id = cf->can_id & CAN_EFF_MASK;
		id |= TX_BUF_XTD;
	} else {
		id = ((cf->can_id & CAN_SFF_MASK) << 18);
	}

	if (cf->can_id & CAN_RTR_FLAG)
		id |= TX_BUF_RTR;

	if (priv->version == 30) {
		netif_stop_queue(dev);

		/* message ram configuration */
		m_can_fifo_write(priv, 0, M_CAN_FIFO_ID, id);
		m_can_fifo_write(priv, 0, M_CAN_FIFO_DLC,
				 can_len2dlc(cf->len) << 16);

		for (i = 0; i < cf->len; i += 4)
			m_can_fifo_write(priv, 0,
					 M_CAN_FIFO_DATA(i / 4),
					 *(u32 *)(cf->data + i));

		can_put_echo_skb(skb, dev, 0);

		if (priv->can.ctrlmode & CAN_CTRLMODE_FD) {
			cccr = m_can_read(priv, M_CAN_CCCR);
			cccr &= ~(CCCR_CMR_MASK << CCCR_CMR_SHIFT);
			if (can_is_canfd_skb(skb)) {
				if (cf->flags & CANFD_BRS)
					cccr |= CCCR_CMR_CANFD_BRS <<
						CCCR_CMR_SHIFT;
				else
					cccr |= CCCR_CMR_CANFD <<
						CCCR_CMR_SHIFT;
			} else {
				cccr |= CCCR_CMR_CAN << CCCR_CMR_SHIFT;
			}
			m_can_write(priv, M_CAN_CCCR, cccr);
		}
		m_can_write(priv, M_CAN_TXBTIE, 0x1);
		m_can_write(priv, M_CAN_TXBAR, 0x1);
=======
		fifo_header.id = cf->can_id & CAN_EFF_MASK;
		fifo_header.id |= TX_BUF_XTD;
	} else {
		fifo_header.id = ((cf->can_id & CAN_SFF_MASK) << 18);
	}

	if (cf->can_id & CAN_RTR_FLAG)
		fifo_header.id |= TX_BUF_RTR;

	if (cdev->version == 30) {
		netif_stop_queue(dev);

		fifo_header.dlc = can_fd_len2dlc(cf->len) << 16;

		/* Write the frame ID, DLC, and payload to the FIFO element. */
		err = m_can_fifo_write(cdev, 0, M_CAN_FIFO_ID, &fifo_header, 2);
		if (err)
			goto out_fail;

		err = m_can_fifo_write(cdev, 0, M_CAN_FIFO_DATA,
				       cf->data, DIV_ROUND_UP(cf->len, 4));
		if (err)
			goto out_fail;

		if (cdev->can.ctrlmode & CAN_CTRLMODE_FD) {
			cccr = m_can_read(cdev, M_CAN_CCCR);
			cccr &= ~CCCR_CMR_MASK;
			if (can_is_canfd_skb(skb)) {
				if (cf->flags & CANFD_BRS)
					cccr |= FIELD_PREP(CCCR_CMR_MASK,
							   CCCR_CMR_CANFD_BRS);
				else
					cccr |= FIELD_PREP(CCCR_CMR_MASK,
							   CCCR_CMR_CANFD);
			} else {
				cccr |= FIELD_PREP(CCCR_CMR_MASK, CCCR_CMR_CAN);
			}
			m_can_write(cdev, M_CAN_CCCR, cccr);
		}
		m_can_write(cdev, M_CAN_TXBTIE, 0x1);

		can_put_echo_skb(skb, dev, 0, 0);

		m_can_write(cdev, M_CAN_TXBAR, 0x1);
>>>>>>> upstream/android-13
		/* End of xmit function for version 3.0.x */
	} else {
		/* Transmit routine for version >= v3.1.x */

		/* Check if FIFO full */
<<<<<<< HEAD
		if (m_can_tx_fifo_full(priv)) {
=======
		if (m_can_tx_fifo_full(cdev)) {
>>>>>>> upstream/android-13
			/* This shouldn't happen */
			netif_stop_queue(dev);
			netdev_warn(dev,
				    "TX queue active although FIFO is full.");
<<<<<<< HEAD
			return NETDEV_TX_BUSY;
		}

		/* get put index for frame */
		putidx = ((m_can_read(priv, M_CAN_TXFQS) & TXFQS_TFQPI_MASK)
				  >> TXFQS_TFQPI_SHIFT);
		/* Write ID Field to FIFO Element */
		m_can_fifo_write(priv, putidx, M_CAN_FIFO_ID, id);
=======

			if (cdev->is_peripheral) {
				kfree_skb(skb);
				dev->stats.tx_dropped++;
				return NETDEV_TX_OK;
			} else {
				return NETDEV_TX_BUSY;
			}
		}

		/* get put index for frame */
		putidx = FIELD_GET(TXFQS_TFQPI_MASK,
				   m_can_read(cdev, M_CAN_TXFQS));

		/* Construct DLC Field, with CAN-FD configuration.
		 * Use the put index of the fifo as the message marker,
		 * used in the TX interrupt for sending the correct echo frame.
		 */
>>>>>>> upstream/android-13

		/* get CAN FD configuration of frame */
		fdflags = 0;
		if (can_is_canfd_skb(skb)) {
			fdflags |= TX_BUF_FDF;
			if (cf->flags & CANFD_BRS)
				fdflags |= TX_BUF_BRS;
		}

<<<<<<< HEAD
		/* Construct DLC Field. Also contains CAN-FD configuration
		 * use put index of fifo as message marker
		 * it is used in TX interrupt for
		 * sending the correct echo frame
		 */
		m_can_fifo_write(priv, putidx, M_CAN_FIFO_DLC,
				 ((putidx << TX_BUF_MM_SHIFT) &
				  TX_BUF_MM_MASK) |
				 (can_len2dlc(cf->len) << 16) |
				 fdflags | TX_BUF_EFC);

		for (i = 0; i < cf->len; i += 4)
			m_can_fifo_write(priv, putidx, M_CAN_FIFO_DATA(i / 4),
					 *(u32 *)(cf->data + i));
=======
		fifo_header.dlc = FIELD_PREP(TX_BUF_MM_MASK, putidx) |
			FIELD_PREP(TX_BUF_DLC_MASK, can_fd_len2dlc(cf->len)) |
			fdflags | TX_BUF_EFC;
		err = m_can_fifo_write(cdev, putidx, M_CAN_FIFO_ID, &fifo_header, 2);
		if (err)
			goto out_fail;

		err = m_can_fifo_write(cdev, putidx, M_CAN_FIFO_DATA,
				       cf->data, DIV_ROUND_UP(cf->len, 4));
		if (err)
			goto out_fail;
>>>>>>> upstream/android-13

		/* Push loopback echo.
		 * Will be looped back on TX interrupt based on message marker
		 */
<<<<<<< HEAD
		can_put_echo_skb(skb, dev, putidx);

		/* Enable TX FIFO element to start transfer  */
		m_can_write(priv, M_CAN_TXBAR, (1 << putidx));

		/* stop network queue if fifo full */
			if (m_can_tx_fifo_full(priv) ||
			    m_can_next_echo_skb_occupied(dev, putidx))
				netif_stop_queue(dev);
	}

	return NETDEV_TX_OK;
=======
		can_put_echo_skb(skb, dev, putidx, 0);

		/* Enable TX FIFO element to start transfer  */
		m_can_write(cdev, M_CAN_TXBAR, (1 << putidx));

		/* stop network queue if fifo full */
		if (m_can_tx_fifo_full(cdev) ||
		    m_can_next_echo_skb_occupied(dev, putidx))
			netif_stop_queue(dev);
	}

	return NETDEV_TX_OK;

out_fail:
	netdev_err(dev, "FIFO write returned %d\n", err);
	m_can_disable_all_interrupts(cdev);
	return NETDEV_TX_BUSY;
}

static void m_can_tx_work_queue(struct work_struct *ws)
{
	struct m_can_classdev *cdev = container_of(ws, struct m_can_classdev,
						   tx_work);

	m_can_tx_handler(cdev);
}

static netdev_tx_t m_can_start_xmit(struct sk_buff *skb,
				    struct net_device *dev)
{
	struct m_can_classdev *cdev = netdev_priv(dev);

	if (can_dropped_invalid_skb(dev, skb))
		return NETDEV_TX_OK;

	if (cdev->is_peripheral) {
		if (cdev->tx_skb) {
			netdev_err(dev, "hard_xmit called while tx busy\n");
			return NETDEV_TX_BUSY;
		}

		if (cdev->can.state == CAN_STATE_BUS_OFF) {
			m_can_clean(dev);
		} else {
			/* Need to stop the queue to avoid numerous requests
			 * from being sent.  Suggested improvement is to create
			 * a queueing mechanism that will queue the skbs and
			 * process them in order.
			 */
			cdev->tx_skb = skb;
			netif_stop_queue(cdev->net);
			queue_work(cdev->tx_wq, &cdev->tx_work);
		}
	} else {
		cdev->tx_skb = skb;
		return m_can_tx_handler(cdev);
	}

	return NETDEV_TX_OK;
}

static int m_can_open(struct net_device *dev)
{
	struct m_can_classdev *cdev = netdev_priv(dev);
	int err;

	err = phy_power_on(cdev->transceiver);
	if (err)
		return err;

	err = m_can_clk_start(cdev);
	if (err)
		goto out_phy_power_off;

	/* open the can device */
	err = open_candev(dev);
	if (err) {
		netdev_err(dev, "failed to open can device\n");
		goto exit_disable_clks;
	}

	if (cdev->is_peripheral)
		can_rx_offload_enable(&cdev->offload);

	/* register interrupt handler */
	if (cdev->is_peripheral) {
		cdev->tx_skb = NULL;
		cdev->tx_wq = alloc_workqueue("mcan_wq",
					      WQ_FREEZABLE | WQ_MEM_RECLAIM, 0);
		if (!cdev->tx_wq) {
			err = -ENOMEM;
			goto out_wq_fail;
		}

		INIT_WORK(&cdev->tx_work, m_can_tx_work_queue);

		err = request_threaded_irq(dev->irq, NULL, m_can_isr,
					   IRQF_ONESHOT,
					   dev->name, dev);
	} else {
		err = request_irq(dev->irq, m_can_isr, IRQF_SHARED, dev->name,
				  dev);
	}

	if (err < 0) {
		netdev_err(dev, "failed to request interrupt\n");
		goto exit_irq_fail;
	}

	/* start the m_can controller */
	m_can_start(dev);

	can_led_event(dev, CAN_LED_EVENT_OPEN);

	if (!cdev->is_peripheral)
		napi_enable(&cdev->napi);

	netif_start_queue(dev);

	return 0;

exit_irq_fail:
	if (cdev->is_peripheral)
		destroy_workqueue(cdev->tx_wq);
out_wq_fail:
	if (cdev->is_peripheral)
		can_rx_offload_disable(&cdev->offload);
	close_candev(dev);
exit_disable_clks:
	m_can_clk_stop(cdev);
out_phy_power_off:
	phy_power_off(cdev->transceiver);
	return err;
>>>>>>> upstream/android-13
}

static const struct net_device_ops m_can_netdev_ops = {
	.ndo_open = m_can_open,
	.ndo_stop = m_can_close,
	.ndo_start_xmit = m_can_start_xmit,
	.ndo_change_mtu = can_change_mtu,
};

static int register_m_can_dev(struct net_device *dev)
{
	dev->flags |= IFF_ECHO;	/* we support local echo */
	dev->netdev_ops = &m_can_netdev_ops;

	return register_candev(dev);
}

<<<<<<< HEAD
static void m_can_init_ram(struct m_can_priv *priv)
{
	int end, i, start;
=======
static void m_can_of_parse_mram(struct m_can_classdev *cdev,
				const u32 *mram_config_vals)
{
	cdev->mcfg[MRAM_SIDF].off = mram_config_vals[0];
	cdev->mcfg[MRAM_SIDF].num = mram_config_vals[1];
	cdev->mcfg[MRAM_XIDF].off = cdev->mcfg[MRAM_SIDF].off +
		cdev->mcfg[MRAM_SIDF].num * SIDF_ELEMENT_SIZE;
	cdev->mcfg[MRAM_XIDF].num = mram_config_vals[2];
	cdev->mcfg[MRAM_RXF0].off = cdev->mcfg[MRAM_XIDF].off +
		cdev->mcfg[MRAM_XIDF].num * XIDF_ELEMENT_SIZE;
	cdev->mcfg[MRAM_RXF0].num = mram_config_vals[3] &
		FIELD_MAX(RXFC_FS_MASK);
	cdev->mcfg[MRAM_RXF1].off = cdev->mcfg[MRAM_RXF0].off +
		cdev->mcfg[MRAM_RXF0].num * RXF0_ELEMENT_SIZE;
	cdev->mcfg[MRAM_RXF1].num = mram_config_vals[4] &
		FIELD_MAX(RXFC_FS_MASK);
	cdev->mcfg[MRAM_RXB].off = cdev->mcfg[MRAM_RXF1].off +
		cdev->mcfg[MRAM_RXF1].num * RXF1_ELEMENT_SIZE;
	cdev->mcfg[MRAM_RXB].num = mram_config_vals[5];
	cdev->mcfg[MRAM_TXE].off = cdev->mcfg[MRAM_RXB].off +
		cdev->mcfg[MRAM_RXB].num * RXB_ELEMENT_SIZE;
	cdev->mcfg[MRAM_TXE].num = mram_config_vals[6];
	cdev->mcfg[MRAM_TXB].off = cdev->mcfg[MRAM_TXE].off +
		cdev->mcfg[MRAM_TXE].num * TXE_ELEMENT_SIZE;
	cdev->mcfg[MRAM_TXB].num = mram_config_vals[7] &
		FIELD_MAX(TXBC_NDTB_MASK);

	dev_dbg(cdev->dev,
		"sidf 0x%x %d xidf 0x%x %d rxf0 0x%x %d rxf1 0x%x %d rxb 0x%x %d txe 0x%x %d txb 0x%x %d\n",
		cdev->mcfg[MRAM_SIDF].off, cdev->mcfg[MRAM_SIDF].num,
		cdev->mcfg[MRAM_XIDF].off, cdev->mcfg[MRAM_XIDF].num,
		cdev->mcfg[MRAM_RXF0].off, cdev->mcfg[MRAM_RXF0].num,
		cdev->mcfg[MRAM_RXF1].off, cdev->mcfg[MRAM_RXF1].num,
		cdev->mcfg[MRAM_RXB].off, cdev->mcfg[MRAM_RXB].num,
		cdev->mcfg[MRAM_TXE].off, cdev->mcfg[MRAM_TXE].num,
		cdev->mcfg[MRAM_TXB].off, cdev->mcfg[MRAM_TXB].num);
}

int m_can_init_ram(struct m_can_classdev *cdev)
{
	int end, i, start;
	int err = 0;
>>>>>>> upstream/android-13

	/* initialize the entire Message RAM in use to avoid possible
	 * ECC/parity checksum errors when reading an uninitialized buffer
	 */
<<<<<<< HEAD
	start = priv->mcfg[MRAM_SIDF].off;
	end = priv->mcfg[MRAM_TXB].off +
		priv->mcfg[MRAM_TXB].num * TXB_ELEMENT_SIZE;
	for (i = start; i < end; i += 4)
		writel(0x0, priv->mram_base + i);
}

static void m_can_of_parse_mram(struct m_can_priv *priv,
				const u32 *mram_config_vals)
{
	priv->mcfg[MRAM_SIDF].off = mram_config_vals[0];
	priv->mcfg[MRAM_SIDF].num = mram_config_vals[1];
	priv->mcfg[MRAM_XIDF].off = priv->mcfg[MRAM_SIDF].off +
			priv->mcfg[MRAM_SIDF].num * SIDF_ELEMENT_SIZE;
	priv->mcfg[MRAM_XIDF].num = mram_config_vals[2];
	priv->mcfg[MRAM_RXF0].off = priv->mcfg[MRAM_XIDF].off +
			priv->mcfg[MRAM_XIDF].num * XIDF_ELEMENT_SIZE;
	priv->mcfg[MRAM_RXF0].num = mram_config_vals[3] &
			(RXFC_FS_MASK >> RXFC_FS_SHIFT);
	priv->mcfg[MRAM_RXF1].off = priv->mcfg[MRAM_RXF0].off +
			priv->mcfg[MRAM_RXF0].num * RXF0_ELEMENT_SIZE;
	priv->mcfg[MRAM_RXF1].num = mram_config_vals[4] &
			(RXFC_FS_MASK >> RXFC_FS_SHIFT);
	priv->mcfg[MRAM_RXB].off = priv->mcfg[MRAM_RXF1].off +
			priv->mcfg[MRAM_RXF1].num * RXF1_ELEMENT_SIZE;
	priv->mcfg[MRAM_RXB].num = mram_config_vals[5];
	priv->mcfg[MRAM_TXE].off = priv->mcfg[MRAM_RXB].off +
			priv->mcfg[MRAM_RXB].num * RXB_ELEMENT_SIZE;
	priv->mcfg[MRAM_TXE].num = mram_config_vals[6];
	priv->mcfg[MRAM_TXB].off = priv->mcfg[MRAM_TXE].off +
			priv->mcfg[MRAM_TXE].num * TXE_ELEMENT_SIZE;
	priv->mcfg[MRAM_TXB].num = mram_config_vals[7] &
			(TXBC_NDTB_MASK >> TXBC_NDTB_SHIFT);

	dev_dbg(priv->device,
		"mram_base %p sidf 0x%x %d xidf 0x%x %d rxf0 0x%x %d rxf1 0x%x %d rxb 0x%x %d txe 0x%x %d txb 0x%x %d\n",
		priv->mram_base,
		priv->mcfg[MRAM_SIDF].off, priv->mcfg[MRAM_SIDF].num,
		priv->mcfg[MRAM_XIDF].off, priv->mcfg[MRAM_XIDF].num,
		priv->mcfg[MRAM_RXF0].off, priv->mcfg[MRAM_RXF0].num,
		priv->mcfg[MRAM_RXF1].off, priv->mcfg[MRAM_RXF1].num,
		priv->mcfg[MRAM_RXB].off, priv->mcfg[MRAM_RXB].num,
		priv->mcfg[MRAM_TXE].off, priv->mcfg[MRAM_TXE].num,
		priv->mcfg[MRAM_TXB].off, priv->mcfg[MRAM_TXB].num);

	m_can_init_ram(priv);
}

static int m_can_plat_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	struct m_can_priv *priv;
	struct resource *res;
	void __iomem *addr;
	void __iomem *mram_addr;
	struct clk *hclk, *cclk;
	int irq, ret;
	struct device_node *np;
	u32 mram_config_vals[MRAM_CFG_LEN];
	u32 tx_fifo_size;

	np = pdev->dev.of_node;

	hclk = devm_clk_get(&pdev->dev, "hclk");
	cclk = devm_clk_get(&pdev->dev, "cclk");

	if (IS_ERR(hclk) || IS_ERR(cclk)) {
		dev_err(&pdev->dev, "no clock found\n");
		ret = -ENODEV;
		goto failed_ret;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "m_can");
	addr = devm_ioremap_resource(&pdev->dev, res);
	irq = platform_get_irq_byname(pdev, "int0");

	if (IS_ERR(addr) || irq < 0) {
		ret = -EINVAL;
		goto failed_ret;
	}

	/* message ram could be shared */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "message_ram");
	if (!res) {
		ret = -ENODEV;
		goto failed_ret;
	}

	mram_addr = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!mram_addr) {
		ret = -ENOMEM;
		goto failed_ret;
	}

	/* get message ram configuration */
	ret = of_property_read_u32_array(np, "bosch,mram-cfg",
					 mram_config_vals,
					 sizeof(mram_config_vals) / 4);
	if (ret) {
		dev_err(&pdev->dev, "Could not get Message RAM configuration.");
		goto failed_ret;
=======
	start = cdev->mcfg[MRAM_SIDF].off;
	end = cdev->mcfg[MRAM_TXB].off +
		cdev->mcfg[MRAM_TXB].num * TXB_ELEMENT_SIZE;

	for (i = start; i < end; i += 4) {
		err = m_can_fifo_write_no_off(cdev, i, 0x0);
		if (err)
			break;
	}

	return err;
}
EXPORT_SYMBOL_GPL(m_can_init_ram);

int m_can_class_get_clocks(struct m_can_classdev *cdev)
{
	int ret = 0;

	cdev->hclk = devm_clk_get(cdev->dev, "hclk");
	cdev->cclk = devm_clk_get(cdev->dev, "cclk");

	if (IS_ERR(cdev->cclk)) {
		dev_err(cdev->dev, "no clock found\n");
		ret = -ENODEV;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(m_can_class_get_clocks);

struct m_can_classdev *m_can_class_allocate_dev(struct device *dev,
						int sizeof_priv)
{
	struct m_can_classdev *class_dev = NULL;
	u32 mram_config_vals[MRAM_CFG_LEN];
	struct net_device *net_dev;
	u32 tx_fifo_size;
	int ret;

	ret = fwnode_property_read_u32_array(dev_fwnode(dev),
					     "bosch,mram-cfg",
					     mram_config_vals,
					     sizeof(mram_config_vals) / 4);
	if (ret) {
		dev_err(dev, "Could not get Message RAM configuration.");
		goto out;
>>>>>>> upstream/android-13
	}

	/* Get TX FIFO size
	 * Defines the total amount of echo buffers for loopback
	 */
	tx_fifo_size = mram_config_vals[7];

	/* allocate the m_can device */
<<<<<<< HEAD
	dev = alloc_candev(sizeof(*priv), tx_fifo_size);
	if (!dev) {
		ret = -ENOMEM;
		goto failed_ret;
	}

	priv = netdev_priv(dev);
	dev->irq = irq;
	priv->device = &pdev->dev;
	priv->hclk = hclk;
	priv->cclk = cclk;
	priv->can.clock.freq = clk_get_rate(cclk);
	priv->mram_base = mram_addr;

	platform_set_drvdata(pdev, dev);
	SET_NETDEV_DEV(dev, &pdev->dev);

	/* Enable clocks. Necessary to read Core Release in order to determine
	 * M_CAN version
	 */
	pm_runtime_enable(&pdev->dev);
	ret = m_can_clk_start(priv);
	if (ret)
		goto pm_runtime_fail;

	ret = m_can_dev_setup(pdev, dev, addr);
	if (ret)
		goto clk_disable;

	ret = register_m_can_dev(dev);
	if (ret) {
		dev_err(&pdev->dev, "registering %s failed (err=%d)\n",
			KBUILD_MODNAME, ret);
		goto clk_disable;
	}

	m_can_of_parse_mram(priv, mram_config_vals);

	devm_can_led_init(dev);

	of_can_transceiver(dev);

	dev_info(&pdev->dev, "%s device registered (irq=%d, version=%d)\n",
		 KBUILD_MODNAME, dev->irq, priv->version);
=======
	net_dev = alloc_candev(sizeof_priv, tx_fifo_size);
	if (!net_dev) {
		dev_err(dev, "Failed to allocate CAN device");
		goto out;
	}

	class_dev = netdev_priv(net_dev);
	class_dev->net = net_dev;
	class_dev->dev = dev;
	SET_NETDEV_DEV(net_dev, dev);

	m_can_of_parse_mram(class_dev, mram_config_vals);
out:
	return class_dev;
}
EXPORT_SYMBOL_GPL(m_can_class_allocate_dev);

void m_can_class_free_dev(struct net_device *net)
{
	free_candev(net);
}
EXPORT_SYMBOL_GPL(m_can_class_free_dev);

int m_can_class_register(struct m_can_classdev *cdev)
{
	int ret;

	if (cdev->pm_clock_support) {
		ret = m_can_clk_start(cdev);
		if (ret)
			return ret;
	}

	if (cdev->is_peripheral) {
		ret = can_rx_offload_add_manual(cdev->net, &cdev->offload,
						M_CAN_NAPI_WEIGHT);
		if (ret)
			goto clk_disable;
	}

	ret = m_can_dev_setup(cdev);
	if (ret)
		goto rx_offload_del;

	ret = register_m_can_dev(cdev->net);
	if (ret) {
		dev_err(cdev->dev, "registering %s failed (err=%d)\n",
			cdev->net->name, ret);
		goto rx_offload_del;
	}

	devm_can_led_init(cdev->net);

	of_can_transceiver(cdev->net);

	dev_info(cdev->dev, "%s device registered (irq=%d, version=%d)\n",
		 KBUILD_MODNAME, cdev->net->irq, cdev->version);
>>>>>>> upstream/android-13

	/* Probe finished
	 * Stop clocks. They will be reactivated once the M_CAN device is opened
	 */
<<<<<<< HEAD
clk_disable:
	m_can_clk_stop(priv);
pm_runtime_fail:
	if (ret) {
		pm_runtime_disable(&pdev->dev);
		free_candev(dev);
	}
failed_ret:
	return ret;
}

static __maybe_unused int m_can_suspend(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct m_can_priv *priv = netdev_priv(ndev);
=======
	m_can_clk_stop(cdev);

	return 0;

rx_offload_del:
	if (cdev->is_peripheral)
		can_rx_offload_del(&cdev->offload);
clk_disable:
	m_can_clk_stop(cdev);

	return ret;
}
EXPORT_SYMBOL_GPL(m_can_class_register);

void m_can_class_unregister(struct m_can_classdev *cdev)
{
	if (cdev->is_peripheral)
		can_rx_offload_del(&cdev->offload);
	unregister_candev(cdev->net);
}
EXPORT_SYMBOL_GPL(m_can_class_unregister);

int m_can_class_suspend(struct device *dev)
{
	struct m_can_classdev *cdev = dev_get_drvdata(dev);
	struct net_device *ndev = cdev->net;
>>>>>>> upstream/android-13

	if (netif_running(ndev)) {
		netif_stop_queue(ndev);
		netif_device_detach(ndev);
		m_can_stop(ndev);
<<<<<<< HEAD
		m_can_clk_stop(priv);
=======
		m_can_clk_stop(cdev);
>>>>>>> upstream/android-13
	}

	pinctrl_pm_select_sleep_state(dev);

<<<<<<< HEAD
	priv->can.state = CAN_STATE_SLEEPING;

	return 0;
}

static __maybe_unused int m_can_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct m_can_priv *priv = netdev_priv(ndev);

	pinctrl_pm_select_default_state(dev);

	priv->can.state = CAN_STATE_ERROR_ACTIVE;
=======
	cdev->can.state = CAN_STATE_SLEEPING;

	return 0;
}
EXPORT_SYMBOL_GPL(m_can_class_suspend);

int m_can_class_resume(struct device *dev)
{
	struct m_can_classdev *cdev = dev_get_drvdata(dev);
	struct net_device *ndev = cdev->net;

	pinctrl_pm_select_default_state(dev);

	cdev->can.state = CAN_STATE_ERROR_ACTIVE;
>>>>>>> upstream/android-13

	if (netif_running(ndev)) {
		int ret;

<<<<<<< HEAD
		ret = m_can_clk_start(priv);
		if (ret)
			return ret;

		m_can_init_ram(priv);
=======
		ret = m_can_clk_start(cdev);
		if (ret)
			return ret;

		m_can_init_ram(cdev);
>>>>>>> upstream/android-13
		m_can_start(ndev);
		netif_device_attach(ndev);
		netif_start_queue(ndev);
	}

	return 0;
}
<<<<<<< HEAD

static void unregister_m_can_dev(struct net_device *dev)
{
	unregister_candev(dev);
}

static int m_can_plat_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);

	unregister_m_can_dev(dev);

	pm_runtime_disable(&pdev->dev);

	platform_set_drvdata(pdev, NULL);

	free_candev(dev);

	return 0;
}

static int __maybe_unused m_can_runtime_suspend(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct m_can_priv *priv = netdev_priv(ndev);

	clk_disable_unprepare(priv->cclk);
	clk_disable_unprepare(priv->hclk);

	return 0;
}

static int __maybe_unused m_can_runtime_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct m_can_priv *priv = netdev_priv(ndev);
	int err;

	err = clk_prepare_enable(priv->hclk);
	if (err)
		return err;

	err = clk_prepare_enable(priv->cclk);
	if (err)
		clk_disable_unprepare(priv->hclk);

	return err;
}

static const struct dev_pm_ops m_can_pmops = {
	SET_RUNTIME_PM_OPS(m_can_runtime_suspend,
			   m_can_runtime_resume, NULL)
	SET_SYSTEM_SLEEP_PM_OPS(m_can_suspend, m_can_resume)
};

static const struct of_device_id m_can_of_table[] = {
	{ .compatible = "bosch,m_can", .data = NULL },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, m_can_of_table);

static struct platform_driver m_can_plat_driver = {
	.driver = {
		.name = KBUILD_MODNAME,
		.of_match_table = m_can_of_table,
		.pm     = &m_can_pmops,
	},
	.probe = m_can_plat_probe,
	.remove = m_can_plat_remove,
};

module_platform_driver(m_can_plat_driver);

MODULE_AUTHOR("Dong Aisheng <b29396@freescale.com>");
=======
EXPORT_SYMBOL_GPL(m_can_class_resume);

MODULE_AUTHOR("Dong Aisheng <b29396@freescale.com>");
MODULE_AUTHOR("Dan Murphy <dmurphy@ti.com>");
>>>>>>> upstream/android-13
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("CAN bus driver for Bosch M_CAN controller");
