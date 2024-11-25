

#ifndef SEC_EXT_H
#define SEC_EXT_H



#ifdef CONFIG_SEC_BOOTSTAT
extern bool init_command_debug;
extern int board_rev;

extern void sec_boot_stat_get_start_kernel(void);
extern void sec_boot_stat_get_mct(u32);
extern void sec_boot_stat_add(const char *);
extern void sec_boot_stat_add_initcall(const char *);
extern void sec_boot_stat_add_init_command(const char *name,
	int duration);
#ifdef CONFIG_SEC_DEVICE_BOOTSTAT
#define DEVICE_INIT_TIME_100MS 100000
extern struct list_head device_init_time_list;
struct device_init_time_entry {
	struct list_head next;
	char *buf;
	unsigned long long duration;
};
#endif
#define MAX_LENGTH_OF_SYSTEMSERVER_LOG 90
struct systemserver_init_time_entry {
	struct list_head next;
	char buf[MAX_LENGTH_OF_SYSTEMSERVER_LOG];
};
#endif 


#ifdef CONFIG_SEC_PARAM
#define CM_OFFSET				CONFIG_CM_OFFSET
#define CM_OFFSET_LIMIT				CM_OFFSET+4
#define WC_OFFSET				CONFIG_WC_OFFSET
#define WC_OFFSET_LIMIT				0
#define FMM_LOCK_OFFSET				CONFIG_FMM_LOCK_OFFSET

enum {
	PARAM_OFF = '0',
	PARAM_ON = '1',
};

extern int sec_set_param(unsigned long offset, char val);
extern int sec_set_param_str(unsigned long offset, const char *val, int size);
extern int sec_set_param_extra(unsigned long offset, void *extra, size_t size);
extern int sec_get_param_str(unsigned long offset, char *val);
extern void sec_debug_recovery_reboot(void);
#else
#define sec_set_param(a, b)			(-1)
#endif 

#endif 
