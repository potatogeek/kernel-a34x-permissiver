

#ifndef _LINUX_WKUP_M3_IPC_H
#define _LINUX_WKUP_M3_IPC_H

#define WKUP_M3_DEEPSLEEP	1
#define WKUP_M3_STANDBY		2
#define WKUP_M3_IDLE		3

#include <linux/mailbox_client.h>

struct wkup_m3_ipc_ops;

struct wkup_m3_ipc {
	struct rproc *rproc;

	void __iomem *ipc_mem_base;
	struct device *dev;

	int mem_type;
	unsigned long resume_addr;
	int state;

	struct completion sync_complete;
	struct mbox_client mbox_client;
	struct mbox_chan *mbox;

	struct wkup_m3_ipc_ops *ops;
	int is_rtc_only;
};

struct wkup_m3_wakeup_src {
	int irq_nr;
	char src[10];
};

struct wkup_m3_ipc_ops {
	void (*set_mem_type)(struct wkup_m3_ipc *m3_ipc, int mem_type);
	void (*set_resume_address)(struct wkup_m3_ipc *m3_ipc, void *addr);
	int (*prepare_low_power)(struct wkup_m3_ipc *m3_ipc, int state);
	int (*finish_low_power)(struct wkup_m3_ipc *m3_ipc);
	int (*request_pm_status)(struct wkup_m3_ipc *m3_ipc);
	const char *(*request_wake_src)(struct wkup_m3_ipc *m3_ipc);
	void (*set_rtc_only)(struct wkup_m3_ipc *m3_ipc);
};

struct wkup_m3_ipc *wkup_m3_ipc_get(void);
void wkup_m3_ipc_put(struct wkup_m3_ipc *m3_ipc);
void wkup_m3_set_rtc_only_mode(void);
#endif 
