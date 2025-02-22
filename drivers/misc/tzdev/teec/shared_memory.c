/*
<<<<<<< HEAD
 * Copyright (C) 2012-2019, Samsung Electronics Co., Ltd.
=======
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
>>>>>>> upstream/android-13
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/export.h>
<<<<<<< HEAD
=======
#include <linux/mm.h>
>>>>>>> upstream/android-13
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/page.h>

#include <tzdev/tee_client_api.h>

#include "iw_messages.h"
#include "misc.h"
#include "types.h"
#include "core/log.h"
<<<<<<< HEAD
#include "core/mem.h"
#include "core/wait.h"

#define PTR_ALIGN_PGDN(p)	((typeof(p))(((uintptr_t)(p)) & PAGE_MASK))

static void tzdev_teec_release_shared_memory(void *data)
{
	struct tzdev_teec_shared_memory *shm = data;

	shm->released = 1;
	wake_up(&shm->wq);
}

static uint32_t tzdev_teec_shared_memory_init(TEEC_Context *context, TEEC_SharedMemory *sharedMem,
		struct page **pages, unsigned int num_pages)
=======
#include "core/wait.h"
#include "core/iw_shmem.h"

#define PTR_ALIGN_PGDN(p)	((typeof(p))(((uintptr_t)(p)) & PAGE_MASK))
#define OFFSET_IN_PAGE(x)	((x) & (~PAGE_MASK))

static uint32_t tzdev_teec_shared_memory_init(TEEC_Context *context,
		TEEC_SharedMemory *sharedMem)
>>>>>>> upstream/android-13
{
	struct tzdev_teec_shared_memory *shm;
	uint32_t result;
	void *addr;
	size_t size;
	unsigned int offset;
<<<<<<< HEAD
=======
	unsigned int flags = TZDEV_IW_SHMEM_FLAG_SYNC
			| (sharedMem->flags & TEEC_MEM_OUTPUT ? TZDEV_IW_SHMEM_FLAG_WRITE : 0);
>>>>>>> upstream/android-13
	int ret;

	log_debug(tzdev_teec, "Enter, context = %pK sharedMem = %pK buffer = %pK size = %zu flags = %x\n",
			context, sharedMem, sharedMem->buffer, sharedMem->size, sharedMem->flags);

	shm = kmalloc(sizeof(struct tzdev_teec_shared_memory), GFP_KERNEL);
	if (!shm) {
		log_error(tzdev_teec, "Failed to allocate shared memory struct\n");
		result = TEEC_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	addr = PTR_ALIGN_PGDN(sharedMem->buffer);
	offset = OFFSET_IN_PAGE((uintptr_t)sharedMem->buffer);
	size = PAGE_ALIGN(sharedMem->size + offset);

<<<<<<< HEAD
	shm->id = 0;

	if (sharedMem->size) {
		ret = tzdev_mem_register(addr, size,
				(sharedMem->flags & TEEC_MEM_OUTPUT) ? 1 : 0,
				tzdev_teec_release_shared_memory, shm);
		if (ret < 0) {
			log_error(tzdev_teec, "Failed to register shared memory, ret = %d\n", ret);
			result = tzdev_teec_error_to_tee_error(ret);
			kfree(shm);
			goto out;
		}

		shm->id = ret;
	}

	init_waitqueue_head(&shm->wq);
	shm->released = 0;
	shm->context = context;
	shm->pages = pages;
	shm->num_pages = num_pages;
=======
	shm->is_allocated = !sharedMem->buffer;

	if (shm->is_allocated)
		/* According to the TEE Client API Specification the size is allowed to be zero. In this case
		 * sharedMem->buffer MUST not be NULL. So we register 1 byte shared memory and write its
		 * pointer to the buffer. */
		ret = tzdev_iw_shmem_register(&sharedMem->buffer, size ? size : 1, flags);
	else
		ret = tzdev_iw_shmem_register_exist(addr, size, flags);
	if (ret < 0) {
		log_error(tzdev_teec, "Failed to register shared memory, ret = %d\n", ret);
		result = tzdev_teec_error_to_tee_error(ret);
		kfree(shm);
		goto out;
	}

	shm->id = ret;
	shm->context = context;
