

#ifndef __SB_NOTIFY_H
#define __SB_NOTIFY_H __FILE__

#include <linux/notifier.h>
#include <linux/battery/sb_def.h>

enum sbn_type {
	SB_NOTIFY_UNKNOWN = 0,

	SB_NOTIFY_DEV_PROBE,
	SB_NOTIFY_DEV_SHUTDOWN,
	SB_NOTIFY_DEV_LIST,

	SB_NOTIFY_EVENT_MISC,
	SB_NOTIFY_EVENT_SIOP,
	SB_NOTIFY_EVENT_SLATE_MODE,

	SB_NOTIFY_MAX,

};

struct sbn_dev_list {
	const char		**list;
	unsigned int	count;
};

struct sbn_bit_event {
	unsigned int	value;
	unsigned int	mask;
};

#define SB_NOTIFY_DISABLE	(-3661)
#if IS_ENABLED(CONFIG_SB_NOTIFY)
int sb_notify_call(enum sbn_type ntype, sb_data *ndata);
int sb_notify_register(struct notifier_block *nb,
		notifier_fn_t notifier, const char *name, enum sb_dev_type type);

int sb_notify_unregister(struct notifier_block *nb);
#else
static inline int sb_notify_call(enum sbn_type ntype, sb_data *ndata)
{ return SB_NOTIFY_DISABLE; }

static inline int sb_notify_register(struct notifier_block *nb,
		notifier_fn_t notifier, const char *name, enum sb_dev_type type)
{ return SB_NOTIFY_DISABLE; }

static inline int sb_notify_unregister(struct notifier_block *nb)
{ return SB_NOTIFY_DISABLE; }
#endif

#endif 

