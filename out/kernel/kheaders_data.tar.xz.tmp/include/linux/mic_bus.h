
#ifndef _MIC_BUS_H_
#define _MIC_BUS_H_

#include <linux/interrupt.h>
#include <linux/dma-mapping.h>

struct mbus_device_id {
	__u32 device;
	__u32 vendor;
};

#define MBUS_DEV_DMA_HOST 2
#define MBUS_DEV_DMA_MIC 3
#define MBUS_DEV_ANY_ID 0xffffffff


struct mbus_device {
	void __iomem *mmio_va;
	struct mbus_hw_ops *hw_ops;
	struct mbus_device_id id;
	struct device dev;
	int index;
};


struct mbus_driver {
	struct device_driver driver;
	const struct mbus_device_id *id_table;
	int (*probe)(struct mbus_device *dev);
	void (*scan)(struct mbus_device *dev);
	void (*remove)(struct mbus_device *dev);
};


struct mic_irq;


struct mbus_hw_ops {
	struct mic_irq* (*request_threaded_irq)(struct mbus_device *mbdev,
						irq_handler_t handler,
						irq_handler_t thread_fn,
						const char *name, void *data,
						int intr_src);
	void (*free_irq)(struct mbus_device *mbdev,
			 struct mic_irq *cookie, void *data);
	void (*ack_interrupt)(struct mbus_device *mbdev, int num);
};

struct mbus_device *
mbus_register_device(struct device *pdev, int id, const struct dma_map_ops *dma_ops,
		     struct mbus_hw_ops *hw_ops, int index,
		     void __iomem *mmio_va);
void mbus_unregister_device(struct mbus_device *mbdev);

int mbus_register_driver(struct mbus_driver *drv);
void mbus_unregister_driver(struct mbus_driver *drv);

static inline struct mbus_device *dev_to_mbus(struct device *_dev)
{
	return container_of(_dev, struct mbus_device, dev);
}

static inline struct mbus_driver *drv_to_mbus(struct device_driver *drv)
{
	return container_of(drv, struct mbus_driver, driver);
}

#endif 
