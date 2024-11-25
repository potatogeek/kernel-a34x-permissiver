

#ifndef __MUIC_CORE_H__
#define __MUIC_CORE_H__

#ifdef CONFIG_IFCONN_NOTIFIER
#include <linux/ifconn/ifconn_notifier.h>
#endif

#define MUIC_CORE "MUIC_CORE"

enum {
	MUIC_SUSPEND		= 0,
	MUIC_RESUME,
};


enum {
	MUIC_INTR_DETACH	= 0,
	MUIC_INTR_ATTACH
};


enum {
	MUIC_DOCK_DETACHED	= 0,
	MUIC_DOCK_DESKDOCK	= 1,
	MUIC_DOCK_CARDOCK	= 2,
	MUIC_DOCK_AUDIODOCK	= 7,
	MUIC_DOCK_SMARTDOCK	= 8,
	MUIC_DOCK_HMT		= 11,
	MUIC_DOCK_ABNORMAL      = 106,
	MUIC_DOCK_GAMEPAD       = 107,
	MUIC_DOCK_GAMEPAD_WITH_EARJACK  = 108,
};


enum {
	MUIC_PATH_USB_AP	= 0,
	MUIC_PATH_USB_CP,
	MUIC_PATH_UART_AP,
	MUIC_PATH_UART_CP,
	MUIC_PATH_OPEN,
	MUIC_PATH_AUDIO,
};

#ifdef CONFIG_MUIC_HV_FORCE_LIMIT
enum {
	HV_9V = 0,
	HV_5V,
};
#endif


enum {
	SWITCH_SEL_USB_MASK	= 0x1,
	SWITCH_SEL_UART_MASK	= 0x2,
	SWITCH_SEL_RUSTPROOF_MASK	= 0x8,
	SWITCH_SEL_AFC_DISABLE_MASK	= 0x100,
};


enum {
	CH_MODE_AFC_DISABLE_VAL = 0x31, 
};


typedef enum {
	ADC_GND			= 0x00,
	ADC_SEND_END		= 0x01, 
	ADC_REMOTE_S11		= 0x0c, 
	ADC_REMOTE_S12		= 0x0d, 
	ADC_RESERVED_VZW	= 0x0e, 
	ADC_INCOMPATIBLE_VZW	= 0x0f, 
	ADC_SMARTDOCK		= 0x10, 
	ADC_HMT			= 0x11, 
	ADC_AUDIODOCK		= 0x12, 
	ADC_USB_LANHUB		= 0x13, 
	ADC_CHARGING_CABLE	= 0x14,	
	ADC_UNIVERSAL_MMDOCK	= 0x15, 
	ADC_UART_CABLE		= 0x16, 
	ADC_CEA936ATYPE1_CHG	= 0x17,	
	ADC_JIG_USB_OFF		= 0x18, 
	ADC_JIG_USB_ON		= 0x19, 
	ADC_DESKDOCK		= 0x1a, 
	ADC_CEA936ATYPE2_CHG	= 0x1b, 
	ADC_JIG_UART_OFF	= 0x1c, 
	ADC_JIG_UART_ON		= 0x1d, 
	ADC_AUDIOMODE_W_REMOTE	= 0x1e, 
	ADC_OPEN		= 0x1f,
	ADC_OPEN_219		= 0xfb, 
	ADC_219			= 0xfc, 

	ADC_UNDEFINED		= 0xfd, 
	ADC_DONTCARE		= 0xfe, 
	ADC_ERROR		= 0xff, 
} muic_adc_t;


