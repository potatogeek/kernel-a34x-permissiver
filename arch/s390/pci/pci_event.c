// SPDX-License-Identifier: GPL-2.0
/*
 *  Copyright IBM Corp. 2012
 *
 *  Author(s):
 *    Jan Glauber <jang@linux.vnet.ibm.com>
 */

#define KMSG_COMPONENT "zpci"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/pci.h>
#include <asm/pci_debug.h>
<<<<<<< HEAD
#include <asm/sclp.h>

=======
#include <asm/pci_dma.h>
#include <asm/sclp.h>

#include "pci_bus.h"

>>>>>>> upstream/android-13
/* Content Code Description for PCI Function Error */
struct zpci_ccdf_err {
	u32 reserved1;
	u32 fh;				/* function handle */
	u32 fid;			/* function id */
	u32 ett		:  4;		/* expected table type */
	u32 mvn		: 12;		/* MSI vector number */
	u32 dmaas	:  8;		/* DMA address space */
	u32		:  6;
	u32 q		:  1;		/* event qualifier */
	u32 rw		:  1;		/* read/write */
	u64 faddr;			/* failing address */
	u32 reserved3;
	u16 reserved4;
	u16 pec;			/* PCI event code */
} __packed;

/* Content Code Description for PCI Function Availability */
struct zpci_ccdf_avail {
	u32 reserved1;
	u32 fh;				/* function handle */
	u32 fid;			/* function id */
	u32 reserved2;
	u32 reserved3;
	u32 reserved4;
	u32 reserved5;
	u16 reserved6;
	u16 pec;			/* PCI event code */
} __packed;

static void __zpci_event_error(struct zpci_ccdf_err *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);
	struct pci_dev *pdev = NULL;

	zpci_err("error CCDF:\n");
	zpci_err_hex(ccdf, sizeof(*ccdf));

	if (zdev)
<<<<<<< HEAD
		pdev = pci_get_slot(zdev->bus, ZPCI_DEVFN);
=======
		pdev = pci_get_slot(zdev->zbus->bus, zdev->devfn);
>>>>>>> upstream/android-13

	pr_err("%s: Event 0x%x reports an error for PCI function 0x%x\n",
	       pdev ? pci_name(pdev) : "n/a", ccdf->pec, ccdf->fid);

	if (!pdev)
		return;

	pdev->error_state = pci_channel_io_perm_failure;
	pci_dev_put(pdev);
}

void zpci_event_error(void *data)
{
	if (zpci_is_enabled())
		__zpci_event_error(data);
}

<<<<<<< HEAD
static void __zpci_event_availability(struct zpci_ccdf_avail *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);
	struct pci_dev *pdev = NULL;
	enum zpci_state state;
	int ret;

	if (zdev)
		pdev = pci_get_slot(zdev->bus, ZPCI_DEVFN);

	pr_info("%s: Event 0x%x reconfigured PCI function 0x%x\n",
		pdev ? pci_name(pdev) : "n/a", ccdf->pec, ccdf->fid);
=======
static void zpci_event_hard_deconfigured(struct zpci_dev *zdev, u32 fh)
{
	zdev->fh = fh;
	/* Give the driver a hint that the function is
	 * already unusable.
	 */
	zpci_bus_remove_device(zdev, true);
	/* Even though the device is already gone we still
	 * need to free zPCI resources as part of the disable.
	 */
	if (zdev->dma_table)
		zpci_dma_exit_device(zdev);
	if (zdev_enabled(zdev))
		zpci_disable_device(zdev);
	zdev->state = ZPCI_FN_STATE_STANDBY;
}

