/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _LINUX_VHOST_H
#define _LINUX_VHOST_H
/* Userspace interface for in-kernel virtio accelerators. */

/* vhost is used to reduce the number of system calls involved in virtio.
 *
 * Existing virtio net code is used in the guest without modification.
 *
 * This header includes interface used by userspace hypervisor for
 * device configuration.
 */

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/ioctl.h>
#include <linux/virtio_config.h>
#include <linux/virtio_ring.h>

struct vhost_vring_state {
	unsigned int index;
	unsigned int num;
};

struct vhost_vring_file {
	unsigned int index;
	int fd; /* Pass -1 to unbind from file. */

};

struct vhost_vring_addr {
	unsigned int index;
	/* Option flags. */
	unsigned int flags;
	/* Flag values: */
	/* Whether log address is valid. If set enables logging. */
#define VHOST_VRING_F_LOG 0

	/* Start of array of descriptors (virtually contiguous) */
	__u64 desc_user_addr;
	/* Used structure address. Must be 32 bit aligned */
	__u64 used_user_addr;
	/* Available structure address. Must be 16 bit aligned */
	__u64 avail_user_addr;
	/* Logging support. */
	/* Log writes to used structure, at offset calculated from specified
	 * address. Address must be 32 bit aligned. */
	__u64 log_guest_addr;
};

/* no alignment requirement */
struct vhost_iotlb_msg {
	__u64 iova;
	__u64 size;
	__u64 uaddr;
#define VHOST_ACCESS_RO      0x1
#define VHOST_ACCESS_WO      0x2
#define VHOST_ACCESS_RW      0x3
	__u8 perm;
#define VHOST_IOTLB_MISS           1
#define VHOST_IOTLB_UPDATE         2
#define VHOST_IOTLB_INVALIDATE     3
#define VHOST_IOTLB_ACCESS_FAIL    4
	__u8 type;
};

#define VHOST_IOTLB_MSG 0x1
#define VHOST_IOTLB_MSG_V2 0x2

struct vhost_msg {
	int type;
	union {
		struct vhost_iotlb_msg iotlb;
		__u8 padding[64];
	};
};

struct vhost_msg_v2 {
	__u32 type;
	__u32 reserved;
	union {
		struct vhost_iotlb_msg iotlb;
		__u8 padding[64];
	};
};

struct vhost_memory_region {
	__u64 guest_phys_addr;
	__u64 memory_size; /* bytes */
	__u64 userspace_addr;
	__u64 flags_padding; /* No flags are currently specified. */
};

/* All region addresses and sizes must be 4K aligned. */
#define VHOST_PAGE_SIZE 0x1000

struct vhost_memory {
	__u32 nregions;
	__u32 padding;
	struct vhost_memory_region regions[0];
};
=======
#include <linux/vhost_types.h>
#include <linux/types.h>
#include <linux/ioctl.h>

#define VHOST_FILE_UNBIND -1
>>>>>>> upstream/android-13

/* ioctls */

#define VHOST_VIRTIO 0xAF

/* Features bitmask for forward compatibility.  Transport bits are used for
 * vhost specific features. */
#define VHOST_GET_FEATURES	_IOR(VHOST_VIRTIO, 0x00, __u64)
#define VHOST_SET_FEATURES	_IOW(VHOST_VIRTIO, 0x00, __u64)

/* Set current process as the (exclusive) owner of this file descriptor.  This
 * must be called before any other vhost command.  Further calls to
 * VHOST_OWNER_SET fail until VHOST_OWNER_RESET is called. */
#define VHOST_SET_OWNER _IO(VHOST_VIRTIO, 0x01)
/* Give up ownership, and reset the device to default values.
 * Allows subsequent call to VHOST_OWNER_SET to succeed. */
#define VHOST_RESET_OWNER _IO(VHOST_VIRTIO, 0x02)

/* Set up/modify memory layout */
#define VHOST_SET_MEM_TABLE	_IOW(VHOST_VIRTIO, 0x03, struct vhost_memory)

