
#ifndef _VIDEO_SA1100FB_H
#define _VIDEO_SA1100FB_H

#include <linux/fb.h>
#include <linux/types.h>

#define RGB_4	0
#define RGB_8	1
#define RGB_16	2
#define NR_RGB	3


struct sa1100fb_rgb {
	struct fb_bitfield	red;
	struct fb_bitfield	green;
	struct fb_bitfield	blue;
	struct fb_bitfield	transp;
};


struct sa1100fb_mach_info {
	u_long		pixclock;

	u_short		xres;
	u_short		yres;

	u_char		bpp;
	u_char		hsync_len;
	u_char		left_margin;
	u_char		right_margin;

	u_char		vsync_len;
	u_char		upper_margin;
	u_char		lower_margin;
	u_char		sync;

	u_int		cmap_greyscale:1,
			cmap_inverse:1,
			cmap_static:1,
			unused:29;

	u_int		lccr0;
	u_int		lccr3;

	
	const struct sa1100fb_rgb *rgb[NR_RGB];

	void (*backlight_power)(int);
	void (*lcd_power)(int);
	void (*set_visual)(u32);
};

#endif
