<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * PCI glue for ISHTP provider device (ISH) driver
 *
 * Copyright (c) 2014-2016, Intel Corporation.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 */

=======
 */

#include <linux/acpi.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/sched.h>
<<<<<<< HEAD
=======
#include <linux/suspend.h>
>>>>>>> upstream/android-13
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#define CREATE_TRACE_POINTS
#include <trace/events/intel_ish.h>
#include "ishtp-dev.h"
#include "hw-ish.h"

static const struct pci_device_id ish_pci_tbl[] = {
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, CHV_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, BXT_Ax_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, BXT_Bx_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, APL_Ax_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, SPT_Ax_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, CNL_Ax_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, GLK_Ax_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, CNL_H_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, ICL_MOBILE_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, SPT_H_DEVICE_ID)},
<<<<<<< HEAD
=======
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, CML_LP_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, CMP_H_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, EHL_Ax_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, TGL_LP_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, TGL_H_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, ADL_S_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, ADL_P_DEVICE_ID)},
>>>>>>> upstream/android-13
	{0, }
};
MODULE_DEVICE_TABLE(pci, ish_pci_tbl);

/**
 * ish_event_tracer() - Callback function to dump trace messages
 * @dev:	ishtp device
 * @format:	printf style format
 *
 * Callback to direct log messages to Linux trace buffers
 */
static __printf(2, 3)
void ish_event_tracer(struct ishtp_device *dev, const char *format, ...)
{
	if (trace_ishtp_dump_enabled()) {
		va_list args;
		char tmp_buf[100];

		va_start(args, format);
		vsnprintf(tmp_buf, sizeof(tmp_buf), format, args);
		va_end(args);

		trace_ishtp_dump(tmp_buf);
	}
}

/**
 * ish_init() - Init function
 * @dev:	ishtp device
 *
 * This function initialize wait queues for suspend/resume and call
 * calls hadware initialization function. This will initiate
 * startup sequence
 *
 * Return: 0 for success or error code for failure
 */
static int ish_init(struct ishtp_device *dev)
{
	int ret;

	/* Set the state of ISH HW to start */
	ret = ish_hw_start(dev);
	if (ret) {
		dev_err(dev->devc, "ISH: hw start failed.\n");
		return ret;
	}

	/* Start the inter process communication to ISH processor */
	ret = ishtp_start(dev);
	if (ret) {
		dev_err(dev->devc, "ISHTP: Protocol init failed.\n");
		return ret;
	}

	return 0;
}

static const struct pci_device_id ish_invalid_pci_ids[] = {
	/* Mehlow platform special pci ids */
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0xA309)},
	{PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0xA30A)},
	{}
};

<<<<<<< HEAD
=======
static inline bool ish_should_enter_d0i3(struct pci_dev *pdev)
{
	return !pm_suspend_via_firmware() || pdev->device == CHV_DEVICE_ID;
}

static inline bool ish_should_leave_d0i3(struct pci_dev *pdev)
{
	return !pm_resume_via_firmware() || pdev->device == CHV_DEVICE_ID;
}

static int enable_gpe(struct device *dev)
{
#ifdef CONFIG_ACPI
	acpi_status acpi_sts;
	struct acpi_device *adev;
	struct acpi_device_wakeup *wakeup;

	adev = ACPI_COMPANION(dev);
	if (!adev) {
		dev_err(dev, "get acpi handle failed\n");
		return -ENODEV;
	}
	wakeup = &adev->wakeup;

	acpi_sts = acpi_enable_gpe(wakeup->gpe_device, wakeup->gpe_number);
	if (ACPI_FAILURE(acpi_sts)) {
		dev_err(dev, "enable ose_gpe failed\n");
		return -EIO;
	}

	return 0;
#else
	return -ENODEV;
#endif
}

static void enable_pme_wake(struct pci_dev *pdev)
{
	if ((pci_pme_capable(pdev, PCI_D0) ||
	     pci_pme_capable(pdev, PCI_D3hot) ||
	     pci_pme_capable(pdev, PCI_D3cold)) && !enable_gpe(&pdev->dev)) {
		pci_pme_active(pdev, true);
		dev_dbg(&pdev->dev, "ish ipc driver pme wake enabled\n");
	}
}

