/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef TOOLS_ARCH_POWERPC_UAPI_ASM_MMAN_FIX_H
#define TOOLS_ARCH_POWERPC_UAPI_ASM_MMAN_FIX_H
#define MAP_DENYWRITE	0x0800
#define MAP_EXECUTABLE	0x1000
#define MAP_GROWSDOWN	0x0100
<<<<<<< HEAD
#define MAP_HUGETLB	0x40000
#define MAP_LOCKED	0x80
#define MAP_NONBLOCK	0x10000
#define MAP_NORESERVE   0x40
#define MAP_POPULATE	0x8000
#define MAP_STACK	0x20000
=======
#define MAP_LOCKED	0x80
#define MAP_NORESERVE   0x40
>>>>>>> upstream/android-13
#include <uapi/asm-generic/mman-common.h>
/* MAP_32BIT is undefined on powerpc, fix it for perf */
#define MAP_32BIT	0
#endif
