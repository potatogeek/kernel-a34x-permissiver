// SPDX-License-Identifier: GPL-2.0
/*
 *  pkey device driver
 *
<<<<<<< HEAD
 *  Copyright IBM Corp. 2017
=======
 *  Copyright IBM Corp. 2017,2019
>>>>>>> upstream/android-13
 *  Author(s): Harald Freudenberger
 */

#define KMSG_COMPONENT "pkey"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kallsyms.h>
#include <linux/debugfs.h>
<<<<<<< HEAD
#include <asm/zcrypt.h>
#include <asm/cpacf.h>
#include <asm/pkey.h>

#include "zcrypt_api.h"
=======
#include <linux/random.h>
#include <linux/cpufeature.h>
#include <asm/zcrypt.h>
#include <asm/cpacf.h>
#include <asm/pkey.h>
#include <crypto/aes.h>

#include "zcrypt_api.h"
#include "zcrypt_ccamisc.h"
#include "zcrypt_ep11misc.h"
>>>>>>> upstream/android-13

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IBM Corporation");
MODULE_DESCRIPTION("s390 protected key interface");

<<<<<<< HEAD
/* Size of parameter block used for all cca requests/replies */
#define PARMBSIZE 512

/* Size of vardata block used for some of the cca requests/replies */
#define VARDATASIZE 4096
=======
#define KEYBLOBBUFSIZE 8192	/* key buffer size used for internal processing */
#define PROTKEYBLOBBUFSIZE 256	/* protected key buffer size used internal */
#define MAXAPQNSINLIST 64	/* max 64 apqns within a apqn list */
>>>>>>> upstream/android-13

/*
 * debug feature data and functions
 */

static debug_info_t *debug_info;

#define DEBUG_DBG(...)	debug_sprintf_event(debug_info, 6, ##__VA_ARGS__)
#define DEBUG_INFO(...) debug_sprintf_event(debug_info, 5, ##__VA_ARGS__)
#define DEBUG_WARN(...) debug_sprintf_event(debug_info, 4, ##__VA_ARGS__)
#define DEBUG_ERR(...)	debug_sprintf_event(debug_info, 3, ##__VA_ARGS__)

static void __init pkey_debug_init(void)
{
	/* 5 arguments per dbf entry (including the format string ptr) */
	debug_info = debug_register("pkey", 1, 1, 5 * sizeof(long));
	debug_register_view(debug_info, &debug_sprintf_view);
	debug_set_level(debug_info, 3);
}

static void __exit pkey_debug_exit(void)
{
	debug_unregister(debug_info);
}

<<<<<<< HEAD
/* inside view of a secure key token (only type 0x01 version 0x04) */
struct secaeskeytoken {
	u8  type;     /* 0x01 for internal key token */
	u8  res0[3];
	u8  version;  /* should be 0x04 */
	u8  res1[1];
	u8  flag;     /* key flags */
	u8  res2[1];
	u64 mkvp;     /* master key verification pattern */
	u8  key[32];  /* key value (encrypted) */
	u8  cv[8];    /* control vector */
	u16 bitsize;  /* key bit size */
	u16 keysize;  /* key byte size */
	u8  tvv[4];   /* token validation value */
} __packed;

/*
 * Simple check if the token is a valid CCA secure AES key
 * token. If keybitsize is given, the bitsize of the key is
 * also checked. Returns 0 on success or errno value on failure.
 */
static int check_secaeskeytoken(const u8 *token, int keybitsize)
{
	struct secaeskeytoken *t = (struct secaeskeytoken *) token;

	if (t->type != 0x01) {
		DEBUG_ERR(
			"%s secure token check failed, type mismatch 0x%02x != 0x01\n",
			__func__, (int) t->type);
		return -EINVAL;
	}
	if (t->version != 0x04) {
		DEBUG_ERR(
			"%s secure token check failed, version mismatch 0x%02x != 0x04\n",
			__func__, (int) t->version);
		return -EINVAL;
	}
	if (keybitsize > 0 && t->bitsize != keybitsize) {
		DEBUG_ERR(
			"%s secure token check failed, bitsize mismatch %d != %d\n",
			__func__, (int) t->bitsize, keybitsize);
		return -EINVAL;
	}

	return 0;
}

/*
 * Allocate consecutive memory for request CPRB, request param
 * block, reply CPRB and reply param block and fill in values
 * for the common fields. Returns 0 on success or errno value
 * on failure.
 */
static int alloc_and_prep_cprbmem(size_t paramblen,
				  u8 **pcprbmem,
				  struct CPRBX **preqCPRB,
				  struct CPRBX **prepCPRB)
{
	u8 *cprbmem;
	size_t cprbplusparamblen = sizeof(struct CPRBX) + paramblen;
	struct CPRBX *preqcblk, *prepcblk;

	/*
	 * allocate consecutive memory for request CPRB, request param
	 * block, reply CPRB and reply param block
	 */
	cprbmem = kcalloc(2, cprbplusparamblen, GFP_KERNEL);
	if (!cprbmem)
		return -ENOMEM;

	preqcblk = (struct CPRBX *) cprbmem;
	prepcblk = (struct CPRBX *) (cprbmem + cprbplusparamblen);

	/* fill request cprb struct */
	preqcblk->cprb_len = sizeof(struct CPRBX);
	preqcblk->cprb_ver_id = 0x02;
	memcpy(preqcblk->func_id, "T2", 2);
	preqcblk->rpl_msgbl = cprbplusparamblen;
	if (paramblen) {
		preqcblk->req_parmb =
			((u8 *) preqcblk) + sizeof(struct CPRBX);
		preqcblk->rpl_parmb =
			((u8 *) prepcblk) + sizeof(struct CPRBX);
	}

	*pcprbmem = cprbmem;
	*preqCPRB = preqcblk;
	*prepCPRB = prepcblk;

	return 0;
}

/*
 * Free the cprb memory allocated with the function above.
 * If the scrub value is not zero, the memory is filled
 * with zeros before freeing (useful if there was some
 * clear key material in there).
 */
static void free_cprbmem(void *mem, size_t paramblen, int scrub)
{
	if (scrub)
		memzero_explicit(mem, 2 * (sizeof(struct CPRBX) + paramblen));
	kfree(mem);
}

/*
 * Helper function to prepare the xcrb struct
 */
static inline void prep_xcrb(struct ica_xcRB *pxcrb,
			     u16 cardnr,
			     struct CPRBX *preqcblk,
			     struct CPRBX *prepcblk)
{
	memset(pxcrb, 0, sizeof(*pxcrb));
	pxcrb->agent_ID = 0x4341; /* 'CA' */
	pxcrb->user_defined = (cardnr == 0xFFFF ? AUTOSELECT : cardnr);
	pxcrb->request_control_blk_length =
		preqcblk->cprb_len + preqcblk->req_parml;
	pxcrb->request_control_blk_addr = (void __user *) preqcblk;
	pxcrb->reply_control_blk_length = preqcblk->rpl_msgbl;
	pxcrb->reply_control_blk_addr = (void __user *) prepcblk;
}

/*
 * Helper function which calls zcrypt_send_cprb with
 * memory management segment adjusted to kernel space
 * so that the copy_from_user called within this
 * function do in fact copy from kernel space.
 */
static inline int _zcrypt_send_cprb(struct ica_xcRB *xcrb)
{
	int rc;
	mm_segment_t old_fs = get_fs();

	set_fs(KERNEL_DS);
	rc = zcrypt_send_cprb(xcrb);
	set_fs(old_fs);

	return rc;
}

/*
 * Generate (random) AES secure key.
 */
int pkey_genseckey(u16 cardnr, u16 domain,
		   u32 keytype, struct pkey_seckey *seckey)
{
	int i, rc, keysize;
	int seckeysize;
	u8 *mem;
	struct CPRBX *preqcblk, *prepcblk;
	struct ica_xcRB xcrb;
	struct kgreqparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		struct lv1 {
			u16 len;
			char  key_form[8];
			char  key_length[8];
			char  key_type1[8];
			char  key_type2[8];
		} lv1;
		struct lv2 {
			u16 len;
			struct keyid {
				u16 len;
				u16 attr;
				u8  data[SECKEYBLOBSIZE];
			} keyid[6];
		} lv2;
	} *preqparm;
	struct kgrepparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		struct lv3 {
			u16 len;
			u16 keyblocklen;
			struct {
				u16 toklen;
				u16 tokattr;
				u8  tok[0];
				/* ... some more data ... */
			} keyblock;
		} lv3;
	} *prepparm;

	/* get already prepared memory for 2 cprbs with param block each */
	rc = alloc_and_prep_cprbmem(PARMBSIZE, &mem, &preqcblk, &prepcblk);
	if (rc)
		return rc;

	/* fill request cprb struct */
	preqcblk->domain = domain;

	/* fill request cprb param block with KG request */
	preqparm = (struct kgreqparm *) preqcblk->req_parmb;
	memcpy(preqparm->subfunc_code, "KG", 2);
	preqparm->rule_array_len = sizeof(preqparm->rule_array_len);
	preqparm->lv1.len = sizeof(struct lv1);
	memcpy(preqparm->lv1.key_form,	 "OP      ", 8);
	switch (keytype) {
	case PKEY_KEYTYPE_AES_128:
		keysize = 16;
		memcpy(preqparm->lv1.key_length, "KEYLN16 ", 8);
		break;
	case PKEY_KEYTYPE_AES_192:
		keysize = 24;
		memcpy(preqparm->lv1.key_length, "KEYLN24 ", 8);
		break;
	case PKEY_KEYTYPE_AES_256:
		keysize = 32;
		memcpy(preqparm->lv1.key_length, "KEYLN32 ", 8);
		break;
	default:
		DEBUG_ERR(
			"%s unknown/unsupported keytype %d\n",
			__func__, keytype);
		rc = -EINVAL;
		goto out;
	}
	memcpy(preqparm->lv1.key_type1,  "AESDATA ", 8);
	preqparm->lv2.len = sizeof(struct lv2);
	for (i = 0; i < 6; i++) {
		preqparm->lv2.keyid[i].len = sizeof(struct keyid);
		preqparm->lv2.keyid[i].attr = (i == 2 ? 0x30 : 0x10);
	}
	preqcblk->req_parml = sizeof(struct kgreqparm);

	/* fill xcrb struct */
	prep_xcrb(&xcrb, cardnr, preqcblk, prepcblk);

	/* forward xcrb with request CPRB and reply CPRB to zcrypt dd */
	rc = _zcrypt_send_cprb(&xcrb);
	if (rc) {
		DEBUG_ERR(
			"%s zcrypt_send_cprb (cardnr=%d domain=%d) failed with errno %d\n",
			__func__, (int) cardnr, (int) domain, rc);
		goto out;
	}

	/* check response returncode and reasoncode */
	if (prepcblk->ccp_rtcode != 0) {
		DEBUG_ERR(
			"%s secure key generate failure, card response %d/%d\n",
			__func__,
			(int) prepcblk->ccp_rtcode,
			(int) prepcblk->ccp_rscode);
		rc = -EIO;
		goto out;
	}

	/* process response cprb param block */
	prepcblk->rpl_parmb = ((u8 *) prepcblk) + sizeof(struct CPRBX);
	prepparm = (struct kgrepparm *) prepcblk->rpl_parmb;

	/* check length of the returned secure key token */
	seckeysize = prepparm->lv3.keyblock.toklen
		- sizeof(prepparm->lv3.keyblock.toklen)
		- sizeof(prepparm->lv3.keyblock.tokattr);
	if (seckeysize != SECKEYBLOBSIZE) {
		DEBUG_ERR(
			"%s secure token size mismatch %d != %d bytes\n",
			__func__, seckeysize, SECKEYBLOBSIZE);
		rc = -EIO;
		goto out;
	}

	/* check secure key token */
	rc = check_secaeskeytoken(prepparm->lv3.keyblock.tok, 8*keysize);
	if (rc) {
		rc = -EIO;
		goto out;
	}

	/* copy the generated secure key token */
	memcpy(seckey->seckey, prepparm->lv3.keyblock.tok, SECKEYBLOBSIZE);

out:
	free_cprbmem(mem, PARMBSIZE, 0);
	return rc;
}
EXPORT_SYMBOL(pkey_genseckey);

/*
 * Generate an AES secure key with given key value.
 */