>>>>>>> upstream/android-13
/**
 * ish_probe() - PCI driver probe callback
 * @pdev:	pci device
 * @ent:	pci device id
 *
 * Initialize PCI function, setup interrupt and call for ISH initialization
 *
 * Return: 0 for success or error code for failure
 */
static int ish_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
<<<<<<< HEAD
	struct ishtp_device *dev;
	struct ish_hw *hw;
	int	ret;
=======
	int ret;
	struct ish_hw *hw;
	unsigned long irq_flag = 0;
	struct ishtp_device *ishtp;
	struct device *dev = &pdev->dev;
>>>>>>> upstream/android-13

	/* Check for invalid platforms for ISH support */
	if (pci_dev_present(ish_invalid_pci_ids))
		return -ENODEV;

	/* enable pci dev */
<<<<<<< HEAD
	ret = pci_enable_device(pdev);
	if (ret) {
		dev_err(&pdev->dev, "ISH: Failed to enable PCI device\n");
=======
	ret = pcim_enable_device(pdev);
	if (ret) {
		dev_err(dev, "ISH: Failed to enable PCI device\n");
>>>>>>> upstream/android-13
		return ret;
	}

	/* set PCI host mastering */
	pci_set_master(pdev);

	/* pci request regions for ISH driver */
<<<<<<< HEAD
	ret = pci_request_regions(pdev, KBUILD_MODNAME);
	if (ret) {
		dev_err(&pdev->dev, "ISH: Failed to get PCI regions\n");
		goto disable_device;
	}

	/* allocates and initializes the ISH dev structure */
	dev = ish_dev_init(pdev);
	if (!dev) {
		ret = -ENOMEM;
		goto release_regions;
	}
	hw = to_ish_hw(dev);
	dev->print_log = ish_event_tracer;

	/* mapping IO device memory */
	hw->mem_addr = pci_iomap(pdev, 0, 0);
	if (!hw->mem_addr) {
		dev_err(&pdev->dev, "ISH: mapping I/O range failure\n");
		ret = -ENOMEM;
		goto free_device;
	}

	dev->pdev = pdev;

	pdev->dev_flags |= PCI_DEV_FLAGS_NO_D3;

	/* request and enable interrupt */
	ret = request_irq(pdev->irq, ish_irq_handler, IRQF_SHARED,
			  KBUILD_MODNAME, dev);
	if (ret) {
		dev_err(&pdev->dev, "ISH: request IRQ failure (%d)\n",
			pdev->irq);
		goto free_device;
	}

	dev_set_drvdata(dev->devc, dev);

	init_waitqueue_head(&dev->suspend_wait);
	init_waitqueue_head(&dev->resume_wait);

	ret = ish_init(dev);
	if (ret)
		goto free_irq;

	return 0;

free_irq:
	free_irq(pdev->irq, dev);
free_device:
	pci_iounmap(pdev, hw->mem_addr);
release_regions:
	pci_release_regions(pdev);
disable_device:
	pci_clear_master(pdev);
	pci_disable_device(pdev);
	dev_err(&pdev->dev, "ISH: PCI driver initialization failed.\n");

	return ret;
=======
	ret = pcim_iomap_regions(pdev, 1 << 0, KBUILD_MODNAME);
	if (ret) {
		dev_err(dev, "ISH: Failed to get PCI regions\n");
		return ret;
	}

	/* allocates and initializes the ISH dev structure */
	ishtp = ish_dev_init(pdev);
	if (!ishtp) {
		ret = -ENOMEM;
		return ret;
	}
	hw = to_ish_hw(ishtp);
	ishtp->print_log = ish_event_tracer;

	/* mapping IO device memory */
	hw->mem_addr = pcim_iomap_table(pdev)[0];
	ishtp->pdev = pdev;

	/* request and enable interrupt */
	ret = pci_alloc_irq_vectors(pdev, 1, 1, PCI_IRQ_ALL_TYPES);
	if (!pdev->msi_enabled && !pdev->msix_enabled)
		irq_flag = IRQF_SHARED;

	ret = devm_request_irq(dev, pdev->irq, ish_irq_handler,
			       irq_flag, KBUILD_MODNAME, ishtp);
	if (ret) {
		dev_err(dev, "ISH: request IRQ %d failed\n", pdev->irq);
		return ret;
	}

	dev_set_drvdata(ishtp->devc, ishtp);

	init_waitqueue_head(&ishtp->suspend_wait);
	init_waitqueue_head(&ishtp->resume_wait);

	/* Enable PME for EHL */
	if (pdev->device == EHL_Ax_DEVICE_ID)
		enable_pme_wake(pdev);

	ret = ish_init(ishtp);
	if (ret)
		return ret;

	return 0;
>>>>>>> upstream/android-13
}

