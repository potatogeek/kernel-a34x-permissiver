/*
 * Copyright 2014 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <linux/ratelimit.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/sched.h>
#include "kfd_priv.h"
#include "kfd_device_queue_manager.h"
#include "kfd_mqd_manager.h"
#include "cik_regs.h"
#include "kfd_kernel_queue.h"
<<<<<<< HEAD
=======
#include "amdgpu_amdkfd.h"
>>>>>>> upstream/android-13

/* Size of the per-pipe EOP queue */
#define CIK_HPD_EOP_BYTES_LOG2 11
#define CIK_HPD_EOP_BYTES (1U << CIK_HPD_EOP_BYTES_LOG2)

static int set_pasid_vmid_mapping(struct device_queue_manager *dqm,
<<<<<<< HEAD
					unsigned int pasid, unsigned int vmid);

static int create_compute_queue_nocpsch(struct device_queue_manager *dqm,
					struct queue *q,
					struct qcm_process_device *qpd);
=======
				  u32 pasid, unsigned int vmid);
>>>>>>> upstream/android-13

static int execute_queues_cpsch(struct device_queue_manager *dqm,
				enum kfd_unmap_queues_filter filter,
				uint32_t filter_param);
static int unmap_queues_cpsch(struct device_queue_manager *dqm,
				enum kfd_unmap_queues_filter filter,
				uint32_t filter_param);

static int map_queues_cpsch(struct device_queue_manager *dqm);

<<<<<<< HEAD
static int create_sdma_queue_nocpsch(struct device_queue_manager *dqm,
					struct queue *q,
					struct qcm_process_device *qpd);

static void deallocate_sdma_queue(struct device_queue_manager *dqm,
				unsigned int sdma_queue_id);

=======
static void deallocate_sdma_queue(struct device_queue_manager *dqm,
				struct queue *q);

static inline void deallocate_hqd(struct device_queue_manager *dqm,
				struct queue *q);
static int allocate_hqd(struct device_queue_manager *dqm, struct queue *q);
static int allocate_sdma_queue(struct device_queue_manager *dqm,
				struct queue *q);
>>>>>>> upstream/android-13
static void kfd_process_hw_exception(struct work_struct *work);

static inline
enum KFD_MQD_TYPE get_mqd_type_from_queue_type(enum kfd_queue_type type)
{
<<<<<<< HEAD
	if (type == KFD_QUEUE_TYPE_SDMA)
=======
	if (type == KFD_QUEUE_TYPE_SDMA || type == KFD_QUEUE_TYPE_SDMA_XGMI)
>>>>>>> upstream/android-13
		return KFD_MQD_TYPE_SDMA;
	return KFD_MQD_TYPE_CP;
}

static bool is_pipe_enabled(struct device_queue_manager *dqm, int mec, int pipe)
{
	int i;
<<<<<<< HEAD
	int pipe_offset = mec * dqm->dev->shared_resources.num_pipe_per_mec
		+ pipe * dqm->dev->shared_resources.num_queue_per_pipe;
=======
	int pipe_offset = (mec * dqm->dev->shared_resources.num_pipe_per_mec
		+ pipe) * dqm->dev->shared_resources.num_queue_per_pipe;
>>>>>>> upstream/android-13

	/* queue is available for KFD usage if bit is 1 */
	for (i = 0; i <  dqm->dev->shared_resources.num_queue_per_pipe; ++i)
		if (test_bit(pipe_offset + i,
<<<<<<< HEAD
			      dqm->dev->shared_resources.queue_bitmap))
=======
			      dqm->dev->shared_resources.cp_queue_bitmap))
>>>>>>> upstream/android-13
			return true;
	return false;
}

<<<<<<< HEAD
unsigned int get_queues_num(struct device_queue_manager *dqm)
{
	return bitmap_weight(dqm->dev->shared_resources.queue_bitmap,
=======
unsigned int get_cp_queues_num(struct device_queue_manager *dqm)
{
	return bitmap_weight(dqm->dev->shared_resources.cp_queue_bitmap,
>>>>>>> upstream/android-13
				KGD_MAX_QUEUES);
}

unsigned int get_queues_per_pipe(struct device_queue_manager *dqm)
{
	return dqm->dev->shared_resources.num_queue_per_pipe;
}

unsigned int get_pipes_per_mec(struct device_queue_manager *dqm)
{
	return dqm->dev->shared_resources.num_pipe_per_mec;
}

static unsigned int get_num_sdma_engines(struct device_queue_manager *dqm)
{
	return dqm->dev->device_info->num_sdma_engines;
}

<<<<<<< HEAD
unsigned int get_num_sdma_queues(struct device_queue_manager *dqm)
{
	return dqm->dev->device_info->num_sdma_engines
			* KFD_SDMA_QUEUES_PER_ENGINE;
=======
static unsigned int get_num_xgmi_sdma_engines(struct device_queue_manager *dqm)
{
	return dqm->dev->device_info->num_xgmi_sdma_engines;
}

static unsigned int get_num_all_sdma_engines(struct device_queue_manager *dqm)
{
	return get_num_sdma_engines(dqm) + get_num_xgmi_sdma_engines(dqm);
}

unsigned int get_num_sdma_queues(struct device_queue_manager *dqm)
{
	return dqm->dev->device_info->num_sdma_engines
			* dqm->dev->device_info->num_sdma_queues_per_engine;
}

unsigned int get_num_xgmi_sdma_queues(struct device_queue_manager *dqm)
{
	return dqm->dev->device_info->num_xgmi_sdma_engines
			* dqm->dev->device_info->num_sdma_queues_per_engine;
>>>>>>> upstream/android-13
}

void program_sh_mem_settings(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd)
{
	return dqm->dev->kfd2kgd->program_sh_mem_settings(
						dqm->dev->kgd, qpd->vmid,
						qpd->sh_mem_config,
						qpd->sh_mem_ape1_base,
						qpd->sh_mem_ape1_limit,
						qpd->sh_mem_bases);
}

<<<<<<< HEAD
=======
static void increment_queue_count(struct device_queue_manager *dqm,
				  struct qcm_process_device *qpd,
				  struct queue *q)
{
	dqm->active_queue_count++;
	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE ||
	    q->properties.type == KFD_QUEUE_TYPE_DIQ)
		dqm->active_cp_queue_count++;

	if (q->properties.is_gws) {
		dqm->gws_queue_count++;
		qpd->mapped_gws_queue = true;
	}
}

static void decrement_queue_count(struct device_queue_manager *dqm,
				  struct qcm_process_device *qpd,
				  struct queue *q)
{
	dqm->active_queue_count--;
	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE ||
	    q->properties.type == KFD_QUEUE_TYPE_DIQ)
		dqm->active_cp_queue_count--;

	if (q->properties.is_gws) {
		dqm->gws_queue_count--;
		qpd->mapped_gws_queue = false;
	}
}

>>>>>>> upstream/android-13
static int allocate_doorbell(struct qcm_process_device *qpd, struct queue *q)
{
	struct kfd_dev *dev = qpd->dqm->dev;

	if (!KFD_IS_SOC15(dev->device_info->asic_family)) {
		/* On pre-SOC15 chips we need to use the queue ID to
		 * preserve the user mode ABI.
		 */
		q->doorbell_id = q->properties.queue_id;
<<<<<<< HEAD
	} else if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
		/* For SDMA queues on SOC15, use static doorbell
		 * assignments based on the engine and queue.
		 */
		q->doorbell_id = dev->shared_resources.sdma_doorbell
			[q->properties.sdma_engine_id]
			[q->properties.sdma_queue_id];
=======
	} else if (q->properties.type == KFD_QUEUE_TYPE_SDMA ||
			q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI) {
		/* For SDMA queues on SOC15 with 8-byte doorbell, use static
		 * doorbell assignments based on the engine and queue id.
		 * The doobell index distance between RLC (2*i) and (2*i+1)
		 * for a SDMA engine is 512.
		 */
		uint32_t *idx_offset =
				dev->shared_resources.sdma_doorbell_idx;

		q->doorbell_id = idx_offset[q->properties.sdma_engine_id]
			+ (q->properties.sdma_queue_id & 1)
			* KFD_QUEUE_DOORBELL_MIRROR_OFFSET
			+ (q->properties.sdma_queue_id >> 1);
>>>>>>> upstream/android-13
	} else {
		/* For CP queues on SOC15 reserve a free doorbell ID */
		unsigned int found;

		found = find_first_zero_bit(qpd->doorbell_bitmap,
					    KFD_MAX_NUM_OF_QUEUES_PER_PROCESS);
		if (found >= KFD_MAX_NUM_OF_QUEUES_PER_PROCESS) {
			pr_debug("No doorbells available");
			return -EBUSY;
		}
		set_bit(found, qpd->doorbell_bitmap);
		q->doorbell_id = found;
	}

	q->properties.doorbell_off =
<<<<<<< HEAD
		kfd_doorbell_id_to_offset(dev, q->process,
					  q->doorbell_id);

=======
		kfd_get_doorbell_dw_offset_in_bar(dev, qpd_to_pdd(qpd),
					  q->doorbell_id);
>>>>>>> upstream/android-13
	return 0;
}

static void deallocate_doorbell(struct qcm_process_device *qpd,
				struct queue *q)
{
	unsigned int old;
	struct kfd_dev *dev = qpd->dqm->dev;

	if (!KFD_IS_SOC15(dev->device_info->asic_family) ||
<<<<<<< HEAD
	    q->properties.type == KFD_QUEUE_TYPE_SDMA)
=======
	    q->properties.type == KFD_QUEUE_TYPE_SDMA ||
	    q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)
>>>>>>> upstream/android-13
		return;

	old = test_and_clear_bit(q->doorbell_id, qpd->doorbell_bitmap);
	WARN_ON(!old);
}

<<<<<<< HEAD
=======
static void program_trap_handler_settings(struct device_queue_manager *dqm,
				struct qcm_process_device *qpd)
{
	if (dqm->dev->kfd2kgd->program_trap_handler_settings)
		dqm->dev->kfd2kgd->program_trap_handler_settings(
						dqm->dev->kgd, qpd->vmid,
						qpd->tba_addr, qpd->tma_addr);
}

>>>>>>> upstream/android-13
static int allocate_vmid(struct device_queue_manager *dqm,
			struct qcm_process_device *qpd,
			struct queue *q)
{
<<<<<<< HEAD
	int bit, allocated_vmid;

	if (dqm->vmid_bitmap == 0)
		return -ENOMEM;

	bit = ffs(dqm->vmid_bitmap) - 1;
	dqm->vmid_bitmap &= ~(1 << bit);

	allocated_vmid = bit + dqm->dev->vm_info.first_vmid_kfd;
	pr_debug("vmid allocation %d\n", allocated_vmid);
	qpd->vmid = allocated_vmid;
	q->properties.vmid = allocated_vmid;

	set_pasid_vmid_mapping(dqm, q->process->pasid, q->properties.vmid);
	program_sh_mem_settings(dqm, qpd);

=======
	int allocated_vmid = -1, i;

	for (i = dqm->dev->vm_info.first_vmid_kfd;
			i <= dqm->dev->vm_info.last_vmid_kfd; i++) {
		if (!dqm->vmid_pasid[i]) {
			allocated_vmid = i;
			break;
		}
	}

	if (allocated_vmid < 0) {
		pr_err("no more vmid to allocate\n");
		return -ENOSPC;
	}

	pr_debug("vmid allocated: %d\n", allocated_vmid);

	dqm->vmid_pasid[allocated_vmid] = q->process->pasid;

	set_pasid_vmid_mapping(dqm, q->process->pasid, allocated_vmid);

	qpd->vmid = allocated_vmid;
	q->properties.vmid = allocated_vmid;

	program_sh_mem_settings(dqm, qpd);

	if (dqm->dev->device_info->asic_family >= CHIP_VEGA10 &&
	    dqm->dev->cwsr_enabled)
		program_trap_handler_settings(dqm, qpd);

>>>>>>> upstream/android-13
	/* qpd->page_table_base is set earlier when register_process()
	 * is called, i.e. when the first queue is created.
	 */
	dqm->dev->kfd2kgd->set_vm_context_page_table_base(dqm->dev->kgd,
			qpd->vmid,
			qpd->page_table_base);
	/* invalidate the VM context after pasid and vmid mapping is set up */
<<<<<<< HEAD
	kfd_flush_tlb(qpd_to_pdd(qpd));
=======
	kfd_flush_tlb(qpd_to_pdd(qpd), TLB_FLUSH_LEGACY);

	if (dqm->dev->kfd2kgd->set_scratch_backing_va)
		dqm->dev->kfd2kgd->set_scratch_backing_va(dqm->dev->kgd,
				qpd->sh_hidden_private_base, qpd->vmid);
>>>>>>> upstream/android-13

	return 0;
}

