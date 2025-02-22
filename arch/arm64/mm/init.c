<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Based on arch/arm/mm/init.c
 *
 * Copyright (C) 1995-2005 Russell King
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
 *
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
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/errno.h>
#include <linux/swap.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
>>>>>>> upstream/android-13
#include <linux/cache.h>
#include <linux/mman.h>
#include <linux/nodemask.h>
#include <linux/initrd.h>
#include <linux/gfp.h>
#include <linux/memblock.h>
#include <linux/sort.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
<<<<<<< HEAD
#include <linux/dma-mapping.h>
#include <linux/dma-contiguous.h>
=======
#include <linux/dma-direct.h>
#include <linux/dma-map-ops.h>
>>>>>>> upstream/android-13
#include <linux/efi.h>
#include <linux/swiotlb.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/kexec.h>
#include <linux/crash_dump.h>
<<<<<<< HEAD
=======
#include <linux/hugetlb.h>
#include <linux/acpi_iort.h>
#include <linux/kmemleak.h>
>>>>>>> upstream/android-13

#include <asm/boot.h>
#include <asm/fixmap.h>
#include <asm/kasan.h>
#include <asm/kernel-pgtable.h>
<<<<<<< HEAD
=======
#include <asm/kvm_host.h>
>>>>>>> upstream/android-13
#include <asm/memory.h>
#include <asm/numa.h>
#include <asm/sections.h>
#include <asm/setup.h>
<<<<<<< HEAD
#include <asm/sizes.h>
#include <asm/tlb.h>
#include <asm/alternative.h>

EXPORT_SYMBOL_GPL(kimage_vaddr);
=======
#include <linux/sizes.h>
#include <asm/tlb.h>
#include <asm/alternative.h>
#include <asm/xen/swiotlb-xen.h>
>>>>>>> upstream/android-13

/*
 * We need to be able to catch inadvertent references to memstart_addr
 * that occur (potentially in generic code) before arm64_memblock_init()
 * executes, which assigns it its actual value. So use a default value
 * that cannot be mistaken for a real physical address.
 */
s64 memstart_addr __ro_after_init = -1;
<<<<<<< HEAD
phys_addr_t arm64_dma_phys_limit __ro_after_init;

#ifdef CONFIG_BLK_DEV_INITRD
static int __init early_initrd(char *p)
{
	unsigned long start, size;
	char *endp;

	start = memparse(p, &endp);
	if (*endp == ',') {
		size = memparse(endp + 1, NULL);

		initrd_start = start;
		initrd_end = start + size;
	}
	return 0;
}
early_param("initrd", early_initrd);
#endif

=======
EXPORT_SYMBOL(memstart_addr);

/*
 * If the corresponding config options are enabled, we create both ZONE_DMA
 * and ZONE_DMA32. By default ZONE_DMA covers the 32-bit addressable memory
 * unless restricted on specific platforms (e.g. 30-bit on Raspberry Pi 4).
 * In such case, ZONE_DMA32 covers the rest of the 32-bit addressable memory,
 * otherwise it is empty.
 *
 * Memory reservation for crash kernel either done early or deferred
 * depending on DMA memory zones configs (ZONE_DMA) --
 *
 * In absence of ZONE_DMA configs arm64_dma_phys_limit initialized
 * here instead of max_zone_phys().  This lets early reservation of
 * crash kernel memory which has a dependency on arm64_dma_phys_limit.
 * Reserving memory early for crash kernel allows linear creation of block
 * mappings (greater than page-granularity) for all the memory bank rangs.
 * In this scheme a comparatively quicker boot is observed.
 *
 * If ZONE_DMA configs are defined, crash kernel memory reservation
 * is delayed until DMA zone memory range size initilazation performed in
 * zone_sizes_init().  The defer is necessary to steer clear of DMA zone
 * memory range to avoid overlap allocation.  So crash kernel memory boundaries
 * are not known when mapping all bank memory ranges, which otherwise means
 * not possible to exclude crash kernel range from creating block mappings
 * so page-granularity mappings are created for the entire memory range.
 * Hence a slightly slower boot is observed.
 *
 * Note: Page-granularity mapppings are necessary for crash kernel memory
 * range for shrinking its size via /sys/kernel/kexec_crash_size interface.
 */
#if IS_ENABLED(CONFIG_ZONE_DMA) || IS_ENABLED(CONFIG_ZONE_DMA32)
phys_addr_t __ro_after_init arm64_dma_phys_limit;
#else
phys_addr_t __ro_after_init arm64_dma_phys_limit = PHYS_MASK + 1;
#endif

