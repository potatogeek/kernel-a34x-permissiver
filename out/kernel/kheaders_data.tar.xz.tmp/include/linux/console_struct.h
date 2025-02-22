/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_CONSOLE_STRUCT_H
#define _LINUX_CONSOLE_STRUCT_H

#include <linux/wait.h>
#include <linux/vt.h>
#include <linux/workqueue.h>

struct uni_pagedir;
struct uni_screen;

#define NPAR 16


struct vc_data {
	struct tty_port port;			

	unsigned short	vc_num;			
	unsigned int	vc_cols;		
	unsigned int	vc_rows;
	unsigned int	vc_size_row;		
	unsigned int	vc_scan_lines;		
	unsigned long	vc_origin;		
	unsigned long	vc_scr_end;		
	unsigned long	vc_visible_origin;	
	unsigned int	vc_top, vc_bottom;	
	const struct consw *vc_sw;
	unsigned short	*vc_screenbuf;		
	unsigned int	vc_screenbuf_size;
	unsigned char	vc_mode;		
	
	unsigned char	vc_attr;		
	unsigned char	vc_def_color;		
	unsigned char	vc_color;		
	unsigned char	vc_s_color;		
	unsigned char	vc_ulcolor;		
	unsigned char   vc_itcolor;
	unsigned char	vc_halfcolor;		
	
	unsigned int	vc_cursor_type;
	unsigned short	vc_complement_mask;	
	unsigned short	vc_s_complement_mask;	
	unsigned int	vc_x, vc_y;		
	unsigned int	vc_saved_x, vc_saved_y;
	unsigned long	vc_pos;			
		
	unsigned short	vc_hi_font_mask;	
	struct console_font vc_font;		
	unsigned short	vc_video_erase_char;	
	
	unsigned int	vc_state;		
	unsigned int	vc_npar,vc_par[NPAR];	
	
	struct vt_mode	vt_mode;
	struct pid 	*vt_pid;
	int		vt_newvt;
	wait_queue_head_t paste_wait;
	
	unsigned int	vc_charset	: 1;	
	unsigned int	vc_s_charset	: 1;	
	unsigned int	vc_disp_ctrl	: 1;	
	unsigned int	vc_toggle_meta	: 1;	
	unsigned int	vc_decscnm	: 1;	
	unsigned int	vc_decom	: 1;	
	unsigned int	vc_decawm	: 1;	
	unsigned int	vc_deccm	: 1;	
	unsigned int	vc_decim	: 1;	
	
	unsigned int	vc_intensity	: 2;	
	unsigned int    vc_italic:1;
	unsigned int	vc_underline	: 1;
	unsigned int	vc_blink	: 1;
	unsigned int	vc_reverse	: 1;
	unsigned int	vc_s_intensity	: 2;	
	unsigned int    vc_s_italic:1;
	unsigned int	vc_s_underline	: 1;
	unsigned int	vc_s_blink	: 1;
	unsigned int	vc_s_reverse	: 1;
	
	unsigned int	vc_ques		: 1;
	unsigned int	vc_need_wrap	: 1;
	unsigned int	vc_can_do_color	: 1;
	unsigned int	vc_report_mouse : 2;
	unsigned char	vc_utf		: 1;	
	unsigned char	vc_utf_count;
		 int	vc_utf_char;
	unsigned int	vc_tab_stop[8];		
	unsigned char   vc_palette[16*3];       
	unsigned short * vc_translate;
	unsigned char 	vc_G0_charset;
	unsigned char 	vc_G1_charset;
	unsigned char 	vc_saved_G0;
	unsigned char 	vc_saved_G1;
	unsigned int    vc_resize_user;         
	unsigned int	vc_bell_pitch;		
	unsigned int	vc_bell_duration;	
	unsigned short	vc_cur_blink_ms;	
	struct vc_data **vc_display_fg;		
	struct uni_pagedir *vc_uni_pagedir;
	struct uni_pagedir **vc_uni_pagedir_loc; 
	struct uni_screen *vc_uni_screen;	
	bool vc_panic_force_write; 
	
};

struct vc {
	struct vc_data *d;
	struct work_struct SAK_work;

	
};

extern struct vc vc_cons [MAX_NR_CONSOLES];
extern void vc_SAK(struct work_struct *work);

#define CUR_DEF		0
#define CUR_NONE	1
#define CUR_UNDERLINE	2
#define CUR_LOWER_THIRD	3
#define CUR_LOWER_HALF	4
#define CUR_TWO_THIRDS	5
#define CUR_BLOCK	6
#define CUR_HWMASK	0x0f
#define CUR_SWMASK	0xfff0

#define CUR_DEFAULT CUR_UNDERLINE

static inline bool con_is_visible(const struct vc_data *vc)
{
	return *vc->vc_display_fg == vc;
}

#endif 
