// SPDX-License-Identifier: GPL-2.0 OR MIT
/**************************************************************************
 *
 * Copyright 2009 - 2015 VMware, Inc., Palo Alto, CA., USA
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
#include <linux/sync_file.h>

#include "vmwgfx_drv.h"
#include "vmwgfx_reg.h"
#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_placement.h>
#include "vmwgfx_so.h"
#include "vmwgfx_binding.h"
<<<<<<< HEAD

#define VMW_RES_HT_ORDER 12

=======
#include "vmwgfx_mksstat.h"

#define VMW_RES_HT_ORDER 12

/*
 * Helper macro to get dx_ctx_node if available otherwise print an error
 * message. This is for use in command verifier function where if dx_ctx_node
 * is not set then command is invalid.
 */
#define VMW_GET_CTX_NODE(__sw_context)                                        \
({                                                                            \
	__sw_context->dx_ctx_node ? __sw_context->dx_ctx_node : ({            \
		VMW_DEBUG_USER("SM context is not set at %s\n", __func__);    \
		__sw_context->dx_ctx_node;                                    \
	});                                                                   \
})

#define VMW_DECLARE_CMD_VAR(__var, __type)                                    \
	struct {                                                              \
		SVGA3dCmdHeader header;                                       \
		__type body;                                                  \
	} __var

/**
 * struct vmw_relocation - Buffer object relocation
 *
 * @head: List head for the command submission context's relocation list
 * @vbo: Non ref-counted pointer to buffer object
 * @mob_loc: Pointer to location for mob id to be modified
 * @location: Pointer to location for guest pointer to be modified
 */
struct vmw_relocation {
	struct list_head head;
	struct vmw_buffer_object *vbo;
	union {
		SVGAMobId *mob_loc;
		SVGAGuestPtr *location;
	};
};

>>>>>>> upstream/android-13
/**
 * enum vmw_resource_relocation_type - Relocation type for resources
 *
 * @vmw_res_rel_normal: Traditional relocation. The resource id in the
 * command stream is replaced with the actual id after validation.
 * @vmw_res_rel_nop: NOP relocation. The command is unconditionally replaced
 * with a NOP.
<<<<<<< HEAD
 * @vmw_res_rel_cond_nop: Conditional NOP relocation. If the resource id
 * after validation is -1, the command is replaced with a NOP. Otherwise no
 * action.
 */
=======
 * @vmw_res_rel_cond_nop: Conditional NOP relocation. If the resource id after
 * validation is -1, the command is replaced with a NOP. Otherwise no action.
 * @vmw_res_rel_max: Last value in the enum - used for error checking
*/
>>>>>>> upstream/android-13
enum vmw_resource_relocation_type {
	vmw_res_rel_normal,
	vmw_res_rel_nop,
	vmw_res_rel_cond_nop,
	vmw_res_rel_max
};

/**
 * struct vmw_resource_relocation - Relocation info for resources
 *
 * @head: List head for the software context's relocation list.
 * @res: Non-ref-counted pointer to the resource.
<<<<<<< HEAD
 * @offset: Offset of single byte entries into the command buffer where the
 * id that needs fixup is located.
=======
 * @offset: Offset of single byte entries into the command buffer where the id
 * that needs fixup is located.
>>>>>>> upstream/android-13
 * @rel_type: Type of relocation.
 */
struct vmw_resource_relocation {
	struct list_head head;
	const struct vmw_resource *res;
	u32 offset:29;
	enum vmw_resource_relocation_type rel_type:3;
};

/**
<<<<<<< HEAD
 * struct vmw_resource_val_node - Validation info for resources
 *
 * @head: List head for the software context's resource list.
 * @hash: Hash entry for quick resouce to val_node lookup.
 * @res: Ref-counted pointer to the resource.
 * @switch_backup: Boolean whether to switch backup buffer on unreserve.
 * @new_backup: Refcounted pointer to the new backup buffer.
 * @staged_bindings: If @res is a context, tracks bindings set up during
 * the command batch. Otherwise NULL.
 * @new_backup_offset: New backup buffer offset if @new_backup is non-NUll.
 * @first_usage: Set to true the first time the resource is referenced in
 * the command stream.
 * @switching_backup: The command stream provides a new backup buffer for a
 * resource.
 * @no_buffer_needed: This means @switching_backup is true on first buffer
 * reference. So resource reservation does not need to allocate a backup
 * buffer for the resource.
 */
struct vmw_resource_val_node {
	struct list_head head;
	struct drm_hash_item hash;
	struct vmw_resource *res;
	struct vmw_buffer_object *new_backup;
	struct vmw_ctx_binding_state *staged_bindings;
	unsigned long new_backup_offset;
	u32 first_usage : 1;
	u32 switching_backup : 1;
	u32 no_buffer_needed : 1;
=======
 * struct vmw_ctx_validation_info - Extra validation metadata for contexts
 *
 * @head: List head of context list
 * @ctx: The context resource
 * @cur: The context's persistent binding state
 * @staged: The binding state changes of this command buffer
 */
struct vmw_ctx_validation_info {
	struct list_head head;
	struct vmw_resource *ctx;
	struct vmw_ctx_binding_state *cur;
	struct vmw_ctx_binding_state *staged;
>>>>>>> upstream/android-13
};

/**
 * struct vmw_cmd_entry - Describe a command for the verifier
 *
<<<<<<< HEAD
 * @user_allow: Whether allowed from the execbuf ioctl.
 * @gb_disable: Whether disabled if guest-backed objects are available.
 * @gb_enable: Whether enabled iff guest-backed objects are available.
=======
 * @func: Call-back to handle the command.
 * @user_allow: Whether allowed from the execbuf ioctl.
 * @gb_disable: Whether disabled if guest-backed objects are available.
 * @gb_enable: Whether enabled iff guest-backed objects are available.
 * @cmd_name: Name of the command.
>>>>>>> upstream/android-13
 */
struct vmw_cmd_entry {
	int (*func) (struct vmw_private *, struct vmw_sw_context *,
		     SVGA3dCmdHeader *);
	bool user_allow;
	bool gb_disable;
	bool gb_enable;
	const char *cmd_name;
};

#define VMW_CMD_DEF(_cmd, _func, _user_allow, _gb_disable, _gb_enable)	\
	[(_cmd) - SVGA_3D_CMD_BASE] = {(_func), (_user_allow),\
				       (_gb_disable), (_gb_enable), #_cmd}

static int vmw_resource_context_res_add(struct vmw_private *dev_priv,
					struct vmw_sw_context *sw_context,
					struct vmw_resource *ctx);
static int vmw_translate_mob_ptr(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGAMobId *id,
				 struct vmw_buffer_object **vmw_bo_p);
<<<<<<< HEAD
static int vmw_bo_to_validate_list(struct vmw_sw_context *sw_context,
				   struct vmw_buffer_object *vbo,
				   bool validate_as_mob,
				   uint32_t *p_val_node);
=======
>>>>>>> upstream/android-13
/**
 * vmw_ptr_diff - Compute the offset from a to b in bytes
 *
 * @a: A starting pointer.
 * @b: A pointer offset in the same address space.
 *
 * Returns: The offset in bytes between the two pointers.
 */
static size_t vmw_ptr_diff(void *a, void *b)
{
	return (unsigned long) b - (unsigned long) a;
}

/**
<<<<<<< HEAD
 * vmw_resources_unreserve - unreserve resources previously reserved for
 * command submission.
 *
 * @sw_context: pointer to the software context
 * @backoff: Whether command submission failed.
 */
static void vmw_resources_unreserve(struct vmw_sw_context *sw_context,
				    bool backoff)
{
	struct vmw_resource_val_node *val;
	struct list_head *list = &sw_context->resource_list;

	if (sw_context->dx_query_mob && !backoff)
		vmw_context_bind_dx_query(sw_context->dx_query_ctx,
					  sw_context->dx_query_mob);

	list_for_each_entry(val, list, head) {
		struct vmw_resource *res = val->res;
		bool switch_backup =
			(backoff) ? false : val->switching_backup;

		/*
		 * Transfer staged context bindings to the
		 * persistent context binding tracker.
		 */
		if (unlikely(val->staged_bindings)) {
			if (!backoff) {
				vmw_binding_state_commit
					(vmw_context_binding_state(val->res),
					 val->staged_bindings);
			}

			if (val->staged_bindings != sw_context->staged_bindings)
				vmw_binding_state_free(val->staged_bindings);
			else
				sw_context->staged_bindings_inuse = false;
			val->staged_bindings = NULL;
		}
		vmw_resource_unreserve(res, switch_backup, val->new_backup,
				       val->new_backup_offset);
		vmw_bo_unreference(&val->new_backup);
	}
}

/**
 * vmw_cmd_ctx_first_setup - Perform the setup needed when a context is
 * added to the validate list.
 *
 * @dev_priv: Pointer to the device private:
 * @sw_context: The validation context:
 * @node: The validation node holding this context.
 */
static int vmw_cmd_ctx_first_setup(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   struct vmw_resource_val_node *node)
{
	int ret;

	ret = vmw_resource_context_res_add(dev_priv, sw_context, node->res);
=======
 * vmw_execbuf_bindings_commit - Commit modified binding state
 *
 * @sw_context: The command submission context
 * @backoff: Whether this is part of the error path and binding state changes
 * should be ignored
 */
static void vmw_execbuf_bindings_commit(struct vmw_sw_context *sw_context,
					bool backoff)
{
	struct vmw_ctx_validation_info *entry;

	list_for_each_entry(entry, &sw_context->ctx_list, head) {
		if (!backoff)
			vmw_binding_state_commit(entry->cur, entry->staged);

		if (entry->staged != sw_context->staged_bindings)
			vmw_binding_state_free(entry->staged);
		else
			sw_context->staged_bindings_inuse = false;
	}

	/* List entries are freed with the validation context */
	INIT_LIST_HEAD(&sw_context->ctx_list);
}

/**
 * vmw_bind_dx_query_mob - Bind the DX query MOB if referenced
 *
 * @sw_context: The command submission context
 */
static void vmw_bind_dx_query_mob(struct vmw_sw_context *sw_context)
{
	if (sw_context->dx_query_mob)
		vmw_context_bind_dx_query(sw_context->dx_query_ctx,
					  sw_context->dx_query_mob);
}

/**
 * vmw_cmd_ctx_first_setup - Perform the setup needed when a context is added to
 * the validate list.
 *
 * @dev_priv: Pointer to the device private:
 * @sw_context: The command submission context
 * @res: Pointer to the resource
 * @node: The validation node holding the context resource metadata
 */
static int vmw_cmd_ctx_first_setup(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   struct vmw_resource *res,
				   struct vmw_ctx_validation_info *node)
{
	int ret;

	ret = vmw_resource_context_res_add(dev_priv, sw_context, res);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		goto out_err;

	if (!sw_context->staged_bindings) {
<<<<<<< HEAD
		sw_context->staged_bindings =
			vmw_binding_state_alloc(dev_priv);
		if (IS_ERR(sw_context->staged_bindings)) {
			DRM_ERROR("Failed to allocate context binding "
				  "information.\n");
=======
		sw_context->staged_bindings = vmw_binding_state_alloc(dev_priv);
		if (IS_ERR(sw_context->staged_bindings)) {
>>>>>>> upstream/android-13
			ret = PTR_ERR(sw_context->staged_bindings);
			sw_context->staged_bindings = NULL;
			goto out_err;
		}
	}

	if (sw_context->staged_bindings_inuse) {
<<<<<<< HEAD
		node->staged_bindings = vmw_binding_state_alloc(dev_priv);
		if (IS_ERR(node->staged_bindings)) {
			DRM_ERROR("Failed to allocate context binding "
				  "information.\n");
			ret = PTR_ERR(node->staged_bindings);
			node->staged_bindings = NULL;
			goto out_err;
		}
	} else {
		node->staged_bindings = sw_context->staged_bindings;
		sw_context->staged_bindings_inuse = true;
	}

	return 0;
=======
		node->staged = vmw_binding_state_alloc(dev_priv);
		if (IS_ERR(node->staged)) {
			ret = PTR_ERR(node->staged);
			node->staged = NULL;
			goto out_err;
		}
	} else {
		node->staged = sw_context->staged_bindings;
		sw_context->staged_bindings_inuse = true;
	}

	node->ctx = res;
	node->cur = vmw_context_binding_state(res);
	list_add_tail(&node->head, &sw_context->ctx_list);

	return 0;

>>>>>>> upstream/android-13
out_err:
	return ret;
}

/**
<<<<<<< HEAD
 * vmw_resource_val_add - Add a resource to the software context's
 * resource list if it's not already on it.
 *
 * @sw_context: Pointer to the software context.
 * @res: Pointer to the resource.
 * @p_node On successful return points to a valid pointer to a
 * struct vmw_resource_val_node, if non-NULL on entry.
 */
static int vmw_resource_val_add(struct vmw_sw_context *sw_context,
				struct vmw_resource *res,
				struct vmw_resource_val_node **p_node)
{
	struct vmw_private *dev_priv = res->dev_priv;
	struct vmw_resource_val_node *node;
	struct drm_hash_item *hash;
	int ret;

	if (likely(drm_ht_find_item(&sw_context->res_ht, (unsigned long) res,
				    &hash) == 0)) {
		node = container_of(hash, struct vmw_resource_val_node, hash);
		node->first_usage = false;
		if (unlikely(p_node != NULL))
			*p_node = node;
		return 0;
	}

	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if (unlikely(!node)) {
		DRM_ERROR("Failed to allocate a resource validation "
			  "entry.\n");
		return -ENOMEM;
	}

	node->hash.key = (unsigned long) res;
	ret = drm_ht_insert_item(&sw_context->res_ht, &node->hash);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Failed to initialize a resource validation "
			  "entry.\n");
		kfree(node);
		return ret;
	}
	node->res = vmw_resource_reference(res);
	node->first_usage = true;
	if (unlikely(p_node != NULL))
		*p_node = node;

	if (!dev_priv->has_mob) {
		list_add_tail(&node->head, &sw_context->resource_list);
		return 0;
	}

	switch (vmw_res_type(res)) {
	case vmw_res_context:
	case vmw_res_dx_context:
		list_add(&node->head, &sw_context->ctx_resource_list);
		ret = vmw_cmd_ctx_first_setup(dev_priv, sw_context, node);
		break;
	case vmw_res_cotable:
		list_add_tail(&node->head, &sw_context->ctx_resource_list);
		break;
	default:
		list_add_tail(&node->head, &sw_context->resource_list);
		break;
	}

	return ret;
}

/**
 * vmw_view_res_val_add - Add a view and the surface it's pointing to
 * to the validation list
=======
 * vmw_execbuf_res_size - calculate extra size fore the resource validation node
 *
 * @dev_priv: Pointer to the device private struct.
 * @res_type: The resource type.
 *
 * Guest-backed contexts and DX contexts require extra size to store execbuf
 * private information in the validation node. Typically the binding manager
 * associated data structures.
 *
 * Returns: The extra size requirement based on resource type.
 */
static unsigned int vmw_execbuf_res_size(struct vmw_private *dev_priv,
					 enum vmw_res_type res_type)
{
	return (res_type == vmw_res_dx_context ||
		(res_type == vmw_res_context && dev_priv->has_mob)) ?
		sizeof(struct vmw_ctx_validation_info) : 0;
}

/**
 * vmw_execbuf_rcache_update - Update a resource-node cache entry
 *
 * @rcache: Pointer to the entry to update.
 * @res: Pointer to the resource.
 * @private: Pointer to the execbuf-private space in the resource validation
 * node.
 */
static void vmw_execbuf_rcache_update(struct vmw_res_cache_entry *rcache,
				      struct vmw_resource *res,
				      void *private)
{
	rcache->res = res;
	rcache->private = private;
	rcache->valid = 1;
	rcache->valid_handle = 0;
}

/**
 * vmw_execbuf_res_noref_val_add - Add a resource described by an unreferenced
 * rcu-protected pointer to the validation list.
 *
 * @sw_context: Pointer to the software context.
 * @res: Unreferenced rcu-protected pointer to the resource.
 * @dirty: Whether to change dirty status.
 *
 * Returns: 0 on success. Negative error code on failure. Typical error codes
 * are %-EINVAL on inconsistency and %-ESRCH if the resource was doomed.
 */
static int vmw_execbuf_res_noref_val_add(struct vmw_sw_context *sw_context,
					 struct vmw_resource *res,
					 u32 dirty)
{
	struct vmw_private *dev_priv = res->dev_priv;
	int ret;
	enum vmw_res_type res_type = vmw_res_type(res);
	struct vmw_res_cache_entry *rcache;
	struct vmw_ctx_validation_info *ctx_info;
	bool first_usage;
	unsigned int priv_size;

	rcache = &sw_context->res_cache[res_type];
	if (likely(rcache->valid && rcache->res == res)) {
		if (dirty)
			vmw_validation_res_set_dirty(sw_context->ctx,
						     rcache->private, dirty);
		vmw_user_resource_noref_release();
		return 0;
	}

	priv_size = vmw_execbuf_res_size(dev_priv, res_type);
	ret = vmw_validation_add_resource(sw_context->ctx, res, priv_size,
					  dirty, (void **)&ctx_info,
					  &first_usage);
	vmw_user_resource_noref_release();
	if (ret)
		return ret;

	if (priv_size && first_usage) {
		ret = vmw_cmd_ctx_first_setup(dev_priv, sw_context, res,
					      ctx_info);
		if (ret) {
			VMW_DEBUG_USER("Failed first usage context setup.\n");
			return ret;
		}
	}

	vmw_execbuf_rcache_update(rcache, res, ctx_info);
	return 0;
}

/**
 * vmw_execbuf_res_noctx_val_add - Add a non-context resource to the resource
 * validation list if it's not already on it
 *
 * @sw_context: Pointer to the software context.
 * @res: Pointer to the resource.
 * @dirty: Whether to change dirty status.
 *
 * Returns: Zero on success. Negative error code on failure.
 */
static int vmw_execbuf_res_noctx_val_add(struct vmw_sw_context *sw_context,
					 struct vmw_resource *res,
					 u32 dirty)
{
	struct vmw_res_cache_entry *rcache;
	enum vmw_res_type res_type = vmw_res_type(res);
	void *ptr;
	int ret;

	rcache = &sw_context->res_cache[res_type];
	if (likely(rcache->valid && rcache->res == res)) {
		if (dirty)
			vmw_validation_res_set_dirty(sw_context->ctx,
						     rcache->private, dirty);
		return 0;
	}

	ret = vmw_validation_add_resource(sw_context->ctx, res, 0, dirty,
					  &ptr, NULL);
	if (ret)
		return ret;

	vmw_execbuf_rcache_update(rcache, res, ptr);

	return 0;
}

/**
 * vmw_view_res_val_add - Add a view and the surface it's pointing to to the
 * validation list
>>>>>>> upstream/android-13
 *
 * @sw_context: The software context holding the validation list.
 * @view: Pointer to the view resource.
 *
 * Returns 0 if success, negative error code otherwise.
 */
static int vmw_view_res_val_add(struct vmw_sw_context *sw_context,
				struct vmw_resource *view)
{
	int ret;

	/*
<<<<<<< HEAD
	 * First add the resource the view is pointing to, otherwise
	 * it may be swapped out when the view is validated.
	 */
	ret = vmw_resource_val_add(sw_context, vmw_view_srf(view), NULL);
	if (ret)
		return ret;

	return vmw_resource_val_add(sw_context, view, NULL);
}

/**
 * vmw_view_id_val_add - Look up a view and add it and the surface it's
 * pointing to to the validation list.
=======
	 * First add the resource the view is pointing to, otherwise it may be
	 * swapped out when the view is validated.
	 */
	ret = vmw_execbuf_res_noctx_val_add(sw_context, vmw_view_srf(view),
					    vmw_view_dirtying(view));
	if (ret)
		return ret;

	return vmw_execbuf_res_noctx_val_add(sw_context, view,
					     VMW_RES_DIRTY_NONE);
}

/**
 * vmw_view_id_val_add - Look up a view and add it and the surface it's pointing
 * to to the validation list.
>>>>>>> upstream/android-13
 *
 * @sw_context: The software context holding the validation list.
 * @view_type: The view type to look up.
 * @id: view id of the view.
 *
<<<<<<< HEAD
 * The view is represented by a view id and the DX context it's created on,
 * or scheduled for creation on. If there is no DX context set, the function
 * will return -EINVAL. Otherwise returns 0 on success and -EINVAL on failure.
 */
static int vmw_view_id_val_add(struct vmw_sw_context *sw_context,
			       enum vmw_view_type view_type, u32 id)
{
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *view;
	int ret;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	view = vmw_view_lookup(sw_context->man, view_type, id);
	if (IS_ERR(view))
		return PTR_ERR(view);

	ret = vmw_view_res_val_add(sw_context, view);
	vmw_resource_unreference(&view);

	return ret;
=======
 * The view is represented by a view id and the DX context it's created on, or
 * scheduled for creation on. If there is no DX context set, the function will
 * return an -EINVAL error pointer.
 *
 * Returns: Unreferenced pointer to the resource on success, negative error
 * pointer on failure.
 */
static struct vmw_resource *
vmw_view_id_val_add(struct vmw_sw_context *sw_context,
		    enum vmw_view_type view_type, u32 id)
{
	struct vmw_ctx_validation_info *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *view;
	int ret;

	if (!ctx_node)
		return ERR_PTR(-EINVAL);

	view = vmw_view_lookup(sw_context->man, view_type, id);
	if (IS_ERR(view))
		return view;

	ret = vmw_view_res_val_add(sw_context, view);
	if (ret)
		return ERR_PTR(ret);

	return view;
>>>>>>> upstream/android-13
}

/**
 * vmw_resource_context_res_add - Put resources previously bound to a context on
 * the validation list
 *
 * @dev_priv: Pointer to a device private structure
 * @sw_context: Pointer to a software context used for this command submission
 * @ctx: Pointer to the context resource
 *
<<<<<<< HEAD
 * This function puts all resources that were previously bound to @ctx on
 * the resource validation list. This is part of the context state reemission
=======
 * This function puts all resources that were previously bound to @ctx on the
 * resource validation list. This is part of the context state reemission
>>>>>>> upstream/android-13
 */
static int vmw_resource_context_res_add(struct vmw_private *dev_priv,
					struct vmw_sw_context *sw_context,
					struct vmw_resource *ctx)
{
	struct list_head *binding_list;
	struct vmw_ctx_bindinfo *entry;
	int ret = 0;
	struct vmw_resource *res;
	u32 i;
<<<<<<< HEAD

