<<<<<<< HEAD
/*
 * Copyright (C) 2015 Linaro Ltd.
 * Author: Shannon Zhao <shannon.zhao@linaro.org>
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
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2015 Linaro Ltd.
 * Author: Shannon Zhao <shannon.zhao@linaro.org>
>>>>>>> upstream/android-13
 */

#ifndef __ASM_ARM_KVM_PMU_H
#define __ASM_ARM_KVM_PMU_H

#include <linux/perf_event.h>
#include <asm/perf_event.h>

#define ARMV8_PMU_CYCLE_IDX		(ARMV8_PMU_MAX_COUNTERS - 1)
<<<<<<< HEAD

#ifdef CONFIG_KVM_ARM_PMU
=======
#define ARMV8_PMU_MAX_COUNTER_PAIRS	((ARMV8_PMU_MAX_COUNTERS + 1) >> 1)

DECLARE_STATIC_KEY_FALSE(kvm_arm_pmu_available);

static __always_inline bool kvm_arm_support_pmu_v3(void)
{
	return static_branch_likely(&kvm_arm_pmu_available);
}

#ifdef CONFIG_HW_PERF_EVENTS
>>>>>>> upstream/android-13

struct kvm_pmc {
	u8 idx;	/* index into the pmu->pmc array */
	struct perf_event *perf_event;
<<<<<<< HEAD
	u64 bitmask;
=======
>>>>>>> upstream/android-13
};

struct kvm_pmu {
	int irq_num;
	struct kvm_pmc pmc[ARMV8_PMU_MAX_COUNTERS];
<<<<<<< HEAD
	bool ready;
	bool created;
	bool irq_level;
};

#define kvm_arm_pmu_v3_ready(v)		((v)->arch.pmu.ready)
=======
	DECLARE_BITMAP(chained, ARMV8_PMU_MAX_COUNTER_PAIRS);
	bool created;
	bool irq_level;
	struct irq_work overflow_work;
};

>>>>>>> upstream/android-13
#define kvm_arm_pmu_irq_initialized(v)	((v)->arch.pmu.irq_num >= VGIC_NR_SGIS)
u64 kvm_pmu_get_counter_value(struct kvm_vcpu *vcpu, u64 select_idx);
void kvm_pmu_set_counter_value(struct kvm_vcpu *vcpu, u64 select_idx, u64 val);
u64 kvm_pmu_valid_counter_mask(struct kvm_vcpu *vcpu);
<<<<<<< HEAD
void kvm_pmu_vcpu_reset(struct kvm_vcpu *vcpu);
void kvm_pmu_vcpu_destroy(struct kvm_vcpu *vcpu);
void kvm_pmu_disable_counter(struct kvm_vcpu *vcpu, u64 val);
void kvm_pmu_enable_counter(struct kvm_vcpu *vcpu, u64 val);
=======
u64 kvm_pmu_get_pmceid(struct kvm_vcpu *vcpu, bool pmceid1);
void kvm_pmu_vcpu_init(struct kvm_vcpu *vcpu);
void kvm_pmu_vcpu_reset(struct kvm_vcpu *vcpu);
void kvm_pmu_vcpu_destroy(struct kvm_vcpu *vcpu);
void kvm_pmu_disable_counter_mask(struct kvm_vcpu *vcpu, u64 val);
void kvm_pmu_enable_counter_mask(struct kvm_vcpu *vcpu, u64 val);
>>>>>>> upstream/android-13
void kvm_pmu_flush_hwstate(struct kvm_vcpu *vcpu);
void kvm_pmu_sync_hwstate(struct kvm_vcpu *vcpu);
bool kvm_pmu_should_notify_user(struct kvm_vcpu *vcpu);
void kvm_pmu_update_run(struct kvm_vcpu *vcpu);
void kvm_pmu_software_increment(struct kvm_vcpu *vcpu, u64 val);
void kvm_pmu_handle_pmcr(struct kvm_vcpu *vcpu, u64 val);
void kvm_pmu_set_counter_event_type(struct kvm_vcpu *vcpu, u64 data,
				    u64 select_idx);
