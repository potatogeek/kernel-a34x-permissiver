<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/* ----------------------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007 rPath, Inc. - All Rights Reserved
 *
<<<<<<< HEAD
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
=======
>>>>>>> upstream/android-13
 * ----------------------------------------------------------------------- */

#include <linux/linkage.h>

/*
 * Memory copy routines
 */

	.code16
	.text

<<<<<<< HEAD
GLOBAL(memcpy)
=======
SYM_FUNC_START_NOALIGN(memcpy)
>>>>>>> upstream/android-13
	pushw	%si
	pushw	%di
	movw	%ax, %di
	movw	%dx, %si
	pushw	%cx
	shrw	$2, %cx
	rep; movsl
	popw	%cx
	andw	$3, %cx
	rep; movsb
	popw	%di
	popw	%si
	retl
<<<<<<< HEAD
ENDPROC(memcpy)

GLOBAL(memset)
=======
SYM_FUNC_END(memcpy)

SYM_FUNC_START_NOALIGN(memset)
>>>>>>> upstream/android-13
	pushw	%di
	movw	%ax, %di
	movzbl	%dl, %eax
	imull	$0x01010101,%eax
	pushw	%cx
	shrw	$2, %cx
	rep; stosl
	popw	%cx
	andw	$3, %cx
	rep; stosb
	popw	%di
	retl
<<<<<<< HEAD
ENDPROC(memset)

GLOBAL(copy_from_fs)
=======
SYM_FUNC_END(memset)

SYM_FUNC_START_NOALIGN(copy_from_fs)
>>>>>>> upstream/android-13
	pushw	%ds
	pushw	%fs
	popw	%ds
	calll	memcpy
	popw	%ds
	retl
<<<<<<< HEAD
ENDPROC(copy_from_fs)

GLOBAL(copy_to_fs)
=======
SYM_FUNC_END(copy_from_fs)

SYM_FUNC_START_NOALIGN(copy_to_fs)
>>>>>>> upstream/android-13
	pushw	%es
	pushw	%fs
	popw	%es
	calll	memcpy
	popw	%es
	retl
<<<<<<< HEAD
ENDPROC(copy_to_fs)
=======
SYM_FUNC_END(copy_to_fs)
>>>>>>> upstream/android-13