	/* Add all cotables to the validation list. */
	if (dev_priv->has_dx && vmw_res_type(ctx) == vmw_res_dx_context) {
		for (i = 0; i < SVGA_COTABLE_DX10_MAX; ++i) {
=======
	u32 cotable_max = has_sm5_context(ctx->dev_priv) ?
		SVGA_COTABLE_MAX : SVGA_COTABLE_DX10_MAX;

	/* Add all cotables to the validation list. */
	if (has_sm4_context(dev_priv) &&
	    vmw_res_type(ctx) == vmw_res_dx_context) {
		for (i = 0; i < cotable_max; ++i) {
>>>>>>> upstream/android-13
			res = vmw_context_cotable(ctx, i);
			if (IS_ERR(res))
				continue;

<<<<<<< HEAD
			ret = vmw_resource_val_add(sw_context, res, NULL);
			vmw_resource_unreference(&res);
=======
			ret = vmw_execbuf_res_noctx_val_add(sw_context, res,
							    VMW_RES_DIRTY_SET);
>>>>>>> upstream/android-13
			if (unlikely(ret != 0))
				return ret;
		}
	}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	/* Add all resources bound to the context to the validation list */
	mutex_lock(&dev_priv->binding_mutex);
	binding_list = vmw_context_binding_list(ctx);

	list_for_each_entry(entry, binding_list, ctx_list) {
<<<<<<< HEAD
		/* entry->res is not refcounted */
		res = vmw_resource_reference_unless_doomed(entry->res);
		if (unlikely(res == NULL))
			continue;

		if (vmw_res_type(entry->res) == vmw_res_view)
			ret = vmw_view_res_val_add(sw_context, entry->res);
		else
			ret = vmw_resource_val_add(sw_context, entry->res,
						   NULL);
		vmw_resource_unreference(&res);
=======
		if (vmw_res_type(entry->res) == vmw_res_view)
			ret = vmw_view_res_val_add(sw_context, entry->res);
		else
			ret = vmw_execbuf_res_noctx_val_add
				(sw_context, entry->res,
				 vmw_binding_dirtying(entry->bt));
>>>>>>> upstream/android-13
		if (unlikely(ret != 0))
			break;
	}

<<<<<<< HEAD
	if (dev_priv->has_dx && vmw_res_type(ctx) == vmw_res_dx_context) {
=======
	if (has_sm4_context(dev_priv) &&
	    vmw_res_type(ctx) == vmw_res_dx_context) {
>>>>>>> upstream/android-13
		struct vmw_buffer_object *dx_query_mob;

		dx_query_mob = vmw_context_get_dx_query_mob(ctx);
		if (dx_query_mob)
<<<<<<< HEAD
			ret = vmw_bo_to_validate_list(sw_context,
						      dx_query_mob,
						      true, NULL);
=======
			ret = vmw_validation_add_bo(sw_context->ctx,
						    dx_query_mob, true, false);
>>>>>>> upstream/android-13
	}

	mutex_unlock(&dev_priv->binding_mutex);
	return ret;
}

/**
 * vmw_resource_relocation_add - Add a relocation to the relocation list
 *
<<<<<<< HEAD
 * @list: Pointer to head of relocation list.
 * @res: The resource.
 * @offset: Offset into the command buffer currently being parsed where the
 * id that needs fixup is located. Granularity is one byte.
 * @rel_type: Relocation type.
 */
static int vmw_resource_relocation_add(struct list_head *list,
=======
 * @sw_context: Pointer to the software context.
 * @res: The resource.
 * @offset: Offset into the command buffer currently being parsed where the id
 * that needs fixup is located. Granularity is one byte.
 * @rel_type: Relocation type.
 */
static int vmw_resource_relocation_add(struct vmw_sw_context *sw_context,
>>>>>>> upstream/android-13
				       const struct vmw_resource *res,
				       unsigned long offset,
				       enum vmw_resource_relocation_type
				       rel_type)
{
	struct vmw_resource_relocation *rel;

<<<<<<< HEAD
	rel = kmalloc(sizeof(*rel), GFP_KERNEL);
	if (unlikely(!rel)) {
		DRM_ERROR("Failed to allocate a resource relocation.\n");
=======
	rel = vmw_validation_mem_alloc(sw_context->ctx, sizeof(*rel));
	if (unlikely(!rel)) {
		VMW_DEBUG_USER("Failed to allocate a resource relocation.\n");
>>>>>>> upstream/android-13
		return -ENOMEM;
	}

	rel->res = res;
	rel->offset = offset;
	rel->rel_type = rel_type;
<<<<<<< HEAD
	list_add_tail(&rel->head, list);
=======
	list_add_tail(&rel->head, &sw_context->res_relocations);
>>>>>>> upstream/android-13

	return 0;
}

/**
 * vmw_resource_relocations_free - Free all relocations on a list
 *
<<<<<<< HEAD
 * @list: Pointer to the head of the relocation list.
 */
static void vmw_resource_relocations_free(struct list_head *list)
{
	struct vmw_resource_relocation *rel, *n;

	list_for_each_entry_safe(rel, n, list, head) {
		list_del(&rel->head);
		kfree(rel);
	}
=======
 * @list: Pointer to the head of the relocation list
 */
static void vmw_resource_relocations_free(struct list_head *list)
{
	/* Memory is validation context memory, so no need to free it */
	INIT_LIST_HEAD(list);
>>>>>>> upstream/android-13
}

/**
 * vmw_resource_relocations_apply - Apply all relocations on a list
 *
<<<<<<< HEAD
 * @cb: Pointer to the start of the command buffer bein patch. This need
 * not be the same buffer as the one being parsed when the relocation
 * list was built, but the contents must be the same modulo the
 * resource ids.
=======
 * @cb: Pointer to the start of the command buffer bein patch. This need not be
 * the same buffer as the one being parsed when the relocation list was built,
 * but the contents must be the same modulo the resource ids.
>>>>>>> upstream/android-13
 * @list: Pointer to the head of the relocation list.
 */
static void vmw_resource_relocations_apply(uint32_t *cb,
					   struct list_head *list)
{
	struct vmw_resource_relocation *rel;

	/* Validate the struct vmw_resource_relocation member size */
	BUILD_BUG_ON(SVGA_CB_MAX_SIZE >= (1 << 29));
	BUILD_BUG_ON(vmw_res_rel_max >= (1 << 3));

	list_for_each_entry(rel, list, head) {
		u32 *addr = (u32 *)((unsigned long) cb + rel->offset);
		switch (rel->rel_type) {
		case vmw_res_rel_normal:
			*addr = rel->res->id;
			break;
		case vmw_res_rel_nop:
			*addr = SVGA_3D_CMD_NOP;
			break;
		default:
			if (rel->res->id == -1)
				*addr = SVGA_3D_CMD_NOP;
			break;
		}
	}
}

static int vmw_cmd_invalid(struct vmw_private *dev_priv,
			   struct vmw_sw_context *sw_context,
			   SVGA3dCmdHeader *header)
{
	return -EINVAL;
}

static int vmw_cmd_ok(struct vmw_private *dev_priv,
		      struct vmw_sw_context *sw_context,
		      SVGA3dCmdHeader *header)
{
	return 0;
}

/**
<<<<<<< HEAD
 * vmw_bo_to_validate_list - add a bo to a validate list
 *
 * @sw_context: The software context used for this command submission batch.
 * @bo: The buffer object to add.
 * @validate_as_mob: Validate this buffer as a MOB.
 * @p_val_node: If non-NULL Will be updated with the validate node number
 * on return.
 *
 * Returns -EINVAL if the limit of number of buffer objects per command
 * submission is reached.
 */
static int vmw_bo_to_validate_list(struct vmw_sw_context *sw_context,
				   struct vmw_buffer_object *vbo,
				   bool validate_as_mob,
				   uint32_t *p_val_node)
{
	uint32_t val_node;
	struct vmw_validate_buffer *vval_buf;
	struct ttm_validate_buffer *val_buf;
	struct drm_hash_item *hash;
	int ret;

	if (likely(drm_ht_find_item(&sw_context->res_ht, (unsigned long) vbo,
				    &hash) == 0)) {
		vval_buf = container_of(hash, struct vmw_validate_buffer,
					hash);
		if (unlikely(vval_buf->validate_as_mob != validate_as_mob)) {
			DRM_ERROR("Inconsistent buffer usage.\n");
			return -EINVAL;
		}
		val_buf = &vval_buf->base;
		val_node = vval_buf - sw_context->val_bufs;
	} else {
		val_node = sw_context->cur_val_buf;
		if (unlikely(val_node >= VMWGFX_MAX_VALIDATIONS)) {
			DRM_ERROR("Max number of DMA buffers per submission "
				  "exceeded.\n");
			return -EINVAL;
		}
		vval_buf = &sw_context->val_bufs[val_node];
		vval_buf->hash.key = (unsigned long) vbo;
		ret = drm_ht_insert_item(&sw_context->res_ht, &vval_buf->hash);
		if (unlikely(ret != 0)) {
			DRM_ERROR("Failed to initialize a buffer validation "
				  "entry.\n");
			return ret;
		}
		++sw_context->cur_val_buf;
		val_buf = &vval_buf->base;
		val_buf->bo = ttm_bo_reference(&vbo->base);
		val_buf->shared = false;
		list_add_tail(&val_buf->head, &sw_context->validate_nodes);
		vval_buf->validate_as_mob = validate_as_mob;
	}

	if (p_val_node)
		*p_val_node = val_node;

	return 0;
}

/**
 * vmw_resources_reserve - Reserve all resources on the sw_context's
 * resource list.
 *
 * @sw_context: Pointer to the software context.
 *
 * Note that since vmware's command submission currently is protected by
 * the cmdbuf mutex, no fancy deadlock avoidance is required for resources,
 * since only a single thread at once will attempt this.
 */
static int vmw_resources_reserve(struct vmw_sw_context *sw_context)
{
	struct vmw_resource_val_node *val;
	int ret = 0;

	list_for_each_entry(val, &sw_context->resource_list, head) {
		struct vmw_resource *res = val->res;

		ret = vmw_resource_reserve(res, true, val->no_buffer_needed);
		if (unlikely(ret != 0))
			return ret;

		if (res->backup) {
			struct vmw_buffer_object *vbo = res->backup;

			ret = vmw_bo_to_validate_list
				(sw_context, vbo,
				 vmw_resource_needs_backup(res), NULL);

			if (unlikely(ret != 0))
				return ret;
		}
	}
=======
 * vmw_resources_reserve - Reserve all resources on the sw_context's resource
 * list.
 *
 * @sw_context: Pointer to the software context.
 *
 * Note that since vmware's command submission currently is protected by the
 * cmdbuf mutex, no fancy deadlock avoidance is required for resources, since
 * only a single thread at once will attempt this.
 */
static int vmw_resources_reserve(struct vmw_sw_context *sw_context)
{
	int ret;

	ret = vmw_validation_res_reserve(sw_context->ctx, true);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	if (sw_context->dx_query_mob) {
		struct vmw_buffer_object *expected_dx_query_mob;

		expected_dx_query_mob =
			vmw_context_get_dx_query_mob(sw_context->dx_query_ctx);
		if (expected_dx_query_mob &&
		    expected_dx_query_mob != sw_context->dx_query_mob) {
			ret = -EINVAL;
		}
	}

	return ret;
}

/**
<<<<<<< HEAD
 * vmw_resources_validate - Validate all resources on the sw_context's
 * resource list.
 *
 * @sw_context: Pointer to the software context.
 *
 * Before this function is called, all resource backup buffers must have
 * been validated.
 */
static int vmw_resources_validate(struct vmw_sw_context *sw_context)
{
	struct vmw_resource_val_node *val;
	int ret;

	list_for_each_entry(val, &sw_context->resource_list, head) {
		struct vmw_resource *res = val->res;
		struct vmw_buffer_object *backup = res->backup;

		ret = vmw_resource_validate(res);
		if (unlikely(ret != 0)) {
			if (ret != -ERESTARTSYS)
				DRM_ERROR("Failed to validate resource.\n");
			return ret;
		}

		/* Check if the resource switched backup buffer */
		if (backup && res->backup && (backup != res->backup)) {
			struct vmw_buffer_object *vbo = res->backup;

			ret = vmw_bo_to_validate_list
				(sw_context, vbo,
				 vmw_resource_needs_backup(res), NULL);
			if (ret) {
				ttm_bo_unreserve(&vbo->base);
				return ret;
			}
		}
	}
	return 0;
}

/**
 * vmw_cmd_res_reloc_add - Add a resource to a software context's
 * relocation- and validation lists.
 *
 * @dev_priv: Pointer to a struct vmw_private identifying the device.
 * @sw_context: Pointer to the software context.
 * @id_loc: Pointer to where the id that needs translation is located.
 * @res: Valid pointer to a struct vmw_resource.
 * @p_val: If non null, a pointer to the struct vmw_resource_validate_node
 * used for this resource is returned here.
 */
static int vmw_cmd_res_reloc_add(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 uint32_t *id_loc,
				 struct vmw_resource *res,
				 struct vmw_resource_val_node **p_val)
{
	int ret;
	struct vmw_resource_val_node *node;

	*p_val = NULL;
	ret = vmw_resource_relocation_add(&sw_context->res_relocations,
					  res,
					  vmw_ptr_diff(sw_context->buf_start,
						       id_loc),
					  vmw_res_rel_normal);
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_resource_val_add(sw_context, res, &node);
	if (unlikely(ret != 0))
		return ret;

	if (p_val)
		*p_val = node;

	return 0;
}


/**
 * vmw_cmd_res_check - Check that a resource is present and if so, put it
 * on the resource validate list unless it's already there.
=======
 * vmw_cmd_res_check - Check that a resource is present and if so, put it on the
 * resource validate list unless it's already there.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private structure.
 * @sw_context: Pointer to the software context.
 * @res_type: Resource type.
<<<<<<< HEAD
 * @converter: User-space visisble type specific information.
 * @id_loc: Pointer to the location in the command buffer currently being
 * parsed from where the user-space resource id handle is located.
 * @p_val: Pointer to pointer to resource validalidation node. Populated
 * on exit.
=======
 * @dirty: Whether to change dirty status.
 * @converter: User-space visisble type specific information.
 * @id_loc: Pointer to the location in the command buffer currently being parsed
 * from where the user-space resource id handle is located.
 * @p_res: Pointer to pointer to resource validalidation node. Populated on
 * exit.
>>>>>>> upstream/android-13
 */
static int
vmw_cmd_res_check(struct vmw_private *dev_priv,
		  struct vmw_sw_context *sw_context,
		  enum vmw_res_type res_type,
<<<<<<< HEAD
		  const struct vmw_user_resource_conv *converter,
		  uint32_t *id_loc,
		  struct vmw_resource_val_node **p_val)
{
	struct vmw_res_cache_entry *rcache =
		&sw_context->res_cache[res_type];
	struct vmw_resource *res;
	struct vmw_resource_val_node *node;
	int ret;

	if (*id_loc == SVGA3D_INVALID_ID) {
		if (p_val)
			*p_val = NULL;
		if (res_type == vmw_res_context) {
			DRM_ERROR("Illegal context invalid id.\n");
=======
		  u32 dirty,
		  const struct vmw_user_resource_conv *converter,
		  uint32_t *id_loc,
		  struct vmw_resource **p_res)
{
	struct vmw_res_cache_entry *rcache = &sw_context->res_cache[res_type];
	struct vmw_resource *res;
	int ret;

	if (p_res)
		*p_res = NULL;

	if (*id_loc == SVGA3D_INVALID_ID) {
		if (res_type == vmw_res_context) {
			VMW_DEBUG_USER("Illegal context invalid id.\n");
>>>>>>> upstream/android-13
			return -EINVAL;
		}
		return 0;
	}

<<<<<<< HEAD
	/*
	 * Fastpath in case of repeated commands referencing the same
	 * resource
	 */

	if (likely(rcache->valid && *id_loc == rcache->handle)) {
		const struct vmw_resource *res = rcache->res;

		rcache->node->first_usage = false;
		if (p_val)
			*p_val = rcache->node;

		return vmw_resource_relocation_add
			(&sw_context->res_relocations, res,
			 vmw_ptr_diff(sw_context->buf_start, id_loc),
			 vmw_res_rel_normal);
	}

	ret = vmw_user_resource_lookup_handle(dev_priv,
					      sw_context->fp->tfile,
					      *id_loc,
					      converter,
					      &res);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Could not find or use resource 0x%08x.\n",
			  (unsigned) *id_loc);
		dump_stack();
		return ret;
	}

	rcache->valid = true;
	rcache->res = res;
	rcache->handle = *id_loc;

	ret = vmw_cmd_res_reloc_add(dev_priv, sw_context, id_loc,
				    res, &node);
	if (unlikely(ret != 0))
		goto out_no_reloc;

	rcache->node = node;
	if (p_val)
		*p_val = node;
	vmw_resource_unreference(&res);
	return 0;

out_no_reloc:
	BUG_ON(sw_context->error_resource != NULL);
	sw_context->error_resource = res;

	return ret;
}

/**
 * vmw_rebind_dx_query - Rebind DX query associated with the context
=======
	if (likely(rcache->valid_handle && *id_loc == rcache->handle)) {
		res = rcache->res;
		if (dirty)
			vmw_validation_res_set_dirty(sw_context->ctx,
						     rcache->private, dirty);
	} else {
		unsigned int size = vmw_execbuf_res_size(dev_priv, res_type);

		ret = vmw_validation_preload_res(sw_context->ctx, size);
		if (ret)
			return ret;

		res = vmw_user_resource_noref_lookup_handle
			(dev_priv, sw_context->fp->tfile, *id_loc, converter);
		if (IS_ERR(res)) {
			VMW_DEBUG_USER("Could not find/use resource 0x%08x.\n",
				       (unsigned int) *id_loc);
			return PTR_ERR(res);
		}

		ret = vmw_execbuf_res_noref_val_add(sw_context, res, dirty);
		if (unlikely(ret != 0))
			return ret;

		if (rcache->valid && rcache->res == res) {
			rcache->valid_handle = true;
			rcache->handle = *id_loc;
		}
	}

	ret = vmw_resource_relocation_add(sw_context, res,
					  vmw_ptr_diff(sw_context->buf_start,
						       id_loc),
					  vmw_res_rel_normal);
	if (p_res)
		*p_res = res;

	return 0;
}

/**
 * vmw_rebind_all_dx_query - Rebind DX query associated with the context
>>>>>>> upstream/android-13
 *
 * @ctx_res: context the query belongs to
 *
 * This function assumes binding_mutex is held.
 */
static int vmw_rebind_all_dx_query(struct vmw_resource *ctx_res)
{
	struct vmw_private *dev_priv = ctx_res->dev_priv;
	struct vmw_buffer_object *dx_query_mob;
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXBindAllQuery body;
	} *cmd;

=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXBindAllQuery);
>>>>>>> upstream/android-13

	dx_query_mob = vmw_context_get_dx_query_mob(ctx_res);

	if (!dx_query_mob || dx_query_mob->dx_query_ctx)
		return 0;

<<<<<<< HEAD
	cmd = vmw_fifo_reserve_dx(dev_priv, sizeof(*cmd), ctx_res->id);

	if (cmd == NULL) {
		DRM_ERROR("Failed to rebind queries.\n");
		return -ENOMEM;
	}
=======
	cmd = VMW_CMD_CTX_RESERVE(dev_priv, sizeof(*cmd), ctx_res->id);
	if (cmd == NULL)
		return -ENOMEM;
>>>>>>> upstream/android-13

	cmd->header.id = SVGA_3D_CMD_DX_BIND_ALL_QUERY;
	cmd->header.size = sizeof(cmd->body);
	cmd->body.cid = ctx_res->id;
<<<<<<< HEAD
	cmd->body.mobid = dx_query_mob->base.mem.start;
	vmw_fifo_commit(dev_priv, sizeof(*cmd));
=======
	cmd->body.mobid = dx_query_mob->base.resource->start;
	vmw_cmd_commit(dev_priv, sizeof(*cmd));
>>>>>>> upstream/android-13

	vmw_context_bind_dx_query(ctx_res, dx_query_mob);

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_rebind_contexts - Rebind all resources previously bound to
 * referenced contexts.
=======
 * vmw_rebind_contexts - Rebind all resources previously bound to referenced
 * contexts.
>>>>>>> upstream/android-13
 *
 * @sw_context: Pointer to the software context.
 *
 * Rebind context binding points that have been scrubbed because of eviction.
 */
static int vmw_rebind_contexts(struct vmw_sw_context *sw_context)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *val;
	int ret;

	list_for_each_entry(val, &sw_context->resource_list, head) {
		if (unlikely(!val->staged_bindings))
			break;

		ret = vmw_binding_rebind_all
			(vmw_context_binding_state(val->res));
		if (unlikely(ret != 0)) {
			if (ret != -ERESTARTSYS)
				DRM_ERROR("Failed to rebind context.\n");
			return ret;
		}

		ret = vmw_rebind_all_dx_query(val->res);
		if (ret != 0)
			return ret;
=======
	struct vmw_ctx_validation_info *val;
	int ret;

	list_for_each_entry(val, &sw_context->ctx_list, head) {
		ret = vmw_binding_rebind_all(val->cur);
		if (unlikely(ret != 0)) {
			if (ret != -ERESTARTSYS)
				VMW_DEBUG_USER("Failed to rebind context.\n");
			return ret;
		}

		ret = vmw_rebind_all_dx_query(val->ctx);
		if (ret != 0) {
			VMW_DEBUG_USER("Failed to rebind queries.\n");
			return ret;
		}
>>>>>>> upstream/android-13
	}

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_view_bindings_add - Add an array of view bindings to a context
 * binding state tracker.
=======
 * vmw_view_bindings_add - Add an array of view bindings to a context binding
 * state tracker.
>>>>>>> upstream/android-13
 *
 * @sw_context: The execbuf state used for this command.
 * @view_type: View type for the bindings.
 * @binding_type: Binding type for the bindings.
 * @shader_slot: The shader slot to user for the bindings.
 * @view_ids: Array of view ids to be bound.
 * @num_views: Number of view ids in @view_ids.
 * @first_slot: The binding slot to be used for the first view id in @view_ids.
 */
static int vmw_view_bindings_add(struct vmw_sw_context *sw_context,
				 enum vmw_view_type view_type,
				 enum vmw_ctx_binding_type binding_type,
				 uint32 shader_slot,
				 uint32 view_ids[], u32 num_views,
				 u32 first_slot)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_cmdbuf_res_manager *man;
	u32 i;
	int ret;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	man = sw_context->man;
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	u32 i;

	if (!ctx_node)
		return -EINVAL;

>>>>>>> upstream/android-13
	for (i = 0; i < num_views; ++i) {
		struct vmw_ctx_bindinfo_view binding;
		struct vmw_resource *view = NULL;

		if (view_ids[i] != SVGA3D_INVALID_ID) {
<<<<<<< HEAD
			view = vmw_view_lookup(man, view_type, view_ids[i]);
			if (IS_ERR(view)) {
				DRM_ERROR("View not found.\n");
				return PTR_ERR(view);
			}

			ret = vmw_view_res_val_add(sw_context, view);
			if (ret) {
				DRM_ERROR("Could not add view to "
					  "validation list.\n");
				vmw_resource_unreference(&view);
				return ret;
			}
		}
		binding.bi.ctx = ctx_node->res;
=======
			view = vmw_view_id_val_add(sw_context, view_type,
						   view_ids[i]);
			if (IS_ERR(view)) {
				VMW_DEBUG_USER("View not found.\n");
				return PTR_ERR(view);
			}
		}
		binding.bi.ctx = ctx_node->ctx;
>>>>>>> upstream/android-13
		binding.bi.res = view;
		binding.bi.bt = binding_type;
		binding.shader_slot = shader_slot;
		binding.slot = first_slot + i;
<<<<<<< HEAD
		vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
				shader_slot, binding.slot);
		if (view)
			vmw_resource_unreference(&view);
=======
		vmw_binding_add(ctx_node->staged, &binding.bi,
				shader_slot, binding.slot);
>>>>>>> upstream/android-13
	}

	return 0;
}

/**
 * vmw_cmd_cid_check - Check a command header for valid context information.
 *
 * @dev_priv: Pointer to a device private structure.
 * @sw_context: Pointer to the software context.
 * @header: A command header with an embedded user-space context handle.
 *
 * Convenience function: Call vmw_cmd_res_check with the user-space context
 * handle embedded in @header.
 */
static int vmw_cmd_cid_check(struct vmw_private *dev_priv,
			     struct vmw_sw_context *sw_context,
			     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_cid_cmd {
		SVGA3dCmdHeader header;
		uint32_t cid;
	} *cmd;

	cmd = container_of(header, struct vmw_cid_cmd, header);
	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				 user_context_converter, &cmd->cid, NULL);
=======
	VMW_DECLARE_CMD_VAR(*cmd, uint32_t) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				 VMW_RES_DIRTY_SET, user_context_converter,
				 &cmd->body, NULL);
}

/**
 * vmw_execbuf_info_from_res - Get the private validation metadata for a
 * recently validated resource
 *
 * @sw_context: Pointer to the command submission context
 * @res: The resource
 *
 * The resource pointed to by @res needs to be present in the command submission
 * context's resource cache and hence the last resource of that type to be
 * processed by the validation code.
 *
 * Return: a pointer to the private metadata of the resource, or NULL if it
 * wasn't found
 */
static struct vmw_ctx_validation_info *
vmw_execbuf_info_from_res(struct vmw_sw_context *sw_context,
			  struct vmw_resource *res)
{
	struct vmw_res_cache_entry *rcache =
		&sw_context->res_cache[vmw_res_type(res)];

	if (rcache->valid && rcache->res == res)
		return rcache->private;

	WARN_ON_ONCE(true);
	return NULL;
>>>>>>> upstream/android-13
}

static int vmw_cmd_set_render_target_check(struct vmw_private *dev_priv,
					   struct vmw_sw_context *sw_context,
					   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_sid_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSetRenderTarget body;
	} *cmd;
	struct vmw_resource_val_node *ctx_node;
	struct vmw_resource_val_node *res_node;
	int ret;

	cmd = container_of(header, struct vmw_sid_cmd, header);

	if (cmd->body.type >= SVGA3D_RT_MAX) {
		DRM_ERROR("Illegal render target type %u.\n",
			  (unsigned) cmd->body.type);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSetRenderTarget);
	struct vmw_resource *ctx;
	struct vmw_resource *res;
	int ret;

	cmd = container_of(header, typeof(*cmd), header);

	if (cmd->body.type >= SVGA3D_RT_MAX) {
		VMW_DEBUG_USER("Illegal render target type %u.\n",
			       (unsigned int) cmd->body.type);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
<<<<<<< HEAD
				user_context_converter, &cmd->body.cid,
				&ctx_node);
=======
				VMW_RES_DIRTY_SET, user_context_converter,
				&cmd->body.cid, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				user_surface_converter,
				&cmd->body.target.sid, &res_node);
	if (unlikely(ret != 0))
=======
				VMW_RES_DIRTY_SET, user_surface_converter,
				&cmd->body.target.sid, &res);
	if (unlikely(ret))
>>>>>>> upstream/android-13
		return ret;

	if (dev_priv->has_mob) {
		struct vmw_ctx_bindinfo_view binding;
<<<<<<< HEAD

		binding.bi.ctx = ctx_node->res;
		binding.bi.res = res_node ? res_node->res : NULL;
		binding.bi.bt = vmw_ctx_binding_rt;
		binding.slot = cmd->body.type;
		vmw_binding_add(ctx_node->staged_bindings,
				&binding.bi, 0, binding.slot);
=======
		struct vmw_ctx_validation_info *node;

		node = vmw_execbuf_info_from_res(sw_context, ctx);
		if (!node)
			return -EINVAL;

		binding.bi.ctx = ctx;
		binding.bi.res = res;
		binding.bi.bt = vmw_ctx_binding_rt;
		binding.slot = cmd->body.type;
		vmw_binding_add(node->staged, &binding.bi, 0, binding.slot);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int vmw_cmd_surface_copy_check(struct vmw_private *dev_priv,
				      struct vmw_sw_context *sw_context,
				      SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_sid_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSurfaceCopy body;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_sid_cmd, header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSurfaceCopy);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				&cmd->body.src.sid, NULL);
	if (ret)
		return ret;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				 user_surface_converter,
=======
				 VMW_RES_DIRTY_SET, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.dest.sid, NULL);
}

static int vmw_cmd_buffer_copy_check(struct vmw_private *dev_priv,
<<<<<<< HEAD
				      struct vmw_sw_context *sw_context,
				      SVGA3dCmdHeader *header)
{
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXBufferCopy body;
	} *cmd;
=======
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXBufferCopy);
>>>>>>> upstream/android-13
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				user_surface_converter,
=======
				VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				&cmd->body.src, NULL);
	if (ret != 0)
		return ret;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				 user_surface_converter,
=======
				 VMW_RES_DIRTY_SET, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.dest, NULL);
}

static int vmw_cmd_pred_copy_check(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXPredCopyRegion body;
	} *cmd;
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXPredCopyRegion);
>>>>>>> upstream/android-13
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				user_surface_converter,
=======
				VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				&cmd->body.srcSid, NULL);
	if (ret != 0)
		return ret;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				 user_surface_converter,
=======
				 VMW_RES_DIRTY_SET, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.dstSid, NULL);
}

static int vmw_cmd_stretch_blt_check(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_sid_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSurfaceStretchBlt body;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_sid_cmd, header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter,
				&cmd->body.src.sid, NULL);
	if (unlikely(ret != 0))
		return ret;
	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSurfaceStretchBlt);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				VMW_RES_DIRTY_NONE, user_surface_converter,
				&cmd->body.src.sid, NULL);
	if (unlikely(ret != 0))
		return ret;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_SET, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.dest.sid, NULL);
}

static int vmw_cmd_blt_surf_screen_check(struct vmw_private *dev_priv,
					 struct vmw_sw_context *sw_context,
					 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_sid_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdBlitSurfaceToScreen body;
	} *cmd;

	cmd = container_of(header, struct vmw_sid_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdBlitSurfaceToScreen) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.srcImage.sid, NULL);
}

static int vmw_cmd_present_check(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_sid_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdPresent body;
	} *cmd;


	cmd = container_of(header, struct vmw_sid_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter, &cmd->body.sid,
				 NULL);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdPresent) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
				 &cmd->body.sid, NULL);
>>>>>>> upstream/android-13
}

/**
 * vmw_query_bo_switch_prepare - Prepare to switch pinned buffer for queries.
 *
 * @dev_priv: The device private structure.
 * @new_query_bo: The new buffer holding query results.
 * @sw_context: The software context used for this command submission.
 *
<<<<<<< HEAD
 * This function checks whether @new_query_bo is suitable for holding
 * query results, and if another buffer currently is pinned for query
 * results. If so, the function prepares the state of @sw_context for
 * switching pinned buffers after successful submission of the current
 * command batch.
=======
 * This function checks whether @new_query_bo is suitable for holding query
 * results, and if another buffer currently is pinned for query results. If so,
 * the function prepares the state of @sw_context for switching pinned buffers
 * after successful submission of the current command batch.
>>>>>>> upstream/android-13
 */
static int vmw_query_bo_switch_prepare(struct vmw_private *dev_priv,
				       struct vmw_buffer_object *new_query_bo,
				       struct vmw_sw_context *sw_context)
{
	struct vmw_res_cache_entry *ctx_entry =
		&sw_context->res_cache[vmw_res_context];
	int ret;

	BUG_ON(!ctx_entry->valid);
	sw_context->last_query_ctx = ctx_entry->res;

