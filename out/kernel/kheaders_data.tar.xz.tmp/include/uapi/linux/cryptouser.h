/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#include <linux/types.h>


enum {
	CRYPTO_MSG_BASE = 0x10,
	CRYPTO_MSG_NEWALG = 0x10,
	CRYPTO_MSG_DELALG,
	CRYPTO_MSG_UPDATEALG,
	CRYPTO_MSG_GETALG,
	CRYPTO_MSG_DELRNG,
	__CRYPTO_MSG_MAX
};
#define CRYPTO_MSG_MAX (__CRYPTO_MSG_MAX - 1)
#define CRYPTO_NR_MSGTYPES (CRYPTO_MSG_MAX + 1 - CRYPTO_MSG_BASE)

#define CRYPTO_MAX_NAME 64


enum crypto_attr_type_t {
	CRYPTOCFGA_UNSPEC,
	CRYPTOCFGA_PRIORITY_VAL,	
	CRYPTOCFGA_REPORT_LARVAL,	
	CRYPTOCFGA_REPORT_HASH,		
	CRYPTOCFGA_REPORT_BLKCIPHER,	
	CRYPTOCFGA_REPORT_AEAD,		
	CRYPTOCFGA_REPORT_COMPRESS,	
	CRYPTOCFGA_REPORT_RNG,		
	CRYPTOCFGA_REPORT_CIPHER,	
	CRYPTOCFGA_REPORT_AKCIPHER,	
	CRYPTOCFGA_REPORT_KPP,		
	CRYPTOCFGA_REPORT_ACOMP,	
	__CRYPTOCFGA_MAX

#define CRYPTOCFGA_MAX (__CRYPTOCFGA_MAX - 1)
};

struct crypto_user_alg {
	char cru_name[CRYPTO_MAX_NAME];
	char cru_driver_name[CRYPTO_MAX_NAME];
	char cru_module_name[CRYPTO_MAX_NAME];
	__u32 cru_type;
	__u32 cru_mask;
	__u32 cru_refcnt;
	__u32 cru_flags;
};

struct crypto_report_larval {
	char type[CRYPTO_MAX_NAME];
};

struct crypto_report_hash {
	char type[CRYPTO_MAX_NAME];
	unsigned int blocksize;
	unsigned int digestsize;
};

struct crypto_report_cipher {
	char type[CRYPTO_MAX_NAME];
	unsigned int blocksize;
	unsigned int min_keysize;
	unsigned int max_keysize;
};

struct crypto_report_blkcipher {
	char type[CRYPTO_MAX_NAME];
	char geniv[CRYPTO_MAX_NAME];
	unsigned int blocksize;
	unsigned int min_keysize;
	unsigned int max_keysize;
	unsigned int ivsize;
};

struct crypto_report_aead {
	char type[CRYPTO_MAX_NAME];
	char geniv[CRYPTO_MAX_NAME];
	unsigned int blocksize;
	unsigned int maxauthsize;
	unsigned int ivsize;
};

struct crypto_report_comp {
	char type[CRYPTO_MAX_NAME];
};

struct crypto_report_rng {
	char type[CRYPTO_MAX_NAME];
	unsigned int seedsize;
};

struct crypto_report_akcipher {
	char type[CRYPTO_MAX_NAME];
};

struct crypto_report_kpp {
	char type[CRYPTO_MAX_NAME];
};

struct crypto_report_acomp {
	char type[CRYPTO_MAX_NAME];
};

#define CRYPTO_REPORT_MAXSIZE (sizeof(struct crypto_user_alg) + \
			       sizeof(struct crypto_report_blkcipher))
