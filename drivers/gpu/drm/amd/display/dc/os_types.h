/*
 * Copyright 2012-16 Advanced Micro Devices, Inc.
<<<<<<< HEAD
=======
 * Copyright 2019 Raptor Engineering, LLC
>>>>>>> upstream/android-13
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef _OS_TYPES_H_
#define _OS_TYPES_H_

<<<<<<< HEAD
#include <asm/byteorder.h>
#include <linux/types.h>
#include <drm/drmP.h>

#include <linux/kref.h>
=======
#include <linux/kgdb.h>
#include <linux/kref.h>
#include <linux/types.h>
#include <linux/slab.h>

#include <asm/byteorder.h>

#include <drm/drm_print.h>
>>>>>>> upstream/android-13

#include "cgs_common.h"

#if defined(__BIG_ENDIAN) && !defined(BIGENDIAN_CPU)
#define BIGENDIAN_CPU
#elif defined(__LITTLE_ENDIAN) && !defined(LITTLEENDIAN_CPU)
#define LITTLEENDIAN_CPU
#endif

<<<<<<< HEAD
#undef READ
#undef WRITE
=======
>>>>>>> upstream/android-13
#undef FRAME_SIZE

#define dm_output_to_console(fmt, ...) DRM_DEBUG_KMS(fmt, ##__VA_ARGS__)

#define dm_error(fmt, ...) DRM_ERROR(fmt, ##__VA_ARGS__)

<<<<<<< HEAD
#if defined(CONFIG_DRM_AMD_DC_DCN1_0)
#include <asm/fpu/api.h>
=======
#if defined(CONFIG_DRM_AMD_DC_DCN)
#include "amdgpu_dm/dc_fpu.h"
#define DC_FP_START() dc_fpu_begin(__func__, __LINE__)
#define DC_FP_END() dc_fpu_end(__func__, __LINE__)
>>>>>>> upstream/android-13
#endif

/*
 *
 * general debug capabilities
 *
 */
<<<<<<< HEAD
#if defined(CONFIG_DEBUG_KERNEL_DC) && (defined(CONFIG_HAVE_KGDB) || defined(CONFIG_KGDB))
#define ASSERT_CRITICAL(expr) do {	\
	if (WARN_ON(!(expr))) { \
		kgdb_breakpoint(); \
	} \
} while (0)
#else
#define ASSERT_CRITICAL(expr) do {	\
	if (WARN_ON(!(expr))) { \
		; \
	} \
} while (0)
#endif

#if defined(CONFIG_DEBUG_KERNEL_DC)
#define ASSERT(expr) ASSERT_CRITICAL(expr)

#else
#define ASSERT(expr) WARN_ON(!(expr))
#endif

#define BREAK_TO_DEBUGGER() ASSERT(0)
=======
#ifdef CONFIG_DEBUG_KERNEL_DC
#define dc_breakpoint()		kgdb_breakpoint()
#else
#define dc_breakpoint()		do {} while (0)
#endif

#define ASSERT_CRITICAL(expr) do {		\
		if (WARN_ON(!(expr)))		\
			dc_breakpoint();	\
	} while (0)

#define ASSERT(expr) do {			\
		if (WARN_ON_ONCE(!(expr)))	\
			dc_breakpoint();	\
	} while (0)

#define BREAK_TO_DEBUGGER() \
	do { \
		DRM_DEBUG_DRIVER("%s():%d\n", __func__, __LINE__); \
		dc_breakpoint(); \
	} while (0)
>>>>>>> upstream/android-13

#define DC_ERR(...)  do { \
	dm_error(__VA_ARGS__); \
	BREAK_TO_DEBUGGER(); \
} while (0)

#endif /* _OS_TYPES_H_ */
