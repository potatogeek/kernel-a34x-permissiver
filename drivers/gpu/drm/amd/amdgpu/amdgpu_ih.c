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

<<<<<<< HEAD
#include <drm/drmP.h>
#include "amdgpu.h"
#include "amdgpu_ih.h"
#include "amdgpu_amdkfd.h"

/**
 * amdgpu_ih_ring_alloc - allocate memory for the IH ring
 *
 * @adev: amdgpu_device pointer
 *
 * Allocate a ring buffer for the interrupt controller.
 * Returns 0 for success, errors for failure.
 */
static int amdgpu_ih_ring_alloc(struct amdgpu_device *adev)
{
	int r;

	/* Allocate ring buffer */
	if (adev->irq.ih.ring_obj == NULL) {
		r = amdgpu_bo_create_kernel(adev, adev->irq.ih.ring_size,
					    PAGE_SIZE, AMDGPU_GEM_DOMAIN_GTT,
					    &adev->irq.ih.ring_obj,
					    &adev->irq.ih.gpu_addr,
					    (void **)&adev->irq.ih.ring);
		if (r) {
			DRM_ERROR("amdgpu: failed to create ih ring buffer (%d).\n", r);
			return r;
		}
	}
	return 0;
}
=======
#include <linux/dma-mapping.h>

#include "amdgpu.h"
#include "amdgpu_ih.h"
>>>>>>> upstream/android-13

/**
 * amdgpu_ih_ring_init - initialize the IH state
 *
 * @adev: amdgpu_device pointer
<<<<<<< HEAD
=======
 * @ih: ih ring to initialize
 * @ring_size: ring size to allocate
 * @use_bus_addr: true when we can use dma_alloc_coherent
>>>>>>> upstream/android-13
 *
 * Initializes the IH state and allocates a buffer
 * for the IH ring buffer.
 * Returns 0 for success, errors for failure.
 */
<<<<<<< HEAD
int amdgpu_ih_ring_init(struct amdgpu_device *adev, unsigned ring_size,
			bool use_bus_addr)
=======
int amdgpu_ih_ring_init(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih,
			unsigned ring_size, bool use_bus_addr)
>>>>>>> upstream/android-13
{
	u32 rb_bufsz;
	int r;

	/* Align ring size */
	rb_bufsz = order_base_2(ring_size / 4);
	ring_size = (1 << rb_bufsz) * 4;
<<<<<<< HEAD
	adev->irq.ih.ring_size = ring_size;
	adev->irq.ih.ptr_mask = adev->irq.ih.ring_size - 1;
	adev->irq.ih.rptr = 0;
	adev->irq.ih.use_bus_addr = use_bus_addr;

	if (adev->irq.ih.use_bus_addr) {
		if (!adev->irq.ih.ring) {
			/* add 8 bytes for the rptr/wptr shadows and
			 * add them to the end of the ring allocation.
			 */
			adev->irq.ih.ring = pci_alloc_consistent(adev->pdev,
								 adev->irq.ih.ring_size + 8,
								 &adev->irq.ih.rb_dma_addr);
			if (adev->irq.ih.ring == NULL)
				return -ENOMEM;
			memset((void *)adev->irq.ih.ring, 0, adev->irq.ih.ring_size + 8);
			adev->irq.ih.wptr_offs = (adev->irq.ih.ring_size / 4) + 0;
			adev->irq.ih.rptr_offs = (adev->irq.ih.ring_size / 4) + 1;
		}
		return 0;
	} else {
		r = amdgpu_device_wb_get(adev, &adev->irq.ih.wptr_offs);
		if (r) {
			dev_err(adev->dev, "(%d) ih wptr_offs wb alloc failed\n", r);
			return r;
		}

		r = amdgpu_device_wb_get(adev, &adev->irq.ih.rptr_offs);
		if (r) {
			amdgpu_device_wb_free(adev, adev->irq.ih.wptr_offs);
			dev_err(adev->dev, "(%d) ih rptr_offs wb alloc failed\n", r);
			return r;
		}

		return amdgpu_ih_ring_alloc(adev);
	}
=======
	ih->ring_size = ring_size;
	ih->ptr_mask = ih->ring_size - 1;
	ih->rptr = 0;
	ih->use_bus_addr = use_bus_addr;

	if (use_bus_addr) {
		dma_addr_t dma_addr;

		if (ih->ring)
			return 0;

		/* add 8 bytes for the rptr/wptr shadows and
		 * add them to the end of the ring allocation.
		 */
		ih->ring = dma_alloc_coherent(adev->dev, ih->ring_size + 8,
					      &dma_addr, GFP_KERNEL);
		if (ih->ring == NULL)
			return -ENOMEM;

		ih->gpu_addr = dma_addr;
		ih->wptr_addr = dma_addr + ih->ring_size;
		ih->wptr_cpu = &ih->ring[ih->ring_size / 4];
		ih->rptr_addr = dma_addr + ih->ring_size + 4;
		ih->rptr_cpu = &ih->ring[(ih->ring_size / 4) + 1];
	} else {
		unsigned wptr_offs, rptr_offs;

		r = amdgpu_device_wb_get(adev, &wptr_offs);
		if (r)
			return r;

		r = amdgpu_device_wb_get(adev, &rptr_offs);
		if (r) {
			amdgpu_device_wb_free(adev, wptr_offs);
			return r;
		}

		r = amdgpu_bo_create_kernel(adev, ih->ring_size, PAGE_SIZE,
					    AMDGPU_GEM_DOMAIN_GTT,
					    &ih->ring_obj, &ih->gpu_addr,
					    (void **)&ih->ring);
		if (r) {
			amdgpu_device_wb_free(adev, rptr_offs);
			amdgpu_device_wb_free(adev, wptr_offs);
			return r;
		}

		ih->wptr_addr = adev->wb.gpu_addr + wptr_offs * 4;
		ih->wptr_cpu = &adev->wb.wb[wptr_offs];
		ih->rptr_addr = adev->wb.gpu_addr + rptr_offs * 4;
		ih->rptr_cpu = &adev->wb.wb[rptr_offs];
	}

	init_waitqueue_head(&ih->wait_process);
	return 0;
>>>>>>> upstream/android-13
}

