<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * ST Microelectronics
 * Flexible Static Memory Controller (FSMC)
 * Driver for NAND portions
 *
 * Copyright © 2010 ST Microelectronics
 * Vipin Kumar <vipin.kumar@st.com>
 * Ashish Priyadarshi
 *
 * Based on drivers/mtd/nand/nomadik_nand.c (removed in v3.8)
 *  Copyright © 2007 STMicroelectronics Pvt. Ltd.
 *  Copyright © 2009 Alessandro Rubini
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
=======
>>>>>>> upstream/android-13
 */

#include <linux/clk.h>
#include <linux/completion.h>
<<<<<<< HEAD
=======
#include <linux/delay.h>
>>>>>>> upstream/android-13
#include <linux/dmaengine.h>
#include <linux/dma-direction.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/resource.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/mtd/mtd.h>
<<<<<<< HEAD
#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand_ecc.h>
=======
#include <linux/mtd/nand-ecc-sw-hamming.h>
#include <linux/mtd/rawnand.h>
>>>>>>> upstream/android-13
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mtd/partitions.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/amba/bus.h>
#include <mtd/mtd-abi.h>

/* fsmc controller registers for NOR flash */
#define CTRL			0x0
	/* ctrl register definitions */
<<<<<<< HEAD
	#define BANK_ENABLE		(1 << 0)
	#define MUXED			(1 << 1)
	#define NOR_DEV			(2 << 2)
	#define WIDTH_8			(0 << 4)
	#define WIDTH_16		(1 << 4)
	#define RSTPWRDWN		(1 << 6)
	#define WPROT			(1 << 7)
	#define WRT_ENABLE		(1 << 12)
	#define WAIT_ENB		(1 << 13)
=======
	#define BANK_ENABLE		BIT(0)
	#define MUXED			BIT(1)
	#define NOR_DEV			(2 << 2)
	#define WIDTH_16		BIT(4)
	#define RSTPWRDWN		BIT(6)
	#define WPROT			BIT(7)
	#define WRT_ENABLE		BIT(12)
	#define WAIT_ENB		BIT(13)
>>>>>>> upstream/android-13

#define CTRL_TIM		0x4
	/* ctrl_tim register definitions */

#define FSMC_NOR_BANK_SZ	0x8
#define FSMC_NOR_REG_SIZE	0x40

<<<<<<< HEAD
#define FSMC_NOR_REG(base, bank, reg)		(base + \
						FSMC_NOR_BANK_SZ * (bank) + \
						reg)
=======
#define FSMC_NOR_REG(base, bank, reg)	((base) +			\
					 (FSMC_NOR_BANK_SZ * (bank)) +	\
					 (reg))
>>>>>>> upstream/android-13

/* fsmc controller registers for NAND flash */
#define FSMC_PC			0x00
	/* pc register definitions */
<<<<<<< HEAD
	#define FSMC_RESET		(1 << 0)
	#define FSMC_WAITON		(1 << 1)
	#define FSMC_ENABLE		(1 << 2)
	#define FSMC_DEVTYPE_NAND	(1 << 3)
	#define FSMC_DEVWID_8		(0 << 4)
	#define FSMC_DEVWID_16		(1 << 4)
	#define FSMC_ECCEN		(1 << 6)
	#define FSMC_ECCPLEN_512	(0 << 7)
	#define FSMC_ECCPLEN_256	(1 << 7)
	#define FSMC_TCLR_1		(1)
	#define FSMC_TCLR_SHIFT		(9)
	#define FSMC_TCLR_MASK		(0xF)
	#define FSMC_TAR_1		(1)
=======
	#define FSMC_RESET		BIT(0)
	#define FSMC_WAITON		BIT(1)
	#define FSMC_ENABLE		BIT(2)
	#define FSMC_DEVTYPE_NAND	BIT(3)
	#define FSMC_DEVWID_16		BIT(4)
	#define FSMC_ECCEN		BIT(6)
	#define FSMC_ECCPLEN_256	BIT(7)
	#define FSMC_TCLR_SHIFT		(9)
	#define FSMC_TCLR_MASK		(0xF)
>>>>>>> upstream/android-13
	#define FSMC_TAR_SHIFT		(13)
	#define FSMC_TAR_MASK		(0xF)
#define STS			0x04
	/* sts register definitions */
<<<<<<< HEAD
	#define FSMC_CODE_RDY		(1 << 15)
#define COMM			0x08
	/* comm register definitions */
	#define FSMC_TSET_0		0
	#define FSMC_TSET_SHIFT		0
	#define FSMC_TSET_MASK		0xFF
	#define FSMC_TWAIT_6		6
	#define FSMC_TWAIT_SHIFT	8
	#define FSMC_TWAIT_MASK		0xFF
	#define FSMC_THOLD_4		4
	#define FSMC_THOLD_SHIFT	16
	#define FSMC_THOLD_MASK		0xFF
	#define FSMC_THIZ_1		1
=======
	#define FSMC_CODE_RDY		BIT(15)
#define COMM			0x08
	/* comm register definitions */
	#define FSMC_TSET_SHIFT		0
	#define FSMC_TSET_MASK		0xFF
	#define FSMC_TWAIT_SHIFT	8
	#define FSMC_TWAIT_MASK		0xFF
	#define FSMC_THOLD_SHIFT	16
	#define FSMC_THOLD_MASK		0xFF
>>>>>>> upstream/android-13
	#define FSMC_THIZ_SHIFT		24
	#define FSMC_THIZ_MASK		0xFF
#define ATTRIB			0x0C
#define IOATA			0x10
#define ECC1			0x14
#define ECC2			0x18
#define ECC3			0x1C
#define FSMC_NAND_BANK_SZ	0x20

#define FSMC_BUSY_WAIT_TIMEOUT	(1 * HZ)

<<<<<<< HEAD
struct fsmc_nand_timings {
	uint8_t tclr;
	uint8_t tar;
	uint8_t thiz;
	uint8_t thold;
	uint8_t twait;
	uint8_t tset;
=======
/*
 * According to SPEAr300 Reference Manual (RM0082)
 *  TOUDEL = 7ns (Output delay from the flip-flops to the board)
 *  TINDEL = 5ns (Input delay from the board to the flipflop)
 */
#define TOUTDEL	7000
#define TINDEL	5000

struct fsmc_nand_timings {
	u8 tclr;
	u8 tar;
	u8 thiz;
	u8 thold;
	u8 twait;
	u8 tset;
>>>>>>> upstream/android-13
};

enum access_mode {
	USE_DMA_ACCESS = 1,
	USE_WORD_ACCESS,
};

/**
 * struct fsmc_nand_data - structure for FSMC NAND device state
 *
<<<<<<< HEAD
 * @pid:		Part ID on the AMBA PrimeCell format
 * @mtd:		MTD info for a NAND flash.
 * @nand:		Chip related info for a NAND flash.
 * @partitions:		Partition info for a NAND Flash.
 * @nr_partitions:	Total number of partition of a NAND flash.
 *
 * @bank:		Bank number for probed device.
=======
 * @base:		Inherit from the nand_controller struct
 * @pid:		Part ID on the AMBA PrimeCell format
 * @nand:		Chip related info for a NAND flash.
 *
 * @bank:		Bank number for probed device.
 * @dev:		Parent device
 * @mode:		Access mode
>>>>>>> upstream/android-13
 * @clk:		Clock structure for FSMC.
 *
 * @read_dma_chan:	DMA channel for read access
 * @write_dma_chan:	DMA channel for write access to NAND
 * @dma_access_complete: Completion structure
 *
<<<<<<< HEAD
=======
 * @dev_timings:	NAND timings
 *
>>>>>>> upstream/android-13
 * @data_pa:		NAND Physical port for Data.
 * @data_va:		NAND port for Data.
 * @cmd_va:		NAND port for Command.
 * @addr_va:		NAND port for Address.
 * @regs_va:		Registers base address for a given bank.
 */
struct fsmc_nand_data {
<<<<<<< HEAD
=======
	struct nand_controller	base;
>>>>>>> upstream/android-13
	u32			pid;
	struct nand_chip	nand;

