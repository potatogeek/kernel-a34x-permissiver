/*
 * Copyright (c) 2008 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Keith Packard <keithp@keithp.com>
 *    Mika Kuoppala <mika.kuoppala@intel.com>
 *
 */

<<<<<<< HEAD
#include <generated/utsrelease.h>
#include <linux/stop_machine.h>
#include <linux/zlib.h>
#include <drm/drm_print.h>
#include <linux/ascii85.h>

#include "i915_gpu_error.h"
#include "i915_drv.h"

static inline const struct intel_engine_cs *
engine_lookup(const struct drm_i915_private *i915, unsigned int id)
{
	if (id >= I915_NUM_ENGINES)
		return NULL;

	return i915->engine[id];
}

static inline const char *
__engine_name(const struct intel_engine_cs *engine)
{
	return engine ? engine->name : "";
}

static const char *
engine_name(const struct drm_i915_private *i915, unsigned int id)
{
	return __engine_name(engine_lookup(i915, id));
}

static const char *tiling_flag(int tiling)
{
	switch (tiling) {
	default:
	case I915_TILING_NONE: return "";
	case I915_TILING_X: return " X";
	case I915_TILING_Y: return " Y";
	}
}

static const char *dirty_flag(int dirty)
{
	return dirty ? " dirty" : "";
}

static const char *purgeable_flag(int purgeable)
{
	return purgeable ? " purgeable" : "";
}

static bool __i915_error_ok(struct drm_i915_error_state_buf *e)
{

	if (!e->err && WARN(e->bytes > (e->size - 1), "overflow")) {
		e->err = -ENOSPC;
		return false;
	}

	if (e->bytes == e->size - 1 || e->err)
		return false;

	return true;
}

static bool __i915_error_seek(struct drm_i915_error_state_buf *e,
			      unsigned len)
{
	if (e->pos + len <= e->start) {
		e->pos += len;
		return false;
	}

	/* First vsnprintf needs to fit in its entirety for memmove */
	if (len >= e->size) {
		e->err = -EIO;
		return false;
	}

	return true;
}

