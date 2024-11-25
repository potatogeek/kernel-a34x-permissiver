<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/*
 *	LAPB release 002
 *
 *	This code REQUIRES 2.1.15 or higher/ NET3.038
 *
<<<<<<< HEAD
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> upstream/android-13
 *	History
 *	LAPB 001	Jonathan Naylor	Started Coding
 *	LAPB 002	Jonathan Naylor	New timer architecture.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/uaccess.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <net/lapb.h>

static void lapb_t1timer_expiry(struct timer_list *);
static void lapb_t2timer_expiry(struct timer_list *);

void lapb_start_t1timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t1timer);

	lapb->t1timer.function = lapb_t1timer_expiry;
	lapb->t1timer.expires  = jiffies + lapb->t1;

<<<<<<< HEAD
=======
	lapb->t1timer_running = true;
>>>>>>> upstream/android-13
	add_timer(&lapb->t1timer);
}

void lapb_start_t2timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t2timer);

	lapb->t2timer.function = lapb_t2timer_expiry;
	lapb->t2timer.expires  = jiffies + lapb->t2;

<<<<<<< HEAD
=======
	lapb->t2timer_running = true;
>>>>>>> upstream/android-13
	add_timer(&lapb->t2timer);
}

void lapb_stop_t1timer(struct lapb_cb *lapb)
{
<<<<<<< HEAD
=======
	lapb->t1timer_running = false;
>>>>>>> upstream/android-13
	del_timer(&lapb->t1timer);
}

void lapb_stop_t2timer(struct lapb_cb *lapb)
{
<<<<<<< HEAD
=======
	lapb->t2timer_running = false;
>>>>>>> upstream/android-13
	del_timer(&lapb->t2timer);
}

int lapb_t1timer_running(struct lapb_cb *lapb)
{
<<<<<<< HEAD
	return timer_pending(&lapb->t1timer);
=======
	return lapb->t1timer_running;
>>>>>>> upstream/android-13
}

static void lapb_t2timer_expiry(struct timer_list *t)
{
	struct lapb_cb *lapb = from_timer(lapb, t, t2timer);

<<<<<<< HEAD
=======
	spin_lock_bh(&lapb->lock);
	if (timer_pending(&lapb->t2timer)) /* A new timer has been set up */
		goto out;
	if (!lapb->t2timer_running) /* The timer has been stopped */
		goto out;

>>>>>>> upstream/android-13
	if (lapb->condition & LAPB_ACK_PENDING_CONDITION) {
		lapb->condition &= ~LAPB_ACK_PENDING_CONDITION;
		lapb_timeout_response(lapb);
	}
<<<<<<< HEAD
=======
	lapb->t2timer_running = false;

out:
	spin_unlock_bh(&lapb->lock);
>>>>>>> upstream/android-13
}

static void lapb_t1timer_expiry(struct timer_list *t)
{
	struct lapb_cb *lapb = from_timer(lapb, t, t1timer);

<<<<<<< HEAD
	switch (lapb->state) {

		/*
		 *	If we are a DCE, keep going DM .. DM .. DM
		 */
		case LAPB_STATE_0:
			if (lapb->mode & LAPB_DCE)
				lapb_send_control(lapb, LAPB_DM, LAPB_POLLOFF, LAPB_RESPONSE);
=======
	spin_lock_bh(&lapb->lock);
	if (timer_pending(&lapb->t1timer)) /* A new timer has been set up */
		goto out;
	if (!lapb->t1timer_running) /* The timer has been stopped */
		goto out;

	switch (lapb->state) {

		/*
		 *	If we are a DCE, send DM up to N2 times, then switch to
		 *	STATE_1 and send SABM(E).
		 */
		case LAPB_STATE_0:
			if (lapb->mode & LAPB_DCE &&
			    lapb->n2count != lapb->n2) {
				lapb->n2count++;
				lapb_send_control(lapb, LAPB_DM, LAPB_POLLOFF, LAPB_RESPONSE);
			} else {
				lapb->state = LAPB_STATE_1;
				lapb_establish_data_link(lapb);
			}
>>>>>>> upstream/android-13
			break;

		/*
		 *	Awaiting connection state, send SABM(E), up to N2 times.
		 */
		case LAPB_STATE_1:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
				lapb_dbg(0, "(%p) S1 -> S0\n", lapb->dev);
<<<<<<< HEAD
				return;
=======
				lapb->t1timer_running = false;
				goto out;
>>>>>>> upstream/android-13
			} else {
				lapb->n2count++;
				if (lapb->mode & LAPB_EXTENDED) {
					lapb_dbg(1, "(%p) S1 TX SABME(1)\n",
						 lapb->dev);
					lapb_send_control(lapb, LAPB_SABME, LAPB_POLLON, LAPB_COMMAND);
				} else {
					lapb_dbg(1, "(%p) S1 TX SABM(1)\n",
						 lapb->dev);
					lapb_send_control(lapb, LAPB_SABM, LAPB_POLLON, LAPB_COMMAND);
				}
			}
			break;

		/*
		 *	Awaiting disconnection state, send DISC, up to N2 times.
		 */
		case LAPB_STATE_2:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_confirmation(lapb, LAPB_TIMEDOUT);
				lapb_dbg(0, "(%p) S2 -> S0\n", lapb->dev);
<<<<<<< HEAD
				return;
=======
				lapb->t1timer_running = false;
				goto out;
>>>>>>> upstream/android-13
			} else {
				lapb->n2count++;
				lapb_dbg(1, "(%p) S2 TX DISC(1)\n", lapb->dev);
				lapb_send_control(lapb, LAPB_DISC, LAPB_POLLON, LAPB_COMMAND);
			}
			break;

		/*
		 *	Data transfer state, restransmit I frames, up to N2 times.
		 */
		case LAPB_STATE_3:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_stop_t2timer(lapb);
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
				lapb_dbg(0, "(%p) S3 -> S0\n", lapb->dev);
<<<<<<< HEAD
				return;
=======
				lapb->t1timer_running = false;
				goto out;
>>>>>>> upstream/android-13
			} else {
				lapb->n2count++;
				lapb_requeue_frames(lapb);
				lapb_kick(lapb);
			}
			break;

		/*
		 *	Frame reject state, restransmit FRMR frames, up to N2 times.
		 */
		case LAPB_STATE_4:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
				lapb_dbg(0, "(%p) S4 -> S0\n", lapb->dev);
<<<<<<< HEAD
				return;
=======
				lapb->t1timer_running = false;
				goto out;
>>>>>>> upstream/android-13
			} else {
				lapb->n2count++;
				lapb_transmit_frmr(lapb);
			}
			break;
	}

	lapb_start_t1timer(lapb);
<<<<<<< HEAD
=======

out:
	spin_unlock_bh(&lapb->lock);
>>>>>>> upstream/android-13
}
