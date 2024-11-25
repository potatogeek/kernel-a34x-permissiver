/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IRQHANDLER_H
#define _LINUX_IRQHANDLER_H

/*
 * Interrupt flow handler typedefs are defined here to avoid circular
 * include dependencies.
 */

struct irq_desc;
struct irq_data;
typedef	void (*irq_flow_handler_t)(struct irq_desc *desc);
<<<<<<< HEAD
typedef	void (*irq_preflow_handler_t)(struct irq_data *data);
=======
>>>>>>> upstream/android-13

#endif
