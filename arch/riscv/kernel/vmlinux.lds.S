<<<<<<< HEAD
/*
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */

#define LOAD_OFFSET PAGE_OFFSET
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
 */

#define RO_EXCEPTION_TABLE_ALIGN	16

#ifdef CONFIG_XIP_KERNEL
#include "vmlinux-xip.lds.S"
#else

#include <asm/pgtable.h>
#define LOAD_OFFSET KERNEL_LINK_ADDR

>>>>>>> upstream/android-13
#include <asm/vmlinux.lds.h>
#include <asm/page.h>
#include <asm/cache.h>
#include <asm/thread_info.h>
<<<<<<< HEAD

=======
#include <asm/set_memory.h>
#include "image-vars.h"

#include <linux/sizes.h>
>>>>>>> upstream/android-13
OUTPUT_ARCH(riscv)
ENTRY(_start)

jiffies = jiffies_64;

<<<<<<< HEAD
=======
PECOFF_SECTION_ALIGNMENT = 0x1000;
PECOFF_FILE_ALIGNMENT = 0x200;

>>>>>>> upstream/android-13
SECTIONS
{
	/* Beginning of code and text segment */
	. = LOAD_OFFSET;
	_start = .;
<<<<<<< HEAD
	__init_begin = .;
	HEAD_TEXT_SECTION
	INIT_TEXT_SECTION(PAGE_SIZE)
	INIT_DATA_SECTION(16)
	/* we have to discard exit text and such at runtime, not link time */
	.exit.text :
	{
		EXIT_TEXT
	}
	.exit.data :
	{
		EXIT_DATA
	}
	PERCPU_SECTION(L1_CACHE_BYTES)
	__init_end = .;
=======
	HEAD_TEXT_SECTION
	. = ALIGN(PAGE_SIZE);
>>>>>>> upstream/android-13

	.text : {
		_text = .;
		_stext = .;
		TEXT_TEXT
		SCHED_TEXT
		CPUIDLE_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		ENTRY_TEXT
		IRQENTRY_TEXT
<<<<<<< HEAD
=======
		SOFTIRQENTRY_TEXT
>>>>>>> upstream/android-13
		*(.fixup)
		_etext = .;
	}

<<<<<<< HEAD
	/* Start of data section */
	_sdata = .;
	RO_DATA_SECTION(L1_CACHE_BYTES)
	.srodata : {
		*(.srodata*)
	}

	RW_DATA_SECTION(L1_CACHE_BYTES, PAGE_SIZE, THREAD_SIZE)
	.sdata : {
		__global_pointer$ = . + 0x800;
		*(.sdata*)
		/* End of data section */
		_edata = .;
		*(.sbss*)
	}

	BSS_SECTION(0, 0, 0)

	EXCEPTION_TABLE(0x10)
	NOTES
=======
	. = ALIGN(SECTION_ALIGN);
	__init_begin = .;
	__init_text_begin = .;
	.init.text : AT(ADDR(.init.text) - LOAD_OFFSET) ALIGN(SECTION_ALIGN) { \
		_sinittext = .;						\
		INIT_TEXT						\
		_einittext = .;						\
	}

	. = ALIGN(8);
	__soc_early_init_table : {
		__soc_early_init_table_start = .;
		KEEP(*(__soc_early_init_table))
		__soc_early_init_table_end = .;
	}
	__soc_builtin_dtb_table : {
		__soc_builtin_dtb_table_start = .;
		KEEP(*(__soc_builtin_dtb_table))
		__soc_builtin_dtb_table_end = .;
	}
	/* we have to discard exit text and such at runtime, not link time */
	.exit.text :
	{
		EXIT_TEXT
	}

	__init_text_end = .;
	. = ALIGN(SECTION_ALIGN);
#ifdef CONFIG_EFI
	. = ALIGN(PECOFF_SECTION_ALIGNMENT);
	__pecoff_text_end = .;
#endif
	/* Start of init data section */
	__init_data_begin = .;
	INIT_DATA_SECTION(16)
	.exit.data :
	{
		EXIT_DATA
	}
	PERCPU_SECTION(L1_CACHE_BYTES)
>>>>>>> upstream/android-13

	.rel.dyn : {
		*(.rel.dyn*)
	}

<<<<<<< HEAD
=======
	__init_data_end = .;

	. = ALIGN(8);
	.alternative : {
		__alt_start = .;
		*(.alternative)
		__alt_end = .;
	}
	__init_end = .;

	/* Start of data section */
	_sdata = .;
	RO_DATA(SECTION_ALIGN)
	.srodata : {
		*(.srodata*)
	}

	. = ALIGN(SECTION_ALIGN);
	_data = .;

	RW_DATA(L1_CACHE_BYTES, PAGE_SIZE, THREAD_ALIGN)
	.sdata : {
		__global_pointer$ = . + 0x800;
		*(.sdata*)
	}

#ifdef CONFIG_EFI
	.pecoff_edata_padding : { BYTE(0); . = ALIGN(PECOFF_FILE_ALIGNMENT); }
	__pecoff_data_raw_size = ABSOLUTE(. - __pecoff_text_end);
#endif

	/* End of data section */
	_edata = .;

	BSS_SECTION(PAGE_SIZE, PAGE_SIZE, 0)

#ifdef CONFIG_EFI
	. = ALIGN(PECOFF_SECTION_ALIGNMENT);
	__pecoff_data_virt_size = ABSOLUTE(. - __pecoff_text_end);
#endif
>>>>>>> upstream/android-13
	_end = .;

	STABS_DEBUG
	DWARF_DEBUG
<<<<<<< HEAD

	DISCARDS
}
=======
	ELF_DETAILS

	DISCARDS
}
#endif /* CONFIG_XIP_KERNEL */
>>>>>>> upstream/android-13