static int flush_texture_cache_nocpsch(struct kfd_dev *kdev,
				struct qcm_process_device *qpd)
{
<<<<<<< HEAD
	const struct packet_manager_funcs *pmf = qpd->dqm->packets.pmf;
=======
	const struct packet_manager_funcs *pmf = qpd->dqm->packet_mgr.pmf;
>>>>>>> upstream/android-13
	int ret;

	if (!qpd->ib_kaddr)
		return -ENOMEM;

	ret = pmf->release_mem(qpd->ib_base, (uint32_t *)qpd->ib_kaddr);
	if (ret)
		return ret;

<<<<<<< HEAD
	return kdev->kfd2kgd->submit_ib(kdev->kgd, KGD_ENGINE_MEC1, qpd->vmid,
=======
	return amdgpu_amdkfd_submit_ib(kdev->kgd, KGD_ENGINE_MEC1, qpd->vmid,
>>>>>>> upstream/android-13
				qpd->ib_base, (uint32_t *)qpd->ib_kaddr,
				pmf->release_mem_size / sizeof(uint32_t));
}

static void deallocate_vmid(struct device_queue_manager *dqm,
				struct qcm_process_device *qpd,
				struct queue *q)
{
<<<<<<< HEAD
	int bit = qpd->vmid - dqm->dev->vm_info.first_vmid_kfd;

=======
>>>>>>> upstream/android-13
	/* On GFX v7, CP doesn't flush TC at dequeue */
	if (q->device->device_info->asic_family == CHIP_HAWAII)
		if (flush_texture_cache_nocpsch(q->device, qpd))
			pr_err("Failed to flush TC\n");

<<<<<<< HEAD
	kfd_flush_tlb(qpd_to_pdd(qpd));

	/* Release the vmid mapping */
	set_pasid_vmid_mapping(dqm, 0, qpd->vmid);

	dqm->vmid_bitmap |= (1 << bit);
=======
	kfd_flush_tlb(qpd_to_pdd(qpd), TLB_FLUSH_LEGACY);

	/* Release the vmid mapping */
	set_pasid_vmid_mapping(dqm, 0, qpd->vmid);
	dqm->vmid_pasid[qpd->vmid] = 0;

>>>>>>> upstream/android-13
	qpd->vmid = 0;
	q->properties.vmid = 0;
}

static int create_queue_nocpsch(struct device_queue_manager *dqm,
				struct queue *q,
				struct qcm_process_device *qpd)
{
<<<<<<< HEAD
	int retval;

	print_queue(q);

=======
	struct mqd_manager *mqd_mgr;
	int retval;

>>>>>>> upstream/android-13
	dqm_lock(dqm);

	if (dqm->total_queue_count >= max_num_of_queues_per_device) {
		pr_warn("Can't create new usermode queue because %d queues were already created\n",
				dqm->total_queue_count);
		retval = -EPERM;
		goto out_unlock;
	}

	if (list_empty(&qpd->queues_list)) {
		retval = allocate_vmid(dqm, qpd, q);
		if (retval)
			goto out_unlock;
	}
	q->properties.vmid = qpd->vmid;
	/*
<<<<<<< HEAD
	 * Eviction state logic: we only mark active queues as evicted
	 * to avoid the overhead of restoring inactive queues later
	 */
	if (qpd->evicted)
		q->properties.is_evicted = (q->properties.queue_size > 0 &&
					    q->properties.queue_percent > 0 &&
					    q->properties.queue_address != 0);
=======
	 * Eviction state logic: mark all queues as evicted, even ones
	 * not currently active. Restoring inactive queues later only
	 * updates the is_evicted flag but is a no-op otherwise.
	 */
	q->properties.is_evicted = !!qpd->evicted;
>>>>>>> upstream/android-13

	q->properties.tba_addr = qpd->tba_addr;
	q->properties.tma_addr = qpd->tma_addr;

<<<<<<< HEAD
	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE)
		retval = create_compute_queue_nocpsch(dqm, q, qpd);
	else if (q->properties.type == KFD_QUEUE_TYPE_SDMA)
		retval = create_sdma_queue_nocpsch(dqm, q, qpd);
	else
		retval = -EINVAL;

	if (retval) {
		if (list_empty(&qpd->queues_list))
			deallocate_vmid(dqm, qpd, q);
		goto out_unlock;
	}

	list_add(&q->list, &qpd->queues_list);
	qpd->queue_count++;
	if (q->properties.is_active)
		dqm->queue_count++;

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA)
		dqm->sdma_queue_count++;
=======
	mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
			q->properties.type)];
	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE) {
		retval = allocate_hqd(dqm, q);
		if (retval)
			goto deallocate_vmid;
		pr_debug("Loading mqd to hqd on pipe %d, queue %d\n",
			q->pipe, q->queue);
	} else if (q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI) {
		retval = allocate_sdma_queue(dqm, q);
		if (retval)
			goto deallocate_vmid;
		dqm->asic_ops.init_sdma_vm(dqm, q, qpd);
	}

	retval = allocate_doorbell(qpd, q);
	if (retval)
		goto out_deallocate_hqd;

	/* Temporarily release dqm lock to avoid a circular lock dependency */
	dqm_unlock(dqm);
	q->mqd_mem_obj = mqd_mgr->allocate_mqd(mqd_mgr->dev, &q->properties);
	dqm_lock(dqm);

	if (!q->mqd_mem_obj) {
		retval = -ENOMEM;
		goto out_deallocate_doorbell;
	}
	mqd_mgr->init_mqd(mqd_mgr, &q->mqd, q->mqd_mem_obj,
				&q->gart_mqd_addr, &q->properties);
	if (q->properties.is_active) {
		if (!dqm->sched_running) {
			WARN_ONCE(1, "Load non-HWS mqd while stopped\n");
			goto add_queue_to_list;
		}

		if (WARN(q->process->mm != current->mm,
					"should only run in user thread"))
			retval = -EFAULT;
		else
			retval = mqd_mgr->load_mqd(mqd_mgr, q->mqd, q->pipe,
					q->queue, &q->properties, current->mm);
		if (retval)
			goto out_free_mqd;
	}

add_queue_to_list:
	list_add(&q->list, &qpd->queues_list);
	qpd->queue_count++;
	if (q->properties.is_active)
		increment_queue_count(dqm, qpd, q);
>>>>>>> upstream/android-13

	/*
	 * Unconditionally increment this counter, regardless of the queue's
	 * type or whether the queue is active.
	 */
	dqm->total_queue_count++;
	pr_debug("Total of %d queues are accountable so far\n",
			dqm->total_queue_count);
<<<<<<< HEAD

=======
	goto out_unlock;

out_free_mqd:
	mqd_mgr->free_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);
out_deallocate_doorbell:
	deallocate_doorbell(qpd, q);
out_deallocate_hqd:
	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE)
		deallocate_hqd(dqm, q);
	else if (q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)
		deallocate_sdma_queue(dqm, q);
deallocate_vmid:
	if (list_empty(&qpd->queues_list))
		deallocate_vmid(dqm, qpd, q);
>>>>>>> upstream/android-13
out_unlock:
	dqm_unlock(dqm);
	return retval;
}

static int allocate_hqd(struct device_queue_manager *dqm, struct queue *q)
{
	bool set;
	int pipe, bit, i;

	set = false;

	for (pipe = dqm->next_pipe_to_allocate, i = 0;
			i < get_pipes_per_mec(dqm);
			pipe = ((pipe + 1) % get_pipes_per_mec(dqm)), ++i) {

		if (!is_pipe_enabled(dqm, 0, pipe))
			continue;

		if (dqm->allocated_queues[pipe] != 0) {
			bit = ffs(dqm->allocated_queues[pipe]) - 1;
			dqm->allocated_queues[pipe] &= ~(1 << bit);
			q->pipe = pipe;
			q->queue = bit;
			set = true;
			break;
		}
	}

	if (!set)
		return -EBUSY;

	pr_debug("hqd slot - pipe %d, queue %d\n", q->pipe, q->queue);
	/* horizontal hqd allocation */
	dqm->next_pipe_to_allocate = (pipe + 1) % get_pipes_per_mec(dqm);

	return 0;
}

static inline void deallocate_hqd(struct device_queue_manager *dqm,
				struct queue *q)
{
	dqm->allocated_queues[q->pipe] |= (1 << q->queue);
}

<<<<<<< HEAD
static int create_compute_queue_nocpsch(struct device_queue_manager *dqm,
					struct queue *q,
					struct qcm_process_device *qpd)
{
	struct mqd_manager *mqd_mgr;
	int retval;

	mqd_mgr = dqm->ops.get_mqd_manager(dqm, KFD_MQD_TYPE_COMPUTE);
	if (!mqd_mgr)
		return -ENOMEM;

	retval = allocate_hqd(dqm, q);
	if (retval)
		return retval;

	retval = allocate_doorbell(qpd, q);
	if (retval)
		goto out_deallocate_hqd;

	retval = mqd_mgr->init_mqd(mqd_mgr, &q->mqd, &q->mqd_mem_obj,
				&q->gart_mqd_addr, &q->properties);
	if (retval)
		goto out_deallocate_doorbell;

	pr_debug("Loading mqd to hqd on pipe %d, queue %d\n",
			q->pipe, q->queue);

	dqm->dev->kfd2kgd->set_scratch_backing_va(
			dqm->dev->kgd, qpd->sh_hidden_private_base, qpd->vmid);

	if (!q->properties.is_active)
		return 0;

	if (WARN(q->process->mm != current->mm,
		 "should only run in user thread"))
		retval = -EFAULT;
	else
		retval = mqd_mgr->load_mqd(mqd_mgr, q->mqd, q->pipe, q->queue,
					   &q->properties, current->mm);
	if (retval)
		goto out_uninit_mqd;

	return 0;

out_uninit_mqd:
	mqd_mgr->uninit_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);
out_deallocate_doorbell:
	deallocate_doorbell(qpd, q);
out_deallocate_hqd:
	deallocate_hqd(dqm, q);

	return retval;
}

=======
>>>>>>> upstream/android-13
/* Access to DQM has to be locked before calling destroy_queue_nocpsch_locked
 * to avoid asynchronized access
 */
static int destroy_queue_nocpsch_locked(struct device_queue_manager *dqm,
				struct qcm_process_device *qpd,
				struct queue *q)
{
	int retval;
	struct mqd_manager *mqd_mgr;

<<<<<<< HEAD
	mqd_mgr = dqm->ops.get_mqd_manager(dqm,
		get_mqd_type_from_queue_type(q->properties.type));
	if (!mqd_mgr)
		return -ENOMEM;

	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE) {
		deallocate_hqd(dqm, q);
	} else if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
		dqm->sdma_queue_count--;
		deallocate_sdma_queue(dqm, q->sdma_id);
	} else {
=======
	mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
			q->properties.type)];

	if (q->properties.type == KFD_QUEUE_TYPE_COMPUTE)
		deallocate_hqd(dqm, q);
	else if (q->properties.type == KFD_QUEUE_TYPE_SDMA)
		deallocate_sdma_queue(dqm, q);
	else if (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)
		deallocate_sdma_queue(dqm, q);
	else {
>>>>>>> upstream/android-13
		pr_debug("q->properties.type %d is invalid\n",
				q->properties.type);
		return -EINVAL;
	}
	dqm->total_queue_count--;

	deallocate_doorbell(qpd, q);

<<<<<<< HEAD
=======
	if (!dqm->sched_running) {
		WARN_ONCE(1, "Destroy non-HWS queue while stopped\n");
		return 0;
	}

>>>>>>> upstream/android-13
	retval = mqd_mgr->destroy_mqd(mqd_mgr, q->mqd,
				KFD_PREEMPT_TYPE_WAVEFRONT_RESET,
				KFD_UNMAP_LATENCY_MS,
				q->pipe, q->queue);
	if (retval == -ETIME)
		qpd->reset_wavefronts = true;

<<<<<<< HEAD
	mqd_mgr->uninit_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);

=======
>>>>>>> upstream/android-13
	list_del(&q->list);
	if (list_empty(&qpd->queues_list)) {
		if (qpd->reset_wavefronts) {
			pr_warn("Resetting wave fronts (nocpsch) on dev %p\n",
					dqm->dev);
			/* dbgdev_wave_reset_wavefronts has to be called before
			 * deallocate_vmid(), i.e. when vmid is still in use.
			 */
			dbgdev_wave_reset_wavefronts(dqm->dev,
					qpd->pqm->process);
			qpd->reset_wavefronts = false;
		}

		deallocate_vmid(dqm, qpd, q);
	}
	qpd->queue_count--;
	if (q->properties.is_active)
<<<<<<< HEAD
		dqm->queue_count--;
=======
		decrement_queue_count(dqm, qpd, q);
>>>>>>> upstream/android-13

	return retval;
}

static int destroy_queue_nocpsch(struct device_queue_manager *dqm,
				struct qcm_process_device *qpd,
				struct queue *q)
{
	int retval;
<<<<<<< HEAD

