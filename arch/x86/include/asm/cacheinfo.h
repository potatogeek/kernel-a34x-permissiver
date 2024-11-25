/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CACHEINFO_H
#define _ASM_X86_CACHEINFO_H

<<<<<<< HEAD
void cacheinfo_amd_init_llc_id(struct cpuinfo_x86 *c, int cpu, u8 node_id);
=======
void cacheinfo_amd_init_llc_id(struct cpuinfo_x86 *c, int cpu);
void cacheinfo_hygon_init_llc_id(struct cpuinfo_x86 *c, int cpu);
>>>>>>> upstream/android-13

#endif /* _ASM_X86_CACHEINFO_H */
