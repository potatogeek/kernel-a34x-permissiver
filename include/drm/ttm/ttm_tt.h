/**************************************************************************
 *
 * Copyright (c) 2006-2009 Vmware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
#ifndef _TTM_TT_H_
#define _TTM_TT_H_

<<<<<<< HEAD
#include <linux/types.h>

struct ttm_tt;
struct ttm_mem_reg;
struct ttm_buffer_object;
struct ttm_operation_ctx;

#define TTM_PAGE_FLAG_WRITE           (1 << 3)
#define TTM_PAGE_FLAG_SWAPPED         (1 << 4)
#define TTM_PAGE_FLAG_PERSISTENT_SWAP (1 << 5)
#define TTM_PAGE_FLAG_ZERO_ALLOC      (1 << 6)
#define TTM_PAGE_FLAG_DMA32           (1 << 7)
#define TTM_PAGE_FLAG_SG              (1 << 8)
#define TTM_PAGE_FLAG_NO_RETRY	      (1 << 9)

enum ttm_caching_state {
	tt_uncached,
	tt_wc,
	tt_cached
};

struct ttm_backend_func {
	/**
	 * struct ttm_backend_func member bind
	 *
	 * @ttm: Pointer to a struct ttm_tt.
	 * @bo_mem: Pointer to a struct ttm_mem_reg describing the
	 * memory type and location for binding.
	 *
	 * Bind the backend pages into the aperture in the location
	 * indicated by @bo_mem. This function should be able to handle
	 * differences between aperture and system page sizes.
	 */
	int (*bind) (struct ttm_tt *ttm, struct ttm_mem_reg *bo_mem);

	/**
	 * struct ttm_backend_func member unbind
	 *
	 * @ttm: Pointer to a struct ttm_tt.
	 *
	 * Unbind previously bound backend pages. This function should be
	 * able to handle differences between aperture and system page sizes.
	 */
	int (*unbind) (struct ttm_tt *ttm);

	/**
	 * struct ttm_backend_func member destroy
	 *
	 * @ttm: Pointer to a struct ttm_tt.
	 *
	 * Destroy the backend. This will be call back from ttm_tt_destroy so
	 * don't call ttm_tt_destroy from the callback or infinite loop.
	 */
	void (*destroy) (struct ttm_tt *ttm);
};
=======
#include <linux/pagemap.h>
#include <linux/types.h>
#include <drm/ttm/ttm_caching.h>
#include <drm/ttm/ttm_kmap_iter.h>

struct ttm_device;
struct ttm_tt;
struct ttm_resource;
struct ttm_buffer_object;
struct ttm_operation_ctx;

#define TTM_PAGE_FLAG_SWAPPED         (1 << 4)
#define TTM_PAGE_FLAG_ZERO_ALLOC      (1 << 6)
#define TTM_PAGE_FLAG_SG              (1 << 8)
#define TTM_PAGE_FLAG_NO_RETRY	      (1 << 9)

#define TTM_PAGE_FLAG_PRIV_POPULATED  (1 << 31)
>>>>>>> upstream/android-13

/**
 * struct ttm_tt
 *
<<<<<<< HEAD
 * @bdev: Pointer to a struct ttm_bo_device.
 * @func: Pointer to a struct ttm_backend_func that describes
 * the backend methods.
 * pointer.
 * @pages: Array of pages backing the data.
 * @num_pages: Number of pages in the page array.
 * @bdev: Pointer to the current struct ttm_bo_device.
 * @be: Pointer to the ttm backend.
 * @swap_storage: Pointer to shmem struct file for swap storage.
 * @caching_state: The current caching state of the pages.
 * @state: The current binding state of the pages.
=======
 * @pages: Array of pages backing the data.
 * @page_flags: see TTM_PAGE_FLAG_*
 * @num_pages: Number of pages in the page array.
 * @sg: for SG objects via dma-buf
 * @dma_address: The DMA (bus) addresses of the pages
 * @swap_storage: Pointer to shmem struct file for swap storage.
 * @pages_list: used by some page allocation backend
 * @caching: The current caching state of the pages.
>>>>>>> upstream/android-13
 *
 * This is a structure holding the pages, caching- and aperture binding
 * status for a buffer object that isn't backed by fixed (VRAM / AGP)
 * memory.
 */