int pkey_clr2seckey(u16 cardnr, u16 domain, u32 keytype,
		    const struct pkey_clrkey *clrkey,
		    struct pkey_seckey *seckey)
{
	int rc, keysize, seckeysize;
	u8 *mem;
	struct CPRBX *preqcblk, *prepcblk;
	struct ica_xcRB xcrb;
	struct cmreqparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		char  rule_array[8];
		struct lv1 {
			u16 len;
			u8  clrkey[0];
		} lv1;
		struct lv2 {
			u16 len;
			struct keyid {
				u16 len;
				u16 attr;
				u8  data[SECKEYBLOBSIZE];
			} keyid;
		} lv2;
	} *preqparm;
	struct lv2 *plv2;
	struct cmrepparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		struct lv3 {
			u16 len;
			u16 keyblocklen;
			struct {
				u16 toklen;
				u16 tokattr;
				u8  tok[0];
				/* ... some more data ... */
			} keyblock;
		} lv3;
	} *prepparm;

	/* get already prepared memory for 2 cprbs with param block each */
	rc = alloc_and_prep_cprbmem(PARMBSIZE, &mem, &preqcblk, &prepcblk);
	if (rc)
		return rc;

	/* fill request cprb struct */
	preqcblk->domain = domain;

	/* fill request cprb param block with CM request */
	preqparm = (struct cmreqparm *) preqcblk->req_parmb;
	memcpy(preqparm->subfunc_code, "CM", 2);
	memcpy(preqparm->rule_array, "AES     ", 8);
	preqparm->rule_array_len =
		sizeof(preqparm->rule_array_len) + sizeof(preqparm->rule_array);
	switch (keytype) {
	case PKEY_KEYTYPE_AES_128:
		keysize = 16;
		break;
	case PKEY_KEYTYPE_AES_192:
		keysize = 24;
		break;
	case PKEY_KEYTYPE_AES_256:
		keysize = 32;
		break;
	default:
		DEBUG_ERR(
			"%s unknown/unsupported keytype %d\n",
			__func__, keytype);
		rc = -EINVAL;
		goto out;
	}
	preqparm->lv1.len = sizeof(struct lv1) + keysize;
	memcpy(preqparm->lv1.clrkey, clrkey->clrkey, keysize);
	plv2 = (struct lv2 *) (((u8 *) &preqparm->lv2) + keysize);
	plv2->len = sizeof(struct lv2);
	plv2->keyid.len = sizeof(struct keyid);
	plv2->keyid.attr = 0x30;
	preqcblk->req_parml = sizeof(struct cmreqparm) + keysize;

	/* fill xcrb struct */
	prep_xcrb(&xcrb, cardnr, preqcblk, prepcblk);

	/* forward xcrb with request CPRB and reply CPRB to zcrypt dd */
	rc = _zcrypt_send_cprb(&xcrb);
	if (rc) {
		DEBUG_ERR(
			"%s zcrypt_send_cprb (cardnr=%d domain=%d) failed with errno %d\n",
			__func__, (int) cardnr, (int) domain, rc);
		goto out;
	}

	/* check response returncode and reasoncode */
	if (prepcblk->ccp_rtcode != 0) {
		DEBUG_ERR(
			"%s clear key import failure, card response %d/%d\n",
			__func__,
			(int) prepcblk->ccp_rtcode,
			(int) prepcblk->ccp_rscode);
		rc = -EIO;
		goto out;
	}

	/* process response cprb param block */
	prepcblk->rpl_parmb = ((u8 *) prepcblk) + sizeof(struct CPRBX);
	prepparm = (struct cmrepparm *) prepcblk->rpl_parmb;

	/* check length of the returned secure key token */
	seckeysize = prepparm->lv3.keyblock.toklen
		- sizeof(prepparm->lv3.keyblock.toklen)
		- sizeof(prepparm->lv3.keyblock.tokattr);
	if (seckeysize != SECKEYBLOBSIZE) {
		DEBUG_ERR(
			"%s secure token size mismatch %d != %d bytes\n",
			__func__, seckeysize, SECKEYBLOBSIZE);
		rc = -EIO;
		goto out;
	}

	/* check secure key token */
	rc = check_secaeskeytoken(prepparm->lv3.keyblock.tok, 8*keysize);
	if (rc) {
		rc = -EIO;
		goto out;
	}

	/* copy the generated secure key token */
	memcpy(seckey->seckey, prepparm->lv3.keyblock.tok, SECKEYBLOBSIZE);

out:
	free_cprbmem(mem, PARMBSIZE, 1);
	return rc;
}
EXPORT_SYMBOL(pkey_clr2seckey);

/*
 * Derive a proteced key from the secure key blob.
 */
int pkey_sec2protkey(u16 cardnr, u16 domain,
		     const struct pkey_seckey *seckey,
		     struct pkey_protkey *protkey)
{
	int rc;
	u8 *mem;
	struct CPRBX *preqcblk, *prepcblk;
	struct ica_xcRB xcrb;
	struct uskreqparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		struct lv1 {
			u16 len;
			u16 attr_len;
			u16 attr_flags;
		} lv1;
		struct lv2 {
			u16 len;
			u16 attr_len;
			u16 attr_flags;
			u8  token[0];	      /* cca secure key token */
		} lv2 __packed;
	} *preqparm;
	struct uskrepparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		struct lv3 {
			u16 len;
			u16 attr_len;
			u16 attr_flags;
			struct cpacfkeyblock {
				u8  version;  /* version of this struct */
				u8  flags[2];
				u8  algo;
				u8  form;
				u8  pad1[3];
				u16 keylen;
				u8  key[64];  /* the key (keylen bytes) */
				u16 keyattrlen;
				u8  keyattr[32];
				u8  pad2[1];
				u8  vptype;
				u8  vp[32];  /* verification pattern */
			} keyblock;
		} lv3 __packed;
	} *prepparm;

	/* get already prepared memory for 2 cprbs with param block each */
	rc = alloc_and_prep_cprbmem(PARMBSIZE, &mem, &preqcblk, &prepcblk);
	if (rc)
		return rc;

	/* fill request cprb struct */
	preqcblk->domain = domain;

	/* fill request cprb param block with USK request */
	preqparm = (struct uskreqparm *) preqcblk->req_parmb;
	memcpy(preqparm->subfunc_code, "US", 2);
	preqparm->rule_array_len = sizeof(preqparm->rule_array_len);
	preqparm->lv1.len = sizeof(struct lv1);
	preqparm->lv1.attr_len = sizeof(struct lv1) - sizeof(preqparm->lv1.len);
	preqparm->lv1.attr_flags = 0x0001;
	preqparm->lv2.len = sizeof(struct lv2) + SECKEYBLOBSIZE;
	preqparm->lv2.attr_len = sizeof(struct lv2)
		- sizeof(preqparm->lv2.len) + SECKEYBLOBSIZE;
	preqparm->lv2.attr_flags = 0x0000;
	memcpy(preqparm->lv2.token, seckey->seckey, SECKEYBLOBSIZE);
	preqcblk->req_parml = sizeof(struct uskreqparm) + SECKEYBLOBSIZE;

	/* fill xcrb struct */
	prep_xcrb(&xcrb, cardnr, preqcblk, prepcblk);

	/* forward xcrb with request CPRB and reply CPRB to zcrypt dd */
	rc = _zcrypt_send_cprb(&xcrb);
	if (rc) {
		DEBUG_ERR(
			"%s zcrypt_send_cprb (cardnr=%d domain=%d) failed with errno %d\n",
			__func__, (int) cardnr, (int) domain, rc);
		goto out;
	}

	/* check response returncode and reasoncode */
	if (prepcblk->ccp_rtcode != 0) {
		DEBUG_ERR(
			"%s unwrap secure key failure, card response %d/%d\n",
			__func__,
			(int) prepcblk->ccp_rtcode,
			(int) prepcblk->ccp_rscode);
		rc = -EIO;
		goto out;
	}
	if (prepcblk->ccp_rscode != 0) {
		DEBUG_WARN(
			"%s unwrap secure key warning, card response %d/%d\n",
			__func__,
			(int) prepcblk->ccp_rtcode,
			(int) prepcblk->ccp_rscode);
	}

	/* process response cprb param block */
	prepcblk->rpl_parmb = ((u8 *) prepcblk) + sizeof(struct CPRBX);
	prepparm = (struct uskrepparm *) prepcblk->rpl_parmb;

	/* check the returned keyblock */
	if (prepparm->lv3.keyblock.version != 0x01) {
		DEBUG_ERR(
			"%s reply param keyblock version mismatch 0x%02x != 0x01\n",
			__func__, (int) prepparm->lv3.keyblock.version);
		rc = -EIO;
		goto out;
	}

	/* copy the tanslated protected key */
	switch (prepparm->lv3.keyblock.keylen) {
	case 16+32:
		protkey->type = PKEY_KEYTYPE_AES_128;
		break;
	case 24+32:
		protkey->type = PKEY_KEYTYPE_AES_192;
		break;
	case 32+32:
		protkey->type = PKEY_KEYTYPE_AES_256;
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported keytype %d\n",
			  __func__, prepparm->lv3.keyblock.keylen);
		rc = -EIO;
		goto out;
	}
	protkey->len = prepparm->lv3.keyblock.keylen;
	memcpy(protkey->protkey, prepparm->lv3.keyblock.key, protkey->len);

out:
	free_cprbmem(mem, PARMBSIZE, 0);
	return rc;
}
EXPORT_SYMBOL(pkey_sec2protkey);
=======
/* inside view of a protected key token (only type 0x00 version 0x01) */
struct protaeskeytoken {
	u8  type;     /* 0x00 for PAES specific key tokens */
	u8  res0[3];
	u8  version;  /* should be 0x01 for protected AES key token */
	u8  res1[3];
	u32 keytype;  /* key type, one of the PKEY_KEYTYPE values */
	u32 len;      /* bytes actually stored in protkey[] */
	u8  protkey[MAXPROTKEYSIZE]; /* the protected key blob */
} __packed;

/* inside view of a clear key token (type 0x00 version 0x02) */
struct clearaeskeytoken {
	u8  type;	 /* 0x00 for PAES specific key tokens */
	u8  res0[3];
	u8  version;	 /* 0x02 for clear AES key token */
	u8  res1[3];
	u32 keytype;	 /* key type, one of the PKEY_KEYTYPE values */
	u32 len;	 /* bytes actually stored in clearkey[] */
	u8  clearkey[]; /* clear key value */
} __packed;
>>>>>>> upstream/android-13

/*
 * Create a protected key from a clear key value.
 */
<<<<<<< HEAD
int pkey_clr2protkey(u32 keytype,
		     const struct pkey_clrkey *clrkey,
		     struct pkey_protkey *protkey)
{
=======
static int pkey_clr2protkey(u32 keytype,
			    const struct pkey_clrkey *clrkey,
			    struct pkey_protkey *protkey)
{
	/* mask of available pckmo subfunctions */
	static cpacf_mask_t pckmo_functions;

>>>>>>> upstream/android-13
	long fc;
	int keysize;
	u8 paramblock[64];

	switch (keytype) {
	case PKEY_KEYTYPE_AES_128:
		keysize = 16;
		fc = CPACF_PCKMO_ENC_AES_128_KEY;
		break;
	case PKEY_KEYTYPE_AES_192:
		keysize = 24;
		fc = CPACF_PCKMO_ENC_AES_192_KEY;
		break;
	case PKEY_KEYTYPE_AES_256:
		keysize = 32;
		fc = CPACF_PCKMO_ENC_AES_256_KEY;
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported keytype %d\n",
			  __func__, keytype);
		return -EINVAL;
	}

<<<<<<< HEAD
=======
	/* Did we already check for PCKMO ? */
	if (!pckmo_functions.bytes[0]) {
		/* no, so check now */
		if (!cpacf_query(CPACF_PCKMO, &pckmo_functions))
			return -ENODEV;
	}
	/* check for the pckmo subfunction we need now */
	if (!cpacf_test_func(&pckmo_functions, fc)) {
		DEBUG_ERR("%s pckmo functions not available\n", __func__);
		return -ENODEV;
	}

>>>>>>> upstream/android-13
	/* prepare param block */
	memset(paramblock, 0, sizeof(paramblock));
	memcpy(paramblock, clrkey->clrkey, keysize);

	/* call the pckmo instruction */
	cpacf_pckmo(fc, paramblock);