typedef enum {
	ATTACHED_DEV_NONE_MUIC = 0,

	ATTACHED_DEV_USB_MUIC,
	ATTACHED_DEV_CDP_MUIC,
	ATTACHED_DEV_OTG_MUIC,
	ATTACHED_DEV_TA_MUIC,
	ATTACHED_DEV_UNOFFICIAL_MUIC,
	ATTACHED_DEV_UNOFFICIAL_TA_MUIC,
	ATTACHED_DEV_UNOFFICIAL_ID_MUIC,
	ATTACHED_DEV_UNOFFICIAL_ID_TA_MUIC,
	ATTACHED_DEV_UNOFFICIAL_ID_ANY_MUIC,
	ATTACHED_DEV_UNOFFICIAL_ID_USB_MUIC,

	ATTACHED_DEV_UNOFFICIAL_ID_CDP_MUIC,
	ATTACHED_DEV_UNDEFINED_CHARGING_MUIC,
	ATTACHED_DEV_DESKDOCK_MUIC,
	ATTACHED_DEV_UNKNOWN_VB_MUIC,
	ATTACHED_DEV_DESKDOCK_VB_MUIC,
	ATTACHED_DEV_CARDOCK_MUIC,
	ATTACHED_DEV_JIG_UART_OFF_MUIC,
	ATTACHED_DEV_JIG_UART_OFF_VB_MUIC,	
	ATTACHED_DEV_JIG_UART_OFF_VB_OTG_MUIC,	
	ATTACHED_DEV_JIG_UART_OFF_VB_FG_MUIC,	

	ATTACHED_DEV_JIG_UART_ON_MUIC,
	ATTACHED_DEV_JIG_UART_ON_VB_MUIC,	
	ATTACHED_DEV_JIG_USB_OFF_MUIC,
	ATTACHED_DEV_JIG_USB_ON_MUIC,
	ATTACHED_DEV_JIG_RID_OPEN_MUIC,	
	ATTACHED_DEV_SMARTDOCK_MUIC,
	ATTACHED_DEV_SMARTDOCK_VB_MUIC,
	ATTACHED_DEV_SMARTDOCK_TA_MUIC,
	ATTACHED_DEV_SMARTDOCK_USB_MUIC,
	ATTACHED_DEV_UNIVERSAL_MMDOCK_MUIC,

	ATTACHED_DEV_AUDIODOCK_MUIC,
	ATTACHED_DEV_MHL_MUIC,
	ATTACHED_DEV_CHARGING_CABLE_MUIC,
	ATTACHED_DEV_AFC_CHARGER_PREPARE_MUIC,
	ATTACHED_DEV_AFC_CHARGER_PREPARE_DUPLI_MUIC,
	ATTACHED_DEV_AFC_CHARGER_5V_MUIC,
	ATTACHED_DEV_AFC_CHARGER_5V_DUPLI_MUIC,
	ATTACHED_DEV_AFC_CHARGER_9V_MUIC,
	ATTACHED_DEV_AFC_CHARGER_ERR_V_MUIC,
	ATTACHED_DEV_AFC_CHARGER_ERR_V_DUPLI_MUIC,

	ATTACHED_DEV_QC_CHARGER_PREPARE_MUIC,
	ATTACHED_DEV_QC_CHARGER_5V_MUIC,
	ATTACHED_DEV_QC_CHARGER_ERR_V_MUIC,
	ATTACHED_DEV_QC_CHARGER_9V_MUIC,
	ATTACHED_DEV_HV_ID_ERR_UNDEFINED_MUIC,
	ATTACHED_DEV_HV_ID_ERR_UNSUPPORTED_MUIC,
	ATTACHED_DEV_HV_ID_ERR_SUPPORTED_MUIC,
	ATTACHED_DEV_HMT_MUIC,
	ATTACHED_DEV_VZW_ACC_MUIC,
	ATTACHED_DEV_VZW_INCOMPATIBLE_MUIC,

	ATTACHED_DEV_USB_LANHUB_MUIC,
	ATTACHED_DEV_TYPE2_CHG_MUIC,
	ATTACHED_DEV_TYPE3_MUIC,
	ATTACHED_DEV_TYPE3_MUIC_TA,
	ATTACHED_DEV_TYPE3_ADAPTER_MUIC,
	ATTACHED_DEV_TYPE3_CHARGER_MUIC,
	ATTACHED_DEV_NONE_TYPE3_MUIC,
	ATTACHED_DEV_UNSUPPORTED_ID_MUIC,
	ATTACHED_DEV_UNSUPPORTED_ID_VB_MUIC,
	ATTACHED_DEV_TIMEOUT_OPEN_MUIC,

	ATTACHED_DEV_WIRELESS_PAD_MUIC,
#if defined(CONFIG_SEC_FACTORY)
	ATTACHED_DEV_CARKIT_MUIC,
#endif
	ATTACHED_DEV_POWERPACK_MUIC,
	ATTACHED_DEV_UNDEFINED_RANGE_MUIC,
	ATTACHED_DEV_WATER_MUIC,
	ATTACHED_DEV_CHK_WATER_REQ,
	ATTACHED_DEV_CHK_WATER_DRY_REQ,
	ATTACHED_DEV_GAMEPAD_MUIC,
	ATTACHED_DEV_CHECK_OCP,
	ATTACHED_DEV_RDU_TA_MUIC,

	ATTACHED_DEV_AFC_CHARGER_9V_DUPLI_MUIC,
	ATTACHED_DEV_UNKNOWN_MUIC,
	ATTACHED_DEV_NUM,
} muic_attached_dev_t;