	unsigned int		bank;
	struct device		*dev;
	enum access_mode	mode;
	struct clk		*clk;

	/* DMA related objects */
	struct dma_chan		*read_dma_chan;
	struct dma_chan		*write_dma_chan;
	struct completion	dma_access_complete;

	struct fsmc_nand_timings *dev_timings;

	dma_addr_t		data_pa;
	void __iomem		*data_va;
	void __iomem		*cmd_va;
	void __iomem		*addr_va;
	void __iomem		*regs_va;
};

static int fsmc_ecc1_ooblayout_ecc(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *oobregion)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	if (section >= chip->ecc.steps)
		return -ERANGE;

	oobregion->offset = (section * 16) + 2;
	oobregion->length = 3;

	return 0;
}

static int fsmc_ecc1_ooblayout_free(struct mtd_info *mtd, int section,
				    struct mtd_oob_region *oobregion)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	if (section >= chip->ecc.steps)
		return -ERANGE;

	oobregion->offset = (section * 16) + 8;

	if (section < chip->ecc.steps - 1)
		oobregion->length = 8;
	else
		oobregion->length = mtd->oobsize - oobregion->offset;

	return 0;
}

static const struct mtd_ooblayout_ops fsmc_ecc1_ooblayout_ops = {
	.ecc = fsmc_ecc1_ooblayout_ecc,
	.free = fsmc_ecc1_ooblayout_free,
};

/*
 * ECC placement definitions in oobfree type format.
 * There are 13 bytes of ecc for every 512 byte block and it has to be read
 * consecutively and immediately after the 512 byte data block for hardware to
 * generate the error bit offsets in 512 byte data.
 */
static int fsmc_ecc4_ooblayout_ecc(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *oobregion)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	if (section >= chip->ecc.steps)
		return -ERANGE;

	oobregion->length = chip->ecc.bytes;

	if (!section && mtd->writesize <= 512)
		oobregion->offset = 0;
	else
		oobregion->offset = (section * 16) + 2;

	return 0;
}

static int fsmc_ecc4_ooblayout_free(struct mtd_info *mtd, int section,
				    struct mtd_oob_region *oobregion)
{
	struct nand_chip *chip = mtd_to_nand(mtd);

	if (section >= chip->ecc.steps)
		return -ERANGE;

	oobregion->offset = (section * 16) + 15;

	if (section < chip->ecc.steps - 1)
		oobregion->length = 3;
	else
		oobregion->length = mtd->oobsize - oobregion->offset;

	return 0;
}

static const struct mtd_ooblayout_ops fsmc_ecc4_ooblayout_ops = {
	.ecc = fsmc_ecc4_ooblayout_ecc,
	.free = fsmc_ecc4_ooblayout_free,
};

<<<<<<< HEAD
static inline struct fsmc_nand_data *mtd_to_fsmc(struct mtd_info *mtd)
{
	return container_of(mtd_to_nand(mtd), struct fsmc_nand_data, nand);
=======
static inline struct fsmc_nand_data *nand_to_fsmc(struct nand_chip *chip)
{
	return container_of(chip, struct fsmc_nand_data, nand);
>>>>>>> upstream/android-13
}

/*
 * fsmc_nand_setup - FSMC (Flexible Static Memory Controller) init routine
 *
 * This routine initializes timing parameters related to NAND memory access in
 * FSMC registers
 */
static void fsmc_nand_setup(struct fsmc_nand_data *host,
			    struct fsmc_nand_timings *tims)
{
<<<<<<< HEAD
	uint32_t value = FSMC_DEVTYPE_NAND | FSMC_ENABLE | FSMC_WAITON;
	uint32_t tclr, tar, thiz, thold, twait, tset;
=======
	u32 value = FSMC_DEVTYPE_NAND | FSMC_ENABLE | FSMC_WAITON;
	u32 tclr, tar, thiz, thold, twait, tset;
>>>>>>> upstream/android-13

	tclr = (tims->tclr & FSMC_TCLR_MASK) << FSMC_TCLR_SHIFT;
	tar = (tims->tar & FSMC_TAR_MASK) << FSMC_TAR_SHIFT;
	thiz = (tims->thiz & FSMC_THIZ_MASK) << FSMC_THIZ_SHIFT;
	thold = (tims->thold & FSMC_THOLD_MASK) << FSMC_THOLD_SHIFT;
	twait = (tims->twait & FSMC_TWAIT_MASK) << FSMC_TWAIT_SHIFT;
	tset = (tims->tset & FSMC_TSET_MASK) << FSMC_TSET_SHIFT;

	if (host->nand.options & NAND_BUSWIDTH_16)
<<<<<<< HEAD
		writel_relaxed(value | FSMC_DEVWID_16,
			       host->regs_va + FSMC_PC);
	else
		writel_relaxed(value | FSMC_DEVWID_8, host->regs_va + FSMC_PC);

	writel_relaxed(readl(host->regs_va + FSMC_PC) | tclr | tar,
		       host->regs_va + FSMC_PC);
=======
		value |= FSMC_DEVWID_16;

	writel_relaxed(value | tclr | tar, host->regs_va + FSMC_PC);
>>>>>>> upstream/android-13
	writel_relaxed(thiz | thold | twait | tset, host->regs_va + COMM);
	writel_relaxed(thiz | thold | twait | tset, host->regs_va + ATTRIB);
}

static int fsmc_calc_timings(struct fsmc_nand_data *host,
			     const struct nand_sdr_timings *sdrt,
			     struct fsmc_nand_timings *tims)
{
	unsigned long hclk = clk_get_rate(host->clk);
	unsigned long hclkn = NSEC_PER_SEC / hclk;
<<<<<<< HEAD
	uint32_t thiz, thold, twait, tset;
=======
	u32 thiz, thold, twait, tset, twait_min;
>>>>>>> upstream/android-13

	if (sdrt->tRC_min < 30000)
		return -EOPNOTSUPP;

	tims->tar = DIV_ROUND_UP(sdrt->tAR_min / 1000, hclkn) - 1;
	if (tims->tar > FSMC_TAR_MASK)
		tims->tar = FSMC_TAR_MASK;
	tims->tclr = DIV_ROUND_UP(sdrt->tCLR_min / 1000, hclkn) - 1;
	if (tims->tclr > FSMC_TCLR_MASK)
		tims->tclr = FSMC_TCLR_MASK;

	thiz = sdrt->tCS_min - sdrt->tWP_min;
	tims->thiz = DIV_ROUND_UP(thiz / 1000, hclkn);

	thold = sdrt->tDH_min;
	if (thold < sdrt->tCH_min)
		thold = sdrt->tCH_min;
	if (thold < sdrt->tCLH_min)
		thold = sdrt->tCLH_min;
	if (thold < sdrt->tWH_min)
		thold = sdrt->tWH_min;
	if (thold < sdrt->tALH_min)
		thold = sdrt->tALH_min;
	if (thold < sdrt->tREH_min)
		thold = sdrt->tREH_min;
	tims->thold = DIV_ROUND_UP(thold / 1000, hclkn);
	if (tims->thold == 0)
		tims->thold = 1;
	else if (tims->thold > FSMC_THOLD_MASK)
		tims->thold = FSMC_THOLD_MASK;

<<<<<<< HEAD
	twait = max(sdrt->tRP_min, sdrt->tWP_min);
	tims->twait = DIV_ROUND_UP(twait / 1000, hclkn) - 1;
	if (tims->twait == 0)
		tims->twait = 1;
	else if (tims->twait > FSMC_TWAIT_MASK)
		tims->twait = FSMC_TWAIT_MASK;

=======
>>>>>>> upstream/android-13
	tset = max(sdrt->tCS_min - sdrt->tWP_min,
		   sdrt->tCEA_max - sdrt->tREA_max);
	tims->tset = DIV_ROUND_UP(tset / 1000, hclkn) - 1;
	if (tims->tset == 0)
		tims->tset = 1;
	else if (tims->tset > FSMC_TSET_MASK)
		tims->tset = FSMC_TSET_MASK;

<<<<<<< HEAD
	return 0;
}

static int fsmc_setup_data_interface(struct mtd_info *mtd, int csline,
				     const struct nand_data_interface *conf)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct fsmc_nand_data *host = nand_get_controller_data(nand);
=======
	/*
	 * According to SPEAr300 Reference Manual (RM0082) which gives more
	 * information related to FSMSC timings than the SPEAr600 one (RM0305),
	 *   twait >= tCEA - (tset * TCLK) + TOUTDEL + TINDEL
	 */
	twait_min = sdrt->tCEA_max - ((tims->tset + 1) * hclkn * 1000)
		    + TOUTDEL + TINDEL;
	twait = max3(sdrt->tRP_min, sdrt->tWP_min, twait_min);

	tims->twait = DIV_ROUND_UP(twait / 1000, hclkn) - 1;
	if (tims->twait == 0)
		tims->twait = 1;
	else if (tims->twait > FSMC_TWAIT_MASK)
		tims->twait = FSMC_TWAIT_MASK;

	return 0;
}