/**
 * ish_remove() - PCI driver remove callback
 * @pdev:	pci device
 *
 * This function does cleanup of ISH on pci remove callback
 */
static void ish_remove(struct pci_dev *pdev)
{
	struct ishtp_device *ishtp_dev = pci_get_drvdata(pdev);
<<<<<<< HEAD
	struct ish_hw *hw = to_ish_hw(ishtp_dev);

	ishtp_bus_remove_all_clients(ishtp_dev, false);
	ish_device_disable(ishtp_dev);

	free_irq(pdev->irq, ishtp_dev);
	pci_iounmap(pdev, hw->mem_addr);
	pci_release_regions(pdev);
	pci_clear_master(pdev);
	pci_disable_device(pdev);
=======

	ishtp_bus_remove_all_clients(ishtp_dev, false);
	ish_device_disable(ishtp_dev);
>>>>>>> upstream/android-13
}

static struct device __maybe_unused *ish_resume_device;

/* 50ms to get resume response */
#define WAIT_FOR_RESUME_ACK_MS		50

/**
 * ish_resume_handler() - Work function to complete resume
 * @work:	work struct
 *
 * The resume work function to complete resume function asynchronously.
 * There are two resume paths, one where ISH is not powered off,
 * in that case a simple resume message is enough, others we need
 * a reset sequence.
 */
static void __maybe_unused ish_resume_handler(struct work_struct *work)
{
	struct pci_dev *pdev = to_pci_dev(ish_resume_device);
	struct ishtp_device *dev = pci_get_drvdata(pdev);
<<<<<<< HEAD
	uint32_t fwsts;
	int ret;

	/* Get ISH FW status */
	fwsts = IPC_GET_ISH_FWSTS(dev->ops->get_fw_status(dev));

	/*
	 * If currently, in ISH FW, sensor app is loaded or beyond that,
	 * it means ISH isn't powered off, in this case, send a resume message.
	 */
	if (fwsts >= FWSTS_SENSOR_APP_LOADED) {
=======
	uint32_t fwsts = dev->ops->get_fw_status(dev);

	if (ish_should_leave_d0i3(pdev) && !dev->suspend_flag
			&& IPC_IS_ISH_ILUP(fwsts)) {
		if (device_may_wakeup(&pdev->dev))
			disable_irq_wake(pdev->irq);

		ish_set_host_ready(dev);

>>>>>>> upstream/android-13
		ishtp_send_resume(dev);

		/* Waiting to get resume response */
		if (dev->resume_flag)
<<<<<<< HEAD
			ret = wait_event_interruptible_timeout(dev->resume_wait,
				!dev->resume_flag,
				msecs_to_jiffies(WAIT_FOR_RESUME_ACK_MS));
	}

	/*
	 * If in ISH FW, sensor app isn't loaded yet, or no resume response.
	 * That means this platform is not S0ix compatible, or something is
	 * wrong with ISH FW. So on resume, full reboot of ISH processor will
	 * happen, so need to go through init sequence again.
	 */
	if (dev->resume_flag)
		ish_init(dev);
=======
			wait_event_interruptible_timeout(dev->resume_wait,
				!dev->resume_flag,
				msecs_to_jiffies(WAIT_FOR_RESUME_ACK_MS));

		/*
		 * If the flag is not cleared, something is wrong with ISH FW.
		 * So on resume, need to go through init sequence again.
		 */
		if (dev->resume_flag)
			ish_init(dev);
	} else {
		/*
		 * Resume from the D3, full reboot of ISH processor will happen,
		 * so need to go through init sequence again.
		 */
		ish_init(dev);
	}
>>>>>>> upstream/android-13
}

