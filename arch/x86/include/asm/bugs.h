/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_BUGS_H
#define _ASM_X86_BUGS_H

#include <asm/processor.h>

extern void check_bugs(void);

<<<<<<< HEAD
#if defined(CONFIG_CPU_SUP_INTEL)
void check_mpx_erratum(struct cpuinfo_x86 *c);
#else
static inline void check_mpx_erratum(struct cpuinfo_x86 *c) {}
#endif

=======
>>>>>>> upstream/android-13
#if defined(CONFIG_CPU_SUP_INTEL) && defined(CONFIG_X86_32)
int ppro_with_ram_bug(void);
#else
static inline int ppro_with_ram_bug(void) { return 0; }
#endif

<<<<<<< HEAD
=======
extern void cpu_bugs_smt_update(void);

>>>>>>> upstream/android-13
#endif /* _ASM_X86_BUGS_H */