	dqm_lock(dqm);
	retval = destroy_queue_nocpsch_locked(dqm, qpd, q);
	dqm_unlock(dqm);

=======
	uint64_t sdma_val = 0;
	struct kfd_process_device *pdd = qpd_to_pdd(qpd);
	struct mqd_manager *mqd_mgr =
		dqm->mqd_mgrs[get_mqd_type_from_queue_type(q->properties.type)];

	/* Get the SDMA queue stats */
	if ((q->properties.type == KFD_QUEUE_TYPE_SDMA) ||
	    (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)) {
		retval = read_sdma_queue_counter((uint64_t __user *)q->properties.read_ptr,
							&sdma_val);
		if (retval)
			pr_err("Failed to read SDMA queue counter for queue: %d\n",
				q->properties.queue_id);
	}

	dqm_lock(dqm);
	retval = destroy_queue_nocpsch_locked(dqm, qpd, q);
	if (!retval)
		pdd->sdma_past_activity_counter += sdma_val;
	dqm_unlock(dqm);

	mqd_mgr->free_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);

>>>>>>> upstream/android-13
	return retval;
}

static int update_queue(struct device_queue_manager *dqm, struct queue *q)
{
<<<<<<< HEAD
	int retval;
=======
	int retval = 0;
>>>>>>> upstream/android-13
	struct mqd_manager *mqd_mgr;
	struct kfd_process_device *pdd;
	bool prev_active = false;

	dqm_lock(dqm);
	pdd = kfd_get_process_device_data(q->device, q->process);
	if (!pdd) {
		retval = -ENODEV;
		goto out_unlock;
	}
<<<<<<< HEAD
	mqd_mgr = dqm->ops.get_mqd_manager(dqm,
			get_mqd_type_from_queue_type(q->properties.type));
	if (!mqd_mgr) {
		retval = -ENOMEM;
		goto out_unlock;
	}
	/*
	 * Eviction state logic: we only mark active queues as evicted
	 * to avoid the overhead of restoring inactive queues later
	 */
	if (pdd->qpd.evicted)
		q->properties.is_evicted = (q->properties.queue_size > 0 &&
					    q->properties.queue_percent > 0 &&
					    q->properties.queue_address != 0);
=======
	mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
			q->properties.type)];
>>>>>>> upstream/android-13

	/* Save previous activity state for counters */
	prev_active = q->properties.is_active;

	/* Make sure the queue is unmapped before updating the MQD */
	if (dqm->sched_policy != KFD_SCHED_POLICY_NO_HWS) {
		retval = unmap_queues_cpsch(dqm,
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
		if (retval) {
			pr_err("unmap queue failed\n");
			goto out_unlock;
		}
	} else if (prev_active &&
		   (q->properties.type == KFD_QUEUE_TYPE_COMPUTE ||
<<<<<<< HEAD
		    q->properties.type == KFD_QUEUE_TYPE_SDMA)) {
		retval = mqd_mgr->destroy_mqd(mqd_mgr, q->mqd,
				KFD_PREEMPT_TYPE_WAVEFRONT_DRAIN,
=======
		    q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		    q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)) {

		if (!dqm->sched_running) {
			WARN_ONCE(1, "Update non-HWS queue while stopped\n");
			goto out_unlock;
		}

		retval = mqd_mgr->destroy_mqd(mqd_mgr, q->mqd,
				(dqm->dev->cwsr_enabled?
				 KFD_PREEMPT_TYPE_WAVEFRONT_SAVE:
				KFD_PREEMPT_TYPE_WAVEFRONT_DRAIN),
>>>>>>> upstream/android-13
				KFD_UNMAP_LATENCY_MS, q->pipe, q->queue);
		if (retval) {
			pr_err("destroy mqd failed\n");
			goto out_unlock;
		}
	}

<<<<<<< HEAD
	retval = mqd_mgr->update_mqd(mqd_mgr, q->mqd, &q->properties);
=======
	mqd_mgr->update_mqd(mqd_mgr, q->mqd, &q->properties);
>>>>>>> upstream/android-13

	/*
	 * check active state vs. the previous state and modify
	 * counter accordingly. map_queues_cpsch uses the
<<<<<<< HEAD
	 * dqm->queue_count to determine whether a new runlist must be
	 * uploaded.
	 */
	if (q->properties.is_active && !prev_active)
		dqm->queue_count++;
	else if (!q->properties.is_active && prev_active)
		dqm->queue_count--;
=======
	 * dqm->active_queue_count to determine whether a new runlist must be
	 * uploaded.
	 */
	if (q->properties.is_active && !prev_active) {
		increment_queue_count(dqm, &pdd->qpd, q);
	} else if (!q->properties.is_active && prev_active) {
		decrement_queue_count(dqm, &pdd->qpd, q);
	} else if (q->gws && !q->properties.is_gws) {
		if (q->properties.is_active) {
			dqm->gws_queue_count++;
			pdd->qpd.mapped_gws_queue = true;
		}
		q->properties.is_gws = true;
	} else if (!q->gws && q->properties.is_gws) {
		if (q->properties.is_active) {
			dqm->gws_queue_count--;
			pdd->qpd.mapped_gws_queue = false;
		}
		q->properties.is_gws = false;
	}
>>>>>>> upstream/android-13

	if (dqm->sched_policy != KFD_SCHED_POLICY_NO_HWS)
		retval = map_queues_cpsch(dqm);
	else if (q->properties.is_active &&
		 (q->properties.type == KFD_QUEUE_TYPE_COMPUTE ||
<<<<<<< HEAD
		  q->properties.type == KFD_QUEUE_TYPE_SDMA)) {
=======
		  q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		  q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)) {
>>>>>>> upstream/android-13
		if (WARN(q->process->mm != current->mm,
			 "should only run in user thread"))
			retval = -EFAULT;
		else
			retval = mqd_mgr->load_mqd(mqd_mgr, q->mqd,
						   q->pipe, q->queue,
						   &q->properties, current->mm);
	}

out_unlock:
	dqm_unlock(dqm);
	return retval;
}

<<<<<<< HEAD
static struct mqd_manager *get_mqd_manager(
		struct device_queue_manager *dqm, enum KFD_MQD_TYPE type)
{
	struct mqd_manager *mqd_mgr;

	if (WARN_ON(type >= KFD_MQD_TYPE_MAX))
		return NULL;

	pr_debug("mqd type %d\n", type);

	mqd_mgr = dqm->mqd_mgrs[type];
	if (!mqd_mgr) {
		mqd_mgr = mqd_manager_init(type, dqm->dev);
		if (!mqd_mgr)
			pr_err("mqd manager is NULL");
		dqm->mqd_mgrs[type] = mqd_mgr;
	}

	return mqd_mgr;
}

=======
>>>>>>> upstream/android-13
static int evict_process_queues_nocpsch(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd)
{
	struct queue *q;
	struct mqd_manager *mqd_mgr;
	struct kfd_process_device *pdd;
<<<<<<< HEAD
	int retval = 0;
=======
	int retval, ret = 0;
>>>>>>> upstream/android-13

	dqm_lock(dqm);
	if (qpd->evicted++ > 0) /* already evicted, do nothing */
		goto out;

	pdd = qpd_to_pdd(qpd);
<<<<<<< HEAD
	pr_info_ratelimited("Evicting PASID %u queues\n",
			    pdd->process->pasid);

	/* unactivate all active queues on the qpd */
	list_for_each_entry(q, &qpd->queues_list, list) {
		if (!q->properties.is_active)
			continue;
		mqd_mgr = dqm->ops.get_mqd_manager(dqm,
			get_mqd_type_from_queue_type(q->properties.type));
		if (!mqd_mgr) { /* should not be here */
			pr_err("Cannot evict queue, mqd mgr is NULL\n");
			retval = -ENOMEM;
			goto out;
		}
		q->properties.is_evicted = true;
		q->properties.is_active = false;
		retval = mqd_mgr->destroy_mqd(mqd_mgr, q->mqd,
				KFD_PREEMPT_TYPE_WAVEFRONT_DRAIN,
				KFD_UNMAP_LATENCY_MS, q->pipe, q->queue);
		if (retval)
			goto out;
		dqm->queue_count--;
=======
	pr_debug_ratelimited("Evicting PASID 0x%x queues\n",
			    pdd->process->pasid);

	pdd->last_evict_timestamp = get_jiffies_64();
	/* Mark all queues as evicted. Deactivate all active queues on
	 * the qpd.
	 */
	list_for_each_entry(q, &qpd->queues_list, list) {
		q->properties.is_evicted = true;
		if (!q->properties.is_active)
			continue;

		mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
				q->properties.type)];
		q->properties.is_active = false;
		decrement_queue_count(dqm, qpd, q);

		if (WARN_ONCE(!dqm->sched_running, "Evict when stopped\n"))
			continue;

		retval = mqd_mgr->destroy_mqd(mqd_mgr, q->mqd,
				(dqm->dev->cwsr_enabled?
				 KFD_PREEMPT_TYPE_WAVEFRONT_SAVE:
				KFD_PREEMPT_TYPE_WAVEFRONT_DRAIN),
				KFD_UNMAP_LATENCY_MS, q->pipe, q->queue);
		if (retval && !ret)
			/* Return the first error, but keep going to
			 * maintain a consistent eviction state
			 */
			ret = retval;
>>>>>>> upstream/android-13
	}

out:
	dqm_unlock(dqm);
<<<<<<< HEAD
	return retval;
=======
	return ret;
>>>>>>> upstream/android-13
}

static int evict_process_queues_cpsch(struct device_queue_manager *dqm,
				      struct qcm_process_device *qpd)
{
	struct queue *q;
	struct kfd_process_device *pdd;
	int retval = 0;

	dqm_lock(dqm);
	if (qpd->evicted++ > 0) /* already evicted, do nothing */
		goto out;

	pdd = qpd_to_pdd(qpd);
<<<<<<< HEAD
	pr_info_ratelimited("Evicting PASID %u queues\n",
			    pdd->process->pasid);

	/* unactivate all active queues on the qpd */
	list_for_each_entry(q, &qpd->queues_list, list) {
		if (!q->properties.is_active)
			continue;
		q->properties.is_evicted = true;
		q->properties.is_active = false;
		dqm->queue_count--;
	}
=======
	pr_debug_ratelimited("Evicting PASID 0x%x queues\n",
			    pdd->process->pasid);

	/* Mark all queues as evicted. Deactivate all active queues on
	 * the qpd.
	 */
	list_for_each_entry(q, &qpd->queues_list, list) {
		q->properties.is_evicted = true;
		if (!q->properties.is_active)
			continue;

		q->properties.is_active = false;
		decrement_queue_count(dqm, qpd, q);
	}
	pdd->last_evict_timestamp = get_jiffies_64();
>>>>>>> upstream/android-13
	retval = execute_queues_cpsch(dqm,
				qpd->is_debug ?
				KFD_UNMAP_QUEUES_FILTER_ALL_QUEUES :
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);

out:
	dqm_unlock(dqm);
	return retval;
}

static int restore_process_queues_nocpsch(struct device_queue_manager *dqm,
					  struct qcm_process_device *qpd)
{
	struct mm_struct *mm = NULL;
	struct queue *q;
	struct mqd_manager *mqd_mgr;
	struct kfd_process_device *pdd;
<<<<<<< HEAD
	uint32_t pd_base;
	int retval = 0;

	pdd = qpd_to_pdd(qpd);
	/* Retrieve PD base */
	pd_base = dqm->dev->kfd2kgd->get_process_page_dir(pdd->vm);
=======
	uint64_t pd_base;
	uint64_t eviction_duration;
	int retval, ret = 0;

	pdd = qpd_to_pdd(qpd);
	/* Retrieve PD base */
	pd_base = amdgpu_amdkfd_gpuvm_get_process_page_dir(pdd->drm_priv);
>>>>>>> upstream/android-13

	dqm_lock(dqm);
	if (WARN_ON_ONCE(!qpd->evicted)) /* already restored, do nothing */
		goto out;
	if (qpd->evicted > 1) { /* ref count still > 0, decrement & quit */
		qpd->evicted--;
		goto out;
	}

<<<<<<< HEAD
	pr_info_ratelimited("Restoring PASID %u queues\n",
=======
	pr_debug_ratelimited("Restoring PASID 0x%x queues\n",
>>>>>>> upstream/android-13
			    pdd->process->pasid);

	/* Update PD Base in QPD */
	qpd->page_table_base = pd_base;
<<<<<<< HEAD
	pr_debug("Updated PD address to 0x%08x\n", pd_base);
=======
	pr_debug("Updated PD address to 0x%llx\n", pd_base);
>>>>>>> upstream/android-13

	if (!list_empty(&qpd->queues_list)) {
		dqm->dev->kfd2kgd->set_vm_context_page_table_base(
				dqm->dev->kgd,
				qpd->vmid,
				qpd->page_table_base);
<<<<<<< HEAD
		kfd_flush_tlb(pdd);
=======
		kfd_flush_tlb(pdd, TLB_FLUSH_LEGACY);
>>>>>>> upstream/android-13
	}

	/* Take a safe reference to the mm_struct, which may otherwise
	 * disappear even while the kfd_process is still referenced.
	 */
	mm = get_task_mm(pdd->process->lead_thread);
	if (!mm) {
<<<<<<< HEAD
		retval = -EFAULT;
		goto out;
	}

	/* activate all active queues on the qpd */
	list_for_each_entry(q, &qpd->queues_list, list) {
		if (!q->properties.is_evicted)
			continue;
		mqd_mgr = dqm->ops.get_mqd_manager(dqm,
			get_mqd_type_from_queue_type(q->properties.type));
		if (!mqd_mgr) { /* should not be here */
			pr_err("Cannot restore queue, mqd mgr is NULL\n");
			retval = -ENOMEM;
			goto out;
		}
		q->properties.is_evicted = false;
		q->properties.is_active = true;
		retval = mqd_mgr->load_mqd(mqd_mgr, q->mqd, q->pipe,
				       q->queue, &q->properties, mm);
		if (retval)
			goto out;
		dqm->queue_count++;
	}
	qpd->evicted = 0;
=======
		ret = -EFAULT;
		goto out;
	}

