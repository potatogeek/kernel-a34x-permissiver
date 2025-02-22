/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * PCI Express Hot Plug Controller Driver
 *
 * Copyright (C) 1995,2001 Compaq Computer Corporation
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
 * Copyright (C) 2003-2004 Intel Corporation
 *
 * All rights reserved.
 *
 * Send feedback to <greg@kroah.com>, <kristen.c.accardi@intel.com>
 *
 */
#ifndef _PCIEHP_H
#define _PCIEHP_H

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/sched/signal.h>		/* signal_pending() */
=======
>>>>>>> upstream/android-13
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/workqueue.h>

#include "../pcie/portdrv.h"

<<<<<<< HEAD
#define MY_NAME	"pciehp"

extern bool pciehp_poll_mode;
extern int pciehp_poll_time;
extern bool pciehp_debug;

#define dbg(format, arg...)						\
do {									\
	if (pciehp_debug)						\
		printk(KERN_DEBUG "%s: " format, MY_NAME, ## arg);	\
} while (0)
#define err(format, arg...)						\
	printk(KERN_ERR "%s: " format, MY_NAME, ## arg)
#define info(format, arg...)						\
	printk(KERN_INFO "%s: " format, MY_NAME, ## arg)
#define warn(format, arg...)						\
	printk(KERN_WARNING "%s: " format, MY_NAME, ## arg)

#define ctrl_dbg(ctrl, format, arg...)					\
	do {								\
		if (pciehp_debug)					\
			dev_printk(KERN_DEBUG, &ctrl->pcie->device,	\
					format, ## arg);		\
	} while (0)
#define ctrl_err(ctrl, format, arg...)					\
	dev_err(&ctrl->pcie->device, format, ## arg)
#define ctrl_info(ctrl, format, arg...)					\
	dev_info(&ctrl->pcie->device, format, ## arg)
#define ctrl_warn(ctrl, format, arg...)					\
	dev_warn(&ctrl->pcie->device, format, ## arg)
=======
extern bool pciehp_poll_mode;
extern int pciehp_poll_time;

/*
 * Set CONFIG_DYNAMIC_DEBUG=y and boot with 'dyndbg="file pciehp* +p"' to
 * enable debug messages.
 */
#define ctrl_dbg(ctrl, format, arg...)					\
	pci_dbg(ctrl->pcie->port, format, ## arg)
#define ctrl_err(ctrl, format, arg...)					\
	pci_err(ctrl->pcie->port, format, ## arg)
#define ctrl_info(ctrl, format, arg...)					\
	pci_info(ctrl->pcie->port, format, ## arg)
#define ctrl_warn(ctrl, format, arg...)					\
	pci_warn(ctrl->pcie->port, format, ## arg)
>>>>>>> upstream/android-13

#define SLOT_NAME_SIZE 10

/**
<<<<<<< HEAD
 * struct slot - PCIe hotplug slot
 * @state: current state machine position
 * @ctrl: pointer to the slot's controller structure
 * @hotplug_slot: pointer to the structure registered with the PCI hotplug core
 * @work: work item to turn the slot on or off after 5 seconds in response to
 *	an Attention Button press
 * @lock: protects reads and writes of @state;
 *	protects scheduling, execution and cancellation of @work
 */
struct slot {
	u8 state;
	struct controller *ctrl;
	struct hotplug_slot *hotplug_slot;
	struct delayed_work work;
	struct mutex lock;
};

/**
 * struct controller - PCIe hotplug controller
 * @ctrl_lock: serializes writes to the Slot Control register
 * @pcie: pointer to the controller's PCIe port service device
 * @reset_lock: prevents access to the Data Link Layer Link Active bit in the
 *	Link Status register and to the Presence Detect State bit in the Slot
 *	Status register during a slot reset which may cause them to flap
 * @slot: pointer to the controller's slot structure
 * @queue: wait queue to wake up on reception of a Command Completed event,
 *	used for synchronous writes to the Slot Control register
 * @slot_cap: cached copy of the Slot Capabilities register
 * @slot_ctrl: cached copy of the Slot Control register
 * @poll_thread: thread to poll for slot events if no IRQ is available,
 *	enabled with pciehp_poll_mode module parameter
=======
 * struct controller - PCIe hotplug controller
 * @pcie: pointer to the controller's PCIe port service device
 * @slot_cap: cached copy of the Slot Capabilities register
 * @inband_presence_disabled: In-Band Presence Detect Disable supported by
 *	controller and disabled per spec recommendation (PCIe r5.0, appendix I
 *	implementation note)
 * @slot_ctrl: cached copy of the Slot Control register
 * @ctrl_lock: serializes writes to the Slot Control register
>>>>>>> upstream/android-13
 * @cmd_started: jiffies when the Slot Control register was last written;
 *	the next write is allowed 1 second later, absent a Command Completed
 *	interrupt (PCIe r4.0, sec 6.7.3.2)
 * @cmd_busy: flag set on Slot Control register write, cleared by IRQ handler
 *	on reception of a Command Completed event
<<<<<<< HEAD
 * @link_active_reporting: cached copy of Data Link Layer Link Active Reporting
 *	Capable bit in Link Capabilities register; if this bit is zero, the
 *	Data Link Layer Link Active bit in the Link Status register will never
 *	be set and the driver is thus confined to wait 1 second before assuming
 *	the link to a hotplugged device is up and accessing it
 * @notification_enabled: whether the IRQ was requested successfully
 * @power_fault_detected: whether a power fault was detected by the hardware
 *	that has not yet been cleared by the user
 * @pending_events: used by the IRQ handler to save events retrieved from the
 *	Slot Status register for later consumption by the IRQ thread
=======
 * @queue: wait queue to wake up on reception of a Command Completed event,
 *	used for synchronous writes to the Slot Control register
 * @pending_events: used by the IRQ handler to save events retrieved from the
 *	Slot Status register for later consumption by the IRQ thread
 * @notification_enabled: whether the IRQ was requested successfully
 * @power_fault_detected: whether a power fault was detected by the hardware
 *	that has not yet been cleared by the user
 * @poll_thread: thread to poll for slot events if no IRQ is available,
 *	enabled with pciehp_poll_mode module parameter
 * @state: current state machine position
 * @state_lock: protects reads and writes of @state;
 *	protects scheduling, execution and cancellation of @button_work
 * @button_work: work item to turn the slot on or off after 5 seconds
 *	in response to an Attention Button press
 * @hotplug_slot: structure registered with the PCI hotplug core
 * @reset_lock: prevents access to the Data Link Layer Link Active bit in the
 *	Link Status register and to the Presence Detect State bit in the Slot
 *	Status register during a slot reset which may cause them to flap
 * @depth: Number of additional hotplug ports in the path to the root bus,
 *	used as lock subclass for @reset_lock
>>>>>>> upstream/android-13
 * @ist_running: flag to keep user request waiting while IRQ thread is running
 * @request_result: result of last user request submitted to the IRQ thread
 * @requester: wait queue to wake up on completion of user request,
 *	used for synchronous slot enable/disable request via sysfs
<<<<<<< HEAD
 */
struct controller {
	struct mutex ctrl_lock;
	struct pcie_device *pcie;
	struct rw_semaphore reset_lock;
	struct slot *slot;
	wait_queue_head_t queue;
	u32 slot_cap;
	u16 slot_ctrl;
	struct task_struct *poll_thread;
	unsigned long cmd_started;	/* jiffies */
	unsigned int cmd_busy:1;
	unsigned int link_active_reporting:1;
	unsigned int notification_enabled:1;
	unsigned int power_fault_detected;
	atomic_t pending_events;
=======
 *
 * PCIe hotplug has a 1:1 relationship between controller and slot, hence
 * unlike other drivers, the two aren't represented by separate structures.
 */
struct controller {
	struct pcie_device *pcie;

	u32 slot_cap;				/* capabilities and quirks */
	unsigned int inband_presence_disabled:1;

	u16 slot_ctrl;				/* control register access */
	struct mutex ctrl_lock;
	unsigned long cmd_started;
	unsigned int cmd_busy:1;
	wait_queue_head_t queue;

	atomic_t pending_events;		/* event handling */
	unsigned int notification_enabled:1;
	unsigned int power_fault_detected;
	struct task_struct *poll_thread;

	u8 state;				/* state machine */
	struct mutex state_lock;
	struct delayed_work button_work;

	struct hotplug_slot hotplug_slot;	/* hotplug core interface */
	struct rw_semaphore reset_lock;
	unsigned int depth;
>>>>>>> upstream/android-13
	unsigned int ist_running;
	int request_result;
	wait_queue_head_t requester;
};

/**
 * DOC: Slot state
 *
 * @OFF_STATE: slot is powered off, no subordinate devices are enumerated
 * @BLINKINGON_STATE: slot will be powered on after the 5 second delay,
<<<<<<< HEAD
 *	green led is blinking
 * @BLINKINGOFF_STATE: slot will be powered off after the 5 second delay,
 *	green led is blinking
=======
 *	Power Indicator is blinking
 * @BLINKINGOFF_STATE: slot will be powered off after the 5 second delay,
 *	Power Indicator is blinking
>>>>>>> upstream/android-13
 * @POWERON_STATE: slot is currently powering on
 * @POWEROFF_STATE: slot is currently powering off
 * @ON_STATE: slot is powered on, subordinate devices have been enumerated
 */
#define OFF_STATE			0
#define BLINKINGON_STATE		1
#define BLINKINGOFF_STATE		2
#define POWERON_STATE			3
#define POWEROFF_STATE			4
#define ON_STATE			5

/**
 * DOC: Flags to request an action from the IRQ thread
 *
 * These are stored together with events read from the Slot Status register,
 * hence must be greater than its 16-bit width.
 *
 * %DISABLE_SLOT: Disable the slot in response to a user request via sysfs or
 *	an Attention Button press after the 5 second delay
 * %RERUN_ISR: Used by the IRQ handler to inform the IRQ thread that the
 *	hotplug port was inaccessible when the interrupt occurred, requiring
 *	that the IRQ handler is rerun by the IRQ thread after it has made the
 *	hotplug port accessible by runtime resuming its parents to D0
 */
#define DISABLE_SLOT		(1 << 16)
#define RERUN_ISR		(1 << 17)

#define ATTN_BUTTN(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_ABP)
#define POWER_CTRL(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_PCP)
#define MRL_SENS(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_MRLSP)
#define ATTN_LED(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_AIP)
#define PWR_LED(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_PIP)
<<<<<<< HEAD
#define HP_SUPR_RM(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_HPS)
#define EMI(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_EIP)
#define NO_CMD_CMPL(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_NCCS)
#define PSN(ctrl)		(((ctrl)->slot_cap & PCI_EXP_SLTCAP_PSN) >> 19)

int pciehp_sysfs_enable_slot(struct slot *slot);
int pciehp_sysfs_disable_slot(struct slot *slot);
void pciehp_request(struct controller *ctrl, int action);
void pciehp_handle_button_press(struct slot *slot);
void pciehp_handle_disable_request(struct slot *slot);
void pciehp_handle_presence_or_link_change(struct slot *slot, u32 events);
int pciehp_configure_device(struct slot *p_slot);
void pciehp_unconfigure_device(struct slot *p_slot);
=======
#define NO_CMD_CMPL(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_NCCS)
#define PSN(ctrl)		(((ctrl)->slot_cap & PCI_EXP_SLTCAP_PSN) >> 19)

void pciehp_request(struct controller *ctrl, int action);
void pciehp_handle_button_press(struct controller *ctrl);
void pciehp_handle_disable_request(struct controller *ctrl);
void pciehp_handle_presence_or_link_change(struct controller *ctrl, u32 events);
int pciehp_configure_device(struct controller *ctrl);
void pciehp_unconfigure_device(struct controller *ctrl, bool presence);
>>>>>>> upstream/android-13
void pciehp_queue_pushbutton_work(struct work_struct *work);
struct controller *pcie_init(struct pcie_device *dev);
int pcie_init_notification(struct controller *ctrl);
void pcie_shutdown_notification(struct controller *ctrl);
void pcie_clear_hotplug_events(struct controller *ctrl);
<<<<<<< HEAD
int pciehp_power_on_slot(struct slot *slot);
void pciehp_power_off_slot(struct slot *slot);
void pciehp_get_power_status(struct slot *slot, u8 *status);
void pciehp_get_attention_status(struct slot *slot, u8 *status);

void pciehp_set_attention_status(struct slot *slot, u8 status);
void pciehp_get_latch_status(struct slot *slot, u8 *status);
void pciehp_get_adapter_status(struct slot *slot, u8 *status);
int pciehp_query_power_fault(struct slot *slot);
void pciehp_green_led_on(struct slot *slot);
void pciehp_green_led_off(struct slot *slot);
void pciehp_green_led_blink(struct slot *slot);
int pciehp_check_link_status(struct controller *ctrl);
bool pciehp_check_link_active(struct controller *ctrl);
void pciehp_release_ctrl(struct controller *ctrl);
int pciehp_reset_slot(struct slot *slot, int probe);

int pciehp_set_raw_indicator_status(struct hotplug_slot *h_slot, u8 status);
int pciehp_get_raw_indicator_status(struct hotplug_slot *h_slot, u8 *status);

static inline const char *slot_name(struct slot *slot)
{
	return hotplug_slot_name(slot->hotplug_slot);
=======
void pcie_enable_interrupt(struct controller *ctrl);
void pcie_disable_interrupt(struct controller *ctrl);
int pciehp_power_on_slot(struct controller *ctrl);
void pciehp_power_off_slot(struct controller *ctrl);
void pciehp_get_power_status(struct controller *ctrl, u8 *status);

#define INDICATOR_NOOP -1	/* Leave indicator unchanged */
void pciehp_set_indicators(struct controller *ctrl, int pwr, int attn);

void pciehp_get_latch_status(struct controller *ctrl, u8 *status);
int pciehp_query_power_fault(struct controller *ctrl);
int pciehp_card_present(struct controller *ctrl);
int pciehp_card_present_or_link_active(struct controller *ctrl);
int pciehp_check_link_status(struct controller *ctrl);
int pciehp_check_link_active(struct controller *ctrl);
void pciehp_release_ctrl(struct controller *ctrl);

int pciehp_sysfs_enable_slot(struct hotplug_slot *hotplug_slot);
int pciehp_sysfs_disable_slot(struct hotplug_slot *hotplug_slot);
int pciehp_reset_slot(struct hotplug_slot *hotplug_slot, bool probe);
int pciehp_get_attention_status(struct hotplug_slot *hotplug_slot, u8 *status);
int pciehp_set_raw_indicator_status(struct hotplug_slot *h_slot, u8 status);
int pciehp_get_raw_indicator_status(struct hotplug_slot *h_slot, u8 *status);

static inline const char *slot_name(struct controller *ctrl)
{
	return hotplug_slot_name(&ctrl->hotplug_slot);
}

static inline struct controller *to_ctrl(struct hotplug_slot *hotplug_slot)
{
	return container_of(hotplug_slot, struct controller, hotplug_slot);
>>>>>>> upstream/android-13
}

#endif				/* _PCIEHP_H */
