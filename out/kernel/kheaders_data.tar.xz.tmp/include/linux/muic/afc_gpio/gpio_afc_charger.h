#ifndef _AFC_CHARGER_INTF
#define _AFC_CHARGER_INTF

#include <linux/kernel.h>
#include <linux/kthread.h>


#define UI				160
#define MPING			(UI << 4)
#define DATA_DELAY		(UI << 3)
#define SYNC_PULSE		(UI >> 2)
#define RESET_DELAY		(UI * 100)

#define AFC_RETRY_CNT			5
#define AFC_RETRY_MAX			10
#define VBUS_RETRY_MAX		5
#define AFC_SPING_CNT			7
#define AFC_SPING_MIN			2
#define AFC_SPING_MAX			20

enum GPIO_AFC_HICCUP_MODE {
	SEC_HICCUP_MODE_OFF	=	0,
	SEC_HICCUP_MODE_ON,
};

struct gpio_afc_pdata {
	int gpio_afc_switch;
	int gpio_afc_data;
};

struct gpio_afc_ddata {
	struct device *dev;
	struct gpio_afc_pdata *pdata;
	struct wakeup_source *ws;
	struct mutex mutex;
	struct kthread_worker kworker;
	struct kthread_work kwork;
	spinlock_t spin_lock;
	bool gpio_input;
	bool check_performance;
	int afc_disable;
	int curr_voltage;
	int set_voltage;
	int is_hiccup_mode;
#if IS_ENABLED(CONFIG_MUIC_NOTIFIER)
	struct muic_platform_data *muic_pdata;
#endif
};

#ifdef CONFIG_AFC_CHARGER
extern int set_afc_voltage(int voltage);
extern void set_afc_voltage_for_performance(bool enable);
#else 
static inline int set_afc_voltage(int voltage);
{
	return -ENOTSUPP;
}
#endif 
#if IS_ENABLED(CONFIG_SEC_HICCUP)
void set_sec_hiccup(bool en);
#endif 
#endif 
