<<<<<<< HEAD
/*
 * Copyright (C) 2015 Linaro Ltd <ard.biesheuvel@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2015 Linaro Ltd <ard.biesheuvel@linaro.org>
>>>>>>> upstream/android-13
 */

#ifndef __ASM_ARM_EFI_H
#define __ASM_ARM_EFI_H

#include <asm/cacheflush.h>
#include <asm/cachetype.h>
#include <asm/early_ioremap.h>
#include <asm/fixmap.h>
#include <asm/highmem.h>
#include <asm/mach/map.h>
#include <asm/mmu_context.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> upstream/android-13
#include <asm/ptrace.h>

#ifdef CONFIG_EFI
void efi_init(void);

int efi_create_mapping(struct mm_struct *mm, efi_memory_desc_t *md);
int efi_set_mapping_permissions(struct mm_struct *mm, efi_memory_desc_t *md);

#define arch_efi_call_virt_setup()	efi_virtmap_load()
#define arch_efi_call_virt_teardown()	efi_virtmap_unload()

#define arch_efi_call_virt(p, f, args...)				\
({									\
	efi_##f##_t *__f;						\
	__f = p->f;							\
	__f(args);							\
})

#define ARCH_EFI_IRQ_FLAGS_MASK \
	(PSR_J_BIT | PSR_E_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT | \
	 PSR_T_BIT | MODE_MASK)

static inline void efi_set_pgd(struct mm_struct *mm)
{
	check_and_switch_context(mm, NULL);
}

void efi_virtmap_load(void);
void efi_virtmap_unload(void);

#else
#define efi_init()
#endif /* CONFIG_EFI */

/* arch specific definitions used by the stub code */

<<<<<<< HEAD
#define efi_call_early(f, ...)		sys_table_arg->boottime->f(__VA_ARGS__)
#define __efi_call_early(f, ...)	f(__VA_ARGS__)
#define efi_call_runtime(f, ...)	sys_table_arg->runtime->f(__VA_ARGS__)
#define efi_is_64bit()			(false)

#define efi_table_attr(table, attr, instance)				\
	((table##_t *)instance)->attr

#define efi_call_proto(protocol, f, instance, ...)			\
	((protocol##_t *)instance)->f(instance, ##__VA_ARGS__)

struct screen_info *alloc_screen_info(efi_system_table_t *sys_table_arg);
void free_screen_info(efi_system_table_t *sys_table, struct screen_info *si);

static inline void efifb_setup_from_dmi(struct screen_info *si, const char *opt)
{
}
=======
struct screen_info *alloc_screen_info(void);
void free_screen_info(struct screen_info *si);
>>>>>>> upstream/android-13

/*
 * A reasonable upper bound for the uncompressed kernel size is 32 MBytes,
 * so we will reserve that amount of memory. We have no easy way to tell what
 * the actuall size of code + data the uncompressed kernel will use.
 * If this is insufficient, the decompressor will relocate itself out of the
 * way before performing the decompression.
 */
#define MAX_UNCOMP_KERNEL_SIZE	SZ_32M

/*
<<<<<<< HEAD
 * The kernel zImage should preferably be located between 32 MB and 128 MB
 * from the base of DRAM. The min address leaves space for a maximal size
 * uncompressed image, and the max address is due to how the zImage decompressor
 * picks a destination address.
 */
#define ZIMAGE_OFFSET_LIMIT	SZ_128M
#define MIN_ZIMAGE_OFFSET	MAX_UNCOMP_KERNEL_SIZE

/* on ARM, the FDT should be located in the first 128 MB of RAM */
static inline unsigned long efi_get_max_fdt_addr(unsigned long dram_base)
{
	return dram_base + ZIMAGE_OFFSET_LIMIT;
}

/* on ARM, the initrd should be loaded in a lowmem region */
static inline unsigned long efi_get_max_initrd_addr(unsigned long dram_base,
						    unsigned long image_addr)
{
	return dram_base + SZ_512M;
=======
 * phys-to-virt patching requires that the physical to virtual offset is a
 * multiple of 2 MiB. However, using an alignment smaller than TEXT_OFFSET
 * here throws off the memory allocation logic, so let's use the lowest power
 * of two greater than 2 MiB and greater than TEXT_OFFSET.
 */
#define EFI_PHYS_ALIGN		max(UL(SZ_2M), roundup_pow_of_two(TEXT_OFFSET))

/* on ARM, the initrd should be loaded in a lowmem region */
static inline unsigned long efi_get_max_initrd_addr(unsigned long image_addr)
{
	return round_down(image_addr, SZ_4M) + SZ_512M;
}

struct efi_arm_entry_state {
	u32	cpsr_before_ebs;
	u32	sctlr_before_ebs;
	u32	cpsr_after_ebs;
	u32	sctlr_after_ebs;
};

static inline void efi_capsule_flush_cache_range(void *addr, int size)
{
	__cpuc_flush_dcache_area(addr, size);
>>>>>>> upstream/android-13
}

#endif /* _ASM_ARM_EFI_H */
