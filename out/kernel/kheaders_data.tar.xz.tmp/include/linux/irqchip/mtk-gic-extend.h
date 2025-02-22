/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __MTK_GIC_EXTEND_H
#define __MTK_GIC_EXTEND_H

#define MT_EDGE_SENSITIVE	0
#define MT_LEVEL_SENSITIVE	1
#define MT_POLARITY_LOW		0
#define MT_POLARITY_HIGH	1

#define INTID_INVALID		1024

#ifndef FIQ_SMP_CALL_SGI
#define FIQ_SMP_CALL_SGI	13
#endif

#define GIC_IIDR                    0x8
#define GICD_V3_IIDR_GIC600         0x2
#define GICD_V3_IIDR_PROD_ID_SHIFT  24

#include <linux/irq.h>
typedef void (*fiq_isr_handler) (void *arg, void *regs, void *svc_sp);

enum {
	IRQ_MASK_HEADER = 0xF1F1F1F1,
	IRQ_MASK_FOOTER = 0xF2F2F2F2
};

struct mtk_irq_mask {
	unsigned int header;	
	__u32 mask0;
	__u32 mask1;
	__u32 mask2;
	__u32 mask3;
	__u32 mask4;
	__u32 mask5;
	__u32 mask6;
	__u32 mask7;
	__u32 mask8;
	__u32 mask9;
	__u32 mask10;
	__u32 mask11;
	__u32 mask12;
	unsigned int footer;	
};

unsigned int get_hardware_irq(unsigned int virq);
void mt_irq_unmask_for_sleep(unsigned int hwirq);
void mt_irq_unmask_for_sleep_ex(unsigned int virq);
void mt_irq_mask_for_sleep(unsigned int virq);
int mt_irq_mask_all(struct mtk_irq_mask *mask);
int mt_irq_mask_restore(struct mtk_irq_mask *mask);
void mt_irq_set_pending_for_sleep(unsigned int irq);
extern u32 mt_irq_get_pending_vec(u32 start_irq);
extern void mt_irq_set_pending(unsigned int irq);
extern void mt_irq_set_pending_hw(unsigned int hwirq);
extern unsigned int mt_irq_get_pending(unsigned int irq);
extern unsigned int mt_irq_get_pending_hw(unsigned int hwirq);
extern u32 mt_irq_get_pol(u32 irq);
extern u32 mt_irq_get_pol_hw(u32 hwirq);
extern void mt_irq_dump_status(int irq);
void mt_gic_set_priority(unsigned int irq);
void mt_set_irq_priority(unsigned int irq, unsigned int priority);
unsigned int mt_get_irq_priority(unsigned int irq);

#ifdef CONFIG_FAST_CIRQ_CLONE_FLUSH
extern void __iomem *get_dist_base(void);
extern u32 mt_irq_get_en_hw(unsigned int hwirq);
#endif

#if defined(CONFIG_FIQ_GLUE)
int request_fiq(int irq, fiq_isr_handler handler,
		unsigned long irq_flags, void *arg);
void irq_raise_softirq(const struct cpumask *mask, unsigned int irq);
#endif

void gic_set_primask(void);

void gic_clear_primask(void);
int add_cpu_to_prefer_schedule_domain(unsigned int cpu);
int remove_cpu_from_prefer_schedule_domain(unsigned int cpu);

#ifdef CONFIG_MTK_SYSIRQ
static inline struct irq_data *get_gic_irq_data(struct irq_data *d)
{
	return d->parent_data;
}
#endif


static inline unsigned int gic_irq(struct irq_data *d)
{
#ifdef CONFIG_MTK_SYSIRQ
	d = get_gic_irq_data(d);
#endif
	return d->hwirq;
}

static inline unsigned int virq_to_hwirq(unsigned int virq)
{
	struct irq_desc *desc;
	unsigned int hwirq;

	desc = irq_to_desc(virq);

	if (desc) {
		hwirq = gic_irq(&desc->irq_data);
	} else {
		WARN_ON(!desc);
		hwirq = INTID_INVALID;
	}

	return hwirq;
}
#endif

