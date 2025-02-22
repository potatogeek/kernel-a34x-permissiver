<<<<<<< HEAD
/*
 * SuperH MSIOF SPI Master Interface
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * SuperH MSIOF SPI Controller Interface
>>>>>>> upstream/android-13
 *
 * Copyright (c) 2009 Magnus Damm
 * Copyright (C) 2014 Renesas Electronics Corporation
 * Copyright (C) 2014-2017 Glider bvba
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/bitmap.h>
#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/io.h>
=======
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/iopoll.h>
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/sh_dma.h>

#include <linux/spi/sh_msiof.h>
#include <linux/spi/spi.h>

#include <asm/unaligned.h>

struct sh_msiof_chipdata {
<<<<<<< HEAD
	u16 tx_fifo_size;
	u16 rx_fifo_size;
	u16 master_flags;
=======
	u32 bits_per_word_mask;
	u16 tx_fifo_size;
	u16 rx_fifo_size;
	u16 ctlr_flags;
>>>>>>> upstream/android-13
	u16 min_div_pow;
};

struct sh_msiof_spi_priv {
<<<<<<< HEAD
	struct spi_master *master;
=======
	struct spi_controller *ctlr;
>>>>>>> upstream/android-13
	void __iomem *mapbase;
	struct clk *clk;
	struct platform_device *pdev;
	struct sh_msiof_spi_info *info;
	struct completion done;
	struct completion done_txdma;
	unsigned int tx_fifo_size;
	unsigned int rx_fifo_size;
	unsigned int min_div_pow;
	void *tx_dma_page;
	void *rx_dma_page;
	dma_addr_t tx_dma_addr;
	dma_addr_t rx_dma_addr;
<<<<<<< HEAD
	unsigned short unused_ss;
=======
>>>>>>> upstream/android-13
	bool native_cs_inited;
	bool native_cs_high;
	bool slave_aborted;
};

#define MAX_SS	3	/* Maximum number of native chip selects */

<<<<<<< HEAD
#define TMDR1	0x00	/* Transmit Mode Register 1 */
#define TMDR2	0x04	/* Transmit Mode Register 2 */
#define TMDR3	0x08	/* Transmit Mode Register 3 */
#define RMDR1	0x10	/* Receive Mode Register 1 */
#define RMDR2	0x14	/* Receive Mode Register 2 */
#define RMDR3	0x18	/* Receive Mode Register 3 */
#define TSCR	0x20	/* Transmit Clock Select Register */
#define RSCR	0x22	/* Receive Clock Select Register (SH, A1, APE6) */
#define CTR	0x28	/* Control Register */
#define FCTR	0x30	/* FIFO Control Register */
#define STR	0x40	/* Status Register */
#define IER	0x44	/* Interrupt Enable Register */
#define TDR1	0x48	/* Transmit Control Data Register 1 (SH, A1) */
#define TDR2	0x4c	/* Transmit Control Data Register 2 (SH, A1) */
#define TFDR	0x50	/* Transmit FIFO Data Register */
#define RDR1	0x58	/* Receive Control Data Register 1 (SH, A1) */
#define RDR2	0x5c	/* Receive Control Data Register 2 (SH, A1) */
#define RFDR	0x60	/* Receive FIFO Data Register */

/* TMDR1 and RMDR1 */
#define MDR1_TRMD	 0x80000000 /* Transfer Mode (1 = Master mode) */
#define MDR1_SYNCMD_MASK 0x30000000 /* SYNC Mode */
#define MDR1_SYNCMD_SPI	 0x20000000 /*   Level mode/SPI */
#define MDR1_SYNCMD_LR	 0x30000000 /*   L/R mode */
#define MDR1_SYNCAC_SHIFT	 25 /* Sync Polarity (1 = Active-low) */
#define MDR1_BITLSB_SHIFT	 24 /* MSB/LSB First (1 = LSB first) */
#define MDR1_DTDL_SHIFT		 20 /* Data Pin Bit Delay for MSIOF_SYNC */
#define MDR1_SYNCDL_SHIFT	 16 /* Frame Sync Signal Timing Delay */
#define MDR1_FLD_MASK	 0x0000000c /* Frame Sync Signal Interval (0-3) */
#define MDR1_FLD_SHIFT		  2
#define MDR1_XXSTP	 0x00000001 /* Transmission/Reception Stop on FIFO */
/* TMDR1 */
#define TMDR1_PCON	 0x40000000 /* Transfer Signal Connection */
#define TMDR1_SYNCCH_MASK 0xc000000 /* Synchronization Signal Channel Select */
#define TMDR1_SYNCCH_SHIFT	 26 /* 0=MSIOF_SYNC, 1=MSIOF_SS1, 2=MSIOF_SS2 */

/* TMDR2 and RMDR2 */
#define MDR2_BITLEN1(i)	(((i) - 1) << 24) /* Data Size (8-32 bits) */
#define MDR2_WDLEN1(i)	(((i) - 1) << 16) /* Word Count (1-64/256 (SH, A1))) */
#define MDR2_GRPMASK1	0x00000001 /* Group Output Mask 1 (SH, A1) */

/* TSCR and RSCR */
#define SCR_BRPS_MASK	    0x1f00 /* Prescaler Setting (1-32) */
#define SCR_BRPS(i)	(((i) - 1) << 8)
#define SCR_BRDV_MASK	    0x0007 /* Baud Rate Generator's Division Ratio */
#define SCR_BRDV_DIV_2	    0x0000
#define SCR_BRDV_DIV_4	    0x0001
#define SCR_BRDV_DIV_8	    0x0002
#define SCR_BRDV_DIV_16	    0x0003
#define SCR_BRDV_DIV_32	    0x0004
#define SCR_BRDV_DIV_1	    0x0007

/* CTR */
#define CTR_TSCKIZ_MASK	0xc0000000 /* Transmit Clock I/O Polarity Select */
#define CTR_TSCKIZ_SCK	0x80000000 /*   Disable SCK when TX disabled */
#define CTR_TSCKIZ_POL_SHIFT	30 /*   Transmit Clock Polarity */
#define CTR_RSCKIZ_MASK	0x30000000 /* Receive Clock Polarity Select */
#define CTR_RSCKIZ_SCK	0x20000000 /*   Must match CTR_TSCKIZ_SCK */
#define CTR_RSCKIZ_POL_SHIFT	28 /*   Receive Clock Polarity */
#define CTR_TEDG_SHIFT		27 /* Transmit Timing (1 = falling edge) */
#define CTR_REDG_SHIFT		26 /* Receive Timing (1 = falling edge) */
#define CTR_TXDIZ_MASK	0x00c00000 /* Pin Output When TX is Disabled */
#define CTR_TXDIZ_LOW	0x00000000 /*   0 */
#define CTR_TXDIZ_HIGH	0x00400000 /*   1 */
#define CTR_TXDIZ_HIZ	0x00800000 /*   High-impedance */
#define CTR_TSCKE	0x00008000 /* Transmit Serial Clock Output Enable */
#define CTR_TFSE	0x00004000 /* Transmit Frame Sync Signal Output Enable */
#define CTR_TXE		0x00000200 /* Transmit Enable */
#define CTR_RXE		0x00000100 /* Receive Enable */

/* FCTR */
#define FCTR_TFWM_MASK	0xe0000000 /* Transmit FIFO Watermark */
#define FCTR_TFWM_64	0x00000000 /*  Transfer Request when 64 empty stages */
#define FCTR_TFWM_32	0x20000000 /*  Transfer Request when 32 empty stages */
#define FCTR_TFWM_24	0x40000000 /*  Transfer Request when 24 empty stages */
#define FCTR_TFWM_16	0x60000000 /*  Transfer Request when 16 empty stages */
#define FCTR_TFWM_12	0x80000000 /*  Transfer Request when 12 empty stages */
#define FCTR_TFWM_8	0xa0000000 /*  Transfer Request when 8 empty stages */
#define FCTR_TFWM_4	0xc0000000 /*  Transfer Request when 4 empty stages */
#define FCTR_TFWM_1	0xe0000000 /*  Transfer Request when 1 empty stage */
#define FCTR_TFUA_MASK	0x07f00000 /* Transmit FIFO Usable Area */
#define FCTR_TFUA_SHIFT		20
#define FCTR_TFUA(i)	((i) << FCTR_TFUA_SHIFT)
#define FCTR_RFWM_MASK	0x0000e000 /* Receive FIFO Watermark */
#define FCTR_RFWM_1	0x00000000 /*  Transfer Request when 1 valid stages */
#define FCTR_RFWM_4	0x00002000 /*  Transfer Request when 4 valid stages */
#define FCTR_RFWM_8	0x00004000 /*  Transfer Request when 8 valid stages */
#define FCTR_RFWM_16	0x00006000 /*  Transfer Request when 16 valid stages */
#define FCTR_RFWM_32	0x00008000 /*  Transfer Request when 32 valid stages */
#define FCTR_RFWM_64	0x0000a000 /*  Transfer Request when 64 valid stages */
#define FCTR_RFWM_128	0x0000c000 /*  Transfer Request when 128 valid stages */
#define FCTR_RFWM_256	0x0000e000 /*  Transfer Request when 256 valid stages */
#define FCTR_RFUA_MASK	0x00001ff0 /* Receive FIFO Usable Area (0x40 = full) */
#define FCTR_RFUA_SHIFT		 4
#define FCTR_RFUA(i)	((i) << FCTR_RFUA_SHIFT)

/* STR */
#define STR_TFEMP	0x20000000 /* Transmit FIFO Empty */
#define STR_TDREQ	0x10000000 /* Transmit Data Transfer Request */
#define STR_TEOF	0x00800000 /* Frame Transmission End */
#define STR_TFSERR	0x00200000 /* Transmit Frame Synchronization Error */
#define STR_TFOVF	0x00100000 /* Transmit FIFO Overflow */
#define STR_TFUDF	0x00080000 /* Transmit FIFO Underflow */
#define STR_RFFUL	0x00002000 /* Receive FIFO Full */
#define STR_RDREQ	0x00001000 /* Receive Data Transfer Request */
#define STR_REOF	0x00000080 /* Frame Reception End */
#define STR_RFSERR	0x00000020 /* Receive Frame Synchronization Error */
#define STR_RFUDF	0x00000010 /* Receive FIFO Underflow */
#define STR_RFOVF	0x00000008 /* Receive FIFO Overflow */

