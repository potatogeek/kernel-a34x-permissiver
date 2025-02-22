<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2012,2013 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * Derived from arch/arm/include/asm/kvm_host.h:
 * Copyright (C) 2012 - Virtual Open Systems and Columbia University
 * Author: Christoffer Dall <c.dall@virtualopensystems.com>
<<<<<<< HEAD
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
>>>>>>> upstream/android-13
 */

#ifndef __ARM64_KVM_HOST_H__
#define __ARM64_KVM_HOST_H__

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/kvm_types.h>
#include <asm/cpufeature.h>
=======
#include <linux/arm-smccc.h>
#include <linux/bitmap.h>
#include <linux/types.h>
#include <linux/jump_label.h>
#include <linux/kvm_types.h>
#include <linux/percpu.h>
#include <linux/psci.h>
#include <asm/arch_gicv3.h>
#include <asm/barrier.h>
#include <asm/cpufeature.h>
#include <asm/cputype.h>
>>>>>>> upstream/android-13
#include <asm/daifflags.h>
#include <asm/fpsimd.h>
#include <asm/kvm.h>
#include <asm/kvm_asm.h>
<<<<<<< HEAD
#include <asm/kvm_mmio.h>
#include <asm/thread_info.h>

#define __KVM_HAVE_ARCH_INTC_INITIALIZED

#define KVM_USER_MEM_SLOTS 512
=======

#define __KVM_HAVE_ARCH_INTC_INITIALIZED

>>>>>>> upstream/android-13
#define KVM_HALT_POLL_NS_DEFAULT 500000

#include <kvm/arm_vgic.h>
#include <kvm/arm_arch_timer.h>
#include <kvm/arm_pmu.h>

#define KVM_MAX_VCPUS VGIC_V3_MAX_CPUS

<<<<<<< HEAD
#define KVM_VCPU_MAX_FEATURES 4
=======
#define KVM_VCPU_MAX_FEATURES 7
>>>>>>> upstream/android-13

#define KVM_REQ_SLEEP \
	KVM_ARCH_REQ_FLAGS(0, KVM_REQUEST_WAIT | KVM_REQUEST_NO_WAKEUP)
#define KVM_REQ_IRQ_PENDING	KVM_ARCH_REQ(1)
#define KVM_REQ_VCPU_RESET	KVM_ARCH_REQ(2)
<<<<<<< HEAD

DECLARE_STATIC_KEY_FALSE(userspace_irqchip_in_use);

int __attribute_const__ kvm_target_cpu(void);
int kvm_reset_vcpu(struct kvm_vcpu *vcpu);
int kvm_arch_dev_ioctl_check_extension(struct kvm *kvm, long ext);
void __extended_idmap_trampoline(phys_addr_t boot_pgd, phys_addr_t idmap_start);

struct kvm_arch {
	/* The VMID generation used for the virt. memory system */
	u64    vmid_gen;
	u32    vmid;

	/* 1-level 2nd stage table, protected by kvm->mmu_lock */
	pgd_t *pgd;

	/* VTTBR value associated with above pgd and vmid */
	u64    vttbr;
=======
#define KVM_REQ_RECORD_STEAL	KVM_ARCH_REQ(3)
#define KVM_REQ_RELOAD_GICv4	KVM_ARCH_REQ(4)
#define KVM_REQ_RELOAD_PMU	KVM_ARCH_REQ(5)

#define KVM_DIRTY_LOG_MANUAL_CAPS   (KVM_DIRTY_LOG_MANUAL_PROTECT_ENABLE | \
				     KVM_DIRTY_LOG_INITIALLY_SET)

/*
 * Mode of operation configurable with kvm-arm.mode early param.
 * See Documentation/admin-guide/kernel-parameters.txt for more information.
 */
enum kvm_mode {
	KVM_MODE_DEFAULT,
	KVM_MODE_PROTECTED,
	KVM_MODE_NONE,
};
enum kvm_mode kvm_get_mode(void);

DECLARE_STATIC_KEY_FALSE(userspace_irqchip_in_use);

extern unsigned int kvm_sve_max_vl;
int kvm_arm_init_sve(void);

u32 __attribute_const__ kvm_target_cpu(void);
int kvm_reset_vcpu(struct kvm_vcpu *vcpu);
void kvm_arm_vcpu_destroy(struct kvm_vcpu *vcpu);

struct kvm_hyp_memcache {
	phys_addr_t head;
	unsigned long nr_pages;
};

static inline void push_hyp_memcache(struct kvm_hyp_memcache *mc,
				     phys_addr_t *p,
				     phys_addr_t (*to_pa)(void *virt))
{
	*p = mc->head;
	mc->head = to_pa(p);
	mc->nr_pages++;
}

static inline void *pop_hyp_memcache(struct kvm_hyp_memcache *mc,
				     void *(*to_va)(phys_addr_t phys))
{
	phys_addr_t *p = to_va(mc->head);

	if (!mc->nr_pages)
		return NULL;

	mc->head = *p;
	mc->nr_pages--;

	return p;
}

static inline int __topup_hyp_memcache(struct kvm_hyp_memcache *mc,
				       unsigned long min_pages,
				       void *(*alloc_fn)(void *arg),
				       phys_addr_t (*to_pa)(void *virt),
				       void *arg)
{
	while (mc->nr_pages < min_pages) {
		phys_addr_t *p = alloc_fn(arg);

		if (!p)
			return -ENOMEM;
		push_hyp_memcache(mc, p, to_pa);
	}

	return 0;
}

static inline void __free_hyp_memcache(struct kvm_hyp_memcache *mc,
				       void (*free_fn)(void *virt, void *arg),
				       void *(*to_va)(phys_addr_t phys),
				       void *arg)
{
	while (mc->nr_pages)
		free_fn(pop_hyp_memcache(mc, to_va), arg);
}

void free_hyp_memcache(struct kvm_hyp_memcache *mc);
int topup_hyp_memcache(struct kvm_vcpu *vcpu);

struct kvm_vmid {
	/* The VMID generation used for the virt. memory system */
	u64    vmid_gen;
	u32    vmid;
};

struct kvm_s2_mmu {
	struct kvm_vmid vmid;

	/*
	 * stage2 entry level table
	 *
	 * Two kvm_s2_mmu structures in the same VM can point to the same
	 * pgd here.  This happens when running a guest using a
	 * translation regime that isn't affected by its own stage-2
	 * translation, such as a non-VHE hypervisor running at vEL2, or
	 * for vEL1/EL0 with vHCR_EL2.VM == 0.  In that case, we use the
	 * canonical stage-2 page tables.
	 */
	phys_addr_t	pgd_phys;
	struct kvm_pgtable *pgt;
>>>>>>> upstream/android-13

	/* The last vcpu id that ran on each physical CPU */
	int __percpu *last_vcpu_ran;

<<<<<<< HEAD
=======
	struct kvm_arch *arch;
};

struct kvm_arch_memory_slot {
};

struct kvm_pinned_page {
	struct list_head	link;
	struct page		*page;
};

struct kvm_protected_vm {
	bool enabled;
	int shadow_handle;
	struct mutex shadow_lock;
	struct kvm_hyp_memcache teardown_mc;
	struct list_head pinned_pages;
	gpa_t pvmfw_load_addr;
};

