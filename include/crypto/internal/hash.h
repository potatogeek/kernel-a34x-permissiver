<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> upstream/android-13
/*
 * Hash algorithms.
 * 
 * Copyright (c) 2008 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
=======
>>>>>>> upstream/android-13
 */

#ifndef _CRYPTO_INTERNAL_HASH_H
#define _CRYPTO_INTERNAL_HASH_H

#include <crypto/algapi.h>
#include <crypto/hash.h>

struct ahash_request;
struct scatterlist;

struct crypto_hash_walk {
	char *data;

	unsigned int offset;
	unsigned int alignmask;

	struct page *pg;
	unsigned int entrylen;

	unsigned int total;
	struct scatterlist *sg;

	unsigned int flags;
};

struct ahash_instance {
<<<<<<< HEAD
	struct ahash_alg alg;
};

struct shash_instance {
	struct shash_alg alg;
=======
	void (*free)(struct ahash_instance *inst);
	union {
		struct {
			char head[offsetof(struct ahash_alg, halg.base)];
			struct crypto_instance base;
		} s;
		struct ahash_alg alg;
	};
};

struct shash_instance {
	void (*free)(struct shash_instance *inst);
	union {
		struct {
			char head[offsetof(struct shash_alg, base)];
			struct crypto_instance base;
		} s;
		struct shash_alg alg;
	};
>>>>>>> upstream/android-13
};

struct crypto_ahash_spawn {
	struct crypto_spawn base;
};

struct crypto_shash_spawn {
	struct crypto_spawn base;
};

<<<<<<< HEAD
extern const struct crypto_type crypto_ahash_type;

int crypto_hash_walk_done(struct crypto_hash_walk *walk, int err);
int crypto_hash_walk_first(struct ahash_request *req,
			   struct crypto_hash_walk *walk);
int crypto_ahash_walk_first(struct ahash_request *req,
			   struct crypto_hash_walk *walk);

static inline int crypto_ahash_walk_done(struct crypto_hash_walk *walk,
					 int err)
{
	return crypto_hash_walk_done(walk, err);
}
=======
int crypto_hash_walk_done(struct crypto_hash_walk *walk, int err);
int crypto_hash_walk_first(struct ahash_request *req,
			   struct crypto_hash_walk *walk);
>>>>>>> upstream/android-13

static inline int crypto_hash_walk_last(struct crypto_hash_walk *walk)
{
	return !(walk->entrylen | walk->total);
}

<<<<<<< HEAD
static inline int crypto_ahash_walk_last(struct crypto_hash_walk *walk)
{
	return crypto_hash_walk_last(walk);
}

int crypto_register_ahash(struct ahash_alg *alg);
int crypto_unregister_ahash(struct ahash_alg *alg);
=======
int crypto_register_ahash(struct ahash_alg *alg);
void crypto_unregister_ahash(struct ahash_alg *alg);
>>>>>>> upstream/android-13
int crypto_register_ahashes(struct ahash_alg *algs, int count);
void crypto_unregister_ahashes(struct ahash_alg *algs, int count);
int ahash_register_instance(struct crypto_template *tmpl,
			    struct ahash_instance *inst);
<<<<<<< HEAD
void ahash_free_instance(struct crypto_instance *inst);

int shash_no_setkey(struct crypto_shash *tfm, const u8 *key,
		    unsigned int keylen);