	if (unlikely(new_query_bo != sw_context->cur_query_bo)) {

<<<<<<< HEAD
		if (unlikely(new_query_bo->base.num_pages > 4)) {
			DRM_ERROR("Query buffer too large.\n");
=======
		if (unlikely(new_query_bo->base.resource->num_pages > 4)) {
			VMW_DEBUG_USER("Query buffer too large.\n");
>>>>>>> upstream/android-13
			return -EINVAL;
		}

		if (unlikely(sw_context->cur_query_bo != NULL)) {
			sw_context->needs_post_query_barrier = true;
<<<<<<< HEAD
			ret = vmw_bo_to_validate_list(sw_context,
						      sw_context->cur_query_bo,
						      dev_priv->has_mob, NULL);
=======
			ret = vmw_validation_add_bo(sw_context->ctx,
						    sw_context->cur_query_bo,
						    dev_priv->has_mob, false);
>>>>>>> upstream/android-13
			if (unlikely(ret != 0))
				return ret;
		}
		sw_context->cur_query_bo = new_query_bo;

<<<<<<< HEAD
		ret = vmw_bo_to_validate_list(sw_context,
					      dev_priv->dummy_query_bo,
					      dev_priv->has_mob, NULL);
		if (unlikely(ret != 0))
			return ret;

=======
		ret = vmw_validation_add_bo(sw_context->ctx,
					    dev_priv->dummy_query_bo,
					    dev_priv->has_mob, false);
		if (unlikely(ret != 0))
			return ret;
>>>>>>> upstream/android-13
	}

	return 0;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
/**
 * vmw_query_bo_switch_commit - Finalize switching pinned query buffer
 *
 * @dev_priv: The device private structure.
 * @sw_context: The software context used for this command submission batch.
 *
 * This function will check if we're switching query buffers, and will then,
 * issue a dummy occlusion query wait used as a query barrier. When the fence
<<<<<<< HEAD
 * object following that query wait has signaled, we are sure that all
 * preceding queries have finished, and the old query buffer can be unpinned.
 * However, since both the new query buffer and the old one are fenced with
 * that fence, we can do an asynchronus unpin now, and be sure that the
 * old query buffer won't be moved until the fence has signaled.
=======
 * object following that query wait has signaled, we are sure that all preceding
 * queries have finished, and the old query buffer can be unpinned. However,
 * since both the new query buffer and the old one are fenced with that fence,
 * we can do an asynchronus unpin now, and be sure that the old query buffer
 * won't be moved until the fence has signaled.
>>>>>>> upstream/android-13
 *
 * As mentioned above, both the new - and old query buffers need to be fenced
 * using a sequence emitted *after* calling this function.
 */
static void vmw_query_bo_switch_commit(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context)
{
	/*
	 * The validate list should still hold references to all
	 * contexts here.
	 */
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
	if (sw_context->needs_post_query_barrier) {
		struct vmw_res_cache_entry *ctx_entry =
			&sw_context->res_cache[vmw_res_context];
		struct vmw_resource *ctx;
		int ret;

		BUG_ON(!ctx_entry->valid);
		ctx = ctx_entry->res;

<<<<<<< HEAD
		ret = vmw_fifo_emit_dummy_query(dev_priv, ctx->id);

		if (unlikely(ret != 0))
			DRM_ERROR("Out of fifo space for dummy query.\n");
=======
		ret = vmw_cmd_emit_dummy_query(dev_priv, ctx->id);

		if (unlikely(ret != 0))
			VMW_DEBUG_USER("Out of fifo space for dummy query.\n");
>>>>>>> upstream/android-13
	}

	if (dev_priv->pinned_bo != sw_context->cur_query_bo) {
		if (dev_priv->pinned_bo) {
			vmw_bo_pin_reserved(dev_priv->pinned_bo, false);
			vmw_bo_unreference(&dev_priv->pinned_bo);
		}

		if (!sw_context->needs_post_query_barrier) {
			vmw_bo_pin_reserved(sw_context->cur_query_bo, true);

			/*
			 * We pin also the dummy_query_bo buffer so that we
<<<<<<< HEAD
			 * don't need to validate it when emitting
			 * dummy queries in context destroy paths.
			 */

=======
			 * don't need to validate it when emitting dummy queries
			 * in context destroy paths.
			 */
>>>>>>> upstream/android-13
			if (!dev_priv->dummy_query_bo_pinned) {
				vmw_bo_pin_reserved(dev_priv->dummy_query_bo,
						    true);
				dev_priv->dummy_query_bo_pinned = true;
			}

			BUG_ON(sw_context->last_query_ctx == NULL);
			dev_priv->query_cid = sw_context->last_query_ctx->id;
			dev_priv->query_cid_valid = true;
			dev_priv->pinned_bo =
				vmw_bo_reference(sw_context->cur_query_bo);
		}
	}
}

/**
<<<<<<< HEAD
 * vmw_translate_mob_pointer - Prepare to translate a user-space buffer
 * handle to a MOB id.
=======
 * vmw_translate_mob_ptr - Prepare to translate a user-space buffer handle
 * to a MOB id.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private structure.
 * @sw_context: The software context used for this command batch validation.
 * @id: Pointer to the user-space handle to be translated.
<<<<<<< HEAD
 * @vmw_bo_p: Points to a location that, on successful return will carry
 * a reference-counted pointer to the DMA buffer identified by the
=======
 * @vmw_bo_p: Points to a location that, on successful return will carry a
 * non-reference-counted pointer to the buffer object identified by the
>>>>>>> upstream/android-13
 * user-space handle in @id.
 *
 * This function saves information needed to translate a user-space buffer
 * handle to a MOB id. The translation does not take place immediately, but
<<<<<<< HEAD
 * during a call to vmw_apply_relocations(). This function builds a relocation
 * list and a list of buffers to validate. The former needs to be freed using
 * either vmw_apply_relocations() or vmw_free_relocations(). The latter
 * needs to be freed using vmw_clear_validations.
=======
 * during a call to vmw_apply_relocations().
 *
 * This function builds a relocation list and a list of buffers to validate. The
 * former needs to be freed using either vmw_apply_relocations() or
 * vmw_free_relocations(). The latter needs to be freed using
 * vmw_clear_validations.
>>>>>>> upstream/android-13
 */
static int vmw_translate_mob_ptr(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGAMobId *id,
				 struct vmw_buffer_object **vmw_bo_p)
{
<<<<<<< HEAD
	struct vmw_buffer_object *vmw_bo = NULL;
=======
	struct vmw_buffer_object *vmw_bo;
>>>>>>> upstream/android-13
	uint32_t handle = *id;
	struct vmw_relocation *reloc;
	int ret;

<<<<<<< HEAD
	ret = vmw_user_bo_lookup(sw_context->fp->tfile, handle, &vmw_bo, NULL);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Could not find or use MOB buffer.\n");
		ret = -EINVAL;
		goto out_no_reloc;
	}

	if (unlikely(sw_context->cur_reloc >= VMWGFX_MAX_RELOCATIONS)) {
		DRM_ERROR("Max number relocations per submission"
			  " exceeded\n");
		ret = -EINVAL;
		goto out_no_reloc;
	}

	reloc = &sw_context->relocs[sw_context->cur_reloc++];
	reloc->mob_loc = id;
	reloc->location = NULL;

	ret = vmw_bo_to_validate_list(sw_context, vmw_bo, true, &reloc->index);
	if (unlikely(ret != 0))
		goto out_no_reloc;

	*vmw_bo_p = vmw_bo;
	return 0;

out_no_reloc:
	vmw_bo_unreference(&vmw_bo);
	*vmw_bo_p = NULL;
	return ret;
}

/**
 * vmw_translate_guest_pointer - Prepare to translate a user-space buffer
 * handle to a valid SVGAGuestPtr
=======
	vmw_validation_preload_bo(sw_context->ctx);
	vmw_bo = vmw_user_bo_noref_lookup(sw_context->fp->tfile, handle);
	if (IS_ERR(vmw_bo)) {
		VMW_DEBUG_USER("Could not find or use MOB buffer.\n");
		return PTR_ERR(vmw_bo);
	}

	ret = vmw_validation_add_bo(sw_context->ctx, vmw_bo, true, false);
	vmw_user_bo_noref_release();
	if (unlikely(ret != 0))
		return ret;

	reloc = vmw_validation_mem_alloc(sw_context->ctx, sizeof(*reloc));
	if (!reloc)
		return -ENOMEM;

	reloc->mob_loc = id;
	reloc->vbo = vmw_bo;

	*vmw_bo_p = vmw_bo;
	list_add_tail(&reloc->head, &sw_context->bo_relocations);

	return 0;
}

/**
 * vmw_translate_guest_ptr - Prepare to translate a user-space buffer handle
 * to a valid SVGAGuestPtr
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private structure.
 * @sw_context: The software context used for this command batch validation.
 * @ptr: Pointer to the user-space handle to be translated.
<<<<<<< HEAD
 * @vmw_bo_p: Points to a location that, on successful return will carry
 * a reference-counted pointer to the DMA buffer identified by the
 * user-space handle in @id.
=======
 * @vmw_bo_p: Points to a location that, on successful return will carry a
 * non-reference-counted pointer to the DMA buffer identified by the user-space
 * handle in @id.
>>>>>>> upstream/android-13
 *
 * This function saves information needed to translate a user-space buffer
 * handle to a valid SVGAGuestPtr. The translation does not take place
 * immediately, but during a call to vmw_apply_relocations().
<<<<<<< HEAD
=======
 *
>>>>>>> upstream/android-13
 * This function builds a relocation list and a list of buffers to validate.
 * The former needs to be freed using either vmw_apply_relocations() or
 * vmw_free_relocations(). The latter needs to be freed using
 * vmw_clear_validations.
 */
static int vmw_translate_guest_ptr(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGAGuestPtr *ptr,
				   struct vmw_buffer_object **vmw_bo_p)
{
<<<<<<< HEAD
	struct vmw_buffer_object *vmw_bo = NULL;
=======
	struct vmw_buffer_object *vmw_bo;
>>>>>>> upstream/android-13
	uint32_t handle = ptr->gmrId;
	struct vmw_relocation *reloc;
	int ret;

<<<<<<< HEAD
	ret = vmw_user_bo_lookup(sw_context->fp->tfile, handle, &vmw_bo, NULL);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Could not find or use GMR region.\n");
		ret = -EINVAL;
		goto out_no_reloc;
	}

	if (unlikely(sw_context->cur_reloc >= VMWGFX_MAX_RELOCATIONS)) {
		DRM_ERROR("Max number relocations per submission"
			  " exceeded\n");
		ret = -EINVAL;
		goto out_no_reloc;
	}

	reloc = &sw_context->relocs[sw_context->cur_reloc++];
	reloc->location = ptr;

	ret = vmw_bo_to_validate_list(sw_context, vmw_bo, false, &reloc->index);
	if (unlikely(ret != 0))
		goto out_no_reloc;

	*vmw_bo_p = vmw_bo;
	return 0;

out_no_reloc:
	vmw_bo_unreference(&vmw_bo);
	*vmw_bo_p = NULL;
	return ret;
}



/**
 * vmw_cmd_dx_define_query - validate a SVGA_3D_CMD_DX_DEFINE_QUERY command.
=======
	vmw_validation_preload_bo(sw_context->ctx);
	vmw_bo = vmw_user_bo_noref_lookup(sw_context->fp->tfile, handle);
	if (IS_ERR(vmw_bo)) {
		VMW_DEBUG_USER("Could not find or use GMR region.\n");
		return PTR_ERR(vmw_bo);
	}

	ret = vmw_validation_add_bo(sw_context->ctx, vmw_bo, false, false);
	vmw_user_bo_noref_release();
	if (unlikely(ret != 0))
		return ret;

	reloc = vmw_validation_mem_alloc(sw_context->ctx, sizeof(*reloc));
	if (!reloc)
		return -ENOMEM;

	reloc->location = ptr;
	reloc->vbo = vmw_bo;
	*vmw_bo_p = vmw_bo;
	list_add_tail(&reloc->head, &sw_context->bo_relocations);

	return 0;
}

/**
 * vmw_cmd_dx_define_query - validate SVGA_3D_CMD_DX_DEFINE_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 *
 * This function adds the new query into the query COTABLE
 */
static int vmw_cmd_dx_define_query(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_dx_define_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDefineQuery q;
	} *cmd;

	int    ret;
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *cotable_res;


	if (ctx_node == NULL) {
		DRM_ERROR("DX Context not set for query.\n");
		return -EINVAL;
	}

	cmd = container_of(header, struct vmw_dx_define_query_cmd, header);

	if (cmd->q.type <  SVGA3D_QUERYTYPE_MIN ||
	    cmd->q.type >= SVGA3D_QUERYTYPE_MAX)
		return -EINVAL;

	cotable_res = vmw_context_cotable(ctx_node->res, SVGA_COTABLE_DXQUERY);
	ret = vmw_cotable_notify(cotable_res, cmd->q.queryId);
	vmw_resource_unreference(&cotable_res);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXDefineQuery);
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_resource *cotable_res;
	int ret;

	if (!ctx_node)
		return -EINVAL;

	cmd = container_of(header, typeof(*cmd), header);

	if (cmd->body.type <  SVGA3D_QUERYTYPE_MIN ||
	    cmd->body.type >= SVGA3D_QUERYTYPE_MAX)
		return -EINVAL;

	cotable_res = vmw_context_cotable(ctx_node->ctx, SVGA_COTABLE_DXQUERY);
	ret = vmw_cotable_notify(cotable_res, cmd->body.queryId);
>>>>>>> upstream/android-13

	return ret;
}

<<<<<<< HEAD


/**
 * vmw_cmd_dx_bind_query - validate a SVGA_3D_CMD_DX_BIND_QUERY command.
=======
/**
 * vmw_cmd_dx_bind_query - validate SVGA_3D_CMD_DX_BIND_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 *
<<<<<<< HEAD
 * The query bind operation will eventually associate the query ID
 * with its backing MOB.  In this function, we take the user mode
 * MOB ID and use vmw_translate_mob_ptr() to translate it to its
 * kernel mode equivalent.
=======
 * The query bind operation will eventually associate the query ID with its
 * backing MOB.  In this function, we take the user mode MOB ID and use
 * vmw_translate_mob_ptr() to translate it to its kernel mode equivalent.
>>>>>>> upstream/android-13
 */
static int vmw_cmd_dx_bind_query(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_dx_bind_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXBindQuery q;
	} *cmd;

	struct vmw_buffer_object *vmw_bo;
	int    ret;


	cmd = container_of(header, struct vmw_dx_bind_query_cmd, header);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXBindQuery);
	struct vmw_buffer_object *vmw_bo;
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
>>>>>>> upstream/android-13

	/*
	 * Look up the buffer pointed to by q.mobid, put it on the relocation
	 * list so its kernel mode MOB ID can be filled in later
	 */
<<<<<<< HEAD
	ret = vmw_translate_mob_ptr(dev_priv, sw_context, &cmd->q.mobid,
=======
	ret = vmw_translate_mob_ptr(dev_priv, sw_context, &cmd->body.mobid,
>>>>>>> upstream/android-13
				    &vmw_bo);

	if (ret != 0)
		return ret;

	sw_context->dx_query_mob = vmw_bo;
<<<<<<< HEAD
	sw_context->dx_query_ctx = sw_context->dx_ctx_node->res;

	vmw_bo_unreference(&vmw_bo);

	return ret;
}



/**
 * vmw_cmd_begin_gb_query - validate a  SVGA_3D_CMD_BEGIN_GB_QUERY command.
=======
	sw_context->dx_query_ctx = sw_context->dx_ctx_node->ctx;
	return 0;
}

/**
 * vmw_cmd_begin_gb_query - validate SVGA_3D_CMD_BEGIN_GB_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_begin_gb_query(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_begin_gb_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdBeginGBQuery q;
	} *cmd;

	cmd = container_of(header, struct vmw_begin_gb_query_cmd,
			   header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				 user_context_converter, &cmd->q.cid,
				 NULL);
}

/**
 * vmw_cmd_begin_query - validate a  SVGA_3D_CMD_BEGIN_QUERY command.
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdBeginGBQuery) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				 VMW_RES_DIRTY_SET, user_context_converter,
				 &cmd->body.cid, NULL);
}

/**
 * vmw_cmd_begin_query - validate SVGA_3D_CMD_BEGIN_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_begin_query(struct vmw_private *dev_priv,
			       struct vmw_sw_context *sw_context,
			       SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_begin_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdBeginQuery q;
	} *cmd;

	cmd = container_of(header, struct vmw_begin_query_cmd,
			   header);

	if (unlikely(dev_priv->has_mob)) {
		struct {
			SVGA3dCmdHeader header;
			SVGA3dCmdBeginGBQuery q;
		} gb_cmd;
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdBeginQuery) =
		container_of(header, typeof(*cmd), header);

	if (unlikely(dev_priv->has_mob)) {
		VMW_DECLARE_CMD_VAR(gb_cmd, SVGA3dCmdBeginGBQuery);
>>>>>>> upstream/android-13

		BUG_ON(sizeof(gb_cmd) != sizeof(*cmd));

		gb_cmd.header.id = SVGA_3D_CMD_BEGIN_GB_QUERY;
		gb_cmd.header.size = cmd->header.size;
<<<<<<< HEAD
		gb_cmd.q.cid = cmd->q.cid;
		gb_cmd.q.type = cmd->q.type;
=======
		gb_cmd.body.cid = cmd->body.cid;
		gb_cmd.body.type = cmd->body.type;
>>>>>>> upstream/android-13

		memcpy(cmd, &gb_cmd, sizeof(*cmd));
		return vmw_cmd_begin_gb_query(dev_priv, sw_context, header);
	}

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
<<<<<<< HEAD
				 user_context_converter, &cmd->q.cid,
				 NULL);
}

/**
 * vmw_cmd_end_gb_query - validate a  SVGA_3D_CMD_END_GB_QUERY command.
=======
				 VMW_RES_DIRTY_SET, user_context_converter,
				 &cmd->body.cid, NULL);
}

/**
 * vmw_cmd_end_gb_query - validate SVGA_3D_CMD_END_GB_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_end_gb_query(struct vmw_private *dev_priv,
				struct vmw_sw_context *sw_context,
				SVGA3dCmdHeader *header)
{
	struct vmw_buffer_object *vmw_bo;
<<<<<<< HEAD
	struct vmw_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdEndGBQuery q;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_query_cmd, header);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdEndGBQuery);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
>>>>>>> upstream/android-13
	ret = vmw_cmd_cid_check(dev_priv, sw_context, header);
	if (unlikely(ret != 0))
		return ret;

<<<<<<< HEAD
	ret = vmw_translate_mob_ptr(dev_priv, sw_context,
				    &cmd->q.mobid,
=======
	ret = vmw_translate_mob_ptr(dev_priv, sw_context, &cmd->body.mobid,
>>>>>>> upstream/android-13
				    &vmw_bo);
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_query_bo_switch_prepare(dev_priv, vmw_bo, sw_context);

<<<<<<< HEAD
	vmw_bo_unreference(&vmw_bo);
=======
>>>>>>> upstream/android-13
	return ret;
}

/**
<<<<<<< HEAD
 * vmw_cmd_end_query - validate a  SVGA_3D_CMD_END_QUERY command.
=======
 * vmw_cmd_end_query - validate SVGA_3D_CMD_END_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_end_query(struct vmw_private *dev_priv,
			     struct vmw_sw_context *sw_context,
			     SVGA3dCmdHeader *header)
{
	struct vmw_buffer_object *vmw_bo;
<<<<<<< HEAD
	struct vmw_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdEndQuery q;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_query_cmd, header);
	if (dev_priv->has_mob) {
		struct {
			SVGA3dCmdHeader header;
			SVGA3dCmdEndGBQuery q;
		} gb_cmd;
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdEndQuery);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
	if (dev_priv->has_mob) {
		VMW_DECLARE_CMD_VAR(gb_cmd, SVGA3dCmdEndGBQuery);
>>>>>>> upstream/android-13

		BUG_ON(sizeof(gb_cmd) != sizeof(*cmd));

		gb_cmd.header.id = SVGA_3D_CMD_END_GB_QUERY;
		gb_cmd.header.size = cmd->header.size;
<<<<<<< HEAD
		gb_cmd.q.cid = cmd->q.cid;
		gb_cmd.q.type = cmd->q.type;
		gb_cmd.q.mobid = cmd->q.guestResult.gmrId;
		gb_cmd.q.offset = cmd->q.guestResult.offset;
=======
		gb_cmd.body.cid = cmd->body.cid;
		gb_cmd.body.type = cmd->body.type;
		gb_cmd.body.mobid = cmd->body.guestResult.gmrId;
		gb_cmd.body.offset = cmd->body.guestResult.offset;
>>>>>>> upstream/android-13

		memcpy(cmd, &gb_cmd, sizeof(*cmd));
		return vmw_cmd_end_gb_query(dev_priv, sw_context, header);
	}

	ret = vmw_cmd_cid_check(dev_priv, sw_context, header);
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_translate_guest_ptr(dev_priv, sw_context,
<<<<<<< HEAD
				      &cmd->q.guestResult,
				      &vmw_bo);
=======
				      &cmd->body.guestResult, &vmw_bo);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_query_bo_switch_prepare(dev_priv, vmw_bo, sw_context);

<<<<<<< HEAD
	vmw_bo_unreference(&vmw_bo);
=======
>>>>>>> upstream/android-13
	return ret;
}

/**
<<<<<<< HEAD
 * vmw_cmd_wait_gb_query - validate a  SVGA_3D_CMD_WAIT_GB_QUERY command.
=======
 * vmw_cmd_wait_gb_query - validate SVGA_3D_CMD_WAIT_GB_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_wait_gb_query(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGA3dCmdHeader *header)
{
	struct vmw_buffer_object *vmw_bo;
<<<<<<< HEAD
	struct vmw_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdWaitForGBQuery q;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_query_cmd, header);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdWaitForGBQuery);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
>>>>>>> upstream/android-13
	ret = vmw_cmd_cid_check(dev_priv, sw_context, header);
	if (unlikely(ret != 0))
		return ret;

<<<<<<< HEAD
	ret = vmw_translate_mob_ptr(dev_priv, sw_context,
				    &cmd->q.mobid,
=======
	ret = vmw_translate_mob_ptr(dev_priv, sw_context, &cmd->body.mobid,
>>>>>>> upstream/android-13
				    &vmw_bo);
	if (unlikely(ret != 0))
		return ret;

<<<<<<< HEAD
	vmw_bo_unreference(&vmw_bo);
=======
>>>>>>> upstream/android-13
	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_wait_query - validate a  SVGA_3D_CMD_WAIT_QUERY command.
=======
 * vmw_cmd_wait_query - validate SVGA_3D_CMD_WAIT_QUERY command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context used for this command submission.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_wait_query(struct vmw_private *dev_priv,
			      struct vmw_sw_context *sw_context,
			      SVGA3dCmdHeader *header)
{
	struct vmw_buffer_object *vmw_bo;
<<<<<<< HEAD
	struct vmw_query_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdWaitForQuery q;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_query_cmd, header);
	if (dev_priv->has_mob) {
		struct {
			SVGA3dCmdHeader header;
			SVGA3dCmdWaitForGBQuery q;
		} gb_cmd;
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdWaitForQuery);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);
	if (dev_priv->has_mob) {
		VMW_DECLARE_CMD_VAR(gb_cmd, SVGA3dCmdWaitForGBQuery);
>>>>>>> upstream/android-13

		BUG_ON(sizeof(gb_cmd) != sizeof(*cmd));

		gb_cmd.header.id = SVGA_3D_CMD_WAIT_FOR_GB_QUERY;
		gb_cmd.header.size = cmd->header.size;
<<<<<<< HEAD
		gb_cmd.q.cid = cmd->q.cid;
		gb_cmd.q.type = cmd->q.type;
		gb_cmd.q.mobid = cmd->q.guestResult.gmrId;
		gb_cmd.q.offset = cmd->q.guestResult.offset;
=======
		gb_cmd.body.cid = cmd->body.cid;
		gb_cmd.body.type = cmd->body.type;
		gb_cmd.body.mobid = cmd->body.guestResult.gmrId;
		gb_cmd.body.offset = cmd->body.guestResult.offset;
>>>>>>> upstream/android-13

		memcpy(cmd, &gb_cmd, sizeof(*cmd));
		return vmw_cmd_wait_gb_query(dev_priv, sw_context, header);
	}

	ret = vmw_cmd_cid_check(dev_priv, sw_context, header);
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_translate_guest_ptr(dev_priv, sw_context,
<<<<<<< HEAD
				      &cmd->q.guestResult,
				      &vmw_bo);
	if (unlikely(ret != 0))
		return ret;

	vmw_bo_unreference(&vmw_bo);
=======
				      &cmd->body.guestResult, &vmw_bo);
	if (unlikely(ret != 0))
		return ret;

>>>>>>> upstream/android-13
	return 0;
}

static int vmw_cmd_dma(struct vmw_private *dev_priv,
		       struct vmw_sw_context *sw_context,
		       SVGA3dCmdHeader *header)
{
	struct vmw_buffer_object *vmw_bo = NULL;
	struct vmw_surface *srf = NULL;
<<<<<<< HEAD
	struct vmw_dma_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSurfaceDMA dma;
	} *cmd;
	int ret;
	SVGA3dCmdSurfaceDMASuffix *suffix;
	uint32_t bo_size;

	cmd = container_of(header, struct vmw_dma_cmd, header);
	suffix = (SVGA3dCmdSurfaceDMASuffix *)((unsigned long) &cmd->dma +
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSurfaceDMA);
	int ret;
	SVGA3dCmdSurfaceDMASuffix *suffix;
	uint32_t bo_size;
	bool dirty;

	cmd = container_of(header, typeof(*cmd), header);
	suffix = (SVGA3dCmdSurfaceDMASuffix *)((unsigned long) &cmd->body +
>>>>>>> upstream/android-13
					       header->size - sizeof(*suffix));

	/* Make sure device and verifier stays in sync. */
	if (unlikely(suffix->suffixSize != sizeof(*suffix))) {
<<<<<<< HEAD
		DRM_ERROR("Invalid DMA suffix size.\n");
=======
		VMW_DEBUG_USER("Invalid DMA suffix size.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	ret = vmw_translate_guest_ptr(dev_priv, sw_context,
<<<<<<< HEAD
				      &cmd->dma.guest.ptr,
				      &vmw_bo);
=======
				      &cmd->body.guest.ptr, &vmw_bo);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	/* Make sure DMA doesn't cross BO boundaries. */