struct kvm_arch {
	struct kvm_s2_mmu mmu;

	/* VTCR_EL2 value for this VM */
	u64    vtcr;

>>>>>>> upstream/android-13
	/* The maximum number of vCPUs depends on the used GIC model */
	int max_vcpus;

	/* Interrupt controller */
	struct vgic_dist	vgic;

	/* Mandated version of PSCI */
	u32 psci_version;
<<<<<<< HEAD
};

#define KVM_NR_MEM_OBJS     40

/*
 * We don't want allocation failures within the mmu code, so we preallocate
 * enough memory for a single page fault in a cache.
 */
struct kvm_mmu_memory_cache {
	int nobjs;
	void *objects[KVM_NR_MEM_OBJS];
=======

	/*
	 * If we encounter a data abort without valid instruction syndrome
	 * information, report this to user space.  User space can (and
	 * should) opt in to this feature if KVM_CAP_ARM_NISV_TO_USER is
	 * supported.
	 */
#define KVM_ARCH_FLAG_RETURN_NISV_IO_ABORT_TO_USER	0
	/* Memory Tagging Extension enabled for the guest */
#define KVM_ARCH_FLAG_MTE_ENABLED			1
	/* Guest has bought into the MMIO guard extension */
#define KVM_ARCH_FLAG_MMIO_GUARD			2
	unsigned long flags;

	/*
	 * VM-wide PMU filter, implemented as a bitmap and big enough for
	 * up to 2^10 events (ARMv8.0) or 2^16 events (ARMv8.1+).
	 */
	unsigned long *pmu_filter;
	unsigned int pmuver;

	u8 pfr0_csv2;
	u8 pfr0_csv3;

	struct kvm_protected_vm pkvm;
};

struct kvm_protected_vcpu {
	/* A pointer to the host's vcpu. */
	struct kvm_vcpu *host_vcpu;

	/* A pointer to the shadow vm. */
	struct kvm_shadow_vm *shadow_vm;

	/* Tracks exit code for the protected guest. */
	int exit_code;

	/*
	 * Track the power state transition of a protected vcpu.
	 * Can be in one of three states:
	 * PSCI_0_2_AFFINITY_LEVEL_ON
	 * PSCI_0_2_AFFINITY_LEVEL_OFF
	 * PSCI_0_2_AFFINITY_LEVEL_PENDING
	 */
	int power_state;

	/* True if this vcpu is currently loaded on a cpu. */
	bool loaded_on_cpu;
>>>>>>> upstream/android-13
};

struct kvm_vcpu_fault_info {
	u32 esr_el2;		/* Hyp Syndrom Register */
	u64 far_el2;		/* Hyp Fault Address Register */
	u64 hpfar_el2;		/* Hyp IPA Fault Address Register */
	u64 disr_el1;		/* Deferred [SError] Status Register */
};

<<<<<<< HEAD
/*
 * 0 is reserved as an invalid value.
 * Order should be kept in sync with the save/restore code.
 */
enum vcpu_sysreg {
	__INVALID_SYSREG__,
=======
enum vcpu_sysreg {
	__INVALID_SYSREG__,   /* 0 is reserved as an invalid value */
>>>>>>> upstream/android-13
	MPIDR_EL1,	/* MultiProcessor Affinity Register */
	CSSELR_EL1,	/* Cache Size Selection Register */
	SCTLR_EL1,	/* System Control Register */
	ACTLR_EL1,	/* Auxiliary Control Register */
	CPACR_EL1,	/* Coprocessor Access Control */
<<<<<<< HEAD
=======
	ZCR_EL1,	/* SVE Control */
>>>>>>> upstream/android-13
	TTBR0_EL1,	/* Translation Table Base Register 0 */
	TTBR1_EL1,	/* Translation Table Base Register 1 */
	TCR_EL1,	/* Translation Control Register */
	ESR_EL1,	/* Exception Syndrome Register */
	AFSR0_EL1,	/* Auxiliary Fault Status Register 0 */
	AFSR1_EL1,	/* Auxiliary Fault Status Register 1 */
	FAR_EL1,	/* Fault Address Register */
	MAIR_EL1,	/* Memory Attribute Indirection Register */
	VBAR_EL1,	/* Vector Base Address Register */
	CONTEXTIDR_EL1,	/* Context ID Register */
	TPIDR_EL0,	/* Thread ID, User R/W */
	TPIDRRO_EL0,	/* Thread ID, User R/O */
	TPIDR_EL1,	/* Thread ID, Privileged */
	AMAIR_EL1,	/* Aux Memory Attribute Indirection Register */
	CNTKCTL_EL1,	/* Timer Control Register (EL1) */
	PAR_EL1,	/* Physical Address Register */
	MDSCR_EL1,	/* Monitor Debug System Control Register */
	MDCCINT_EL1,	/* Monitor Debug Comms Channel Interrupt Enable Reg */
	DISR_EL1,	/* Deferred Interrupt Status Register */

	/* Performance Monitors Registers */
	PMCR_EL0,	/* Control Register */
	PMSELR_EL0,	/* Event Counter Selection Register */
	PMEVCNTR0_EL0,	/* Event Counter Register (0-30) */
	PMEVCNTR30_EL0 = PMEVCNTR0_EL0 + 30,
	PMCCNTR_EL0,	/* Cycle Counter Register */
	PMEVTYPER0_EL0,	/* Event Type Register (0-30) */
	PMEVTYPER30_EL0 = PMEVTYPER0_EL0 + 30,
	PMCCFILTR_EL0,	/* Cycle Count Filter Register */
	PMCNTENSET_EL0,	/* Count Enable Set Register */
	PMINTENSET_EL1,	/* Interrupt Enable Set Register */
	PMOVSSET_EL0,	/* Overflow Flag Status Set Register */
<<<<<<< HEAD
	PMSWINC_EL0,	/* Software Increment Register */
	PMUSERENR_EL0,	/* User Enable Register */

=======
	PMUSERENR_EL0,	/* User Enable Register */

	/* Pointer Authentication Registers in a strict increasing order. */
	APIAKEYLO_EL1,
	APIAKEYHI_EL1,
	APIBKEYLO_EL1,
	APIBKEYHI_EL1,
	APDAKEYLO_EL1,
	APDAKEYHI_EL1,
	APDBKEYLO_EL1,
	APDBKEYHI_EL1,
	APGAKEYLO_EL1,
	APGAKEYHI_EL1,

	ELR_EL1,
	SP_EL1,
	SPSR_EL1,

	CNTVOFF_EL2,
	CNTV_CVAL_EL0,
	CNTV_CTL_EL0,
	CNTP_CVAL_EL0,
	CNTP_CTL_EL0,

	/* Memory Tagging Extension registers */
	RGSR_EL1,	/* Random Allocation Tag Seed Register */
	GCR_EL1,	/* Tag Control Register */
	TFSR_EL1,	/* Tag Fault Status Register (EL1) */
	TFSRE0_EL1,	/* Tag Fault Status Register (EL0) */

>>>>>>> upstream/android-13
	/* 32bit specific registers. Keep them at the end of the range */
	DACR32_EL2,	/* Domain Access Control Register */
	IFSR32_EL2,	/* Instruction Fault Status Register */
	FPEXC32_EL2,	/* Floating-Point Exception Control Register */
	DBGVCR32_EL2,	/* Debug Vector Catch Register */