static void __zpci_event_availability(struct zpci_ccdf_avail *ccdf)
{
	struct zpci_dev *zdev = get_zdev_by_fid(ccdf->fid);
	enum zpci_state state;

>>>>>>> upstream/android-13
	zpci_err("avail CCDF:\n");
	zpci_err_hex(ccdf, sizeof(*ccdf));

	switch (ccdf->pec) {
	case 0x0301: /* Reserved|Standby -> Configured */
		if (!zdev) {
<<<<<<< HEAD
			ret = clp_add_pci_device(ccdf->fid, ccdf->fh, 0);
			if (ret)
				break;
			zdev = get_zdev_by_fid(ccdf->fid);
		}
		if (!zdev || zdev->state != ZPCI_FN_STATE_STANDBY)
			break;
		zdev->state = ZPCI_FN_STATE_CONFIGURED;
		zdev->fh = ccdf->fh;
		ret = zpci_enable_device(zdev);
		if (ret)
			break;
		pci_lock_rescan_remove();
		pci_rescan_bus(zdev->bus);
		pci_unlock_rescan_remove();
		break;
	case 0x0302: /* Reserved -> Standby */
		if (!zdev)
			clp_add_pci_device(ccdf->fid, ccdf->fh, 0);
		break;
	case 0x0303: /* Deconfiguration requested */
		if (!zdev)
			break;
		if (pdev)
			pci_stop_and_remove_bus_device_locked(pdev);

		ret = zpci_disable_device(zdev);
		if (ret)
			break;

		ret = sclp_pci_deconfigure(zdev->fid);
		zpci_dbg(3, "deconf fid:%x, rc:%d\n", zdev->fid, ret);
		if (!ret)
			zdev->state = ZPCI_FN_STATE_STANDBY;

		break;
	case 0x0304: /* Configured -> Standby|Reserved */
		if (!zdev)
			break;
		if (pdev) {
			/* Give the driver a hint that the function is
			 * already unusable. */
			pdev->error_state = pci_channel_io_perm_failure;
			pci_stop_and_remove_bus_device_locked(pdev);
		}

		zdev->fh = ccdf->fh;
		zpci_disable_device(zdev);
		zdev->state = ZPCI_FN_STATE_STANDBY;
		if (!clp_get_state(ccdf->fid, &state) &&
		    state == ZPCI_FN_STATE_RESERVED) {
			zpci_remove_device(zdev);
		}
		break;
	case 0x0306: /* 0x308 or 0x302 for multiple devices */
		clp_rescan_pci_devices();
=======
			zdev = zpci_create_device(ccdf->fid, ccdf->fh, ZPCI_FN_STATE_CONFIGURED);
			if (IS_ERR(zdev))
				break;
		} else {
			/* the configuration request may be stale */
			if (zdev->state != ZPCI_FN_STATE_STANDBY)
				break;
			zdev->state = ZPCI_FN_STATE_CONFIGURED;
		}
		zpci_scan_configured_device(zdev, ccdf->fh);
		break;
	case 0x0302: /* Reserved -> Standby */
		if (!zdev)
			zpci_create_device(ccdf->fid, ccdf->fh, ZPCI_FN_STATE_STANDBY);
		else
			zdev->fh = ccdf->fh;
		break;
	case 0x0303: /* Deconfiguration requested */
		if (zdev) {
			/* The event may have been queued before we confirgured
			 * the device.
			 */
			if (zdev->state != ZPCI_FN_STATE_CONFIGURED)
				break;
			zdev->fh = ccdf->fh;
			zpci_deconfigure_device(zdev);
		}
		break;
	case 0x0304: /* Configured -> Standby|Reserved */
		if (zdev) {
			/* The event may have been queued before we confirgured
			 * the device.:
			 */
			if (zdev->state == ZPCI_FN_STATE_CONFIGURED)
				zpci_event_hard_deconfigured(zdev, ccdf->fh);
			/* The 0x0304 event may immediately reserve the device */
			if (!clp_get_state(zdev->fid, &state) &&
			    state == ZPCI_FN_STATE_RESERVED) {
				zpci_device_reserved(zdev);
			}
		}
		break;
	case 0x0306: /* 0x308 or 0x302 for multiple devices */
		zpci_remove_reserved_devices();
		clp_scan_pci_devices();
>>>>>>> upstream/android-13
		break;
	case 0x0308: /* Standby -> Reserved */
		if (!zdev)
			break;
<<<<<<< HEAD
		zpci_remove_device(zdev);
=======
		zpci_device_reserved(zdev);
>>>>>>> upstream/android-13
		break;
	default:
		break;
	}
<<<<<<< HEAD
	pci_dev_put(pdev);
=======
>>>>>>> upstream/android-13
}

void zpci_event_availability(void *data)
{
	if (zpci_is_enabled())
		__zpci_event_availability(data);
}
