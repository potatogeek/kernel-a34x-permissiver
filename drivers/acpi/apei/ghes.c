<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * APEI Generic Hardware Error Source support
 *
 * Generic Hardware Error Source provides a way to report platform
 * hardware errors (such as that from chipset). It works in so called
 * "Firmware First" mode, that is, hardware errors are reported to
 * firmware firstly, then reported to Linux by firmware. This way,
 * some non-standard hardware error registers or non-standard hardware
 * link can be checked by firmware to produce more hardware error
 * information for Linux.
 *
 * For more information about Generic Hardware Error Source, please
 * refer to ACPI Specification version 4.0, section 17.3.2.6
 *
 * Copyright 2010,2011 Intel Corp.
 *   Author: Huang Ying <ying.huang@intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

=======
 */

#include <linux/arm_sdei.h>
>>>>>>> upstream/android-13
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/cper.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/ratelimit.h>
#include <linux/vmalloc.h>
#include <linux/irq_work.h>
#include <linux/llist.h>
#include <linux/genalloc.h>
#include <linux/pci.h>
<<<<<<< HEAD
=======
#include <linux/pfn.h>
>>>>>>> upstream/android-13
#include <linux/aer.h>
#include <linux/nmi.h>
#include <linux/sched/clock.h>
#include <linux/uuid.h>
#include <linux/ras.h>
<<<<<<< HEAD
=======
#include <linux/task_work.h>
>>>>>>> upstream/android-13

#include <acpi/actbl1.h>
#include <acpi/ghes.h>
#include <acpi/apei.h>
#include <asm/fixmap.h>
#include <asm/tlbflush.h>
#include <ras/ras_event.h>

#include "apei-internal.h"

#define GHES_PFX	"GHES: "

#define GHES_ESTATUS_MAX_SIZE		65536
#define GHES_ESOURCE_PREALLOC_MAX_SIZE	65536

#define GHES_ESTATUS_POOL_MIN_ALLOC_ORDER 3

/* This is just an estimation for memory pool allocation */
#define GHES_ESTATUS_CACHE_AVG_SIZE	512

#define GHES_ESTATUS_CACHES_SIZE	4

#define GHES_ESTATUS_IN_CACHE_MAX_NSEC	10000000000ULL
/* Prevent too many caches are allocated because of RCU */
#define GHES_ESTATUS_CACHE_ALLOCED_MAX	(GHES_ESTATUS_CACHES_SIZE * 3 / 2)

#define GHES_ESTATUS_CACHE_LEN(estatus_len)			\
	(sizeof(struct ghes_estatus_cache) + (estatus_len))
#define GHES_ESTATUS_FROM_CACHE(estatus_cache)			\
	((struct acpi_hest_generic_status *)				\
	 ((struct ghes_estatus_cache *)(estatus_cache) + 1))

#define GHES_ESTATUS_NODE_LEN(estatus_len)			\
	(sizeof(struct ghes_estatus_node) + (estatus_len))
#define GHES_ESTATUS_FROM_NODE(estatus_node)			\
	((struct acpi_hest_generic_status *)				\
	 ((struct ghes_estatus_node *)(estatus_node) + 1))

<<<<<<< HEAD
=======
#define GHES_VENDOR_ENTRY_LEN(gdata_len)                               \
	(sizeof(struct ghes_vendor_record_entry) + (gdata_len))
#define GHES_GDATA_FROM_VENDOR_ENTRY(vendor_entry)                     \
	((struct acpi_hest_generic_data *)                              \
	((struct ghes_vendor_record_entry *)(vendor_entry) + 1))

/*
 *  NMI-like notifications vary by architecture, before the compiler can prune
 *  unused static functions it needs a value for these enums.
 */
#ifndef CONFIG_ARM_SDE_INTERFACE
#define FIX_APEI_GHES_SDEI_NORMAL	__end_of_fixed_addresses
#define FIX_APEI_GHES_SDEI_CRITICAL	__end_of_fixed_addresses
#endif

>>>>>>> upstream/android-13
static inline bool is_hest_type_generic_v2(struct ghes *ghes)
{
	return ghes->generic->header.type == ACPI_HEST_TYPE_GENERIC_ERROR_V2;
}

/*
 * This driver isn't really modular, however for the time being,
 * continuing to use module_param is the easiest way to remain
 * compatible with existing boot arg use cases.
 */
bool ghes_disable;
module_param_named(disable, ghes_disable, bool, 0);

/*
 * All error sources notified with HED (Hardware Error Device) share a
 * single notifier callback, so they need to be linked and checked one
 * by one. This holds true for NMI too.
 *
 * RCU is used for these lists, so ghes_list_mutex is only used for
 * list changing, not for traversing.
 */
static LIST_HEAD(ghes_hed);
static DEFINE_MUTEX(ghes_list_mutex);

/*
 * Because the memory area used to transfer hardware error information
 * from BIOS to Linux can be determined only in NMI, IRQ or timer
 * handler, but general ioremap can not be used in atomic context, so
 * the fixmap is used instead.
 *
<<<<<<< HEAD
 * These 2 spinlocks are used to prevent the fixmap entries from being used
 * simultaneously.
 */
static DEFINE_RAW_SPINLOCK(ghes_ioremap_lock_nmi);
static DEFINE_SPINLOCK(ghes_ioremap_lock_irq);
=======
 * This spinlock is used to prevent the fixmap entry from being used
 * simultaneously.
 */
static DEFINE_SPINLOCK(ghes_notify_lock_irq);

struct ghes_vendor_record_entry {
	struct work_struct work;
	int error_severity;
	char vendor_record[];
};
>>>>>>> upstream/android-13

static struct gen_pool *ghes_estatus_pool;
static unsigned long ghes_estatus_pool_size_request;

static struct ghes_estatus_cache *ghes_estatus_caches[GHES_ESTATUS_CACHES_SIZE];
static atomic_t ghes_estatus_cache_alloced;

static int ghes_panic_timeout __read_mostly = 30;

<<<<<<< HEAD
static void __iomem *ghes_ioremap_pfn_nmi(u64 pfn)
=======
static void __iomem *ghes_map(u64 pfn, enum fixed_addresses fixmap_idx)
>>>>>>> upstream/android-13
{
	phys_addr_t paddr;
	pgprot_t prot;

<<<<<<< HEAD
	paddr = pfn << PAGE_SHIFT;
	prot = arch_apei_get_mem_attribute(paddr);
	__set_fixmap(FIX_APEI_GHES_NMI, paddr, prot);

	return (void __iomem *) fix_to_virt(FIX_APEI_GHES_NMI);
}

static void __iomem *ghes_ioremap_pfn_irq(u64 pfn)
{
	phys_addr_t paddr;
	pgprot_t prot;

	paddr = pfn << PAGE_SHIFT;
	prot = arch_apei_get_mem_attribute(paddr);
	__set_fixmap(FIX_APEI_GHES_IRQ, paddr, prot);

	return (void __iomem *) fix_to_virt(FIX_APEI_GHES_IRQ);
}

static void ghes_iounmap_nmi(void)
{
	clear_fixmap(FIX_APEI_GHES_NMI);
}

static void ghes_iounmap_irq(void)
{
	clear_fixmap(FIX_APEI_GHES_IRQ);
}

static int ghes_estatus_pool_init(void)
{
	ghes_estatus_pool = gen_pool_create(GHES_ESTATUS_POOL_MIN_ALLOC_ORDER, -1);
	if (!ghes_estatus_pool)
		return -ENOMEM;
	return 0;
}

static void ghes_estatus_pool_free_chunk(struct gen_pool *pool,
					      struct gen_pool_chunk *chunk,
					      void *data)
{
	vfree((void *)chunk->start_addr);
}

static void ghes_estatus_pool_exit(void)
{
	gen_pool_for_each_chunk(ghes_estatus_pool,
				ghes_estatus_pool_free_chunk, NULL);
	gen_pool_destroy(ghes_estatus_pool);
}

static int ghes_estatus_pool_expand(unsigned long len)
{
	unsigned long size, addr;

	ghes_estatus_pool_size_request += PAGE_ALIGN(len);
	size = gen_pool_size(ghes_estatus_pool);
	if (size >= ghes_estatus_pool_size_request)
		return 0;

	addr = (unsigned long)vmalloc(PAGE_ALIGN(len));
	if (!addr)
		return -ENOMEM;

	/*
	 * New allocation must be visible in all pgd before it can be found by
	 * an NMI allocating from the pool.
	 */
	vmalloc_sync_mappings();

	return gen_pool_add(ghes_estatus_pool, addr, PAGE_ALIGN(len), -1);
=======
	paddr = PFN_PHYS(pfn);
	prot = arch_apei_get_mem_attribute(paddr);
	__set_fixmap(fixmap_idx, paddr, prot);

	return (void __iomem *) __fix_to_virt(fixmap_idx);
}