<<<<<<< HEAD
	bo_size = vmw_bo->base.num_pages * PAGE_SIZE;
	if (unlikely(cmd->dma.guest.ptr.offset > bo_size)) {
		DRM_ERROR("Invalid DMA offset.\n");
		return -EINVAL;
	}

	bo_size -= cmd->dma.guest.ptr.offset;
	if (unlikely(suffix->maximumOffset > bo_size))
		suffix->maximumOffset = bo_size;

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter, &cmd->dma.host.sid,
				NULL);
	if (unlikely(ret != 0)) {
		if (unlikely(ret != -ERESTARTSYS))
			DRM_ERROR("could not find surface for DMA.\n");
		goto out_no_surface;
=======
	bo_size = vmw_bo->base.base.size;
	if (unlikely(cmd->body.guest.ptr.offset > bo_size)) {
		VMW_DEBUG_USER("Invalid DMA offset.\n");
		return -EINVAL;
	}

	bo_size -= cmd->body.guest.ptr.offset;
	if (unlikely(suffix->maximumOffset > bo_size))
		suffix->maximumOffset = bo_size;

	dirty = (cmd->body.transfer == SVGA3D_WRITE_HOST_VRAM) ?
		VMW_RES_DIRTY_SET : 0;
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				dirty, user_surface_converter,
				&cmd->body.host.sid, NULL);
	if (unlikely(ret != 0)) {
		if (unlikely(ret != -ERESTARTSYS))
			VMW_DEBUG_USER("could not find surface for DMA.\n");
		return ret;
>>>>>>> upstream/android-13
	}

	srf = vmw_res_to_srf(sw_context->res_cache[vmw_res_surface].res);

<<<<<<< HEAD
	vmw_kms_cursor_snoop(srf, sw_context->fp->tfile, &vmw_bo->base,
			     header);

out_no_surface:
	vmw_bo_unreference(&vmw_bo);
	return ret;
=======
	vmw_kms_cursor_snoop(srf, sw_context->fp->tfile, &vmw_bo->base, header);

	return 0;
>>>>>>> upstream/android-13
}

static int vmw_cmd_draw(struct vmw_private *dev_priv,
			struct vmw_sw_context *sw_context,
			SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_draw_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDrawPrimitives body;
	} *cmd;
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDrawPrimitives);
>>>>>>> upstream/android-13
	SVGA3dVertexDecl *decl = (SVGA3dVertexDecl *)(
		(unsigned long)header + sizeof(*cmd));
	SVGA3dPrimitiveRange *range;
	uint32_t i;
	uint32_t maxnum;
	int ret;

	ret = vmw_cmd_cid_check(dev_priv, sw_context, header);
	if (unlikely(ret != 0))
		return ret;

<<<<<<< HEAD
	cmd = container_of(header, struct vmw_draw_cmd, header);
	maxnum = (header->size - sizeof(cmd->body)) / sizeof(*decl);

	if (unlikely(cmd->body.numVertexDecls > maxnum)) {
		DRM_ERROR("Illegal number of vertex declarations.\n");
=======
	cmd = container_of(header, typeof(*cmd), header);
	maxnum = (header->size - sizeof(cmd->body)) / sizeof(*decl);

	if (unlikely(cmd->body.numVertexDecls > maxnum)) {
		VMW_DEBUG_USER("Illegal number of vertex declarations.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	for (i = 0; i < cmd->body.numVertexDecls; ++i, ++decl) {
		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
=======
					VMW_RES_DIRTY_NONE,
>>>>>>> upstream/android-13
					user_surface_converter,
					&decl->array.surfaceId, NULL);
		if (unlikely(ret != 0))
			return ret;
	}

	maxnum = (header->size - sizeof(cmd->body) -
		  cmd->body.numVertexDecls * sizeof(*decl)) / sizeof(*range);
	if (unlikely(cmd->body.numRanges > maxnum)) {
<<<<<<< HEAD
		DRM_ERROR("Illegal number of index ranges.\n");
=======
		VMW_DEBUG_USER("Illegal number of index ranges.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	range = (SVGA3dPrimitiveRange *) decl;
	for (i = 0; i < cmd->body.numRanges; ++i, ++range) {
		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
=======
					VMW_RES_DIRTY_NONE,
>>>>>>> upstream/android-13
					user_surface_converter,
					&range->indexArray.surfaceId, NULL);
		if (unlikely(ret != 0))
			return ret;
	}
	return 0;
}

<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
static int vmw_cmd_tex_state(struct vmw_private *dev_priv,
			     struct vmw_sw_context *sw_context,
			     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_tex_state_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSetTextureState state;
	} *cmd;

	SVGA3dTextureState *last_state = (SVGA3dTextureState *)
	  ((unsigned long) header + header->size + sizeof(header));
	SVGA3dTextureState *cur_state = (SVGA3dTextureState *)
		((unsigned long) header + sizeof(struct vmw_tex_state_cmd));
	struct vmw_resource_val_node *ctx_node;
	struct vmw_resource_val_node *res_node;
	int ret;

	cmd = container_of(header, struct vmw_tex_state_cmd,
			   header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				user_context_converter, &cmd->state.cid,
				&ctx_node);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSetTextureState);
	SVGA3dTextureState *last_state = (SVGA3dTextureState *)
	  ((unsigned long) header + header->size + sizeof(header));
	SVGA3dTextureState *cur_state = (SVGA3dTextureState *)
		((unsigned long) header + sizeof(*cmd));
	struct vmw_resource *ctx;
	struct vmw_resource *res;
	int ret;

	cmd = container_of(header, typeof(*cmd), header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				VMW_RES_DIRTY_SET, user_context_converter,
				&cmd->body.cid, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	for (; cur_state < last_state; ++cur_state) {
		if (likely(cur_state->name != SVGA3D_TS_BIND_TEXTURE))
			continue;

		if (cur_state->stage >= SVGA3D_NUM_TEXTURE_UNITS) {
<<<<<<< HEAD
			DRM_ERROR("Illegal texture/sampler unit %u.\n",
				  (unsigned) cur_state->stage);
=======
			VMW_DEBUG_USER("Illegal texture/sampler unit %u.\n",
				       (unsigned int) cur_state->stage);
>>>>>>> upstream/android-13
			return -EINVAL;
		}

		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
					user_surface_converter,
					&cur_state->value, &res_node);
=======
					VMW_RES_DIRTY_NONE,
					user_surface_converter,
					&cur_state->value, &res);
>>>>>>> upstream/android-13
		if (unlikely(ret != 0))
			return ret;

		if (dev_priv->has_mob) {
			struct vmw_ctx_bindinfo_tex binding;
<<<<<<< HEAD

			binding.bi.ctx = ctx_node->res;
			binding.bi.res = res_node ? res_node->res : NULL;
			binding.bi.bt = vmw_ctx_binding_tex;
			binding.texture_stage = cur_state->stage;
			vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
					0, binding.texture_stage);
=======
			struct vmw_ctx_validation_info *node;

			node = vmw_execbuf_info_from_res(sw_context, ctx);
			if (!node)
				return -EINVAL;

			binding.bi.ctx = ctx;
			binding.bi.res = res;
			binding.bi.bt = vmw_ctx_binding_tex;
			binding.texture_stage = cur_state->stage;
			vmw_binding_add(node->staged, &binding.bi, 0,
					binding.texture_stage);
>>>>>>> upstream/android-13
		}
	}

	return 0;
}

static int vmw_cmd_check_define_gmrfb(struct vmw_private *dev_priv,
				      struct vmw_sw_context *sw_context,
				      void *buf)
{
	struct vmw_buffer_object *vmw_bo;
<<<<<<< HEAD
	int ret;
=======
>>>>>>> upstream/android-13

	struct {
		uint32_t header;
		SVGAFifoCmdDefineGMRFB body;
	} *cmd = buf;

<<<<<<< HEAD
	ret = vmw_translate_guest_ptr(dev_priv, sw_context,
				      &cmd->body.ptr,
				      &vmw_bo);
	if (unlikely(ret != 0))
		return ret;

	vmw_bo_unreference(&vmw_bo);

	return ret;
}


=======
	return vmw_translate_guest_ptr(dev_priv, sw_context, &cmd->body.ptr,
				       &vmw_bo);
}

>>>>>>> upstream/android-13
/**
 * vmw_cmd_res_switch_backup - Utility function to handle backup buffer
 * switching
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
<<<<<<< HEAD
 * @val_node: The validation node representing the resource.
=======
 * @res: Pointer to the resource.
>>>>>>> upstream/android-13
 * @buf_id: Pointer to the user-space backup buffer handle in the command
 * stream.
 * @backup_offset: Offset of backup into MOB.
 *
<<<<<<< HEAD
 * This function prepares for registering a switch of backup buffers
 * in the resource metadata just prior to unreserving. It's basically a wrapper
 * around vmw_cmd_res_switch_backup with a different interface.
 */
static int vmw_cmd_res_switch_backup(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     struct vmw_resource_val_node *val_node,
				     uint32_t *buf_id,
				     unsigned long backup_offset)
{
	struct vmw_buffer_object *dma_buf;
	int ret;

	ret = vmw_translate_mob_ptr(dev_priv, sw_context, buf_id, &dma_buf);
	if (ret)
		return ret;

	val_node->switching_backup = true;
	if (val_node->first_usage)
		val_node->no_buffer_needed = true;

	vmw_bo_unreference(&val_node->new_backup);
	val_node->new_backup = dma_buf;
	val_node->new_backup_offset = backup_offset;

	return 0;
}


=======
 * This function prepares for registering a switch of backup buffers in the
 * resource metadata just prior to unreserving. It's basically a wrapper around
 * vmw_cmd_res_switch_backup with a different interface.
 */
static int vmw_cmd_res_switch_backup(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     struct vmw_resource *res, uint32_t *buf_id,
				     unsigned long backup_offset)
{
	struct vmw_buffer_object *vbo;
	void *info;
	int ret;

	info = vmw_execbuf_info_from_res(sw_context, res);
	if (!info)
		return -EINVAL;

	ret = vmw_translate_mob_ptr(dev_priv, sw_context, buf_id, &vbo);
	if (ret)
		return ret;

	vmw_validation_res_switch_backup(sw_context->ctx, info, vbo,
					 backup_offset);
	return 0;
}

>>>>>>> upstream/android-13
/**
 * vmw_cmd_switch_backup - Utility function to handle backup buffer switching
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @res_type: The resource type.
 * @converter: Information about user-space binding for this resource type.
 * @res_id: Pointer to the user-space resource handle in the command stream.
 * @buf_id: Pointer to the user-space backup buffer handle in the command
 * stream.
 * @backup_offset: Offset of backup into MOB.
 *
<<<<<<< HEAD
 * This function prepares for registering a switch of backup buffers
 * in the resource metadata just prior to unreserving. It's basically a wrapper
 * around vmw_cmd_res_switch_backup with a different interface.
=======
 * This function prepares for registering a switch of backup buffers in the
 * resource metadata just prior to unreserving. It's basically a wrapper around
 * vmw_cmd_res_switch_backup with a different interface.
>>>>>>> upstream/android-13
 */
static int vmw_cmd_switch_backup(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 enum vmw_res_type res_type,
				 const struct vmw_user_resource_conv
<<<<<<< HEAD
				 *converter,
				 uint32_t *res_id,
				 uint32_t *buf_id,
				 unsigned long backup_offset)
{
	struct vmw_resource_val_node *val_node;
	int ret;

	ret = vmw_cmd_res_check(dev_priv, sw_context, res_type,
				converter, res_id, &val_node);
	if (ret)
		return ret;

	return vmw_cmd_res_switch_backup(dev_priv, sw_context, val_node,
					 buf_id, backup_offset);
}

/**
 * vmw_cmd_bind_gb_surface - Validate an SVGA_3D_CMD_BIND_GB_SURFACE
 * command
=======
				 *converter, uint32_t *res_id, uint32_t *buf_id,
				 unsigned long backup_offset)
{
	struct vmw_resource *res;
	int ret;

	ret = vmw_cmd_res_check(dev_priv, sw_context, res_type,
				VMW_RES_DIRTY_NONE, converter, res_id, &res);
	if (ret)
		return ret;

	return vmw_cmd_res_switch_backup(dev_priv, sw_context, res, buf_id,
					 backup_offset);
}

/**
 * vmw_cmd_bind_gb_surface - Validate SVGA_3D_CMD_BIND_GB_SURFACE command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_bind_gb_surface(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_bind_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdBindGBSurface body;
	} *cmd;

	cmd = container_of(header, struct vmw_bind_gb_surface_cmd, header);

	return vmw_cmd_switch_backup(dev_priv, sw_context, vmw_res_surface,
				     user_surface_converter,
				     &cmd->body.sid, &cmd->body.mobid,
				     0);
}

/**
 * vmw_cmd_update_gb_image - Validate an SVGA_3D_CMD_UPDATE_GB_IMAGE
 * command
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdBindGBSurface) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_switch_backup(dev_priv, sw_context, vmw_res_surface,
				     user_surface_converter, &cmd->body.sid,
				     &cmd->body.mobid, 0);
}

/**
 * vmw_cmd_update_gb_image - Validate SVGA_3D_CMD_UPDATE_GB_IMAGE command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_update_gb_image(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdUpdateGBImage body;
	} *cmd;

	cmd = container_of(header, struct vmw_gb_surface_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdUpdateGBImage) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.image.sid, NULL);
}

/**
<<<<<<< HEAD
 * vmw_cmd_update_gb_surface - Validate an SVGA_3D_CMD_UPDATE_GB_SURFACE
 * command
=======
 * vmw_cmd_update_gb_surface - Validate SVGA_3D_CMD_UPDATE_GB_SURFACE command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_update_gb_surface(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdUpdateGBSurface body;
	} *cmd;

	cmd = container_of(header, struct vmw_gb_surface_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdUpdateGBSurface) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_CLEAR, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.sid, NULL);
}

/**
<<<<<<< HEAD
 * vmw_cmd_readback_gb_image - Validate an SVGA_3D_CMD_READBACK_GB_IMAGE
 * command
=======
 * vmw_cmd_readback_gb_image - Validate SVGA_3D_CMD_READBACK_GB_IMAGE command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_readback_gb_image(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdReadbackGBImage body;
	} *cmd;

	cmd = container_of(header, struct vmw_gb_surface_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdReadbackGBImage) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.image.sid, NULL);
}

/**
<<<<<<< HEAD
 * vmw_cmd_readback_gb_surface - Validate an SVGA_3D_CMD_READBACK_GB_SURFACE
=======
 * vmw_cmd_readback_gb_surface - Validate SVGA_3D_CMD_READBACK_GB_SURFACE
>>>>>>> upstream/android-13
 * command
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_readback_gb_surface(struct vmw_private *dev_priv,
				       struct vmw_sw_context *sw_context,
				       SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdReadbackGBSurface body;
	} *cmd;

	cmd = container_of(header, struct vmw_gb_surface_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdReadbackGBSurface) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_CLEAR, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.sid, NULL);
}

/**
<<<<<<< HEAD
 * vmw_cmd_invalidate_gb_image - Validate an SVGA_3D_CMD_INVALIDATE_GB_IMAGE
=======
 * vmw_cmd_invalidate_gb_image - Validate SVGA_3D_CMD_INVALIDATE_GB_IMAGE
>>>>>>> upstream/android-13
 * command
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_invalidate_gb_image(struct vmw_private *dev_priv,
				       struct vmw_sw_context *sw_context,
				       SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdInvalidateGBImage body;
	} *cmd;

	cmd = container_of(header, struct vmw_gb_surface_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdInvalidateGBImage) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.image.sid, NULL);
}

/**
<<<<<<< HEAD
 * vmw_cmd_invalidate_gb_surface - Validate an
 * SVGA_3D_CMD_INVALIDATE_GB_SURFACE command
=======
 * vmw_cmd_invalidate_gb_surface - Validate SVGA_3D_CMD_INVALIDATE_GB_SURFACE
 * command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_invalidate_gb_surface(struct vmw_private *dev_priv,
					 struct vmw_sw_context *sw_context,
					 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_gb_surface_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdInvalidateGBSurface body;
	} *cmd;

	cmd = container_of(header, struct vmw_gb_surface_cmd, header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
				 &cmd->body.sid, NULL);
}


/**
 * vmw_cmd_shader_define - Validate an SVGA_3D_CMD_SHADER_DEFINE
 * command
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdInvalidateGBSurface) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_CLEAR, user_surface_converter,
				 &cmd->body.sid, NULL);
}

/**
 * vmw_cmd_shader_define - Validate SVGA_3D_CMD_SHADER_DEFINE command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_shader_define(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_shader_define_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDefineShader body;
	} *cmd;
	int ret;
	size_t size;
	struct vmw_resource_val_node *val;

	cmd = container_of(header, struct vmw_shader_define_cmd,
			   header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				user_context_converter, &cmd->body.cid,
				&val);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDefineShader);
	int ret;
	size_t size;
	struct vmw_resource *ctx;

	cmd = container_of(header, typeof(*cmd), header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				VMW_RES_DIRTY_SET, user_context_converter,
				&cmd->body.cid, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	if (unlikely(!dev_priv->has_mob))
		return 0;

	size = cmd->header.size - sizeof(cmd->body);
<<<<<<< HEAD
	ret = vmw_compat_shader_add(dev_priv,
				    vmw_context_res_man(val->res),
				    cmd->body.shid, cmd + 1,
				    cmd->body.type, size,
				    &sw_context->staged_cmd_res);
	if (unlikely(ret != 0))
		return ret;

	return vmw_resource_relocation_add(&sw_context->res_relocations,
					   NULL,
=======
	ret = vmw_compat_shader_add(dev_priv, vmw_context_res_man(ctx),
				    cmd->body.shid, cmd + 1, cmd->body.type,
				    size, &sw_context->staged_cmd_res);
	if (unlikely(ret != 0))
		return ret;

	return vmw_resource_relocation_add(sw_context, NULL,
>>>>>>> upstream/android-13
					   vmw_ptr_diff(sw_context->buf_start,
							&cmd->header.id),
					   vmw_res_rel_nop);
}

/**
<<<<<<< HEAD
 * vmw_cmd_shader_destroy - Validate an SVGA_3D_CMD_SHADER_DESTROY
 * command
=======
 * vmw_cmd_shader_destroy - Validate SVGA_3D_CMD_SHADER_DESTROY command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_shader_destroy(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_shader_destroy_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDestroyShader body;
	} *cmd;
	int ret;
	struct vmw_resource_val_node *val;

	cmd = container_of(header, struct vmw_shader_destroy_cmd,
			   header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				user_context_converter, &cmd->body.cid,
				&val);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDestroyShader);
	int ret;
	struct vmw_resource *ctx;

	cmd = container_of(header, typeof(*cmd), header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				VMW_RES_DIRTY_SET, user_context_converter,
				&cmd->body.cid, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	if (unlikely(!dev_priv->has_mob))
		return 0;

<<<<<<< HEAD
	ret = vmw_shader_remove(vmw_context_res_man(val->res),
				cmd->body.shid,
				cmd->body.type,
				&sw_context->staged_cmd_res);
	if (unlikely(ret != 0))
		return ret;

	return vmw_resource_relocation_add(&sw_context->res_relocations,
					   NULL,
=======
	ret = vmw_shader_remove(vmw_context_res_man(ctx), cmd->body.shid,
				cmd->body.type, &sw_context->staged_cmd_res);
	if (unlikely(ret != 0))
		return ret;

	return vmw_resource_relocation_add(sw_context, NULL,
>>>>>>> upstream/android-13
					   vmw_ptr_diff(sw_context->buf_start,
							&cmd->header.id),
					   vmw_res_rel_nop);
}

/**
<<<<<<< HEAD
 * vmw_cmd_set_shader - Validate an SVGA_3D_CMD_SET_SHADER
 * command
=======
 * vmw_cmd_set_shader - Validate SVGA_3D_CMD_SET_SHADER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_set_shader(struct vmw_private *dev_priv,
			      struct vmw_sw_context *sw_context,
			      SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_set_shader_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSetShader body;
	} *cmd;
	struct vmw_resource_val_node *ctx_node, *res_node = NULL;
	struct vmw_ctx_bindinfo_shader binding;
	struct vmw_resource *res = NULL;
	int ret;

	cmd = container_of(header, struct vmw_set_shader_cmd,
			   header);

	if (cmd->body.type >= SVGA3D_SHADERTYPE_PREDX_MAX) {
		DRM_ERROR("Illegal shader type %u.\n",
			  (unsigned) cmd->body.type);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSetShader);
	struct vmw_ctx_bindinfo_shader binding;
	struct vmw_resource *ctx, *res = NULL;
	struct vmw_ctx_validation_info *ctx_info;
	int ret;

	cmd = container_of(header, typeof(*cmd), header);

	if (cmd->body.type >= SVGA3D_SHADERTYPE_PREDX_MAX) {
		VMW_DEBUG_USER("Illegal shader type %u.\n",
			       (unsigned int) cmd->body.type);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
<<<<<<< HEAD
				user_context_converter, &cmd->body.cid,
				&ctx_node);
=======
				VMW_RES_DIRTY_SET, user_context_converter,
				&cmd->body.cid, &ctx);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	if (!dev_priv->has_mob)
		return 0;

	if (cmd->body.shid != SVGA3D_INVALID_ID) {
<<<<<<< HEAD
		res = vmw_shader_lookup(vmw_context_res_man(ctx_node->res),
					cmd->body.shid,
					cmd->body.type);

		if (!IS_ERR(res)) {
			ret = vmw_cmd_res_reloc_add(dev_priv, sw_context,
						    &cmd->body.shid, res,
						    &res_node);
			vmw_resource_unreference(&res);
=======
		/*
		 * This is the compat shader path - Per device guest-backed
		 * shaders, but user-space thinks it's per context host-
		 * backed shaders.
		 */
		res = vmw_shader_lookup(vmw_context_res_man(ctx),
					cmd->body.shid, cmd->body.type);
		if (!IS_ERR(res)) {
			ret = vmw_execbuf_res_noctx_val_add(sw_context, res,
							    VMW_RES_DIRTY_NONE);
			if (unlikely(ret != 0))
				return ret;

			ret = vmw_resource_relocation_add
				(sw_context, res,
				 vmw_ptr_diff(sw_context->buf_start,
					      &cmd->body.shid),
				 vmw_res_rel_normal);
>>>>>>> upstream/android-13
			if (unlikely(ret != 0))
				return ret;
		}
	}

<<<<<<< HEAD
	if (!res_node) {
		ret = vmw_cmd_res_check(dev_priv, sw_context,
					vmw_res_shader,
					user_shader_converter,
					&cmd->body.shid, &res_node);
=======
	if (IS_ERR_OR_NULL(res)) {
		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_shader,
					VMW_RES_DIRTY_NONE,
					user_shader_converter, &cmd->body.shid,
					&res);
>>>>>>> upstream/android-13
		if (unlikely(ret != 0))
			return ret;
	}

<<<<<<< HEAD
	binding.bi.ctx = ctx_node->res;
	binding.bi.res = res_node ? res_node->res : NULL;
	binding.bi.bt = vmw_ctx_binding_shader;
	binding.shader_slot = cmd->body.type - SVGA3D_SHADERTYPE_MIN;
	vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
			binding.shader_slot, 0);
=======
	ctx_info = vmw_execbuf_info_from_res(sw_context, ctx);
	if (!ctx_info)
		return -EINVAL;

	binding.bi.ctx = ctx;
	binding.bi.res = res;
	binding.bi.bt = vmw_ctx_binding_shader;
	binding.shader_slot = cmd->body.type - SVGA3D_SHADERTYPE_MIN;
	vmw_binding_add(ctx_info->staged, &binding.bi, binding.shader_slot, 0);

>>>>>>> upstream/android-13
	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_set_shader_const - Validate an SVGA_3D_CMD_SET_SHADER_CONST
 * command
=======
 * vmw_cmd_set_shader_const - Validate SVGA_3D_CMD_SET_SHADER_CONST command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_set_shader_const(struct vmw_private *dev_priv,
				    struct vmw_sw_context *sw_context,
				    SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_set_shader_const_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdSetShaderConst body;
	} *cmd;
	int ret;

	cmd = container_of(header, struct vmw_set_shader_const_cmd,
			   header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				user_context_converter, &cmd->body.cid,
				NULL);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdSetShaderConst);
	int ret;

	cmd = container_of(header, typeof(*cmd), header);

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
				VMW_RES_DIRTY_SET, user_context_converter,
				&cmd->body.cid, NULL);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		return ret;

	if (dev_priv->has_mob)
		header->id = SVGA_3D_CMD_SET_GB_SHADERCONSTS_INLINE;

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_bind_gb_shader - Validate an SVGA_3D_CMD_BIND_GB_SHADER
 * command
=======
 * vmw_cmd_bind_gb_shader - Validate SVGA_3D_CMD_BIND_GB_SHADER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_bind_gb_shader(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_bind_gb_shader_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdBindGBShader body;
	} *cmd;

	cmd = container_of(header, struct vmw_bind_gb_shader_cmd,
			   header);

	return vmw_cmd_switch_backup(dev_priv, sw_context, vmw_res_shader,
				     user_shader_converter,
				     &cmd->body.shid, &cmd->body.mobid,
				     cmd->body.offsetInBytes);
}

