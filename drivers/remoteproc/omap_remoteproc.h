<<<<<<< HEAD
/*
 * Remote processor messaging
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 * Copyright (C) 2011 Google, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name Texas Instruments nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=======
/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Remote processor messaging
 *
 * Copyright (C) 2011-2020 Texas Instruments, Inc.
 * Copyright (C) 2011 Google, Inc.
 * All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef _OMAP_RPMSG_H
#define _OMAP_RPMSG_H

/*
 * enum - Predefined Mailbox Messages
 *
 * @RP_MBOX_READY: informs the M3's that we're up and running. this is
 * part of the init sequence sent that the M3 expects to see immediately
 * after it is booted.
 *
 * @RP_MBOX_PENDING_MSG: informs the receiver that there is an inbound
 * message waiting in its own receive-side vring. please note that currently
 * this message is optional: alternatively, one can explicitly send the index
 * of the triggered virtqueue itself. the preferred approach will be decided
 * as we progress and experiment with those two different approaches.
 *
 * @RP_MBOX_CRASH: this message is sent if BIOS crashes
 *
 * @RP_MBOX_ECHO_REQUEST: a mailbox-level "ping" message.
 *
 * @RP_MBOX_ECHO_REPLY: a mailbox-level reply to a "ping"
 *
 * @RP_MBOX_ABORT_REQUEST: a "please crash" request, used for testing the
 * recovery mechanism (to some extent).
<<<<<<< HEAD
=======
 *
 * @RP_MBOX_SUSPEND_AUTO: auto suspend request for the remote processor
 *
 * @RP_MBOX_SUSPEND_SYSTEM: system suspend request for the remote processor
 *
 * @RP_MBOX_SUSPEND_ACK: successful response from remote processor for a
 * suspend request
 *
 * @RP_MBOX_SUSPEND_CANCEL: a cancel suspend response from a remote processor
 * on a suspend request
 *
 * Introduce new message definitions if any here.
 *
 * @RP_MBOX_END_MSG: Indicates end of known/defined messages from remote core
 * This should be the last definition.
 *
>>>>>>> upstream/android-13
 */
enum omap_rp_mbox_messages {
	RP_MBOX_READY		= 0xFFFFFF00,
	RP_MBOX_PENDING_MSG	= 0xFFFFFF01,
	RP_MBOX_CRASH		= 0xFFFFFF02,
	RP_MBOX_ECHO_REQUEST	= 0xFFFFFF03,
	RP_MBOX_ECHO_REPLY	= 0xFFFFFF04,
	RP_MBOX_ABORT_REQUEST	= 0xFFFFFF05,
<<<<<<< HEAD
=======
	RP_MBOX_SUSPEND_AUTO	= 0xFFFFFF10,
	RP_MBOX_SUSPEND_SYSTEM	= 0xFFFFFF11,
	RP_MBOX_SUSPEND_ACK	= 0xFFFFFF12,
	RP_MBOX_SUSPEND_CANCEL	= 0xFFFFFF13,
	RP_MBOX_END_MSG		= 0xFFFFFF14,
>>>>>>> upstream/android-13
};

#endif /* _OMAP_RPMSG_H */
