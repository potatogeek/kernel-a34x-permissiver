<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * This control block defines the PACA which defines the processor
 * specific data for each logical processor on the system.
 * There are some pointers defined that are utilized by PLIC.
 *
 * C 2001 PPC 64 Team, IBM Corp
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */
#ifndef _ASM_POWERPC_PACA_H
#define _ASM_POWERPC_PACA_H
#ifdef __KERNEL__

#ifdef CONFIG_PPC64

#include <linux/string.h>
#include <asm/types.h>
#include <asm/lppaca.h>
#include <asm/mmu.h>
#include <asm/page.h>
#ifdef CONFIG_PPC_BOOK3E
#include <asm/exception-64e.h>
#else
#include <asm/exception-64s.h>
#endif
#ifdef CONFIG_KVM_BOOK3S_64_HANDLER
#include <asm/kvm_book3s_asm.h>
#endif
#include <asm/accounting.h>
#include <asm/hmi.h>
#include <asm/cpuidle.h>
#include <asm/atomic.h>
<<<<<<< HEAD
=======
#include <asm/mce.h>

#include <asm-generic/mmiowb_types.h>
>>>>>>> upstream/android-13

register struct paca_struct *local_paca asm("r13");

#if defined(CONFIG_DEBUG_PREEMPT) && defined(CONFIG_SMP)
extern unsigned int debug_smp_processor_id(void); /* from linux/smp.h */
/*
 * Add standard checks that preemption cannot occur when using get_paca():
 * otherwise the paca_struct it points to may be the wrong one just after.
 */
#define get_paca()	((void) debug_smp_processor_id(), local_paca)
#else
#define get_paca()	local_paca
#endif

#ifdef CONFIG_PPC_PSERIES
#define get_lppaca()	(get_paca()->lppaca_ptr)
#endif

#define get_slb_shadow()	(get_paca()->slb_shadow_ptr)

struct task_struct;
<<<<<<< HEAD
=======
struct rtas_args;
>>>>>>> upstream/android-13

/*
 * Defines the layout of the paca.
 *
 * This structure is not directly accessed by firmware or the service
 * processor.
 */
struct paca_struct {
#ifdef CONFIG_PPC_PSERIES
	/*
	 * Because hw_cpu_id, unlike other paca fields, is accessed
	 * routinely from other CPUs (from the IRQ code), we stick to
	 * read-only (after boot) fields in the first cacheline to
	 * avoid cacheline bouncing.
	 */

	struct lppaca *lppaca_ptr;	/* Pointer to LpPaca for PLIC */
#endif /* CONFIG_PPC_PSERIES */

	/*
	 * MAGIC: the spinlock functions in arch/powerpc/lib/locks.c 
	 * load lock_token and paca_index with a single lwz
	 * instruction.  They must travel together and be properly
	 * aligned.
	 */
#ifdef __BIG_ENDIAN__
	u16 lock_token;			/* Constant 0x8000, used in locks */
	u16 paca_index;			/* Logical processor number */
#else
	u16 paca_index;			/* Logical processor number */
	u16 lock_token;			/* Constant 0x8000, used in locks */
#endif

	u64 kernel_toc;			/* Kernel TOC address */
	u64 kernelbase;			/* Base address of kernel */
	u64 kernel_msr;			/* MSR while running in kernel */
	void *emergency_sp;		/* pointer to emergency stack */
	u64 data_offset;		/* per cpu data offset */
	s16 hw_cpu_id;			/* Physical processor number */
	u8 cpu_start;			/* At startup, processor spins until */
					/* this becomes non-zero. */
	u8 kexec_state;		/* set when kexec down has irqs off */
#ifdef CONFIG_PPC_BOOK3S_64
	struct slb_shadow *slb_shadow_ptr;
	struct dtl_entry *dispatch_log;
	struct dtl_entry *dispatch_log_end;
#endif
	u64 dscr_default;		/* per-CPU default DSCR */

#ifdef CONFIG_PPC_BOOK3S_64
	/*
	 * Now, starting in cacheline 2, the exception save areas
	 */
	/* used for most interrupts/exceptions */
	u64 exgen[EX_SIZE] __attribute__((aligned(0x80)));
<<<<<<< HEAD
	u64 exslb[EX_SIZE];	/* used for SLB/segment table misses
 				 * on the linear mapping */
	/* SLB related definitions */
	u16 vmalloc_sllp;
	u16 slb_cache_ptr;
=======

