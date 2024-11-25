<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * irq.h: in kernel interrupt controller related definitions
 * Copyright (c) 2016 Red Hat, Inc.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
=======
>>>>>>> upstream/android-13
 * This header is included by irqchip.c. However, on ARM, interrupt
 * controller declarations are located in include/kvm/arm_vgic.h since
 * they are mostly shared between arm and arm64.
 */

#ifndef __IRQ_H
#define __IRQ_H

#include <kvm/arm_vgic.h>

#endif
