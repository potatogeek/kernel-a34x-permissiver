<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * include/linux/irqchip/arm-gic-common.h
 *
 * Copyright (C) 2016 ARM Limited, All Rights Reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */
#ifndef __LINUX_IRQCHIP_ARM_GIC_COMMON_H
#define __LINUX_IRQCHIP_ARM_GIC_COMMON_H

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/ioport.h>

enum gic_type {
	GIC_V2,
	GIC_V3,
};

struct gic_kvm_info {
	/* GIC type */
	enum gic_type	type;
	/* Virtual CPU interface */
	struct resource vcpu;
	/* Interrupt number */
	unsigned int	maint_irq;
	/* Virtual control interface */
	struct resource vctrl;
	/* vlpi support */
	bool		has_v4;
};

const struct gic_kvm_info *gic_get_kvm_info(void);
=======
#include <linux/irqchip/arm-vgic-info.h>

#define GICD_INT_DEF_PRI		0xa0
#define GICD_INT_DEF_PRI_X4		((GICD_INT_DEF_PRI << 24) |\
					(GICD_INT_DEF_PRI << 16) |\
					(GICD_INT_DEF_PRI << 8) |\
					GICD_INT_DEF_PRI)

struct irq_domain;
struct fwnode_handle;
int gicv2m_init(struct fwnode_handle *parent_handle,
		struct irq_domain *parent);
>>>>>>> upstream/android-13

#endif /* __LINUX_IRQCHIP_ARM_GIC_COMMON_H */
