// SPDX-License-Identifier: GPL-2.0+
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

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/pci.h>
#include "../pci.h"
=======
#define dev_fmt(fmt) "pciehp: " fmt

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pm_runtime.h>
#include <linux/pci.h>
>>>>>>> upstream/android-13
#include "pciehp.h"

/* The following routines constitute the bulk of the
   hotplug controller logic
 */

<<<<<<< HEAD
static void set_slot_off(struct controller *ctrl, struct slot *pslot)
{
	/* turn off slot, turn on Amber LED, turn off Green LED if supported*/
	if (POWER_CTRL(ctrl)) {
		pciehp_power_off_slot(pslot);
=======
#define SAFE_REMOVAL	 true
#define SURPRISE_REMOVAL false

static void set_slot_off(struct controller *ctrl)
{
	/*
	 * Turn off slot, turn on attention indicator, turn off power
	 * indicator
	 */
	if (POWER_CTRL(ctrl)) {
		pciehp_power_off_slot(ctrl);
>>>>>>> upstream/android-13

		/*
		 * After turning power off, we must wait for at least 1 second
		 * before taking any action that relies on power having been
		 * removed from the slot/adapter.
		 */
		msleep(1000);
	}

<<<<<<< HEAD
	pciehp_green_led_off(pslot);
	pciehp_set_attention_status(pslot, 1);
=======
	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
			      PCI_EXP_SLTCTL_ATTN_IND_ON);
>>>>>>> upstream/android-13
}

/**
 * board_added - Called after a board has been added to the system.
<<<<<<< HEAD
 * @p_slot: &slot where board is added
=======
 * @ctrl: PCIe hotplug controller where board is added
>>>>>>> upstream/android-13
 *
 * Turns power on for the board.
 * Configures board.
 */
<<<<<<< HEAD
static int board_added(struct slot *p_slot)
{
	int retval = 0;
	struct controller *ctrl = p_slot->ctrl;
=======
static int board_added(struct controller *ctrl)
{
	int retval = 0;
>>>>>>> upstream/android-13
	struct pci_bus *parent = ctrl->pcie->port->subordinate;

	if (POWER_CTRL(ctrl)) {
		/* Power on slot */
<<<<<<< HEAD
		retval = pciehp_power_on_slot(p_slot);
=======
		retval = pciehp_power_on_slot(ctrl);
>>>>>>> upstream/android-13
		if (retval)
			return retval;
	}

<<<<<<< HEAD
	pciehp_green_led_blink(p_slot);

	/* Check link training status */
	retval = pciehp_check_link_status(ctrl);
	if (retval) {
		ctrl_err(ctrl, "Failed to check link status\n");
		goto err_exit;
	}

	/* Check for a power fault */
	if (ctrl->power_fault_detected || pciehp_query_power_fault(p_slot)) {
		ctrl_err(ctrl, "Slot(%s): Power fault\n", slot_name(p_slot));
=======
	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_BLINK,
			      INDICATOR_NOOP);

	/* Check link training status */
	retval = pciehp_check_link_status(ctrl);
	if (retval)
		goto err_exit;

	/* Check for a power fault */
	if (ctrl->power_fault_detected || pciehp_query_power_fault(ctrl)) {
		ctrl_err(ctrl, "Slot(%s): Power fault\n", slot_name(ctrl));
>>>>>>> upstream/android-13
		retval = -EIO;
		goto err_exit;
	}

<<<<<<< HEAD
	retval = pciehp_configure_device(p_slot);
=======
	retval = pciehp_configure_device(ctrl);
>>>>>>> upstream/android-13
	if (retval) {
		if (retval != -EEXIST) {
			ctrl_err(ctrl, "Cannot add device at %04x:%02x:00\n",
				 pci_domain_nr(parent), parent->number);
			goto err_exit;
		}
	}

<<<<<<< HEAD
	pciehp_green_led_on(p_slot);
	pciehp_set_attention_status(p_slot, 0);
	return 0;

err_exit:
	set_slot_off(ctrl, p_slot);
=======
	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_ON,
			      PCI_EXP_SLTCTL_ATTN_IND_OFF);
	return 0;

err_exit:
	set_slot_off(ctrl);
>>>>>>> upstream/android-13
	return retval;
}

/**
<<<<<<< HEAD
 * remove_board - Turns off slot and LEDs
 * @p_slot: slot where board is being removed
 */
static void remove_board(struct slot *p_slot)
{
	struct controller *ctrl = p_slot->ctrl;

	pciehp_unconfigure_device(p_slot);

	if (POWER_CTRL(ctrl)) {
		pciehp_power_off_slot(p_slot);
=======
 * remove_board - Turn off slot and Power Indicator
 * @ctrl: PCIe hotplug controller where board is being removed
 * @safe_removal: whether the board is safely removed (versus surprise removed)
 */
static void remove_board(struct controller *ctrl, bool safe_removal)
{
	pciehp_unconfigure_device(ctrl, safe_removal);

	if (POWER_CTRL(ctrl)) {
		pciehp_power_off_slot(ctrl);
>>>>>>> upstream/android-13

		/*
		 * After turning power off, we must wait for at least 1 second
		 * before taking any action that relies on power having been
		 * removed from the slot/adapter.
		 */
		msleep(1000);

		/* Ignore link or presence changes caused by power off */
		atomic_and(~(PCI_EXP_SLTSTA_DLLSC | PCI_EXP_SLTSTA_PDC),
			   &ctrl->pending_events);
	}

<<<<<<< HEAD
	/* turn off Green LED */
	pciehp_green_led_off(p_slot);
}

static int pciehp_enable_slot(struct slot *slot);
static int pciehp_disable_slot(struct slot *slot);
=======
	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
			      INDICATOR_NOOP);
}

static int pciehp_enable_slot(struct controller *ctrl);
static int pciehp_disable_slot(struct controller *ctrl, bool safe_removal);
>>>>>>> upstream/android-13

void pciehp_request(struct controller *ctrl, int action)
{
	atomic_or(action, &ctrl->pending_events);
	if (!pciehp_poll_mode)
		irq_wake_thread(ctrl->pcie->irq, ctrl);
}

void pciehp_queue_pushbutton_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct slot *p_slot = container_of(work, struct slot, work.work);
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
=======
	struct controller *ctrl = container_of(work, struct controller,
					       button_work.work);

	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
>>>>>>> upstream/android-13
	case BLINKINGOFF_STATE:
		pciehp_request(ctrl, DISABLE_SLOT);
		break;
	case BLINKINGON_STATE:
		pciehp_request(ctrl, PCI_EXP_SLTSTA_PDC);
		break;
	default:
		break;
	}
