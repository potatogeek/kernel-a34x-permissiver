// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright IBM Corp. 2012
 *
 * Author(s):
 *   Jan Glauber <jang@linux.vnet.ibm.com>
 *
 * The System z PCI code is a rewrite from a prototype by
 * the following people (Kudoz!):
 *   Alexander Schmidt
 *   Christoph Raisch
 *   Hannes Hering
 *   Hoang-Nam Nguyen
 *   Jan-Bernd Themann
 *   Stefan Roscher
 *   Thomas Klein
 */

#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/export.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/irq.h>
#include <linux/kernel_stat.h>
#include <linux/seq_file.h>
#include <linux/pci.h>
#include <linux/msi.h>
=======
#include <linux/seq_file.h>
#include <linux/jump_label.h>
#include <linux/pci.h>
#include <linux/printk.h>
>>>>>>> upstream/android-13

#include <asm/isc.h>
#include <asm/airq.h>
#include <asm/facility.h>
#include <asm/pci_insn.h>
#include <asm/pci_clp.h>
#include <asm/pci_dma.h>

<<<<<<< HEAD
#define DEBUG				/* enable pr_debug */

#define	SIC_IRQ_MODE_ALL		0
#define	SIC_IRQ_MODE_SINGLE		1

#define ZPCI_NR_DMA_SPACES		1
#define ZPCI_NR_DEVICES			CONFIG_PCI_NR_FUNCTIONS
=======
#include "pci_bus.h"
#include "pci_iov.h"
>>>>>>> upstream/android-13

/* list of all detected zpci devices */
static LIST_HEAD(zpci_list);
static DEFINE_SPINLOCK(zpci_list_lock);

<<<<<<< HEAD
static struct irq_chip zpci_irq_chip = {
	.name = "zPCI",
	.irq_unmask = pci_msi_unmask_irq,
	.irq_mask = pci_msi_mask_irq,
};

static DECLARE_BITMAP(zpci_domain, ZPCI_NR_DEVICES);
static DEFINE_SPINLOCK(zpci_domain_lock);

static struct airq_iv *zpci_aisb_iv;
static struct airq_iv *zpci_aibv[ZPCI_NR_DEVICES];

#define ZPCI_IOMAP_ENTRIES						\
	min(((unsigned long) ZPCI_NR_DEVICES * PCI_BAR_COUNT / 2),	\
	    ZPCI_IOMAP_MAX_ENTRIES)

=======
static DECLARE_BITMAP(zpci_domain, ZPCI_DOMAIN_BITMAP_SIZE);
static DEFINE_SPINLOCK(zpci_domain_lock);

#define ZPCI_IOMAP_ENTRIES						\
	min(((unsigned long) ZPCI_NR_DEVICES * PCI_STD_NUM_BARS / 2),	\
	    ZPCI_IOMAP_MAX_ENTRIES)

unsigned int s390_pci_no_rid;

>>>>>>> upstream/android-13
static DEFINE_SPINLOCK(zpci_iomap_lock);
static unsigned long *zpci_iomap_bitmap;
struct zpci_iomap_entry *zpci_iomap_start;
EXPORT_SYMBOL_GPL(zpci_iomap_start);

<<<<<<< HEAD
=======
DEFINE_STATIC_KEY_FALSE(have_mio);

>>>>>>> upstream/android-13
static struct kmem_cache *zdev_fmb_cache;

struct zpci_dev *get_zdev_by_fid(u32 fid)
{
	struct zpci_dev *tmp, *zdev = NULL;

	spin_lock(&zpci_list_lock);
	list_for_each_entry(tmp, &zpci_list, entry) {
		if (tmp->fid == fid) {
			zdev = tmp;
			break;
		}
	}
	spin_unlock(&zpci_list_lock);
	return zdev;
}

void zpci_remove_reserved_devices(void)
{
	struct zpci_dev *tmp, *zdev;
	enum zpci_state state;
	LIST_HEAD(remove);

	spin_lock(&zpci_list_lock);
	list_for_each_entry_safe(zdev, tmp, &zpci_list, entry) {
		if (zdev->state == ZPCI_FN_STATE_STANDBY &&
		    !clp_get_state(zdev->fid, &state) &&
		    state == ZPCI_FN_STATE_RESERVED)
			list_move_tail(&zdev->entry, &remove);
	}
	spin_unlock(&zpci_list_lock);

	list_for_each_entry_safe(zdev, tmp, &remove, entry)
<<<<<<< HEAD
		zpci_remove_device(zdev);
}

static struct zpci_dev *get_zdev_by_bus(struct pci_bus *bus)
{
	return (bus && bus->sysdata) ? (struct zpci_dev *) bus->sysdata : NULL;
=======
		zpci_device_reserved(zdev);
>>>>>>> upstream/android-13
}

int pci_domain_nr(struct pci_bus *bus)
{
<<<<<<< HEAD
	return ((struct zpci_dev *) bus->sysdata)->domain;
=======
	return ((struct zpci_bus *) bus->sysdata)->domain_nr;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(pci_domain_nr);

int pci_proc_domain(struct pci_bus *bus)
{
	return pci_domain_nr(bus);
}
EXPORT_SYMBOL_GPL(pci_proc_domain);

<<<<<<< HEAD
/* Modify PCI: Register adapter interruptions */
static int zpci_set_airq(struct zpci_dev *zdev)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, 0, ZPCI_MOD_FC_REG_INT);
	struct zpci_fib fib = {0};
	u8 status;

	fib.isc = PCI_ISC;
	fib.sum = 1;		/* enable summary notifications */
	fib.noi = airq_iv_end(zdev->aibv);
	fib.aibv = (unsigned long) zdev->aibv->vector;
	fib.aibvo = 0;		/* each zdev has its own interrupt vector */
	fib.aisb = (unsigned long) zpci_aisb_iv->vector + (zdev->aisb/64)*8;
	fib.aisbo = zdev->aisb & 63;

	return zpci_mod_fc(req, &fib, &status) ? -EIO : 0;
}

/* Modify PCI: Unregister adapter interruptions */
static int zpci_clear_airq(struct zpci_dev *zdev)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, 0, ZPCI_MOD_FC_DEREG_INT);
	struct zpci_fib fib = {0};
	u8 cc, status;

	cc = zpci_mod_fc(req, &fib, &status);
	if (cc == 3 || (cc == 1 && status == 24))
		/* Function already gone or IRQs already deregistered. */
		cc = 0;

	return cc ? -EIO : 0;
}

