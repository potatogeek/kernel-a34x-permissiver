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

#ifndef __AMDGPU_IH_H__
#define __AMDGPU_IH_H__

<<<<<<< HEAD
#include <linux/chash.h>
#include "soc15_ih_clientid.h"

struct amdgpu_device;

#define AMDGPU_IH_CLIENTID_LEGACY 0
#define AMDGPU_IH_CLIENTID_MAX SOC15_IH_CLIENTID_MAX

#define AMDGPU_PAGEFAULT_HASH_BITS 8
struct amdgpu_retryfault_hashtable {
	DECLARE_CHASH_TABLE(hash, AMDGPU_PAGEFAULT_HASH_BITS, 8, 0);
	spinlock_t	lock;
	int		count;
=======
/* Maximum number of IVs processed at once */
#define AMDGPU_IH_MAX_NUM_IVS	32

struct amdgpu_device;
struct amdgpu_iv_entry;

struct amdgpu_ih_regs {
	uint32_t ih_rb_base;
	uint32_t ih_rb_base_hi;
	uint32_t ih_rb_cntl;
	uint32_t ih_rb_wptr;
	uint32_t ih_rb_rptr;
	uint32_t ih_doorbell_rptr;
	uint32_t ih_rb_wptr_addr_lo;
	uint32_t ih_rb_wptr_addr_hi;
	uint32_t psp_reg_id;
>>>>>>> upstream/android-13
};

/*
 * R6xx+ IH ring
 */
struct amdgpu_ih_ring {
<<<<<<< HEAD
	struct amdgpu_bo	*ring_obj;
	volatile uint32_t	*ring;
	unsigned		rptr;
	unsigned		ring_size;
	uint64_t		gpu_addr;
	uint32_t		ptr_mask;
	atomic_t		lock;
	bool                    enabled;
	unsigned		wptr_offs;
	unsigned		rptr_offs;
	u32			doorbell_index;
	bool			use_doorbell;
	bool			use_bus_addr;
	dma_addr_t		rb_dma_addr; /* only used when use_bus_addr = true */
	struct amdgpu_retryfault_hashtable *faults;
};

#define AMDGPU_IH_SRC_DATA_MAX_SIZE_DW 4

struct amdgpu_iv_entry {
	unsigned client_id;
	unsigned src_id;
	unsigned ring_id;
	unsigned vmid;
	unsigned vmid_src;
	uint64_t timestamp;
	unsigned timestamp_src;
	unsigned pasid;
	unsigned pasid_src;
	unsigned src_data[AMDGPU_IH_SRC_DATA_MAX_SIZE_DW];
	const uint32_t *iv_entry;
};

int amdgpu_ih_ring_init(struct amdgpu_device *adev, unsigned ring_size,
			bool use_bus_addr);
void amdgpu_ih_ring_fini(struct amdgpu_device *adev);
int amdgpu_ih_process(struct amdgpu_device *adev);
int amdgpu_ih_add_fault(struct amdgpu_device *adev, u64 key);
void amdgpu_ih_clear_fault(struct amdgpu_device *adev, u64 key);

=======
	unsigned		ring_size;
	uint32_t		ptr_mask;
	u32			doorbell_index;
	bool			use_doorbell;
	bool			use_bus_addr;

	struct amdgpu_bo	*ring_obj;
	volatile uint32_t	*ring;
	uint64_t		gpu_addr;

	uint64_t		wptr_addr;
	volatile uint32_t	*wptr_cpu;

	uint64_t		rptr_addr;
	volatile uint32_t	*rptr_cpu;

	bool                    enabled;
	unsigned		rptr;
	struct amdgpu_ih_regs	ih_regs;

	/* For waiting on IH processing at checkpoint. */
	wait_queue_head_t wait_process;
};

/* provided by the ih block */
struct amdgpu_ih_funcs {
	/* ring read/write ptr handling, called from interrupt context */
	u32 (*get_wptr)(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih);
	void (*decode_iv)(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih,
			  struct amdgpu_iv_entry *entry);
	void (*set_rptr)(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih);
};

#define amdgpu_ih_get_wptr(adev, ih) (adev)->irq.ih_funcs->get_wptr((adev), (ih))
#define amdgpu_ih_decode_iv(adev, iv) \
	(adev)->irq.ih_funcs->decode_iv((adev), (ih), (iv))
#define amdgpu_ih_set_rptr(adev, ih) (adev)->irq.ih_funcs->set_rptr((adev), (ih))

int amdgpu_ih_ring_init(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih,
			unsigned ring_size, bool use_bus_addr);
void amdgpu_ih_ring_fini(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih);
void amdgpu_ih_ring_write(struct amdgpu_ih_ring *ih, const uint32_t *iv,
			  unsigned int num_dw);
int amdgpu_ih_wait_on_checkpoint_process(struct amdgpu_device *adev,
					struct amdgpu_ih_ring *ih);
int amdgpu_ih_process(struct amdgpu_device *adev, struct amdgpu_ih_ring *ih);
void amdgpu_ih_decode_iv_helper(struct amdgpu_device *adev,
				struct amdgpu_ih_ring *ih,
				struct amdgpu_iv_entry *entry);
>>>>>>> upstream/android-13
#endif