struct ttm_tt {
<<<<<<< HEAD
	struct ttm_bo_device *bdev;
	struct ttm_backend_func *func;
	struct page **pages;
	uint32_t page_flags;
	unsigned long num_pages;
	struct sg_table *sg; /* for SG objects via dma-buf */
	struct file *swap_storage;
	enum ttm_caching_state caching_state;
	enum {
		tt_bound,
		tt_unbound,
		tt_unpopulated,
	} state;
};

/**
 * struct ttm_dma_tt
 *
 * @ttm: Base ttm_tt struct.
 * @dma_address: The DMA (bus) addresses of the pages
 * @pages_list: used by some page allocation backend
 *
 * This is a structure holding the pages, caching- and aperture binding
 * status for a buffer object that isn't backed by fixed (VRAM / AGP)
 * memory.
 */
struct ttm_dma_tt {
	struct ttm_tt ttm;
	dma_addr_t *dma_address;
	struct list_head pages_list;
};

=======
	struct page **pages;
	uint32_t page_flags;
	uint32_t num_pages;
	struct sg_table *sg;
	dma_addr_t *dma_address;
	struct file *swap_storage;
	enum ttm_caching caching;
};

/**
 * struct ttm_kmap_iter_tt - Specialization of a mappig iterator for a tt.
 * @base: Embedded struct ttm_kmap_iter providing the usage interface
 * @tt: Cached struct ttm_tt.
 * @prot: Cached page protection for mapping.
 */
struct ttm_kmap_iter_tt {
	struct ttm_kmap_iter base;
	struct ttm_tt *tt;
	pgprot_t prot;
};

static inline bool ttm_tt_is_populated(struct ttm_tt *tt)
{
	return tt->page_flags & TTM_PAGE_FLAG_PRIV_POPULATED;
}

>>>>>>> upstream/android-13
/**
 * ttm_tt_create
 *
 * @bo: pointer to a struct ttm_buffer_object
 * @zero_alloc: true if allocated pages needs to be zeroed
 *
 * Make sure we have a TTM structure allocated for the given BO.
 * No pages are actually allocated.
 */
int ttm_tt_create(struct ttm_buffer_object *bo, bool zero_alloc);

/**
 * ttm_tt_init
 *
 * @ttm: The struct ttm_tt.
 * @bo: The buffer object we create the ttm for.
 * @page_flags: Page flags as identified by TTM_PAGE_FLAG_XX flags.
<<<<<<< HEAD
=======
 * @caching: the desired caching state of the pages
>>>>>>> upstream/android-13
 *
 * Create a struct ttm_tt to back data with system memory pages.
 * No pages are actually allocated.
 * Returns:
 * NULL: Out of memory.
 */
int ttm_tt_init(struct ttm_tt *ttm, struct ttm_buffer_object *bo,
<<<<<<< HEAD
		uint32_t page_flags);
int ttm_dma_tt_init(struct ttm_dma_tt *ttm_dma, struct ttm_buffer_object *bo,
		    uint32_t page_flags);
int ttm_sg_tt_init(struct ttm_dma_tt *ttm_dma, struct ttm_buffer_object *bo,
		   uint32_t page_flags);
=======
		uint32_t page_flags, enum ttm_caching caching);
int ttm_sg_tt_init(struct ttm_tt *ttm_dma, struct ttm_buffer_object *bo,
		   uint32_t page_flags, enum ttm_caching caching);
>>>>>>> upstream/android-13

/**
 * ttm_tt_fini
 *
 * @ttm: the ttm_tt structure.
 *
 * Free memory of ttm_tt structure
 */
void ttm_tt_fini(struct ttm_tt *ttm);
<<<<<<< HEAD
void ttm_dma_tt_fini(struct ttm_dma_tt *ttm_dma);

/**
 * ttm_ttm_bind:
 *
 * @ttm: The struct ttm_tt containing backing pages.
 * @bo_mem: The struct ttm_mem_reg identifying the binding location.
 *
 * Bind the pages of @ttm to an aperture location identified by @bo_mem
 */
int ttm_tt_bind(struct ttm_tt *ttm, struct ttm_mem_reg *bo_mem,
		struct ttm_operation_ctx *ctx);
=======
>>>>>>> upstream/android-13

/**
 * ttm_ttm_destroy:
 *
 * @ttm: The struct ttm_tt.
 *
 * Unbind, unpopulate and destroy common struct ttm_tt.
 */
<<<<<<< HEAD
void ttm_tt_destroy(struct ttm_tt *ttm);

/**
 * ttm_ttm_unbind:
 *
 * @ttm: The struct ttm_tt.
 *
 * Unbind a struct ttm_tt.
 */
