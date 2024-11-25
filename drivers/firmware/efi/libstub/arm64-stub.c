<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Copyright (C) 2013, 2014 Linaro Ltd;  <roy.franz@linaro.org>
 *
 * This file implements the EFI boot stub for the arm64 kernel.
 * Adapted from ARM version by Mark Salter <msalter@redhat.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

/*
 * To prevent the compiler from emitting GOT-indirected (and thus absolute)
 * references to the section markers, override their visibility as 'hidden'
 */
#pragma GCC visibility push(hidden)
#include <asm/sections.h>
#pragma GCC visibility pop
=======
 */

>>>>>>> upstream/android-13

#include <linux/efi.h>
#include <asm/efi.h>
#include <asm/memory.h>
<<<<<<< HEAD
=======
#include <asm/sections.h>
>>>>>>> upstream/android-13
#include <asm/sysreg.h>

#include "efistub.h"

<<<<<<< HEAD
efi_status_t check_platform_features(efi_system_table_t *sys_table_arg)
=======
efi_status_t check_platform_features(void)
>>>>>>> upstream/android-13
{
	u64 tg;

	/* UEFI mandates support for 4 KB granularity, no need to check */
	if (IS_ENABLED(CONFIG_ARM64_4K_PAGES))
		return EFI_SUCCESS;

	tg = (read_cpuid(ID_AA64MMFR0_EL1) >> ID_AA64MMFR0_TGRAN_SHIFT) & 0xf;
<<<<<<< HEAD
	if (tg != ID_AA64MMFR0_TGRAN_SUPPORTED) {
		if (IS_ENABLED(CONFIG_ARM64_64K_PAGES))
			pr_efi_err(sys_table_arg, "This 64 KB granular kernel is not supported by your CPU\n");
		else
			pr_efi_err(sys_table_arg, "This 16 KB granular kernel is not supported by your CPU\n");
=======
	if (tg < ID_AA64MMFR0_TGRAN_SUPPORTED_MIN || tg > ID_AA64MMFR0_TGRAN_SUPPORTED_MAX) {
		if (IS_ENABLED(CONFIG_ARM64_64K_PAGES))
			efi_err("This 64 KB granular kernel is not supported by your CPU\n");
		else
			efi_err("This 16 KB granular kernel is not supported by your CPU\n");
>>>>>>> upstream/android-13
		return EFI_UNSUPPORTED;
	}
	return EFI_SUCCESS;
}

<<<<<<< HEAD
efi_status_t handle_kernel_image(efi_system_table_t *sys_table_arg,
				 unsigned long *image_addr,
				 unsigned long *image_size,
				 unsigned long *reserve_addr,
				 unsigned long *reserve_size,
				 unsigned long dram_base,
=======
/*
 * Distro versions of GRUB may ignore the BSS allocation entirely (i.e., fail
 * to provide space, and fail to zero it). Check for this condition by double
 * checking that the first and the last byte of the image are covered by the
 * same EFI memory map entry.
 */
static bool check_image_region(u64 base, u64 size)
{
	unsigned long map_size, desc_size, buff_size;
	efi_memory_desc_t *memory_map;
	struct efi_boot_memmap map;
	efi_status_t status;
	bool ret = false;
	int map_offset;

	map.map =	&memory_map;
	map.map_size =	&map_size;
	map.desc_size =	&desc_size;
	map.desc_ver =	NULL;
	map.key_ptr =	NULL;
	map.buff_size =	&buff_size;

	status = efi_get_memory_map(&map);
	if (status != EFI_SUCCESS)
		return false;

	for (map_offset = 0; map_offset < map_size; map_offset += desc_size) {
		efi_memory_desc_t *md = (void *)memory_map + map_offset;
		u64 end = md->phys_addr + md->num_pages * EFI_PAGE_SIZE;

		/*
		 * Find the region that covers base, and return whether
		 * it covers base+size bytes.
		 */
		if (base >= md->phys_addr && base < end) {
			ret = (base + size) <= end;
			break;
		}
	}

	efi_bs_call(free_pool, memory_map);

	return ret;
}

efi_status_t handle_kernel_image(unsigned long *image_addr,
				 unsigned long *image_size,
				 unsigned long *reserve_addr,
				 unsigned long *reserve_size,
>>>>>>> upstream/android-13
				 efi_loaded_image_t *image)
{
	efi_status_t status;
	unsigned long kernel_size, kernel_memsize = 0;
<<<<<<< HEAD
	void *old_image_addr = (void *)*image_addr;
	unsigned long preferred_offset;
	u64 phys_seed = 0;

	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE)) {
		if (!nokaslr()) {
			status = efi_get_random_bytes(sys_table_arg,
						      sizeof(phys_seed),
						      (u8 *)&phys_seed);
			if (status == EFI_NOT_FOUND) {
				pr_efi(sys_table_arg, "EFI_RNG_PROTOCOL unavailable, no randomness supplied\n");
			} else if (status != EFI_SUCCESS) {
				pr_efi_err(sys_table_arg, "efi_get_random_bytes() failed\n");
				return status;
			}
		} else {
			pr_efi(sys_table_arg, "KASLR disabled on kernel command line\n");
		}
	}

	/*
	 * The preferred offset of the kernel Image is TEXT_OFFSET bytes beyond
	 * a 2 MB aligned base, which itself may be lower than dram_base, as
	 * long as the resulting offset equals or exceeds it.
	 */
	preferred_offset = round_down(dram_base, MIN_KIMG_ALIGN) + TEXT_OFFSET;
	if (preferred_offset < dram_base)
		preferred_offset += MIN_KIMG_ALIGN;

	kernel_size = _edata - _text;
	kernel_memsize = kernel_size + (_end - _edata);

	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE) && phys_seed != 0) {
		/*
		 * If CONFIG_DEBUG_ALIGN_RODATA is not set, produce a
		 * displacement in the interval [0, MIN_KIMG_ALIGN) that
		 * doesn't violate this kernel's de-facto alignment
		 * constraints.
		 */
		u32 mask = (MIN_KIMG_ALIGN - 1) & ~(EFI_KIMG_ALIGN - 1);
		u32 offset = !IS_ENABLED(CONFIG_DEBUG_ALIGN_RODATA) ?
			     (phys_seed >> 32) & mask : TEXT_OFFSET;

		/*
		 * With CONFIG_RANDOMIZE_TEXT_OFFSET=y, TEXT_OFFSET may not
		 * be a multiple of EFI_KIMG_ALIGN, and we must ensure that
		 * we preserve the misalignment of 'offset' relative to
		 * EFI_KIMG_ALIGN so that statically allocated objects whose
		 * alignment exceeds PAGE_SIZE appear correctly aligned in
		 * memory.
		 */
		offset |= TEXT_OFFSET % EFI_KIMG_ALIGN;

		/*
		 * If KASLR is enabled, and we have some randomness available,
		 * locate the kernel at a randomized offset in physical memory.
		 */
		*reserve_size = kernel_memsize + offset;
		status = efi_random_alloc(sys_table_arg, *reserve_size,
					  MIN_KIMG_ALIGN, reserve_addr,
					  (u32)phys_seed);

		*image_addr = *reserve_addr + offset;
	} else {
		/*
		 * Else, try a straight allocation at the preferred offset.
		 * This will work around the issue where, if dram_base == 0x0,
		 * efi_low_alloc() refuses to allocate at 0x0 (to prevent the
		 * address of the allocation to be mistaken for a FAIL return
		 * value or a NULL pointer). It will also ensure that, on
		 * platforms where the [dram_base, dram_base + TEXT_OFFSET)
		 * interval is partially occupied by the firmware (like on APM
		 * Mustang), we can still place the kernel at the address
		 * 'dram_base + TEXT_OFFSET'.
		 */
		if (*image_addr == preferred_offset)
			return EFI_SUCCESS;

		*image_addr = *reserve_addr = preferred_offset;
		*reserve_size = round_up(kernel_memsize, EFI_ALLOC_ALIGN);

		status = efi_call_early(allocate_pages, EFI_ALLOCATE_ADDRESS,
					EFI_LOADER_DATA,
					*reserve_size / EFI_PAGE_SIZE,
					(efi_physical_addr_t *)reserve_addr);
	}

	if (status != EFI_SUCCESS) {
		*reserve_size = kernel_memsize + TEXT_OFFSET;
		status = efi_low_alloc(sys_table_arg, *reserve_size,
				       MIN_KIMG_ALIGN, reserve_addr);

		if (status != EFI_SUCCESS) {
			pr_efi_err(sys_table_arg, "Failed to relocate kernel\n");
			*reserve_size = 0;
			return status;
		}
		*image_addr = *reserve_addr + TEXT_OFFSET;
	}
	memcpy((void *)*image_addr, old_image_addr, kernel_size);