static void ghes_unmap(void __iomem *vaddr, enum fixed_addresses fixmap_idx)
{
	int _idx = virt_to_fix((unsigned long)vaddr);

	WARN_ON_ONCE(fixmap_idx != _idx);
	clear_fixmap(fixmap_idx);
}

int ghes_estatus_pool_init(int num_ghes)
{
	unsigned long addr, len;
	int rc;

	ghes_estatus_pool = gen_pool_create(GHES_ESTATUS_POOL_MIN_ALLOC_ORDER, -1);
	if (!ghes_estatus_pool)
		return -ENOMEM;

	len = GHES_ESTATUS_CACHE_AVG_SIZE * GHES_ESTATUS_CACHE_ALLOCED_MAX;
	len += (num_ghes * GHES_ESOURCE_PREALLOC_MAX_SIZE);

	ghes_estatus_pool_size_request = PAGE_ALIGN(len);
	addr = (unsigned long)vmalloc(PAGE_ALIGN(len));
	if (!addr)
		goto err_pool_alloc;

	rc = gen_pool_add(ghes_estatus_pool, addr, PAGE_ALIGN(len), -1);
	if (rc)
		goto err_pool_add;

	return 0;

err_pool_add:
	vfree((void *)addr);

err_pool_alloc:
	gen_pool_destroy(ghes_estatus_pool);

	return -ENOMEM;
>>>>>>> upstream/android-13
}

static int map_gen_v2(struct ghes *ghes)
{
	return apei_map_generic_address(&ghes->generic_v2->read_ack_register);
}

static void unmap_gen_v2(struct ghes *ghes)
{
	apei_unmap_generic_address(&ghes->generic_v2->read_ack_register);
}

<<<<<<< HEAD
=======
static void ghes_ack_error(struct acpi_hest_generic_v2 *gv2)
{
	int rc;
	u64 val = 0;

	rc = apei_read(&val, &gv2->read_ack_register);
	if (rc)
		return;

	val &= gv2->read_ack_preserve << gv2->read_ack_register.bit_offset;
	val |= gv2->read_ack_write    << gv2->read_ack_register.bit_offset;

	apei_write(val, &gv2->read_ack_register);
}

>>>>>>> upstream/android-13
static struct ghes *ghes_new(struct acpi_hest_generic *generic)
{
	struct ghes *ghes;
	unsigned int error_block_length;
	int rc;

	ghes = kzalloc(sizeof(*ghes), GFP_KERNEL);
	if (!ghes)
		return ERR_PTR(-ENOMEM);

	ghes->generic = generic;
	if (is_hest_type_generic_v2(ghes)) {
		rc = map_gen_v2(ghes);
		if (rc)
			goto err_free;
	}

	rc = apei_map_generic_address(&generic->error_status_address);
	if (rc)
		goto err_unmap_read_ack_addr;
	error_block_length = generic->error_block_length;
	if (error_block_length > GHES_ESTATUS_MAX_SIZE) {
<<<<<<< HEAD
		pr_warning(FW_WARN GHES_PFX
			   "Error status block length is too long: %u for "
			   "generic hardware error source: %d.\n",
			   error_block_length, generic->header.source_id);
=======
		pr_warn(FW_WARN GHES_PFX
			"Error status block length is too long: %u for "
			"generic hardware error source: %d.\n",
			error_block_length, generic->header.source_id);
>>>>>>> upstream/android-13
		error_block_length = GHES_ESTATUS_MAX_SIZE;
	}
	ghes->estatus = kmalloc(error_block_length, GFP_KERNEL);
	if (!ghes->estatus) {
		rc = -ENOMEM;
		goto err_unmap_status_addr;
	}

	return ghes;

err_unmap_status_addr:
	apei_unmap_generic_address(&generic->error_status_address);
err_unmap_read_ack_addr:
	if (is_hest_type_generic_v2(ghes))
		unmap_gen_v2(ghes);
err_free:
	kfree(ghes);
	return ERR_PTR(rc);
}

static void ghes_fini(struct ghes *ghes)
{
	kfree(ghes->estatus);
	apei_unmap_generic_address(&ghes->generic->error_status_address);
	if (is_hest_type_generic_v2(ghes))
		unmap_gen_v2(ghes);
}

static inline int ghes_severity(int severity)
{
	switch (severity) {
	case CPER_SEV_INFORMATIONAL:
		return GHES_SEV_NO;
	case CPER_SEV_CORRECTED:
		return GHES_SEV_CORRECTED;
	case CPER_SEV_RECOVERABLE:
		return GHES_SEV_RECOVERABLE;
	case CPER_SEV_FATAL:
		return GHES_SEV_PANIC;
	default:
		/* Unknown, go panic */
		return GHES_SEV_PANIC;
	}
}

static void ghes_copy_tofrom_phys(void *buffer, u64 paddr, u32 len,
<<<<<<< HEAD
				  int from_phys)
{
	void __iomem *vaddr;
	unsigned long flags = 0;
	int in_nmi = in_nmi();
=======
				  int from_phys,
				  enum fixed_addresses fixmap_idx)
{
	void __iomem *vaddr;
>>>>>>> upstream/android-13
	u64 offset;
	u32 trunk;

	while (len > 0) {
		offset = paddr - (paddr & PAGE_MASK);
<<<<<<< HEAD
		if (in_nmi) {
			raw_spin_lock(&ghes_ioremap_lock_nmi);
			vaddr = ghes_ioremap_pfn_nmi(paddr >> PAGE_SHIFT);
		} else {
			spin_lock_irqsave(&ghes_ioremap_lock_irq, flags);
			vaddr = ghes_ioremap_pfn_irq(paddr >> PAGE_SHIFT);
		}
=======
		vaddr = ghes_map(PHYS_PFN(paddr), fixmap_idx);
>>>>>>> upstream/android-13
		trunk = PAGE_SIZE - offset;
		trunk = min(trunk, len);
		if (from_phys)
			memcpy_fromio(buffer, vaddr + offset, trunk);
		else
			memcpy_toio(vaddr + offset, buffer, trunk);
		len -= trunk;
		paddr += trunk;
		buffer += trunk;
<<<<<<< HEAD
		if (in_nmi) {
			ghes_iounmap_nmi();
			raw_spin_unlock(&ghes_ioremap_lock_nmi);
		} else {
			ghes_iounmap_irq();
			spin_unlock_irqrestore(&ghes_ioremap_lock_irq, flags);
		}
	}
}

