<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *
 * Copyright (c) 2009, Microsoft Corporation.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Authors:
 *   Haiyang Zhang <haiyangz@microsoft.com>
 *   Hank Janssen  <hjanssen@microsoft.com>
 *
=======
 * Authors:
 *   Haiyang Zhang <haiyangz@microsoft.com>
 *   Hank Janssen  <hjanssen@microsoft.com>
>>>>>>> upstream/android-13
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/mm.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/hyperv.h>
#include <linux/export.h>
#include <asm/mshyperv.h>

#include "hyperv_vmbus.h"


struct vmbus_connection vmbus_connection = {
	.conn_state		= DISCONNECTED,
<<<<<<< HEAD
	.next_gpadl_handle	= ATOMIC_INIT(0xE1E10),
=======
	.unload_event		= COMPLETION_INITIALIZER(
				  vmbus_connection.unload_event),
	.next_gpadl_handle	= ATOMIC_INIT(0xE1E10),

	.ready_for_suspend_event = COMPLETION_INITIALIZER(
				  vmbus_connection.ready_for_suspend_event),
	.ready_for_resume_event	= COMPLETION_INITIALIZER(
				  vmbus_connection.ready_for_resume_event),
>>>>>>> upstream/android-13
};
EXPORT_SYMBOL_GPL(vmbus_connection);

/*
 * Negotiated protocol version with the host.
 */
__u32 vmbus_proto_version;
EXPORT_SYMBOL_GPL(vmbus_proto_version);

<<<<<<< HEAD
static __u32 vmbus_get_next_version(__u32 current_version)
{
	switch (current_version) {
	case (VERSION_WIN7):
		return VERSION_WS2008;

	case (VERSION_WIN8):
		return VERSION_WIN7;

	case (VERSION_WIN8_1):
		return VERSION_WIN8;

	case (VERSION_WIN10):
		return VERSION_WIN8_1;

	case (VERSION_WIN10_V5):
		return VERSION_WIN10;

	case (VERSION_WS2008):
	default:
		return VERSION_INVAL;
	}
}

