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

#ifndef __MANTIS_REG_H
#define __MANTIS_REG_H

/* Interrupts */
#define MANTIS_INT_STAT			0x00
#define MANTIS_INT_MASK			0x04

#define MANTIS_INT_RISCSTAT		(0x0f << 28)
<<<<<<< HEAD
#define MANTIS_INT_RISCEN		(0x01 << 27)
#define MANTIS_INT_I2CRACK		(0x01 << 26)

/* #define MANTIS_INT_GPIF			(0xff << 12) */

#define MANTIS_INT_PCMCIA7		(0x01 << 19)
#define MANTIS_INT_PCMCIA6		(0x01 << 18)
#define MANTIS_INT_PCMCIA5		(0x01 << 17)
#define MANTIS_INT_PCMCIA4		(0x01 << 16)
#define MANTIS_INT_PCMCIA3		(0x01 << 15)
#define MANTIS_INT_PCMCIA2		(0x01 << 14)
#define MANTIS_INT_PCMCIA1		(0x01 << 13)
#define MANTIS_INT_PCMCIA0		(0x01 << 12)
#define MANTIS_INT_IRQ1			(0x01 << 11)
#define MANTIS_INT_IRQ0			(0x01 << 10)
#define MANTIS_INT_OCERR		(0x01 <<  8)
#define MANTIS_INT_PABORT		(0x01 <<  7)
#define MANTIS_INT_RIPERR		(0x01 <<  6)
#define MANTIS_INT_PPERR		(0x01 <<  5)
#define MANTIS_INT_FTRGT		(0x01 <<  3)
#define MANTIS_INT_RISCI		(0x01 <<  1)
#define MANTIS_INT_I2CDONE		(0x01 <<  0)
=======
#define MANTIS_INT_RISCEN		BIT(27)
#define MANTIS_INT_I2CRACK		BIT(26)

/* #define MANTIS_INT_GPIF			(0xff << 12) */

#define MANTIS_INT_PCMCIA7		BIT(19)
#define MANTIS_INT_PCMCIA6		BIT(18)
#define MANTIS_INT_PCMCIA5		BIT(17)
#define MANTIS_INT_PCMCIA4		BIT(16)
#define MANTIS_INT_PCMCIA3		BIT(15)
#define MANTIS_INT_PCMCIA2		BIT(14)
#define MANTIS_INT_PCMCIA1		BIT(13)
#define MANTIS_INT_PCMCIA0		BIT(12)
#define MANTIS_INT_IRQ1			BIT(11)
#define MANTIS_INT_IRQ0			BIT(10)
#define MANTIS_INT_OCERR		BIT(8)
#define MANTIS_INT_PABORT		BIT(7)
#define MANTIS_INT_RIPERR		BIT(6)
#define MANTIS_INT_PPERR		BIT(5)
#define MANTIS_INT_FTRGT		BIT(3)
#define MANTIS_INT_RISCI		BIT(1)
#define MANTIS_INT_I2CDONE		BIT(0)
>>>>>>> upstream/android-13

/* DMA */
#define MANTIS_DMA_CTL			0x08
#define MANTIS_GPIF_RD			(0xff << 24)
#define MANTIS_GPIF_WR			(0xff << 16)
<<<<<<< HEAD
#define MANTIS_CPU_DO			(0x01 << 10)
#define MANTIS_DRV_DO			(0x01 <<  9)
#define	MANTIS_I2C_RD			(0x01 <<  7)
#define MANTIS_I2C_WR			(0x01 <<  6)
#define MANTIS_DCAP_MODE		(0x01 <<  5)
#define MANTIS_FIFO_TP_4		(0x00 <<  3)
#define MANTIS_FIFO_TP_8		(0x01 <<  3)
#define MANTIS_FIFO_TP_16		(0x02 <<  3)
#define MANTIS_FIFO_EN			(0x01 <<  2)
#define MANTIS_DCAP_EN			(0x01 <<  1)
#define MANTIS_RISC_EN			(0x01 <<  0)
=======
#define MANTIS_CPU_DO			BIT(10)
#define MANTIS_DRV_DO			BIT(9)
#define	MANTIS_I2C_RD			BIT(7)
#define MANTIS_I2C_WR			BIT(6)
#define MANTIS_DCAP_MODE		BIT(5)
#define MANTIS_FIFO_TP_4		(0x00 <<  3)
#define MANTIS_FIFO_TP_8		(0x01 <<  3)
#define MANTIS_FIFO_TP_16		(0x02 <<  3)
#define MANTIS_FIFO_EN			BIT(2)
#define MANTIS_DCAP_EN			BIT(1)
#define MANTIS_RISC_EN			BIT(0)
>>>>>>> upstream/android-13