=======
>>>>>>> upstream/android-13
/* Modify PCI: Register I/O address translation parameters */
int zpci_register_ioat(struct zpci_dev *zdev, u8 dmaas,
		       u64 base, u64 limit, u64 iota)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, dmaas, ZPCI_MOD_FC_REG_IOAT);
	struct zpci_fib fib = {0};
<<<<<<< HEAD
	u8 status;
=======
	u8 cc, status;
>>>>>>> upstream/android-13

	WARN_ON_ONCE(iota & 0x3fff);
	fib.pba = base;
	fib.pal = limit;
	fib.iota = iota | ZPCI_IOTA_RTTO_FLAG;
<<<<<<< HEAD
	return zpci_mod_fc(req, &fib, &status) ? -EIO : 0;
=======
	cc = zpci_mod_fc(req, &fib, &status);
	if (cc)
		zpci_dbg(3, "reg ioat fid:%x, cc:%d, status:%d\n", zdev->fid, cc, status);
	return cc;
>>>>>>> upstream/android-13
}

/* Modify PCI: Unregister I/O address translation parameters */
int zpci_unregister_ioat(struct zpci_dev *zdev, u8 dmaas)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, dmaas, ZPCI_MOD_FC_DEREG_IOAT);
	struct zpci_fib fib = {0};
	u8 cc, status;

	cc = zpci_mod_fc(req, &fib, &status);
<<<<<<< HEAD
	if (cc == 3) /* Function already gone. */
		cc = 0;
	return cc ? -EIO : 0;
=======
	if (cc)
		zpci_dbg(3, "unreg ioat fid:%x, cc:%d, status:%d\n", zdev->fid, cc, status);
	return cc;
>>>>>>> upstream/android-13
}

/* Modify PCI: Set PCI function measurement parameters */
int zpci_fmb_enable_device(struct zpci_dev *zdev)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, 0, ZPCI_MOD_FC_SET_MEASURE);
	struct zpci_fib fib = {0};
	u8 cc, status;

	if (zdev->fmb || sizeof(*zdev->fmb) < zdev->fmb_length)
		return -EINVAL;

	zdev->fmb = kmem_cache_zalloc(zdev_fmb_cache, GFP_KERNEL);
	if (!zdev->fmb)
		return -ENOMEM;
	WARN_ON((u64) zdev->fmb & 0xf);

	/* reset software counters */
	atomic64_set(&zdev->allocated_pages, 0);
	atomic64_set(&zdev->mapped_pages, 0);
	atomic64_set(&zdev->unmapped_pages, 0);

	fib.fmb_addr = virt_to_phys(zdev->fmb);
	cc = zpci_mod_fc(req, &fib, &status);
	if (cc) {
		kmem_cache_free(zdev_fmb_cache, zdev->fmb);
		zdev->fmb = NULL;
	}
	return cc ? -EIO : 0;
}

/* Modify PCI: Disable PCI function measurement */
int zpci_fmb_disable_device(struct zpci_dev *zdev)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, 0, ZPCI_MOD_FC_SET_MEASURE);
	struct zpci_fib fib = {0};
	u8 cc, status;

	if (!zdev->fmb)
		return -EINVAL;

	/* Function measurement is disabled if fmb address is zero */
	cc = zpci_mod_fc(req, &fib, &status);
	if (cc == 3) /* Function already gone. */
		cc = 0;

	if (!cc) {
		kmem_cache_free(zdev_fmb_cache, zdev->fmb);
		zdev->fmb = NULL;
	}
	return cc ? -EIO : 0;
}

static int zpci_cfg_load(struct zpci_dev *zdev, int offset, u32 *val, u8 len)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, ZPCI_PCIAS_CFGSPC, len);
	u64 data;
	int rc;

<<<<<<< HEAD
	rc = zpci_load(&data, req, offset);
=======
	rc = __zpci_load(&data, req, offset);
>>>>>>> upstream/android-13
	if (!rc) {
		data = le64_to_cpu((__force __le64) data);
		data >>= (8 - len) * 8;
		*val = (u32) data;
	} else
		*val = 0xffffffff;
	return rc;
}

static int zpci_cfg_store(struct zpci_dev *zdev, int offset, u32 val, u8 len)
{
	u64 req = ZPCI_CREATE_REQ(zdev->fh, ZPCI_PCIAS_CFGSPC, len);
	u64 data = val;
	int rc;

	data <<= (8 - len) * 8;
	data = (__force u64) cpu_to_le64(data);
<<<<<<< HEAD
	rc = zpci_store(data, req, offset);
=======
	rc = __zpci_store(data, req, offset);
>>>>>>> upstream/android-13
	return rc;
}

resource_size_t pcibios_align_resource(void *data, const struct resource *res,
				       resource_size_t size,
				       resource_size_t align)
{
	return 0;
}

/* combine single writes by using store-block insn */
void __iowrite64_copy(void __iomem *to, const void *from, size_t count)
{
       zpci_memcpy_toio(to, from, count);
}

<<<<<<< HEAD
/* Create a virtual mapping cookie for a PCI BAR */
void __iomem *pci_iomap_range(struct pci_dev *pdev,
			      int bar,
			      unsigned long offset,
			      unsigned long max)
=======
static void __iomem *__ioremap(phys_addr_t addr, size_t size, pgprot_t prot)
{
	unsigned long offset, vaddr;
	struct vm_struct *area;
	phys_addr_t last_addr;

	last_addr = addr + size - 1;
	if (!size || last_addr < addr)
		return NULL;

	if (!static_branch_unlikely(&have_mio))
		return (void __iomem *) addr;

	offset = addr & ~PAGE_MASK;
	addr &= PAGE_MASK;
	size = PAGE_ALIGN(size + offset);
	area = get_vm_area(size, VM_IOREMAP);
	if (!area)
		return NULL;

	vaddr = (unsigned long) area->addr;
	if (ioremap_page_range(vaddr, vaddr + size, addr, prot)) {
		free_vm_area(area);
		return NULL;
	}
	return (void __iomem *) ((unsigned long) area->addr + offset);
}

void __iomem *ioremap_prot(phys_addr_t addr, size_t size, unsigned long prot)
{
	return __ioremap(addr, size, __pgprot(prot));
}
EXPORT_SYMBOL(ioremap_prot);

void __iomem *ioremap(phys_addr_t addr, size_t size)
{
	return __ioremap(addr, size, PAGE_KERNEL);
}
EXPORT_SYMBOL(ioremap);