static int vmbus_negotiate_version(struct vmbus_channel_msginfo *msginfo,
					__u32 version)
{
	int ret = 0;
	unsigned int cur_cpu;
=======
/*
 * Table of VMBus versions listed from newest to oldest.
 */
static __u32 vmbus_versions[] = {
	VERSION_WIN10_V5_3,
	VERSION_WIN10_V5_2,
	VERSION_WIN10_V5_1,
	VERSION_WIN10_V5,
	VERSION_WIN10_V4_1,
	VERSION_WIN10,
	VERSION_WIN8_1,
	VERSION_WIN8,
	VERSION_WIN7,
	VERSION_WS2008
};

/*
 * Maximal VMBus protocol version guests can negotiate.  Useful to cap the
 * VMBus version for testing and debugging purpose.
 */
static uint max_version = VERSION_WIN10_V5_3;

module_param(max_version, uint, S_IRUGO);
MODULE_PARM_DESC(max_version,
		 "Maximal VMBus protocol version which can be negotiated");

int vmbus_negotiate_version(struct vmbus_channel_msginfo *msginfo, u32 version)
{
	int ret = 0;
>>>>>>> upstream/android-13
	struct vmbus_channel_initiate_contact *msg;
	unsigned long flags;

	init_completion(&msginfo->waitevent);

	msg = (struct vmbus_channel_initiate_contact *)msginfo->msg;

	memset(msg, 0, sizeof(*msg));
	msg->header.msgtype = CHANNELMSG_INITIATE_CONTACT;
	msg->vmbus_version_requested = version;

	/*
<<<<<<< HEAD
	 * VMBus protocol 5.0 (VERSION_WIN10_V5) requires that we must use
	 * VMBUS_MESSAGE_CONNECTION_ID_4 for the Initiate Contact Message,
	 * and for subsequent messages, we must use the Message Connection ID
	 * field in the host-returned Version Response Message. And, with
	 * VERSION_WIN10_V5, we don't use msg->interrupt_page, but we tell
	 * the host explicitly that we still use VMBUS_MESSAGE_SINT(2) for
=======
	 * VMBus protocol 5.0 (VERSION_WIN10_V5) and higher require that we must
	 * use VMBUS_MESSAGE_CONNECTION_ID_4 for the Initiate Contact Message,
	 * and for subsequent messages, we must use the Message Connection ID
	 * field in the host-returned Version Response Message. And, with
	 * VERSION_WIN10_V5 and higher, we don't use msg->interrupt_page, but we
	 * tell the host explicitly that we still use VMBUS_MESSAGE_SINT(2) for
>>>>>>> upstream/android-13
	 * compatibility.
	 *
	 * On old hosts, we should always use VMBUS_MESSAGE_CONNECTION_ID (1).
	 */
	if (version >= VERSION_WIN10_V5) {
		msg->msg_sint = VMBUS_MESSAGE_SINT;
		vmbus_connection.msg_conn_id = VMBUS_MESSAGE_CONNECTION_ID_4;
	} else {
		msg->interrupt_page = virt_to_phys(vmbus_connection.int_page);
		vmbus_connection.msg_conn_id = VMBUS_MESSAGE_CONNECTION_ID;
	}

	msg->monitor_page1 = virt_to_phys(vmbus_connection.monitor_pages[0]);
	msg->monitor_page2 = virt_to_phys(vmbus_connection.monitor_pages[1]);
<<<<<<< HEAD
	/*
	 * We want all channel messages to be delivered on CPU 0.
	 * This has been the behavior pre-win8. This is not
	 * perf issue and having all channel messages delivered on CPU 0
	 * would be ok.
	 * For post win8 hosts, we support receiving channel messagges on
	 * all the CPUs. This is needed for kexec to work correctly where
	 * the CPU attempting to connect may not be CPU 0.
	 */
	if (version >= VERSION_WIN8_1) {
		cur_cpu = get_cpu();
		msg->target_vcpu = hv_cpu_number_to_vp_number(cur_cpu);
		vmbus_connection.connect_cpu = cur_cpu;
		put_cpu();
	} else {
		msg->target_vcpu = 0;
		vmbus_connection.connect_cpu = 0;
	}
=======
	msg->target_vcpu = hv_cpu_number_to_vp_number(VMBUS_CONNECT_CPU);
>>>>>>> upstream/android-13

	/*
	 * Add to list before we send the request since we may
	 * receive the response before returning from this routine
	 */
	spin_lock_irqsave(&vmbus_connection.channelmsg_lock, flags);
	list_add_tail(&msginfo->msglistentry,
		      &vmbus_connection.chn_msg_list);

	spin_unlock_irqrestore(&vmbus_connection.channelmsg_lock, flags);

	ret = vmbus_post_msg(msg,
			     sizeof(struct vmbus_channel_initiate_contact),
			     true);

	trace_vmbus_negotiate_version(msg, ret);

	if (ret != 0) {
		spin_lock_irqsave(&vmbus_connection.channelmsg_lock, flags);
		list_del(&msginfo->msglistentry);
		spin_unlock_irqrestore(&vmbus_connection.channelmsg_lock,
					flags);
		return ret;
	}

	/* Wait for the connection response */
	wait_for_completion(&msginfo->waitevent);

	spin_lock_irqsave(&vmbus_connection.channelmsg_lock, flags);
	list_del(&msginfo->msglistentry);
	spin_unlock_irqrestore(&vmbus_connection.channelmsg_lock, flags);

	/* Check if successful */
	if (msginfo->response.version_response.version_supported) {
		vmbus_connection.conn_state = CONNECTED;

		if (version >= VERSION_WIN10_V5)
			vmbus_connection.msg_conn_id =
				msginfo->response.version_response.msg_conn_id;
	} else {
		return -ECONNREFUSED;
	}

	return ret;
}

/*
 * vmbus_connect - Sends a connect request on the partition service connection
 */
int vmbus_connect(void)
{
<<<<<<< HEAD
	int ret = 0;
	struct vmbus_channel_msginfo *msginfo = NULL;
=======
	struct vmbus_channel_msginfo *msginfo = NULL;
	int i, ret = 0;
>>>>>>> upstream/android-13
	__u32 version;

	/* Initialize the vmbus connection */
	vmbus_connection.conn_state = CONNECTING;
	vmbus_connection.work_queue = create_workqueue("hv_vmbus_con");
	if (!vmbus_connection.work_queue) {
		ret = -ENOMEM;
		goto cleanup;
	}

	vmbus_connection.handle_primary_chan_wq =
		create_workqueue("hv_pri_chan");
	if (!vmbus_connection.handle_primary_chan_wq) {
		ret = -ENOMEM;
		goto cleanup;
	}

	vmbus_connection.handle_sub_chan_wq =
		create_workqueue("hv_sub_chan");
	if (!vmbus_connection.handle_sub_chan_wq) {
		ret = -ENOMEM;
		goto cleanup;
	}

	INIT_LIST_HEAD(&vmbus_connection.chn_msg_list);
	spin_lock_init(&vmbus_connection.channelmsg_lock);

	INIT_LIST_HEAD(&vmbus_connection.chn_list);
	mutex_init(&vmbus_connection.channel_mutex);

	/*
	 * Setup the vmbus event connection for channel interrupt
	 * abstraction stuff
	 */
	vmbus_connection.int_page =
<<<<<<< HEAD
	(void *)__get_free_pages(GFP_KERNEL|__GFP_ZERO, 0);
=======
	(void *)hv_alloc_hyperv_zeroed_page();
>>>>>>> upstream/android-13
	if (vmbus_connection.int_page == NULL) {
		ret = -ENOMEM;
		goto cleanup;
	}

	vmbus_connection.recv_int_page = vmbus_connection.int_page;
	vmbus_connection.send_int_page =
		(void *)((unsigned long)vmbus_connection.int_page +
<<<<<<< HEAD
			(PAGE_SIZE >> 1));
=======
			(HV_HYP_PAGE_SIZE >> 1));
>>>>>>> upstream/android-13

	/*
	 * Setup the monitor notification facility. The 1st page for
	 * parent->child and the 2nd page for child->parent
	 */
<<<<<<< HEAD
	vmbus_connection.monitor_pages[0] = (void *)__get_free_pages((GFP_KERNEL|__GFP_ZERO), 0);
	vmbus_connection.monitor_pages[1] = (void *)__get_free_pages((GFP_KERNEL|__GFP_ZERO), 0);
=======
	vmbus_connection.monitor_pages[0] = (void *)hv_alloc_hyperv_zeroed_page();
	vmbus_connection.monitor_pages[1] = (void *)hv_alloc_hyperv_zeroed_page();
>>>>>>> upstream/android-13
	if ((vmbus_connection.monitor_pages[0] == NULL) ||
	    (vmbus_connection.monitor_pages[1] == NULL)) {
		ret = -ENOMEM;
		goto cleanup;
	}

	msginfo = kzalloc(sizeof(*msginfo) +
			  sizeof(struct vmbus_channel_initiate_contact),
			  GFP_KERNEL);
	if (msginfo == NULL) {
		ret = -ENOMEM;
		goto cleanup;
	}

	/*
	 * Negotiate a compatible VMBUS version number with the
	 * host. We start with the highest number we can support
	 * and work our way down until we negotiate a compatible
	 * version.
	 */

<<<<<<< HEAD
	version = VERSION_CURRENT;

	do {
=======
	for (i = 0; ; i++) {
		if (i == ARRAY_SIZE(vmbus_versions)) {
			ret = -EDOM;
			goto cleanup;
		}

		version = vmbus_versions[i];
		if (version > max_version)
			continue;

>>>>>>> upstream/android-13
		ret = vmbus_negotiate_version(msginfo, version);
		if (ret == -ETIMEDOUT)
			goto cleanup;

		if (vmbus_connection.conn_state == CONNECTED)
			break;
<<<<<<< HEAD

		version = vmbus_get_next_version(version);
	} while (version != VERSION_INVAL);

	if (version == VERSION_INVAL)
		goto cleanup;
=======
	}

	if (hv_is_isolation_supported() && version < VERSION_WIN10_V5_2) {
		pr_err("Invalid VMBus version %d.%d (expected >= %d.%d) from the host supporting isolation\n",
		       version >> 16, version & 0xFFFF, VERSION_WIN10_V5_2 >> 16, VERSION_WIN10_V5_2 & 0xFFFF);
		ret = -EINVAL;
		goto cleanup;
	}
>>>>>>> upstream/android-13

	vmbus_proto_version = version;
	pr_info("Vmbus version:%d.%d\n",
		version >> 16, version & 0xFFFF);

<<<<<<< HEAD
=======
	vmbus_connection.channels = kcalloc(MAX_CHANNEL_RELIDS,
					    sizeof(struct vmbus_channel *),
					    GFP_KERNEL);
	if (vmbus_connection.channels == NULL) {
		ret = -ENOMEM;
		goto cleanup;
	}

>>>>>>> upstream/android-13
	kfree(msginfo);
	return 0;

cleanup:
	pr_err("Unable to connect to host\n");

	vmbus_connection.conn_state = DISCONNECTED;
	vmbus_disconnect();

	kfree(msginfo);

	return ret;
}