static int ghes_read_estatus(struct ghes *ghes, int silent)
{
	struct acpi_hest_generic *g = ghes->generic;
	u64 buf_paddr;
	u32 len;
	int rc;

	rc = apei_read(&buf_paddr, &g->error_status_address);
	if (rc) {
		if (!silent && printk_ratelimit())
			pr_warning(FW_WARN GHES_PFX
=======
		ghes_unmap(vaddr, fixmap_idx);
	}
}

/* Check the top-level record header has an appropriate size. */
static int __ghes_check_estatus(struct ghes *ghes,
				struct acpi_hest_generic_status *estatus)
{
	u32 len = cper_estatus_len(estatus);

	if (len < sizeof(*estatus)) {
		pr_warn_ratelimited(FW_WARN GHES_PFX "Truncated error status block!\n");
		return -EIO;
	}

	if (len > ghes->generic->error_block_length) {
		pr_warn_ratelimited(FW_WARN GHES_PFX "Invalid error status block length!\n");
		return -EIO;
	}

	if (cper_estatus_check_header(estatus)) {
		pr_warn_ratelimited(FW_WARN GHES_PFX "Invalid CPER header!\n");
		return -EIO;
	}

	return 0;
}

/* Read the CPER block, returning its address, and header in estatus. */
static int __ghes_peek_estatus(struct ghes *ghes,
			       struct acpi_hest_generic_status *estatus,
			       u64 *buf_paddr, enum fixed_addresses fixmap_idx)
{
	struct acpi_hest_generic *g = ghes->generic;
	int rc;

	rc = apei_read(buf_paddr, &g->error_status_address);
	if (rc) {
		*buf_paddr = 0;
		pr_warn_ratelimited(FW_WARN GHES_PFX
>>>>>>> upstream/android-13
"Failed to read error status block address for hardware error source: %d.\n",
				   g->header.source_id);
		return -EIO;
	}
<<<<<<< HEAD
	if (!buf_paddr)
		return -ENOENT;

	ghes_copy_tofrom_phys(ghes->estatus, buf_paddr,
			      sizeof(*ghes->estatus), 1);
	if (!ghes->estatus->block_status)
		return -ENOENT;

	ghes->buffer_paddr = buf_paddr;
	ghes->flags |= GHES_TO_CLEAR;

	rc = -EIO;
	len = cper_estatus_len(ghes->estatus);
	if (len < sizeof(*ghes->estatus))
		goto err_read_block;
	if (len > ghes->generic->error_block_length)
		goto err_read_block;
	if (cper_estatus_check_header(ghes->estatus))
		goto err_read_block;
	ghes_copy_tofrom_phys(ghes->estatus + 1,
			      buf_paddr + sizeof(*ghes->estatus),
			      len - sizeof(*ghes->estatus), 1);
	if (cper_estatus_check(ghes->estatus))
		goto err_read_block;
	rc = 0;

err_read_block:
	if (rc && !silent && printk_ratelimit())
		pr_warning(FW_WARN GHES_PFX
			   "Failed to read error status block!\n");
	return rc;
}

static void ghes_clear_estatus(struct ghes *ghes)
{
	ghes->estatus->block_status = 0;
	if (!(ghes->flags & GHES_TO_CLEAR))
		return;
	ghes_copy_tofrom_phys(ghes->estatus, ghes->buffer_paddr,
			      sizeof(ghes->estatus->block_status), 0);
	ghes->flags &= ~GHES_TO_CLEAR;
}

static void ghes_handle_memory_failure(struct acpi_hest_generic_data *gdata, int sev)
{
#ifdef CONFIG_ACPI_APEI_MEMORY_FAILURE
	unsigned long pfn;
=======
	if (!*buf_paddr)
		return -ENOENT;

	ghes_copy_tofrom_phys(estatus, *buf_paddr, sizeof(*estatus), 1,
			      fixmap_idx);
	if (!estatus->block_status) {
		*buf_paddr = 0;
		return -ENOENT;
	}

	return 0;
}

static int __ghes_read_estatus(struct acpi_hest_generic_status *estatus,
			       u64 buf_paddr, enum fixed_addresses fixmap_idx,
			       size_t buf_len)
{
	ghes_copy_tofrom_phys(estatus, buf_paddr, buf_len, 1, fixmap_idx);
	if (cper_estatus_check(estatus)) {
		pr_warn_ratelimited(FW_WARN GHES_PFX
				    "Failed to read error status block!\n");
		return -EIO;
	}

	return 0;
}

static int ghes_read_estatus(struct ghes *ghes,
			     struct acpi_hest_generic_status *estatus,
			     u64 *buf_paddr, enum fixed_addresses fixmap_idx)
{
	int rc;

	rc = __ghes_peek_estatus(ghes, estatus, buf_paddr, fixmap_idx);
	if (rc)
		return rc;

	rc = __ghes_check_estatus(ghes, estatus);
	if (rc)
		return rc;

	return __ghes_read_estatus(estatus, *buf_paddr, fixmap_idx,
				   cper_estatus_len(estatus));
}

static void ghes_clear_estatus(struct ghes *ghes,
			       struct acpi_hest_generic_status *estatus,
			       u64 buf_paddr, enum fixed_addresses fixmap_idx)
{
	estatus->block_status = 0;

	if (!buf_paddr)
		return;

	ghes_copy_tofrom_phys(estatus, buf_paddr,
			      sizeof(estatus->block_status), 0,
			      fixmap_idx);

	/*
	 * GHESv2 type HEST entries introduce support for error acknowledgment,
	 * so only acknowledge the error if this support is present.
	 */
	if (is_hest_type_generic_v2(ghes))
		ghes_ack_error(ghes->generic_v2);
}

/*
 * Called as task_work before returning to user-space.
 * Ensure any queued work has been done before we return to the context that
 * triggered the notification.
 */
static void ghes_kick_task_work(struct callback_head *head)
{
	struct acpi_hest_generic_status *estatus;
	struct ghes_estatus_node *estatus_node;
	u32 node_len;

	estatus_node = container_of(head, struct ghes_estatus_node, task_work);
	if (IS_ENABLED(CONFIG_ACPI_APEI_MEMORY_FAILURE))
		memory_failure_queue_kick(estatus_node->task_work_cpu);

	estatus = GHES_ESTATUS_FROM_NODE(estatus_node);
	node_len = GHES_ESTATUS_NODE_LEN(cper_estatus_len(estatus));
	gen_pool_free(ghes_estatus_pool, (unsigned long)estatus_node, node_len);
}

static bool ghes_do_memory_failure(u64 physical_addr, int flags)
{
	unsigned long pfn;

	if (!IS_ENABLED(CONFIG_ACPI_APEI_MEMORY_FAILURE))
		return false;

	pfn = PHYS_PFN(physical_addr);
	if (!pfn_valid(pfn)) {
		pr_warn_ratelimited(FW_WARN GHES_PFX
		"Invalid address in generic error data: %#llx\n",
		physical_addr);
		return false;
	}

	memory_failure_queue(pfn, flags);
	return true;
}

static bool ghes_handle_memory_failure(struct acpi_hest_generic_data *gdata,
				       int sev)
{
>>>>>>> upstream/android-13
	int flags = -1;
	int sec_sev = ghes_severity(gdata->error_severity);
	struct cper_sec_mem_err *mem_err = acpi_hest_get_payload(gdata);

	if (!(mem_err->validation_bits & CPER_MEM_VALID_PA))
<<<<<<< HEAD
		return;

	pfn = mem_err->physical_addr >> PAGE_SHIFT;
	if (!pfn_valid(pfn)) {
		pr_warn_ratelimited(FW_WARN GHES_PFX
		"Invalid address in generic error data: %#llx\n",
		mem_err->physical_addr);
		return;
	}
=======
		return false;
>>>>>>> upstream/android-13

	/* iff following two events can be handled properly by now */
	if (sec_sev == GHES_SEV_CORRECTED &&
	    (gdata->flags & CPER_SEC_ERROR_THRESHOLD_EXCEEDED))
		flags = MF_SOFT_OFFLINE;
	if (sev == GHES_SEV_RECOVERABLE && sec_sev == GHES_SEV_RECOVERABLE)
		flags = 0;

	if (flags != -1)
<<<<<<< HEAD
		memory_failure_queue(pfn, flags);
#endif
=======
		return ghes_do_memory_failure(mem_err->physical_addr, flags);

	return false;
}

static bool ghes_handle_arm_hw_error(struct acpi_hest_generic_data *gdata, int sev)
{
	struct cper_sec_proc_arm *err = acpi_hest_get_payload(gdata);
	bool queued = false;
	int sec_sev, i;
	char *p;

	log_arm_hw_error(err);

	sec_sev = ghes_severity(gdata->error_severity);
	if (sev != GHES_SEV_RECOVERABLE || sec_sev != GHES_SEV_RECOVERABLE)
		return false;

	p = (char *)(err + 1);
	for (i = 0; i < err->err_info_num; i++) {
		struct cper_arm_err_info *err_info = (struct cper_arm_err_info *)p;
		bool is_cache = (err_info->type == CPER_ARM_CACHE_ERROR);
		bool has_pa = (err_info->validation_bits & CPER_ARM_INFO_VALID_PHYSICAL_ADDR);
		const char *error_type = "unknown error";

		/*
		 * The field (err_info->error_info & BIT(26)) is fixed to set to
		 * 1 in some old firmware of HiSilicon Kunpeng920. We assume that
		 * firmware won't mix corrected errors in an uncorrected section,
		 * and don't filter out 'corrected' error here.
		 */
		if (is_cache && has_pa) {
			queued = ghes_do_memory_failure(err_info->physical_fault_addr, 0);
			p += err_info->length;
			continue;
		}

		if (err_info->type < ARRAY_SIZE(cper_proc_error_type_strs))
			error_type = cper_proc_error_type_strs[err_info->type];

		pr_warn_ratelimited(FW_WARN GHES_PFX
				    "Unhandled processor error type: %s\n",
				    error_type);
		p += err_info->length;
	}

	return queued;
>>>>>>> upstream/android-13
}

/*
 * PCIe AER errors need to be sent to the AER driver for reporting and
 * recovery. The GHES severities map to the following AER severities and
 * require the following handling:
 *
 * GHES_SEV_CORRECTABLE -> AER_CORRECTABLE
 *     These need to be reported by the AER driver but no recovery is
 *     necessary.
 * GHES_SEV_RECOVERABLE -> AER_NONFATAL
 * GHES_SEV_RECOVERABLE && CPER_SEC_RESET -> AER_FATAL
 *     These both need to be reported and recovered from by the AER driver.
 * GHES_SEV_PANIC does not make it to this handling since the kernel must
 *     panic.
 */
static void ghes_handle_aer(struct acpi_hest_generic_data *gdata)
{
#ifdef CONFIG_ACPI_APEI_PCIEAER
	struct cper_sec_pcie *pcie_err = acpi_hest_get_payload(gdata);

	if (pcie_err->validation_bits & CPER_PCIE_VALID_DEVICE_ID &&
	    pcie_err->validation_bits & CPER_PCIE_VALID_AER_INFO) {
		unsigned int devfn;
		int aer_severity;

		devfn = PCI_DEVFN(pcie_err->device_id.device,
				  pcie_err->device_id.function);
		aer_severity = cper_severity_to_aer(gdata->error_severity);

		/*
		 * If firmware reset the component to contain
		 * the error, we must reinitialize it before
		 * use, so treat it as a fatal AER error.
		 */
		if (gdata->flags & CPER_SEC_RESET)
			aer_severity = AER_FATAL;

		aer_recover_queue(pcie_err->device_id.segment,
				  pcie_err->device_id.bus,
				  devfn, aer_severity,
				  (struct aer_capability_regs *)
				  pcie_err->aer_info);
	}
#endif
}

<<<<<<< HEAD
static void ghes_do_proc(struct ghes *ghes,
=======
static BLOCKING_NOTIFIER_HEAD(vendor_record_notify_list);

int ghes_register_vendor_record_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&vendor_record_notify_list, nb);
}
EXPORT_SYMBOL_GPL(ghes_register_vendor_record_notifier);