void __iomem *ioremap_wc(phys_addr_t addr, size_t size)
{
	return __ioremap(addr, size, pgprot_writecombine(PAGE_KERNEL));
}
EXPORT_SYMBOL(ioremap_wc);

void __iomem *ioremap_wt(phys_addr_t addr, size_t size)
{
	return __ioremap(addr, size, pgprot_writethrough(PAGE_KERNEL));
}
EXPORT_SYMBOL(ioremap_wt);

void iounmap(volatile void __iomem *addr)
{
	if (static_branch_likely(&have_mio))
		vunmap((__force void *) ((unsigned long) addr & PAGE_MASK));
}
EXPORT_SYMBOL(iounmap);

/* Create a virtual mapping cookie for a PCI BAR */
static void __iomem *pci_iomap_range_fh(struct pci_dev *pdev, int bar,
					unsigned long offset, unsigned long max)
>>>>>>> upstream/android-13
{
	struct zpci_dev *zdev =	to_zpci(pdev);
	int idx;

<<<<<<< HEAD
	if (!pci_resource_len(pdev, bar))
		return NULL;

=======
>>>>>>> upstream/android-13
	idx = zdev->bars[bar].map_idx;
	spin_lock(&zpci_iomap_lock);
	/* Detect overrun */
	WARN_ON(!++zpci_iomap_start[idx].count);
	zpci_iomap_start[idx].fh = zdev->fh;
	zpci_iomap_start[idx].bar = bar;
	spin_unlock(&zpci_iomap_lock);

	return (void __iomem *) ZPCI_ADDR(idx) + offset;
}
<<<<<<< HEAD
=======

static void __iomem *pci_iomap_range_mio(struct pci_dev *pdev, int bar,
					 unsigned long offset,
					 unsigned long max)
{
	unsigned long barsize = pci_resource_len(pdev, bar);
	struct zpci_dev *zdev = to_zpci(pdev);
	void __iomem *iova;

	iova = ioremap((unsigned long) zdev->bars[bar].mio_wt, barsize);
	return iova ? iova + offset : iova;
}

void __iomem *pci_iomap_range(struct pci_dev *pdev, int bar,
			      unsigned long offset, unsigned long max)
{
	if (bar >= PCI_STD_NUM_BARS || !pci_resource_len(pdev, bar))
		return NULL;

	if (static_branch_likely(&have_mio))
		return pci_iomap_range_mio(pdev, bar, offset, max);
	else
		return pci_iomap_range_fh(pdev, bar, offset, max);
}
>>>>>>> upstream/android-13
EXPORT_SYMBOL(pci_iomap_range);

void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long maxlen)
{
	return pci_iomap_range(dev, bar, 0, maxlen);
}
EXPORT_SYMBOL(pci_iomap);

<<<<<<< HEAD
void pci_iounmap(struct pci_dev *pdev, void __iomem *addr)
=======
static void __iomem *pci_iomap_wc_range_mio(struct pci_dev *pdev, int bar,
					    unsigned long offset, unsigned long max)
{
	unsigned long barsize = pci_resource_len(pdev, bar);
	struct zpci_dev *zdev = to_zpci(pdev);
	void __iomem *iova;

	iova = ioremap((unsigned long) zdev->bars[bar].mio_wb, barsize);
	return iova ? iova + offset : iova;
}

void __iomem *pci_iomap_wc_range(struct pci_dev *pdev, int bar,
				 unsigned long offset, unsigned long max)
{
	if (bar >= PCI_STD_NUM_BARS || !pci_resource_len(pdev, bar))
		return NULL;

	if (static_branch_likely(&have_mio))
		return pci_iomap_wc_range_mio(pdev, bar, offset, max);
	else
		return pci_iomap_range_fh(pdev, bar, offset, max);
}
EXPORT_SYMBOL(pci_iomap_wc_range);

void __iomem *pci_iomap_wc(struct pci_dev *dev, int bar, unsigned long maxlen)
{
	return pci_iomap_wc_range(dev, bar, 0, maxlen);
}
EXPORT_SYMBOL(pci_iomap_wc);

static void pci_iounmap_fh(struct pci_dev *pdev, void __iomem *addr)
>>>>>>> upstream/android-13
{
	unsigned int idx = ZPCI_IDX(addr);

	spin_lock(&zpci_iomap_lock);
	/* Detect underrun */
	WARN_ON(!zpci_iomap_start[idx].count);
	if (!--zpci_iomap_start[idx].count) {
		zpci_iomap_start[idx].fh = 0;
		zpci_iomap_start[idx].bar = 0;
	}
	spin_unlock(&zpci_iomap_lock);
}
<<<<<<< HEAD
=======

static void pci_iounmap_mio(struct pci_dev *pdev, void __iomem *addr)
{
	iounmap(addr);
}

void pci_iounmap(struct pci_dev *pdev, void __iomem *addr)
{
	if (static_branch_likely(&have_mio))
		pci_iounmap_mio(pdev, addr);
	else
		pci_iounmap_fh(pdev, addr);
}
>>>>>>> upstream/android-13
EXPORT_SYMBOL(pci_iounmap);

static int pci_read(struct pci_bus *bus, unsigned int devfn, int where,
		    int size, u32 *val)
{
<<<<<<< HEAD
	struct zpci_dev *zdev = get_zdev_by_bus(bus);
	int ret;

	if (!zdev || devfn != ZPCI_DEVFN)
		ret = -ENODEV;
	else
		ret = zpci_cfg_load(zdev, where, val, size);

	return ret;
=======
	struct zpci_dev *zdev = get_zdev_by_bus(bus, devfn);

	return (zdev) ? zpci_cfg_load(zdev, where, val, size) : -ENODEV;
>>>>>>> upstream/android-13
}

static int pci_write(struct pci_bus *bus, unsigned int devfn, int where,
		     int size, u32 val)
{
<<<<<<< HEAD
	struct zpci_dev *zdev = get_zdev_by_bus(bus);
	int ret;

	if (!zdev || devfn != ZPCI_DEVFN)
		ret = -ENODEV;
	else
		ret = zpci_cfg_store(zdev, where, val, size);

	return ret;
=======
	struct zpci_dev *zdev = get_zdev_by_bus(bus, devfn);

	return (zdev) ? zpci_cfg_store(zdev, where, val, size) : -ENODEV;
>>>>>>> upstream/android-13
}

static struct pci_ops pci_root_ops = {
	.read = pci_read,
	.write = pci_write,
};

