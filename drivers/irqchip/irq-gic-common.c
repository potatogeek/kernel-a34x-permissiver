<<<<<<< HEAD
/*
 * Copyright (C) 2002 ARM Limited, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2002 ARM Limited, All Rights Reserved.
>>>>>>> upstream/android-13
 */

#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/irqchip/arm-gic.h>

#include "irq-gic-common.h"

static DEFINE_RAW_SPINLOCK(irq_controller_lock);

<<<<<<< HEAD
static const struct gic_kvm_info *gic_kvm_info;

const struct gic_kvm_info *gic_get_kvm_info(void)
{
	return gic_kvm_info;
}

void gic_set_kvm_info(const struct gic_kvm_info *info)
{
	BUG_ON(gic_kvm_info != NULL);
	gic_kvm_info = info;
=======
void gic_enable_of_quirks(const struct device_node *np,
			  const struct gic_quirk *quirks, void *data)
{
	for (; quirks->desc; quirks++) {
		if (!of_device_is_compatible(np, quirks->compatible))
			continue;
		if (quirks->init(data))
			pr_info("GIC: enabling workaround for %s\n",
				quirks->desc);
	}
>>>>>>> upstream/android-13
}

void gic_enable_quirks(u32 iidr, const struct gic_quirk *quirks,
		void *data)
{
	for (; quirks->desc; quirks++) {
<<<<<<< HEAD
=======
		if (quirks->compatible)
			continue;
>>>>>>> upstream/android-13
		if (quirks->iidr != (quirks->mask & iidr))
			continue;
		if (quirks->init(data))
			pr_info("GIC: enabling workaround for %s\n",
				quirks->desc);
	}
}

int gic_configure_irq(unsigned int irq, unsigned int type,
		       void __iomem *base, void (*sync_access)(void))
{
	u32 confmask = 0x2 << ((irq % 16) * 2);
	u32 confoff = (irq / 16) * 4;
	u32 val, oldval;
	int ret = 0;
	unsigned long flags;

	/*
	 * Read current configuration register, and insert the config
	 * for "irq", depending on "type".
	 */
	raw_spin_lock_irqsave(&irq_controller_lock, flags);
<<<<<<< HEAD
	val = oldval = readl_relaxed(base + GIC_DIST_CONFIG + confoff);
=======
	val = oldval = readl_relaxed(base + confoff);
>>>>>>> upstream/android-13
	if (type & IRQ_TYPE_LEVEL_MASK)
		val &= ~confmask;
	else if (type & IRQ_TYPE_EDGE_BOTH)
		val |= confmask;

	/* If the current configuration is the same, then we are done */
	if (val == oldval) {
		raw_spin_unlock_irqrestore(&irq_controller_lock, flags);
		return 0;
	}

	/*
	 * Write back the new configuration, and possibly re-enable
	 * the interrupt. If we fail to write a new configuration for
	 * an SPI then WARN and return an error. If we fail to write the
	 * configuration for a PPI this is most likely because the GIC
	 * does not allow us to set the configuration or we are in a
	 * non-secure mode, and hence it may not be catastrophic.
	 */
<<<<<<< HEAD
	writel_relaxed(val, base + GIC_DIST_CONFIG + confoff);
	if (readl_relaxed(base + GIC_DIST_CONFIG + confoff) != val) {
		if (WARN_ON(irq >= 32))
			ret = -EINVAL;
		else
			pr_warn("GIC: PPI%d is secure or misconfigured\n",
				irq - 16);
	}
=======
	writel_relaxed(val, base + confoff);
	if (readl_relaxed(base + confoff) != val)
		ret = -EINVAL;

>>>>>>> upstream/android-13
	raw_spin_unlock_irqrestore(&irq_controller_lock, flags);

	if (sync_access)
		sync_access();

	return ret;
}

void gic_dist_config(void __iomem *base, int gic_irqs,
		     void (*sync_access)(void))
{
	unsigned int i;

	/*
	 * Set all global interrupts to be level triggered, active low.
	 */
	for (i = 32; i < gic_irqs; i += 16)
		writel_relaxed(GICD_INT_ACTLOW_LVLTRIG,
					base + GIC_DIST_CONFIG + i / 4);

	/*
	 * Set priority on all global interrupts.
	 */
	for (i = 32; i < gic_irqs; i += 4)
		writel_relaxed(GICD_INT_DEF_PRI_X4, base + GIC_DIST_PRI + i);

	/*
	 * Deactivate and disable all SPIs. Leave the PPI and SGIs
	 * alone as they are in the redistributor registers on GICv3.
	 */
	for (i = 32; i < gic_irqs; i += 32) {
		writel_relaxed(GICD_INT_EN_CLR_X32,
			       base + GIC_DIST_ACTIVE_CLEAR + i / 8);
		writel_relaxed(GICD_INT_EN_CLR_X32,
			       base + GIC_DIST_ENABLE_CLEAR + i / 8);
	}

	if (sync_access)
		sync_access();
}

<<<<<<< HEAD
void gic_cpu_config(void __iomem *base, void (*sync_access)(void))
=======
void gic_cpu_config(void __iomem *base, int nr, void (*sync_access)(void))
>>>>>>> upstream/android-13
{
	int i;

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
<<<<<<< HEAD
	 * PPI interrupts, ensure all SGI interrupts are enabled.
	 * Make sure everything is deactivated.
	 */
	writel_relaxed(GICD_INT_EN_CLR_X32, base + GIC_DIST_ACTIVE_CLEAR);
	writel_relaxed(GICD_INT_EN_CLR_PPI, base + GIC_DIST_ENABLE_CLEAR);
	writel_relaxed(GICD_INT_EN_SET_SGI, base + GIC_DIST_ENABLE_SET);
=======
	 * private interrupts. Make sure everything is deactivated.
	 */
	for (i = 0; i < nr; i += 32) {
		writel_relaxed(GICD_INT_EN_CLR_X32,
			       base + GIC_DIST_ACTIVE_CLEAR + i / 8);
		writel_relaxed(GICD_INT_EN_CLR_X32,
			       base + GIC_DIST_ENABLE_CLEAR + i / 8);
	}
>>>>>>> upstream/android-13

	/*
	 * Set priority on PPI and SGI interrupts
	 */
<<<<<<< HEAD
	for (i = 0; i < 32; i += 4)
=======
	for (i = 0; i < nr; i += 4)
>>>>>>> upstream/android-13
		writel_relaxed(GICD_INT_DEF_PRI_X4,
					base + GIC_DIST_PRI + i * 4 / 4);

	if (sync_access)
		sync_access();
}