<<<<<<< HEAD
	mutex_unlock(&p_slot->lock);
}

void pciehp_handle_button_press(struct slot *p_slot)
{
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case OFF_STATE:
	case ON_STATE:
		if (p_slot->state == ON_STATE) {
			p_slot->state = BLINKINGOFF_STATE;
			ctrl_info(ctrl, "Slot(%s): Powering off due to button press\n",
				  slot_name(p_slot));
		} else {
			p_slot->state = BLINKINGON_STATE;
			ctrl_info(ctrl, "Slot(%s) Powering on due to button press\n",
				  slot_name(p_slot));
		}
		/* blink green LED and turn off amber */
		pciehp_green_led_blink(p_slot);
		pciehp_set_attention_status(p_slot, 0);
		schedule_delayed_work(&p_slot->work, 5 * HZ);
=======
	mutex_unlock(&ctrl->state_lock);
}

void pciehp_handle_button_press(struct controller *ctrl)
{
	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case OFF_STATE:
	case ON_STATE:
		if (ctrl->state == ON_STATE) {
			ctrl->state = BLINKINGOFF_STATE;
			ctrl_info(ctrl, "Slot(%s): Powering off due to button press\n",
				  slot_name(ctrl));
		} else {
			ctrl->state = BLINKINGON_STATE;
			ctrl_info(ctrl, "Slot(%s) Powering on due to button press\n",
				  slot_name(ctrl));
		}
		/* blink power indicator and turn off attention */
		pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_BLINK,
				      PCI_EXP_SLTCTL_ATTN_IND_OFF);
		schedule_delayed_work(&ctrl->button_work, 5 * HZ);
>>>>>>> upstream/android-13
		break;
	case BLINKINGOFF_STATE:
	case BLINKINGON_STATE:
		/*
		 * Cancel if we are still blinking; this means that we
		 * press the attention again before the 5 sec. limit
		 * expires to cancel hot-add or hot-remove
		 */