	NR_SYS_REGS	/* Nothing after this line! */
};

<<<<<<< HEAD
/* 32bit mapping */
#define c0_MPIDR	(MPIDR_EL1 * 2)	/* MultiProcessor ID Register */
#define c0_CSSELR	(CSSELR_EL1 * 2)/* Cache Size Selection Register */
#define c1_SCTLR	(SCTLR_EL1 * 2)	/* System Control Register */
#define c1_ACTLR	(ACTLR_EL1 * 2)	/* Auxiliary Control Register */
#define c1_CPACR	(CPACR_EL1 * 2)	/* Coprocessor Access Control */
#define c2_TTBR0	(TTBR0_EL1 * 2)	/* Translation Table Base Register 0 */
#define c2_TTBR0_high	(c2_TTBR0 + 1)	/* TTBR0 top 32 bits */
#define c2_TTBR1	(TTBR1_EL1 * 2)	/* Translation Table Base Register 1 */
#define c2_TTBR1_high	(c2_TTBR1 + 1)	/* TTBR1 top 32 bits */
#define c2_TTBCR	(TCR_EL1 * 2)	/* Translation Table Base Control R. */
#define c2_TTBCR2	(c2_TTBCR + 1)	/* Translation Table Base Control R. 2 */
#define c3_DACR		(DACR32_EL2 * 2)/* Domain Access Control Register */
#define c5_DFSR		(ESR_EL1 * 2)	/* Data Fault Status Register */
#define c5_IFSR		(IFSR32_EL2 * 2)/* Instruction Fault Status Register */
#define c5_ADFSR	(AFSR0_EL1 * 2)	/* Auxiliary Data Fault Status R */
#define c5_AIFSR	(AFSR1_EL1 * 2)	/* Auxiliary Instr Fault Status R */
#define c6_DFAR		(FAR_EL1 * 2)	/* Data Fault Address Register */
#define c6_IFAR		(c6_DFAR + 1)	/* Instruction Fault Address Register */
#define c7_PAR		(PAR_EL1 * 2)	/* Physical Address Register */
#define c7_PAR_high	(c7_PAR + 1)	/* PAR top 32 bits */
#define c10_PRRR	(MAIR_EL1 * 2)	/* Primary Region Remap Register */
#define c10_NMRR	(c10_PRRR + 1)	/* Normal Memory Remap Register */
#define c12_VBAR	(VBAR_EL1 * 2)	/* Vector Base Address Register */
#define c13_CID		(CONTEXTIDR_EL1 * 2)	/* Context ID Register */
#define c13_TID_URW	(TPIDR_EL0 * 2)	/* Thread ID, User R/W */
#define c13_TID_URO	(TPIDRRO_EL0 * 2)/* Thread ID, User R/O */
#define c13_TID_PRIV	(TPIDR_EL1 * 2)	/* Thread ID, Privileged */
#define c10_AMAIR0	(AMAIR_EL1 * 2)	/* Aux Memory Attr Indirection Reg */
#define c10_AMAIR1	(c10_AMAIR0 + 1)/* Aux Memory Attr Indirection Reg */
#define c14_CNTKCTL	(CNTKCTL_EL1 * 2) /* Timer Control Register (PL1) */

#define cp14_DBGDSCRext	(MDSCR_EL1 * 2)
#define cp14_DBGBCR0	(DBGBCR0_EL1 * 2)
#define cp14_DBGBVR0	(DBGBVR0_EL1 * 2)
#define cp14_DBGBXVR0	(cp14_DBGBVR0 + 1)
#define cp14_DBGWCR0	(DBGWCR0_EL1 * 2)
#define cp14_DBGWVR0	(DBGWVR0_EL1 * 2)
#define cp14_DBGDCCINT	(MDCCINT_EL1 * 2)
#define cp14_DBGVCR	(DBGVCR32_EL2 * 2)

#define NR_COPRO_REGS	(NR_SYS_REGS * 2)

struct kvm_cpu_context {
	struct kvm_regs	gp_regs;
	union {
		u64 sys_regs[NR_SYS_REGS];
		u32 copro[NR_COPRO_REGS];
	};
=======
struct kvm_cpu_context {
	struct user_pt_regs regs;	/* sp = sp_el0 */

	u64	spsr_abt;
	u64	spsr_und;
	u64	spsr_irq;
	u64	spsr_fiq;

	struct user_fpsimd_state fp_regs;

	u64 sys_regs[NR_SYS_REGS];
>>>>>>> upstream/android-13

	struct kvm_vcpu *__hyp_running_vcpu;
};

<<<<<<< HEAD
typedef struct kvm_cpu_context kvm_cpu_context_t;
=======
struct kvm_pmu_events {
	u32 events_host;
	u32 events_guest;
};

struct kvm_host_data {
	struct kvm_cpu_context host_ctxt;
	struct kvm_pmu_events pmu_events;
};

struct kvm_host_psci_config {
	/* PSCI version used by host. */
	u32 version;
	u32 smccc_version;

	/* Function IDs used by host if version is v0.1. */
	struct psci_0_1_function_ids function_ids_0_1;

	bool psci_0_1_cpu_suspend_implemented;
	bool psci_0_1_cpu_on_implemented;
	bool psci_0_1_cpu_off_implemented;
	bool psci_0_1_migrate_implemented;
};

extern struct kvm_host_psci_config kvm_nvhe_sym(kvm_host_psci_config);
#define kvm_host_psci_config CHOOSE_NVHE_SYM(kvm_host_psci_config)

extern s64 kvm_nvhe_sym(hyp_physvirt_offset);
#define hyp_physvirt_offset CHOOSE_NVHE_SYM(hyp_physvirt_offset)

extern u64 kvm_nvhe_sym(hyp_cpu_logical_map)[NR_CPUS];
#define hyp_cpu_logical_map CHOOSE_NVHE_SYM(hyp_cpu_logical_map)

enum pkvm_iommu_driver_id {
	PKVM_IOMMU_DRIVER_S2MPU,
	PKVM_IOMMU_DRIVER_SYSMMU_SYNC,
	PKVM_IOMMU_NR_DRIVERS,
};

enum pkvm_iommu_pm_event {
	PKVM_IOMMU_PM_SUSPEND,
	PKVM_IOMMU_PM_RESUME,
};

int pkvm_iommu_driver_init(enum pkvm_iommu_driver_id drv_id, void *data, size_t size);
int pkvm_iommu_register(struct device *dev, enum pkvm_iommu_driver_id drv_id,
			phys_addr_t pa, size_t size, struct device *parent);
int pkvm_iommu_suspend(struct device *dev);
int pkvm_iommu_resume(struct device *dev);

int pkvm_iommu_s2mpu_register(struct device *dev, phys_addr_t pa);
int pkvm_iommu_sysmmu_sync_register(struct device *dev, phys_addr_t pa,
				    struct device *parent);
/* Reject future calls to pkvm_iommu_driver_init() and pkvm_iommu_register(). */
int pkvm_iommu_finalize(void);
>>>>>>> upstream/android-13

struct vcpu_reset_state {
	unsigned long	pc;
	unsigned long	r0;
	bool		be;
	bool		reset;
};

struct kvm_vcpu_arch {
	struct kvm_cpu_context ctxt;
<<<<<<< HEAD

