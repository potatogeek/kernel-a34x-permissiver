#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__


#define TSK_ACTIVE_MM 1288 

#define TSK_TI_FLAGS 0 
#define TSK_TI_PREEMPT 24 
#define TSK_TI_ADDR_LIMIT 8 
#define TSK_TI_TTBR0 16 
#define TSK_STACK 40 

#define THREAD_CPU_CONTEXT 2736 

#define S_X0 0 
#define S_X1 8 
#define S_X2 16 
#define S_X3 24 
#define S_X4 32 
#define S_X5 40 
#define S_X6 48 
#define S_X7 56 
#define S_X8 64 
#define S_X10 80 
#define S_X12 96 
#define S_X14 112 
#define S_X16 128 
#define S_X18 144 
#define S_X20 160 
#define S_X22 176 
#define S_X24 192 
#define S_X26 208 
#define S_X28 224 
#define S_LR 240 
#define S_SP 248 
#define S_COMPAT_SP 104 
#define S_PSTATE 264 
#define S_PC 256 
#define S_ORIG_X0 272 
#define S_SYSCALLNO 280 
#define S_ORIG_ADDR_LIMIT 288 
#define S_STACKFRAME 304 
#define S_FRAME_SIZE 320 

#define COMPAT_SIGFRAME_REGS_OFFSET 32 
#define COMPAT_RT_SIGFRAME_REGS_OFFSET 160 

#define MM_CONTEXT_ID 760 

#define VMA_VM_MM 64 
#define VMA_VM_FLAGS 80 

#define VM_EXEC 4 

#define PAGE_SZ 4096 

#define DMA_BIDIRECTIONAL 0 
#define DMA_TO_DEVICE 1 
#define DMA_FROM_DEVICE 2 

#define PREEMPT_DISABLE_OFFSET 1 

#define CLOCK_REALTIME 0 
#define CLOCK_MONOTONIC 1 
#define CLOCK_MONOTONIC_RAW 4 
#define CLOCK_REALTIME_RES 232 
#define CLOCK_REALTIME_COARSE 5 
#define CLOCK_MONOTONIC_COARSE 6 
#define CLOCK_COARSE_RES 4000000 
#define NSEC_PER_SEC 1000000000 

#define VDSO_SEQ 0 
#define VDSO_CLK_MODE 4 
#define VDSO_CYCLE_LAST 8 
#define VDSO_MASK 16 
#define VDSO_MULT 24 
#define VDSO_SHIFT 28 
#define VDSO_REALTIME_SEC 32 
#define VDSO_REALTIME_NSEC 40 
#define VDSO_MONO_SEC 48 
#define VDSO_MONO_NSEC 56 
#define VDSO_MONO_RAW_SEC 96 
#define VDSO_MONO_RAW_NSEC 104 
#define VDSO_BOOTTIME_SEC 144 
#define VDSO_BOOTTIME_NSEC 152 
#define VDSO_TAI_SEC 208 
#define VDSO_TAI_NSEC 216 
#define VDSO_RT_COARSE_SEC 112 
#define VDSO_RT_COARSE_NSEC 120 
#define VDSO_MONO_COARSE_SEC 128 
#define VDSO_MONO_COARSE_NSEC 136 
#define VDSO_TZ_MINWEST 224 
#define VDSO_TZ_DSTTIME 228 

#define TVAL_TV_SEC 0 
#define TVAL_TV_USEC 8 
#define TSPEC_TV_SEC 0 
#define TSPEC_TV_NSEC 8 

#define TZ_MINWEST 0 
#define TZ_DSTTIME 4 

#define CPU_BOOT_STACK 0 
#define CPU_BOOT_TASK 8 

#define CPU_SUSPEND_SZ 112 
#define CPU_CTX_SP 104 
#define MPIDR_HASH_MASK 0 
#define MPIDR_HASH_SHIFTS 8 
#define SLEEP_STACK_DATA_SYSTEM_REGS 0 
#define SLEEP_STACK_DATA_CALLEE_REGS 112 
#define ARM_SMCCC_RES_X0_OFFS 0 
#define ARM_SMCCC_RES_X2_OFFS 16 
#define ARM_SMCCC_QUIRK_ID_OFFS 0 
#define ARM_SMCCC_QUIRK_STATE_OFFS 8 

#define HIBERN_PBE_ORIG 8 
#define HIBERN_PBE_ADDR 0 
#define HIBERN_PBE_NEXT 16 
#define ARM64_FTR_SYSVAL 24 


#endif