void ghes_unregister_vendor_record_notifier(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&vendor_record_notify_list, nb);
}
EXPORT_SYMBOL_GPL(ghes_unregister_vendor_record_notifier);

static void ghes_vendor_record_work_func(struct work_struct *work)
{
	struct ghes_vendor_record_entry *entry;
	struct acpi_hest_generic_data *gdata;
	u32 len;

	entry = container_of(work, struct ghes_vendor_record_entry, work);
	gdata = GHES_GDATA_FROM_VENDOR_ENTRY(entry);

	blocking_notifier_call_chain(&vendor_record_notify_list,
				     entry->error_severity, gdata);

	len = GHES_VENDOR_ENTRY_LEN(acpi_hest_get_record_size(gdata));
	gen_pool_free(ghes_estatus_pool, (unsigned long)entry, len);
}

static void ghes_defer_non_standard_event(struct acpi_hest_generic_data *gdata,
					  int sev)
{
	struct acpi_hest_generic_data *copied_gdata;
	struct ghes_vendor_record_entry *entry;
	u32 len;

	len = GHES_VENDOR_ENTRY_LEN(acpi_hest_get_record_size(gdata));
	entry = (void *)gen_pool_alloc(ghes_estatus_pool, len);
	if (!entry)
		return;

	copied_gdata = GHES_GDATA_FROM_VENDOR_ENTRY(entry);
	memcpy(copied_gdata, gdata, acpi_hest_get_record_size(gdata));
	entry->error_severity = sev;

	INIT_WORK(&entry->work, ghes_vendor_record_work_func);
	schedule_work(&entry->work);
}

static bool ghes_do_proc(struct ghes *ghes,
>>>>>>> upstream/android-13
			 const struct acpi_hest_generic_status *estatus)
{
	int sev, sec_sev;
	struct acpi_hest_generic_data *gdata;
	guid_t *sec_type;
<<<<<<< HEAD
	guid_t *fru_id = &NULL_UUID_LE;
	char *fru_text = "";
=======
	const guid_t *fru_id = &guid_null;
	char *fru_text = "";
	bool queued = false;
>>>>>>> upstream/android-13

	sev = ghes_severity(estatus->error_severity);
	apei_estatus_for_each_section(estatus, gdata) {
		sec_type = (guid_t *)gdata->section_type;
		sec_sev = ghes_severity(gdata->error_severity);
		if (gdata->validation_bits & CPER_SEC_VALID_FRU_ID)
			fru_id = (guid_t *)gdata->fru_id;

		if (gdata->validation_bits & CPER_SEC_VALID_FRU_TEXT)
			fru_text = gdata->fru_text;

		if (guid_equal(sec_type, &CPER_SEC_PLATFORM_MEM)) {
			struct cper_sec_mem_err *mem_err = acpi_hest_get_payload(gdata);

			ghes_edac_report_mem_error(sev, mem_err);

			arch_apei_report_mem_error(sev, mem_err);
<<<<<<< HEAD
			ghes_handle_memory_failure(gdata, sev);
=======
			queued = ghes_handle_memory_failure(gdata, sev);
>>>>>>> upstream/android-13
		}
		else if (guid_equal(sec_type, &CPER_SEC_PCIE)) {
			ghes_handle_aer(gdata);
		}
		else if (guid_equal(sec_type, &CPER_SEC_PROC_ARM)) {
<<<<<<< HEAD
			struct cper_sec_proc_arm *err = acpi_hest_get_payload(gdata);

			log_arm_hw_error(err);
		} else {
			void *err = acpi_hest_get_payload(gdata);

=======
			queued = ghes_handle_arm_hw_error(gdata, sev);
		} else {
			void *err = acpi_hest_get_payload(gdata);

			ghes_defer_non_standard_event(gdata, sev);
>>>>>>> upstream/android-13
			log_non_standard_event(sec_type, fru_id, fru_text,
					       sec_sev, err,
					       gdata->error_data_length);
		}
	}
<<<<<<< HEAD
=======

	return queued;
>>>>>>> upstream/android-13
}

static void __ghes_print_estatus(const char *pfx,
				 const struct acpi_hest_generic *generic,
				 const struct acpi_hest_generic_status *estatus)
{
	static atomic_t seqno;
	unsigned int curr_seqno;
	char pfx_seq[64];

	if (pfx == NULL) {
		if (ghes_severity(estatus->error_severity) <=
		    GHES_SEV_CORRECTED)
			pfx = KERN_WARNING;
		else
			pfx = KERN_ERR;
	}
	curr_seqno = atomic_inc_return(&seqno);
	snprintf(pfx_seq, sizeof(pfx_seq), "%s{%u}" HW_ERR, pfx, curr_seqno);
	printk("%s""Hardware error from APEI Generic Hardware Error Source: %d\n",
	       pfx_seq, generic->header.source_id);
	cper_estatus_print(pfx_seq, estatus);
}

static int ghes_print_estatus(const char *pfx,
			      const struct acpi_hest_generic *generic,
			      const struct acpi_hest_generic_status *estatus)
{
	/* Not more than 2 messages every 5 seconds */
	static DEFINE_RATELIMIT_STATE(ratelimit_corrected, 5*HZ, 2);
	static DEFINE_RATELIMIT_STATE(ratelimit_uncorrected, 5*HZ, 2);
	struct ratelimit_state *ratelimit;

	if (ghes_severity(estatus->error_severity) <= GHES_SEV_CORRECTED)
		ratelimit = &ratelimit_corrected;
	else
		ratelimit = &ratelimit_uncorrected;
	if (__ratelimit(ratelimit)) {
		__ghes_print_estatus(pfx, generic, estatus);
		return 1;
	}
	return 0;
}

/*
 * GHES error status reporting throttle, to report more kinds of
 * errors, instead of just most frequently occurred errors.
 */
static int ghes_estatus_cached(struct acpi_hest_generic_status *estatus)
{
	u32 len;
	int i, cached = 0;
	unsigned long long now;
	struct ghes_estatus_cache *cache;
	struct acpi_hest_generic_status *cache_estatus;

	len = cper_estatus_len(estatus);
	rcu_read_lock();
	for (i = 0; i < GHES_ESTATUS_CACHES_SIZE; i++) {
		cache = rcu_dereference(ghes_estatus_caches[i]);
		if (cache == NULL)
			continue;
		if (len != cache->estatus_len)
			continue;
		cache_estatus = GHES_ESTATUS_FROM_CACHE(cache);
		if (memcmp(estatus, cache_estatus, len))
			continue;
		atomic_inc(&cache->count);
		now = sched_clock();
		if (now - cache->time_in < GHES_ESTATUS_IN_CACHE_MAX_NSEC)
			cached = 1;
		break;
	}
	rcu_read_unlock();
	return cached;
}

static struct ghes_estatus_cache *ghes_estatus_cache_alloc(
	struct acpi_hest_generic *generic,
	struct acpi_hest_generic_status *estatus)
{
	int alloced;
	u32 len, cache_len;
	struct ghes_estatus_cache *cache;
	struct acpi_hest_generic_status *cache_estatus;

	alloced = atomic_add_return(1, &ghes_estatus_cache_alloced);
	if (alloced > GHES_ESTATUS_CACHE_ALLOCED_MAX) {
		atomic_dec(&ghes_estatus_cache_alloced);
		return NULL;
	}
	len = cper_estatus_len(estatus);
	cache_len = GHES_ESTATUS_CACHE_LEN(len);
	cache = (void *)gen_pool_alloc(ghes_estatus_pool, cache_len);
	if (!cache) {
		atomic_dec(&ghes_estatus_cache_alloced);
		return NULL;
	}
	cache_estatus = GHES_ESTATUS_FROM_CACHE(cache);
	memcpy(cache_estatus, estatus, len);
	cache->estatus_len = len;
	atomic_set(&cache->count, 0);
	cache->generic = generic;
	cache->time_in = sched_clock();
	return cache;
}