static int fsmc_setup_interface(struct nand_chip *nand, int csline,
				const struct nand_interface_config *conf)
{
	struct fsmc_nand_data *host = nand_to_fsmc(nand);
>>>>>>> upstream/android-13
	struct fsmc_nand_timings tims;
	const struct nand_sdr_timings *sdrt;
	int ret;

	sdrt = nand_get_sdr_timings(conf);
	if (IS_ERR(sdrt))
		return PTR_ERR(sdrt);

	ret = fsmc_calc_timings(host, sdrt, &tims);
	if (ret)
		return ret;

	if (csline == NAND_DATA_IFACE_CHECK_ONLY)
		return 0;

	fsmc_nand_setup(host, &tims);

	return 0;
}

/*
 * fsmc_enable_hwecc - Enables Hardware ECC through FSMC registers
 */
<<<<<<< HEAD
static void fsmc_enable_hwecc(struct mtd_info *mtd, int mode)
{
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
=======
static void fsmc_enable_hwecc(struct nand_chip *chip, int mode)
{
	struct fsmc_nand_data *host = nand_to_fsmc(chip);
>>>>>>> upstream/android-13

	writel_relaxed(readl(host->regs_va + FSMC_PC) & ~FSMC_ECCPLEN_256,
		       host->regs_va + FSMC_PC);
	writel_relaxed(readl(host->regs_va + FSMC_PC) & ~FSMC_ECCEN,
		       host->regs_va + FSMC_PC);
	writel_relaxed(readl(host->regs_va + FSMC_PC) | FSMC_ECCEN,
		       host->regs_va + FSMC_PC);
}

/*
 * fsmc_read_hwecc_ecc4 - Hardware ECC calculator for ecc4 option supported by
 * FSMC. ECC is 13 bytes for 512 bytes of data (supports error correction up to
 * max of 8-bits)
 */
<<<<<<< HEAD
static int fsmc_read_hwecc_ecc4(struct mtd_info *mtd, const uint8_t *data,
				uint8_t *ecc)
{
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
	uint32_t ecc_tmp;
=======
static int fsmc_read_hwecc_ecc4(struct nand_chip *chip, const u8 *data,
				u8 *ecc)
{
	struct fsmc_nand_data *host = nand_to_fsmc(chip);
	u32 ecc_tmp;
>>>>>>> upstream/android-13
	unsigned long deadline = jiffies + FSMC_BUSY_WAIT_TIMEOUT;

	do {
		if (readl_relaxed(host->regs_va + STS) & FSMC_CODE_RDY)
			break;
<<<<<<< HEAD
		else
			cond_resched();
=======

		cond_resched();
>>>>>>> upstream/android-13
	} while (!time_after_eq(jiffies, deadline));

	if (time_after_eq(jiffies, deadline)) {
		dev_err(host->dev, "calculate ecc timed out\n");
		return -ETIMEDOUT;
	}

	ecc_tmp = readl_relaxed(host->regs_va + ECC1);
<<<<<<< HEAD
	ecc[0] = (uint8_t) (ecc_tmp >> 0);
	ecc[1] = (uint8_t) (ecc_tmp >> 8);
	ecc[2] = (uint8_t) (ecc_tmp >> 16);
	ecc[3] = (uint8_t) (ecc_tmp >> 24);

	ecc_tmp = readl_relaxed(host->regs_va + ECC2);
	ecc[4] = (uint8_t) (ecc_tmp >> 0);
	ecc[5] = (uint8_t) (ecc_tmp >> 8);
	ecc[6] = (uint8_t) (ecc_tmp >> 16);
	ecc[7] = (uint8_t) (ecc_tmp >> 24);

	ecc_tmp = readl_relaxed(host->regs_va + ECC3);
	ecc[8] = (uint8_t) (ecc_tmp >> 0);
	ecc[9] = (uint8_t) (ecc_tmp >> 8);
	ecc[10] = (uint8_t) (ecc_tmp >> 16);
	ecc[11] = (uint8_t) (ecc_tmp >> 24);

	ecc_tmp = readl_relaxed(host->regs_va + STS);
	ecc[12] = (uint8_t) (ecc_tmp >> 16);
=======
	ecc[0] = ecc_tmp;
	ecc[1] = ecc_tmp >> 8;
	ecc[2] = ecc_tmp >> 16;
	ecc[3] = ecc_tmp >> 24;

	ecc_tmp = readl_relaxed(host->regs_va + ECC2);
	ecc[4] = ecc_tmp;
	ecc[5] = ecc_tmp >> 8;
	ecc[6] = ecc_tmp >> 16;
	ecc[7] = ecc_tmp >> 24;

	ecc_tmp = readl_relaxed(host->regs_va + ECC3);
	ecc[8] = ecc_tmp;
	ecc[9] = ecc_tmp >> 8;
	ecc[10] = ecc_tmp >> 16;
	ecc[11] = ecc_tmp >> 24;

	ecc_tmp = readl_relaxed(host->regs_va + STS);
	ecc[12] = ecc_tmp >> 16;
>>>>>>> upstream/android-13

	return 0;
}

/*
 * fsmc_read_hwecc_ecc1 - Hardware ECC calculator for ecc1 option supported by
 * FSMC. ECC is 3 bytes for 512 bytes of data (supports error correction up to
 * max of 1-bit)
 */
<<<<<<< HEAD
static int fsmc_read_hwecc_ecc1(struct mtd_info *mtd, const uint8_t *data,
				uint8_t *ecc)
{
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
	uint32_t ecc_tmp;

	ecc_tmp = readl_relaxed(host->regs_va + ECC1);
	ecc[0] = (uint8_t) (ecc_tmp >> 0);
	ecc[1] = (uint8_t) (ecc_tmp >> 8);
	ecc[2] = (uint8_t) (ecc_tmp >> 16);
=======
static int fsmc_read_hwecc_ecc1(struct nand_chip *chip, const u8 *data,
				u8 *ecc)
{
	struct fsmc_nand_data *host = nand_to_fsmc(chip);
	u32 ecc_tmp;

	ecc_tmp = readl_relaxed(host->regs_va + ECC1);
	ecc[0] = ecc_tmp;
	ecc[1] = ecc_tmp >> 8;
	ecc[2] = ecc_tmp >> 16;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/* Count the number of 0's in buff upto a max of max_bits */
static int count_written_bits(uint8_t *buff, int size, int max_bits)
=======
static int fsmc_correct_ecc1(struct nand_chip *chip,
			     unsigned char *buf,
			     unsigned char *read_ecc,
			     unsigned char *calc_ecc)
{
	bool sm_order = chip->ecc.options & NAND_ECC_SOFT_HAMMING_SM_ORDER;

	return ecc_sw_hamming_correct(buf, read_ecc, calc_ecc,
				      chip->ecc.size, sm_order);
}

/* Count the number of 0's in buff upto a max of max_bits */
static int count_written_bits(u8 *buff, int size, int max_bits)
>>>>>>> upstream/android-13
{
	int k, written_bits = 0;

	for (k = 0; k < size; k++) {
		written_bits += hweight8(~buff[k]);
		if (written_bits > max_bits)
			break;
	}

	return written_bits;
}

static void dma_complete(void *param)
{
	struct fsmc_nand_data *host = param;

	complete(&host->dma_access_complete);
}

static int dma_xfer(struct fsmc_nand_data *host, void *buffer, int len,
<<<<<<< HEAD
		enum dma_data_direction direction)
=======
		    enum dma_data_direction direction)
>>>>>>> upstream/android-13
{
	struct dma_chan *chan;
	struct dma_device *dma_dev;
	struct dma_async_tx_descriptor *tx;
	dma_addr_t dma_dst, dma_src, dma_addr;
	dma_cookie_t cookie;
	unsigned long flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;
	int ret;
	unsigned long time_left;

	if (direction == DMA_TO_DEVICE)
		chan = host->write_dma_chan;
	else if (direction == DMA_FROM_DEVICE)
		chan = host->read_dma_chan;
	else
		return -EINVAL;

	dma_dev = chan->device;
	dma_addr = dma_map_single(dma_dev->dev, buffer, len, direction);

	if (direction == DMA_TO_DEVICE) {
		dma_src = dma_addr;
		dma_dst = host->data_pa;
	} else {
		dma_src = host->data_pa;
		dma_dst = dma_addr;
	}

	tx = dma_dev->device_prep_dma_memcpy(chan, dma_dst, dma_src,
			len, flags);
	if (!tx) {
		dev_err(host->dev, "device_prep_dma_memcpy error\n");
		ret = -EIO;
		goto unmap_dma;
	}

	tx->callback = dma_complete;
	tx->callback_param = host;
	cookie = tx->tx_submit(tx);

	ret = dma_submit_error(cookie);
	if (ret) {
		dev_err(host->dev, "dma_submit_error %d\n", cookie);
		goto unmap_dma;
	}

	dma_async_issue_pending(chan);

	time_left =
	wait_for_completion_timeout(&host->dma_access_complete,
<<<<<<< HEAD
				msecs_to_jiffies(3000));
=======
				    msecs_to_jiffies(3000));
>>>>>>> upstream/android-13
	if (time_left == 0) {
		dmaengine_terminate_all(chan);
		dev_err(host->dev, "wait_for_completion_timeout\n");
		ret = -ETIMEDOUT;
		goto unmap_dma;
	}

	ret = 0;

unmap_dma:
	dma_unmap_single(dma_dev->dev, dma_addr, len, direction);

	return ret;
}

/*
 * fsmc_write_buf - write buffer to chip
<<<<<<< HEAD
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void fsmc_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct fsmc_nand_data *host  = mtd_to_fsmc(mtd);
	int i;

	if (IS_ALIGNED((uintptr_t)buf, sizeof(uint32_t)) &&
			IS_ALIGNED(len, sizeof(uint32_t))) {
		uint32_t *p = (uint32_t *)buf;
=======
 * @host:	FSMC NAND controller
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void fsmc_write_buf(struct fsmc_nand_data *host, const u8 *buf,
			   int len)
{
	int i;

	if (IS_ALIGNED((uintptr_t)buf, sizeof(u32)) &&
	    IS_ALIGNED(len, sizeof(u32))) {
		u32 *p = (u32 *)buf;

>>>>>>> upstream/android-13
		len = len >> 2;
		for (i = 0; i < len; i++)
			writel_relaxed(p[i], host->data_va);
	} else {
		for (i = 0; i < len; i++)
			writeb_relaxed(buf[i], host->data_va);
	}
}

/*
 * fsmc_read_buf - read chip data into buffer
<<<<<<< HEAD
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void fsmc_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct fsmc_nand_data *host  = mtd_to_fsmc(mtd);
	int i;

	if (IS_ALIGNED((uintptr_t)buf, sizeof(uint32_t)) &&
			IS_ALIGNED(len, sizeof(uint32_t))) {
		uint32_t *p = (uint32_t *)buf;
=======
 * @host:	FSMC NAND controller
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void fsmc_read_buf(struct fsmc_nand_data *host, u8 *buf, int len)
{
	int i;

	if (IS_ALIGNED((uintptr_t)buf, sizeof(u32)) &&
	    IS_ALIGNED(len, sizeof(u32))) {
		u32 *p = (u32 *)buf;

>>>>>>> upstream/android-13
		len = len >> 2;
		for (i = 0; i < len; i++)
			p[i] = readl_relaxed(host->data_va);
	} else {
		for (i = 0; i < len; i++)
			buf[i] = readb_relaxed(host->data_va);
	}
}

/*
 * fsmc_read_buf_dma - read chip data into buffer
<<<<<<< HEAD
 * @mtd:	MTD device structure
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void fsmc_read_buf_dma(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct fsmc_nand_data *host  = mtd_to_fsmc(mtd);

=======
 * @host:	FSMC NAND controller
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 */
static void fsmc_read_buf_dma(struct fsmc_nand_data *host, u8 *buf,
			      int len)
{
>>>>>>> upstream/android-13
	dma_xfer(host, buf, len, DMA_FROM_DEVICE);
}

/*
 * fsmc_write_buf_dma - write buffer to chip
<<<<<<< HEAD
 * @mtd:	MTD device structure
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void fsmc_write_buf_dma(struct mtd_info *mtd, const uint8_t *buf,
		int len)
{
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);

	dma_xfer(host, (void *)buf, len, DMA_TO_DEVICE);
}

/* fsmc_select_chip - assert or deassert nCE */
static void fsmc_select_chip(struct mtd_info *mtd, int chipnr)
{
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
	u32 pc;

	/* Support only one CS */
	if (chipnr > 0)
		return;

	pc = readl(host->regs_va + FSMC_PC);
	if (chipnr < 0)
		writel_relaxed(pc & ~FSMC_ENABLE, host->regs_va + FSMC_PC);
	else
		writel_relaxed(pc | FSMC_ENABLE, host->regs_va + FSMC_PC);

	/* nCE line must be asserted before starting any operation */
	mb();
}

=======
 * @host:	FSMC NAND controller
 * @buf:	data buffer
 * @len:	number of bytes to write
 */
static void fsmc_write_buf_dma(struct fsmc_nand_data *host, const u8 *buf,
			       int len)
{
	dma_xfer(host, (void *)buf, len, DMA_TO_DEVICE);
}

>>>>>>> upstream/android-13
/*
 * fsmc_exec_op - hook called by the core to execute NAND operations
 *
 * This controller is simple enough and thus does not need to use the parser
 * provided by the core, instead, handle every situation here.
 */
static int fsmc_exec_op(struct nand_chip *chip, const struct nand_operation *op,
			bool check_only)
{
<<<<<<< HEAD
	struct mtd_info *mtd = nand_to_mtd(chip);
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
=======
	struct fsmc_nand_data *host = nand_to_fsmc(chip);
>>>>>>> upstream/android-13
	const struct nand_op_instr *instr = NULL;
	int ret = 0;
	unsigned int op_id;
	int i;

<<<<<<< HEAD
	pr_debug("Executing operation [%d instructions]:\n", op->ninstrs);
	for (op_id = 0; op_id < op->ninstrs; op_id++) {
		instr = &op->instrs[op_id];

		switch (instr->type) {
		case NAND_OP_CMD_INSTR:
			pr_debug("  ->CMD      [0x%02x]\n",
				 instr->ctx.cmd.opcode);

=======
	if (check_only)
		return 0;

	pr_debug("Executing operation [%d instructions]:\n", op->ninstrs);

	for (op_id = 0; op_id < op->ninstrs; op_id++) {
		instr = &op->instrs[op_id];

		nand_op_trace("  ", instr);

		switch (instr->type) {
		case NAND_OP_CMD_INSTR:
>>>>>>> upstream/android-13
			writeb_relaxed(instr->ctx.cmd.opcode, host->cmd_va);
			break;

		case NAND_OP_ADDR_INSTR:
<<<<<<< HEAD
			pr_debug("  ->ADDR     [%d cyc]",
				 instr->ctx.addr.naddrs);

=======
>>>>>>> upstream/android-13
			for (i = 0; i < instr->ctx.addr.naddrs; i++)
				writeb_relaxed(instr->ctx.addr.addrs[i],
					       host->addr_va);
			break;

		case NAND_OP_DATA_IN_INSTR:
<<<<<<< HEAD
			pr_debug("  ->DATA_IN  [%d B%s]\n", instr->ctx.data.len,
				 instr->ctx.data.force_8bit ?
				 ", force 8-bit" : "");

			if (host->mode == USE_DMA_ACCESS)
				fsmc_read_buf_dma(mtd, instr->ctx.data.buf.in,
						  instr->ctx.data.len);
			else
				fsmc_read_buf(mtd, instr->ctx.data.buf.in,
=======
			if (host->mode == USE_DMA_ACCESS)
				fsmc_read_buf_dma(host, instr->ctx.data.buf.in,
						  instr->ctx.data.len);
			else
				fsmc_read_buf(host, instr->ctx.data.buf.in,
>>>>>>> upstream/android-13
					      instr->ctx.data.len);
			break;

		case NAND_OP_DATA_OUT_INSTR:
<<<<<<< HEAD
			pr_debug("  ->DATA_OUT [%d B%s]\n", instr->ctx.data.len,
				 instr->ctx.data.force_8bit ?
				 ", force 8-bit" : "");

			if (host->mode == USE_DMA_ACCESS)
				fsmc_write_buf_dma(mtd, instr->ctx.data.buf.out,
						   instr->ctx.data.len);
			else
				fsmc_write_buf(mtd, instr->ctx.data.buf.out,
=======
			if (host->mode == USE_DMA_ACCESS)
				fsmc_write_buf_dma(host,
						   instr->ctx.data.buf.out,
						   instr->ctx.data.len);
			else
				fsmc_write_buf(host, instr->ctx.data.buf.out,
>>>>>>> upstream/android-13
					       instr->ctx.data.len);
			break;

		case NAND_OP_WAITRDY_INSTR:
<<<<<<< HEAD
			pr_debug("  ->WAITRDY  [max %d ms]\n",
				 instr->ctx.waitrdy.timeout_ms);

=======
>>>>>>> upstream/android-13
			ret = nand_soft_waitrdy(chip,
						instr->ctx.waitrdy.timeout_ms);
			break;
		}
<<<<<<< HEAD
=======

		if (instr->delay_ns)
			ndelay(instr->delay_ns);
>>>>>>> upstream/android-13
	}

	return ret;
}

/*
 * fsmc_read_page_hwecc
<<<<<<< HEAD
 * @mtd:	mtd info structure
=======
>>>>>>> upstream/android-13
 * @chip:	nand chip info structure
 * @buf:	buffer to store read data
 * @oob_required:	caller expects OOB data read to chip->oob_poi
 * @page:	page number to read
 *
 * This routine is needed for fsmc version 8 as reading from NAND chip has to be
 * performed in a strict sequence as follows:
 * data(512 byte) -> ecc(13 byte)
 * After this read, fsmc hardware generates and reports error data bits(up to a
 * max of 8 bits)
 */
<<<<<<< HEAD
static int fsmc_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
				 uint8_t *buf, int oob_required, int page)
{
	int i, j, s, stat, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_calc = chip->ecc.calc_buf;
	uint8_t *ecc_code = chip->ecc.code_buf;
	int off, len, group = 0;
	/*
	 * ecc_oob is intentionally taken as uint16_t. In 16bit devices, we
	 * end up reading 14 bytes (7 words) from oob. The local array is
	 * to maintain word alignment
	 */
	uint16_t ecc_oob[7];
	uint8_t *oob = (uint8_t *)&ecc_oob[0];
=======
static int fsmc_read_page_hwecc(struct nand_chip *chip, u8 *buf,
				int oob_required, int page)
{
	struct mtd_info *mtd = nand_to_mtd(chip);
	int i, j, s, stat, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	u8 *p = buf;
	u8 *ecc_calc = chip->ecc.calc_buf;
	u8 *ecc_code = chip->ecc.code_buf;
	int off, len, ret, group = 0;
	/*
	 * ecc_oob is intentionally taken as u16. In 16bit devices, we
	 * end up reading 14 bytes (7 words) from oob. The local array is
	 * to maintain word alignment
	 */
	u16 ecc_oob[7];
	u8 *oob = (u8 *)&ecc_oob[0];
>>>>>>> upstream/android-13
	unsigned int max_bitflips = 0;

	for (i = 0, s = 0; s < eccsteps; s++, i += eccbytes, p += eccsize) {
		nand_read_page_op(chip, page, s * eccsize, NULL, 0);
<<<<<<< HEAD
		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		nand_read_data_op(chip, p, eccsize, false);

		for (j = 0; j < eccbytes;) {
			struct mtd_oob_region oobregion;
			int ret;
=======
		chip->ecc.hwctl(chip, NAND_ECC_READ);
		ret = nand_read_data_op(chip, p, eccsize, false, false);
		if (ret)
			return ret;

		for (j = 0; j < eccbytes;) {
			struct mtd_oob_region oobregion;
>>>>>>> upstream/android-13

			ret = mtd_ooblayout_ecc(mtd, group++, &oobregion);
			if (ret)
				return ret;

			off = oobregion.offset;
			len = oobregion.length;

			/*
			 * length is intentionally kept a higher multiple of 2
			 * to read at least 13 bytes even in case of 16 bit NAND
			 * devices
			 */
			if (chip->options & NAND_BUSWIDTH_16)
				len = roundup(len, 2);

			nand_read_oob_op(chip, page, off, oob + j, len);
			j += len;
		}

		memcpy(&ecc_code[i], oob, chip->ecc.bytes);
<<<<<<< HEAD
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);

		stat = chip->ecc.correct(mtd, p, &ecc_code[i], &ecc_calc[i]);
=======
		chip->ecc.calculate(chip, p, &ecc_calc[i]);

		stat = chip->ecc.correct(chip, p, &ecc_code[i], &ecc_calc[i]);
>>>>>>> upstream/android-13
		if (stat < 0) {
			mtd->ecc_stats.failed++;
		} else {
			mtd->ecc_stats.corrected += stat;
			max_bitflips = max_t(unsigned int, max_bitflips, stat);
		}
	}

	return max_bitflips;
}

/*
 * fsmc_bch8_correct_data
 * @mtd:	mtd info structure
 * @dat:	buffer of read data
 * @read_ecc:	ecc read from device spare area
 * @calc_ecc:	ecc calculated from read data
 *
 * calc_ecc is a 104 bit information containing maximum of 8 error
<<<<<<< HEAD
 * offset informations of 13 bits each in 512 bytes of read data.
 */
static int fsmc_bch8_correct_data(struct mtd_info *mtd, uint8_t *dat,
			     uint8_t *read_ecc, uint8_t *calc_ecc)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
	uint32_t err_idx[8];
	uint32_t num_err, i;
	uint32_t ecc1, ecc2, ecc3, ecc4;
=======
 * offset information of 13 bits each in 512 bytes of read data.
 */
static int fsmc_bch8_correct_data(struct nand_chip *chip, u8 *dat,
				  u8 *read_ecc, u8 *calc_ecc)
{
	struct fsmc_nand_data *host = nand_to_fsmc(chip);
	u32 err_idx[8];
	u32 num_err, i;
	u32 ecc1, ecc2, ecc3, ecc4;
>>>>>>> upstream/android-13

	num_err = (readl_relaxed(host->regs_va + STS) >> 10) & 0xF;

	/* no bit flipping */
	if (likely(num_err == 0))
		return 0;

	/* too many errors */
	if (unlikely(num_err > 8)) {
		/*
		 * This is a temporary erase check. A newly erased page read
		 * would result in an ecc error because the oob data is also
		 * erased to FF and the calculated ecc for an FF data is not
		 * FF..FF.
		 * This is a workaround to skip performing correction in case
		 * data is FF..FF
		 *
		 * Logic:
		 * For every page, each bit written as 0 is counted until these
		 * number of bits are greater than 8 (the maximum correction
		 * capability of FSMC for each 512 + 13 bytes)
		 */

		int bits_ecc = count_written_bits(read_ecc, chip->ecc.bytes, 8);
		int bits_data = count_written_bits(dat, chip->ecc.size, 8);

		if ((bits_ecc + bits_data) <= 8) {
			if (bits_data)
				memset(dat, 0xff, chip->ecc.size);
			return bits_data;
		}

		return -EBADMSG;
	}

	/*
	 * ------------------- calc_ecc[] bit wise -----------|--13 bits--|
	 * |---idx[7]--|--.....-----|---idx[2]--||---idx[1]--||---idx[0]--|
	 *
	 * calc_ecc is a 104 bit information containing maximum of 8 error
<<<<<<< HEAD
	 * offset informations of 13 bits each. calc_ecc is copied into a
	 * uint64_t array and error offset indexes are populated in err_idx
=======
	 * offset information of 13 bits each. calc_ecc is copied into a
	 * u64 array and error offset indexes are populated in err_idx
>>>>>>> upstream/android-13
	 * array
	 */
	ecc1 = readl_relaxed(host->regs_va + ECC1);
	ecc2 = readl_relaxed(host->regs_va + ECC2);
	ecc3 = readl_relaxed(host->regs_va + ECC3);
	ecc4 = readl_relaxed(host->regs_va + STS);

	err_idx[0] = (ecc1 >> 0) & 0x1FFF;
	err_idx[1] = (ecc1 >> 13) & 0x1FFF;
	err_idx[2] = (((ecc2 >> 0) & 0x7F) << 6) | ((ecc1 >> 26) & 0x3F);
	err_idx[3] = (ecc2 >> 7) & 0x1FFF;
	err_idx[4] = (((ecc3 >> 0) & 0x1) << 12) | ((ecc2 >> 20) & 0xFFF);
	err_idx[5] = (ecc3 >> 1) & 0x1FFF;
	err_idx[6] = (ecc3 >> 14) & 0x1FFF;
	err_idx[7] = (((ecc4 >> 16) & 0xFF) << 5) | ((ecc3 >> 27) & 0x1F);

	i = 0;
	while (num_err--) {
<<<<<<< HEAD
		change_bit(0, (unsigned long *)&err_idx[i]);
		change_bit(1, (unsigned long *)&err_idx[i]);

		if (err_idx[i] < chip->ecc.size * 8) {
			change_bit(err_idx[i], (unsigned long *)dat);
=======
		err_idx[i] ^= 3;

		if (err_idx[i] < chip->ecc.size * 8) {
			int err = err_idx[i];

			dat[err >> 3] ^= BIT(err & 7);
>>>>>>> upstream/android-13
			i++;
		}
	}
	return i;
}

static bool filter(struct dma_chan *chan, void *slave)
{
	chan->private = slave;
	return true;
}

static int fsmc_nand_probe_config_dt(struct platform_device *pdev,
				     struct fsmc_nand_data *host,
				     struct nand_chip *nand)
{
	struct device_node *np = pdev->dev.of_node;
	u32 val;
	int ret;

	nand->options = 0;

	if (!of_property_read_u32(np, "bank-width", &val)) {
		if (val == 2) {
			nand->options |= NAND_BUSWIDTH_16;
		} else if (val != 1) {
			dev_err(&pdev->dev, "invalid bank-width %u\n", val);
			return -EINVAL;
		}
	}

	if (of_get_property(np, "nand-skip-bbtscan", NULL))
		nand->options |= NAND_SKIP_BBTSCAN;

	host->dev_timings = devm_kzalloc(&pdev->dev,
<<<<<<< HEAD
				sizeof(*host->dev_timings), GFP_KERNEL);
	if (!host->dev_timings)
		return -ENOMEM;
	ret = of_property_read_u8_array(np, "timings", (u8 *)host->dev_timings,
						sizeof(*host->dev_timings));
=======
					 sizeof(*host->dev_timings),
					 GFP_KERNEL);
	if (!host->dev_timings)
		return -ENOMEM;

	ret = of_property_read_u8_array(np, "timings", (u8 *)host->dev_timings,
					sizeof(*host->dev_timings));
>>>>>>> upstream/android-13
	if (ret)
		host->dev_timings = NULL;

	/* Set default NAND bank to 0 */
	host->bank = 0;
	if (!of_property_read_u32(np, "bank", &val)) {
		if (val > 3) {
			dev_err(&pdev->dev, "invalid bank %u\n", val);
			return -EINVAL;
		}
		host->bank = val;
	}
	return 0;
}

static int fsmc_nand_attach_chip(struct nand_chip *nand)
{
	struct mtd_info *mtd = nand_to_mtd(nand);
<<<<<<< HEAD
	struct fsmc_nand_data *host = mtd_to_fsmc(mtd);
=======
	struct fsmc_nand_data *host = nand_to_fsmc(nand);

	if (nand->ecc.engine_type == NAND_ECC_ENGINE_TYPE_INVALID)
		nand->ecc.engine_type = NAND_ECC_ENGINE_TYPE_ON_HOST;

	if (!nand->ecc.size)
		nand->ecc.size = 512;

	if (AMBA_REV_BITS(host->pid) >= 8) {
		nand->ecc.read_page = fsmc_read_page_hwecc;
		nand->ecc.calculate = fsmc_read_hwecc_ecc4;
		nand->ecc.correct = fsmc_bch8_correct_data;
		nand->ecc.bytes = 13;
		nand->ecc.strength = 8;
	}
>>>>>>> upstream/android-13

	if (AMBA_REV_BITS(host->pid) >= 8) {
		switch (mtd->oobsize) {
		case 16:
		case 64:
		case 128:
		case 224:
		case 256:
			break;
		default:
			dev_warn(host->dev,
				 "No oob scheme defined for oobsize %d\n",
				 mtd->oobsize);
			return -EINVAL;
		}

		mtd_set_ooblayout(mtd, &fsmc_ecc4_ooblayout_ops);

		return 0;
	}

<<<<<<< HEAD
	switch (nand->ecc.mode) {
	case NAND_ECC_HW:
		dev_info(host->dev, "Using 1-bit HW ECC scheme\n");
		nand->ecc.calculate = fsmc_read_hwecc_ecc1;
		nand->ecc.correct = nand_correct_data;
		nand->ecc.bytes = 3;
		nand->ecc.strength = 1;
		break;

	case NAND_ECC_SOFT:
		if (nand->ecc.algo == NAND_ECC_BCH) {
=======
	switch (nand->ecc.engine_type) {
	case NAND_ECC_ENGINE_TYPE_ON_HOST:
		dev_info(host->dev, "Using 1-bit HW ECC scheme\n");
		nand->ecc.calculate = fsmc_read_hwecc_ecc1;
		nand->ecc.correct = fsmc_correct_ecc1;
		nand->ecc.hwctl = fsmc_enable_hwecc;
		nand->ecc.bytes = 3;
		nand->ecc.strength = 1;
		nand->ecc.options |= NAND_ECC_SOFT_HAMMING_SM_ORDER;
		break;

	case NAND_ECC_ENGINE_TYPE_SOFT:
		if (nand->ecc.algo == NAND_ECC_ALGO_BCH) {
>>>>>>> upstream/android-13
			dev_info(host->dev,
				 "Using 4-bit SW BCH ECC scheme\n");
			break;
		}
<<<<<<< HEAD

	case NAND_ECC_ON_DIE:
=======
		break;

	case NAND_ECC_ENGINE_TYPE_ON_DIE:
>>>>>>> upstream/android-13
		break;

	default:
		dev_err(host->dev, "Unsupported ECC mode!\n");
		return -ENOTSUPP;
	}

	/*
	 * Don't set layout for BCH4 SW ECC. This will be
<<<<<<< HEAD
	 * generated later in nand_bch_init() later.
	 */
	if (nand->ecc.mode == NAND_ECC_HW) {
=======
	 * generated later during BCH initialization.
	 */
	if (nand->ecc.engine_type == NAND_ECC_ENGINE_TYPE_ON_HOST) {
>>>>>>> upstream/android-13
		switch (mtd->oobsize) {
		case 16:
		case 64:
		case 128:
			mtd_set_ooblayout(mtd,
					  &fsmc_ecc1_ooblayout_ops);
			break;
		default:
			dev_warn(host->dev,
				 "No oob scheme defined for oobsize %d\n",
				 mtd->oobsize);
			return -EINVAL;
		}
	}

	return 0;
}

static const struct nand_controller_ops fsmc_nand_controller_ops = {
	.attach_chip = fsmc_nand_attach_chip,
<<<<<<< HEAD
};

=======
	.exec_op = fsmc_exec_op,
	.setup_interface = fsmc_setup_interface,
};

/**
 * fsmc_nand_disable() - Disables the NAND bank
 * @host: The instance to disable
 */
static void fsmc_nand_disable(struct fsmc_nand_data *host)
{
	u32 val;

	val = readl(host->regs_va + FSMC_PC);
	val &= ~FSMC_ENABLE;
	writel(val, host->regs_va + FSMC_PC);
}

>>>>>>> upstream/android-13
/*
 * fsmc_nand_probe - Probe function
 * @pdev:       platform device structure
 */
static int __init fsmc_nand_probe(struct platform_device *pdev)
{
	struct fsmc_nand_data *host;
	struct mtd_info *mtd;
	struct nand_chip *nand;
	struct resource *res;
	void __iomem *base;
	dma_cap_mask_t mask;
	int ret = 0;
	u32 pid;
	int i;

	/* Allocate memory for the device structure (and zero it) */
	host = devm_kzalloc(&pdev->dev, sizeof(*host), GFP_KERNEL);
	if (!host)
		return -ENOMEM;

	nand = &host->nand;

	ret = fsmc_nand_probe_config_dt(pdev, host, nand);
	if (ret)
		return ret;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "nand_data");
	host->data_va = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(host->data_va))
		return PTR_ERR(host->data_va);

	host->data_pa = (dma_addr_t)res->start;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "nand_addr");
	host->addr_va = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(host->addr_va))
		return PTR_ERR(host->addr_va);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "nand_cmd");
	host->cmd_va = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(host->cmd_va))
		return PTR_ERR(host->cmd_va);

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "fsmc_regs");
	base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base))
		return PTR_ERR(base);

	host->regs_va = base + FSMC_NOR_REG_SIZE +
		(host->bank * FSMC_NAND_BANK_SZ);

	host->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(host->clk)) {
		dev_err(&pdev->dev, "failed to fetch block clock\n");
		return PTR_ERR(host->clk);
	}

	ret = clk_prepare_enable(host->clk);
	if (ret)
		return ret;

	/*
	 * This device ID is actually a common AMBA ID as used on the
	 * AMBA PrimeCell bus. However it is not a PrimeCell.
	 */
	for (pid = 0, i = 0; i < 4; i++)
