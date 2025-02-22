

#ifndef _DIGSIG_H
#define _DIGSIG_H

#include <linux/key.h>

enum pubkey_algo {
	PUBKEY_ALGO_RSA,
	PUBKEY_ALGO_MAX,
};

enum digest_algo {
	DIGEST_ALGO_SHA1,
	DIGEST_ALGO_SHA256,
	DIGEST_ALGO_MAX
};

struct pubkey_hdr {
	uint8_t		version;	
	uint32_t	timestamp;	
	uint8_t		algo;
	uint8_t		nmpi;
	char		mpi[0];
} __packed;

struct signature_hdr {
	uint8_t		version;	
	uint32_t	timestamp;	
	uint8_t		algo;
	uint8_t		hash;
	uint8_t		keyid[8];
	uint8_t		nmpi;
	char		mpi[0];
} __packed;

#if defined(CONFIG_SIGNATURE) || defined(CONFIG_SIGNATURE_MODULE)

int digsig_verify(struct key *keyring, const char *sig, int siglen,
					const char *digest, int digestlen);

#else

static inline int digsig_verify(struct key *keyring, const char *sig,
				int siglen, const char *digest, int digestlen)
{
	return -EOPNOTSUPP;
}

#endif 

#endif 