/**
 * ish_suspend() - ISH suspend callback
 * @device:	device pointer
 *
 * ISH suspend callback
 *
 * Return: 0 to the pm core
 */
static int __maybe_unused ish_suspend(struct device *device)
{
	struct pci_dev *pdev = to_pci_dev(device);
	struct ishtp_device *dev = pci_get_drvdata(pdev);

<<<<<<< HEAD
	enable_irq_wake(pdev->irq);
	/*
	 * If previous suspend hasn't been asnwered then ISH is likely dead,
	 * don't attempt nested notification
	 */
	if (dev->suspend_flag)
		return	0;

	dev->resume_flag = 0;
	dev->suspend_flag = 1;
	ishtp_send_suspend(dev);

	/* 25 ms should be enough for live ISH to flush all IPC buf */
	if (dev->suspend_flag)
		wait_event_interruptible_timeout(dev->suspend_wait,
						 !dev->suspend_flag,
						  msecs_to_jiffies(25));
=======
	if (ish_should_enter_d0i3(pdev)) {
		/*
		 * If previous suspend hasn't been asnwered then ISH is likely
		 * dead, don't attempt nested notification
		 */
		if (dev->suspend_flag)
			return	0;

		dev->resume_flag = 0;
		dev->suspend_flag = 1;
		ishtp_send_suspend(dev);

		/* 25 ms should be enough for live ISH to flush all IPC buf */
		if (dev->suspend_flag)
			wait_event_interruptible_timeout(dev->suspend_wait,
					!dev->suspend_flag,
					msecs_to_jiffies(25));

		if (dev->suspend_flag) {
			/*
			 * It looks like FW halt, clear the DMA bit, and put
			 * ISH into D3, and FW would reset on resume.
			 */
			ish_disable_dma(dev);
		} else {
			/*
			 * Save state so PCI core will keep the device at D0,
			 * the ISH would enter D0i3
			 */
			pci_save_state(pdev);

			if (device_may_wakeup(&pdev->dev))
				enable_irq_wake(pdev->irq);
		}
	} else {
		/*
		 * Clear the DMA bit before putting ISH into D3,
		 * or ISH FW would reset automatically.
		 */
		ish_disable_dma(dev);
	}
>>>>>>> upstream/android-13

	return 0;
}

static __maybe_unused DECLARE_WORK(resume_work, ish_resume_handler);
/**
 * ish_resume() - ISH resume callback
 * @device:	device pointer
 *
 * ISH resume callback
 *
 * Return: 0 to the pm core
 */
static int __maybe_unused ish_resume(struct device *device)
{
	struct pci_dev *pdev = to_pci_dev(device);
	struct ishtp_device *dev = pci_get_drvdata(pdev);

<<<<<<< HEAD
	ish_resume_device = device;
	dev->resume_flag = 1;

	disable_irq_wake(pdev->irq);
=======
	/* add this to finish power flow for EHL */
	if (dev->pdev->device == EHL_Ax_DEVICE_ID) {
		pci_set_power_state(pdev, PCI_D0);
		enable_pme_wake(pdev);
		dev_dbg(dev->devc, "set power state to D0 for ehl\n");
	}

	ish_resume_device = device;
	dev->resume_flag = 1;

>>>>>>> upstream/android-13
	schedule_work(&resume_work);

	return 0;
}

static SIMPLE_DEV_PM_OPS(ish_pm_ops, ish_suspend, ish_resume);

static struct pci_driver ish_driver = {
	.name = KBUILD_MODNAME,
	.id_table = ish_pci_tbl,
	.probe = ish_probe,
	.remove = ish_remove,
	.driver.pm = &ish_pm_ops,
};

module_pci_driver(ish_driver);

/* Original author */
MODULE_AUTHOR("Daniel Drubin <daniel.drubin@intel.com>");
/* Adoption to upstream Linux kernel */
MODULE_AUTHOR("Srinivas Pandruvada <srinivas.pandruvada@linux.intel.com>");

MODULE_DESCRIPTION("Intel(R) Integrated Sensor Hub PCI Device Driver");
MODULE_LICENSE("GPL");