	/* copy created protected key */
	protkey->type = keytype;
	protkey->len = keysize + 32;
	memcpy(protkey->protkey, paramblock, keysize + 32);

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(pkey_clr2protkey);

/*
 * query cryptographic facility from adapter
 */
static int query_crypto_facility(u16 cardnr, u16 domain,
				 const char *keyword,
				 u8 *rarray, size_t *rarraylen,
				 u8 *varray, size_t *varraylen)
{
	int rc;
	u16 len;
	u8 *mem, *ptr;
	struct CPRBX *preqcblk, *prepcblk;
	struct ica_xcRB xcrb;
	struct fqreqparm {
		u8  subfunc_code[2];
		u16 rule_array_len;
		char  rule_array[8];
		struct lv1 {
			u16 len;
			u8  data[VARDATASIZE];
		} lv1;
		u16 dummylen;
	} *preqparm;
	size_t parmbsize = sizeof(struct fqreqparm);
	struct fqrepparm {
		u8  subfunc_code[2];
		u8  lvdata[0];
	} *prepparm;

	/* get already prepared memory for 2 cprbs with param block each */
	rc = alloc_and_prep_cprbmem(parmbsize, &mem, &preqcblk, &prepcblk);
	if (rc)
		return rc;

	/* fill request cprb struct */
	preqcblk->domain = domain;

	/* fill request cprb param block with FQ request */
	preqparm = (struct fqreqparm *) preqcblk->req_parmb;
	memcpy(preqparm->subfunc_code, "FQ", 2);
	memcpy(preqparm->rule_array, keyword, sizeof(preqparm->rule_array));
	preqparm->rule_array_len =
		sizeof(preqparm->rule_array_len) + sizeof(preqparm->rule_array);
	preqparm->lv1.len = sizeof(preqparm->lv1);
	preqparm->dummylen = sizeof(preqparm->dummylen);
	preqcblk->req_parml = parmbsize;

	/* fill xcrb struct */
	prep_xcrb(&xcrb, cardnr, preqcblk, prepcblk);

	/* forward xcrb with request CPRB and reply CPRB to zcrypt dd */
	rc = _zcrypt_send_cprb(&xcrb);
	if (rc) {
		DEBUG_ERR(
			"%s zcrypt_send_cprb (cardnr=%d domain=%d) failed with errno %d\n",
			__func__, (int) cardnr, (int) domain, rc);
		goto out;
	}

	/* check response returncode and reasoncode */
	if (prepcblk->ccp_rtcode != 0) {
		DEBUG_ERR(
			"%s unwrap secure key failure, card response %d/%d\n",
			__func__,
			(int) prepcblk->ccp_rtcode,
			(int) prepcblk->ccp_rscode);
		rc = -EIO;
		goto out;
	}

	/* process response cprb param block */
	prepcblk->rpl_parmb = ((u8 *) prepcblk) + sizeof(struct CPRBX);
	prepparm = (struct fqrepparm *) prepcblk->rpl_parmb;
	ptr = prepparm->lvdata;

	/* check and possibly copy reply rule array */
	len = *((u16 *) ptr);
	if (len > sizeof(u16)) {
		ptr += sizeof(u16);
		len -= sizeof(u16);
		if (rarray && rarraylen && *rarraylen > 0) {
			*rarraylen = (len > *rarraylen ? *rarraylen : len);
			memcpy(rarray, ptr, *rarraylen);
		}
		ptr += len;
	}
	/* check and possible copy reply var array */
	len = *((u16 *) ptr);
	if (len > sizeof(u16)) {
		ptr += sizeof(u16);
		len -= sizeof(u16);
		if (varray && varraylen && *varraylen > 0) {
			*varraylen = (len > *varraylen ? *varraylen : len);
			memcpy(varray, ptr, *varraylen);
		}
		ptr += len;
	}

out:
	free_cprbmem(mem, parmbsize, 0);
	return rc;
}

/*
 * Fetch the current and old mkvp values via
 * query_crypto_facility from adapter.
 */
static int fetch_mkvp(u16 cardnr, u16 domain, u64 mkvp[2])
{
	int rc, found = 0;
	size_t rlen, vlen;
	u8 *rarray, *varray, *pg;

	pg = (u8 *) __get_free_page(GFP_KERNEL);
	if (!pg)
		return -ENOMEM;
	rarray = pg;
	varray = pg + PAGE_SIZE/2;
	rlen = vlen = PAGE_SIZE/2;

	rc = query_crypto_facility(cardnr, domain, "STATICSA",
				   rarray, &rlen, varray, &vlen);
	if (rc == 0 && rlen > 8*8 && vlen > 184+8) {
		if (rarray[8*8] == '2') {
			/* current master key state is valid */
			mkvp[0] = *((u64 *)(varray + 184));
			mkvp[1] = *((u64 *)(varray + 172));
			found = 1;
		}
	}

	free_page((unsigned long) pg);

	return found ? 0 : -ENOENT;
}

/* struct to hold cached mkvp info for each card/domain */
struct mkvp_info {
	struct list_head list;
	u16 cardnr;
	u16 domain;
	u64 mkvp[2];
};

/* a list with mkvp_info entries */
static LIST_HEAD(mkvp_list);
static DEFINE_SPINLOCK(mkvp_list_lock);

static int mkvp_cache_fetch(u16 cardnr, u16 domain, u64 mkvp[2])
{
	int rc = -ENOENT;
	struct mkvp_info *ptr;

	spin_lock_bh(&mkvp_list_lock);
	list_for_each_entry(ptr, &mkvp_list, list) {
		if (ptr->cardnr == cardnr &&
		    ptr->domain == domain) {
			memcpy(mkvp, ptr->mkvp, 2 * sizeof(u64));
			rc = 0;
			break;
		}
	}
	spin_unlock_bh(&mkvp_list_lock);

	return rc;
}

static void mkvp_cache_update(u16 cardnr, u16 domain, u64 mkvp[2])
{
	int found = 0;
	struct mkvp_info *ptr;

	spin_lock_bh(&mkvp_list_lock);
	list_for_each_entry(ptr, &mkvp_list, list) {
		if (ptr->cardnr == cardnr &&
		    ptr->domain == domain) {
			memcpy(ptr->mkvp, mkvp, 2 * sizeof(u64));
			found = 1;
			break;
		}
	}
	if (!found) {
		ptr = kmalloc(sizeof(*ptr), GFP_ATOMIC);
		if (!ptr) {
			spin_unlock_bh(&mkvp_list_lock);
			return;
		}
		ptr->cardnr = cardnr;
		ptr->domain = domain;
		memcpy(ptr->mkvp, mkvp, 2 * sizeof(u64));
		list_add(&ptr->list, &mkvp_list);
	}
	spin_unlock_bh(&mkvp_list_lock);
}

static void mkvp_cache_scrub(u16 cardnr, u16 domain)
{
	struct mkvp_info *ptr;

	spin_lock_bh(&mkvp_list_lock);
	list_for_each_entry(ptr, &mkvp_list, list) {
		if (ptr->cardnr == cardnr &&
		    ptr->domain == domain) {
			list_del(&ptr->list);
			kfree(ptr);
			break;
		}
	}
	spin_unlock_bh(&mkvp_list_lock);
}

static void __exit mkvp_cache_free(void)
{
	struct mkvp_info *ptr, *pnext;

	spin_lock_bh(&mkvp_list_lock);
	list_for_each_entry_safe(ptr, pnext, &mkvp_list, list) {
		list_del(&ptr->list);
		kfree(ptr);
	}
	spin_unlock_bh(&mkvp_list_lock);
}

/*
 * Search for a matching crypto card based on the Master Key
 * Verification Pattern provided inside a secure key.
 */
int pkey_findcard(const struct pkey_seckey *seckey,
		  u16 *pcardnr, u16 *pdomain, int verify)
{
	struct secaeskeytoken *t = (struct secaeskeytoken *) seckey;
	struct zcrypt_device_status_ext *device_status;
	u16 card, dom;
	u64 mkvp[2];
	int i, rc, oi = -1;

	/* mkvp must not be zero */
	if (t->mkvp == 0)
		return -EINVAL;

	/* fetch status of all crypto cards */
	device_status = kmalloc_array(MAX_ZDEV_ENTRIES_EXT,
				      sizeof(struct zcrypt_device_status_ext),
				      GFP_KERNEL);
	if (!device_status)
		return -ENOMEM;
	zcrypt_device_status_mask_ext(device_status);

	/* walk through all crypto cards */
	for (i = 0; i < MAX_ZDEV_ENTRIES_EXT; i++) {
		card = AP_QID_CARD(device_status[i].qid);
		dom = AP_QID_QUEUE(device_status[i].qid);
		if (device_status[i].online &&
		    device_status[i].functions & 0x04) {
			/* an enabled CCA Coprocessor card */
			/* try cached mkvp */
			if (mkvp_cache_fetch(card, dom, mkvp) == 0 &&
			    t->mkvp == mkvp[0]) {
				if (!verify)
					break;
				/* verify: fetch mkvp from adapter */
				if (fetch_mkvp(card, dom, mkvp) == 0) {
					mkvp_cache_update(card, dom, mkvp);
					if (t->mkvp == mkvp[0])
						break;
				}
			}
		} else {
			/* Card is offline and/or not a CCA card. */
			/* del mkvp entry from cache if it exists */
			mkvp_cache_scrub(card, dom);
		}
	}
	if (i >= MAX_ZDEV_ENTRIES_EXT) {
		/* nothing found, so this time without cache */
		for (i = 0; i < MAX_ZDEV_ENTRIES_EXT; i++) {
			if (!(device_status[i].online &&
			      device_status[i].functions & 0x04))
				continue;
			card = AP_QID_CARD(device_status[i].qid);
			dom = AP_QID_QUEUE(device_status[i].qid);
			/* fresh fetch mkvp from adapter */
			if (fetch_mkvp(card, dom, mkvp) == 0) {
				mkvp_cache_update(card, dom, mkvp);
				if (t->mkvp == mkvp[0])
					break;
				if (t->mkvp == mkvp[1] && oi < 0)
					oi = i;
			}
		}
		if (i >= MAX_ZDEV_ENTRIES_EXT && oi >= 0) {
			/* old mkvp matched, use this card then */
			card = AP_QID_CARD(device_status[oi].qid);
			dom = AP_QID_QUEUE(device_status[oi].qid);
		}
	}
	if (i < MAX_ZDEV_ENTRIES_EXT || oi >= 0) {
		if (pcardnr)
			*pcardnr = card;
		if (pdomain)
			*pdomain = dom;
		rc = 0;
	} else
		rc = -ENODEV;

	kfree(device_status);
	return rc;
}
EXPORT_SYMBOL(pkey_findcard);
=======
>>>>>>> upstream/android-13

/*
 * Find card and transform secure key into protected key.
 */
<<<<<<< HEAD
int pkey_skey2pkey(const struct pkey_seckey *seckey,
		   struct pkey_protkey *protkey)
{
	u16 cardnr, domain;
	int rc, verify;

