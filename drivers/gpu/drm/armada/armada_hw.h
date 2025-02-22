<<<<<<< HEAD
/*
 * Copyright (C) 2012 Russell King
 *  Rewritten from the dovefb driver, and Armada510 manuals.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Russell King
 *  Rewritten from the dovefb driver, and Armada510 manuals.
>>>>>>> upstream/android-13
 */
#ifndef ARMADA_HW_H
#define ARMADA_HW_H

/*
 * Note: the following registers are written from IRQ context:
 *  LCD_SPU_V_PORCH, LCD_SPU_ADV_REG, LCD_SPUT_V_H_TOTAL
 *  LCD_SPU_DMA_START_ADDR_[YUV][01], LCD_SPU_DMA_PITCH_YC,
 *  LCD_SPU_DMA_PITCH_UV, LCD_SPU_DMA_OVSA_HPXL_VLN,
 *  LCD_SPU_DMA_HPXL_VLN, LCD_SPU_DZM_HPXL_VLN, LCD_SPU_DMA_CTRL0
 */
enum {
	LCD_SPU_ADV_REG			= 0x0084,	/* Armada 510 */
	LCD_SPU_DMA_START_ADDR_Y0	= 0x00c0,
	LCD_SPU_DMA_START_ADDR_U0	= 0x00c4,
	LCD_SPU_DMA_START_ADDR_V0	= 0x00c8,
	LCD_CFG_DMA_START_ADDR_0	= 0x00cc,
	LCD_SPU_DMA_START_ADDR_Y1	= 0x00d0,
	LCD_SPU_DMA_START_ADDR_U1	= 0x00d4,
	LCD_SPU_DMA_START_ADDR_V1	= 0x00d8,
	LCD_CFG_DMA_START_ADDR_1	= 0x00dc,
	LCD_SPU_DMA_PITCH_YC		= 0x00e0,
	LCD_SPU_DMA_PITCH_UV		= 0x00e4,
	LCD_SPU_DMA_OVSA_HPXL_VLN	= 0x00e8,
	LCD_SPU_DMA_HPXL_VLN		= 0x00ec,
	LCD_SPU_DZM_HPXL_VLN		= 0x00f0,
	LCD_CFG_GRA_START_ADDR0		= 0x00f4,
	LCD_CFG_GRA_START_ADDR1		= 0x00f8,
	LCD_CFG_GRA_PITCH		= 0x00fc,
	LCD_SPU_GRA_OVSA_HPXL_VLN	= 0x0100,
	LCD_SPU_GRA_HPXL_VLN		= 0x0104,
	LCD_SPU_GZM_HPXL_VLN		= 0x0108,
	LCD_SPU_HWC_OVSA_HPXL_VLN	= 0x010c,
	LCD_SPU_HWC_HPXL_VLN		= 0x0110,
	LCD_SPUT_V_H_TOTAL		= 0x0114,
	LCD_SPU_V_H_ACTIVE		= 0x0118,
	LCD_SPU_H_PORCH			= 0x011c,
	LCD_SPU_V_PORCH			= 0x0120,
	LCD_SPU_BLANKCOLOR		= 0x0124,
	LCD_SPU_ALPHA_COLOR1		= 0x0128,
	LCD_SPU_ALPHA_COLOR2		= 0x012c,
	LCD_SPU_COLORKEY_Y		= 0x0130,
	LCD_SPU_COLORKEY_U		= 0x0134,
	LCD_SPU_COLORKEY_V		= 0x0138,
	LCD_CFG_RDREG4F			= 0x013c,	/* Armada 510 */
	LCD_SPU_SPI_RXDATA		= 0x0140,
	LCD_SPU_ISA_RXDATA		= 0x0144,
	LCD_SPU_HWC_RDDAT		= 0x0158,
	LCD_SPU_GAMMA_RDDAT		= 0x015c,
	LCD_SPU_PALETTE_RDDAT		= 0x0160,
	LCD_SPU_IOPAD_IN		= 0x0178,
	LCD_CFG_RDREG5F			= 0x017c,
	LCD_SPU_SPI_CTRL		= 0x0180,
	LCD_SPU_SPI_TXDATA		= 0x0184,
	LCD_SPU_SMPN_CTRL		= 0x0188,
	LCD_SPU_DMA_CTRL0		= 0x0190,
	LCD_SPU_DMA_CTRL1		= 0x0194,
	LCD_SPU_SRAM_CTRL		= 0x0198,
	LCD_SPU_SRAM_WRDAT		= 0x019c,
	LCD_SPU_SRAM_PARA0		= 0x01a0,	/* Armada 510 */
	LCD_SPU_SRAM_PARA1		= 0x01a4,
	LCD_CFG_SCLK_DIV		= 0x01a8,
	LCD_SPU_CONTRAST		= 0x01ac,
	LCD_SPU_SATURATION		= 0x01b0,
	LCD_SPU_CBSH_HUE		= 0x01b4,
	LCD_SPU_DUMB_CTRL		= 0x01b8,
	LCD_SPU_IOPAD_CONTROL		= 0x01bc,
	LCD_SPU_IRQ_ENA			= 0x01c0,
	LCD_SPU_IRQ_ISR			= 0x01c4,
};