>>>>>>> upstream/android-13
	shm->offset = offset;

	sharedMem->imp = shm;

	result = TEEC_SUCCESS;

out:
	log_debug(tzdev_teec, "Exit, context = %pK sharedMem = %pK buffer = %pK size = %zu flags = %x\n",
			context, sharedMem, sharedMem->buffer, sharedMem->size, sharedMem->flags);

	return result;
}

static void tzdev_teec_shared_memory_fini(TEEC_SharedMemory *sharedMem)
{
	struct tzdev_teec_shared_memory *shm = sharedMem->imp;

	log_debug(tzdev_teec, "Enter, sharedMem = %pK id = %u\n", sharedMem, shm->id);

	sharedMem->imp = NULL;

<<<<<<< HEAD
	if (shm->id) {
		BUG_ON(tzdev_mem_release(shm->id));
		wait_event_uninterruptible_freezable_nested(shm->wq, shm->released == 1);
	}
=======
	if (shm->id)
		BUG_ON(tzdev_iw_shmem_release(shm->id));
>>>>>>> upstream/android-13

	kfree(shm);

	log_debug(tzdev_teec, "Exit, sharedMem = %pK\n", sharedMem);
}

static uint32_t tzdev_teec_grant_shared_memory_rights(TEEC_SharedMemory *sharedMem, uint32_t *origin)
{
	struct tzdev_teec_shared_memory *shm = sharedMem->imp;
	struct tzdev_teec_context *ctx = shm->context->imp;
	struct cmd_set_shared_memory_rights cmd;
	struct cmd_reply_set_shared_memory_rights ack;
	uint32_t result;
	int ret;

	log_debug(tzdev_teec, "Enter, sharedMem = %pK\n", sharedMem);

	*origin = TEEC_ORIGIN_API;

	cmd.base.cmd = CMD_SET_SHMEM_RIGHTS;
	cmd.base.serial = ctx->serial;
	cmd.buf_desc.offset = shm->offset;
	cmd.buf_desc.size = sharedMem->size;
	cmd.buf_desc.id = shm->id;

	ret = tzdev_teec_send_then_recv(ctx->socket,
			&cmd, sizeof(cmd), 0x0,
			&ack, sizeof(ack), 0x0,
			&result, origin);
	if (ret < 0) {
		log_error(tzdev_teec, "Failed to xmit set shmem rights, ret = %d\n", ret);
		goto out;
	}

	ret = tzdev_teec_check_reply(&ack.base, CMD_REPLY_SET_SHMEM_RIGHTS,
			ctx->serial, &result, origin);
	if (ret) {
		log_error(tzdev_teec, "Failed to check set shmem rights reply, ret = %d\n", ret);
		goto out;
	}

	result = ack.base.result;
	*origin = ack.base.origin;

out:
	ctx->serial++;

	tzdev_teec_fixup_origin(result, origin);

	log_debug(tzdev_teec, "Exit, sharedMem = %pK\n", sharedMem);

	return result;
}

<<<<<<< HEAD
static uint32_t tzdev_teec_register_shared_memory(TEEC_Context *context,
		TEEC_SharedMemory *sharedMem, struct page **pages,
		unsigned int num_pages, uint32_t *origin)
{
	struct tzdev_teec_context *ctx = context->imp;
	uint32_t result;

	log_debug(tzdev_teec, "Enter, context = %pK sharedMem = %pK\n", context, sharedMem);

	*origin = TEEC_ORIGIN_API;

	result = tzdev_teec_shared_memory_init(context, sharedMem, pages, num_pages);
	if (result != TEEC_SUCCESS) {
		log_error(tzdev_teec, "Failed to create shared memory, context = %pK sharedMem = %pK\n",
				context, sharedMem);
		goto out;
	}

	if (sharedMem->size) {
		mutex_lock(&ctx->mutex);
		result = tzdev_teec_grant_shared_memory_rights(sharedMem, origin);
		mutex_unlock(&ctx->mutex);

		if (result != TEEC_SUCCESS) {
			log_error(tzdev_teec, "Failed to grant shared memory rights, context = %pK sharedMem = %pK\n",
					context, sharedMem);
			tzdev_teec_shared_memory_fini(sharedMem);
			goto out;
		}
	}

out:
	tzdev_teec_fixup_origin(result, origin);

	log_debug(tzdev_teec, "Exit, context = %pK sharedMem = %pK\n", context, sharedMem);

	return result;
}