<<<<<<< HEAD
		ctrl_info(ctrl, "Slot(%s): Button cancel\n", slot_name(p_slot));
		cancel_delayed_work(&p_slot->work);
		if (p_slot->state == BLINKINGOFF_STATE) {
			p_slot->state = ON_STATE;
			pciehp_green_led_on(p_slot);
		} else {
			p_slot->state = OFF_STATE;
			pciehp_green_led_off(p_slot);
		}
		pciehp_set_attention_status(p_slot, 0);
		ctrl_info(ctrl, "Slot(%s): Action canceled due to button press\n",
			  slot_name(p_slot));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Ignoring invalid state %#x\n",
			 slot_name(p_slot), p_slot->state);
		break;
	}
	mutex_unlock(&p_slot->lock);
}

void pciehp_handle_disable_request(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;

	mutex_lock(&slot->lock);
	switch (slot->state) {
	case BLINKINGON_STATE:
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&slot->work);
		break;
	}
	slot->state = POWEROFF_STATE;
	mutex_unlock(&slot->lock);

	ctrl->request_result = pciehp_disable_slot(slot);
}

void pciehp_handle_presence_or_link_change(struct slot *slot, u32 events)
{
	struct controller *ctrl = slot->ctrl;
	bool link_active;
	u8 present;
=======
		ctrl_info(ctrl, "Slot(%s): Button cancel\n", slot_name(ctrl));
		cancel_delayed_work(&ctrl->button_work);
		if (ctrl->state == BLINKINGOFF_STATE) {
			ctrl->state = ON_STATE;
			pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_ON,
					      PCI_EXP_SLTCTL_ATTN_IND_OFF);
		} else {
			ctrl->state = OFF_STATE;
			pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
					      PCI_EXP_SLTCTL_ATTN_IND_OFF);
		}
		ctrl_info(ctrl, "Slot(%s): Action canceled due to button press\n",
			  slot_name(ctrl));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Ignoring invalid state %#x\n",
			 slot_name(ctrl), ctrl->state);
		break;
	}
	mutex_unlock(&ctrl->state_lock);
}

void pciehp_handle_disable_request(struct controller *ctrl)
{
	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGON_STATE:
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&ctrl->button_work);
		break;
	}
	ctrl->state = POWEROFF_STATE;
	mutex_unlock(&ctrl->state_lock);

	ctrl->request_result = pciehp_disable_slot(ctrl, SAFE_REMOVAL);
}

void pciehp_handle_presence_or_link_change(struct controller *ctrl, u32 events)
{
	int present, link_active;
>>>>>>> upstream/android-13

	/*
	 * If the slot is on and presence or link has changed, turn it off.
	 * Even if it's occupied again, we cannot assume the card is the same.
	 */
<<<<<<< HEAD
	mutex_lock(&slot->lock);
	switch (slot->state) {
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&slot->work);
		/* fall through */
	case ON_STATE:
		slot->state = POWEROFF_STATE;
		mutex_unlock(&slot->lock);
		if (events & PCI_EXP_SLTSTA_DLLSC)
			ctrl_info(ctrl, "Slot(%s): Link Down\n",
				  slot_name(slot));
		if (events & PCI_EXP_SLTSTA_PDC)
			ctrl_info(ctrl, "Slot(%s): Card not present\n",
				  slot_name(slot));
		pciehp_disable_slot(slot);
		break;
	default:
		mutex_unlock(&slot->lock);
=======
	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&ctrl->button_work);
		fallthrough;
	case ON_STATE:
		ctrl->state = POWEROFF_STATE;
		mutex_unlock(&ctrl->state_lock);
		if (events & PCI_EXP_SLTSTA_DLLSC)
			ctrl_info(ctrl, "Slot(%s): Link Down\n",
				  slot_name(ctrl));
		if (events & PCI_EXP_SLTSTA_PDC)
			ctrl_info(ctrl, "Slot(%s): Card not present\n",
				  slot_name(ctrl));
		pciehp_disable_slot(ctrl, SURPRISE_REMOVAL);
		break;
	default:
		mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13
		break;
	}

	/* Turn the slot on if it's occupied or link is up */