/**
 * vmw_cmd_dx_set_single_constant_buffer - Validate an
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdBindGBShader) =
		container_of(header, typeof(*cmd), header);

	return vmw_cmd_switch_backup(dev_priv, sw_context, vmw_res_shader,
				     user_shader_converter, &cmd->body.shid,
				     &cmd->body.mobid, cmd->body.offsetInBytes);
}

/**
 * vmw_cmd_dx_set_single_constant_buffer - Validate
>>>>>>> upstream/android-13
 * SVGA_3D_CMD_DX_SET_SINGLE_CONSTANT_BUFFER command.
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int
vmw_cmd_dx_set_single_constant_buffer(struct vmw_private *dev_priv,
				      struct vmw_sw_context *sw_context,
				      SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetSingleConstantBuffer body;
	} *cmd;
	struct vmw_resource_val_node *res_node = NULL;
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_ctx_bindinfo_cb binding;
	int ret;

	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter,
				&cmd->body.sid, &res_node);
	if (unlikely(ret != 0))
		return ret;

	binding.bi.ctx = ctx_node->res;
	binding.bi.res = res_node ? res_node->res : NULL;
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXSetSingleConstantBuffer);
	SVGA3dShaderType max_shader_num = has_sm5_context(dev_priv) ?
		SVGA3D_NUM_SHADERTYPE : SVGA3D_NUM_SHADERTYPE_DX10;

	struct vmw_resource *res = NULL;
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_ctx_bindinfo_cb binding;
	int ret;

	if (!ctx_node)
		return -EINVAL;

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				VMW_RES_DIRTY_NONE, user_surface_converter,
				&cmd->body.sid, &res);
	if (unlikely(ret != 0))
		return ret;

	binding.bi.ctx = ctx_node->ctx;
	binding.bi.res = res;
>>>>>>> upstream/android-13
	binding.bi.bt = vmw_ctx_binding_cb;
	binding.shader_slot = cmd->body.type - SVGA3D_SHADERTYPE_MIN;
	binding.offset = cmd->body.offsetInBytes;
	binding.size = cmd->body.sizeInBytes;
	binding.slot = cmd->body.slot;

<<<<<<< HEAD
	if (binding.shader_slot >= SVGA3D_NUM_SHADERTYPE_DX10 ||
	    binding.slot >= SVGA3D_DX_MAX_CONSTBUFFERS) {
		DRM_ERROR("Illegal const buffer shader %u slot %u.\n",
			  (unsigned) cmd->body.type,
			  (unsigned) binding.slot);
		return -EINVAL;
	}

	vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
			binding.shader_slot, binding.slot);
=======
	if (binding.shader_slot >= max_shader_num ||
	    binding.slot >= SVGA3D_DX_MAX_CONSTBUFFERS) {
		VMW_DEBUG_USER("Illegal const buffer shader %u slot %u.\n",
			       (unsigned int) cmd->body.type,
			       (unsigned int) binding.slot);
		return -EINVAL;
	}

	vmw_binding_add(ctx_node->staged, &binding.bi, binding.shader_slot,
			binding.slot);
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_set_shader_res - Validate an
 * SVGA_3D_CMD_DX_SET_SHADER_RESOURCES command
=======
 * vmw_cmd_dx_set_shader_res - Validate SVGA_3D_CMD_DX_SET_SHADER_RESOURCES
 * command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_set_shader_res(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetShaderResources body;
	} *cmd = container_of(header, typeof(*cmd), header);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXSetShaderResources) =
		container_of(header, typeof(*cmd), header);
	SVGA3dShaderType max_allowed = has_sm5_context(dev_priv) ?
		SVGA3D_SHADERTYPE_MAX : SVGA3D_SHADERTYPE_DX10_MAX;

>>>>>>> upstream/android-13
	u32 num_sr_view = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dShaderResourceViewId);

	if ((u64) cmd->body.startView + (u64) num_sr_view >
	    (u64) SVGA3D_DX_MAX_SRVIEWS ||
<<<<<<< HEAD
	    cmd->body.type >= SVGA3D_SHADERTYPE_DX10_MAX) {
		DRM_ERROR("Invalid shader binding.\n");
=======
	    cmd->body.type >= max_allowed) {
		VMW_DEBUG_USER("Invalid shader binding.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	return vmw_view_bindings_add(sw_context, vmw_view_sr,
				     vmw_ctx_binding_sr,
				     cmd->body.type - SVGA3D_SHADERTYPE_MIN,
				     (void *) &cmd[1], num_sr_view,
				     cmd->body.startView);
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_set_shader - Validate an SVGA_3D_CMD_DX_SET_SHADER
 * command
=======
 * vmw_cmd_dx_set_shader - Validate SVGA_3D_CMD_DX_SET_SHADER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_set_shader(struct vmw_private *dev_priv,
				 struct vmw_sw_context *sw_context,
				 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetShader body;
	} *cmd;
	struct vmw_resource *res = NULL;
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_ctx_bindinfo_shader binding;
	int ret = 0;

	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	cmd = container_of(header, typeof(*cmd), header);

	if (cmd->body.type >= SVGA3D_SHADERTYPE_DX10_MAX ||
	    cmd->body.type < SVGA3D_SHADERTYPE_MIN) {
		DRM_ERROR("Illegal shader type %u.\n",
			  (unsigned) cmd->body.type);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXSetShader);
	SVGA3dShaderType max_allowed = has_sm5_context(dev_priv) ?
		SVGA3D_SHADERTYPE_MAX : SVGA3D_SHADERTYPE_DX10_MAX;
	struct vmw_resource *res = NULL;
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_ctx_bindinfo_shader binding;
	int ret = 0;

	if (!ctx_node)
		return -EINVAL;

	cmd = container_of(header, typeof(*cmd), header);

	if (cmd->body.type >= max_allowed ||
	    cmd->body.type < SVGA3D_SHADERTYPE_MIN) {
		VMW_DEBUG_USER("Illegal shader type %u.\n",
			       (unsigned int) cmd->body.type);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	if (cmd->body.shaderId != SVGA3D_INVALID_ID) {
		res = vmw_shader_lookup(sw_context->man, cmd->body.shaderId, 0);
		if (IS_ERR(res)) {
<<<<<<< HEAD
			DRM_ERROR("Could not find shader for binding.\n");
			return PTR_ERR(res);
		}

		ret = vmw_resource_val_add(sw_context, res, NULL);
		if (ret)
			goto out_unref;
	}

	binding.bi.ctx = ctx_node->res;
=======
			VMW_DEBUG_USER("Could not find shader for binding.\n");
			return PTR_ERR(res);
		}

		ret = vmw_execbuf_res_noctx_val_add(sw_context, res,
						    VMW_RES_DIRTY_NONE);
		if (ret)
			return ret;
	}

	binding.bi.ctx = ctx_node->ctx;
>>>>>>> upstream/android-13
	binding.bi.res = res;
	binding.bi.bt = vmw_ctx_binding_dx_shader;
	binding.shader_slot = cmd->body.type - SVGA3D_SHADERTYPE_MIN;

<<<<<<< HEAD
	vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
			binding.shader_slot, 0);
out_unref:
	if (res)
		vmw_resource_unreference(&res);

	return ret;
}

/**
 * vmw_cmd_dx_set_vertex_buffers - Validates an
 * SVGA_3D_CMD_DX_SET_VERTEX_BUFFERS command
=======
	vmw_binding_add(ctx_node->staged, &binding.bi, binding.shader_slot, 0);

	return 0;
}

/**
 * vmw_cmd_dx_set_vertex_buffers - Validates SVGA_3D_CMD_DX_SET_VERTEX_BUFFERS
 * command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_set_vertex_buffers(struct vmw_private *dev_priv,
					 struct vmw_sw_context *sw_context,
					 SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_ctx_bindinfo_vb binding;
	struct vmw_resource_val_node *res_node;
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_ctx_bindinfo_vb binding;
	struct vmw_resource *res;
>>>>>>> upstream/android-13
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetVertexBuffers body;
		SVGA3dVertexBuffer buf[];
	} *cmd;
	int i, ret, num;

<<<<<<< HEAD
	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}
=======
	if (!ctx_node)
		return -EINVAL;
>>>>>>> upstream/android-13

	cmd = container_of(header, typeof(*cmd), header);
	num = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dVertexBuffer);
	if ((u64)num + (u64)cmd->body.startBuffer >
	    (u64)SVGA3D_DX_MAX_VERTEXBUFFERS) {
<<<<<<< HEAD
		DRM_ERROR("Invalid number of vertex buffers.\n");
=======
		VMW_DEBUG_USER("Invalid number of vertex buffers.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	for (i = 0; i < num; i++) {
		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
					user_surface_converter,
					&cmd->buf[i].sid, &res_node);
		if (unlikely(ret != 0))
			return ret;

		binding.bi.ctx = ctx_node->res;
		binding.bi.bt = vmw_ctx_binding_vb;
		binding.bi.res = ((res_node) ? res_node->res : NULL);
=======
					VMW_RES_DIRTY_NONE,
					user_surface_converter,
					&cmd->buf[i].sid, &res);
		if (unlikely(ret != 0))
			return ret;

		binding.bi.ctx = ctx_node->ctx;
		binding.bi.bt = vmw_ctx_binding_vb;
		binding.bi.res = res;
>>>>>>> upstream/android-13
		binding.offset = cmd->buf[i].offset;
		binding.stride = cmd->buf[i].stride;
		binding.slot = i + cmd->body.startBuffer;

<<<<<<< HEAD
		vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
				0, binding.slot);
=======
		vmw_binding_add(ctx_node->staged, &binding.bi, 0, binding.slot);
>>>>>>> upstream/android-13
	}

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_ia_set_vertex_buffers - Validate an
=======
 * vmw_cmd_dx_set_index_buffer - Validate
>>>>>>> upstream/android-13
 * SVGA_3D_CMD_DX_IA_SET_INDEX_BUFFER command.
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_set_index_buffer(struct vmw_private *dev_priv,
				       struct vmw_sw_context *sw_context,
				       SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_ctx_bindinfo_ib binding;
	struct vmw_resource_val_node *res_node;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetIndexBuffer body;
	} *cmd;
	int ret;

	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter,
				&cmd->body.sid, &res_node);
	if (unlikely(ret != 0))
		return ret;

	binding.bi.ctx = ctx_node->res;
	binding.bi.res = ((res_node) ? res_node->res : NULL);
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_ctx_bindinfo_ib binding;
	struct vmw_resource *res;
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXSetIndexBuffer);
	int ret;

	if (!ctx_node)
		return -EINVAL;

	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				VMW_RES_DIRTY_NONE, user_surface_converter,
				&cmd->body.sid, &res);
	if (unlikely(ret != 0))
		return ret;

	binding.bi.ctx = ctx_node->ctx;
	binding.bi.res = res;
>>>>>>> upstream/android-13
	binding.bi.bt = vmw_ctx_binding_ib;
	binding.offset = cmd->body.offset;
	binding.format = cmd->body.format;

<<<<<<< HEAD
	vmw_binding_add(ctx_node->staged_bindings, &binding.bi, 0, 0);
=======
	vmw_binding_add(ctx_node->staged, &binding.bi, 0, 0);
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_set_rendertarget - Validate an
 * SVGA_3D_CMD_DX_SET_RENDERTARGETS command
=======
 * vmw_cmd_dx_set_rendertargets - Validate SVGA_3D_CMD_DX_SET_RENDERTARGETS
 * command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_set_rendertargets(struct vmw_private *dev_priv,
					struct vmw_sw_context *sw_context,
					SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetRenderTargets body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;
	u32 num_rt_view = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dRenderTargetViewId);

	if (num_rt_view > SVGA3D_MAX_SIMULTANEOUS_RENDER_TARGETS) {
		DRM_ERROR("Invalid DX Rendertarget binding.\n");
		return -EINVAL;
	}

	ret = vmw_view_bindings_add(sw_context, vmw_view_ds,
				    vmw_ctx_binding_ds, 0,
				    &cmd->body.depthStencilViewId, 1, 0);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXSetRenderTargets) =
		container_of(header, typeof(*cmd), header);
	u32 num_rt_view = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dRenderTargetViewId);
	int ret;

	if (num_rt_view > SVGA3D_DX_MAX_RENDER_TARGETS) {
		VMW_DEBUG_USER("Invalid DX Rendertarget binding.\n");
		return -EINVAL;
	}

	ret = vmw_view_bindings_add(sw_context, vmw_view_ds, vmw_ctx_binding_ds,
				    0, &cmd->body.depthStencilViewId, 1, 0);
>>>>>>> upstream/android-13
	if (ret)
		return ret;

	return vmw_view_bindings_add(sw_context, vmw_view_rt,
<<<<<<< HEAD
				     vmw_ctx_binding_dx_rt, 0,
				     (void *)&cmd[1], num_rt_view, 0);
}

/**
 * vmw_cmd_dx_clear_rendertarget_view - Validate an
=======
				     vmw_ctx_binding_dx_rt, 0, (void *)&cmd[1],
				     num_rt_view, 0);
}

/**
 * vmw_cmd_dx_clear_rendertarget_view - Validate
>>>>>>> upstream/android-13
 * SVGA_3D_CMD_DX_CLEAR_RENDERTARGET_VIEW command
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_clear_rendertarget_view(struct vmw_private *dev_priv,
					      struct vmw_sw_context *sw_context,
					      SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXClearRenderTargetView body;
	} *cmd = container_of(header, typeof(*cmd), header);

	return vmw_view_id_val_add(sw_context, vmw_view_rt,
				   cmd->body.renderTargetViewId);
}

/**
 * vmw_cmd_dx_clear_rendertarget_view - Validate an
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXClearRenderTargetView) =
		container_of(header, typeof(*cmd), header);
	struct vmw_resource *ret;

	ret = vmw_view_id_val_add(sw_context, vmw_view_rt,
				  cmd->body.renderTargetViewId);

	return PTR_ERR_OR_ZERO(ret);
}

/**
 * vmw_cmd_dx_clear_depthstencil_view - Validate
>>>>>>> upstream/android-13
 * SVGA_3D_CMD_DX_CLEAR_DEPTHSTENCIL_VIEW command
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_clear_depthstencil_view(struct vmw_private *dev_priv,
					      struct vmw_sw_context *sw_context,
					      SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXClearDepthStencilView body;
	} *cmd = container_of(header, typeof(*cmd), header);

	return vmw_view_id_val_add(sw_context, vmw_view_ds,
				   cmd->body.depthStencilViewId);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXClearDepthStencilView) =
		container_of(header, typeof(*cmd), header);
	struct vmw_resource *ret;

	ret = vmw_view_id_val_add(sw_context, vmw_view_ds,
				  cmd->body.depthStencilViewId);

	return PTR_ERR_OR_ZERO(ret);
>>>>>>> upstream/android-13
}

static int vmw_cmd_dx_view_define(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource_val_node *srf_node;
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_resource *srf;
>>>>>>> upstream/android-13
	struct vmw_resource *res;
	enum vmw_view_type view_type;
	int ret;
	/*
<<<<<<< HEAD
	 * This is based on the fact that all affected define commands have
	 * the same initial command body layout.
=======
	 * This is based on the fact that all affected define commands have the
	 * same initial command body layout.
>>>>>>> upstream/android-13
	 */
	struct {
		SVGA3dCmdHeader header;
		uint32 defined_id;
		uint32 sid;
	} *cmd;

<<<<<<< HEAD
	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}
=======
	if (!ctx_node)
		return -EINVAL;
>>>>>>> upstream/android-13

	view_type = vmw_view_cmd_to_type(header->id);
	if (view_type == vmw_view_max)
		return -EINVAL;
<<<<<<< HEAD
	cmd = container_of(header, typeof(*cmd), header);
	if (unlikely(cmd->sid == SVGA3D_INVALID_ID)) {
		DRM_ERROR("Invalid surface id.\n");
		return -EINVAL;
	}
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter,
				&cmd->sid, &srf_node);
	if (unlikely(ret != 0))
		return ret;

	res = vmw_context_cotable(ctx_node->res, vmw_view_cotables[view_type]);
	ret = vmw_cotable_notify(res, cmd->defined_id);
	vmw_resource_unreference(&res);
	if (unlikely(ret != 0))
		return ret;

	return vmw_view_add(sw_context->man,
			    ctx_node->res,
			    srf_node->res,
			    view_type,
			    cmd->defined_id,
			    header,
=======

	cmd = container_of(header, typeof(*cmd), header);
	if (unlikely(cmd->sid == SVGA3D_INVALID_ID)) {
		VMW_DEBUG_USER("Invalid surface id.\n");
		return -EINVAL;
	}
	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				VMW_RES_DIRTY_NONE, user_surface_converter,
				&cmd->sid, &srf);
	if (unlikely(ret != 0))
		return ret;

	res = vmw_context_cotable(ctx_node->ctx, vmw_view_cotables[view_type]);
	ret = vmw_cotable_notify(res, cmd->defined_id);
	if (unlikely(ret != 0))
		return ret;

	return vmw_view_add(sw_context->man, ctx_node->ctx, srf, view_type,
			    cmd->defined_id, header,
>>>>>>> upstream/android-13
			    header->size + sizeof(*header),
			    &sw_context->staged_cmd_res);
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_set_so_targets - Validate an
 * SVGA_3D_CMD_DX_SET_SOTARGETS command.
=======
 * vmw_cmd_dx_set_so_targets - Validate SVGA_3D_CMD_DX_SET_SOTARGETS command.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_set_so_targets(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_ctx_bindinfo_so binding;
	struct vmw_resource_val_node *res_node;
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_ctx_bindinfo_so_target binding;
	struct vmw_resource *res;
>>>>>>> upstream/android-13
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetSOTargets body;
		SVGA3dSoTarget targets[];
	} *cmd;
	int i, ret, num;

<<<<<<< HEAD
	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	cmd = container_of(header, typeof(*cmd), header);
	num = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dSoTarget);

	if (num > SVGA3D_DX_MAX_SOTARGETS) {
		DRM_ERROR("Invalid DX SO binding.\n");
=======
	if (!ctx_node)
		return -EINVAL;

	cmd = container_of(header, typeof(*cmd), header);
	num = (cmd->header.size - sizeof(cmd->body)) / sizeof(SVGA3dSoTarget);

	if (num > SVGA3D_DX_MAX_SOTARGETS) {
		VMW_DEBUG_USER("Invalid DX SO binding.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	for (i = 0; i < num; i++) {
		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
					user_surface_converter,
					&cmd->targets[i].sid, &res_node);
		if (unlikely(ret != 0))
			return ret;

		binding.bi.ctx = ctx_node->res;
		binding.bi.res = ((res_node) ? res_node->res : NULL);
		binding.bi.bt = vmw_ctx_binding_so,
=======
					VMW_RES_DIRTY_SET,
					user_surface_converter,
					&cmd->targets[i].sid, &res);
		if (unlikely(ret != 0))
			return ret;

		binding.bi.ctx = ctx_node->ctx;
		binding.bi.res = res;
		binding.bi.bt = vmw_ctx_binding_so_target;
>>>>>>> upstream/android-13
		binding.offset = cmd->targets[i].offset;
		binding.size = cmd->targets[i].sizeInBytes;
		binding.slot = i;

<<<<<<< HEAD
		vmw_binding_add(ctx_node->staged_bindings, &binding.bi,
				0, binding.slot);
=======
		vmw_binding_add(ctx_node->staged, &binding.bi, 0, binding.slot);
>>>>>>> upstream/android-13
	}

	return 0;
}

static int vmw_cmd_dx_so_define(struct vmw_private *dev_priv,
				struct vmw_sw_context *sw_context,
				SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
>>>>>>> upstream/android-13
	struct vmw_resource *res;
	/*
	 * This is based on the fact that all affected define commands have
	 * the same initial command body layout.
	 */
	struct {
		SVGA3dCmdHeader header;
		uint32 defined_id;
	} *cmd;
	enum vmw_so_type so_type;
	int ret;

<<<<<<< HEAD
	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	so_type = vmw_so_cmd_to_type(header->id);
	res = vmw_context_cotable(ctx_node->res, vmw_so_cotables[so_type]);
	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cotable_notify(res, cmd->defined_id);
	vmw_resource_unreference(&res);
=======
	if (!ctx_node)
		return -EINVAL;

	so_type = vmw_so_cmd_to_type(header->id);
	res = vmw_context_cotable(ctx_node->ctx, vmw_so_cotables[so_type]);
	if (IS_ERR(res))
		return PTR_ERR(res);
	cmd = container_of(header, typeof(*cmd), header);
	ret = vmw_cotable_notify(res, cmd->defined_id);
>>>>>>> upstream/android-13

	return ret;
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_check_subresource - Validate an
 * SVGA_3D_CMD_DX_[X]_SUBRESOURCE command
=======
 * vmw_cmd_dx_check_subresource - Validate SVGA_3D_CMD_DX_[X]_SUBRESOURCE
 * command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_check_subresource(struct vmw_private *dev_priv,
					struct vmw_sw_context *sw_context,
					SVGA3dCmdHeader *header)
{
	struct {
		SVGA3dCmdHeader header;
		union {
			SVGA3dCmdDXReadbackSubResource r_body;
			SVGA3dCmdDXInvalidateSubResource i_body;
			SVGA3dCmdDXUpdateSubResource u_body;
			SVGA3dSurfaceId sid;
		};
	} *cmd;

	BUILD_BUG_ON(offsetof(typeof(*cmd), r_body.sid) !=
		     offsetof(typeof(*cmd), sid));
	BUILD_BUG_ON(offsetof(typeof(*cmd), i_body.sid) !=
		     offsetof(typeof(*cmd), sid));
	BUILD_BUG_ON(offsetof(typeof(*cmd), u_body.sid) !=
		     offsetof(typeof(*cmd), sid));

	cmd = container_of(header, typeof(*cmd), header);
<<<<<<< HEAD

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 user_surface_converter,
=======
	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->sid, NULL);
}

static int vmw_cmd_dx_cid_check(struct vmw_private *dev_priv,
				struct vmw_sw_context *sw_context,
				SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;

	if (unlikely(ctx_node == NULL)) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);

	if (!ctx_node)
		return -EINVAL;
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_view_remove - validate a view remove command and
 * schedule the view resource for removal.
=======
 * vmw_cmd_dx_view_remove - validate a view remove command and schedule the view
 * resource for removal.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 *
<<<<<<< HEAD
 * Check that the view exists, and if it was not created using this
 * command batch, conditionally make this command a NOP.
=======
 * Check that the view exists, and if it was not created using this command
 * batch, conditionally make this command a NOP.
>>>>>>> upstream/android-13
 */
static int vmw_cmd_dx_view_remove(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
>>>>>>> upstream/android-13
	struct {
		SVGA3dCmdHeader header;
		union vmw_view_destroy body;
	} *cmd = container_of(header, typeof(*cmd), header);
	enum vmw_view_type view_type = vmw_view_cmd_to_type(header->id);
	struct vmw_resource *view;
	int ret;

<<<<<<< HEAD
	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	ret = vmw_view_remove(sw_context->man,
			      cmd->body.view_id, view_type,
			      &sw_context->staged_cmd_res,
			      &view);
=======
	if (!ctx_node)
		return -EINVAL;

	ret = vmw_view_remove(sw_context->man, cmd->body.view_id, view_type,
			      &sw_context->staged_cmd_res, &view);
>>>>>>> upstream/android-13
	if (ret || !view)
		return ret;

	/*
	 * If the view wasn't created during this command batch, it might
	 * have been removed due to a context swapout, so add a
	 * relocation to conditionally make this command a NOP to avoid
	 * device errors.
	 */
<<<<<<< HEAD
	return vmw_resource_relocation_add(&sw_context->res_relocations,
					   view,
=======
	return vmw_resource_relocation_add(sw_context, view,
>>>>>>> upstream/android-13
					   vmw_ptr_diff(sw_context->buf_start,
							&cmd->header.id),
					   vmw_res_rel_cond_nop);
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_define_shader - Validate an SVGA_3D_CMD_DX_DEFINE_SHADER
 * command
=======
 * vmw_cmd_dx_define_shader - Validate SVGA_3D_CMD_DX_DEFINE_SHADER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_define_shader(struct vmw_private *dev_priv,
				    struct vmw_sw_context *sw_context,
				    SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *res;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDefineShader body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	res = vmw_context_cotable(ctx_node->res, SVGA_COTABLE_DXSHADER);
	ret = vmw_cotable_notify(res, cmd->body.shaderId);
	vmw_resource_unreference(&res);
	if (ret)
		return ret;

	return vmw_dx_shader_add(sw_context->man, ctx_node->res,
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	struct vmw_resource *res;
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXDefineShader) =
		container_of(header, typeof(*cmd), header);
	int ret;

	if (!ctx_node)
		return -EINVAL;

	res = vmw_context_cotable(ctx_node->ctx, SVGA_COTABLE_DXSHADER);
	ret = vmw_cotable_notify(res, cmd->body.shaderId);
	if (ret)
		return ret;

	return vmw_dx_shader_add(sw_context->man, ctx_node->ctx,
>>>>>>> upstream/android-13
				 cmd->body.shaderId, cmd->body.type,
				 &sw_context->staged_cmd_res);
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_destroy_shader - Validate an SVGA_3D_CMD_DX_DESTROY_SHADER
 * command
=======
 * vmw_cmd_dx_destroy_shader - Validate SVGA_3D_CMD_DX_DESTROY_SHADER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_destroy_shader(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node = sw_context->dx_ctx_node;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDestroyShader body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	ret = vmw_shader_remove(sw_context->man, cmd->body.shaderId, 0,
				&sw_context->staged_cmd_res);
	if (ret)
		DRM_ERROR("Could not find shader to remove.\n");
=======
	struct vmw_ctx_validation_info *ctx_node = VMW_GET_CTX_NODE(sw_context);
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXDestroyShader) =
		container_of(header, typeof(*cmd), header);
	int ret;

	if (!ctx_node)
		return -EINVAL;

	ret = vmw_shader_remove(sw_context->man, cmd->body.shaderId, 0,
				&sw_context->staged_cmd_res);
>>>>>>> upstream/android-13

	return ret;
}

/**
<<<<<<< HEAD
 * vmw_cmd_dx_bind_shader - Validate an SVGA_3D_CMD_DX_BIND_SHADER
 * command
=======
 * vmw_cmd_dx_bind_shader - Validate SVGA_3D_CMD_DX_BIND_SHADER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_bind_shader(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node;
	struct vmw_resource_val_node *res_node;
	struct vmw_resource *res;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXBindShader body;
	} *cmd = container_of(header, typeof(*cmd), header);
=======
	struct vmw_resource *ctx;
	struct vmw_resource *res;
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXBindShader) =
		container_of(header, typeof(*cmd), header);
>>>>>>> upstream/android-13
	int ret;

	if (cmd->body.cid != SVGA3D_INVALID_ID) {
		ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_context,
<<<<<<< HEAD
					user_context_converter,
					&cmd->body.cid, &ctx_node);
		if (ret)
			return ret;
	} else {
		ctx_node = sw_context->dx_ctx_node;
		if (!ctx_node) {
			DRM_ERROR("DX Context not set.\n");
			return -EINVAL;
		}
	}

	res = vmw_shader_lookup(vmw_context_res_man(ctx_node->res),
				cmd->body.shid, 0);
	if (IS_ERR(res)) {
		DRM_ERROR("Could not find shader to bind.\n");
		return PTR_ERR(res);
	}

	ret = vmw_resource_val_add(sw_context, res, &res_node);
	if (ret) {
		DRM_ERROR("Error creating resource validation node.\n");
		goto out_unref;
	}


	ret = vmw_cmd_res_switch_backup(dev_priv, sw_context, res_node,
					&cmd->body.mobid,
					cmd->body.offsetInBytes);
out_unref:
	vmw_resource_unreference(&res);

	return ret;
}

/**
 * vmw_cmd_dx_genmips - Validate an SVGA_3D_CMD_DX_GENMIPS command
=======
					VMW_RES_DIRTY_SET,
					user_context_converter, &cmd->body.cid,
					&ctx);
		if (ret)
			return ret;
	} else {
		struct vmw_ctx_validation_info *ctx_node =
			VMW_GET_CTX_NODE(sw_context);

		if (!ctx_node)
			return -EINVAL;

		ctx = ctx_node->ctx;
	}

	res = vmw_shader_lookup(vmw_context_res_man(ctx), cmd->body.shid, 0);
	if (IS_ERR(res)) {
		VMW_DEBUG_USER("Could not find shader to bind.\n");
		return PTR_ERR(res);
	}

	ret = vmw_execbuf_res_noctx_val_add(sw_context, res,
					    VMW_RES_DIRTY_NONE);
	if (ret) {
		VMW_DEBUG_USER("Error creating resource validation node.\n");
		return ret;
	}

	return vmw_cmd_res_switch_backup(dev_priv, sw_context, res,
					 &cmd->body.mobid,
					 cmd->body.offsetInBytes);
}

/**
 * vmw_cmd_dx_genmips - Validate SVGA_3D_CMD_DX_GENMIPS command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_genmips(struct vmw_private *dev_priv,
			      struct vmw_sw_context *sw_context,
			      SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXGenMips body;
	} *cmd = container_of(header, typeof(*cmd), header);

	return vmw_view_id_val_add(sw_context, vmw_view_sr,
				   cmd->body.shaderResourceViewId);
}

/**
 * vmw_cmd_dx_transfer_from_buffer -
 * Validate an SVGA_3D_CMD_DX_TRANSFER_FROM_BUFFER command
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXGenMips) =
		container_of(header, typeof(*cmd), header);
	struct vmw_resource *view;
	struct vmw_res_cache_entry *rcache;

	view = vmw_view_id_val_add(sw_context, vmw_view_sr,
				   cmd->body.shaderResourceViewId);
	if (IS_ERR(view))
		return PTR_ERR(view);

	/*
	 * Normally the shader-resource view is not gpu-dirtying, but for
	 * this particular command it is...
	 * So mark the last looked-up surface, which is the surface
	 * the view points to, gpu-dirty.
	 */
	rcache = &sw_context->res_cache[vmw_res_surface];
	vmw_validation_res_set_dirty(sw_context->ctx, rcache->private,
				     VMW_RES_DIRTY_SET);
	return 0;
}

/**
 * vmw_cmd_dx_transfer_from_buffer - Validate
 * SVGA_3D_CMD_DX_TRANSFER_FROM_BUFFER command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_dx_transfer_from_buffer(struct vmw_private *dev_priv,
					   struct vmw_sw_context *sw_context,
					   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXTransferFromBuffer body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				user_surface_converter,
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdDXTransferFromBuffer) =
		container_of(header, typeof(*cmd), header);
	int ret;

	ret = vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				VMW_RES_DIRTY_NONE, user_surface_converter,
>>>>>>> upstream/android-13
				&cmd->body.srcSid, NULL);
	if (ret != 0)
		return ret;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				 user_surface_converter,
=======
				 VMW_RES_DIRTY_SET, user_surface_converter,
>>>>>>> upstream/android-13
				 &cmd->body.destSid, NULL);
}

/**
<<<<<<< HEAD
 * vmw_cmd_intra_surface_copy -
 * Validate an SVGA_3D_CMD_INTRA_SURFACE_COPY command
=======
 * vmw_cmd_intra_surface_copy - Validate SVGA_3D_CMD_INTRA_SURFACE_COPY command
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private struct.
 * @sw_context: The software context being used for this batch.
 * @header: Pointer to the command header in the command stream.
 */
static int vmw_cmd_intra_surface_copy(struct vmw_private *dev_priv,
					   struct vmw_sw_context *sw_context,
					   SVGA3dCmdHeader *header)
{
<<<<<<< HEAD
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdIntraSurfaceCopy body;
	} *cmd = container_of(header, typeof(*cmd), header);
=======
	VMW_DECLARE_CMD_VAR(*cmd, SVGA3dCmdIntraSurfaceCopy) =
		container_of(header, typeof(*cmd), header);
>>>>>>> upstream/android-13

	if (!(dev_priv->capabilities2 & SVGA_CAP2_INTRA_SURFACE_COPY))
		return -EINVAL;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
<<<<<<< HEAD
				user_surface_converter,
				&cmd->body.surface.sid, NULL);
}