<<<<<<< HEAD
static void zpci_irq_handler(struct airq_struct *airq)
{
	unsigned long si, ai;
	struct airq_iv *aibv;
	int irqs_on = 0;

	inc_irq_stat(IRQIO_PCI);
	for (si = 0;;) {
		/* Scan adapter summary indicator bit vector */
		si = airq_iv_scan(zpci_aisb_iv, si, airq_iv_end(zpci_aisb_iv));
		if (si == -1UL) {
			if (irqs_on++)
				/* End of second scan with interrupts on. */
				break;
			/* First scan complete, reenable interrupts. */
			if (zpci_set_irq_ctrl(SIC_IRQ_MODE_SINGLE, NULL, PCI_ISC))
				break;
			si = 0;
			continue;
		}

		/* Scan the adapter interrupt vector for this device. */
		aibv = zpci_aibv[si];
		for (ai = 0;;) {
			ai = airq_iv_scan(aibv, ai, airq_iv_end(aibv));
			if (ai == -1UL)
				break;
			inc_irq_stat(IRQIO_MSI);
			airq_iv_lock(aibv, ai);
			generic_handle_irq(airq_iv_get_data(aibv, ai));
			airq_iv_unlock(aibv, ai);
		}
	}
}

int arch_setup_msi_irqs(struct pci_dev *pdev, int nvec, int type)
{
	struct zpci_dev *zdev = to_zpci(pdev);
	unsigned int hwirq, msi_vecs;
	unsigned long aisb;
	struct msi_desc *msi;
	struct msi_msg msg;
	int rc, irq;

	zdev->aisb = -1UL;
	if (type == PCI_CAP_ID_MSI && nvec > 1)
		return 1;
	msi_vecs = min_t(unsigned int, nvec, zdev->max_msi);

	/* Allocate adapter summary indicator bit */
	aisb = airq_iv_alloc_bit(zpci_aisb_iv);
	if (aisb == -1UL)
		return -EIO;
	zdev->aisb = aisb;

	/* Create adapter interrupt vector */
	zdev->aibv = airq_iv_create(msi_vecs, AIRQ_IV_DATA | AIRQ_IV_BITLOCK);
	if (!zdev->aibv)
		return -ENOMEM;

	/* Wire up shortcut pointer */
	zpci_aibv[aisb] = zdev->aibv;

	/* Request MSI interrupts */
	hwirq = 0;
	for_each_pci_msi_entry(msi, pdev) {
		if (hwirq >= msi_vecs)
			break;
		irq = irq_alloc_desc(0);	/* Alloc irq on node 0 */
		if (irq < 0)
			return -ENOMEM;
		rc = irq_set_msi_desc(irq, msi);
		if (rc)
			return rc;
		irq_set_chip_and_handler(irq, &zpci_irq_chip,
					 handle_simple_irq);
		msg.data = hwirq;
		msg.address_lo = zdev->msi_addr & 0xffffffff;
		msg.address_hi = zdev->msi_addr >> 32;
		pci_write_msi_msg(irq, &msg);
		airq_iv_set_data(zdev->aibv, hwirq, irq);
		hwirq++;
	}

	/* Enable adapter interrupts */
	rc = zpci_set_airq(zdev);
	if (rc)
		return rc;

	return (msi_vecs == nvec) ? 0 : msi_vecs;
}

void arch_teardown_msi_irqs(struct pci_dev *pdev)
{
	struct zpci_dev *zdev = to_zpci(pdev);
	struct msi_desc *msi;
	int rc;

	/* Disable adapter interrupts */
	rc = zpci_clear_airq(zdev);
	if (rc)
		return;

	/* Release MSI interrupts */
	for_each_pci_msi_entry(msi, pdev) {
		if (!msi->irq)
			continue;
		if (msi->msi_attrib.is_msix)
			__pci_msix_desc_mask_irq(msi, 1);
		else
			__pci_msi_desc_mask_irq(msi, 1, 1);
		irq_set_msi_desc(msi->irq, NULL);
		irq_free_desc(msi->irq);
		msi->msg.address_lo = 0;
		msi->msg.address_hi = 0;
		msi->msg.data = 0;
		msi->irq = 0;
	}

	if (zdev->aisb != -1UL) {
		zpci_aibv[zdev->aisb] = NULL;
		airq_iv_free_bit(zpci_aisb_iv, zdev->aisb);
		zdev->aisb = -1UL;
	}
	if (zdev->aibv) {
		airq_iv_release(zdev->aibv);
		zdev->aibv = NULL;
	}
}

static void zpci_map_resources(struct pci_dev *pdev)
{
	resource_size_t len;
	int i;

	for (i = 0; i < PCI_BAR_COUNT; i++) {
		len = pci_resource_len(pdev, i);
		if (!len)
			continue;
		pdev->resource[i].start =
			(resource_size_t __force) pci_iomap(pdev, i, 0);
		pdev->resource[i].end = pdev->resource[i].start + len - 1;
	}
=======
static void zpci_map_resources(struct pci_dev *pdev)
{
	struct zpci_dev *zdev = to_zpci(pdev);
	resource_size_t len;
	int i;

	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
		len = pci_resource_len(pdev, i);
		if (!len)
			continue;

		if (zpci_use_mio(zdev))
			pdev->resource[i].start =
				(resource_size_t __force) zdev->bars[i].mio_wt;
		else
			pdev->resource[i].start = (resource_size_t __force)
				pci_iomap_range_fh(pdev, i, 0, 0);
		pdev->resource[i].end = pdev->resource[i].start + len - 1;
	}

	zpci_iov_map_resources(pdev);
>>>>>>> upstream/android-13
}

static void zpci_unmap_resources(struct pci_dev *pdev)
{
<<<<<<< HEAD
	resource_size_t len;
	int i;

	for (i = 0; i < PCI_BAR_COUNT; i++) {
		len = pci_resource_len(pdev, i);
		if (!len)
			continue;
		pci_iounmap(pdev, (void __iomem __force *)
			    pdev->resource[i].start);
	}
}

static struct airq_struct zpci_airq = {
	.handler = zpci_irq_handler,
	.isc = PCI_ISC,
};

