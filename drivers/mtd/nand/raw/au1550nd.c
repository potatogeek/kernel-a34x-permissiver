<<<<<<< HEAD
/*
 *  Copyright (C) 2004 Embedded Edge, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2004 Embedded Edge, LLC
 */

#include <linux/delay.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/partitions.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-au1x00/au1550nd.h>


struct au1550nd_ctx {
<<<<<<< HEAD
=======
	struct nand_controller controller;
>>>>>>> upstream/android-13
	struct nand_chip chip;

	int cs;
	void __iomem *base;
<<<<<<< HEAD
	void (*write_byte)(struct mtd_info *, u_char);
};

/**
 * au_read_byte -  read one byte from the chip
 * @mtd:	MTD device structure
 *
 * read function for 8bit buswidth
 */
static u_char au_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	u_char ret = readb(this->IO_ADDR_R);
	wmb(); /* drain writebuffer */
	return ret;
}

/**
 * au_write_byte -  write one byte to the chip
 * @mtd:	MTD device structure
 * @byte:	pointer to data byte to write
 *
 * write function for 8it buswidth
 */
static void au_write_byte(struct mtd_info *mtd, u_char byte)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	writeb(byte, this->IO_ADDR_W);
	wmb(); /* drain writebuffer */
}

/**
 * au_read_byte16 -  read one byte endianness aware from the chip
 * @mtd:	MTD device structure
 *
 * read function for 16bit buswidth with endianness conversion
 */
static u_char au_read_byte16(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	u_char ret = (u_char) cpu_to_le16(readw(this->IO_ADDR_R));
	wmb(); /* drain writebuffer */
	return ret;
}

/**
 * au_write_byte16 -  write one byte endianness aware to the chip
 * @mtd:	MTD device structure
 * @byte:	pointer to data byte to write
 *
 * write function for 16bit buswidth with endianness conversion
 */
static void au_write_byte16(struct mtd_info *mtd, u_char byte)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	writew(le16_to_cpu((u16) byte), this->IO_ADDR_W);
	wmb(); /* drain writebuffer */
}

/**
 * au_read_word -  read one word from the chip
 * @mtd:	MTD device structure
 *
 * read function for 16bit buswidth without endianness conversion
 */
static u16 au_read_word(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	u16 ret = readw(this->IO_ADDR_R);
	wmb(); /* drain writebuffer */
	return ret;
=======
};

static struct au1550nd_ctx *chip_to_au_ctx(struct nand_chip *this)
{
	return container_of(this, struct au1550nd_ctx, chip);
>>>>>>> upstream/android-13
}

/**
 * au_write_buf -  write buffer to chip
<<<<<<< HEAD
 * @mtd:	MTD device structure
=======
 * @this:	NAND chip object
>>>>>>> upstream/android-13
 * @buf:	data buffer
 * @len:	number of bytes to write
 *
 * write function for 8bit buswidth
 */