=======
				 VMW_RES_DIRTY_SET, user_surface_converter,
				 &cmd->body.surface.sid, NULL);
}

static int vmw_cmd_sm5(struct vmw_private *dev_priv,
		       struct vmw_sw_context *sw_context,
		       SVGA3dCmdHeader *header)
{
	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	return 0;
}

static int vmw_cmd_sm5_view_define(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	return vmw_cmd_dx_view_define(dev_priv, sw_context, header);
}

static int vmw_cmd_sm5_view_remove(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	return vmw_cmd_dx_view_remove(dev_priv, sw_context, header);
}

static int vmw_cmd_clear_uav_uint(struct vmw_private *dev_priv,
				  struct vmw_sw_context *sw_context,
				  SVGA3dCmdHeader *header)
{
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXClearUAViewUint body;
	} *cmd = container_of(header, typeof(*cmd), header);
	struct vmw_resource *ret;

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	ret = vmw_view_id_val_add(sw_context, vmw_view_ua,
				  cmd->body.uaViewId);

	return PTR_ERR_OR_ZERO(ret);
}

static int vmw_cmd_clear_uav_float(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   SVGA3dCmdHeader *header)
{
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXClearUAViewFloat body;
	} *cmd = container_of(header, typeof(*cmd), header);
	struct vmw_resource *ret;

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	ret = vmw_view_id_val_add(sw_context, vmw_view_ua,
				  cmd->body.uaViewId);

	return PTR_ERR_OR_ZERO(ret);
}

static int vmw_cmd_set_uav(struct vmw_private *dev_priv,
			   struct vmw_sw_context *sw_context,
			   SVGA3dCmdHeader *header)
{
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetUAViews body;
	} *cmd = container_of(header, typeof(*cmd), header);
	u32 num_uav = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dUAViewId);
	int ret;

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	if (num_uav > SVGA3D_MAX_UAVIEWS) {
		VMW_DEBUG_USER("Invalid UAV binding.\n");
		return -EINVAL;
	}

	ret = vmw_view_bindings_add(sw_context, vmw_view_ua,
				    vmw_ctx_binding_uav, 0, (void *)&cmd[1],
				    num_uav, 0);
	if (ret)
		return ret;

	vmw_binding_add_uav_index(sw_context->dx_ctx_node->staged, 0,
					 cmd->body.uavSpliceIndex);

	return ret;
}

static int vmw_cmd_set_cs_uav(struct vmw_private *dev_priv,
			      struct vmw_sw_context *sw_context,
			      SVGA3dCmdHeader *header)
{
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetCSUAViews body;
	} *cmd = container_of(header, typeof(*cmd), header);
	u32 num_uav = (cmd->header.size - sizeof(cmd->body)) /
		sizeof(SVGA3dUAViewId);
	int ret;

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	if (num_uav > SVGA3D_MAX_UAVIEWS) {
		VMW_DEBUG_USER("Invalid UAV binding.\n");
		return -EINVAL;
	}

	ret = vmw_view_bindings_add(sw_context, vmw_view_ua,
				    vmw_ctx_binding_cs_uav, 0, (void *)&cmd[1],
				    num_uav, 0);
	if (ret)
		return ret;

	vmw_binding_add_uav_index(sw_context->dx_ctx_node->staged, 1,
				  cmd->body.startIndex);

	return ret;
}

static int vmw_cmd_dx_define_streamoutput(struct vmw_private *dev_priv,
					  struct vmw_sw_context *sw_context,
					  SVGA3dCmdHeader *header)
{
	struct vmw_ctx_validation_info *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *res;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDefineStreamOutputWithMob body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	res = vmw_context_cotable(ctx_node->ctx, SVGA_COTABLE_STREAMOUTPUT);
	ret = vmw_cotable_notify(res, cmd->body.soid);
	if (ret)
		return ret;

	return vmw_dx_streamoutput_add(sw_context->man, ctx_node->ctx,
				       cmd->body.soid,
				       &sw_context->staged_cmd_res);
}

static int vmw_cmd_dx_destroy_streamoutput(struct vmw_private *dev_priv,
					   struct vmw_sw_context *sw_context,
					   SVGA3dCmdHeader *header)
{
	struct vmw_ctx_validation_info *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *res;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDestroyStreamOutput body;
	} *cmd = container_of(header, typeof(*cmd), header);

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	/*
	 * When device does not support SM5 then streamoutput with mob command is
	 * not available to user-space. Simply return in this case.
	 */
	if (!has_sm5_context(dev_priv))
		return 0;

	/*
	 * With SM5 capable device if lookup fails then user-space probably used
	 * old streamoutput define command. Return without an error.
	 */
	res = vmw_dx_streamoutput_lookup(vmw_context_res_man(ctx_node->ctx),
					 cmd->body.soid);
	if (IS_ERR(res))
		return 0;

	return vmw_dx_streamoutput_remove(sw_context->man, cmd->body.soid,
					  &sw_context->staged_cmd_res);
}

static int vmw_cmd_dx_bind_streamoutput(struct vmw_private *dev_priv,
					struct vmw_sw_context *sw_context,
					SVGA3dCmdHeader *header)
{
	struct vmw_ctx_validation_info *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *res;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXBindStreamOutput body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	res = vmw_dx_streamoutput_lookup(vmw_context_res_man(ctx_node->ctx),
					 cmd->body.soid);
	if (IS_ERR(res)) {
		DRM_ERROR("Could not find streamoutput to bind.\n");
		return PTR_ERR(res);
	}

	vmw_dx_streamoutput_set_size(res, cmd->body.sizeInBytes);

	ret = vmw_execbuf_res_noctx_val_add(sw_context, res,
					    VMW_RES_DIRTY_NONE);
	if (ret) {
		DRM_ERROR("Error creating resource validation node.\n");
		return ret;
	}

	return vmw_cmd_res_switch_backup(dev_priv, sw_context, res,
					 &cmd->body.mobid,
					 cmd->body.offsetInBytes);
}

static int vmw_cmd_dx_set_streamoutput(struct vmw_private *dev_priv,
				       struct vmw_sw_context *sw_context,
				       SVGA3dCmdHeader *header)
{
	struct vmw_ctx_validation_info *ctx_node = sw_context->dx_ctx_node;
	struct vmw_resource *res;
	struct vmw_ctx_bindinfo_so binding;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXSetStreamOutput body;
	} *cmd = container_of(header, typeof(*cmd), header);
	int ret;

	if (!ctx_node) {
		DRM_ERROR("DX Context not set.\n");
		return -EINVAL;
	}

	if (cmd->body.soid == SVGA3D_INVALID_ID)
		return 0;

	/*
	 * When device does not support SM5 then streamoutput with mob command is
	 * not available to user-space. Simply return in this case.
	 */
	if (!has_sm5_context(dev_priv))
		return 0;

	/*
	 * With SM5 capable device if lookup fails then user-space probably used
	 * old streamoutput define command. Return without an error.
	 */
	res = vmw_dx_streamoutput_lookup(vmw_context_res_man(ctx_node->ctx),
					 cmd->body.soid);
	if (IS_ERR(res)) {
		return 0;
	}

	ret = vmw_execbuf_res_noctx_val_add(sw_context, res,
					    VMW_RES_DIRTY_NONE);
	if (ret) {
		DRM_ERROR("Error creating resource validation node.\n");
		return ret;
	}

	binding.bi.ctx = ctx_node->ctx;
	binding.bi.res = res;
	binding.bi.bt = vmw_ctx_binding_so;
	binding.slot = 0; /* Only one SO set to context at a time. */

	vmw_binding_add(sw_context->dx_ctx_node->staged, &binding.bi, 0,
			binding.slot);

	return ret;
}

static int vmw_cmd_indexed_instanced_indirect(struct vmw_private *dev_priv,
					      struct vmw_sw_context *sw_context,
					      SVGA3dCmdHeader *header)
{
	struct vmw_draw_indexed_instanced_indirect_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDrawIndexedInstancedIndirect body;
	} *cmd = container_of(header, typeof(*cmd), header);

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
				 &cmd->body.argsBufferSid, NULL);
}

static int vmw_cmd_instanced_indirect(struct vmw_private *dev_priv,
				      struct vmw_sw_context *sw_context,
				      SVGA3dCmdHeader *header)
{
	struct vmw_draw_instanced_indirect_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDrawInstancedIndirect body;
	} *cmd = container_of(header, typeof(*cmd), header);

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
				 &cmd->body.argsBufferSid, NULL);
}

static int vmw_cmd_dispatch_indirect(struct vmw_private *dev_priv,
				     struct vmw_sw_context *sw_context,
				     SVGA3dCmdHeader *header)
{
	struct vmw_dispatch_indirect_cmd {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXDispatchIndirect body;
	} *cmd = container_of(header, typeof(*cmd), header);

	if (!has_sm5_context(dev_priv))
		return -EINVAL;

	return vmw_cmd_res_check(dev_priv, sw_context, vmw_res_surface,
				 VMW_RES_DIRTY_NONE, user_surface_converter,
				 &cmd->body.argsBufferSid, NULL);
}
>>>>>>> upstream/android-13

static int vmw_cmd_check_not_3d(struct vmw_private *dev_priv,
				struct vmw_sw_context *sw_context,
				void *buf, uint32_t *size)
{
	uint32_t size_remaining = *size;
	uint32_t cmd_id;

	cmd_id = ((uint32_t *)buf)[0];
	switch (cmd_id) {
	case SVGA_CMD_UPDATE:
		*size = sizeof(uint32_t) + sizeof(SVGAFifoCmdUpdate);
		break;
	case SVGA_CMD_DEFINE_GMRFB:
		*size = sizeof(uint32_t) + sizeof(SVGAFifoCmdDefineGMRFB);
		break;
	case SVGA_CMD_BLIT_GMRFB_TO_SCREEN:
		*size = sizeof(uint32_t) + sizeof(SVGAFifoCmdBlitGMRFBToScreen);
		break;
	case SVGA_CMD_BLIT_SCREEN_TO_GMRFB:
		*size = sizeof(uint32_t) + sizeof(SVGAFifoCmdBlitGMRFBToScreen);
		break;
	default:
<<<<<<< HEAD
		DRM_ERROR("Unsupported SVGA command: %u.\n", cmd_id);
=======
		VMW_DEBUG_USER("Unsupported SVGA command: %u.\n", cmd_id);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	if (*size > size_remaining) {
<<<<<<< HEAD
		DRM_ERROR("Invalid SVGA command (size mismatch):"
			  " %u.\n", cmd_id);
=======
		VMW_DEBUG_USER("Invalid SVGA command (size mismatch): %u.\n",
			       cmd_id);
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	if (unlikely(!sw_context->kernel)) {
<<<<<<< HEAD
		DRM_ERROR("Kernel only SVGA command: %u.\n", cmd_id);
=======
		VMW_DEBUG_USER("Kernel only SVGA command: %u.\n", cmd_id);
>>>>>>> upstream/android-13
		return -EPERM;
	}

	if (cmd_id == SVGA_CMD_DEFINE_GMRFB)
		return vmw_cmd_check_define_gmrfb(dev_priv, sw_context, buf);

	return 0;
}

static const struct vmw_cmd_entry vmw_cmd_entries[SVGA_3D_CMD_MAX] = {
	VMW_CMD_DEF(SVGA_3D_CMD_SURFACE_DEFINE, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SURFACE_DESTROY, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SURFACE_COPY, &vmw_cmd_surface_copy_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SURFACE_STRETCHBLT, &vmw_cmd_stretch_blt_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SURFACE_DMA, &vmw_cmd_dma,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_CONTEXT_DEFINE, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_CONTEXT_DESTROY, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETTRANSFORM, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETZRANGE, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETRENDERSTATE, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETRENDERTARGET,
		    &vmw_cmd_set_render_target_check, true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETTEXTURESTATE, &vmw_cmd_tex_state,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETMATERIAL, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETLIGHTDATA, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETLIGHTENABLED, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETVIEWPORT, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETCLIPPLANE, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_CLEAR, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_PRESENT, &vmw_cmd_present_check,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SHADER_DEFINE, &vmw_cmd_shader_define,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SHADER_DESTROY, &vmw_cmd_shader_destroy,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SET_SHADER, &vmw_cmd_set_shader,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SET_SHADER_CONST, &vmw_cmd_set_shader_const,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DRAW_PRIMITIVES, &vmw_cmd_draw,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SETSCISSORRECT, &vmw_cmd_cid_check,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_BEGIN_QUERY, &vmw_cmd_begin_query,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_END_QUERY, &vmw_cmd_end_query,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_WAIT_FOR_QUERY, &vmw_cmd_wait_query,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_PRESENT_READBACK, &vmw_cmd_ok,
		    true, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_BLIT_SURFACE_TO_SCREEN,
		    &vmw_cmd_blt_surf_screen_check, false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SURFACE_DEFINE_V2, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_GENERATE_MIPMAPS, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_ACTIVATE_SURFACE, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEACTIVATE_SURFACE, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_SCREEN_DMA, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD1, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD2, &vmw_cmd_invalid,
		    false, false, false),
<<<<<<< HEAD
	VMW_CMD_DEF(SVGA_3D_CMD_LOGICOPS_BITBLT, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_LOGICOPS_TRANSBLT, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_LOGICOPS_STRETCHBLT, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_LOGICOPS_COLORFILL, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_LOGICOPS_ALPHABLEND, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_LOGICOPS_CLEARTYPEBLEND, &vmw_cmd_invalid,
		    false, false, false),
=======
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD12, &vmw_cmd_invalid, false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD13, &vmw_cmd_invalid, false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD14, &vmw_cmd_invalid, false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD15, &vmw_cmd_invalid, false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD16, &vmw_cmd_invalid, false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_DEAD17, &vmw_cmd_invalid, false, false, false),
>>>>>>> upstream/android-13
	VMW_CMD_DEF(SVGA_3D_CMD_SET_OTABLE_BASE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_READBACK_OTABLE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DEFINE_GB_MOB, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DESTROY_GB_MOB, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_REDEFINE_GB_MOB64, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_UPDATE_GB_MOB_MAPPING, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DEFINE_GB_SURFACE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DESTROY_GB_SURFACE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_BIND_GB_SURFACE, &vmw_cmd_bind_gb_surface,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_COND_BIND_GB_SURFACE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_UPDATE_GB_IMAGE, &vmw_cmd_update_gb_image,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_UPDATE_GB_SURFACE,
		    &vmw_cmd_update_gb_surface, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_READBACK_GB_IMAGE,
		    &vmw_cmd_readback_gb_image, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_READBACK_GB_SURFACE,
		    &vmw_cmd_readback_gb_surface, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_INVALIDATE_GB_IMAGE,
		    &vmw_cmd_invalidate_gb_image, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_INVALIDATE_GB_SURFACE,
		    &vmw_cmd_invalidate_gb_surface, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DEFINE_GB_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DESTROY_GB_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_BIND_GB_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_READBACK_GB_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_INVALIDATE_GB_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DEFINE_GB_SHADER, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_BIND_GB_SHADER, &vmw_cmd_bind_gb_shader,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DESTROY_GB_SHADER, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_SET_OTABLE_BASE64, &vmw_cmd_invalid,
		    false, false, false),
	VMW_CMD_DEF(SVGA_3D_CMD_BEGIN_GB_QUERY, &vmw_cmd_begin_gb_query,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_END_GB_QUERY, &vmw_cmd_end_gb_query,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_WAIT_FOR_GB_QUERY, &vmw_cmd_wait_gb_query,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_NOP, &vmw_cmd_ok,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_NOP_ERROR, &vmw_cmd_ok,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_ENABLE_GART, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DISABLE_GART, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_MAP_MOB_INTO_GART, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_UNMAP_GART_RANGE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DEFINE_GB_SCREENTARGET, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DESTROY_GB_SCREENTARGET, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_BIND_GB_SCREENTARGET, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_UPDATE_GB_SCREENTARGET, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_READBACK_GB_IMAGE_PARTIAL, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_INVALIDATE_GB_IMAGE_PARTIAL, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_SET_GB_SHADERCONSTS_INLINE, &vmw_cmd_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_GB_SCREEN_DMA, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_BIND_GB_SURFACE_WITH_PITCH, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_GB_MOB_FENCE, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DEFINE_GB_SURFACE_V2, &vmw_cmd_invalid,
		    false, false, true),

<<<<<<< HEAD
	/*
	 * DX commands
	 */
=======
	/* SM commands */
>>>>>>> upstream/android-13
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_BIND_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_READBACK_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_INVALIDATE_CONTEXT, &vmw_cmd_invalid,
		    false, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_SINGLE_CONSTANT_BUFFER,
		    &vmw_cmd_dx_set_single_constant_buffer, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_SHADER_RESOURCES,
		    &vmw_cmd_dx_set_shader_res, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_SHADER, &vmw_cmd_dx_set_shader,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_SAMPLERS, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW_INDEXED, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW_INSTANCED, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW_INDEXED_INSTANCED,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW_AUTO, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_VERTEX_BUFFERS,
		    &vmw_cmd_dx_set_vertex_buffers, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_INDEX_BUFFER,
		    &vmw_cmd_dx_set_index_buffer, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_RENDERTARGETS,
		    &vmw_cmd_dx_set_rendertargets, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_BLEND_STATE, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_DEPTHSTENCIL_STATE,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_RASTERIZER_STATE,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_QUERY, &vmw_cmd_dx_define_query,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_QUERY, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_BIND_QUERY, &vmw_cmd_dx_bind_query,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_QUERY_OFFSET,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_BEGIN_QUERY, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_END_QUERY, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_READBACK_QUERY, &vmw_cmd_invalid,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_PREDICATION, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_VIEWPORTS, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_SCISSORRECTS, &vmw_cmd_dx_cid_check,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_CLEAR_RENDERTARGET_VIEW,
		    &vmw_cmd_dx_clear_rendertarget_view, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_CLEAR_DEPTHSTENCIL_VIEW,
		    &vmw_cmd_dx_clear_depthstencil_view, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_PRED_COPY, &vmw_cmd_invalid,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_GENMIPS, &vmw_cmd_dx_genmips,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_UPDATE_SUBRESOURCE,
		    &vmw_cmd_dx_check_subresource, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_READBACK_SUBRESOURCE,
		    &vmw_cmd_dx_check_subresource, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_INVALIDATE_SUBRESOURCE,
		    &vmw_cmd_dx_check_subresource, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_SHADERRESOURCE_VIEW,
		    &vmw_cmd_dx_view_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_SHADERRESOURCE_VIEW,
		    &vmw_cmd_dx_view_remove, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_RENDERTARGET_VIEW,
		    &vmw_cmd_dx_view_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_RENDERTARGET_VIEW,
		    &vmw_cmd_dx_view_remove, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_DEPTHSTENCIL_VIEW,
		    &vmw_cmd_dx_view_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_DEPTHSTENCIL_VIEW,
		    &vmw_cmd_dx_view_remove, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_ELEMENTLAYOUT,
		    &vmw_cmd_dx_so_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_ELEMENTLAYOUT,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_BLEND_STATE,
		    &vmw_cmd_dx_so_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_BLEND_STATE,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_DEPTHSTENCIL_STATE,
		    &vmw_cmd_dx_so_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_DEPTHSTENCIL_STATE,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_RASTERIZER_STATE,
		    &vmw_cmd_dx_so_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_RASTERIZER_STATE,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_SAMPLER_STATE,
		    &vmw_cmd_dx_so_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_SAMPLER_STATE,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_SHADER,
		    &vmw_cmd_dx_define_shader, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_SHADER,
		    &vmw_cmd_dx_destroy_shader, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_BIND_SHADER,
		    &vmw_cmd_dx_bind_shader, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_STREAMOUTPUT,
		    &vmw_cmd_dx_so_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_STREAMOUTPUT,
<<<<<<< HEAD
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_STREAMOUTPUT, &vmw_cmd_dx_cid_check,
		    true, false, true),
=======
		    &vmw_cmd_dx_destroy_streamoutput, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_STREAMOUTPUT,
		    &vmw_cmd_dx_set_streamoutput, true, false, true),
>>>>>>> upstream/android-13
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_SOTARGETS,
		    &vmw_cmd_dx_set_so_targets, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_INPUT_LAYOUT,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_TOPOLOGY,
		    &vmw_cmd_dx_cid_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_BUFFER_COPY,
		    &vmw_cmd_buffer_copy_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_PRED_COPY_REGION,
		    &vmw_cmd_pred_copy_check, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_TRANSFER_FROM_BUFFER,
		    &vmw_cmd_dx_transfer_from_buffer,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_INTRA_SURFACE_COPY, &vmw_cmd_intra_surface_copy,
		    true, false, true),
<<<<<<< HEAD
=======

	/*
	 * SM5 commands
	 */
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_UA_VIEW, &vmw_cmd_sm5_view_define,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DESTROY_UA_VIEW, &vmw_cmd_sm5_view_remove,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_CLEAR_UA_VIEW_UINT, &vmw_cmd_clear_uav_uint,
		    true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_CLEAR_UA_VIEW_FLOAT,
		    &vmw_cmd_clear_uav_float, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_COPY_STRUCTURE_COUNT, &vmw_cmd_invalid, true,
		    false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_UA_VIEWS, &vmw_cmd_set_uav, true, false,
		    true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW_INDEXED_INSTANCED_INDIRECT,
		    &vmw_cmd_indexed_instanced_indirect, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DRAW_INSTANCED_INDIRECT,
		    &vmw_cmd_instanced_indirect, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DISPATCH, &vmw_cmd_sm5, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DISPATCH_INDIRECT,
		    &vmw_cmd_dispatch_indirect, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_SET_CS_UA_VIEWS, &vmw_cmd_set_cs_uav, true,
		    false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_DEPTHSTENCIL_VIEW_V2,
		    &vmw_cmd_sm5_view_define, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_DEFINE_STREAMOUTPUT_WITH_MOB,
		    &vmw_cmd_dx_define_streamoutput, true, false, true),
	VMW_CMD_DEF(SVGA_3D_CMD_DX_BIND_STREAMOUTPUT,
		    &vmw_cmd_dx_bind_streamoutput, true, false, true),
>>>>>>> upstream/android-13
};

bool vmw_cmd_describe(const void *buf, u32 *size, char const **cmd)
{
	u32 cmd_id = ((u32 *) buf)[0];

	if (cmd_id >= SVGA_CMD_MAX) {
		SVGA3dCmdHeader *header = (SVGA3dCmdHeader *) buf;
		const struct vmw_cmd_entry *entry;

		*size = header->size + sizeof(SVGA3dCmdHeader);
		cmd_id = header->id;
		if (cmd_id >= SVGA_3D_CMD_MAX)
			return false;

		cmd_id -= SVGA_3D_CMD_BASE;
		entry = &vmw_cmd_entries[cmd_id];
		*cmd = entry->cmd_name;
		return true;
	}

	switch (cmd_id) {
	case SVGA_CMD_UPDATE:
		*cmd = "SVGA_CMD_UPDATE";
		*size = sizeof(u32) + sizeof(SVGAFifoCmdUpdate);
		break;
	case SVGA_CMD_DEFINE_GMRFB:
		*cmd = "SVGA_CMD_DEFINE_GMRFB";
		*size = sizeof(u32) + sizeof(SVGAFifoCmdDefineGMRFB);
		break;
	case SVGA_CMD_BLIT_GMRFB_TO_SCREEN:
		*cmd = "SVGA_CMD_BLIT_GMRFB_TO_SCREEN";
		*size = sizeof(u32) + sizeof(SVGAFifoCmdBlitGMRFBToScreen);
		break;
	case SVGA_CMD_BLIT_SCREEN_TO_GMRFB:
		*cmd = "SVGA_CMD_BLIT_SCREEN_TO_GMRFB";
		*size = sizeof(u32) + sizeof(SVGAFifoCmdBlitGMRFBToScreen);
		break;
	default:
		*cmd = "UNKNOWN";
		*size = 0;
		return false;
	}

	return true;
}

static int vmw_cmd_check(struct vmw_private *dev_priv,
<<<<<<< HEAD
			 struct vmw_sw_context *sw_context,
			 void *buf, uint32_t *size)
=======
			 struct vmw_sw_context *sw_context, void *buf,
			 uint32_t *size)