<<<<<<< HEAD
	mutex_lock(&slot->lock);
	pciehp_get_adapter_status(slot, &present);
	link_active = pciehp_check_link_active(ctrl);
	if (!present && !link_active) {
		mutex_unlock(&slot->lock);
		return;
	}

	switch (slot->state) {
	case BLINKINGON_STATE:
		cancel_delayed_work(&slot->work);
		/* fall through */
	case OFF_STATE:
		slot->state = POWERON_STATE;
		mutex_unlock(&slot->lock);
		if (present)
			ctrl_info(ctrl, "Slot(%s): Card present\n",
				  slot_name(slot));
		if (link_active)
			ctrl_info(ctrl, "Slot(%s): Link Up\n",
				  slot_name(slot));
		ctrl->request_result = pciehp_enable_slot(slot);
		break;
	default:
		mutex_unlock(&slot->lock);
=======
	mutex_lock(&ctrl->state_lock);
	present = pciehp_card_present(ctrl);
	link_active = pciehp_check_link_active(ctrl);
	if (present <= 0 && link_active <= 0) {
		mutex_unlock(&ctrl->state_lock);
		return;
	}

	switch (ctrl->state) {
	case BLINKINGON_STATE:
		cancel_delayed_work(&ctrl->button_work);
		fallthrough;
	case OFF_STATE:
		ctrl->state = POWERON_STATE;
		mutex_unlock(&ctrl->state_lock);
		if (present)
			ctrl_info(ctrl, "Slot(%s): Card present\n",
				  slot_name(ctrl));
		if (link_active)
			ctrl_info(ctrl, "Slot(%s): Link Up\n",
				  slot_name(ctrl));
		ctrl->request_result = pciehp_enable_slot(ctrl);
		break;
	default:
		mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13
		break;
	}
}

