/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright 2001-2002 Pavel Machek <pavel@suse.cz>
 * Based on code
 * Copyright 2001 Patrick Mochel <mochel@osdl.org>
 */
#ifndef _ASM_X86_SUSPEND_32_H
#define _ASM_X86_SUSPEND_32_H

#include <asm/desc.h>
#include <asm/fpu/api.h>

/* image of the saved processor state */
struct saved_context {
	/*
<<<<<<< HEAD
	 * On x86_32, all segment registers, with the possible exception of
	 * gs, are saved at kernel entry in pt_regs.
	 */
#ifdef CONFIG_X86_32_LAZY_GS
	u16 gs;
#endif
=======
	 * On x86_32, all segment registers except gs are saved at kernel
	 * entry in pt_regs.
	 */
	u16 gs;
>>>>>>> upstream/android-13
	unsigned long cr0, cr2, cr3, cr4;
	u64 misc_enable;
	bool misc_enable_saved;
	struct saved_msrs saved_msrs;
	struct desc_ptr gdt_desc;
	struct desc_ptr idt;
	u16 ldt;
	u16 tss;
	unsigned long tr;
	unsigned long safety;
	unsigned long return_address;
} __attribute__((packed));

<<<<<<< HEAD
=======
/* routines for saving/restoring kernel state */
extern char core_restore_code[];
extern char restore_registers[];

>>>>>>> upstream/android-13
#endif /* _ASM_X86_SUSPEND_32_H */