static void __i915_error_advance(struct drm_i915_error_state_buf *e,
				 unsigned len)
{
	/* If this is first printf in this window, adjust it so that
	 * start position matches start of the buffer
	 */

	if (e->pos < e->start) {
		const size_t off = e->start - e->pos;

		/* Should not happen but be paranoid */
		if (off > len || e->bytes) {
			e->err = -EIO;
			return;
		}

		memmove(e->buf, e->buf + off, len - off);
		e->bytes = len - off;
		e->pos = e->start;
		return;
	}

	e->bytes += len;
	e->pos += len;
=======
#include <linux/ascii85.h>
#include <linux/nmi.h>
#include <linux/pagevec.h>
#include <linux/scatterlist.h>
#include <linux/utsname.h>
#include <linux/zlib.h>

#include <drm/drm_print.h>

#include "display/intel_dmc.h"
#include "display/intel_overlay.h"

#include "gem/i915_gem_context.h"
#include "gem/i915_gem_lmem.h"
#include "gt/intel_gt.h"
#include "gt/intel_gt_pm.h"

#include "i915_drv.h"
#include "i915_gpu_error.h"
#include "i915_memcpy.h"
#include "i915_scatterlist.h"

#define ALLOW_FAIL (GFP_KERNEL | __GFP_RETRY_MAYFAIL | __GFP_NOWARN)
#define ATOMIC_MAYFAIL (GFP_ATOMIC | __GFP_NOWARN)

static void __sg_set_buf(struct scatterlist *sg,
			 void *addr, unsigned int len, loff_t it)
{
	sg->page_link = (unsigned long)virt_to_page(addr);
	sg->offset = offset_in_page(addr);
	sg->length = len;
	sg->dma_address = it;
}

static bool __i915_error_grow(struct drm_i915_error_state_buf *e, size_t len)
{
	if (!len)
		return false;

	if (e->bytes + len + 1 <= e->size)
		return true;

	if (e->bytes) {
		__sg_set_buf(e->cur++, e->buf, e->bytes, e->iter);
		e->iter += e->bytes;
		e->buf = NULL;
		e->bytes = 0;
	}

	if (e->cur == e->end) {
		struct scatterlist *sgl;

		sgl = (typeof(sgl))__get_free_page(ALLOW_FAIL);
		if (!sgl) {
			e->err = -ENOMEM;
			return false;
		}

		if (e->cur) {
			e->cur->offset = 0;
			e->cur->length = 0;
			e->cur->page_link =
				(unsigned long)sgl | SG_CHAIN;
		} else {
			e->sgl = sgl;
		}

		e->cur = sgl;
		e->end = sgl + SG_MAX_SINGLE_ALLOC - 1;
	}

	e->size = ALIGN(len + 1, SZ_64K);
	e->buf = kmalloc(e->size, ALLOW_FAIL);
	if (!e->buf) {
		e->size = PAGE_ALIGN(len + 1);
		e->buf = kmalloc(e->size, GFP_KERNEL);
	}
	if (!e->buf) {
		e->err = -ENOMEM;
		return false;
	}

	return true;
>>>>>>> upstream/android-13
}

__printf(2, 0)
static void i915_error_vprintf(struct drm_i915_error_state_buf *e,
<<<<<<< HEAD
			       const char *f, va_list args)
{
	unsigned len;

	if (!__i915_error_ok(e))
		return;

	/* Seek the first printf which is hits start position */
	if (e->pos < e->start) {
		va_list tmp;

		va_copy(tmp, args);
		len = vsnprintf(NULL, 0, f, tmp);
		va_end(tmp);

		if (!__i915_error_seek(e, len))
			return;
	}

	len = vsnprintf(e->buf + e->bytes, e->size - e->bytes, f, args);
	if (len >= e->size - e->bytes)
		len = e->size - e->bytes - 1;

	__i915_error_advance(e, len);
}

static void i915_error_puts(struct drm_i915_error_state_buf *e,
			    const char *str)
{
	unsigned len;

	if (!__i915_error_ok(e))
		return;

	len = strlen(str);

	/* Seek the first printf which is hits start position */
	if (e->pos < e->start) {
		if (!__i915_error_seek(e, len))
			return;
	}

	if (len >= e->size - e->bytes)
		len = e->size - e->bytes - 1;
	memcpy(e->buf + e->bytes, str, len);

	__i915_error_advance(e, len);
=======
			       const char *fmt, va_list args)
{
	va_list ap;
	int len;

	if (e->err)
		return;

	va_copy(ap, args);
	len = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	if (len <= 0) {
		e->err = len;
		return;
	}

	if (!__i915_error_grow(e, len))
		return;

	GEM_BUG_ON(e->bytes >= e->size);
	len = vscnprintf(e->buf + e->bytes, e->size - e->bytes, fmt, args);
	if (len < 0) {
		e->err = len;
		return;
	}
	e->bytes += len;
}

static void i915_error_puts(struct drm_i915_error_state_buf *e, const char *str)
{
	unsigned len;

	if (e->err || !str)
		return;

	len = strlen(str);
	if (!__i915_error_grow(e, len))
		return;

	GEM_BUG_ON(e->bytes + len > e->size);
	memcpy(e->buf + e->bytes, str, len);
	e->bytes += len;
>>>>>>> upstream/android-13
}

#define err_printf(e, ...) i915_error_printf(e, __VA_ARGS__)
#define err_puts(e, s) i915_error_puts(e, s)

static void __i915_printfn_error(struct drm_printer *p, struct va_format *vaf)
{
	i915_error_vprintf(p->arg, vaf->fmt, *vaf->va);
}

static inline struct drm_printer
i915_error_printer(struct drm_i915_error_state_buf *e)
{
	struct drm_printer p = {
		.printfn = __i915_printfn_error,
		.arg = e,
	};
	return p;
}

<<<<<<< HEAD
#ifdef CONFIG_DRM_I915_COMPRESS_ERROR

struct compress {
=======
/* single threaded page allocator with a reserved stash for emergencies */
static void pool_fini(struct pagevec *pv)
{
	pagevec_release(pv);
}

static int pool_refill(struct pagevec *pv, gfp_t gfp)
{
	while (pagevec_space(pv)) {
		struct page *p;

		p = alloc_page(gfp);
		if (!p)
			return -ENOMEM;

		pagevec_add(pv, p);
	}

	return 0;
}

static int pool_init(struct pagevec *pv, gfp_t gfp)
{
	int err;

	pagevec_init(pv);

	err = pool_refill(pv, gfp);
	if (err)
		pool_fini(pv);

	return err;
}

static void *pool_alloc(struct pagevec *pv, gfp_t gfp)
{
	struct page *p;

	p = alloc_page(gfp);
	if (!p && pagevec_count(pv))
		p = pv->pages[--pv->nr];

	return p ? page_address(p) : NULL;
}

static void pool_free(struct pagevec *pv, void *addr)
{
	struct page *p = virt_to_page(addr);

	if (pagevec_space(pv))
		pagevec_add(pv, p);
	else
		__free_page(p);
}

#ifdef CONFIG_DRM_I915_COMPRESS_ERROR

struct i915_vma_compress {
	struct pagevec pool;
>>>>>>> upstream/android-13
	struct z_stream_s zstream;
	void *tmp;
};

<<<<<<< HEAD
static bool compress_init(struct compress *c)
{
	struct z_stream_s *zstream = memset(&c->zstream, 0, sizeof(c->zstream));

	zstream->workspace =
		kmalloc(zlib_deflate_workspacesize(MAX_WBITS, MAX_MEM_LEVEL),
			GFP_ATOMIC | __GFP_NOWARN);
	if (!zstream->workspace)
		return false;

	if (zlib_deflateInit(zstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
		kfree(zstream->workspace);
=======
static bool compress_init(struct i915_vma_compress *c)
{
	struct z_stream_s *zstream = &c->zstream;

	if (pool_init(&c->pool, ALLOW_FAIL))
		return false;

	zstream->workspace =
		kmalloc(zlib_deflate_workspacesize(MAX_WBITS, MAX_MEM_LEVEL),
			ALLOW_FAIL);
	if (!zstream->workspace) {
		pool_fini(&c->pool);
>>>>>>> upstream/android-13
		return false;
	}

	c->tmp = NULL;
	if (i915_has_memcpy_from_wc())
<<<<<<< HEAD
		c->tmp = (void *)__get_free_page(GFP_ATOMIC | __GFP_NOWARN);
=======
		c->tmp = pool_alloc(&c->pool, ALLOW_FAIL);
>>>>>>> upstream/android-13

	return true;
}

<<<<<<< HEAD
static void *compress_next_page(struct drm_i915_error_object *dst)
{
	unsigned long page;
=======
static bool compress_start(struct i915_vma_compress *c)
{
	struct z_stream_s *zstream = &c->zstream;
	void *workspace = zstream->workspace;

	memset(zstream, 0, sizeof(*zstream));
	zstream->workspace = workspace;

	return zlib_deflateInit(zstream, Z_DEFAULT_COMPRESSION) == Z_OK;
}

static void *compress_next_page(struct i915_vma_compress *c,
				struct i915_vma_coredump *dst)
{
	void *page;
>>>>>>> upstream/android-13

	if (dst->page_count >= dst->num_pages)
		return ERR_PTR(-ENOSPC);

<<<<<<< HEAD
	page = __get_free_page(GFP_ATOMIC | __GFP_NOWARN);
	if (!page)
		return ERR_PTR(-ENOMEM);

	return dst->pages[dst->page_count++] = (void *)page;
}

static int compress_page(struct compress *c,
			 void *src,
			 struct drm_i915_error_object *dst)
=======
	page = pool_alloc(&c->pool, ALLOW_FAIL);
	if (!page)
		return ERR_PTR(-ENOMEM);

	return dst->pages[dst->page_count++] = page;
}

static int compress_page(struct i915_vma_compress *c,
			 void *src,
			 struct i915_vma_coredump *dst,
			 bool wc)
>>>>>>> upstream/android-13
{
	struct z_stream_s *zstream = &c->zstream;

	zstream->next_in = src;
<<<<<<< HEAD
	if (c->tmp && i915_memcpy_from_wc(c->tmp, src, PAGE_SIZE))
=======
	if (wc && c->tmp && i915_memcpy_from_wc(c->tmp, src, PAGE_SIZE))
>>>>>>> upstream/android-13
		zstream->next_in = c->tmp;
	zstream->avail_in = PAGE_SIZE;

	do {
		if (zstream->avail_out == 0) {
<<<<<<< HEAD
			zstream->next_out = compress_next_page(dst);
=======
			zstream->next_out = compress_next_page(c, dst);
>>>>>>> upstream/android-13
			if (IS_ERR(zstream->next_out))
				return PTR_ERR(zstream->next_out);

			zstream->avail_out = PAGE_SIZE;
		}

		if (zlib_deflate(zstream, Z_NO_FLUSH) != Z_OK)
			return -EIO;

		cond_resched();
	} while (zstream->avail_in);

	/* Fallback to uncompressed if we increase size? */
	if (0 && zstream->total_out > zstream->total_in)
		return -E2BIG;

	return 0;
}

<<<<<<< HEAD
static int compress_flush(struct compress *c,
			  struct drm_i915_error_object *dst)
=======
static int compress_flush(struct i915_vma_compress *c,
			  struct i915_vma_coredump *dst)
>>>>>>> upstream/android-13
{
	struct z_stream_s *zstream = &c->zstream;

	do {
		switch (zlib_deflate(zstream, Z_FINISH)) {
		case Z_OK: /* more space requested */
<<<<<<< HEAD
			zstream->next_out = compress_next_page(dst);
=======
			zstream->next_out = compress_next_page(c, dst);
>>>>>>> upstream/android-13
			if (IS_ERR(zstream->next_out))
				return PTR_ERR(zstream->next_out);

			zstream->avail_out = PAGE_SIZE;
			break;

		case Z_STREAM_END:
			goto end;

		default: /* any error */
			return -EIO;
		}
	} while (1);

end:
	memset(zstream->next_out, 0, zstream->avail_out);
	dst->unused = zstream->avail_out;
	return 0;
}

<<<<<<< HEAD
static void compress_fini(struct compress *c,
			  struct drm_i915_error_object *dst)
{
	struct z_stream_s *zstream = &c->zstream;

	zlib_deflateEnd(zstream);
	kfree(zstream->workspace);
	if (c->tmp)
		free_page((unsigned long)c->tmp);
=======
static void compress_finish(struct i915_vma_compress *c)
{
	zlib_deflateEnd(&c->zstream);
}

static void compress_fini(struct i915_vma_compress *c)
{
	kfree(c->zstream.workspace);
	if (c->tmp)
		pool_free(&c->pool, c->tmp);
	pool_fini(&c->pool);
>>>>>>> upstream/android-13
}

static void err_compression_marker(struct drm_i915_error_state_buf *m)
{
	err_puts(m, ":");
}

#else

<<<<<<< HEAD
struct compress {
};

static bool compress_init(struct compress *c)
=======
struct i915_vma_compress {
	struct pagevec pool;
};

static bool compress_init(struct i915_vma_compress *c)
{
	return pool_init(&c->pool, ALLOW_FAIL) == 0;
}

static bool compress_start(struct i915_vma_compress *c)
>>>>>>> upstream/android-13
{
	return true;
}

<<<<<<< HEAD
static int compress_page(struct compress *c,
			 void *src,
			 struct drm_i915_error_object *dst)
{
	unsigned long page;
	void *ptr;

	page = __get_free_page(GFP_ATOMIC | __GFP_NOWARN);
	if (!page)
		return -ENOMEM;

	ptr = (void *)page;
	if (!i915_memcpy_from_wc(ptr, src, PAGE_SIZE))
=======
static int compress_page(struct i915_vma_compress *c,
			 void *src,
			 struct i915_vma_coredump *dst,
			 bool wc)
{
	void *ptr;

	ptr = pool_alloc(&c->pool, ALLOW_FAIL);
	if (!ptr)
		return -ENOMEM;

	if (!(wc && i915_memcpy_from_wc(ptr, src, PAGE_SIZE)))
>>>>>>> upstream/android-13
		memcpy(ptr, src, PAGE_SIZE);
	dst->pages[dst->page_count++] = ptr;
	cond_resched();

	return 0;
}

<<<<<<< HEAD
static int compress_flush(struct compress *c,
			  struct drm_i915_error_object *dst)
=======
static int compress_flush(struct i915_vma_compress *c,
			  struct i915_vma_coredump *dst)
>>>>>>> upstream/android-13
{
	return 0;
}

<<<<<<< HEAD
static void compress_fini(struct compress *c,
			  struct drm_i915_error_object *dst)
{
}

=======
static void compress_finish(struct i915_vma_compress *c)
{
}

static void compress_fini(struct i915_vma_compress *c)
{
	pool_fini(&c->pool);
}

>>>>>>> upstream/android-13
static void err_compression_marker(struct drm_i915_error_state_buf *m)
{
	err_puts(m, "~");
}

#endif

<<<<<<< HEAD
static void print_error_buffers(struct drm_i915_error_state_buf *m,
				const char *name,
				struct drm_i915_error_buffer *err,
				int count)
{
	err_printf(m, "%s [%d]:\n", name, count);

	while (count--) {
		err_printf(m, "    %08x_%08x %8u %02x %02x %02x",
			   upper_32_bits(err->gtt_offset),
			   lower_32_bits(err->gtt_offset),
			   err->size,
			   err->read_domains,
			   err->write_domain,
			   err->wseqno);
		err_puts(m, tiling_flag(err->tiling));
		err_puts(m, dirty_flag(err->dirty));
		err_puts(m, purgeable_flag(err->purgeable));
		err_puts(m, err->userptr ? " userptr" : "");
		err_puts(m, err->engine != -1 ? " " : "");
		err_puts(m, engine_name(m->i915, err->engine));
		err_puts(m, i915_cache_level_str(m->i915, err->cache_level));

		if (err->name)
			err_printf(m, " (name: %d)", err->name);
		if (err->fence_reg != I915_FENCE_REG_NONE)
			err_printf(m, " (fence: %d)", err->fence_reg);

		err_puts(m, "\n");
		err++;
	}
}

static void error_print_instdone(struct drm_i915_error_state_buf *m,
				 const struct drm_i915_error_engine *ee)
{
=======
static void error_print_instdone(struct drm_i915_error_state_buf *m,
				 const struct intel_engine_coredump *ee)
{
	const struct sseu_dev_info *sseu = &ee->engine->gt->info.sseu;
>>>>>>> upstream/android-13
	int slice;
	int subslice;

	err_printf(m, "  INSTDONE: 0x%08x\n",
		   ee->instdone.instdone);

<<<<<<< HEAD
	if (ee->engine_id != RCS || INTEL_GEN(m->i915) <= 3)
=======
	if (ee->engine->class != RENDER_CLASS || GRAPHICS_VER(m->i915) <= 3)
>>>>>>> upstream/android-13
		return;

	err_printf(m, "  SC_INSTDONE: 0x%08x\n",
		   ee->instdone.slice_common);

<<<<<<< HEAD
	if (INTEL_GEN(m->i915) <= 6)
		return;

	for_each_instdone_slice_subslice(m->i915, slice, subslice)
=======
	if (GRAPHICS_VER(m->i915) <= 6)
		return;

	for_each_instdone_slice_subslice(m->i915, sseu, slice, subslice)
>>>>>>> upstream/android-13
		err_printf(m, "  SAMPLER_INSTDONE[%d][%d]: 0x%08x\n",
			   slice, subslice,
			   ee->instdone.sampler[slice][subslice]);

<<<<<<< HEAD
	for_each_instdone_slice_subslice(m->i915, slice, subslice)
		err_printf(m, "  ROW_INSTDONE[%d][%d]: 0x%08x\n",
			   slice, subslice,
			   ee->instdone.row[slice][subslice]);
}

static const char *bannable(const struct drm_i915_error_context *ctx)
{
	return ctx->bannable ? "" : " (unbannable)";
=======
	for_each_instdone_slice_subslice(m->i915, sseu, slice, subslice)
		err_printf(m, "  ROW_INSTDONE[%d][%d]: 0x%08x\n",
			   slice, subslice,
			   ee->instdone.row[slice][subslice]);

	if (GRAPHICS_VER(m->i915) < 12)
		return;

	err_printf(m, "  SC_INSTDONE_EXTRA: 0x%08x\n",
		   ee->instdone.slice_common_extra[0]);
	err_printf(m, "  SC_INSTDONE_EXTRA2: 0x%08x\n",
		   ee->instdone.slice_common_extra[1]);
>>>>>>> upstream/android-13
}

static void error_print_request(struct drm_i915_error_state_buf *m,
				const char *prefix,
<<<<<<< HEAD
				const struct drm_i915_error_request *erq,
				const unsigned long epoch)
=======
				const struct i915_request_coredump *erq)
>>>>>>> upstream/android-13
{
	if (!erq->seqno)
		return;

<<<<<<< HEAD
	err_printf(m, "%s pid %d, ban score %d, seqno %8x:%08x, prio %d, emitted %dms, start %08x, head %08x, tail %08x\n",
		   prefix, erq->pid, erq->ban_score,
		   erq->context, erq->seqno, erq->sched_attr.priority,
		   jiffies_to_msecs(erq->jiffies - epoch),
		   erq->start, erq->head, erq->tail);
=======
	err_printf(m, "%s pid %d, seqno %8x:%08x%s%s, prio %d, head %08x, tail %08x\n",
		   prefix, erq->pid, erq->context, erq->seqno,
		   test_bit(DMA_FENCE_FLAG_SIGNALED_BIT,
			    &erq->flags) ? "!" : "",
		   test_bit(DMA_FENCE_FLAG_ENABLE_SIGNAL_BIT,
			    &erq->flags) ? "+" : "",
		   erq->sched_attr.priority,
		   erq->head, erq->tail);
>>>>>>> upstream/android-13
}

static void error_print_context(struct drm_i915_error_state_buf *m,
				const char *header,
<<<<<<< HEAD
				const struct drm_i915_error_context *ctx)
{
	err_printf(m, "%s%s[%d] user_handle %d hw_id %d, prio %d, ban score %d%s guilty %d active %d\n",
		   header, ctx->comm, ctx->pid, ctx->handle, ctx->hw_id,
		   ctx->sched_attr.priority, ctx->ban_score, bannable(ctx),
		   ctx->guilty, ctx->active);
}

static void error_print_engine(struct drm_i915_error_state_buf *m,
			       const struct drm_i915_error_engine *ee,
			       const unsigned long epoch)
{
	int n;

	err_printf(m, "%s command stream:\n",
		   engine_name(m->i915, ee->engine_id));
	err_printf(m, "  IDLE?: %s\n", yesno(ee->idle));
=======
				const struct i915_gem_context_coredump *ctx)
{
	const u32 period = m->i915->gt.clock_period_ns;

	err_printf(m, "%s%s[%d] prio %d, guilty %d active %d, runtime total %lluns, avg %lluns\n",
		   header, ctx->comm, ctx->pid, ctx->sched_attr.priority,
		   ctx->guilty, ctx->active,
		   ctx->total_runtime * period,
		   mul_u32_u32(ctx->avg_runtime, period));
}

static struct i915_vma_coredump *
__i915_find_vma(struct i915_vma_coredump *vma, const char *name)
{
	while (vma) {
		if (strcmp(vma->name, name) == 0)
			return vma;
		vma = vma->next;
	}

	return NULL;
}

static struct i915_vma_coredump *
find_batch(const struct intel_engine_coredump *ee)
{
	return __i915_find_vma(ee->vma, "batch");
}

static void error_print_engine(struct drm_i915_error_state_buf *m,
			       const struct intel_engine_coredump *ee)
{
	struct i915_vma_coredump *batch;
	int n;

	err_printf(m, "%s command stream:\n", ee->engine->name);
	err_printf(m, "  CCID:  0x%08x\n", ee->ccid);
>>>>>>> upstream/android-13
	err_printf(m, "  START: 0x%08x\n", ee->start);
	err_printf(m, "  HEAD:  0x%08x [0x%08x]\n", ee->head, ee->rq_head);
	err_printf(m, "  TAIL:  0x%08x [0x%08x, 0x%08x]\n",
		   ee->tail, ee->rq_post, ee->rq_tail);
	err_printf(m, "  CTL:   0x%08x\n", ee->ctl);
	err_printf(m, "  MODE:  0x%08x\n", ee->mode);
	err_printf(m, "  HWS:   0x%08x\n", ee->hws);
	err_printf(m, "  ACTHD: 0x%08x %08x\n",
		   (u32)(ee->acthd>>32), (u32)ee->acthd);
	err_printf(m, "  IPEIR: 0x%08x\n", ee->ipeir);
	err_printf(m, "  IPEHR: 0x%08x\n", ee->ipehr);
<<<<<<< HEAD

	error_print_instdone(m, ee);

	if (ee->batchbuffer) {
		u64 start = ee->batchbuffer->gtt_offset;
		u64 end = start + ee->batchbuffer->gtt_size;
=======
	err_printf(m, "  ESR:   0x%08x\n", ee->esr);

	error_print_instdone(m, ee);

	batch = find_batch(ee);
	if (batch) {
		u64 start = batch->gtt_offset;
		u64 end = start + batch->gtt_size;
>>>>>>> upstream/android-13

		err_printf(m, "  batch: [0x%08x_%08x, 0x%08x_%08x]\n",
			   upper_32_bits(start), lower_32_bits(start),
			   upper_32_bits(end), lower_32_bits(end));
	}
<<<<<<< HEAD
	if (INTEL_GEN(m->i915) >= 4) {
=======
	if (GRAPHICS_VER(m->i915) >= 4) {
>>>>>>> upstream/android-13
		err_printf(m, "  BBADDR: 0x%08x_%08x\n",
			   (u32)(ee->bbaddr>>32), (u32)ee->bbaddr);
		err_printf(m, "  BB_STATE: 0x%08x\n", ee->bbstate);
		err_printf(m, "  INSTPS: 0x%08x\n", ee->instps);
	}
	err_printf(m, "  INSTPM: 0x%08x\n", ee->instpm);
	err_printf(m, "  FADDR: 0x%08x %08x\n", upper_32_bits(ee->faddr),
		   lower_32_bits(ee->faddr));
<<<<<<< HEAD
	if (INTEL_GEN(m->i915) >= 6) {
		err_printf(m, "  RC PSMI: 0x%08x\n", ee->rc_psmi);
		err_printf(m, "  FAULT_REG: 0x%08x\n", ee->fault_reg);
		err_printf(m, "  SYNC_0: 0x%08x\n",
			   ee->semaphore_mboxes[0]);
		err_printf(m, "  SYNC_1: 0x%08x\n",
			   ee->semaphore_mboxes[1]);
		if (HAS_VEBOX(m->i915))
			err_printf(m, "  SYNC_2: 0x%08x\n",
				   ee->semaphore_mboxes[2]);
	}
	if (USES_PPGTT(m->i915)) {
		err_printf(m, "  GFX_MODE: 0x%08x\n", ee->vm_info.gfx_mode);

		if (INTEL_GEN(m->i915) >= 8) {
=======
	if (GRAPHICS_VER(m->i915) >= 6) {
		err_printf(m, "  RC PSMI: 0x%08x\n", ee->rc_psmi);
		err_printf(m, "  FAULT_REG: 0x%08x\n", ee->fault_reg);
	}
	if (HAS_PPGTT(m->i915)) {
		err_printf(m, "  GFX_MODE: 0x%08x\n", ee->vm_info.gfx_mode);

		if (GRAPHICS_VER(m->i915) >= 8) {
>>>>>>> upstream/android-13
			int i;
			for (i = 0; i < 4; i++)
				err_printf(m, "  PDP%d: 0x%016llx\n",
					   i, ee->vm_info.pdp[i]);
		} else {
			err_printf(m, "  PP_DIR_BASE: 0x%08x\n",
				   ee->vm_info.pp_dir_base);
		}
	}
<<<<<<< HEAD
	err_printf(m, "  seqno: 0x%08x\n", ee->seqno);
	err_printf(m, "  last_seqno: 0x%08x\n", ee->last_seqno);
	err_printf(m, "  waiting: %s\n", yesno(ee->waiting));
	err_printf(m, "  ring->head: 0x%08x\n", ee->cpu_ring_head);
	err_printf(m, "  ring->tail: 0x%08x\n", ee->cpu_ring_tail);
	err_printf(m, "  hangcheck stall: %s\n", yesno(ee->hangcheck_stalled));
	err_printf(m, "  hangcheck action: %s\n",
		   hangcheck_action_to_str(ee->hangcheck_action));
	err_printf(m, "  hangcheck action timestamp: %dms (%lu%s)\n",
		   jiffies_to_msecs(ee->hangcheck_timestamp - epoch),
		   ee->hangcheck_timestamp,
		   ee->hangcheck_timestamp == epoch ? "; epoch" : "");
=======
	err_printf(m, "  hung: %u\n", ee->hung);
>>>>>>> upstream/android-13
	err_printf(m, "  engine reset count: %u\n", ee->reset_count);

	for (n = 0; n < ee->num_ports; n++) {
		err_printf(m, "  ELSP[%d]:", n);
<<<<<<< HEAD
		error_print_request(m, " ", &ee->execlist[n], epoch);
=======
		error_print_request(m, " ", &ee->execlist[n]);
>>>>>>> upstream/android-13
	}

	error_print_context(m, "  Active context: ", &ee->context);
}

void i915_error_printf(struct drm_i915_error_state_buf *e, const char *f, ...)
{
	va_list args;

	va_start(args, f);
	i915_error_vprintf(e, f, args);
	va_end(args);
}

<<<<<<< HEAD
static void print_error_obj(struct drm_i915_error_state_buf *m,
			    struct intel_engine_cs *engine,
			    const char *name,
			    struct drm_i915_error_object *obj)
=======
static void print_error_vma(struct drm_i915_error_state_buf *m,
			    const struct intel_engine_cs *engine,
			    const struct i915_vma_coredump *vma)
>>>>>>> upstream/android-13
{
	char out[ASCII85_BUFSZ];
	int page;

<<<<<<< HEAD
	if (!obj)
		return;

	if (name) {
		err_printf(m, "%s --- %s = 0x%08x %08x\n",
			   engine ? engine->name : "global", name,
			   upper_32_bits(obj->gtt_offset),
			   lower_32_bits(obj->gtt_offset));
	}

	err_compression_marker(m);
	for (page = 0; page < obj->page_count; page++) {
		int i, len;

		len = PAGE_SIZE;
		if (page == obj->page_count - 1)
			len -= obj->unused;
		len = ascii85_encode_len(len);

		for (i = 0; i < len; i++)
			err_puts(m, ascii85_encode(obj->pages[page][i], out));
=======
	if (!vma)
		return;

	err_printf(m, "%s --- %s = 0x%08x %08x\n",
		   engine ? engine->name : "global", vma->name,
		   upper_32_bits(vma->gtt_offset),
		   lower_32_bits(vma->gtt_offset));

	if (vma->gtt_page_sizes > I915_GTT_PAGE_SIZE_4K)
		err_printf(m, "gtt_page_sizes = 0x%08x\n", vma->gtt_page_sizes);

	err_compression_marker(m);
	for (page = 0; page < vma->page_count; page++) {
		int i, len;

		len = PAGE_SIZE;
		if (page == vma->page_count - 1)
			len -= vma->unused;
		len = ascii85_encode_len(len);

		for (i = 0; i < len; i++)
			err_puts(m, ascii85_encode(vma->pages[page][i], out));
>>>>>>> upstream/android-13
	}
	err_puts(m, "\n");
}

static void err_print_capabilities(struct drm_i915_error_state_buf *m,
<<<<<<< HEAD
				   const struct intel_device_info *info,
				   const struct intel_driver_caps *caps)
{
	struct drm_printer p = i915_error_printer(m);

	intel_device_info_dump_flags(info, &p);
	intel_driver_caps_print(caps, &p);
	intel_device_info_dump_topology(&info->sseu, &p);
=======
				   struct i915_gpu_coredump *error)
{
	struct drm_printer p = i915_error_printer(m);

	intel_device_info_print_static(&error->device_info, &p);
	intel_device_info_print_runtime(&error->runtime_info, &p);
	intel_driver_caps_print(&error->driver_caps, &p);
>>>>>>> upstream/android-13
}

static void err_print_params(struct drm_i915_error_state_buf *m,
			     const struct i915_params *params)
{
	struct drm_printer p = i915_error_printer(m);

	i915_params_dump(params, &p);
}

static void err_print_pciid(struct drm_i915_error_state_buf *m,
			    struct drm_i915_private *i915)
{
<<<<<<< HEAD
	struct pci_dev *pdev = i915->drm.pdev;
=======
	struct pci_dev *pdev = to_pci_dev(i915->drm.dev);
>>>>>>> upstream/android-13

	err_printf(m, "PCI ID: 0x%04x\n", pdev->device);
	err_printf(m, "PCI Revision: 0x%02x\n", pdev->revision);
	err_printf(m, "PCI Subsystem: %04x:%04x\n",
		   pdev->subsystem_vendor,
		   pdev->subsystem_device);
}

static void err_print_uc(struct drm_i915_error_state_buf *m,
<<<<<<< HEAD
			 const struct i915_error_uc *error_uc)
{
	struct drm_printer p = i915_error_printer(m);
	const struct i915_gpu_state *error =
		container_of(error_uc, typeof(*error), uc);

	if (!error->device_info.has_guc)
		return;

	intel_uc_fw_dump(&error_uc->guc_fw, &p);
	intel_uc_fw_dump(&error_uc->huc_fw, &p);
	print_error_obj(m, NULL, "GuC log buffer", error_uc->guc_log);
}

int i915_error_state_to_str(struct drm_i915_error_state_buf *m,
			    const struct i915_gpu_state *error)
{
	struct drm_i915_private *dev_priv = m->i915;
	struct drm_i915_error_object *obj;
	struct timespec64 ts;
	int i, j;

	if (!error) {
		err_printf(m, "No error state collected\n");
		return 0;
	}

	if (*error->error_msg)
		err_printf(m, "%s\n", error->error_msg);
	err_printf(m, "Kernel: " UTS_RELEASE "\n");
=======
			 const struct intel_uc_coredump *error_uc)
{
	struct drm_printer p = i915_error_printer(m);

	intel_uc_fw_dump(&error_uc->guc_fw, &p);
	intel_uc_fw_dump(&error_uc->huc_fw, &p);
	print_error_vma(m, NULL, error_uc->guc_log);
}

static void err_free_sgl(struct scatterlist *sgl)
{
	while (sgl) {
		struct scatterlist *sg;

		for (sg = sgl; !sg_is_chain(sg); sg++) {
			kfree(sg_virt(sg));
			if (sg_is_last(sg))
				break;
		}

		sg = sg_is_last(sg) ? NULL : sg_chain_ptr(sg);
		free_page((unsigned long)sgl);
		sgl = sg;
	}
}

static void err_print_gt_info(struct drm_i915_error_state_buf *m,
			      struct intel_gt_coredump *gt)
{
	struct drm_printer p = i915_error_printer(m);

	intel_gt_info_print(&gt->info, &p);
	intel_sseu_print_topology(&gt->info.sseu, &p);
}

static void err_print_gt(struct drm_i915_error_state_buf *m,
			 struct intel_gt_coredump *gt)
{
	const struct intel_engine_coredump *ee;
	int i;

	err_printf(m, "GT awake: %s\n", yesno(gt->awake));
	err_printf(m, "EIR: 0x%08x\n", gt->eir);
	err_printf(m, "IER: 0x%08x\n", gt->ier);
	for (i = 0; i < gt->ngtier; i++)
		err_printf(m, "GTIER[%d]: 0x%08x\n", i, gt->gtier[i]);
	err_printf(m, "PGTBL_ER: 0x%08x\n", gt->pgtbl_er);
	err_printf(m, "FORCEWAKE: 0x%08x\n", gt->forcewake);
	err_printf(m, "DERRMR: 0x%08x\n", gt->derrmr);

	for (i = 0; i < gt->nfence; i++)
		err_printf(m, "  fence[%d] = %08llx\n", i, gt->fence[i]);

	if (IS_GRAPHICS_VER(m->i915, 6, 11)) {
		err_printf(m, "ERROR: 0x%08x\n", gt->error);
		err_printf(m, "DONE_REG: 0x%08x\n", gt->done_reg);
	}

	if (GRAPHICS_VER(m->i915) >= 8)
		err_printf(m, "FAULT_TLB_DATA: 0x%08x 0x%08x\n",
			   gt->fault_data1, gt->fault_data0);

	if (GRAPHICS_VER(m->i915) == 7)
		err_printf(m, "ERR_INT: 0x%08x\n", gt->err_int);

	if (IS_GRAPHICS_VER(m->i915, 8, 11))
		err_printf(m, "GTT_CACHE_EN: 0x%08x\n", gt->gtt_cache);

	if (GRAPHICS_VER(m->i915) == 12)
		err_printf(m, "AUX_ERR_DBG: 0x%08x\n", gt->aux_err);

	if (GRAPHICS_VER(m->i915) >= 12) {
		int i;

		for (i = 0; i < GEN12_SFC_DONE_MAX; i++) {
			/*
			 * SFC_DONE resides in the VD forcewake domain, so it
			 * only exists if the corresponding VCS engine is
			 * present.
			 */
			if (!HAS_ENGINE(gt->_gt, _VCS(i * 2)))
				continue;

			err_printf(m, "  SFC_DONE[%d]: 0x%08x\n", i,
				   gt->sfc_done[i]);
		}

		err_printf(m, "  GAM_DONE: 0x%08x\n", gt->gam_done);
	}

	for (ee = gt->engine; ee; ee = ee->next) {
		const struct i915_vma_coredump *vma;

		error_print_engine(m, ee);
		for (vma = ee->vma; vma; vma = vma->next)
			print_error_vma(m, ee->engine, vma);
	}

	if (gt->uc)
		err_print_uc(m, gt->uc);

	err_print_gt_info(m, gt);
}

static void __err_print_to_sgl(struct drm_i915_error_state_buf *m,
			       struct i915_gpu_coredump *error)
{
	const struct intel_engine_coredump *ee;
	struct timespec64 ts;

	if (*error->error_msg)
		err_printf(m, "%s\n", error->error_msg);
	err_printf(m, "Kernel: %s %s\n",
		   init_utsname()->release,
		   init_utsname()->machine);
	err_printf(m, "Driver: %s\n", DRIVER_DATE);
>>>>>>> upstream/android-13
	ts = ktime_to_timespec64(error->time);
	err_printf(m, "Time: %lld s %ld us\n",
		   (s64)ts.tv_sec, ts.tv_nsec / NSEC_PER_USEC);
	ts = ktime_to_timespec64(error->boottime);
	err_printf(m, "Boottime: %lld s %ld us\n",
		   (s64)ts.tv_sec, ts.tv_nsec / NSEC_PER_USEC);
	ts = ktime_to_timespec64(error->uptime);
	err_printf(m, "Uptime: %lld s %ld us\n",
		   (s64)ts.tv_sec, ts.tv_nsec / NSEC_PER_USEC);
<<<<<<< HEAD
	err_printf(m, "Epoch: %lu jiffies (%u HZ)\n", error->epoch, HZ);
	err_printf(m, "Capture: %lu jiffies; %d ms ago, %d ms after epoch\n",
		   error->capture,
		   jiffies_to_msecs(jiffies - error->capture),
		   jiffies_to_msecs(error->capture - error->epoch));

	for (i = 0; i < ARRAY_SIZE(error->engine); i++) {
		if (error->engine[i].hangcheck_stalled &&
		    error->engine[i].context.pid) {
			err_printf(m, "Active process (on ring %s): %s [%d], score %d%s\n",
				   engine_name(m->i915, i),
				   error->engine[i].context.comm,
				   error->engine[i].context.pid,
				   error->engine[i].context.ban_score,
				   bannable(&error->engine[i].context));
		}
	}
	err_printf(m, "Reset count: %u\n", error->reset_count);
	err_printf(m, "Suspend count: %u\n", error->suspend_count);
	err_printf(m, "Platform: %s\n", intel_platform_name(error->device_info.platform));
	err_print_pciid(m, error->i915);

	err_printf(m, "IOMMU enabled?: %d\n", error->iommu);

	if (HAS_CSR(dev_priv)) {
		struct intel_csr *csr = &dev_priv->csr;

		err_printf(m, "DMC loaded: %s\n",
			   yesno(csr->dmc_payload != NULL));
		err_printf(m, "DMC fw version: %d.%d\n",
			   CSR_VERSION_MAJOR(csr->version),
			   CSR_VERSION_MINOR(csr->version));
	}

	err_printf(m, "GT awake: %s\n", yesno(error->awake));
	err_printf(m, "RPM wakelock: %s\n", yesno(error->wakelock));
	err_printf(m, "PM suspended: %s\n", yesno(error->suspended));
	err_printf(m, "EIR: 0x%08x\n", error->eir);
	err_printf(m, "IER: 0x%08x\n", error->ier);
	for (i = 0; i < error->ngtier; i++)
		err_printf(m, "GTIER[%d]: 0x%08x\n", i, error->gtier[i]);
	err_printf(m, "PGTBL_ER: 0x%08x\n", error->pgtbl_er);
	err_printf(m, "FORCEWAKE: 0x%08x\n", error->forcewake);
	err_printf(m, "DERRMR: 0x%08x\n", error->derrmr);
	err_printf(m, "CCID: 0x%08x\n", error->ccid);
	err_printf(m, "Missed interrupts: 0x%08lx\n", dev_priv->gpu_error.missed_irq_rings);

	for (i = 0; i < error->nfence; i++)
		err_printf(m, "  fence[%d] = %08llx\n", i, error->fence[i]);

	if (INTEL_GEN(dev_priv) >= 6) {
		err_printf(m, "ERROR: 0x%08x\n", error->error);

		if (INTEL_GEN(dev_priv) >= 8)
			err_printf(m, "FAULT_TLB_DATA: 0x%08x 0x%08x\n",
				   error->fault_data1, error->fault_data0);

		err_printf(m, "DONE_REG: 0x%08x\n", error->done_reg);
	}

	if (IS_GEN7(dev_priv))
		err_printf(m, "ERR_INT: 0x%08x\n", error->err_int);

	for (i = 0; i < ARRAY_SIZE(error->engine); i++) {
		if (error->engine[i].engine_id != -1)
			error_print_engine(m, &error->engine[i], error->epoch);
	}

	for (i = 0; i < ARRAY_SIZE(error->active_vm); i++) {
		char buf[128];
		int len, first = 1;

		if (!error->active_vm[i])
			break;

		len = scnprintf(buf, sizeof(buf), "Active (");
		for (j = 0; j < ARRAY_SIZE(error->engine); j++) {
			if (error->engine[j].vm != error->active_vm[i])
				continue;

			len += scnprintf(buf + len, sizeof(buf), "%s%s",
					 first ? "" : ", ",
					 dev_priv->engine[j]->name);
			first = 0;
		}
		scnprintf(buf + len, sizeof(buf), ")");
		print_error_buffers(m, buf,
				    error->active_bo[i],
				    error->active_bo_count[i]);
	}

	print_error_buffers(m, "Pinned (global)",
			    error->pinned_bo,
			    error->pinned_bo_count);

	for (i = 0; i < ARRAY_SIZE(error->engine); i++) {
		const struct drm_i915_error_engine *ee = &error->engine[i];

		obj = ee->batchbuffer;
		if (obj) {
			err_puts(m, dev_priv->engine[i]->name);
			if (ee->context.pid)
				err_printf(m, " (submitted by %s [%d], ctx %d [%d], score %d%s)",
					   ee->context.comm,
					   ee->context.pid,
					   ee->context.handle,
					   ee->context.hw_id,
					   ee->context.ban_score,
					   bannable(&ee->context));
			err_printf(m, " --- gtt_offset = 0x%08x %08x\n",
				   upper_32_bits(obj->gtt_offset),
				   lower_32_bits(obj->gtt_offset));
			print_error_obj(m, dev_priv->engine[i], NULL, obj);
		}

		for (j = 0; j < ee->user_bo_count; j++)
			print_error_obj(m, dev_priv->engine[i],
					"user", ee->user_bo[j]);

		if (ee->num_requests) {
			err_printf(m, "%s --- %d requests\n",
				   dev_priv->engine[i]->name,
				   ee->num_requests);
			for (j = 0; j < ee->num_requests; j++)
				error_print_request(m, " ",
						    &ee->requests[j],
						    error->epoch);
		}

		if (IS_ERR(ee->waiters)) {
			err_printf(m, "%s --- ? waiters [unable to acquire spinlock]\n",
				   dev_priv->engine[i]->name);
		} else if (ee->num_waiters) {
			err_printf(m, "%s --- %d waiters\n",
				   dev_priv->engine[i]->name,
				   ee->num_waiters);
			for (j = 0; j < ee->num_waiters; j++) {
				err_printf(m, " seqno 0x%08x for %s [%d]\n",
					   ee->waiters[j].seqno,
					   ee->waiters[j].comm,
					   ee->waiters[j].pid);
			}
		}

		print_error_obj(m, dev_priv->engine[i],
				"ringbuffer", ee->ringbuffer);

		print_error_obj(m, dev_priv->engine[i],
				"HW Status", ee->hws_page);

		print_error_obj(m, dev_priv->engine[i],
				"HW context", ee->ctx);

		print_error_obj(m, dev_priv->engine[i],
				"WA context", ee->wa_ctx);

		print_error_obj(m, dev_priv->engine[i],
				"WA batchbuffer", ee->wa_batchbuffer);

		print_error_obj(m, dev_priv->engine[i],
				"NULL context", ee->default_state);
	}
=======
	err_printf(m, "Capture: %lu jiffies; %d ms ago\n",
		   error->capture, jiffies_to_msecs(jiffies - error->capture));

	for (ee = error->gt ? error->gt->engine : NULL; ee; ee = ee->next)
		err_printf(m, "Active process (on ring %s): %s [%d]\n",
			   ee->engine->name,
			   ee->context.comm,
			   ee->context.pid);

	err_printf(m, "Reset count: %u\n", error->reset_count);
	err_printf(m, "Suspend count: %u\n", error->suspend_count);
	err_printf(m, "Platform: %s\n", intel_platform_name(error->device_info.platform));
	err_printf(m, "Subplatform: 0x%x\n",
		   intel_subplatform(&error->runtime_info,
				     error->device_info.platform));
	err_print_pciid(m, m->i915);

	err_printf(m, "IOMMU enabled?: %d\n", error->iommu);

	if (HAS_DMC(m->i915)) {
		struct intel_dmc *dmc = &m->i915->dmc;

		err_printf(m, "DMC loaded: %s\n",
			   yesno(intel_dmc_has_payload(m->i915) != 0));
		err_printf(m, "DMC fw version: %d.%d\n",
			   DMC_VERSION_MAJOR(dmc->version),
			   DMC_VERSION_MINOR(dmc->version));
	}

	err_printf(m, "RPM wakelock: %s\n", yesno(error->wakelock));
	err_printf(m, "PM suspended: %s\n", yesno(error->suspended));

	if (error->gt)
		err_print_gt(m, error->gt);
>>>>>>> upstream/android-13

	if (error->overlay)
		intel_overlay_print_error_state(m, error->overlay);

<<<<<<< HEAD
	if (error->display)
		intel_display_print_error_state(m, error->display);

	err_print_capabilities(m, &error->device_info, &error->driver_caps);
	err_print_params(m, &error->params);
	err_print_uc(m, &error->uc);

	if (m->bytes == 0 && m->err)
		return m->err;
=======
	err_print_capabilities(m, error);
	err_print_params(m, &error->params);
}

static int err_print_to_sgl(struct i915_gpu_coredump *error)
{
	struct drm_i915_error_state_buf m;

	if (IS_ERR(error))
		return PTR_ERR(error);

	if (READ_ONCE(error->sgl))
		return 0;

	memset(&m, 0, sizeof(m));
	m.i915 = error->i915;

	__err_print_to_sgl(&m, error);

	if (m.buf) {
		__sg_set_buf(m.cur++, m.buf, m.bytes, m.iter);
		m.bytes = 0;
		m.buf = NULL;
	}
	if (m.cur) {
		GEM_BUG_ON(m.end < m.cur);
		sg_mark_end(m.cur - 1);
	}
	GEM_BUG_ON(m.sgl && !m.cur);

	if (m.err) {
		err_free_sgl(m.sgl);
		return m.err;
	}

	if (cmpxchg(&error->sgl, NULL, m.sgl))
		err_free_sgl(m.sgl);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
int i915_error_state_buf_init(struct drm_i915_error_state_buf *ebuf,
			      struct drm_i915_private *i915,
			      size_t count, loff_t pos)
{
	memset(ebuf, 0, sizeof(*ebuf));
	ebuf->i915 = i915;

	/* We need to have enough room to store any i915_error_state printf
	 * so that we can move it to start position.
	 */
	ebuf->size = count + 1 > PAGE_SIZE ? count + 1 : PAGE_SIZE;
	ebuf->buf = kmalloc(ebuf->size,
				GFP_KERNEL | __GFP_NORETRY | __GFP_NOWARN);

	if (ebuf->buf == NULL) {
		ebuf->size = PAGE_SIZE;
		ebuf->buf = kmalloc(ebuf->size, GFP_KERNEL);
	}

	if (ebuf->buf == NULL) {
		ebuf->size = 128;
		ebuf->buf = kmalloc(ebuf->size, GFP_KERNEL);
	}

	if (ebuf->buf == NULL)
		return -ENOMEM;

	ebuf->start = pos;

	return 0;
}

static void i915_error_object_free(struct drm_i915_error_object *obj)
{
	int page;

	if (obj == NULL)
		return;

	for (page = 0; page < obj->page_count; page++)
		free_page((unsigned long)obj->pages[page]);

	kfree(obj);
}

static __always_inline void free_param(const char *type, void *x)
{
	if (!__builtin_strcmp(type, "char *"))
		kfree(*(void **)x);
}

static void cleanup_params(struct i915_gpu_state *error)
{
#define FREE(T, x, ...) free_param(#T, &error->params.x);
	I915_PARAMS_FOR_EACH(FREE);
#undef FREE
}

static void cleanup_uc_state(struct i915_gpu_state *error)
{
	struct i915_error_uc *error_uc = &error->uc;

	kfree(error_uc->guc_fw.path);
	kfree(error_uc->huc_fw.path);
	i915_error_object_free(error_uc->guc_log);
}

void __i915_gpu_state_free(struct kref *error_ref)
{
	struct i915_gpu_state *error =
		container_of(error_ref, typeof(*error), ref);
	long i, j;

	for (i = 0; i < ARRAY_SIZE(error->engine); i++) {
		struct drm_i915_error_engine *ee = &error->engine[i];

		for (j = 0; j < ee->user_bo_count; j++)
			i915_error_object_free(ee->user_bo[j]);
		kfree(ee->user_bo);

		i915_error_object_free(ee->batchbuffer);
		i915_error_object_free(ee->wa_batchbuffer);
		i915_error_object_free(ee->ringbuffer);
		i915_error_object_free(ee->hws_page);
		i915_error_object_free(ee->ctx);
		i915_error_object_free(ee->wa_ctx);

		kfree(ee->requests);
		if (!IS_ERR_OR_NULL(ee->waiters))
			kfree(ee->waiters);
	}

	for (i = 0; i < ARRAY_SIZE(error->active_bo); i++)
		kfree(error->active_bo[i]);
	kfree(error->pinned_bo);

	kfree(error->overlay);
	kfree(error->display);

	cleanup_params(error);
	cleanup_uc_state(error);

	kfree(error);
}

static struct drm_i915_error_object *
i915_error_object_create(struct drm_i915_private *i915,
			 struct i915_vma *vma)
{
	struct i915_ggtt *ggtt = &i915->ggtt;
	const u64 slot = ggtt->error_capture.start;
	struct drm_i915_error_object *dst;
	struct compress compress;
	unsigned long num_pages;
	struct sgt_iter iter;
	dma_addr_t dma;
	int ret;

	if (!vma)
=======
ssize_t i915_gpu_coredump_copy_to_buffer(struct i915_gpu_coredump *error,
					 char *buf, loff_t off, size_t rem)
{
	struct scatterlist *sg;
	size_t count;
	loff_t pos;
	int err;

	if (!error || !rem)
		return 0;

	err = err_print_to_sgl(error);
	if (err)
		return err;

	sg = READ_ONCE(error->fit);
	if (!sg || off < sg->dma_address)
		sg = error->sgl;
	if (!sg)
		return 0;

	pos = sg->dma_address;
	count = 0;
	do {
		size_t len, start;

		if (sg_is_chain(sg)) {
			sg = sg_chain_ptr(sg);
			GEM_BUG_ON(sg_is_chain(sg));
		}

		len = sg->length;
		if (pos + len <= off) {
			pos += len;
			continue;
		}

		start = sg->offset;
		if (pos < off) {
			GEM_BUG_ON(off - pos > len);
			len -= off - pos;
			start += off - pos;
			pos = off;
		}

		len = min(len, rem);
		GEM_BUG_ON(!len || len > sg->length);

		memcpy(buf, page_address(sg_page(sg)) + start, len);

		count += len;
		pos += len;

		buf += len;
		rem -= len;
		if (!rem) {
			WRITE_ONCE(error->fit, sg);
			break;
		}
	} while (!sg_is_last(sg++));

	return count;
}

static void i915_vma_coredump_free(struct i915_vma_coredump *vma)
{
	while (vma) {
		struct i915_vma_coredump *next = vma->next;
		int page;

		for (page = 0; page < vma->page_count; page++)
			free_page((unsigned long)vma->pages[page]);

		kfree(vma);
		vma = next;
	}
}

static void cleanup_params(struct i915_gpu_coredump *error)
{
	i915_params_free(&error->params);
}

static void cleanup_uc(struct intel_uc_coredump *uc)
{
	kfree(uc->guc_fw.path);
	kfree(uc->huc_fw.path);
	i915_vma_coredump_free(uc->guc_log);

	kfree(uc);
}

static void cleanup_gt(struct intel_gt_coredump *gt)
{
	while (gt->engine) {
		struct intel_engine_coredump *ee = gt->engine;

		gt->engine = ee->next;

		i915_vma_coredump_free(ee->vma);
		kfree(ee);
	}

	if (gt->uc)
		cleanup_uc(gt->uc);

	kfree(gt);
}

void __i915_gpu_coredump_free(struct kref *error_ref)
{
	struct i915_gpu_coredump *error =
		container_of(error_ref, typeof(*error), ref);

	while (error->gt) {
		struct intel_gt_coredump *gt = error->gt;

		error->gt = gt->next;
		cleanup_gt(gt);
	}

	kfree(error->overlay);

	cleanup_params(error);

	err_free_sgl(error->sgl);
	kfree(error);
}

static struct i915_vma_coredump *
i915_vma_coredump_create(const struct intel_gt *gt,
			 const struct i915_vma *vma,
			 const char *name,
			 struct i915_vma_compress *compress)
{
	struct i915_ggtt *ggtt = gt->ggtt;
	const u64 slot = ggtt->error_capture.start;
	struct i915_vma_coredump *dst;
	unsigned long num_pages;
	struct sgt_iter iter;
	int ret;

	might_sleep();

	if (!vma || !vma->pages || !compress)
>>>>>>> upstream/android-13
		return NULL;

	num_pages = min_t(u64, vma->size, vma->obj->base.size) >> PAGE_SHIFT;
	num_pages = DIV_ROUND_UP(10 * num_pages, 8); /* worstcase zlib growth */
<<<<<<< HEAD
	dst = kmalloc(sizeof(*dst) + num_pages * sizeof(u32 *),
		      GFP_ATOMIC | __GFP_NOWARN);
	if (!dst)
		return NULL;

	dst->gtt_offset = vma->node.start;
	dst->gtt_size = vma->node.size;
=======
	dst = kmalloc(sizeof(*dst) + num_pages * sizeof(u32 *), ALLOW_FAIL);
	if (!dst)
		return NULL;

	if (!compress_start(compress)) {
		kfree(dst);
		return NULL;
	}

	strcpy(dst->name, name);
	dst->next = NULL;

	dst->gtt_offset = vma->node.start;
	dst->gtt_size = vma->node.size;
	dst->gtt_page_sizes = vma->page_sizes.gtt;
>>>>>>> upstream/android-13
	dst->num_pages = num_pages;
	dst->page_count = 0;
	dst->unused = 0;

<<<<<<< HEAD
	if (!compress_init(&compress)) {
		kfree(dst);
		return NULL;
	}

	ret = -EINVAL;
	for_each_sgt_dma(dma, iter, vma->pages) {
		void __iomem *s;

		ggtt->vm.insert_page(&ggtt->vm, dma, slot, I915_CACHE_NONE, 0);

		s = io_mapping_map_atomic_wc(&ggtt->iomap, slot);
		ret = compress_page(&compress, (void  __force *)s, dst);
		io_mapping_unmap_atomic(s);
		if (ret)
			break;
	}

	if (ret || compress_flush(&compress, dst)) {
		while (dst->page_count--)
			free_page((unsigned long)dst->pages[dst->page_count]);
		kfree(dst);
		dst = NULL;
	}

	compress_fini(&compress, dst);
	ggtt->vm.clear_range(&ggtt->vm, slot, PAGE_SIZE);
	return dst;
}

/* The error capture is special as tries to run underneath the normal
 * locking rules - so we use the raw version of the i915_gem_active lookup.
 */
static inline uint32_t
__active_get_seqno(struct i915_gem_active *active)
{
	struct i915_request *request;

	request = __i915_gem_active_peek(active);
	return request ? request->global_seqno : 0;
}

static inline int
__active_get_engine_id(struct i915_gem_active *active)
{
	struct i915_request *request;

	request = __i915_gem_active_peek(active);
	return request ? request->engine->id : -1;
}

static void capture_bo(struct drm_i915_error_buffer *err,
		       struct i915_vma *vma)
{
	struct drm_i915_gem_object *obj = vma->obj;

	err->size = obj->base.size;
	err->name = obj->base.name;

	err->wseqno = __active_get_seqno(&obj->frontbuffer_write);
	err->engine = __active_get_engine_id(&obj->frontbuffer_write);

	err->gtt_offset = vma->node.start;
	err->read_domains = obj->read_domains;
	err->write_domain = obj->write_domain;
	err->fence_reg = vma->fence ? vma->fence->id : -1;
	err->tiling = i915_gem_object_get_tiling(obj);
	err->dirty = obj->mm.dirty;
	err->purgeable = obj->mm.madv != I915_MADV_WILLNEED;
	err->userptr = obj->userptr.mm != NULL;
	err->cache_level = obj->cache_level;
}

static u32 capture_error_bo(struct drm_i915_error_buffer *err,
			    int count, struct list_head *head,
			    bool pinned_only)
{
	struct i915_vma *vma;
	int i = 0;

	list_for_each_entry(vma, head, vm_link) {
		if (!vma->obj)
			continue;

		if (pinned_only && !i915_vma_is_pinned(vma))
			continue;

		capture_bo(err++, vma);
		if (++i == count)
			break;
	}

	return i;
}

/* Generate a semi-unique error code. The code is not meant to have meaning, The
 * code's only purpose is to try to prevent false duplicated bug reports by
 * grossly estimating a GPU error state.
 *
 * TODO Ideally, hashing the batchbuffer would be a very nice way to determine
 * the hang if we could strip the GTT offset information from it.
 *
 * It's only a small step better than a random number in its current form.
 */
static uint32_t i915_error_generate_code(struct drm_i915_private *dev_priv,
					 struct i915_gpu_state *error,
					 int *engine_id)
{
	uint32_t error_code = 0;
	int i;

	/* IPEHR would be an ideal way to detect errors, as it's the gross
	 * measure of "the command that hung." However, has some very common
	 * synchronization commands which almost always appear in the case
	 * strictly a client bug. Use instdone to differentiate those some.
	 */
	for (i = 0; i < I915_NUM_ENGINES; i++) {
		if (error->engine[i].hangcheck_stalled) {
			if (engine_id)
				*engine_id = i;

			return error->engine[i].ipehr ^
			       error->engine[i].instdone.instdone;
		}
	}

	return error_code;
}

static void gem_record_fences(struct i915_gpu_state *error)
{
	struct drm_i915_private *dev_priv = error->i915;
	int i;

	if (INTEL_GEN(dev_priv) >= 6) {
		for (i = 0; i < dev_priv->num_fence_regs; i++)
			error->fence[i] = I915_READ64(FENCE_REG_GEN6_LO(i));
	} else if (INTEL_GEN(dev_priv) >= 4) {
		for (i = 0; i < dev_priv->num_fence_regs; i++)
			error->fence[i] = I915_READ64(FENCE_REG_965_LO(i));
	} else {
		for (i = 0; i < dev_priv->num_fence_regs; i++)
			error->fence[i] = I915_READ(FENCE_REG(i));
	}
	error->nfence = i;
}

static void gen6_record_semaphore_state(struct intel_engine_cs *engine,
					struct drm_i915_error_engine *ee)
{
	struct drm_i915_private *dev_priv = engine->i915;

	ee->semaphore_mboxes[0] = I915_READ(RING_SYNC_0(engine->mmio_base));
	ee->semaphore_mboxes[1] = I915_READ(RING_SYNC_1(engine->mmio_base));
	if (HAS_VEBOX(dev_priv))
		ee->semaphore_mboxes[2] =
			I915_READ(RING_SYNC_2(engine->mmio_base));
}

static void error_record_engine_waiters(struct intel_engine_cs *engine,
					struct drm_i915_error_engine *ee)
{
	struct intel_breadcrumbs *b = &engine->breadcrumbs;
	struct drm_i915_error_waiter *waiter;
	struct rb_node *rb;
	int count;

	ee->num_waiters = 0;
	ee->waiters = NULL;

	if (RB_EMPTY_ROOT(&b->waiters))
		return;

	if (!spin_trylock_irq(&b->rb_lock)) {
		ee->waiters = ERR_PTR(-EDEADLK);
		return;
	}

	count = 0;
	for (rb = rb_first(&b->waiters); rb != NULL; rb = rb_next(rb))
		count++;
	spin_unlock_irq(&b->rb_lock);

	waiter = NULL;
	if (count)
		waiter = kmalloc_array(count,
				       sizeof(struct drm_i915_error_waiter),
				       GFP_ATOMIC);
	if (!waiter)
		return;

	if (!spin_trylock_irq(&b->rb_lock)) {
		kfree(waiter);
		ee->waiters = ERR_PTR(-EDEADLK);
		return;
	}

	ee->waiters = waiter;
	for (rb = rb_first(&b->waiters); rb; rb = rb_next(rb)) {
		struct intel_wait *w = rb_entry(rb, typeof(*w), node);

		strcpy(waiter->comm, w->tsk->comm);
		waiter->pid = w->tsk->pid;
		waiter->seqno = w->seqno;
		waiter++;

		if (++ee->num_waiters == count)
			break;
	}
	spin_unlock_irq(&b->rb_lock);
}

static void error_record_engine_registers(struct i915_gpu_state *error,
					  struct intel_engine_cs *engine,
					  struct drm_i915_error_engine *ee)
{
	struct drm_i915_private *dev_priv = engine->i915;

	if (INTEL_GEN(dev_priv) >= 6) {
		ee->rc_psmi = I915_READ(RING_PSMI_CTL(engine->mmio_base));
		if (INTEL_GEN(dev_priv) >= 8) {
			ee->fault_reg = I915_READ(GEN8_RING_FAULT_REG);
		} else {
			gen6_record_semaphore_state(engine, ee);
			ee->fault_reg = I915_READ(RING_FAULT_REG(engine));
		}
	}

	if (INTEL_GEN(dev_priv) >= 4) {
		ee->faddr = I915_READ(RING_DMA_FADD(engine->mmio_base));
		ee->ipeir = I915_READ(RING_IPEIR(engine->mmio_base));
		ee->ipehr = I915_READ(RING_IPEHR(engine->mmio_base));
		ee->instps = I915_READ(RING_INSTPS(engine->mmio_base));
		ee->bbaddr = I915_READ(RING_BBADDR(engine->mmio_base));
		if (INTEL_GEN(dev_priv) >= 8) {
			ee->faddr |= (u64) I915_READ(RING_DMA_FADD_UDW(engine->mmio_base)) << 32;
			ee->bbaddr |= (u64) I915_READ(RING_BBADDR_UDW(engine->mmio_base)) << 32;
		}
		ee->bbstate = I915_READ(RING_BBSTATE(engine->mmio_base));
	} else {
		ee->faddr = I915_READ(DMA_FADD_I8XX);
		ee->ipeir = I915_READ(IPEIR);
		ee->ipehr = I915_READ(IPEHR);
=======
	ret = -EINVAL;
	if (drm_mm_node_allocated(&ggtt->error_capture)) {
		void __iomem *s;
		dma_addr_t dma;

		for_each_sgt_daddr(dma, iter, vma->pages) {
			mutex_lock(&ggtt->error_mutex);
			ggtt->vm.insert_page(&ggtt->vm, dma, slot,
					     I915_CACHE_NONE, 0);
			mb();

			s = io_mapping_map_wc(&ggtt->iomap, slot, PAGE_SIZE);
			ret = compress_page(compress,
					    (void  __force *)s, dst,
					    true);
			io_mapping_unmap(s);

			mb();
			ggtt->vm.clear_range(&ggtt->vm, slot, PAGE_SIZE);
			mutex_unlock(&ggtt->error_mutex);
			if (ret)
				break;
		}
	} else if (__i915_gem_object_is_lmem(vma->obj)) {
		struct intel_memory_region *mem = vma->obj->mm.region;
		dma_addr_t dma;

		for_each_sgt_daddr(dma, iter, vma->pages) {
			void __iomem *s;

			s = io_mapping_map_wc(&mem->iomap,
					      dma - mem->region.start,
					      PAGE_SIZE);
			ret = compress_page(compress,
					    (void __force *)s, dst,
					    true);
			io_mapping_unmap(s);
			if (ret)
				break;
		}
	} else {
		struct page *page;

		for_each_sgt_page(page, iter, vma->pages) {
			void *s;

			drm_clflush_pages(&page, 1);

			s = kmap(page);
			ret = compress_page(compress, s, dst, false);
			kunmap(page);

			drm_clflush_pages(&page, 1);

			if (ret)
				break;
		}
	}

	if (ret || compress_flush(compress, dst)) {
		while (dst->page_count--)
			pool_free(&compress->pool, dst->pages[dst->page_count]);
		kfree(dst);
		dst = NULL;
	}
	compress_finish(compress);

	return dst;
}

static void gt_record_fences(struct intel_gt_coredump *gt)
{
	struct i915_ggtt *ggtt = gt->_gt->ggtt;
	struct intel_uncore *uncore = gt->_gt->uncore;
	int i;

	if (GRAPHICS_VER(uncore->i915) >= 6) {
		for (i = 0; i < ggtt->num_fences; i++)
			gt->fence[i] =
				intel_uncore_read64(uncore,
						    FENCE_REG_GEN6_LO(i));
	} else if (GRAPHICS_VER(uncore->i915) >= 4) {
		for (i = 0; i < ggtt->num_fences; i++)
			gt->fence[i] =
				intel_uncore_read64(uncore,
						    FENCE_REG_965_LO(i));
	} else {
		for (i = 0; i < ggtt->num_fences; i++)
			gt->fence[i] =
				intel_uncore_read(uncore, FENCE_REG(i));
	}
	gt->nfence = i;
}

static void engine_record_registers(struct intel_engine_coredump *ee)
{
	const struct intel_engine_cs *engine = ee->engine;
	struct drm_i915_private *i915 = engine->i915;

	if (GRAPHICS_VER(i915) >= 6) {
		ee->rc_psmi = ENGINE_READ(engine, RING_PSMI_CTL);

		if (GRAPHICS_VER(i915) >= 12)
			ee->fault_reg = intel_uncore_read(engine->uncore,
							  GEN12_RING_FAULT_REG);
		else if (GRAPHICS_VER(i915) >= 8)
			ee->fault_reg = intel_uncore_read(engine->uncore,
							  GEN8_RING_FAULT_REG);
		else
			ee->fault_reg = GEN6_RING_FAULT_REG_READ(engine);
	}

	if (GRAPHICS_VER(i915) >= 4) {
		ee->esr = ENGINE_READ(engine, RING_ESR);
		ee->faddr = ENGINE_READ(engine, RING_DMA_FADD);
		ee->ipeir = ENGINE_READ(engine, RING_IPEIR);
		ee->ipehr = ENGINE_READ(engine, RING_IPEHR);
		ee->instps = ENGINE_READ(engine, RING_INSTPS);
		ee->bbaddr = ENGINE_READ(engine, RING_BBADDR);
		ee->ccid = ENGINE_READ(engine, CCID);
		if (GRAPHICS_VER(i915) >= 8) {
			ee->faddr |= (u64)ENGINE_READ(engine, RING_DMA_FADD_UDW) << 32;
			ee->bbaddr |= (u64)ENGINE_READ(engine, RING_BBADDR_UDW) << 32;
		}
		ee->bbstate = ENGINE_READ(engine, RING_BBSTATE);
	} else {
		ee->faddr = ENGINE_READ(engine, DMA_FADD_I8XX);
		ee->ipeir = ENGINE_READ(engine, IPEIR);
		ee->ipehr = ENGINE_READ(engine, IPEHR);
>>>>>>> upstream/android-13
	}

	intel_engine_get_instdone(engine, &ee->instdone);

<<<<<<< HEAD
	ee->waiting = intel_engine_has_waiter(engine);
	ee->instpm = I915_READ(RING_INSTPM(engine->mmio_base));
	ee->acthd = intel_engine_get_active_head(engine);
	ee->seqno = intel_engine_get_seqno(engine);
	ee->last_seqno = intel_engine_last_submit(engine);
	ee->start = I915_READ_START(engine);
	ee->head = I915_READ_HEAD(engine);
	ee->tail = I915_READ_TAIL(engine);
	ee->ctl = I915_READ_CTL(engine);
	if (INTEL_GEN(dev_priv) > 2)
		ee->mode = I915_READ_MODE(engine);

	if (!HWS_NEEDS_PHYSICAL(dev_priv)) {
		i915_reg_t mmio;

		if (IS_GEN7(dev_priv)) {
			switch (engine->id) {
			default:
			case RCS:
				mmio = RENDER_HWS_PGA_GEN7;
				break;
			case BCS:
				mmio = BLT_HWS_PGA_GEN7;
				break;
			case VCS:
				mmio = BSD_HWS_PGA_GEN7;
				break;
			case VECS:
				mmio = VEBOX_HWS_PGA_GEN7;
				break;
			}
		} else if (IS_GEN6(engine->i915)) {
=======
	ee->instpm = ENGINE_READ(engine, RING_INSTPM);
	ee->acthd = intel_engine_get_active_head(engine);
	ee->start = ENGINE_READ(engine, RING_START);
	ee->head = ENGINE_READ(engine, RING_HEAD);
	ee->tail = ENGINE_READ(engine, RING_TAIL);
	ee->ctl = ENGINE_READ(engine, RING_CTL);
	if (GRAPHICS_VER(i915) > 2)
		ee->mode = ENGINE_READ(engine, RING_MI_MODE);

	if (!HWS_NEEDS_PHYSICAL(i915)) {
		i915_reg_t mmio;

		if (GRAPHICS_VER(i915) == 7) {
			switch (engine->id) {
			default:
				MISSING_CASE(engine->id);
				fallthrough;
			case RCS0:
				mmio = RENDER_HWS_PGA_GEN7;
				break;
			case BCS0:
				mmio = BLT_HWS_PGA_GEN7;
				break;
			case VCS0:
				mmio = BSD_HWS_PGA_GEN7;
				break;
			case VECS0:
				mmio = VEBOX_HWS_PGA_GEN7;
				break;
			}
		} else if (GRAPHICS_VER(engine->i915) == 6) {
>>>>>>> upstream/android-13
			mmio = RING_HWS_PGA_GEN6(engine->mmio_base);
		} else {
			/* XXX: gen8 returns to sanity */
			mmio = RING_HWS_PGA(engine->mmio_base);
		}

<<<<<<< HEAD
		ee->hws = I915_READ(mmio);
	}

	ee->idle = intel_engine_is_idle(engine);
	ee->hangcheck_timestamp = engine->hangcheck.action_timestamp;
	ee->hangcheck_action = engine->hangcheck.action;
	ee->hangcheck_stalled = engine->hangcheck.stalled;
	ee->reset_count = i915_reset_engine_count(&dev_priv->gpu_error,
						  engine);

	if (USES_PPGTT(dev_priv)) {
		int i;

		ee->vm_info.gfx_mode = I915_READ(RING_MODE_GEN7(engine));

		if (IS_GEN6(dev_priv))
			ee->vm_info.pp_dir_base =
				I915_READ(RING_PP_DIR_BASE_READ(engine));
		else if (IS_GEN7(dev_priv))
			ee->vm_info.pp_dir_base =
				I915_READ(RING_PP_DIR_BASE(engine));
		else if (INTEL_GEN(dev_priv) >= 8)
			for (i = 0; i < 4; i++) {
				ee->vm_info.pdp[i] =
					I915_READ(GEN8_RING_PDP_UDW(engine, i));
				ee->vm_info.pdp[i] <<= 32;
				ee->vm_info.pdp[i] |=
					I915_READ(GEN8_RING_PDP_LDW(engine, i));
			}
	}
}

static void record_request(struct i915_request *request,
			   struct drm_i915_error_request *erq)
{
	struct i915_gem_context *ctx = request->gem_context;

	erq->context = ctx->hw_id;
	erq->sched_attr = request->sched.attr;
	erq->ban_score = atomic_read(&ctx->ban_score);
	erq->seqno = request->global_seqno;
	erq->jiffies = request->emitted_jiffies;
	erq->start = i915_ggtt_offset(request->ring->vma);
	erq->head = request->head;
	erq->tail = request->tail;

	rcu_read_lock();
	erq->pid = ctx->pid ? pid_nr(ctx->pid) : 0;
	rcu_read_unlock();
}

static void engine_record_requests(struct intel_engine_cs *engine,
				   struct i915_request *first,
				   struct drm_i915_error_engine *ee)
{
	struct i915_request *request;
	int count;

	count = 0;
	request = first;
	list_for_each_entry_from(request, &engine->timeline.requests, link)
		count++;
	if (!count)
		return;

	ee->requests = kcalloc(count, sizeof(*ee->requests), GFP_ATOMIC);
	if (!ee->requests)
		return;

	ee->num_requests = count;

	count = 0;
	request = first;
	list_for_each_entry_from(request, &engine->timeline.requests, link) {
		if (count >= ee->num_requests) {
			/*
			 * If the ring request list was changed in
			 * between the point where the error request
			 * list was created and dimensioned and this
			 * point then just exit early to avoid crashes.
			 *
			 * We don't need to communicate that the
			 * request list changed state during error
			 * state capture and that the error state is
			 * slightly incorrect as a consequence since we
			 * are typically only interested in the request
			 * list state at the point of error state
			 * capture, not in any changes happening during
			 * the capture.
			 */
			break;
		}

		record_request(request, &ee->requests[count++]);
	}
	ee->num_requests = count;
}

static void error_record_engine_execlists(struct intel_engine_cs *engine,
					  struct drm_i915_error_engine *ee)
{
	const struct intel_engine_execlists * const execlists = &engine->execlists;
	unsigned int n;

	for (n = 0; n < execlists_num_ports(execlists); n++) {
		struct i915_request *rq = port_request(&execlists->port[n]);

		if (!rq)
			break;

		record_request(rq, &ee->execlist[n]);
	}
=======
		ee->hws = intel_uncore_read(engine->uncore, mmio);
	}

	ee->reset_count = i915_reset_engine_count(&i915->gpu_error, engine);

	if (HAS_PPGTT(i915)) {
		int i;

		ee->vm_info.gfx_mode = ENGINE_READ(engine, RING_MODE_GEN7);

		if (GRAPHICS_VER(i915) == 6) {
			ee->vm_info.pp_dir_base =
				ENGINE_READ(engine, RING_PP_DIR_BASE_READ);
		} else if (GRAPHICS_VER(i915) == 7) {
			ee->vm_info.pp_dir_base =
				ENGINE_READ(engine, RING_PP_DIR_BASE);
		} else if (GRAPHICS_VER(i915) >= 8) {
			u32 base = engine->mmio_base;

			for (i = 0; i < 4; i++) {
				ee->vm_info.pdp[i] =
					intel_uncore_read(engine->uncore,
							  GEN8_RING_PDP_UDW(base, i));
				ee->vm_info.pdp[i] <<= 32;
				ee->vm_info.pdp[i] |=
					intel_uncore_read(engine->uncore,
							  GEN8_RING_PDP_LDW(base, i));
			}
		}
	}
}

static void record_request(const struct i915_request *request,
			   struct i915_request_coredump *erq)
{
	erq->flags = request->fence.flags;
	erq->context = request->fence.context;
	erq->seqno = request->fence.seqno;
	erq->sched_attr = request->sched.attr;
	erq->head = request->head;
	erq->tail = request->tail;

	erq->pid = 0;
	rcu_read_lock();
	if (!intel_context_is_closed(request->context)) {
		const struct i915_gem_context *ctx;

		ctx = rcu_dereference(request->context->gem_context);
		if (ctx)
			erq->pid = pid_nr(ctx->pid);
	}
	rcu_read_unlock();
}

static void engine_record_execlists(struct intel_engine_coredump *ee)
{
	const struct intel_engine_execlists * const el = &ee->engine->execlists;
	struct i915_request * const *port = el->active;
	unsigned int n = 0;

	while (*port)
		record_request(*port++, &ee->execlist[n++]);
>>>>>>> upstream/android-13

	ee->num_ports = n;
}

<<<<<<< HEAD
static void record_context(struct drm_i915_error_context *e,
			   struct i915_gem_context *ctx)
{
	if (ctx->pid) {
		struct task_struct *task;

		rcu_read_lock();
		task = pid_task(ctx->pid, PIDTYPE_PID);
		if (task) {
			strcpy(e->comm, task->comm);
			e->pid = task->pid;
		}
		rcu_read_unlock();
	}

	e->handle = ctx->user_handle;
	e->hw_id = ctx->hw_id;
	e->sched_attr = ctx->sched;
	e->ban_score = atomic_read(&ctx->ban_score);
	e->bannable = i915_gem_context_is_bannable(ctx);
	e->guilty = atomic_read(&ctx->guilty_count);
	e->active = atomic_read(&ctx->active_count);
}

static void request_record_user_bo(struct i915_request *request,
				   struct drm_i915_error_engine *ee)
{
	struct i915_capture_list *c;
	struct drm_i915_error_object **bo;
	long count;

	count = 0;
	for (c = request->capture_list; c; c = c->next)
		count++;

	bo = NULL;
	if (count)
		bo = kcalloc(count, sizeof(*bo), GFP_ATOMIC);
	if (!bo)
		return;

	count = 0;
	for (c = request->capture_list; c; c = c->next) {
		bo[count] = i915_error_object_create(request->i915, c->vma);
		if (!bo[count])
			break;
		count++;
	}

	ee->user_bo = bo;
	ee->user_bo_count = count;
}

static struct drm_i915_error_object *
capture_object(struct drm_i915_private *dev_priv,
	       struct drm_i915_gem_object *obj)
{
	if (obj && i915_gem_object_has_pages(obj)) {
		struct i915_vma fake = {
			.node = { .start = U64_MAX, .size = obj->base.size },
			.size = obj->base.size,
			.pages = obj->mm.pages,
			.obj = obj,
		};

		return i915_error_object_create(dev_priv, &fake);
	} else {
		return NULL;
	}
}

static void gem_record_rings(struct i915_gpu_state *error)
{
	struct drm_i915_private *i915 = error->i915;
	struct i915_ggtt *ggtt = &i915->ggtt;
	int i;

	for (i = 0; i < I915_NUM_ENGINES; i++) {
		struct intel_engine_cs *engine = i915->engine[i];
		struct drm_i915_error_engine *ee = &error->engine[i];
		struct i915_request *request;

		ee->engine_id = -1;

		if (!engine)
			continue;

		ee->engine_id = i;

		error_record_engine_registers(error, engine, ee);
		error_record_engine_waiters(engine, ee);
		error_record_engine_execlists(engine, ee);

		request = i915_gem_find_active_request(engine);
		if (request) {
			struct i915_gem_context *ctx = request->gem_context;
			struct intel_ring *ring;

			ee->vm = ctx->ppgtt ? &ctx->ppgtt->vm : &ggtt->vm;

			record_context(&ee->context, ctx);

			/* We need to copy these to an anonymous buffer
			 * as the simplest method to avoid being overwritten
			 * by userspace.
			 */
			ee->batchbuffer =
				i915_error_object_create(i915, request->batch);

			if (HAS_BROKEN_CS_TLB(i915))
				ee->wa_batchbuffer =
					i915_error_object_create(i915,
								 engine->scratch);
			request_record_user_bo(request, ee);

			ee->ctx =
				i915_error_object_create(i915,
							 request->hw_context->state);

			error->simulated |=
				i915_gem_context_no_error_capture(ctx);

			ee->rq_head = request->head;
			ee->rq_post = request->postfix;
			ee->rq_tail = request->tail;

			ring = request->ring;
			ee->cpu_ring_head = ring->head;
			ee->cpu_ring_tail = ring->tail;
			ee->ringbuffer =
				i915_error_object_create(i915, ring->vma);

			engine_record_requests(engine, request, ee);
		}

		ee->hws_page =
			i915_error_object_create(i915,
						 engine->status_page.vma);

		ee->wa_ctx = i915_error_object_create(i915, engine->wa_ctx.vma);

		ee->default_state = capture_object(i915, engine->default_state);
	}
}

static void gem_capture_vm(struct i915_gpu_state *error,
			   struct i915_address_space *vm,
			   int idx)
{
	struct drm_i915_error_buffer *active_bo;
	struct i915_vma *vma;
	int count;

	count = 0;
	list_for_each_entry(vma, &vm->active_list, vm_link)
		count++;

	active_bo = NULL;
	if (count)
		active_bo = kcalloc(count, sizeof(*active_bo), GFP_ATOMIC);
	if (active_bo)
		count = capture_error_bo(active_bo, count, &vm->active_list, false);
	else
		count = 0;

	error->active_vm[idx] = vm;
	error->active_bo[idx] = active_bo;
	error->active_bo_count[idx] = count;
}

static void capture_active_buffers(struct i915_gpu_state *error)
{
	int cnt = 0, i, j;

	BUILD_BUG_ON(ARRAY_SIZE(error->engine) > ARRAY_SIZE(error->active_bo));
	BUILD_BUG_ON(ARRAY_SIZE(error->active_bo) != ARRAY_SIZE(error->active_vm));
	BUILD_BUG_ON(ARRAY_SIZE(error->active_bo) != ARRAY_SIZE(error->active_bo_count));

	/* Scan each engine looking for unique active contexts/vm */
	for (i = 0; i < ARRAY_SIZE(error->engine); i++) {
		struct drm_i915_error_engine *ee = &error->engine[i];
		bool found;

		if (!ee->vm)
			continue;

		found = false;
		for (j = 0; j < i && !found; j++)
			found = error->engine[j].vm == ee->vm;
		if (!found)
			gem_capture_vm(error, ee->vm, cnt++);
	}
}

static void capture_pinned_buffers(struct i915_gpu_state *error)
{
	struct i915_address_space *vm = &error->i915->ggtt.vm;
	struct drm_i915_error_buffer *bo;
	struct i915_vma *vma;
	int count_inactive, count_active;

	count_inactive = 0;
	list_for_each_entry(vma, &vm->inactive_list, vm_link)
		count_inactive++;

	count_active = 0;
	list_for_each_entry(vma, &vm->active_list, vm_link)
		count_active++;

	bo = NULL;
	if (count_inactive + count_active)
		bo = kcalloc(count_inactive + count_active,
			     sizeof(*bo), GFP_ATOMIC);
	if (!bo)
		return;

	count_inactive = capture_error_bo(bo, count_inactive,
					  &vm->active_list, true);
	count_active = capture_error_bo(bo + count_inactive, count_active,
					&vm->inactive_list, true);
	error->pinned_bo_count = count_inactive + count_active;
	error->pinned_bo = bo;
}

static void capture_uc_state(struct i915_gpu_state *error)
{
	struct drm_i915_private *i915 = error->i915;
	struct i915_error_uc *error_uc = &error->uc;

	/* Capturing uC state won't be useful if there is no GuC */
	if (!error->device_info.has_guc)
		return;

	error_uc->guc_fw = i915->guc.fw;
	error_uc->huc_fw = i915->huc.fw;
=======
static bool record_context(struct i915_gem_context_coredump *e,
			   const struct i915_request *rq)
{
	struct i915_gem_context *ctx;
	struct task_struct *task;
	bool simulated;

	rcu_read_lock();
	ctx = rcu_dereference(rq->context->gem_context);
	if (ctx && !kref_get_unless_zero(&ctx->ref))
		ctx = NULL;
	rcu_read_unlock();
	if (!ctx)
		return true;

	rcu_read_lock();
	task = pid_task(ctx->pid, PIDTYPE_PID);
	if (task) {
		strcpy(e->comm, task->comm);
		e->pid = task->pid;
	}
	rcu_read_unlock();

	e->sched_attr = ctx->sched;
	e->guilty = atomic_read(&ctx->guilty_count);
	e->active = atomic_read(&ctx->active_count);

	e->total_runtime = rq->context->runtime.total;
	e->avg_runtime = ewma_runtime_read(&rq->context->runtime.avg);

	simulated = i915_gem_context_no_error_capture(ctx);

	i915_gem_context_put(ctx);
	return simulated;
}

struct intel_engine_capture_vma {
	struct intel_engine_capture_vma *next;
	struct i915_vma *vma;
	char name[16];
};

static struct intel_engine_capture_vma *
capture_vma(struct intel_engine_capture_vma *next,
	    struct i915_vma *vma,
	    const char *name,
	    gfp_t gfp)
{
	struct intel_engine_capture_vma *c;

	if (!vma)
		return next;

	c = kmalloc(sizeof(*c), gfp);
	if (!c)
		return next;

	if (!i915_active_acquire_if_busy(&vma->active)) {
		kfree(c);
		return next;
	}

	strcpy(c->name, name);
	c->vma = vma; /* reference held while active */

	c->next = next;
	return c;
}

static struct intel_engine_capture_vma *
capture_user(struct intel_engine_capture_vma *capture,
	     const struct i915_request *rq,
	     gfp_t gfp)
{
	struct i915_capture_list *c;

	for (c = rq->capture_list; c; c = c->next)
		capture = capture_vma(capture, c->vma, "user", gfp);

	return capture;
}

static void add_vma(struct intel_engine_coredump *ee,
		    struct i915_vma_coredump *vma)
{
	if (vma) {
		vma->next = ee->vma;
		ee->vma = vma;
	}
}

struct intel_engine_coredump *
intel_engine_coredump_alloc(struct intel_engine_cs *engine, gfp_t gfp)
{
	struct intel_engine_coredump *ee;

	ee = kzalloc(sizeof(*ee), gfp);
	if (!ee)
		return NULL;

	ee->engine = engine;

	engine_record_registers(ee);
	engine_record_execlists(ee);

	return ee;
}

struct intel_engine_capture_vma *
intel_engine_coredump_add_request(struct intel_engine_coredump *ee,
				  struct i915_request *rq,
				  gfp_t gfp)
{
	struct intel_engine_capture_vma *vma = NULL;

	ee->simulated |= record_context(&ee->context, rq);
	if (ee->simulated)
		return NULL;

	/*
	 * We need to copy these to an anonymous buffer
	 * as the simplest method to avoid being overwritten
	 * by userspace.
	 */
	vma = capture_vma(vma, rq->batch, "batch", gfp);
	vma = capture_user(vma, rq, gfp);
	vma = capture_vma(vma, rq->ring->vma, "ring", gfp);
	vma = capture_vma(vma, rq->context->state, "HW context", gfp);

	ee->rq_head = rq->head;
	ee->rq_post = rq->postfix;
	ee->rq_tail = rq->tail;

	return vma;
}

void
intel_engine_coredump_add_vma(struct intel_engine_coredump *ee,
			      struct intel_engine_capture_vma *capture,
			      struct i915_vma_compress *compress)
{
	const struct intel_engine_cs *engine = ee->engine;

	while (capture) {
		struct intel_engine_capture_vma *this = capture;
		struct i915_vma *vma = this->vma;

		add_vma(ee,
			i915_vma_coredump_create(engine->gt,
						 vma, this->name,
						 compress));

		i915_active_release(&vma->active);

		capture = this->next;
		kfree(this);
	}

	add_vma(ee,
		i915_vma_coredump_create(engine->gt,
					 engine->status_page.vma,
					 "HW Status",
					 compress));

	add_vma(ee,
		i915_vma_coredump_create(engine->gt,
					 engine->wa_ctx.vma,
					 "WA context",
					 compress));
}

static struct intel_engine_coredump *
capture_engine(struct intel_engine_cs *engine,
	       struct i915_vma_compress *compress)
{
	struct intel_engine_capture_vma *capture = NULL;
	struct intel_engine_coredump *ee;
	struct intel_context *ce;
	struct i915_request *rq = NULL;
	unsigned long flags;

	ee = intel_engine_coredump_alloc(engine, GFP_KERNEL);
	if (!ee)
		return NULL;

	ce = intel_engine_get_hung_context(engine);
	if (ce) {
		intel_engine_clear_hung_context(engine);
		rq = intel_context_find_active_request(ce);
		if (!rq || !i915_request_started(rq))
			goto no_request_capture;
	} else {
		/*
		 * Getting here with GuC enabled means it is a forced error capture
		 * with no actual hang. So, no need to attempt the execlist search.
		 */
		if (!intel_uc_uses_guc_submission(&engine->gt->uc)) {
			spin_lock_irqsave(&engine->sched_engine->lock, flags);
			rq = intel_engine_execlist_find_hung_request(engine);
			spin_unlock_irqrestore(&engine->sched_engine->lock,
					       flags);
		}
	}
	if (rq)
		capture = intel_engine_coredump_add_request(ee, rq,
							    ATOMIC_MAYFAIL);
	if (!capture) {
no_request_capture:
		kfree(ee);
		return NULL;
	}

	intel_engine_coredump_add_vma(ee, capture, compress);

	return ee;
}

static void
gt_record_engines(struct intel_gt_coredump *gt,
		  intel_engine_mask_t engine_mask,
		  struct i915_vma_compress *compress)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	for_each_engine(engine, gt->_gt, id) {
		struct intel_engine_coredump *ee;

		/* Refill our page pool before entering atomic section */
		pool_refill(&compress->pool, ALLOW_FAIL);

		ee = capture_engine(engine, compress);
		if (!ee)
			continue;

		ee->hung = engine->mask & engine_mask;

		gt->simulated |= ee->simulated;
		if (ee->simulated) {
			kfree(ee);
			continue;
		}

		ee->next = gt->engine;
		gt->engine = ee;
	}
}

static struct intel_uc_coredump *
gt_record_uc(struct intel_gt_coredump *gt,
	     struct i915_vma_compress *compress)
{
	const struct intel_uc *uc = &gt->_gt->uc;
	struct intel_uc_coredump *error_uc;

	error_uc = kzalloc(sizeof(*error_uc), ALLOW_FAIL);
	if (!error_uc)
		return NULL;

	memcpy(&error_uc->guc_fw, &uc->guc.fw, sizeof(uc->guc.fw));
	memcpy(&error_uc->huc_fw, &uc->huc.fw, sizeof(uc->huc.fw));
>>>>>>> upstream/android-13

	/* Non-default firmware paths will be specified by the modparam.
	 * As modparams are generally accesible from the userspace make
	 * explicit copies of the firmware paths.
	 */
<<<<<<< HEAD
	error_uc->guc_fw.path = kstrdup(i915->guc.fw.path, GFP_ATOMIC);
	error_uc->huc_fw.path = kstrdup(i915->huc.fw.path, GFP_ATOMIC);
	error_uc->guc_log = i915_error_object_create(i915, i915->guc.log.vma);
}

/* Capture all registers which don't fit into another category. */
static void capture_reg_state(struct i915_gpu_state *error)
{
	struct drm_i915_private *dev_priv = error->i915;
	int i;

	/* General organization
=======
	error_uc->guc_fw.path = kstrdup(uc->guc.fw.path, ALLOW_FAIL);
	error_uc->huc_fw.path = kstrdup(uc->huc.fw.path, ALLOW_FAIL);
	error_uc->guc_log =
		i915_vma_coredump_create(gt->_gt,
					 uc->guc.log.vma, "GuC log buffer",
					 compress);

	return error_uc;
}

/* Capture all registers which don't fit into another category. */
static void gt_record_regs(struct intel_gt_coredump *gt)
{
	struct intel_uncore *uncore = gt->_gt->uncore;
	struct drm_i915_private *i915 = uncore->i915;
	int i;

	/*
	 * General organization
>>>>>>> upstream/android-13
	 * 1. Registers specific to a single generation
	 * 2. Registers which belong to multiple generations
	 * 3. Feature specific registers.
	 * 4. Everything else
	 * Please try to follow the order.
	 */

	/* 1: Registers specific to a single generation */
<<<<<<< HEAD
	if (IS_VALLEYVIEW(dev_priv)) {
		error->gtier[0] = I915_READ(GTIER);
		error->ier = I915_READ(VLV_IER);
		error->forcewake = I915_READ_FW(FORCEWAKE_VLV);
	}

	if (IS_GEN7(dev_priv))
		error->err_int = I915_READ(GEN7_ERR_INT);

	if (INTEL_GEN(dev_priv) >= 8) {
		error->fault_data0 = I915_READ(GEN8_FAULT_TLB_DATA0);
		error->fault_data1 = I915_READ(GEN8_FAULT_TLB_DATA1);
	}

	if (IS_GEN6(dev_priv)) {
		error->forcewake = I915_READ_FW(FORCEWAKE);
		error->gab_ctl = I915_READ(GAB_CTL);
		error->gfx_mode = I915_READ(GFX_MODE);
	}

	/* 2: Registers which belong to multiple generations */
	if (INTEL_GEN(dev_priv) >= 7)
		error->forcewake = I915_READ_FW(FORCEWAKE_MT);

	if (INTEL_GEN(dev_priv) >= 6) {
		error->derrmr = I915_READ(DERRMR);
		error->error = I915_READ(ERROR_GEN6);
		error->done_reg = I915_READ(DONE_REG);
	}

	if (INTEL_GEN(dev_priv) >= 5)
		error->ccid = I915_READ(CCID);

	/* 3: Feature specific registers */
	if (IS_GEN6(dev_priv) || IS_GEN7(dev_priv)) {
		error->gam_ecochk = I915_READ(GAM_ECOCHK);
		error->gac_eco = I915_READ(GAC_ECO_BITS);
	}

	/* 4: Everything else */
	if (INTEL_GEN(dev_priv) >= 11) {
		error->ier = I915_READ(GEN8_DE_MISC_IER);
		error->gtier[0] = I915_READ(GEN11_RENDER_COPY_INTR_ENABLE);
		error->gtier[1] = I915_READ(GEN11_VCS_VECS_INTR_ENABLE);
		error->gtier[2] = I915_READ(GEN11_GUC_SG_INTR_ENABLE);
		error->gtier[3] = I915_READ(GEN11_GPM_WGBOXPERF_INTR_ENABLE);
		error->gtier[4] = I915_READ(GEN11_CRYPTO_RSVD_INTR_ENABLE);
		error->gtier[5] = I915_READ(GEN11_GUNIT_CSME_INTR_ENABLE);
		error->ngtier = 6;
	} else if (INTEL_GEN(dev_priv) >= 8) {
		error->ier = I915_READ(GEN8_DE_MISC_IER);
		for (i = 0; i < 4; i++)
			error->gtier[i] = I915_READ(GEN8_GT_IER(i));
		error->ngtier = 4;
	} else if (HAS_PCH_SPLIT(dev_priv)) {
		error->ier = I915_READ(DEIER);
		error->gtier[0] = I915_READ(GTIER);
		error->ngtier = 1;
	} else if (IS_GEN2(dev_priv)) {
		error->ier = I915_READ16(IER);
	} else if (!IS_VALLEYVIEW(dev_priv)) {
		error->ier = I915_READ(IER);
	}
	error->eir = I915_READ(EIR);
	error->pgtbl_er = I915_READ(PGTBL_ER);
}

static void i915_error_capture_msg(struct drm_i915_private *dev_priv,
				   struct i915_gpu_state *error,
				   u32 engine_mask,
				   const char *error_msg)
{
	u32 ecode;
	int engine_id = -1, len;

	ecode = i915_error_generate_code(dev_priv, error, &engine_id);

	len = scnprintf(error->error_msg, sizeof(error->error_msg),
			"GPU HANG: ecode %d:%d:0x%08x",
			INTEL_GEN(dev_priv), engine_id, ecode);

	if (engine_id != -1 && error->engine[engine_id].context.pid)
		len += scnprintf(error->error_msg + len,
				 sizeof(error->error_msg) - len,
				 ", in %s [%d]",
				 error->engine[engine_id].context.comm,
				 error->engine[engine_id].context.pid);

	scnprintf(error->error_msg + len, sizeof(error->error_msg) - len,
		  ", reason: %s, action: %s",
		  error_msg,
		  engine_mask ? "reset" : "continue");
}

static void capture_gen_state(struct i915_gpu_state *error)
{
	struct drm_i915_private *i915 = error->i915;

	error->awake = i915->gt.awake;
=======
	if (IS_VALLEYVIEW(i915)) {
		gt->gtier[0] = intel_uncore_read(uncore, GTIER);
		gt->ier = intel_uncore_read(uncore, VLV_IER);
		gt->forcewake = intel_uncore_read_fw(uncore, FORCEWAKE_VLV);
	}

	if (GRAPHICS_VER(i915) == 7)
		gt->err_int = intel_uncore_read(uncore, GEN7_ERR_INT);

	if (GRAPHICS_VER(i915) >= 12) {
		gt->fault_data0 = intel_uncore_read(uncore,
						    GEN12_FAULT_TLB_DATA0);
		gt->fault_data1 = intel_uncore_read(uncore,
						    GEN12_FAULT_TLB_DATA1);
	} else if (GRAPHICS_VER(i915) >= 8) {
		gt->fault_data0 = intel_uncore_read(uncore,
						    GEN8_FAULT_TLB_DATA0);
		gt->fault_data1 = intel_uncore_read(uncore,
						    GEN8_FAULT_TLB_DATA1);
	}

	if (GRAPHICS_VER(i915) == 6) {
		gt->forcewake = intel_uncore_read_fw(uncore, FORCEWAKE);
		gt->gab_ctl = intel_uncore_read(uncore, GAB_CTL);
		gt->gfx_mode = intel_uncore_read(uncore, GFX_MODE);
	}

	/* 2: Registers which belong to multiple generations */
	if (GRAPHICS_VER(i915) >= 7)
		gt->forcewake = intel_uncore_read_fw(uncore, FORCEWAKE_MT);

	if (GRAPHICS_VER(i915) >= 6) {
		gt->derrmr = intel_uncore_read(uncore, DERRMR);
		if (GRAPHICS_VER(i915) < 12) {
			gt->error = intel_uncore_read(uncore, ERROR_GEN6);
			gt->done_reg = intel_uncore_read(uncore, DONE_REG);
		}
	}

	/* 3: Feature specific registers */
	if (IS_GRAPHICS_VER(i915, 6, 7)) {
		gt->gam_ecochk = intel_uncore_read(uncore, GAM_ECOCHK);
		gt->gac_eco = intel_uncore_read(uncore, GAC_ECO_BITS);
	}

	if (IS_GRAPHICS_VER(i915, 8, 11))
		gt->gtt_cache = intel_uncore_read(uncore, HSW_GTT_CACHE_EN);

	if (GRAPHICS_VER(i915) == 12)
		gt->aux_err = intel_uncore_read(uncore, GEN12_AUX_ERR_DBG);

	if (GRAPHICS_VER(i915) >= 12) {
		for (i = 0; i < GEN12_SFC_DONE_MAX; i++) {
			/*
			 * SFC_DONE resides in the VD forcewake domain, so it
			 * only exists if the corresponding VCS engine is
			 * present.
			 */
			if (!HAS_ENGINE(gt->_gt, _VCS(i * 2)))
				continue;

			gt->sfc_done[i] =
				intel_uncore_read(uncore, GEN12_SFC_DONE(i));
		}

		gt->gam_done = intel_uncore_read(uncore, GEN12_GAM_DONE);
	}

	/* 4: Everything else */
	if (GRAPHICS_VER(i915) >= 11) {
		gt->ier = intel_uncore_read(uncore, GEN8_DE_MISC_IER);
		gt->gtier[0] =
			intel_uncore_read(uncore,
					  GEN11_RENDER_COPY_INTR_ENABLE);
		gt->gtier[1] =
			intel_uncore_read(uncore, GEN11_VCS_VECS_INTR_ENABLE);
		gt->gtier[2] =
			intel_uncore_read(uncore, GEN11_GUC_SG_INTR_ENABLE);
		gt->gtier[3] =
			intel_uncore_read(uncore,
					  GEN11_GPM_WGBOXPERF_INTR_ENABLE);
		gt->gtier[4] =
			intel_uncore_read(uncore,
					  GEN11_CRYPTO_RSVD_INTR_ENABLE);
		gt->gtier[5] =
			intel_uncore_read(uncore,
					  GEN11_GUNIT_CSME_INTR_ENABLE);
		gt->ngtier = 6;
	} else if (GRAPHICS_VER(i915) >= 8) {
		gt->ier = intel_uncore_read(uncore, GEN8_DE_MISC_IER);
		for (i = 0; i < 4; i++)
			gt->gtier[i] =
				intel_uncore_read(uncore, GEN8_GT_IER(i));
		gt->ngtier = 4;
	} else if (HAS_PCH_SPLIT(i915)) {
		gt->ier = intel_uncore_read(uncore, DEIER);
		gt->gtier[0] = intel_uncore_read(uncore, GTIER);
		gt->ngtier = 1;
	} else if (GRAPHICS_VER(i915) == 2) {
		gt->ier = intel_uncore_read16(uncore, GEN2_IER);
	} else if (!IS_VALLEYVIEW(i915)) {
		gt->ier = intel_uncore_read(uncore, GEN2_IER);
	}
	gt->eir = intel_uncore_read(uncore, EIR);
	gt->pgtbl_er = intel_uncore_read(uncore, PGTBL_ER);
}

static void gt_record_info(struct intel_gt_coredump *gt)
{
	memcpy(&gt->info, &gt->_gt->info, sizeof(struct intel_gt_info));
}

/*
 * Generate a semi-unique error code. The code is not meant to have meaning, The
 * code's only purpose is to try to prevent false duplicated bug reports by
 * grossly estimating a GPU error state.
 *
 * TODO Ideally, hashing the batchbuffer would be a very nice way to determine
 * the hang if we could strip the GTT offset information from it.
 *
 * It's only a small step better than a random number in its current form.
 */
static u32 generate_ecode(const struct intel_engine_coredump *ee)
{
	/*
	 * IPEHR would be an ideal way to detect errors, as it's the gross
	 * measure of "the command that hung." However, has some very common
	 * synchronization commands which almost always appear in the case
	 * strictly a client bug. Use instdone to differentiate those some.
	 */
	return ee ? ee->ipehr ^ ee->instdone.instdone : 0;
}

static const char *error_msg(struct i915_gpu_coredump *error)
{
	struct intel_engine_coredump *first = NULL;
	unsigned int hung_classes = 0;
	struct intel_gt_coredump *gt;
	int len;

	for (gt = error->gt; gt; gt = gt->next) {
		struct intel_engine_coredump *cs;

		for (cs = gt->engine; cs; cs = cs->next) {
			if (cs->hung) {
				hung_classes |= BIT(cs->engine->uabi_class);
				if (!first)
					first = cs;
			}
		}
	}

	len = scnprintf(error->error_msg, sizeof(error->error_msg),
			"GPU HANG: ecode %d:%x:%08x",
			GRAPHICS_VER(error->i915), hung_classes,
			generate_ecode(first));
	if (first && first->context.pid) {
		/* Just show the first executing process, more is confusing */
		len += scnprintf(error->error_msg + len,
				 sizeof(error->error_msg) - len,
				 ", in %s [%d]",
				 first->context.comm, first->context.pid);
	}

	return error->error_msg;
}

static void capture_gen(struct i915_gpu_coredump *error)
{
	struct drm_i915_private *i915 = error->i915;

>>>>>>> upstream/android-13
	error->wakelock = atomic_read(&i915->runtime_pm.wakeref_count);
	error->suspended = i915->runtime_pm.suspended;

	error->iommu = -1;
#ifdef CONFIG_INTEL_IOMMU
	error->iommu = intel_iommu_gfx_mapped;
#endif
	error->reset_count = i915_reset_count(&i915->gpu_error);
	error->suspend_count = i915->suspend_count;

<<<<<<< HEAD
	memcpy(&error->device_info,
	       INTEL_INFO(i915),
	       sizeof(error->device_info));
	error->driver_caps = i915->caps;
}

static __always_inline void dup_param(const char *type, void *x)
{
	if (!__builtin_strcmp(type, "char *"))
		*(void **)x = kstrdup(*(void **)x, GFP_ATOMIC);
}

static void capture_params(struct i915_gpu_state *error)
{
	error->params = i915_modparams;
#define DUP(T, x, ...) dup_param(#T, &error->params.x);
	I915_PARAMS_FOR_EACH(DUP);
#undef DUP
}

static unsigned long capture_find_epoch(const struct i915_gpu_state *error)
{
	unsigned long epoch = error->capture;
	int i;

	for (i = 0; i < ARRAY_SIZE(error->engine); i++) {
		const struct drm_i915_error_engine *ee = &error->engine[i];

		if (ee->hangcheck_stalled &&
		    time_before(ee->hangcheck_timestamp, epoch))
			epoch = ee->hangcheck_timestamp;
	}

	return epoch;
}

static int capture(void *data)
{
	struct i915_gpu_state *error = data;

	error->time = ktime_get_real();
	error->boottime = ktime_get_boottime();
	error->uptime = ktime_sub(ktime_get(),
				  error->i915->gt.last_init_time);
	error->capture = jiffies;

	capture_params(error);
	capture_gen_state(error);
	capture_uc_state(error);
	capture_reg_state(error);
	gem_record_fences(error);
	gem_record_rings(error);
	capture_active_buffers(error);
	capture_pinned_buffers(error);

	error->overlay = intel_overlay_capture_error_state(error->i915);
	error->display = intel_display_capture_error_state(error->i915);

	error->epoch = capture_find_epoch(error);

	return 0;
}

#define DAY_AS_SECONDS(x) (24 * 60 * 60 * (x))

struct i915_gpu_state *
i915_capture_gpu_state(struct drm_i915_private *i915)
{
	struct i915_gpu_state *error;

	error = kzalloc(sizeof(*error), GFP_ATOMIC);
=======
	i915_params_copy(&error->params, &i915->params);
	memcpy(&error->device_info,
	       INTEL_INFO(i915),
	       sizeof(error->device_info));
	memcpy(&error->runtime_info,
	       RUNTIME_INFO(i915),
	       sizeof(error->runtime_info));
	error->driver_caps = i915->caps;
}

struct i915_gpu_coredump *
i915_gpu_coredump_alloc(struct drm_i915_private *i915, gfp_t gfp)
{
	struct i915_gpu_coredump *error;

	if (!i915->params.error_capture)
		return NULL;

	error = kzalloc(sizeof(*error), gfp);
>>>>>>> upstream/android-13
	if (!error)
		return NULL;

	kref_init(&error->ref);
	error->i915 = i915;

<<<<<<< HEAD
	stop_machine(capture, error, NULL);
=======
	error->time = ktime_get_real();
	error->boottime = ktime_get_boottime();
	error->uptime = ktime_sub(ktime_get(), i915->gt.last_init_time);
	error->capture = jiffies;

	capture_gen(error);
>>>>>>> upstream/android-13

	return error;
}

<<<<<<< HEAD
/**
 * i915_capture_error_state - capture an error record for later analysis
 * @i915: i915 device
 * @engine_mask: the mask of engines triggering the hang
 * @error_msg: a message to insert into the error capture header
=======
#define DAY_AS_SECONDS(x) (24 * 60 * 60 * (x))

struct intel_gt_coredump *
intel_gt_coredump_alloc(struct intel_gt *gt, gfp_t gfp)
{
	struct intel_gt_coredump *gc;

	gc = kzalloc(sizeof(*gc), gfp);
	if (!gc)
		return NULL;

	gc->_gt = gt;
	gc->awake = intel_gt_pm_is_awake(gt);

	gt_record_regs(gc);
	gt_record_fences(gc);

	return gc;
}

struct i915_vma_compress *
i915_vma_capture_prepare(struct intel_gt_coredump *gt)
{
	struct i915_vma_compress *compress;

	compress = kmalloc(sizeof(*compress), ALLOW_FAIL);
	if (!compress)
		return NULL;

	if (!compress_init(compress)) {
		kfree(compress);
		return NULL;
	}

	return compress;
}

void i915_vma_capture_finish(struct intel_gt_coredump *gt,
			     struct i915_vma_compress *compress)
{
	if (!compress)
		return;

	compress_fini(compress);
	kfree(compress);
}

struct i915_gpu_coredump *
i915_gpu_coredump(struct intel_gt *gt, intel_engine_mask_t engine_mask)
{
	struct drm_i915_private *i915 = gt->i915;
	struct i915_gpu_coredump *error;

	/* Check if GPU capture has been disabled */
	error = READ_ONCE(i915->gpu_error.first_error);
	if (IS_ERR(error))
		return error;

	error = i915_gpu_coredump_alloc(i915, ALLOW_FAIL);
	if (!error)
		return ERR_PTR(-ENOMEM);

	error->gt = intel_gt_coredump_alloc(gt, ALLOW_FAIL);
	if (error->gt) {
		struct i915_vma_compress *compress;

		compress = i915_vma_capture_prepare(error->gt);
		if (!compress) {
			kfree(error->gt);
			kfree(error);
			return ERR_PTR(-ENOMEM);
		}

		gt_record_info(error->gt);
		gt_record_engines(error->gt, engine_mask, compress);

		if (INTEL_INFO(i915)->has_gt_uc)
			error->gt->uc = gt_record_uc(error->gt, compress);

		i915_vma_capture_finish(error->gt, compress);

		error->simulated |= error->gt->simulated;
	}

	error->overlay = intel_overlay_capture_error_state(i915);

	return error;
}

void i915_error_state_store(struct i915_gpu_coredump *error)
{
	struct drm_i915_private *i915;
	static bool warned;

	if (IS_ERR_OR_NULL(error))
		return;

	i915 = error->i915;
	drm_info(&i915->drm, "%s\n", error_msg(error));

	if (error->simulated ||
	    cmpxchg(&i915->gpu_error.first_error, NULL, error))
		return;

	i915_gpu_coredump_get(error);

	if (!xchg(&warned, true) &&
	    ktime_get_real_seconds() - DRIVER_TIMESTAMP < DAY_AS_SECONDS(180)) {
		pr_info("GPU hangs can indicate a bug anywhere in the entire gfx stack, including userspace.\n");
		pr_info("Please file a _new_ bug report at https://gitlab.freedesktop.org/drm/intel/issues/new.\n");
		pr_info("Please see https://gitlab.freedesktop.org/drm/intel/-/wikis/How-to-file-i915-bugs for details.\n");
		pr_info("drm/i915 developers can then reassign to the right component if it's not a kernel issue.\n");
		pr_info("The GPU crash dump is required to analyze GPU hangs, so please always attach it.\n");
		pr_info("GPU crash dump saved to /sys/class/drm/card%d/error\n",
			i915->drm.primary->index);
	}
}

/**
 * i915_capture_error_state - capture an error record for later analysis
 * @gt: intel_gt which originated the hang
 * @engine_mask: hung engines
 *
>>>>>>> upstream/android-13
 *
 * Should be called when an error is detected (either a hang or an error
 * interrupt) to capture error state from the time of the error.  Fills
 * out a structure which becomes available in debugfs for user level tools
 * to pick up.
 */
<<<<<<< HEAD
void i915_capture_error_state(struct drm_i915_private *i915,
			      u32 engine_mask,
			      const char *error_msg)
{
	static bool warned;
	struct i915_gpu_state *error;
	unsigned long flags;

	if (!i915_modparams.error_capture)
		return;

	if (READ_ONCE(i915->gpu_error.first_error))
		return;

	error = i915_capture_gpu_state(i915);
	if (!error) {
		DRM_DEBUG_DRIVER("out of memory, not capturing error state\n");
		return;
	}

	i915_error_capture_msg(i915, error, engine_mask, error_msg);
	DRM_INFO("%s\n", error->error_msg);

	if (!error->simulated) {
		spin_lock_irqsave(&i915->gpu_error.lock, flags);
		if (!i915->gpu_error.first_error) {
			i915->gpu_error.first_error = error;
			error = NULL;
		}
		spin_unlock_irqrestore(&i915->gpu_error.lock, flags);
	}

	if (error) {
		__i915_gpu_state_free(&error->ref);
		return;
	}

	if (!warned &&
	    ktime_get_real_seconds() - DRIVER_TIMESTAMP < DAY_AS_SECONDS(180)) {
		DRM_INFO("GPU hangs can indicate a bug anywhere in the entire gfx stack, including userspace.\n");
		DRM_INFO("Please file a _new_ bug report on bugs.freedesktop.org against DRI -> DRM/Intel\n");
		DRM_INFO("drm/i915 developers can then reassign to the right component if it's not a kernel issue.\n");
		DRM_INFO("The gpu crash dump is required to analyze gpu hangs, so please always attach it.\n");
		DRM_INFO("GPU crash dump saved to /sys/class/drm/card%d/error\n",
			 i915->drm.primary->index);
		warned = true;
	}
}

struct i915_gpu_state *
i915_first_error_state(struct drm_i915_private *i915)
{
	struct i915_gpu_state *error;

	spin_lock_irq(&i915->gpu_error.lock);
	error = i915->gpu_error.first_error;
	if (error)
		i915_gpu_state_get(error);
=======
void i915_capture_error_state(struct intel_gt *gt,
			      intel_engine_mask_t engine_mask)
{
	struct i915_gpu_coredump *error;

	error = i915_gpu_coredump(gt, engine_mask);
	if (IS_ERR(error)) {
		cmpxchg(&gt->i915->gpu_error.first_error, NULL, error);
		return;
	}

	i915_error_state_store(error);
	i915_gpu_coredump_put(error);
}

struct i915_gpu_coredump *
i915_first_error_state(struct drm_i915_private *i915)
{
	struct i915_gpu_coredump *error;

	spin_lock_irq(&i915->gpu_error.lock);
	error = i915->gpu_error.first_error;
	if (!IS_ERR_OR_NULL(error))
		i915_gpu_coredump_get(error);
>>>>>>> upstream/android-13
	spin_unlock_irq(&i915->gpu_error.lock);

	return error;
}

void i915_reset_error_state(struct drm_i915_private *i915)
{
<<<<<<< HEAD
	struct i915_gpu_state *error;

	spin_lock_irq(&i915->gpu_error.lock);
	error = i915->gpu_error.first_error;
	i915->gpu_error.first_error = NULL;
	spin_unlock_irq(&i915->gpu_error.lock);

	i915_gpu_state_put(error);
=======
	struct i915_gpu_coredump *error;

	spin_lock_irq(&i915->gpu_error.lock);
	error = i915->gpu_error.first_error;
	if (error != ERR_PTR(-ENODEV)) /* if disabled, always disabled */
		i915->gpu_error.first_error = NULL;
	spin_unlock_irq(&i915->gpu_error.lock);

	if (!IS_ERR_OR_NULL(error))
		i915_gpu_coredump_put(error);
}

void i915_disable_error_state(struct drm_i915_private *i915, int err)
{
	spin_lock_irq(&i915->gpu_error.lock);
	if (!i915->gpu_error.first_error)
		i915->gpu_error.first_error = ERR_PTR(err);
	spin_unlock_irq(&i915->gpu_error.lock);
>>>>>>> upstream/android-13
}
