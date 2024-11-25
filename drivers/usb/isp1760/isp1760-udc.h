<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/*
 * Driver for the NXP ISP1761 device controller
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Driver for the NXP ISP1761 device controller
 *
 * Copyright 2021 Linaro, Rui Miguel Silva
>>>>>>> upstream/android-13
 * Copyright 2014 Ideas on Board Oy
 *
 * Contacts:
 *	Laurent Pinchart <laurent.pinchart@ideasonboard.com>
<<<<<<< HEAD
=======
 *	Rui Miguel Silva <rui.silva@linaro.org>
>>>>>>> upstream/android-13
 */

#ifndef _ISP1760_UDC_H_
#define _ISP1760_UDC_H_

#include <linux/ioport.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/usb/gadget.h>

<<<<<<< HEAD
=======
#include "isp1760-regs.h"

>>>>>>> upstream/android-13
struct isp1760_device;
struct isp1760_udc;

enum isp1760_ctrl_state {
	ISP1760_CTRL_SETUP,		/* Waiting for a SETUP transaction */
	ISP1760_CTRL_DATA_IN,		/* Setup received, data IN stage */
	ISP1760_CTRL_DATA_OUT,		/* Setup received, data OUT stage */
	ISP1760_CTRL_STATUS,		/* 0-length request in status stage */
};

struct isp1760_ep {
	struct isp1760_udc *udc;
	struct usb_ep ep;

	struct list_head queue;

	unsigned int addr;
	unsigned int maxpacket;
	char name[7];

	const struct usb_endpoint_descriptor *desc;

	bool rx_pending;
	bool halted;
	bool wedged;
};

/**
 * struct isp1760_udc - UDC state information
 * irq: IRQ number
 * irqname: IRQ name (as passed to request_irq)
<<<<<<< HEAD
 * regs: Base address of the UDC registers
=======
 * regs: regmap for UDC registers
>>>>>>> upstream/android-13
 * driver: Gadget driver
 * gadget: Gadget device
 * lock: Protects driver, vbus_timer, ep, ep0_*, DC_EPINDEX register
 * ep: Array of endpoints
 * ep0_state: Control request state for endpoint 0
 * ep0_dir: Direction of the current control request
 * ep0_length: Length of the current control request
 * connected: Tracks gadget driver bus connection state
 */
struct isp1760_udc {
<<<<<<< HEAD
#ifdef CONFIG_USB_ISP1761_UDC
=======
>>>>>>> upstream/android-13
	struct isp1760_device *isp;

	int irq;
	char *irqname;
<<<<<<< HEAD
	void __iomem *regs;
=======

	struct regmap *regs;
	struct regmap_field *fields[DC_FIELD_MAX];
>>>>>>> upstream/android-13

	struct usb_gadget_driver *driver;
	struct usb_gadget gadget;

	spinlock_t lock;
	struct timer_list vbus_timer;

	struct isp1760_ep ep[15];

	enum isp1760_ctrl_state ep0_state;
	u8 ep0_dir;
	u16 ep0_length;

	bool connected;
<<<<<<< HEAD

	unsigned int devstatus;
#endif
=======
	bool is_isp1763;

	unsigned int devstatus;
>>>>>>> upstream/android-13
};

#ifdef CONFIG_USB_ISP1761_UDC
int isp1760_udc_register(struct isp1760_device *isp, int irq,
			 unsigned long irqflags);
void isp1760_udc_unregister(struct isp1760_device *isp);
#else
static inline int isp1760_udc_register(struct isp1760_device *isp, int irq,
				       unsigned long irqflags)
{
	return 0;
}

static inline void isp1760_udc_unregister(struct isp1760_device *isp)
{
}
#endif

#endif