/*
 * Provide a run-time mean of disabling ZONE_DMA32 if it is enabled via
 * CONFIG_ZONE_DMA32.
 */
static bool disable_dma32 __ro_after_init;

>>>>>>> upstream/android-13
#ifdef CONFIG_KEXEC_CORE
/*
 * reserve_crashkernel() - reserves memory for crash kernel
 *
 * This function reserves memory area given in "crashkernel=" kernel command
 * line parameter. The memory reserved is used by dump capture kernel when
 * primary kernel is crashing.
 */
static void __init reserve_crashkernel(void)
{
	unsigned long long crash_base, crash_size;
<<<<<<< HEAD
=======
	unsigned long long crash_max = arm64_dma_phys_limit;
>>>>>>> upstream/android-13
	int ret;

	ret = parse_crashkernel(boot_command_line, memblock_phys_mem_size(),
				&crash_size, &crash_base);
	/* no crashkernel= or invalid value specified */
	if (ret || !crash_size)
		return;

	crash_size = PAGE_ALIGN(crash_size);

<<<<<<< HEAD
	if (crash_base == 0) {
		/* Current arm64 boot protocol requires 2MB alignment */
		crash_base = memblock_find_in_range(0, ARCH_LOW_ADDRESS_LIMIT,
				crash_size, SZ_2M);
		if (crash_base == 0) {
			pr_warn("cannot allocate crashkernel (size:0x%llx)\n",
				crash_size);
			return;
		}
	} else {
		/* User specifies base address explicitly. */
		if (!memblock_is_region_memory(crash_base, crash_size)) {
			pr_warn("cannot reserve crashkernel: region is not memory\n");
			return;
		}

		if (memblock_is_region_reserved(crash_base, crash_size)) {
			pr_warn("cannot reserve crashkernel: region overlaps reserved memory\n");
			return;
		}

		if (!IS_ALIGNED(crash_base, SZ_2M)) {
			pr_warn("cannot reserve crashkernel: base address is not 2MB aligned\n");
			return;
		}
	}
	memblock_reserve(crash_base, crash_size);
=======
	/* User specifies base address explicitly. */
	if (crash_base)
		crash_max = crash_base + crash_size;

	/* Current arm64 boot protocol requires 2MB alignment */
	crash_base = memblock_phys_alloc_range(crash_size, SZ_2M,
					       crash_base, crash_max);
	if (!crash_base) {
		pr_warn("cannot allocate crashkernel (size:0x%llx)\n",
			crash_size);
		return;
	}
>>>>>>> upstream/android-13

	pr_info("crashkernel reserved: 0x%016llx - 0x%016llx (%lld MB)\n",
		crash_base, crash_base + crash_size, crash_size >> 20);

<<<<<<< HEAD
	crashk_res.start = crash_base;
	crashk_res.end = crash_base + crash_size - 1;
}

static void __init kexec_reserve_crashkres_pages(void)
{
#ifdef CONFIG_HIBERNATION
	phys_addr_t addr;
	struct page *page;

	if (!crashk_res.end)
		return;

	/*
	 * To reduce the size of hibernation image, all the pages are
	 * marked as Reserved initially.
	 */
	for (addr = crashk_res.start; addr < (crashk_res.end + 1);
			addr += PAGE_SIZE) {
		page = phys_to_page(addr);
		SetPageReserved(page);
	}
#endif
}
=======
	/*
	 * The crashkernel memory will be removed from the kernel linear
	 * map. Inform kmemleak so that it won't try to access it.
	 */
	kmemleak_ignore_phys(crash_base);
	crashk_res.start = crash_base;
	crashk_res.end = crash_base + crash_size - 1;
}
>>>>>>> upstream/android-13
#else
static void __init reserve_crashkernel(void)
{
}
<<<<<<< HEAD

static void __init kexec_reserve_crashkres_pages(void)
{
}
#endif /* CONFIG_KEXEC_CORE */

#ifdef CONFIG_CRASH_DUMP
static int __init early_init_dt_scan_elfcorehdr(unsigned long node,
		const char *uname, int depth, void *data)
{
	const __be32 *reg;
	int len;

	if (depth != 1 || strcmp(uname, "chosen") != 0)
		return 0;

	reg = of_get_flat_dt_prop(node, "linux,elfcorehdr", &len);
	if (!reg || (len < (dt_root_addr_cells + dt_root_size_cells)))
		return 1;

	elfcorehdr_addr = dt_mem_next_cell(dt_root_addr_cells, &reg);
	elfcorehdr_size = dt_mem_next_cell(dt_root_size_cells, &reg);

	return 1;
}