/* DEBUG */
#define MANTIS_DEBUGREG			0x0c
#define MANTIS_DATINV			(0x0e <<  7)
#define MANTIS_TOP_DEBUGSEL		(0x07 <<  4)
#define MANTIS_PCMCIA_DEBUGSEL		(0x0f <<  0)

#define MANTIS_RISC_START		0x10
#define MANTIS_RISC_PC			0x14

/* I2C */
#define MANTIS_I2CDATA_CTL		0x18
#define MANTIS_I2C_RATE_1		(0x00 <<  6)
#define MANTIS_I2C_RATE_2		(0x01 <<  6)
#define MANTIS_I2C_RATE_3		(0x02 <<  6)
#define MANTIS_I2C_RATE_4		(0x03 <<  6)
<<<<<<< HEAD
#define MANTIS_I2C_STOP			(0x01 <<  5)
#define MANTIS_I2C_PGMODE		(0x01 <<  3)
=======
#define MANTIS_I2C_STOP			BIT(5)
#define MANTIS_I2C_PGMODE		BIT(3)
>>>>>>> upstream/android-13

/* DATA */
#define MANTIS_CMD_DATA_R1		0x20
#define MANTIS_CMD_DATA_3		(0xff << 24)
#define MANTIS_CMD_DATA_2		(0xff << 16)
#define MANTIS_CMD_DATA_1		(0xff <<  8)
#define MANTIS_CMD_DATA_0		(0xff <<  0)

#define MANTIS_CMD_DATA_R2		0x24
#define MANTIS_CMD_DATA_7		(0xff << 24)
#define MANTIS_CMD_DATA_6		(0xff << 16)
#define MANTIS_CMD_DATA_5		(0xff <<  8)
#define MANTIS_CMD_DATA_4		(0xff <<  0)

#define MANTIS_CONTROL			0x28
<<<<<<< HEAD
#define MANTIS_DET			(0x01 <<  7)
#define MANTIS_DAT_CF_EN		(0x01 <<  6)
#define MANTIS_ACS			(0x03 <<  4)
#define MANTIS_VCCEN			(0x01 <<  3)
#define MANTIS_BYPASS			(0x01 <<  2)
#define MANTIS_MRST			(0x01 <<  1)
#define MANTIS_CRST_INT			(0x01 <<  0)

#define MANTIS_GPIF_CFGSLA		0x84
#define MANTIS_GPIF_WAITSMPL		(0x07 << 28)
#define MANTIS_GPIF_BYTEADDRSUB		(0x01 << 25)
#define MANTIS_GPIF_WAITPOL		(0x01 << 24)
#define MANTIS_GPIF_NCDELAY		(0x07 << 20)
#define MANTIS_GPIF_RW2CSDELAY		(0x07 << 16)
#define MANTIS_GPIF_SLFTIMEDMODE	(0x01 << 15)
#define MANTIS_GPIF_SLFTIMEDDELY	(0x7f <<  8)
#define MANTIS_GPIF_DEVTYPE		(0x07 <<  4)
#define MANTIS_GPIF_BIGENDIAN		(0x01 <<  3)
#define MANTIS_GPIF_FETCHCMD		(0x03 <<  1)
#define MANTIS_GPIF_HWORDDEV		(0x01 <<  0)

#define MANTIS_GPIF_WSTOPER		0x90
#define MANTIS_GPIF_WSTOPERWREN3	(0x01 << 31)
#define MANTIS_GPIF_PARBOOTN		(0x01 << 29)
#define MANTIS_GPIF_WSTOPERSLID3	(0x1f << 24)
#define MANTIS_GPIF_WSTOPERWREN2	(0x01 << 23)
#define MANTIS_GPIF_WSTOPERSLID2	(0x1f << 16)
#define MANTIS_GPIF_WSTOPERWREN1	(0x01 << 15)
#define MANTIS_GPIF_WSTOPERSLID1	(0x1f <<  8)
#define MANTIS_GPIF_WSTOPERWREN0	(0x01 <<  7)
#define MANTIS_GPIF_WSTOPERSLID0	(0x1f <<  0)