<<<<<<< HEAD
static void au_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd_to_nand(mtd);

	for (i = 0; i < len; i++) {
		writeb(buf[i], this->IO_ADDR_W);
=======
static void au_write_buf(struct nand_chip *this, const void *buf,
			 unsigned int len)
{
	struct au1550nd_ctx *ctx = chip_to_au_ctx(this);
	const u8 *p = buf;
	int i;

	for (i = 0; i < len; i++) {
		writeb(p[i], ctx->base + MEM_STNAND_DATA);
>>>>>>> upstream/android-13
		wmb(); /* drain writebuffer */
	}
}

/**
 * au_read_buf -  read chip data into buffer
<<<<<<< HEAD
 * @mtd:	MTD device structure
=======
 * @this:	NAND chip object
>>>>>>> upstream/android-13
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 *
 * read function for 8bit buswidth
 */
<<<<<<< HEAD
static void au_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd_to_nand(mtd);

	for (i = 0; i < len; i++) {
		buf[i] = readb(this->IO_ADDR_R);
=======
static void au_read_buf(struct nand_chip *this, void *buf,
			unsigned int len)
{
	struct au1550nd_ctx *ctx = chip_to_au_ctx(this);
	u8 *p = buf;
	int i;

	for (i = 0; i < len; i++) {
		p[i] = readb(ctx->base + MEM_STNAND_DATA);
>>>>>>> upstream/android-13
		wmb(); /* drain writebuffer */
	}
}

/**
 * au_write_buf16 -  write buffer to chip
<<<<<<< HEAD
 * @mtd:	MTD device structure
=======
 * @this:	NAND chip object
>>>>>>> upstream/android-13
 * @buf:	data buffer
 * @len:	number of bytes to write
 *
 * write function for 16bit buswidth
 */
<<<<<<< HEAD
static void au_write_buf16(struct mtd_info *mtd, const u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd_to_nand(mtd);
	u16 *p = (u16 *) buf;
	len >>= 1;

	for (i = 0; i < len; i++) {
		writew(p[i], this->IO_ADDR_W);
		wmb(); /* drain writebuffer */
	}

=======
static void au_write_buf16(struct nand_chip *this, const void *buf,
			   unsigned int len)
{
	struct au1550nd_ctx *ctx = chip_to_au_ctx(this);
	const u16 *p = buf;
	unsigned int i;

	len >>= 1;
	for (i = 0; i < len; i++) {
		writew(p[i], ctx->base + MEM_STNAND_DATA);
		wmb(); /* drain writebuffer */
	}
>>>>>>> upstream/android-13
}

/**
 * au_read_buf16 -  read chip data into buffer
<<<<<<< HEAD
 * @mtd:	MTD device structure
=======
 * @this:	NAND chip object
>>>>>>> upstream/android-13
 * @buf:	buffer to store date
 * @len:	number of bytes to read
 *
 * read function for 16bit buswidth
 */
<<<<<<< HEAD
static void au_read_buf16(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd_to_nand(mtd);
	u16 *p = (u16 *) buf;
	len >>= 1;

	for (i = 0; i < len; i++) {
		p[i] = readw(this->IO_ADDR_R);
=======
static void au_read_buf16(struct nand_chip *this, void *buf, unsigned int len)
{
	struct au1550nd_ctx *ctx = chip_to_au_ctx(this);
	unsigned int i;
	u16 *p = buf;

	len >>= 1;
	for (i = 0; i < len; i++) {
		p[i] = readw(ctx->base + MEM_STNAND_DATA);
>>>>>>> upstream/android-13
		wmb(); /* drain writebuffer */
	}
}

<<<<<<< HEAD
/* Select the chip by setting nCE to low */
#define NAND_CTL_SETNCE		1
/* Deselect the chip by setting nCE to high */
#define NAND_CTL_CLRNCE		2
/* Select the command latch by setting CLE to high */
#define NAND_CTL_SETCLE		3
/* Deselect the command latch by setting CLE to low */
#define NAND_CTL_CLRCLE		4
/* Select the address latch by setting ALE to high */
#define NAND_CTL_SETALE		5
/* Deselect the address latch by setting ALE to low */
#define NAND_CTL_CLRALE		6

static void au1550_hwcontrol(struct mtd_info *mtd, int cmd)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	struct au1550nd_ctx *ctx = container_of(this, struct au1550nd_ctx,
						chip);

	switch (cmd) {

	case NAND_CTL_SETCLE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_CMD;
		break;

	case NAND_CTL_CLRCLE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_DATA;
		break;

	case NAND_CTL_SETALE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_ADDR;
		break;

	case NAND_CTL_CLRALE:
		this->IO_ADDR_W = ctx->base + MEM_STNAND_DATA;
		/* FIXME: Nobody knows why this is necessary,
		 * but it works only that way */
		udelay(1);
		break;

	case NAND_CTL_SETNCE:
		/* assert (force assert) chip enable */
		alchemy_wrsmem((1 << (4 + ctx->cs)), AU1000_MEM_STNDCTL);
		break;

	case NAND_CTL_CLRNCE:
		/* deassert chip enable */
		alchemy_wrsmem(0, AU1000_MEM_STNDCTL);
		break;
	}

	this->IO_ADDR_R = this->IO_ADDR_W;

	wmb(); /* Drain the writebuffer */
}

int au1550_device_ready(struct mtd_info *mtd)
{
	return (alchemy_rdsmem(AU1000_MEM_STSTAT) & 0x1) ? 1 : 0;
}

/**
 * au1550_select_chip - control -CE line
 *	Forbid driving -CE manually permitting the NAND controller to do this.
 *	Keeping -CE asserted during the whole sector reads interferes with the
 *	NOR flash and PCMCIA drivers as it causes contention on the static bus.
 *	We only have to hold -CE low for the NAND read commands since the flash
 *	chip needs it to be asserted during chip not ready time but the NAND
 *	controller keeps it released.
 *
 * @mtd:	MTD device structure
 * @chip:	chipnumber to select, -1 for deselect
 */
static void au1550_select_chip(struct mtd_info *mtd, int chip)
{
}

/**
 * au1550_command - Send command to NAND device
 * @mtd:	MTD device structure
 * @command:	the command to be sent
 * @column:	the column address for this command, -1 if none
 * @page_addr:	the page address for this command, -1 if none
 */
static void au1550_command(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
	struct nand_chip *this = mtd_to_nand(mtd);
	struct au1550nd_ctx *ctx = container_of(this, struct au1550nd_ctx,
						chip);
	int ce_override = 0, i;
	unsigned long flags = 0;

	/* Begin command latch cycle */
	au1550_hwcontrol(mtd, NAND_CTL_SETCLE);
	/*
	 * Write out the command to the device.
	 */
	if (command == NAND_CMD_SEQIN) {
		int readcmd;

		if (column >= mtd->writesize) {
			/* OOB area */
			column -= mtd->writesize;
			readcmd = NAND_CMD_READOOB;
		} else if (column < 256) {
			/* First 256 bytes --> READ0 */
			readcmd = NAND_CMD_READ0;
		} else {
			column -= 256;
			readcmd = NAND_CMD_READ1;
		}
		ctx->write_byte(mtd, readcmd);
	}
	ctx->write_byte(mtd, command);

	/* Set ALE and clear CLE to start address cycle */
	au1550_hwcontrol(mtd, NAND_CTL_CLRCLE);

	if (column != -1 || page_addr != -1) {
		au1550_hwcontrol(mtd, NAND_CTL_SETALE);

		/* Serially input address */
		if (column != -1) {
			/* Adjust columns for 16 bit buswidth */
			if (this->options & NAND_BUSWIDTH_16 &&
					!nand_opcode_8bits(command))
				column >>= 1;
			ctx->write_byte(mtd, column);
		}
		if (page_addr != -1) {
			ctx->write_byte(mtd, (u8)(page_addr & 0xff));

			if (command == NAND_CMD_READ0 ||
			    command == NAND_CMD_READ1 ||
			    command == NAND_CMD_READOOB) {
				/*
				 * NAND controller will release -CE after
				 * the last address byte is written, so we'll
				 * have to forcibly assert it. No interrupts
				 * are allowed while we do this as we don't
				 * want the NOR flash or PCMCIA drivers to
				 * steal our precious bytes of data...
				 */
				ce_override = 1;
				local_irq_save(flags);
				au1550_hwcontrol(mtd, NAND_CTL_SETNCE);
			}

			ctx->write_byte(mtd, (u8)(page_addr >> 8));

			if (this->options & NAND_ROW_ADDR_3)
				ctx->write_byte(mtd,
						((page_addr >> 16) & 0x0f));
		}
		/* Latch in address */
		au1550_hwcontrol(mtd, NAND_CTL_CLRALE);
	}

	/*
	 * Program and erase have their own busy handlers.
	 * Status and sequential in need no delay.
	 */
	switch (command) {

	case NAND_CMD_PAGEPROG:
	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
	case NAND_CMD_SEQIN:
	case NAND_CMD_STATUS:
		return;

	case NAND_CMD_RESET:
		break;

	case NAND_CMD_READ0:
	case NAND_CMD_READ1:
	case NAND_CMD_READOOB:
		/* Check if we're really driving -CE low (just in case) */
		if (unlikely(!ce_override))
			break;

		/* Apply a short delay always to ensure that we do wait tWB. */
		ndelay(100);
		/* Wait for a chip to become ready... */
		for (i = this->chip_delay; !this->dev_ready(mtd) && i > 0; --i)
			udelay(1);

		/* Release -CE and re-enable interrupts. */
		au1550_hwcontrol(mtd, NAND_CTL_CLRNCE);
		local_irq_restore(flags);
		return;
	}
	/* Apply this short delay always to ensure that we do wait tWB. */
	ndelay(100);

	while(!this->dev_ready(mtd));
}

=======
>>>>>>> upstream/android-13
static int find_nand_cs(unsigned long nand_base)
{
	void __iomem *base =
			(void __iomem *)KSEG1ADDR(AU1000_STATIC_MEM_PHYS_ADDR);
	unsigned long addr, staddr, start, mask, end;
	int i;

	for (i = 0; i < 4; i++) {
		addr = 0x1000 + (i * 0x10);			/* CSx */
		staddr = __raw_readl(base + addr + 0x08);	/* STADDRx */
		/* figure out the decoded range of this CS */
		start = (staddr << 4) & 0xfffc0000;
		mask = (staddr << 18) & 0xfffc0000;
		end = (start | (start - 1)) & ~(start ^ mask);
		if ((nand_base >= start) && (nand_base < end))
			return i;
	}

	return -ENODEV;
}

<<<<<<< HEAD
=======
static int au1550nd_waitrdy(struct nand_chip *this, unsigned int timeout_ms)
{
	unsigned long timeout_jiffies = jiffies;

	timeout_jiffies += msecs_to_jiffies(timeout_ms) + 1;
	do {
		if (alchemy_rdsmem(AU1000_MEM_STSTAT) & 0x1)
			return 0;

		usleep_range(10, 100);
	} while (time_before(jiffies, timeout_jiffies));

	return -ETIMEDOUT;
}

static int au1550nd_exec_instr(struct nand_chip *this,
			       const struct nand_op_instr *instr)
{
	struct au1550nd_ctx *ctx = chip_to_au_ctx(this);
	unsigned int i;
	int ret = 0;

	switch (instr->type) {
	case NAND_OP_CMD_INSTR:
		writeb(instr->ctx.cmd.opcode,
		       ctx->base + MEM_STNAND_CMD);
		/* Drain the writebuffer */
		wmb();
		break;

	case NAND_OP_ADDR_INSTR:
		for (i = 0; i < instr->ctx.addr.naddrs; i++) {
			writeb(instr->ctx.addr.addrs[i],
			       ctx->base + MEM_STNAND_ADDR);
			/* Drain the writebuffer */
			wmb();
		}
		break;

	case NAND_OP_DATA_IN_INSTR:
		if ((this->options & NAND_BUSWIDTH_16) &&
		    !instr->ctx.data.force_8bit)
			au_read_buf16(this, instr->ctx.data.buf.in,
				      instr->ctx.data.len);
		else
			au_read_buf(this, instr->ctx.data.buf.in,
				    instr->ctx.data.len);
		break;

	case NAND_OP_DATA_OUT_INSTR:
		if ((this->options & NAND_BUSWIDTH_16) &&
		    !instr->ctx.data.force_8bit)
			au_write_buf16(this, instr->ctx.data.buf.out,
				       instr->ctx.data.len);
		else
			au_write_buf(this, instr->ctx.data.buf.out,
				     instr->ctx.data.len);
		break;

	case NAND_OP_WAITRDY_INSTR:
		ret = au1550nd_waitrdy(this, instr->ctx.waitrdy.timeout_ms);
		break;
	default:
		return -EINVAL;
	}

	if (instr->delay_ns)
		ndelay(instr->delay_ns);

	return ret;
}

static int au1550nd_exec_op(struct nand_chip *this,
			    const struct nand_operation *op,
			    bool check_only)
{
	struct au1550nd_ctx *ctx = chip_to_au_ctx(this);
	unsigned int i;
	int ret;

	if (check_only)
		return 0;

	/* assert (force assert) chip enable */
	alchemy_wrsmem((1 << (4 + ctx->cs)), AU1000_MEM_STNDCTL);
	/* Drain the writebuffer */
	wmb();

	for (i = 0; i < op->ninstrs; i++) {
		ret = au1550nd_exec_instr(this, &op->instrs[i]);
		if (ret)
			break;
	}

	/* deassert chip enable */
	alchemy_wrsmem(0, AU1000_MEM_STNDCTL);
	/* Drain the writebuffer */
	wmb();

	return ret;
}

static int au1550nd_attach_chip(struct nand_chip *chip)
{
	if (chip->ecc.engine_type == NAND_ECC_ENGINE_TYPE_SOFT &&
	    chip->ecc.algo == NAND_ECC_ALGO_UNKNOWN)
		chip->ecc.algo = NAND_ECC_ALGO_HAMMING;

	return 0;
}

static const struct nand_controller_ops au1550nd_ops = {
	.exec_op = au1550nd_exec_op,
	.attach_chip = au1550nd_attach_chip,
};

>>>>>>> upstream/android-13
static int au1550nd_probe(struct platform_device *pdev)
{
	struct au1550nd_platdata *pd;
	struct au1550nd_ctx *ctx;
	struct nand_chip *this;
	struct mtd_info *mtd;
	struct resource *r;
	int ret, cs;

	pd = dev_get_platdata(&pdev->dev);
	if (!pd) {
		dev_err(&pdev->dev, "missing platform data\n");
		return -ENODEV;
	}

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r) {
		dev_err(&pdev->dev, "no NAND memory resource\n");
		ret = -ENODEV;
		goto out1;
	}
	if (request_mem_region(r->start, resource_size(r), "au1550-nand")) {
		dev_err(&pdev->dev, "cannot claim NAND memory area\n");
		ret = -ENOMEM;
		goto out1;
	}

<<<<<<< HEAD
	ctx->base = ioremap_nocache(r->start, 0x1000);
=======
	ctx->base = ioremap(r->start, 0x1000);
>>>>>>> upstream/android-13
	if (!ctx->base) {
		dev_err(&pdev->dev, "cannot remap NAND memory area\n");
		ret = -ENODEV;
		goto out2;
	}

	this = &ctx->chip;
	mtd = nand_to_mtd(this);
	mtd->dev.parent = &pdev->dev;

	/* figure out which CS# r->start belongs to */
	cs = find_nand_cs(r->start);
	if (cs < 0) {
		dev_err(&pdev->dev, "cannot detect NAND chipselect\n");
		ret = -ENODEV;
		goto out3;
	}
	ctx->cs = cs;

<<<<<<< HEAD
	this->dev_ready = au1550_device_ready;
	this->select_chip = au1550_select_chip;
	this->cmdfunc = au1550_command;

	/* 30 us command delay time */
	this->chip_delay = 30;
	this->ecc.mode = NAND_ECC_SOFT;
	this->ecc.algo = NAND_ECC_HAMMING;
=======
	nand_controller_init(&ctx->controller);
	ctx->controller.ops = &au1550nd_ops;
	this->controller = &ctx->controller;
>>>>>>> upstream/android-13

	if (pd->devwidth)
		this->options |= NAND_BUSWIDTH_16;

<<<<<<< HEAD
	this->read_byte = (pd->devwidth) ? au_read_byte16 : au_read_byte;
	ctx->write_byte = (pd->devwidth) ? au_write_byte16 : au_write_byte;
	this->read_word = au_read_word;
	this->write_buf = (pd->devwidth) ? au_write_buf16 : au_write_buf;
	this->read_buf = (pd->devwidth) ? au_read_buf16 : au_read_buf;
=======
	/*
	 * This driver assumes that the default ECC engine should be TYPE_SOFT.
	 * Set ->engine_type before registering the NAND devices in order to
	 * provide a driver specific default value.
	 */
	this->ecc.engine_type = NAND_ECC_ENGINE_TYPE_SOFT;
>>>>>>> upstream/android-13

	ret = nand_scan(this, 1);
	if (ret) {
		dev_err(&pdev->dev, "NAND scan failed with %d\n", ret);
		goto out3;
	}

	mtd_device_register(mtd, pd->parts, pd->num_parts);

	platform_set_drvdata(pdev, ctx);

	return 0;

out3:
	iounmap(ctx->base);
out2:
	release_mem_region(r->start, resource_size(r));
out1:
	kfree(ctx);
	return ret;
}

static int au1550nd_remove(struct platform_device *pdev)
{
	struct au1550nd_ctx *ctx = platform_get_drvdata(pdev);
	struct resource *r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD

	nand_release(&ctx->chip);
=======
	struct nand_chip *chip = &ctx->chip;
	int ret;

	ret = mtd_device_unregister(nand_to_mtd(chip));
	WARN_ON(ret);
	nand_cleanup(chip);
>>>>>>> upstream/android-13
	iounmap(ctx->base);
	release_mem_region(r->start, 0x1000);
	kfree(ctx);
	return 0;
}

static struct platform_driver au1550nd_driver = {
	.driver = {
		.name	= "au1550-nand",
	},
	.probe		= au1550nd_probe,
	.remove		= au1550nd_remove,
};

module_platform_driver(au1550nd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Edge, LLC");
MODULE_DESCRIPTION("Board-specific glue layer for NAND flash on Pb1550 board");