	/* Remove the eviction flags. Activate queues that are not
	 * inactive for other reasons.
	 */
	list_for_each_entry(q, &qpd->queues_list, list) {
		q->properties.is_evicted = false;
		if (!QUEUE_IS_ACTIVE(q->properties))
			continue;

		mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
				q->properties.type)];
		q->properties.is_active = true;
		increment_queue_count(dqm, qpd, q);

		if (WARN_ONCE(!dqm->sched_running, "Restore when stopped\n"))
			continue;

		retval = mqd_mgr->load_mqd(mqd_mgr, q->mqd, q->pipe,
				       q->queue, &q->properties, mm);
		if (retval && !ret)
			/* Return the first error, but keep going to
			 * maintain a consistent eviction state
			 */
			ret = retval;
	}
	qpd->evicted = 0;
	eviction_duration = get_jiffies_64() - pdd->last_evict_timestamp;
	atomic64_add(eviction_duration, &pdd->evict_duration_counter);
>>>>>>> upstream/android-13
out:
	if (mm)
		mmput(mm);
	dqm_unlock(dqm);
<<<<<<< HEAD
	return retval;
=======
	return ret;
>>>>>>> upstream/android-13
}

static int restore_process_queues_cpsch(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd)
{
	struct queue *q;
	struct kfd_process_device *pdd;
<<<<<<< HEAD
	uint32_t pd_base;
=======
	uint64_t pd_base;
	uint64_t eviction_duration;
>>>>>>> upstream/android-13
	int retval = 0;

	pdd = qpd_to_pdd(qpd);
	/* Retrieve PD base */
<<<<<<< HEAD
	pd_base = dqm->dev->kfd2kgd->get_process_page_dir(pdd->vm);
=======
	pd_base = amdgpu_amdkfd_gpuvm_get_process_page_dir(pdd->drm_priv);
>>>>>>> upstream/android-13

	dqm_lock(dqm);
	if (WARN_ON_ONCE(!qpd->evicted)) /* already restored, do nothing */
		goto out;
	if (qpd->evicted > 1) { /* ref count still > 0, decrement & quit */
		qpd->evicted--;
		goto out;
	}

<<<<<<< HEAD
	pr_info_ratelimited("Restoring PASID %u queues\n",
=======
	pr_debug_ratelimited("Restoring PASID 0x%x queues\n",
>>>>>>> upstream/android-13
			    pdd->process->pasid);

	/* Update PD Base in QPD */
	qpd->page_table_base = pd_base;
<<<<<<< HEAD
	pr_debug("Updated PD address to 0x%08x\n", pd_base);

	/* activate all active queues on the qpd */
	list_for_each_entry(q, &qpd->queues_list, list) {
		if (!q->properties.is_evicted)
			continue;
		q->properties.is_evicted = false;
		q->properties.is_active = true;
		dqm->queue_count++;
	}
	retval = execute_queues_cpsch(dqm,
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
	if (!retval)
		qpd->evicted = 0;
=======
	pr_debug("Updated PD address to 0x%llx\n", pd_base);

	/* activate all active queues on the qpd */
	list_for_each_entry(q, &qpd->queues_list, list) {
		q->properties.is_evicted = false;
		if (!QUEUE_IS_ACTIVE(q->properties))
			continue;

		q->properties.is_active = true;
		increment_queue_count(dqm, &pdd->qpd, q);
	}
	retval = execute_queues_cpsch(dqm,
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
	qpd->evicted = 0;
	eviction_duration = get_jiffies_64() - pdd->last_evict_timestamp;
	atomic64_add(eviction_duration, &pdd->evict_duration_counter);
>>>>>>> upstream/android-13
out:
	dqm_unlock(dqm);
	return retval;
}

static int register_process(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd)
{
	struct device_process_node *n;
	struct kfd_process_device *pdd;
<<<<<<< HEAD
	uint32_t pd_base;
=======
	uint64_t pd_base;
>>>>>>> upstream/android-13
	int retval;

	n = kzalloc(sizeof(*n), GFP_KERNEL);
	if (!n)
		return -ENOMEM;

	n->qpd = qpd;

	pdd = qpd_to_pdd(qpd);
	/* Retrieve PD base */
<<<<<<< HEAD
	pd_base = dqm->dev->kfd2kgd->get_process_page_dir(pdd->vm);
=======
	pd_base = amdgpu_amdkfd_gpuvm_get_process_page_dir(pdd->drm_priv);
>>>>>>> upstream/android-13

	dqm_lock(dqm);
	list_add(&n->list, &dqm->queues);

	/* Update PD Base in QPD */
	qpd->page_table_base = pd_base;
<<<<<<< HEAD

	retval = dqm->asic_ops.update_qpd(dqm, qpd);

	if (dqm->processes_count++ == 0)
		dqm->dev->kfd2kgd->set_compute_idle(dqm->dev->kgd, false);

	dqm_unlock(dqm);

=======
	pr_debug("Updated PD address to 0x%llx\n", pd_base);

	retval = dqm->asic_ops.update_qpd(dqm, qpd);

	dqm->processes_count++;

	dqm_unlock(dqm);

	/* Outside the DQM lock because under the DQM lock we can't do
	 * reclaim or take other locks that others hold while reclaiming.
	 */
	kfd_inc_compute_active(dqm->dev);

>>>>>>> upstream/android-13
	return retval;
}

static int unregister_process(struct device_queue_manager *dqm,
					struct qcm_process_device *qpd)
{
	int retval;
	struct device_process_node *cur, *next;

	pr_debug("qpd->queues_list is %s\n",
			list_empty(&qpd->queues_list) ? "empty" : "not empty");

	retval = 0;
	dqm_lock(dqm);

	list_for_each_entry_safe(cur, next, &dqm->queues, list) {
		if (qpd == cur->qpd) {
			list_del(&cur->list);
			kfree(cur);
<<<<<<< HEAD
			if (--dqm->processes_count == 0)
				dqm->dev->kfd2kgd->set_compute_idle(
					dqm->dev->kgd, true);
=======
			dqm->processes_count--;
>>>>>>> upstream/android-13
			goto out;
		}
	}
	/* qpd not found in dqm list */
	retval = 1;
out:
	dqm_unlock(dqm);
<<<<<<< HEAD
=======

	/* Outside the DQM lock because under the DQM lock we can't do
	 * reclaim or take other locks that others hold while reclaiming.
	 */
	if (!retval)
		kfd_dec_compute_active(dqm->dev);

>>>>>>> upstream/android-13
	return retval;
}

static int
<<<<<<< HEAD
set_pasid_vmid_mapping(struct device_queue_manager *dqm, unsigned int pasid,
			unsigned int vmid)
{
	uint32_t pasid_mapping;

	pasid_mapping = (pasid == 0) ? 0 :
		(uint32_t)pasid |
		ATC_VMID_PASID_MAPPING_VALID;

	return dqm->dev->kfd2kgd->set_pasid_vmid_mapping(
						dqm->dev->kgd, pasid_mapping,
						vmid);
=======
set_pasid_vmid_mapping(struct device_queue_manager *dqm, u32 pasid,
			unsigned int vmid)
{
	return dqm->dev->kfd2kgd->set_pasid_vmid_mapping(
						dqm->dev->kgd, pasid, vmid);
>>>>>>> upstream/android-13
}

static void init_interrupts(struct device_queue_manager *dqm)
{
	unsigned int i;

	for (i = 0 ; i < get_pipes_per_mec(dqm) ; i++)
		if (is_pipe_enabled(dqm, 0, i))
			dqm->dev->kfd2kgd->init_interrupts(dqm->dev->kgd, i);
}

static int initialize_nocpsch(struct device_queue_manager *dqm)
{
	int pipe, queue;

	pr_debug("num of pipes: %d\n", get_pipes_per_mec(dqm));

	dqm->allocated_queues = kcalloc(get_pipes_per_mec(dqm),
					sizeof(unsigned int), GFP_KERNEL);
	if (!dqm->allocated_queues)
		return -ENOMEM;

	mutex_init(&dqm->lock_hidden);
	INIT_LIST_HEAD(&dqm->queues);
<<<<<<< HEAD
	dqm->queue_count = dqm->next_pipe_to_allocate = 0;
	dqm->sdma_queue_count = 0;
=======
	dqm->active_queue_count = dqm->next_pipe_to_allocate = 0;
	dqm->active_cp_queue_count = 0;
	dqm->gws_queue_count = 0;
>>>>>>> upstream/android-13

	for (pipe = 0; pipe < get_pipes_per_mec(dqm); pipe++) {
		int pipe_offset = pipe * get_queues_per_pipe(dqm);

		for (queue = 0; queue < get_queues_per_pipe(dqm); queue++)
			if (test_bit(pipe_offset + queue,
<<<<<<< HEAD
				     dqm->dev->shared_resources.queue_bitmap))
				dqm->allocated_queues[pipe] |= 1 << queue;
	}

	dqm->vmid_bitmap = (1 << dqm->dev->vm_info.vmid_num_kfd) - 1;
	dqm->sdma_bitmap = (1 << get_num_sdma_queues(dqm)) - 1;
=======
				     dqm->dev->shared_resources.cp_queue_bitmap))
				dqm->allocated_queues[pipe] |= 1 << queue;
	}

	memset(dqm->vmid_pasid, 0, sizeof(dqm->vmid_pasid));

	dqm->sdma_bitmap = ~0ULL >> (64 - get_num_sdma_queues(dqm));
	dqm->xgmi_sdma_bitmap = ~0ULL >> (64 - get_num_xgmi_sdma_queues(dqm));
>>>>>>> upstream/android-13

	return 0;
}

static void uninitialize(struct device_queue_manager *dqm)
{
	int i;

<<<<<<< HEAD
	WARN_ON(dqm->queue_count > 0 || dqm->processes_count > 0);
=======
	WARN_ON(dqm->active_queue_count > 0 || dqm->processes_count > 0);
>>>>>>> upstream/android-13

	kfree(dqm->allocated_queues);
	for (i = 0 ; i < KFD_MQD_TYPE_MAX ; i++)
		kfree(dqm->mqd_mgrs[i]);
	mutex_destroy(&dqm->lock_hidden);
<<<<<<< HEAD
	kfd_gtt_sa_free(dqm->dev, dqm->pipeline_mem);
=======
>>>>>>> upstream/android-13
}

static int start_nocpsch(struct device_queue_manager *dqm)
{
<<<<<<< HEAD
	init_interrupts(dqm);
	return pm_init(&dqm->packets, dqm);
=======
	pr_info("SW scheduler is used");
	init_interrupts(dqm);
	
	if (dqm->dev->device_info->asic_family == CHIP_HAWAII)
		return pm_init(&dqm->packet_mgr, dqm);
	dqm->sched_running = true;

	return 0;
>>>>>>> upstream/android-13
}

static int stop_nocpsch(struct device_queue_manager *dqm)
{
<<<<<<< HEAD
	pm_uninit(&dqm->packets);
	return 0;
}

static int allocate_sdma_queue(struct device_queue_manager *dqm,
				unsigned int *sdma_queue_id)
{
	int bit;

	if (dqm->sdma_bitmap == 0)
		return -ENOMEM;

	bit = ffs(dqm->sdma_bitmap) - 1;
	dqm->sdma_bitmap &= ~(1 << bit);
	*sdma_queue_id = bit;
=======
	if (dqm->dev->device_info->asic_family == CHIP_HAWAII)
		pm_uninit(&dqm->packet_mgr, false);
	dqm->sched_running = false;

	return 0;
}

static void pre_reset(struct device_queue_manager *dqm)
{
	dqm_lock(dqm);
	dqm->is_resetting = true;
	dqm_unlock(dqm);
}

static int allocate_sdma_queue(struct device_queue_manager *dqm,
				struct queue *q)
{
	int bit;

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
		if (dqm->sdma_bitmap == 0) {
			pr_err("No more SDMA queue to allocate\n");
			return -ENOMEM;
		}

		bit = __ffs64(dqm->sdma_bitmap);
		dqm->sdma_bitmap &= ~(1ULL << bit);
		q->sdma_id = bit;
		q->properties.sdma_engine_id = q->sdma_id %
				get_num_sdma_engines(dqm);
		q->properties.sdma_queue_id = q->sdma_id /
				get_num_sdma_engines(dqm);
	} else if (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI) {
		if (dqm->xgmi_sdma_bitmap == 0) {
			pr_err("No more XGMI SDMA queue to allocate\n");
			return -ENOMEM;
		}
		bit = __ffs64(dqm->xgmi_sdma_bitmap);
		dqm->xgmi_sdma_bitmap &= ~(1ULL << bit);
		q->sdma_id = bit;
		/* sdma_engine_id is sdma id including
		 * both PCIe-optimized SDMAs and XGMI-
		 * optimized SDMAs. The calculation below
		 * assumes the first N engines are always
		 * PCIe-optimized ones
		 */
		q->properties.sdma_engine_id = get_num_sdma_engines(dqm) +
				q->sdma_id % get_num_xgmi_sdma_engines(dqm);
		q->properties.sdma_queue_id = q->sdma_id /
				get_num_xgmi_sdma_engines(dqm);
	}

	pr_debug("SDMA engine id: %d\n", q->properties.sdma_engine_id);
	pr_debug("SDMA queue id: %d\n", q->properties.sdma_queue_id);
>>>>>>> upstream/android-13

	return 0;
}