<<<<<<< HEAD
		pid |= (readl(base + resource_size(res) - 0x20 + 4 * i) & 255) << (i * 8);
	host->pid = pid;
	dev_info(&pdev->dev, "FSMC device partno %03x, manufacturer %02x, "
		 "revision %02x, config %02x\n",
=======
		pid |= (readl(base + resource_size(res) - 0x20 + 4 * i) &
			255) << (i * 8);

	host->pid = pid;

	dev_info(&pdev->dev,
		 "FSMC device partno %03x, manufacturer %02x, revision %02x, config %02x\n",
>>>>>>> upstream/android-13
		 AMBA_PART_BITS(pid), AMBA_MANF_BITS(pid),
		 AMBA_REV_BITS(pid), AMBA_CONFIG_BITS(pid));

	host->dev = &pdev->dev;

	if (host->mode == USE_DMA_ACCESS)
		init_completion(&host->dma_access_complete);

	/* Link all private pointers */
	mtd = nand_to_mtd(&host->nand);
<<<<<<< HEAD
	nand_set_controller_data(nand, host);
	nand_set_flash_node(nand, pdev->dev.of_node);

	mtd->dev.parent = &pdev->dev;
	nand->exec_op = fsmc_exec_op;
	nand->select_chip = fsmc_select_chip;
	nand->chip_delay = 30;

	/*
	 * Setup default ECC mode. nand_dt_init() called from nand_scan_ident()
	 * can overwrite this value if the DT provides a different value.
	 */
	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.hwctl = fsmc_enable_hwecc;
	nand->ecc.size = 512;
=======
	nand_set_flash_node(nand, pdev->dev.of_node);

	mtd->dev.parent = &pdev->dev;

>>>>>>> upstream/android-13
	nand->badblockbits = 7;

	if (host->mode == USE_DMA_ACCESS) {
		dma_cap_zero(mask);
		dma_cap_set(DMA_MEMCPY, mask);
		host->read_dma_chan = dma_request_channel(mask, filter, NULL);
		if (!host->read_dma_chan) {
			dev_err(&pdev->dev, "Unable to get read dma channel\n");
			ret = -ENODEV;
			goto disable_clk;
		}
		host->write_dma_chan = dma_request_channel(mask, filter, NULL);
		if (!host->write_dma_chan) {
			dev_err(&pdev->dev, "Unable to get write dma channel\n");
			ret = -ENODEV;
			goto release_dma_read_chan;
		}
	}

<<<<<<< HEAD
	if (host->dev_timings)
		fsmc_nand_setup(host, host->dev_timings);
	else
		nand->setup_data_interface = fsmc_setup_data_interface;

	if (AMBA_REV_BITS(host->pid) >= 8) {
		nand->ecc.read_page = fsmc_read_page_hwecc;
		nand->ecc.calculate = fsmc_read_hwecc_ecc4;
		nand->ecc.correct = fsmc_bch8_correct_data;
		nand->ecc.bytes = 13;
		nand->ecc.strength = 8;
	}

	/*
	 * Scan to find existence of the device
	 */
	nand->dummy_controller.ops = &fsmc_nand_controller_ops;
=======
	if (host->dev_timings) {
		fsmc_nand_setup(host, host->dev_timings);
		nand->options |= NAND_KEEP_TIMINGS;
	}

	nand_controller_init(&host->base);
	host->base.ops = &fsmc_nand_controller_ops;
	nand->controller = &host->base;

	/*
	 * Scan to find existence of the device
	 */
>>>>>>> upstream/android-13
	ret = nand_scan(nand, 1);
	if (ret)
		goto release_dma_write_chan;

	mtd->name = "nand";
	ret = mtd_device_register(mtd, NULL, 0);
	if (ret)
		goto cleanup_nand;

	platform_set_drvdata(pdev, host);
	dev_info(&pdev->dev, "FSMC NAND driver registration successful\n");

	return 0;

cleanup_nand:
	nand_cleanup(nand);
release_dma_write_chan:
	if (host->mode == USE_DMA_ACCESS)
		dma_release_channel(host->write_dma_chan);
release_dma_read_chan:
	if (host->mode == USE_DMA_ACCESS)
		dma_release_channel(host->read_dma_chan);
disable_clk:
<<<<<<< HEAD
=======
	fsmc_nand_disable(host);
>>>>>>> upstream/android-13
	clk_disable_unprepare(host->clk);

	return ret;
}