#define MANTIS_GPIF_CS2RW		0x94
#define MANTIS_GPIF_CS2RWWREN3		(0x01 << 31)
#define MANTIS_GPIF_CS2RWDELY3		(0x3f << 24)
#define MANTIS_GPIF_CS2RWWREN2		(0x01 << 23)
#define MANTIS_GPIF_CS2RWDELY2		(0x3f << 16)
#define MANTIS_GPIF_CS2RWWREN1		(0x01 << 15)
#define MANTIS_GPIF_CS2RWDELY1		(0x3f <<  8)
#define MANTIS_GPIF_CS2RWWREN0		(0x01 <<  7)
#define MANTIS_GPIF_CS2RWDELY0		(0x3f <<  0)

#define MANTIS_GPIF_IRQCFG		0x98
#define MANTIS_GPIF_IRQPOL		(0x01 <<  8)
#define MANTIS_MASK_WRACK		(0x01 <<  7)
#define MANTIS_MASK_BRRDY		(0x01 <<  6)
#define MANTIS_MASK_OVFLW		(0x01 <<  5)
#define MANTIS_MASK_OTHERR		(0x01 <<  4)
#define MANTIS_MASK_WSTO		(0x01 <<  3)
#define MANTIS_MASK_EXTIRQ		(0x01 <<  2)
#define MANTIS_MASK_PLUGIN		(0x01 <<  1)
#define MANTIS_MASK_PLUGOUT		(0x01 <<  0)

#define MANTIS_GPIF_STATUS		0x9c
#define MANTIS_SBUF_KILLOP		(0x01 << 15)
#define MANTIS_SBUF_OPDONE		(0x01 << 14)
#define MANTIS_SBUF_EMPTY		(0x01 << 13)
#define MANTIS_GPIF_DETSTAT		(0x01 <<  9)
#define MANTIS_GPIF_INTSTAT		(0x01 <<  8)
#define MANTIS_GPIF_WRACK		(0x01 <<  7)
#define MANTIS_GPIF_BRRDY		(0x01 <<  6)
#define MANTIS_SBUF_OVFLW		(0x01 <<  5)
#define MANTIS_GPIF_OTHERR		(0x01 <<  4)
#define MANTIS_SBUF_WSTO		(0x01 <<  3)
#define MANTIS_GPIF_EXTIRQ		(0x01 <<  2)
#define MANTIS_CARD_PLUGIN		(0x01 <<  1)
#define MANTIS_CARD_PLUGOUT		(0x01 <<  0)

#define MANTIS_GPIF_BRADDR		0xa0
#define MANTIS_GPIF_PCMCIAREG		(0x01		<< 27)
#define MANTIS_GPIF_PCMCIAIOM		(0x01		<< 26)
=======
#define MANTIS_DET			BIT(7)
#define MANTIS_DAT_CF_EN		BIT(6)
#define MANTIS_ACS			(0x03 <<  4)
#define MANTIS_VCCEN			BIT(3)
#define MANTIS_BYPASS			BIT(2)
#define MANTIS_MRST			BIT(1)
#define MANTIS_CRST_INT			BIT(0)

#define MANTIS_GPIF_CFGSLA		0x84
#define MANTIS_GPIF_WAITSMPL		(0x07 << 28)
#define MANTIS_GPIF_BYTEADDRSUB		BIT(25)
#define MANTIS_GPIF_WAITPOL		BIT(24)
#define MANTIS_GPIF_NCDELAY		(0x07 << 20)
#define MANTIS_GPIF_RW2CSDELAY		(0x07 << 16)
#define MANTIS_GPIF_SLFTIMEDMODE	BIT(15)
#define MANTIS_GPIF_SLFTIMEDDELY	(0x7f <<  8)
#define MANTIS_GPIF_DEVTYPE		(0x07 <<  4)
#define MANTIS_GPIF_BIGENDIAN		BIT(3)
#define MANTIS_GPIF_FETCHCMD		(0x03 <<  1)
#define MANTIS_GPIF_HWORDDEV		BIT(0)

#define MANTIS_GPIF_WSTOPER		0x90
#define MANTIS_GPIF_WSTOPERWREN3	BIT(31)
#define MANTIS_GPIF_PARBOOTN		BIT(29)
#define MANTIS_GPIF_WSTOPERSLID3	(0x1f << 24)
#define MANTIS_GPIF_WSTOPERWREN2	BIT(23)
#define MANTIS_GPIF_WSTOPERSLID2	(0x1f << 16)
#define MANTIS_GPIF_WSTOPERWREN1	BIT(15)
#define MANTIS_GPIF_WSTOPERSLID1	(0x1f <<  8)
#define MANTIS_GPIF_WSTOPERWREN0	BIT(7)
#define MANTIS_GPIF_WSTOPERSLID0	(0x1f <<  0)