/* For LCD_SPU_ADV_REG */
enum {
	ADV_VSYNC_L_OFF	= 0xfff << 20,
	ADV_GRACOLORKEY	= 1 << 19,
	ADV_VIDCOLORKEY	= 1 << 18,
	ADV_HWC32BLEND	= 1 << 15,
	ADV_HWC32ARGB	= 1 << 14,
	ADV_HWC32ENABLE	= 1 << 13,
	ADV_VSYNCOFFEN	= 1 << 12,
	ADV_VSYNC_H_OFF	= 0xfff << 0,
};

<<<<<<< HEAD
=======
/* LCD_CFG_RDREG4F - Armada 510 only */
enum {
	CFG_SRAM_WAIT	= BIT(11),
	CFG_SMPN_FASTTX	= BIT(10),
	CFG_DMA_ARB	= BIT(9),
	CFG_DMA_WM_EN	= BIT(8),
	CFG_DMA_WM_MASK	= 0xff,
#define CFG_DMA_WM(x)	((x) & CFG_DMA_WM_MASK)
};

>>>>>>> upstream/android-13
enum {
	CFG_565		= 0,
	CFG_1555	= 1,
	CFG_888PACK	= 2,
	CFG_X888	= 3,
	CFG_8888	= 4,
	CFG_422PACK	= 5,
	CFG_422		= 6,
	CFG_420		= 7,
	CFG_PSEUDO4	= 9,
	CFG_PSEUDO8	= 10,
	CFG_SWAPRB	= 1 << 4,
	CFG_SWAPUV	= 1 << 3,
	CFG_SWAPYU	= 1 << 2,
	CFG_YUV2RGB	= 1 << 1,
};

/* For LCD_SPU_DMA_CTRL0 */
enum {
	CFG_NOBLENDING	= 1 << 31,
	CFG_GAMMA_ENA	= 1 << 30,
	CFG_CBSH_ENA	= 1 << 29,
	CFG_PALETTE_ENA	= 1 << 28,
	CFG_ARBFAST_ENA	= 1 << 27,
	CFG_HWC_1BITMOD	= 1 << 26,
	CFG_HWC_1BITENA	= 1 << 25,
	CFG_HWC_ENA	= 1 << 24,
	CFG_DMAFORMAT	= 0xf << 20,
#define	CFG_DMA_FMT(x)	((x) << 20)
	CFG_GRAFORMAT	= 0xf << 16,
#define	CFG_GRA_FMT(x)	((x) << 16)
#define CFG_GRA_MOD(x)	((x) << 8)
	CFG_GRA_FTOGGLE	= 1 << 15,
	CFG_GRA_HSMOOTH	= 1 << 14,
	CFG_GRA_TSTMODE	= 1 << 13,
	CFG_GRA_ENA	= 1 << 8,
#define CFG_DMA_MOD(x)	((x) << 0)
	CFG_DMA_FTOGGLE	= 1 << 7,
	CFG_DMA_HSMOOTH	= 1 << 6,
	CFG_DMA_TSTMODE	= 1 << 5,
	CFG_DMA_ENA	= 1 << 0,
};