static void ghes_estatus_cache_free(struct ghes_estatus_cache *cache)
{
	u32 len;

	len = cper_estatus_len(GHES_ESTATUS_FROM_CACHE(cache));
	len = GHES_ESTATUS_CACHE_LEN(len);
	gen_pool_free(ghes_estatus_pool, (unsigned long)cache, len);
	atomic_dec(&ghes_estatus_cache_alloced);
}

static void ghes_estatus_cache_rcu_free(struct rcu_head *head)
{
	struct ghes_estatus_cache *cache;

	cache = container_of(head, struct ghes_estatus_cache, rcu);
	ghes_estatus_cache_free(cache);
}

static void ghes_estatus_cache_add(
	struct acpi_hest_generic *generic,
	struct acpi_hest_generic_status *estatus)
{
	int i, slot = -1, count;
	unsigned long long now, duration, period, max_period = 0;
	struct ghes_estatus_cache *cache, *slot_cache = NULL, *new_cache;

	new_cache = ghes_estatus_cache_alloc(generic, estatus);
	if (new_cache == NULL)
		return;
	rcu_read_lock();
	now = sched_clock();
	for (i = 0; i < GHES_ESTATUS_CACHES_SIZE; i++) {
		cache = rcu_dereference(ghes_estatus_caches[i]);
		if (cache == NULL) {
			slot = i;
			slot_cache = NULL;
			break;
		}
		duration = now - cache->time_in;
		if (duration >= GHES_ESTATUS_IN_CACHE_MAX_NSEC) {
			slot = i;
			slot_cache = cache;
			break;
		}
		count = atomic_read(&cache->count);
		period = duration;
		do_div(period, (count + 1));
		if (period > max_period) {
			max_period = period;
			slot = i;
			slot_cache = cache;
		}
	}
	/* new_cache must be put into array after its contents are written */
	smp_wmb();
	if (slot != -1 && cmpxchg(ghes_estatus_caches + slot,
				  slot_cache, new_cache) == slot_cache) {
		if (slot_cache)
			call_rcu(&slot_cache->rcu, ghes_estatus_cache_rcu_free);
	} else
		ghes_estatus_cache_free(new_cache);
	rcu_read_unlock();
}

<<<<<<< HEAD
static int ghes_ack_error(struct acpi_hest_generic_v2 *gv2)
{
	int rc;
	u64 val = 0;

	rc = apei_read(&val, &gv2->read_ack_register);
	if (rc)
		return rc;

	val &= gv2->read_ack_preserve << gv2->read_ack_register.bit_offset;
	val |= gv2->read_ack_write    << gv2->read_ack_register.bit_offset;

	return apei_write(val, &gv2->read_ack_register);
}

static void __ghes_panic(struct ghes *ghes)
{
	__ghes_print_estatus(KERN_EMERG, ghes->generic, ghes->estatus);

	ghes_clear_estatus(ghes);
=======
static void __ghes_panic(struct ghes *ghes,
			 struct acpi_hest_generic_status *estatus,
			 u64 buf_paddr, enum fixed_addresses fixmap_idx)
{
	__ghes_print_estatus(KERN_EMERG, ghes->generic, estatus);

	ghes_clear_estatus(ghes, estatus, buf_paddr, fixmap_idx);
>>>>>>> upstream/android-13

	/* reboot to log the error! */
	if (!panic_timeout)
		panic_timeout = ghes_panic_timeout;
	panic("Fatal hardware error!");
}

static int ghes_proc(struct ghes *ghes)
{
<<<<<<< HEAD
	int rc;

	rc = ghes_read_estatus(ghes, 0);
	if (rc)
		goto out;

	if (ghes_severity(ghes->estatus->error_severity) >= GHES_SEV_PANIC) {
		__ghes_panic(ghes);
	}

	if (!ghes_estatus_cached(ghes->estatus)) {
		if (ghes_print_estatus(NULL, ghes->generic, ghes->estatus))
			ghes_estatus_cache_add(ghes->generic, ghes->estatus);
	}
	ghes_do_proc(ghes, ghes->estatus);

out:
	ghes_clear_estatus(ghes);

	if (rc == -ENOENT)
		return rc;

	/*
	 * GHESv2 type HEST entries introduce support for error acknowledgment,
	 * so only acknowledge the error if this support is present.
	 */
	if (is_hest_type_generic_v2(ghes))
		return ghes_ack_error(ghes->generic_v2);
=======
	struct acpi_hest_generic_status *estatus = ghes->estatus;
	u64 buf_paddr;
	int rc;

	rc = ghes_read_estatus(ghes, estatus, &buf_paddr, FIX_APEI_GHES_IRQ);
	if (rc)
		goto out;

	if (ghes_severity(estatus->error_severity) >= GHES_SEV_PANIC)
		__ghes_panic(ghes, estatus, buf_paddr, FIX_APEI_GHES_IRQ);

	if (!ghes_estatus_cached(estatus)) {
		if (ghes_print_estatus(NULL, ghes->generic, estatus))
			ghes_estatus_cache_add(ghes->generic, estatus);
	}
	ghes_do_proc(ghes, estatus);

out:
	ghes_clear_estatus(ghes, estatus, buf_paddr, FIX_APEI_GHES_IRQ);
>>>>>>> upstream/android-13

	return rc;
}

static void ghes_add_timer(struct ghes *ghes)
{
	struct acpi_hest_generic *g = ghes->generic;
	unsigned long expire;

	if (!g->notify.poll_interval) {
<<<<<<< HEAD
		pr_warning(FW_WARN GHES_PFX "Poll interval is 0 for generic hardware error source: %d, disabled.\n",
			   g->header.source_id);
=======
		pr_warn(FW_WARN GHES_PFX "Poll interval is 0 for generic hardware error source: %d, disabled.\n",
			g->header.source_id);
>>>>>>> upstream/android-13
		return;
	}
	expire = jiffies + msecs_to_jiffies(g->notify.poll_interval);
	ghes->timer.expires = round_jiffies_relative(expire);
	add_timer(&ghes->timer);
}

static void ghes_poll_func(struct timer_list *t)
{
	struct ghes *ghes = from_timer(ghes, t, timer);
<<<<<<< HEAD

	ghes_proc(ghes);
=======
	unsigned long flags;

	spin_lock_irqsave(&ghes_notify_lock_irq, flags);
	ghes_proc(ghes);
	spin_unlock_irqrestore(&ghes_notify_lock_irq, flags);
>>>>>>> upstream/android-13
	if (!(ghes->flags & GHES_EXITING))
		ghes_add_timer(ghes);
}

static irqreturn_t ghes_irq_func(int irq, void *data)
{
	struct ghes *ghes = data;
<<<<<<< HEAD
	int rc;

	rc = ghes_proc(ghes);
=======
	unsigned long flags;
	int rc;

	spin_lock_irqsave(&ghes_notify_lock_irq, flags);
	rc = ghes_proc(ghes);
	spin_unlock_irqrestore(&ghes_notify_lock_irq, flags);
>>>>>>> upstream/android-13
	if (rc)
		return IRQ_NONE;

	return IRQ_HANDLED;
}

static int ghes_notify_hed(struct notifier_block *this, unsigned long event,
			   void *data)
{
	struct ghes *ghes;
<<<<<<< HEAD
	int ret = NOTIFY_DONE;

=======
	unsigned long flags;
	int ret = NOTIFY_DONE;

	spin_lock_irqsave(&ghes_notify_lock_irq, flags);
>>>>>>> upstream/android-13
	rcu_read_lock();
	list_for_each_entry_rcu(ghes, &ghes_hed, list) {
		if (!ghes_proc(ghes))
			ret = NOTIFY_OK;
	}
	rcu_read_unlock();
<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&ghes_notify_lock_irq, flags);
>>>>>>> upstream/android-13

	return ret;
}

static struct notifier_block ghes_notifier_hed = {
	.notifier_call = ghes_notify_hed,
};

<<<<<<< HEAD
#ifdef CONFIG_ACPI_APEI_SEA
static LIST_HEAD(ghes_sea);

/*
 * Return 0 only if one of the SEA error sources successfully reported an error
 * record sent from the firmware.
 */
int ghes_notify_sea(void)
{
	struct ghes *ghes;
	int ret = -ENOENT;

	rcu_read_lock();
	list_for_each_entry_rcu(ghes, &ghes_sea, list) {
		if (!ghes_proc(ghes))
			ret = 0;
	}
	rcu_read_unlock();
	return ret;
}

static void ghes_sea_add(struct ghes *ghes)
{
	mutex_lock(&ghes_list_mutex);
	list_add_rcu(&ghes->list, &ghes_sea);
	mutex_unlock(&ghes_list_mutex);
}

