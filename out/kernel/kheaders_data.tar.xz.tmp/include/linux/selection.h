/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_SELECTION_H_
#define _LINUX_SELECTION_H_

#include <linux/tiocl.h>
#include <linux/vt_buffer.h>

struct tty_struct;

struct tty_struct;
struct vc_data;

extern void clear_selection(void);
extern int set_selection(const struct tiocl_selection __user *sel, struct tty_struct *tty);
extern int paste_selection(struct tty_struct *tty);
extern int sel_loadlut(char __user *p);
extern int mouse_reporting(void);
extern void mouse_report(struct tty_struct * tty, int butt, int mrx, int mry);

bool vc_is_sel(struct vc_data *vc);

extern int console_blanked;

extern const unsigned char color_table[];
extern unsigned char default_red[];
extern unsigned char default_grn[];
extern unsigned char default_blu[];

extern unsigned short *screen_pos(struct vc_data *vc, int w_offset, int viewed);
extern u16 screen_glyph(struct vc_data *vc, int offset);
extern u32 screen_glyph_unicode(struct vc_data *vc, int offset);
extern void complement_pos(struct vc_data *vc, int offset);
extern void invert_screen(struct vc_data *vc, int offset, int count, int shift);

extern void getconsxy(struct vc_data *vc, unsigned char *p);
extern void putconsxy(struct vc_data *vc, unsigned char *p);

extern u16 vcs_scr_readw(struct vc_data *vc, const u16 *org);
extern void vcs_scr_writew(struct vc_data *vc, u16 val, u16 *org);
extern void vcs_scr_updated(struct vc_data *vc);

extern int vc_uniscr_check(struct vc_data *vc);
extern void vc_uniscr_copy_line(struct vc_data *vc, void *dest, int viewed,
				unsigned int row, unsigned int col,
				unsigned int nr);

#endif