#ifdef CONFIG_MUIC_HV_FORCE_LIMIT

typedef enum {
	SILENT_CHG_DONE = 0,
	SILENT_CHG_CHANGING = 1,

	SILENT_CHG_NUM,
} muic_silent_change_state_t;
#endif


struct muic_platform_data {
	void *drv_data;
	void *muic_if;
	int irq_gpio;
	bool suspended;
	bool need_to_noti;

	int switch_sel;

	
	int usb_path;
	int uart_path;

	int gpio_uart_sel;
	int gpio_usb_sel;

	bool rustproof_on;
	bool afc_disable;

#ifdef CONFIG_MUIC_HV_FORCE_LIMIT
	int hv_sel;
	int silent_chg_change_state;
#endif

	
	muic_attached_dev_t	attached_dev;

	bool is_usb_ready;
	bool is_factory_start;
	bool is_rustproof;
	bool is_otg_test;

	bool is_jig_on;

	int vbvolt;
	int adc;
	int vmid;

	
	void (*init_switch_dev_cb)(void);
	void (*cleanup_switch_dev_cb)(void);

	void (*jig_uart_cb)(int jig_state);

	
	int (*init_gpio_cb)(void *, int switch_sel);
	int (*set_gpio_usb_sel)(int usb_path);
	int (*set_gpio_uart_sel)(int uart_path);
	int (*set_safeout)(int safeout_path);

	
	int (*muic_afc_set_voltage_cb)(int voltage);

	
	int (*muic_hv_charger_disable_cb)(bool en);

	
	void (*set_path_switch_suspend)(struct device *dev);
	void (*set_path_switch_resume)(struct device *dev);
};

#define MUIC_PDATA_FUNC(func, param, ret) \
{\
	*ret = -1;	\
	if (func)	\
		*ret = func(param);	\
	else	\
		pr_err("[muic_core] func not defined %s\n", __func__);	\
}

#define MUIC_PDATA_FUNC_MULTI_PARAM(func, param1, param2, ret) \
{					\
	*ret = -1;	\
	if (func)	\
		*ret = func(param1, param2);	\
	else	\
		pr_err("[muic_core] func not defined %s\n", __func__);	\
}

#ifdef CONFIG_IFCONN_NOTIFIER
#define MUIC_SEND_NOTI_ATTACH(dev) \
{	\
	int ret;	\
	ret = ifconn_notifier_notify( \
					IFCONN_NOTIFY_MUIC,	\
					IFCONN_NOTIFY_MANAGER,	\
					IFCONN_NOTIFY_ID_ATTACH,	\
					dev,	\
					IFCONN_NOTIFY_PARAM_DATA,	\
					NULL);	\
	if (ret < 0) {	\
		pr_err("%s: Fail to send noti\n", \
				__func__);	\
	}	\
}

#define MUIC_SEND_NOTI_ATTACH_ALL(dev) \
{	\
	int ret;	\
	ret = ifconn_notifier_notify( \
					IFCONN_NOTIFY_MUIC,	\
					IFCONN_NOTIFY_ALL,	\
					IFCONN_NOTIFY_ID_ATTACH,	\
					dev,	\
					IFCONN_NOTIFY_PARAM_DATA,	\
					NULL);	\
	if (ret < 0) {	\
		pr_err("%s: Fail to send noti\n", \
				__func__);	\
	}	\
}