/*
 * Clean up routine
 */
static int fsmc_nand_remove(struct platform_device *pdev)
{
	struct fsmc_nand_data *host = platform_get_drvdata(pdev);

	if (host) {
<<<<<<< HEAD
		nand_release(&host->nand);
=======
		struct nand_chip *chip = &host->nand;
		int ret;

		ret = mtd_device_unregister(nand_to_mtd(chip));
		WARN_ON(ret);
		nand_cleanup(chip);
		fsmc_nand_disable(host);
>>>>>>> upstream/android-13

		if (host->mode == USE_DMA_ACCESS) {
			dma_release_channel(host->write_dma_chan);
			dma_release_channel(host->read_dma_chan);
		}
		clk_disable_unprepare(host->clk);
	}

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int fsmc_nand_suspend(struct device *dev)
{
	struct fsmc_nand_data *host = dev_get_drvdata(dev);
<<<<<<< HEAD
	if (host)
		clk_disable_unprepare(host->clk);
=======

	if (host)
		clk_disable_unprepare(host->clk);

>>>>>>> upstream/android-13
	return 0;
}

static int fsmc_nand_resume(struct device *dev)
{
	struct fsmc_nand_data *host = dev_get_drvdata(dev);
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	if (host) {
		clk_prepare_enable(host->clk);
		if (host->dev_timings)
			fsmc_nand_setup(host, host->dev_timings);
<<<<<<< HEAD
	}
=======
		nand_reset(&host->nand, 0);
	}

>>>>>>> upstream/android-13
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(fsmc_nand_pm_ops, fsmc_nand_suspend, fsmc_nand_resume);

static const struct of_device_id fsmc_nand_id_table[] = {
	{ .compatible = "st,spear600-fsmc-nand" },
	{ .compatible = "stericsson,fsmc-nand" },
	{}
};
MODULE_DEVICE_TABLE(of, fsmc_nand_id_table);

static struct platform_driver fsmc_nand_driver = {
	.remove = fsmc_nand_remove,
	.driver = {
		.name = "fsmc-nand",
		.of_match_table = fsmc_nand_id_table,
		.pm = &fsmc_nand_pm_ops,
	},
};

module_platform_driver_probe(fsmc_nand_driver, fsmc_nand_probe);

<<<<<<< HEAD
MODULE_LICENSE("GPL");
=======
MODULE_LICENSE("GPL v2");
>>>>>>> upstream/android-13
MODULE_AUTHOR("Vipin Kumar <vipin.kumar@st.com>, Ashish Priyadarshi");
MODULE_DESCRIPTION("NAND driver for SPEAr Platforms");