#define MANTIS_GPIF_CS2RW		0x94
#define MANTIS_GPIF_CS2RWWREN3		BIT(31)
#define MANTIS_GPIF_CS2RWDELY3		(0x3f << 24)
#define MANTIS_GPIF_CS2RWWREN2		BIT(23)
#define MANTIS_GPIF_CS2RWDELY2		(0x3f << 16)
#define MANTIS_GPIF_CS2RWWREN1		BIT(15)
#define MANTIS_GPIF_CS2RWDELY1		(0x3f <<  8)
#define MANTIS_GPIF_CS2RWWREN0		BIT(7)
#define MANTIS_GPIF_CS2RWDELY0		(0x3f <<  0)

#define MANTIS_GPIF_IRQCFG		0x98
#define MANTIS_GPIF_IRQPOL		BIT(8)
#define MANTIS_MASK_WRACK		BIT(7)
#define MANTIS_MASK_BRRDY		BIT(6)
#define MANTIS_MASK_OVFLW		BIT(5)
#define MANTIS_MASK_OTHERR		BIT(4)
#define MANTIS_MASK_WSTO		BIT(3)
#define MANTIS_MASK_EXTIRQ		BIT(2)
#define MANTIS_MASK_PLUGIN		BIT(1)
#define MANTIS_MASK_PLUGOUT		BIT(0)

#define MANTIS_GPIF_STATUS		0x9c
#define MANTIS_SBUF_KILLOP		BIT(15)
#define MANTIS_SBUF_OPDONE		BIT(14)
#define MANTIS_SBUF_EMPTY		BIT(13)
#define MANTIS_GPIF_DETSTAT		BIT(9)
#define MANTIS_GPIF_INTSTAT		BIT(8)
#define MANTIS_GPIF_WRACK		BIT(7)
#define MANTIS_GPIF_BRRDY		BIT(6)
#define MANTIS_SBUF_OVFLW		BIT(5)
#define MANTIS_GPIF_OTHERR		BIT(4)
#define MANTIS_SBUF_WSTO		BIT(3)
#define MANTIS_GPIF_EXTIRQ		BIT(2)
#define MANTIS_CARD_PLUGIN		BIT(1)
#define MANTIS_CARD_PLUGOUT		BIT(0)

#define MANTIS_GPIF_BRADDR		0xa0
#define MANTIS_GPIF_PCMCIAREG		BIT(27)
#define MANTIS_GPIF_PCMCIAIOM		BIT(26)
>>>>>>> upstream/android-13
#define MANTIS_GPIF_BR_ADDR		(0xfffffff	<<  0)

#define MANTIS_GPIF_BRBYTES		0xa4
#define MANTIS_GPIF_BRCNT		(0xfff		<<  0)

#define MANTIS_PCMCIA_RESET		0xa8
#define MANTIS_PCMCIA_RSTVAL		(0xff << 0)

#define MANTIS_CARD_RESET		0xac

#define MANTIS_GPIF_ADDR		0xb0
<<<<<<< HEAD
#define MANTIS_GPIF_HIFRDWRN		(0x01		<< 31)
#define MANTIS_GPIF_PCMCIAREG		(0x01		<< 27)
#define MANTIS_GPIF_PCMCIAIOM		(0x01		<< 26)
=======
#define MANTIS_GPIF_HIFRDWRN		BIT(31)
#define MANTIS_GPIF_PCMCIAREG		BIT(27)
#define MANTIS_GPIF_PCMCIAIOM		BIT(26)
>>>>>>> upstream/android-13
#define MANTIS_GPIF_HIFADDR		(0xfffffff	<<  0)

#define MANTIS_GPIF_DOUT		0xb4
#define MANTIS_GPIF_HIFDOUT		(0xfffffff	<<  0)

#define MANTIS_GPIF_DIN			0xb8
#define MANTIS_GPIF_HIFDIN		(0xfffffff	<<  0)

#define MANTIS_GPIF_SPARE		0xbc
#define MANTIS_GPIF_LOGICRD		(0xffff		<< 16)
#define MANTIS_GPIF_LOGICRW		(0xffff		<<  0)

#endif /* __MANTIS_REG_H */
