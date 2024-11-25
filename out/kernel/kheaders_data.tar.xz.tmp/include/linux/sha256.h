

#ifndef SHA256_H
#define SHA256_H

#include <linux/types.h>
#include <crypto/sha.h>



extern int sha256_init(struct sha256_state *sctx);
extern int sha256_update(struct sha256_state *sctx, const u8 *input,
			 unsigned int length);
extern int sha256_final(struct sha256_state *sctx, u8 *hash);

#endif 
