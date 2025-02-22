/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _UVESAFB_H
#define _UVESAFB_H

#include <uapi/video/uvesafb.h>



struct vbe_crtc_ib {
	u16 horiz_total;
	u16 horiz_start;
	u16 horiz_end;
	u16 vert_total;
	u16 vert_start;
	u16 vert_end;
	u8  flags;
	u32 pixel_clock;
	u16 refresh_rate;
	u8  reserved[40];
} __attribute__ ((packed));

#define VBE_MODE_VGACOMPAT	0x20
#define VBE_MODE_COLOR		0x08
#define VBE_MODE_SUPPORTEDHW	0x01
#define VBE_MODE_GRAPHICS	0x10
#define VBE_MODE_LFB		0x80

#define VBE_MODE_MASK		(VBE_MODE_COLOR | VBE_MODE_SUPPORTEDHW | \
				VBE_MODE_GRAPHICS | VBE_MODE_LFB)


struct vbe_mode_ib {
	
	u16 mode_attr;
	u8  winA_attr;
	u8  winB_attr;
	u16 win_granularity;
	u16 win_size;
	u16 winA_seg;
	u16 winB_seg;
	u32 win_func_ptr;
	u16 bytes_per_scan_line;

	
	u16 x_res;
	u16 y_res;
	u8  x_char_size;
	u8  y_char_size;
	u8  planes;
	u8  bits_per_pixel;
	u8  banks;
	u8  memory_model;
	u8  bank_size;
	u8  image_pages;
	u8  reserved1;

	
	
	u8  red_len;
	u8  red_off;
	u8  green_len;
	u8  green_off;
	u8  blue_len;
	u8  blue_off;
	u8  rsvd_len;
	u8  rsvd_off;
	u8  direct_color_info;	

	
	u32 phys_base_ptr;
	u8  reserved2[6];

	
	u16 lin_bytes_per_scan_line;
	u8  bnk_image_pages;
	u8  lin_image_pages;
	u8  lin_red_len;
	u8  lin_red_off;
	u8  lin_green_len;
	u8  lin_green_off;
	u8  lin_blue_len;
	u8  lin_blue_off;
	u8  lin_rsvd_len;
	u8  lin_rsvd_off;
	u32 max_pixel_clock;
	u16 mode_id;
	u8  depth;
} __attribute__ ((packed));

#define UVESAFB_DEFAULT_MODE "640x480-16"


#define UVESAFB_TIMEOUT 5000


#define UVESAFB_TASKS_MAX 16

#define dac_reg	(0x3c8)
#define dac_val	(0x3c9)

struct uvesafb_pal_entry {
	u_char blue, green, red, pad;
} __attribute__ ((packed));

struct uvesafb_ktask {
	struct uvesafb_task t;
	void *buf;
	struct completion *done;
	u32 ack;
};

static int uvesafb_exec(struct uvesafb_ktask *tsk);

#define UVESAFB_EXACT_RES	1
#define UVESAFB_EXACT_DEPTH	2

struct uvesafb_par {
	struct vbe_ib vbe_ib;		
	struct vbe_mode_ib *vbe_modes;	
	int vbe_modes_cnt;

	u8 nocrtc;
	u8 ypan;			
	u8 pmi_setpal;			
	u16 *pmi_base;			
	void *pmi_start;
	void *pmi_pal;
	u8 *vbe_state_orig;		
	u8 *vbe_state_saved;		
	int vbe_state_size;
	atomic_t ref_count;

	int mode_idx;
	struct vbe_crtc_ib crtc;
	int mtrr_handle;
};

#endif 
