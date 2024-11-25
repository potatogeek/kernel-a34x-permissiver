<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 * Zoran zr36057/zr36067 PCI controller driver, for the
 * Pinnacle/Miro DC10/DC10+/DC30/DC30+, Iomega Buz, Linux
 * Media Labs LML33/LML33R10.
 *
 * This part handles device access (PCI/I2C/codec/...)
 *
 * Copyright (C) 2000 Serguei Miridonov <mirsev@cicese.mx>
<<<<<<< HEAD
 *
 * Currently maintained by:
 *   Ronald Bultje    <rbultje@ronald.bitfreak.net>
 *   Laurent Pinchart <laurent.pinchart@skynet.be>
 *   Mailinglist      <mjpeg-users@lists.sf.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
>>>>>>> upstream/android-13
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/vmalloc.h>
#include <linux/ktime.h>
#include <linux/sched/signal.h>

#include <linux/interrupt.h>
#include <linux/proc_fs.h>
=======

#include <linux/interrupt.h>
>>>>>>> upstream/android-13
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/videodev2.h>
#include <media/v4l2-common.h>
#include <linux/spinlock.h>
<<<<<<< HEAD
#include <linux/sem.h>
=======
>>>>>>> upstream/android-13

#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/wait.h>
<<<<<<< HEAD

#include <asm/byteorder.h>
#include <asm/io.h>
=======
#include <linux/dma-mapping.h>

#include <linux/io.h>
>>>>>>> upstream/android-13

#include "videocodec.h"
#include "zoran.h"
#include "zoran_device.h"
#include "zoran_card.h"

<<<<<<< HEAD
#define IRQ_MASK ( ZR36057_ISR_GIRQ0 | \
		   ZR36057_ISR_GIRQ1 | \
		   ZR36057_ISR_JPEGRepIRQ )
=======
#define IRQ_MASK (ZR36057_ISR_GIRQ0 | \
		  ZR36057_ISR_GIRQ1 | \
		  ZR36057_ISR_JPEG_REP_IRQ)
>>>>>>> upstream/android-13

static bool lml33dpath;		/* default = 0
				 * 1 will use digital path in capture
				 * mode instead of analog. It can be
				 * used for picture adjustments using
				 * tool like xawtv while watching image
				 * on TV monitor connected to the output.
				 * However, due to absence of 75 Ohm
				 * load on Bt819 input, there will be
<<<<<<< HEAD
				 * some image imperfections */

module_param(lml33dpath, bool, 0644);
MODULE_PARM_DESC(lml33dpath,
		 "Use digital path capture mode (on LML33 cards)");

static void
zr36057_init_vfe (struct zoran *zr);
=======
				 * some image imperfections
				 */

module_param(lml33dpath, bool, 0644);
MODULE_PARM_DESC(lml33dpath, "Use digital path capture mode (on LML33 cards)");

int zr_set_buf(struct zoran *zr);
/*
 * initialize video front end
 */
static void zr36057_init_vfe(struct zoran *zr)
{
	u32 reg;

	reg = btread(ZR36057_VFESPFR);
	reg |= ZR36057_VFESPFR_LITTLE_ENDIAN;
	reg &= ~ZR36057_VFESPFR_VCLK_POL;
	reg |= ZR36057_VFESPFR_EXT_FL;
	reg |= ZR36057_VFESPFR_TOP_FIELD;
	btwrite(reg, ZR36057_VFESPFR);
	reg = btread(ZR36057_VDCR);
	if (pci_pci_problems & PCIPCI_TRITON)
		// || zr->revision < 1) // Revision 1 has also Triton support
		reg &= ~ZR36057_VDCR_TRITON;
	else
		reg |= ZR36057_VDCR_TRITON;
	btwrite(reg, ZR36057_VDCR);
}
>>>>>>> upstream/android-13

/*
 * General Purpose I/O and Guest bus access
 */

/*
 * This is a bit tricky. When a board lacks a GPIO function, the corresponding
 * GPIO bit number in the card_info structure is set to 0.
 */

<<<<<<< HEAD
void
GPIO (struct zoran *zr,
      int           bit,
      unsigned int  value)
=======
void GPIO(struct zoran *zr, int bit, unsigned int value)
>>>>>>> upstream/android-13
{
	u32 reg;
	u32 mask;

	/* Make sure the bit number is legal
	 * A bit number of -1 (lacking) gives a mask of 0,
<<<<<<< HEAD
	 * making it harmless */
	mask = (1 << (24 + bit)) & 0xff000000;
	reg = btread(ZR36057_GPPGCR1) & ~mask;
	if (value) {
		reg |= mask;
	}
=======
	 * making it harmless
	 */
	mask = (1 << (24 + bit)) & 0xff000000;
	reg = btread(ZR36057_GPPGCR1) & ~mask;
	if (value)
		reg |= mask;

>>>>>>> upstream/android-13
	btwrite(reg, ZR36057_GPPGCR1);
	udelay(1);
}

/*
 * Wait til post office is no longer busy
 */