/* IER */
#define IER_TDMAE	0x80000000 /* Transmit Data DMA Transfer Req. Enable */
#define IER_TFEMPE	0x20000000 /* Transmit FIFO Empty Enable */
#define IER_TDREQE	0x10000000 /* Transmit Data Transfer Request Enable */
#define IER_TEOFE	0x00800000 /* Frame Transmission End Enable */
#define IER_TFSERRE	0x00200000 /* Transmit Frame Sync Error Enable */
#define IER_TFOVFE	0x00100000 /* Transmit FIFO Overflow Enable */
#define IER_TFUDFE	0x00080000 /* Transmit FIFO Underflow Enable */
#define IER_RDMAE	0x00008000 /* Receive Data DMA Transfer Req. Enable */
#define IER_RFFULE	0x00002000 /* Receive FIFO Full Enable */
#define IER_RDREQE	0x00001000 /* Receive Data Transfer Request Enable */
#define IER_REOFE	0x00000080 /* Frame Reception End Enable */
#define IER_RFSERRE	0x00000020 /* Receive Frame Sync Error Enable */
#define IER_RFUDFE	0x00000010 /* Receive FIFO Underflow Enable */
#define IER_RFOVFE	0x00000008 /* Receive FIFO Overflow Enable */
=======
#define SITMDR1	0x00	/* Transmit Mode Register 1 */
#define SITMDR2	0x04	/* Transmit Mode Register 2 */
#define SITMDR3	0x08	/* Transmit Mode Register 3 */
#define SIRMDR1	0x10	/* Receive Mode Register 1 */
#define SIRMDR2	0x14	/* Receive Mode Register 2 */
#define SIRMDR3	0x18	/* Receive Mode Register 3 */
#define SITSCR	0x20	/* Transmit Clock Select Register */
#define SIRSCR	0x22	/* Receive Clock Select Register (SH, A1, APE6) */
#define SICTR	0x28	/* Control Register */
#define SIFCTR	0x30	/* FIFO Control Register */
#define SISTR	0x40	/* Status Register */
#define SIIER	0x44	/* Interrupt Enable Register */
#define SITDR1	0x48	/* Transmit Control Data Register 1 (SH, A1) */
#define SITDR2	0x4c	/* Transmit Control Data Register 2 (SH, A1) */
#define SITFDR	0x50	/* Transmit FIFO Data Register */
#define SIRDR1	0x58	/* Receive Control Data Register 1 (SH, A1) */
#define SIRDR2	0x5c	/* Receive Control Data Register 2 (SH, A1) */
#define SIRFDR	0x60	/* Receive FIFO Data Register */

/* SITMDR1 and SIRMDR1 */
#define SIMDR1_TRMD		BIT(31)		/* Transfer Mode (1 = Master mode) */
#define SIMDR1_SYNCMD_MASK	GENMASK(29, 28)	/* SYNC Mode */
#define SIMDR1_SYNCMD_SPI	(2 << 28)	/*   Level mode/SPI */
#define SIMDR1_SYNCMD_LR	(3 << 28)	/*   L/R mode */
#define SIMDR1_SYNCAC_SHIFT	25		/* Sync Polarity (1 = Active-low) */
#define SIMDR1_BITLSB_SHIFT	24		/* MSB/LSB First (1 = LSB first) */
#define SIMDR1_DTDL_SHIFT	20		/* Data Pin Bit Delay for MSIOF_SYNC */
#define SIMDR1_SYNCDL_SHIFT	16		/* Frame Sync Signal Timing Delay */
#define SIMDR1_FLD_MASK		GENMASK(3, 2)	/* Frame Sync Signal Interval (0-3) */
#define SIMDR1_FLD_SHIFT	2
#define SIMDR1_XXSTP		BIT(0)		/* Transmission/Reception Stop on FIFO */
/* SITMDR1 */
#define SITMDR1_PCON		BIT(30)		/* Transfer Signal Connection */
#define SITMDR1_SYNCCH_MASK	GENMASK(27, 26)	/* Sync Signal Channel Select */
#define SITMDR1_SYNCCH_SHIFT	26		/* 0=MSIOF_SYNC, 1=MSIOF_SS1, 2=MSIOF_SS2 */

/* SITMDR2 and SIRMDR2 */
#define SIMDR2_BITLEN1(i)	(((i) - 1) << 24) /* Data Size (8-32 bits) */
#define SIMDR2_WDLEN1(i)	(((i) - 1) << 16) /* Word Count (1-64/256 (SH, A1))) */
#define SIMDR2_GRPMASK1		BIT(0)		/* Group Output Mask 1 (SH, A1) */

/* SITSCR and SIRSCR */
#define SISCR_BRPS_MASK		GENMASK(12, 8)	/* Prescaler Setting (1-32) */
#define SISCR_BRPS(i)		(((i) - 1) << 8)
#define SISCR_BRDV_MASK		GENMASK(2, 0)	/* Baud Rate Generator's Division Ratio */
#define SISCR_BRDV_DIV_2	0
#define SISCR_BRDV_DIV_4	1
#define SISCR_BRDV_DIV_8	2
#define SISCR_BRDV_DIV_16	3
#define SISCR_BRDV_DIV_32	4
#define SISCR_BRDV_DIV_1	7

/* SICTR */
#define SICTR_TSCKIZ_MASK	GENMASK(31, 30)	/* Transmit Clock I/O Polarity Select */
#define SICTR_TSCKIZ_SCK	BIT(31)		/*   Disable SCK when TX disabled */
#define SICTR_TSCKIZ_POL_SHIFT	30		/*   Transmit Clock Polarity */
#define SICTR_RSCKIZ_MASK	GENMASK(29, 28) /* Receive Clock Polarity Select */
#define SICTR_RSCKIZ_SCK	BIT(29)		/*   Must match CTR_TSCKIZ_SCK */
#define SICTR_RSCKIZ_POL_SHIFT	28		/*   Receive Clock Polarity */
#define SICTR_TEDG_SHIFT	27		/* Transmit Timing (1 = falling edge) */
#define SICTR_REDG_SHIFT	26		/* Receive Timing (1 = falling edge) */
#define SICTR_TXDIZ_MASK	GENMASK(23, 22)	/* Pin Output When TX is Disabled */
#define SICTR_TXDIZ_LOW		(0 << 22)	/*   0 */
#define SICTR_TXDIZ_HIGH	(1 << 22)	/*   1 */
#define SICTR_TXDIZ_HIZ		(2 << 22)	/*   High-impedance */
#define SICTR_TSCKE		BIT(15)		/* Transmit Serial Clock Output Enable */
#define SICTR_TFSE		BIT(14)		/* Transmit Frame Sync Signal Output Enable */
#define SICTR_TXE		BIT(9)		/* Transmit Enable */
#define SICTR_RXE		BIT(8)		/* Receive Enable */
#define SICTR_TXRST		BIT(1)		/* Transmit Reset */
#define SICTR_RXRST		BIT(0)		/* Receive Reset */

/* SIFCTR */
#define SIFCTR_TFWM_MASK	GENMASK(31, 29)	/* Transmit FIFO Watermark */
#define SIFCTR_TFWM_64		(0 << 29)	/*  Transfer Request when 64 empty stages */
#define SIFCTR_TFWM_32		(1 << 29)	/*  Transfer Request when 32 empty stages */
#define SIFCTR_TFWM_24		(2 << 29)	/*  Transfer Request when 24 empty stages */
#define SIFCTR_TFWM_16		(3 << 29)	/*  Transfer Request when 16 empty stages */
#define SIFCTR_TFWM_12		(4 << 29)	/*  Transfer Request when 12 empty stages */
#define SIFCTR_TFWM_8		(5 << 29)	/*  Transfer Request when 8 empty stages */
#define SIFCTR_TFWM_4		(6 << 29)	/*  Transfer Request when 4 empty stages */
#define SIFCTR_TFWM_1		(7 << 29)	/*  Transfer Request when 1 empty stage */
#define SIFCTR_TFUA_MASK	GENMASK(26, 20) /* Transmit FIFO Usable Area */
#define SIFCTR_TFUA_SHIFT	20
#define SIFCTR_TFUA(i)		((i) << SIFCTR_TFUA_SHIFT)
#define SIFCTR_RFWM_MASK	GENMASK(15, 13)	/* Receive FIFO Watermark */
#define SIFCTR_RFWM_1		(0 << 13)	/*  Transfer Request when 1 valid stages */
#define SIFCTR_RFWM_4		(1 << 13)	/*  Transfer Request when 4 valid stages */
#define SIFCTR_RFWM_8		(2 << 13)	/*  Transfer Request when 8 valid stages */
#define SIFCTR_RFWM_16		(3 << 13)	/*  Transfer Request when 16 valid stages */
#define SIFCTR_RFWM_32		(4 << 13)	/*  Transfer Request when 32 valid stages */
#define SIFCTR_RFWM_64		(5 << 13)	/*  Transfer Request when 64 valid stages */
#define SIFCTR_RFWM_128		(6 << 13)	/*  Transfer Request when 128 valid stages */
#define SIFCTR_RFWM_256		(7 << 13)	/*  Transfer Request when 256 valid stages */
#define SIFCTR_RFUA_MASK	GENMASK(12, 4)	/* Receive FIFO Usable Area (0x40 = full) */
#define SIFCTR_RFUA_SHIFT	4
#define SIFCTR_RFUA(i)		((i) << SIFCTR_RFUA_SHIFT)

/* SISTR */
#define SISTR_TFEMP		BIT(29) /* Transmit FIFO Empty */
#define SISTR_TDREQ		BIT(28) /* Transmit Data Transfer Request */
#define SISTR_TEOF		BIT(23) /* Frame Transmission End */
#define SISTR_TFSERR		BIT(21) /* Transmit Frame Synchronization Error */
#define SISTR_TFOVF		BIT(20) /* Transmit FIFO Overflow */
#define SISTR_TFUDF		BIT(19) /* Transmit FIFO Underflow */
#define SISTR_RFFUL		BIT(13) /* Receive FIFO Full */
#define SISTR_RDREQ		BIT(12) /* Receive Data Transfer Request */
#define SISTR_REOF		BIT(7)  /* Frame Reception End */
#define SISTR_RFSERR		BIT(5)  /* Receive Frame Synchronization Error */
#define SISTR_RFUDF		BIT(4)  /* Receive FIFO Underflow */
#define SISTR_RFOVF		BIT(3)  /* Receive FIFO Overflow */

