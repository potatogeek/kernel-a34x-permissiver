<<<<<<< HEAD
/*
 * Copyright (c) 2017 Sagi Grimberg.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2017 Sagi Grimberg.
>>>>>>> upstream/android-13
 */
#include <linux/blk-mq.h>
#include <linux/blk-mq-rdma.h>
#include <rdma/ib_verbs.h>

/**
 * blk_mq_rdma_map_queues - provide a default queue mapping for rdma device
<<<<<<< HEAD
 * @set:	tagset to provide the mapping for
 * @dev:	rdma device associated with @set.
=======
 * @map:	CPU to hardware queue map.
 * @dev:	rdma device to provide a mapping for.
>>>>>>> upstream/android-13
 * @first_vec:	first interrupt vectors to use for queues (usually 0)
 *
 * This function assumes the rdma device @dev has at least as many available
 * interrupt vetors as @set has queues.  It will then query it's affinity mask
 * and built queue mapping that maps a queue to the CPUs that have irq affinity
 * for the corresponding vector.
 *
 * In case either the driver passed a @dev with less vectors than
 * @set->nr_hw_queues, or @dev does not provide an affinity mask for a
 * vector, we fallback to the naive mapping.
 */
<<<<<<< HEAD
int blk_mq_rdma_map_queues(struct blk_mq_tag_set *set,
=======
int blk_mq_rdma_map_queues(struct blk_mq_queue_map *map,
>>>>>>> upstream/android-13
		struct ib_device *dev, int first_vec)
{
	const struct cpumask *mask;
	unsigned int queue, cpu;

<<<<<<< HEAD
	for (queue = 0; queue < set->nr_hw_queues; queue++) {
=======
	for (queue = 0; queue < map->nr_queues; queue++) {
>>>>>>> upstream/android-13
		mask = ib_get_vector_affinity(dev, first_vec + queue);
		if (!mask)
			goto fallback;

		for_each_cpu(cpu, mask)
<<<<<<< HEAD
			set->mq_map[cpu] = queue;
=======
			map->mq_map[cpu] = map->queue_offset + queue;
>>>>>>> upstream/android-13
	}

	return 0;

fallback:
<<<<<<< HEAD
	return blk_mq_map_queues(set);
=======
	return blk_mq_map_queues(map);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL_GPL(blk_mq_rdma_map_queues);
