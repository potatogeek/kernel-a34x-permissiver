

#ifndef __LINUX_SAMSUNG_TUI_INF_H
#define __LINUX_SAMSUNG_TUI_INF_H

#define STUI_MODE_OFF			0x00
#define STUI_MODE_TUI_SESSION		0x01
#define STUI_MODE_DISPLAY_SEC		0x02
#define STUI_MODE_TOUCH_SEC		0x04
#define STUI_MODE_ALL			(STUI_MODE_TUI_SESSION | STUI_MODE_DISPLAY_SEC | STUI_MODE_TOUCH_SEC)

int  stui_inc_blank_ref(void);
int  stui_dec_blank_ref(void);
int  stui_get_blank_ref(void);
void stui_set_blank_ref(int ref);

int  stui_get_mode(void);
void stui_set_mode(int mode);

int  stui_set_mask(int mask);
int  stui_clear_mask(int mask);

#endif 
