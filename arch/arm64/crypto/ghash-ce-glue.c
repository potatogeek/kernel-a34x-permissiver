<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * Accelerated GHASH implementation with ARMv8 PMULL instructions.
 *
 * Copyright (C) 2014 - 2018 Linaro Ltd. <ard.biesheuvel@linaro.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
=======
>>>>>>> upstream/android-13
 */

#include <asm/neon.h>
#include <asm/simd.h>
#include <asm/unaligned.h>
#include <crypto/aes.h>
#include <crypto/algapi.h>
#include <crypto/b128ops.h>
#include <crypto/gf128mul.h>
#include <crypto/internal/aead.h>
#include <crypto/internal/hash.h>
<<<<<<< HEAD
=======
#include <crypto/internal/simd.h>
>>>>>>> upstream/android-13
#include <crypto/internal/skcipher.h>
#include <crypto/scatterwalk.h>
#include <linux/cpufeature.h>
#include <linux/crypto.h>
#include <linux/module.h>

MODULE_DESCRIPTION("GHASH and AES-GCM using ARMv8 Crypto Extensions");
MODULE_AUTHOR("Ard Biesheuvel <ard.biesheuvel@linaro.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS_CRYPTO("ghash");

#define GHASH_BLOCK_SIZE	16
#define GHASH_DIGEST_SIZE	16
#define GCM_IV_SIZE		12

struct ghash_key {
<<<<<<< HEAD
	u64			h[2];
	u64			h2[2];
	u64			h3[2];
	u64			h4[2];

	be128			k;
=======
	be128			k;
	u64			h[][2];
>>>>>>> upstream/android-13
};

struct ghash_desc_ctx {
	u64 digest[GHASH_DIGEST_SIZE/sizeof(u64)];
	u8 buf[GHASH_BLOCK_SIZE];
	u32 count;
};

struct gcm_aes_ctx {
	struct crypto_aes_ctx	aes_key;
	struct ghash_key	ghash_key;
};

asmlinkage void pmull_ghash_update_p64(int blocks, u64 dg[], const char *src,
<<<<<<< HEAD
				       struct ghash_key const *k,
				       const char *head);

asmlinkage void pmull_ghash_update_p8(int blocks, u64 dg[], const char *src,
				      struct ghash_key const *k,
				      const char *head);

#ifdef CONFIG_CFI_CLANG
static inline void __cfi_pmull_ghash_update_p64(int blocks, u64 dg[],
                const char *src, struct ghash_key const *k, const char *head)
{
        return pmull_ghash_update_p64(blocks, dg, src, k, head);
}
#define pmull_ghash_update_p64 __cfi_pmull_ghash_update_p64

static inline void __cfi_pmull_ghash_update_p8(int blocks, u64 dg[],
                const char *src, struct ghash_key const *k, const char *head)
{
        return pmull_ghash_update_p8(blocks, dg, src, k, head);
}
#define pmull_ghash_update_p8 __cfi_pmull_ghash_update_p8
#endif

static void (*pmull_ghash_update)(int blocks, u64 dg[], const char *src,
				  struct ghash_key const *k,
				  const char *head);

asmlinkage void pmull_gcm_encrypt(int blocks, u64 dg[], u8 dst[],
				  const u8 src[], struct ghash_key const *k,
				  u8 ctr[], u32 const rk[], int rounds,
				  u8 ks[]);

asmlinkage void pmull_gcm_decrypt(int blocks, u64 dg[], u8 dst[],
				  const u8 src[], struct ghash_key const *k,
				  u8 ctr[], u32 const rk[], int rounds);

asmlinkage void pmull_gcm_encrypt_block(u8 dst[], u8 const src[],
					u32 const rk[], int rounds);

asmlinkage void __aes_arm64_encrypt(u32 *rk, u8 *out, const u8 *in, int rounds);
=======
				       u64 const h[][2], const char *head);

asmlinkage void pmull_ghash_update_p8(int blocks, u64 dg[], const char *src,
				      u64 const h[][2], const char *head);

asmlinkage void pmull_gcm_encrypt(int bytes, u8 dst[], const u8 src[],
				  u64 const h[][2], u64 dg[], u8 ctr[],
				  u32 const rk[], int rounds, u8 tag[]);
asmlinkage int pmull_gcm_decrypt(int bytes, u8 dst[], const u8 src[],
				 u64 const h[][2], u64 dg[], u8 ctr[],
				 u32 const rk[], int rounds, const u8 l[],
				 const u8 tag[], u64 authsize);
>>>>>>> upstream/android-13

static int ghash_init(struct shash_desc *desc)
{
	struct ghash_desc_ctx *ctx = shash_desc_ctx(desc);

	*ctx = (struct ghash_desc_ctx){};
	return 0;
}