static void deallocate_sdma_queue(struct device_queue_manager *dqm,
<<<<<<< HEAD
				unsigned int sdma_queue_id)
{
	if (sdma_queue_id >= get_num_sdma_queues(dqm))
		return;
	dqm->sdma_bitmap |= (1 << sdma_queue_id);
}

static int create_sdma_queue_nocpsch(struct device_queue_manager *dqm,
					struct queue *q,
					struct qcm_process_device *qpd)
{
	struct mqd_manager *mqd_mgr;
	int retval;

	mqd_mgr = dqm->ops.get_mqd_manager(dqm, KFD_MQD_TYPE_SDMA);
	if (!mqd_mgr)
		return -ENOMEM;

	retval = allocate_sdma_queue(dqm, &q->sdma_id);
	if (retval)
		return retval;

	q->properties.sdma_queue_id = q->sdma_id / get_num_sdma_engines(dqm);
	q->properties.sdma_engine_id = q->sdma_id % get_num_sdma_engines(dqm);

	retval = allocate_doorbell(qpd, q);
	if (retval)
		goto out_deallocate_sdma_queue;

	pr_debug("SDMA id is:    %d\n", q->sdma_id);
	pr_debug("SDMA queue id: %d\n", q->properties.sdma_queue_id);
	pr_debug("SDMA engine id: %d\n", q->properties.sdma_engine_id);

	dqm->asic_ops.init_sdma_vm(dqm, q, qpd);
	retval = mqd_mgr->init_mqd(mqd_mgr, &q->mqd, &q->mqd_mem_obj,
				&q->gart_mqd_addr, &q->properties);
	if (retval)
		goto out_deallocate_doorbell;

	retval = mqd_mgr->load_mqd(mqd_mgr, q->mqd, 0, 0, &q->properties,
				NULL);
	if (retval)
		goto out_uninit_mqd;

	return 0;

out_uninit_mqd:
	mqd_mgr->uninit_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);
out_deallocate_doorbell:
	deallocate_doorbell(qpd, q);
out_deallocate_sdma_queue:
	deallocate_sdma_queue(dqm, q->sdma_id);

	return retval;
=======
				struct queue *q)
{
	if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
		if (q->sdma_id >= get_num_sdma_queues(dqm))
			return;
		dqm->sdma_bitmap |= (1ULL << q->sdma_id);
	} else if (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI) {
		if (q->sdma_id >= get_num_xgmi_sdma_queues(dqm))
			return;
		dqm->xgmi_sdma_bitmap |= (1ULL << q->sdma_id);
	}
>>>>>>> upstream/android-13
}

/*
 * Device Queue Manager implementation for cp scheduler
 */

static int set_sched_resources(struct device_queue_manager *dqm)
{
	int i, mec;
	struct scheduling_resources res;

	res.vmid_mask = dqm->dev->shared_resources.compute_vmid_bitmap;

	res.queue_mask = 0;
	for (i = 0; i < KGD_MAX_QUEUES; ++i) {
		mec = (i / dqm->dev->shared_resources.num_queue_per_pipe)
			/ dqm->dev->shared_resources.num_pipe_per_mec;

<<<<<<< HEAD
		if (!test_bit(i, dqm->dev->shared_resources.queue_bitmap))
=======
		if (!test_bit(i, dqm->dev->shared_resources.cp_queue_bitmap))
>>>>>>> upstream/android-13
			continue;

		/* only acquire queues from the first MEC */
		if (mec > 0)
			continue;

		/* This situation may be hit in the future if a new HW
		 * generation exposes more than 64 queues. If so, the
		 * definition of res.queue_mask needs updating
		 */
		if (WARN_ON(i >= (sizeof(res.queue_mask)*8))) {
			pr_err("Invalid queue enabled by amdgpu: %d\n", i);
			break;
		}

<<<<<<< HEAD
		res.queue_mask |= (1ull << i);
	}
	res.gws_mask = res.oac_mask = res.gds_heap_base =
						res.gds_heap_size = 0;
=======
		res.queue_mask |= 1ull
			<< amdgpu_queue_mask_bit_to_set_resource_bit(
				(struct amdgpu_device *)dqm->dev->kgd, i);
	}
	res.gws_mask = ~0ull;
	res.oac_mask = res.gds_heap_base = res.gds_heap_size = 0;
>>>>>>> upstream/android-13

	pr_debug("Scheduling resources:\n"
			"vmid mask: 0x%8X\n"
			"queue mask: 0x%8llX\n",
			res.vmid_mask, res.queue_mask);

<<<<<<< HEAD
	return pm_send_set_resources(&dqm->packets, &res);
=======
	return pm_send_set_resources(&dqm->packet_mgr, &res);
>>>>>>> upstream/android-13
}

static int initialize_cpsch(struct device_queue_manager *dqm)
{
<<<<<<< HEAD
=======
	uint64_t num_sdma_queues;
	uint64_t num_xgmi_sdma_queues;

>>>>>>> upstream/android-13
	pr_debug("num of pipes: %d\n", get_pipes_per_mec(dqm));

	mutex_init(&dqm->lock_hidden);
	INIT_LIST_HEAD(&dqm->queues);
<<<<<<< HEAD
	dqm->queue_count = dqm->processes_count = 0;
	dqm->sdma_queue_count = 0;
	dqm->active_runlist = false;
	dqm->sdma_bitmap = (1 << get_num_sdma_queues(dqm)) - 1;
=======
	dqm->active_queue_count = dqm->processes_count = 0;
	dqm->active_cp_queue_count = 0;
	dqm->gws_queue_count = 0;
	dqm->active_runlist = false;

	num_sdma_queues = get_num_sdma_queues(dqm);
	if (num_sdma_queues >= BITS_PER_TYPE(dqm->sdma_bitmap))
		dqm->sdma_bitmap = ULLONG_MAX;
	else
		dqm->sdma_bitmap = (BIT_ULL(num_sdma_queues) - 1);

	num_xgmi_sdma_queues = get_num_xgmi_sdma_queues(dqm);
	if (num_xgmi_sdma_queues >= BITS_PER_TYPE(dqm->xgmi_sdma_bitmap))
		dqm->xgmi_sdma_bitmap = ULLONG_MAX;
	else
		dqm->xgmi_sdma_bitmap = (BIT_ULL(num_xgmi_sdma_queues) - 1);
>>>>>>> upstream/android-13

	INIT_WORK(&dqm->hw_exception_work, kfd_process_hw_exception);

	return 0;
}

static int start_cpsch(struct device_queue_manager *dqm)
{
	int retval;

	retval = 0;

<<<<<<< HEAD
	retval = pm_init(&dqm->packets, dqm);
=======
	dqm_lock(dqm);
	retval = pm_init(&dqm->packet_mgr, dqm);
>>>>>>> upstream/android-13
	if (retval)
		goto fail_packet_manager_init;

	retval = set_sched_resources(dqm);
	if (retval)
		goto fail_set_sched_resources;

	pr_debug("Allocating fence memory\n");

	/* allocate fence memory on the gart */
	retval = kfd_gtt_sa_allocate(dqm->dev, sizeof(*dqm->fence_addr),
					&dqm->fence_mem);

	if (retval)
		goto fail_allocate_vidmem;

<<<<<<< HEAD
	dqm->fence_addr = dqm->fence_mem->cpu_ptr;
=======
	dqm->fence_addr = (uint64_t *)dqm->fence_mem->cpu_ptr;
>>>>>>> upstream/android-13
	dqm->fence_gpu_addr = dqm->fence_mem->gpu_addr;

	init_interrupts(dqm);

<<<<<<< HEAD
	dqm_lock(dqm);
	/* clear hang status when driver try to start the hw scheduler */
	dqm->is_hws_hang = false;
=======
	/* clear hang status when driver try to start the hw scheduler */
	dqm->is_hws_hang = false;
	dqm->is_resetting = false;
	dqm->sched_running = true;
>>>>>>> upstream/android-13
	execute_queues_cpsch(dqm, KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
	dqm_unlock(dqm);

	return 0;
fail_allocate_vidmem:
fail_set_sched_resources:
<<<<<<< HEAD
	pm_uninit(&dqm->packets);
fail_packet_manager_init:
=======
	pm_uninit(&dqm->packet_mgr, false);
fail_packet_manager_init:
	dqm_unlock(dqm);
>>>>>>> upstream/android-13
	return retval;
}

static int stop_cpsch(struct device_queue_manager *dqm)
{
<<<<<<< HEAD
	dqm_lock(dqm);
	unmap_queues_cpsch(dqm, KFD_UNMAP_QUEUES_FILTER_ALL_QUEUES, 0);
	dqm_unlock(dqm);

	pm_release_ib(&dqm->packets);

	kfd_gtt_sa_free(dqm->dev, dqm->fence_mem);
	pm_uninit(&dqm->packets);
=======
	bool hanging;

	dqm_lock(dqm);
	if (!dqm->sched_running) {
		dqm_unlock(dqm);
		return 0;
	}

	if (!dqm->is_hws_hang)
		unmap_queues_cpsch(dqm, KFD_UNMAP_QUEUES_FILTER_ALL_QUEUES, 0);
	hanging = dqm->is_hws_hang || dqm->is_resetting;
	dqm->sched_running = false;

	pm_release_ib(&dqm->packet_mgr);

	kfd_gtt_sa_free(dqm->dev, dqm->fence_mem);
	pm_uninit(&dqm->packet_mgr, hanging);
	dqm_unlock(dqm);
>>>>>>> upstream/android-13

	return 0;
}

static int create_kernel_queue_cpsch(struct device_queue_manager *dqm,
					struct kernel_queue *kq,
					struct qcm_process_device *qpd)
{
	dqm_lock(dqm);
	if (dqm->total_queue_count >= max_num_of_queues_per_device) {
		pr_warn("Can't create new kernel queue because %d queues were already created\n",
				dqm->total_queue_count);
		dqm_unlock(dqm);
		return -EPERM;
	}

	/*
	 * Unconditionally increment this counter, regardless of the queue's
	 * type or whether the queue is active.
	 */
	dqm->total_queue_count++;
	pr_debug("Total of %d queues are accountable so far\n",
			dqm->total_queue_count);

	list_add(&kq->list, &qpd->priv_queue_list);
<<<<<<< HEAD
	dqm->queue_count++;
=======
	increment_queue_count(dqm, qpd, kq->queue);
>>>>>>> upstream/android-13
	qpd->is_debug = true;
	execute_queues_cpsch(dqm, KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
	dqm_unlock(dqm);

	return 0;
}

static void destroy_kernel_queue_cpsch(struct device_queue_manager *dqm,
					struct kernel_queue *kq,
					struct qcm_process_device *qpd)
{
	dqm_lock(dqm);
	list_del(&kq->list);
<<<<<<< HEAD
	dqm->queue_count--;
=======
	decrement_queue_count(dqm, qpd, kq->queue);
>>>>>>> upstream/android-13
	qpd->is_debug = false;
	execute_queues_cpsch(dqm, KFD_UNMAP_QUEUES_FILTER_ALL_QUEUES, 0);
	/*
	 * Unconditionally decrement this counter, regardless of the queue's
	 * type.
	 */
	dqm->total_queue_count--;
	pr_debug("Total of %d queues are accountable so far\n",
			dqm->total_queue_count);
	dqm_unlock(dqm);
}

static int create_queue_cpsch(struct device_queue_manager *dqm, struct queue *q,
			struct qcm_process_device *qpd)
{
	int retval;
	struct mqd_manager *mqd_mgr;

<<<<<<< HEAD
	retval = 0;

	dqm_lock(dqm);

=======
>>>>>>> upstream/android-13
	if (dqm->total_queue_count >= max_num_of_queues_per_device) {
		pr_warn("Can't create new usermode queue because %d queues were already created\n",
				dqm->total_queue_count);
		retval = -EPERM;
<<<<<<< HEAD
		goto out_unlock;
	}

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
		retval = allocate_sdma_queue(dqm, &q->sdma_id);
		if (retval)
			goto out_unlock;
		q->properties.sdma_queue_id =
			q->sdma_id / get_num_sdma_engines(dqm);
		q->properties.sdma_engine_id =
			q->sdma_id % get_num_sdma_engines(dqm);
=======
		goto out;
	}

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI) {
		dqm_lock(dqm);
		retval = allocate_sdma_queue(dqm, q);
		dqm_unlock(dqm);
		if (retval)
			goto out;
>>>>>>> upstream/android-13
	}

	retval = allocate_doorbell(qpd, q);
	if (retval)
		goto out_deallocate_sdma_queue;

<<<<<<< HEAD
	mqd_mgr = dqm->ops.get_mqd_manager(dqm,
			get_mqd_type_from_queue_type(q->properties.type));

	if (!mqd_mgr) {
		retval = -ENOMEM;
		goto out_deallocate_doorbell;
	}
	/*
	 * Eviction state logic: we only mark active queues as evicted
	 * to avoid the overhead of restoring inactive queues later
	 */
	if (qpd->evicted)
		q->properties.is_evicted = (q->properties.queue_size > 0 &&
					    q->properties.queue_percent > 0 &&
					    q->properties.queue_address != 0);

	dqm->asic_ops.init_sdma_vm(dqm, q, qpd);

	q->properties.tba_addr = qpd->tba_addr;
	q->properties.tma_addr = qpd->tma_addr;
	retval = mqd_mgr->init_mqd(mqd_mgr, &q->mqd, &q->mqd_mem_obj,
				&q->gart_mqd_addr, &q->properties);
	if (retval)
		goto out_deallocate_doorbell;

	list_add(&q->list, &qpd->queues_list);
	qpd->queue_count++;
	if (q->properties.is_active) {
		dqm->queue_count++;
		retval = execute_queues_cpsch(dqm,
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
	}

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA)
		dqm->sdma_queue_count++;
=======
	mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
			q->properties.type)];

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)
		dqm->asic_ops.init_sdma_vm(dqm, q, qpd);
	q->properties.tba_addr = qpd->tba_addr;
	q->properties.tma_addr = qpd->tma_addr;
	q->mqd_mem_obj = mqd_mgr->allocate_mqd(mqd_mgr->dev, &q->properties);
	if (!q->mqd_mem_obj) {
		retval = -ENOMEM;
		goto out_deallocate_doorbell;
	}

	dqm_lock(dqm);
	/*
	 * Eviction state logic: mark all queues as evicted, even ones
	 * not currently active. Restoring inactive queues later only
	 * updates the is_evicted flag but is a no-op otherwise.
	 */
	q->properties.is_evicted = !!qpd->evicted;
	mqd_mgr->init_mqd(mqd_mgr, &q->mqd, q->mqd_mem_obj,
				&q->gart_mqd_addr, &q->properties);

	list_add(&q->list, &qpd->queues_list);
	qpd->queue_count++;

	if (q->properties.is_active) {
		increment_queue_count(dqm, qpd, q);

		execute_queues_cpsch(dqm,
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
	}

>>>>>>> upstream/android-13
	/*
	 * Unconditionally increment this counter, regardless of the queue's
	 * type or whether the queue is active.
	 */
	dqm->total_queue_count++;

	pr_debug("Total of %d queues are accountable so far\n",
			dqm->total_queue_count);

	dqm_unlock(dqm);
	return retval;

out_deallocate_doorbell:
	deallocate_doorbell(qpd, q);
out_deallocate_sdma_queue:
<<<<<<< HEAD
	if (q->properties.type == KFD_QUEUE_TYPE_SDMA)
		deallocate_sdma_queue(dqm, q->sdma_id);
out_unlock:
	dqm_unlock(dqm);

	return retval;
}