/* Write logging setup. */
/* Memory writes can optionally be logged by setting bit at an offset
 * (calculated from the physical address) from specified log base.
 * The bit is set using an atomic 32 bit operation. */
/* Set base address for logging. */
#define VHOST_SET_LOG_BASE _IOW(VHOST_VIRTIO, 0x04, __u64)
/* Specify an eventfd file descriptor to signal on log write. */
#define VHOST_SET_LOG_FD _IOW(VHOST_VIRTIO, 0x07, int)

/* Ring setup. */
/* Set number of descriptors in ring. This parameter can not
 * be modified while ring is running (bound to a device). */
#define VHOST_SET_VRING_NUM _IOW(VHOST_VIRTIO, 0x10, struct vhost_vring_state)
/* Set addresses for the ring. */
#define VHOST_SET_VRING_ADDR _IOW(VHOST_VIRTIO, 0x11, struct vhost_vring_addr)
/* Base value where queue looks for available descriptors */
#define VHOST_SET_VRING_BASE _IOW(VHOST_VIRTIO, 0x12, struct vhost_vring_state)
/* Get accessor: reads index, writes value in num */
#define VHOST_GET_VRING_BASE _IOWR(VHOST_VIRTIO, 0x12, struct vhost_vring_state)

/* Set the vring byte order in num. Valid values are VHOST_VRING_LITTLE_ENDIAN
 * or VHOST_VRING_BIG_ENDIAN (other values return -EINVAL).
 * The byte order cannot be changed while the device is active: trying to do so
 * returns -EBUSY.
 * This is a legacy only API that is simply ignored when VIRTIO_F_VERSION_1 is
 * set.
 * Not all kernel configurations support this ioctl, but all configurations that
 * support SET also support GET.
 */
#define VHOST_VRING_LITTLE_ENDIAN 0
#define VHOST_VRING_BIG_ENDIAN 1
#define VHOST_SET_VRING_ENDIAN _IOW(VHOST_VIRTIO, 0x13, struct vhost_vring_state)
#define VHOST_GET_VRING_ENDIAN _IOW(VHOST_VIRTIO, 0x14, struct vhost_vring_state)

/* The following ioctls use eventfd file descriptors to signal and poll
 * for events. */

/* Set eventfd to poll for added buffers */
#define VHOST_SET_VRING_KICK _IOW(VHOST_VIRTIO, 0x20, struct vhost_vring_file)
/* Set eventfd to signal when buffers have beed used */
#define VHOST_SET_VRING_CALL _IOW(VHOST_VIRTIO, 0x21, struct vhost_vring_file)
/* Set eventfd to signal an error */
#define VHOST_SET_VRING_ERR _IOW(VHOST_VIRTIO, 0x22, struct vhost_vring_file)
/* Set busy loop timeout (in us) */
#define VHOST_SET_VRING_BUSYLOOP_TIMEOUT _IOW(VHOST_VIRTIO, 0x23,	\
					 struct vhost_vring_state)
/* Get busy loop timeout (in us) */
#define VHOST_GET_VRING_BUSYLOOP_TIMEOUT _IOW(VHOST_VIRTIO, 0x24,	\
					 struct vhost_vring_state)

/* Set or get vhost backend capability */

/* Use message type V2 */
#define VHOST_BACKEND_F_IOTLB_MSG_V2 0x1
<<<<<<< HEAD
=======
/* IOTLB can accept batching hints */
#define VHOST_BACKEND_F_IOTLB_BATCH  0x2
>>>>>>> upstream/android-13

#define VHOST_SET_BACKEND_FEATURES _IOW(VHOST_VIRTIO, 0x25, __u64)
#define VHOST_GET_BACKEND_FEATURES _IOR(VHOST_VIRTIO, 0x26, __u64)

/* VHOST_NET specific defines */

/* Attach virtio net ring to a raw socket, or tap device.
 * The socket must be already bound to an ethernet device, this device will be
 * used for transmit.  Pass fd -1 to unbind from the socket and the transmit
 * device.  This can be used to stop the ring (e.g. for migration). */
#define VHOST_NET_SET_BACKEND _IOW(VHOST_VIRTIO, 0x30, struct vhost_vring_file)