static void ghash_do_update(int blocks, u64 dg[], const char *src,
			    struct ghash_key *key, const char *head)
{
<<<<<<< HEAD
	if (likely(may_use_simd())) {
		kernel_neon_begin();
		pmull_ghash_update(blocks, dg, src, key, head);
		kernel_neon_end();
	} else {
		be128 dst = { cpu_to_be64(dg[1]), cpu_to_be64(dg[0]) };

		do {
			const u8 *in = src;

			if (head) {
				in = head;
				blocks++;
				head = NULL;
			} else {
				src += GHASH_BLOCK_SIZE;
			}

			crypto_xor((u8 *)&dst, in, GHASH_BLOCK_SIZE);
			gf128mul_lle(&dst, &key->k);
		} while (--blocks);

		dg[0] = be64_to_cpu(dst.b);
		dg[1] = be64_to_cpu(dst.a);
=======
	be128 dst = { cpu_to_be64(dg[1]), cpu_to_be64(dg[0]) };

	do {
		const u8 *in = src;

		if (head) {
			in = head;
			blocks++;
			head = NULL;
		} else {
			src += GHASH_BLOCK_SIZE;
		}

		crypto_xor((u8 *)&dst, in, GHASH_BLOCK_SIZE);
		gf128mul_lle(&dst, &key->k);
	} while (--blocks);

	dg[0] = be64_to_cpu(dst.b);
	dg[1] = be64_to_cpu(dst.a);
}

static __always_inline
void ghash_do_simd_update(int blocks, u64 dg[], const char *src,
			  struct ghash_key *key, const char *head,
			  void (*simd_update)(int blocks, u64 dg[],
					      const char *src,
					      u64 const h[][2],
					      const char *head))
{
	if (likely(crypto_simd_usable())) {
		kernel_neon_begin();
		simd_update(blocks, dg, src, key->h, head);
		kernel_neon_end();
	} else {
		ghash_do_update(blocks, dg, src, key, head);
>>>>>>> upstream/android-13
	}
}

/* avoid hogging the CPU for too long */
#define MAX_BLOCKS	(SZ_64K / GHASH_BLOCK_SIZE)

static int ghash_update(struct shash_desc *desc, const u8 *src,
			unsigned int len)
{
	struct ghash_desc_ctx *ctx = shash_desc_ctx(desc);
	unsigned int partial = ctx->count % GHASH_BLOCK_SIZE;

	ctx->count += len;

	if ((partial + len) >= GHASH_BLOCK_SIZE) {
		struct ghash_key *key = crypto_shash_ctx(desc->tfm);
		int blocks;

		if (partial) {
			int p = GHASH_BLOCK_SIZE - partial;

			memcpy(ctx->buf + partial, src, p);
			src += p;
			len -= p;
		}

		blocks = len / GHASH_BLOCK_SIZE;
		len %= GHASH_BLOCK_SIZE;

		do {
			int chunk = min(blocks, MAX_BLOCKS);

<<<<<<< HEAD
			ghash_do_update(chunk, ctx->digest, src, key,
					partial ? ctx->buf : NULL);
=======
			ghash_do_simd_update(chunk, ctx->digest, src, key,
					     partial ? ctx->buf : NULL,
					     pmull_ghash_update_p8);
>>>>>>> upstream/android-13

			blocks -= chunk;
			src += chunk * GHASH_BLOCK_SIZE;
			partial = 0;
		} while (unlikely(blocks > 0));
	}
	if (len)
		memcpy(ctx->buf + partial, src, len);
	return 0;
}

static int ghash_final(struct shash_desc *desc, u8 *dst)
{
	struct ghash_desc_ctx *ctx = shash_desc_ctx(desc);
	unsigned int partial = ctx->count % GHASH_BLOCK_SIZE;

	if (partial) {
		struct ghash_key *key = crypto_shash_ctx(desc->tfm);

		memset(ctx->buf + partial, 0, GHASH_BLOCK_SIZE - partial);

<<<<<<< HEAD
		ghash_do_update(1, ctx->digest, ctx->buf, key, NULL);
=======
		ghash_do_simd_update(1, ctx->digest, ctx->buf, key, NULL,
				     pmull_ghash_update_p8);
>>>>>>> upstream/android-13
	}
	put_unaligned_be64(ctx->digest[1], dst);
	put_unaligned_be64(ctx->digest[0], dst + 8);

<<<<<<< HEAD
	*ctx = (struct ghash_desc_ctx){};
=======
	memzero_explicit(ctx, sizeof(*ctx));
>>>>>>> upstream/android-13
	return 0;
}

static void ghash_reflect(u64 h[], const be128 *k)
{
	u64 carry = be64_to_cpu(k->a) & BIT(63) ? 1 : 0;

	h[0] = (be64_to_cpu(k->b) << 1) | carry;
	h[1] = (be64_to_cpu(k->a) << 1) | (be64_to_cpu(k->b) >> 63);

	if (carry)
		h[1] ^= 0xc200000000000000UL;
}

<<<<<<< HEAD
static int __ghash_setkey(struct ghash_key *key,
			  const u8 *inkey, unsigned int keylen)
{
	be128 h;

	/* needed for the fallback */
	memcpy(&key->k, inkey, GHASH_BLOCK_SIZE);

	ghash_reflect(key->h, &key->k);

	h = key->k;
	gf128mul_lle(&h, &key->k);
	ghash_reflect(key->h2, &h);

	gf128mul_lle(&h, &key->k);
	ghash_reflect(key->h3, &h);

	gf128mul_lle(&h, &key->k);
	ghash_reflect(key->h4, &h);

	return 0;
}

=======
>>>>>>> upstream/android-13
static int ghash_setkey(struct crypto_shash *tfm,
			const u8 *inkey, unsigned int keylen)
{
	struct ghash_key *key = crypto_shash_ctx(tfm);

<<<<<<< HEAD
	if (keylen != GHASH_BLOCK_SIZE) {
		crypto_shash_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	return __ghash_setkey(key, inkey, keylen);
=======
	if (keylen != GHASH_BLOCK_SIZE)
		return -EINVAL;

	/* needed for the fallback */
	memcpy(&key->k, inkey, GHASH_BLOCK_SIZE);

	ghash_reflect(key->h[0], &key->k);
	return 0;
>>>>>>> upstream/android-13
}

static struct shash_alg ghash_alg = {
	.base.cra_name		= "ghash",
<<<<<<< HEAD
	.base.cra_driver_name	= "ghash-ce",
	.base.cra_priority	= 200,
	.base.cra_blocksize	= GHASH_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct ghash_key),
=======
	.base.cra_driver_name	= "ghash-neon",
	.base.cra_priority	= 150,
	.base.cra_blocksize	= GHASH_BLOCK_SIZE,
	.base.cra_ctxsize	= sizeof(struct ghash_key) + sizeof(u64[2]),
>>>>>>> upstream/android-13
	.base.cra_module	= THIS_MODULE,

	.digestsize		= GHASH_DIGEST_SIZE,
	.init			= ghash_init,
	.update			= ghash_update,
	.final			= ghash_final,
	.setkey			= ghash_setkey,
	.descsize		= sizeof(struct ghash_desc_ctx),
};

static int num_rounds(struct crypto_aes_ctx *ctx)
{
	/*
	 * # of rounds specified by AES:
	 * 128 bit key		10 rounds
	 * 192 bit key		12 rounds
	 * 256 bit key		14 rounds
	 * => n byte key	=> 6 + (n/4) rounds
	 */
	return 6 + ctx->key_length / 4;
}

static int gcm_setkey(struct crypto_aead *tfm, const u8 *inkey,
		      unsigned int keylen)
{
	struct gcm_aes_ctx *ctx = crypto_aead_ctx(tfm);
	u8 key[GHASH_BLOCK_SIZE];
<<<<<<< HEAD
	int ret;

	ret = crypto_aes_expand_key(&ctx->aes_key, inkey, keylen);
	if (ret) {
		tfm->base.crt_flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}

	__aes_arm64_encrypt(ctx->aes_key.key_enc, key, (u8[AES_BLOCK_SIZE]){},
			    num_rounds(&ctx->aes_key));

	return __ghash_setkey(&ctx->ghash_key, key, sizeof(be128));
=======
	be128 h;
	int ret;

	ret = aes_expandkey(&ctx->aes_key, inkey, keylen);
	if (ret)
		return -EINVAL;

	aes_encrypt(&ctx->aes_key, key, (u8[AES_BLOCK_SIZE]){});

	/* needed for the fallback */
	memcpy(&ctx->ghash_key.k, key, GHASH_BLOCK_SIZE);

	ghash_reflect(ctx->ghash_key.h[0], &ctx->ghash_key.k);

	h = ctx->ghash_key.k;
	gf128mul_lle(&h, &ctx->ghash_key.k);
	ghash_reflect(ctx->ghash_key.h[1], &h);

	gf128mul_lle(&h, &ctx->ghash_key.k);
	ghash_reflect(ctx->ghash_key.h[2], &h);

	gf128mul_lle(&h, &ctx->ghash_key.k);
	ghash_reflect(ctx->ghash_key.h[3], &h);

	return 0;
>>>>>>> upstream/android-13
}

static int gcm_setauthsize(struct crypto_aead *tfm, unsigned int authsize)
{
	switch (authsize) {
	case 4:
	case 8:
	case 12 ... 16:
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void gcm_update_mac(u64 dg[], const u8 *src, int count, u8 buf[],
			   int *buf_count, struct gcm_aes_ctx *ctx)
{
	if (*buf_count > 0) {
		int buf_added = min(count, GHASH_BLOCK_SIZE - *buf_count);

		memcpy(&buf[*buf_count], src, buf_added);

		*buf_count += buf_added;
		src += buf_added;
		count -= buf_added;
	}

	if (count >= GHASH_BLOCK_SIZE || *buf_count == GHASH_BLOCK_SIZE) {
		int blocks = count / GHASH_BLOCK_SIZE;

<<<<<<< HEAD
		ghash_do_update(blocks, dg, src, &ctx->ghash_key,
				*buf_count ? buf : NULL);
=======
		ghash_do_simd_update(blocks, dg, src, &ctx->ghash_key,
				     *buf_count ? buf : NULL,
				     pmull_ghash_update_p64);
>>>>>>> upstream/android-13

		src += blocks * GHASH_BLOCK_SIZE;
		count %= GHASH_BLOCK_SIZE;
		*buf_count = 0;
	}

	if (count > 0) {
		memcpy(buf, src, count);
		*buf_count = count;
	}
}

static void gcm_calculate_auth_mac(struct aead_request *req, u64 dg[])
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct gcm_aes_ctx *ctx = crypto_aead_ctx(aead);
	u8 buf[GHASH_BLOCK_SIZE];
	struct scatter_walk walk;
	u32 len = req->assoclen;
	int buf_count = 0;

	scatterwalk_start(&walk, req->src);

	do {
		u32 n = scatterwalk_clamp(&walk, len);
		u8 *p;

		if (!n) {
			scatterwalk_start(&walk, sg_next(walk.sg));
			n = scatterwalk_clamp(&walk, len);
		}
		p = scatterwalk_map(&walk);

		gcm_update_mac(dg, p, n, buf, &buf_count, ctx);
		len -= n;

		scatterwalk_unmap(p);
		scatterwalk_advance(&walk, n);
		scatterwalk_done(&walk, 0, len);
	} while (len);

	if (buf_count) {
		memset(&buf[buf_count], 0, GHASH_BLOCK_SIZE - buf_count);
<<<<<<< HEAD
		ghash_do_update(1, dg, buf, &ctx->ghash_key, NULL);
	}
}

static void gcm_final(struct aead_request *req, struct gcm_aes_ctx *ctx,
		      u64 dg[], u8 tag[], int cryptlen)
{
	u8 mac[AES_BLOCK_SIZE];
	u128 lengths;

	lengths.a = cpu_to_be64(req->assoclen * 8);
	lengths.b = cpu_to_be64(cryptlen * 8);

	ghash_do_update(1, dg, (void *)&lengths, &ctx->ghash_key, NULL);

	put_unaligned_be64(dg[1], mac);
	put_unaligned_be64(dg[0], mac + 8);

	crypto_xor(tag, mac, AES_BLOCK_SIZE);
}

=======
		ghash_do_simd_update(1, dg, buf, &ctx->ghash_key, NULL,
				     pmull_ghash_update_p64);
	}
}

