// SPDX-License-Identifier: GPL-2.0
#define BUILD_VDSO32

<<<<<<< HEAD
#ifndef CONFIG_CC_OPTIMIZE_FOR_SIZE
#undef CONFIG_OPTIMIZE_INLINING
#endif

=======
>>>>>>> upstream/android-13
#ifdef CONFIG_X86_64

/*
 * in case of a 32 bit VDSO for a 64 bit kernel fake a 32 bit kernel
 * configuration
 */
#undef CONFIG_64BIT
#undef CONFIG_X86_64
<<<<<<< HEAD
=======
#undef CONFIG_COMPAT
>>>>>>> upstream/android-13
#undef CONFIG_PGTABLE_LEVELS
#undef CONFIG_ILLEGAL_POINTER_VALUE
#undef CONFIG_SPARSEMEM_VMEMMAP
#undef CONFIG_NR_CPUS
<<<<<<< HEAD
=======
#undef CONFIG_PARAVIRT_XXL
>>>>>>> upstream/android-13

#define CONFIG_X86_32 1
#define CONFIG_PGTABLE_LEVELS 2
#define CONFIG_PAGE_OFFSET 0
#define CONFIG_ILLEGAL_POINTER_VALUE 0
#define CONFIG_NR_CPUS 1

#define BUILD_VDSO32_64

#endif

#include "../vclock_gettime.c"