	/* SLB related definitions */
	u16 vmalloc_sllp;
	u8 slb_cache_ptr;
	u8 stab_rr;			/* stab/slb round-robin counter */
#ifdef CONFIG_DEBUG_VM
	u8 in_kernel_slb_handler;
#endif
	u32 slb_used_bitmap;		/* Bitmaps for first 32 SLB entries. */
	u32 slb_kern_bitmap;
>>>>>>> upstream/android-13
	u32 slb_cache[SLB_CACHE_ENTRIES];
#endif /* CONFIG_PPC_BOOK3S_64 */

#ifdef CONFIG_PPC_BOOK3E
	u64 exgen[8] __aligned(0x40);
	/* Keep pgd in the same cacheline as the start of extlb */
	pgd_t *pgd __aligned(0x40); /* Current PGD */
	pgd_t *kernel_pgd;		/* Kernel PGD */

	/* Shared by all threads of a core -- points to tcd of first thread */
	struct tlb_core_data *tcd_ptr;

	/*
	 * We can have up to 3 levels of reentrancy in the TLB miss handler,
	 * in each of four exception levels (normal, crit, mcheck, debug).
	 */
	u64 extlb[12][EX_TLB_SIZE / sizeof(u64)];
	u64 exmc[8];		/* used for machine checks */
	u64 excrit[8];		/* used for crit interrupts */
	u64 exdbg[8];		/* used for debug interrupts */

	/* Kernel stack pointers for use by special exceptions */
	void *mc_kstack;
	void *crit_kstack;
	void *dbg_kstack;

	struct tlb_core_data tcd;
#endif /* CONFIG_PPC_BOOK3E */

#ifdef CONFIG_PPC_BOOK3S
<<<<<<< HEAD
	mm_context_id_t mm_ctx_id;
#ifdef CONFIG_PPC_MM_SLICES
	unsigned char mm_ctx_low_slices_psize[BITS_PER_LONG / BITS_PER_BYTE];
	unsigned char mm_ctx_high_slices_psize[SLICE_ARRAY_SIZE];
	unsigned long mm_ctx_slb_addr_limit;
=======
#ifdef CONFIG_PPC_MM_SLICES
	unsigned char mm_ctx_low_slices_psize[BITS_PER_LONG / BITS_PER_BYTE];
	unsigned char mm_ctx_high_slices_psize[SLICE_ARRAY_SIZE];
>>>>>>> upstream/android-13
#else
	u16 mm_ctx_user_psize;
	u16 mm_ctx_sllp;
#endif
#endif

	/*
	 * then miscellaneous read-write fields
	 */
	struct task_struct *__current;	/* Pointer to current */
	u64 kstack;			/* Saved Kernel stack addr */
<<<<<<< HEAD
	u64 stab_rr;			/* stab/slb round-robin counter */
	u64 saved_r1;			/* r1 save for RTAS calls or PM or EE=0 */
	u64 saved_msr;			/* MSR saved here by enter_rtas */
	u16 trap_save;			/* Used when bad stack is encountered */
	u8 irq_soft_mask;		/* mask for irq soft masking */
	u8 irq_happened;		/* irq happened while soft-disabled */
	u8 io_sync;			/* writel() needs spin_unlock sync */
	u8 irq_work_pending;		/* IRQ_WORK interrupt while soft-disable */
	u8 nap_state_lost;		/* NV GPR values lost in power7_idle */
=======
	u64 saved_r1;			/* r1 save for RTAS calls or PM or EE=0 */
	u64 saved_msr;			/* MSR saved here by enter_rtas */
#ifdef CONFIG_PPC64
	u64 exit_save_r1;		/* Syscall/interrupt R1 save */
#endif
#ifdef CONFIG_PPC_BOOK3E
	u16 trap_save;			/* Used when bad stack is encountered */
#endif
#ifdef CONFIG_PPC_BOOK3S_64
	u8 hsrr_valid;			/* HSRRs set for HRFID */
	u8 srr_valid;			/* SRRs set for RFID */
#endif
	u8 irq_soft_mask;		/* mask for irq soft masking */
	u8 irq_happened;		/* irq happened while soft-disabled */
	u8 irq_work_pending;		/* IRQ_WORK interrupt while soft-disable */
>>>>>>> upstream/android-13
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
	u8 pmcregs_in_use;		/* pseries puts this in lppaca */
#endif
	u64 sprg_vdso;			/* Saved user-visible sprg */
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	u64 tm_scratch;                 /* TM scratch area for reclaim */
#endif

#ifdef CONFIG_PPC_POWERNV
<<<<<<< HEAD
	/* Per-core mask tracking idle threads and a lock bit-[L][TTTTTTTT] */
	u32 *core_idle_state_ptr;
	u8 thread_idle_state;		/* PNV_THREAD_RUNNING/NAP/SLEEP	*/
	/* Mask to indicate thread id in core */
	u8 thread_mask;
	/* Mask to denote subcore sibling threads */
	u8 subcore_sibling_mask;
	/* Flag to request this thread not to stop */
	atomic_t dont_stop;
	/* The PSSCR value that the kernel requested before going to stop */
	u64 requested_psscr;