static void ghes_sea_remove(struct ghes *ghes)
{
	mutex_lock(&ghes_list_mutex);
	list_del_rcu(&ghes->list);
	mutex_unlock(&ghes_list_mutex);
	synchronize_rcu();
}
#else /* CONFIG_ACPI_APEI_SEA */
static inline void ghes_sea_add(struct ghes *ghes) { }
static inline void ghes_sea_remove(struct ghes *ghes) { }
#endif /* CONFIG_ACPI_APEI_SEA */

#ifdef CONFIG_HAVE_ACPI_APEI_NMI
/*
 * printk is not safe in NMI context.  So in NMI handler, we allocate
 * required memory from lock-less memory allocator
 * (ghes_estatus_pool), save estatus into it, put them into lock-less
 * list (ghes_estatus_llist), then delay printk into IRQ context via
 * irq_work (ghes_proc_irq_work).  ghes_estatus_size_request record
 * required pool size by all NMI error source.
=======
/*
 * Handlers for CPER records may not be NMI safe. For example,
 * memory_failure_queue() takes spinlocks and calls schedule_work_on().
 * In any NMI-like handler, memory from ghes_estatus_pool is used to save
 * estatus, and added to the ghes_estatus_llist. irq_work_queue() causes
 * ghes_proc_in_irq() to run in IRQ context where each estatus in
 * ghes_estatus_llist is processed.
 *
 * Memory from the ghes_estatus_pool is also used with the ghes_estatus_cache
 * to suppress frequent messages.
>>>>>>> upstream/android-13
 */
static struct llist_head ghes_estatus_llist;
static struct irq_work ghes_proc_irq_work;

<<<<<<< HEAD
/*
 * NMI may be triggered on any CPU, so ghes_in_nmi is used for
 * having only one concurrent reader.
 */
static atomic_t ghes_in_nmi = ATOMIC_INIT(0);

static LIST_HEAD(ghes_nmi);

=======
>>>>>>> upstream/android-13
static void ghes_proc_in_irq(struct irq_work *irq_work)
{
	struct llist_node *llnode, *next;
	struct ghes_estatus_node *estatus_node;
	struct acpi_hest_generic *generic;
	struct acpi_hest_generic_status *estatus;
<<<<<<< HEAD
	u32 len, node_len;
=======
	bool task_work_pending;
	u32 len, node_len;
	int ret;
>>>>>>> upstream/android-13

	llnode = llist_del_all(&ghes_estatus_llist);
	/*
	 * Because the time order of estatus in list is reversed,
	 * revert it back to proper order.
	 */
	llnode = llist_reverse_order(llnode);
	while (llnode) {
		next = llnode->next;
		estatus_node = llist_entry(llnode, struct ghes_estatus_node,
					   llnode);
		estatus = GHES_ESTATUS_FROM_NODE(estatus_node);
		len = cper_estatus_len(estatus);
		node_len = GHES_ESTATUS_NODE_LEN(len);
<<<<<<< HEAD
		ghes_do_proc(estatus_node->ghes, estatus);
=======
		task_work_pending = ghes_do_proc(estatus_node->ghes, estatus);
>>>>>>> upstream/android-13
		if (!ghes_estatus_cached(estatus)) {
			generic = estatus_node->generic;
			if (ghes_print_estatus(NULL, generic, estatus))
				ghes_estatus_cache_add(generic, estatus);
		}
<<<<<<< HEAD
		gen_pool_free(ghes_estatus_pool, (unsigned long)estatus_node,
			      node_len);
=======

		if (task_work_pending && current->mm != &init_mm) {
			estatus_node->task_work.func = ghes_kick_task_work;
			estatus_node->task_work_cpu = smp_processor_id();
			ret = task_work_add(current, &estatus_node->task_work,
					    TWA_RESUME);
			if (ret)
				estatus_node->task_work.func = NULL;
		}

		if (!estatus_node->task_work.func)
			gen_pool_free(ghes_estatus_pool,
				      (unsigned long)estatus_node, node_len);

>>>>>>> upstream/android-13
		llnode = next;
	}
}

static void ghes_print_queued_estatus(void)
{
	struct llist_node *llnode;
	struct ghes_estatus_node *estatus_node;
	struct acpi_hest_generic *generic;
	struct acpi_hest_generic_status *estatus;

	llnode = llist_del_all(&ghes_estatus_llist);
	/*
	 * Because the time order of estatus in list is reversed,
	 * revert it back to proper order.
	 */
	llnode = llist_reverse_order(llnode);
	while (llnode) {
		estatus_node = llist_entry(llnode, struct ghes_estatus_node,
					   llnode);
		estatus = GHES_ESTATUS_FROM_NODE(estatus_node);
		generic = estatus_node->generic;
		ghes_print_estatus(NULL, generic, estatus);
		llnode = llnode->next;
	}
}

<<<<<<< HEAD
/* Save estatus for further processing in IRQ context */
static void __process_error(struct ghes *ghes)
{
#ifdef CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG
	u32 len, node_len;
	struct ghes_estatus_node *estatus_node;
	struct acpi_hest_generic_status *estatus;

	if (ghes_estatus_cached(ghes->estatus))
		return;

	len = cper_estatus_len(ghes->estatus);
	node_len = GHES_ESTATUS_NODE_LEN(len);

	estatus_node = (void *)gen_pool_alloc(ghes_estatus_pool, node_len);
	if (!estatus_node)
		return;

	estatus_node->ghes = ghes;
	estatus_node->generic = ghes->generic;
	estatus = GHES_ESTATUS_FROM_NODE(estatus_node);
	memcpy(estatus, ghes->estatus, len);
	llist_add(&estatus_node->llnode, &ghes_estatus_llist);
#endif
}

