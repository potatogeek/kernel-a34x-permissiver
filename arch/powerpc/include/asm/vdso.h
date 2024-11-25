/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
#ifndef __PPC64_VDSO_H__
#define __PPC64_VDSO_H__

#ifdef __KERNEL__

/* Default link addresses for the vDSOs */
#define VDSO32_LBASE	0x0
#define VDSO64_LBASE	0x0
=======
#ifndef _ASM_POWERPC_VDSO_H
#define _ASM_POWERPC_VDSO_H
>>>>>>> upstream/android-13

/* Default map addresses for 32bit vDSO */
#define VDSO32_MBASE	0x100000

#define VDSO_VERSION_STRING	LINUX_2.6.15

<<<<<<< HEAD
/* Define if 64 bits VDSO has procedure descriptors */
#undef VDS64_HAS_DESCRIPTORS

#ifndef __ASSEMBLY__

/* Offsets relative to thread->vdso_base */
extern unsigned long vdso64_rt_sigtramp;
extern unsigned long vdso32_sigtramp;
extern unsigned long vdso32_rt_sigtramp;
=======
#ifndef __ASSEMBLY__

#ifdef CONFIG_PPC64
#include <generated/vdso64-offsets.h>
#endif

#ifdef CONFIG_VDSO32
#include <generated/vdso32-offsets.h>
#endif

#define VDSO64_SYMBOL(base, name) ((unsigned long)(base) + (vdso64_offset_##name))

#define VDSO32_SYMBOL(base, name) ((unsigned long)(base) + (vdso32_offset_##name))
>>>>>>> upstream/android-13

int vdso_getcpu_init(void);

#else /* __ASSEMBLY__ */

#ifdef __VDSO64__
<<<<<<< HEAD
#ifdef VDS64_HAS_DESCRIPTORS
#define V_FUNCTION_BEGIN(name)		\
	.globl name;			\
        .section ".opd","a";		\
        .align 3;			\
	name:				\
	.quad .name,.TOC.@tocbase,0;	\
	.previous;			\
	.globl .name;			\
	.type .name,@function; 		\
	.name:				\

#define V_FUNCTION_END(name)		\
	.size .name,.-.name;

#define V_LOCAL_FUNC(name) (.name)

#else /* VDS64_HAS_DESCRIPTORS */

=======
>>>>>>> upstream/android-13
#define V_FUNCTION_BEGIN(name)		\
	.globl name;			\
	name:				\

#define V_FUNCTION_END(name)		\
	.size name,.-name;

#define V_LOCAL_FUNC(name) (name)
<<<<<<< HEAD

#endif /* VDS64_HAS_DESCRIPTORS */
=======
>>>>>>> upstream/android-13
#endif /* __VDSO64__ */

#ifdef __VDSO32__

#define V_FUNCTION_BEGIN(name)		\
	.globl name;			\
	.type name,@function; 		\
	name:				\

#define V_FUNCTION_END(name)		\
	.size name,.-name;

#define V_LOCAL_FUNC(name) (name)

#endif /* __VDSO32__ */

#endif /* __ASSEMBLY__ */

<<<<<<< HEAD
#endif /* __KERNEL__ */

#endif /* __PPC64_VDSO_H__ */
=======
#endif /* _ASM_POWERPC_VDSO_H */
>>>>>>> upstream/android-13
