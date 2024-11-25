

#ifndef __CABLE_TYPE_NOTIFIER_H__
#define __CABLE_TYPE_NOTIFIER_H__

typedef enum {
	CABLE_TYPE_NONE = 0,
	CABLE_TYPE_USB,
	CABLE_TYPE_USB_SDP,
	CABLE_TYPE_USB_CDP,
	CABLE_TYPE_OTG,
	CABLE_TYPE_TA,
	CABLE_TYPE_NONSTANDARD,
	CABLE_TYPE_UNKNOWN,
} cable_type_attached_dev_t;

static const char *cable_type_names[] = {
	"NONE",
	"USB",
	"USB SDP",
	"USB CDP",
	"OTG",
	"TA",
	"NON-STANDARD Charger",
	"UNKNOWN",
};


typedef enum {
	CABLE_TYPE_NOTIFY_CMD_DETACH	= 0,
	CABLE_TYPE_NOTIFY_CMD_ATTACH,
} cable_type_notifier_cmd_t;


typedef enum {
	CABLE_TYPE_NOTIFY_DEV_CHARGER = 0,
	CABLE_TYPE_NOTIFY_DEV_USB,
} cable_type_notifier_device_t;

struct cable_type_notifier_struct {
	cable_type_attached_dev_t attached_dev;
	cable_type_notifier_cmd_t cmd;
	struct blocking_notifier_head notifier_call_chain;
};

#define CABLE_TYPE_NOTIFIER_BLOCK(name)	\
	struct notifier_block (name)


extern void cable_type_notifier_set_attached_dev(cable_type_attached_dev_t new_dev);


extern int cable_type_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, cable_type_notifier_device_t listener);
extern int cable_type_notifier_unregister(struct notifier_block *nb);

#endif 