	/*
	 * The pkey_sec2protkey call may fail when a card has been
	 * addressed where the master key was changed after last fetch
	 * of the mkvp into the cache. So first try without verify then
	 * with verify enabled (thus refreshing the mkvp for each card).
	 */
	for (verify = 0; verify < 2; verify++) {
		rc = pkey_findcard(seckey, &cardnr, &domain, verify);
		if (rc)
			continue;
		rc = pkey_sec2protkey(cardnr, domain, seckey, protkey);
=======
static int pkey_skey2pkey(const u8 *key, struct pkey_protkey *pkey)
{
	int rc, verify;
	u16 cardnr, domain;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	zcrypt_wait_api_operational();

	/*
	 * The cca_xxx2protkey call may fail when a card has been
	 * addressed where the master key was changed after last fetch
	 * of the mkvp into the cache. Try 3 times: First witout verify
	 * then with verify and last round with verify and old master
	 * key verification pattern match not ignored.
	 */
	for (verify = 0; verify < 3; verify++) {
		rc = cca_findcard(key, &cardnr, &domain, verify);
		if (rc < 0)
			continue;
		if (rc > 0 && verify < 2)
			continue;
		switch (hdr->version) {
		case TOKVER_CCA_AES:
			rc = cca_sec2protkey(cardnr, domain,
					     key, pkey->protkey,
					     &pkey->len, &pkey->type);
			break;
		case TOKVER_CCA_VLSC:
			rc = cca_cipher2protkey(cardnr, domain,
						key, pkey->protkey,
						&pkey->len, &pkey->type);
			break;
		default:
			return -EINVAL;
		}
>>>>>>> upstream/android-13
		if (rc == 0)
			break;
	}

	if (rc)
		DEBUG_DBG("%s failed rc=%d\n", __func__, rc);

	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL(pkey_skey2pkey);
=======

/*
 * Construct EP11 key with given clear key value.
 */
static int pkey_clr2ep11key(const u8 *clrkey, size_t clrkeylen,
			    u8 *keybuf, size_t *keybuflen)
{
	int i, rc;
	u16 card, dom;
	u32 nr_apqns, *apqns = NULL;

	zcrypt_wait_api_operational();

	/* build a list of apqns suitable for ep11 keys with cpacf support */
	rc = ep11_findcard2(&apqns, &nr_apqns, 0xFFFF, 0xFFFF,
			    ZCRYPT_CEX7, EP11_API_V, NULL);
	if (rc)
		goto out;

	/* go through the list of apqns and try to bild an ep11 key */
	for (rc = -ENODEV, i = 0; i < nr_apqns; i++) {
		card = apqns[i] >> 16;
		dom = apqns[i] & 0xFFFF;
		rc = ep11_clr2keyblob(card, dom, clrkeylen * 8,
				      0, clrkey, keybuf, keybuflen);
		if (rc == 0)
			break;
	}

out:
	kfree(apqns);
	if (rc)
		DEBUG_DBG("%s failed rc=%d\n", __func__, rc);
	return rc;
}

/*
 * Find card and transform EP11 secure key into protected key.
 */
static int pkey_ep11key2pkey(const u8 *key, struct pkey_protkey *pkey)
{
	int i, rc;
	u16 card, dom;
	u32 nr_apqns, *apqns = NULL;
	struct ep11keyblob *kb = (struct ep11keyblob *) key;

	zcrypt_wait_api_operational();

	/* build a list of apqns suitable for this key */
	rc = ep11_findcard2(&apqns, &nr_apqns, 0xFFFF, 0xFFFF,
			    ZCRYPT_CEX7, EP11_API_V, kb->wkvp);
	if (rc)
		goto out;

	/* go through the list of apqns and try to derive an pkey */
	for (rc = -ENODEV, i = 0; i < nr_apqns; i++) {
		card = apqns[i] >> 16;
		dom = apqns[i] & 0xFFFF;
		pkey->len = sizeof(pkey->protkey);
		rc = ep11_kblob2protkey(card, dom, key, kb->head.len,
					pkey->protkey, &pkey->len, &pkey->type);
		if (rc == 0)
			break;
	}

out:
	kfree(apqns);
	if (rc)
		DEBUG_DBG("%s failed rc=%d\n", __func__, rc);
	return rc;
}
>>>>>>> upstream/android-13

/*
 * Verify key and give back some info about the key.
 */
<<<<<<< HEAD
int pkey_verifykey(const struct pkey_seckey *seckey,
		   u16 *pcardnr, u16 *pdomain,
		   u16 *pkeysize, u32 *pattributes)
{
	struct secaeskeytoken *t = (struct secaeskeytoken *) seckey;
	u16 cardnr, domain;
	u64 mkvp[2];
	int rc;

	/* check the secure key for valid AES secure key */
	rc = check_secaeskeytoken((u8 *) seckey, 0);
=======
static int pkey_verifykey(const struct pkey_seckey *seckey,
			  u16 *pcardnr, u16 *pdomain,
			  u16 *pkeysize, u32 *pattributes)
{
	struct secaeskeytoken *t = (struct secaeskeytoken *) seckey;
	u16 cardnr, domain;
	int rc;

	/* check the secure key for valid AES secure key */
	rc = cca_check_secaeskeytoken(debug_info, 3, (u8 *) seckey, 0);
>>>>>>> upstream/android-13
	if (rc)
		goto out;
	if (pattributes)
		*pattributes = PKEY_VERIFY_ATTR_AES;
	if (pkeysize)
		*pkeysize = t->bitsize;

	/* try to find a card which can handle this key */
<<<<<<< HEAD
	rc = pkey_findcard(seckey, &cardnr, &domain, 1);
	if (rc)
		goto out;

	/* check mkvp for old mkvp match */
	rc = mkvp_cache_fetch(cardnr, domain, mkvp);
	if (rc)
		goto out;
	if (t->mkvp == mkvp[1]) {
		DEBUG_DBG("%s secure key has old mkvp\n", __func__);
		if (pattributes)
			*pattributes |= PKEY_VERIFY_ATTR_OLD_MKVP;
=======
	rc = cca_findcard(seckey->seckey, &cardnr, &domain, 1);
	if (rc < 0)
		goto out;

	if (rc > 0) {
		/* key mkvp matches to old master key mkvp */
		DEBUG_DBG("%s secure key has old mkvp\n", __func__);
		if (pattributes)
			*pattributes |= PKEY_VERIFY_ATTR_OLD_MKVP;
		rc = 0;
>>>>>>> upstream/android-13
	}

	if (pcardnr)
		*pcardnr = cardnr;
	if (pdomain)
		*pdomain = domain;

out:
	DEBUG_DBG("%s rc=%d\n", __func__, rc);
	return rc;
}
<<<<<<< HEAD
EXPORT_SYMBOL(pkey_verifykey);
=======

/*
 * Generate a random protected key
 */
static int pkey_genprotkey(u32 keytype, struct pkey_protkey *protkey)
{
	struct pkey_clrkey clrkey;
	int keysize;
	int rc;

	switch (keytype) {
	case PKEY_KEYTYPE_AES_128:
		keysize = 16;
		break;
	case PKEY_KEYTYPE_AES_192:
		keysize = 24;
		break;
	case PKEY_KEYTYPE_AES_256:
		keysize = 32;
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported keytype %d\n", __func__,
			  keytype);
		return -EINVAL;
	}

	/* generate a dummy random clear key */
	get_random_bytes(clrkey.clrkey, keysize);

	/* convert it to a dummy protected key */
	rc = pkey_clr2protkey(keytype, &clrkey, protkey);
	if (rc)
		return rc;

	/* replace the key part of the protected key with random bytes */
	get_random_bytes(protkey->protkey, keysize);

	return 0;
}

/*
 * Verify if a protected key is still valid
 */
static int pkey_verifyprotkey(const struct pkey_protkey *protkey)
{
	unsigned long fc;
	struct {
		u8 iv[AES_BLOCK_SIZE];
		u8 key[MAXPROTKEYSIZE];
	} param;
	u8 null_msg[AES_BLOCK_SIZE];
	u8 dest_buf[AES_BLOCK_SIZE];
	unsigned int k;

	switch (protkey->type) {
	case PKEY_KEYTYPE_AES_128:
		fc = CPACF_KMC_PAES_128;
		break;
	case PKEY_KEYTYPE_AES_192:
		fc = CPACF_KMC_PAES_192;
		break;
	case PKEY_KEYTYPE_AES_256:
		fc = CPACF_KMC_PAES_256;
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported keytype %d\n", __func__,
			  protkey->type);
		return -EINVAL;
	}

	memset(null_msg, 0, sizeof(null_msg));

	memset(param.iv, 0, sizeof(param.iv));
	memcpy(param.key, protkey->protkey, sizeof(param.key));

	k = cpacf_kmc(fc | CPACF_ENCRYPT, &param, null_msg, dest_buf,
		      sizeof(null_msg));
	if (k != sizeof(null_msg)) {
		DEBUG_ERR("%s protected key is not valid\n", __func__);
		return -EKEYREJECTED;
	}

	return 0;
}

/*
 * Transform a non-CCA key token into a protected key
 */
static int pkey_nonccatok2pkey(const u8 *key, u32 keylen,
			       struct pkey_protkey *protkey)
{
	int rc = -EINVAL;
	u8 *tmpbuf = NULL;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	switch (hdr->version) {
	case TOKVER_PROTECTED_KEY: {
		struct protaeskeytoken *t;

		if (keylen != sizeof(struct protaeskeytoken))
			goto out;
		t = (struct protaeskeytoken *)key;
		protkey->len = t->len;
		protkey->type = t->keytype;
		memcpy(protkey->protkey, t->protkey,
		       sizeof(protkey->protkey));
		rc = pkey_verifyprotkey(protkey);
		break;
	}
	case TOKVER_CLEAR_KEY: {
		struct clearaeskeytoken *t;
		struct pkey_clrkey ckey;
		union u_tmpbuf {
			u8 skey[SECKEYBLOBSIZE];
			u8 ep11key[MAXEP11AESKEYBLOBSIZE];
		};
		size_t tmpbuflen = sizeof(union u_tmpbuf);

		if (keylen < sizeof(struct clearaeskeytoken))
			goto out;
		t = (struct clearaeskeytoken *)key;
		if (keylen != sizeof(*t) + t->len)
			goto out;
		if ((t->keytype == PKEY_KEYTYPE_AES_128 && t->len == 16)
		    || (t->keytype == PKEY_KEYTYPE_AES_192 && t->len == 24)
		    || (t->keytype == PKEY_KEYTYPE_AES_256 && t->len == 32))
			memcpy(ckey.clrkey, t->clearkey, t->len);
		else
			goto out;
		/* alloc temp key buffer space */
		tmpbuf = kmalloc(tmpbuflen, GFP_ATOMIC);
		if (!tmpbuf) {
			rc = -ENOMEM;
			goto out;
		}
		/* try direct way with the PCKMO instruction */
		rc = pkey_clr2protkey(t->keytype, &ckey, protkey);
		if (rc == 0)
			break;
		/* PCKMO failed, so try the CCA secure key way */
		zcrypt_wait_api_operational();
		rc = cca_clr2seckey(0xFFFF, 0xFFFF, t->keytype,
				    ckey.clrkey, tmpbuf);
		if (rc == 0)
			rc = pkey_skey2pkey(tmpbuf, protkey);
		if (rc == 0)
			break;
		/* if the CCA way also failed, let's try via EP11 */
		rc = pkey_clr2ep11key(ckey.clrkey, t->len,
				      tmpbuf, &tmpbuflen);
		if (rc == 0)
			rc = pkey_ep11key2pkey(tmpbuf, protkey);
		/* now we should really have an protected key */
		DEBUG_ERR("%s unable to build protected key from clear",
			  __func__);
		break;
	}
	case TOKVER_EP11_AES: {
		/* check ep11 key for exportable as protected key */
		rc = ep11_check_aes_key(debug_info, 3, key, keylen, 1);
		if (rc)
			goto out;
		rc = pkey_ep11key2pkey(key, protkey);
		break;
	}
	case TOKVER_EP11_AES_WITH_HEADER:
		/* check ep11 key with header for exportable as protected key */
		rc = ep11_check_aes_key_with_hdr(debug_info, 3, key, keylen, 1);
		if (rc)
			goto out;
		rc = pkey_ep11key2pkey(key + sizeof(struct ep11kblob_header),
				       protkey);
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported non-CCA token version %d\n",
			  __func__, hdr->version);
		rc = -EINVAL;
	}

out:
	kfree(tmpbuf);
	return rc;
}

/*
 * Transform a CCA internal key token into a protected key
 */
static int pkey_ccainttok2pkey(const u8 *key, u32 keylen,
			       struct pkey_protkey *protkey)
{
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	switch (hdr->version) {
	case TOKVER_CCA_AES:
		if (keylen != sizeof(struct secaeskeytoken))
			return -EINVAL;
		break;
	case TOKVER_CCA_VLSC:
		if (keylen < hdr->len || keylen > MAXCCAVLSCTOKENSIZE)
			return -EINVAL;
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported CCA internal token version %d\n",
			  __func__, hdr->version);
		return -EINVAL;
	}

	return pkey_skey2pkey(key, protkey);
}

/*
 * Transform a key blob (of any type) into a protected key
 */
int pkey_keyblob2pkey(const u8 *key, u32 keylen,
		      struct pkey_protkey *protkey)
{
	int rc;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	if (keylen < sizeof(struct keytoken_header)) {
		DEBUG_ERR("%s invalid keylen %d\n", __func__, keylen);
		return -EINVAL;
	}

	switch (hdr->type) {
	case TOKTYPE_NON_CCA:
		rc = pkey_nonccatok2pkey(key, keylen, protkey);
		break;
	case TOKTYPE_CCA_INTERNAL:
		rc = pkey_ccainttok2pkey(key, keylen, protkey);
		break;
	default:
		DEBUG_ERR("%s unknown/unsupported blob type %d\n",
			  __func__, hdr->type);
		return -EINVAL;
	}

	DEBUG_DBG("%s rc=%d\n", __func__, rc);
	return rc;

}
EXPORT_SYMBOL(pkey_keyblob2pkey);

static int pkey_genseckey2(const struct pkey_apqn *apqns, size_t nr_apqns,
			   enum pkey_key_type ktype, enum pkey_key_size ksize,
			   u32 kflags, u8 *keybuf, size_t *keybufsize)
{
	int i, card, dom, rc;

	/* check for at least one apqn given */
	if (!apqns || !nr_apqns)
		return -EINVAL;

	/* check key type and size */
	switch (ktype) {
	case PKEY_TYPE_CCA_DATA:
	case PKEY_TYPE_CCA_CIPHER:
		if (*keybufsize < SECKEYBLOBSIZE)
			return -EINVAL;
		break;
	case PKEY_TYPE_EP11:
		if (*keybufsize < MINEP11AESKEYBLOBSIZE)
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}
	switch (ksize) {
	case PKEY_SIZE_AES_128:
	case PKEY_SIZE_AES_192:
	case PKEY_SIZE_AES_256:
		break;
	default:
		return -EINVAL;
	}

	/* simple try all apqns from the list */
	for (i = 0, rc = -ENODEV; i < nr_apqns; i++) {
		card = apqns[i].card;
		dom = apqns[i].domain;
		if (ktype == PKEY_TYPE_EP11) {
			rc = ep11_genaeskey(card, dom, ksize, kflags,
					    keybuf, keybufsize);
		} else if (ktype == PKEY_TYPE_CCA_DATA) {
			rc = cca_genseckey(card, dom, ksize, keybuf);
			*keybufsize = (rc ? 0 : SECKEYBLOBSIZE);
		} else /* TOKVER_CCA_VLSC */
			rc = cca_gencipherkey(card, dom, ksize, kflags,
					      keybuf, keybufsize);
		if (rc == 0)
			break;
	}

	return rc;
}

static int pkey_clr2seckey2(const struct pkey_apqn *apqns, size_t nr_apqns,
			    enum pkey_key_type ktype, enum pkey_key_size ksize,
			    u32 kflags, const u8 *clrkey,
			    u8 *keybuf, size_t *keybufsize)
{
	int i, card, dom, rc;

	/* check for at least one apqn given */
	if (!apqns || !nr_apqns)
		return -EINVAL;

	/* check key type and size */
	switch (ktype) {
	case PKEY_TYPE_CCA_DATA:
	case PKEY_TYPE_CCA_CIPHER:
		if (*keybufsize < SECKEYBLOBSIZE)
			return -EINVAL;
		break;
	case PKEY_TYPE_EP11:
		if (*keybufsize < MINEP11AESKEYBLOBSIZE)
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}
	switch (ksize) {
	case PKEY_SIZE_AES_128:
	case PKEY_SIZE_AES_192:
	case PKEY_SIZE_AES_256:
		break;
	default:
		return -EINVAL;
	}

	zcrypt_wait_api_operational();

	/* simple try all apqns from the list */
	for (i = 0, rc = -ENODEV; i < nr_apqns; i++) {
		card = apqns[i].card;
		dom = apqns[i].domain;
		if (ktype == PKEY_TYPE_EP11) {
			rc = ep11_clr2keyblob(card, dom, ksize, kflags,
					      clrkey, keybuf, keybufsize);
		} else if (ktype == PKEY_TYPE_CCA_DATA) {
			rc = cca_clr2seckey(card, dom, ksize,
					    clrkey, keybuf);
			*keybufsize = (rc ? 0 : SECKEYBLOBSIZE);
		} else /* TOKVER_CCA_VLSC */
			rc = cca_clr2cipherkey(card, dom, ksize, kflags,
					       clrkey, keybuf, keybufsize);
		if (rc == 0)
			break;
	}

	return rc;
}

static int pkey_verifykey2(const u8 *key, size_t keylen,
			   u16 *cardnr, u16 *domain,
			   enum pkey_key_type *ktype,
			   enum pkey_key_size *ksize, u32 *flags)
{
	int rc;
	u32 _nr_apqns, *_apqns = NULL;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	if (keylen < sizeof(struct keytoken_header))
		return -EINVAL;

