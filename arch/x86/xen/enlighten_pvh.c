// SPDX-License-Identifier: GPL-2.0
#include <linux/acpi.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> upstream/android-13

#include <xen/hvc-console.h>

#include <asm/io_apic.h>
#include <asm/hypervisor.h>
#include <asm/e820/api.h>
<<<<<<< HEAD
#include <asm/x86_init.h>

=======

#include <xen/xen.h>
>>>>>>> upstream/android-13
#include <asm/xen/interface.h>
#include <asm/xen/hypercall.h>

#include <xen/interface/memory.h>
<<<<<<< HEAD
#include <xen/interface/hvm/start_info.h>
=======
>>>>>>> upstream/android-13

#include "xen-ops.h"

/*
 * PVH variables.
 *
<<<<<<< HEAD
 * xen_pvh pvh_bootparams and pvh_start_info need to live in data segment
 * since they are used after startup_{32|64}, which clear .bss, are invoked.
 */
bool xen_pvh __attribute__((section(".data"))) = 0;
struct boot_params pvh_bootparams __attribute__((section(".data")));
struct hvm_start_info pvh_start_info __attribute__((section(".data")));

unsigned int pvh_start_info_sz = sizeof(pvh_start_info);

static u64 pvh_get_root_pointer(void)
{
	return pvh_start_info.rsdp_paddr;
}

static void __init init_pvh_bootparams(void)
{
	struct xen_memory_map memmap;
	int rc;

	memset(&pvh_bootparams, 0, sizeof(pvh_bootparams));

	memmap.nr_entries = ARRAY_SIZE(pvh_bootparams.e820_table);
	set_xen_guest_handle(memmap.buffer, pvh_bootparams.e820_table);
	rc = HYPERVISOR_memory_op(XENMEM_memory_map, &memmap);
	if (rc) {
		xen_raw_printk("XENMEM_memory_map failed (%d)\n", rc);
		BUG();
	}
	pvh_bootparams.e820_entries = memmap.nr_entries;

	if (pvh_bootparams.e820_entries < E820_MAX_ENTRIES_ZEROPAGE - 1) {
		pvh_bootparams.e820_table[pvh_bootparams.e820_entries].addr =
			ISA_START_ADDRESS;
		pvh_bootparams.e820_table[pvh_bootparams.e820_entries].size =
			ISA_END_ADDRESS - ISA_START_ADDRESS;
		pvh_bootparams.e820_table[pvh_bootparams.e820_entries].type =
			E820_TYPE_RESERVED;
		pvh_bootparams.e820_entries++;
	} else
		xen_raw_printk("Warning: Can fit ISA range into e820\n");

	pvh_bootparams.hdr.cmd_line_ptr =
		pvh_start_info.cmdline_paddr;

	/* The first module is always ramdisk. */
	if (pvh_start_info.nr_modules) {
		struct hvm_modlist_entry *modaddr =
			__va(pvh_start_info.modlist_paddr);
		pvh_bootparams.hdr.ramdisk_image = modaddr->paddr;
		pvh_bootparams.hdr.ramdisk_size = modaddr->size;
	}

	/*
	 * See Documentation/x86/boot.txt.
	 *
	 * Version 2.12 supports Xen entry point but we will use default x86/PC
	 * environment (i.e. hardware_subarch 0).
	 */
	pvh_bootparams.hdr.version = (2 << 8) | 12;
	pvh_bootparams.hdr.type_of_loader = (9 << 4) | 0; /* Xen loader */

	x86_init.acpi.get_root_pointer = pvh_get_root_pointer;

	xen_efi_init(&pvh_bootparams);
}

/*
 * This routine (and those that it might call) should not use
 * anything that lives in .bss since that segment will be cleared later.
 */
void __init xen_prepare_pvh(void)
=======
 * The variable xen_pvh needs to live in a data segment since it is used
 * after startup_{32|64} is invoked, which will clear the .bss segment.
 */
bool __ro_after_init xen_pvh;
EXPORT_SYMBOL_GPL(xen_pvh);

void __init xen_pvh_init(struct boot_params *boot_params)
>>>>>>> upstream/android-13
{
	u32 msr;
	u64 pfn;

<<<<<<< HEAD
	if (pvh_start_info.magic != XEN_HVM_START_MAGIC_VALUE) {
		xen_raw_printk("Error: Unexpected magic value (0x%08x)\n",
				pvh_start_info.magic);
		BUG();
	}

=======
>>>>>>> upstream/android-13
	xen_pvh = 1;
	xen_domain_type = XEN_HVM_DOMAIN;
	xen_start_flags = pvh_start_info.flags;

	msr = cpuid_ebx(xen_cpuid_base() + 2);
	pfn = __pa(hypercall_page);
	wrmsr_safe(msr, (u32)pfn, (u32)(pfn >> 32));

<<<<<<< HEAD
	init_pvh_bootparams();
=======
	if (xen_initial_domain())
		x86_init.oem.arch_setup = xen_add_preferred_consoles;
	x86_init.oem.banner = xen_banner;

	xen_efi_init(boot_params);
}

void __init mem_map_via_hcall(struct boot_params *boot_params_p)
{
	struct xen_memory_map memmap;
	int rc;

	memmap.nr_entries = ARRAY_SIZE(boot_params_p->e820_table);
	set_xen_guest_handle(memmap.buffer, boot_params_p->e820_table);
	rc = HYPERVISOR_memory_op(XENMEM_memory_map, &memmap);
	if (rc) {
		xen_raw_printk("XENMEM_memory_map failed (%d)\n", rc);
		BUG();
	}
	boot_params_p->e820_entries = memmap.nr_entries;
>>>>>>> upstream/android-13
}
