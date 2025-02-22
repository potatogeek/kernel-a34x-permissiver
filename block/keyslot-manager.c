// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2019 Google LLC
 */

/**
 * DOC: The Keyslot Manager
 *
 * Many devices with inline encryption support have a limited number of "slots"
 * into which encryption contexts may be programmed, and requests can be tagged
 * with a slot number to specify the key to use for en/decryption.
 *
<<<<<<< HEAD
 * As the number of slots are limited, and programming keys is expensive on
=======
 * As the number of slots is limited, and programming keys is expensive on
>>>>>>> upstream/android-13
 * many inline encryption hardware, we don't want to program the same key into
 * multiple slots - if multiple requests are using the same key, we want to
 * program just one slot with that key and use that slot for all requests.
 *
 * The keyslot manager manages these keyslots appropriately, and also acts as
 * an abstraction between the inline encryption hardware and the upper layers.
 *
 * Lower layer devices will set up a keyslot manager in their request queue
 * and tell it how to perform device specific operations like programming/
 * evicting keys from keyslots.
 *
<<<<<<< HEAD
 * Upper layers will call keyslot_manager_get_slot_for_key() to program a
 * key into some slot in the inline encryption hardware.
 */
#include <crypto/algapi.h>
#include <linux/keyslot-manager.h>
=======
 * Upper layers will call blk_ksm_get_slot_for_key() to program a
 * key into some slot in the inline encryption hardware.
 */

#define pr_fmt(fmt) "blk-crypto: " fmt

#include <linux/keyslot-manager.h>
#include <linux/device.h>
>>>>>>> upstream/android-13
#include <linux/atomic.h>
#include <linux/mutex.h>
#include <linux/pm_runtime.h>
#include <linux/wait.h>
#include <linux/blkdev.h>

<<<<<<< HEAD
struct keyslot {
	atomic_t slot_refs;
	struct list_head idle_slot_node;
	struct hlist_node hash_node;
	struct blk_crypto_key key;
};

struct keyslot_manager {
	unsigned int num_slots;
	struct keyslot_mgmt_ll_ops ksm_ll_ops;
	unsigned int features;
	unsigned int crypto_mode_supported[BLK_ENCRYPTION_MODE_MAX];
	unsigned int max_dun_bytes_supported;
	void *ll_priv_data;

#ifdef CONFIG_PM
	/* Device for runtime power management (NULL if none) */
	struct device *dev;
#endif

	/* Protects programming and evicting keys from the device */
	struct rw_semaphore lock;
	/*
	 * Above rw_semaphore maybe nested when used a dm stack layer
	 * which is with inline encryption
	 */
	unsigned int lock_flags;

	/* List of idle slots, with least recently used slot at front */
	wait_queue_head_t idle_slots_wait_queue;
	struct list_head idle_slots;
	spinlock_t idle_slots_lock;

	/*
	 * Hash table which maps key hashes to keyslots, so that we can find a
	 * key's keyslot in O(1) time rather than O(num_slots).  Protected by
	 * 'lock'.  A cryptographic hash function is used so that timing attacks
	 * can't leak information about the raw keys.
	 */
	struct hlist_head *slot_hashtable;
	unsigned int slot_hashtable_size;

	/* Per-keyslot data */
	struct keyslot slots[];
};

static inline bool keyslot_manager_is_passthrough(struct keyslot_manager *ksm)
{
	return ksm->num_slots == 0;
}

#ifdef CONFIG_PM
static inline void keyslot_manager_set_dev(struct keyslot_manager *ksm,
					   struct device *dev)
{
	ksm->dev = dev;
}

/* If there's an underlying device and it's suspended, resume it. */
static inline void keyslot_manager_pm_get(struct keyslot_manager *ksm)
{
	if (ksm->dev)
		pm_runtime_get_sync(ksm->dev);
}

static inline void keyslot_manager_pm_put(struct keyslot_manager *ksm)
{
	if (ksm->dev)
		pm_runtime_put_sync(ksm->dev);
}
#else /* CONFIG_PM */
static inline void keyslot_manager_set_dev(struct keyslot_manager *ksm,
					   struct device *dev)
{
}

static inline void keyslot_manager_pm_get(struct keyslot_manager *ksm)
{
}

static inline void keyslot_manager_pm_put(struct keyslot_manager *ksm)
{
}
#endif /* !CONFIG_PM */

static inline void keyslot_manager_hw_enter(struct keyslot_manager *ksm)
=======
struct blk_ksm_keyslot {
	atomic_t slot_refs;
	struct list_head idle_slot_node;
	struct hlist_node hash_node;
	const struct blk_crypto_key *key;
	struct blk_keyslot_manager *ksm;
};

static inline void blk_ksm_hw_enter(struct blk_keyslot_manager *ksm)
>>>>>>> upstream/android-13
{
	/*
	 * Calling into the driver requires ksm->lock held and the device
	 * resumed.  But we must resume the device first, since that can acquire
<<<<<<< HEAD
	 * and release ksm->lock via keyslot_manager_reprogram_all_keys().
	 */
	keyslot_manager_pm_get(ksm);
	if (!ksm->lock_flags)
		down_write(&ksm->lock);
	else
		down_write_nested(&ksm->lock, ksm->lock_flags);
}

static inline void keyslot_manager_hw_exit(struct keyslot_manager *ksm)
{
	up_write(&ksm->lock);
	keyslot_manager_pm_put(ksm);
}

