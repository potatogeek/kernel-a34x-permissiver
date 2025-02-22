

#ifndef __BATTERY_NOTIFIER_H__
#define __BATTERY_NOTIFIER_H__
#define MAX_PDO_NUM 8
#define AVAILABLE_VOLTAGE 9000
#define UNIT_FOR_VOLTAGE 50
#define UNIT_FOR_CURRENT 10
#define UNIT_FOR_APDO_VOLTAGE 100
#define UNIT_FOR_APDO_CURRENT 50

typedef enum {
	CHARGER_NOTIFY = 0,
	PDIC_NOTIFY,
} battery_notifier_id_t;


typedef enum {
	CHARGER_NOTIFY_DEV_BATTERY = 0,
	CHARGER_NOTIFY_DEV_MULTICHARGER,
	CHARGER_NOTIFY_DEV_CHARGER,
	CHARGER_NOTIFY_DEV_FUELGAUGE,
} charger_notifier_device_t;

typedef enum {
	CHARGER_NOTIFY_EVENT_INITIAL = 0,
	CHARGER_NOTIFY_EVENT_AICL,
} charger_notifier_event_t;

typedef struct _charger_aicl_status {
	int input_current;
	int charging_current;
} CHARGER_AICL_STATUS;

struct charger_notifier_struct {
	charger_notifier_event_t event;
	CHARGER_AICL_STATUS aicl_status;
	struct blocking_notifier_head notifier_call_chain;
};


extern void charger_notifier_call(struct charger_notifier_struct *value);
extern int charger_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, charger_notifier_device_t listener);
extern int charger_notifier_unregister(struct notifier_block *nb);





typedef enum {
	PDIC_NOTIFY_DEV_BATTERY = 0,
	PDIC_NOTIFY_DEV_CHARGER,
	PDIC_NOTIFY_DEV_FUELGAUGE,
	PDIC_NOTIFY_DEV_MULTICHARGER,
} pdic_notifier_device_t;

typedef enum {
	PDIC_NOTIFY_EVENT_DETACH = 0,
	PDIC_NOTIFY_EVENT_PDIC_ATTACH,
	PDIC_NOTIFY_EVENT_PD_SINK,
	PDIC_NOTIFY_EVENT_PD_SOURCE,
	PDIC_NOTIFY_EVENT_PD_SINK_CAP,
	PDIC_NOTIFY_EVENT_PD_PRSWAP_SNKTOSRC,
	PDIC_NOTIFY_EVENT_PD_PRSWAP_SRCTOSNK,
} pdic_notifier_event_t;

typedef enum
{
	FPDO_TYPE = 0,
	APDO_TYPE,
	VPDO_TYPE,
} PDO_TYPE_T;

typedef enum {
	AUTH_NONE = 0,
	AUTH_LOW_PWR,
	AUTH_HIGH_PWR,
} AUTH_TYPE_T;

typedef struct _power_list {
	int accept;
	int max_voltage;
	int min_voltage;
	int max_current;
	int apdo;
	int pdo_type;
	int comm_capable;
	int suspend;
} POWER_LIST;

typedef enum
{
	RP_CURRENT_LEVEL_NONE = 0,
	RP_CURRENT_LEVEL_DEFAULT,
	RP_CURRENT_LEVEL2,
	RP_CURRENT_LEVEL3,
	RP_CURRENT_ABNORMAL,
} RP_CURRENT_LEVEL;

typedef struct _pdic_sink_status {
	POWER_LIST power_list[MAX_PDO_NUM+1];
	int available_pdo_num; // the number of available PDO
	int selected_pdo_num; // selected number of PDO to change
	int current_pdo_num; // current number of PDO
	int pps_voltage;
	int pps_current;
	int request_apdo; // apdo for pps communication
	int has_apdo; // pd source has apdo or not
	unsigned int rp_currentlvl; // rp current level by pdic
} PDIC_SINK_STATUS;

struct pdic_notifier_struct {
	pdic_notifier_event_t event;
	PDIC_SINK_STATUS sink_status;
	struct blocking_notifier_head notifier_call_chain;
	void *pusbpd;
};

extern void pdic_notifier_call(struct pdic_notifier_struct *value);
extern int bat_pdic_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, pdic_notifier_device_t listener);
extern int bat_pdic_notifier_unregister(struct notifier_block *nb);
#endif 