void vmbus_disconnect(void)
{
	/*
	 * First send the unload request to the host.
	 */
	vmbus_initiate_unload(false);

	if (vmbus_connection.handle_sub_chan_wq)
		destroy_workqueue(vmbus_connection.handle_sub_chan_wq);

	if (vmbus_connection.handle_primary_chan_wq)
		destroy_workqueue(vmbus_connection.handle_primary_chan_wq);

	if (vmbus_connection.work_queue)
		destroy_workqueue(vmbus_connection.work_queue);

	if (vmbus_connection.int_page) {
<<<<<<< HEAD
		free_pages((unsigned long)vmbus_connection.int_page, 0);
		vmbus_connection.int_page = NULL;
	}

	free_pages((unsigned long)vmbus_connection.monitor_pages[0], 0);
	free_pages((unsigned long)vmbus_connection.monitor_pages[1], 0);
=======
		hv_free_hyperv_page((unsigned long)vmbus_connection.int_page);
		vmbus_connection.int_page = NULL;
	}

	hv_free_hyperv_page((unsigned long)vmbus_connection.monitor_pages[0]);
	hv_free_hyperv_page((unsigned long)vmbus_connection.monitor_pages[1]);
>>>>>>> upstream/android-13
	vmbus_connection.monitor_pages[0] = NULL;
	vmbus_connection.monitor_pages[1] = NULL;
}