	if (hdr->type == TOKTYPE_CCA_INTERNAL
	    && hdr->version == TOKVER_CCA_AES) {
		struct secaeskeytoken *t = (struct secaeskeytoken *)key;

		rc = cca_check_secaeskeytoken(debug_info, 3, key, 0);
		if (rc)
			goto out;
		if (ktype)
			*ktype = PKEY_TYPE_CCA_DATA;
		if (ksize)
			*ksize = (enum pkey_key_size) t->bitsize;

		rc = cca_findcard2(&_apqns, &_nr_apqns, *cardnr, *domain,
				   ZCRYPT_CEX3C, AES_MK_SET, t->mkvp, 0, 1);
		if (rc == 0 && flags)
			*flags = PKEY_FLAGS_MATCH_CUR_MKVP;
		if (rc == -ENODEV) {
			rc = cca_findcard2(&_apqns, &_nr_apqns,
					   *cardnr, *domain,
					   ZCRYPT_CEX3C, AES_MK_SET,
					   0, t->mkvp, 1);
			if (rc == 0 && flags)
				*flags = PKEY_FLAGS_MATCH_ALT_MKVP;
		}
		if (rc)
			goto out;

		*cardnr = ((struct pkey_apqn *)_apqns)->card;
		*domain = ((struct pkey_apqn *)_apqns)->domain;

	} else if (hdr->type == TOKTYPE_CCA_INTERNAL
		   && hdr->version == TOKVER_CCA_VLSC) {
		struct cipherkeytoken *t = (struct cipherkeytoken *)key;

		rc = cca_check_secaescipherkey(debug_info, 3, key, 0, 1);
		if (rc)
			goto out;
		if (ktype)
			*ktype = PKEY_TYPE_CCA_CIPHER;
		if (ksize) {
			*ksize = PKEY_SIZE_UNKNOWN;
			if (!t->plfver && t->wpllen == 512)
				*ksize = PKEY_SIZE_AES_128;
			else if (!t->plfver && t->wpllen == 576)
				*ksize = PKEY_SIZE_AES_192;
			else if (!t->plfver && t->wpllen == 640)
				*ksize = PKEY_SIZE_AES_256;
		}

		rc = cca_findcard2(&_apqns, &_nr_apqns, *cardnr, *domain,
				   ZCRYPT_CEX6, AES_MK_SET, t->mkvp0, 0, 1);
		if (rc == 0 && flags)
			*flags = PKEY_FLAGS_MATCH_CUR_MKVP;
		if (rc == -ENODEV) {
			rc = cca_findcard2(&_apqns, &_nr_apqns,
					   *cardnr, *domain,
					   ZCRYPT_CEX6, AES_MK_SET,
					   0, t->mkvp0, 1);
			if (rc == 0 && flags)
				*flags = PKEY_FLAGS_MATCH_ALT_MKVP;
		}
		if (rc)
			goto out;

		*cardnr = ((struct pkey_apqn *)_apqns)->card;
		*domain = ((struct pkey_apqn *)_apqns)->domain;

	} else if (hdr->type == TOKTYPE_NON_CCA
		   && hdr->version == TOKVER_EP11_AES) {
		struct ep11keyblob *kb = (struct ep11keyblob *)key;

		rc = ep11_check_aes_key(debug_info, 3, key, keylen, 1);
		if (rc)
			goto out;
		if (ktype)
			*ktype = PKEY_TYPE_EP11;
		if (ksize)
			*ksize = kb->head.keybitlen;

		rc = ep11_findcard2(&_apqns, &_nr_apqns, *cardnr, *domain,
				    ZCRYPT_CEX7, EP11_API_V, kb->wkvp);
		if (rc)
			goto out;

		if (flags)
			*flags = PKEY_FLAGS_MATCH_CUR_MKVP;

		*cardnr = ((struct pkey_apqn *)_apqns)->card;
		*domain = ((struct pkey_apqn *)_apqns)->domain;

	} else
		rc = -EINVAL;

out:
	kfree(_apqns);
	return rc;
}

static int pkey_keyblob2pkey2(const struct pkey_apqn *apqns, size_t nr_apqns,
			      const u8 *key, size_t keylen,
			      struct pkey_protkey *pkey)
{
	int i, card, dom, rc;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	/* check for at least one apqn given */
	if (!apqns || !nr_apqns)
		return -EINVAL;

	if (keylen < sizeof(struct keytoken_header))
		return -EINVAL;

	if (hdr->type == TOKTYPE_CCA_INTERNAL) {
		if (hdr->version == TOKVER_CCA_AES) {
			if (keylen != sizeof(struct secaeskeytoken))
				return -EINVAL;
			if (cca_check_secaeskeytoken(debug_info, 3, key, 0))
				return -EINVAL;
		} else if (hdr->version == TOKVER_CCA_VLSC) {
			if (keylen < hdr->len || keylen > MAXCCAVLSCTOKENSIZE)
				return -EINVAL;
			if (cca_check_secaescipherkey(debug_info, 3, key, 0, 1))
				return -EINVAL;
		} else {
			DEBUG_ERR("%s unknown CCA internal token version %d\n",
				  __func__, hdr->version);
			return -EINVAL;
		}
	} else if (hdr->type == TOKTYPE_NON_CCA) {
		if (hdr->version == TOKVER_EP11_AES) {
			if (keylen < sizeof(struct ep11keyblob))
				return -EINVAL;
			if (ep11_check_aes_key(debug_info, 3, key, keylen, 1))
				return -EINVAL;
		} else {
			return pkey_nonccatok2pkey(key, keylen, pkey);
		}
	} else {
		DEBUG_ERR("%s unknown/unsupported blob type %d\n",
			  __func__, hdr->type);
		return -EINVAL;
	}

	zcrypt_wait_api_operational();

	/* simple try all apqns from the list */
	for (i = 0, rc = -ENODEV; i < nr_apqns; i++) {
		card = apqns[i].card;
		dom = apqns[i].domain;
		if (hdr->type == TOKTYPE_CCA_INTERNAL
		    && hdr->version == TOKVER_CCA_AES)
			rc = cca_sec2protkey(card, dom, key, pkey->protkey,
					     &pkey->len, &pkey->type);
		else if (hdr->type == TOKTYPE_CCA_INTERNAL
			 && hdr->version == TOKVER_CCA_VLSC)
			rc = cca_cipher2protkey(card, dom, key, pkey->protkey,
						&pkey->len, &pkey->type);
		else { /* EP11 AES secure key blob */
			struct ep11keyblob *kb = (struct ep11keyblob *) key;

			pkey->len = sizeof(pkey->protkey);
			rc = ep11_kblob2protkey(card, dom, key, kb->head.len,
						pkey->protkey, &pkey->len,
						&pkey->type);
		}
		if (rc == 0)
			break;
	}

	return rc;
}

static int pkey_apqns4key(const u8 *key, size_t keylen, u32 flags,
			  struct pkey_apqn *apqns, size_t *nr_apqns)
{
	int rc;
	u32 _nr_apqns, *_apqns = NULL;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	if (keylen < sizeof(struct keytoken_header) || flags == 0)
		return -EINVAL;

	zcrypt_wait_api_operational();

	if (hdr->type == TOKTYPE_NON_CCA
	    && (hdr->version == TOKVER_EP11_AES_WITH_HEADER
		|| hdr->version == TOKVER_EP11_ECC_WITH_HEADER)
	    && is_ep11_keyblob(key + sizeof(struct ep11kblob_header))) {
		int minhwtype = 0, api = 0;
		struct ep11keyblob *kb = (struct ep11keyblob *)
			(key + sizeof(struct ep11kblob_header));

		if (flags != PKEY_FLAGS_MATCH_CUR_MKVP)
			return -EINVAL;
		if (kb->attr & EP11_BLOB_PKEY_EXTRACTABLE) {
			minhwtype = ZCRYPT_CEX7;
			api = EP11_API_V;
		}
		rc = ep11_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				    minhwtype, api, kb->wkvp);
		if (rc)
			goto out;
	} else if (hdr->type == TOKTYPE_NON_CCA
		   && hdr->version == TOKVER_EP11_AES
		   && is_ep11_keyblob(key)) {
		int minhwtype = 0, api = 0;
		struct ep11keyblob *kb = (struct ep11keyblob *) key;

		if (flags != PKEY_FLAGS_MATCH_CUR_MKVP)
			return -EINVAL;
		if (kb->attr & EP11_BLOB_PKEY_EXTRACTABLE) {
			minhwtype = ZCRYPT_CEX7;
			api = EP11_API_V;
		}
		rc = ep11_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				    minhwtype, api, kb->wkvp);
		if (rc)
			goto out;
	} else if (hdr->type == TOKTYPE_CCA_INTERNAL) {
		int minhwtype = ZCRYPT_CEX3C;
		u64 cur_mkvp = 0, old_mkvp = 0;

		if (hdr->version == TOKVER_CCA_AES) {
			struct secaeskeytoken *t = (struct secaeskeytoken *)key;

			if (flags & PKEY_FLAGS_MATCH_CUR_MKVP)
				cur_mkvp = t->mkvp;
			if (flags & PKEY_FLAGS_MATCH_ALT_MKVP)
				old_mkvp = t->mkvp;
		} else if (hdr->version == TOKVER_CCA_VLSC) {
			struct cipherkeytoken *t = (struct cipherkeytoken *)key;

			minhwtype = ZCRYPT_CEX6;
			if (flags & PKEY_FLAGS_MATCH_CUR_MKVP)
				cur_mkvp = t->mkvp0;
			if (flags & PKEY_FLAGS_MATCH_ALT_MKVP)
				old_mkvp = t->mkvp0;
		} else {
			/* unknown cca internal token type */
			return -EINVAL;
		}
		rc = cca_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				   minhwtype, AES_MK_SET,
				   cur_mkvp, old_mkvp, 1);
		if (rc)
			goto out;
	} else if (hdr->type == TOKTYPE_CCA_INTERNAL_PKA) {
		u64 cur_mkvp = 0, old_mkvp = 0;
		struct eccprivkeytoken *t = (struct eccprivkeytoken *)key;

		if (t->secid == 0x20) {
			if (flags & PKEY_FLAGS_MATCH_CUR_MKVP)
				cur_mkvp = t->mkvp;
			if (flags & PKEY_FLAGS_MATCH_ALT_MKVP)
				old_mkvp = t->mkvp;
		} else {
			/* unknown cca internal 2 token type */
			return -EINVAL;
		}
		rc = cca_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				   ZCRYPT_CEX7, APKA_MK_SET,
				   cur_mkvp, old_mkvp, 1);
		if (rc)
			goto out;
	} else
		return -EINVAL;

	if (apqns) {
		if (*nr_apqns < _nr_apqns)
			rc = -ENOSPC;
		else
			memcpy(apqns, _apqns, _nr_apqns * sizeof(u32));
	}
	*nr_apqns = _nr_apqns;

out:
	kfree(_apqns);
	return rc;
}

static int pkey_apqns4keytype(enum pkey_key_type ktype,
			      u8 cur_mkvp[32], u8 alt_mkvp[32], u32 flags,
			      struct pkey_apqn *apqns, size_t *nr_apqns)
{
	int rc;
	u32 _nr_apqns, *_apqns = NULL;

	zcrypt_wait_api_operational();

	if (ktype == PKEY_TYPE_CCA_DATA || ktype == PKEY_TYPE_CCA_CIPHER) {
		u64 cur_mkvp = 0, old_mkvp = 0;
		int minhwtype = ZCRYPT_CEX3C;

		if (flags & PKEY_FLAGS_MATCH_CUR_MKVP)
			cur_mkvp = *((u64 *) cur_mkvp);
		if (flags & PKEY_FLAGS_MATCH_ALT_MKVP)
			old_mkvp = *((u64 *) alt_mkvp);
		if (ktype == PKEY_TYPE_CCA_CIPHER)
			minhwtype = ZCRYPT_CEX6;
		rc = cca_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				   minhwtype, AES_MK_SET,
				   cur_mkvp, old_mkvp, 1);
		if (rc)
			goto out;
	} else if (ktype == PKEY_TYPE_CCA_ECC) {
		u64 cur_mkvp = 0, old_mkvp = 0;

		if (flags & PKEY_FLAGS_MATCH_CUR_MKVP)
			cur_mkvp = *((u64 *) cur_mkvp);
		if (flags & PKEY_FLAGS_MATCH_ALT_MKVP)
			old_mkvp = *((u64 *) alt_mkvp);
		rc = cca_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				   ZCRYPT_CEX7, APKA_MK_SET,
				   cur_mkvp, old_mkvp, 1);
		if (rc)
			goto out;

	} else if (ktype == PKEY_TYPE_EP11 ||
		   ktype == PKEY_TYPE_EP11_AES ||
		   ktype == PKEY_TYPE_EP11_ECC) {
		u8 *wkvp = NULL;

		if (flags & PKEY_FLAGS_MATCH_CUR_MKVP)
			wkvp = cur_mkvp;
		rc = ep11_findcard2(&_apqns, &_nr_apqns, 0xFFFF, 0xFFFF,
				    ZCRYPT_CEX7, EP11_API_V, wkvp);
		if (rc)
			goto out;

	} else
		return -EINVAL;

	if (apqns) {
		if (*nr_apqns < _nr_apqns)
			rc = -ENOSPC;
		else
			memcpy(apqns, _apqns, _nr_apqns * sizeof(u32));
	}
	*nr_apqns = _nr_apqns;

out:
	kfree(_apqns);
	return rc;
}

