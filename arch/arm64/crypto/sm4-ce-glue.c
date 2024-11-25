// SPDX-License-Identifier: GPL-2.0

#include <asm/neon.h>
#include <asm/simd.h>
#include <crypto/sm4.h>
<<<<<<< HEAD
=======
#include <crypto/internal/simd.h>
>>>>>>> upstream/android-13
#include <linux/module.h>
#include <linux/cpufeature.h>
#include <linux/crypto.h>
#include <linux/types.h>

MODULE_ALIAS_CRYPTO("sm4");
MODULE_ALIAS_CRYPTO("sm4-ce");
MODULE_DESCRIPTION("SM4 symmetric cipher using ARMv8 Crypto Extensions");
MODULE_AUTHOR("Ard Biesheuvel <ard.biesheuvel@linaro.org>");
MODULE_LICENSE("GPL v2");

asmlinkage void sm4_ce_do_crypt(const u32 *rk, void *out, const void *in);

<<<<<<< HEAD
static void sm4_ce_encrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	const struct crypto_sm4_ctx *ctx = crypto_tfm_ctx(tfm);

	if (!may_use_simd()) {
		crypto_sm4_encrypt(tfm, out, in);
=======
static int sm4_ce_setkey(struct crypto_tfm *tfm, const u8 *key,
		       unsigned int key_len)
{
	struct sm4_ctx *ctx = crypto_tfm_ctx(tfm);

	return sm4_expandkey(ctx, key, key_len);
}

static void sm4_ce_encrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	const struct sm4_ctx *ctx = crypto_tfm_ctx(tfm);

	if (!crypto_simd_usable()) {
		sm4_crypt_block(ctx->rkey_enc, out, in);
>>>>>>> upstream/android-13
	} else {
		kernel_neon_begin();
		sm4_ce_do_crypt(ctx->rkey_enc, out, in);
		kernel_neon_end();
	}
}

static void sm4_ce_decrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
<<<<<<< HEAD
	const struct crypto_sm4_ctx *ctx = crypto_tfm_ctx(tfm);

	if (!may_use_simd()) {
		crypto_sm4_decrypt(tfm, out, in);
=======
	const struct sm4_ctx *ctx = crypto_tfm_ctx(tfm);

	if (!crypto_simd_usable()) {
		sm4_crypt_block(ctx->rkey_dec, out, in);
>>>>>>> upstream/android-13
	} else {
		kernel_neon_begin();
		sm4_ce_do_crypt(ctx->rkey_dec, out, in);
		kernel_neon_end();
	}
}

static struct crypto_alg sm4_ce_alg = {
	.cra_name			= "sm4",
	.cra_driver_name		= "sm4-ce",
	.cra_priority			= 200,
	.cra_flags			= CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize			= SM4_BLOCK_SIZE,
<<<<<<< HEAD
	.cra_ctxsize			= sizeof(struct crypto_sm4_ctx),
=======
	.cra_ctxsize			= sizeof(struct sm4_ctx),
>>>>>>> upstream/android-13
	.cra_module			= THIS_MODULE,
	.cra_u.cipher = {
		.cia_min_keysize	= SM4_KEY_SIZE,
		.cia_max_keysize	= SM4_KEY_SIZE,
<<<<<<< HEAD
		.cia_setkey		= crypto_sm4_set_key,
=======
		.cia_setkey		= sm4_ce_setkey,
>>>>>>> upstream/android-13
		.cia_encrypt		= sm4_ce_encrypt,
		.cia_decrypt		= sm4_ce_decrypt
	}
};

static int __init sm4_ce_mod_init(void)
{
	return crypto_register_alg(&sm4_ce_alg);
}

static void __exit sm4_ce_mod_fini(void)
{
	crypto_unregister_alg(&sm4_ce_alg);
}

module_cpu_feature_match(SM4, sm4_ce_mod_init);
module_exit(sm4_ce_mod_fini);