>>>>>>> upstream/android-13
{
	uint32_t cmd_id;
	uint32_t size_remaining = *size;
	SVGA3dCmdHeader *header = (SVGA3dCmdHeader *) buf;
	int ret;
	const struct vmw_cmd_entry *entry;
	bool gb = dev_priv->capabilities & SVGA_CAP_GBOBJECTS;

	cmd_id = ((uint32_t *)buf)[0];
	/* Handle any none 3D commands */
	if (unlikely(cmd_id < SVGA_CMD_MAX))
		return vmw_cmd_check_not_3d(dev_priv, sw_context, buf, size);


	cmd_id = header->id;
	*size = header->size + sizeof(SVGA3dCmdHeader);

	cmd_id -= SVGA_3D_CMD_BASE;
	if (unlikely(*size > size_remaining))
		goto out_invalid;

	if (unlikely(cmd_id >= SVGA_3D_CMD_MAX - SVGA_3D_CMD_BASE))
		goto out_invalid;

	entry = &vmw_cmd_entries[cmd_id];
	if (unlikely(!entry->func))
		goto out_invalid;

	if (unlikely(!entry->user_allow && !sw_context->kernel))
		goto out_privileged;

	if (unlikely(entry->gb_disable && gb))
		goto out_old;

	if (unlikely(entry->gb_enable && !gb))
		goto out_new;

	ret = entry->func(dev_priv, sw_context, header);
<<<<<<< HEAD
	if (unlikely(ret != 0))
		goto out_invalid;

	return 0;
out_invalid:
	DRM_ERROR("Invalid SVGA3D command: %d\n",
		  cmd_id + SVGA_3D_CMD_BASE);
	return -EINVAL;
out_privileged:
	DRM_ERROR("Privileged SVGA3D command: %d\n",
		  cmd_id + SVGA_3D_CMD_BASE);
	return -EPERM;
out_old:
	DRM_ERROR("Deprecated (disallowed) SVGA3D command: %d\n",
		  cmd_id + SVGA_3D_CMD_BASE);
	return -EINVAL;
out_new:
	DRM_ERROR("SVGA3D command: %d not supported by virtual hardware.\n",
		  cmd_id + SVGA_3D_CMD_BASE);
=======
	if (unlikely(ret != 0)) {
		VMW_DEBUG_USER("SVGA3D command: %d failed with error %d\n",
			       cmd_id + SVGA_3D_CMD_BASE, ret);
		return ret;
	}

	return 0;
out_invalid:
	VMW_DEBUG_USER("Invalid SVGA3D command: %d\n",
		       cmd_id + SVGA_3D_CMD_BASE);
	return -EINVAL;
out_privileged:
	VMW_DEBUG_USER("Privileged SVGA3D command: %d\n",
		       cmd_id + SVGA_3D_CMD_BASE);
	return -EPERM;
out_old:
	VMW_DEBUG_USER("Deprecated (disallowed) SVGA3D command: %d\n",
		       cmd_id + SVGA_3D_CMD_BASE);
	return -EINVAL;
out_new:
	VMW_DEBUG_USER("SVGA3D command: %d not supported by virtual device.\n",
		       cmd_id + SVGA_3D_CMD_BASE);
>>>>>>> upstream/android-13
	return -EINVAL;
}

static int vmw_cmd_check_all(struct vmw_private *dev_priv,
<<<<<<< HEAD
			     struct vmw_sw_context *sw_context,
			     void *buf,
=======
			     struct vmw_sw_context *sw_context, void *buf,
>>>>>>> upstream/android-13
			     uint32_t size)
{
	int32_t cur_size = size;
	int ret;

	sw_context->buf_start = buf;

	while (cur_size > 0) {
		size = cur_size;
		ret = vmw_cmd_check(dev_priv, sw_context, buf, &size);
		if (unlikely(ret != 0))
			return ret;
		buf = (void *)((unsigned long) buf + size);
		cur_size -= size;
	}

	if (unlikely(cur_size != 0)) {
<<<<<<< HEAD
		DRM_ERROR("Command verifier out of sync.\n");
=======
		VMW_DEBUG_USER("Command verifier out of sync.\n");
>>>>>>> upstream/android-13
		return -EINVAL;
	}

	return 0;
}

static void vmw_free_relocations(struct vmw_sw_context *sw_context)
{
<<<<<<< HEAD
	sw_context->cur_reloc = 0;
=======
	/* Memory is validation context memory, so no need to free it */
	INIT_LIST_HEAD(&sw_context->bo_relocations);
>>>>>>> upstream/android-13
}

