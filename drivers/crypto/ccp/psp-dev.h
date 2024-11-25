<<<<<<< HEAD
/*
 * AMD Platform Security Processor (PSP) interface driver
 *
 * Copyright (C) 2017 Advanced Micro Devices, Inc.
 *
 * Author: Brijesh Singh <brijesh.singh@amd.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * AMD Platform Security Processor (PSP) interface driver
 *
 * Copyright (C) 2017-2019 Advanced Micro Devices, Inc.
 *
 * Author: Brijesh Singh <brijesh.singh@amd.com>
>>>>>>> upstream/android-13
 */

#ifndef __PSP_DEV_H__
#define __PSP_DEV_H__

#include <linux/device.h>
<<<<<<< HEAD
#include <linux/pci.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/dmapool.h>
#include <linux/hw_random.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/dmaengine.h>
#include <linux/psp-sev.h>
#include <linux/miscdevice.h>

#include "sp-dev.h"

#define PSP_CMD_COMPLETE		BIT(1)

#define PSP_CMDRESP_CMD_SHIFT		16
#define PSP_CMDRESP_IOC			BIT(0)
=======
#include <linux/list.h>
#include <linux/bits.h>
#include <linux/interrupt.h>

#include "sp-dev.h"

>>>>>>> upstream/android-13
#define PSP_CMDRESP_RESP		BIT(31)
#define PSP_CMDRESP_ERR_MASK		0xffff

#define MAX_PSP_NAME_LEN		16

<<<<<<< HEAD
struct sev_misc_dev {
	struct kref refcount;
	struct miscdevice misc;
};
=======
extern struct psp_device *psp_master;

typedef void (*psp_irq_handler_t)(int, void *, unsigned int);
>>>>>>> upstream/android-13

struct psp_device {
	struct list_head entry;

	struct psp_vdata *vdata;
	char name[MAX_PSP_NAME_LEN];

	struct device *dev;
	struct sp_device *sp;

	void __iomem *io_regs;

<<<<<<< HEAD
	int sev_state;
	unsigned int sev_int_rcvd;
	wait_queue_head_t sev_int_queue;
	struct sev_misc_dev *sev_misc;
	struct sev_user_data_status status_cmd_buf;
	struct sev_data_init init_cmd_buf;

	u8 api_major;
	u8 api_minor;
	u8 build;
};

=======
	psp_irq_handler_t sev_irq_handler;
	void *sev_irq_data;

	psp_irq_handler_t tee_irq_handler;
	void *tee_irq_data;

	void *sev_data;
	void *tee_data;
};

void psp_set_sev_irq_handler(struct psp_device *psp, psp_irq_handler_t handler,
			     void *data);
void psp_clear_sev_irq_handler(struct psp_device *psp);

void psp_set_tee_irq_handler(struct psp_device *psp, psp_irq_handler_t handler,
			     void *data);
void psp_clear_tee_irq_handler(struct psp_device *psp);

struct psp_device *psp_get_master_device(void);

>>>>>>> upstream/android-13
#endif /* __PSP_DEV_H */