=======
	u32 phys_seed = 0;

	/*
	 * Although relocatable kernels can fix up the misalignment with
	 * respect to MIN_KIMG_ALIGN, the resulting virtual text addresses are
	 * subtly out of sync with those recorded in the vmlinux when kaslr is
	 * disabled but the image required relocation anyway. Therefore retain
	 * 2M alignment if KASLR was explicitly disabled, even if it was not
	 * going to be activated to begin with.
	 */
	u64 min_kimg_align = efi_nokaslr ? MIN_KIMG_ALIGN : EFI_KIMG_ALIGN;

	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE)) {
		if (!efi_nokaslr) {
			status = efi_get_random_bytes(sizeof(phys_seed),
						      (u8 *)&phys_seed);
			if (status == EFI_NOT_FOUND) {
				efi_info("EFI_RNG_PROTOCOL unavailable\n");
				efi_nokaslr = true;
			} else if (status != EFI_SUCCESS) {
				efi_err("efi_get_random_bytes() failed (0x%lx)\n",
					status);
				efi_nokaslr = true;
			}
		} else {
			efi_info("KASLR disabled on kernel command line\n");
		}
	}

	if (image->image_base != _text)
		efi_err("FIRMWARE BUG: efi_loaded_image_t::image_base has bogus value\n");

	if (!IS_ALIGNED((u64)_text, SEGMENT_ALIGN))
		efi_err("FIRMWARE BUG: kernel image not aligned on %dk boundary\n",
			SEGMENT_ALIGN >> 10);

	kernel_size = _edata - _text;
	kernel_memsize = kernel_size + (_end - _edata);
	*reserve_size = kernel_memsize;

	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE) && phys_seed != 0) {
		/*
		 * If KASLR is enabled, and we have some randomness available,
		 * locate the kernel at a randomized offset in physical memory.
		 */
		status = efi_random_alloc(*reserve_size, min_kimg_align,
					  reserve_addr, phys_seed);
		if (status != EFI_SUCCESS)
			efi_warn("efi_random_alloc() failed: 0x%lx\n", status);
	} else {
		status = EFI_OUT_OF_RESOURCES;
	}

	if (status != EFI_SUCCESS) {
		if (!check_image_region((u64)_text, kernel_memsize)) {
			efi_err("FIRMWARE BUG: Image BSS overlaps adjacent EFI memory region\n");
		} else if (IS_ALIGNED((u64)_text, min_kimg_align)) {
			/*
			 * Just execute from wherever we were loaded by the
			 * UEFI PE/COFF loader if the alignment is suitable.
			 */
			*image_addr = (u64)_text;
			*reserve_size = 0;
			return EFI_SUCCESS;
		}

		status = efi_allocate_pages_aligned(*reserve_size, reserve_addr,
						    ULONG_MAX, min_kimg_align);

		if (status != EFI_SUCCESS) {
			efi_err("Failed to relocate kernel\n");
			*reserve_size = 0;
			return status;
		}
	}

	*image_addr = *reserve_addr;
	memcpy((void *)*image_addr, _text, kernel_size);
>>>>>>> upstream/android-13

	return EFI_SUCCESS;
}
