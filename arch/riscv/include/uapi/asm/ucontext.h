<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note */
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2012 ARM Ltd.
 * Copyright (C) 2017 SiFive, Inc.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This file was copied from arch/arm64/include/uapi/asm/ucontext.h
 */
#ifndef _UAPI__ASM_UCONTEXT_H
#define _UAPI__ASM_UCONTEXT_H
=======
 * This file was copied from arch/arm64/include/uapi/asm/ucontext.h
 */
#ifndef _UAPI_ASM_RISCV_UCONTEXT_H
#define _UAPI_ASM_RISCV_UCONTEXT_H
>>>>>>> upstream/android-13

#include <linux/types.h>

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext	 *uc_link;
	stack_t		  uc_stack;
	sigset_t	  uc_sigmask;
	/* There's some padding here to allow sigset_t to be expanded in the
	 * future.  Though this is unlikely, other architectures put uc_sigmask
	 * at the end of this structure and explicitly state it can be
	 * expanded, so we didn't want to box ourselves in here. */
	__u8		  __unused[1024 / 8 - sizeof(sigset_t)];
	/* We can't put uc_sigmask at the end of this structure because we need
	 * to be able to expand sigcontext in the future.  For example, the
	 * vector ISA extension will almost certainly add ISA state.  We want
	 * to ensure all user-visible ISA state can be saved and restored via a
	 * ucontext, so we're putting this at the end in order to allow for
	 * infinite extensibility.  Since we know this will be extended and we
	 * assume sigset_t won't be extended an extreme amount, we're
	 * prioritizing this. */
	struct sigcontext uc_mcontext;
};

<<<<<<< HEAD
#endif /* _UAPI__ASM_UCONTEXT_H */
=======
#endif /* _UAPI_ASM_RISCV_UCONTEXT_H */
>>>>>>> upstream/android-13