/*
 * reserve_elfcorehdr() - reserves memory for elf core header
 *
 * This function reserves the memory occupied by an elf core header
 * described in the device tree. This region contains all the
 * information about primary kernel's core image and is used by a dump
 * capture kernel to access the system memory on primary kernel.
 */
static void __init reserve_elfcorehdr(void)
{
	of_scan_flat_dt(early_init_dt_scan_elfcorehdr, NULL);

	if (!elfcorehdr_size)
		return;

	if (memblock_is_region_reserved(elfcorehdr_addr, elfcorehdr_size)) {
		pr_warn("elfcorehdr is overlapped\n");
		return;
	}

	memblock_reserve(elfcorehdr_addr, elfcorehdr_size);

	pr_info("Reserving %lldKB of memory at 0x%llx for elfcorehdr\n",
		elfcorehdr_size >> 10, elfcorehdr_addr);
}
#else
static void __init reserve_elfcorehdr(void)
{
}
#endif /* CONFIG_CRASH_DUMP */
/*
 * Return the maximum physical address for ZONE_DMA32 (DMA_BIT_MASK(32)). It
 * currently assumes that for memory starting above 4G, 32-bit devices will
 * use a DMA offset.
 */
static phys_addr_t __init max_zone_dma_phys(void)
{
	phys_addr_t offset = memblock_start_of_DRAM() & GENMASK_ULL(63, 32);
	return min(offset + (1ULL << 32), memblock_end_of_DRAM());
}

#ifdef CONFIG_NUMA
=======
#endif /* CONFIG_KEXEC_CORE */

/*
 * Return the maximum physical address for a zone accessible by the given bits
 * limit. If DRAM starts above 32-bit, expand the zone to the maximum
 * available memory, otherwise cap it at 32-bit.
 */
static phys_addr_t __init max_zone_phys(unsigned int zone_bits)
{
	phys_addr_t zone_mask = DMA_BIT_MASK(zone_bits);
	phys_addr_t phys_start = memblock_start_of_DRAM();

	if (phys_start > U32_MAX)
		zone_mask = PHYS_ADDR_MAX;
	else if (phys_start > zone_mask)
		zone_mask = U32_MAX;

	return min(zone_mask, memblock_end_of_DRAM() - 1) + 1;
}
>>>>>>> upstream/android-13

static void __init zone_sizes_init(unsigned long min, unsigned long max)
{
	unsigned long max_zone_pfns[MAX_NR_ZONES]  = {0};
<<<<<<< HEAD

#ifdef CONFIG_ZONE_DMA32
	max_zone_pfns[ZONE_DMA32] = PFN_DOWN(max_zone_dma_phys());
#endif
	max_zone_pfns[ZONE_NORMAL] = max;

	free_area_init_nodes(max_zone_pfns);
}

#else

static void __init zone_sizes_init(unsigned long min, unsigned long max)
{
	struct memblock_region *reg;
	unsigned long zone_size[MAX_NR_ZONES], zhole_size[MAX_NR_ZONES];
	unsigned long max_dma = min;

	memset(zone_size, 0, sizeof(zone_size));

	/* 4GB maximum for 32-bit only capable devices */
#ifdef CONFIG_ZONE_DMA32
	max_dma = PFN_DOWN(arm64_dma_phys_limit);
	zone_size[ZONE_DMA32] = max_dma - min;
#endif
	zone_size[ZONE_NORMAL] = max - max_dma;

	memcpy(zhole_size, zone_size, sizeof(zhole_size));

	for_each_memblock(memory, reg) {
		unsigned long start = memblock_region_memory_base_pfn(reg);
		unsigned long end = memblock_region_memory_end_pfn(reg);

		if (start >= max)
			continue;

#ifdef CONFIG_ZONE_DMA32
		if (start < max_dma) {
			unsigned long dma_end = min(end, max_dma);
			zhole_size[ZONE_DMA32] -= dma_end - start;
		}
#endif
		if (end > max_dma) {
			unsigned long normal_end = min(end, max);
			unsigned long normal_start = max(start, max_dma);
			zhole_size[ZONE_NORMAL] -= normal_end - normal_start;
		}
	}

	free_area_init_node(0, zone_size, min, zhole_size);
}