void ttm_tt_unbind(struct ttm_tt *ttm);
=======
void ttm_tt_destroy(struct ttm_device *bdev, struct ttm_tt *ttm);

/**
 * ttm_tt_destroy_common:
 *
 * Called from driver to destroy common path.
 */
void ttm_tt_destroy_common(struct ttm_device *bdev, struct ttm_tt *ttm);
>>>>>>> upstream/android-13

/**
 * ttm_tt_swapin:
 *
 * @ttm: The struct ttm_tt.
 *
 * Swap in a previously swap out ttm_tt.
 */
int ttm_tt_swapin(struct ttm_tt *ttm);
<<<<<<< HEAD

/**
 * ttm_tt_set_placement_caching:
 *
 * @ttm A struct ttm_tt the backing pages of which will change caching policy.
 * @placement: Flag indicating the desired caching policy.
 *
 * This function will change caching policy of any default kernel mappings of
 * the pages backing @ttm. If changing from cached to uncached or
 * write-combined,
 * all CPU caches will first be flushed to make sure the data of the pages
 * hit RAM. This function may be very costly as it involves global TLB
 * and cache flushes and potential page splitting / combining.
 */
int ttm_tt_set_placement_caching(struct ttm_tt *ttm, uint32_t placement);
int ttm_tt_swapout(struct ttm_tt *ttm, struct file *persistent_swap_storage);
=======
int ttm_tt_swapout(struct ttm_device *bdev, struct ttm_tt *ttm,
		   gfp_t gfp_flags);
>>>>>>> upstream/android-13

/**
 * ttm_tt_populate - allocate pages for a ttm
 *
 * @ttm: Pointer to the ttm_tt structure
 *
 * Calls the driver method to allocate pages for a ttm
 */
<<<<<<< HEAD
int ttm_tt_populate(struct ttm_tt *ttm, struct ttm_operation_ctx *ctx);
=======
int ttm_tt_populate(struct ttm_device *bdev, struct ttm_tt *ttm, struct ttm_operation_ctx *ctx);
>>>>>>> upstream/android-13

/**
 * ttm_tt_unpopulate - free pages from a ttm
 *
 * @ttm: Pointer to the ttm_tt structure
 *
 * Calls the driver method to free all pages from a ttm
 */
<<<<<<< HEAD
void ttm_tt_unpopulate(struct ttm_tt *ttm);
=======
void ttm_tt_unpopulate(struct ttm_device *bdev, struct ttm_tt *ttm);

/**
 * ttm_tt_mark_for_clear - Mark pages for clearing on populate.
 *
 * @ttm: Pointer to the ttm_tt structure
 *
 * Marks pages for clearing so that the next time the page vector is
 * populated, the pages will be cleared.
 */
static inline void ttm_tt_mark_for_clear(struct ttm_tt *ttm)
{
	ttm->page_flags |= TTM_PAGE_FLAG_ZERO_ALLOC;
}

void ttm_tt_mgr_init(unsigned long num_pages, unsigned long num_dma32_pages);

struct ttm_kmap_iter *ttm_kmap_iter_tt_init(struct ttm_kmap_iter_tt *iter_tt,
					    struct ttm_tt *tt);
>>>>>>> upstream/android-13

#if IS_ENABLED(CONFIG_AGP)
#include <linux/agp_backend.h>

/**
 * ttm_agp_tt_create
 *
 * @bo: Buffer object we allocate the ttm for.
 * @bridge: The agp bridge this device is sitting on.
 * @page_flags: Page flags as identified by TTM_PAGE_FLAG_XX flags.
 *
 *
 * Create a TTM backend that uses the indicated AGP bridge as an aperture
 * for TT memory. This function uses the linux agpgart interface to
 * bind and unbind memory backing a ttm_tt.
 */
struct ttm_tt *ttm_agp_tt_create(struct ttm_buffer_object *bo,
				 struct agp_bridge_data *bridge,
				 uint32_t page_flags);
<<<<<<< HEAD
int ttm_agp_tt_populate(struct ttm_tt *ttm, struct ttm_operation_ctx *ctx);
void ttm_agp_tt_unpopulate(struct ttm_tt *ttm);
=======
int ttm_agp_bind(struct ttm_tt *ttm, struct ttm_resource *bo_mem);
void ttm_agp_unbind(struct ttm_tt *ttm);
void ttm_agp_destroy(struct ttm_tt *ttm);
bool ttm_agp_is_bound(struct ttm_tt *ttm);
>>>>>>> upstream/android-13
#endif

#endif