/* SIIER */
#define SIIER_TDMAE		BIT(31) /* Transmit Data DMA Transfer Req. Enable */
#define SIIER_TFEMPE		BIT(29) /* Transmit FIFO Empty Enable */
#define SIIER_TDREQE		BIT(28) /* Transmit Data Transfer Request Enable */
#define SIIER_TEOFE		BIT(23) /* Frame Transmission End Enable */
#define SIIER_TFSERRE		BIT(21) /* Transmit Frame Sync Error Enable */
#define SIIER_TFOVFE		BIT(20) /* Transmit FIFO Overflow Enable */
#define SIIER_TFUDFE		BIT(19) /* Transmit FIFO Underflow Enable */
#define SIIER_RDMAE		BIT(15) /* Receive Data DMA Transfer Req. Enable */
#define SIIER_RFFULE		BIT(13) /* Receive FIFO Full Enable */
#define SIIER_RDREQE		BIT(12) /* Receive Data Transfer Request Enable */
#define SIIER_REOFE		BIT(7)  /* Frame Reception End Enable */
#define SIIER_RFSERRE		BIT(5)  /* Receive Frame Sync Error Enable */
#define SIIER_RFUDFE		BIT(4)  /* Receive FIFO Underflow Enable */
#define SIIER_RFOVFE		BIT(3)  /* Receive FIFO Overflow Enable */
>>>>>>> upstream/android-13


static u32 sh_msiof_read(struct sh_msiof_spi_priv *p, int reg_offs)
{
	switch (reg_offs) {
<<<<<<< HEAD
	case TSCR:
	case RSCR:
=======
	case SITSCR:
	case SIRSCR:
>>>>>>> upstream/android-13
		return ioread16(p->mapbase + reg_offs);
	default:
		return ioread32(p->mapbase + reg_offs);
	}
}

static void sh_msiof_write(struct sh_msiof_spi_priv *p, int reg_offs,
			   u32 value)
{
	switch (reg_offs) {
<<<<<<< HEAD
	case TSCR:
	case RSCR:
=======
	case SITSCR:
	case SIRSCR:
>>>>>>> upstream/android-13
		iowrite16(value, p->mapbase + reg_offs);
		break;
	default:
		iowrite32(value, p->mapbase + reg_offs);
		break;
	}
}

static int sh_msiof_modify_ctr_wait(struct sh_msiof_spi_priv *p,
				    u32 clr, u32 set)
{
	u32 mask = clr | set;
	u32 data;
<<<<<<< HEAD
	int k;

	data = sh_msiof_read(p, CTR);
	data &= ~clr;
	data |= set;
	sh_msiof_write(p, CTR, data);

	for (k = 100; k > 0; k--) {
		if ((sh_msiof_read(p, CTR) & mask) == set)
			break;

		udelay(10);
	}

	return k > 0 ? 0 : -ETIMEDOUT;
=======

	data = sh_msiof_read(p, SICTR);
	data &= ~clr;
	data |= set;
	sh_msiof_write(p, SICTR, data);

	return readl_poll_timeout_atomic(p->mapbase + SICTR, data,
					 (data & mask) == set, 1, 100);
>>>>>>> upstream/android-13
}