#endif /* CONFIG_NUMA */

#ifdef CONFIG_HAVE_ARCH_PFN_VALID
int pfn_valid(unsigned long pfn)
{
	phys_addr_t addr = pfn << PAGE_SHIFT;

	if ((addr >> PAGE_SHIFT) != pfn)
		return 0;
	return memblock_is_map_memory(addr);
}
EXPORT_SYMBOL(pfn_valid);
#endif

#ifndef CONFIG_SPARSEMEM
static void __init arm64_memory_present(void)
{
}
#else
static void __init arm64_memory_present(void)
{
	struct memblock_region *reg;

	for_each_memblock(memory, reg) {
		int nid = memblock_get_region_node(reg);

		memory_present(nid, memblock_region_memory_base_pfn(reg),
				memblock_region_memory_end_pfn(reg));
	}
}
#endif
=======
	unsigned int __maybe_unused acpi_zone_dma_bits;
	unsigned int __maybe_unused dt_zone_dma_bits;
	phys_addr_t __maybe_unused dma32_phys_limit = max_zone_phys(32);

#ifdef CONFIG_ZONE_DMA
	acpi_zone_dma_bits = fls64(acpi_iort_dma_get_max_cpu_address());
	dt_zone_dma_bits = fls64(of_dma_get_max_cpu_address(NULL));
	zone_dma_bits = min3(32U, dt_zone_dma_bits, acpi_zone_dma_bits);
	arm64_dma_phys_limit = max_zone_phys(zone_dma_bits);
	max_zone_pfns[ZONE_DMA] = PFN_DOWN(arm64_dma_phys_limit);
#endif
#ifdef CONFIG_ZONE_DMA32
	max_zone_pfns[ZONE_DMA32] = disable_dma32 ? 0 : PFN_DOWN(dma32_phys_limit);
	if (!arm64_dma_phys_limit)
		arm64_dma_phys_limit = dma32_phys_limit;
#endif
	max_zone_pfns[ZONE_NORMAL] = max;

	free_area_init(max_zone_pfns);
}

static int __init early_disable_dma32(char *buf)
{
	if (!buf)
		return -EINVAL;

	if (!strcmp(buf, "on"))
		disable_dma32 = true;

	return 0;
}
early_param("disable_dma32", early_disable_dma32);

int pfn_is_map_memory(unsigned long pfn)
{
	phys_addr_t addr = PFN_PHYS(pfn);

	/* avoid false positives for bogus PFNs, see comment in pfn_valid() */
	if (PHYS_PFN(addr) != pfn)
		return 0;

	return memblock_is_map_memory(addr);
}
EXPORT_SYMBOL(pfn_is_map_memory);
>>>>>>> upstream/android-13

static phys_addr_t memory_limit = PHYS_ADDR_MAX;

/*
 * Limit the memory size that was specified via FDT.
 */
static int __init early_mem(char *p)
{
	if (!p)
		return 1;

	memory_limit = memparse(p, &p) & PAGE_MASK;
	pr_notice("Memory limited to %lldMB\n", memory_limit >> 20);

	return 0;
}
early_param("mem", early_mem);

<<<<<<< HEAD
static int __init early_init_dt_scan_usablemem(unsigned long node,
		const char *uname, int depth, void *data)
{
	struct memblock_region *usablemem = data;
	const __be32 *reg;
	int len;

	if (depth != 1 || strcmp(uname, "chosen") != 0)
		return 0;

	reg = of_get_flat_dt_prop(node, "linux,usable-memory-range", &len);
	if (!reg || (len < (dt_root_addr_cells + dt_root_size_cells)))
		return 1;

	usablemem->base = dt_mem_next_cell(dt_root_addr_cells, &reg);
	usablemem->size = dt_mem_next_cell(dt_root_size_cells, &reg);

	return 1;
}

static void __init fdt_enforce_memory_region(void)
{
	struct memblock_region reg = {
		.size = 0,
	};

	of_scan_flat_dt(early_init_dt_scan_usablemem, &reg);

	if (reg.size)
		memblock_cap_memory_range(reg.base, reg.size);
}