static int __init zpci_irq_init(void)
{
	int rc;

	rc = register_adapter_interrupt(&zpci_airq);
	if (rc)
		goto out;
	/* Set summary to 1 to be called every time for the ISC. */
	*zpci_airq.lsi_ptr = 1;

	rc = -ENOMEM;
	zpci_aisb_iv = airq_iv_create(ZPCI_NR_DEVICES, AIRQ_IV_ALLOC);
	if (!zpci_aisb_iv)
		goto out_airq;

	zpci_set_irq_ctrl(SIC_IRQ_MODE_SINGLE, NULL, PCI_ISC);
	return 0;

out_airq:
	unregister_adapter_interrupt(&zpci_airq);
out:
	return rc;
}

static void zpci_irq_exit(void)
{
	airq_iv_release(zpci_aisb_iv);
	unregister_adapter_interrupt(&zpci_airq);
}

=======
	struct zpci_dev *zdev = to_zpci(pdev);
	resource_size_t len;
	int i;

	if (zpci_use_mio(zdev))
		return;

	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
		len = pci_resource_len(pdev, i);
		if (!len)
			continue;
		pci_iounmap_fh(pdev, (void __iomem __force *)
			       pdev->resource[i].start);
	}
}

>>>>>>> upstream/android-13
static int zpci_alloc_iomap(struct zpci_dev *zdev)
{
	unsigned long entry;

	spin_lock(&zpci_iomap_lock);
	entry = find_first_zero_bit(zpci_iomap_bitmap, ZPCI_IOMAP_ENTRIES);
	if (entry == ZPCI_IOMAP_ENTRIES) {
		spin_unlock(&zpci_iomap_lock);
		return -ENOSPC;
	}
	set_bit(entry, zpci_iomap_bitmap);
	spin_unlock(&zpci_iomap_lock);
	return entry;
}

static void zpci_free_iomap(struct zpci_dev *zdev, int entry)
{
	spin_lock(&zpci_iomap_lock);
	memset(&zpci_iomap_start[entry], 0, sizeof(struct zpci_iomap_entry));
	clear_bit(entry, zpci_iomap_bitmap);
	spin_unlock(&zpci_iomap_lock);
}

static struct resource *__alloc_res(struct zpci_dev *zdev, unsigned long start,
				    unsigned long size, unsigned long flags)
{
	struct resource *r;

	r = kzalloc(sizeof(*r), GFP_KERNEL);
	if (!r)
		return NULL;

	r->start = start;
	r->end = r->start + size - 1;
	r->flags = flags;
	r->name = zdev->res_name;

	if (request_resource(&iomem_resource, r)) {
		kfree(r);
		return NULL;
	}
	return r;
}

<<<<<<< HEAD
static int zpci_setup_bus_resources(struct zpci_dev *zdev,
				    struct list_head *resources)
=======
int zpci_setup_bus_resources(struct zpci_dev *zdev,
			     struct list_head *resources)
>>>>>>> upstream/android-13
{
	unsigned long addr, size, flags;
	struct resource *res;
	int i, entry;

	snprintf(zdev->res_name, sizeof(zdev->res_name),
<<<<<<< HEAD
		 "PCI Bus %04x:%02x", zdev->domain, ZPCI_BUS_NR);

	for (i = 0; i < PCI_BAR_COUNT; i++) {
=======
		 "PCI Bus %04x:%02x", zdev->uid, ZPCI_BUS_NR);

	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
>>>>>>> upstream/android-13
		if (!zdev->bars[i].size)
			continue;
		entry = zpci_alloc_iomap(zdev);
		if (entry < 0)
			return entry;
		zdev->bars[i].map_idx = entry;

		/* only MMIO is supported */
		flags = IORESOURCE_MEM;
		if (zdev->bars[i].val & 8)
			flags |= IORESOURCE_PREFETCH;
		if (zdev->bars[i].val & 4)
			flags |= IORESOURCE_MEM_64;

<<<<<<< HEAD
		addr = ZPCI_ADDR(entry);
=======
		if (zpci_use_mio(zdev))
			addr = (unsigned long) zdev->bars[i].mio_wt;
		else
			addr = ZPCI_ADDR(entry);
>>>>>>> upstream/android-13
		size = 1UL << zdev->bars[i].size;

		res = __alloc_res(zdev, addr, size, flags);
		if (!res) {
			zpci_free_iomap(zdev, entry);
			return -ENOMEM;
		}
		zdev->bars[i].res = res;
		pci_add_resource(resources, res);
	}
<<<<<<< HEAD
=======
	zdev->has_resources = 1;
>>>>>>> upstream/android-13

	return 0;
}

static void zpci_cleanup_bus_resources(struct zpci_dev *zdev)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < PCI_BAR_COUNT; i++) {
=======
	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
>>>>>>> upstream/android-13
		if (!zdev->bars[i].size || !zdev->bars[i].res)
			continue;

		zpci_free_iomap(zdev, zdev->bars[i].map_idx);
		release_resource(zdev->bars[i].res);
		kfree(zdev->bars[i].res);
	}
<<<<<<< HEAD
=======
	zdev->has_resources = 0;
>>>>>>> upstream/android-13
}