	/* HYP configuration */
	u64 hcr_el2;
	u32 mdcr_el2;
=======
	void *sve_state;
	unsigned int sve_max_vl;

	/* Stage 2 paging state used by the hardware on next switch */
	struct kvm_s2_mmu *hw_mmu;

	/* Values of trap registers for the guest. */
	u64 hcr_el2;
	u64 mdcr_el2;
	u64 cptr_el2;

	/* Values of trap registers for the host before guest entry. */
	u64 mdcr_el2_host;
>>>>>>> upstream/android-13

	/* Exception Information */
	struct kvm_vcpu_fault_info fault;

<<<<<<< HEAD
	/* State of various workarounds, see kvm_asm.h for bit assignment */
	u64 workaround_flags;

=======
>>>>>>> upstream/android-13
	/* Miscellaneous vcpu state flags */
	u64 flags;

	/*
	 * We maintain more than a single set of debug registers to support
	 * debugging the guest from the host and to maintain separate host and
	 * guest state during world switches. vcpu_debug_state are the debug
	 * registers of the vcpu as the guest sees them.  host_debug_state are
	 * the host registers which are saved and restored during
	 * world switches. external_debug_state contains the debug
	 * values we want to debug the guest. This is set via the
	 * KVM_SET_GUEST_DEBUG ioctl.
	 *
	 * debug_ptr points to the set of debug registers that should be loaded
	 * onto the hardware when running the guest.
	 */
	struct kvm_guest_debug_arch *debug_ptr;
	struct kvm_guest_debug_arch vcpu_debug_state;
	struct kvm_guest_debug_arch external_debug_state;

<<<<<<< HEAD
	/* Pointer to host CPU context */
	kvm_cpu_context_t *host_cpu_context;

	struct thread_info *host_thread_info;	/* hyp VA */
	struct user_fpsimd_state *host_fpsimd_state;	/* hyp VA */
=======
	struct user_fpsimd_state *host_fpsimd_state;	/* hyp VA */
	struct task_struct *parent_task;
>>>>>>> upstream/android-13

	struct {
		/* {Break,watch}point registers */
		struct kvm_guest_debug_arch regs;
		/* Statistical profiling extension */
		u64 pmscr_el1;
<<<<<<< HEAD
=======
		/* Self-hosted trace */
		u64 trfcr_el1;
>>>>>>> upstream/android-13
	} host_debug_state;

	/* VGIC state */
	struct vgic_cpu vgic_cpu;
	struct arch_timer_cpu timer_cpu;
	struct kvm_pmu pmu;

	/*
	 * Anything that is not used directly from assembly code goes
	 * here.
	 */

	/*
	 * Guest registers we preserve during guest debugging.
	 *
	 * These shadow registers are updated by the kvm_handle_sys_reg
	 * trap handler if the guest accesses or updates them while we
	 * are using guest debug.
	 */
	struct {
		u32	mdscr_el1;
	} guest_debug_preserved;

	/* vcpu power-off state */
	bool power_off;

	/* Don't run the guest (internal implementation need) */
	bool pause;

<<<<<<< HEAD
	/* IO related fields */
	struct kvm_decode mmio_decode;

	/* Cache some mmu pages needed inside spinlock regions */
	struct kvm_mmu_memory_cache mmu_page_cache;
=======
	union {
		/* Cache some mmu pages needed inside spinlock regions */
		struct kvm_mmu_memory_cache mmu_page_cache;
		/* Pages to be donated to pkvm/EL2e if it runs out */
		struct kvm_hyp_memcache pkvm_memcache;
	};
>>>>>>> upstream/android-13

	/* Target CPU and feature flags */
	int target;
	DECLARE_BITMAP(features, KVM_VCPU_MAX_FEATURES);

<<<<<<< HEAD
	/* Detect first run of a vcpu */
	bool has_run_once;

=======
>>>>>>> upstream/android-13
	/* Virtual SError ESR to restore when HCR_EL2.VSE is set */
	u64 vsesr_el2;

	/* Additional reset state */
	struct vcpu_reset_state	reset_state;

	/* True when deferrable sysregs are loaded on the physical CPU,
<<<<<<< HEAD
	 * see kvm_vcpu_load_sysregs and kvm_vcpu_put_sysregs. */
	bool sysregs_loaded_on_cpu;
};

=======
	 * see kvm_vcpu_load_sysregs_vhe and kvm_vcpu_put_sysregs_vhe. */
	bool sysregs_loaded_on_cpu;

	/* Guest PV state */
	struct {
		u64 last_steal;
		gpa_t base;
	} steal;

	struct kvm_protected_vcpu pkvm;
};

/* Pointer to the vcpu's SVE FFR for sve_{save,load}_state() */
#define vcpu_sve_pffr(vcpu) (kern_hyp_va((vcpu)->arch.sve_state) +	\
			     sve_ffr_offset((vcpu)->arch.sve_max_vl))

#define vcpu_sve_max_vq(vcpu)	sve_vq_from_vl((vcpu)->arch.sve_max_vl)

#define vcpu_sve_state_size(vcpu) ({					\
	size_t __size_ret;						\
	unsigned int __vcpu_vq;						\
									\
	if (WARN_ON(!sve_vl_valid((vcpu)->arch.sve_max_vl))) {		\
		__size_ret = 0;						\
	} else {							\
		__vcpu_vq = vcpu_sve_max_vq(vcpu);			\
		__size_ret = SVE_SIG_REGS_SIZE(__vcpu_vq);		\
	}								\
									\
	__size_ret;							\
})

>>>>>>> upstream/android-13
/* vcpu_arch flags field values: */
#define KVM_ARM64_DEBUG_DIRTY		(1 << 0)
#define KVM_ARM64_FP_ENABLED		(1 << 1) /* guest FP regs loaded */
#define KVM_ARM64_FP_HOST		(1 << 2) /* host FP regs loaded */
<<<<<<< HEAD
#define KVM_ARM64_HOST_SVE_IN_USE	(1 << 3) /* backup for host TIF_SVE */
#define KVM_ARM64_HOST_SVE_ENABLED	(1 << 4) /* SVE enabled for EL0 */

#define vcpu_gp_regs(v)		(&(v)->arch.ctxt.gp_regs)

/*
 * Only use __vcpu_sys_reg if you know you want the memory backed version of a
 * register, and not the one most recently accessed by a running VCPU.  For
 * example, for userspace access or for system registers that are never context
 * switched, but only emulated.
 */
#define __vcpu_sys_reg(v,r)	((v)->arch.ctxt.sys_regs[(r)])

u64 vcpu_read_sys_reg(struct kvm_vcpu *vcpu, int reg);
void vcpu_write_sys_reg(struct kvm_vcpu *vcpu, u64 val, int reg);

/*
 * CP14 and CP15 live in the same array, as they are backed by the
 * same system registers.
 */
#define CPx_BIAS		IS_ENABLED(CONFIG_CPU_BIG_ENDIAN)

#define vcpu_cp14(v,r)		((v)->arch.ctxt.copro[(r) ^ CPx_BIAS])
#define vcpu_cp15(v,r)		((v)->arch.ctxt.copro[(r) ^ CPx_BIAS])