#define MUIC_SEND_NOTI_DETACH_ALL(dev) \
{	\
	int ret;	\
	ret = ifconn_notifier_notify( \
					IFCONN_NOTIFY_MUIC,	\
					IFCONN_NOTIFY_ALL,	\
					IFCONN_NOTIFY_ID_DETACH,	\
					dev,	\
					IFCONN_NOTIFY_PARAM_DATA,	\
					NULL);	\
	if (ret < 0) {	\
		pr_err("%s: Fail to send noti\n", \
				__func__);	\
	}	\
}

#define MUIC_SEND_NOTI_TO_PDIC_ATTACH(dev) \
{	\
	int ret;	\
	struct ifconn_notifier_template template;	\
	template.cable_type = dev;	\
	ret = ifconn_notifier_notify( \
					IFCONN_NOTIFY_MUIC,	\
					IFCONN_NOTIFY_PDIC,	\
					IFCONN_NOTIFY_ID_ATTACH,	\
					IFCONN_NOTIFY_EVENT_ATTACH,	\
					IFCONN_NOTIFY_PARAM_DATA,	\
					&template);	\
	if (ret < 0) {	\
		pr_err("%s: Fail to send noti\n", \
				__func__);	\
	}	\
}

#define MUIC_SEND_NOTI_TO_PDIC_DETACH(dev) \
{	\
	int ret;	\
	struct ifconn_notifier_template template;	\
	template.cable_type = dev;	\
	ret = ifconn_notifier_notify( \
					IFCONN_NOTIFY_MUIC,	\
					IFCONN_NOTIFY_PDIC,	\
					IFCONN_NOTIFY_ID_DETACH,	\
					IFCONN_NOTIFY_EVENT_DETACH,	\
					IFCONN_NOTIFY_PARAM_DATA,	\
					&template);	\
	if (ret < 0) {	\
		pr_err("%s: Fail to send noti\n", \
				__func__);	\
	}	\
}

#define MUIC_SEND_NOTI_DETACH(dev) \
{	\
	int ret;	\
	struct ifconn_notifier_template template;	\
	template.cable_type = dev;	\
	ret = ifconn_notifier_notify( \
					IFCONN_NOTIFY_MUIC,	\
					IFCONN_NOTIFY_MANAGER,	\
					IFCONN_NOTIFY_ID_DETACH,	\
					IFCONN_NOTIFY_EVENT_DETACH,	\
					IFCONN_NOTIFY_PARAM_DATA,	\
					&template);	\
	if (ret < 0) {	\
		pr_err("%s: Fail to send noti\n", \
				__func__);	\
	}	\
}
#else
#define MUIC_SEND_NOTI_ATTACH(dev)	\
		muic_notifier_attach_attached_dev(dev)
#define MUIC_SEND_NOTI_DETACH(dev) \
		muic_notifier_detach_attached_dev(dev)
#define MUIC_SEND_NOTI_TO_PDIC_ATTACH(dev) \
		muic_pdic_notifier_attach_attached_dev(dev)
#define MUIC_SEND_NOTI_TO_PDIC_DETACH(dev) \
		muic_pdic_notifier_detach_attached_dev(dev)
#endif

int get_switch_sel(void);
int get_afc_mode(void);
void muic_set_hmt_status(int status);
int muic_core_handle_attach(struct muic_platform_data *muic_pdata,
			muic_attached_dev_t new_dev, int adc, u8 vbvolt);
int muic_core_handle_detach(struct muic_platform_data *muic_pdata);
bool muic_core_get_pdic_cable_state(struct muic_platform_data *muic_pdata);
struct muic_platform_data *muic_core_init(void *drv_data);
void muic_core_exit(struct muic_platform_data *muic_pdata);
extern void muic_disable_otg_detect(void);
bool muic_core_is_cable_attached(struct muic_platform_data *muic_pdata);
#endif 