enum {
	CKMODE_DISABLE	= 0,
	CKMODE_Y	= 1,
	CKMODE_U	= 2,
	CKMODE_RGB	= 3,
	CKMODE_V	= 4,
	CKMODE_R	= 5,
	CKMODE_G	= 6,
	CKMODE_B	= 7,
};

/* For LCD_SPU_DMA_CTRL1 */
enum {
	CFG_FRAME_TRIG		= 1 << 31,
	CFG_VSYNC_INV		= 1 << 27,
	CFG_CKMODE_MASK		= 0x7 << 24,
#define CFG_CKMODE(x)		((x) << 24)
	CFG_CARRY		= 1 << 23,
	CFG_GATED_CLK		= 1 << 21,
	CFG_PWRDN_ENA		= 1 << 20,
	CFG_DSCALE_MASK		= 0x3 << 18,
	CFG_DSCALE_NONE		= 0x0 << 18,
	CFG_DSCALE_HALF		= 0x1 << 18,
	CFG_DSCALE_QUAR		= 0x2 << 18,
	CFG_ALPHAM_MASK		= 0x3 << 16,
	CFG_ALPHAM_VIDEO	= 0x0 << 16,
	CFG_ALPHAM_GRA		= 0x1 << 16,
	CFG_ALPHAM_CFG		= 0x2 << 16,
	CFG_ALPHA_MASK		= 0xff << 8,
#define CFG_ALPHA(x)		((x) << 8)
	CFG_PIXCMD_MASK		= 0xff,
};

/* For LCD_SPU_SRAM_CTRL */
enum {
	SRAM_READ	= 0 << 14,
	SRAM_WRITE	= 2 << 14,
	SRAM_INIT	= 3 << 14,
<<<<<<< HEAD
=======
	SRAM_GAMMA_YR	= 0x0 << 8,
	SRAM_GAMMA_UG	= 0x1 << 8,
	SRAM_GAMMA_VB	= 0x2 << 8,
	SRAM_PALETTE	= 0x3 << 8,
>>>>>>> upstream/android-13
	SRAM_HWC32_RAM1	= 0xc << 8,
	SRAM_HWC32_RAM2	= 0xd << 8,
	SRAM_HWC32_RAMR	= SRAM_HWC32_RAM1,
	SRAM_HWC32_RAMG	= SRAM_HWC32_RAM2,
	SRAM_HWC32_RAMB	= 0xe << 8,
	SRAM_HWC32_TRAN	= 0xf << 8,
	SRAM_HWC	= 0xf << 8,
};

/* For LCD_SPU_SRAM_PARA1 */
enum {
	CFG_CSB_256x32	= 1 << 15,	/* cursor */
	CFG_CSB_256x24	= 1 << 14,	/* palette */
	CFG_CSB_256x8	= 1 << 13,	/* gamma */
	CFG_PDWN1920x32	= 1 << 8,	/* Armada 510: power down vscale ram */
	CFG_PDWN256x32	= 1 << 7,	/* power down cursor */
	CFG_PDWN256x24	= 1 << 6,	/* power down palette */
	CFG_PDWN256x8	= 1 << 5,	/* power down gamma */
	CFG_PDWNHWC	= 1 << 4,	/* Armada 510: power down all hwc ram */
	CFG_PDWN32x32	= 1 << 3,	/* power down slave->smart ram */
	CFG_PDWN16x66	= 1 << 2,	/* power down UV fifo */
	CFG_PDWN32x66	= 1 << 1,	/* power down Y fifo */
	CFG_PDWN64x66	= 1 << 0,	/* power down graphic fifo */
};

