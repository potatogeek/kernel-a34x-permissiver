

#ifndef __ASM_MACH_PXA168FB_H
#define __ASM_MACH_PXA168FB_H

#include <linux/fb.h>
#include <linux/interrupt.h>


#define PIN_MODE_DUMB_24		0
#define PIN_MODE_DUMB_18_SPI		1
#define PIN_MODE_DUMB_18_GPIO		2
#define PIN_MODE_DUMB_16_SPI		3
#define PIN_MODE_DUMB_16_GPIO		4
#define PIN_MODE_DUMB_12_SPI_GPIO	5
#define PIN_MODE_SMART_18_SPI		6
#define PIN_MODE_SMART_16_SPI		7
#define PIN_MODE_SMART_8_SPI_GPIO	8


#define DUMB_MODE_RGB565		0
#define DUMB_MODE_RGB565_UPPER		1
#define DUMB_MODE_RGB666		2
#define DUMB_MODE_RGB666_UPPER		3
#define DUMB_MODE_RGB444		4
#define DUMB_MODE_RGB444_UPPER		5
#define DUMB_MODE_RGB888		6


#define DEFAULT_FB_SIZE	(800 * 480 * 4)


#define PIX_FMT_RGB565		0
#define PIX_FMT_BGR565		1
#define PIX_FMT_RGB1555		2
#define PIX_FMT_BGR1555		3
#define PIX_FMT_RGB888PACK	4
#define PIX_FMT_BGR888PACK	5
#define PIX_FMT_RGB888UNPACK	6
#define PIX_FMT_BGR888UNPACK	7
#define PIX_FMT_RGBA888		8
#define PIX_FMT_BGRA888		9
#define PIX_FMT_YUV422PACK	10
#define PIX_FMT_YVU422PACK	11
#define PIX_FMT_YUV422PLANAR	12
#define PIX_FMT_YVU422PLANAR	13
#define PIX_FMT_YUV420PLANAR	14
#define PIX_FMT_YVU420PLANAR	15
#define PIX_FMT_PSEUDOCOLOR	20
#define PIX_FMT_UYVY422PACK	(0x1000|PIX_FMT_YUV422PACK)


struct pxa168fb_info {
	struct device		*dev;
	struct clk		*clk;
	struct fb_info		*info;

	void __iomem		*reg_base;
	dma_addr_t		fb_start_dma;
	u32			pseudo_palette[16];

	int			pix_fmt;
	unsigned		is_blanked:1;
	unsigned		panel_rbswap:1;
	unsigned		active:1;
};


struct pxa168fb_mach_info {
	char	id[16];

	int		num_modes;
	struct fb_videomode *modes;

	
	unsigned	pix_fmt;

	
	unsigned	io_pin_allocation_mode:4;

	
	unsigned	dumb_mode:4;
	unsigned	panel_rgb_reverse_lanes:1;

	
	unsigned	gpio_output_mask:8;
	unsigned	gpio_output_data:8;

	
	unsigned	invert_composite_blank:1;
	unsigned	invert_pix_val_ena:1;
	unsigned	invert_pixclock:1;
	unsigned	panel_rbswap:1;
	unsigned	active:1;
	unsigned	enable_lcd:1;
};

#endif 
