#ifndef _ASM_X86_UMIP_H
#define _ASM_X86_UMIP_H

#include <linux/types.h>
#include <asm/ptrace.h>

<<<<<<< HEAD
#ifdef CONFIG_X86_INTEL_UMIP
bool fixup_umip_exception(struct pt_regs *regs);
#else
static inline bool fixup_umip_exception(struct pt_regs *regs) { return false; }
#endif  /* CONFIG_X86_INTEL_UMIP */
=======
#ifdef CONFIG_X86_UMIP
bool fixup_umip_exception(struct pt_regs *regs);
#else
static inline bool fixup_umip_exception(struct pt_regs *regs) { return false; }
#endif  /* CONFIG_X86_UMIP */
>>>>>>> upstream/android-13
#endif  /* _ASM_X86_UMIP_H */