/* For LCD_CFG_SCLK_DIV */
enum {
	/* Armada 510 */
	SCLK_510_AXI		= 0x0 << 30,
	SCLK_510_EXTCLK0	= 0x1 << 30,
	SCLK_510_PLL		= 0x2 << 30,
	SCLK_510_EXTCLK1	= 0x3 << 30,
	SCLK_510_DIV_CHANGE	= 1 << 29,
	SCLK_510_FRAC_DIV_MASK	= 0xfff << 16,
	SCLK_510_INT_DIV_MASK	= 0xffff << 0,

	/* Armada 16x */
	SCLK_16X_AHB		= 0x0 << 28,
	SCLK_16X_PCLK		= 0x1 << 28,
	SCLK_16X_AXI		= 0x4 << 28,
	SCLK_16X_PLL		= 0x8 << 28,
	SCLK_16X_FRAC_DIV_MASK	= 0xfff << 16,
	SCLK_16X_INT_DIV_MASK	= 0xffff << 0,
};

/* For LCD_SPU_DUMB_CTRL */
enum {
	DUMB16_RGB565_0	= 0x0 << 28,
	DUMB16_RGB565_1	= 0x1 << 28,
	DUMB18_RGB666_0	= 0x2 << 28,
	DUMB18_RGB666_1	= 0x3 << 28,
	DUMB12_RGB444_0	= 0x4 << 28,
	DUMB12_RGB444_1	= 0x5 << 28,
	DUMB24_RGB888_0	= 0x6 << 28,
	DUMB_BLANK	= 0x7 << 28,
	DUMB_MASK	= 0xf << 28,
	CFG_BIAS_OUT	= 1 << 8,
	CFG_REV_RGB	= 1 << 7,
	CFG_INV_CBLANK	= 1 << 6,
	CFG_INV_CSYNC	= 1 << 5,	/* Normally active high */
	CFG_INV_HENA	= 1 << 4,
	CFG_INV_VSYNC	= 1 << 3,	/* Normally active high */
	CFG_INV_HSYNC	= 1 << 2,	/* Normally active high */
	CFG_INV_PCLK	= 1 << 1,
	CFG_DUMB_ENA	= 1 << 0,
};

/* For LCD_SPU_IOPAD_CONTROL */
enum {
	CFG_VSCALE_LN_EN	= 3 << 18,
	CFG_GRA_VM_ENA		= 1 << 15,
	CFG_DMA_VM_ENA		= 1 << 13,
	CFG_CMD_VM_ENA		= 1 << 11,
	CFG_CSC_MASK		= 3 << 8,
	CFG_CSC_YUV_CCIR709	= 1 << 9,
	CFG_CSC_YUV_CCIR601	= 0 << 9,
	CFG_CSC_RGB_STUDIO	= 1 << 8,
	CFG_CSC_RGB_COMPUTER	= 0 << 8,
	CFG_IOPAD_MASK		= 0xf << 0,
	CFG_IOPAD_DUMB24	= 0x0 << 0,
	CFG_IOPAD_DUMB18SPI	= 0x1 << 0,
	CFG_IOPAD_DUMB18GPIO	= 0x2 << 0,
	CFG_IOPAD_DUMB16SPI	= 0x3 << 0,
	CFG_IOPAD_DUMB16GPIO	= 0x4 << 0,
	CFG_IOPAD_DUMB12GPIO	= 0x5 << 0,
	CFG_IOPAD_SMART18	= 0x6 << 0,
	CFG_IOPAD_SMART16	= 0x7 << 0,
	CFG_IOPAD_SMART8	= 0x8 << 0,
};

#define IOPAD_DUMB24                0x0

