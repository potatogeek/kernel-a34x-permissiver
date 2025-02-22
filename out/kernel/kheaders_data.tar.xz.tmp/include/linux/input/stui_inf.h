

#ifndef __LINUX_SAMSUNG_TUI_INF_H
#define __LINUX_SAMSUNG_TUI_INF_H

#define STUI_TSP_TYPE_SLSI	0
#define STUI_TSP_TYPE_ILITEK	1
#define STUI_TSP_TYPE_NOVATEK	2
#define STUI_TSP_TYPE_MELFAS	3
#define STUI_TSP_TYPE_STM	4
#define STUI_TSP_TYPE_ZINITIX	5
#define STUI_TSP_TYPE_SNAPTICS	6
#define STUI_TSP_TYPE_GOODIX	7
#define STUI_TSP_TYPE_FOCALTECH	8
#define STUI_TSP_TYPE_HIMAX	9
#define STUI_TSP_TYPE_OVT	10

#define STUI_MODE_OFF           0x00
#define STUI_MODE_TUI_SESSION   0x01
#define STUI_MODE_DISPLAY_SEC   0x02
#define STUI_MODE_TOUCH_SEC     0x04
#define STUI_MODE_ALL           (STUI_MODE_TUI_SESSION | STUI_MODE_DISPLAY_SEC | STUI_MODE_TOUCH_SEC)

int  stui_inc_blank_ref(void);
int  stui_dec_blank_ref(void);
int  stui_get_blank_ref(void);
void stui_set_blank_ref(int ref);

int  stui_get_mode(void);
void stui_set_mode(int mode);

int  stui_set_mask(int mask);
int  stui_clear_mask(int mask);

int stui_cancel_session(void);
int stui_register_on_events(void);
void stui_unregister_from_events(void);

int stui_set_info(int (*tsp_enter_cb)(void), int (*tsp_exit_cb)(void), uint32_t touch_type);
void stui_set_touch_type(uint32_t type);
uint32_t stui_get_touch_type(void);

#endif 