static inline bool crypto_shash_alg_has_setkey(struct shash_alg *alg)
{
	return alg->setkey != shash_no_setkey;
=======

bool crypto_shash_alg_has_setkey(struct shash_alg *alg);

static inline bool crypto_shash_alg_needs_key(struct shash_alg *alg)
{
	return crypto_shash_alg_has_setkey(alg) &&
		!(alg->base.cra_flags & CRYPTO_ALG_OPTIONAL_KEY);
>>>>>>> upstream/android-13
}

bool crypto_hash_alg_has_setkey(struct hash_alg_common *halg);

<<<<<<< HEAD
int crypto_init_ahash_spawn(struct crypto_ahash_spawn *spawn,
			    struct hash_alg_common *alg,
			    struct crypto_instance *inst);
=======
int crypto_grab_ahash(struct crypto_ahash_spawn *spawn,
		      struct crypto_instance *inst,
		      const char *name, u32 type, u32 mask);
>>>>>>> upstream/android-13

static inline void crypto_drop_ahash(struct crypto_ahash_spawn *spawn)
{
	crypto_drop_spawn(&spawn->base);
}

<<<<<<< HEAD
struct hash_alg_common *ahash_attr_alg(struct rtattr *rta, u32 type, u32 mask);

int crypto_register_shash(struct shash_alg *alg);
int crypto_unregister_shash(struct shash_alg *alg);
int crypto_register_shashes(struct shash_alg *algs, int count);
int crypto_unregister_shashes(struct shash_alg *algs, int count);
int shash_register_instance(struct crypto_template *tmpl,
			    struct shash_instance *inst);
void shash_free_instance(struct crypto_instance *inst);

int crypto_init_shash_spawn(struct crypto_shash_spawn *spawn,
			    struct shash_alg *alg,
			    struct crypto_instance *inst);
=======
static inline struct hash_alg_common *crypto_spawn_ahash_alg(
	struct crypto_ahash_spawn *spawn)
{
	return __crypto_hash_alg_common(spawn->base.alg);
}

int crypto_register_shash(struct shash_alg *alg);
void crypto_unregister_shash(struct shash_alg *alg);
int crypto_register_shashes(struct shash_alg *algs, int count);
void crypto_unregister_shashes(struct shash_alg *algs, int count);
int shash_register_instance(struct crypto_template *tmpl,
			    struct shash_instance *inst);
void shash_free_singlespawn_instance(struct shash_instance *inst);

int crypto_grab_shash(struct crypto_shash_spawn *spawn,
		      struct crypto_instance *inst,
		      const char *name, u32 type, u32 mask);
>>>>>>> upstream/android-13

static inline void crypto_drop_shash(struct crypto_shash_spawn *spawn)
{
	crypto_drop_spawn(&spawn->base);
}

<<<<<<< HEAD
struct shash_alg *shash_attr_alg(struct rtattr *rta, u32 type, u32 mask);
=======
static inline struct shash_alg *crypto_spawn_shash_alg(
	struct crypto_shash_spawn *spawn)
{
	return __crypto_shash_alg(spawn->base.alg);
}
>>>>>>> upstream/android-13

int shash_ahash_update(struct ahash_request *req, struct shash_desc *desc);
int shash_ahash_finup(struct ahash_request *req, struct shash_desc *desc);
int shash_ahash_digest(struct ahash_request *req, struct shash_desc *desc);

int crypto_init_shash_ops_async(struct crypto_tfm *tfm);

static inline void *crypto_ahash_ctx(struct crypto_ahash *tfm)
{
	return crypto_tfm_ctx(crypto_ahash_tfm(tfm));
}

static inline struct ahash_alg *__crypto_ahash_alg(struct crypto_alg *alg)
{
	return container_of(__crypto_hash_alg_common(alg), struct ahash_alg,
			    halg);
}

static inline void crypto_ahash_set_reqsize(struct crypto_ahash *tfm,
					    unsigned int reqsize)
{
	tfm->reqsize = reqsize;
}

static inline struct crypto_instance *ahash_crypto_instance(
	struct ahash_instance *inst)
{
<<<<<<< HEAD
	return container_of(&inst->alg.halg.base, struct crypto_instance, alg);
=======
	return &inst->s.base;
>>>>>>> upstream/android-13
}

static inline struct ahash_instance *ahash_instance(
	struct crypto_instance *inst)
{
<<<<<<< HEAD
	return container_of(&inst->alg, struct ahash_instance, alg.halg.base);
=======
	return container_of(inst, struct ahash_instance, s.base);
}

static inline struct ahash_instance *ahash_alg_instance(
	struct crypto_ahash *ahash)
{
	return ahash_instance(crypto_tfm_alg_instance(&ahash->base));
>>>>>>> upstream/android-13
}

static inline void *ahash_instance_ctx(struct ahash_instance *inst)
{
	return crypto_instance_ctx(ahash_crypto_instance(inst));
}

<<<<<<< HEAD
static inline unsigned int ahash_instance_headroom(void)
{
	return sizeof(struct ahash_alg) - sizeof(struct crypto_alg);
}

static inline struct ahash_instance *ahash_alloc_instance(
	const char *name, struct crypto_alg *alg)
{
	return crypto_alloc_instance2(name, alg, ahash_instance_headroom());
}

=======
>>>>>>> upstream/android-13
static inline void ahash_request_complete(struct ahash_request *req, int err)
{
	req->base.complete(&req->base, err);
}

static inline u32 ahash_request_flags(struct ahash_request *req)
{
	return req->base.flags;
}

static inline struct crypto_ahash *crypto_spawn_ahash(
	struct crypto_ahash_spawn *spawn)
{
	return crypto_spawn_tfm2(&spawn->base);
}

static inline int ahash_enqueue_request(struct crypto_queue *queue,
					     struct ahash_request *request)
{
	return crypto_enqueue_request(queue, &request->base);
}

static inline struct ahash_request *ahash_dequeue_request(
	struct crypto_queue *queue)
{
	return ahash_request_cast(crypto_dequeue_request(queue));
}

<<<<<<< HEAD
static inline int ahash_tfm_in_queue(struct crypto_queue *queue,
					  struct crypto_ahash *tfm)
{
	return crypto_tfm_in_queue(queue, crypto_ahash_tfm(tfm));
}

=======
>>>>>>> upstream/android-13
static inline void *crypto_shash_ctx(struct crypto_shash *tfm)
{
	return crypto_tfm_ctx(&tfm->base);
}

static inline struct crypto_instance *shash_crypto_instance(
	struct shash_instance *inst)
{
<<<<<<< HEAD
	return container_of(&inst->alg.base, struct crypto_instance, alg);
=======
	return &inst->s.base;
>>>>>>> upstream/android-13
}

static inline struct shash_instance *shash_instance(
	struct crypto_instance *inst)
{
<<<<<<< HEAD
	return container_of(__crypto_shash_alg(&inst->alg),
			    struct shash_instance, alg);
=======
	return container_of(inst, struct shash_instance, s.base);
}

static inline struct shash_instance *shash_alg_instance(
	struct crypto_shash *shash)
{
	return shash_instance(crypto_tfm_alg_instance(&shash->base));
>>>>>>> upstream/android-13
}

static inline void *shash_instance_ctx(struct shash_instance *inst)
{
	return crypto_instance_ctx(shash_crypto_instance(inst));
}

<<<<<<< HEAD
static inline struct shash_instance *shash_alloc_instance(
	const char *name, struct crypto_alg *alg)
{
	return crypto_alloc_instance2(name, alg,
				      sizeof(struct shash_alg) - sizeof(*alg));
}

=======
>>>>>>> upstream/android-13
static inline struct crypto_shash *crypto_spawn_shash(
	struct crypto_shash_spawn *spawn)
{
	return crypto_spawn_tfm2(&spawn->base);
}

static inline void *crypto_shash_ctx_aligned(struct crypto_shash *tfm)
{
	return crypto_tfm_ctx_aligned(&tfm->base);
}

static inline struct crypto_shash *__crypto_shash_cast(struct crypto_tfm *tfm)
{
	return container_of(tfm, struct crypto_shash, base);
}

#endif	/* _CRYPTO_INTERNAL_HASH_H */