=======
>>>>>>> upstream/android-13
static uint32_t tzdev_teec_shared_memory_check_args(TEEC_Context *context,
		TEEC_SharedMemory *sharedMem, bool null_buffer)
{
	if (!sharedMem) {
		log_error(tzdev_teec, "Null shared memory passed\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (null_buffer)
		sharedMem->buffer = NULL;

	if (!context) {
		log_error(tzdev_teec, "Null context passed\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (sharedMem->size > TEEC_CONFIG_SHAREDMEM_MAX_SIZE) {
		log_error(tzdev_teec, "Too big shared memory requested, size = %zu max = %u\n",
				sharedMem->size, TEEC_CONFIG_SHAREDMEM_MAX_SIZE);
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (sharedMem->flags & ~(TEEC_MEM_INPUT | TEEC_MEM_OUTPUT)) {
		log_error(tzdev_teec, "Invalid flags passed, flags = %x\n", sharedMem->flags);
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (!sharedMem->buffer && !null_buffer) {
		log_error(tzdev_teec, "No buffer provided for shared memory\n");
		return TEEC_ERROR_NO_DATA;
	}

	return TEEC_SUCCESS;
}

<<<<<<< HEAD
TEEC_Result TEEC_RegisterSharedMemory(TEEC_Context *context, TEEC_SharedMemory *sharedMem)
{
	struct tzdev_teec_shared_memory *shm;
	uint32_t result;
	uint32_t origin;
=======
static uint32_t tzdev_teec_register_shared_memory(TEEC_Context *context, TEEC_SharedMemory *sharedMem,
		bool null_buffer)
{
	struct tzdev_teec_context *ctx;
	struct tzdev_teec_shared_memory *shm;
	uint32_t origin;
	uint32_t result;
>>>>>>> upstream/android-13

	log_debug(tzdev_teec, "Enter, context = %pK sharedMem = %pK\n", context, sharedMem);

	origin = TEEC_ORIGIN_API;

<<<<<<< HEAD
	result = tzdev_teec_shared_memory_check_args(context, sharedMem, false);
	if (result != TEEC_SUCCESS)
		goto out;

	result = tzdev_teec_register_shared_memory(context, sharedMem, NULL, 0, &origin);
	if (result != TEEC_SUCCESS)
		goto out;
=======
	result = tzdev_teec_shared_memory_check_args(context, sharedMem, null_buffer);
	if (result != TEEC_SUCCESS)
		goto out;

	ctx = context->imp;

	result = tzdev_teec_shared_memory_init(context, sharedMem);
	if (result != TEEC_SUCCESS) {
		log_error(tzdev_teec, "Failed to create shared memory, context = %pK sharedMem = %pK\n",
				context, sharedMem);
		goto out;
	}

	mutex_lock(&ctx->mutex);
	result = tzdev_teec_grant_shared_memory_rights(sharedMem, &origin);
	mutex_unlock(&ctx->mutex);

	if (result != TEEC_SUCCESS) {
		log_error(tzdev_teec, "Failed to grant shared memory rights, context = %pK sharedMem = %pK\n",
				context, sharedMem);
		tzdev_teec_shared_memory_fini(sharedMem);
		goto out;
	}
>>>>>>> upstream/android-13

	shm = sharedMem->imp;

	log_debug(tzdev_teec, "Success, context = %pK sharedMem = %pK id = %u\n",
<<<<<<< HEAD
			context, sharedMem, shm->id);

out:
	log_debug(tzdev_teec, "Exit, context = %pK sharedMem = %pK result = %x origin = %u\n",
			context, sharedMem, result, origin);

	return result;
}
=======
		  context, sharedMem, shm->id);

out:
	tzdev_teec_fixup_origin(result, &origin);

	log_debug(tzdev_teec, "Exit, context = %pK sharedMem = %pK\n", context, sharedMem);

	return result;
}

TEEC_Result TEEC_RegisterSharedMemory(TEEC_Context *context, TEEC_SharedMemory *sharedMem)
{
	return tzdev_teec_register_shared_memory(context, sharedMem, false);
}
>>>>>>> upstream/android-13
EXPORT_SYMBOL(TEEC_RegisterSharedMemory);

TEEC_Result TEEC_AllocateSharedMemory(TEEC_Context *context, TEEC_SharedMemory *sharedMem)
{
<<<<<<< HEAD
	struct tzdev_teec_shared_memory *shm;
	struct page **pages;
	unsigned int num_pages;
	unsigned int i, j;
	uint32_t result;
	uint32_t origin;

	log_debug(tzdev_teec, "Enter, context = %pK sharedMem = %pK\n", context, sharedMem);

	origin = TEEC_ORIGIN_API;

	result = tzdev_teec_shared_memory_check_args(context, sharedMem, true);
	if (result != TEEC_SUCCESS)
		goto out;

	num_pages = NUM_PAGES(sharedMem->size);
	if (!num_pages)
		num_pages = 1;

	pages = kcalloc(num_pages, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		log_error(tzdev_teec, "Failed to allocate pages for shared memory\n");
		result = TEEC_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	for (i = 0; i < num_pages; ++i) {
		pages[i] = alloc_page(GFP_KERNEL);
		if (!pages[i])
			goto out_free_pages;
	}

	sharedMem->buffer = vmap(pages, num_pages, VM_MAP, PAGE_KERNEL);
	if (!sharedMem->buffer) {
		log_error(tzdev_teec, "Failed to vmap shared memory pages\n");
		result = TEEC_ERROR_OUT_OF_MEMORY;
		goto out_free_pages;
	}

	result = tzdev_teec_register_shared_memory(context, sharedMem, pages, num_pages, &origin);
	if (result != TEEC_SUCCESS)
		goto out_unmap_buffer;

	shm = sharedMem->imp;

	log_debug(tzdev_teec, "Success, context = %pK sharedMem = %pK id = %u\n",
			context, sharedMem, shm->id);
	goto out;

out_unmap_buffer:
	vunmap(sharedMem->buffer);
	sharedMem->buffer = NULL;
out_free_pages:
	for (j = 0; j < i; ++j)
		__free_page(pages[j]);
	kfree(pages);
out:
	log_debug(tzdev_teec, "Exit, context = %pK sharedMem = %pK result = %x origin = %u\n",
			context, sharedMem, result, origin);

	return result;
=======
	return tzdev_teec_register_shared_memory(context, sharedMem, true);
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(TEEC_AllocateSharedMemory);

void TEEC_ReleaseSharedMemory(TEEC_SharedMemory *sharedMem)
{
	struct tzdev_teec_shared_memory *shm;
<<<<<<< HEAD
	unsigned int i;
=======
>>>>>>> upstream/android-13
	uint32_t result = TEEC_SUCCESS;
	uint32_t origin;

	log_debug(tzdev_teec, "Enter, sharedMem = %pK\n", sharedMem);

	origin = TEEC_ORIGIN_API;

	if (!sharedMem || !sharedMem->imp) {
		log_error(tzdev_teec, "Null shared memory passed\n");
		result = TEEC_ERROR_BAD_PARAMETERS;
		goto out;
	}

	shm = sharedMem->imp;
<<<<<<< HEAD
	if (shm->num_pages) {
		vunmap(sharedMem->buffer);

		for (i = 0; i < shm->num_pages; ++i)
			__free_page(shm->pages[i]);

		kfree(shm->pages);

=======
	if (shm->is_allocated) {
>>>>>>> upstream/android-13
		sharedMem->buffer = NULL;
		sharedMem->size = 0;
	}

	tzdev_teec_shared_memory_fini(sharedMem);

out:
	log_debug(tzdev_teec, "Exit, sharedMem = %pK result = %x origin = %u\n",
			sharedMem, result, origin);
}
EXPORT_SYMBOL(TEEC_ReleaseSharedMemory);
