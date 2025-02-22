/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR BSD-3-Clause) */
<<<<<<< HEAD
/******************************************************************************
 * Intel Management Engine Interface (Intel MEI) Linux driver
 * Intel MEI Interface Header
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2003 - 2012 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110,
 * USA
 *
 * The full GNU General Public License is included in this distribution
 * in the file called LICENSE.GPL.
 *
 * Contact Information:
 *	Intel Corporation.
 *	linux-mei@linux.intel.com
 *	http://www.intel.com
 *
 * BSD LICENSE
 *
 * Copyright(c) 2003 - 2012 Intel Corporation. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 *
 *****************************************************************************/

=======
/*
 * Copyright(c) 2003-2015 Intel Corporation. All rights reserved.
 * Intel Management Engine Interface (Intel MEI) Linux driver
 * Intel MEI Interface Header
 */
>>>>>>> upstream/android-13
#ifndef _LINUX_MEI_H
#define _LINUX_MEI_H

#include <linux/uuid.h>

/*
 * This IOCTL is used to associate the current file descriptor with a
 * FW Client (given by UUID). This opens a communication channel
 * between a host client and a FW client. From this point every read and write
 * will communicate with the associated FW client.
 * Only in close() (file_operation release()) the communication between
 * the clients is disconnected
 *
 * The IOCTL argument is a struct with a union that contains
 * the input parameter and the output parameter for this IOCTL.
 *
 * The input parameter is UUID of the FW Client.
 * The output parameter is the properties of the FW client
 * (FW protocol version and max message size).
 *
 */
#define IOCTL_MEI_CONNECT_CLIENT \
	_IOWR('H' , 0x01, struct mei_connect_client_data)

/*
 * Intel MEI client information struct
 */
struct mei_client {
	__u32 max_msg_length;
	__u8 protocol_version;
	__u8 reserved[3];
};

/*
 * IOCTL Connect Client Data structure
 */
struct mei_connect_client_data {
	union {
		uuid_le in_client_uuid;
		struct mei_client out_client_properties;
	};
};

/**
 * DOC: set and unset event notification for a connected client
 *
 * The IOCTL argument is 1 for enabling event notification and 0 for
 * disabling the service
 * Return:  -EOPNOTSUPP if the devices doesn't support the feature
 */
#define IOCTL_MEI_NOTIFY_SET _IOW('H', 0x02, __u32)

/**
 * DOC: retrieve notification
 *
 * The IOCTL output argument is 1 if an event was is pending and 0 otherwise
 * the ioctl has to be called in order to acknowledge pending event
 *
 * Return:  -EOPNOTSUPP if the devices doesn't support the feature
 */
#define IOCTL_MEI_NOTIFY_GET _IOR('H', 0x03, __u32)

<<<<<<< HEAD
=======
/**
 * struct mei_connect_client_vtag - mei client information struct with vtag
 *
 * @in_client_uuid: UUID of client to connect
 * @vtag: virtual tag
 * @reserved: reserved for future use
 */
struct mei_connect_client_vtag {
	uuid_le in_client_uuid;
	__u8 vtag;
	__u8 reserved[3];
};

/**
 * struct mei_connect_client_data_vtag - IOCTL connect data union
 *
 * @connect: input connect data
 * @out_client_properties: output client data
 */
struct mei_connect_client_data_vtag {
	union {
		struct mei_connect_client_vtag connect;
		struct mei_client out_client_properties;
	};
};

/**
 * DOC:
 * This IOCTL is used to associate the current file descriptor with a
 * FW Client (given by UUID), and virtual tag (vtag).
 * The IOCTL opens a communication channel between a host client and
 * a FW client on a tagged channel. From this point on, every read
 * and write will communicate with the associated FW client with
 * on the tagged channel.
 * Upone close() the communication is terminated.
 *
 * The IOCTL argument is a struct with a union that contains
 * the input parameter and the output parameter for this IOCTL.
 *
 * The input parameter is UUID of the FW Client, a vtag [0,255]
 * The output parameter is the properties of the FW client
 * (FW protocool version and max message size).
 *
 * Clients that do not support tagged connection
 * will respond with -EOPNOTSUPP.
 */
#define IOCTL_MEI_CONNECT_CLIENT_VTAG \
	_IOWR('H', 0x04, struct mei_connect_client_data_vtag)

>>>>>>> upstream/android-13
#endif /* _LINUX_MEI_H  */