struct kvm_vm_stat {
	ulong remote_tlb_flush;
};

struct kvm_vcpu_stat {
	u64 halt_successful_poll;
	u64 halt_attempted_poll;
	u64 halt_poll_invalid;
	u64 halt_wakeup;
=======
#define KVM_ARM64_HOST_SVE_ENABLED	(1 << 4) /* SVE enabled for EL0 */
#define KVM_ARM64_GUEST_HAS_SVE		(1 << 5) /* SVE exposed to guest */
#define KVM_ARM64_VCPU_SVE_FINALIZED	(1 << 6) /* SVE config completed */
#define KVM_ARM64_GUEST_HAS_PTRAUTH	(1 << 7) /* PTRAUTH exposed to guest */
#define KVM_ARM64_PENDING_EXCEPTION	(1 << 8) /* Exception pending */
/*
 * Overlaps with KVM_ARM64_EXCEPT_MASK on purpose so that it can't be
 * set together with an exception...
 */
#define KVM_ARM64_INCREMENT_PC		(1 << 9) /* Increment PC */
#define KVM_ARM64_EXCEPT_MASK		(7 << 9) /* Target EL/MODE */
/*
 * When KVM_ARM64_PENDING_EXCEPTION is set, KVM_ARM64_EXCEPT_MASK can
 * take the following values:
 *
 * For AArch32 EL1:
 */
#define KVM_ARM64_EXCEPT_AA32_UND	(0 << 9)
#define KVM_ARM64_EXCEPT_AA32_IABT	(1 << 9)
#define KVM_ARM64_EXCEPT_AA32_DABT	(2 << 9)
/* For AArch64: */
#define KVM_ARM64_EXCEPT_AA64_ELx_SYNC	(0 << 9)
#define KVM_ARM64_EXCEPT_AA64_ELx_IRQ	(1 << 9)
#define KVM_ARM64_EXCEPT_AA64_ELx_FIQ	(2 << 9)
#define KVM_ARM64_EXCEPT_AA64_ELx_SERR	(3 << 9)
#define KVM_ARM64_EXCEPT_AA64_EL1	(0 << 11)
#define KVM_ARM64_EXCEPT_AA64_EL2	(1 << 11)

#define KVM_ARM64_DEBUG_STATE_SAVE_SPE	(1 << 12) /* Save SPE context if active  */
#define KVM_ARM64_DEBUG_STATE_SAVE_TRBE	(1 << 13) /* Save TRBE context if active  */
#define KVM_ARM64_FP_FOREIGN_FPSTATE	(1 << 14)
#define KVM_ARM64_PKVM_STATE_DIRTY	(1 << 15)

#define KVM_GUESTDBG_VALID_MASK (KVM_GUESTDBG_ENABLE | \
				 KVM_GUESTDBG_USE_SW_BP | \
				 KVM_GUESTDBG_USE_HW | \
				 KVM_GUESTDBG_SINGLESTEP)

#define vcpu_has_sve(vcpu) (system_supports_sve() &&			\
			    ((vcpu)->arch.flags & KVM_ARM64_GUEST_HAS_SVE))

#ifdef CONFIG_ARM64_PTR_AUTH
#define vcpu_has_ptrauth(vcpu)						\
	((cpus_have_final_cap(ARM64_HAS_ADDRESS_AUTH) ||		\
	  cpus_have_final_cap(ARM64_HAS_GENERIC_AUTH)) &&		\
	 (vcpu)->arch.flags & KVM_ARM64_GUEST_HAS_PTRAUTH)
#else
#define vcpu_has_ptrauth(vcpu)		false
#endif

#define vcpu_gp_regs(v)		(&(v)->arch.ctxt.regs)

/*
 * Only use __vcpu_sys_reg/ctxt_sys_reg if you know you want the
 * memory backed version of a register, and not the one most recently
 * accessed by a running VCPU.  For example, for userspace access or
 * for system registers that are never context switched, but only
 * emulated.
 */
#define __ctxt_sys_reg(c,r)	(&(c)->sys_regs[(r)])

#define ctxt_sys_reg(c,r)	(*__ctxt_sys_reg(c,r))

#define __vcpu_sys_reg(v,r)	(ctxt_sys_reg(&(v)->arch.ctxt, (r)))

static inline bool __vcpu_read_sys_reg_from_cpu(int reg, u64 *val)
{
	/*
	 * *** VHE ONLY ***
	 *
	 * System registers listed in the switch are not saved on every
	 * exit from the guest but are only saved on vcpu_put.
	 *
	 * Note that MPIDR_EL1 for the guest is set by KVM via VMPIDR_EL2 but
	 * should never be listed below, because the guest cannot modify its
	 * own MPIDR_EL1 and MPIDR_EL1 is accessed for VCPU A from VCPU B's
	 * thread when emulating cross-VCPU communication.
	 */
	if (!has_vhe())
		return false;

	switch (reg) {
	case CSSELR_EL1:	*val = read_sysreg_s(SYS_CSSELR_EL1);	break;
	case SCTLR_EL1:		*val = read_sysreg_s(SYS_SCTLR_EL12);	break;
	case CPACR_EL1:		*val = read_sysreg_s(SYS_CPACR_EL12);	break;
	case TTBR0_EL1:		*val = read_sysreg_s(SYS_TTBR0_EL12);	break;
	case TTBR1_EL1:		*val = read_sysreg_s(SYS_TTBR1_EL12);	break;
	case TCR_EL1:		*val = read_sysreg_s(SYS_TCR_EL12);	break;
	case ESR_EL1:		*val = read_sysreg_s(SYS_ESR_EL12);	break;
	case AFSR0_EL1:		*val = read_sysreg_s(SYS_AFSR0_EL12);	break;
	case AFSR1_EL1:		*val = read_sysreg_s(SYS_AFSR1_EL12);	break;
	case FAR_EL1:		*val = read_sysreg_s(SYS_FAR_EL12);	break;
	case MAIR_EL1:		*val = read_sysreg_s(SYS_MAIR_EL12);	break;
	case VBAR_EL1:		*val = read_sysreg_s(SYS_VBAR_EL12);	break;
	case CONTEXTIDR_EL1:	*val = read_sysreg_s(SYS_CONTEXTIDR_EL12);break;
	case TPIDR_EL0:		*val = read_sysreg_s(SYS_TPIDR_EL0);	break;
	case TPIDRRO_EL0:	*val = read_sysreg_s(SYS_TPIDRRO_EL0);	break;
	case TPIDR_EL1:		*val = read_sysreg_s(SYS_TPIDR_EL1);	break;
	case AMAIR_EL1:		*val = read_sysreg_s(SYS_AMAIR_EL12);	break;
	case CNTKCTL_EL1:	*val = read_sysreg_s(SYS_CNTKCTL_EL12);	break;
	case ELR_EL1:		*val = read_sysreg_s(SYS_ELR_EL12);	break;
	case PAR_EL1:		*val = read_sysreg_par();		break;
	case DACR32_EL2:	*val = read_sysreg_s(SYS_DACR32_EL2);	break;
	case IFSR32_EL2:	*val = read_sysreg_s(SYS_IFSR32_EL2);	break;
	case DBGVCR32_EL2:	*val = read_sysreg_s(SYS_DBGVCR32_EL2);	break;
	default:		return false;
	}

	return true;
}

