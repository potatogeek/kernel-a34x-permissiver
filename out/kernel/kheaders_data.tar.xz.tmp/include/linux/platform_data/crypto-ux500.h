
#ifndef _CRYPTO_UX500_H
#define _CRYPTO_UX500_H
#include <linux/dmaengine.h>
#include <linux/platform_data/dma-ste-dma40.h>

struct hash_platform_data {
	void *mem_to_engine;
	bool (*dma_filter)(struct dma_chan *chan, void *filter_param);
};

struct cryp_platform_data {
	struct stedma40_chan_cfg mem_to_engine;
	struct stedma40_chan_cfg engine_to_mem;
};

#endif