static int pkey_keyblob2pkey3(const struct pkey_apqn *apqns, size_t nr_apqns,
			      const u8 *key, size_t keylen, u32 *protkeytype,
			      u8 *protkey, u32 *protkeylen)
{
	int i, card, dom, rc;
	struct keytoken_header *hdr = (struct keytoken_header *)key;

	/* check for at least one apqn given */
	if (!apqns || !nr_apqns)
		return -EINVAL;

	if (keylen < sizeof(struct keytoken_header))
		return -EINVAL;

	if (hdr->type == TOKTYPE_NON_CCA
	    && hdr->version == TOKVER_EP11_AES_WITH_HEADER
	    && is_ep11_keyblob(key + sizeof(struct ep11kblob_header))) {
		/* EP11 AES key blob with header */
		if (ep11_check_aes_key_with_hdr(debug_info, 3, key, keylen, 1))
			return -EINVAL;
	} else if (hdr->type == TOKTYPE_NON_CCA
		   && hdr->version == TOKVER_EP11_ECC_WITH_HEADER
		   && is_ep11_keyblob(key + sizeof(struct ep11kblob_header))) {
		/* EP11 ECC key blob with header */
		if (ep11_check_ecc_key_with_hdr(debug_info, 3, key, keylen, 1))
			return -EINVAL;
	} else if (hdr->type == TOKTYPE_NON_CCA
		   && hdr->version == TOKVER_EP11_AES
		   && is_ep11_keyblob(key)) {
		/* EP11 AES key blob with header in session field */
		if (ep11_check_aes_key(debug_info, 3, key, keylen, 1))
			return -EINVAL;
	} else	if (hdr->type == TOKTYPE_CCA_INTERNAL) {
		if (hdr->version == TOKVER_CCA_AES) {
			/* CCA AES data key */
			if (keylen != sizeof(struct secaeskeytoken))
				return -EINVAL;
			if (cca_check_secaeskeytoken(debug_info, 3, key, 0))
				return -EINVAL;
		} else if (hdr->version == TOKVER_CCA_VLSC) {
			/* CCA AES cipher key */
			if (keylen < hdr->len || keylen > MAXCCAVLSCTOKENSIZE)
				return -EINVAL;
			if (cca_check_secaescipherkey(debug_info, 3, key, 0, 1))
				return -EINVAL;
		} else {
			DEBUG_ERR("%s unknown CCA internal token version %d\n",
				  __func__, hdr->version);
			return -EINVAL;
		}
	} else if (hdr->type == TOKTYPE_CCA_INTERNAL_PKA) {
		/* CCA ECC (private) key */
		if (keylen < sizeof(struct eccprivkeytoken))
			return -EINVAL;
		if (cca_check_sececckeytoken(debug_info, 3, key, keylen, 1))
			return -EINVAL;
	} else if (hdr->type == TOKTYPE_NON_CCA) {
		struct pkey_protkey pkey;

		rc = pkey_nonccatok2pkey(key, keylen, &pkey);
		if (rc)
			return rc;
		memcpy(protkey, pkey.protkey, pkey.len);
		*protkeylen = pkey.len;
		*protkeytype = pkey.type;
		return 0;
	} else {
		DEBUG_ERR("%s unknown/unsupported blob type %d\n",
			  __func__, hdr->type);
		return -EINVAL;
	}

	/* simple try all apqns from the list */
	for (rc = -ENODEV, i = 0; rc && i < nr_apqns; i++) {
		card = apqns[i].card;
		dom = apqns[i].domain;
		if (hdr->type == TOKTYPE_NON_CCA
		    && (hdr->version == TOKVER_EP11_AES_WITH_HEADER
			|| hdr->version == TOKVER_EP11_ECC_WITH_HEADER)
		    && is_ep11_keyblob(key + sizeof(struct ep11kblob_header)))
			rc = ep11_kblob2protkey(card, dom, key, hdr->len,
						protkey, protkeylen, protkeytype);
		else if (hdr->type == TOKTYPE_NON_CCA
			 && hdr->version == TOKVER_EP11_AES
			 && is_ep11_keyblob(key))
			rc = ep11_kblob2protkey(card, dom, key, hdr->len,
						protkey, protkeylen, protkeytype);
		else if (hdr->type == TOKTYPE_CCA_INTERNAL &&
			 hdr->version == TOKVER_CCA_AES)
			rc = cca_sec2protkey(card, dom, key, protkey,
					     protkeylen, protkeytype);
		else if (hdr->type == TOKTYPE_CCA_INTERNAL &&
			 hdr->version == TOKVER_CCA_VLSC)
			rc = cca_cipher2protkey(card, dom, key, protkey,
						protkeylen, protkeytype);
		else if (hdr->type == TOKTYPE_CCA_INTERNAL_PKA)
			rc = cca_ecc2protkey(card, dom, key, protkey,
					     protkeylen, protkeytype);
		else
			return -EINVAL;
	}

	return rc;
}
>>>>>>> upstream/android-13

/*
 * File io functions
 */

<<<<<<< HEAD
=======
static void *_copy_key_from_user(void __user *ukey, size_t keylen)
{
	if (!ukey || keylen < MINKEYBLOBSIZE || keylen > KEYBLOBBUFSIZE)
		return ERR_PTR(-EINVAL);

	return memdup_user(ukey, keylen);
}

static void *_copy_apqns_from_user(void __user *uapqns, size_t nr_apqns)
{
	if (!uapqns || nr_apqns == 0)
		return NULL;

	return memdup_user(uapqns, nr_apqns * sizeof(struct pkey_apqn));
}

>>>>>>> upstream/android-13
static long pkey_unlocked_ioctl(struct file *filp, unsigned int cmd,
				unsigned long arg)
{
	int rc;

	switch (cmd) {
	case PKEY_GENSECK: {
		struct pkey_genseck __user *ugs = (void __user *) arg;
		struct pkey_genseck kgs;

		if (copy_from_user(&kgs, ugs, sizeof(kgs)))
			return -EFAULT;
<<<<<<< HEAD
		rc = pkey_genseckey(kgs.cardnr, kgs.domain,
				    kgs.keytype, &kgs.seckey);
		DEBUG_DBG("%s pkey_genseckey()=%d\n", __func__, rc);
=======
		rc = cca_genseckey(kgs.cardnr, kgs.domain,
				   kgs.keytype, kgs.seckey.seckey);
		DEBUG_DBG("%s cca_genseckey()=%d\n", __func__, rc);
>>>>>>> upstream/android-13
		if (rc)
			break;
		if (copy_to_user(ugs, &kgs, sizeof(kgs)))
			return -EFAULT;
		break;
	}
	case PKEY_CLR2SECK: {
		struct pkey_clr2seck __user *ucs = (void __user *) arg;
		struct pkey_clr2seck kcs;

		if (copy_from_user(&kcs, ucs, sizeof(kcs)))
			return -EFAULT;
<<<<<<< HEAD
		rc = pkey_clr2seckey(kcs.cardnr, kcs.domain, kcs.keytype,
				     &kcs.clrkey, &kcs.seckey);
		DEBUG_DBG("%s pkey_clr2seckey()=%d\n", __func__, rc);
=======
		rc = cca_clr2seckey(kcs.cardnr, kcs.domain, kcs.keytype,
				    kcs.clrkey.clrkey, kcs.seckey.seckey);
		DEBUG_DBG("%s cca_clr2seckey()=%d\n", __func__, rc);
>>>>>>> upstream/android-13
		if (rc)
			break;
		if (copy_to_user(ucs, &kcs, sizeof(kcs)))
			return -EFAULT;
		memzero_explicit(&kcs, sizeof(kcs));
		break;
	}
	case PKEY_SEC2PROTK: {
		struct pkey_sec2protk __user *usp = (void __user *) arg;
		struct pkey_sec2protk ksp;

		if (copy_from_user(&ksp, usp, sizeof(ksp)))
			return -EFAULT;
<<<<<<< HEAD
		rc = pkey_sec2protkey(ksp.cardnr, ksp.domain,
				      &ksp.seckey, &ksp.protkey);
		DEBUG_DBG("%s pkey_sec2protkey()=%d\n", __func__, rc);
=======
		rc = cca_sec2protkey(ksp.cardnr, ksp.domain,
				     ksp.seckey.seckey, ksp.protkey.protkey,
				     &ksp.protkey.len, &ksp.protkey.type);
		DEBUG_DBG("%s cca_sec2protkey()=%d\n", __func__, rc);
>>>>>>> upstream/android-13
		if (rc)
			break;
		if (copy_to_user(usp, &ksp, sizeof(ksp)))
			return -EFAULT;
		break;
	}
	case PKEY_CLR2PROTK: {
		struct pkey_clr2protk __user *ucp = (void __user *) arg;
		struct pkey_clr2protk kcp;

		if (copy_from_user(&kcp, ucp, sizeof(kcp)))
			return -EFAULT;
		rc = pkey_clr2protkey(kcp.keytype,
				      &kcp.clrkey, &kcp.protkey);
		DEBUG_DBG("%s pkey_clr2protkey()=%d\n", __func__, rc);
		if (rc)
			break;
		if (copy_to_user(ucp, &kcp, sizeof(kcp)))
			return -EFAULT;
		memzero_explicit(&kcp, sizeof(kcp));
		break;
	}
	case PKEY_FINDCARD: {
		struct pkey_findcard __user *ufc = (void __user *) arg;
		struct pkey_findcard kfc;

		if (copy_from_user(&kfc, ufc, sizeof(kfc)))
			return -EFAULT;
<<<<<<< HEAD
		rc = pkey_findcard(&kfc.seckey,
				   &kfc.cardnr, &kfc.domain, 1);
		DEBUG_DBG("%s pkey_findcard()=%d\n", __func__, rc);
		if (rc)
=======
		rc = cca_findcard(kfc.seckey.seckey,
				  &kfc.cardnr, &kfc.domain, 1);
		DEBUG_DBG("%s cca_findcard()=%d\n", __func__, rc);
		if (rc < 0)
>>>>>>> upstream/android-13
			break;
		if (copy_to_user(ufc, &kfc, sizeof(kfc)))
			return -EFAULT;
		break;
	}
	case PKEY_SKEY2PKEY: {
		struct pkey_skey2pkey __user *usp = (void __user *) arg;
		struct pkey_skey2pkey ksp;

		if (copy_from_user(&ksp, usp, sizeof(ksp)))
			return -EFAULT;
<<<<<<< HEAD
		rc = pkey_skey2pkey(&ksp.seckey, &ksp.protkey);
=======
		rc = pkey_skey2pkey(ksp.seckey.seckey, &ksp.protkey);
>>>>>>> upstream/android-13
		DEBUG_DBG("%s pkey_skey2pkey()=%d\n", __func__, rc);
		if (rc)
			break;
		if (copy_to_user(usp, &ksp, sizeof(ksp)))
			return -EFAULT;
		break;
	}
	case PKEY_VERIFYKEY: {
		struct pkey_verifykey __user *uvk = (void __user *) arg;
		struct pkey_verifykey kvk;

		if (copy_from_user(&kvk, uvk, sizeof(kvk)))
			return -EFAULT;
		rc = pkey_verifykey(&kvk.seckey, &kvk.cardnr, &kvk.domain,
				    &kvk.keysize, &kvk.attributes);
		DEBUG_DBG("%s pkey_verifykey()=%d\n", __func__, rc);
		if (rc)
			break;
		if (copy_to_user(uvk, &kvk, sizeof(kvk)))
			return -EFAULT;
		break;
	}
<<<<<<< HEAD
=======
	case PKEY_GENPROTK: {
		struct pkey_genprotk __user *ugp = (void __user *) arg;
		struct pkey_genprotk kgp;

		if (copy_from_user(&kgp, ugp, sizeof(kgp)))
			return -EFAULT;
		rc = pkey_genprotkey(kgp.keytype, &kgp.protkey);
		DEBUG_DBG("%s pkey_genprotkey()=%d\n", __func__, rc);
		if (rc)
			break;
		if (copy_to_user(ugp, &kgp, sizeof(kgp)))
			return -EFAULT;
		break;
	}
	case PKEY_VERIFYPROTK: {
		struct pkey_verifyprotk __user *uvp = (void __user *) arg;
		struct pkey_verifyprotk kvp;

		if (copy_from_user(&kvp, uvp, sizeof(kvp)))
			return -EFAULT;
		rc = pkey_verifyprotkey(&kvp.protkey);
		DEBUG_DBG("%s pkey_verifyprotkey()=%d\n", __func__, rc);
		break;
	}
	case PKEY_KBLOB2PROTK: {
		struct pkey_kblob2pkey __user *utp = (void __user *) arg;
		struct pkey_kblob2pkey ktp;
		u8 *kkey;

		if (copy_from_user(&ktp, utp, sizeof(ktp)))
			return -EFAULT;
		kkey = _copy_key_from_user(ktp.key, ktp.keylen);
		if (IS_ERR(kkey))
			return PTR_ERR(kkey);
		rc = pkey_keyblob2pkey(kkey, ktp.keylen, &ktp.protkey);
		DEBUG_DBG("%s pkey_keyblob2pkey()=%d\n", __func__, rc);
		kfree(kkey);
		if (rc)
			break;
		if (copy_to_user(utp, &ktp, sizeof(ktp)))
			return -EFAULT;
		break;
	}
	case PKEY_GENSECK2: {
		struct pkey_genseck2 __user *ugs = (void __user *) arg;
		struct pkey_genseck2 kgs;
		struct pkey_apqn *apqns;
		size_t klen = KEYBLOBBUFSIZE;
		u8 *kkey;

		if (copy_from_user(&kgs, ugs, sizeof(kgs)))
			return -EFAULT;
		apqns = _copy_apqns_from_user(kgs.apqns, kgs.apqn_entries);
		if (IS_ERR(apqns))
			return PTR_ERR(apqns);
		kkey = kmalloc(klen, GFP_KERNEL);
		if (!kkey) {
			kfree(apqns);
			return -ENOMEM;
		}
		rc = pkey_genseckey2(apqns, kgs.apqn_entries,
				     kgs.type, kgs.size, kgs.keygenflags,
				     kkey, &klen);
		DEBUG_DBG("%s pkey_genseckey2()=%d\n", __func__, rc);
		kfree(apqns);
		if (rc) {
			kfree(kkey);
			break;
		}
		if (kgs.key) {
			if (kgs.keylen < klen) {
				kfree(kkey);
				return -EINVAL;
			}
			if (copy_to_user(kgs.key, kkey, klen)) {
				kfree(kkey);
				return -EFAULT;
			}
		}
		kgs.keylen = klen;
		if (copy_to_user(ugs, &kgs, sizeof(kgs)))
			rc = -EFAULT;
		kfree(kkey);
		break;
	}
	case PKEY_CLR2SECK2: {
		struct pkey_clr2seck2 __user *ucs = (void __user *) arg;
		struct pkey_clr2seck2 kcs;
		struct pkey_apqn *apqns;
		size_t klen = KEYBLOBBUFSIZE;
		u8 *kkey;

		if (copy_from_user(&kcs, ucs, sizeof(kcs)))
			return -EFAULT;
		apqns = _copy_apqns_from_user(kcs.apqns, kcs.apqn_entries);
		if (IS_ERR(apqns))
			return PTR_ERR(apqns);
		kkey = kmalloc(klen, GFP_KERNEL);
		if (!kkey) {
			kfree(apqns);
			return -ENOMEM;
		}
		rc = pkey_clr2seckey2(apqns, kcs.apqn_entries,
				      kcs.type, kcs.size, kcs.keygenflags,
				      kcs.clrkey.clrkey, kkey, &klen);
		DEBUG_DBG("%s pkey_clr2seckey2()=%d\n", __func__, rc);
		kfree(apqns);
		if (rc) {
			kfree(kkey);
			break;
		}
		if (kcs.key) {
			if (kcs.keylen < klen) {
				kfree(kkey);
				return -EINVAL;
			}
			if (copy_to_user(kcs.key, kkey, klen)) {
				kfree(kkey);
				return -EFAULT;
			}
		}
		kcs.keylen = klen;
		if (copy_to_user(ucs, &kcs, sizeof(kcs)))
			rc = -EFAULT;
		memzero_explicit(&kcs, sizeof(kcs));
		kfree(kkey);
		break;
	}
	case PKEY_VERIFYKEY2: {
		struct pkey_verifykey2 __user *uvk = (void __user *) arg;
		struct pkey_verifykey2 kvk;
		u8 *kkey;

		if (copy_from_user(&kvk, uvk, sizeof(kvk)))
			return -EFAULT;
		kkey = _copy_key_from_user(kvk.key, kvk.keylen);
		if (IS_ERR(kkey))
			return PTR_ERR(kkey);
		rc = pkey_verifykey2(kkey, kvk.keylen,
				     &kvk.cardnr, &kvk.domain,
				     &kvk.type, &kvk.size, &kvk.flags);
		DEBUG_DBG("%s pkey_verifykey2()=%d\n", __func__, rc);
		kfree(kkey);
		if (rc)
			break;
		if (copy_to_user(uvk, &kvk, sizeof(kvk)))
			return -EFAULT;
		break;
	}
	case PKEY_KBLOB2PROTK2: {
		struct pkey_kblob2pkey2 __user *utp = (void __user *) arg;
		struct pkey_kblob2pkey2 ktp;
		struct pkey_apqn *apqns = NULL;
		u8 *kkey;

		if (copy_from_user(&ktp, utp, sizeof(ktp)))
			return -EFAULT;
		apqns = _copy_apqns_from_user(ktp.apqns, ktp.apqn_entries);
		if (IS_ERR(apqns))
			return PTR_ERR(apqns);
		kkey = _copy_key_from_user(ktp.key, ktp.keylen);
		if (IS_ERR(kkey)) {
			kfree(apqns);
			return PTR_ERR(kkey);
		}
		rc = pkey_keyblob2pkey2(apqns, ktp.apqn_entries,
					kkey, ktp.keylen, &ktp.protkey);
		DEBUG_DBG("%s pkey_keyblob2pkey2()=%d\n", __func__, rc);
		kfree(apqns);
		kfree(kkey);
		if (rc)
			break;
		if (copy_to_user(utp, &ktp, sizeof(ktp)))
			return -EFAULT;
		break;
	}
	case PKEY_APQNS4K: {
		struct pkey_apqns4key __user *uak = (void __user *) arg;
		struct pkey_apqns4key kak;
		struct pkey_apqn *apqns = NULL;
		size_t nr_apqns, len;
		u8 *kkey;

		if (copy_from_user(&kak, uak, sizeof(kak)))
			return -EFAULT;
		nr_apqns = kak.apqn_entries;
		if (nr_apqns) {
			apqns = kmalloc_array(nr_apqns,
					      sizeof(struct pkey_apqn),
					      GFP_KERNEL);
			if (!apqns)
				return -ENOMEM;
		}
		kkey = _copy_key_from_user(kak.key, kak.keylen);
		if (IS_ERR(kkey)) {
			kfree(apqns);
			return PTR_ERR(kkey);
		}
		rc = pkey_apqns4key(kkey, kak.keylen, kak.flags,
				    apqns, &nr_apqns);
		DEBUG_DBG("%s pkey_apqns4key()=%d\n", __func__, rc);
		kfree(kkey);
		if (rc && rc != -ENOSPC) {
			kfree(apqns);
			break;
		}
		if (!rc && kak.apqns) {
			if (nr_apqns > kak.apqn_entries) {
				kfree(apqns);
				return -EINVAL;
			}
			len = nr_apqns * sizeof(struct pkey_apqn);
			if (len) {
				if (copy_to_user(kak.apqns, apqns, len)) {
					kfree(apqns);
					return -EFAULT;
				}
			}
		}
		kak.apqn_entries = nr_apqns;
		if (copy_to_user(uak, &kak, sizeof(kak)))
			rc = -EFAULT;
		kfree(apqns);
		break;
	}
	case PKEY_APQNS4KT: {
		struct pkey_apqns4keytype __user *uat = (void __user *) arg;
		struct pkey_apqns4keytype kat;
		struct pkey_apqn *apqns = NULL;
		size_t nr_apqns, len;

		if (copy_from_user(&kat, uat, sizeof(kat)))
			return -EFAULT;
		nr_apqns = kat.apqn_entries;
		if (nr_apqns) {
			apqns = kmalloc_array(nr_apqns,
					      sizeof(struct pkey_apqn),
					      GFP_KERNEL);
			if (!apqns)
				return -ENOMEM;
		}
		rc = pkey_apqns4keytype(kat.type, kat.cur_mkvp, kat.alt_mkvp,
					kat.flags, apqns, &nr_apqns);
		DEBUG_DBG("%s pkey_apqns4keytype()=%d\n", __func__, rc);
		if (rc && rc != -ENOSPC) {
			kfree(apqns);
			break;
		}
		if (!rc && kat.apqns) {
			if (nr_apqns > kat.apqn_entries) {
				kfree(apqns);
				return -EINVAL;
			}
			len = nr_apqns * sizeof(struct pkey_apqn);
			if (len) {
				if (copy_to_user(kat.apqns, apqns, len)) {
					kfree(apqns);
					return -EFAULT;
				}
			}
		}
		kat.apqn_entries = nr_apqns;
		if (copy_to_user(uat, &kat, sizeof(kat)))
			rc = -EFAULT;
		kfree(apqns);
		break;
	}
	case PKEY_KBLOB2PROTK3: {
		struct pkey_kblob2pkey3 __user *utp = (void __user *) arg;
		struct pkey_kblob2pkey3 ktp;
		struct pkey_apqn *apqns = NULL;
		u32 protkeylen = PROTKEYBLOBBUFSIZE;
		u8 *kkey, *protkey;

		if (copy_from_user(&ktp, utp, sizeof(ktp)))
			return -EFAULT;
		apqns = _copy_apqns_from_user(ktp.apqns, ktp.apqn_entries);
		if (IS_ERR(apqns))
			return PTR_ERR(apqns);
		kkey = _copy_key_from_user(ktp.key, ktp.keylen);
		if (IS_ERR(kkey)) {
			kfree(apqns);
			return PTR_ERR(kkey);
		}
		protkey = kmalloc(protkeylen, GFP_KERNEL);
		if (!protkey) {
			kfree(apqns);
			kfree(kkey);
			return -ENOMEM;
		}
		rc = pkey_keyblob2pkey3(apqns, ktp.apqn_entries, kkey,
					ktp.keylen, &ktp.pkeytype,
					protkey, &protkeylen);
		DEBUG_DBG("%s pkey_keyblob2pkey3()=%d\n", __func__, rc);
		kfree(apqns);
		kfree(kkey);
		if (rc) {
			kfree(protkey);
			break;
		}
		if (ktp.pkey && ktp.pkeylen) {
			if (protkeylen > ktp.pkeylen) {
				kfree(protkey);
				return -EINVAL;
			}
			if (copy_to_user(ktp.pkey, protkey, protkeylen)) {
				kfree(protkey);
				return -EFAULT;
			}
		}
		kfree(protkey);
		ktp.pkeylen = protkeylen;
		if (copy_to_user(utp, &ktp, sizeof(ktp)))
			return -EFAULT;
		break;
	}
>>>>>>> upstream/android-13
	default:
		/* unknown/unsupported ioctl cmd */
		return -ENOTTY;
	}

	return rc;
}