<<<<<<< HEAD
int
post_office_wait (struct zoran *zr)
{
	u32 por;

//      while (((por = btread(ZR36057_POR)) & (ZR36057_POR_POPen | ZR36057_POR_POTime)) == ZR36057_POR_POPen) {
	while ((por = btread(ZR36057_POR)) & ZR36057_POR_POPen) {
		/* wait for something to happen */
	}
	if ((por & ZR36057_POR_POTime) && !zr->card.gws_not_connected) {
		/* In LML33/BUZ \GWS line is not connected, so it has always timeout set */
		dprintk(1, KERN_INFO "%s: pop timeout %08x\n", ZR_DEVNAME(zr),
			por);
=======
int post_office_wait(struct zoran *zr)
{
	u32 por;

//      while (((por = btread(ZR36057_POR)) & (ZR36057_POR_PO_PEN | ZR36057_POR_PO_TIME)) == ZR36057_POR_PO_PEN) {
	while ((por = btread(ZR36057_POR)) & ZR36057_POR_PO_PEN) {
		/* wait for something to happen */
		/* TODO add timeout */
	}
	if ((por & ZR36057_POR_PO_TIME) && !zr->card.gws_not_connected) {
		/* In LML33/BUZ \GWS line is not connected, so it has always timeout set */
		pci_info(zr->pci_dev, "pop timeout %08x\n", por);
>>>>>>> upstream/android-13
		return -1;
	}

	return 0;
}

<<<<<<< HEAD
int
post_office_write (struct zoran *zr,
		   unsigned int  guest,
		   unsigned int  reg,
		   unsigned int  value)
=======
int post_office_write(struct zoran *zr, unsigned int guest,
		      unsigned int reg, unsigned int value)
>>>>>>> upstream/android-13
{
	u32 por;

	por =
<<<<<<< HEAD
	    ZR36057_POR_PODir | ZR36057_POR_POTime | ((guest & 7) << 20) |
=======
	    ZR36057_POR_PO_DIR | ZR36057_POR_PO_TIME | ((guest & 7) << 20) |
>>>>>>> upstream/android-13
	    ((reg & 7) << 16) | (value & 0xFF);
	btwrite(por, ZR36057_POR);

	return post_office_wait(zr);
}

<<<<<<< HEAD
int
post_office_read (struct zoran *zr,
		  unsigned int  guest,
		  unsigned int  reg)
{
	u32 por;

	por = ZR36057_POR_POTime | ((guest & 7) << 20) | ((reg & 7) << 16);
	btwrite(por, ZR36057_POR);
	if (post_office_wait(zr) < 0) {
		return -1;
	}
=======
int post_office_read(struct zoran *zr, unsigned int guest, unsigned int reg)
{
	u32 por;

	por = ZR36057_POR_PO_TIME | ((guest & 7) << 20) | ((reg & 7) << 16);
	btwrite(por, ZR36057_POR);
	if (post_office_wait(zr) < 0)
		return -1;
>>>>>>> upstream/android-13

	return btread(ZR36057_POR) & 0xFF;
}

/*
<<<<<<< HEAD
 * detect guests
 */

static void
dump_guests (struct zoran *zr)
{
	if (zr36067_debug > 2) {
		int i, guest[8];

		for (i = 1; i < 8; i++) {	// Don't read jpeg codec here
			guest[i] = post_office_read(zr, i, 0);
		}

		printk(KERN_INFO "%s: Guests: %*ph\n",
		       ZR_DEVNAME(zr), 8, guest);
	}
}

void
detect_guest_activity (struct zoran *zr)
{
	int timeout, i, j, res, guest[8], guest0[8], change[8][3];
	ktime_t t0, t1;

	dump_guests(zr);
	printk(KERN_INFO "%s: Detecting guests activity, please wait...\n",
	       ZR_DEVNAME(zr));
	for (i = 1; i < 8; i++) {	// Don't read jpeg codec here
		guest0[i] = guest[i] = post_office_read(zr, i, 0);
	}

	timeout = 0;
	j = 0;
	t0 = ktime_get();
	while (timeout < 10000) {
		udelay(10);
		timeout++;
		for (i = 1; (i < 8) && (j < 8); i++) {
			res = post_office_read(zr, i, 0);
			if (res != guest[i]) {
				t1 = ktime_get();
				change[j][0] = ktime_to_us(ktime_sub(t1, t0));
				t0 = t1;
				change[j][1] = i;
				change[j][2] = res;
				j++;
				guest[i] = res;
			}
		}
		if (j >= 8)
			break;
	}

	printk(KERN_INFO "%s: Guests: %*ph\n", ZR_DEVNAME(zr), 8, guest0);

	if (j == 0) {
		printk(KERN_INFO "%s: No activity detected.\n", ZR_DEVNAME(zr));
		return;
	}
	for (i = 0; i < j; i++) {
		printk(KERN_INFO "%s: %6d: %d => 0x%02x\n", ZR_DEVNAME(zr),
		       change[i][0], change[i][1], change[i][2]);
	}
}

/*
 * JPEG Codec access
 */

void
jpeg_codec_sleep (struct zoran *zr,
		  int           sleep)
{
	GPIO(zr, zr->card.gpio[ZR_GPIO_JPEG_SLEEP], !sleep);
	if (!sleep) {
		dprintk(3,
			KERN_DEBUG
			"%s: jpeg_codec_sleep() - wake GPIO=0x%08x\n",
			ZR_DEVNAME(zr), btread(ZR36057_GPPGCR1));
		udelay(500);
	} else {
		dprintk(3,
			KERN_DEBUG
			"%s: jpeg_codec_sleep() - sleep GPIO=0x%08x\n",
			ZR_DEVNAME(zr), btread(ZR36057_GPPGCR1));
=======
 * JPEG Codec access
 */

void jpeg_codec_sleep(struct zoran *zr, int sleep)
{
	GPIO(zr, zr->card.gpio[ZR_GPIO_JPEG_SLEEP], !sleep);
	if (!sleep) {
		pci_dbg(zr->pci_dev, "%s() - wake GPIO=0x%08x\n", __func__, btread(ZR36057_GPPGCR1));
		udelay(500);
	} else {
		pci_dbg(zr->pci_dev, "%s() - sleep GPIO=0x%08x\n", __func__, btread(ZR36057_GPPGCR1));
>>>>>>> upstream/android-13
		udelay(2);
	}
}

<<<<<<< HEAD
int
jpeg_codec_reset (struct zoran *zr)
=======
int jpeg_codec_reset(struct zoran *zr)
>>>>>>> upstream/android-13
{
	/* Take the codec out of sleep */
	jpeg_codec_sleep(zr, 0);

	if (zr->card.gpcs[GPCS_JPEG_RESET] != 0xff) {
		post_office_write(zr, zr->card.gpcs[GPCS_JPEG_RESET], 0,
				  0);
		udelay(2);
	} else {
		GPIO(zr, zr->card.gpio[ZR_GPIO_JPEG_RESET], 0);
		udelay(2);
		GPIO(zr, zr->card.gpio[ZR_GPIO_JPEG_RESET], 1);
		udelay(2);
	}

	return 0;
}

/*
 *   Set the registers for the size we have specified. Don't bother
 *   trying to understand this without the ZR36057 manual in front of
 *   you [AC].
<<<<<<< HEAD
 *
 *   PS: The manual is free for download in .pdf format from
 *   www.zoran.com - nicely done those folks.
 */

static void
zr36057_adjust_vfe (struct zoran          *zr,
		    enum zoran_codec_mode  mode)
=======
 */
static void zr36057_adjust_vfe(struct zoran *zr, enum zoran_codec_mode mode)
>>>>>>> upstream/android-13
{
	u32 reg;

	switch (mode) {
	case BUZ_MODE_MOTION_DECOMPRESS:
<<<<<<< HEAD
		btand(~ZR36057_VFESPFR_ExtFl, ZR36057_VFESPFR);
		reg = btread(ZR36057_VFEHCR);
		if ((reg & (1 << 10)) && zr->card.type != LML33R10) {
			reg += ((1 << 10) | 1);
		}
=======
		btand(~ZR36057_VFESPFR_EXT_FL, ZR36057_VFESPFR);
		reg = btread(ZR36057_VFEHCR);
		if ((reg & (1 << 10)) && zr->card.type != LML33R10)
			reg += ((1 << 10) | 1);

>>>>>>> upstream/android-13
		btwrite(reg, ZR36057_VFEHCR);
		break;
	case BUZ_MODE_MOTION_COMPRESS:
	case BUZ_MODE_IDLE:
	default:
		if ((zr->norm & V4L2_STD_NTSC) ||
		    (zr->card.type == LML33R10 &&
		     (zr->norm & V4L2_STD_PAL)))
<<<<<<< HEAD
			btand(~ZR36057_VFESPFR_ExtFl, ZR36057_VFESPFR);
		else
			btor(ZR36057_VFESPFR_ExtFl, ZR36057_VFESPFR);
		reg = btread(ZR36057_VFEHCR);
		if (!(reg & (1 << 10)) && zr->card.type != LML33R10) {
			reg -= ((1 << 10) | 1);
		}
=======
			btand(~ZR36057_VFESPFR_EXT_FL, ZR36057_VFESPFR);
		else
			btor(ZR36057_VFESPFR_EXT_FL, ZR36057_VFESPFR);
		reg = btread(ZR36057_VFEHCR);
		if (!(reg & (1 << 10)) && zr->card.type != LML33R10)
			reg -= ((1 << 10) | 1);

>>>>>>> upstream/android-13
		btwrite(reg, ZR36057_VFEHCR);
		break;
	}
}

/*
 * set geometry
 */

<<<<<<< HEAD
static void
zr36057_set_vfe (struct zoran              *zr,
		 int                        video_width,
		 int                        video_height,
		 const struct zoran_format *format)
{
	struct tvnorm *tvn;
	unsigned HStart, HEnd, VStart, VEnd;
	unsigned DispMode;
	unsigned VidWinWid, VidWinHt;
	unsigned hcrop1, hcrop2, vcrop1, vcrop2;
	unsigned Wa, We, Ha, He;
	unsigned X, Y, HorDcm, VerDcm;
	u32 reg;
	unsigned mask_line_size;

	tvn = zr->timing;

	Wa = tvn->Wa;
	Ha = tvn->Ha;

	dprintk(2, KERN_INFO "%s: set_vfe() - width = %d, height = %d\n",
		ZR_DEVNAME(zr), video_width, video_height);

	if (video_width < BUZ_MIN_WIDTH ||
	    video_height < BUZ_MIN_HEIGHT ||
	    video_width > Wa || video_height > Ha) {
		dprintk(1, KERN_ERR "%s: set_vfe: w=%d h=%d not valid\n",
			ZR_DEVNAME(zr), video_width, video_height);
=======
static void zr36057_set_vfe(struct zoran *zr, int video_width, int video_height,
			    const struct zoran_format *format)
{
	const struct tvnorm *tvn;
	unsigned int h_start, h_end, v_start, v_end;
	unsigned int disp_mode;
	unsigned int vid_win_wid, vid_win_ht;
	unsigned int hcrop1, hcrop2, vcrop1, vcrop2;
	unsigned int wa, we, ha, he;
	unsigned int X, Y, hor_dcm, ver_dcm;
	u32 reg;

	tvn = zr->timing;

	wa = tvn->wa;
	ha = tvn->ha;

	pci_info(zr->pci_dev, "set_vfe() - width = %d, height = %d\n", video_width, video_height);

	if (video_width < BUZ_MIN_WIDTH ||
	    video_height < BUZ_MIN_HEIGHT ||
	    video_width > wa || video_height > ha) {
		pci_err(zr->pci_dev, "set_vfe: w=%d h=%d not valid\n", video_width, video_height);
>>>>>>> upstream/android-13
		return;
	}

	/**** zr36057 ****/

	/* horizontal */
<<<<<<< HEAD
	VidWinWid = video_width;
	X = DIV_ROUND_UP(VidWinWid * 64, tvn->Wa);
	We = (VidWinWid * 64) / X;
	HorDcm = 64 - X;
	hcrop1 = 2 * ((tvn->Wa - We) / 4);
	hcrop2 = tvn->Wa - We - hcrop1;
	HStart = tvn->HStart ? tvn->HStart : 1;
=======
	vid_win_wid = video_width;
	X = DIV_ROUND_UP(vid_win_wid * 64, tvn->wa);
	we = (vid_win_wid * 64) / X;
	hor_dcm = 64 - X;
	hcrop1 = 2 * ((tvn->wa - we) / 4);
	hcrop2 = tvn->wa - we - hcrop1;
	h_start = tvn->h_start ? tvn->h_start : 1;
>>>>>>> upstream/android-13
	/* (Ronald) Original comment:
	 * "| 1 Doesn't have any effect, tested on both a DC10 and a DC10+"
	 * this is false. It inverses chroma values on the LML33R10 (so Cr
	 * suddenly is shown as Cb and reverse, really cool effect if you
	 * want to see blue faces, not useful otherwise). So don't use |1.
<<<<<<< HEAD
	 * However, the DC10 has '0' as HStart, but does need |1, so we
	 * use a dirty check...
	 */
	HEnd = HStart + tvn->Wa - 1;
	HStart += hcrop1;
	HEnd -= hcrop2;
	reg = ((HStart & ZR36057_VFEHCR_Hmask) << ZR36057_VFEHCR_HStart)
	    | ((HEnd & ZR36057_VFEHCR_Hmask) << ZR36057_VFEHCR_HEnd);
	if (zr->card.vfe_pol.hsync_pol)
		reg |= ZR36057_VFEHCR_HSPol;
	btwrite(reg, ZR36057_VFEHCR);

	/* Vertical */
	DispMode = !(video_height > BUZ_MAX_HEIGHT / 2);
	VidWinHt = DispMode ? video_height : video_height / 2;
	Y = DIV_ROUND_UP(VidWinHt * 64 * 2, tvn->Ha);
	He = (VidWinHt * 64) / Y;
	VerDcm = 64 - Y;
	vcrop1 = (tvn->Ha / 2 - He) / 2;
	vcrop2 = tvn->Ha / 2 - He - vcrop1;
	VStart = tvn->VStart;
	VEnd = VStart + tvn->Ha / 2;	// - 1; FIXME SnapShot times out with -1 in 768*576 on the DC10 - LP
	VStart += vcrop1;
	VEnd -= vcrop2;
	reg = ((VStart & ZR36057_VFEVCR_Vmask) << ZR36057_VFEVCR_VStart)
	    | ((VEnd & ZR36057_VFEVCR_Vmask) << ZR36057_VFEVCR_VEnd);
	if (zr->card.vfe_pol.vsync_pol)
		reg |= ZR36057_VFEVCR_VSPol;
=======
	 * However, the DC10 has '0' as h_start, but does need |1, so we
	 * use a dirty check...
	 */
	h_end = h_start + tvn->wa - 1;
	h_start += hcrop1;
	h_end -= hcrop2;
	reg = ((h_start & ZR36057_VFEHCR_HMASK) << ZR36057_VFEHCR_H_START)
	    | ((h_end & ZR36057_VFEHCR_HMASK) << ZR36057_VFEHCR_H_END);
	if (zr->card.vfe_pol.hsync_pol)
		reg |= ZR36057_VFEHCR_HS_POL;
	btwrite(reg, ZR36057_VFEHCR);

	/* Vertical */
	disp_mode = !(video_height > BUZ_MAX_HEIGHT / 2);
	vid_win_ht = disp_mode ? video_height : video_height / 2;
	Y = DIV_ROUND_UP(vid_win_ht * 64 * 2, tvn->ha);
	he = (vid_win_ht * 64) / Y;
	ver_dcm = 64 - Y;
	vcrop1 = (tvn->ha / 2 - he) / 2;
	vcrop2 = tvn->ha / 2 - he - vcrop1;
	v_start = tvn->v_start;
	v_end = v_start + tvn->ha / 2;	// - 1; FIXME SnapShot times out with -1 in 768*576 on the DC10 - LP
	v_start += vcrop1;
	v_end -= vcrop2;
	reg = ((v_start & ZR36057_VFEVCR_VMASK) << ZR36057_VFEVCR_V_START)
	    | ((v_end & ZR36057_VFEVCR_VMASK) << ZR36057_VFEVCR_V_END);
	if (zr->card.vfe_pol.vsync_pol)
		reg |= ZR36057_VFEVCR_VS_POL;
>>>>>>> upstream/android-13
	btwrite(reg, ZR36057_VFEVCR);

	/* scaler and pixel format */
	reg = 0;
<<<<<<< HEAD
	reg |= (HorDcm << ZR36057_VFESPFR_HorDcm);
	reg |= (VerDcm << ZR36057_VFESPFR_VerDcm);
	reg |= (DispMode << ZR36057_VFESPFR_DispMode);
	/* RJ: I don't know, why the following has to be the opposite
	 * of the corresponding ZR36060 setting, but only this way
	 * we get the correct colors when uncompressing to the screen  */
	//reg |= ZR36057_VFESPFR_VCLKPol; /**/
	/* RJ: Don't know if that is needed for NTSC also */
	if (!(zr->norm & V4L2_STD_NTSC))
		reg |= ZR36057_VFESPFR_ExtFl;	// NEEDED!!!!!!! Wolfgang
	reg |= ZR36057_VFESPFR_TopField;
	if (HorDcm >= 48) {
		reg |= 3 << ZR36057_VFESPFR_HFilter;	/* 5 tap filter */
	} else if (HorDcm >= 32) {
		reg |= 2 << ZR36057_VFESPFR_HFilter;	/* 4 tap filter */
	} else if (HorDcm >= 16) {
		reg |= 1 << ZR36057_VFESPFR_HFilter;	/* 3 tap filter */
	}
=======
	reg |= (hor_dcm << ZR36057_VFESPFR_HOR_DCM);
	reg |= (ver_dcm << ZR36057_VFESPFR_VER_DCM);
	reg |= (disp_mode << ZR36057_VFESPFR_DISP_MODE);
	/* RJ: I don't know, why the following has to be the opposite
	 * of the corresponding ZR36060 setting, but only this way
	 * we get the correct colors when uncompressing to the screen  */
	//reg |= ZR36057_VFESPFR_VCLK_POL; /**/
	/* RJ: Don't know if that is needed for NTSC also */
	if (!(zr->norm & V4L2_STD_NTSC))
		reg |= ZR36057_VFESPFR_EXT_FL;	// NEEDED!!!!!!! Wolfgang
	reg |= ZR36057_VFESPFR_TOP_FIELD;
	if (hor_dcm >= 48)
		reg |= 3 << ZR36057_VFESPFR_H_FILTER;	/* 5 tap filter */
	else if (hor_dcm >= 32)
		reg |= 2 << ZR36057_VFESPFR_H_FILTER;	/* 4 tap filter */
	else if (hor_dcm >= 16)
		reg |= 1 << ZR36057_VFESPFR_H_FILTER;	/* 3 tap filter */

>>>>>>> upstream/android-13
	reg |= format->vfespfr;
	btwrite(reg, ZR36057_VFESPFR);

	/* display configuration */
<<<<<<< HEAD
	reg = (16 << ZR36057_VDCR_MinPix)
	    | (VidWinHt << ZR36057_VDCR_VidWinHt)
	    | (VidWinWid << ZR36057_VDCR_VidWinWid);
	if (pci_pci_problems & PCIPCI_TRITON)
		// || zr->revision < 1) // Revision 1 has also Triton support
		reg &= ~ZR36057_VDCR_Triton;
	else
		reg |= ZR36057_VDCR_Triton;
	btwrite(reg, ZR36057_VDCR);

	/* (Ronald) don't write this if overlay_mask = NULL */
	if (zr->overlay_mask) {
		/* Write overlay clipping mask data, but don't enable overlay clipping */
		/* RJ: since this makes only sense on the screen, we use
		 * zr->overlay_settings.width instead of video_width */

		mask_line_size = (BUZ_MAX_WIDTH + 31) / 32;
		reg = virt_to_bus(zr->overlay_mask);
		btwrite(reg, ZR36057_MMTR);
		reg = virt_to_bus(zr->overlay_mask + mask_line_size);
		btwrite(reg, ZR36057_MMBR);
		reg =
		    mask_line_size - (zr->overlay_settings.width +
				      31) / 32;
		if (DispMode == 0)
			reg += mask_line_size;
		reg <<= ZR36057_OCR_MaskStride;
		btwrite(reg, ZR36057_OCR);
	}

	zr36057_adjust_vfe(zr, zr->codec_mode);
}

/*
 * Switch overlay on or off
 */

void
zr36057_overlay (struct zoran *zr,
		 int           on)
{
	u32 reg;

	if (on) {
		/* do the necessary settings ... */
		btand(~ZR36057_VDCR_VidEn, ZR36057_VDCR);	/* switch it off first */

		zr36057_set_vfe(zr,
				zr->overlay_settings.width,
				zr->overlay_settings.height,
				zr->overlay_settings.format);

		/* Start and length of each line MUST be 4-byte aligned.
		 * This should be already checked before the call to this routine.
		 * All error messages are internal driver checking only! */

		/* video display top and bottom registers */
		reg = (long) zr->vbuf_base +
		    zr->overlay_settings.x *
		    ((zr->overlay_settings.format->depth + 7) / 8) +
		    zr->overlay_settings.y *
		    zr->vbuf_bytesperline;
		btwrite(reg, ZR36057_VDTR);
		if (reg & 3)
			dprintk(1,
				KERN_ERR
				"%s: zr36057_overlay() - video_address not aligned\n",
				ZR_DEVNAME(zr));
		if (zr->overlay_settings.height > BUZ_MAX_HEIGHT / 2)
			reg += zr->vbuf_bytesperline;
		btwrite(reg, ZR36057_VDBR);

		/* video stride, status, and frame grab register */
		reg = zr->vbuf_bytesperline -
		    zr->overlay_settings.width *
		    ((zr->overlay_settings.format->depth + 7) / 8);
		if (zr->overlay_settings.height > BUZ_MAX_HEIGHT / 2)
			reg += zr->vbuf_bytesperline;
		if (reg & 3)
			dprintk(1,
				KERN_ERR
				"%s: zr36057_overlay() - video_stride not aligned\n",
				ZR_DEVNAME(zr));
		reg = (reg << ZR36057_VSSFGR_DispStride);
		reg |= ZR36057_VSSFGR_VidOvf;	/* clear overflow status */
		btwrite(reg, ZR36057_VSSFGR);

		/* Set overlay clipping */
		if (zr->overlay_settings.clipcount > 0)
			btor(ZR36057_OCR_OvlEnable, ZR36057_OCR);

		/* ... and switch it on */
		btor(ZR36057_VDCR_VidEn, ZR36057_VDCR);
	} else {
		/* Switch it off */
		btand(~ZR36057_VDCR_VidEn, ZR36057_VDCR);
	}
}

/*
 * The overlay mask has one bit for each pixel on a scan line,
 *  and the maximum window size is BUZ_MAX_WIDTH * BUZ_MAX_HEIGHT pixels.
 */

void write_overlay_mask(struct zoran_fh *fh, struct v4l2_clip *vp, int count)
{
	struct zoran *zr = fh->zr;
	unsigned mask_line_size = (BUZ_MAX_WIDTH + 31) / 32;
	u32 *mask;
	int x, y, width, height;
	unsigned i, j, k;

	/* fill mask with one bits */
	memset(fh->overlay_mask, ~0, mask_line_size * 4 * BUZ_MAX_HEIGHT);

	for (i = 0; i < count; ++i) {
		/* pick up local copy of clip */
		x = vp[i].c.left;
		y = vp[i].c.top;
		width = vp[i].c.width;
		height = vp[i].c.height;

		/* trim clips that extend beyond the window */
		if (x < 0) {
			width += x;
			x = 0;
		}
		if (y < 0) {
			height += y;
			y = 0;
		}
		if (x + width > fh->overlay_settings.width) {
			width = fh->overlay_settings.width - x;
		}
		if (y + height > fh->overlay_settings.height) {
			height = fh->overlay_settings.height - y;
		}

		/* ignore degenerate clips */
		if (height <= 0) {
			continue;
		}
		if (width <= 0) {
			continue;
		}

		/* apply clip for each scan line */
		for (j = 0; j < height; ++j) {
			/* reset bit for each pixel */
			/* this can be optimized later if need be */
			mask = fh->overlay_mask + (y + j) * mask_line_size;
			for (k = 0; k < width; ++k) {
				mask[(x + k) / 32] &=
				    ~((u32) 1 << (x + k) % 32);
			}
		}
	}
}

/* Enable/Disable uncompressed memory grabbing of the 36057 */

void
zr36057_set_memgrab (struct zoran *zr,
		     int           mode)
=======
	reg = (16 << ZR36057_VDCR_MIN_PIX)
	    | (vid_win_ht << ZR36057_VDCR_VID_WIN_HT)
	    | (vid_win_wid << ZR36057_VDCR_VID_WIN_WID);
	if (pci_pci_problems & PCIPCI_TRITON)
		// || zr->revision < 1) // Revision 1 has also Triton support
		reg &= ~ZR36057_VDCR_TRITON;
	else
		reg |= ZR36057_VDCR_TRITON;
	btwrite(reg, ZR36057_VDCR);

	zr36057_adjust_vfe(zr, zr->codec_mode);
}

/* Enable/Disable uncompressed memory grabbing of the 36057 */
void zr36057_set_memgrab(struct zoran *zr, int mode)
>>>>>>> upstream/android-13
{
	if (mode) {
		/* We only check SnapShot and not FrameGrab here.  SnapShot==1
		 * means a capture is already in progress, but FrameGrab==1
		 * doesn't necessary mean that.  It's more correct to say a 1
		 * to 0 transition indicates a capture completed.  If a
		 * capture is pending when capturing is tuned off, FrameGrab
		 * will be stuck at 1 until capturing is turned back on.
		 */
<<<<<<< HEAD
		if (btread(ZR36057_VSSFGR) & ZR36057_VSSFGR_SnapShot)
			dprintk(1,
				KERN_WARNING
				"%s: zr36057_set_memgrab(1) with SnapShot on!?\n",
				ZR_DEVNAME(zr));
=======
		if (btread(ZR36057_VSSFGR) & ZR36057_VSSFGR_SNAP_SHOT)
			pci_warn(zr->pci_dev, "zr36057_set_memgrab(1) with SnapShot on!?\n");
>>>>>>> upstream/android-13

		/* switch on VSync interrupts */
		btwrite(IRQ_MASK, ZR36057_ISR);	// Clear Interrupts
		btor(zr->card.vsync_int, ZR36057_ICR);	// SW

		/* enable SnapShot */
<<<<<<< HEAD
		btor(ZR36057_VSSFGR_SnapShot, ZR36057_VSSFGR);
=======
		btor(ZR36057_VSSFGR_SNAP_SHOT, ZR36057_VSSFGR);
>>>>>>> upstream/android-13

		/* Set zr36057 video front end  and enable video */
		zr36057_set_vfe(zr, zr->v4l_settings.width,
				zr->v4l_settings.height,
				zr->v4l_settings.format);
<<<<<<< HEAD

		zr->v4l_memgrab_active = 1;
=======
>>>>>>> upstream/android-13
	} else {
		/* switch off VSync interrupts */
		btand(~zr->card.vsync_int, ZR36057_ICR);	// SW

<<<<<<< HEAD
		zr->v4l_memgrab_active = 0;
		zr->v4l_grab_frame = NO_GRAB_ACTIVE;

		/* reenable grabbing to screen if it was running */
		if (zr->v4l_overlay_active) {
			zr36057_overlay(zr, 1);
		} else {
			btand(~ZR36057_VDCR_VidEn, ZR36057_VDCR);
			btand(~ZR36057_VSSFGR_SnapShot, ZR36057_VSSFGR);
		}
	}
}

int
wait_grab_pending (struct zoran *zr)
{
	unsigned long flags;

	/* wait until all pending grabs are finished */

	if (!zr->v4l_memgrab_active)
		return 0;

	wait_event_interruptible(zr->v4l_capq,
			(zr->v4l_pend_tail == zr->v4l_pend_head));
	if (signal_pending(current))
		return -ERESTARTSYS;

	spin_lock_irqsave(&zr->spinlock, flags);
	zr36057_set_memgrab(zr, 0);
	spin_unlock_irqrestore(&zr->spinlock, flags);

	return 0;
}

=======
		/* re-enable grabbing to screen if it was running */
		btand(~ZR36057_VDCR_VID_EN, ZR36057_VDCR);
		btand(~ZR36057_VSSFGR_SNAP_SHOT, ZR36057_VSSFGR);
	}
}

>>>>>>> upstream/android-13
/*****************************************************************************
 *                                                                           *
 *  Set up the Buz-specific MJPEG part                                       *
 *                                                                           *
 *****************************************************************************/

<<<<<<< HEAD
static inline void
set_frame (struct zoran *zr,
	   int           val)
=======
static inline void set_frame(struct zoran *zr, int val)
>>>>>>> upstream/android-13
{
	GPIO(zr, zr->card.gpio[ZR_GPIO_JPEG_FRAME], val);
}

<<<<<<< HEAD
static void
set_videobus_dir (struct zoran *zr,
		  int           val)
=======
static void set_videobus_dir(struct zoran *zr, int val)
>>>>>>> upstream/android-13
{
	switch (zr->card.type) {
	case LML33:
	case LML33R10:
		if (!lml33dpath)
			GPIO(zr, 5, val);
		else
			GPIO(zr, 5, 1);
		break;
	default:
		GPIO(zr, zr->card.gpio[ZR_GPIO_VID_DIR],
		     zr->card.gpio_pol[ZR_GPIO_VID_DIR] ? !val : val);
		break;
	}
}

<<<<<<< HEAD
static void
init_jpeg_queue (struct zoran *zr)
=======
static void init_jpeg_queue(struct zoran *zr)
>>>>>>> upstream/android-13
{
	int i;

	/* re-initialize DMA ring stuff */
	zr->jpg_que_head = 0;
	zr->jpg_dma_head = 0;
	zr->jpg_dma_tail = 0;
	zr->jpg_que_tail = 0;
	zr->jpg_seq_num = 0;
<<<<<<< HEAD
	zr->JPEG_error = 0;
=======
	zr->jpeg_error = 0;
>>>>>>> upstream/android-13
	zr->num_errors = 0;
	zr->jpg_err_seq = 0;
	zr->jpg_err_shift = 0;
	zr->jpg_queued_num = 0;
<<<<<<< HEAD
	for (i = 0; i < zr->jpg_buffers.num_buffers; i++) {
		zr->jpg_buffers.buffer[i].state = BUZ_STATE_USER;	/* nothing going on */
	}
	for (i = 0; i < BUZ_NUM_STAT_COM; i++) {
		zr->stat_com[i] = cpu_to_le32(1);	/* mark as unavailable to zr36057 */
	}
}

static void
zr36057_set_jpg (struct zoran          *zr,
		 enum zoran_codec_mode  mode)
{
	struct tvnorm *tvn;
=======
	for (i = 0; i < BUZ_NUM_STAT_COM; i++)
		zr->stat_com[i] = cpu_to_le32(1);	/* mark as unavailable to zr36057 */
}

static void zr36057_set_jpg(struct zoran *zr, enum zoran_codec_mode mode)
{
	const struct tvnorm *tvn;
>>>>>>> upstream/android-13
	u32 reg;

	tvn = zr->timing;

	/* assert P_Reset, disable code transfer, deassert Active */
	btwrite(0, ZR36057_JPC);

	/* MJPEG compression mode */
	switch (mode) {
<<<<<<< HEAD

	case BUZ_MODE_MOTION_COMPRESS:
	default:
		reg = ZR36057_JMC_MJPGCmpMode;
		break;

	case BUZ_MODE_MOTION_DECOMPRESS:
		reg = ZR36057_JMC_MJPGExpMode;
		reg |= ZR36057_JMC_SyncMstr;
=======
	case BUZ_MODE_MOTION_COMPRESS:
	default:
		reg = ZR36057_JMC_MJPG_CMP_MODE;
		break;

	case BUZ_MODE_MOTION_DECOMPRESS:
		reg = ZR36057_JMC_MJPG_EXP_MODE;
		reg |= ZR36057_JMC_SYNC_MSTR;
>>>>>>> upstream/android-13
		/* RJ: The following is experimental - improves the output to screen */
		//if(zr->jpg_settings.VFIFO_FB) reg |= ZR36057_JMC_VFIFO_FB; // No, it doesn't. SM
		break;

	case BUZ_MODE_STILL_COMPRESS:
<<<<<<< HEAD
		reg = ZR36057_JMC_JPGCmpMode;
		break;

	case BUZ_MODE_STILL_DECOMPRESS:
		reg = ZR36057_JMC_JPGExpMode;
		break;

	}
	reg |= ZR36057_JMC_JPG;
	if (zr->jpg_settings.field_per_buff == 1)
		reg |= ZR36057_JMC_Fld_per_buff;
	btwrite(reg, ZR36057_JMC);

	/* vertical */
	btor(ZR36057_VFEVCR_VSPol, ZR36057_VFEVCR);
	reg = (6 << ZR36057_VSP_VsyncSize) |
	      (tvn->Ht << ZR36057_VSP_FrmTot);
	btwrite(reg, ZR36057_VSP);
	reg = ((zr->jpg_settings.img_y + tvn->VStart) << ZR36057_FVAP_NAY) |
=======
		reg = ZR36057_JMC_JPG_CMP_MODE;
		break;

	case BUZ_MODE_STILL_DECOMPRESS:
		reg = ZR36057_JMC_JPG_EXP_MODE;
		break;
	}
	reg |= ZR36057_JMC_JPG;
	if (zr->jpg_settings.field_per_buff == 1)
		reg |= ZR36057_JMC_FLD_PER_BUFF;
	btwrite(reg, ZR36057_JMC);

	/* vertical */
	btor(ZR36057_VFEVCR_VS_POL, ZR36057_VFEVCR);
	reg = (6 << ZR36057_VSP_VSYNC_SIZE) |
	      (tvn->ht << ZR36057_VSP_FRM_TOT);
	btwrite(reg, ZR36057_VSP);
	reg = ((zr->jpg_settings.img_y + tvn->v_start) << ZR36057_FVAP_NAY) |
>>>>>>> upstream/android-13
	      (zr->jpg_settings.img_height << ZR36057_FVAP_PAY);
	btwrite(reg, ZR36057_FVAP);

	/* horizontal */
	if (zr->card.vfe_pol.hsync_pol)
<<<<<<< HEAD
		btor(ZR36057_VFEHCR_HSPol, ZR36057_VFEHCR);
	else
		btand(~ZR36057_VFEHCR_HSPol, ZR36057_VFEHCR);
	reg = ((tvn->HSyncStart) << ZR36057_HSP_HsyncStart) |
	      (tvn->Wt << ZR36057_HSP_LineTot);
	btwrite(reg, ZR36057_HSP);
	reg = ((zr->jpg_settings.img_x +
		tvn->HStart + 4) << ZR36057_FHAP_NAX) |
=======
		btor(ZR36057_VFEHCR_HS_POL, ZR36057_VFEHCR);
	else
		btand(~ZR36057_VFEHCR_HS_POL, ZR36057_VFEHCR);
	reg = ((tvn->h_sync_start) << ZR36057_HSP_HSYNC_START) |
	      (tvn->wt << ZR36057_HSP_LINE_TOT);
	btwrite(reg, ZR36057_HSP);
	reg = ((zr->jpg_settings.img_x +
		tvn->h_start + 4) << ZR36057_FHAP_NAX) |
>>>>>>> upstream/android-13
	      (zr->jpg_settings.img_width << ZR36057_FHAP_PAX);
	btwrite(reg, ZR36057_FHAP);

	/* field process parameters */
	if (zr->jpg_settings.odd_even)
<<<<<<< HEAD
		reg = ZR36057_FPP_Odd_Even;
=======
		reg = ZR36057_FPP_ODD_EVEN;
>>>>>>> upstream/android-13
	else
		reg = 0;

	btwrite(reg, ZR36057_FPP);

	/* Set proper VCLK Polarity, else colors will be wrong during playback */
<<<<<<< HEAD
	//btor(ZR36057_VFESPFR_VCLKPol, ZR36057_VFESPFR);

	/* code base address */
	reg = virt_to_bus(zr->stat_com);
	btwrite(reg, ZR36057_JCBA);
=======
	//btor(ZR36057_VFESPFR_VCLK_POL, ZR36057_VFESPFR);

	/* code base address */
	btwrite(zr->p_sc, ZR36057_JCBA);
>>>>>>> upstream/android-13

	/* FIFO threshold (FIFO is 160. double words) */
	/* NOTE: decimal values here */
	switch (mode) {
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	case BUZ_MODE_STILL_COMPRESS:
	case BUZ_MODE_MOTION_COMPRESS:
		if (zr->card.type != BUZ)
			reg = 140;
		else
			reg = 60;
		break;

	case BUZ_MODE_STILL_DECOMPRESS:
	case BUZ_MODE_MOTION_DECOMPRESS:
		reg = 20;
		break;

	default:
		reg = 80;
		break;
<<<<<<< HEAD

	}
	btwrite(reg, ZR36057_JCFT);
	zr36057_adjust_vfe(zr, mode);

}

void
print_interrupts (struct zoran *zr)
{
	int res, noerr = 0;

	printk(KERN_INFO "%s: interrupts received:", ZR_DEVNAME(zr));
	if ((res = zr->field_counter) < -1 || res > 1) {
		printk(KERN_CONT " FD:%d", res);
	}
	if ((res = zr->intr_counter_GIRQ1) != 0) {
		printk(KERN_CONT " GIRQ1:%d", res);
		noerr++;
	}
	if ((res = zr->intr_counter_GIRQ0) != 0) {
		printk(KERN_CONT " GIRQ0:%d", res);
		noerr++;
	}
	if ((res = zr->intr_counter_CodRepIRQ) != 0) {
		printk(KERN_CONT " CodRepIRQ:%d", res);
		noerr++;
	}
	if ((res = zr->intr_counter_JPEGRepIRQ) != 0) {
		printk(KERN_CONT " JPEGRepIRQ:%d", res);
		noerr++;
	}
	if (zr->JPEG_max_missed) {
		printk(KERN_CONT " JPEG delays: max=%d min=%d", zr->JPEG_max_missed,
		       zr->JPEG_min_missed);
	}
	if (zr->END_event_missed) {
		printk(KERN_CONT " ENDs missed: %d", zr->END_event_missed);
	}
	//if (zr->jpg_queued_num) {
	printk(KERN_CONT " queue_state=%ld/%ld/%ld/%ld", zr->jpg_que_tail,
	       zr->jpg_dma_tail, zr->jpg_dma_head, zr->jpg_que_head);
	//}
	if (!noerr) {
		printk(KERN_CONT ": no interrupts detected.");
	}
	printk(KERN_CONT "\n");
}

void
clear_interrupt_counters (struct zoran *zr)
{
	zr->intr_counter_GIRQ1 = 0;
	zr->intr_counter_GIRQ0 = 0;
	zr->intr_counter_CodRepIRQ = 0;
	zr->intr_counter_JPEGRepIRQ = 0;
	zr->field_counter = 0;
	zr->IRQ1_in = 0;
	zr->IRQ1_out = 0;
	zr->JPEG_in = 0;
	zr->JPEG_out = 0;
	zr->JPEG_0 = 0;
	zr->JPEG_1 = 0;
	zr->END_event_missed = 0;
	zr->JPEG_missed = 0;
	zr->JPEG_max_missed = 0;
	zr->JPEG_min_missed = 0x7fffffff;
}

static u32
count_reset_interrupt (struct zoran *zr)
{
	u32 isr;

	if ((isr = btread(ZR36057_ISR) & 0x78000000)) {
=======
	}
	btwrite(reg, ZR36057_JCFT);
	zr36057_adjust_vfe(zr, mode);
}

void clear_interrupt_counters(struct zoran *zr)
{
	zr->intr_counter_GIRQ1 = 0;
	zr->intr_counter_GIRQ0 = 0;
	zr->intr_counter_cod_rep_irq = 0;
	zr->intr_counter_jpeg_rep_irq = 0;
	zr->field_counter = 0;
	zr->irq1_in = 0;
	zr->irq1_out = 0;
	zr->jpeg_in = 0;
	zr->jpeg_out = 0;
	zr->JPEG_0 = 0;
	zr->JPEG_1 = 0;
	zr->end_event_missed = 0;
	zr->jpeg_missed = 0;
	zr->jpeg_max_missed = 0;
	zr->jpeg_min_missed = 0x7fffffff;
}

static u32 count_reset_interrupt(struct zoran *zr)
{
	u32 isr;

	isr = btread(ZR36057_ISR) & 0x78000000;
	if (isr) {
>>>>>>> upstream/android-13
		if (isr & ZR36057_ISR_GIRQ1) {
			btwrite(ZR36057_ISR_GIRQ1, ZR36057_ISR);
			zr->intr_counter_GIRQ1++;
		}
		if (isr & ZR36057_ISR_GIRQ0) {
			btwrite(ZR36057_ISR_GIRQ0, ZR36057_ISR);
			zr->intr_counter_GIRQ0++;
		}
<<<<<<< HEAD
		if (isr & ZR36057_ISR_CodRepIRQ) {
			btwrite(ZR36057_ISR_CodRepIRQ, ZR36057_ISR);
			zr->intr_counter_CodRepIRQ++;
		}
		if (isr & ZR36057_ISR_JPEGRepIRQ) {
			btwrite(ZR36057_ISR_JPEGRepIRQ, ZR36057_ISR);
			zr->intr_counter_JPEGRepIRQ++;
=======
		if (isr & ZR36057_ISR_COD_REP_IRQ) {
			btwrite(ZR36057_ISR_COD_REP_IRQ, ZR36057_ISR);
			zr->intr_counter_cod_rep_irq++;
		}
		if (isr & ZR36057_ISR_JPEG_REP_IRQ) {
			btwrite(ZR36057_ISR_JPEG_REP_IRQ, ZR36057_ISR);
			zr->intr_counter_jpeg_rep_irq++;
>>>>>>> upstream/android-13
		}
	}
	return isr;
}

<<<<<<< HEAD
void
jpeg_start (struct zoran *zr)
=======
void jpeg_start(struct zoran *zr)
>>>>>>> upstream/android-13
{
	int reg;

	zr->frame_num = 0;

	/* deassert P_reset, disable code transfer, deassert Active */
<<<<<<< HEAD
	btwrite(ZR36057_JPC_P_Reset, ZR36057_JPC);
	/* stop flushing the internal code buffer */
	btand(~ZR36057_MCTCR_CFlush, ZR36057_MCTCR);
	/* enable code transfer */
	btor(ZR36057_JPC_CodTrnsEn, ZR36057_JPC);
=======
	btwrite(ZR36057_JPC_P_RESET, ZR36057_JPC);
	/* stop flushing the internal code buffer */
	btand(~ZR36057_MCTCR_C_FLUSH, ZR36057_MCTCR);
	/* enable code transfer */
	btor(ZR36057_JPC_COD_TRNS_EN, ZR36057_JPC);
>>>>>>> upstream/android-13

	/* clear IRQs */
	btwrite(IRQ_MASK, ZR36057_ISR);
	/* enable the JPEG IRQs */
<<<<<<< HEAD
	btwrite(zr->card.jpeg_int |
			ZR36057_ICR_JPEGRepIRQ |
			ZR36057_ICR_IntPinEn,
=======
	btwrite(zr->card.jpeg_int | ZR36057_ICR_JPEG_REP_IRQ | ZR36057_ICR_INT_PIN_EN,
>>>>>>> upstream/android-13
		ZR36057_ICR);

	set_frame(zr, 0);	// \FRAME

	/* set the JPEG codec guest ID */
<<<<<<< HEAD
	reg = (zr->card.gpcs[1] << ZR36057_JCGI_JPEGuestID) |
	       (0 << ZR36057_JCGI_JPEGuestReg);
=======
	reg = (zr->card.gpcs[1] << ZR36057_JCGI_JPE_GUEST_ID) |
	       (0 << ZR36057_JCGI_JPE_GUEST_REG);
>>>>>>> upstream/android-13
	btwrite(reg, ZR36057_JCGI);

	if (zr->card.video_vfe == CODEC_TYPE_ZR36016 &&
	    zr->card.video_codec == CODEC_TYPE_ZR36050) {
		/* Enable processing on the ZR36016 */
		if (zr->vfe)
			zr36016_write(zr->vfe, 0, 1);

		/* load the address of the GO register in the ZR36050 latch */
		post_office_write(zr, 0, 0, 0);
	}

	/* assert Active */
<<<<<<< HEAD
	btor(ZR36057_JPC_Active, ZR36057_JPC);

	/* enable the Go generation */
	btor(ZR36057_JMC_Go_en, ZR36057_JMC);
=======
	btor(ZR36057_JPC_ACTIVE, ZR36057_JPC);

	/* enable the Go generation */
	btor(ZR36057_JMC_GO_EN, ZR36057_JMC);
>>>>>>> upstream/android-13
	udelay(30);

	set_frame(zr, 1);	// /FRAME

<<<<<<< HEAD
	dprintk(3, KERN_DEBUG "%s: jpeg_start\n", ZR_DEVNAME(zr));
}

void
zr36057_enable_jpg (struct zoran          *zr,
		    enum zoran_codec_mode  mode)
{
	struct vfe_settings cap;
	int field_size =
	    zr->jpg_buffers.buffer_size / zr->jpg_settings.field_per_buff;
=======
	pci_dbg(zr->pci_dev, "jpeg_start\n");
}

void zr36057_enable_jpg(struct zoran *zr, enum zoran_codec_mode mode)
{
	struct vfe_settings cap;
	int field_size = zr->buffer_size / zr->jpg_settings.field_per_buff;
>>>>>>> upstream/android-13

	zr->codec_mode = mode;

	cap.x = zr->jpg_settings.img_x;
	cap.y = zr->jpg_settings.img_y;
	cap.width = zr->jpg_settings.img_width;
	cap.height = zr->jpg_settings.img_height;
	cap.decimation =
<<<<<<< HEAD
	    zr->jpg_settings.HorDcm | (zr->jpg_settings.VerDcm << 8);
	cap.quality = zr->jpg_settings.jpg_comp.quality;

	switch (mode) {

=======
	    zr->jpg_settings.hor_dcm | (zr->jpg_settings.ver_dcm << 8);
	cap.quality = zr->jpg_settings.jpg_comp.quality;

	switch (mode) {
>>>>>>> upstream/android-13
	case BUZ_MODE_MOTION_COMPRESS: {
		struct jpeg_app_marker app;
		struct jpeg_com_marker com;

		/* In motion compress mode, the decoder output must be enabled, and
		 * the video bus direction set to input.
		 */
		set_videobus_dir(zr, 0);
		decoder_call(zr, video, s_stream, 1);
		encoder_call(zr, video, s_routing, 0, 0, 0);

		/* Take the JPEG codec and the VFE out of sleep */
		jpeg_codec_sleep(zr, 0);

		/* set JPEG app/com marker */
		app.appn = zr->jpg_settings.jpg_comp.APPn;
		app.len = zr->jpg_settings.jpg_comp.APP_len;
		memcpy(app.data, zr->jpg_settings.jpg_comp.APP_data, 60);
		zr->codec->control(zr->codec, CODEC_S_JPEG_APP_DATA,
				   sizeof(struct jpeg_app_marker), &app);

		com.len = zr->jpg_settings.jpg_comp.COM_len;
		memcpy(com.data, zr->jpg_settings.jpg_comp.COM_data, 60);
		zr->codec->control(zr->codec, CODEC_S_JPEG_COM_DATA,
				   sizeof(struct jpeg_com_marker), &com);

		/* Setup the JPEG codec */
		zr->codec->control(zr->codec, CODEC_S_JPEG_TDS_BYTE,
				   sizeof(int), &field_size);
		zr->codec->set_video(zr->codec, zr->timing, &cap,
				     &zr->card.vfe_pol);
		zr->codec->set_mode(zr->codec, CODEC_DO_COMPRESSION);

		/* Setup the VFE */
		if (zr->vfe) {
			zr->vfe->control(zr->vfe, CODEC_S_JPEG_TDS_BYTE,
					 sizeof(int), &field_size);
			zr->vfe->set_video(zr->vfe, zr->timing, &cap,
					   &zr->card.vfe_pol);
			zr->vfe->set_mode(zr->vfe, CODEC_DO_COMPRESSION);
		}

		init_jpeg_queue(zr);
		zr36057_set_jpg(zr, mode);	// \P_Reset, ... Video param, FIFO

		clear_interrupt_counters(zr);
<<<<<<< HEAD
		dprintk(2, KERN_INFO "%s: enable_jpg(MOTION_COMPRESS)\n",
			ZR_DEVNAME(zr));
=======
		pci_info(zr->pci_dev, "enable_jpg(MOTION_COMPRESS)\n");
>>>>>>> upstream/android-13
		break;
	}

	case BUZ_MODE_MOTION_DECOMPRESS:
		/* In motion decompression mode, the decoder output must be disabled, and
		 * the video bus direction set to output.
		 */
		decoder_call(zr, video, s_stream, 0);
		set_videobus_dir(zr, 1);
		encoder_call(zr, video, s_routing, 1, 0, 0);

		/* Take the JPEG codec and the VFE out of sleep */
		jpeg_codec_sleep(zr, 0);
		/* Setup the VFE */
		if (zr->vfe) {
			zr->vfe->set_video(zr->vfe, zr->timing, &cap,
					   &zr->card.vfe_pol);
			zr->vfe->set_mode(zr->vfe, CODEC_DO_EXPANSION);
		}
		/* Setup the JPEG codec */
		zr->codec->set_video(zr->codec, zr->timing, &cap,
				     &zr->card.vfe_pol);
		zr->codec->set_mode(zr->codec, CODEC_DO_EXPANSION);

		init_jpeg_queue(zr);
		zr36057_set_jpg(zr, mode);	// \P_Reset, ... Video param, FIFO

		clear_interrupt_counters(zr);
<<<<<<< HEAD
		dprintk(2, KERN_INFO "%s: enable_jpg(MOTION_DECOMPRESS)\n",
			ZR_DEVNAME(zr));
=======
		pci_info(zr->pci_dev, "enable_jpg(MOTION_DECOMPRESS)\n");
>>>>>>> upstream/android-13
		break;

	case BUZ_MODE_IDLE:
	default:
		/* shut down processing */
<<<<<<< HEAD
		btand(~(zr->card.jpeg_int | ZR36057_ICR_JPEGRepIRQ),
		      ZR36057_ICR);
		btwrite(zr->card.jpeg_int | ZR36057_ICR_JPEGRepIRQ,
			ZR36057_ISR);
		btand(~ZR36057_JMC_Go_en, ZR36057_JMC);	// \Go_en
=======
		btand(~(zr->card.jpeg_int | ZR36057_ICR_JPEG_REP_IRQ),
		      ZR36057_ICR);
		btwrite(zr->card.jpeg_int | ZR36057_ICR_JPEG_REP_IRQ,
			ZR36057_ISR);
		btand(~ZR36057_JMC_GO_EN, ZR36057_JMC);	// \Go_en
>>>>>>> upstream/android-13

		msleep(50);

		set_videobus_dir(zr, 0);
		set_frame(zr, 1);	// /FRAME
<<<<<<< HEAD
		btor(ZR36057_MCTCR_CFlush, ZR36057_MCTCR);	// /CFlush
		btwrite(0, ZR36057_JPC);	// \P_Reset,\CodTrnsEn,\Active
		btand(~ZR36057_JMC_VFIFO_FB, ZR36057_JMC);
		btand(~ZR36057_JMC_SyncMstr, ZR36057_JMC);
=======
		btor(ZR36057_MCTCR_C_FLUSH, ZR36057_MCTCR);	// /CFlush
		btwrite(0, ZR36057_JPC);	// \P_Reset,\CodTrnsEn,\Active
		btand(~ZR36057_JMC_VFIFO_FB, ZR36057_JMC);
		btand(~ZR36057_JMC_SYNC_MSTR, ZR36057_JMC);
>>>>>>> upstream/android-13
		jpeg_codec_reset(zr);
		jpeg_codec_sleep(zr, 1);
		zr36057_adjust_vfe(zr, mode);

		decoder_call(zr, video, s_stream, 1);
		encoder_call(zr, video, s_routing, 0, 0, 0);

<<<<<<< HEAD
		dprintk(2, KERN_INFO "%s: enable_jpg(IDLE)\n", ZR_DEVNAME(zr));
		break;

=======
		pci_info(zr->pci_dev, "enable_jpg(IDLE)\n");
		break;
>>>>>>> upstream/android-13
	}
}

/* when this is called the spinlock must be held */
<<<<<<< HEAD
void
zoran_feed_stat_com (struct zoran *zr)
{
	/* move frames from pending queue to DMA */

	int frame, i, max_stat_com;

	max_stat_com =
	    (zr->jpg_settings.TmpDcm ==
	     1) ? BUZ_NUM_STAT_COM : (BUZ_NUM_STAT_COM >> 1);

	while ((zr->jpg_dma_head - zr->jpg_dma_tail) < max_stat_com &&
	       zr->jpg_dma_head < zr->jpg_que_head) {

		frame = zr->jpg_pend[zr->jpg_dma_head & BUZ_MASK_FRAME];
		if (zr->jpg_settings.TmpDcm == 1) {
=======
void zoran_feed_stat_com(struct zoran *zr)
{
	/* move frames from pending queue to DMA */

	int i, max_stat_com;
	struct zr_buffer *buf;
	struct vb2_v4l2_buffer *vbuf;
	dma_addr_t phys_addr = 0;
	unsigned long flags;
	unsigned long payload;

	max_stat_com =
	    (zr->jpg_settings.tmp_dcm ==
	     1) ? BUZ_NUM_STAT_COM : (BUZ_NUM_STAT_COM >> 1);

	spin_lock_irqsave(&zr->queued_bufs_lock, flags);
	while ((zr->jpg_dma_head - zr->jpg_dma_tail) < max_stat_com) {
		buf = list_first_entry_or_null(&zr->queued_bufs, struct zr_buffer, queue);
		if (!buf) {
			pci_err(zr->pci_dev, "No buffer available to queue\n");
			spin_unlock_irqrestore(&zr->queued_bufs_lock, flags);
			return;
		}
		list_del(&buf->queue);
		zr->buf_in_reserve--;
		vbuf = &buf->vbuf;
		vbuf->vb2_buf.state = VB2_BUF_STATE_ACTIVE;
		phys_addr = vb2_dma_contig_plane_dma_addr(&vbuf->vb2_buf, 0);
		payload = vb2_get_plane_payload(&vbuf->vb2_buf, 0);
		if (payload == 0)
			payload = zr->buffer_size;
		if (zr->jpg_settings.tmp_dcm == 1) {
>>>>>>> upstream/android-13
			/* fill 1 stat_com entry */
			i = (zr->jpg_dma_head -
			     zr->jpg_err_shift) & BUZ_MASK_STAT_COM;
			if (!(zr->stat_com[i] & cpu_to_le32(1)))
				break;
<<<<<<< HEAD
			zr->stat_com[i] =
			    cpu_to_le32(zr->jpg_buffers.buffer[frame].jpg.frag_tab_bus);
=======
			zr->stat_comb[i * 2] = cpu_to_le32(phys_addr);
			zr->stat_comb[i * 2 + 1] = cpu_to_le32((payload >> 1) | 1);
			zr->inuse[i] = buf;
			zr->stat_com[i] = cpu_to_le32(zr->p_scb + i * 2 * 4);
>>>>>>> upstream/android-13
		} else {
			/* fill 2 stat_com entries */
			i = ((zr->jpg_dma_head -
			      zr->jpg_err_shift) & 1) * 2;
			if (!(zr->stat_com[i] & cpu_to_le32(1)))
				break;
<<<<<<< HEAD
			zr->stat_com[i] =
			    cpu_to_le32(zr->jpg_buffers.buffer[frame].jpg.frag_tab_bus);
			zr->stat_com[i + 1] =
			    cpu_to_le32(zr->jpg_buffers.buffer[frame].jpg.frag_tab_bus);
		}
		zr->jpg_buffers.buffer[frame].state = BUZ_STATE_DMA;
		zr->jpg_dma_head++;

	}
=======
			zr->stat_com[i] = cpu_to_le32(zr->p_scb + i * 2 * 4);
			zr->stat_com[i + 1] = cpu_to_le32(zr->p_scb + i * 2 * 4);

			zr->stat_comb[i * 2] = cpu_to_le32(phys_addr);
			zr->stat_comb[i * 2 + 1] = cpu_to_le32((payload >> 1) | 1);

			zr->inuse[i] = buf;
			zr->inuse[i + 1] = NULL;
		}
		zr->jpg_dma_head++;
	}
	spin_unlock_irqrestore(&zr->queued_bufs_lock, flags);
>>>>>>> upstream/android-13
	if (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS)
		zr->jpg_queued_num++;
}

/* when this is called the spinlock must be held */
<<<<<<< HEAD
static void
zoran_reap_stat_com (struct zoran *zr)
=======
static void zoran_reap_stat_com(struct zoran *zr)
>>>>>>> upstream/android-13
{
	/* move frames from DMA queue to done queue */

	int i;
	u32 stat_com;
	unsigned int seq;
	unsigned int dif;
<<<<<<< HEAD
	struct zoran_buffer *buffer;
	int frame;
=======
	unsigned long flags;
	struct zr_buffer *buf;
	unsigned int size = 0;
	u32 fcnt;
>>>>>>> upstream/android-13

	/* In motion decompress we don't have a hardware frame counter,
	 * we just count the interrupts here */

<<<<<<< HEAD
	if (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS) {
		zr->jpg_seq_num++;
	}
	while (zr->jpg_dma_tail < zr->jpg_dma_head) {
		if (zr->jpg_settings.TmpDcm == 1)
			i = (zr->jpg_dma_tail -
			     zr->jpg_err_shift) & BUZ_MASK_STAT_COM;
		else
			i = ((zr->jpg_dma_tail -
			      zr->jpg_err_shift) & 1) * 2 + 1;

		stat_com = le32_to_cpu(zr->stat_com[i]);

		if ((stat_com & 1) == 0) {
			return;
		}
		frame = zr->jpg_pend[zr->jpg_dma_tail & BUZ_MASK_FRAME];
		buffer = &zr->jpg_buffers.buffer[frame];
		v4l2_get_timestamp(&buffer->bs.timestamp);

		if (zr->codec_mode == BUZ_MODE_MOTION_COMPRESS) {
			buffer->bs.length = (stat_com & 0x7fffff) >> 1;

			/* update sequence number with the help of the counter in stat_com */

			seq = ((stat_com >> 24) + zr->jpg_err_seq) & 0xff;
			dif = (seq - zr->jpg_seq_num) & 0xff;
			zr->jpg_seq_num += dif;
		} else {
			buffer->bs.length = 0;
		}
		buffer->bs.seq =
		    zr->jpg_settings.TmpDcm ==
		    2 ? (zr->jpg_seq_num >> 1) : zr->jpg_seq_num;
		buffer->state = BUZ_STATE_DONE;

		zr->jpg_dma_tail++;
	}
}

static void zoran_restart(struct zoran *zr)
{
	/* Now the stat_comm buffer is ready for restart */
	unsigned int status = 0;
	int mode;

	if (zr->codec_mode == BUZ_MODE_MOTION_COMPRESS) {
		decoder_call(zr, video, g_input_status, &status);
		mode = CODEC_DO_COMPRESSION;
	} else {
		status = V4L2_IN_ST_NO_SIGNAL;
		mode = CODEC_DO_EXPANSION;
	}
	if (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS ||
	    !(status & V4L2_IN_ST_NO_SIGNAL)) {
		/********** RESTART code *************/
		jpeg_codec_reset(zr);
		zr->codec->set_mode(zr->codec, mode);
		zr36057_set_jpg(zr, zr->codec_mode);
		jpeg_start(zr);

		if (zr->num_errors <= 8)
			dprintk(2, KERN_INFO "%s: Restart\n",
				ZR_DEVNAME(zr));

		zr->JPEG_missed = 0;
		zr->JPEG_error = 2;
		/********** End RESTART code ***********/
	}
}

static void
error_handler (struct zoran *zr,
	       u32           astat,
	       u32           stat)
{
	int i;

	/* This is JPEG error handling part */
	if (zr->codec_mode != BUZ_MODE_MOTION_COMPRESS &&
	    zr->codec_mode != BUZ_MODE_MOTION_DECOMPRESS) {
		return;
	}

	if ((stat & 1) == 0 &&
	    zr->codec_mode == BUZ_MODE_MOTION_COMPRESS &&
	    zr->jpg_dma_tail - zr->jpg_que_tail >= zr->jpg_buffers.num_buffers) {
		/* No free buffers... */
		zoran_reap_stat_com(zr);
		zoran_feed_stat_com(zr);
		wake_up_interruptible(&zr->jpg_capq);
		zr->JPEG_missed = 0;
		return;
	}

	if (zr->JPEG_error == 1) {
		zoran_restart(zr);
		return;
	}

	/*
	 * First entry: error just happened during normal operation
	 *
	 * In BUZ_MODE_MOTION_COMPRESS:
	 *
	 * Possible glitch in TV signal. In this case we should
	 * stop the codec and wait for good quality signal before
	 * restarting it to avoid further problems
	 *
	 * In BUZ_MODE_MOTION_DECOMPRESS:
	 *
	 * Bad JPEG frame: we have to mark it as processed (codec crashed
	 * and was not able to do it itself), and to remove it from queue.
	 */
	btand(~ZR36057_JMC_Go_en, ZR36057_JMC);
	udelay(1);
	stat = stat | (post_office_read(zr, 7, 0) & 3) << 8;
	btwrite(0, ZR36057_JPC);
	btor(ZR36057_MCTCR_CFlush, ZR36057_MCTCR);
	jpeg_codec_reset(zr);
	jpeg_codec_sleep(zr, 1);
	zr->JPEG_error = 1;
	zr->num_errors++;

	/* Report error */
	if (zr36067_debug > 1 && zr->num_errors <= 8) {
		long frame;
		int j;

		frame = zr->jpg_pend[zr->jpg_dma_tail & BUZ_MASK_FRAME];
		printk(KERN_ERR
		       "%s: JPEG error stat=0x%08x(0x%08x) queue_state=%ld/%ld/%ld/%ld seq=%ld frame=%ld. Codec stopped. ",
		       ZR_DEVNAME(zr), stat, zr->last_isr,
		       zr->jpg_que_tail, zr->jpg_dma_tail,
		       zr->jpg_dma_head, zr->jpg_que_head,
		       zr->jpg_seq_num, frame);
		printk(KERN_INFO "stat_com frames:");
		for (j = 0; j < BUZ_NUM_STAT_COM; j++) {
			for (i = 0; i < zr->jpg_buffers.num_buffers; i++) {
				if (le32_to_cpu(zr->stat_com[j]) == zr->jpg_buffers.buffer[i].jpg.frag_tab_bus)
					printk(KERN_CONT "% d->%d", j, i);
			}
		}
		printk(KERN_CONT "\n");
	}
	/* Find an entry in stat_com and rotate contents */
	if (zr->jpg_settings.TmpDcm == 1)
		i = (zr->jpg_dma_tail - zr->jpg_err_shift) & BUZ_MASK_STAT_COM;
	else
		i = ((zr->jpg_dma_tail - zr->jpg_err_shift) & 1) * 2;
	if (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS) {
		/* Mimic zr36067 operation */
		zr->stat_com[i] |= cpu_to_le32(1);
		if (zr->jpg_settings.TmpDcm != 1)
			zr->stat_com[i + 1] |= cpu_to_le32(1);
		/* Refill */
		zoran_reap_stat_com(zr);
		zoran_feed_stat_com(zr);
		wake_up_interruptible(&zr->jpg_capq);
		/* Find an entry in stat_com again after refill */
		if (zr->jpg_settings.TmpDcm == 1)
			i = (zr->jpg_dma_tail - zr->jpg_err_shift) & BUZ_MASK_STAT_COM;
		else
			i = ((zr->jpg_dma_tail - zr->jpg_err_shift) & 1) * 2;
	}
	if (i) {
		/* Rotate stat_comm entries to make current entry first */
		int j;
		__le32 bus_addr[BUZ_NUM_STAT_COM];

		/* Here we are copying the stat_com array, which
		 * is already in little endian format, so
		 * no endian conversions here
		 */
		memcpy(bus_addr, zr->stat_com, sizeof(bus_addr));

		for (j = 0; j < BUZ_NUM_STAT_COM; j++)
			zr->stat_com[j] = bus_addr[(i + j) & BUZ_MASK_STAT_COM];

		zr->jpg_err_shift += i;
		zr->jpg_err_shift &= BUZ_MASK_STAT_COM;
	}
	if (zr->codec_mode == BUZ_MODE_MOTION_COMPRESS)
		zr->jpg_err_seq = zr->jpg_seq_num;	/* + 1; */
	zoran_restart(zr);
}

irqreturn_t
zoran_irq (int             irq,
	   void           *dev_id)
{
	u32 stat, astat;
	int count;
	struct zoran *zr;
	unsigned long flags;

	zr = dev_id;
	count = 0;

	if (zr->testing) {
		/* Testing interrupts */
		spin_lock_irqsave(&zr->spinlock, flags);
		while ((stat = count_reset_interrupt(zr))) {
			if (count++ > 100) {
				btand(~ZR36057_ICR_IntPinEn, ZR36057_ICR);
				dprintk(1,
					KERN_ERR
					"%s: IRQ lockup while testing, isr=0x%08x, cleared int mask\n",
					ZR_DEVNAME(zr), stat);
				wake_up_interruptible(&zr->test_q);
			}
		}
		zr->last_isr = stat;
		spin_unlock_irqrestore(&zr->spinlock, flags);
		return IRQ_HANDLED;
	}

	spin_lock_irqsave(&zr->spinlock, flags);
	while (1) {
		/* get/clear interrupt status bits */
		stat = count_reset_interrupt(zr);
		astat = stat & IRQ_MASK;
		if (!astat) {
			break;
		}
		dprintk(4,
			KERN_DEBUG
			"zoran_irq: astat: 0x%08x, mask: 0x%08x\n",
			astat, btread(ZR36057_ICR));
		if (astat & zr->card.vsync_int) {	// SW

			if (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS ||
			    zr->codec_mode == BUZ_MODE_MOTION_COMPRESS) {
				/* count missed interrupts */
				zr->JPEG_missed++;
			}
			//post_office_read(zr,1,0);
			/* Interrupts may still happen when
			 * zr->v4l_memgrab_active is switched off.
			 * We simply ignore them */

			if (zr->v4l_memgrab_active) {
				/* A lot more checks should be here ... */
				if ((btread(ZR36057_VSSFGR) & ZR36057_VSSFGR_SnapShot) == 0)
					dprintk(1,
						KERN_WARNING
						"%s: BuzIRQ with SnapShot off ???\n",
						ZR_DEVNAME(zr));

				if (zr->v4l_grab_frame != NO_GRAB_ACTIVE) {
					/* There is a grab on a frame going on, check if it has finished */
					if ((btread(ZR36057_VSSFGR) & ZR36057_VSSFGR_FrameGrab) == 0) {
						/* it is finished, notify the user */

						zr->v4l_buffers.buffer[zr->v4l_grab_frame].state = BUZ_STATE_DONE;
						zr->v4l_buffers.buffer[zr->v4l_grab_frame].bs.seq = zr->v4l_grab_seq;
						v4l2_get_timestamp(&zr->v4l_buffers.buffer[zr->v4l_grab_frame].bs.timestamp);
						zr->v4l_grab_frame = NO_GRAB_ACTIVE;
						zr->v4l_pend_tail++;
					}
				}

				if (zr->v4l_grab_frame == NO_GRAB_ACTIVE)
					wake_up_interruptible(&zr->v4l_capq);

				/* Check if there is another grab queued */

				if (zr->v4l_grab_frame == NO_GRAB_ACTIVE &&
				    zr->v4l_pend_tail != zr->v4l_pend_head) {
					int frame = zr->v4l_pend[zr->v4l_pend_tail & V4L_MASK_FRAME];
					u32 reg;

					zr->v4l_grab_frame = frame;

					/* Set zr36057 video front end and enable video */

					/* Buffer address */

					reg = zr->v4l_buffers.buffer[frame].v4l.fbuffer_bus;
					btwrite(reg, ZR36057_VDTR);
					if (zr->v4l_settings.height > BUZ_MAX_HEIGHT / 2)
						reg += zr->v4l_settings.bytesperline;
					btwrite(reg, ZR36057_VDBR);

					/* video stride, status, and frame grab register */
					reg = 0;
					if (zr->v4l_settings.height > BUZ_MAX_HEIGHT / 2)
						reg += zr->v4l_settings.bytesperline;
					reg = (reg << ZR36057_VSSFGR_DispStride);
					reg |= ZR36057_VSSFGR_VidOvf;
					reg |= ZR36057_VSSFGR_SnapShot;
					reg |= ZR36057_VSSFGR_FrameGrab;
					btwrite(reg, ZR36057_VSSFGR);

					btor(ZR36057_VDCR_VidEn,
					     ZR36057_VDCR);
				}
			}

			/* even if we don't grab, we do want to increment
			 * the sequence counter to see lost frames */
			zr->v4l_grab_seq++;
		}
#if (IRQ_MASK & ZR36057_ISR_CodRepIRQ)
		if (astat & ZR36057_ISR_CodRepIRQ) {
			zr->intr_counter_CodRepIRQ++;
			IDEBUG(printk(KERN_DEBUG "%s: ZR36057_ISR_CodRepIRQ\n",
				ZR_DEVNAME(zr)));
			btand(~ZR36057_ICR_CodRepIRQ, ZR36057_ICR);
		}
#endif				/* (IRQ_MASK & ZR36057_ISR_CodRepIRQ) */

#if (IRQ_MASK & ZR36057_ISR_JPEGRepIRQ)
		if ((astat & ZR36057_ISR_JPEGRepIRQ) &&
		    (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS ||
		     zr->codec_mode == BUZ_MODE_MOTION_COMPRESS)) {
			if (zr36067_debug > 1 && (!zr->frame_num || zr->JPEG_error)) {
				char sv[BUZ_NUM_STAT_COM + 1];
				int i;

				printk(KERN_INFO
				       "%s: first frame ready: state=0x%08x odd_even=%d field_per_buff=%d delay=%d\n",
				       ZR_DEVNAME(zr), stat,
				       zr->jpg_settings.odd_even,
				       zr->jpg_settings.field_per_buff,
				       zr->JPEG_missed);

				for (i = 0; i < BUZ_NUM_STAT_COM; i++)
					sv[i] = le32_to_cpu(zr->stat_com[i]) & 1 ? '1' : '0';
				sv[BUZ_NUM_STAT_COM] = 0;
				printk(KERN_INFO
				       "%s: stat_com=%s queue_state=%ld/%ld/%ld/%ld\n",
				       ZR_DEVNAME(zr), sv,
				       zr->jpg_que_tail,
				       zr->jpg_dma_tail,
				       zr->jpg_dma_head,
				       zr->jpg_que_head);
			} else {
				/* Get statistics */
				if (zr->JPEG_missed > zr->JPEG_max_missed)
					zr->JPEG_max_missed = zr->JPEG_missed;
				if (zr->JPEG_missed < zr->JPEG_min_missed)
					zr->JPEG_min_missed = zr->JPEG_missed;
			}

			if (zr36067_debug > 2 && zr->frame_num < 6) {
				int i;

				printk(KERN_INFO "%s: seq=%ld stat_com:",
				       ZR_DEVNAME(zr), zr->jpg_seq_num);
				for (i = 0; i < 4; i++) {
					printk(KERN_CONT " %08x",
					       le32_to_cpu(zr->stat_com[i]));
				}
				printk(KERN_CONT "\n");
			}
			zr->frame_num++;
			zr->JPEG_missed = 0;
			zr->JPEG_error = 0;
			zoran_reap_stat_com(zr);
			zoran_feed_stat_com(zr);
			wake_up_interruptible(&zr->jpg_capq);
		}
#endif				/* (IRQ_MASK & ZR36057_ISR_JPEGRepIRQ) */

		/* DATERR, too many fields missed, error processing */
		if ((astat & zr->card.jpeg_int) ||
		    zr->JPEG_missed > 25 ||
		    zr->JPEG_error == 1	||
		    ((zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS) &&
		     (zr->frame_num && (zr->JPEG_missed > zr->jpg_settings.field_per_buff)))) {
			error_handler(zr, astat, stat);
		}

		count++;
		if (count > 10) {
			dprintk(2, KERN_WARNING "%s: irq loop %d\n",
				ZR_DEVNAME(zr), count);
			if (count > 20) {
				btand(~ZR36057_ICR_IntPinEn, ZR36057_ICR);
				dprintk(2,
					KERN_ERR
					"%s: IRQ lockup, cleared int mask\n",
					ZR_DEVNAME(zr));
				break;
			}
		}
		zr->last_isr = stat;
	}
	spin_unlock_irqrestore(&zr->spinlock, flags);

	return IRQ_HANDLED;
}

void
zoran_set_pci_master (struct zoran *zr,
		      int           set_master)
=======
	if (zr->codec_mode == BUZ_MODE_MOTION_DECOMPRESS)
		zr->jpg_seq_num++;

	spin_lock_irqsave(&zr->queued_bufs_lock, flags);
	while (zr->jpg_dma_tail < zr->jpg_dma_head) {
		if (zr->jpg_settings.tmp_dcm == 1)
			i = (zr->jpg_dma_tail - zr->jpg_err_shift) & BUZ_MASK_STAT_COM;
		else
			i = ((zr->jpg_dma_tail - zr->jpg_err_shift) & 1) * 2;

		stat_com = le32_to_cpu(zr->stat_com[i]);
		if ((stat_com & 1) == 0) {
			spin_unlock_irqrestore(&zr->queued_bufs_lock, flags);
			return;
		}

		fcnt = (stat_com & GENMASK(31, 24)) >> 24;
		size = (stat_com & GENMASK(22, 1)) >> 1;

		buf = zr->inuse[i];
		if (!buf) {
			spin_unlock_irqrestore(&zr->queued_bufs_lock, flags);
			pci_err(zr->pci_dev, "No buffer at slot %d\n", i);
			return;
		}
		buf->vbuf.vb2_buf.timestamp = ktime_get_ns();

		if (zr->codec_mode == BUZ_MODE_MOTION_COMPRESS) {
			vb2_set_plane_payload(&buf->vbuf.vb2_buf, 0, size);

			/* update sequence number with the help of the counter in stat_com */
			seq = (fcnt + zr->jpg_err_seq) & 0xff;
			dif = (seq - zr->jpg_seq_num) & 0xff;
			zr->jpg_seq_num += dif;
		}
		buf->vbuf.sequence = zr->jpg_settings.tmp_dcm ==
		    2 ? (zr->jpg_seq_num >> 1) : zr->jpg_seq_num;
		zr->inuse[i] = NULL;
		if (zr->jpg_settings.tmp_dcm != 1)
			buf->vbuf.field = zr->jpg_settings.odd_even ?
				V4L2_FIELD_TOP : V4L2_FIELD_BOTTOM;
		else
			buf->vbuf.field = zr->jpg_settings.odd_even ?
				V4L2_FIELD_SEQ_TB : V4L2_FIELD_SEQ_BT;
		vb2_buffer_done(&buf->vbuf.vb2_buf, VB2_BUF_STATE_DONE);

		zr->jpg_dma_tail++;
	}
	spin_unlock_irqrestore(&zr->queued_bufs_lock, flags);
}

irqreturn_t zoran_irq(int irq, void *dev_id)
{
	struct zoran *zr = dev_id;
	u32 stat, astat;

	stat = count_reset_interrupt(zr);
	astat = stat & IRQ_MASK;
	if (astat & zr->card.vsync_int) {
		if (zr->running == ZORAN_MAP_MODE_RAW) {
			if ((btread(ZR36057_VSSFGR) & ZR36057_VSSFGR_SNAP_SHOT) == 0)
				pci_warn(zr->pci_dev, "BuzIRQ with SnapShot off ???\n");
			if ((btread(ZR36057_VSSFGR) & ZR36057_VSSFGR_FRAME_GRAB) == 0)
				zr_set_buf(zr);
			return IRQ_HANDLED;
		}
		if (astat & ZR36057_ISR_JPEG_REP_IRQ) {
			if (zr->codec_mode != BUZ_MODE_MOTION_DECOMPRESS &&
			    zr->codec_mode != BUZ_MODE_MOTION_COMPRESS) {
				pci_err(zr->pci_dev, "JPG IRQ when not in good mode\n");
				return IRQ_HANDLED;
			}
			zr->frame_num++;
			zoran_reap_stat_com(zr);
			zoran_feed_stat_com(zr);
			return IRQ_HANDLED;
		}
		/* unused interrupts */
	}
	zr->ghost_int++;
	return IRQ_HANDLED;
}

void zoran_set_pci_master(struct zoran *zr, int set_master)
>>>>>>> upstream/android-13
{
	if (set_master) {
		pci_set_master(zr->pci_dev);
	} else {
		u16 command;

		pci_read_config_word(zr->pci_dev, PCI_COMMAND, &command);
		command &= ~PCI_COMMAND_MASTER;
		pci_write_config_word(zr->pci_dev, PCI_COMMAND, command);
	}
}

<<<<<<< HEAD
void
zoran_init_hardware (struct zoran *zr)
=======
void zoran_init_hardware(struct zoran *zr)
>>>>>>> upstream/android-13
{
	/* Enable bus-mastering */
	zoran_set_pci_master(zr, 1);

	/* Initialize the board */
<<<<<<< HEAD
	if (zr->card.init) {
		zr->card.init(zr);
	}
=======
	if (zr->card.init)
		zr->card.init(zr);
>>>>>>> upstream/android-13

	decoder_call(zr, core, init, 0);
	decoder_call(zr, video, s_std, zr->norm);
	decoder_call(zr, video, s_routing,
<<<<<<< HEAD
		zr->card.input[zr->input].muxsel, 0, 0);
=======
		     zr->card.input[zr->input].muxsel, 0, 0);
>>>>>>> upstream/android-13

	encoder_call(zr, core, init, 0);
	encoder_call(zr, video, s_std_output, zr->norm);
	encoder_call(zr, video, s_routing, 0, 0, 0);

	/* toggle JPEG codec sleep to sync PLL */
	jpeg_codec_sleep(zr, 1);
	jpeg_codec_sleep(zr, 0);

	/*
	 * set individual interrupt enables (without GIRQ1)
	 * but don't global enable until zoran_open()
	 */
	zr36057_init_vfe(zr);

	zr36057_enable_jpg(zr, BUZ_MODE_IDLE);

	btwrite(IRQ_MASK, ZR36057_ISR);	// Clears interrupts
}

<<<<<<< HEAD
void
zr36057_restart (struct zoran *zr)
{
	btwrite(0, ZR36057_SPGPPCR);
	mdelay(1);
	btor(ZR36057_SPGPPCR_SoftReset, ZR36057_SPGPPCR);
	mdelay(1);
=======
void zr36057_restart(struct zoran *zr)
{
	btwrite(0, ZR36057_SPGPPCR);
	udelay(1000);
	btor(ZR36057_SPGPPCR_SOFT_RESET, ZR36057_SPGPPCR);
	udelay(1000);
>>>>>>> upstream/android-13

	/* assert P_Reset */
	btwrite(0, ZR36057_JPC);
	/* set up GPIO direction - all output */
<<<<<<< HEAD
	btwrite(ZR36057_SPGPPCR_SoftReset | 0, ZR36057_SPGPPCR);
=======
	btwrite(ZR36057_SPGPPCR_SOFT_RESET | 0, ZR36057_SPGPPCR);
>>>>>>> upstream/android-13

	/* set up GPIO pins and guest bus timing */
	btwrite((0x81 << 24) | 0x8888, ZR36057_GPPGCR1);
}

<<<<<<< HEAD
/*
 * initialize video front end
 */

static void
zr36057_init_vfe (struct zoran *zr)
{
	u32 reg;

	reg = btread(ZR36057_VFESPFR);
	reg |= ZR36057_VFESPFR_LittleEndian;
	reg &= ~ZR36057_VFESPFR_VCLKPol;
	reg |= ZR36057_VFESPFR_ExtFl;
	reg |= ZR36057_VFESPFR_TopField;
	btwrite(reg, ZR36057_VFESPFR);
	reg = btread(ZR36057_VDCR);
	if (pci_pci_problems & PCIPCI_TRITON)
		// || zr->revision < 1) // Revision 1 has also Triton support
		reg &= ~ZR36057_VDCR_Triton;
	else
		reg |= ZR36057_VDCR_Triton;
	btwrite(reg, ZR36057_VDCR);
}
=======
>>>>>>> upstream/android-13
