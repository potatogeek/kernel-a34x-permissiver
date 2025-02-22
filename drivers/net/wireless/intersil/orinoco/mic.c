/* Orinoco MIC helpers
 *
 * See copyright notice in main.c
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/if_ether.h>
#include <linux/scatterlist.h>
#include <crypto/hash.h>

#include "orinoco.h"
#include "mic.h"

/********************************************************************/
/* Michael MIC crypto setup                                         */
/********************************************************************/
int orinoco_mic_init(struct orinoco_private *priv)
{
<<<<<<< HEAD
	priv->tx_tfm_mic = crypto_alloc_shash("michael_mic", 0,
					      CRYPTO_ALG_ASYNC);
	if (IS_ERR(priv->tx_tfm_mic)) {
		printk(KERN_DEBUG "orinoco_mic_init: could not allocate "
		       "crypto API michael_mic\n");
=======
	priv->tx_tfm_mic = crypto_alloc_shash("michael_mic", 0, 0);
	if (IS_ERR(priv->tx_tfm_mic)) {
		printk(KERN_DEBUG "%s: could not allocate "
		       "crypto API michael_mic\n", __func__);
>>>>>>> upstream/android-13
		priv->tx_tfm_mic = NULL;
		return -ENOMEM;
	}

<<<<<<< HEAD
	priv->rx_tfm_mic = crypto_alloc_shash("michael_mic", 0,
					      CRYPTO_ALG_ASYNC);
	if (IS_ERR(priv->rx_tfm_mic)) {
		printk(KERN_DEBUG "orinoco_mic_init: could not allocate "
		       "crypto API michael_mic\n");
=======
	priv->rx_tfm_mic = crypto_alloc_shash("michael_mic", 0, 0);
	if (IS_ERR(priv->rx_tfm_mic)) {
		printk(KERN_DEBUG "%s: could not allocate "
		       "crypto API michael_mic\n", __func__);
>>>>>>> upstream/android-13
		priv->rx_tfm_mic = NULL;
		return -ENOMEM;
	}

	return 0;
}

void orinoco_mic_free(struct orinoco_private *priv)
{
	if (priv->tx_tfm_mic)
		crypto_free_shash(priv->tx_tfm_mic);
	if (priv->rx_tfm_mic)
		crypto_free_shash(priv->rx_tfm_mic);
}

int orinoco_mic(struct crypto_shash *tfm_michael, u8 *key,
		u8 *da, u8 *sa, u8 priority,
		u8 *data, size_t data_len, u8 *mic)
{
	SHASH_DESC_ON_STACK(desc, tfm_michael);
	u8 hdr[ETH_HLEN + 2]; /* size of header + padding */
	int err;

	if (tfm_michael == NULL) {
<<<<<<< HEAD
		printk(KERN_WARNING "orinoco_mic: tfm_michael == NULL\n");
=======
		printk(KERN_WARNING "%s: tfm_michael == NULL\n", __func__);
>>>>>>> upstream/android-13
		return -1;
	}

	/* Copy header into buffer. We need the padding on the end zeroed */
	memcpy(&hdr[0], da, ETH_ALEN);
	memcpy(&hdr[ETH_ALEN], sa, ETH_ALEN);
	hdr[ETH_ALEN * 2] = priority;
	hdr[ETH_ALEN * 2 + 1] = 0;
	hdr[ETH_ALEN * 2 + 2] = 0;
	hdr[ETH_ALEN * 2 + 3] = 0;

	desc->tfm = tfm_michael;
<<<<<<< HEAD
	desc->flags = 0;
=======
>>>>>>> upstream/android-13

	err = crypto_shash_setkey(tfm_michael, key, MIC_KEYLEN);
	if (err)
		return err;

	err = crypto_shash_init(desc);
	if (err)
		return err;

	err = crypto_shash_update(desc, hdr, sizeof(hdr));
	if (err)
		return err;

	err = crypto_shash_update(desc, data, data_len);
	if (err)
		return err;

	err = crypto_shash_final(desc, mic);
	shash_desc_zero(desc);

	return err;
}
