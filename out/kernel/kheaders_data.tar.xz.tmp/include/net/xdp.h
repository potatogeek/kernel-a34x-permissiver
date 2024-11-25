
#ifndef __LINUX_NET_XDP_H__
#define __LINUX_NET_XDP_H__



enum xdp_mem_type {
	MEM_TYPE_PAGE_SHARED = 0, 
	MEM_TYPE_PAGE_ORDER0,     
	MEM_TYPE_PAGE_POOL,
	MEM_TYPE_ZERO_COPY,
	MEM_TYPE_MAX,
};


#define XDP_XMIT_FLUSH		(1U << 0)	
#define XDP_XMIT_FLAGS_MASK	XDP_XMIT_FLUSH

struct xdp_mem_info {
	u32 type; 
	u32 id;
};

struct page_pool;

struct zero_copy_allocator {
	void (*free)(struct zero_copy_allocator *zca, unsigned long handle);
};

struct xdp_rxq_info {
	struct net_device *dev;
	u32 queue_index;
	u32 reg_state;
	struct xdp_mem_info mem;
} ____cacheline_aligned; 

struct xdp_buff {
	void *data;
	void *data_end;
	void *data_meta;
	void *data_hard_start;
	unsigned long handle;
	struct xdp_rxq_info *rxq;
};

struct xdp_frame {
	void *data;
	u16 len;
	u16 headroom;
	u16 metasize;
	
	struct xdp_mem_info mem;
	struct net_device *dev_rx; 
};


static inline void xdp_scrub_frame(struct xdp_frame *frame)
{
	frame->data = NULL;
	frame->dev_rx = NULL;
}


static inline
struct xdp_frame *convert_to_xdp_frame(struct xdp_buff *xdp)
{
	struct xdp_frame *xdp_frame;
	int metasize;
	int headroom;

	
	if (xdp->rxq->mem.type == MEM_TYPE_ZERO_COPY)
		return NULL;

	
	headroom = xdp->data - xdp->data_hard_start;
	metasize = xdp->data - xdp->data_meta;
	metasize = metasize > 0 ? metasize : 0;
	if (unlikely((headroom - metasize) < sizeof(*xdp_frame)))
		return NULL;

	
	xdp_frame = xdp->data_hard_start;

	xdp_frame->data = xdp->data;
	xdp_frame->len  = xdp->data_end - xdp->data;
	xdp_frame->headroom = headroom - sizeof(*xdp_frame);
	xdp_frame->metasize = metasize;

	
	xdp_frame->mem = xdp->rxq->mem;

	return xdp_frame;
}

void xdp_return_frame(struct xdp_frame *xdpf);
void xdp_return_frame_rx_napi(struct xdp_frame *xdpf);
void xdp_return_buff(struct xdp_buff *xdp);

int xdp_rxq_info_reg(struct xdp_rxq_info *xdp_rxq,
		     struct net_device *dev, u32 queue_index);
void xdp_rxq_info_unreg(struct xdp_rxq_info *xdp_rxq);
void xdp_rxq_info_unused(struct xdp_rxq_info *xdp_rxq);
bool xdp_rxq_info_is_reg(struct xdp_rxq_info *xdp_rxq);
int xdp_rxq_info_reg_mem_model(struct xdp_rxq_info *xdp_rxq,
			       enum xdp_mem_type type, void *allocator);


static __always_inline void
xdp_set_data_meta_invalid(struct xdp_buff *xdp)
{
	xdp->data_meta = xdp->data + 1;
}

static __always_inline bool
xdp_data_meta_unsupported(const struct xdp_buff *xdp)
{
	return unlikely(xdp->data_meta > xdp->data);
}

struct xdp_attachment_info {
	struct bpf_prog *prog;
	u32 flags;
};

struct netdev_bpf;
int xdp_attachment_query(struct xdp_attachment_info *info,
			 struct netdev_bpf *bpf);
bool xdp_attachment_flags_ok(struct xdp_attachment_info *info,
			     struct netdev_bpf *bpf);
void xdp_attachment_setup(struct xdp_attachment_info *info,
			  struct netdev_bpf *bpf);

#endif 
