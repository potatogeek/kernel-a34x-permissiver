

#ifndef __LINUX_PDIC_PARAM_H__
#define __LINUX_PDIC_PARAM_H__

enum pdic_param_usbmode {
	PDIC_PARAM_MODE_NO,
	PDIC_PARAM_MODE_OB,
	PDIC_PARAM_MODE_IB,
	PDIC_PARAM_MODE_DL,
	PDIC_PARAM_MODE_LC,
};

#if IS_ENABLED(CONFIG_PDIC_NOTIFIER)
extern int check_factory_mode_boot(void);
extern int get_usb_factory_mode(void);
extern int is_lpcharge_pdic_param(void);
extern int is_factory_mode_pdic_param(void);
#else
static inline int check_factory_mode_boot(void)
	{return 0;}
static inline int get_usb_factory_mode(void)
	{return PDIC_PARAM_MODE_NO;}
static inline int is_lpcharge_pdic_param(void)
	{return 0;}
static inline int is_factory_mode_pdic_param(void)
	{return 0;}
#endif
#endif 