void __init arm64_memblock_init(void)
{
	const s64 linear_region_size = -(s64)PAGE_OFFSET;

	set_memsize_kernel_type(MEMSIZE_KERNEL_STOP);
	/* Handle linux,usable-memory-range property */
	fdt_enforce_memory_region();
=======
void __init arm64_memblock_init(void)
{
	s64 linear_region_size = PAGE_END - _PAGE_OFFSET(vabits_actual);

	/*
	 * Corner case: 52-bit VA capable systems running KVM in nVHE mode may
	 * be limited in their ability to support a linear map that exceeds 51
	 * bits of VA space, depending on the placement of the ID map. Given
	 * that the placement of the ID map may be randomized, let's simply
	 * limit the kernel's linear map to 51 bits as well if we detect this
	 * configuration.
	 */
	if (IS_ENABLED(CONFIG_KVM) && vabits_actual == 52 &&
	    is_hyp_mode_available() && !is_kernel_in_hyp_mode()) {
		pr_info("Capping linear region to 51 bits for KVM in nVHE mode on LVA capable hardware.\n");
		linear_region_size = min_t(u64, linear_region_size, BIT(51));
	}
>>>>>>> upstream/android-13

	/* Remove memory above our supported physical address size */
	memblock_remove(1ULL << PHYS_MASK_SHIFT, ULLONG_MAX);

	/*
<<<<<<< HEAD
	 * Ensure that the linear region takes up exactly half of the kernel
	 * virtual address space. This way, we can distinguish a linear address
	 * from a kernel/module/vmalloc address by testing a single bit.
	 */
	BUILD_BUG_ON(linear_region_size != BIT(VA_BITS - 1));

	/*
=======
>>>>>>> upstream/android-13
	 * Select a suitable value for the base of physical memory.
	 */
	memstart_addr = round_down(memblock_start_of_DRAM(),
				   ARM64_MEMSTART_ALIGN);

<<<<<<< HEAD
=======
	if ((memblock_end_of_DRAM() - memstart_addr) > linear_region_size)
		pr_warn("Memory doesn't fit in the linear mapping, VA_BITS too small\n");

>>>>>>> upstream/android-13
	/*
	 * Remove the memory that we will not be able to cover with the
	 * linear mapping. Take care not to clip the kernel which may be
	 * high in memory.
	 */
	memblock_remove(max_t(u64, memstart_addr + linear_region_size,
			__pa_symbol(_end)), ULLONG_MAX);
	if (memstart_addr + linear_region_size < memblock_end_of_DRAM()) {
		/* ensure that memstart_addr remains sufficiently aligned */
		memstart_addr = round_up(memblock_end_of_DRAM() - linear_region_size,
					 ARM64_MEMSTART_ALIGN);
		memblock_remove(0, memstart_addr);
	}

	/*
<<<<<<< HEAD
=======
	 * If we are running with a 52-bit kernel VA config on a system that
	 * does not support it, we have to place the available physical
	 * memory in the 48-bit addressable part of the linear region, i.e.,
	 * we have to move it upward. Since memstart_addr represents the
	 * physical address of PAGE_OFFSET, we have to *subtract* from it.
	 */
	if (IS_ENABLED(CONFIG_ARM64_VA_BITS_52) && (vabits_actual != 52))
		memstart_addr -= _PAGE_OFFSET(48) - _PAGE_OFFSET(52);

	/*
>>>>>>> upstream/android-13
	 * Apply the memory limit if it was set. Since the kernel may be loaded
	 * high up in memory, add back the kernel region that must be accessible
	 * via the linear mapping.
	 */
	if (memory_limit != PHYS_ADDR_MAX) {
		memblock_mem_limit_remove_map(memory_limit);
		memblock_add(__pa_symbol(_text), (u64)(_end - _text));
	}

<<<<<<< HEAD
	if (IS_ENABLED(CONFIG_BLK_DEV_INITRD) && initrd_start) {
=======
	if (IS_ENABLED(CONFIG_BLK_DEV_INITRD) && phys_initrd_size) {
>>>>>>> upstream/android-13
		/*
		 * Add back the memory we just removed if it results in the
		 * initrd to become inaccessible via the linear mapping.
		 * Otherwise, this is a no-op
		 */
<<<<<<< HEAD
		u64 base = initrd_start & PAGE_MASK;
		u64 size = PAGE_ALIGN(initrd_end) - base;
=======
		u64 base = phys_initrd_start & PAGE_MASK;
		u64 size = PAGE_ALIGN(phys_initrd_start + phys_initrd_size) - base;
>>>>>>> upstream/android-13

		/*
		 * We can only add back the initrd memory if we don't end up
		 * with more memory than we can address via the linear mapping.
		 * It is up to the bootloader to position the kernel and the
		 * initrd reasonably close to each other (i.e., within 32 GB of
		 * each other) so that all granule/#levels combinations can
		 * always access both.
		 */
		if (WARN(base < memblock_start_of_DRAM() ||
			 base + size > memblock_start_of_DRAM() +
				       linear_region_size,
			"initrd not fully accessible via the linear mapping -- please check your bootloader ...\n")) {
<<<<<<< HEAD
			initrd_start = 0;
=======
			phys_initrd_size = 0;
>>>>>>> upstream/android-13
		} else {
			memblock_remove(base, size); /* clear MEMBLOCK_ flags */
			memblock_add(base, size);
			memblock_reserve(base, size);
<<<<<<< HEAD
			record_memsize_reserved("initrd", base, size, false, false);
=======
>>>>>>> upstream/android-13
		}
	}

	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE)) {
		extern u16 memstart_offset_seed;
<<<<<<< HEAD
		u64 range = linear_region_size -
			    (memblock_end_of_DRAM() - memblock_start_of_DRAM());

		/*
		 * If the size of the linear region exceeds, by a sufficient
		 * margin, the size of the region that the available physical
		 * memory spans, randomize the linear region as well.
		 */
		if (memstart_offset_seed > 0 && range >= ARM64_MEMSTART_ALIGN) {
=======
		u64 mmfr0 = read_cpuid(ID_AA64MMFR0_EL1);
		int parange = cpuid_feature_extract_unsigned_field(
					mmfr0, ID_AA64MMFR0_PARANGE_SHIFT);
		s64 range = linear_region_size -
			    BIT(id_aa64mmfr0_parange_to_phys_shift(parange));

		/*
		 * If the size of the linear region exceeds, by a sufficient
		 * margin, the size of the region that the physical memory can
		 * span, randomize the linear region as well.
		 */
		if (memstart_offset_seed > 0 && range >= (s64)ARM64_MEMSTART_ALIGN) {
>>>>>>> upstream/android-13
			range /= ARM64_MEMSTART_ALIGN;
			memstart_addr -= ARM64_MEMSTART_ALIGN *
					 ((range * memstart_offset_seed) >> 16);
		}
	}

	/*
	 * Register the kernel text, kernel data, initrd, and initial
	 * pagetables with memblock.
	 */
<<<<<<< HEAD
	set_memsize_kernel_type(MEMSIZE_KERNEL_KERNEL);
	memblock_reserve(__pa_symbol(_text), _end - _text);
	set_memsize_kernel_type(MEMSIZE_KERNEL_STOP);
	record_memsize_reserved("initmem", __pa(__init_begin),
				__init_end - __init_begin, false, false);
#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_start) {
		memblock_reserve(initrd_start, initrd_end - initrd_start);

		/* the generic initrd code expects virtual addresses */
		initrd_start = __phys_to_virt(initrd_start);
		initrd_end = __phys_to_virt(initrd_end);
	}
#endif

	early_init_fdt_scan_reserved_mem();

	/* 4GB maximum for 32-bit only capable devices */
	if (IS_ENABLED(CONFIG_ZONE_DMA32))
		arm64_dma_phys_limit = max_zone_dma_phys();
	else
		arm64_dma_phys_limit = PHYS_MASK + 1;

	reserve_crashkernel();

	reserve_elfcorehdr();

	high_memory = __va(memblock_end_of_DRAM() - 1) + 1;

	dma_contiguous_reserve(arm64_dma_phys_limit);
	set_memsize_kernel_type(MEMSIZE_KERNEL_OTHERS);

	memblock_allow_resize();
=======
	memblock_reserve(__pa_symbol(_stext), _end - _stext);
	if (IS_ENABLED(CONFIG_BLK_DEV_INITRD) && phys_initrd_size) {
		/* the generic initrd code expects virtual addresses */
		initrd_start = __phys_to_virt(phys_initrd_start);
		initrd_end = initrd_start + phys_initrd_size;
	}

	early_init_fdt_scan_reserved_mem();

	if (!IS_ENABLED(CONFIG_ZONE_DMA) && !IS_ENABLED(CONFIG_ZONE_DMA32))
		reserve_crashkernel();

	high_memory = __va(memblock_end_of_DRAM() - 1) + 1;
>>>>>>> upstream/android-13
}

