/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ISP1760_HCD_H_
#define _ISP1760_HCD_H_

#include <linux/spinlock.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>

#include "isp1760-regs.h"
>>>>>>> upstream/android-13

struct isp1760_qh;
struct isp1760_qtd;
struct resource;
struct usb_hcd;

<<<<<<< HEAD
/*
 * 60kb divided in:
 * - 32 blocks @ 256  bytes
 * - 20 blocks @ 1024 bytes
 * -  4 blocks @ 8192 bytes
 */

#define BLOCK_1_NUM 32
#define BLOCK_2_NUM 20
#define BLOCK_3_NUM 4

#define BLOCK_1_SIZE 256
#define BLOCK_2_SIZE 1024
#define BLOCK_3_SIZE 8192
#define BLOCKS (BLOCK_1_NUM + BLOCK_2_NUM + BLOCK_3_NUM)
#define MAX_PAYLOAD_SIZE BLOCK_3_SIZE
#define PAYLOAD_AREA_SIZE 0xf000

=======
>>>>>>> upstream/android-13
struct isp1760_slotinfo {
	struct isp1760_qh *qh;
	struct isp1760_qtd *qtd;
	unsigned long timestamp;
};

/* chip memory management */
<<<<<<< HEAD
=======
#define ISP176x_BLOCK_MAX (32 + 20 + 4)
#define ISP176x_BLOCK_NUM 3

struct isp1760_memory_layout {
	unsigned int blocks[ISP176x_BLOCK_NUM];
	unsigned int blocks_size[ISP176x_BLOCK_NUM];

	unsigned int slot_num;
	unsigned int payload_blocks;
	unsigned int payload_area_size;
};

>>>>>>> upstream/android-13
struct isp1760_memory_chunk {
	unsigned int start;
	unsigned int size;
	unsigned int free;
};

enum isp1760_queue_head_types {
	QH_CONTROL,
	QH_BULK,
	QH_INTERRUPT,
	QH_END
};

struct isp1760_hcd {
<<<<<<< HEAD
#ifdef CONFIG_USB_ISP1760_HCD
	struct usb_hcd		*hcd;

	u32 hcs_params;
	spinlock_t		lock;
	struct isp1760_slotinfo	atl_slots[32];
	int			atl_done_map;
	struct isp1760_slotinfo	int_slots[32];
	int			int_done_map;
	struct isp1760_memory_chunk memory_pool[BLOCKS];
=======
	struct usb_hcd		*hcd;

	void __iomem		*base;

	struct regmap		*regs;
	struct regmap_field	*fields[HC_FIELD_MAX];

	bool			is_isp1763;
	const struct isp1760_memory_layout	*memory_layout;

	spinlock_t		lock;
	struct isp1760_slotinfo	*atl_slots;
	int			atl_done_map;
	struct isp1760_slotinfo	*int_slots;
	int			int_done_map;
	struct isp1760_memory_chunk memory_pool[ISP176x_BLOCK_MAX];
>>>>>>> upstream/android-13
	struct list_head	qh_list[QH_END];

	/* periodic schedule support */
#define	DEFAULT_I_TDPS		1024
	unsigned		periodic_size;
	unsigned		i_thresh;
	unsigned long		reset_done;
	unsigned long		next_statechange;
<<<<<<< HEAD
#endif
};

#ifdef CONFIG_USB_ISP1760_HCD
int isp1760_hcd_register(struct isp1760_hcd *priv, void __iomem *regs,
			 struct resource *mem, int irq, unsigned long irqflags,
			 struct device *dev);
=======
};

#ifdef CONFIG_USB_ISP1760_HCD
int isp1760_hcd_register(struct isp1760_hcd *priv, struct resource *mem,
			 int irq, unsigned long irqflags, struct device *dev);
>>>>>>> upstream/android-13
void isp1760_hcd_unregister(struct isp1760_hcd *priv);

int isp1760_init_kmem_once(void);
void isp1760_deinit_kmem_cache(void);
#else
static inline int isp1760_hcd_register(struct isp1760_hcd *priv,
<<<<<<< HEAD
				       void __iomem *regs, struct resource *mem,
=======
				       struct resource *mem,
>>>>>>> upstream/android-13
				       int irq, unsigned long irqflags,
				       struct device *dev)
{
	return 0;
}

static inline void isp1760_hcd_unregister(struct isp1760_hcd *priv)
{
}

static inline int isp1760_init_kmem_once(void)
{
	return 0;
}

static inline void isp1760_deinit_kmem_cache(void)
{
}
#endif

#endif /* _ISP1760_HCD_H_ */