/*
 * Sysfs and file io operations
 */
<<<<<<< HEAD
=======

/*
 * Sysfs attribute read function for all protected key binary attributes.
 * The implementation can not deal with partial reads, because a new random
 * protected key blob is generated with each read. In case of partial reads
 * (i.e. off != 0 or count < key blob size) -EINVAL is returned.
 */
static ssize_t pkey_protkey_aes_attr_read(u32 keytype, bool is_xts, char *buf,
					  loff_t off, size_t count)
{
	struct protaeskeytoken protkeytoken;
	struct pkey_protkey protkey;
	int rc;

	if (off != 0 || count < sizeof(protkeytoken))
		return -EINVAL;
	if (is_xts)
		if (count < 2 * sizeof(protkeytoken))
			return -EINVAL;

	memset(&protkeytoken, 0, sizeof(protkeytoken));
	protkeytoken.type = TOKTYPE_NON_CCA;
	protkeytoken.version = TOKVER_PROTECTED_KEY;
	protkeytoken.keytype = keytype;

	rc = pkey_genprotkey(protkeytoken.keytype, &protkey);
	if (rc)
		return rc;

	protkeytoken.len = protkey.len;
	memcpy(&protkeytoken.protkey, &protkey.protkey, protkey.len);

	memcpy(buf, &protkeytoken, sizeof(protkeytoken));

	if (is_xts) {
		rc = pkey_genprotkey(protkeytoken.keytype, &protkey);
		if (rc)
			return rc;

		protkeytoken.len = protkey.len;
		memcpy(&protkeytoken.protkey, &protkey.protkey, protkey.len);

		memcpy(buf + sizeof(protkeytoken), &protkeytoken,
		       sizeof(protkeytoken));

		return 2 * sizeof(protkeytoken);
	}

	return sizeof(protkeytoken);
}

static ssize_t protkey_aes_128_read(struct file *filp,
				    struct kobject *kobj,
				    struct bin_attribute *attr,
				    char *buf, loff_t off,
				    size_t count)
{
	return pkey_protkey_aes_attr_read(PKEY_KEYTYPE_AES_128, false, buf,
					  off, count);
}

static ssize_t protkey_aes_192_read(struct file *filp,
				    struct kobject *kobj,
				    struct bin_attribute *attr,
				    char *buf, loff_t off,
				    size_t count)
{
	return pkey_protkey_aes_attr_read(PKEY_KEYTYPE_AES_192, false, buf,
					  off, count);
}

static ssize_t protkey_aes_256_read(struct file *filp,
				    struct kobject *kobj,
				    struct bin_attribute *attr,
				    char *buf, loff_t off,
				    size_t count)
{
	return pkey_protkey_aes_attr_read(PKEY_KEYTYPE_AES_256, false, buf,
					  off, count);
}

static ssize_t protkey_aes_128_xts_read(struct file *filp,
					struct kobject *kobj,
					struct bin_attribute *attr,
					char *buf, loff_t off,
					size_t count)
{
	return pkey_protkey_aes_attr_read(PKEY_KEYTYPE_AES_128, true, buf,
					  off, count);
}

static ssize_t protkey_aes_256_xts_read(struct file *filp,
					struct kobject *kobj,
					struct bin_attribute *attr,
					char *buf, loff_t off,
					size_t count)
{
	return pkey_protkey_aes_attr_read(PKEY_KEYTYPE_AES_256, true, buf,
					  off, count);
}

static BIN_ATTR_RO(protkey_aes_128, sizeof(struct protaeskeytoken));
static BIN_ATTR_RO(protkey_aes_192, sizeof(struct protaeskeytoken));
static BIN_ATTR_RO(protkey_aes_256, sizeof(struct protaeskeytoken));
static BIN_ATTR_RO(protkey_aes_128_xts, 2 * sizeof(struct protaeskeytoken));
static BIN_ATTR_RO(protkey_aes_256_xts, 2 * sizeof(struct protaeskeytoken));

static struct bin_attribute *protkey_attrs[] = {
	&bin_attr_protkey_aes_128,
	&bin_attr_protkey_aes_192,
	&bin_attr_protkey_aes_256,
	&bin_attr_protkey_aes_128_xts,
	&bin_attr_protkey_aes_256_xts,
	NULL
};

static struct attribute_group protkey_attr_group = {
	.name	   = "protkey",
	.bin_attrs = protkey_attrs,
};

/*
 * Sysfs attribute read function for all secure key ccadata binary attributes.
 * The implementation can not deal with partial reads, because a new random
 * protected key blob is generated with each read. In case of partial reads
 * (i.e. off != 0 or count < key blob size) -EINVAL is returned.
 */
static ssize_t pkey_ccadata_aes_attr_read(u32 keytype, bool is_xts, char *buf,
					  loff_t off, size_t count)
{
	int rc;
	struct pkey_seckey *seckey = (struct pkey_seckey *) buf;

	if (off != 0 || count < sizeof(struct secaeskeytoken))
		return -EINVAL;
	if (is_xts)
		if (count < 2 * sizeof(struct secaeskeytoken))
			return -EINVAL;

	rc = cca_genseckey(-1, -1, keytype, seckey->seckey);
	if (rc)
		return rc;

	if (is_xts) {
		seckey++;
		rc = cca_genseckey(-1, -1, keytype, seckey->seckey);
		if (rc)
			return rc;

		return 2 * sizeof(struct secaeskeytoken);
	}

	return sizeof(struct secaeskeytoken);
}

static ssize_t ccadata_aes_128_read(struct file *filp,
				    struct kobject *kobj,
				    struct bin_attribute *attr,
				    char *buf, loff_t off,
				    size_t count)
{
	return pkey_ccadata_aes_attr_read(PKEY_KEYTYPE_AES_128, false, buf,
					  off, count);
}

static ssize_t ccadata_aes_192_read(struct file *filp,
				    struct kobject *kobj,
				    struct bin_attribute *attr,
				    char *buf, loff_t off,
				    size_t count)
{
	return pkey_ccadata_aes_attr_read(PKEY_KEYTYPE_AES_192, false, buf,
					  off, count);
}

