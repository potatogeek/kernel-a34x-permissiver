

#ifndef _MEDIA_VIDEOBUF2_DMA_CONTIG_H
#define _MEDIA_VIDEOBUF2_DMA_CONTIG_H

#include <media/videobuf2-v4l2.h>
#include <linux/dma-mapping.h>

static inline dma_addr_t
vb2_dma_contig_plane_dma_addr(struct vb2_buffer *vb, unsigned int plane_no)
{
	dma_addr_t *addr = vb2_plane_cookie(vb, plane_no);

	return *addr;
}

int vb2_dma_contig_set_max_seg_size(struct device *dev, unsigned int size);
void vb2_dma_contig_clear_max_seg_size(struct device *dev);

extern const struct vb2_mem_ops vb2_dma_contig_memops;

#endif