static inline bool __vcpu_write_sys_reg_to_cpu(u64 val, int reg)
{
	/*
	 * *** VHE ONLY ***
	 *
	 * System registers listed in the switch are not restored on every
	 * entry to the guest but are only restored on vcpu_load.
	 *
	 * Note that MPIDR_EL1 for the guest is set by KVM via VMPIDR_EL2 but
	 * should never be listed below, because the MPIDR should only be set
	 * once, before running the VCPU, and never changed later.
	 */
	if (!has_vhe())
		return false;

	switch (reg) {
	case CSSELR_EL1:	write_sysreg_s(val, SYS_CSSELR_EL1);	break;
	case SCTLR_EL1:		write_sysreg_s(val, SYS_SCTLR_EL12);	break;
	case CPACR_EL1:		write_sysreg_s(val, SYS_CPACR_EL12);	break;
	case TTBR0_EL1:		write_sysreg_s(val, SYS_TTBR0_EL12);	break;
	case TTBR1_EL1:		write_sysreg_s(val, SYS_TTBR1_EL12);	break;
	case TCR_EL1:		write_sysreg_s(val, SYS_TCR_EL12);	break;
	case ESR_EL1:		write_sysreg_s(val, SYS_ESR_EL12);	break;
	case AFSR0_EL1:		write_sysreg_s(val, SYS_AFSR0_EL12);	break;
	case AFSR1_EL1:		write_sysreg_s(val, SYS_AFSR1_EL12);	break;
	case FAR_EL1:		write_sysreg_s(val, SYS_FAR_EL12);	break;
	case MAIR_EL1:		write_sysreg_s(val, SYS_MAIR_EL12);	break;
	case VBAR_EL1:		write_sysreg_s(val, SYS_VBAR_EL12);	break;
	case CONTEXTIDR_EL1:	write_sysreg_s(val, SYS_CONTEXTIDR_EL12);break;
	case TPIDR_EL0:		write_sysreg_s(val, SYS_TPIDR_EL0);	break;
	case TPIDRRO_EL0:	write_sysreg_s(val, SYS_TPIDRRO_EL0);	break;
	case TPIDR_EL1:		write_sysreg_s(val, SYS_TPIDR_EL1);	break;
	case AMAIR_EL1:		write_sysreg_s(val, SYS_AMAIR_EL12);	break;
	case CNTKCTL_EL1:	write_sysreg_s(val, SYS_CNTKCTL_EL12);	break;
	case ELR_EL1:		write_sysreg_s(val, SYS_ELR_EL12);	break;
	case PAR_EL1:		write_sysreg_s(val, SYS_PAR_EL1);	break;
	case DACR32_EL2:	write_sysreg_s(val, SYS_DACR32_EL2);	break;
	case IFSR32_EL2:	write_sysreg_s(val, SYS_IFSR32_EL2);	break;
	case DBGVCR32_EL2:	write_sysreg_s(val, SYS_DBGVCR32_EL2);	break;
	default:		return false;
	}

	return true;
}

static inline u64 vcpu_arch_read_sys_reg(const struct kvm_vcpu_arch *vcpu_arch, int reg)
{
	u64 val = 0x8badf00d8badf00d;

	/* sysregs_loaded_on_cpu is only used in VHE */
	if (!is_nvhe_hyp_code() && vcpu_arch->sysregs_loaded_on_cpu &&
	    __vcpu_read_sys_reg_from_cpu(reg, &val))
		return val;

	return ctxt_sys_reg(&vcpu_arch->ctxt, reg);
}

static inline void vcpu_arch_write_sys_reg(struct kvm_vcpu_arch *vcpu_arch, u64 val, int reg)
{
	/* sysregs_loaded_on_cpu is only used in VHE */
	if (!is_nvhe_hyp_code() && vcpu_arch->sysregs_loaded_on_cpu &&
	    __vcpu_write_sys_reg_to_cpu(val, reg))
		return;

	 ctxt_sys_reg(&vcpu_arch->ctxt, reg) = val;
}

#define vcpu_read_sys_reg(vcpu, reg) vcpu_arch_read_sys_reg(&((vcpu)->arch), reg)
#define vcpu_write_sys_reg(vcpu, val, reg) vcpu_arch_write_sys_reg(&((vcpu)->arch), val, reg)

struct kvm_vm_stat {
	struct kvm_vm_stat_generic generic;
};

struct kvm_vcpu_stat {
	struct kvm_vcpu_stat_generic generic;
>>>>>>> upstream/android-13
	u64 hvc_exit_stat;
	u64 wfe_exit_stat;
	u64 wfi_exit_stat;
	u64 mmio_exit_user;
	u64 mmio_exit_kernel;
<<<<<<< HEAD
	u64 exits;
};

int kvm_vcpu_preferred_target(struct kvm_vcpu_init *init);
=======
	u64 signal_exits;
	u64 exits;
};

void kvm_vcpu_preferred_target(struct kvm_vcpu_init *init);
>>>>>>> upstream/android-13
unsigned long kvm_arm_num_regs(struct kvm_vcpu *vcpu);
int kvm_arm_copy_reg_indices(struct kvm_vcpu *vcpu, u64 __user *indices);
int kvm_arm_get_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);
int kvm_arm_set_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *reg);
<<<<<<< HEAD
=======

unsigned long kvm_arm_num_sys_reg_descs(struct kvm_vcpu *vcpu);
int kvm_arm_copy_sys_reg_indices(struct kvm_vcpu *vcpu, u64 __user *uindices);
int kvm_arm_sys_reg_get_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *);
int kvm_arm_sys_reg_set_reg(struct kvm_vcpu *vcpu, const struct kvm_one_reg *);

>>>>>>> upstream/android-13
int __kvm_arm_vcpu_get_events(struct kvm_vcpu *vcpu,
			      struct kvm_vcpu_events *events);

int __kvm_arm_vcpu_set_events(struct kvm_vcpu *vcpu,
			      struct kvm_vcpu_events *events);

#define KVM_ARCH_WANT_MMU_NOTIFIER
<<<<<<< HEAD
int kvm_unmap_hva_range(struct kvm *kvm,
			unsigned long start, unsigned long end, bool blockable);
void kvm_set_spte_hva(struct kvm *kvm, unsigned long hva, pte_t pte);
int kvm_age_hva(struct kvm *kvm, unsigned long start, unsigned long end);
int kvm_test_age_hva(struct kvm *kvm, unsigned long hva);

struct kvm_vcpu *kvm_arm_get_running_vcpu(void);
struct kvm_vcpu * __percpu *kvm_get_running_vcpus(void);
void kvm_arm_halt_guest(struct kvm *kvm);
void kvm_arm_resume_guest(struct kvm *kvm);

u64 __kvm_call_hyp(void *hypfn, ...);
#define kvm_call_hyp(f, ...) __kvm_call_hyp(kvm_ksym_ref(f), ##__VA_ARGS__)

void force_vm_exit(const cpumask_t *mask);
void kvm_mmu_wp_memory_region(struct kvm *kvm, int slot);

int handle_exit(struct kvm_vcpu *vcpu, struct kvm_run *run,
		int exception_index);
void handle_exit_early(struct kvm_vcpu *vcpu, struct kvm_run *run,
		       int exception_index);
=======