static void vmw_apply_relocations(struct vmw_sw_context *sw_context)
{
<<<<<<< HEAD
	uint32_t i;
	struct vmw_relocation *reloc;
	struct ttm_validate_buffer *validate;
	struct ttm_buffer_object *bo;

	for (i = 0; i < sw_context->cur_reloc; ++i) {
		reloc = &sw_context->relocs[i];
		validate = &sw_context->val_bufs[reloc->index].base;
		bo = validate->bo;
		switch (bo->mem.mem_type) {
		case TTM_PL_VRAM:
			reloc->location->offset += bo->offset;
			reloc->location->gmrId = SVGA_GMR_FRAMEBUFFER;
			break;
		case VMW_PL_GMR:
			reloc->location->gmrId = bo->mem.start;
			break;
		case VMW_PL_MOB:
			*reloc->mob_loc = bo->mem.start;
=======
	struct vmw_relocation *reloc;
	struct ttm_buffer_object *bo;

	list_for_each_entry(reloc, &sw_context->bo_relocations, head) {
		bo = &reloc->vbo->base;
		switch (bo->resource->mem_type) {
		case TTM_PL_VRAM:
			reloc->location->offset += bo->resource->start << PAGE_SHIFT;
			reloc->location->gmrId = SVGA_GMR_FRAMEBUFFER;
			break;
		case VMW_PL_GMR:
			reloc->location->gmrId = bo->resource->start;
			break;
		case VMW_PL_MOB:
			*reloc->mob_loc = bo->resource->start;
>>>>>>> upstream/android-13
			break;
		default:
			BUG();
		}
	}
	vmw_free_relocations(sw_context);
}

<<<<<<< HEAD
/**
 * vmw_resource_list_unrefererence - Free up a resource list and unreference
 * all resources referenced by it.
 *
 * @list: The resource list.
 */
static void vmw_resource_list_unreference(struct vmw_sw_context *sw_context,
					  struct list_head *list)
{
	struct vmw_resource_val_node *val, *val_next;

	/*
	 * Drop references to resources held during command submission.
	 */

	list_for_each_entry_safe(val, val_next, list, head) {
		list_del_init(&val->head);
		vmw_resource_unreference(&val->res);

		if (val->staged_bindings) {
			if (val->staged_bindings != sw_context->staged_bindings)
				vmw_binding_state_free(val->staged_bindings);
			else
				sw_context->staged_bindings_inuse = false;
			val->staged_bindings = NULL;
		}

		kfree(val);
	}
}

static void vmw_clear_validations(struct vmw_sw_context *sw_context)
{
	struct vmw_validate_buffer *entry, *next;
	struct vmw_resource_val_node *val;

	/*
	 * Drop references to DMA buffers held during command submission.
	 */
	list_for_each_entry_safe(entry, next, &sw_context->validate_nodes,
				 base.head) {
		list_del(&entry->base.head);
		ttm_bo_unref(&entry->base.bo);
		(void) drm_ht_remove_item(&sw_context->res_ht, &entry->hash);
		sw_context->cur_val_buf--;
	}
	BUG_ON(sw_context->cur_val_buf != 0);

	list_for_each_entry(val, &sw_context->resource_list, head)
		(void) drm_ht_remove_item(&sw_context->res_ht, &val->hash);
}

int vmw_validate_single_buffer(struct vmw_private *dev_priv,
			       struct ttm_buffer_object *bo,
			       bool interruptible,
			       bool validate_as_mob)
{
	struct vmw_buffer_object *vbo =
		container_of(bo, struct vmw_buffer_object, base);
	struct ttm_operation_ctx ctx = { interruptible, false };
	int ret;

	if (vbo->pin_count > 0)
		return 0;

	if (validate_as_mob)
		return ttm_bo_validate(bo, &vmw_mob_placement, &ctx);

	/**
	 * Put BO in VRAM if there is space, otherwise as a GMR.
	 * If there is no space in VRAM and GMR ids are all used up,
	 * start evicting GMRs to make room. If the DMA buffer can't be
	 * used as a GMR, this will return -ENOMEM.
	 */

	ret = ttm_bo_validate(bo, &vmw_vram_gmr_placement, &ctx);
	if (likely(ret == 0 || ret == -ERESTARTSYS))
		return ret;

	/**
	 * If that failed, try VRAM again, this time evicting
	 * previous contents.
	 */

	ret = ttm_bo_validate(bo, &vmw_vram_placement, &ctx);
	return ret;
}

static int vmw_validate_buffers(struct vmw_private *dev_priv,
				struct vmw_sw_context *sw_context)
{
	struct vmw_validate_buffer *entry;
	int ret;

	list_for_each_entry(entry, &sw_context->validate_nodes, base.head) {
		ret = vmw_validate_single_buffer(dev_priv, entry->base.bo,
						 true,
						 entry->validate_as_mob);
		if (unlikely(ret != 0))
			return ret;
	}
	return 0;
}

=======
>>>>>>> upstream/android-13
static int vmw_resize_cmd_bounce(struct vmw_sw_context *sw_context,
				 uint32_t size)
{
	if (likely(sw_context->cmd_bounce_size >= size))
		return 0;

	if (sw_context->cmd_bounce_size == 0)
		sw_context->cmd_bounce_size = VMWGFX_CMD_BOUNCE_INIT_SIZE;

	while (sw_context->cmd_bounce_size < size) {
		sw_context->cmd_bounce_size =
			PAGE_ALIGN(sw_context->cmd_bounce_size +
				   (sw_context->cmd_bounce_size >> 1));
	}

	vfree(sw_context->cmd_bounce);
	sw_context->cmd_bounce = vmalloc(sw_context->cmd_bounce_size);

	if (sw_context->cmd_bounce == NULL) {
<<<<<<< HEAD
		DRM_ERROR("Failed to allocate command bounce buffer.\n");
=======
		VMW_DEBUG_USER("Failed to allocate command bounce buffer.\n");
>>>>>>> upstream/android-13
		sw_context->cmd_bounce_size = 0;
		return -ENOMEM;
	}

	return 0;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> upstream/android-13
 * vmw_execbuf_fence_commands - create and submit a command stream fence
 *
 * Creates a fence object and submits a command stream marker.
 * If this fails for some reason, We sync the fifo and return NULL.
 * It is then safe to fence buffers with a NULL pointer.
 *
<<<<<<< HEAD
 * If @p_handle is not NULL @file_priv must also not be NULL. Creates
 * a userspace handle if @p_handle is not NULL, otherwise not.
=======
 * If @p_handle is not NULL @file_priv must also not be NULL. Creates a
 * userspace handle if @p_handle is not NULL, otherwise not.
>>>>>>> upstream/android-13
 */

int vmw_execbuf_fence_commands(struct drm_file *file_priv,
			       struct vmw_private *dev_priv,
			       struct vmw_fence_obj **p_fence,
			       uint32_t *p_handle)
{
	uint32_t sequence;
	int ret;
	bool synced = false;

	/* p_handle implies file_priv. */
	BUG_ON(p_handle != NULL && file_priv == NULL);

<<<<<<< HEAD
	ret = vmw_fifo_send_fence(dev_priv, &sequence);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Fence submission error. Syncing.\n");
=======
	ret = vmw_cmd_send_fence(dev_priv, &sequence);
	if (unlikely(ret != 0)) {
		VMW_DEBUG_USER("Fence submission error. Syncing.\n");
>>>>>>> upstream/android-13
		synced = true;
	}

	if (p_handle != NULL)
		ret = vmw_user_fence_create(file_priv, dev_priv->fman,
					    sequence, p_fence, p_handle);
	else
		ret = vmw_fence_create(dev_priv->fman, sequence, p_fence);

	if (unlikely(ret != 0 && !synced)) {
<<<<<<< HEAD
		(void) vmw_fallback_wait(dev_priv, false, false,
					 sequence, false,
					 VMW_FENCE_WAIT_TIMEOUT);
=======
		(void) vmw_fallback_wait(dev_priv, false, false, sequence,
					 false, VMW_FENCE_WAIT_TIMEOUT);
>>>>>>> upstream/android-13
		*p_fence = NULL;
	}

	return ret;
}

/**
<<<<<<< HEAD
 * vmw_execbuf_copy_fence_user - copy fence object information to
 * user-space.
=======
 * vmw_execbuf_copy_fence_user - copy fence object information to user-space.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a vmw_private struct.
 * @vmw_fp: Pointer to the struct vmw_fpriv representing the calling file.
 * @ret: Return value from fence object creation.
<<<<<<< HEAD
 * @user_fence_rep: User space address of a struct drm_vmw_fence_rep to
 * which the information should be copied.
=======
 * @user_fence_rep: User space address of a struct drm_vmw_fence_rep to which
 * the information should be copied.
>>>>>>> upstream/android-13
 * @fence: Pointer to the fenc object.
 * @fence_handle: User-space fence handle.
 * @out_fence_fd: exported file descriptor for the fence.  -1 if not used
 * @sync_file:  Only used to clean up in case of an error in this function.
 *
<<<<<<< HEAD
 * This function copies fence information to user-space. If copying fails,
 * The user-space struct drm_vmw_fence_rep::error member is hopefully
 * left untouched, and if it's preloaded with an -EFAULT by user-space,
 * the error will hopefully be detected.
 * Also if copying fails, user-space will be unable to signal the fence
 * object so we wait for it immediately, and then unreference the
 * user-space reference.
 */
void
vmw_execbuf_copy_fence_user(struct vmw_private *dev_priv,
			    struct vmw_fpriv *vmw_fp,
			    int ret,
			    struct drm_vmw_fence_rep __user *user_fence_rep,
			    struct vmw_fence_obj *fence,
			    uint32_t fence_handle,
			    int32_t out_fence_fd,
			    struct sync_file *sync_file)
=======
 * This function copies fence information to user-space. If copying fails, the
 * user-space struct drm_vmw_fence_rep::error member is hopefully left
 * untouched, and if it's preloaded with an -EFAULT by user-space, the error
 * will hopefully be detected.
 *
 * Also if copying fails, user-space will be unable to signal the fence object
 * so we wait for it immediately, and then unreference the user-space reference.
 */
int
vmw_execbuf_copy_fence_user(struct vmw_private *dev_priv,
			    struct vmw_fpriv *vmw_fp, int ret,
			    struct drm_vmw_fence_rep __user *user_fence_rep,
			    struct vmw_fence_obj *fence, uint32_t fence_handle,
			    int32_t out_fence_fd)
>>>>>>> upstream/android-13
{
	struct drm_vmw_fence_rep fence_rep;

	if (user_fence_rep == NULL)
<<<<<<< HEAD
		return;
=======
		return 0;
>>>>>>> upstream/android-13

	memset(&fence_rep, 0, sizeof(fence_rep));

	fence_rep.error = ret;
	fence_rep.fd = out_fence_fd;
	if (ret == 0) {
		BUG_ON(fence == NULL);

		fence_rep.handle = fence_handle;
		fence_rep.seqno = fence->base.seqno;
<<<<<<< HEAD
		vmw_update_seqno(dev_priv, &dev_priv->fifo);
=======
		vmw_update_seqno(dev_priv);
>>>>>>> upstream/android-13
		fence_rep.passed_seqno = dev_priv->last_read_seqno;
	}

	/*
<<<<<<< HEAD
	 * copy_to_user errors will be detected by user space not
	 * seeing fence_rep::error filled in. Typically
	 * user-space would have pre-set that member to -EFAULT.
=======
	 * copy_to_user errors will be detected by user space not seeing
	 * fence_rep::error filled in. Typically user-space would have pre-set
	 * that member to -EFAULT.
>>>>>>> upstream/android-13
	 */
	ret = copy_to_user(user_fence_rep, &fence_rep,
			   sizeof(fence_rep));

	/*
<<<<<<< HEAD
	 * User-space lost the fence object. We need to sync
	 * and unreference the handle.
	 */
	if (unlikely(ret != 0) && (fence_rep.error == 0)) {
		if (sync_file)
			fput(sync_file->file);

		if (fence_rep.fd != -1) {
			put_unused_fd(fence_rep.fd);
			fence_rep.fd = -1;
		}

		ttm_ref_object_base_unref(vmw_fp->tfile,
					  fence_handle, TTM_REF_USAGE);
		DRM_ERROR("Fence copy error. Syncing.\n");
		(void) vmw_fence_obj_wait(fence, false, false,
					  VMW_FENCE_WAIT_TIMEOUT);
	}
}

/**
 * vmw_execbuf_submit_fifo - Patch a command batch and submit it using
 * the fifo.
=======
	 * User-space lost the fence object. We need to sync and unreference the
	 * handle.
	 */
	if (unlikely(ret != 0) && (fence_rep.error == 0)) {
		ttm_ref_object_base_unref(vmw_fp->tfile, fence_handle,
					  TTM_REF_USAGE);
		VMW_DEBUG_USER("Fence copy error. Syncing.\n");
		(void) vmw_fence_obj_wait(fence, false, false,
					  VMW_FENCE_WAIT_TIMEOUT);
	}

	return ret ? -EFAULT : 0;
}

/**
 * vmw_execbuf_submit_fifo - Patch a command batch and submit it using the fifo.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private structure.
 * @kernel_commands: Pointer to the unpatched command batch.
 * @command_size: Size of the unpatched command batch.
 * @sw_context: Structure holding the relocation lists.
 *
<<<<<<< HEAD
 * Side effects: If this function returns 0, then the command batch
 * pointed to by @kernel_commands will have been modified.
 */
static int vmw_execbuf_submit_fifo(struct vmw_private *dev_priv,
				   void *kernel_commands,
				   u32 command_size,
=======
 * Side effects: If this function returns 0, then the command batch pointed to
 * by @kernel_commands will have been modified.
 */
static int vmw_execbuf_submit_fifo(struct vmw_private *dev_priv,
				   void *kernel_commands, u32 command_size,
>>>>>>> upstream/android-13
				   struct vmw_sw_context *sw_context)
{
	void *cmd;

	if (sw_context->dx_ctx_node)
<<<<<<< HEAD
		cmd = vmw_fifo_reserve_dx(dev_priv, command_size,
					  sw_context->dx_ctx_node->res->id);
	else
		cmd = vmw_fifo_reserve(dev_priv, command_size);
	if (!cmd) {
		DRM_ERROR("Failed reserving fifo space for commands.\n");
		return -ENOMEM;
	}
=======
		cmd = VMW_CMD_CTX_RESERVE(dev_priv, command_size,
					  sw_context->dx_ctx_node->ctx->id);
	else
		cmd = VMW_CMD_RESERVE(dev_priv, command_size);

	if (!cmd)
		return -ENOMEM;
>>>>>>> upstream/android-13

	vmw_apply_relocations(sw_context);
	memcpy(cmd, kernel_commands, command_size);
	vmw_resource_relocations_apply(cmd, &sw_context->res_relocations);
	vmw_resource_relocations_free(&sw_context->res_relocations);
<<<<<<< HEAD
	vmw_fifo_commit(dev_priv, command_size);
=======
	vmw_cmd_commit(dev_priv, command_size);
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * vmw_execbuf_submit_cmdbuf - Patch a command batch and submit it using
 * the command buffer manager.
=======
 * vmw_execbuf_submit_cmdbuf - Patch a command batch and submit it using the
 * command buffer manager.
>>>>>>> upstream/android-13
 *
 * @dev_priv: Pointer to a device private structure.
 * @header: Opaque handle to the command buffer allocation.
 * @command_size: Size of the unpatched command batch.
 * @sw_context: Structure holding the relocation lists.
 *
<<<<<<< HEAD
 * Side effects: If this function returns 0, then the command buffer
 * represented by @header will have been modified.
=======
 * Side effects: If this function returns 0, then the command buffer represented
 * by @header will have been modified.
>>>>>>> upstream/android-13
 */
static int vmw_execbuf_submit_cmdbuf(struct vmw_private *dev_priv,
				     struct vmw_cmdbuf_header *header,
				     u32 command_size,
				     struct vmw_sw_context *sw_context)
{
<<<<<<< HEAD
	u32 id = ((sw_context->dx_ctx_node) ? sw_context->dx_ctx_node->res->id :
		  SVGA3D_INVALID_ID);
	void *cmd = vmw_cmdbuf_reserve(dev_priv->cman, command_size,
				       id, false, header);
=======
	u32 id = ((sw_context->dx_ctx_node) ? sw_context->dx_ctx_node->ctx->id :
		  SVGA3D_INVALID_ID);
	void *cmd = vmw_cmdbuf_reserve(dev_priv->cman, command_size, id, false,
				       header);
>>>>>>> upstream/android-13

	vmw_apply_relocations(sw_context);
	vmw_resource_relocations_apply(cmd, &sw_context->res_relocations);
	vmw_resource_relocations_free(&sw_context->res_relocations);
	vmw_cmdbuf_commit(dev_priv->cman, command_size, header, false);

	return 0;
}

/**
 * vmw_execbuf_cmdbuf - Prepare, if possible, a user-space command batch for
 * submission using a command buffer.
 *
 * @dev_priv: Pointer to a device private structure.
 * @user_commands: User-space pointer to the commands to be submitted.
 * @command_size: Size of the unpatched command batch.
 * @header: Out parameter returning the opaque pointer to the command buffer.
 *
 * This function checks whether we can use the command buffer manager for
<<<<<<< HEAD
 * submission and if so, creates a command buffer of suitable size and
 * copies the user data into that buffer.
 *
 * On successful return, the function returns a pointer to the data in the
 * command buffer and *@header is set to non-NULL.
 * If command buffers could not be used, the function will return the value
 * of @kernel_commands on function call. That value may be NULL. In that case,
 * the value of *@header will be set to NULL.
=======
 * submission and if so, creates a command buffer of suitable size and copies
 * the user data into that buffer.
 *
 * On successful return, the function returns a pointer to the data in the
 * command buffer and *@header is set to non-NULL.
 *
 * @kernel_commands: If command buffers could not be used, the function will
 * return the value of @kernel_commands on function call. That value may be
 * NULL. In that case, the value of *@header will be set to NULL.
 *
>>>>>>> upstream/android-13
 * If an error is encountered, the function will return a pointer error value.
 * If the function is interrupted by a signal while sleeping, it will return
 * -ERESTARTSYS casted to a pointer error value.
 */
static void *vmw_execbuf_cmdbuf(struct vmw_private *dev_priv,
				void __user *user_commands,
<<<<<<< HEAD
				void *kernel_commands,
				u32 command_size,
=======
				void *kernel_commands, u32 command_size,
>>>>>>> upstream/android-13
				struct vmw_cmdbuf_header **header)
{
	size_t cmdbuf_size;
	int ret;

	*header = NULL;
	if (command_size > SVGA_CB_MAX_SIZE) {
<<<<<<< HEAD
		DRM_ERROR("Command buffer is too large.\n");
=======
		VMW_DEBUG_USER("Command buffer is too large.\n");
>>>>>>> upstream/android-13
		return ERR_PTR(-EINVAL);
	}

	if (!dev_priv->cman || kernel_commands)
		return kernel_commands;

	/* If possible, add a little space for fencing. */
	cmdbuf_size = command_size + 512;
	cmdbuf_size = min_t(size_t, cmdbuf_size, SVGA_CB_MAX_SIZE);
<<<<<<< HEAD
	kernel_commands = vmw_cmdbuf_alloc(dev_priv->cman, cmdbuf_size,
					   true, header);
	if (IS_ERR(kernel_commands))
		return kernel_commands;

	ret = copy_from_user(kernel_commands, user_commands,
			     command_size);
	if (ret) {
		DRM_ERROR("Failed copying commands.\n");
=======
	kernel_commands = vmw_cmdbuf_alloc(dev_priv->cman, cmdbuf_size, true,
					   header);
	if (IS_ERR(kernel_commands))
		return kernel_commands;

	ret = copy_from_user(kernel_commands, user_commands, command_size);
	if (ret) {
		VMW_DEBUG_USER("Failed copying commands.\n");
>>>>>>> upstream/android-13
		vmw_cmdbuf_header_free(*header);
		*header = NULL;
		return ERR_PTR(-EFAULT);
	}

	return kernel_commands;
}

static int vmw_execbuf_tie_context(struct vmw_private *dev_priv,
				   struct vmw_sw_context *sw_context,
				   uint32_t handle)
{
<<<<<<< HEAD
	struct vmw_resource_val_node *ctx_node;
	struct vmw_resource *res;
	int ret;
=======
	struct vmw_resource *res;
	int ret;
	unsigned int size;
>>>>>>> upstream/android-13

	if (handle == SVGA3D_INVALID_ID)
		return 0;

<<<<<<< HEAD
	ret = vmw_user_resource_lookup_handle(dev_priv, sw_context->fp->tfile,
					      handle, user_context_converter,
					      &res);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Could not find or user DX context 0x%08x.\n",
			  (unsigned) handle);
		return ret;
	}

	ret = vmw_resource_val_add(sw_context, res, &ctx_node);
	if (unlikely(ret != 0))
		goto out_err;

	sw_context->dx_ctx_node = ctx_node;
	sw_context->man = vmw_context_res_man(res);
out_err:
	vmw_resource_unreference(&res);
	return ret;
=======
	size = vmw_execbuf_res_size(dev_priv, vmw_res_dx_context);
	ret = vmw_validation_preload_res(sw_context->ctx, size);
	if (ret)
		return ret;

	res = vmw_user_resource_noref_lookup_handle
		(dev_priv, sw_context->fp->tfile, handle,
		 user_context_converter);
	if (IS_ERR(res)) {
		VMW_DEBUG_USER("Could not find or user DX context 0x%08x.\n",
			       (unsigned int) handle);
		return PTR_ERR(res);
	}

	ret = vmw_execbuf_res_noref_val_add(sw_context, res, VMW_RES_DIRTY_SET);
	if (unlikely(ret != 0))
		return ret;

	sw_context->dx_ctx_node = vmw_execbuf_info_from_res(sw_context, res);
	sw_context->man = vmw_context_res_man(res);

	return 0;
>>>>>>> upstream/android-13
}

int vmw_execbuf_process(struct drm_file *file_priv,
			struct vmw_private *dev_priv,
<<<<<<< HEAD
			void __user *user_commands,
			void *kernel_commands,
			uint32_t command_size,
			uint64_t throttle_us,
			uint32_t dx_context_handle,
			struct drm_vmw_fence_rep __user *user_fence_rep,
			struct vmw_fence_obj **out_fence,
			uint32_t flags)
{
	struct vmw_sw_context *sw_context = &dev_priv->ctx;
	struct vmw_fence_obj *fence = NULL;
	struct vmw_resource *error_resource;
	struct list_head resource_list;
	struct vmw_cmdbuf_header *header;
	struct ww_acquire_ctx ticket;
	uint32_t handle;
	int ret;
	int32_t out_fence_fd = -1;
	struct sync_file *sync_file = NULL;

=======
			void __user *user_commands, void *kernel_commands,
			uint32_t command_size, uint64_t throttle_us,
			uint32_t dx_context_handle,
			struct drm_vmw_fence_rep __user *user_fence_rep,
			struct vmw_fence_obj **out_fence, uint32_t flags)
{
	struct vmw_sw_context *sw_context = &dev_priv->ctx;
	struct vmw_fence_obj *fence = NULL;
	struct vmw_cmdbuf_header *header;
	uint32_t handle = 0;
	int ret;
	int32_t out_fence_fd = -1;
	struct sync_file *sync_file = NULL;
	DECLARE_VAL_CONTEXT(val_ctx, &sw_context->res_ht, 1);

	vmw_validation_set_val_mem(&val_ctx, &dev_priv->vvm);
>>>>>>> upstream/android-13

	if (flags & DRM_VMW_EXECBUF_FLAG_EXPORT_FENCE_FD) {
		out_fence_fd = get_unused_fd_flags(O_CLOEXEC);
		if (out_fence_fd < 0) {
<<<<<<< HEAD
			DRM_ERROR("Failed to get a fence file descriptor.\n");
=======
			VMW_DEBUG_USER("Failed to get a fence fd.\n");
>>>>>>> upstream/android-13
			return out_fence_fd;
		}
	}

	if (throttle_us) {
<<<<<<< HEAD
		ret = vmw_wait_lag(dev_priv, &dev_priv->fifo.marker_queue,
				   throttle_us);

		if (ret)
			goto out_free_fence_fd;
=======
		VMW_DEBUG_USER("Throttling is no longer supported.\n");
>>>>>>> upstream/android-13
	}

	kernel_commands = vmw_execbuf_cmdbuf(dev_priv, user_commands,
					     kernel_commands, command_size,
					     &header);
	if (IS_ERR(kernel_commands)) {
		ret = PTR_ERR(kernel_commands);
		goto out_free_fence_fd;
	}

	ret = mutex_lock_interruptible(&dev_priv->cmdbuf_mutex);
	if (ret) {
		ret = -ERESTARTSYS;
		goto out_free_header;
	}

	sw_context->kernel = false;
	if (kernel_commands == NULL) {
		ret = vmw_resize_cmd_bounce(sw_context, command_size);
		if (unlikely(ret != 0))
			goto out_unlock;

<<<<<<< HEAD

		ret = copy_from_user(sw_context->cmd_bounce,
				     user_commands, command_size);

		if (unlikely(ret != 0)) {
			ret = -EFAULT;
			DRM_ERROR("Failed copying commands.\n");
			goto out_unlock;
		}
		kernel_commands = sw_context->cmd_bounce;
	} else if (!header)
		sw_context->kernel = true;

	sw_context->fp = vmw_fpriv(file_priv);
	sw_context->cur_reloc = 0;
	sw_context->cur_val_buf = 0;
	INIT_LIST_HEAD(&sw_context->resource_list);
	INIT_LIST_HEAD(&sw_context->ctx_resource_list);
=======
		ret = copy_from_user(sw_context->cmd_bounce, user_commands,
				     command_size);
		if (unlikely(ret != 0)) {
			ret = -EFAULT;
			VMW_DEBUG_USER("Failed copying commands.\n");
			goto out_unlock;
		}

		kernel_commands = sw_context->cmd_bounce;
	} else if (!header) {
		sw_context->kernel = true;
	}

	sw_context->fp = vmw_fpriv(file_priv);
	INIT_LIST_HEAD(&sw_context->ctx_list);
>>>>>>> upstream/android-13
	sw_context->cur_query_bo = dev_priv->pinned_bo;
	sw_context->last_query_ctx = NULL;
	sw_context->needs_post_query_barrier = false;
	sw_context->dx_ctx_node = NULL;
	sw_context->dx_query_mob = NULL;
	sw_context->dx_query_ctx = NULL;
	memset(sw_context->res_cache, 0, sizeof(sw_context->res_cache));
<<<<<<< HEAD
	INIT_LIST_HEAD(&sw_context->validate_nodes);
	INIT_LIST_HEAD(&sw_context->res_relocations);
=======
	INIT_LIST_HEAD(&sw_context->res_relocations);
	INIT_LIST_HEAD(&sw_context->bo_relocations);

>>>>>>> upstream/android-13
	if (sw_context->staged_bindings)
		vmw_binding_state_reset(sw_context->staged_bindings);

	if (!sw_context->res_ht_initialized) {
		ret = drm_ht_create(&sw_context->res_ht, VMW_RES_HT_ORDER);
		if (unlikely(ret != 0))
			goto out_unlock;
<<<<<<< HEAD
		sw_context->res_ht_initialized = true;
	}
	INIT_LIST_HEAD(&sw_context->staged_cmd_res);
	INIT_LIST_HEAD(&resource_list);
	ret = vmw_execbuf_tie_context(dev_priv, sw_context, dx_context_handle);
	if (unlikely(ret != 0)) {
		list_splice_init(&sw_context->ctx_resource_list,
				 &sw_context->resource_list);
		goto out_err_nores;
	}

	ret = vmw_cmd_check_all(dev_priv, sw_context, kernel_commands,
				command_size);
	/*
	 * Merge the resource lists before checking the return status
	 * from vmd_cmd_check_all so that all the open hashtabs will
	 * be handled properly even if vmw_cmd_check_all fails.
	 */
	list_splice_init(&sw_context->ctx_resource_list,
			 &sw_context->resource_list);

=======

		sw_context->res_ht_initialized = true;
	}

	INIT_LIST_HEAD(&sw_context->staged_cmd_res);
	sw_context->ctx = &val_ctx;
	ret = vmw_execbuf_tie_context(dev_priv, sw_context, dx_context_handle);
	if (unlikely(ret != 0))
		goto out_err_nores;

	ret = vmw_cmd_check_all(dev_priv, sw_context, kernel_commands,
				command_size);
>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		goto out_err_nores;

	ret = vmw_resources_reserve(sw_context);
	if (unlikely(ret != 0))
		goto out_err_nores;

<<<<<<< HEAD
	ret = ttm_eu_reserve_buffers(&ticket, &sw_context->validate_nodes,
				     true, NULL);
	if (unlikely(ret != 0))
		goto out_err_nores;

	ret = vmw_validate_buffers(dev_priv, sw_context);
	if (unlikely(ret != 0))
		goto out_err;

	ret = vmw_resources_validate(sw_context);
	if (unlikely(ret != 0))
		goto out_err;

=======
	ret = vmw_validation_bo_reserve(&val_ctx, true);
	if (unlikely(ret != 0))
		goto out_err_nores;

	ret = vmw_validation_bo_validate(&val_ctx, true);
	if (unlikely(ret != 0))
		goto out_err;

	ret = vmw_validation_res_validate(&val_ctx, true);
	if (unlikely(ret != 0))
		goto out_err;

	vmw_validation_drop_ht(&val_ctx);

>>>>>>> upstream/android-13
	ret = mutex_lock_interruptible(&dev_priv->binding_mutex);
	if (unlikely(ret != 0)) {
		ret = -ERESTARTSYS;
		goto out_err;
	}

	if (dev_priv->has_mob) {
		ret = vmw_rebind_contexts(sw_context);
		if (unlikely(ret != 0))
			goto out_unlock_binding;
	}

	if (!header) {
		ret = vmw_execbuf_submit_fifo(dev_priv, kernel_commands,
					      command_size, sw_context);
	} else {
		ret = vmw_execbuf_submit_cmdbuf(dev_priv, header, command_size,
						sw_context);
		header = NULL;
	}
	mutex_unlock(&dev_priv->binding_mutex);
	if (ret)
		goto out_err;

	vmw_query_bo_switch_commit(dev_priv, sw_context);
<<<<<<< HEAD
	ret = vmw_execbuf_fence_commands(file_priv, dev_priv,
					 &fence,
=======
	ret = vmw_execbuf_fence_commands(file_priv, dev_priv, &fence,
>>>>>>> upstream/android-13
					 (user_fence_rep) ? &handle : NULL);
	/*
	 * This error is harmless, because if fence submission fails,
	 * vmw_fifo_send_fence will sync. The error will be propagated to
	 * user-space in @fence_rep
	 */
<<<<<<< HEAD

	if (ret != 0)
		DRM_ERROR("Fence submission error. Syncing.\n");

	vmw_resources_unreserve(sw_context, false);

	ttm_eu_fence_buffer_objects(&ticket, &sw_context->validate_nodes,
				    (void *) fence);

	if (unlikely(dev_priv->pinned_bo != NULL &&
		     !dev_priv->query_cid_valid))
		__vmw_execbuf_release_pinned_bo(dev_priv, fence);

	vmw_clear_validations(sw_context);

	/*
	 * If anything fails here, give up trying to export the fence
	 * and do a sync since the user mode will not be able to sync
	 * the fence itself.  This ensures we are still functionally
	 * correct.
=======
	if (ret != 0)
		VMW_DEBUG_USER("Fence submission error. Syncing.\n");

	vmw_execbuf_bindings_commit(sw_context, false);
	vmw_bind_dx_query_mob(sw_context);
	vmw_validation_res_unreserve(&val_ctx, false);

	vmw_validation_bo_fence(sw_context->ctx, fence);

	if (unlikely(dev_priv->pinned_bo != NULL && !dev_priv->query_cid_valid))
		__vmw_execbuf_release_pinned_bo(dev_priv, fence);

	/*
	 * If anything fails here, give up trying to export the fence and do a
	 * sync since the user mode will not be able to sync the fence itself.
	 * This ensures we are still functionally correct.
>>>>>>> upstream/android-13
	 */
	if (flags & DRM_VMW_EXECBUF_FLAG_EXPORT_FENCE_FD) {

		sync_file = sync_file_create(&fence->base);
		if (!sync_file) {
<<<<<<< HEAD
			DRM_ERROR("Unable to create sync file for fence\n");
=======
			VMW_DEBUG_USER("Sync file create failed for fence\n");
>>>>>>> upstream/android-13
			put_unused_fd(out_fence_fd);
			out_fence_fd = -1;

			(void) vmw_fence_obj_wait(fence, false, false,
						  VMW_FENCE_WAIT_TIMEOUT);
<<<<<<< HEAD
=======
		}
	}

	ret = vmw_execbuf_copy_fence_user(dev_priv, vmw_fpriv(file_priv), ret,
				    user_fence_rep, fence, handle, out_fence_fd);

	if (sync_file) {
		if (ret) {
			/* usercopy of fence failed, put the file object */
			fput(sync_file->file);
			put_unused_fd(out_fence_fd);
>>>>>>> upstream/android-13
		} else {
			/* Link the fence with the FD created earlier */
			fd_install(out_fence_fd, sync_file->file);
		}
	}

<<<<<<< HEAD
	vmw_execbuf_copy_fence_user(dev_priv, vmw_fpriv(file_priv), ret,
				    user_fence_rep, fence, handle,
				    out_fence_fd, sync_file);

=======
>>>>>>> upstream/android-13
	/* Don't unreference when handing fence out */
	if (unlikely(out_fence != NULL)) {
		*out_fence = fence;
		fence = NULL;
	} else if (likely(fence != NULL)) {
		vmw_fence_obj_unreference(&fence);
	}

<<<<<<< HEAD
	list_splice_init(&sw_context->resource_list, &resource_list);
=======
>>>>>>> upstream/android-13
	vmw_cmdbuf_res_commit(&sw_context->staged_cmd_res);
	mutex_unlock(&dev_priv->cmdbuf_mutex);

	/*
<<<<<<< HEAD
	 * Unreference resources outside of the cmdbuf_mutex to
	 * avoid deadlocks in resource destruction paths.
	 */
	vmw_resource_list_unreference(sw_context, &resource_list);

	return 0;
=======
	 * Unreference resources outside of the cmdbuf_mutex to avoid deadlocks
	 * in resource destruction paths.
	 */
	vmw_validation_unref_lists(&val_ctx);

	return ret;
>>>>>>> upstream/android-13

out_unlock_binding:
	mutex_unlock(&dev_priv->binding_mutex);
out_err:
<<<<<<< HEAD
	ttm_eu_backoff_reservation(&ticket, &sw_context->validate_nodes);
out_err_nores:
	vmw_resources_unreserve(sw_context, true);
	vmw_resource_relocations_free(&sw_context->res_relocations);
	vmw_free_relocations(sw_context);
	vmw_clear_validations(sw_context);
	if (unlikely(dev_priv->pinned_bo != NULL &&
		     !dev_priv->query_cid_valid))
		__vmw_execbuf_release_pinned_bo(dev_priv, NULL);
out_unlock:
	list_splice_init(&sw_context->resource_list, &resource_list);
	error_resource = sw_context->error_resource;
	sw_context->error_resource = NULL;
	vmw_cmdbuf_res_revert(&sw_context->staged_cmd_res);
	mutex_unlock(&dev_priv->cmdbuf_mutex);

	/*
	 * Unreference resources outside of the cmdbuf_mutex to
	 * avoid deadlocks in resource destruction paths.
	 */
	vmw_resource_list_unreference(sw_context, &resource_list);
	if (unlikely(error_resource != NULL))
		vmw_resource_unreference(&error_resource);
=======
	vmw_validation_bo_backoff(&val_ctx);
out_err_nores:
	vmw_execbuf_bindings_commit(sw_context, true);
	vmw_validation_res_unreserve(&val_ctx, true);
	vmw_resource_relocations_free(&sw_context->res_relocations);
	vmw_free_relocations(sw_context);
	if (unlikely(dev_priv->pinned_bo != NULL && !dev_priv->query_cid_valid))
		__vmw_execbuf_release_pinned_bo(dev_priv, NULL);
out_unlock:
	vmw_cmdbuf_res_revert(&sw_context->staged_cmd_res);
	vmw_validation_drop_ht(&val_ctx);
	WARN_ON(!list_empty(&sw_context->ctx_list));
	mutex_unlock(&dev_priv->cmdbuf_mutex);

	/*
	 * Unreference resources outside of the cmdbuf_mutex to avoid deadlocks
	 * in resource destruction paths.
	 */
	vmw_validation_unref_lists(&val_ctx);
>>>>>>> upstream/android-13
out_free_header:
	if (header)
		vmw_cmdbuf_header_free(header);
out_free_fence_fd:
	if (out_fence_fd >= 0)
		put_unused_fd(out_fence_fd);

	return ret;
}

/**
 * vmw_execbuf_unpin_panic - Idle the fifo and unpin the query buffer.
 *
 * @dev_priv: The device private structure.
 *
<<<<<<< HEAD
 * This function is called to idle the fifo and unpin the query buffer
 * if the normal way to do this hits an error, which should typically be
 * extremely rare.
 */
static void vmw_execbuf_unpin_panic(struct vmw_private *dev_priv)
{
	DRM_ERROR("Can't unpin query buffer. Trying to recover.\n");
=======
 * This function is called to idle the fifo and unpin the query buffer if the
 * normal way to do this hits an error, which should typically be extremely
 * rare.
 */
static void vmw_execbuf_unpin_panic(struct vmw_private *dev_priv)
{
	VMW_DEBUG_USER("Can't unpin query buffer. Trying to recover.\n");
>>>>>>> upstream/android-13

	(void) vmw_fallback_wait(dev_priv, false, true, 0, false, 10*HZ);
	vmw_bo_pin_reserved(dev_priv->pinned_bo, false);
	if (dev_priv->dummy_query_bo_pinned) {
		vmw_bo_pin_reserved(dev_priv->dummy_query_bo, false);
		dev_priv->dummy_query_bo_pinned = false;
	}
}


/**
<<<<<<< HEAD
 * __vmw_execbuf_release_pinned_bo - Flush queries and unpin the pinned
 * query bo.
 *
 * @dev_priv: The device private structure.
 * @fence: If non-NULL should point to a struct vmw_fence_obj issued
 * _after_ a query barrier that flushes all queries touching the current
 * buffer pointed to by @dev_priv->pinned_bo
 *
 * This function should be used to unpin the pinned query bo, or
 * as a query barrier when we need to make sure that all queries have
 * finished before the next fifo command. (For example on hardware
 * context destructions where the hardware may otherwise leak unfinished
 * queries).
 *
 * This function does not return any failure codes, but make attempts
 * to do safe unpinning in case of errors.
 *
 * The function will synchronize on the previous query barrier, and will
 * thus not finish until that barrier has executed.
 *
 * the @dev_priv->cmdbuf_mutex needs to be held by the current thread
 * before calling this function.
=======
 * __vmw_execbuf_release_pinned_bo - Flush queries and unpin the pinned query
 * bo.
 *
 * @dev_priv: The device private structure.
 * @fence: If non-NULL should point to a struct vmw_fence_obj issued _after_ a
 * query barrier that flushes all queries touching the current buffer pointed to
 * by @dev_priv->pinned_bo
 *
 * This function should be used to unpin the pinned query bo, or as a query
 * barrier when we need to make sure that all queries have finished before the
 * next fifo command. (For example on hardware context destructions where the
 * hardware may otherwise leak unfinished queries).
 *
 * This function does not return any failure codes, but make attempts to do safe
 * unpinning in case of errors.
 *
 * The function will synchronize on the previous query barrier, and will thus
 * not finish until that barrier has executed.
 *
 * the @dev_priv->cmdbuf_mutex needs to be held by the current thread before
 * calling this function.
>>>>>>> upstream/android-13
 */
void __vmw_execbuf_release_pinned_bo(struct vmw_private *dev_priv,
				     struct vmw_fence_obj *fence)
{
	int ret = 0;
<<<<<<< HEAD
	struct list_head validate_list;
	struct ttm_validate_buffer pinned_val, query_val;
	struct vmw_fence_obj *lfence = NULL;
	struct ww_acquire_ctx ticket;
=======
	struct vmw_fence_obj *lfence = NULL;
	DECLARE_VAL_CONTEXT(val_ctx, NULL, 0);
>>>>>>> upstream/android-13

	if (dev_priv->pinned_bo == NULL)
		goto out_unlock;

<<<<<<< HEAD
	INIT_LIST_HEAD(&validate_list);

	pinned_val.bo = ttm_bo_reference(&dev_priv->pinned_bo->base);
	pinned_val.shared = false;
	list_add_tail(&pinned_val.head, &validate_list);

	query_val.bo = ttm_bo_reference(&dev_priv->dummy_query_bo->base);
	query_val.shared = false;
	list_add_tail(&query_val.head, &validate_list);

	ret = ttm_eu_reserve_buffers(&ticket, &validate_list,
				     false, NULL);
	if (unlikely(ret != 0)) {
		vmw_execbuf_unpin_panic(dev_priv);
		goto out_no_reserve;
	}

	if (dev_priv->query_cid_valid) {
		BUG_ON(fence != NULL);
		ret = vmw_fifo_emit_dummy_query(dev_priv, dev_priv->query_cid);
		if (unlikely(ret != 0)) {
			vmw_execbuf_unpin_panic(dev_priv);
			goto out_no_emit;
		}
=======
	ret = vmw_validation_add_bo(&val_ctx, dev_priv->pinned_bo, false,
				    false);
	if (ret)
		goto out_no_reserve;

	ret = vmw_validation_add_bo(&val_ctx, dev_priv->dummy_query_bo, false,
				    false);
	if (ret)
		goto out_no_reserve;

	ret = vmw_validation_bo_reserve(&val_ctx, false);
	if (ret)
		goto out_no_reserve;

	if (dev_priv->query_cid_valid) {
		BUG_ON(fence != NULL);
		ret = vmw_cmd_emit_dummy_query(dev_priv, dev_priv->query_cid);
		if (ret)
			goto out_no_emit;
>>>>>>> upstream/android-13
		dev_priv->query_cid_valid = false;
	}

	vmw_bo_pin_reserved(dev_priv->pinned_bo, false);
	if (dev_priv->dummy_query_bo_pinned) {
		vmw_bo_pin_reserved(dev_priv->dummy_query_bo, false);
		dev_priv->dummy_query_bo_pinned = false;
	}
	if (fence == NULL) {
		(void) vmw_execbuf_fence_commands(NULL, dev_priv, &lfence,
						  NULL);
		fence = lfence;
	}
<<<<<<< HEAD
	ttm_eu_fence_buffer_objects(&ticket, &validate_list, (void *) fence);
	if (lfence != NULL)
		vmw_fence_obj_unreference(&lfence);

	ttm_bo_unref(&query_val.bo);
	ttm_bo_unref(&pinned_val.bo);
	vmw_bo_unreference(&dev_priv->pinned_bo);
out_unlock:
	return;

out_no_emit:
	ttm_eu_backoff_reservation(&ticket, &validate_list);
out_no_reserve:
	ttm_bo_unref(&query_val.bo);
	ttm_bo_unref(&pinned_val.bo);
=======
	vmw_validation_bo_fence(&val_ctx, fence);
	if (lfence != NULL)
		vmw_fence_obj_unreference(&lfence);

	vmw_validation_unref_lists(&val_ctx);
	vmw_bo_unreference(&dev_priv->pinned_bo);

out_unlock:
	return;
out_no_emit:
	vmw_validation_bo_backoff(&val_ctx);
out_no_reserve:
	vmw_validation_unref_lists(&val_ctx);
	vmw_execbuf_unpin_panic(dev_priv);
>>>>>>> upstream/android-13
	vmw_bo_unreference(&dev_priv->pinned_bo);
}

/**
<<<<<<< HEAD
 * vmw_execbuf_release_pinned_bo - Flush queries and unpin the pinned
 * query bo.
 *
 * @dev_priv: The device private structure.
 *
 * This function should be used to unpin the pinned query bo, or
 * as a query barrier when we need to make sure that all queries have
 * finished before the next fifo command. (For example on hardware
 * context destructions where the hardware may otherwise leak unfinished
 * queries).
 *
 * This function does not return any failure codes, but make attempts
 * to do safe unpinning in case of errors.
 *
 * The function will synchronize on the previous query barrier, and will
 * thus not finish until that barrier has executed.
=======
 * vmw_execbuf_release_pinned_bo - Flush queries and unpin the pinned query bo.
 *
 * @dev_priv: The device private structure.
 *
 * This function should be used to unpin the pinned query bo, or as a query
 * barrier when we need to make sure that all queries have finished before the
 * next fifo command. (For example on hardware context destructions where the
 * hardware may otherwise leak unfinished queries).
 *
 * This function does not return any failure codes, but make attempts to do safe
 * unpinning in case of errors.
 *
 * The function will synchronize on the previous query barrier, and will thus
 * not finish until that barrier has executed.
>>>>>>> upstream/android-13
 */
void vmw_execbuf_release_pinned_bo(struct vmw_private *dev_priv)
{
	mutex_lock(&dev_priv->cmdbuf_mutex);
	if (dev_priv->query_cid_valid)
		__vmw_execbuf_release_pinned_bo(dev_priv, NULL);
	mutex_unlock(&dev_priv->cmdbuf_mutex);
}

<<<<<<< HEAD
int vmw_execbuf_ioctl(struct drm_device *dev, unsigned long data,
		      struct drm_file *file_priv, size_t size)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct drm_vmw_execbuf_arg arg;
	int ret;
	static const size_t copy_offset[] = {
		offsetof(struct drm_vmw_execbuf_arg, context_handle),
		sizeof(struct drm_vmw_execbuf_arg)};
	struct dma_fence *in_fence = NULL;

	if (unlikely(size < copy_offset[0])) {
		DRM_ERROR("Invalid command size, ioctl %d\n",
			  DRM_VMW_EXECBUF);
		return -EINVAL;
	}

	if (copy_from_user(&arg, (void __user *) data, copy_offset[0]) != 0)
		return -EFAULT;

	/*
	 * Extend the ioctl argument while
	 * maintaining backwards compatibility:
	 * We take different code paths depending on the value of
	 * arg.version.
	 */

	if (unlikely(arg.version > DRM_VMW_EXECBUF_VERSION ||
		     arg.version == 0)) {
		DRM_ERROR("Incorrect execbuf version.\n");
		return -EINVAL;
	}

	if (arg.version > 1 &&
	    copy_from_user(&arg.context_handle,
			   (void __user *) (data + copy_offset[0]),
			   copy_offset[arg.version - 1] -
			   copy_offset[0]) != 0)
		return -EFAULT;

	switch (arg.version) {
	case 1:
		arg.context_handle = (uint32_t) -1;
		break;
	case 2:
	default:
		break;
	}


	/* If imported a fence FD from elsewhere, then wait on it */
	if (arg.flags & DRM_VMW_EXECBUF_FLAG_IMPORT_FENCE_FD) {
		in_fence = sync_file_get_fence(arg.imported_fence_fd);

		if (!in_fence) {
			DRM_ERROR("Cannot get imported fence\n");
			return -EINVAL;
=======
int vmw_execbuf_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct drm_vmw_execbuf_arg *arg = data;
	int ret;
	struct dma_fence *in_fence = NULL;

	MKS_STAT_TIME_DECL(MKSSTAT_KERN_EXECBUF);
	MKS_STAT_TIME_PUSH(MKSSTAT_KERN_EXECBUF);

	/*
	 * Extend the ioctl argument while maintaining backwards compatibility:
	 * We take different code paths depending on the value of arg->version.
	 *
	 * Note: The ioctl argument is extended and zeropadded by core DRM.
	 */
	if (unlikely(arg->version > DRM_VMW_EXECBUF_VERSION ||
		     arg->version == 0)) {
		VMW_DEBUG_USER("Incorrect execbuf version.\n");
		ret = -EINVAL;
		goto mksstats_out;
	}

	switch (arg->version) {
	case 1:
		/* For v1 core DRM have extended + zeropadded the data */
		arg->context_handle = (uint32_t) -1;
		break;
	case 2:
	default:
		/* For v2 and later core DRM would have correctly copied it */
		break;
	}

	/* If imported a fence FD from elsewhere, then wait on it */
	if (arg->flags & DRM_VMW_EXECBUF_FLAG_IMPORT_FENCE_FD) {
		in_fence = sync_file_get_fence(arg->imported_fence_fd);

		if (!in_fence) {
			VMW_DEBUG_USER("Cannot get imported fence\n");
			ret = -EINVAL;
			goto mksstats_out;
>>>>>>> upstream/android-13
		}

		ret = vmw_wait_dma_fence(dev_priv->fman, in_fence);
		if (ret)
			goto out;
	}

<<<<<<< HEAD
	ret = ttm_read_lock(&dev_priv->reservation_sem, true);
	if (unlikely(ret != 0))
		return ret;

	ret = vmw_execbuf_process(file_priv, dev_priv,
				  (void __user *)(unsigned long)arg.commands,
				  NULL, arg.command_size, arg.throttle_us,
				  arg.context_handle,
				  (void __user *)(unsigned long)arg.fence_rep,
				  NULL,
				  arg.flags);
	ttm_read_unlock(&dev_priv->reservation_sem);
=======
	ret = vmw_execbuf_process(file_priv, dev_priv,
				  (void __user *)(unsigned long)arg->commands,
				  NULL, arg->command_size, arg->throttle_us,
				  arg->context_handle,
				  (void __user *)(unsigned long)arg->fence_rep,
				  NULL, arg->flags);

>>>>>>> upstream/android-13
	if (unlikely(ret != 0))
		goto out;

	vmw_kms_cursor_post_execbuf(dev_priv);

out:
	if (in_fence)
		dma_fence_put(in_fence);
<<<<<<< HEAD
=======

mksstats_out:
	MKS_STAT_TIME_POP(MKSSTAT_KERN_EXECBUF);
>>>>>>> upstream/android-13
	return ret;
}