/*
 * relid2channel - Get the channel object given its
 * child relative id (ie channel id)
 */
struct vmbus_channel *relid2channel(u32 relid)
{
<<<<<<< HEAD
	struct vmbus_channel *channel;
	struct vmbus_channel *found_channel  = NULL;
	struct list_head *cur, *tmp;
	struct vmbus_channel *cur_sc;

	BUG_ON(!mutex_is_locked(&vmbus_connection.channel_mutex));

	list_for_each_entry(channel, &vmbus_connection.chn_list, listentry) {
		if (channel->offermsg.child_relid == relid) {
			found_channel = channel;
			break;
		} else if (!list_empty(&channel->sc_list)) {
			/*
			 * Deal with sub-channels.
			 */
			list_for_each_safe(cur, tmp, &channel->sc_list) {
				cur_sc = list_entry(cur, struct vmbus_channel,
							sc_list);
				if (cur_sc->offermsg.child_relid == relid) {
					found_channel = cur_sc;
					break;
				}
			}
		}
	}

	return found_channel;
=======
	if (WARN_ON(relid >= MAX_CHANNEL_RELIDS))
		return NULL;
	return READ_ONCE(vmbus_connection.channels[relid]);
>>>>>>> upstream/android-13
}

/*
 * vmbus_on_event - Process a channel event notification
 *
 * For batched channels (default) optimize host to guest signaling
 * by ensuring:
 * 1. While reading the channel, we disable interrupts from host.
 * 2. Ensure that we process all posted messages from the host
 *    before returning from this callback.
 * 3. Once we return, enable signaling from the host. Once this
 *    state is set we check to see if additional packets are
 *    available to read. In this case we repeat the process.
 *    If this tasklet has been running for a long time
 *    then reschedule ourselves.
 */