int amdkfd_fence_wait_timeout(unsigned int *fence_addr,
				unsigned int fence_value,
=======
	if (q->properties.type == KFD_QUEUE_TYPE_SDMA ||
		q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI) {
		dqm_lock(dqm);
		deallocate_sdma_queue(dqm, q);
		dqm_unlock(dqm);
	}
out:
	return retval;
}

int amdkfd_fence_wait_timeout(uint64_t *fence_addr,
				uint64_t fence_value,
>>>>>>> upstream/android-13
				unsigned int timeout_ms)
{
	unsigned long end_jiffies = msecs_to_jiffies(timeout_ms) + jiffies;

	while (*fence_addr != fence_value) {
		if (time_after(jiffies, end_jiffies)) {
			pr_err("qcm fence wait loop timeout expired\n");
			/* In HWS case, this is used to halt the driver thread
			 * in order not to mess up CP states before doing
			 * scandumps for FW debugging.
			 */
			while (halt_if_hws_hang)
				schedule();

			return -ETIME;
		}
		schedule();
	}

	return 0;
}

<<<<<<< HEAD
static int unmap_sdma_queues(struct device_queue_manager *dqm)
{
	int i, retval = 0;

	for (i = 0; i < dqm->dev->device_info->num_sdma_engines; i++) {
		retval = pm_send_unmap_queue(&dqm->packets, KFD_QUEUE_TYPE_SDMA,
			KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0, false, i);
		if (retval)
			return retval;
	}
	return retval;
}

=======
>>>>>>> upstream/android-13
/* dqm->lock mutex has to be locked before calling this function */
static int map_queues_cpsch(struct device_queue_manager *dqm)
{
	int retval;

<<<<<<< HEAD
	if (dqm->queue_count <= 0 || dqm->processes_count <= 0)
		return 0;

	if (dqm->active_runlist)
		return 0;

	retval = pm_send_runlist(&dqm->packets, &dqm->queues);
=======
	if (!dqm->sched_running)
		return 0;
	if (dqm->active_queue_count <= 0 || dqm->processes_count <= 0)
		return 0;
	if (dqm->active_runlist)
		return 0;

	retval = pm_send_runlist(&dqm->packet_mgr, &dqm->queues);
	pr_debug("%s sent runlist\n", __func__);
>>>>>>> upstream/android-13
	if (retval) {
		pr_err("failed to execute runlist\n");
		return retval;
	}
	dqm->active_runlist = true;

	return retval;
}

/* dqm->lock mutex has to be locked before calling this function */
static int unmap_queues_cpsch(struct device_queue_manager *dqm,
				enum kfd_unmap_queues_filter filter,
				uint32_t filter_param)
{
	int retval = 0;
<<<<<<< HEAD

=======
	struct mqd_manager *mqd_mgr;

	if (!dqm->sched_running)
		return 0;
>>>>>>> upstream/android-13
	if (dqm->is_hws_hang)
		return -EIO;
	if (!dqm->active_runlist)
		return retval;

<<<<<<< HEAD
	pr_debug("Before destroying queues, sdma queue count is : %u\n",
		dqm->sdma_queue_count);

	if (dqm->sdma_queue_count > 0)
		unmap_sdma_queues(dqm);

	retval = pm_send_unmap_queue(&dqm->packets, KFD_QUEUE_TYPE_COMPUTE,
=======
	retval = pm_send_unmap_queue(&dqm->packet_mgr, KFD_QUEUE_TYPE_COMPUTE,
>>>>>>> upstream/android-13
			filter, filter_param, false, 0);
	if (retval)
		return retval;

	*dqm->fence_addr = KFD_FENCE_INIT;
<<<<<<< HEAD
	pm_send_query_status(&dqm->packets, dqm->fence_gpu_addr,
				KFD_FENCE_COMPLETED);
	/* should be timed out */
	retval = amdkfd_fence_wait_timeout(dqm->fence_addr, KFD_FENCE_COMPLETED,
				QUEUE_PREEMPT_DEFAULT_TIMEOUT_MS);
	if (retval)
		return retval;

	pm_release_ib(&dqm->packets);
=======
	pm_send_query_status(&dqm->packet_mgr, dqm->fence_gpu_addr,
				KFD_FENCE_COMPLETED);
	/* should be timed out */
	retval = amdkfd_fence_wait_timeout(dqm->fence_addr, KFD_FENCE_COMPLETED,
				queue_preemption_timeout_ms);
	if (retval) {
		pr_err("The cp might be in an unrecoverable state due to an unsuccessful queues preemption\n");
		dqm->is_hws_hang = true;
		/* It's possible we're detecting a HWS hang in the
		 * middle of a GPU reset. No need to schedule another
		 * reset in this case.
		 */
		if (!dqm->is_resetting)
			schedule_work(&dqm->hw_exception_work);
		return retval;
	}

	/* In the current MEC firmware implementation, if compute queue
	 * doesn't response to the preemption request in time, HIQ will
	 * abandon the unmap request without returning any timeout error
	 * to driver. Instead, MEC firmware will log the doorbell of the
	 * unresponding compute queue to HIQ.MQD.queue_doorbell_id fields.
	 * To make sure the queue unmap was successful, driver need to
	 * check those fields
	 */
	mqd_mgr = dqm->mqd_mgrs[KFD_MQD_TYPE_HIQ];
	if (mqd_mgr->read_doorbell_id(dqm->packet_mgr.priv_queue->queue->mqd)) {
		pr_err("HIQ MQD's queue_doorbell_id0 is not 0, Queue preemption time out\n");
		while (halt_if_hws_hang)
			schedule();
		return -ETIME;
	}

	pm_release_ib(&dqm->packet_mgr);
>>>>>>> upstream/android-13
	dqm->active_runlist = false;

	return retval;
}

/* dqm->lock mutex has to be locked before calling this function */
static int execute_queues_cpsch(struct device_queue_manager *dqm,
				enum kfd_unmap_queues_filter filter,
				uint32_t filter_param)
{
	int retval;

	if (dqm->is_hws_hang)
		return -EIO;
	retval = unmap_queues_cpsch(dqm, filter, filter_param);
<<<<<<< HEAD
	if (retval) {
		pr_err("The cp might be in an unrecoverable state due to an unsuccessful queues preemption\n");
		dqm->is_hws_hang = true;
		schedule_work(&dqm->hw_exception_work);
		return retval;
	}
=======
	if (retval)
		return retval;
>>>>>>> upstream/android-13

	return map_queues_cpsch(dqm);
}

static int destroy_queue_cpsch(struct device_queue_manager *dqm,
				struct qcm_process_device *qpd,
				struct queue *q)
{
	int retval;
	struct mqd_manager *mqd_mgr;
<<<<<<< HEAD
	bool preempt_all_queues;

	preempt_all_queues = false;
=======
	uint64_t sdma_val = 0;
	struct kfd_process_device *pdd = qpd_to_pdd(qpd);

	/* Get the SDMA queue stats */
	if ((q->properties.type == KFD_QUEUE_TYPE_SDMA) ||
	    (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)) {
		retval = read_sdma_queue_counter((uint64_t __user *)q->properties.read_ptr,
							&sdma_val);
		if (retval)
			pr_err("Failed to read SDMA queue counter for queue: %d\n",
				q->properties.queue_id);
	}
>>>>>>> upstream/android-13

	retval = 0;

	/* remove queue from list to prevent rescheduling after preemption */
	dqm_lock(dqm);

	if (qpd->is_debug) {
		/*
		 * error, currently we do not allow to destroy a queue
		 * of a currently debugged process
		 */
		retval = -EBUSY;
		goto failed_try_destroy_debugged_queue;

	}

<<<<<<< HEAD
	mqd_mgr = dqm->ops.get_mqd_manager(dqm,
			get_mqd_type_from_queue_type(q->properties.type));
	if (!mqd_mgr) {
		retval = -ENOMEM;
		goto failed;
	}

	deallocate_doorbell(qpd, q);

	if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
		dqm->sdma_queue_count--;
		deallocate_sdma_queue(dqm, q->sdma_id);
=======
	mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
			q->properties.type)];

	deallocate_doorbell(qpd, q);

	if ((q->properties.type == KFD_QUEUE_TYPE_SDMA) ||
	    (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)) {
		deallocate_sdma_queue(dqm, q);
		pdd->sdma_past_activity_counter += sdma_val;
>>>>>>> upstream/android-13
	}

	list_del(&q->list);
	qpd->queue_count--;
	if (q->properties.is_active) {
<<<<<<< HEAD
		dqm->queue_count--;
=======
		decrement_queue_count(dqm, qpd, q);
>>>>>>> upstream/android-13
		retval = execute_queues_cpsch(dqm,
				KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES, 0);
		if (retval == -ETIME)
			qpd->reset_wavefronts = true;
	}