int pcibios_add_device(struct pci_dev *pdev)
{
<<<<<<< HEAD
	struct resource *res;
	int i;

=======
	struct zpci_dev *zdev = to_zpci(pdev);
	struct resource *res;
	int i;

	/* The pdev has a reference to the zdev via its bus */
	zpci_zdev_get(zdev);
	if (pdev->is_physfn)
		pdev->no_vf_scan = 1;

>>>>>>> upstream/android-13
	pdev->dev.groups = zpci_attr_groups;
	pdev->dev.dma_ops = &s390_pci_dma_ops;
	zpci_map_resources(pdev);

<<<<<<< HEAD
	for (i = 0; i < PCI_BAR_COUNT; i++) {
=======
	for (i = 0; i < PCI_STD_NUM_BARS; i++) {
>>>>>>> upstream/android-13
		res = &pdev->resource[i];
		if (res->parent || !res->flags)
			continue;
		pci_claim_resource(pdev, i);
	}

	return 0;
}

void pcibios_release_device(struct pci_dev *pdev)
{
<<<<<<< HEAD
	zpci_unmap_resources(pdev);
=======
	struct zpci_dev *zdev = to_zpci(pdev);

	zpci_unmap_resources(pdev);
	zpci_zdev_put(zdev);
>>>>>>> upstream/android-13
}

int pcibios_enable_device(struct pci_dev *pdev, int mask)
{
	struct zpci_dev *zdev = to_zpci(pdev);

	zpci_debug_init_device(zdev, dev_name(&pdev->dev));
	zpci_fmb_enable_device(zdev);

	return pci_enable_resources(pdev, mask);
}

void pcibios_disable_device(struct pci_dev *pdev)
{
	struct zpci_dev *zdev = to_zpci(pdev);

	zpci_fmb_disable_device(zdev);
	zpci_debug_exit_device(zdev);
}

<<<<<<< HEAD
#ifdef CONFIG_HIBERNATE_CALLBACKS
static int zpci_restore(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct zpci_dev *zdev = to_zpci(pdev);
	int ret = 0;

	if (zdev->state != ZPCI_FN_STATE_ONLINE)
		goto out;

	ret = clp_enable_fh(zdev, ZPCI_NR_DMA_SPACES);
	if (ret)
		goto out;

	zpci_map_resources(pdev);
	zpci_register_ioat(zdev, 0, zdev->start_dma, zdev->end_dma,
			   (u64) zdev->dma_table);

out:
	return ret;
}

static int zpci_freeze(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct zpci_dev *zdev = to_zpci(pdev);

	if (zdev->state != ZPCI_FN_STATE_ONLINE)
		return 0;

	zpci_unregister_ioat(zdev, 0);
	zpci_unmap_resources(pdev);
	return clp_disable_fh(zdev);
}

struct dev_pm_ops pcibios_pm_ops = {
	.thaw_noirq = zpci_restore,
	.freeze_noirq = zpci_freeze,
	.restore_noirq = zpci_restore,
	.poweroff_noirq = zpci_freeze,
};
#endif /* CONFIG_HIBERNATE_CALLBACKS */

static int zpci_alloc_domain(struct zpci_dev *zdev)
{
	if (zpci_unique_uid) {
		zdev->domain = (u16) zdev->uid;
		if (zdev->domain >= ZPCI_NR_DEVICES)
			return 0;

		spin_lock(&zpci_domain_lock);
		if (test_bit(zdev->domain, zpci_domain)) {
			spin_unlock(&zpci_domain_lock);
			return -EEXIST;
		}
		set_bit(zdev->domain, zpci_domain);
		spin_unlock(&zpci_domain_lock);
		return 0;
	}

	spin_lock(&zpci_domain_lock);
	zdev->domain = find_first_zero_bit(zpci_domain, ZPCI_NR_DEVICES);
	if (zdev->domain == ZPCI_NR_DEVICES) {
		spin_unlock(&zpci_domain_lock);
		return -ENOSPC;
	}
	set_bit(zdev->domain, zpci_domain);
	spin_unlock(&zpci_domain_lock);
	return 0;
}

static void zpci_free_domain(struct zpci_dev *zdev)
{
	if (zdev->domain >= ZPCI_NR_DEVICES)
		return;

	spin_lock(&zpci_domain_lock);
	clear_bit(zdev->domain, zpci_domain);
	spin_unlock(&zpci_domain_lock);
}

void pcibios_remove_bus(struct pci_bus *bus)
{
	struct zpci_dev *zdev = get_zdev_by_bus(bus);

	zpci_exit_slot(zdev);
	zpci_cleanup_bus_resources(zdev);
	zpci_destroy_iommu(zdev);
	zpci_free_domain(zdev);

	spin_lock(&zpci_list_lock);
	list_del(&zdev->entry);
	spin_unlock(&zpci_list_lock);

	zpci_dbg(3, "rem fid:%x\n", zdev->fid);
	kfree(zdev);
}

static int zpci_scan_bus(struct zpci_dev *zdev)
{
	LIST_HEAD(resources);
	int ret;

	ret = zpci_setup_bus_resources(zdev, &resources);
	if (ret)
		goto error;

	zdev->bus = pci_scan_root_bus(NULL, ZPCI_BUS_NR, &pci_root_ops,
				      zdev, &resources);
	if (!zdev->bus) {
		ret = -EIO;
		goto error;
	}
	zdev->bus->max_bus_speed = zdev->max_bus_speed;
	pci_bus_add_devices(zdev->bus);
	return 0;

error:
	zpci_cleanup_bus_resources(zdev);
	pci_free_resource_list(&resources);
	return ret;
}

int zpci_enable_device(struct zpci_dev *zdev)
{
	int rc;

	rc = clp_enable_fh(zdev, ZPCI_NR_DMA_SPACES);
	if (rc)
		goto out;

	rc = zpci_dma_init_device(zdev);
	if (rc)
		goto out_dma;

	zdev->state = ZPCI_FN_STATE_ONLINE;
	return 0;

out_dma:
	clp_disable_fh(zdev);
out:
	return rc;
}
EXPORT_SYMBOL_GPL(zpci_enable_device);

int zpci_disable_device(struct zpci_dev *zdev)
{
	zpci_dma_exit_device(zdev);
	return clp_disable_fh(zdev);
}
EXPORT_SYMBOL_GPL(zpci_disable_device);

int zpci_create_device(struct zpci_dev *zdev)
{
	int rc;

	rc = zpci_alloc_domain(zdev);
	if (rc)
		goto out;

	rc = zpci_init_iommu(zdev);
	if (rc)
		goto out_free;

	mutex_init(&zdev->lock);
	if (zdev->state == ZPCI_FN_STATE_CONFIGURED) {
		rc = zpci_enable_device(zdev);
		if (rc)
			goto out_destroy_iommu;
	}
	rc = zpci_scan_bus(zdev);
	if (rc)
		goto out_disable;
=======
static int __zpci_register_domain(int domain)
{
	spin_lock(&zpci_domain_lock);
	if (test_bit(domain, zpci_domain)) {
		spin_unlock(&zpci_domain_lock);
		pr_err("Domain %04x is already assigned\n", domain);
		return -EEXIST;
	}
	set_bit(domain, zpci_domain);
	spin_unlock(&zpci_domain_lock);
	return domain;
}

static int __zpci_alloc_domain(void)
{
	int domain;

	spin_lock(&zpci_domain_lock);
	/*
	 * We can always auto allocate domains below ZPCI_NR_DEVICES.
	 * There is either a free domain or we have reached the maximum in
	 * which case we would have bailed earlier.
	 */
	domain = find_first_zero_bit(zpci_domain, ZPCI_NR_DEVICES);
	set_bit(domain, zpci_domain);
	spin_unlock(&zpci_domain_lock);
	return domain;
}

int zpci_alloc_domain(int domain)
{
	if (zpci_unique_uid) {
		if (domain)
			return __zpci_register_domain(domain);
		pr_warn("UID checking was active but no UID is provided: switching to automatic domain allocation\n");
		update_uid_checking(false);
	}
	return __zpci_alloc_domain();
}

void zpci_free_domain(int domain)
{
	spin_lock(&zpci_domain_lock);
	clear_bit(domain, zpci_domain);
	spin_unlock(&zpci_domain_lock);
}


int zpci_enable_device(struct zpci_dev *zdev)
{
	u32 fh = zdev->fh;
	int rc = 0;

	if (clp_enable_fh(zdev, &fh, ZPCI_NR_DMA_SPACES))
		rc = -EIO;
	else
		zdev->fh = fh;
	return rc;
}

int zpci_disable_device(struct zpci_dev *zdev)
{
	u32 fh = zdev->fh;
	int cc, rc = 0;

	cc = clp_disable_fh(zdev, &fh);
	if (!cc) {
		zdev->fh = fh;
	} else if (cc == CLP_RC_SETPCIFN_ALRDY) {
		pr_info("Disabling PCI function %08x had no effect as it was already disabled\n",
			zdev->fid);
		/* Function is already disabled - update handle */
		rc = clp_refresh_fh(zdev->fid, &fh);
		if (!rc) {
			zdev->fh = fh;
			rc = -EINVAL;
		}
	} else {
		rc = -EIO;
	}
	return rc;
}

/**
 * zpci_create_device() - Create a new zpci_dev and add it to the zbus
 * @fid: Function ID of the device to be created
 * @fh: Current Function Handle of the device to be created
 * @state: Initial state after creation either Standby or Configured
 *
 * Creates a new zpci device and adds it to its, possibly newly created, zbus
 * as well as zpci_list.
 *
 * Returns: the zdev on success or an error pointer otherwise
 */
struct zpci_dev *zpci_create_device(u32 fid, u32 fh, enum zpci_state state)
{
	struct zpci_dev *zdev;
	int rc;

	zpci_dbg(3, "add fid:%x, fh:%x, c:%d\n", fid, fh, state);
	zdev = kzalloc(sizeof(*zdev), GFP_KERNEL);
	if (!zdev)
		return ERR_PTR(-ENOMEM);

	/* FID and Function Handle are the static/dynamic identifiers */
	zdev->fid = fid;
	zdev->fh = fh;

	/* Query function properties and update zdev */
	rc = clp_query_pci_fn(zdev);
	if (rc)
		goto error;
	zdev->state =  state;

	kref_init(&zdev->kref);
	mutex_init(&zdev->lock);

	rc = zpci_init_iommu(zdev);
	if (rc)
		goto error;

	rc = zpci_bus_device_register(zdev, &pci_root_ops);
	if (rc)
		goto error_destroy_iommu;
>>>>>>> upstream/android-13

	spin_lock(&zpci_list_lock);
	list_add_tail(&zdev->entry, &zpci_list);
	spin_unlock(&zpci_list_lock);

<<<<<<< HEAD
	zpci_init_slot(zdev);

	return 0;

out_disable:
	if (zdev->state == ZPCI_FN_STATE_ONLINE)
		zpci_disable_device(zdev);
out_destroy_iommu:
	zpci_destroy_iommu(zdev);
out_free:
	zpci_free_domain(zdev);
out:
	return rc;
}

void zpci_remove_device(struct zpci_dev *zdev)
{
	if (!zdev->bus)
		return;

	pci_stop_root_bus(zdev->bus);
	pci_remove_root_bus(zdev->bus);
=======
	return zdev;

error_destroy_iommu:
	zpci_destroy_iommu(zdev);
error:
	zpci_dbg(0, "add fid:%x, rc:%d\n", fid, rc);
	kfree(zdev);
	return ERR_PTR(rc);
}

bool zpci_is_device_configured(struct zpci_dev *zdev)
{
	enum zpci_state state = zdev->state;

	return state != ZPCI_FN_STATE_RESERVED &&
		state != ZPCI_FN_STATE_STANDBY;
}

/**
 * zpci_scan_configured_device() - Scan a freshly configured zpci_dev
 * @zdev: The zpci_dev to be configured
 * @fh: The general function handle supplied by the platform
 *
 * Given a device in the configuration state Configured, enables, scans and
 * adds it to the common code PCI subsystem if possible. If the PCI device is
 * parked because we can not yet create a PCI bus because we have not seen
 * function 0, it is ignored but will be scanned once function 0 appears.
 * If any failure occurs, the zpci_dev is left disabled.
 *
 * Return: 0 on success, or an error code otherwise
 */
int zpci_scan_configured_device(struct zpci_dev *zdev, u32 fh)
{
	int rc;

	zdev->fh = fh;
	/* the PCI function will be scanned once function 0 appears */
	if (!zdev->zbus->bus)
		return 0;

	/* For function 0 on a multi-function bus scan whole bus as we might
	 * have to pick up existing functions waiting for it to allow creating
	 * the PCI bus
	 */
	if (zdev->devfn == 0 && zdev->zbus->multifunction)
		rc = zpci_bus_scan_bus(zdev->zbus);
	else
		rc = zpci_bus_scan_device(zdev);

	return rc;
}

/**
 * zpci_deconfigure_device() - Deconfigure a zpci_dev
 * @zdev: The zpci_dev to configure
 *
 * Deconfigure a zPCI function that is currently configured and possibly known
 * to the common code PCI subsystem.
 * If any failure occurs the device is left as is.
 *
 * Return: 0 on success, or an error code otherwise
 */
int zpci_deconfigure_device(struct zpci_dev *zdev)
{
	int rc;

	if (zdev->zbus->bus)
		zpci_bus_remove_device(zdev, false);

	if (zdev->dma_table) {
		rc = zpci_dma_exit_device(zdev);
		if (rc)
			return rc;
	}
	if (zdev_enabled(zdev)) {
		rc = zpci_disable_device(zdev);
		if (rc)
			return rc;
	}

	rc = sclp_pci_deconfigure(zdev->fid);
	zpci_dbg(3, "deconf fid:%x, rc:%d\n", zdev->fid, rc);
	if (rc)
		return rc;
	zdev->state = ZPCI_FN_STATE_STANDBY;

	return 0;
}

/**
 * zpci_device_reserved() - Mark device as resverved
 * @zdev: the zpci_dev that was reserved
 *
 * Handle the case that a given zPCI function was reserved by another system.
 * After a call to this function the zpci_dev can not be found via
 * get_zdev_by_fid() anymore but may still be accessible via existing
 * references though it will not be functional anymore.
 */
void zpci_device_reserved(struct zpci_dev *zdev)
{
	if (zdev->has_hp_slot)
		zpci_exit_slot(zdev);
	/*
	 * Remove device from zpci_list as it is going away. This also
	 * makes sure we ignore subsequent zPCI events for this device.
	 */
	spin_lock(&zpci_list_lock);
	list_del(&zdev->entry);
	spin_unlock(&zpci_list_lock);
	zdev->state = ZPCI_FN_STATE_RESERVED;
	zpci_dbg(3, "rsv fid:%x\n", zdev->fid);
	zpci_zdev_put(zdev);
}

void zpci_release_device(struct kref *kref)
{
	struct zpci_dev *zdev = container_of(kref, struct zpci_dev, kref);
	int ret;

	if (zdev->zbus->bus)
		zpci_bus_remove_device(zdev, false);

	if (zdev->dma_table)
		zpci_dma_exit_device(zdev);
	if (zdev_enabled(zdev))
		zpci_disable_device(zdev);

	switch (zdev->state) {
	case ZPCI_FN_STATE_CONFIGURED:
		ret = sclp_pci_deconfigure(zdev->fid);
		zpci_dbg(3, "deconf fid:%x, rc:%d\n", zdev->fid, ret);
		fallthrough;
	case ZPCI_FN_STATE_STANDBY:
		if (zdev->has_hp_slot)
			zpci_exit_slot(zdev);
		spin_lock(&zpci_list_lock);
		list_del(&zdev->entry);
		spin_unlock(&zpci_list_lock);
		zpci_dbg(3, "rsv fid:%x\n", zdev->fid);
		fallthrough;
	case ZPCI_FN_STATE_RESERVED:
		if (zdev->has_resources)
			zpci_cleanup_bus_resources(zdev);
		zpci_bus_device_unregister(zdev);
		zpci_destroy_iommu(zdev);
		fallthrough;
	default:
		break;
	}
	zpci_dbg(3, "rem fid:%x\n", zdev->fid);
	kfree(zdev);
>>>>>>> upstream/android-13
}

int zpci_report_error(struct pci_dev *pdev,
		      struct zpci_report_error_header *report)
{
	struct zpci_dev *zdev = to_zpci(pdev);

	return sclp_pci_report(report, zdev->fh, zdev->fid);
}
EXPORT_SYMBOL(zpci_report_error);

static int zpci_mem_init(void)
{
	BUILD_BUG_ON(!is_power_of_2(__alignof__(struct zpci_fmb)) ||
		     __alignof__(struct zpci_fmb) < sizeof(struct zpci_fmb));

	zdev_fmb_cache = kmem_cache_create("PCI_FMB_cache", sizeof(struct zpci_fmb),
					   __alignof__(struct zpci_fmb), 0, NULL);
	if (!zdev_fmb_cache)
		goto error_fmb;

	zpci_iomap_start = kcalloc(ZPCI_IOMAP_ENTRIES,
				   sizeof(*zpci_iomap_start), GFP_KERNEL);
	if (!zpci_iomap_start)
		goto error_iomap;

	zpci_iomap_bitmap = kcalloc(BITS_TO_LONGS(ZPCI_IOMAP_ENTRIES),
				    sizeof(*zpci_iomap_bitmap), GFP_KERNEL);
	if (!zpci_iomap_bitmap)
		goto error_iomap_bitmap;

<<<<<<< HEAD
=======
	if (static_branch_likely(&have_mio))
		clp_setup_writeback_mio();

>>>>>>> upstream/android-13
	return 0;
error_iomap_bitmap:
	kfree(zpci_iomap_start);
error_iomap:
	kmem_cache_destroy(zdev_fmb_cache);
error_fmb:
	return -ENOMEM;
}

static void zpci_mem_exit(void)
{
	kfree(zpci_iomap_bitmap);
	kfree(zpci_iomap_start);
	kmem_cache_destroy(zdev_fmb_cache);
}

<<<<<<< HEAD
static unsigned int s390_pci_probe = 1;
=======
static unsigned int s390_pci_probe __initdata = 1;
unsigned int s390_pci_force_floating __initdata;
>>>>>>> upstream/android-13
static unsigned int s390_pci_initialized;

char * __init pcibios_setup(char *str)
{
	if (!strcmp(str, "off")) {
		s390_pci_probe = 0;
		return NULL;
	}
<<<<<<< HEAD
=======
	if (!strcmp(str, "nomio")) {
		S390_lowcore.machine_flags &= ~MACHINE_FLAG_PCI_MIO;
		return NULL;
	}
	if (!strcmp(str, "force_floating")) {
		s390_pci_force_floating = 1;
		return NULL;
	}
	if (!strcmp(str, "norid")) {
		s390_pci_no_rid = 1;
		return NULL;
	}
>>>>>>> upstream/android-13
	return str;
}

bool zpci_is_enabled(void)
{
	return s390_pci_initialized;
}

static int __init pci_base_init(void)
{
	int rc;

	if (!s390_pci_probe)
		return 0;

<<<<<<< HEAD
	if (!test_facility(69) || !test_facility(71))
		return 0;
=======
	if (!test_facility(69) || !test_facility(71)) {
		pr_info("PCI is not supported because CPU facilities 69 or 71 are not available\n");
		return 0;
	}

	if (MACHINE_HAS_PCI_MIO) {
		static_branch_enable(&have_mio);
		ctl_set_bit(2, 5);
	}
>>>>>>> upstream/android-13

	rc = zpci_debug_init();
	if (rc)
		goto out;

	rc = zpci_mem_init();
	if (rc)
		goto out_mem;

	rc = zpci_irq_init();
	if (rc)
		goto out_irq;

	rc = zpci_dma_init();
	if (rc)
		goto out_dma;

	rc = clp_scan_pci_devices();
	if (rc)
		goto out_find;
<<<<<<< HEAD
=======
	zpci_bus_scan_busses();
>>>>>>> upstream/android-13

	s390_pci_initialized = 1;
	return 0;

out_find:
	zpci_dma_exit();
out_dma:
	zpci_irq_exit();
out_irq:
	zpci_mem_exit();
out_mem:
	zpci_debug_exit();
out:
	return rc;
}
subsys_initcall_sync(pci_base_init);
<<<<<<< HEAD

void zpci_rescan(void)
{
	if (zpci_is_enabled())
		clp_rescan_pci_devices_simple();
}
=======
>>>>>>> upstream/android-13
