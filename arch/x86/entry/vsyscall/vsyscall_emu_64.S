<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * vsyscall_emu_64.S: Vsyscall emulation page
 *
 * Copyright (c) 2011 Andy Lutomirski
<<<<<<< HEAD
 *
 * Subject to the GNU General Public License, version 2
=======
>>>>>>> upstream/android-13
 */

#include <linux/linkage.h>

#include <asm/irq_vectors.h>
#include <asm/page_types.h>
#include <asm/unistd_64.h>

__PAGE_ALIGNED_DATA
	.globl __vsyscall_page
	.balign PAGE_SIZE, 0xcc
	.type __vsyscall_page, @object
__vsyscall_page:

	mov $__NR_gettimeofday, %rax
	syscall
<<<<<<< HEAD
	ret
=======
	RET
>>>>>>> upstream/android-13

	.balign 1024, 0xcc
	mov $__NR_time, %rax
	syscall
<<<<<<< HEAD
	ret
=======
	RET
>>>>>>> upstream/android-13

	.balign 1024, 0xcc
	mov $__NR_getcpu, %rax
	syscall
<<<<<<< HEAD
	ret
=======
	RET
>>>>>>> upstream/android-13

	.balign 4096, 0xcc

	.size __vsyscall_page, 4096