void __init bootmem_init(void)
{
	unsigned long min, max;

<<<<<<< HEAD
	set_memsize_kernel_type(MEMSIZE_KERNEL_PAGING);
=======
>>>>>>> upstream/android-13
	min = PFN_UP(memblock_start_of_DRAM());
	max = PFN_DOWN(memblock_end_of_DRAM());

	early_memtest(min << PAGE_SHIFT, max << PAGE_SHIFT);

	max_pfn = max_low_pfn = max;
	min_low_pfn = min;

<<<<<<< HEAD
	arm64_numa_init();
	/*
	 * Sparsemem tries to allocate bootmem in memory_present(), so must be
	 * done after the fixed reservations.
	 */
	arm64_memory_present();

	sparse_init();
	zone_sizes_init(min, max);

	memblock_dump_all();
	set_memsize_kernel_type(MEMSIZE_KERNEL_OTHERS);
}

#ifndef CONFIG_SPARSEMEM_VMEMMAP
static inline void free_memmap(unsigned long start_pfn, unsigned long end_pfn)
{
	struct page *start_pg, *end_pg;
	unsigned long pg, pgend;

	/*
	 * Convert start_pfn/end_pfn to a struct page pointer.
	 */
	start_pg = pfn_to_page(start_pfn - 1) + 1;
	end_pg = pfn_to_page(end_pfn - 1) + 1;

	/*
	 * Convert to physical addresses, and round start upwards and end
	 * downwards.
	 */
	pg = (unsigned long)PAGE_ALIGN(__pa(start_pg));
	pgend = (unsigned long)__pa(end_pg) & PAGE_MASK;

	/*
	 * If there are free pages between these, free the section of the
	 * memmap array.
	 */
	if (pg < pgend)
		free_bootmem(pg, pgend - pg);
}