static irqreturn_t sh_msiof_spi_irq(int irq, void *data)
{
	struct sh_msiof_spi_priv *p = data;

	/* just disable the interrupt and wake up */
<<<<<<< HEAD
	sh_msiof_write(p, IER, 0);
=======
	sh_msiof_write(p, SIIER, 0);
>>>>>>> upstream/android-13
	complete(&p->done);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static const u32 sh_msiof_spi_div_array[] = {
	SCR_BRDV_DIV_1, SCR_BRDV_DIV_2,	 SCR_BRDV_DIV_4,
	SCR_BRDV_DIV_8,	SCR_BRDV_DIV_16, SCR_BRDV_DIV_32,
};

static void sh_msiof_spi_set_clk_regs(struct sh_msiof_spi_priv *p,
				      unsigned long parent_rate, u32 spi_hz)
{
	unsigned long div;
	u32 brps, scr;
	unsigned int div_pow = p->min_div_pow;
=======
static void sh_msiof_spi_reset_regs(struct sh_msiof_spi_priv *p)
{
	u32 mask = SICTR_TXRST | SICTR_RXRST;
	u32 data;

	data = sh_msiof_read(p, SICTR);
	data |= mask;
	sh_msiof_write(p, SICTR, data);

	readl_poll_timeout_atomic(p->mapbase + SICTR, data, !(data & mask), 1,
				  100);
}

static const u32 sh_msiof_spi_div_array[] = {
	SISCR_BRDV_DIV_1, SISCR_BRDV_DIV_2, SISCR_BRDV_DIV_4,
	SISCR_BRDV_DIV_8, SISCR_BRDV_DIV_16, SISCR_BRDV_DIV_32,
};

static void sh_msiof_spi_set_clk_regs(struct sh_msiof_spi_priv *p,
				      struct spi_transfer *t)
{
	unsigned long parent_rate = clk_get_rate(p->clk);
	unsigned int div_pow = p->min_div_pow;
	u32 spi_hz = t->speed_hz;
	unsigned long div;
	u32 brps, scr;
>>>>>>> upstream/android-13

	if (!spi_hz || !parent_rate) {
		WARN(1, "Invalid clock rate parameters %lu and %u\n",
		     parent_rate, spi_hz);
		return;
	}

	div = DIV_ROUND_UP(parent_rate, spi_hz);
	if (div <= 1024) {
<<<<<<< HEAD
		/* SCR_BRDV_DIV_1 is valid only if BRPS is x 1/1 or x 1/2 */
=======
		/* SISCR_BRDV_DIV_1 is valid only if BRPS is x 1/1 or x 1/2 */
>>>>>>> upstream/android-13
		if (!div_pow && div <= 32 && div > 2)
			div_pow = 1;

		if (div_pow)
			brps = (div + 1) >> div_pow;
		else
			brps = div;

		for (; brps > 32; div_pow++)
			brps = (brps + 1) >> 1;
	} else {
		/* Set transfer rate composite divisor to 2^5 * 32 = 1024 */
		dev_err(&p->pdev->dev,
			"Requested SPI transfer rate %d is too low\n", spi_hz);
		div_pow = 5;
		brps = 32;
	}

<<<<<<< HEAD
	scr = sh_msiof_spi_div_array[div_pow] | SCR_BRPS(brps);
	sh_msiof_write(p, TSCR, scr);
	if (!(p->master->flags & SPI_MASTER_MUST_TX))
		sh_msiof_write(p, RSCR, scr);
=======
	t->effective_speed_hz = parent_rate / (brps << div_pow);

	scr = sh_msiof_spi_div_array[div_pow] | SISCR_BRPS(brps);
	sh_msiof_write(p, SITSCR, scr);
	if (!(p->ctlr->flags & SPI_CONTROLLER_MUST_TX))
		sh_msiof_write(p, SIRSCR, scr);
>>>>>>> upstream/android-13
}

static u32 sh_msiof_get_delay_bit(u32 dtdl_or_syncdl)
{
	/*
	 * DTDL/SYNCDL bit	: p->info->dtdl or p->info->syncdl
	 * b'000		: 0
	 * b'001		: 100
	 * b'010		: 200
	 * b'011 (SYNCDL only)	: 300
	 * b'101		: 50
	 * b'110		: 150
	 */
	if (dtdl_or_syncdl % 100)
		return dtdl_or_syncdl / 100 + 5;
	else
		return dtdl_or_syncdl / 100;
}

static u32 sh_msiof_spi_get_dtdl_and_syncdl(struct sh_msiof_spi_priv *p)
{
	u32 val;

	if (!p->info)
		return 0;

	/* check if DTDL and SYNCDL is allowed value */
	if (p->info->dtdl > 200 || p->info->syncdl > 300) {
		dev_warn(&p->pdev->dev, "DTDL or SYNCDL is too large\n");
		return 0;
	}

	/* check if the sum of DTDL and SYNCDL becomes an integer value  */
	if ((p->info->dtdl + p->info->syncdl) % 100) {
		dev_warn(&p->pdev->dev, "the sum of DTDL/SYNCDL is not good\n");
		return 0;
	}

<<<<<<< HEAD
	val = sh_msiof_get_delay_bit(p->info->dtdl) << MDR1_DTDL_SHIFT;
	val |= sh_msiof_get_delay_bit(p->info->syncdl) << MDR1_SYNCDL_SHIFT;
=======
	val = sh_msiof_get_delay_bit(p->info->dtdl) << SIMDR1_DTDL_SHIFT;
	val |= sh_msiof_get_delay_bit(p->info->syncdl) << SIMDR1_SYNCDL_SHIFT;
>>>>>>> upstream/android-13

	return val;
}

static void sh_msiof_spi_set_pin_regs(struct sh_msiof_spi_priv *p, u32 ss,
				      u32 cpol, u32 cpha,
				      u32 tx_hi_z, u32 lsb_first, u32 cs_high)
{
	u32 tmp;
	int edge;

	/*
	 * CPOL CPHA     TSCKIZ RSCKIZ TEDG REDG
	 *    0    0         10     10    1    1
	 *    0    1         10     10    0    0
	 *    1    0         11     11    0    0
	 *    1    1         11     11    1    1
	 */
<<<<<<< HEAD
	tmp = MDR1_SYNCMD_SPI | 1 << MDR1_FLD_SHIFT | MDR1_XXSTP;
	tmp |= !cs_high << MDR1_SYNCAC_SHIFT;
	tmp |= lsb_first << MDR1_BITLSB_SHIFT;
	tmp |= sh_msiof_spi_get_dtdl_and_syncdl(p);
	if (spi_controller_is_slave(p->master)) {
		sh_msiof_write(p, TMDR1, tmp | TMDR1_PCON);
	} else {
		sh_msiof_write(p, TMDR1,
			       tmp | MDR1_TRMD | TMDR1_PCON |
			       (ss < MAX_SS ? ss : 0) << TMDR1_SYNCCH_SHIFT);
	}
	if (p->master->flags & SPI_MASTER_MUST_TX) {
		/* These bits are reserved if RX needs TX */
		tmp &= ~0x0000ffff;
	}
	sh_msiof_write(p, RMDR1, tmp);

	tmp = 0;
	tmp |= CTR_TSCKIZ_SCK | cpol << CTR_TSCKIZ_POL_SHIFT;
	tmp |= CTR_RSCKIZ_SCK | cpol << CTR_RSCKIZ_POL_SHIFT;

	edge = cpol ^ !cpha;

	tmp |= edge << CTR_TEDG_SHIFT;
	tmp |= edge << CTR_REDG_SHIFT;
	tmp |= tx_hi_z ? CTR_TXDIZ_HIZ : CTR_TXDIZ_LOW;
	sh_msiof_write(p, CTR, tmp);
=======
	tmp = SIMDR1_SYNCMD_SPI | 1 << SIMDR1_FLD_SHIFT | SIMDR1_XXSTP;
	tmp |= !cs_high << SIMDR1_SYNCAC_SHIFT;
	tmp |= lsb_first << SIMDR1_BITLSB_SHIFT;
	tmp |= sh_msiof_spi_get_dtdl_and_syncdl(p);
	if (spi_controller_is_slave(p->ctlr)) {
		sh_msiof_write(p, SITMDR1, tmp | SITMDR1_PCON);
	} else {
		sh_msiof_write(p, SITMDR1,
			       tmp | SIMDR1_TRMD | SITMDR1_PCON |
			       (ss < MAX_SS ? ss : 0) << SITMDR1_SYNCCH_SHIFT);
	}
	if (p->ctlr->flags & SPI_CONTROLLER_MUST_TX) {
		/* These bits are reserved if RX needs TX */
		tmp &= ~0x0000ffff;
	}
	sh_msiof_write(p, SIRMDR1, tmp);

	tmp = 0;
	tmp |= SICTR_TSCKIZ_SCK | cpol << SICTR_TSCKIZ_POL_SHIFT;
	tmp |= SICTR_RSCKIZ_SCK | cpol << SICTR_RSCKIZ_POL_SHIFT;

	edge = cpol ^ !cpha;

	tmp |= edge << SICTR_TEDG_SHIFT;
	tmp |= edge << SICTR_REDG_SHIFT;
	tmp |= tx_hi_z ? SICTR_TXDIZ_HIZ : SICTR_TXDIZ_LOW;
	sh_msiof_write(p, SICTR, tmp);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_set_mode_regs(struct sh_msiof_spi_priv *p,
				       const void *tx_buf, void *rx_buf,
				       u32 bits, u32 words)
{
<<<<<<< HEAD
	u32 dr2 = MDR2_BITLEN1(bits) | MDR2_WDLEN1(words);

	if (tx_buf || (p->master->flags & SPI_MASTER_MUST_TX))
		sh_msiof_write(p, TMDR2, dr2);
	else
		sh_msiof_write(p, TMDR2, dr2 | MDR2_GRPMASK1);

	if (rx_buf)
		sh_msiof_write(p, RMDR2, dr2);
=======
	u32 dr2 = SIMDR2_BITLEN1(bits) | SIMDR2_WDLEN1(words);

	if (tx_buf || (p->ctlr->flags & SPI_CONTROLLER_MUST_TX))
		sh_msiof_write(p, SITMDR2, dr2);
	else
		sh_msiof_write(p, SITMDR2, dr2 | SIMDR2_GRPMASK1);

	if (rx_buf)
		sh_msiof_write(p, SIRMDR2, dr2);
>>>>>>> upstream/android-13
}

static void sh_msiof_reset_str(struct sh_msiof_spi_priv *p)
{
<<<<<<< HEAD
	sh_msiof_write(p, STR,
		       sh_msiof_read(p, STR) & ~(STR_TDREQ | STR_RDREQ));
=======
	sh_msiof_write(p, SISTR,
		       sh_msiof_read(p, SISTR) & ~(SISTR_TDREQ | SISTR_RDREQ));
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_8(struct sh_msiof_spi_priv *p,
				      const void *tx_buf, int words, int fs)
{
	const u8 *buf_8 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, buf_8[k] << fs);
=======
		sh_msiof_write(p, SITFDR, buf_8[k] << fs);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_16(struct sh_msiof_spi_priv *p,
				       const void *tx_buf, int words, int fs)
{
	const u16 *buf_16 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, buf_16[k] << fs);
=======
		sh_msiof_write(p, SITFDR, buf_16[k] << fs);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_16u(struct sh_msiof_spi_priv *p,
					const void *tx_buf, int words, int fs)
{
	const u16 *buf_16 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, get_unaligned(&buf_16[k]) << fs);
=======
		sh_msiof_write(p, SITFDR, get_unaligned(&buf_16[k]) << fs);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_32(struct sh_msiof_spi_priv *p,
				       const void *tx_buf, int words, int fs)
{
	const u32 *buf_32 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, buf_32[k] << fs);
=======
		sh_msiof_write(p, SITFDR, buf_32[k] << fs);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_32u(struct sh_msiof_spi_priv *p,
					const void *tx_buf, int words, int fs)
{
	const u32 *buf_32 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, get_unaligned(&buf_32[k]) << fs);
=======
		sh_msiof_write(p, SITFDR, get_unaligned(&buf_32[k]) << fs);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_s32(struct sh_msiof_spi_priv *p,
					const void *tx_buf, int words, int fs)
{
	const u32 *buf_32 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, swab32(buf_32[k] << fs));
=======
		sh_msiof_write(p, SITFDR, swab32(buf_32[k] << fs));
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_write_fifo_s32u(struct sh_msiof_spi_priv *p,
					 const void *tx_buf, int words, int fs)
{
	const u32 *buf_32 = tx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		sh_msiof_write(p, TFDR, swab32(get_unaligned(&buf_32[k]) << fs));
=======
		sh_msiof_write(p, SITFDR, swab32(get_unaligned(&buf_32[k]) << fs));
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_8(struct sh_msiof_spi_priv *p,
				     void *rx_buf, int words, int fs)
{
	u8 *buf_8 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		buf_8[k] = sh_msiof_read(p, RFDR) >> fs;
=======
		buf_8[k] = sh_msiof_read(p, SIRFDR) >> fs;
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_16(struct sh_msiof_spi_priv *p,
				      void *rx_buf, int words, int fs)
{
	u16 *buf_16 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		buf_16[k] = sh_msiof_read(p, RFDR) >> fs;
=======
		buf_16[k] = sh_msiof_read(p, SIRFDR) >> fs;
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_16u(struct sh_msiof_spi_priv *p,
				       void *rx_buf, int words, int fs)
{
	u16 *buf_16 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		put_unaligned(sh_msiof_read(p, RFDR) >> fs, &buf_16[k]);
=======
		put_unaligned(sh_msiof_read(p, SIRFDR) >> fs, &buf_16[k]);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_32(struct sh_msiof_spi_priv *p,
				      void *rx_buf, int words, int fs)
{
	u32 *buf_32 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		buf_32[k] = sh_msiof_read(p, RFDR) >> fs;
=======
		buf_32[k] = sh_msiof_read(p, SIRFDR) >> fs;
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_32u(struct sh_msiof_spi_priv *p,
				       void *rx_buf, int words, int fs)
{
	u32 *buf_32 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		put_unaligned(sh_msiof_read(p, RFDR) >> fs, &buf_32[k]);
=======
		put_unaligned(sh_msiof_read(p, SIRFDR) >> fs, &buf_32[k]);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_s32(struct sh_msiof_spi_priv *p,
				       void *rx_buf, int words, int fs)
{
	u32 *buf_32 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		buf_32[k] = swab32(sh_msiof_read(p, RFDR) >> fs);
=======
		buf_32[k] = swab32(sh_msiof_read(p, SIRFDR) >> fs);
>>>>>>> upstream/android-13
}

static void sh_msiof_spi_read_fifo_s32u(struct sh_msiof_spi_priv *p,
				       void *rx_buf, int words, int fs)
{
	u32 *buf_32 = rx_buf;
	int k;

	for (k = 0; k < words; k++)
<<<<<<< HEAD
		put_unaligned(swab32(sh_msiof_read(p, RFDR) >> fs), &buf_32[k]);
=======
		put_unaligned(swab32(sh_msiof_read(p, SIRFDR) >> fs), &buf_32[k]);
>>>>>>> upstream/android-13
}

static int sh_msiof_spi_setup(struct spi_device *spi)
{
<<<<<<< HEAD
	struct device_node	*np = spi->master->dev.of_node;
	struct sh_msiof_spi_priv *p = spi_master_get_devdata(spi->master);
	u32 clr, set, tmp;

	if (!np) {
		/*
		 * Use spi->controller_data for CS (same strategy as spi_gpio),
		 * if any. otherwise let HW control CS
		 */
		spi->cs_gpio = (uintptr_t)spi->controller_data;
	}

	if (gpio_is_valid(spi->cs_gpio)) {
		gpio_direction_output(spi->cs_gpio, !(spi->mode & SPI_CS_HIGH));
		return 0;
	}

	if (spi_controller_is_slave(p->master))
=======
	struct sh_msiof_spi_priv *p =
		spi_controller_get_devdata(spi->controller);
	u32 clr, set, tmp;

	if (spi->cs_gpiod || spi_controller_is_slave(p->ctlr))
>>>>>>> upstream/android-13
		return 0;

	if (p->native_cs_inited &&
	    (p->native_cs_high == !!(spi->mode & SPI_CS_HIGH)))
		return 0;

	/* Configure native chip select mode/polarity early */
<<<<<<< HEAD
	clr = MDR1_SYNCMD_MASK;
	set = MDR1_SYNCMD_SPI;
	if (spi->mode & SPI_CS_HIGH)
		clr |= BIT(MDR1_SYNCAC_SHIFT);
	else
		set |= BIT(MDR1_SYNCAC_SHIFT);
	pm_runtime_get_sync(&p->pdev->dev);
	tmp = sh_msiof_read(p, TMDR1) & ~clr;
	sh_msiof_write(p, TMDR1, tmp | set | MDR1_TRMD | TMDR1_PCON);
	tmp = sh_msiof_read(p, RMDR1) & ~clr;
	sh_msiof_write(p, RMDR1, tmp | set);
=======
	clr = SIMDR1_SYNCMD_MASK;
	set = SIMDR1_SYNCMD_SPI;
	if (spi->mode & SPI_CS_HIGH)
		clr |= BIT(SIMDR1_SYNCAC_SHIFT);
	else
		set |= BIT(SIMDR1_SYNCAC_SHIFT);
	pm_runtime_get_sync(&p->pdev->dev);
	tmp = sh_msiof_read(p, SITMDR1) & ~clr;
	sh_msiof_write(p, SITMDR1, tmp | set | SIMDR1_TRMD | SITMDR1_PCON);
	tmp = sh_msiof_read(p, SIRMDR1) & ~clr;
	sh_msiof_write(p, SIRMDR1, tmp | set);
>>>>>>> upstream/android-13
	pm_runtime_put(&p->pdev->dev);
	p->native_cs_high = spi->mode & SPI_CS_HIGH;
	p->native_cs_inited = true;
	return 0;
}

<<<<<<< HEAD
static int sh_msiof_prepare_message(struct spi_master *master,
				    struct spi_message *msg)
{
	struct sh_msiof_spi_priv *p = spi_master_get_devdata(master);
=======
static int sh_msiof_prepare_message(struct spi_controller *ctlr,
				    struct spi_message *msg)
{
	struct sh_msiof_spi_priv *p = spi_controller_get_devdata(ctlr);
>>>>>>> upstream/android-13
	const struct spi_device *spi = msg->spi;
	u32 ss, cs_high;

	/* Configure pins before asserting CS */
<<<<<<< HEAD
	if (gpio_is_valid(spi->cs_gpio)) {
		ss = p->unused_ss;
=======
	if (spi->cs_gpiod) {
		ss = ctlr->unused_native_cs;
>>>>>>> upstream/android-13
		cs_high = p->native_cs_high;
	} else {
		ss = spi->chip_select;
		cs_high = !!(spi->mode & SPI_CS_HIGH);
	}
	sh_msiof_spi_set_pin_regs(p, ss, !!(spi->mode & SPI_CPOL),
				  !!(spi->mode & SPI_CPHA),
				  !!(spi->mode & SPI_3WIRE),
				  !!(spi->mode & SPI_LSB_FIRST), cs_high);
	return 0;
}

static int sh_msiof_spi_start(struct sh_msiof_spi_priv *p, void *rx_buf)
{
<<<<<<< HEAD
	bool slave = spi_controller_is_slave(p->master);
=======
	bool slave = spi_controller_is_slave(p->ctlr);
>>>>>>> upstream/android-13
	int ret = 0;

	/* setup clock and rx/tx signals */
	if (!slave)
<<<<<<< HEAD
		ret = sh_msiof_modify_ctr_wait(p, 0, CTR_TSCKE);
	if (rx_buf && !ret)
		ret = sh_msiof_modify_ctr_wait(p, 0, CTR_RXE);
	if (!ret)
		ret = sh_msiof_modify_ctr_wait(p, 0, CTR_TXE);

	/* start by setting frame bit */
	if (!ret && !slave)
		ret = sh_msiof_modify_ctr_wait(p, 0, CTR_TFSE);
=======
		ret = sh_msiof_modify_ctr_wait(p, 0, SICTR_TSCKE);
	if (rx_buf && !ret)
		ret = sh_msiof_modify_ctr_wait(p, 0, SICTR_RXE);
	if (!ret)
		ret = sh_msiof_modify_ctr_wait(p, 0, SICTR_TXE);

	/* start by setting frame bit */
	if (!ret && !slave)
		ret = sh_msiof_modify_ctr_wait(p, 0, SICTR_TFSE);
>>>>>>> upstream/android-13

	return ret;
}

static int sh_msiof_spi_stop(struct sh_msiof_spi_priv *p, void *rx_buf)
{
<<<<<<< HEAD
	bool slave = spi_controller_is_slave(p->master);
=======
	bool slave = spi_controller_is_slave(p->ctlr);
>>>>>>> upstream/android-13
	int ret = 0;

	/* shut down frame, rx/tx and clock signals */
	if (!slave)
<<<<<<< HEAD
		ret = sh_msiof_modify_ctr_wait(p, CTR_TFSE, 0);
	if (!ret)
		ret = sh_msiof_modify_ctr_wait(p, CTR_TXE, 0);
	if (rx_buf && !ret)
		ret = sh_msiof_modify_ctr_wait(p, CTR_RXE, 0);
	if (!ret && !slave)
		ret = sh_msiof_modify_ctr_wait(p, CTR_TSCKE, 0);
=======
		ret = sh_msiof_modify_ctr_wait(p, SICTR_TFSE, 0);
	if (!ret)
		ret = sh_msiof_modify_ctr_wait(p, SICTR_TXE, 0);
	if (rx_buf && !ret)
		ret = sh_msiof_modify_ctr_wait(p, SICTR_RXE, 0);
	if (!ret && !slave)
		ret = sh_msiof_modify_ctr_wait(p, SICTR_TSCKE, 0);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int sh_msiof_slave_abort(struct spi_master *master)
{
	struct sh_msiof_spi_priv *p = spi_master_get_devdata(master);
=======
static int sh_msiof_slave_abort(struct spi_controller *ctlr)
{
	struct sh_msiof_spi_priv *p = spi_controller_get_devdata(ctlr);
>>>>>>> upstream/android-13

	p->slave_aborted = true;
	complete(&p->done);
	complete(&p->done_txdma);
	return 0;
}

static int sh_msiof_wait_for_completion(struct sh_msiof_spi_priv *p,
					struct completion *x)
{
<<<<<<< HEAD
	if (spi_controller_is_slave(p->master)) {
=======
	if (spi_controller_is_slave(p->ctlr)) {
>>>>>>> upstream/android-13
		if (wait_for_completion_interruptible(x) ||
		    p->slave_aborted) {
			dev_dbg(&p->pdev->dev, "interrupted\n");
			return -EINTR;
		}
	} else {
		if (!wait_for_completion_timeout(x, HZ)) {
			dev_err(&p->pdev->dev, "timeout\n");
			return -ETIMEDOUT;
		}
	}

	return 0;
}

static int sh_msiof_spi_txrx_once(struct sh_msiof_spi_priv *p,
				  void (*tx_fifo)(struct sh_msiof_spi_priv *,
						  const void *, int, int),
				  void (*rx_fifo)(struct sh_msiof_spi_priv *,
						  void *, int, int),
				  const void *tx_buf, void *rx_buf,
				  int words, int bits)
{
	int fifo_shift;
	int ret;

	/* limit maximum word transfer to rx/tx fifo size */
	if (tx_buf)
		words = min_t(int, words, p->tx_fifo_size);
	if (rx_buf)
		words = min_t(int, words, p->rx_fifo_size);

	/* the fifo contents need shifting */
	fifo_shift = 32 - bits;

	/* default FIFO watermarks for PIO */
<<<<<<< HEAD
	sh_msiof_write(p, FCTR, 0);

	/* setup msiof transfer mode registers */
	sh_msiof_spi_set_mode_regs(p, tx_buf, rx_buf, bits, words);
	sh_msiof_write(p, IER, IER_TEOFE | IER_REOFE);
=======
	sh_msiof_write(p, SIFCTR, 0);

	/* setup msiof transfer mode registers */
	sh_msiof_spi_set_mode_regs(p, tx_buf, rx_buf, bits, words);
	sh_msiof_write(p, SIIER, SIIER_TEOFE | SIIER_REOFE);
>>>>>>> upstream/android-13

	/* write tx fifo */
	if (tx_buf)
		tx_fifo(p, tx_buf, words, fifo_shift);

	reinit_completion(&p->done);
	p->slave_aborted = false;

	ret = sh_msiof_spi_start(p, rx_buf);
	if (ret) {
		dev_err(&p->pdev->dev, "failed to start hardware\n");
		goto stop_ier;
	}

	/* wait for tx fifo to be emptied / rx fifo to be filled */
	ret = sh_msiof_wait_for_completion(p, &p->done);
	if (ret)
		goto stop_reset;

	/* read rx fifo */
	if (rx_buf)
		rx_fifo(p, rx_buf, words, fifo_shift);

	/* clear status bits */
	sh_msiof_reset_str(p);

	ret = sh_msiof_spi_stop(p, rx_buf);
	if (ret) {
		dev_err(&p->pdev->dev, "failed to shut down hardware\n");
		return ret;
	}

	return words;

stop_reset:
	sh_msiof_reset_str(p);
	sh_msiof_spi_stop(p, rx_buf);
stop_ier:
<<<<<<< HEAD
	sh_msiof_write(p, IER, 0);
=======
	sh_msiof_write(p, SIIER, 0);
>>>>>>> upstream/android-13
	return ret;
}

static void sh_msiof_dma_complete(void *arg)
{
	complete(arg);
}

static int sh_msiof_dma_once(struct sh_msiof_spi_priv *p, const void *tx,
			     void *rx, unsigned int len)
{
	u32 ier_bits = 0;
	struct dma_async_tx_descriptor *desc_tx = NULL, *desc_rx = NULL;
	dma_cookie_t cookie;
	int ret;

	/* First prepare and submit the DMA request(s), as this may fail */
	if (rx) {
<<<<<<< HEAD
		ier_bits |= IER_RDREQE | IER_RDMAE;
		desc_rx = dmaengine_prep_slave_single(p->master->dma_rx,
=======
		ier_bits |= SIIER_RDREQE | SIIER_RDMAE;
		desc_rx = dmaengine_prep_slave_single(p->ctlr->dma_rx,
>>>>>>> upstream/android-13
					p->rx_dma_addr, len, DMA_DEV_TO_MEM,
					DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
		if (!desc_rx)
			return -EAGAIN;

		desc_rx->callback = sh_msiof_dma_complete;
		desc_rx->callback_param = &p->done;
		cookie = dmaengine_submit(desc_rx);
		if (dma_submit_error(cookie))
			return cookie;
	}

	if (tx) {
<<<<<<< HEAD
		ier_bits |= IER_TDREQE | IER_TDMAE;
		dma_sync_single_for_device(p->master->dma_tx->device->dev,
					   p->tx_dma_addr, len, DMA_TO_DEVICE);
		desc_tx = dmaengine_prep_slave_single(p->master->dma_tx,
=======
		ier_bits |= SIIER_TDREQE | SIIER_TDMAE;
		dma_sync_single_for_device(p->ctlr->dma_tx->device->dev,
					   p->tx_dma_addr, len, DMA_TO_DEVICE);
		desc_tx = dmaengine_prep_slave_single(p->ctlr->dma_tx,
>>>>>>> upstream/android-13
					p->tx_dma_addr, len, DMA_MEM_TO_DEV,
					DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
		if (!desc_tx) {
			ret = -EAGAIN;
			goto no_dma_tx;
		}

		desc_tx->callback = sh_msiof_dma_complete;
		desc_tx->callback_param = &p->done_txdma;
		cookie = dmaengine_submit(desc_tx);
		if (dma_submit_error(cookie)) {
			ret = cookie;
			goto no_dma_tx;
		}
	}

	/* 1 stage FIFO watermarks for DMA */
<<<<<<< HEAD
	sh_msiof_write(p, FCTR, FCTR_TFWM_1 | FCTR_RFWM_1);
=======
	sh_msiof_write(p, SIFCTR, SIFCTR_TFWM_1 | SIFCTR_RFWM_1);
>>>>>>> upstream/android-13

	/* setup msiof transfer mode registers (32-bit words) */
	sh_msiof_spi_set_mode_regs(p, tx, rx, 32, len / 4);

<<<<<<< HEAD
	sh_msiof_write(p, IER, ier_bits);
=======
	sh_msiof_write(p, SIIER, ier_bits);
>>>>>>> upstream/android-13

	reinit_completion(&p->done);
	if (tx)
		reinit_completion(&p->done_txdma);
	p->slave_aborted = false;

	/* Now start DMA */
	if (rx)
<<<<<<< HEAD
		dma_async_issue_pending(p->master->dma_rx);
	if (tx)
		dma_async_issue_pending(p->master->dma_tx);
=======
		dma_async_issue_pending(p->ctlr->dma_rx);
	if (tx)
		dma_async_issue_pending(p->ctlr->dma_tx);
>>>>>>> upstream/android-13

	ret = sh_msiof_spi_start(p, rx);
	if (ret) {
		dev_err(&p->pdev->dev, "failed to start hardware\n");
		goto stop_dma;
	}

	if (tx) {
		/* wait for tx DMA completion */
		ret = sh_msiof_wait_for_completion(p, &p->done_txdma);
		if (ret)
			goto stop_reset;
	}

	if (rx) {
		/* wait for rx DMA completion */
		ret = sh_msiof_wait_for_completion(p, &p->done);
		if (ret)
			goto stop_reset;

<<<<<<< HEAD
		sh_msiof_write(p, IER, 0);
	} else {
		/* wait for tx fifo to be emptied */
		sh_msiof_write(p, IER, IER_TEOFE);
=======
		sh_msiof_write(p, SIIER, 0);
	} else {
		/* wait for tx fifo to be emptied */
		sh_msiof_write(p, SIIER, SIIER_TEOFE);
>>>>>>> upstream/android-13
		ret = sh_msiof_wait_for_completion(p, &p->done);
		if (ret)
			goto stop_reset;
	}

	/* clear status bits */
	sh_msiof_reset_str(p);

	ret = sh_msiof_spi_stop(p, rx);
	if (ret) {
		dev_err(&p->pdev->dev, "failed to shut down hardware\n");
		return ret;
	}

	if (rx)
<<<<<<< HEAD
		dma_sync_single_for_cpu(p->master->dma_rx->device->dev,
					p->rx_dma_addr, len,
					DMA_FROM_DEVICE);
=======
		dma_sync_single_for_cpu(p->ctlr->dma_rx->device->dev,
					p->rx_dma_addr, len, DMA_FROM_DEVICE);
>>>>>>> upstream/android-13

	return 0;

stop_reset:
	sh_msiof_reset_str(p);
	sh_msiof_spi_stop(p, rx);
stop_dma:
	if (tx)
<<<<<<< HEAD
		dmaengine_terminate_all(p->master->dma_tx);
no_dma_tx:
	if (rx)
		dmaengine_terminate_all(p->master->dma_rx);
	sh_msiof_write(p, IER, 0);
=======
		dmaengine_terminate_sync(p->ctlr->dma_tx);
no_dma_tx:
	if (rx)
		dmaengine_terminate_sync(p->ctlr->dma_rx);
	sh_msiof_write(p, SIIER, 0);
>>>>>>> upstream/android-13
	return ret;
}

static void copy_bswap32(u32 *dst, const u32 *src, unsigned int words)
{
	/* src or dst can be unaligned, but not both */
	if ((unsigned long)src & 3) {
		while (words--) {
			*dst++ = swab32(get_unaligned(src));
			src++;
		}
	} else if ((unsigned long)dst & 3) {
		while (words--) {
			put_unaligned(swab32(*src++), dst);
			dst++;
		}
	} else {
		while (words--)
			*dst++ = swab32(*src++);
	}
}

static void copy_wswap32(u32 *dst, const u32 *src, unsigned int words)
{
	/* src or dst can be unaligned, but not both */
	if ((unsigned long)src & 3) {
		while (words--) {
			*dst++ = swahw32(get_unaligned(src));
			src++;
		}
	} else if ((unsigned long)dst & 3) {
		while (words--) {
			put_unaligned(swahw32(*src++), dst);
			dst++;
		}
	} else {
		while (words--)
			*dst++ = swahw32(*src++);
	}
}

static void copy_plain32(u32 *dst, const u32 *src, unsigned int words)
{
	memcpy(dst, src, words * 4);
}

<<<<<<< HEAD
static int sh_msiof_transfer_one(struct spi_master *master,
				 struct spi_device *spi,
				 struct spi_transfer *t)
{
	struct sh_msiof_spi_priv *p = spi_master_get_devdata(master);
=======
static int sh_msiof_transfer_one(struct spi_controller *ctlr,
				 struct spi_device *spi,
				 struct spi_transfer *t)
{
	struct sh_msiof_spi_priv *p = spi_controller_get_devdata(ctlr);
>>>>>>> upstream/android-13
	void (*copy32)(u32 *, const u32 *, unsigned int);
	void (*tx_fifo)(struct sh_msiof_spi_priv *, const void *, int, int);
	void (*rx_fifo)(struct sh_msiof_spi_priv *, void *, int, int);
	const void *tx_buf = t->tx_buf;
	void *rx_buf = t->rx_buf;
	unsigned int len = t->len;
	unsigned int bits = t->bits_per_word;
	unsigned int bytes_per_word;
	unsigned int words;
	int n;
	bool swab;
	int ret;

<<<<<<< HEAD
	/* setup clocks (clock already enabled in chipselect()) */
	if (!spi_controller_is_slave(p->master))
		sh_msiof_spi_set_clk_regs(p, clk_get_rate(p->clk), t->speed_hz);

	while (master->dma_tx && len > 15) {
=======
	/* reset registers */
	sh_msiof_spi_reset_regs(p);

	/* setup clocks (clock already enabled in chipselect()) */
	if (!spi_controller_is_slave(p->ctlr))
		sh_msiof_spi_set_clk_regs(p, t);

	while (ctlr->dma_tx && len > 15) {
>>>>>>> upstream/android-13
		/*
		 *  DMA supports 32-bit words only, hence pack 8-bit and 16-bit
		 *  words, with byte resp. word swapping.
		 */
		unsigned int l = 0;

		if (tx_buf)
<<<<<<< HEAD
			l = min(len, p->tx_fifo_size * 4);
		if (rx_buf)
			l = min(len, p->rx_fifo_size * 4);

		if (bits <= 8) {
			if (l & 3)
				break;
			copy32 = copy_bswap32;
		} else if (bits <= 16) {
			if (l & 3)
				break;
=======
			l = min(round_down(len, 4), p->tx_fifo_size * 4);
		if (rx_buf)
			l = min(round_down(len, 4), p->rx_fifo_size * 4);

		if (bits <= 8) {
			copy32 = copy_bswap32;
		} else if (bits <= 16) {
>>>>>>> upstream/android-13
			copy32 = copy_wswap32;
		} else {
			copy32 = copy_plain32;
		}

		if (tx_buf)
			copy32(p->tx_dma_page, tx_buf, l / 4);

		ret = sh_msiof_dma_once(p, tx_buf, rx_buf, l);
		if (ret == -EAGAIN) {
			dev_warn_once(&p->pdev->dev,
				"DMA not available, falling back to PIO\n");
			break;
		}
		if (ret)
			return ret;

		if (rx_buf) {
			copy32(rx_buf, p->rx_dma_page, l / 4);
			rx_buf += l;
		}
		if (tx_buf)
			tx_buf += l;

		len -= l;
		if (!len)
			return 0;
	}

<<<<<<< HEAD
	if (bits <= 8 && len > 15 && !(len & 3)) {
=======
	if (bits <= 8 && len > 15) {
>>>>>>> upstream/android-13
		bits = 32;
		swab = true;
	} else {
		swab = false;
	}

	/* setup bytes per word and fifo read/write functions */
	if (bits <= 8) {
		bytes_per_word = 1;
		tx_fifo = sh_msiof_spi_write_fifo_8;
		rx_fifo = sh_msiof_spi_read_fifo_8;
	} else if (bits <= 16) {
		bytes_per_word = 2;
		if ((unsigned long)tx_buf & 0x01)
			tx_fifo = sh_msiof_spi_write_fifo_16u;
		else
			tx_fifo = sh_msiof_spi_write_fifo_16;

		if ((unsigned long)rx_buf & 0x01)
			rx_fifo = sh_msiof_spi_read_fifo_16u;
		else
			rx_fifo = sh_msiof_spi_read_fifo_16;
	} else if (swab) {
		bytes_per_word = 4;
		if ((unsigned long)tx_buf & 0x03)
			tx_fifo = sh_msiof_spi_write_fifo_s32u;
		else
			tx_fifo = sh_msiof_spi_write_fifo_s32;

		if ((unsigned long)rx_buf & 0x03)
			rx_fifo = sh_msiof_spi_read_fifo_s32u;
		else
			rx_fifo = sh_msiof_spi_read_fifo_s32;
	} else {
		bytes_per_word = 4;
		if ((unsigned long)tx_buf & 0x03)
			tx_fifo = sh_msiof_spi_write_fifo_32u;
		else
			tx_fifo = sh_msiof_spi_write_fifo_32;

		if ((unsigned long)rx_buf & 0x03)
			rx_fifo = sh_msiof_spi_read_fifo_32u;
		else
			rx_fifo = sh_msiof_spi_read_fifo_32;
	}

	/* transfer in fifo sized chunks */
	words = len / bytes_per_word;

	while (words > 0) {
		n = sh_msiof_spi_txrx_once(p, tx_fifo, rx_fifo, tx_buf, rx_buf,
					   words, bits);
		if (n < 0)
			return n;

		if (tx_buf)
			tx_buf += n * bytes_per_word;
		if (rx_buf)
			rx_buf += n * bytes_per_word;
		words -= n;
<<<<<<< HEAD
=======

		if (words == 0 && (len % bytes_per_word)) {
			words = len % bytes_per_word;
			bits = t->bits_per_word;
			bytes_per_word = 1;
			tx_fifo = sh_msiof_spi_write_fifo_8;
			rx_fifo = sh_msiof_spi_read_fifo_8;
		}
>>>>>>> upstream/android-13
	}

	return 0;
}

static const struct sh_msiof_chipdata sh_data = {
<<<<<<< HEAD
	.tx_fifo_size = 64,
	.rx_fifo_size = 64,
	.master_flags = 0,
=======
	.bits_per_word_mask = SPI_BPW_RANGE_MASK(8, 32),
	.tx_fifo_size = 64,
	.rx_fifo_size = 64,
	.ctlr_flags = 0,
>>>>>>> upstream/android-13
	.min_div_pow = 0,
};

static const struct sh_msiof_chipdata rcar_gen2_data = {
<<<<<<< HEAD
	.tx_fifo_size = 64,
	.rx_fifo_size = 64,
	.master_flags = SPI_MASTER_MUST_TX,
=======
	.bits_per_word_mask = SPI_BPW_MASK(8) | SPI_BPW_MASK(16) |
			      SPI_BPW_MASK(24) | SPI_BPW_MASK(32),
	.tx_fifo_size = 64,
	.rx_fifo_size = 64,
	.ctlr_flags = SPI_CONTROLLER_MUST_TX,
>>>>>>> upstream/android-13
	.min_div_pow = 0,
};

static const struct sh_msiof_chipdata rcar_gen3_data = {
<<<<<<< HEAD
	.tx_fifo_size = 64,
	.rx_fifo_size = 64,
	.master_flags = SPI_MASTER_MUST_TX,
=======
	.bits_per_word_mask = SPI_BPW_MASK(8) | SPI_BPW_MASK(16) |
			      SPI_BPW_MASK(24) | SPI_BPW_MASK(32),
	.tx_fifo_size = 64,
	.rx_fifo_size = 64,
	.ctlr_flags = SPI_CONTROLLER_MUST_TX,
>>>>>>> upstream/android-13
	.min_div_pow = 1,
};

static const struct of_device_id sh_msiof_match[] = {
	{ .compatible = "renesas,sh-mobile-msiof", .data = &sh_data },
	{ .compatible = "renesas,msiof-r8a7743",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7745",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7790",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7791",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7792",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7793",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7794",   .data = &rcar_gen2_data },
	{ .compatible = "renesas,rcar-gen2-msiof", .data = &rcar_gen2_data },
	{ .compatible = "renesas,msiof-r8a7796",   .data = &rcar_gen3_data },
	{ .compatible = "renesas,rcar-gen3-msiof", .data = &rcar_gen3_data },
	{ .compatible = "renesas,sh-msiof",        .data = &sh_data }, /* Deprecated */
	{},
};
MODULE_DEVICE_TABLE(of, sh_msiof_match);

#ifdef CONFIG_OF
static struct sh_msiof_spi_info *sh_msiof_spi_parse_dt(struct device *dev)
{
	struct sh_msiof_spi_info *info;
	struct device_node *np = dev->of_node;
	u32 num_cs = 1;

	info = devm_kzalloc(dev, sizeof(struct sh_msiof_spi_info), GFP_KERNEL);
	if (!info)
		return NULL;

	info->mode = of_property_read_bool(np, "spi-slave") ? MSIOF_SPI_SLAVE
							    : MSIOF_SPI_MASTER;

	/* Parse the MSIOF properties */
	if (info->mode == MSIOF_SPI_MASTER)
		of_property_read_u32(np, "num-cs", &num_cs);
	of_property_read_u32(np, "renesas,tx-fifo-size",
					&info->tx_fifo_override);
	of_property_read_u32(np, "renesas,rx-fifo-size",
					&info->rx_fifo_override);
	of_property_read_u32(np, "renesas,dtdl", &info->dtdl);
	of_property_read_u32(np, "renesas,syncdl", &info->syncdl);

	info->num_chipselect = num_cs;

	return info;
}
#else
static struct sh_msiof_spi_info *sh_msiof_spi_parse_dt(struct device *dev)
{
	return NULL;
}
#endif

<<<<<<< HEAD
static int sh_msiof_get_cs_gpios(struct sh_msiof_spi_priv *p)
{
	struct device *dev = &p->pdev->dev;
	unsigned int used_ss_mask = 0;
	unsigned int cs_gpios = 0;
	unsigned int num_cs, i;
	int ret;

	ret = gpiod_count(dev, "cs");
	if (ret <= 0)
		return 0;

	num_cs = max_t(unsigned int, ret, p->master->num_chipselect);
	for (i = 0; i < num_cs; i++) {
		struct gpio_desc *gpiod;

		gpiod = devm_gpiod_get_index(dev, "cs", i, GPIOD_ASIS);
		if (!IS_ERR(gpiod)) {
			cs_gpios++;
			continue;
		}

		if (PTR_ERR(gpiod) != -ENOENT)
			return PTR_ERR(gpiod);

		if (i >= MAX_SS) {
			dev_err(dev, "Invalid native chip select %d\n", i);
			return -EINVAL;
		}
		used_ss_mask |= BIT(i);
	}
	p->unused_ss = ffz(used_ss_mask);
	if (cs_gpios && p->unused_ss >= MAX_SS) {
		dev_err(dev, "No unused native chip select available\n");
		return -EINVAL;
	}
	return 0;
}

=======
>>>>>>> upstream/android-13
static struct dma_chan *sh_msiof_request_dma_chan(struct device *dev,
	enum dma_transfer_direction dir, unsigned int id, dma_addr_t port_addr)
{
	dma_cap_mask_t mask;
	struct dma_chan *chan;
	struct dma_slave_config cfg;
	int ret;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	chan = dma_request_slave_channel_compat(mask, shdma_chan_filter,
				(void *)(unsigned long)id, dev,
				dir == DMA_MEM_TO_DEV ? "tx" : "rx");
	if (!chan) {
		dev_warn(dev, "dma_request_slave_channel_compat failed\n");
		return NULL;
	}

	memset(&cfg, 0, sizeof(cfg));
	cfg.direction = dir;
	if (dir == DMA_MEM_TO_DEV) {
		cfg.dst_addr = port_addr;
		cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	} else {
		cfg.src_addr = port_addr;
		cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	}

	ret = dmaengine_slave_config(chan, &cfg);
	if (ret) {
		dev_warn(dev, "dmaengine_slave_config failed %d\n", ret);
		dma_release_channel(chan);
		return NULL;
	}

	return chan;
}

static int sh_msiof_request_dma(struct sh_msiof_spi_priv *p)
{
	struct platform_device *pdev = p->pdev;
	struct device *dev = &pdev->dev;
<<<<<<< HEAD
	const struct sh_msiof_spi_info *info = dev_get_platdata(dev);
	unsigned int dma_tx_id, dma_rx_id;
	const struct resource *res;
	struct spi_master *master;
=======
	const struct sh_msiof_spi_info *info = p->info;
	unsigned int dma_tx_id, dma_rx_id;
	const struct resource *res;
	struct spi_controller *ctlr;
>>>>>>> upstream/android-13
	struct device *tx_dev, *rx_dev;

	if (dev->of_node) {
		/* In the OF case we will get the slave IDs from the DT */
		dma_tx_id = 0;
		dma_rx_id = 0;
	} else if (info && info->dma_tx_id && info->dma_rx_id) {
		dma_tx_id = info->dma_tx_id;
		dma_rx_id = info->dma_rx_id;
	} else {
		/* The driver assumes no error */
		return 0;
	}

	/* The DMA engine uses the second register set, if present */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!res)
		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

<<<<<<< HEAD
	master = p->master;
	master->dma_tx = sh_msiof_request_dma_chan(dev, DMA_MEM_TO_DEV,
						   dma_tx_id,
						   res->start + TFDR);
	if (!master->dma_tx)
		return -ENODEV;

	master->dma_rx = sh_msiof_request_dma_chan(dev, DMA_DEV_TO_MEM,
						   dma_rx_id,
						   res->start + RFDR);
	if (!master->dma_rx)
=======
	ctlr = p->ctlr;
	ctlr->dma_tx = sh_msiof_request_dma_chan(dev, DMA_MEM_TO_DEV,
						 dma_tx_id, res->start + SITFDR);
	if (!ctlr->dma_tx)
		return -ENODEV;

	ctlr->dma_rx = sh_msiof_request_dma_chan(dev, DMA_DEV_TO_MEM,
						 dma_rx_id, res->start + SIRFDR);
	if (!ctlr->dma_rx)
>>>>>>> upstream/android-13
		goto free_tx_chan;

	p->tx_dma_page = (void *)__get_free_page(GFP_KERNEL | GFP_DMA);
	if (!p->tx_dma_page)
		goto free_rx_chan;

	p->rx_dma_page = (void *)__get_free_page(GFP_KERNEL | GFP_DMA);
	if (!p->rx_dma_page)
		goto free_tx_page;

<<<<<<< HEAD
	tx_dev = master->dma_tx->device->dev;
=======
	tx_dev = ctlr->dma_tx->device->dev;
>>>>>>> upstream/android-13
	p->tx_dma_addr = dma_map_single(tx_dev, p->tx_dma_page, PAGE_SIZE,
					DMA_TO_DEVICE);
	if (dma_mapping_error(tx_dev, p->tx_dma_addr))
		goto free_rx_page;

<<<<<<< HEAD
	rx_dev = master->dma_rx->device->dev;
=======
	rx_dev = ctlr->dma_rx->device->dev;
>>>>>>> upstream/android-13
	p->rx_dma_addr = dma_map_single(rx_dev, p->rx_dma_page, PAGE_SIZE,
					DMA_FROM_DEVICE);
	if (dma_mapping_error(rx_dev, p->rx_dma_addr))
		goto unmap_tx_page;

	dev_info(dev, "DMA available");
	return 0;

unmap_tx_page:
	dma_unmap_single(tx_dev, p->tx_dma_addr, PAGE_SIZE, DMA_TO_DEVICE);
free_rx_page:
	free_page((unsigned long)p->rx_dma_page);
free_tx_page:
	free_page((unsigned long)p->tx_dma_page);
free_rx_chan:
<<<<<<< HEAD
	dma_release_channel(master->dma_rx);
free_tx_chan:
	dma_release_channel(master->dma_tx);
	master->dma_tx = NULL;
=======
	dma_release_channel(ctlr->dma_rx);
free_tx_chan:
	dma_release_channel(ctlr->dma_tx);
	ctlr->dma_tx = NULL;
>>>>>>> upstream/android-13
	return -ENODEV;
}

static void sh_msiof_release_dma(struct sh_msiof_spi_priv *p)
{
<<<<<<< HEAD
	struct spi_master *master = p->master;

	if (!master->dma_tx)
		return;

	dma_unmap_single(master->dma_rx->device->dev, p->rx_dma_addr,
			 PAGE_SIZE, DMA_FROM_DEVICE);
	dma_unmap_single(master->dma_tx->device->dev, p->tx_dma_addr,
			 PAGE_SIZE, DMA_TO_DEVICE);
	free_page((unsigned long)p->rx_dma_page);
	free_page((unsigned long)p->tx_dma_page);
	dma_release_channel(master->dma_rx);
	dma_release_channel(master->dma_tx);
=======
	struct spi_controller *ctlr = p->ctlr;

	if (!ctlr->dma_tx)
		return;

	dma_unmap_single(ctlr->dma_rx->device->dev, p->rx_dma_addr, PAGE_SIZE,
			 DMA_FROM_DEVICE);
	dma_unmap_single(ctlr->dma_tx->device->dev, p->tx_dma_addr, PAGE_SIZE,
			 DMA_TO_DEVICE);
	free_page((unsigned long)p->rx_dma_page);
	free_page((unsigned long)p->tx_dma_page);
	dma_release_channel(ctlr->dma_rx);
	dma_release_channel(ctlr->dma_tx);
>>>>>>> upstream/android-13
}

static int sh_msiof_spi_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct resource	*r;
	struct spi_master *master;
	const struct sh_msiof_chipdata *chipdata;
	struct sh_msiof_spi_info *info;
	struct sh_msiof_spi_priv *p;
=======
	struct spi_controller *ctlr;
	const struct sh_msiof_chipdata *chipdata;
	struct sh_msiof_spi_info *info;
	struct sh_msiof_spi_priv *p;
	unsigned long clksrc;
>>>>>>> upstream/android-13
	int i;
	int ret;

	chipdata = of_device_get_match_data(&pdev->dev);
	if (chipdata) {
		info = sh_msiof_spi_parse_dt(&pdev->dev);
	} else {
		chipdata = (const void *)pdev->id_entry->driver_data;
		info = dev_get_platdata(&pdev->dev);
	}

	if (!info) {
		dev_err(&pdev->dev, "failed to obtain device info\n");
		return -ENXIO;
	}

	if (info->mode == MSIOF_SPI_SLAVE)
<<<<<<< HEAD
		master = spi_alloc_slave(&pdev->dev,
					 sizeof(struct sh_msiof_spi_priv));
	else
		master = spi_alloc_master(&pdev->dev,
					  sizeof(struct sh_msiof_spi_priv));
	if (master == NULL)
		return -ENOMEM;

	p = spi_master_get_devdata(master);

	platform_set_drvdata(pdev, p);
	p->master = master;
=======
		ctlr = spi_alloc_slave(&pdev->dev,
				       sizeof(struct sh_msiof_spi_priv));
	else
		ctlr = spi_alloc_master(&pdev->dev,
					sizeof(struct sh_msiof_spi_priv));
	if (ctlr == NULL)
		return -ENOMEM;

	p = spi_controller_get_devdata(ctlr);

	platform_set_drvdata(pdev, p);
	p->ctlr = ctlr;
>>>>>>> upstream/android-13
	p->info = info;
	p->min_div_pow = chipdata->min_div_pow;

	init_completion(&p->done);
	init_completion(&p->done_txdma);

	p->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(p->clk)) {
		dev_err(&pdev->dev, "cannot get clock\n");
		ret = PTR_ERR(p->clk);
		goto err1;
	}

	i = platform_get_irq(pdev, 0);
	if (i < 0) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "cannot get IRQ\n");
=======
>>>>>>> upstream/android-13
		ret = i;
		goto err1;
	}

<<<<<<< HEAD
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	p->mapbase = devm_ioremap_resource(&pdev->dev, r);
=======
	p->mapbase = devm_platform_ioremap_resource(pdev, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(p->mapbase)) {
		ret = PTR_ERR(p->mapbase);
		goto err1;
	}

	ret = devm_request_irq(&pdev->dev, i, sh_msiof_spi_irq, 0,
			       dev_name(&pdev->dev), p);
	if (ret) {
		dev_err(&pdev->dev, "unable to request irq\n");
		goto err1;
	}

	p->pdev = pdev;
	pm_runtime_enable(&pdev->dev);

	/* Platform data may override FIFO sizes */
	p->tx_fifo_size = chipdata->tx_fifo_size;
	p->rx_fifo_size = chipdata->rx_fifo_size;
	if (p->info->tx_fifo_override)
		p->tx_fifo_size = p->info->tx_fifo_override;
	if (p->info->rx_fifo_override)
		p->rx_fifo_size = p->info->rx_fifo_override;

<<<<<<< HEAD
	/* Setup GPIO chip selects */
	master->num_chipselect = p->info->num_chipselect;
	ret = sh_msiof_get_cs_gpios(p);
	if (ret)
		goto err1;

	/* init master code */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;
	master->mode_bits |= SPI_LSB_FIRST | SPI_3WIRE;
	master->flags = chipdata->master_flags;
	master->bus_num = pdev->id;
	master->dev.of_node = pdev->dev.of_node;
	master->setup = sh_msiof_spi_setup;
	master->prepare_message = sh_msiof_prepare_message;
	master->slave_abort = sh_msiof_slave_abort;
	master->bits_per_word_mask = SPI_BPW_RANGE_MASK(8, 32);
	master->auto_runtime_pm = true;
	master->transfer_one = sh_msiof_transfer_one;
=======
	/* init controller code */
	ctlr->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH;
	ctlr->mode_bits |= SPI_LSB_FIRST | SPI_3WIRE;
	clksrc = clk_get_rate(p->clk);
	ctlr->min_speed_hz = DIV_ROUND_UP(clksrc, 1024);
	ctlr->max_speed_hz = DIV_ROUND_UP(clksrc, 1 << p->min_div_pow);
	ctlr->flags = chipdata->ctlr_flags;
	ctlr->bus_num = pdev->id;
	ctlr->num_chipselect = p->info->num_chipselect;
	ctlr->dev.of_node = pdev->dev.of_node;
	ctlr->setup = sh_msiof_spi_setup;
	ctlr->prepare_message = sh_msiof_prepare_message;
	ctlr->slave_abort = sh_msiof_slave_abort;
	ctlr->bits_per_word_mask = chipdata->bits_per_word_mask;
	ctlr->auto_runtime_pm = true;
	ctlr->transfer_one = sh_msiof_transfer_one;
	ctlr->use_gpio_descriptors = true;
	ctlr->max_native_cs = MAX_SS;
>>>>>>> upstream/android-13

	ret = sh_msiof_request_dma(p);
	if (ret < 0)
		dev_warn(&pdev->dev, "DMA not available, using PIO\n");

<<<<<<< HEAD
	ret = devm_spi_register_master(&pdev->dev, master);
	if (ret < 0) {
		dev_err(&pdev->dev, "spi_register_master error.\n");
=======
	ret = devm_spi_register_controller(&pdev->dev, ctlr);
	if (ret < 0) {
		dev_err(&pdev->dev, "devm_spi_register_controller error.\n");
>>>>>>> upstream/android-13
		goto err2;
	}

	return 0;

 err2:
	sh_msiof_release_dma(p);
	pm_runtime_disable(&pdev->dev);
 err1:
<<<<<<< HEAD
	spi_master_put(master);
=======
	spi_controller_put(ctlr);
>>>>>>> upstream/android-13
	return ret;
}

static int sh_msiof_spi_remove(struct platform_device *pdev)
{
	struct sh_msiof_spi_priv *p = platform_get_drvdata(pdev);

	sh_msiof_release_dma(p);
	pm_runtime_disable(&pdev->dev);
	return 0;
}

static const struct platform_device_id spi_driver_ids[] = {
	{ "spi_sh_msiof",	(kernel_ulong_t)&sh_data },
	{},
};
MODULE_DEVICE_TABLE(platform, spi_driver_ids);

#ifdef CONFIG_PM_SLEEP
static int sh_msiof_spi_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_msiof_spi_priv *p = platform_get_drvdata(pdev);

	return spi_master_suspend(p->master);
=======
	struct sh_msiof_spi_priv *p = dev_get_drvdata(dev);

	return spi_controller_suspend(p->ctlr);
>>>>>>> upstream/android-13
}

static int sh_msiof_spi_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_msiof_spi_priv *p = platform_get_drvdata(pdev);

	return spi_master_resume(p->master);
=======
	struct sh_msiof_spi_priv *p = dev_get_drvdata(dev);

	return spi_controller_resume(p->ctlr);
>>>>>>> upstream/android-13
}

static SIMPLE_DEV_PM_OPS(sh_msiof_spi_pm_ops, sh_msiof_spi_suspend,
			 sh_msiof_spi_resume);
<<<<<<< HEAD
#define DEV_PM_OPS	&sh_msiof_spi_pm_ops
=======
#define DEV_PM_OPS	(&sh_msiof_spi_pm_ops)
>>>>>>> upstream/android-13
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM_SLEEP */

static struct platform_driver sh_msiof_spi_drv = {
	.probe		= sh_msiof_spi_probe,
	.remove		= sh_msiof_spi_remove,
	.id_table	= spi_driver_ids,
	.driver		= {
		.name		= "spi_sh_msiof",
		.pm		= DEV_PM_OPS,
		.of_match_table = of_match_ptr(sh_msiof_match),
	},
};
module_platform_driver(sh_msiof_spi_drv);

<<<<<<< HEAD
MODULE_DESCRIPTION("SuperH MSIOF SPI Master Interface Driver");
=======
MODULE_DESCRIPTION("SuperH MSIOF SPI Controller Interface Driver");
>>>>>>> upstream/android-13
MODULE_AUTHOR("Magnus Damm");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:spi_sh_msiof");