/**
 * keyslot_manager_create() - Create a keyslot manager
 * @dev: Device for runtime power management (NULL if none)
 * @num_slots: The number of key slots to manage.
 * @ksm_ll_ops: The struct keyslot_mgmt_ll_ops for the device that this keyslot
 *		manager will use to perform operations like programming and
 *		evicting keys.
 * @features: The supported features as a bitmask of BLK_CRYPTO_FEATURE_* flags.
 *	      Most drivers should set BLK_CRYPTO_FEATURE_STANDARD_KEYS here.
 * @crypto_mode_supported:	Array of size BLK_ENCRYPTION_MODE_MAX of
 *				bitmasks that represents whether a crypto mode
 *				and data unit size are supported. The i'th bit
 *				of crypto_mode_supported[crypto_mode] is set iff
 *				a data unit size of (1 << i) is supported. We
 *				only support data unit sizes that are powers of
 *				2.
 * @ll_priv_data: Private data passed as is to the functions in ksm_ll_ops.
 *
 * Allocate memory for and initialize a keyslot manager. Called by e.g.
 * storage drivers to set up a keyslot manager in their request_queue.
 *
 * Context: May sleep
 * Return: Pointer to constructed keyslot manager or NULL on error.
 */
struct keyslot_manager *keyslot_manager_create(
	struct device *dev,
	unsigned int num_slots,
	const struct keyslot_mgmt_ll_ops *ksm_ll_ops,
	unsigned int features,
	const unsigned int crypto_mode_supported[BLK_ENCRYPTION_MODE_MAX],
	void *ll_priv_data)
{
	struct keyslot_manager *ksm;
	unsigned int slot;
	unsigned int i;

	if (num_slots == 0)
		return NULL;

	/* Check that all ops are specified */
	if (ksm_ll_ops->keyslot_program == NULL ||
	    ksm_ll_ops->keyslot_evict == NULL)
		return NULL;

	ksm = kvzalloc(struct_size(ksm, slots, num_slots), GFP_KERNEL);
	if (!ksm)
		return NULL;

	ksm->num_slots = num_slots;
	ksm->ksm_ll_ops = *ksm_ll_ops;
	ksm->features = features;
	memcpy(ksm->crypto_mode_supported, crypto_mode_supported,
	       sizeof(ksm->crypto_mode_supported));
	ksm->max_dun_bytes_supported = BLK_CRYPTO_MAX_IV_SIZE;
	ksm->ll_priv_data = ll_priv_data;
	keyslot_manager_set_dev(ksm, dev);
=======
	 * and release ksm->lock via blk_ksm_reprogram_all_keys().
	 */
	if (ksm->dev)
		pm_runtime_get_sync(ksm->dev);
	down_write(&ksm->lock);
}

static inline void blk_ksm_hw_exit(struct blk_keyslot_manager *ksm)
{
	up_write(&ksm->lock);
	if (ksm->dev)
		pm_runtime_put_sync(ksm->dev);
}

static inline bool blk_ksm_is_passthrough(struct blk_keyslot_manager *ksm)
{
	return ksm->num_slots == 0;
}

/**
 * blk_ksm_init() - Initialize a keyslot manager
 * @ksm: The keyslot_manager to initialize.
 * @num_slots: The number of key slots to manage.
 *
 * Allocate memory for keyslots and initialize a keyslot manager. Called by
 * e.g. storage drivers to set up a keyslot manager in their request_queue.
 *
 * Return: 0 on success, or else a negative error code.
 */
int blk_ksm_init(struct blk_keyslot_manager *ksm, unsigned int num_slots)
{
	unsigned int slot;
	unsigned int i;
	unsigned int slot_hashtable_size;

	memset(ksm, 0, sizeof(*ksm));

	if (num_slots == 0)
		return -EINVAL;

	ksm->slots = kvcalloc(num_slots, sizeof(ksm->slots[0]), GFP_KERNEL);
	if (!ksm->slots)
		return -ENOMEM;

	ksm->num_slots = num_slots;
>>>>>>> upstream/android-13

	init_rwsem(&ksm->lock);

	init_waitqueue_head(&ksm->idle_slots_wait_queue);
	INIT_LIST_HEAD(&ksm->idle_slots);

	for (slot = 0; slot < num_slots; slot++) {
<<<<<<< HEAD
=======
		ksm->slots[slot].ksm = ksm;
>>>>>>> upstream/android-13
		list_add_tail(&ksm->slots[slot].idle_slot_node,
			      &ksm->idle_slots);
	}

	spin_lock_init(&ksm->idle_slots_lock);

<<<<<<< HEAD
	ksm->slot_hashtable_size = roundup_pow_of_two(num_slots);
	ksm->slot_hashtable = kvmalloc_array(ksm->slot_hashtable_size,
					     sizeof(ksm->slot_hashtable[0]),
					     GFP_KERNEL);
	if (!ksm->slot_hashtable)
		goto err_free_ksm;
	for (i = 0; i < ksm->slot_hashtable_size; i++)
		INIT_HLIST_HEAD(&ksm->slot_hashtable[i]);

	return ksm;

err_free_ksm:
	keyslot_manager_destroy(ksm);
	return NULL;
}
EXPORT_SYMBOL_GPL(keyslot_manager_create);

void keyslot_manager_set_max_dun_bytes(struct keyslot_manager *ksm,
				       unsigned int max_dun_bytes)
{
	ksm->max_dun_bytes_supported = max_dun_bytes;
}
EXPORT_SYMBOL_GPL(keyslot_manager_set_max_dun_bytes);