	/*
	 * Save area for additional SPRs that need to be
	 * saved/restored during cpuidle stop.
	 */
	struct stop_sprs stop_sprs;
=======
	/* PowerNV idle fields */
	/* PNV_CORE_IDLE_* bits, all siblings work on thread 0 paca */
	unsigned long idle_state;
	union {
		/* P7/P8 specific fields */
		struct {
			/* PNV_THREAD_RUNNING/NAP/SLEEP	*/
			u8 thread_idle_state;
			/* Mask to denote subcore sibling threads */
			u8 subcore_sibling_mask;
		};

		/* P9 specific fields */
		struct {
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
			/* The PSSCR value that the kernel requested before going to stop */
			u64 requested_psscr;
			/* Flag to request this thread not to stop */
			atomic_t dont_stop;
#endif
		};
	};
>>>>>>> upstream/android-13
#endif

#ifdef CONFIG_PPC_BOOK3S_64
	/* Non-maskable exceptions that are not performance critical */
	u64 exnmi[EX_SIZE];	/* used for system reset (nmi) */
	u64 exmc[EX_SIZE];	/* used for machine checks */
#endif
#ifdef CONFIG_PPC_BOOK3S_64
	/* Exclusive stacks for system reset and machine check exception. */
	void *nmi_emergency_sp;
	void *mc_emergency_sp;

	u16 in_nmi;			/* In nmi handler */

	/*
	 * Flag to check whether we are in machine check early handler
	 * and already using emergency stack.
	 */
	u16 in_mce;
	u8 hmi_event_available;		/* HMI event is available */
	u8 hmi_p9_special_emu;		/* HMI P9 special emulation */
<<<<<<< HEAD
=======
	u32 hmi_irqs;			/* HMI irq stat */
>>>>>>> upstream/android-13
#endif
	u8 ftrace_enabled;		/* Hard disable ftrace */

	/* Stuff for accurate time accounting */
	struct cpu_accounting_data accounting;
	u64 dtl_ridx;			/* read index in dispatch log */
	struct dtl_entry *dtl_curr;	/* pointer corresponding to dtl_ridx */

#ifdef CONFIG_KVM_BOOK3S_HANDLER
#ifdef CONFIG_KVM_BOOK3S_PR_POSSIBLE
	/* We use this to store guest state in */
	struct kvmppc_book3s_shadow_vcpu shadow_vcpu;
#endif
	struct kvmppc_host_state kvm_hstate;
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
	/*
	 * Bitmap for sibling subcore status. See kvm/book3s_hv_ras.c for
	 * more details
	 */
	struct sibling_subcore_state *sibling_subcore_state;
#endif
#endif
#ifdef CONFIG_PPC_BOOK3S_64
	/*
	 * rfi fallback flush must be in its own cacheline to prevent
	 * other paca data leaking into the L1d
	 */
	u64 exrfi[EX_SIZE] __aligned(0x80);
	void *rfi_flush_fallback_area;
	u64 l1d_flush_size;
#endif
#ifdef CONFIG_PPC_PSERIES
<<<<<<< HEAD
	u8 *mce_data_buf;		/* buffer to hold per cpu rtas errlog */
#endif /* CONFIG_PPC_PSERIES */
=======
	struct rtas_args *rtas_args_reentrant;
	u8 *mce_data_buf;		/* buffer to hold per cpu rtas errlog */
#endif /* CONFIG_PPC_PSERIES */

#ifdef CONFIG_PPC_BOOK3S_64
	/* Capture SLB related old contents in MCE handler. */
	struct slb_entry *mce_faulty_slbs;
	u16 slb_save_cache_ptr;
#endif /* CONFIG_PPC_BOOK3S_64 */
#ifdef CONFIG_STACKPROTECTOR
	unsigned long canary;
#endif
#ifdef CONFIG_MMIOWB
	struct mmiowb_state mmiowb_state;
#endif
#ifdef CONFIG_PPC_BOOK3S_64
	struct mce_info *mce_info;
#endif /* CONFIG_PPC_BOOK3S_64 */
>>>>>>> upstream/android-13
} ____cacheline_aligned;

extern void copy_mm_to_paca(struct mm_struct *mm);
extern struct paca_struct **paca_ptrs;
extern void initialise_paca(struct paca_struct *new_paca, int cpu);
extern void setup_paca(struct paca_struct *new_paca);
extern void allocate_paca_ptrs(void);
extern void allocate_paca(int cpu);
extern void free_unused_pacas(void);

#else /* CONFIG_PPC64 */

<<<<<<< HEAD
static inline void allocate_paca_ptrs(void) { };
static inline void allocate_paca(int cpu) { };
static inline void free_unused_pacas(void) { };
=======
static inline void allocate_paca_ptrs(void) { }
static inline void allocate_paca(int cpu) { }
static inline void free_unused_pacas(void) { }
>>>>>>> upstream/android-13

#endif /* CONFIG_PPC64 */

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_PACA_H */