/*
 * The mem_map array can get very big. Free the unused area of the memory map.
 */
static void __init free_unused_memmap(void)
{
	unsigned long start, prev_end = 0;
	struct memblock_region *reg;

	for_each_memblock(memory, reg) {
		start = __phys_to_pfn(reg->base);

#ifdef CONFIG_SPARSEMEM
		/*
		 * Take care not to free memmap entries that don't exist due
		 * to SPARSEMEM sections which aren't present.
		 */
		start = min(start, ALIGN(prev_end, PAGES_PER_SECTION));
#endif
		/*
		 * If we had a previous bank, and there is a space between the
		 * current bank and the previous, free it.
		 */
		if (prev_end && prev_end < start)
			free_memmap(prev_end, start);

		/*
		 * Align up here since the VM subsystem insists that the
		 * memmap entries are valid from the bank end aligned to
		 * MAX_ORDER_NR_PAGES.
		 */
		prev_end = ALIGN(__phys_to_pfn(reg->base + reg->size),
				 MAX_ORDER_NR_PAGES);
	}

#ifdef CONFIG_SPARSEMEM
	if (!IS_ALIGNED(prev_end, PAGES_PER_SECTION))
		free_memmap(prev_end, ALIGN(prev_end, PAGES_PER_SECTION));
#endif
}
#endif	/* !CONFIG_SPARSEMEM_VMEMMAP */

=======
	arch_numa_init();

	/*
	 * must be done after arch_numa_init() which calls numa_init() to
	 * initialize node_online_map that gets used in hugetlb_cma_reserve()
	 * while allocating required CMA size across online nodes.
	 */
#if defined(CONFIG_HUGETLB_PAGE) && defined(CONFIG_CMA)
	arm64_hugetlb_cma_reserve();
#endif

	dma_pernuma_cma_reserve();

	kvm_hyp_reserve();

	/*
	 * sparse_init() tries to allocate memory from memblock, so must be
	 * done after the fixed reservations
	 */
	sparse_init();
	zone_sizes_init(min, max);

	/*
	 * Reserve the CMA area after arm64_dma_phys_limit was initialised.
	 */
	dma_contiguous_reserve(arm64_dma_phys_limit);

	/*
	 * request_standard_resources() depends on crashkernel's memory being
	 * reserved, so do it here.
	 */
	if (IS_ENABLED(CONFIG_ZONE_DMA) || IS_ENABLED(CONFIG_ZONE_DMA32))
		reserve_crashkernel();

	memblock_dump_all();
}

>>>>>>> upstream/android-13
/*
 * mem_init() marks the free areas in the mem_map and tells us how much memory
 * is free.  This is done after various parts of the system have claimed their
 * memory after the kernel image.
 */