static inline struct hlist_head *
hash_bucket_for_key(struct keyslot_manager *ksm,
		    const struct blk_crypto_key *key)
{
	return &ksm->slot_hashtable[blk_crypto_key_hash(key) &
				    (ksm->slot_hashtable_size - 1)];
}

static void remove_slot_from_lru_list(struct keyslot_manager *ksm, int slot)
{
	unsigned long flags;

	spin_lock_irqsave(&ksm->idle_slots_lock, flags);
	list_del(&ksm->slots[slot].idle_slot_node);
	spin_unlock_irqrestore(&ksm->idle_slots_lock, flags);
}

static int find_keyslot(struct keyslot_manager *ksm,
			const struct blk_crypto_key *key)
{
	const struct hlist_head *head = hash_bucket_for_key(ksm, key);
	const struct keyslot *slotp;

	hlist_for_each_entry(slotp, head, hash_node) {
		if (slotp->key.hash == key->hash &&
		    slotp->key.crypto_mode == key->crypto_mode &&
		    slotp->key.size == key->size &&
		    slotp->key.data_unit_size == key->data_unit_size &&
		    !crypto_memneq(slotp->key.raw, key->raw, key->size))
			return slotp - ksm->slots;
	}
	return -ENOKEY;
}

static int find_and_grab_keyslot(struct keyslot_manager *ksm,
				 const struct blk_crypto_key *key)
{
	int slot;

	slot = find_keyslot(ksm, key);
	if (slot < 0)
		return slot;
	if (atomic_inc_return(&ksm->slots[slot].slot_refs) == 1) {
		/* Took first reference to this slot; remove it from LRU list */
		remove_slot_from_lru_list(ksm, slot);
=======
	slot_hashtable_size = roundup_pow_of_two(num_slots);
	/*
	 * hash_ptr() assumes bits != 0, so ensure the hash table has at least 2
	 * buckets.  This only makes a difference when there is only 1 keyslot.
	 */
	if (slot_hashtable_size < 2)
		slot_hashtable_size = 2;

	ksm->log_slot_ht_size = ilog2(slot_hashtable_size);
	ksm->slot_hashtable = kvmalloc_array(slot_hashtable_size,
					     sizeof(ksm->slot_hashtable[0]),
					     GFP_KERNEL);
	if (!ksm->slot_hashtable)
		goto err_destroy_ksm;
	for (i = 0; i < slot_hashtable_size; i++)
		INIT_HLIST_HEAD(&ksm->slot_hashtable[i]);

	return 0;

err_destroy_ksm:
	blk_ksm_destroy(ksm);
	return -ENOMEM;
}
EXPORT_SYMBOL_GPL(blk_ksm_init);

static void blk_ksm_destroy_callback(void *ksm)
{
	blk_ksm_destroy(ksm);
}

/**
 * devm_blk_ksm_init() - Resource-managed blk_ksm_init()
 * @dev: The device which owns the blk_keyslot_manager.
 * @ksm: The blk_keyslot_manager to initialize.
 * @num_slots: The number of key slots to manage.
 *
 * Like blk_ksm_init(), but causes blk_ksm_destroy() to be called automatically
 * on driver detach.
 *
 * Return: 0 on success, or else a negative error code.
 */
int devm_blk_ksm_init(struct device *dev, struct blk_keyslot_manager *ksm,
		      unsigned int num_slots)
{
	int err = blk_ksm_init(ksm, num_slots);

	if (err)
		return err;

	return devm_add_action_or_reset(dev, blk_ksm_destroy_callback, ksm);
}
EXPORT_SYMBOL_GPL(devm_blk_ksm_init);

static inline struct hlist_head *
blk_ksm_hash_bucket_for_key(struct blk_keyslot_manager *ksm,
			    const struct blk_crypto_key *key)
{
	return &ksm->slot_hashtable[hash_ptr(key, ksm->log_slot_ht_size)];
}

static void blk_ksm_remove_slot_from_lru_list(struct blk_ksm_keyslot *slot)
{
	struct blk_keyslot_manager *ksm = slot->ksm;
	unsigned long flags;

	spin_lock_irqsave(&ksm->idle_slots_lock, flags);
	list_del(&slot->idle_slot_node);
	spin_unlock_irqrestore(&ksm->idle_slots_lock, flags);
}

static struct blk_ksm_keyslot *blk_ksm_find_keyslot(
					struct blk_keyslot_manager *ksm,
					const struct blk_crypto_key *key)
{
	const struct hlist_head *head = blk_ksm_hash_bucket_for_key(ksm, key);
	struct blk_ksm_keyslot *slotp;

	hlist_for_each_entry(slotp, head, hash_node) {
		if (slotp->key == key)
			return slotp;
	}
	return NULL;
}

static struct blk_ksm_keyslot *blk_ksm_find_and_grab_keyslot(
					struct blk_keyslot_manager *ksm,
					const struct blk_crypto_key *key)
{
	struct blk_ksm_keyslot *slot;

	slot = blk_ksm_find_keyslot(ksm, key);
	if (!slot)
		return NULL;
	if (atomic_inc_return(&slot->slot_refs) == 1) {
		/* Took first reference to this slot; remove it from LRU list */
		blk_ksm_remove_slot_from_lru_list(slot);
>>>>>>> upstream/android-13
	}
	return slot;
}

<<<<<<< HEAD
/**
 * keyslot_manager_get_slot_for_key() - Program a key into a keyslot.
 * @ksm: The keyslot manager to program the key into.
 * @key: Pointer to the key object to program, including the raw key, crypto
 *	 mode, and data unit size.
=======
unsigned int blk_ksm_get_slot_idx(struct blk_ksm_keyslot *slot)
{
	return slot - slot->ksm->slots;
}
EXPORT_SYMBOL_GPL(blk_ksm_get_slot_idx);

/**
 * blk_ksm_get_slot_for_key() - Program a key into a keyslot.
 * @ksm: The keyslot manager to program the key into.
 * @key: Pointer to the key object to program, including the raw key, crypto
 *	 mode, and data unit size.
 * @slot_ptr: A pointer to return the pointer of the allocated keyslot.
>>>>>>> upstream/android-13
 *
 * Get a keyslot that's been programmed with the specified key.  If one already
 * exists, return it with incremented refcount.  Otherwise, wait for a keyslot
 * to become idle and program it.
 *
 * Context: Process context. Takes and releases ksm->lock.
<<<<<<< HEAD
 * Return: The keyslot on success, else a -errno value.
 */
int keyslot_manager_get_slot_for_key(struct keyslot_manager *ksm,
				     const struct blk_crypto_key *key)
{
	int slot;
	int err;
	struct keyslot *idle_slot;

	if (keyslot_manager_is_passthrough(ksm))
		return 0;

	down_read(&ksm->lock);
	slot = find_and_grab_keyslot(ksm, key);
	up_read(&ksm->lock);
	if (slot != -ENOKEY)
		return slot;

	for (;;) {
		keyslot_manager_hw_enter(ksm);
		slot = find_and_grab_keyslot(ksm, key);
		if (slot != -ENOKEY) {
			keyslot_manager_hw_exit(ksm);
			return slot;
=======
 * Return: BLK_STS_OK on success (and keyslot is set to the pointer of the
 *	   allocated keyslot), or some other blk_status_t otherwise (and
 *	   keyslot is set to NULL).
 */
blk_status_t blk_ksm_get_slot_for_key(struct blk_keyslot_manager *ksm,
				      const struct blk_crypto_key *key,
				      struct blk_ksm_keyslot **slot_ptr)
{
	struct blk_ksm_keyslot *slot;
	int slot_idx;
	int err;

	*slot_ptr = NULL;

	if (blk_ksm_is_passthrough(ksm))
		return BLK_STS_OK;

	down_read(&ksm->lock);
	slot = blk_ksm_find_and_grab_keyslot(ksm, key);
	up_read(&ksm->lock);
	if (slot)
		goto success;

	for (;;) {
		blk_ksm_hw_enter(ksm);
		slot = blk_ksm_find_and_grab_keyslot(ksm, key);
		if (slot) {
			blk_ksm_hw_exit(ksm);
			goto success;
>>>>>>> upstream/android-13
		}

		/*
		 * If we're here, that means there wasn't a slot that was
		 * already programmed with the key. So try to program it.
		 */
		if (!list_empty(&ksm->idle_slots))
			break;

<<<<<<< HEAD
		keyslot_manager_hw_exit(ksm);
=======
		blk_ksm_hw_exit(ksm);
>>>>>>> upstream/android-13
		wait_event(ksm->idle_slots_wait_queue,
			   !list_empty(&ksm->idle_slots));
	}

<<<<<<< HEAD
	idle_slot = list_first_entry(&ksm->idle_slots, struct keyslot,
					     idle_slot_node);
	slot = idle_slot - ksm->slots;

	err = ksm->ksm_ll_ops.keyslot_program(ksm, key, slot);
	if (err) {
		wake_up(&ksm->idle_slots_wait_queue);
		keyslot_manager_hw_exit(ksm);
		return err;
	}

	/* Move this slot to the hash list for the new key. */
	if (idle_slot->key.crypto_mode != BLK_ENCRYPTION_MODE_INVALID)
		hlist_del(&idle_slot->hash_node);
	hlist_add_head(&idle_slot->hash_node, hash_bucket_for_key(ksm, key));

	atomic_set(&idle_slot->slot_refs, 1);
	idle_slot->key = *key;

	remove_slot_from_lru_list(ksm, slot);

	keyslot_manager_hw_exit(ksm);
	return slot;
}

/**
 * keyslot_manager_get_slot() - Increment the refcount on the specified slot.
 * @ksm: The keyslot manager that we want to modify.
 * @slot: The slot to increment the refcount of.
 *
 * This function assumes that there is already an active reference to that slot
 * and simply increments the refcount. This is useful when cloning a bio that
 * already has a reference to a keyslot, and we want the cloned bio to also have
 * its own reference.
 *
 * Context: Any context.
 */
void keyslot_manager_get_slot(struct keyslot_manager *ksm, unsigned int slot)
{
	if (keyslot_manager_is_passthrough(ksm))
		return;

	if (WARN_ON(slot >= ksm->num_slots))
		return;

	WARN_ON(atomic_inc_return(&ksm->slots[slot].slot_refs) < 2);
}

/**
 * keyslot_manager_put_slot() - Release a reference to a slot
 * @ksm: The keyslot manager to release the reference from.
 * @slot: The slot to release the reference from.
 *
 * Context: Any context.
 */
void keyslot_manager_put_slot(struct keyslot_manager *ksm, unsigned int slot)
{
	unsigned long flags;

	if (keyslot_manager_is_passthrough(ksm))
		return;

	if (WARN_ON(slot >= ksm->num_slots))
		return;

	if (atomic_dec_and_lock_irqsave(&ksm->slots[slot].slot_refs,
					&ksm->idle_slots_lock, flags)) {
		list_add_tail(&ksm->slots[slot].idle_slot_node,
			      &ksm->idle_slots);
=======
	slot = list_first_entry(&ksm->idle_slots, struct blk_ksm_keyslot,
				idle_slot_node);
	slot_idx = blk_ksm_get_slot_idx(slot);

	err = ksm->ksm_ll_ops.keyslot_program(ksm, key, slot_idx);
	if (err) {
		wake_up(&ksm->idle_slots_wait_queue);
		blk_ksm_hw_exit(ksm);
		return errno_to_blk_status(err);
	}

	/* Move this slot to the hash list for the new key. */
	if (slot->key)
		hlist_del(&slot->hash_node);
	slot->key = key;
	hlist_add_head(&slot->hash_node, blk_ksm_hash_bucket_for_key(ksm, key));

	atomic_set(&slot->slot_refs, 1);

	blk_ksm_remove_slot_from_lru_list(slot);

	blk_ksm_hw_exit(ksm);
success:
	*slot_ptr = slot;
	return BLK_STS_OK;
}

/**
 * blk_ksm_put_slot() - Release a reference to a slot
 * @slot: The keyslot to release the reference of.
 *
 * Context: Any context.
 */
void blk_ksm_put_slot(struct blk_ksm_keyslot *slot)
{
	struct blk_keyslot_manager *ksm;
	unsigned long flags;

	if (!slot)
		return;

	ksm = slot->ksm;

	if (atomic_dec_and_lock_irqsave(&slot->slot_refs,
					&ksm->idle_slots_lock, flags)) {
		list_add_tail(&slot->idle_slot_node, &ksm->idle_slots);
>>>>>>> upstream/android-13
		spin_unlock_irqrestore(&ksm->idle_slots_lock, flags);
		wake_up(&ksm->idle_slots_wait_queue);
	}
}

/**
<<<<<<< HEAD
 * keyslot_manager_crypto_mode_supported() - Find out if a crypto_mode /
 *					     data unit size / is_hw_wrapped_key
 *					     combination is supported by a ksm.
 * @ksm: The keyslot manager to check
 * @crypto_mode: The crypto mode to check for.
 * @dun_bytes: The number of bytes that will be used to specify the DUN
 * @data_unit_size: The data_unit_size for the mode.
 * @is_hw_wrapped_key: Whether a hardware-wrapped key will be used.
 *
 * Calls and returns the result of the crypto_mode_supported function specified
 * by the ksm.
 *
 * Context: Process context.
 * Return: Whether or not this ksm supports the specified crypto settings.
 */
bool keyslot_manager_crypto_mode_supported(struct keyslot_manager *ksm,
					   enum blk_crypto_mode_num crypto_mode,
					   unsigned int dun_bytes,
					   unsigned int data_unit_size,
					   bool is_hw_wrapped_key)
{
	if (!ksm)
		return false;
	if (WARN_ON(crypto_mode >= BLK_ENCRYPTION_MODE_MAX))
		return false;
	if (WARN_ON(!is_power_of_2(data_unit_size)))
		return false;
	if (is_hw_wrapped_key) {
=======
 * blk_ksm_crypto_cfg_supported() - Find out if a crypto configuration is
 *				    supported by a ksm.
 * @ksm: The keyslot manager to check
 * @cfg: The crypto configuration to check for.
 *
 * Checks for crypto_mode/data unit size/dun bytes support.
 *
 * Return: Whether or not this ksm supports the specified crypto config.
 */
bool blk_ksm_crypto_cfg_supported(struct blk_keyslot_manager *ksm,
				  const struct blk_crypto_config *cfg)
{
	if (!ksm)
		return false;
	if (!(ksm->crypto_modes_supported[cfg->crypto_mode] &
	      cfg->data_unit_size))
		return false;
	if (ksm->max_dun_bytes_supported < cfg->dun_bytes)
		return false;
	if (cfg->is_hw_wrapped) {
>>>>>>> upstream/android-13
		if (!(ksm->features & BLK_CRYPTO_FEATURE_WRAPPED_KEYS))
			return false;
	} else {
		if (!(ksm->features & BLK_CRYPTO_FEATURE_STANDARD_KEYS))
			return false;
	}
<<<<<<< HEAD
	if (!(ksm->crypto_mode_supported[crypto_mode] & data_unit_size))
		return false;

	return ksm->max_dun_bytes_supported >= dun_bytes;
}

/**
 * keyslot_manager_evict_key() - Evict a key from the lower layer device.
=======
	return true;
}

/**
 * blk_ksm_evict_key() - Evict a key from the lower layer device.
>>>>>>> upstream/android-13
 * @ksm: The keyslot manager to evict from
 * @key: The key to evict
 *
 * Find the keyslot that the specified key was programmed into, and evict that
<<<<<<< HEAD
 * slot from the lower layer device if that slot is not currently in use.
 *
 * Context: Process context. Takes and releases ksm->lock.
 * Return: 0 on success, -EBUSY if the key is still in use, or another
 *	   -errno value on other error.
 */
int keyslot_manager_evict_key(struct keyslot_manager *ksm,
			      const struct blk_crypto_key *key)
{
	int slot;
	int err;
	struct keyslot *slotp;

	if (keyslot_manager_is_passthrough(ksm)) {
		if (ksm->ksm_ll_ops.keyslot_evict) {
			keyslot_manager_hw_enter(ksm);
			err = ksm->ksm_ll_ops.keyslot_evict(ksm, key, -1);
			keyslot_manager_hw_exit(ksm);
=======
 * slot from the lower layer device. The slot must not be in use by any
 * in-flight IO when this function is called.
 *
 * Context: Process context. Takes and releases ksm->lock.
 * Return: 0 on success or if there's no keyslot with the specified key, -EBUSY
 *	   if the keyslot is still in use, or another -errno value on other
 *	   error.
 */
int blk_ksm_evict_key(struct blk_keyslot_manager *ksm,
		      const struct blk_crypto_key *key)
{
	struct blk_ksm_keyslot *slot;
	int err = 0;

	if (blk_ksm_is_passthrough(ksm)) {
		if (ksm->ksm_ll_ops.keyslot_evict) {
			blk_ksm_hw_enter(ksm);
			err = ksm->ksm_ll_ops.keyslot_evict(ksm, key, -1);
			blk_ksm_hw_exit(ksm);
>>>>>>> upstream/android-13
			return err;
		}
		return 0;
	}

<<<<<<< HEAD
	keyslot_manager_hw_enter(ksm);

	slot = find_keyslot(ksm, key);
	if (slot < 0) {
		err = slot;
		goto out_unlock;
	}
	slotp = &ksm->slots[slot];

	if (atomic_read(&slotp->slot_refs) != 0) {
		err = -EBUSY;
		goto out_unlock;
	}
	err = ksm->ksm_ll_ops.keyslot_evict(ksm, key, slot);
	if (err)
		goto out_unlock;

	hlist_del(&slotp->hash_node);
	memzero_explicit(&slotp->key, sizeof(slotp->key));
	err = 0;
out_unlock:
	keyslot_manager_hw_exit(ksm);
=======
	blk_ksm_hw_enter(ksm);
	slot = blk_ksm_find_keyslot(ksm, key);
	if (!slot)
		goto out_unlock;

	if (WARN_ON_ONCE(atomic_read(&slot->slot_refs) != 0)) {
		err = -EBUSY;
		goto out_unlock;
	}
	err = ksm->ksm_ll_ops.keyslot_evict(ksm, key,
					    blk_ksm_get_slot_idx(slot));
	if (err)
		goto out_unlock;

	hlist_del(&slot->hash_node);
	slot->key = NULL;
	err = 0;
out_unlock:
	blk_ksm_hw_exit(ksm);
>>>>>>> upstream/android-13
	return err;
}

/**
<<<<<<< HEAD
 * keyslot_manager_reprogram_all_keys() - Re-program all keyslots.
=======
 * blk_ksm_reprogram_all_keys() - Re-program all keyslots.
>>>>>>> upstream/android-13
 * @ksm: The keyslot manager
 *
 * Re-program all keyslots that are supposed to have a key programmed.  This is
 * intended only for use by drivers for hardware that loses its keys on reset.
 *
 * Context: Process context. Takes and releases ksm->lock.
 */
<<<<<<< HEAD
void keyslot_manager_reprogram_all_keys(struct keyslot_manager *ksm)
{
	unsigned int slot;

	if (WARN_ON(keyslot_manager_is_passthrough(ksm)))
=======
void blk_ksm_reprogram_all_keys(struct blk_keyslot_manager *ksm)
{
	unsigned int slot;

	if (blk_ksm_is_passthrough(ksm))
>>>>>>> upstream/android-13
		return;

	/* This is for device initialization, so don't resume the device */
	down_write(&ksm->lock);
	for (slot = 0; slot < ksm->num_slots; slot++) {
<<<<<<< HEAD
		const struct keyslot *slotp = &ksm->slots[slot];
		int err;

		if (slotp->key.crypto_mode == BLK_ENCRYPTION_MODE_INVALID)
			continue;

		err = ksm->ksm_ll_ops.keyslot_program(ksm, &slotp->key, slot);
=======
		const struct blk_crypto_key *key = ksm->slots[slot].key;
		int err;

		if (!key)
			continue;

		err = ksm->ksm_ll_ops.keyslot_program(ksm, key, slot);
>>>>>>> upstream/android-13
		WARN_ON(err);
	}
	up_write(&ksm->lock);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(keyslot_manager_reprogram_all_keys);

/**
 * keyslot_manager_private() - return the private data stored with ksm
 * @ksm: The keyslot manager
 *
 * Returns the private data passed to the ksm when it was created.
 */
void *keyslot_manager_private(struct keyslot_manager *ksm)
{
	return ksm->ll_priv_data;
}
EXPORT_SYMBOL_GPL(keyslot_manager_private);

void keyslot_manager_destroy(struct keyslot_manager *ksm)
{
	if (ksm) {
		kvfree(ksm->slot_hashtable);
		memzero_explicit(ksm, struct_size(ksm, slots, ksm->num_slots));
		kvfree(ksm);
	}
}
EXPORT_SYMBOL_GPL(keyslot_manager_destroy);

/**
 * keyslot_manager_create_passthrough() - Create a passthrough keyslot manager
 * @dev: Device for runtime power management (NULL if none)
 * @ksm_ll_ops: The struct keyslot_mgmt_ll_ops
 * @features: Bitmask of BLK_CRYPTO_FEATURE_* flags
 * @crypto_mode_supported: Bitmasks for supported encryption modes
 * @ll_priv_data: Private data passed as is to the functions in ksm_ll_ops.
 *
 * Allocate memory for and initialize a passthrough keyslot manager.
 * Called by e.g. storage drivers to set up a keyslot manager in their
 * request_queue, when the storage driver wants to manage its keys by itself.
 * This is useful for inline encryption hardware that don't have a small fixed
 * number of keyslots, and for layered devices.
 *
 * See keyslot_manager_create() for more details about the parameters.
 *
 * Context: This function may sleep
 * Return: Pointer to constructed keyslot manager or NULL on error.
 */
struct keyslot_manager *keyslot_manager_create_passthrough(
	struct device *dev,
	const struct keyslot_mgmt_ll_ops *ksm_ll_ops,
	unsigned int features,
	const unsigned int crypto_mode_supported[BLK_ENCRYPTION_MODE_MAX],
	void *ll_priv_data)
{
	struct keyslot_manager *ksm;

	ksm = kzalloc(sizeof(*ksm), GFP_KERNEL);
	if (!ksm)
		return NULL;

	ksm->ksm_ll_ops = *ksm_ll_ops;
	ksm->features = features;
	memcpy(ksm->crypto_mode_supported, crypto_mode_supported,
	       sizeof(ksm->crypto_mode_supported));
	ksm->max_dun_bytes_supported = BLK_CRYPTO_MAX_IV_SIZE;
	ksm->ll_priv_data = ll_priv_data;
	keyslot_manager_set_dev(ksm, dev);

	init_rwsem(&ksm->lock);

	return ksm;
}
EXPORT_SYMBOL_GPL(keyslot_manager_create_passthrough);

/**
 * keyslot_manager_intersect_modes() - restrict supported modes by child device
 * @parent: The keyslot manager for parent device
 * @child: The keyslot manager for child device, or NULL
 *
 * Clear any crypto mode support bits in @parent that aren't set in @child.
 * If @child is NULL, then all parent bits are cleared.
 *
 * Only use this when setting up the keyslot manager for a layered device,
 * before it's been exposed yet.
 */
void keyslot_manager_intersect_modes(struct keyslot_manager *parent,
				     const struct keyslot_manager *child)
{
	if (child) {
		unsigned int i;

		parent->features &= child->features;
		parent->max_dun_bytes_supported =
			min(parent->max_dun_bytes_supported,
			    child->max_dun_bytes_supported);
		for (i = 0; i < ARRAY_SIZE(child->crypto_mode_supported); i++) {
			parent->crypto_mode_supported[i] &=
				child->crypto_mode_supported[i];
		}
	} else {
		parent->features = 0;
		parent->max_dun_bytes_supported = 0;
		memset(parent->crypto_mode_supported, 0,
		       sizeof(parent->crypto_mode_supported));
	}
}
EXPORT_SYMBOL_GPL(keyslot_manager_intersect_modes);

/**
 * keyslot_manager_derive_raw_secret() - Derive software secret from wrapped key
=======
EXPORT_SYMBOL_GPL(blk_ksm_reprogram_all_keys);

void blk_ksm_destroy(struct blk_keyslot_manager *ksm)
{
	if (!ksm)
		return;
	kvfree(ksm->slot_hashtable);
	kvfree_sensitive(ksm->slots, sizeof(ksm->slots[0]) * ksm->num_slots);
	memzero_explicit(ksm, sizeof(*ksm));
}
EXPORT_SYMBOL_GPL(blk_ksm_destroy);

bool blk_ksm_register(struct blk_keyslot_manager *ksm, struct request_queue *q)
{
	if (blk_integrity_queue_supports_integrity(q)) {
		pr_warn("Integrity and hardware inline encryption are not supported together. Disabling hardware inline encryption.\n");
		return false;
	}
	q->ksm = ksm;
	return true;
}
EXPORT_SYMBOL_GPL(blk_ksm_register);

void blk_ksm_unregister(struct request_queue *q)
{
	q->ksm = NULL;
}

/**
 * blk_ksm_derive_raw_secret() - Derive software secret from wrapped key
>>>>>>> upstream/android-13
 * @ksm: The keyslot manager
 * @wrapped_key: The wrapped key
 * @wrapped_key_size: Size of the wrapped key in bytes
 * @secret: (output) the software secret
 * @secret_size: (output) the number of secret bytes to derive
 *
 * Given a hardware-wrapped key, ask the hardware to derive a secret which
 * software can use for cryptographic tasks other than inline encryption.  The
 * derived secret is guaranteed to be cryptographically isolated from the key
 * with which any inline encryption with this wrapped key would actually be
 * done.  I.e., both will be derived from the unwrapped key.
 *
 * Return: 0 on success, -EOPNOTSUPP if hardware-wrapped keys are unsupported,
 *	   or another -errno code.
 */
<<<<<<< HEAD
int keyslot_manager_derive_raw_secret(struct keyslot_manager *ksm,
				      const u8 *wrapped_key,
				      unsigned int wrapped_key_size,
				      u8 *secret, unsigned int secret_size)
=======
int blk_ksm_derive_raw_secret(struct blk_keyslot_manager *ksm,
			      const u8 *wrapped_key,
			      unsigned int wrapped_key_size,
			      u8 *secret, unsigned int secret_size)
>>>>>>> upstream/android-13
{
	int err;

	if (ksm->ksm_ll_ops.derive_raw_secret) {
<<<<<<< HEAD
		keyslot_manager_hw_enter(ksm);
		err = ksm->ksm_ll_ops.derive_raw_secret(ksm, wrapped_key,
							wrapped_key_size,
							secret, secret_size);
		keyslot_manager_hw_exit(ksm);
=======
		blk_ksm_hw_enter(ksm);
		err = ksm->ksm_ll_ops.derive_raw_secret(ksm, wrapped_key,
							wrapped_key_size,
							secret, secret_size);
		blk_ksm_hw_exit(ksm);
>>>>>>> upstream/android-13
	} else {
		err = -EOPNOTSUPP;
	}

	return err;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(keyslot_manager_derive_raw_secret);

/**
 * ksm_lock() - set one-depth nesting of lock class
 * @flags: now, it's only support one depth
 *
 * Some scenarios ksm->lock will be nest such as DM stack layer,
 * although DM's is different with lower device driver's ksm->lock,
 * lockdep recognizes them as a same one, then will trigger deadlock
 * detection, set another lock sub-class could avoid it.
 *
 */
inline void ksm_flock(struct keyslot_manager *ksm, unsigned int flags)
{
	ksm->lock_flags = flags;
}
EXPORT_SYMBOL_GPL(ksm_flock);
=======
EXPORT_SYMBOL_GPL(blk_ksm_derive_raw_secret);

/**
 * blk_ksm_intersect_modes() - restrict supported modes by child device
 * @parent: The keyslot manager for parent device
 * @child: The keyslot manager for child device, or NULL
 *
 * Clear any crypto mode support bits in @parent that aren't set in @child.
 * If @child is NULL, then all parent bits are cleared.
 *
 * Only use this when setting up the keyslot manager for a layered device,
 * before it's been exposed yet.
 */
void blk_ksm_intersect_modes(struct blk_keyslot_manager *parent,
			     const struct blk_keyslot_manager *child)
{
	if (child) {
		unsigned int i;

		parent->max_dun_bytes_supported =
			min(parent->max_dun_bytes_supported,
			    child->max_dun_bytes_supported);
		for (i = 0; i < ARRAY_SIZE(child->crypto_modes_supported);
		     i++) {
			parent->crypto_modes_supported[i] &=
				child->crypto_modes_supported[i];
		}
		parent->features &= child->features;
	} else {
		parent->max_dun_bytes_supported = 0;
		memset(parent->crypto_modes_supported, 0,
		       sizeof(parent->crypto_modes_supported));
		parent->features = 0;
	}
}
EXPORT_SYMBOL_GPL(blk_ksm_intersect_modes);

/**
 * blk_ksm_is_superset() - Check if a KSM supports a superset of crypto modes
 *			   and DUN bytes that another KSM supports. Here,
 *			   "superset" refers to the mathematical meaning of the
 *			   word - i.e. if two KSMs have the *same* capabilities,
 *			   they *are* considered supersets of each other.
 * @ksm_superset: The KSM that we want to verify is a superset
 * @ksm_subset: The KSM that we want to verify is a subset
 *
 * Return: True if @ksm_superset supports a superset of the crypto modes and DUN
 *	   bytes that @ksm_subset supports.
 */
bool blk_ksm_is_superset(struct blk_keyslot_manager *ksm_superset,
			 struct blk_keyslot_manager *ksm_subset)
{
	int i;

	if (!ksm_subset)
		return true;

	if (!ksm_superset)
		return false;

	for (i = 0; i < ARRAY_SIZE(ksm_superset->crypto_modes_supported); i++) {
		if (ksm_subset->crypto_modes_supported[i] &
		    (~ksm_superset->crypto_modes_supported[i])) {
			return false;
		}
	}

	if (ksm_subset->max_dun_bytes_supported >
	    ksm_superset->max_dun_bytes_supported) {
		return false;
	}

	if (ksm_subset->features & ~ksm_superset->features)
		return false;

	return true;
}
EXPORT_SYMBOL_GPL(blk_ksm_is_superset);

/**
 * blk_ksm_update_capabilities() - Update the restrictions of a KSM to those of
 *				   another KSM
 * @target_ksm: The KSM whose restrictions to update.
 * @reference_ksm: The KSM to whose restrictions this function will update
 *		   @target_ksm's restrictions to.
 *
 * Blk-crypto requires that crypto capabilities that were
 * advertised when a bio was created continue to be supported by the
 * device until that bio is ended. This is turn means that a device cannot
 * shrink its advertised crypto capabilities without any explicit
 * synchronization with upper layers. So if there's no such explicit
 * synchronization, @reference_ksm must support all the crypto capabilities that
 * @target_ksm does
 * (i.e. we need blk_ksm_is_superset(@reference_ksm, @target_ksm) == true).
 *
 * Note also that as long as the crypto capabilities are being expanded, the
 * order of updates becoming visible is not important because it's alright
 * for blk-crypto to see stale values - they only cause blk-crypto to
 * believe that a crypto capability isn't supported when it actually is (which
 * might result in blk-crypto-fallback being used if available, or the bio being
 * failed).
 */
void blk_ksm_update_capabilities(struct blk_keyslot_manager *target_ksm,
				 struct blk_keyslot_manager *reference_ksm)
{
	memcpy(target_ksm->crypto_modes_supported,
	       reference_ksm->crypto_modes_supported,
	       sizeof(target_ksm->crypto_modes_supported));

	target_ksm->max_dun_bytes_supported =
				reference_ksm->max_dun_bytes_supported;

	target_ksm->features = reference_ksm->features;
}
EXPORT_SYMBOL_GPL(blk_ksm_update_capabilities);

/**
 * blk_ksm_init_passthrough() - Init a passthrough keyslot manager
 * @ksm: The keyslot manager to init
 *
 * Initialize a passthrough keyslot manager.
 * Called by e.g. storage drivers to set up a keyslot manager in their
 * request_queue, when the storage driver wants to manage its keys by itself.
 * This is useful for inline encryption hardware that doesn't have the concept
 * of keyslots, and for layered devices.
 */
void blk_ksm_init_passthrough(struct blk_keyslot_manager *ksm)
{
	memset(ksm, 0, sizeof(*ksm));
	init_rwsem(&ksm->lock);
}
EXPORT_SYMBOL_GPL(blk_ksm_init_passthrough);
>>>>>>> upstream/android-13