<<<<<<< HEAD
	mqd_mgr->uninit_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);

=======
>>>>>>> upstream/android-13
	/*
	 * Unconditionally decrement this counter, regardless of the queue's
	 * type
	 */
	dqm->total_queue_count--;
	pr_debug("Total of %d queues are accountable so far\n",
			dqm->total_queue_count);

	dqm_unlock(dqm);

<<<<<<< HEAD
	return retval;

failed:
=======
	/* Do free_mqd after dqm_unlock(dqm) to avoid circular locking */
	mqd_mgr->free_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);

	return retval;

>>>>>>> upstream/android-13
failed_try_destroy_debugged_queue:

	dqm_unlock(dqm);
	return retval;
}

/*
 * Low bits must be 0000/FFFF as required by HW, high bits must be 0 to
 * stay in user mode.
 */
#define APE1_FIXED_BITS_MASK 0xFFFF80000000FFFFULL
/* APE1 limit is inclusive and 64K aligned. */
#define APE1_LIMIT_ALIGNMENT 0xFFFF

static bool set_cache_memory_policy(struct device_queue_manager *dqm,
				   struct qcm_process_device *qpd,
				   enum cache_policy default_policy,
				   enum cache_policy alternate_policy,
				   void __user *alternate_aperture_base,
				   uint64_t alternate_aperture_size)
{
	bool retval = true;

	if (!dqm->asic_ops.set_cache_memory_policy)
		return retval;

	dqm_lock(dqm);

	if (alternate_aperture_size == 0) {
		/* base > limit disables APE1 */
		qpd->sh_mem_ape1_base = 1;
		qpd->sh_mem_ape1_limit = 0;
	} else {
		/*
		 * In FSA64, APE1_Base[63:0] = { 16{SH_MEM_APE1_BASE[31]},
		 *			SH_MEM_APE1_BASE[31:0], 0x0000 }
		 * APE1_Limit[63:0] = { 16{SH_MEM_APE1_LIMIT[31]},
		 *			SH_MEM_APE1_LIMIT[31:0], 0xFFFF }
		 * Verify that the base and size parameters can be
		 * represented in this format and convert them.
		 * Additionally restrict APE1 to user-mode addresses.
		 */

		uint64_t base = (uintptr_t)alternate_aperture_base;
		uint64_t limit = base + alternate_aperture_size - 1;

		if (limit <= base || (base & APE1_FIXED_BITS_MASK) != 0 ||
		   (limit & APE1_FIXED_BITS_MASK) != APE1_LIMIT_ALIGNMENT) {
			retval = false;
			goto out;
		}

		qpd->sh_mem_ape1_base = base >> 16;
		qpd->sh_mem_ape1_limit = limit >> 16;
	}

	retval = dqm->asic_ops.set_cache_memory_policy(
			dqm,
			qpd,
			default_policy,
			alternate_policy,
			alternate_aperture_base,
			alternate_aperture_size);

	if ((dqm->sched_policy == KFD_SCHED_POLICY_NO_HWS) && (qpd->vmid != 0))
		program_sh_mem_settings(dqm, qpd);

	pr_debug("sh_mem_config: 0x%x, ape1_base: 0x%x, ape1_limit: 0x%x\n",
		qpd->sh_mem_config, qpd->sh_mem_ape1_base,
		qpd->sh_mem_ape1_limit);

out:
	dqm_unlock(dqm);
	return retval;
}

<<<<<<< HEAD
static int set_trap_handler(struct device_queue_manager *dqm,
				struct qcm_process_device *qpd,
				uint64_t tba_addr,
				uint64_t tma_addr)
{
	uint64_t *tma;

	if (dqm->dev->cwsr_enabled) {
		/* Jump from CWSR trap handler to user trap */
		tma = (uint64_t *)(qpd->cwsr_kaddr + KFD_CWSR_TMA_OFFSET);
		tma[0] = tba_addr;
		tma[1] = tma_addr;
	} else {
		qpd->tba_addr = tba_addr;
		qpd->tma_addr = tma_addr;
	}

	return 0;
}

static int process_termination_nocpsch(struct device_queue_manager *dqm,
		struct qcm_process_device *qpd)
{
	struct queue *q, *next;
	struct device_process_node *cur, *next_dpn;
	int retval = 0;
=======
static int process_termination_nocpsch(struct device_queue_manager *dqm,
		struct qcm_process_device *qpd)
{
	struct queue *q;
	struct device_process_node *cur, *next_dpn;
	int retval = 0;
	bool found = false;
>>>>>>> upstream/android-13

	dqm_lock(dqm);

	/* Clear all user mode queues */
<<<<<<< HEAD
	list_for_each_entry_safe(q, next, &qpd->queues_list, list) {
		int ret;

		ret = destroy_queue_nocpsch_locked(dqm, qpd, q);
		if (ret)
			retval = ret;
=======
	while (!list_empty(&qpd->queues_list)) {
		struct mqd_manager *mqd_mgr;
		int ret;

		q = list_first_entry(&qpd->queues_list, struct queue, list);
		mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
				q->properties.type)];
		ret = destroy_queue_nocpsch_locked(dqm, qpd, q);
		if (ret)
			retval = ret;
		dqm_unlock(dqm);
		mqd_mgr->free_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);
		dqm_lock(dqm);
>>>>>>> upstream/android-13
	}

	/* Unregister process */
	list_for_each_entry_safe(cur, next_dpn, &dqm->queues, list) {
		if (qpd == cur->qpd) {
			list_del(&cur->list);
			kfree(cur);
			dqm->processes_count--;
<<<<<<< HEAD
=======
			found = true;
>>>>>>> upstream/android-13
			break;
		}
	}

	dqm_unlock(dqm);
<<<<<<< HEAD
	return retval;
}

=======

	/* Outside the DQM lock because under the DQM lock we can't do
	 * reclaim or take other locks that others hold while reclaiming.
	 */
	if (found)
		kfd_dec_compute_active(dqm->dev);

	return retval;
}

static int get_wave_state(struct device_queue_manager *dqm,
			  struct queue *q,
			  void __user *ctl_stack,
			  u32 *ctl_stack_used_size,
			  u32 *save_area_used_size)
{
	struct mqd_manager *mqd_mgr;

	dqm_lock(dqm);

	mqd_mgr = dqm->mqd_mgrs[KFD_MQD_TYPE_CP];

	if (q->properties.type != KFD_QUEUE_TYPE_COMPUTE ||
	    q->properties.is_active || !q->device->cwsr_enabled ||
	    !mqd_mgr->get_wave_state) {
		dqm_unlock(dqm);
		return -EINVAL;
	}

	dqm_unlock(dqm);

	/*
	 * get_wave_state is outside the dqm lock to prevent circular locking
	 * and the queue should be protected against destruction by the process
	 * lock.
	 */
	return mqd_mgr->get_wave_state(mqd_mgr, q->mqd, ctl_stack,
			ctl_stack_used_size, save_area_used_size);
}
>>>>>>> upstream/android-13

static int process_termination_cpsch(struct device_queue_manager *dqm,
		struct qcm_process_device *qpd)
{
	int retval;
<<<<<<< HEAD
	struct queue *q, *next;
=======
	struct queue *q;
>>>>>>> upstream/android-13
	struct kernel_queue *kq, *kq_next;
	struct mqd_manager *mqd_mgr;
	struct device_process_node *cur, *next_dpn;
	enum kfd_unmap_queues_filter filter =
		KFD_UNMAP_QUEUES_FILTER_DYNAMIC_QUEUES;
<<<<<<< HEAD
=======
	bool found = false;
>>>>>>> upstream/android-13

	retval = 0;

	dqm_lock(dqm);

	/* Clean all kernel queues */
	list_for_each_entry_safe(kq, kq_next, &qpd->priv_queue_list, list) {
		list_del(&kq->list);
<<<<<<< HEAD
		dqm->queue_count--;
=======
		decrement_queue_count(dqm, qpd, kq->queue);
>>>>>>> upstream/android-13
		qpd->is_debug = false;
		dqm->total_queue_count--;
		filter = KFD_UNMAP_QUEUES_FILTER_ALL_QUEUES;
	}

	/* Clear all user mode queues */
	list_for_each_entry(q, &qpd->queues_list, list) {
<<<<<<< HEAD
		if (q->properties.type == KFD_QUEUE_TYPE_SDMA) {
			dqm->sdma_queue_count--;
			deallocate_sdma_queue(dqm, q->sdma_id);
		}

		if (q->properties.is_active)
			dqm->queue_count--;
=======
		if (q->properties.type == KFD_QUEUE_TYPE_SDMA)
			deallocate_sdma_queue(dqm, q);
		else if (q->properties.type == KFD_QUEUE_TYPE_SDMA_XGMI)
			deallocate_sdma_queue(dqm, q);

		if (q->properties.is_active)
			decrement_queue_count(dqm, qpd, q);
>>>>>>> upstream/android-13

		dqm->total_queue_count--;
	}

	/* Unregister process */
	list_for_each_entry_safe(cur, next_dpn, &dqm->queues, list) {
		if (qpd == cur->qpd) {
			list_del(&cur->list);
			kfree(cur);
			dqm->processes_count--;
<<<<<<< HEAD
=======
			found = true;
>>>>>>> upstream/android-13
			break;
		}
	}

	retval = execute_queues_cpsch(dqm, filter, 0);
	if ((!dqm->is_hws_hang) && (retval || qpd->reset_wavefronts)) {
		pr_warn("Resetting wave fronts (cpsch) on dev %p\n", dqm->dev);
		dbgdev_wave_reset_wavefronts(dqm->dev, qpd->pqm->process);
		qpd->reset_wavefronts = false;
	}

<<<<<<< HEAD
	/* lastly, free mqd resources */
	list_for_each_entry_safe(q, next, &qpd->queues_list, list) {
		mqd_mgr = dqm->ops.get_mqd_manager(dqm,
			get_mqd_type_from_queue_type(q->properties.type));
		if (!mqd_mgr) {
			retval = -ENOMEM;
			goto out;
		}
		list_del(&q->list);
		qpd->queue_count--;
		mqd_mgr->uninit_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);
	}

out:
	dqm_unlock(dqm);
=======
	/* Lastly, free mqd resources.
	 * Do free_mqd() after dqm_unlock to avoid circular locking.
	 */
	while (!list_empty(&qpd->queues_list)) {
		q = list_first_entry(&qpd->queues_list, struct queue, list);
		mqd_mgr = dqm->mqd_mgrs[get_mqd_type_from_queue_type(
				q->properties.type)];
		list_del(&q->list);
		qpd->queue_count--;
		dqm_unlock(dqm);
		mqd_mgr->free_mqd(mqd_mgr, q->mqd, q->mqd_mem_obj);
		dqm_lock(dqm);
	}
	dqm_unlock(dqm);

	/* Outside the DQM lock because under the DQM lock we can't do
	 * reclaim or take other locks that others hold while reclaiming.
	 */
	if (found)
		kfd_dec_compute_active(dqm->dev);

	return retval;
}

static int init_mqd_managers(struct device_queue_manager *dqm)
{
	int i, j;
	struct mqd_manager *mqd_mgr;

	for (i = 0; i < KFD_MQD_TYPE_MAX; i++) {
		mqd_mgr = dqm->asic_ops.mqd_manager_init(i, dqm->dev);
		if (!mqd_mgr) {
			pr_err("mqd manager [%d] initialization failed\n", i);
			goto out_free;
		}
		dqm->mqd_mgrs[i] = mqd_mgr;
	}

	return 0;

out_free:
	for (j = 0; j < i; j++) {
		kfree(dqm->mqd_mgrs[j]);
		dqm->mqd_mgrs[j] = NULL;
	}

	return -ENOMEM;
}

/* Allocate one hiq mqd (HWS) and all SDMA mqd in a continuous trunk*/
static int allocate_hiq_sdma_mqd(struct device_queue_manager *dqm)
{
	int retval;
	struct kfd_dev *dev = dqm->dev;
	struct kfd_mem_obj *mem_obj = &dqm->hiq_sdma_mqd;
	uint32_t size = dqm->mqd_mgrs[KFD_MQD_TYPE_SDMA]->mqd_size *
		get_num_all_sdma_engines(dqm) *
		dev->device_info->num_sdma_queues_per_engine +
		dqm->mqd_mgrs[KFD_MQD_TYPE_HIQ]->mqd_size;

	retval = amdgpu_amdkfd_alloc_gtt_mem(dev->kgd, size,
		&(mem_obj->gtt_mem), &(mem_obj->gpu_addr),
		(void *)&(mem_obj->cpu_ptr), false);

>>>>>>> upstream/android-13
	return retval;
}

struct device_queue_manager *device_queue_manager_init(struct kfd_dev *dev)
{
	struct device_queue_manager *dqm;

	pr_debug("Loading device queue manager\n");

	dqm = kzalloc(sizeof(*dqm), GFP_KERNEL);
	if (!dqm)
		return NULL;

