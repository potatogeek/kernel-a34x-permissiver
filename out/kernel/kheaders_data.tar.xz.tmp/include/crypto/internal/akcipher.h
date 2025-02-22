
#ifndef _CRYPTO_AKCIPHER_INT_H
#define _CRYPTO_AKCIPHER_INT_H
#include <crypto/akcipher.h>
#include <crypto/algapi.h>

struct akcipher_instance {
	void (*free)(struct akcipher_instance *inst);
	union {
		struct {
			char head[offsetof(struct akcipher_alg, base)];
			struct crypto_instance base;
		} s;
		struct akcipher_alg alg;
	};
};

struct crypto_akcipher_spawn {
	struct crypto_spawn base;
};


static inline void *akcipher_request_ctx(struct akcipher_request *req)
{
	return req->__ctx;
}

static inline void akcipher_set_reqsize(struct crypto_akcipher *akcipher,
					unsigned int reqsize)
{
	crypto_akcipher_alg(akcipher)->reqsize = reqsize;
}

static inline void *akcipher_tfm_ctx(struct crypto_akcipher *tfm)
{
	return tfm->base.__crt_ctx;
}

static inline void akcipher_request_complete(struct akcipher_request *req,
					     int err)
{
	req->base.complete(&req->base, err);
}

static inline const char *akcipher_alg_name(struct crypto_akcipher *tfm)
{
	return crypto_akcipher_tfm(tfm)->__crt_alg->cra_name;
}

static inline struct crypto_instance *akcipher_crypto_instance(
		struct akcipher_instance *inst)
{
	return container_of(&inst->alg.base, struct crypto_instance, alg);
}

static inline struct akcipher_instance *akcipher_instance(
		struct crypto_instance *inst)
{
	return container_of(&inst->alg, struct akcipher_instance, alg.base);
}

static inline struct akcipher_instance *akcipher_alg_instance(
		struct crypto_akcipher *akcipher)
{
	return akcipher_instance(crypto_tfm_alg_instance(&akcipher->base));
}

static inline void *akcipher_instance_ctx(struct akcipher_instance *inst)
{
	return crypto_instance_ctx(akcipher_crypto_instance(inst));
}

static inline void crypto_set_akcipher_spawn(
		struct crypto_akcipher_spawn *spawn,
		struct crypto_instance *inst)
{
	crypto_set_spawn(&spawn->base, inst);
}

int crypto_grab_akcipher(struct crypto_akcipher_spawn *spawn, const char *name,
		u32 type, u32 mask);

static inline struct crypto_akcipher *crypto_spawn_akcipher(
		struct crypto_akcipher_spawn *spawn)
{
	return crypto_spawn_tfm2(&spawn->base);
}

static inline void crypto_drop_akcipher(struct crypto_akcipher_spawn *spawn)
{
	crypto_drop_spawn(&spawn->base);
}

static inline struct akcipher_alg *crypto_spawn_akcipher_alg(
		struct crypto_akcipher_spawn *spawn)
{
	return container_of(spawn->base.alg, struct akcipher_alg, base);
}


int crypto_register_akcipher(struct akcipher_alg *alg);


void crypto_unregister_akcipher(struct akcipher_alg *alg);


int akcipher_register_instance(struct crypto_template *tmpl,
		struct akcipher_instance *inst);
#endif
