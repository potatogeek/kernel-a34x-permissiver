/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _CRYPTO_CRYPT_H
#define _CRYPTO_CRYPT_H

#include <linux/kernel.h>
#include <crypto/aead.h>
#include <crypto/hash.h>
#include <crypto/skcipher.h>

struct cryptd_ablkcipher {
	struct crypto_ablkcipher base;
};

static inline struct cryptd_ablkcipher *__cryptd_ablkcipher_cast(
	struct crypto_ablkcipher *tfm)
{
	return (struct cryptd_ablkcipher *)tfm;
}


struct cryptd_ablkcipher *cryptd_alloc_ablkcipher(const char *alg_name,
						  u32 type, u32 mask);
struct crypto_blkcipher *cryptd_ablkcipher_child(struct cryptd_ablkcipher *tfm);
bool cryptd_ablkcipher_queued(struct cryptd_ablkcipher *tfm);
void cryptd_free_ablkcipher(struct cryptd_ablkcipher *tfm);

struct cryptd_skcipher {
	struct crypto_skcipher base;
};

struct cryptd_skcipher *cryptd_alloc_skcipher(const char *alg_name,
					      u32 type, u32 mask);
struct crypto_skcipher *cryptd_skcipher_child(struct cryptd_skcipher *tfm);

bool cryptd_skcipher_queued(struct cryptd_skcipher *tfm);
void cryptd_free_skcipher(struct cryptd_skcipher *tfm);

struct cryptd_ahash {
	struct crypto_ahash base;
};

static inline struct cryptd_ahash *__cryptd_ahash_cast(
	struct crypto_ahash *tfm)
{
	return (struct cryptd_ahash *)tfm;
}


struct cryptd_ahash *cryptd_alloc_ahash(const char *alg_name,
					u32 type, u32 mask);
struct crypto_shash *cryptd_ahash_child(struct cryptd_ahash *tfm);
struct shash_desc *cryptd_shash_desc(struct ahash_request *req);

bool cryptd_ahash_queued(struct cryptd_ahash *tfm);
void cryptd_free_ahash(struct cryptd_ahash *tfm);

struct cryptd_aead {
	struct crypto_aead base;
};

static inline struct cryptd_aead *__cryptd_aead_cast(
	struct crypto_aead *tfm)
{
	return (struct cryptd_aead *)tfm;
}

struct cryptd_aead *cryptd_alloc_aead(const char *alg_name,
					  u32 type, u32 mask);

struct crypto_aead *cryptd_aead_child(struct cryptd_aead *tfm);

bool cryptd_aead_queued(struct cryptd_aead *tfm);

void cryptd_free_aead(struct cryptd_aead *tfm);

#endif