	switch (dev->device_info->asic_family) {
	/* HWS is not available on Hawaii. */
	case CHIP_HAWAII:
	/* HWS depends on CWSR for timely dequeue. CWSR is not
	 * available on Tonga.
	 *
	 * FIXME: This argument also applies to Kaveri.
	 */
	case CHIP_TONGA:
		dqm->sched_policy = KFD_SCHED_POLICY_NO_HWS;
		break;
	default:
		dqm->sched_policy = sched_policy;
		break;
	}

	dqm->dev = dev;
	switch (dqm->sched_policy) {
	case KFD_SCHED_POLICY_HWS:
	case KFD_SCHED_POLICY_HWS_NO_OVERSUBSCRIPTION:
		/* initialize dqm for cp scheduling */
		dqm->ops.create_queue = create_queue_cpsch;
		dqm->ops.initialize = initialize_cpsch;
		dqm->ops.start = start_cpsch;
		dqm->ops.stop = stop_cpsch;
<<<<<<< HEAD
		dqm->ops.destroy_queue = destroy_queue_cpsch;
		dqm->ops.update_queue = update_queue;
		dqm->ops.get_mqd_manager = get_mqd_manager;
=======
		dqm->ops.pre_reset = pre_reset;
		dqm->ops.destroy_queue = destroy_queue_cpsch;
		dqm->ops.update_queue = update_queue;
>>>>>>> upstream/android-13
		dqm->ops.register_process = register_process;
		dqm->ops.unregister_process = unregister_process;
		dqm->ops.uninitialize = uninitialize;
		dqm->ops.create_kernel_queue = create_kernel_queue_cpsch;
		dqm->ops.destroy_kernel_queue = destroy_kernel_queue_cpsch;
		dqm->ops.set_cache_memory_policy = set_cache_memory_policy;
<<<<<<< HEAD
		dqm->ops.set_trap_handler = set_trap_handler;
		dqm->ops.process_termination = process_termination_cpsch;
		dqm->ops.evict_process_queues = evict_process_queues_cpsch;
		dqm->ops.restore_process_queues = restore_process_queues_cpsch;
=======
		dqm->ops.process_termination = process_termination_cpsch;
		dqm->ops.evict_process_queues = evict_process_queues_cpsch;
		dqm->ops.restore_process_queues = restore_process_queues_cpsch;
		dqm->ops.get_wave_state = get_wave_state;
>>>>>>> upstream/android-13
		break;
	case KFD_SCHED_POLICY_NO_HWS:
		/* initialize dqm for no cp scheduling */
		dqm->ops.start = start_nocpsch;
		dqm->ops.stop = stop_nocpsch;
<<<<<<< HEAD
		dqm->ops.create_queue = create_queue_nocpsch;
		dqm->ops.destroy_queue = destroy_queue_nocpsch;
		dqm->ops.update_queue = update_queue;
		dqm->ops.get_mqd_manager = get_mqd_manager;
=======
		dqm->ops.pre_reset = pre_reset;
		dqm->ops.create_queue = create_queue_nocpsch;
		dqm->ops.destroy_queue = destroy_queue_nocpsch;
		dqm->ops.update_queue = update_queue;
>>>>>>> upstream/android-13
		dqm->ops.register_process = register_process;
		dqm->ops.unregister_process = unregister_process;
		dqm->ops.initialize = initialize_nocpsch;
		dqm->ops.uninitialize = uninitialize;
		dqm->ops.set_cache_memory_policy = set_cache_memory_policy;
<<<<<<< HEAD
		dqm->ops.set_trap_handler = set_trap_handler;
=======
>>>>>>> upstream/android-13
		dqm->ops.process_termination = process_termination_nocpsch;
		dqm->ops.evict_process_queues = evict_process_queues_nocpsch;
		dqm->ops.restore_process_queues =
			restore_process_queues_nocpsch;
<<<<<<< HEAD
=======
		dqm->ops.get_wave_state = get_wave_state;
>>>>>>> upstream/android-13
		break;
	default:
		pr_err("Invalid scheduling policy %d\n", dqm->sched_policy);
		goto out_free;
	}

	switch (dev->device_info->asic_family) {
	case CHIP_CARRIZO:
		device_queue_manager_init_vi(&dqm->asic_ops);
		break;

	case CHIP_KAVERI:
		device_queue_manager_init_cik(&dqm->asic_ops);
		break;

	case CHIP_HAWAII:
		device_queue_manager_init_cik_hawaii(&dqm->asic_ops);
		break;

	case CHIP_TONGA:
	case CHIP_FIJI:
	case CHIP_POLARIS10:
	case CHIP_POLARIS11:
<<<<<<< HEAD
=======
	case CHIP_POLARIS12:
	case CHIP_VEGAM:
>>>>>>> upstream/android-13
		device_queue_manager_init_vi_tonga(&dqm->asic_ops);
		break;

	case CHIP_VEGA10:
<<<<<<< HEAD
	case CHIP_RAVEN:
		device_queue_manager_init_v9(&dqm->asic_ops);
		break;
=======
	case CHIP_VEGA12:
	case CHIP_VEGA20:
	case CHIP_RAVEN:
	case CHIP_RENOIR:
	case CHIP_ARCTURUS:
	case CHIP_ALDEBARAN:
		device_queue_manager_init_v9(&dqm->asic_ops);
		break;
	case CHIP_NAVI10:
	case CHIP_NAVI12:
	case CHIP_NAVI14:
	case CHIP_SIENNA_CICHLID:
	case CHIP_NAVY_FLOUNDER:
	case CHIP_VANGOGH:
	case CHIP_DIMGREY_CAVEFISH:
	case CHIP_BEIGE_GOBY:
	case CHIP_YELLOW_CARP:
	case CHIP_CYAN_SKILLFISH:
		device_queue_manager_init_v10_navi10(&dqm->asic_ops);
		break;
>>>>>>> upstream/android-13
	default:
		WARN(1, "Unexpected ASIC family %u",
		     dev->device_info->asic_family);
		goto out_free;
	}

<<<<<<< HEAD
=======
	if (init_mqd_managers(dqm))
		goto out_free;

	if (allocate_hiq_sdma_mqd(dqm)) {
		pr_err("Failed to allocate hiq sdma mqd trunk buffer\n");
		goto out_free;
	}

>>>>>>> upstream/android-13
	if (!dqm->ops.initialize(dqm))
		return dqm;

out_free:
	kfree(dqm);
	return NULL;
}

<<<<<<< HEAD
void device_queue_manager_uninit(struct device_queue_manager *dqm)
{
	dqm->ops.uninitialize(dqm);
	kfree(dqm);
}

int kfd_process_vm_fault(struct device_queue_manager *dqm,
			 unsigned int pasid)
=======
static void deallocate_hiq_sdma_mqd(struct kfd_dev *dev,
				    struct kfd_mem_obj *mqd)
{
	WARN(!mqd, "No hiq sdma mqd trunk to free");

	amdgpu_amdkfd_free_gtt_mem(dev->kgd, mqd->gtt_mem);
}

void device_queue_manager_uninit(struct device_queue_manager *dqm)
{
	dqm->ops.uninitialize(dqm);
	deallocate_hiq_sdma_mqd(dqm->dev, &dqm->hiq_sdma_mqd);
	kfree(dqm);
}

int kfd_process_vm_fault(struct device_queue_manager *dqm, u32 pasid)
>>>>>>> upstream/android-13
{
	struct kfd_process_device *pdd;
	struct kfd_process *p = kfd_lookup_process_by_pasid(pasid);
	int ret = 0;

	if (!p)
		return -EINVAL;
<<<<<<< HEAD
=======
	WARN(debug_evictions, "Evicting pid %d", p->lead_thread->pid);
>>>>>>> upstream/android-13
	pdd = kfd_get_process_device_data(dqm->dev, p);
	if (pdd)
		ret = dqm->ops.evict_process_queues(dqm, &pdd->qpd);
	kfd_unref_process(p);

	return ret;
}

static void kfd_process_hw_exception(struct work_struct *work)
{
	struct device_queue_manager *dqm = container_of(work,
			struct device_queue_manager, hw_exception_work);
<<<<<<< HEAD
	dqm->dev->kfd2kgd->gpu_recover(dqm->dev->kgd);
=======
	amdgpu_amdkfd_gpu_reset(dqm->dev->kgd);
>>>>>>> upstream/android-13
}

#if defined(CONFIG_DEBUG_FS)

static void seq_reg_dump(struct seq_file *m,
			 uint32_t (*dump)[2], uint32_t n_regs)
{
	uint32_t i, count;

	for (i = 0, count = 0; i < n_regs; i++) {
		if (count == 0 ||
		    dump[i-1][0] + sizeof(uint32_t) != dump[i][0]) {
			seq_printf(m, "%s    %08x: %08x",
				   i ? "\n" : "",
				   dump[i][0], dump[i][1]);
			count = 7;
		} else {
			seq_printf(m, " %08x", dump[i][1]);
			count--;
		}
	}

	seq_puts(m, "\n");
}

int dqm_debugfs_hqds(struct seq_file *m, void *data)
{
	struct device_queue_manager *dqm = data;
	uint32_t (*dump)[2], n_regs;
	int pipe, queue;
	int r = 0;

<<<<<<< HEAD
	r = dqm->dev->kfd2kgd->hqd_dump(dqm->dev->kgd,
		KFD_CIK_HIQ_PIPE, KFD_CIK_HIQ_QUEUE, &dump, &n_regs);
	if (!r) {
		seq_printf(m, "  HIQ on MEC %d Pipe %d Queue %d\n",
				KFD_CIK_HIQ_PIPE/get_pipes_per_mec(dqm)+1,
				KFD_CIK_HIQ_PIPE%get_pipes_per_mec(dqm),
				KFD_CIK_HIQ_QUEUE);
=======
	if (!dqm->sched_running) {
		seq_printf(m, " Device is stopped\n");

		return 0;
	}

	r = dqm->dev->kfd2kgd->hqd_dump(dqm->dev->kgd,
					KFD_CIK_HIQ_PIPE, KFD_CIK_HIQ_QUEUE,
					&dump, &n_regs);
	if (!r) {
		seq_printf(m, "  HIQ on MEC %d Pipe %d Queue %d\n",
			   KFD_CIK_HIQ_PIPE/get_pipes_per_mec(dqm)+1,
			   KFD_CIK_HIQ_PIPE%get_pipes_per_mec(dqm),
			   KFD_CIK_HIQ_QUEUE);
>>>>>>> upstream/android-13
		seq_reg_dump(m, dump, n_regs);

		kfree(dump);
	}

	for (pipe = 0; pipe < get_pipes_per_mec(dqm); pipe++) {
		int pipe_offset = pipe * get_queues_per_pipe(dqm);

		for (queue = 0; queue < get_queues_per_pipe(dqm); queue++) {
			if (!test_bit(pipe_offset + queue,
<<<<<<< HEAD
				      dqm->dev->shared_resources.queue_bitmap))
=======
				      dqm->dev->shared_resources.cp_queue_bitmap))
>>>>>>> upstream/android-13
				continue;

			r = dqm->dev->kfd2kgd->hqd_dump(
				dqm->dev->kgd, pipe, queue, &dump, &n_regs);
			if (r)
				break;

			seq_printf(m, "  CP Pipe %d, Queue %d\n",
				  pipe, queue);
			seq_reg_dump(m, dump, n_regs);

			kfree(dump);
		}
	}

<<<<<<< HEAD
	for (pipe = 0; pipe < get_num_sdma_engines(dqm); pipe++) {
		for (queue = 0; queue < KFD_SDMA_QUEUES_PER_ENGINE; queue++) {
=======
	for (pipe = 0; pipe < get_num_all_sdma_engines(dqm); pipe++) {
		for (queue = 0;
		     queue < dqm->dev->device_info->num_sdma_queues_per_engine;
		     queue++) {
>>>>>>> upstream/android-13
			r = dqm->dev->kfd2kgd->hqd_sdma_dump(
				dqm->dev->kgd, pipe, queue, &dump, &n_regs);
			if (r)
				break;

			seq_printf(m, "  SDMA Engine %d, RLC %d\n",
				  pipe, queue);
			seq_reg_dump(m, dump, n_regs);

			kfree(dump);
		}
	}

	return r;
}

<<<<<<< HEAD
int dqm_debugfs_execute_queues(struct device_queue_manager *dqm)
=======
int dqm_debugfs_hang_hws(struct device_queue_manager *dqm)
>>>>>>> upstream/android-13
{
	int r = 0;

	dqm_lock(dqm);
<<<<<<< HEAD
=======
	r = pm_debugfs_hang_hws(&dqm->packet_mgr);
	if (r) {
		dqm_unlock(dqm);
		return r;
	}
>>>>>>> upstream/android-13
	dqm->active_runlist = true;
	r = execute_queues_cpsch(dqm, KFD_UNMAP_QUEUES_FILTER_ALL_QUEUES, 0);
	dqm_unlock(dqm);

	return r;
}

#endif
