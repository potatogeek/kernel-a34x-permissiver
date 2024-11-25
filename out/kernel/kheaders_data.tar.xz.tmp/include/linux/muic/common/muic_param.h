

#ifndef __LINUX_MUIC_PARAM_H__
#define __LINUX_MUIC_PARAM_H__

#if IS_ENABLED(CONFIG_MUIC_NOTIFIER)
extern int get_switch_sel(void);
extern int get_uart_sel(void);
extern int get_afc_mode(void);
extern int get_pdic_info(void);
#else
static inline int get_switch_sel(void) {return 0; }
static inline int get_uart_sel(void) {return 0; }
static inline int get_afc_mode(void) {return 0; }
static inline int get_pdic_info(void) {return 1; }
#endif
#endif 
