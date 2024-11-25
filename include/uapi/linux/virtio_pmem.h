<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
=======
/* SPDX-License-Identifier: (GPL-2.0 WITH Linux-syscall-note) OR BSD-3-Clause */
>>>>>>> upstream/android-13
/*
 * Definitions for virtio-pmem devices.
 *
 * Copyright (C) 2019 Red Hat, Inc.
 *
 * Author(s): Pankaj Gupta <pagupta@redhat.com>
 */

#ifndef _UAPI_LINUX_VIRTIO_PMEM_H
#define _UAPI_LINUX_VIRTIO_PMEM_H

#include <linux/types.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>

struct virtio_pmem_config {
<<<<<<< HEAD
	__u64 start;
	__u64 size;
=======
	__le64 start;
	__le64 size;
>>>>>>> upstream/android-13
};

#define VIRTIO_PMEM_REQ_TYPE_FLUSH      0

struct virtio_pmem_resp {
	/* Host return status corresponding to flush request */
<<<<<<< HEAD
	__u32 ret;
=======
	__le32 ret;
>>>>>>> upstream/android-13
};

struct virtio_pmem_req {
	/* command type */
<<<<<<< HEAD
	__u32 type;
=======
	__le32 type;
>>>>>>> upstream/android-13
};

#endif