void kvm_arm_halt_guest(struct kvm *kvm);
void kvm_arm_resume_guest(struct kvm *kvm);

#define vcpu_has_run_once(vcpu)	!!rcu_access_pointer((vcpu)->pid)

#ifndef __KVM_NVHE_HYPERVISOR__
#define kvm_call_hyp_nvhe(f, ...)						\
	({								\
		struct arm_smccc_res res;				\
									\
		arm_smccc_1_1_hvc(KVM_HOST_SMCCC_FUNC(f),		\
				  ##__VA_ARGS__, &res);			\
		WARN_ON(res.a0 != SMCCC_RET_SUCCESS);			\
									\
		res.a1;							\
	})

/*
 * The couple of isb() below are there to guarantee the same behaviour
 * on VHE as on !VHE, where the eret to EL1 acts as a context
 * synchronization event.
 */
#define kvm_call_hyp(f, ...)						\
	do {								\
		if (has_vhe()) {					\
			f(__VA_ARGS__);					\
			isb();						\
		} else {						\
			kvm_call_hyp_nvhe(f, ##__VA_ARGS__);		\
		}							\
	} while(0)

#define kvm_call_hyp_ret(f, ...)					\
	({								\
		typeof(f(__VA_ARGS__)) ret;				\
									\
		if (has_vhe()) {					\
			ret = f(__VA_ARGS__);				\
			isb();						\
		} else {						\
			ret = kvm_call_hyp_nvhe(f, ##__VA_ARGS__);	\
		}							\
									\
		ret;							\
	})
#else /* __KVM_NVHE_HYPERVISOR__ */
#define kvm_call_hyp(f, ...) f(__VA_ARGS__)
#define kvm_call_hyp_ret(f, ...) f(__VA_ARGS__)
#define kvm_call_hyp_nvhe(f, ...) f(__VA_ARGS__)
#endif /* __KVM_NVHE_HYPERVISOR__ */

void force_vm_exit(const cpumask_t *mask);

int handle_exit(struct kvm_vcpu *vcpu, int exception_index);
void handle_exit_early(struct kvm_vcpu *vcpu, int exception_index);

int kvm_handle_cp14_load_store(struct kvm_vcpu *vcpu);
int kvm_handle_cp14_32(struct kvm_vcpu *vcpu);
int kvm_handle_cp14_64(struct kvm_vcpu *vcpu);
int kvm_handle_cp15_32(struct kvm_vcpu *vcpu);
int kvm_handle_cp15_64(struct kvm_vcpu *vcpu);
int kvm_handle_sys_reg(struct kvm_vcpu *vcpu);

void kvm_reset_sys_regs(struct kvm_vcpu *vcpu);

void kvm_sys_reg_table_init(void);

/* MMIO helpers */
void kvm_mmio_write_buf(void *buf, unsigned int len, unsigned long data);
unsigned long kvm_mmio_read_buf(const void *buf, unsigned int len);

int kvm_handle_mmio_return(struct kvm_vcpu *vcpu);
int io_mem_abort(struct kvm_vcpu *vcpu, phys_addr_t fault_ipa);
>>>>>>> upstream/android-13

int kvm_perf_init(void);
int kvm_perf_teardown(void);

<<<<<<< HEAD
=======
long kvm_hypercall_pv_features(struct kvm_vcpu *vcpu);
gpa_t kvm_init_stolen_time(struct kvm_vcpu *vcpu);
void kvm_update_stolen_time(struct kvm_vcpu *vcpu);

bool kvm_arm_pvtime_supported(void);
int kvm_arm_pvtime_set_attr(struct kvm_vcpu *vcpu,
			    struct kvm_device_attr *attr);
int kvm_arm_pvtime_get_attr(struct kvm_vcpu *vcpu,
			    struct kvm_device_attr *attr);
int kvm_arm_pvtime_has_attr(struct kvm_vcpu *vcpu,
			    struct kvm_device_attr *attr);

static inline void kvm_arm_pvtime_vcpu_init(struct kvm_vcpu_arch *vcpu_arch)
{
	vcpu_arch->steal.base = GPA_INVALID;
}

static inline bool kvm_arm_is_pvtime_enabled(struct kvm_vcpu_arch *vcpu_arch)
{
	return (vcpu_arch->steal.base != GPA_INVALID);
}

>>>>>>> upstream/android-13
void kvm_set_sei_esr(struct kvm_vcpu *vcpu, u64 syndrome);

struct kvm_vcpu *kvm_mpidr_to_vcpu(struct kvm *kvm, unsigned long mpidr);

<<<<<<< HEAD
DECLARE_PER_CPU(kvm_cpu_context_t, kvm_host_cpu_state);

void __kvm_enable_ssbs(void);

static inline void __cpu_init_hyp_mode(phys_addr_t pgd_ptr,
				       unsigned long hyp_stack_ptr,
				       unsigned long vector_ptr)
{
	/*
	 * Calculate the raw per-cpu offset without a translation from the
	 * kernel's mapping to the linear mapping, and store it in tpidr_el2
	 * so that we can use adr_l to access per-cpu variables in EL2.
	 */
	u64 tpidr_el2 = ((u64)this_cpu_ptr(&kvm_host_cpu_state) -
			 (u64)kvm_ksym_ref(kvm_host_cpu_state));

	/*
	 * Call initialization code, and switch to the full blown HYP code.
	 * If the cpucaps haven't been finalized yet, something has gone very
	 * wrong, and hyp will crash and burn when it uses any
	 * cpus_have_const_cap() wrapper.
	 */
	BUG_ON(!static_branch_likely(&arm64_const_caps_ready));
	__kvm_call_hyp((void *)pgd_ptr, hyp_stack_ptr, vector_ptr, tpidr_el2);

	/*
	 * Disabling SSBD on a non-VHE system requires us to enable SSBS
	 * at EL2.
	 */
	if (!has_vhe() && this_cpu_has_cap(ARM64_SSBS) &&
	    arm64_get_ssbd_state() == ARM64_SSBD_FORCE_DISABLE) {
		kvm_call_hyp(__kvm_enable_ssbs);
	}
}

static inline bool kvm_arch_check_sve_has_vhe(void)
{
	/*
	 * The Arm architecture specifies that implementation of SVE
	 * requires VHE also to be implemented.  The KVM code for arm64
	 * relies on this when SVE is present:
	 */
	if (system_supports_sve())
		return has_vhe();
	else
		return true;
}

static inline void kvm_arch_hardware_unsetup(void) {}
static inline void kvm_arch_sync_events(struct kvm *kvm) {}
static inline void kvm_arch_vcpu_uninit(struct kvm_vcpu *vcpu) {}
=======
DECLARE_KVM_HYP_PER_CPU(struct kvm_host_data, kvm_host_data);

static inline void kvm_init_host_cpu_context(struct kvm_cpu_context *cpu_ctxt)
{
	/* The host's MPIDR is immutable, so let's set it up at boot time */
	ctxt_sys_reg(cpu_ctxt, MPIDR_EL1) = read_cpuid_mpidr();
}

static inline bool kvm_system_needs_idmapped_vectors(void)
{
	return cpus_have_const_cap(ARM64_SPECTRE_V3A);
}

void kvm_arm_vcpu_ptrauth_trap(struct kvm_vcpu *vcpu);

static inline void kvm_arch_hardware_unsetup(void) {}
static inline void kvm_arch_sync_events(struct kvm *kvm) {}
>>>>>>> upstream/android-13
static inline void kvm_arch_sched_in(struct kvm_vcpu *vcpu, int cpu) {}
static inline void kvm_arch_vcpu_block_finish(struct kvm_vcpu *vcpu) {}

void kvm_arm_init_debug(void);
void kvm_arm_vcpu_init_debug(struct kvm_vcpu *vcpu);
void kvm_arm_setup_debug(struct kvm_vcpu *vcpu);
void kvm_arm_clear_debug(struct kvm_vcpu *vcpu);
void kvm_arm_reset_debug_ptr(struct kvm_vcpu *vcpu);
<<<<<<< HEAD
bool kvm_arm_handle_step_debug(struct kvm_vcpu *vcpu, struct kvm_run *run);
=======
>>>>>>> upstream/android-13
int kvm_arm_vcpu_arch_set_attr(struct kvm_vcpu *vcpu,
			       struct kvm_device_attr *attr);
int kvm_arm_vcpu_arch_get_attr(struct kvm_vcpu *vcpu,
			       struct kvm_device_attr *attr);
int kvm_arm_vcpu_arch_has_attr(struct kvm_vcpu *vcpu,
			       struct kvm_device_attr *attr);

<<<<<<< HEAD
static inline void __cpu_init_stage2(void)
{
	u32 parange = kvm_call_hyp(__init_stage2_translation);

	WARN_ONCE(parange < 40,
		  "PARange is %d bits, unsupported configuration!", parange);
}
=======
long kvm_vm_ioctl_mte_copy_tags(struct kvm *kvm,
				struct kvm_arm_copy_mte_tags *copy_tags);
>>>>>>> upstream/android-13

/* Guest/host FPSIMD coordination helpers */
int kvm_arch_vcpu_run_map_fp(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_load_fp(struct kvm_vcpu *vcpu);
<<<<<<< HEAD
void kvm_arch_vcpu_ctxsync_fp(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_put_fp(struct kvm_vcpu *vcpu);

#ifdef CONFIG_KVM /* Avoid conflicts with core headers if CONFIG_KVM=n */
static inline int kvm_arch_vcpu_run_pid_change(struct kvm_vcpu *vcpu)
{
	return kvm_arch_vcpu_run_map_fp(vcpu);
}
#endif

static inline void kvm_arm_vhe_guest_enter(void)
{
	local_daif_mask();
}

static inline void kvm_arm_vhe_guest_exit(void)
{
	local_daif_restore(DAIF_PROCCTX_NOIRQ);

	/*
	 * When we exit from the guest we change a number of CPU configuration
	 * parameters, such as traps.  Make sure these changes take effect
	 * before running the host or additional guests.
	 */
	isb();
}

static inline bool kvm_arm_harden_branch_predictor(void)
{
	return cpus_have_const_cap(ARM64_HARDEN_BRANCH_PREDICTOR);
}

#define KVM_SSBD_UNKNOWN		-1
#define KVM_SSBD_FORCE_DISABLE		0
#define KVM_SSBD_KERNEL		1
#define KVM_SSBD_FORCE_ENABLE		2
#define KVM_SSBD_MITIGATED		3

static inline int kvm_arm_have_ssbd(void)
{
	switch (arm64_get_ssbd_state()) {
	case ARM64_SSBD_FORCE_DISABLE:
		return KVM_SSBD_FORCE_DISABLE;
	case ARM64_SSBD_KERNEL:
		return KVM_SSBD_KERNEL;
	case ARM64_SSBD_FORCE_ENABLE:
		return KVM_SSBD_FORCE_ENABLE;
	case ARM64_SSBD_MITIGATED:
		return KVM_SSBD_MITIGATED;
	case ARM64_SSBD_UNKNOWN:
	default:
		return KVM_SSBD_UNKNOWN;
	}
}

void kvm_vcpu_load_sysregs(struct kvm_vcpu *vcpu);
void kvm_vcpu_put_sysregs(struct kvm_vcpu *vcpu);
=======
void kvm_arch_vcpu_ctxflush_fp(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_ctxsync_fp(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_put_fp(struct kvm_vcpu *vcpu);
void kvm_vcpu_unshare_task_fp(struct kvm_vcpu *vcpu);

static inline bool kvm_pmu_counter_deferred(struct perf_event_attr *attr)
{
	return (!has_vhe() && attr->exclude_host);
}

/* Flags for host debug state */
void kvm_arch_vcpu_load_debug_state_flags(struct kvm_vcpu *vcpu);
void kvm_arch_vcpu_put_debug_state_flags(struct kvm_vcpu *vcpu);

#ifdef CONFIG_KVM
void kvm_set_pmu_events(u32 set, struct perf_event_attr *attr);
void kvm_clr_pmu_events(u32 clr);

void kvm_vcpu_pmu_restore_guest(struct kvm_vcpu *vcpu);
void kvm_vcpu_pmu_restore_host(struct kvm_vcpu *vcpu);
#else
static inline void kvm_set_pmu_events(u32 set, struct perf_event_attr *attr) {}
static inline void kvm_clr_pmu_events(u32 clr) {}
#endif

void kvm_vcpu_load_sysregs_vhe(struct kvm_vcpu *vcpu);
void kvm_vcpu_put_sysregs_vhe(struct kvm_vcpu *vcpu);

int kvm_set_ipa_limit(void);
>>>>>>> upstream/android-13

#define __KVM_HAVE_ARCH_VM_ALLOC
struct kvm *kvm_arch_alloc_vm(void);
void kvm_arch_free_vm(struct kvm *kvm);

<<<<<<< HEAD
#define kvm_arm_vcpu_loaded(vcpu)	((vcpu)->arch.sysregs_loaded_on_cpu)

static inline enum mitigation_state kvm_arm_get_spectre_bhb_state(void)
{
	return arm64_get_spectre_bhb_state();
}
=======
#define kvm_vm_is_protected(kvm)	((kvm)->arch.pkvm.enabled)

void kvm_init_protected_traps(struct kvm_vcpu *vcpu);

int kvm_arm_vcpu_finalize(struct kvm_vcpu *vcpu, int feature);
bool kvm_arm_vcpu_is_finalized(struct kvm_vcpu *vcpu);

#define kvm_arm_vcpu_sve_finalized(vcpu) \
	((vcpu)->arch.flags & KVM_ARM64_VCPU_SVE_FINALIZED)

#define kvm_has_mte(kvm)					\
	(system_supports_mte() &&				\
	 test_bit(KVM_ARCH_FLAG_MTE_ENABLED, &(kvm)->arch.flags))
#define kvm_vcpu_has_pmu(vcpu)					\
	(test_bit(KVM_ARM_VCPU_PMU_V3, (vcpu)->arch.features))

int kvm_trng_call(struct kvm_vcpu *vcpu);
#ifdef CONFIG_KVM
extern phys_addr_t hyp_mem_base;
extern phys_addr_t hyp_mem_size;
void __init kvm_hyp_reserve(void);
#else
static inline void kvm_hyp_reserve(void) { }
#endif
>>>>>>> upstream/android-13

#endif /* __ARM64_KVM_HOST_H__ */