void vmbus_on_event(unsigned long data)
{
	struct vmbus_channel *channel = (void *) data;
	unsigned long time_limit = jiffies + 2;

	trace_vmbus_on_event(channel);

<<<<<<< HEAD
=======
	hv_debug_delay_test(channel, INTERRUPT_DELAY);
>>>>>>> upstream/android-13
	do {
		void (*callback_fn)(void *);

		/* A channel once created is persistent even when
		 * there is no driver handling the device. An
		 * unloading driver sets the onchannel_callback to NULL.
		 */
		callback_fn = READ_ONCE(channel->onchannel_callback);
		if (unlikely(callback_fn == NULL))
			return;

		(*callback_fn)(channel->channel_callback_context);

		if (channel->callback_mode != HV_CALL_BATCHED)
			return;

		if (likely(hv_end_read(&channel->inbound) == 0))
			return;

		hv_begin_read(&channel->inbound);
	} while (likely(time_before(jiffies, time_limit)));

	/* The time limit (2 jiffies) has been reached */
	tasklet_schedule(&channel->callback_event);
}

/*
 * vmbus_post_msg - Send a msg on the vmbus's message connection
 */
int vmbus_post_msg(void *buffer, size_t buflen, bool can_sleep)
{
	struct vmbus_channel_message_header *hdr;
	union hv_connection_id conn_id;
	int ret = 0;
	int retries = 0;
	u32 usec = 1;

	conn_id.asu32 = 0;
	conn_id.u.id = vmbus_connection.msg_conn_id;

	/*
	 * hv_post_message() can have transient failures because of
	 * insufficient resources. Retry the operation a couple of
	 * times before giving up.
	 */
	while (retries < 100) {
		ret = hv_post_message(conn_id, 1, buffer, buflen);

		switch (ret) {
		case HV_STATUS_INVALID_CONNECTION_ID:
			/*
			 * See vmbus_negotiate_version(): VMBus protocol 5.0
<<<<<<< HEAD
			 * requires that we must use
=======
			 * and higher require that we must use
>>>>>>> upstream/android-13
			 * VMBUS_MESSAGE_CONNECTION_ID_4 for the Initiate
			 * Contact message, but on old hosts that only
			 * support VMBus protocol 4.0 or lower, here we get
			 * HV_STATUS_INVALID_CONNECTION_ID and we should
			 * return an error immediately without retrying.
			 */
			hdr = buffer;
			if (hdr->msgtype == CHANNELMSG_INITIATE_CONTACT)
				return -EINVAL;
			/*
			 * We could get this if we send messages too
			 * frequently.
			 */
			ret = -EAGAIN;
			break;
		case HV_STATUS_INSUFFICIENT_MEMORY:
		case HV_STATUS_INSUFFICIENT_BUFFERS:
			ret = -ENOBUFS;
			break;
		case HV_STATUS_SUCCESS:
			return ret;
		default:
			pr_err("hv_post_msg() failed; error code:%d\n", ret);
			return -EINVAL;
		}

		retries++;
		if (can_sleep && usec > 1000)
			msleep(usec / 1000);
		else if (usec < MAX_UDELAY_MS * 1000)
			udelay(usec);
		else
			mdelay(usec / 1000);

		if (retries < 22)
			usec *= 2;
	}
	return ret;
}

/*
 * vmbus_set_event - Send an event notification to the parent
 */
void vmbus_set_event(struct vmbus_channel *channel)
{
	u32 child_relid = channel->offermsg.child_relid;

	if (!channel->is_dedicated_interrupt)
		vmbus_send_interrupt(child_relid);

	++channel->sig_events;

	hv_do_fast_hypercall8(HVCALL_SIGNAL_EVENT, channel->sig_event);
}
EXPORT_SYMBOL_GPL(vmbus_set_event);
