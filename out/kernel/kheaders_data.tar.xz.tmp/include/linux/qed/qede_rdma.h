
#ifndef QEDE_ROCE_H
#define QEDE_ROCE_H

#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/workqueue.h>

struct qedr_dev;
struct qed_dev;
struct qede_dev;

enum qede_rdma_event {
	QEDE_UP,
	QEDE_DOWN,
	QEDE_CHANGE_ADDR,
	QEDE_CLOSE
};

struct qede_rdma_event_work {
	struct list_head list;
	struct work_struct work;
	void *ptr;
	enum qede_rdma_event event;
};

struct qedr_driver {
	unsigned char name[32];

	struct qedr_dev* (*add)(struct qed_dev *, struct pci_dev *,
				struct net_device *);

	void (*remove)(struct qedr_dev *);
	void (*notify)(struct qedr_dev *, enum qede_rdma_event);
};


int qede_rdma_register_driver(struct qedr_driver *drv);
void qede_rdma_unregister_driver(struct qedr_driver *drv);

bool qede_rdma_supported(struct qede_dev *dev);

#if IS_ENABLED(CONFIG_QED_RDMA)
int qede_rdma_dev_add(struct qede_dev *dev);
void qede_rdma_dev_event_open(struct qede_dev *dev);
void qede_rdma_dev_event_close(struct qede_dev *dev);
void qede_rdma_dev_remove(struct qede_dev *dev);
void qede_rdma_event_changeaddr(struct qede_dev *edr);

#else
static inline int qede_rdma_dev_add(struct qede_dev *dev)
{
	return 0;
}

static inline void qede_rdma_dev_event_open(struct qede_dev *dev) {}
static inline void qede_rdma_dev_event_close(struct qede_dev *dev) {}
static inline void qede_rdma_dev_remove(struct qede_dev *dev) {}
static inline void qede_rdma_event_changeaddr(struct qede_dev *edr) {}
#endif
#endif