/**
 * amdgpu_ih_ring_fini - tear down the IH state
 *
 * @adev: amdgpu_device pointer
<<<<<<< HEAD
=======
 * @ih: ih ring to tear down
>>>>>>> upstream/android-13
 *
 * Tears down the IH state and frees buffer
 * used for the IH ring buffer.
 */
<<<<<<< HEAD
void amdgpu_ih_ring_fini(struct amdgpu_device *adev)
{
	if (adev->irq.ih.use_bus_addr) {
		if (adev->irq.ih.ring) {
			/* add 8 bytes for the rptr/wptr shadows and
			 * add them to the end of the ring allocation.
			 */
			pci_free_consistent(adev->pdev, adev->irq.ih.ring_size + 8,
					    (void *)adev->irq.ih.ring,
					    adev->irq.ih.rb_dma_addr);
			adev->irq.ih.ring = NULL;
		}
	} else {
		amdgpu_bo_free_kernel(&adev->irq.ih.ring_obj,
				      &adev->irq.ih.gpu_addr,
				      (void **)&adev->irq.ih.ring);
		amdgpu_device_wb_free(adev, adev->irq.ih.wptr_offs);
		amdgpu_device_wb_free(adev, adev->irq.ih.rptr_offs);
=======
void amdgpu_ih_ring_fini(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih)
{

	if (!ih->ring)
		return;

	if (ih->use_bus_addr) {

		/* add 8 bytes for the rptr/wptr shadows and
		 * add them to the end of the ring allocation.
		 */
		dma_free_coherent(adev->dev, ih->ring_size + 8,
				  (void *)ih->ring, ih->gpu_addr);
		ih->ring = NULL;
	} else {
		amdgpu_bo_free_kernel(&ih->ring_obj, &ih->gpu_addr,
				      (void **)&ih->ring);
		amdgpu_device_wb_free(adev, (ih->wptr_addr - ih->gpu_addr) / 4);
		amdgpu_device_wb_free(adev, (ih->rptr_addr - ih->gpu_addr) / 4);
>>>>>>> upstream/android-13
	}
}

/**
<<<<<<< HEAD
 * amdgpu_ih_process - interrupt handler
 *
 * @adev: amdgpu_device pointer
=======
 * amdgpu_ih_ring_write - write IV to the ring buffer
 *
 * @ih: ih ring to write to
 * @iv: the iv to write
 * @num_dw: size of the iv in dw
 *
 * Writes an IV to the ring buffer using the CPU and increment the wptr.
 * Used for testing and delegating IVs to a software ring.
 */
void amdgpu_ih_ring_write(struct amdgpu_ih_ring *ih, const uint32_t *iv,
			  unsigned int num_dw)
{
	uint32_t wptr = le32_to_cpu(*ih->wptr_cpu) >> 2;
	unsigned int i;

	for (i = 0; i < num_dw; ++i)
	        ih->ring[wptr++] = cpu_to_le32(iv[i]);

	wptr <<= 2;
	wptr &= ih->ptr_mask;

	/* Only commit the new wptr if we don't overflow */
	if (wptr != READ_ONCE(ih->rptr)) {
		wmb();
		WRITE_ONCE(*ih->wptr_cpu, cpu_to_le32(wptr));
	}
}

/* Waiter helper that checks current rptr matches or passes checkpoint wptr */
static bool amdgpu_ih_has_checkpoint_processed(struct amdgpu_device *adev,
					struct amdgpu_ih_ring *ih,
					uint32_t checkpoint_wptr,
					uint32_t *prev_rptr)
{
	uint32_t cur_rptr = ih->rptr | (*prev_rptr & ~ih->ptr_mask);

	/* rptr has wrapped. */
	if (cur_rptr < *prev_rptr)
		cur_rptr += ih->ptr_mask + 1;
	*prev_rptr = cur_rptr;

	/* check ring is empty to workaround missing wptr overflow flag */
	return cur_rptr >= checkpoint_wptr ||
	       (cur_rptr & ih->ptr_mask) == amdgpu_ih_get_wptr(adev, ih);
}

/**
 * amdgpu_ih_wait_on_checkpoint_process - wait to process IVs up to checkpoint
 *
 * @adev: amdgpu_device pointer
 * @ih: ih ring to process
 *
 * Used to ensure ring has processed IVs up to the checkpoint write pointer.
 */
int amdgpu_ih_wait_on_checkpoint_process(struct amdgpu_device *adev,
					struct amdgpu_ih_ring *ih)
{
	uint32_t checkpoint_wptr, rptr;

	if (!ih->enabled || adev->shutdown)
		return -ENODEV;

	checkpoint_wptr = amdgpu_ih_get_wptr(adev, ih);
	/* Order wptr with rptr. */
	rmb();
	rptr = READ_ONCE(ih->rptr);

	/* wptr has wrapped. */
	if (rptr > checkpoint_wptr)
		checkpoint_wptr += ih->ptr_mask + 1;

	return wait_event_interruptible(ih->wait_process,
				amdgpu_ih_has_checkpoint_processed(adev, ih,
						checkpoint_wptr, &rptr));
}

/**
 * amdgpu_ih_process - interrupt handler
 *
 * @adev: amdgpu_device pointer
 * @ih: ih ring to process
>>>>>>> upstream/android-13
 *
 * Interrupt hander (VI), walk the IH ring.
 * Returns irq process return code.
 */
<<<<<<< HEAD
int amdgpu_ih_process(struct amdgpu_device *adev)
{
	struct amdgpu_iv_entry entry;
	u32 wptr;

	if (!adev->irq.ih.enabled || adev->shutdown)
		return IRQ_NONE;

	wptr = amdgpu_ih_get_wptr(adev);

restart_ih:
	/* is somebody else already processing irqs? */
	if (atomic_xchg(&adev->irq.ih.lock, 1))
		return IRQ_NONE;

	DRM_DEBUG("%s: rptr %d, wptr %d\n", __func__, adev->irq.ih.rptr, wptr);
=======
int amdgpu_ih_process(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih)
{
	unsigned int count;
	u32 wptr;

	if (!ih->enabled || adev->shutdown)
		return IRQ_NONE;

	wptr = amdgpu_ih_get_wptr(adev, ih);

restart_ih:
	count  = AMDGPU_IH_MAX_NUM_IVS;
	DRM_DEBUG("%s: rptr %d, wptr %d\n", __func__, ih->rptr, wptr);
>>>>>>> upstream/android-13

	/* Order reading of wptr vs. reading of IH ring data */
	rmb();

<<<<<<< HEAD
	while (adev->irq.ih.rptr != wptr) {
		u32 ring_index = adev->irq.ih.rptr >> 2;

		/* Prescreening of high-frequency interrupts */
		if (!amdgpu_ih_prescreen_iv(adev)) {
			adev->irq.ih.rptr &= adev->irq.ih.ptr_mask;
			continue;
		}

		/* Before dispatching irq to IP blocks, send it to amdkfd */
		amdgpu_amdkfd_interrupt(adev,
				(const void *) &adev->irq.ih.ring[ring_index]);

		entry.iv_entry = (const uint32_t *)
			&adev->irq.ih.ring[ring_index];
		amdgpu_ih_decode_iv(adev, &entry);
		adev->irq.ih.rptr &= adev->irq.ih.ptr_mask;

		amdgpu_irq_dispatch(adev, &entry);
	}
	amdgpu_ih_set_rptr(adev);
	atomic_set(&adev->irq.ih.lock, 0);

	/* make sure wptr hasn't changed while processing */
	wptr = amdgpu_ih_get_wptr(adev);
	if (wptr != adev->irq.ih.rptr)
=======
	while (ih->rptr != wptr && --count) {
		amdgpu_irq_dispatch(adev, ih);
		ih->rptr &= ih->ptr_mask;
	}

	amdgpu_ih_set_rptr(adev, ih);
	wake_up_all(&ih->wait_process);

	/* make sure wptr hasn't changed while processing */
	wptr = amdgpu_ih_get_wptr(adev, ih);
	if (wptr != ih->rptr)
>>>>>>> upstream/android-13
		goto restart_ih;

	return IRQ_HANDLED;
}

/**
<<<<<<< HEAD
 * amdgpu_ih_add_fault - Add a page fault record
 *
 * @adev: amdgpu device pointer
 * @key: 64-bit encoding of PASID and address
 *
 * This should be called when a retry page fault interrupt is
 * received. If this is a new page fault, it will be added to a hash
 * table. The return value indicates whether this is a new fault, or
 * a fault that was already known and is already being handled.
 *
 * If there are too many pending page faults, this will fail. Retry
 * interrupts should be ignored in this case until there is enough
 * free space.
 *
 * Returns 0 if the fault was added, 1 if the fault was already known,
 * -ENOSPC if there are too many pending faults.
 */
int amdgpu_ih_add_fault(struct amdgpu_device *adev, u64 key)
{
	unsigned long flags;
	int r = -ENOSPC;

	if (WARN_ON_ONCE(!adev->irq.ih.faults))
		/* Should be allocated in <IP>_ih_sw_init on GPUs that
		 * support retry faults and require retry filtering.
		 */
		return r;

	spin_lock_irqsave(&adev->irq.ih.faults->lock, flags);

	/* Only let the hash table fill up to 50% for best performance */
	if (adev->irq.ih.faults->count >= (1 << (AMDGPU_PAGEFAULT_HASH_BITS-1)))
		goto unlock_out;

	r = chash_table_copy_in(&adev->irq.ih.faults->hash, key, NULL);
	if (!r)
		adev->irq.ih.faults->count++;

	/* chash_table_copy_in should never fail unless we're losing count */
	WARN_ON_ONCE(r < 0);

unlock_out:
	spin_unlock_irqrestore(&adev->irq.ih.faults->lock, flags);
	return r;
}

/**
 * amdgpu_ih_clear_fault - Remove a page fault record
 *
 * @adev: amdgpu device pointer
 * @key: 64-bit encoding of PASID and address
 *
 * This should be called when a page fault has been handled. Any
 * future interrupt with this key will be processed as a new
 * page fault.
 */
void amdgpu_ih_clear_fault(struct amdgpu_device *adev, u64 key)
{
	unsigned long flags;
	int r;

	if (!adev->irq.ih.faults)
		return;

	spin_lock_irqsave(&adev->irq.ih.faults->lock, flags);

	r = chash_table_remove(&adev->irq.ih.faults->hash, key, NULL);
	if (!WARN_ON_ONCE(r < 0)) {
		adev->irq.ih.faults->count--;
		WARN_ON_ONCE(adev->irq.ih.faults->count < 0);
	}

	spin_unlock_irqrestore(&adev->irq.ih.faults->lock, flags);
=======
 * amdgpu_ih_decode_iv_helper - decode an interrupt vector
 *
 * @adev: amdgpu_device pointer
 * @ih: ih ring to process
 * @entry: IV entry
 *
 * Decodes the interrupt vector at the current rptr
 * position and also advance the position for for Vega10
 * and later GPUs.
 */
void amdgpu_ih_decode_iv_helper(struct amdgpu_device *adev,
				struct amdgpu_ih_ring *ih,
				struct amdgpu_iv_entry *entry)
{
	/* wptr/rptr are in bytes! */
	u32 ring_index = ih->rptr >> 2;
	uint32_t dw[8];

	dw[0] = le32_to_cpu(ih->ring[ring_index + 0]);
	dw[1] = le32_to_cpu(ih->ring[ring_index + 1]);
	dw[2] = le32_to_cpu(ih->ring[ring_index + 2]);
	dw[3] = le32_to_cpu(ih->ring[ring_index + 3]);
	dw[4] = le32_to_cpu(ih->ring[ring_index + 4]);
	dw[5] = le32_to_cpu(ih->ring[ring_index + 5]);
	dw[6] = le32_to_cpu(ih->ring[ring_index + 6]);
	dw[7] = le32_to_cpu(ih->ring[ring_index + 7]);

	entry->client_id = dw[0] & 0xff;
	entry->src_id = (dw[0] >> 8) & 0xff;
	entry->ring_id = (dw[0] >> 16) & 0xff;
	entry->vmid = (dw[0] >> 24) & 0xf;
	entry->vmid_src = (dw[0] >> 31);
	entry->timestamp = dw[1] | ((u64)(dw[2] & 0xffff) << 32);
	entry->timestamp_src = dw[2] >> 31;
	entry->pasid = dw[3] & 0xffff;
	entry->pasid_src = dw[3] >> 31;
	entry->src_data[0] = dw[4];
	entry->src_data[1] = dw[5];
	entry->src_data[2] = dw[6];
	entry->src_data[3] = dw[7];

	/* wptr/rptr are in bytes! */
	ih->rptr += 32;
>>>>>>> upstream/android-13
}