static ssize_t ccadata_aes_256_read(struct file *filp,
				    struct kobject *kobj,
				    struct bin_attribute *attr,
				    char *buf, loff_t off,
				    size_t count)
{
	return pkey_ccadata_aes_attr_read(PKEY_KEYTYPE_AES_256, false, buf,
					  off, count);
}

static ssize_t ccadata_aes_128_xts_read(struct file *filp,
					struct kobject *kobj,
					struct bin_attribute *attr,
					char *buf, loff_t off,
					size_t count)
{
	return pkey_ccadata_aes_attr_read(PKEY_KEYTYPE_AES_128, true, buf,
					  off, count);
}

static ssize_t ccadata_aes_256_xts_read(struct file *filp,
					struct kobject *kobj,
					struct bin_attribute *attr,
					char *buf, loff_t off,
					size_t count)
{
	return pkey_ccadata_aes_attr_read(PKEY_KEYTYPE_AES_256, true, buf,
					  off, count);
}

static BIN_ATTR_RO(ccadata_aes_128, sizeof(struct secaeskeytoken));
static BIN_ATTR_RO(ccadata_aes_192, sizeof(struct secaeskeytoken));
static BIN_ATTR_RO(ccadata_aes_256, sizeof(struct secaeskeytoken));
static BIN_ATTR_RO(ccadata_aes_128_xts, 2 * sizeof(struct secaeskeytoken));
static BIN_ATTR_RO(ccadata_aes_256_xts, 2 * sizeof(struct secaeskeytoken));

static struct bin_attribute *ccadata_attrs[] = {
	&bin_attr_ccadata_aes_128,
	&bin_attr_ccadata_aes_192,
	&bin_attr_ccadata_aes_256,
	&bin_attr_ccadata_aes_128_xts,
	&bin_attr_ccadata_aes_256_xts,
	NULL
};

static struct attribute_group ccadata_attr_group = {
	.name	   = "ccadata",
	.bin_attrs = ccadata_attrs,
};

#define CCACIPHERTOKENSIZE	(sizeof(struct cipherkeytoken) + 80)

/*
 * Sysfs attribute read function for all secure key ccacipher binary attributes.
 * The implementation can not deal with partial reads, because a new random
 * secure key blob is generated with each read. In case of partial reads
 * (i.e. off != 0 or count < key blob size) -EINVAL is returned.
 */
static ssize_t pkey_ccacipher_aes_attr_read(enum pkey_key_size keybits,
					    bool is_xts, char *buf, loff_t off,
					    size_t count)
{
	int i, rc, card, dom;
	u32 nr_apqns, *apqns = NULL;
	size_t keysize = CCACIPHERTOKENSIZE;

	if (off != 0 || count < CCACIPHERTOKENSIZE)
		return -EINVAL;
	if (is_xts)
		if (count < 2 * CCACIPHERTOKENSIZE)
			return -EINVAL;

	/* build a list of apqns able to generate an cipher key */
	rc = cca_findcard2(&apqns, &nr_apqns, 0xFFFF, 0xFFFF,
			   ZCRYPT_CEX6, 0, 0, 0, 0);
	if (rc)
		return rc;

	memset(buf, 0, is_xts ? 2 * keysize : keysize);

	/* simple try all apqns from the list */
	for (i = 0, rc = -ENODEV; i < nr_apqns; i++) {
		card = apqns[i] >> 16;
		dom = apqns[i] & 0xFFFF;
		rc = cca_gencipherkey(card, dom, keybits, 0, buf, &keysize);
		if (rc == 0)
			break;
	}
	if (rc)
		return rc;

	if (is_xts) {
		keysize = CCACIPHERTOKENSIZE;
		buf += CCACIPHERTOKENSIZE;
		rc = cca_gencipherkey(card, dom, keybits, 0, buf, &keysize);
		if (rc == 0)
			return 2 * CCACIPHERTOKENSIZE;
	}

	return CCACIPHERTOKENSIZE;
}

static ssize_t ccacipher_aes_128_read(struct file *filp,
				      struct kobject *kobj,
				      struct bin_attribute *attr,
				      char *buf, loff_t off,
				      size_t count)
{
	return pkey_ccacipher_aes_attr_read(PKEY_SIZE_AES_128, false, buf,
					    off, count);
}

static ssize_t ccacipher_aes_192_read(struct file *filp,
				      struct kobject *kobj,
				      struct bin_attribute *attr,
				      char *buf, loff_t off,
				      size_t count)
{
	return pkey_ccacipher_aes_attr_read(PKEY_SIZE_AES_192, false, buf,
					    off, count);
}

static ssize_t ccacipher_aes_256_read(struct file *filp,
				      struct kobject *kobj,
				      struct bin_attribute *attr,
				      char *buf, loff_t off,
				      size_t count)
{
	return pkey_ccacipher_aes_attr_read(PKEY_SIZE_AES_256, false, buf,
					    off, count);
}

static ssize_t ccacipher_aes_128_xts_read(struct file *filp,
					  struct kobject *kobj,
					  struct bin_attribute *attr,
					  char *buf, loff_t off,
					  size_t count)
{
	return pkey_ccacipher_aes_attr_read(PKEY_SIZE_AES_128, true, buf,
					    off, count);
}

static ssize_t ccacipher_aes_256_xts_read(struct file *filp,
					  struct kobject *kobj,
					  struct bin_attribute *attr,
					  char *buf, loff_t off,
					  size_t count)
{
	return pkey_ccacipher_aes_attr_read(PKEY_SIZE_AES_256, true, buf,
					    off, count);
}

static BIN_ATTR_RO(ccacipher_aes_128, CCACIPHERTOKENSIZE);
static BIN_ATTR_RO(ccacipher_aes_192, CCACIPHERTOKENSIZE);
static BIN_ATTR_RO(ccacipher_aes_256, CCACIPHERTOKENSIZE);
static BIN_ATTR_RO(ccacipher_aes_128_xts, 2 * CCACIPHERTOKENSIZE);
static BIN_ATTR_RO(ccacipher_aes_256_xts, 2 * CCACIPHERTOKENSIZE);

static struct bin_attribute *ccacipher_attrs[] = {
	&bin_attr_ccacipher_aes_128,
	&bin_attr_ccacipher_aes_192,
	&bin_attr_ccacipher_aes_256,
	&bin_attr_ccacipher_aes_128_xts,
	&bin_attr_ccacipher_aes_256_xts,
	NULL
};

static struct attribute_group ccacipher_attr_group = {
	.name	   = "ccacipher",
	.bin_attrs = ccacipher_attrs,
};

/*
 * Sysfs attribute read function for all ep11 aes key binary attributes.
 * The implementation can not deal with partial reads, because a new random
 * secure key blob is generated with each read. In case of partial reads
 * (i.e. off != 0 or count < key blob size) -EINVAL is returned.
 * This function and the sysfs attributes using it provide EP11 key blobs
 * padded to the upper limit of MAXEP11AESKEYBLOBSIZE which is currently
 * 320 bytes.
 */
static ssize_t pkey_ep11_aes_attr_read(enum pkey_key_size keybits,
				       bool is_xts, char *buf, loff_t off,
				       size_t count)
{
	int i, rc, card, dom;
	u32 nr_apqns, *apqns = NULL;
	size_t keysize = MAXEP11AESKEYBLOBSIZE;

	if (off != 0 || count < MAXEP11AESKEYBLOBSIZE)
		return -EINVAL;
	if (is_xts)
		if (count < 2 * MAXEP11AESKEYBLOBSIZE)
			return -EINVAL;

	/* build a list of apqns able to generate an cipher key */
	rc = ep11_findcard2(&apqns, &nr_apqns, 0xFFFF, 0xFFFF,
			    ZCRYPT_CEX7, EP11_API_V, NULL);
	if (rc)
		return rc;

	memset(buf, 0, is_xts ? 2 * keysize : keysize);

	/* simple try all apqns from the list */
	for (i = 0, rc = -ENODEV; i < nr_apqns; i++) {
		card = apqns[i] >> 16;
		dom = apqns[i] & 0xFFFF;
		rc = ep11_genaeskey(card, dom, keybits, 0, buf, &keysize);
		if (rc == 0)
			break;
	}
	if (rc)
		return rc;

	if (is_xts) {
		keysize = MAXEP11AESKEYBLOBSIZE;
		buf += MAXEP11AESKEYBLOBSIZE;
		rc = ep11_genaeskey(card, dom, keybits, 0, buf, &keysize);
		if (rc == 0)
			return 2 * MAXEP11AESKEYBLOBSIZE;
	}

	return MAXEP11AESKEYBLOBSIZE;
}

static ssize_t ep11_aes_128_read(struct file *filp,
				 struct kobject *kobj,
				 struct bin_attribute *attr,
				 char *buf, loff_t off,
				 size_t count)
{
	return pkey_ep11_aes_attr_read(PKEY_SIZE_AES_128, false, buf,
				       off, count);
}

static ssize_t ep11_aes_192_read(struct file *filp,
				 struct kobject *kobj,
				 struct bin_attribute *attr,
				 char *buf, loff_t off,
				 size_t count)
{
	return pkey_ep11_aes_attr_read(PKEY_SIZE_AES_192, false, buf,
				       off, count);
}

static ssize_t ep11_aes_256_read(struct file *filp,
				 struct kobject *kobj,
				 struct bin_attribute *attr,
				 char *buf, loff_t off,
				 size_t count)
{
	return pkey_ep11_aes_attr_read(PKEY_SIZE_AES_256, false, buf,
				       off, count);
}

static ssize_t ep11_aes_128_xts_read(struct file *filp,
				     struct kobject *kobj,
				     struct bin_attribute *attr,
				     char *buf, loff_t off,
				     size_t count)
{
	return pkey_ep11_aes_attr_read(PKEY_SIZE_AES_128, true, buf,
				       off, count);
}

static ssize_t ep11_aes_256_xts_read(struct file *filp,
				     struct kobject *kobj,
				     struct bin_attribute *attr,
				     char *buf, loff_t off,
				     size_t count)
{
	return pkey_ep11_aes_attr_read(PKEY_SIZE_AES_256, true, buf,
				       off, count);
}

static BIN_ATTR_RO(ep11_aes_128, MAXEP11AESKEYBLOBSIZE);
static BIN_ATTR_RO(ep11_aes_192, MAXEP11AESKEYBLOBSIZE);
static BIN_ATTR_RO(ep11_aes_256, MAXEP11AESKEYBLOBSIZE);
static BIN_ATTR_RO(ep11_aes_128_xts, 2 * MAXEP11AESKEYBLOBSIZE);
static BIN_ATTR_RO(ep11_aes_256_xts, 2 * MAXEP11AESKEYBLOBSIZE);

static struct bin_attribute *ep11_attrs[] = {
	&bin_attr_ep11_aes_128,
	&bin_attr_ep11_aes_192,
	&bin_attr_ep11_aes_256,
	&bin_attr_ep11_aes_128_xts,
	&bin_attr_ep11_aes_256_xts,
	NULL
};

static struct attribute_group ep11_attr_group = {
	.name	   = "ep11",
	.bin_attrs = ep11_attrs,
};

static const struct attribute_group *pkey_attr_groups[] = {
	&protkey_attr_group,
	&ccadata_attr_group,
	&ccacipher_attr_group,
	&ep11_attr_group,
	NULL,
};

>>>>>>> upstream/android-13
static const struct file_operations pkey_fops = {
	.owner		= THIS_MODULE,
	.open		= nonseekable_open,
	.llseek		= no_llseek,
	.unlocked_ioctl = pkey_unlocked_ioctl,
};

static struct miscdevice pkey_dev = {
	.name	= "pkey",
	.minor	= MISC_DYNAMIC_MINOR,
	.mode	= 0666,
	.fops	= &pkey_fops,
<<<<<<< HEAD
=======
	.groups = pkey_attr_groups,
>>>>>>> upstream/android-13
};

/*
 * Module init
 */
static int __init pkey_init(void)
{
<<<<<<< HEAD
	cpacf_mask_t pckmo_functions;

	/* check for pckmo instructions available */
	if (!cpacf_query(CPACF_PCKMO, &pckmo_functions))
		return -EOPNOTSUPP;
	if (!cpacf_test_func(&pckmo_functions, CPACF_PCKMO_ENC_AES_128_KEY) ||
	    !cpacf_test_func(&pckmo_functions, CPACF_PCKMO_ENC_AES_192_KEY) ||
	    !cpacf_test_func(&pckmo_functions, CPACF_PCKMO_ENC_AES_256_KEY))
		return -EOPNOTSUPP;
=======
	cpacf_mask_t func_mask;

	/*
	 * The pckmo instruction should be available - even if we don't
	 * actually invoke it. This instruction comes with MSA 3 which
	 * is also the minimum level for the kmc instructions which
	 * are able to work with protected keys.
	 */
	if (!cpacf_query(CPACF_PCKMO, &func_mask))
		return -ENODEV;

	/* check for kmc instructions available */
	if (!cpacf_query(CPACF_KMC, &func_mask))
		return -ENODEV;
	if (!cpacf_test_func(&func_mask, CPACF_KMC_PAES_128) ||
	    !cpacf_test_func(&func_mask, CPACF_KMC_PAES_192) ||
	    !cpacf_test_func(&func_mask, CPACF_KMC_PAES_256))
		return -ENODEV;
>>>>>>> upstream/android-13

	pkey_debug_init();

	return misc_register(&pkey_dev);
}

/*
 * Module exit
 */
static void __exit pkey_exit(void)
{
	misc_deregister(&pkey_dev);
<<<<<<< HEAD
	mkvp_cache_free();
	pkey_debug_exit();
}

module_init(pkey_init);
=======
	pkey_debug_exit();
}

module_cpu_feature_match(MSA, pkey_init);
>>>>>>> upstream/android-13
module_exit(pkey_exit);