void __init mem_init(void)
{
	if (swiotlb_force == SWIOTLB_FORCE ||
<<<<<<< HEAD
	    max_pfn > (arm64_dma_phys_limit >> PAGE_SHIFT))
		swiotlb_init(1);
	else
		swiotlb_force = SWIOTLB_NO_FORCE;

	set_max_mapnr(pfn_to_page(max_pfn) - mem_map);

#ifndef CONFIG_SPARSEMEM_VMEMMAP
	free_unused_memmap();
#endif
	/* this will put all unused low memory onto the freelists */
	free_all_bootmem();

	kexec_reserve_crashkres_pages();

	mem_init_print_info(NULL);
=======
	    max_pfn > PFN_DOWN(arm64_dma_phys_limit))
		swiotlb_init(1);
	else if (!xen_swiotlb_detect())
		swiotlb_force = SWIOTLB_NO_FORCE;

	set_max_mapnr(max_pfn - PHYS_PFN_OFFSET);

	/* this will put all unused low memory onto the freelists */
	memblock_free_all();
>>>>>>> upstream/android-13

	/*
	 * Check boundaries twice: Some fundamental inconsistencies can be
	 * detected at build time already.
	 */
#ifdef CONFIG_COMPAT
<<<<<<< HEAD
	BUILD_BUG_ON(TASK_SIZE_32			> TASK_SIZE_64);
#endif

#ifdef CONFIG_SPARSEMEM_VMEMMAP
	/*
	 * Make sure we chose the upper bound of sizeof(struct page)
	 * correctly when sizing the VMEMMAP array.
	 */
	BUILD_BUG_ON(sizeof(struct page) > (1 << STRUCT_PAGE_MAX_SHIFT));
#endif
=======
	BUILD_BUG_ON(TASK_SIZE_32 > DEFAULT_MAP_WINDOW_64);
#endif

	/*
	 * Selected page table levels should match when derived from
	 * scratch using the virtual address range and page size.
	 */
	BUILD_BUG_ON(ARM64_HW_PGTABLE_LEVELS(CONFIG_ARM64_VA_BITS) !=
		     CONFIG_PGTABLE_LEVELS);
>>>>>>> upstream/android-13

	if (PAGE_SIZE >= 16384 && get_num_physpages() <= 128) {
		extern int sysctl_overcommit_memory;
		/*
		 * On a machine this small we won't get anywhere without
		 * overcommit, so turn it on by default.
		 */
		sysctl_overcommit_memory = OVERCOMMIT_ALWAYS;
	}
}

void free_initmem(void)
{
	free_reserved_area(lm_alias(__init_begin),
			   lm_alias(__init_end),
<<<<<<< HEAD
			   0, "unused kernel");
=======
			   POISON_FREE_INITMEM, "unused kernel");
>>>>>>> upstream/android-13
	/*
	 * Unmap the __init region but leave the VM area in place. This
	 * prevents the region from being reused for kernel modules, which
	 * is not supported by kallsyms.
	 */
<<<<<<< HEAD
	unmap_kernel_range((u64)__init_begin, (u64)(__init_end - __init_begin));
}

#ifdef CONFIG_BLK_DEV_INITRD

static int keep_initrd __initdata;

void __init free_initrd_mem(unsigned long start, unsigned long end)
{
	if (!keep_initrd) {
		free_reserved_area((void *)start, (void *)end, 0, "initrd");
		memblock_free(__virt_to_phys(start), end - start);
	}
}

static int __init keepinitrd_setup(char *__unused)
{
	keep_initrd = 1;
	return 1;
}

__setup("keepinitrd", keepinitrd_setup);
#endif

/*
 * Dump out memory limit information on panic.
 */
static int dump_mem_limit(struct notifier_block *self, unsigned long v, void *p)
=======
	vunmap_range((u64)__init_begin, (u64)__init_end);
}

void dump_mem_limit(void)
>>>>>>> upstream/android-13
{
	if (memory_limit != PHYS_ADDR_MAX) {
		pr_emerg("Memory Limit: %llu MB\n", memory_limit >> 20);
	} else {
		pr_emerg("Memory Limit: none\n");
	}
<<<<<<< HEAD
	return 0;
}

static struct notifier_block mem_limit_notifier = {
	.notifier_call = dump_mem_limit,
};

static int __init register_mem_limit_dumper(void)
{
	atomic_notifier_chain_register(&panic_notifier_list,
				       &mem_limit_notifier);
	return 0;
}
__initcall(register_mem_limit_dumper);
=======
}
>>>>>>> upstream/android-13