<<<<<<< HEAD
static int __pciehp_enable_slot(struct slot *p_slot)
{
	u8 getstatus = 0;
	struct controller *ctrl = p_slot->ctrl;

	pciehp_get_adapter_status(p_slot, &getstatus);
	if (!getstatus) {
		ctrl_info(ctrl, "Slot(%s): No adapter\n", slot_name(p_slot));
		return -ENODEV;
	}
	if (MRL_SENS(p_slot->ctrl)) {
		pciehp_get_latch_status(p_slot, &getstatus);
		if (getstatus) {
			ctrl_info(ctrl, "Slot(%s): Latch open\n",
				  slot_name(p_slot));
=======
static int __pciehp_enable_slot(struct controller *ctrl)
{
	u8 getstatus = 0;

	if (MRL_SENS(ctrl)) {
		pciehp_get_latch_status(ctrl, &getstatus);
		if (getstatus) {
			ctrl_info(ctrl, "Slot(%s): Latch open\n",
				  slot_name(ctrl));
>>>>>>> upstream/android-13
			return -ENODEV;
		}
	}

<<<<<<< HEAD
	if (POWER_CTRL(p_slot->ctrl)) {
		pciehp_get_power_status(p_slot, &getstatus);
		if (getstatus) {
			ctrl_info(ctrl, "Slot(%s): Already enabled\n",
				  slot_name(p_slot));
=======
	if (POWER_CTRL(ctrl)) {
		pciehp_get_power_status(ctrl, &getstatus);
		if (getstatus) {
			ctrl_info(ctrl, "Slot(%s): Already enabled\n",
				  slot_name(ctrl));
>>>>>>> upstream/android-13
			return 0;
		}
	}

<<<<<<< HEAD
	return board_added(p_slot);
}

static int pciehp_enable_slot(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	int ret;

	pm_runtime_get_sync(&ctrl->pcie->port->dev);
	ret = __pciehp_enable_slot(slot);
	if (ret && ATTN_BUTTN(ctrl))
		pciehp_green_led_off(slot); /* may be blinking */
	pm_runtime_put(&ctrl->pcie->port->dev);

	mutex_lock(&slot->lock);
	slot->state = ret ? OFF_STATE : ON_STATE;
	mutex_unlock(&slot->lock);
=======
	return board_added(ctrl);
}

static int pciehp_enable_slot(struct controller *ctrl)
{
	int ret;

	pm_runtime_get_sync(&ctrl->pcie->port->dev);
	ret = __pciehp_enable_slot(ctrl);
	if (ret && ATTN_BUTTN(ctrl))
		/* may be blinking */
		pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
				      INDICATOR_NOOP);
	pm_runtime_put(&ctrl->pcie->port->dev);

	mutex_lock(&ctrl->state_lock);
	ctrl->state = ret ? OFF_STATE : ON_STATE;
	mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
static int __pciehp_disable_slot(struct slot *p_slot)
{
	u8 getstatus = 0;
	struct controller *ctrl = p_slot->ctrl;

	if (POWER_CTRL(p_slot->ctrl)) {
		pciehp_get_power_status(p_slot, &getstatus);
		if (!getstatus) {
			ctrl_info(ctrl, "Slot(%s): Already disabled\n",
				  slot_name(p_slot));
=======
static int __pciehp_disable_slot(struct controller *ctrl, bool safe_removal)
{
	u8 getstatus = 0;

	if (POWER_CTRL(ctrl)) {
		pciehp_get_power_status(ctrl, &getstatus);
		if (!getstatus) {
			ctrl_info(ctrl, "Slot(%s): Already disabled\n",
				  slot_name(ctrl));
>>>>>>> upstream/android-13
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	remove_board(p_slot);
	return 0;
}

static int pciehp_disable_slot(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	int ret;

	pm_runtime_get_sync(&ctrl->pcie->port->dev);
	ret = __pciehp_disable_slot(slot);
	pm_runtime_put(&ctrl->pcie->port->dev);

	mutex_lock(&slot->lock);
	slot->state = OFF_STATE;
	mutex_unlock(&slot->lock);
=======
	remove_board(ctrl, safe_removal);
	return 0;
}

static int pciehp_disable_slot(struct controller *ctrl, bool safe_removal)
{
	int ret;

	pm_runtime_get_sync(&ctrl->pcie->port->dev);
	ret = __pciehp_disable_slot(ctrl, safe_removal);
	pm_runtime_put(&ctrl->pcie->port->dev);

	mutex_lock(&ctrl->state_lock);
	ctrl->state = OFF_STATE;
	mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD
int pciehp_sysfs_enable_slot(struct slot *p_slot)
{
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case BLINKINGON_STATE:
	case OFF_STATE:
		mutex_unlock(&p_slot->lock);
=======
int pciehp_sysfs_enable_slot(struct hotplug_slot *hotplug_slot)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);

	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGON_STATE:
	case OFF_STATE:
		mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13
		/*
		 * The IRQ thread becomes a no-op if the user pulls out the
		 * card before the thread wakes up, so initialize to -ENODEV.
		 */
		ctrl->request_result = -ENODEV;
		pciehp_request(ctrl, PCI_EXP_SLTSTA_PDC);
		wait_event(ctrl->requester,
			   !atomic_read(&ctrl->pending_events) &&
			   !ctrl->ist_running);
		return ctrl->request_result;
	case POWERON_STATE:
		ctrl_info(ctrl, "Slot(%s): Already in powering on state\n",
<<<<<<< HEAD
			  slot_name(p_slot));
=======
			  slot_name(ctrl));
>>>>>>> upstream/android-13
		break;
	case BLINKINGOFF_STATE:
	case ON_STATE:
	case POWEROFF_STATE:
		ctrl_info(ctrl, "Slot(%s): Already enabled\n",
<<<<<<< HEAD
			  slot_name(p_slot));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Invalid state %#x\n",
			 slot_name(p_slot), p_slot->state);
		break;
	}
	mutex_unlock(&p_slot->lock);
=======
			  slot_name(ctrl));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Invalid state %#x\n",
			 slot_name(ctrl), ctrl->state);
		break;
	}
	mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13

	return -ENODEV;
}

<<<<<<< HEAD
int pciehp_sysfs_disable_slot(struct slot *p_slot)
{
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case BLINKINGOFF_STATE:
	case ON_STATE:
		mutex_unlock(&p_slot->lock);
=======
int pciehp_sysfs_disable_slot(struct hotplug_slot *hotplug_slot)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);

	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGOFF_STATE:
	case ON_STATE:
		mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13
		pciehp_request(ctrl, DISABLE_SLOT);
		wait_event(ctrl->requester,
			   !atomic_read(&ctrl->pending_events) &&
			   !ctrl->ist_running);
		return ctrl->request_result;
	case POWEROFF_STATE:
		ctrl_info(ctrl, "Slot(%s): Already in powering off state\n",
<<<<<<< HEAD
			  slot_name(p_slot));
=======
			  slot_name(ctrl));
>>>>>>> upstream/android-13
		break;
	case BLINKINGON_STATE:
	case OFF_STATE:
	case POWERON_STATE:
		ctrl_info(ctrl, "Slot(%s): Already disabled\n",
<<<<<<< HEAD
			  slot_name(p_slot));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Invalid state %#x\n",
			 slot_name(p_slot), p_slot->state);
		break;
	}
	mutex_unlock(&p_slot->lock);
=======
			  slot_name(ctrl));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Invalid state %#x\n",
			 slot_name(ctrl), ctrl->state);
		break;
	}
	mutex_unlock(&ctrl->state_lock);
>>>>>>> upstream/android-13

	return -ENODEV;
}
