

#ifndef SEC_ABC_HUB_H
#define SEC_ABC_HUB_H

#include <linux/sti/abc_common.h>






#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)

struct sub_cond_pdata {
	// common
	int init;
	int enabled;
};
#endif

#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
#define BOOTC_OFFSET_DATA_CNT 1
#define BOOTC_OFFSET_STR_MAX 100

struct abc_hub_bootc_offset_data {
	char module[BOOTC_OFFSET_STR_MAX];
	int offset;
};

struct sub_bootc_pdata {
	// common
	int init;
	int enabled;
	int bootc_time;
	// custom
	int time_spec;
	int time_spec_offset;
	struct abc_hub_bootc_offset_data offset_data[BOOTC_OFFSET_DATA_CNT];
	struct workqueue_struct *workqueue;
	struct delayed_work bootc_work;
};
#endif




enum {
	ABC_HUB_DISABLED,
	ABC_HUB_ENABLED,
};

struct abc_hub_platform_data {
	unsigned int nSub;
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)
	struct sub_cond_pdata cond;
#endif
#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
	struct sub_bootc_pdata bootc_pdata;
#endif
};

struct abc_hub_info {
	struct device *dev;
	struct abc_hub_platform_data *pdata;
	int enabled;
};






#if IS_ENABLED(CONFIG_SEC_ABC_HUB_COND)
void abc_hub_cond_enable(struct device *dev, int enable);
#endif


#if IS_ENABLED(CONFIG_SEC_ABC_HUB_BOOTC)
int parse_bootc_data(struct device *dev,
		     struct abc_hub_platform_data *pdata,
		     struct device_node *np);
int abc_hub_bootc_init(struct device *dev);
void abc_hub_bootc_enable(struct device *dev, int enable);
#endif

extern struct abc_hub_info *abc_hub_pinfo;

void abc_hub_send_event(char *str);
int abc_hub_get_enabled(void);

#endif