<<<<<<< HEAD
bool kvm_arm_support_pmu_v3(void);
=======
>>>>>>> upstream/android-13
int kvm_arm_pmu_v3_set_attr(struct kvm_vcpu *vcpu,
			    struct kvm_device_attr *attr);
int kvm_arm_pmu_v3_get_attr(struct kvm_vcpu *vcpu,
			    struct kvm_device_attr *attr);
int kvm_arm_pmu_v3_has_attr(struct kvm_vcpu *vcpu,
			    struct kvm_device_attr *attr);
int kvm_arm_pmu_v3_enable(struct kvm_vcpu *vcpu);
#else
struct kvm_pmu {
};

<<<<<<< HEAD
#define kvm_arm_pmu_v3_ready(v)		(false)
=======
>>>>>>> upstream/android-13
#define kvm_arm_pmu_irq_initialized(v)	(false)
static inline u64 kvm_pmu_get_counter_value(struct kvm_vcpu *vcpu,
					    u64 select_idx)
{
	return 0;
}
static inline void kvm_pmu_set_counter_value(struct kvm_vcpu *vcpu,
					     u64 select_idx, u64 val) {}
static inline u64 kvm_pmu_valid_counter_mask(struct kvm_vcpu *vcpu)
{
	return 0;
}
<<<<<<< HEAD
static inline void kvm_pmu_vcpu_reset(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_vcpu_destroy(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_disable_counter(struct kvm_vcpu *vcpu, u64 val) {}
static inline void kvm_pmu_enable_counter(struct kvm_vcpu *vcpu, u64 val) {}
=======
static inline void kvm_pmu_vcpu_init(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_vcpu_reset(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_vcpu_destroy(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_disable_counter_mask(struct kvm_vcpu *vcpu, u64 val) {}
static inline void kvm_pmu_enable_counter_mask(struct kvm_vcpu *vcpu, u64 val) {}
>>>>>>> upstream/android-13
static inline void kvm_pmu_flush_hwstate(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_sync_hwstate(struct kvm_vcpu *vcpu) {}
static inline bool kvm_pmu_should_notify_user(struct kvm_vcpu *vcpu)
{
	return false;
}
static inline void kvm_pmu_update_run(struct kvm_vcpu *vcpu) {}
static inline void kvm_pmu_software_increment(struct kvm_vcpu *vcpu, u64 val) {}
static inline void kvm_pmu_handle_pmcr(struct kvm_vcpu *vcpu, u64 val) {}
static inline void kvm_pmu_set_counter_event_type(struct kvm_vcpu *vcpu,
						  u64 data, u64 select_idx) {}
<<<<<<< HEAD
static inline bool kvm_arm_support_pmu_v3(void) { return false; }
=======
>>>>>>> upstream/android-13
static inline int kvm_arm_pmu_v3_set_attr(struct kvm_vcpu *vcpu,
					  struct kvm_device_attr *attr)
{
	return -ENXIO;
}
static inline int kvm_arm_pmu_v3_get_attr(struct kvm_vcpu *vcpu,
					  struct kvm_device_attr *attr)
{
	return -ENXIO;
}
static inline int kvm_arm_pmu_v3_has_attr(struct kvm_vcpu *vcpu,
					  struct kvm_device_attr *attr)
{
	return -ENXIO;
}
static inline int kvm_arm_pmu_v3_enable(struct kvm_vcpu *vcpu)
{
	return 0;
}
<<<<<<< HEAD
=======
static inline u64 kvm_pmu_get_pmceid(struct kvm_vcpu *vcpu, bool pmceid1)
{
	return 0;
}

>>>>>>> upstream/android-13
#endif

#endif
