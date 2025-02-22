/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */


#ifndef _UAPI_LINUX_VIRTIO_PMEM_H
#define _UAPI_LINUX_VIRTIO_PMEM_H

#include <linux/types.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>

struct virtio_pmem_config {
	__u64 start;
	__u64 size;
};

#define VIRTIO_PMEM_REQ_TYPE_FLUSH      0

struct virtio_pmem_resp {
	
	__u32 ret;
};

struct virtio_pmem_req {
	
	__u32 type;
};

#endif