<<<<<<< HEAD
/* Feature bits */
/* Log all write descriptors. Can be changed while device is active. */
#define VHOST_F_LOG_ALL 26
/* vhost-net should add virtio_net_hdr for RX, and strip for TX packets. */
#define VHOST_NET_F_VIRTIO_NET_HDR 27

/* VHOST_SCSI specific definitions */

/*
 * Used by QEMU userspace to ensure a consistent vhost-scsi ABI.
 *
 * ABI Rev 0: July 2012 version starting point for v3.6-rc merge candidate +
 *            RFC-v2 vhost-scsi userspace.  Add GET_ABI_VERSION ioctl usage
 * ABI Rev 1: January 2013. Ignore vhost_tpgt filed in struct vhost_scsi_target.
 *            All the targets under vhost_wwpn can be seen and used by guset.
 */

#define VHOST_SCSI_ABI_VERSION	1

struct vhost_scsi_target {
	int abi_version;
	char vhost_wwpn[224]; /* TRANSPORT_IQN_LEN */
	unsigned short vhost_tpgt;
	unsigned short reserved;
};
=======
/* VHOST_SCSI specific defines */
>>>>>>> upstream/android-13

#define VHOST_SCSI_SET_ENDPOINT _IOW(VHOST_VIRTIO, 0x40, struct vhost_scsi_target)
#define VHOST_SCSI_CLEAR_ENDPOINT _IOW(VHOST_VIRTIO, 0x41, struct vhost_scsi_target)
/* Changing this breaks userspace. */
#define VHOST_SCSI_GET_ABI_VERSION _IOW(VHOST_VIRTIO, 0x42, int)
/* Set and get the events missed flag */
#define VHOST_SCSI_SET_EVENTS_MISSED _IOW(VHOST_VIRTIO, 0x43, __u32)
#define VHOST_SCSI_GET_EVENTS_MISSED _IOW(VHOST_VIRTIO, 0x44, __u32)

/* VHOST_VSOCK specific defines */

#define VHOST_VSOCK_SET_GUEST_CID	_IOW(VHOST_VIRTIO, 0x60, __u64)
#define VHOST_VSOCK_SET_RUNNING		_IOW(VHOST_VIRTIO, 0x61, int)

<<<<<<< HEAD
=======
/* VHOST_VDPA specific defines */

/* Get the device id. The device ids follow the same definition of
 * the device id defined in virtio-spec.
 */
#define VHOST_VDPA_GET_DEVICE_ID	_IOR(VHOST_VIRTIO, 0x70, __u32)
/* Get and set the status. The status bits follow the same definition
 * of the device status defined in virtio-spec.
 */
#define VHOST_VDPA_GET_STATUS		_IOR(VHOST_VIRTIO, 0x71, __u8)
#define VHOST_VDPA_SET_STATUS		_IOW(VHOST_VIRTIO, 0x72, __u8)
/* Get and set the device config. The device config follows the same
 * definition of the device config defined in virtio-spec.
 */
#define VHOST_VDPA_GET_CONFIG		_IOR(VHOST_VIRTIO, 0x73, \
					     struct vhost_vdpa_config)
#define VHOST_VDPA_SET_CONFIG		_IOW(VHOST_VIRTIO, 0x74, \
					     struct vhost_vdpa_config)
/* Enable/disable the ring. */
#define VHOST_VDPA_SET_VRING_ENABLE	_IOW(VHOST_VIRTIO, 0x75, \
					     struct vhost_vring_state)
/* Get the max ring size. */
#define VHOST_VDPA_GET_VRING_NUM	_IOR(VHOST_VIRTIO, 0x76, __u16)

/* Set event fd for config interrupt*/
#define VHOST_VDPA_SET_CONFIG_CALL	_IOW(VHOST_VIRTIO, 0x77, int)

/* Get the valid iova range */
#define VHOST_VDPA_GET_IOVA_RANGE	_IOR(VHOST_VIRTIO, 0x78, \
					     struct vhost_vdpa_iova_range)
>>>>>>> upstream/android-13
#endif