/* For LCD_SPU_IRQ_ENA */
enum {
	DMA_FRAME_IRQ0_ENA	= 1 << 31,
	DMA_FRAME_IRQ1_ENA	= 1 << 30,
	DMA_FRAME_IRQ_ENA	= DMA_FRAME_IRQ0_ENA | DMA_FRAME_IRQ1_ENA,
	DMA_FF_UNDERFLOW_ENA	= 1 << 29,
	GRA_FRAME_IRQ0_ENA	= 1 << 27,
	GRA_FRAME_IRQ1_ENA	= 1 << 26,
	GRA_FRAME_IRQ_ENA	= GRA_FRAME_IRQ0_ENA | GRA_FRAME_IRQ1_ENA,
	GRA_FF_UNDERFLOW_ENA	= 1 << 25,
	VSYNC_IRQ_ENA		= 1 << 23,
	DUMB_FRAMEDONE_ENA	= 1 << 22,
	TWC_FRAMEDONE_ENA	= 1 << 21,
	HWC_FRAMEDONE_ENA	= 1 << 20,
	SLV_IRQ_ENA		= 1 << 19,
	SPI_IRQ_ENA		= 1 << 18,
	PWRDN_IRQ_ENA		= 1 << 17,
	ERR_IRQ_ENA		= 1 << 16,
	CLEAN_SPU_IRQ_ISR	= 0xffff,
};

/* For LCD_SPU_IRQ_ISR */
enum {
	DMA_FRAME_IRQ0		= 1 << 31,
	DMA_FRAME_IRQ1		= 1 << 30,
	DMA_FRAME_IRQ		= DMA_FRAME_IRQ0 | DMA_FRAME_IRQ1,
	DMA_FF_UNDERFLOW	= 1 << 29,
	GRA_FRAME_IRQ0		= 1 << 27,
	GRA_FRAME_IRQ1		= 1 << 26,
	GRA_FRAME_IRQ		= GRA_FRAME_IRQ0 | GRA_FRAME_IRQ1,
	GRA_FF_UNDERFLOW	= 1 << 25,
	VSYNC_IRQ		= 1 << 23,
	DUMB_FRAMEDONE		= 1 << 22,
	TWC_FRAMEDONE		= 1 << 21,
	HWC_FRAMEDONE		= 1 << 20,
	SLV_IRQ			= 1 << 19,
	SPI_IRQ			= 1 << 18,
	PWRDN_IRQ		= 1 << 17,
	ERR_IRQ			= 1 << 16,
	DMA_FRAME_IRQ0_LEVEL	= 1 << 15,
	DMA_FRAME_IRQ1_LEVEL	= 1 << 14,
	DMA_FRAME_CNT_ISR	= 3 << 12,
	GRA_FRAME_IRQ0_LEVEL	= 1 << 11,
	GRA_FRAME_IRQ1_LEVEL	= 1 << 10,
	GRA_FRAME_CNT_ISR	= 3 << 8,
	VSYNC_IRQ_LEVEL		= 1 << 7,
	DUMB_FRAMEDONE_LEVEL	= 1 << 6,
	TWC_FRAMEDONE_LEVEL	= 1 << 5,
	HWC_FRAMEDONE_LEVEL	= 1 << 4,
	SLV_FF_EMPTY		= 1 << 3,
	DMA_FF_ALLEMPTY		= 1 << 2,
	GRA_FF_ALLEMPTY		= 1 << 1,
	PWRDN_IRQ_LEVEL		= 1 << 0,
};

<<<<<<< HEAD
static inline u32 armada_rect_hw_fp(struct drm_rect *r)
{
	return (drm_rect_height(r) & 0xffff0000) | drm_rect_width(r) >> 16;
}

static inline u32 armada_rect_hw(struct drm_rect *r)
{
	return drm_rect_height(r) << 16 | (drm_rect_width(r) & 0x0000ffff);
}

static inline u32 armada_rect_yx(struct drm_rect *r)
{
	return (r)->y1 << 16 | ((r)->x1 & 0x0000ffff);
}

=======
>>>>>>> upstream/android-13
#endif