>>>>>>> upstream/android-13
static int gcm_encrypt(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct gcm_aes_ctx *ctx = crypto_aead_ctx(aead);
<<<<<<< HEAD
	struct skcipher_walk walk;
	u8 iv[AES_BLOCK_SIZE];
	u8 ks[2 * AES_BLOCK_SIZE];
	u8 tag[AES_BLOCK_SIZE];
	u64 dg[2] = {};
	int nrounds = num_rounds(&ctx->aes_key);
	int err;

=======
	int nrounds = num_rounds(&ctx->aes_key);
	struct skcipher_walk walk;
	u8 buf[AES_BLOCK_SIZE];
	u8 iv[AES_BLOCK_SIZE];
	u64 dg[2] = {};
	be128 lengths;
	u8 *tag;
	int err;

	lengths.a = cpu_to_be64(req->assoclen * 8);
	lengths.b = cpu_to_be64(req->cryptlen * 8);

>>>>>>> upstream/android-13
	if (req->assoclen)
		gcm_calculate_auth_mac(req, dg);

	memcpy(iv, req->iv, GCM_IV_SIZE);
<<<<<<< HEAD
	put_unaligned_be32(1, iv + GCM_IV_SIZE);

	err = skcipher_walk_aead_encrypt(&walk, req, false);

	if (likely(may_use_simd() && walk.total >= 2 * AES_BLOCK_SIZE)) {
		u32 const *rk = NULL;

		kernel_neon_begin();
		pmull_gcm_encrypt_block(tag, iv, ctx->aes_key.key_enc, nrounds);
		put_unaligned_be32(2, iv + GCM_IV_SIZE);
		pmull_gcm_encrypt_block(ks, iv, NULL, nrounds);
		put_unaligned_be32(3, iv + GCM_IV_SIZE);
		pmull_gcm_encrypt_block(ks + AES_BLOCK_SIZE, iv, NULL, nrounds);
		put_unaligned_be32(4, iv + GCM_IV_SIZE);

		do {
			int blocks = walk.nbytes / (2 * AES_BLOCK_SIZE) * 2;

			if (rk)
				kernel_neon_begin();

			pmull_gcm_encrypt(blocks, dg, walk.dst.virt.addr,
					  walk.src.virt.addr, &ctx->ghash_key,
					  iv, rk, nrounds, ks);
			kernel_neon_end();

			err = skcipher_walk_done(&walk,
					walk.nbytes % (2 * AES_BLOCK_SIZE));

			rk = ctx->aes_key.key_enc;
		} while (walk.nbytes >= 2 * AES_BLOCK_SIZE);
	} else {
		__aes_arm64_encrypt(ctx->aes_key.key_enc, tag, iv, nrounds);
		put_unaligned_be32(2, iv + GCM_IV_SIZE);

		while (walk.nbytes >= (2 * AES_BLOCK_SIZE)) {
			const int blocks =
				walk.nbytes / (2 * AES_BLOCK_SIZE) * 2;
			u8 *dst = walk.dst.virt.addr;
			u8 *src = walk.src.virt.addr;
			int remaining = blocks;

			do {
				__aes_arm64_encrypt(ctx->aes_key.key_enc,
						    ks, iv, nrounds);
				crypto_xor_cpy(dst, src, ks, AES_BLOCK_SIZE);
=======
	put_unaligned_be32(2, iv + GCM_IV_SIZE);

	err = skcipher_walk_aead_encrypt(&walk, req, false);

	if (likely(crypto_simd_usable())) {
		do {
			const u8 *src = walk.src.virt.addr;
			u8 *dst = walk.dst.virt.addr;
			int nbytes = walk.nbytes;

			tag = (u8 *)&lengths;

			if (unlikely(nbytes > 0 && nbytes < AES_BLOCK_SIZE)) {
				src = dst = memcpy(buf + sizeof(buf) - nbytes,
						   src, nbytes);
			} else if (nbytes < walk.total) {
				nbytes &= ~(AES_BLOCK_SIZE - 1);
				tag = NULL;
			}

			kernel_neon_begin();
			pmull_gcm_encrypt(nbytes, dst, src, ctx->ghash_key.h,
					  dg, iv, ctx->aes_key.key_enc, nrounds,
					  tag);
			kernel_neon_end();

			if (unlikely(!nbytes))
				break;

			if (unlikely(nbytes > 0 && nbytes < AES_BLOCK_SIZE))
				memcpy(walk.dst.virt.addr,
				       buf + sizeof(buf) - nbytes, nbytes);

			err = skcipher_walk_done(&walk, walk.nbytes - nbytes);
		} while (walk.nbytes);
	} else {
		while (walk.nbytes >= AES_BLOCK_SIZE) {
			int blocks = walk.nbytes / AES_BLOCK_SIZE;
			const u8 *src = walk.src.virt.addr;
			u8 *dst = walk.dst.virt.addr;
			int remaining = blocks;

			do {
				aes_encrypt(&ctx->aes_key, buf, iv);
				crypto_xor_cpy(dst, src, buf, AES_BLOCK_SIZE);
>>>>>>> upstream/android-13
				crypto_inc(iv, AES_BLOCK_SIZE);

				dst += AES_BLOCK_SIZE;
				src += AES_BLOCK_SIZE;
			} while (--remaining > 0);

<<<<<<< HEAD
			ghash_do_update(blocks, dg,
					walk.dst.virt.addr, &ctx->ghash_key,
					NULL);

			err = skcipher_walk_done(&walk,
						 walk.nbytes % (2 * AES_BLOCK_SIZE));
		}
		if (walk.nbytes) {
			__aes_arm64_encrypt(ctx->aes_key.key_enc, ks, iv,
					    nrounds);
			if (walk.nbytes > AES_BLOCK_SIZE) {
				crypto_inc(iv, AES_BLOCK_SIZE);
				__aes_arm64_encrypt(ctx->aes_key.key_enc,
					            ks + AES_BLOCK_SIZE, iv,
						    nrounds);
			}
		}
	}

	/* handle the tail */
	if (walk.nbytes) {
		u8 buf[GHASH_BLOCK_SIZE];
		unsigned int nbytes = walk.nbytes;
		u8 *dst = walk.dst.virt.addr;
		u8 *head = NULL;

		crypto_xor_cpy(walk.dst.virt.addr, walk.src.virt.addr, ks,
			       walk.nbytes);

		if (walk.nbytes > GHASH_BLOCK_SIZE) {
			head = dst;
			dst += GHASH_BLOCK_SIZE;
			nbytes %= GHASH_BLOCK_SIZE;
		}

		memcpy(buf, dst, nbytes);
		memset(buf + nbytes, 0, GHASH_BLOCK_SIZE - nbytes);
		ghash_do_update(!!nbytes, dg, buf, &ctx->ghash_key, head);

		err = skcipher_walk_done(&walk, 0);
=======
			ghash_do_update(blocks, dg, walk.dst.virt.addr,
					&ctx->ghash_key, NULL);

			err = skcipher_walk_done(&walk,
						 walk.nbytes % AES_BLOCK_SIZE);
		}

		/* handle the tail */
		if (walk.nbytes) {
			aes_encrypt(&ctx->aes_key, buf, iv);

			crypto_xor_cpy(walk.dst.virt.addr, walk.src.virt.addr,
				       buf, walk.nbytes);

			memcpy(buf, walk.dst.virt.addr, walk.nbytes);
			memset(buf + walk.nbytes, 0, sizeof(buf) - walk.nbytes);
		}

		tag = (u8 *)&lengths;
		ghash_do_update(1, dg, tag, &ctx->ghash_key,
				walk.nbytes ? buf : NULL);

		if (walk.nbytes)
			err = skcipher_walk_done(&walk, 0);

		put_unaligned_be64(dg[1], tag);
		put_unaligned_be64(dg[0], tag + 8);
		put_unaligned_be32(1, iv + GCM_IV_SIZE);
		aes_encrypt(&ctx->aes_key, iv, iv);
		crypto_xor(tag, iv, AES_BLOCK_SIZE);
>>>>>>> upstream/android-13
	}

	if (err)
		return err;

<<<<<<< HEAD
	gcm_final(req, ctx, dg, tag, req->cryptlen);

=======
>>>>>>> upstream/android-13
	/* copy authtag to end of dst */
	scatterwalk_map_and_copy(tag, req->dst, req->assoclen + req->cryptlen,
				 crypto_aead_authsize(aead), 1);

	return 0;
}

static int gcm_decrypt(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct gcm_aes_ctx *ctx = crypto_aead_ctx(aead);
	unsigned int authsize = crypto_aead_authsize(aead);
<<<<<<< HEAD
	struct skcipher_walk walk;
	u8 iv[2 * AES_BLOCK_SIZE];
	u8 tag[AES_BLOCK_SIZE];
	u8 buf[2 * GHASH_BLOCK_SIZE];
	u64 dg[2] = {};
	int nrounds = num_rounds(&ctx->aes_key);
	int err;

=======
	int nrounds = num_rounds(&ctx->aes_key);
	struct skcipher_walk walk;
	u8 otag[AES_BLOCK_SIZE];
	u8 buf[AES_BLOCK_SIZE];
	u8 iv[AES_BLOCK_SIZE];
	u64 dg[2] = {};
	be128 lengths;
	u8 *tag;
	int err;

	lengths.a = cpu_to_be64(req->assoclen * 8);
	lengths.b = cpu_to_be64((req->cryptlen - authsize) * 8);

>>>>>>> upstream/android-13
	if (req->assoclen)
		gcm_calculate_auth_mac(req, dg);

	memcpy(iv, req->iv, GCM_IV_SIZE);
<<<<<<< HEAD
	put_unaligned_be32(1, iv + GCM_IV_SIZE);

	err = skcipher_walk_aead_decrypt(&walk, req, false);

	if (likely(may_use_simd() && walk.total >= 2 * AES_BLOCK_SIZE)) {
		u32 const *rk = NULL;

		kernel_neon_begin();
		pmull_gcm_encrypt_block(tag, iv, ctx->aes_key.key_enc, nrounds);
		put_unaligned_be32(2, iv + GCM_IV_SIZE);

		do {
			int blocks = walk.nbytes / (2 * AES_BLOCK_SIZE) * 2;
			int rem = walk.total - blocks * AES_BLOCK_SIZE;

			if (rk)
				kernel_neon_begin();

			pmull_gcm_decrypt(blocks, dg, walk.dst.virt.addr,
					  walk.src.virt.addr, &ctx->ghash_key,
					  iv, rk, nrounds);

			/* check if this is the final iteration of the loop */
			if (rem < (2 * AES_BLOCK_SIZE)) {
				u8 *iv2 = iv + AES_BLOCK_SIZE;

				if (rem > AES_BLOCK_SIZE) {
					memcpy(iv2, iv, AES_BLOCK_SIZE);
					crypto_inc(iv2, AES_BLOCK_SIZE);
				}

				pmull_gcm_encrypt_block(iv, iv, NULL, nrounds);

				if (rem > AES_BLOCK_SIZE)
					pmull_gcm_encrypt_block(iv2, iv2, NULL,
								nrounds);
			}

			kernel_neon_end();

			err = skcipher_walk_done(&walk,
					walk.nbytes % (2 * AES_BLOCK_SIZE));

			rk = ctx->aes_key.key_enc;
		} while (walk.nbytes >= 2 * AES_BLOCK_SIZE);
	} else {
		__aes_arm64_encrypt(ctx->aes_key.key_enc, tag, iv, nrounds);
		put_unaligned_be32(2, iv + GCM_IV_SIZE);

		while (walk.nbytes >= (2 * AES_BLOCK_SIZE)) {
			int blocks = walk.nbytes / (2 * AES_BLOCK_SIZE) * 2;
			u8 *dst = walk.dst.virt.addr;
			u8 *src = walk.src.virt.addr;
=======
	put_unaligned_be32(2, iv + GCM_IV_SIZE);

	scatterwalk_map_and_copy(otag, req->src,
				 req->assoclen + req->cryptlen - authsize,
				 authsize, 0);

	err = skcipher_walk_aead_decrypt(&walk, req, false);

	if (likely(crypto_simd_usable())) {
		int ret;

		do {
			const u8 *src = walk.src.virt.addr;
			u8 *dst = walk.dst.virt.addr;
			int nbytes = walk.nbytes;

			tag = (u8 *)&lengths;

			if (unlikely(nbytes > 0 && nbytes < AES_BLOCK_SIZE)) {
				src = dst = memcpy(buf + sizeof(buf) - nbytes,
						   src, nbytes);
			} else if (nbytes < walk.total) {
				nbytes &= ~(AES_BLOCK_SIZE - 1);
				tag = NULL;
			}

			kernel_neon_begin();
			ret = pmull_gcm_decrypt(nbytes, dst, src,
						ctx->ghash_key.h,
						dg, iv, ctx->aes_key.key_enc,
						nrounds, tag, otag, authsize);
			kernel_neon_end();

			if (unlikely(!nbytes))
				break;

			if (unlikely(nbytes > 0 && nbytes < AES_BLOCK_SIZE))
				memcpy(walk.dst.virt.addr,
				       buf + sizeof(buf) - nbytes, nbytes);

			err = skcipher_walk_done(&walk, walk.nbytes - nbytes);
		} while (walk.nbytes);

		if (err)
			return err;
		if (ret)
			return -EBADMSG;
	} else {
		while (walk.nbytes >= AES_BLOCK_SIZE) {
			int blocks = walk.nbytes / AES_BLOCK_SIZE;
			const u8 *src = walk.src.virt.addr;
			u8 *dst = walk.dst.virt.addr;
>>>>>>> upstream/android-13

			ghash_do_update(blocks, dg, walk.src.virt.addr,
					&ctx->ghash_key, NULL);

			do {
<<<<<<< HEAD
				__aes_arm64_encrypt(ctx->aes_key.key_enc,
						    buf, iv, nrounds);
=======
				aes_encrypt(&ctx->aes_key, buf, iv);
>>>>>>> upstream/android-13
				crypto_xor_cpy(dst, src, buf, AES_BLOCK_SIZE);
				crypto_inc(iv, AES_BLOCK_SIZE);

				dst += AES_BLOCK_SIZE;
				src += AES_BLOCK_SIZE;
			} while (--blocks > 0);

			err = skcipher_walk_done(&walk,
<<<<<<< HEAD
						 walk.nbytes % (2 * AES_BLOCK_SIZE));
		}
		if (walk.nbytes) {
			if (walk.nbytes > AES_BLOCK_SIZE) {
				u8 *iv2 = iv + AES_BLOCK_SIZE;

				memcpy(iv2, iv, AES_BLOCK_SIZE);
				crypto_inc(iv2, AES_BLOCK_SIZE);

				__aes_arm64_encrypt(ctx->aes_key.key_enc, iv2,
						    iv2, nrounds);
			}
			__aes_arm64_encrypt(ctx->aes_key.key_enc, iv, iv,
					    nrounds);
		}
	}

	/* handle the tail */
	if (walk.nbytes) {
		const u8 *src = walk.src.virt.addr;
		const u8 *head = NULL;
		unsigned int nbytes = walk.nbytes;

		if (walk.nbytes > GHASH_BLOCK_SIZE) {
			head = src;
			src += GHASH_BLOCK_SIZE;
			nbytes %= GHASH_BLOCK_SIZE;
		}

		memcpy(buf, src, nbytes);
		memset(buf + nbytes, 0, GHASH_BLOCK_SIZE - nbytes);
		ghash_do_update(!!nbytes, dg, buf, &ctx->ghash_key, head);

		crypto_xor_cpy(walk.dst.virt.addr, walk.src.virt.addr, iv,
			       walk.nbytes);

		err = skcipher_walk_done(&walk, 0);
	}

	if (err)
		return err;

	gcm_final(req, ctx, dg, tag, req->cryptlen - authsize);

	/* compare calculated auth tag with the stored one */
	scatterwalk_map_and_copy(buf, req->src,
				 req->assoclen + req->cryptlen - authsize,
				 authsize, 0);

	if (crypto_memneq(tag, buf, authsize))
		return -EBADMSG;
=======
						 walk.nbytes % AES_BLOCK_SIZE);
		}

		/* handle the tail */
		if (walk.nbytes) {
			memcpy(buf, walk.src.virt.addr, walk.nbytes);
			memset(buf + walk.nbytes, 0, sizeof(buf) - walk.nbytes);
		}

		tag = (u8 *)&lengths;
		ghash_do_update(1, dg, tag, &ctx->ghash_key,
				walk.nbytes ? buf : NULL);

		if (walk.nbytes) {
			aes_encrypt(&ctx->aes_key, buf, iv);

			crypto_xor_cpy(walk.dst.virt.addr, walk.src.virt.addr,
				       buf, walk.nbytes);

			err = skcipher_walk_done(&walk, 0);
		}

		if (err)
			return err;

		put_unaligned_be64(dg[1], tag);
		put_unaligned_be64(dg[0], tag + 8);
		put_unaligned_be32(1, iv + GCM_IV_SIZE);
		aes_encrypt(&ctx->aes_key, iv, iv);
		crypto_xor(tag, iv, AES_BLOCK_SIZE);

		if (crypto_memneq(tag, otag, authsize)) {
			memzero_explicit(tag, AES_BLOCK_SIZE);
			return -EBADMSG;
		}
	}
>>>>>>> upstream/android-13
	return 0;
}

static struct aead_alg gcm_aes_alg = {
	.ivsize			= GCM_IV_SIZE,
<<<<<<< HEAD
	.chunksize		= 2 * AES_BLOCK_SIZE,
=======
	.chunksize		= AES_BLOCK_SIZE,
>>>>>>> upstream/android-13
	.maxauthsize		= AES_BLOCK_SIZE,
	.setkey			= gcm_setkey,
	.setauthsize		= gcm_setauthsize,
	.encrypt		= gcm_encrypt,
	.decrypt		= gcm_decrypt,

	.base.cra_name		= "gcm(aes)",
	.base.cra_driver_name	= "gcm-aes-ce",
	.base.cra_priority	= 300,
	.base.cra_blocksize	= 1,
<<<<<<< HEAD
	.base.cra_ctxsize	= sizeof(struct gcm_aes_ctx),
=======
	.base.cra_ctxsize	= sizeof(struct gcm_aes_ctx) +
				  4 * sizeof(u64[2]),
>>>>>>> upstream/android-13
	.base.cra_module	= THIS_MODULE,
};

static int __init ghash_ce_mod_init(void)
{
<<<<<<< HEAD
	int ret;

	if (!(elf_hwcap & HWCAP_ASIMD))
		return -ENODEV;

	if (elf_hwcap & HWCAP_PMULL)
		pmull_ghash_update = pmull_ghash_update_p64;

	else
		pmull_ghash_update = pmull_ghash_update_p8;

	ret = crypto_register_shash(&ghash_alg);
	if (ret)
		return ret;

	if (elf_hwcap & HWCAP_PMULL) {
		ret = crypto_register_aead(&gcm_aes_alg);
		if (ret)
			crypto_unregister_shash(&ghash_alg);
	}
	return ret;
=======
	if (!cpu_have_named_feature(ASIMD))
		return -ENODEV;

	if (cpu_have_named_feature(PMULL))
		return crypto_register_aead(&gcm_aes_alg);

	return crypto_register_shash(&ghash_alg);
>>>>>>> upstream/android-13
}

static void __exit ghash_ce_mod_exit(void)
{
<<<<<<< HEAD
	crypto_unregister_shash(&ghash_alg);
	crypto_unregister_aead(&gcm_aes_alg);
=======
	if (cpu_have_named_feature(PMULL))
		crypto_unregister_aead(&gcm_aes_alg);
	else
		crypto_unregister_shash(&ghash_alg);
>>>>>>> upstream/android-13
}

static const struct cpu_feature ghash_cpu_feature[] = {
	{ cpu_feature(PMULL) }, { }
};
MODULE_DEVICE_TABLE(cpu, ghash_cpu_feature);

module_init(ghash_ce_mod_init);
module_exit(ghash_ce_mod_exit);