static int ghes_notify_nmi(unsigned int cmd, struct pt_regs *regs)
{
	struct ghes *ghes;
	int sev, ret = NMI_DONE;
=======
static int ghes_in_nmi_queue_one_entry(struct ghes *ghes,
				       enum fixed_addresses fixmap_idx)
{
	struct acpi_hest_generic_status *estatus, tmp_header;
	struct ghes_estatus_node *estatus_node;
	u32 len, node_len;
	u64 buf_paddr;
	int sev, rc;

	if (!IS_ENABLED(CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG))
		return -EOPNOTSUPP;

	rc = __ghes_peek_estatus(ghes, &tmp_header, &buf_paddr, fixmap_idx);
	if (rc) {
		ghes_clear_estatus(ghes, &tmp_header, buf_paddr, fixmap_idx);
		return rc;
	}

	rc = __ghes_check_estatus(ghes, &tmp_header);
	if (rc) {
		ghes_clear_estatus(ghes, &tmp_header, buf_paddr, fixmap_idx);
		return rc;
	}

	len = cper_estatus_len(&tmp_header);
	node_len = GHES_ESTATUS_NODE_LEN(len);
	estatus_node = (void *)gen_pool_alloc(ghes_estatus_pool, node_len);
	if (!estatus_node)
		return -ENOMEM;

	estatus_node->ghes = ghes;
	estatus_node->generic = ghes->generic;
	estatus_node->task_work.func = NULL;
	estatus = GHES_ESTATUS_FROM_NODE(estatus_node);

	if (__ghes_read_estatus(estatus, buf_paddr, fixmap_idx, len)) {
		ghes_clear_estatus(ghes, estatus, buf_paddr, fixmap_idx);
		rc = -ENOENT;
		goto no_work;
	}

	sev = ghes_severity(estatus->error_severity);
	if (sev >= GHES_SEV_PANIC) {
		ghes_print_queued_estatus();
		__ghes_panic(ghes, estatus, buf_paddr, fixmap_idx);
	}

	ghes_clear_estatus(ghes, &tmp_header, buf_paddr, fixmap_idx);

	/* This error has been reported before, don't process it again. */
	if (ghes_estatus_cached(estatus))
		goto no_work;

	llist_add(&estatus_node->llnode, &ghes_estatus_llist);

	return rc;

no_work:
	gen_pool_free(ghes_estatus_pool, (unsigned long)estatus_node,
		      node_len);

	return rc;
}

static int ghes_in_nmi_spool_from_list(struct list_head *rcu_list,
				       enum fixed_addresses fixmap_idx)
{
	int ret = -ENOENT;
	struct ghes *ghes;

	rcu_read_lock();
	list_for_each_entry_rcu(ghes, rcu_list, list) {
		if (!ghes_in_nmi_queue_one_entry(ghes, fixmap_idx))
			ret = 0;
	}
	rcu_read_unlock();

	if (IS_ENABLED(CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG) && !ret)
		irq_work_queue(&ghes_proc_irq_work);

	return ret;
}

#ifdef CONFIG_ACPI_APEI_SEA
static LIST_HEAD(ghes_sea);

/*
 * Return 0 only if one of the SEA error sources successfully reported an error
 * record sent from the firmware.
 */
int ghes_notify_sea(void)
{
	static DEFINE_RAW_SPINLOCK(ghes_notify_lock_sea);
	int rv;

	raw_spin_lock(&ghes_notify_lock_sea);
	rv = ghes_in_nmi_spool_from_list(&ghes_sea, FIX_APEI_GHES_SEA);
	raw_spin_unlock(&ghes_notify_lock_sea);

	return rv;
}

static void ghes_sea_add(struct ghes *ghes)
{
	mutex_lock(&ghes_list_mutex);
	list_add_rcu(&ghes->list, &ghes_sea);
	mutex_unlock(&ghes_list_mutex);
}

static void ghes_sea_remove(struct ghes *ghes)
{
	mutex_lock(&ghes_list_mutex);
	list_del_rcu(&ghes->list);
	mutex_unlock(&ghes_list_mutex);
	synchronize_rcu();
}
#else /* CONFIG_ACPI_APEI_SEA */
static inline void ghes_sea_add(struct ghes *ghes) { }
static inline void ghes_sea_remove(struct ghes *ghes) { }
#endif /* CONFIG_ACPI_APEI_SEA */

#ifdef CONFIG_HAVE_ACPI_APEI_NMI
/*
 * NMI may be triggered on any CPU, so ghes_in_nmi is used for
 * having only one concurrent reader.
 */
static atomic_t ghes_in_nmi = ATOMIC_INIT(0);

static LIST_HEAD(ghes_nmi);

static int ghes_notify_nmi(unsigned int cmd, struct pt_regs *regs)
{
	static DEFINE_RAW_SPINLOCK(ghes_notify_lock_nmi);
	int ret = NMI_DONE;
>>>>>>> upstream/android-13

	if (!atomic_add_unless(&ghes_in_nmi, 1, 1))
		return ret;

<<<<<<< HEAD
	list_for_each_entry_rcu(ghes, &ghes_nmi, list) {
		if (ghes_read_estatus(ghes, 1)) {
			ghes_clear_estatus(ghes);
			continue;
		} else {
			ret = NMI_HANDLED;
		}

		sev = ghes_severity(ghes->estatus->error_severity);
		if (sev >= GHES_SEV_PANIC) {
			ghes_print_queued_estatus();
			__ghes_panic(ghes);
		}

		if (!(ghes->flags & GHES_TO_CLEAR))
			continue;

		__process_error(ghes);
		ghes_clear_estatus(ghes);
	}

#ifdef CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG
	if (ret == NMI_HANDLED)
		irq_work_queue(&ghes_proc_irq_work);
#endif
=======
	raw_spin_lock(&ghes_notify_lock_nmi);
	if (!ghes_in_nmi_spool_from_list(&ghes_nmi, FIX_APEI_GHES_NMI))
		ret = NMI_HANDLED;
	raw_spin_unlock(&ghes_notify_lock_nmi);

>>>>>>> upstream/android-13
	atomic_dec(&ghes_in_nmi);
	return ret;
}

<<<<<<< HEAD
static unsigned long ghes_esource_prealloc_size(
	const struct acpi_hest_generic *generic)
{
	unsigned long block_length, prealloc_records, prealloc_size;

	block_length = min_t(unsigned long, generic->error_block_length,
			     GHES_ESTATUS_MAX_SIZE);
	prealloc_records = max_t(unsigned long,
				 generic->records_to_preallocate, 1);
	prealloc_size = min_t(unsigned long, block_length * prealloc_records,
			      GHES_ESOURCE_PREALLOC_MAX_SIZE);

	return prealloc_size;
}

static void ghes_estatus_pool_shrink(unsigned long len)
{
	ghes_estatus_pool_size_request -= PAGE_ALIGN(len);
}

static void ghes_nmi_add(struct ghes *ghes)
{
	unsigned long len;

	len = ghes_esource_prealloc_size(ghes->generic);
	ghes_estatus_pool_expand(len);
=======
static void ghes_nmi_add(struct ghes *ghes)
{
>>>>>>> upstream/android-13
	mutex_lock(&ghes_list_mutex);
	if (list_empty(&ghes_nmi))
		register_nmi_handler(NMI_LOCAL, ghes_notify_nmi, 0, "ghes");
	list_add_rcu(&ghes->list, &ghes_nmi);
	mutex_unlock(&ghes_list_mutex);
}

static void ghes_nmi_remove(struct ghes *ghes)
{
<<<<<<< HEAD
	unsigned long len;

=======
>>>>>>> upstream/android-13
	mutex_lock(&ghes_list_mutex);
	list_del_rcu(&ghes->list);
	if (list_empty(&ghes_nmi))
		unregister_nmi_handler(NMI_LOCAL, "ghes");
	mutex_unlock(&ghes_list_mutex);
	/*
	 * To synchronize with NMI handler, ghes can only be
	 * freed after NMI handler finishes.
	 */
	synchronize_rcu();
<<<<<<< HEAD
	len = ghes_esource_prealloc_size(ghes->generic);
	ghes_estatus_pool_shrink(len);
}
=======
}
#else /* CONFIG_HAVE_ACPI_APEI_NMI */
static inline void ghes_nmi_add(struct ghes *ghes) { }
static inline void ghes_nmi_remove(struct ghes *ghes) { }
#endif /* CONFIG_HAVE_ACPI_APEI_NMI */
>>>>>>> upstream/android-13

static void ghes_nmi_init_cxt(void)
{
	init_irq_work(&ghes_proc_irq_work, ghes_proc_in_irq);
}
<<<<<<< HEAD
#else /* CONFIG_HAVE_ACPI_APEI_NMI */
static inline void ghes_nmi_add(struct ghes *ghes) { }
static inline void ghes_nmi_remove(struct ghes *ghes) { }
static inline void ghes_nmi_init_cxt(void) { }
#endif /* CONFIG_HAVE_ACPI_APEI_NMI */
=======

static int __ghes_sdei_callback(struct ghes *ghes,
				enum fixed_addresses fixmap_idx)
{
	if (!ghes_in_nmi_queue_one_entry(ghes, fixmap_idx)) {
		irq_work_queue(&ghes_proc_irq_work);

		return 0;
	}

	return -ENOENT;
}

static int ghes_sdei_normal_callback(u32 event_num, struct pt_regs *regs,
				      void *arg)
{
	static DEFINE_RAW_SPINLOCK(ghes_notify_lock_sdei_normal);
	struct ghes *ghes = arg;
	int err;

	raw_spin_lock(&ghes_notify_lock_sdei_normal);
	err = __ghes_sdei_callback(ghes, FIX_APEI_GHES_SDEI_NORMAL);
	raw_spin_unlock(&ghes_notify_lock_sdei_normal);

	return err;
}

static int ghes_sdei_critical_callback(u32 event_num, struct pt_regs *regs,
				       void *arg)
{
	static DEFINE_RAW_SPINLOCK(ghes_notify_lock_sdei_critical);
	struct ghes *ghes = arg;
	int err;

	raw_spin_lock(&ghes_notify_lock_sdei_critical);
	err = __ghes_sdei_callback(ghes, FIX_APEI_GHES_SDEI_CRITICAL);
	raw_spin_unlock(&ghes_notify_lock_sdei_critical);

	return err;
}

static int apei_sdei_register_ghes(struct ghes *ghes)
{
	if (!IS_ENABLED(CONFIG_ARM_SDE_INTERFACE))
		return -EOPNOTSUPP;

	return sdei_register_ghes(ghes, ghes_sdei_normal_callback,
				 ghes_sdei_critical_callback);
}

static int apei_sdei_unregister_ghes(struct ghes *ghes)
{
	if (!IS_ENABLED(CONFIG_ARM_SDE_INTERFACE))
		return -EOPNOTSUPP;

	return sdei_unregister_ghes(ghes);
}
>>>>>>> upstream/android-13

static int ghes_probe(struct platform_device *ghes_dev)
{
	struct acpi_hest_generic *generic;
	struct ghes *ghes = NULL;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> upstream/android-13

	int rc = -EINVAL;

	generic = *(struct acpi_hest_generic **)ghes_dev->dev.platform_data;
	if (!generic->enabled)
		return -ENODEV;

	switch (generic->notify.type) {
	case ACPI_HEST_NOTIFY_POLLED:
	case ACPI_HEST_NOTIFY_EXTERNAL:
	case ACPI_HEST_NOTIFY_SCI:
	case ACPI_HEST_NOTIFY_GSIV:
	case ACPI_HEST_NOTIFY_GPIO:
		break;

	case ACPI_HEST_NOTIFY_SEA:
		if (!IS_ENABLED(CONFIG_ACPI_APEI_SEA)) {
			pr_warn(GHES_PFX "Generic hardware error source: %d notified via SEA is not supported\n",
				generic->header.source_id);
			rc = -ENOTSUPP;
			goto err;
		}
		break;
	case ACPI_HEST_NOTIFY_NMI:
		if (!IS_ENABLED(CONFIG_HAVE_ACPI_APEI_NMI)) {
			pr_warn(GHES_PFX "Generic hardware error source: %d notified via NMI interrupt is not supported!\n",
				generic->header.source_id);
			goto err;
		}
		break;
<<<<<<< HEAD
	case ACPI_HEST_NOTIFY_LOCAL:
		pr_warning(GHES_PFX "Generic hardware error source: %d notified via local interrupt is not supported!\n",
			   generic->header.source_id);
		goto err;
	default:
		pr_warning(FW_WARN GHES_PFX "Unknown notification type: %u for generic hardware error source: %d\n",
			   generic->notify.type, generic->header.source_id);
=======
	case ACPI_HEST_NOTIFY_SOFTWARE_DELEGATED:
		if (!IS_ENABLED(CONFIG_ARM_SDE_INTERFACE)) {
			pr_warn(GHES_PFX "Generic hardware error source: %d notified via SDE Interface is not supported!\n",
				generic->header.source_id);
			goto err;
		}
		break;
	case ACPI_HEST_NOTIFY_LOCAL:
		pr_warn(GHES_PFX "Generic hardware error source: %d notified via local interrupt is not supported!\n",
			generic->header.source_id);
		goto err;
	default:
		pr_warn(FW_WARN GHES_PFX "Unknown notification type: %u for generic hardware error source: %d\n",
			generic->notify.type, generic->header.source_id);
>>>>>>> upstream/android-13
		goto err;
	}

	rc = -EIO;
	if (generic->error_block_length <
	    sizeof(struct acpi_hest_generic_status)) {
<<<<<<< HEAD
		pr_warning(FW_BUG GHES_PFX "Invalid error block length: %u for generic hardware error source: %d\n",
			   generic->error_block_length,
			   generic->header.source_id);
=======
		pr_warn(FW_BUG GHES_PFX "Invalid error block length: %u for generic hardware error source: %d\n",
			generic->error_block_length, generic->header.source_id);
>>>>>>> upstream/android-13
		goto err;
	}
	ghes = ghes_new(generic);
	if (IS_ERR(ghes)) {
		rc = PTR_ERR(ghes);
		ghes = NULL;
		goto err;
	}

	switch (generic->notify.type) {
	case ACPI_HEST_NOTIFY_POLLED:
<<<<<<< HEAD
		timer_setup(&ghes->timer, ghes_poll_func, TIMER_DEFERRABLE);
=======
		timer_setup(&ghes->timer, ghes_poll_func, 0);
>>>>>>> upstream/android-13
		ghes_add_timer(ghes);
		break;
	case ACPI_HEST_NOTIFY_EXTERNAL:
		/* External interrupt vector is GSI */
		rc = acpi_gsi_to_irq(generic->notify.vector, &ghes->irq);
		if (rc) {
			pr_err(GHES_PFX "Failed to map GSI to IRQ for generic hardware error source: %d\n",
			       generic->header.source_id);
			goto err;
		}
		rc = request_irq(ghes->irq, ghes_irq_func, IRQF_SHARED,
				 "GHES IRQ", ghes);
		if (rc) {
			pr_err(GHES_PFX "Failed to register IRQ for generic hardware error source: %d\n",
			       generic->header.source_id);
			goto err;
		}
		break;

	case ACPI_HEST_NOTIFY_SCI:
	case ACPI_HEST_NOTIFY_GSIV:
	case ACPI_HEST_NOTIFY_GPIO:
		mutex_lock(&ghes_list_mutex);
		if (list_empty(&ghes_hed))
			register_acpi_hed_notifier(&ghes_notifier_hed);
		list_add_rcu(&ghes->list, &ghes_hed);
		mutex_unlock(&ghes_list_mutex);
		break;

	case ACPI_HEST_NOTIFY_SEA:
		ghes_sea_add(ghes);
		break;
	case ACPI_HEST_NOTIFY_NMI:
		ghes_nmi_add(ghes);
		break;
<<<<<<< HEAD
=======
	case ACPI_HEST_NOTIFY_SOFTWARE_DELEGATED:
		rc = apei_sdei_register_ghes(ghes);
		if (rc)
			goto err;
		break;
>>>>>>> upstream/android-13
	default:
		BUG();
	}

	platform_set_drvdata(ghes_dev, ghes);

	ghes_edac_register(ghes, &ghes_dev->dev);

	/* Handle any pending errors right away */
<<<<<<< HEAD
	ghes_proc(ghes);
=======
	spin_lock_irqsave(&ghes_notify_lock_irq, flags);
	ghes_proc(ghes);
	spin_unlock_irqrestore(&ghes_notify_lock_irq, flags);
>>>>>>> upstream/android-13

	return 0;

err:
	if (ghes) {
		ghes_fini(ghes);
		kfree(ghes);
	}
	return rc;
}

static int ghes_remove(struct platform_device *ghes_dev)
{
<<<<<<< HEAD
=======
	int rc;
>>>>>>> upstream/android-13
	struct ghes *ghes;
	struct acpi_hest_generic *generic;

	ghes = platform_get_drvdata(ghes_dev);
	generic = ghes->generic;

	ghes->flags |= GHES_EXITING;
	switch (generic->notify.type) {
	case ACPI_HEST_NOTIFY_POLLED:
		del_timer_sync(&ghes->timer);
		break;
	case ACPI_HEST_NOTIFY_EXTERNAL:
		free_irq(ghes->irq, ghes);
		break;

	case ACPI_HEST_NOTIFY_SCI:
	case ACPI_HEST_NOTIFY_GSIV:
	case ACPI_HEST_NOTIFY_GPIO:
		mutex_lock(&ghes_list_mutex);
		list_del_rcu(&ghes->list);
		if (list_empty(&ghes_hed))
			unregister_acpi_hed_notifier(&ghes_notifier_hed);
		mutex_unlock(&ghes_list_mutex);
		synchronize_rcu();
		break;

	case ACPI_HEST_NOTIFY_SEA:
		ghes_sea_remove(ghes);
		break;
	case ACPI_HEST_NOTIFY_NMI:
		ghes_nmi_remove(ghes);
		break;
<<<<<<< HEAD
=======
	case ACPI_HEST_NOTIFY_SOFTWARE_DELEGATED:
		rc = apei_sdei_unregister_ghes(ghes);
		if (rc)
			return rc;
		break;
>>>>>>> upstream/android-13
	default:
		BUG();
		break;
	}

	ghes_fini(ghes);

	ghes_edac_unregister(ghes);

	kfree(ghes);

	platform_set_drvdata(ghes_dev, NULL);

	return 0;
}

static struct platform_driver ghes_platform_driver = {
	.driver		= {
		.name	= "GHES",
	},
	.probe		= ghes_probe,
	.remove		= ghes_remove,
};

static int __init ghes_init(void)
{
	int rc;

	if (acpi_disabled)
		return -ENODEV;

	switch (hest_disable) {
	case HEST_NOT_FOUND:
		return -ENODEV;
	case HEST_DISABLED:
		pr_info(GHES_PFX "HEST is not enabled!\n");
		return -EINVAL;
	default:
		break;
	}

	if (ghes_disable) {
		pr_info(GHES_PFX "GHES is not enabled!\n");
		return -EINVAL;
	}

	ghes_nmi_init_cxt();

<<<<<<< HEAD
	rc = ghes_estatus_pool_init();
	if (rc)
		goto err;

	rc = ghes_estatus_pool_expand(GHES_ESTATUS_CACHE_AVG_SIZE *
				      GHES_ESTATUS_CACHE_ALLOCED_MAX);
	if (rc)
		goto err_pool_exit;

	rc = platform_driver_register(&ghes_platform_driver);
	if (rc)
		goto err_pool_exit;
=======
	rc = platform_driver_register(&ghes_platform_driver);
	if (rc)
		goto err;
>>>>>>> upstream/android-13

	rc = apei_osc_setup();
	if (rc == 0 && osc_sb_apei_support_acked)
		pr_info(GHES_PFX "APEI firmware first mode is enabled by APEI bit and WHEA _OSC.\n");
	else if (rc == 0 && !osc_sb_apei_support_acked)
		pr_info(GHES_PFX "APEI firmware first mode is enabled by WHEA _OSC.\n");
	else if (rc && osc_sb_apei_support_acked)
		pr_info(GHES_PFX "APEI firmware first mode is enabled by APEI bit.\n");
	else
		pr_info(GHES_PFX "Failed to enable APEI firmware first mode.\n");

	return 0;
<<<<<<< HEAD
err_pool_exit:
	ghes_estatus_pool_exit();
=======
>>>>>>> upstream/android-13
err:
	return rc;
}
device_initcall(ghes_init);
