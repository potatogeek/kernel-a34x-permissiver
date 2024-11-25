/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BLK_MQ_VIRTIO_H
#define _LINUX_BLK_MQ_VIRTIO_H

<<<<<<< HEAD
struct blk_mq_tag_set;
struct virtio_device;

int blk_mq_virtio_map_queues(struct blk_mq_tag_set *set,
=======
struct blk_mq_queue_map;
struct virtio_device;

int blk_mq_virtio_map_queues(struct blk_mq_queue_map *qmap,
>>>>>>> upstream/android-13
		struct virtio_device *vdev, int first_vec);

#endif /* _LINUX_BLK_MQ_VIRTIO_H */
