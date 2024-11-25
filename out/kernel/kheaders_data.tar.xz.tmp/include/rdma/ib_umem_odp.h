

#ifndef IB_UMEM_ODP_H
#define IB_UMEM_ODP_H

#include <rdma/ib_umem.h>
#include <rdma/ib_verbs.h>
#include <linux/interval_tree.h>

struct umem_odp_node {
	u64 __subtree_last;
	struct rb_node rb;
};

struct ib_umem_odp {
	
	struct page		**page_list;
	
	dma_addr_t		*dma_list;
	
	struct mutex		umem_mutex;
	void			*private; 

	
	bool mn_counters_active;
	int notifiers_seq;
	int notifiers_count;

	
	struct list_head no_private_counters;
	struct ib_umem		*umem;

	
	struct umem_odp_node	interval_tree;

	struct completion	notifier_completion;
	int			dying;
	struct work_struct	work;
};

#ifdef CONFIG_INFINIBAND_ON_DEMAND_PAGING

int ib_umem_odp_get(struct ib_ucontext *context, struct ib_umem *umem,
		    int access);
struct ib_umem *ib_alloc_odp_umem(struct ib_ucontext *context,
				  unsigned long addr,
				  size_t size);

void ib_umem_odp_release(struct ib_umem *umem);


#define ODP_READ_ALLOWED_BIT  (1<<0ULL)
#define ODP_WRITE_ALLOWED_BIT (1<<1ULL)

#define ODP_DMA_ADDR_MASK (~(ODP_READ_ALLOWED_BIT | ODP_WRITE_ALLOWED_BIT))

int ib_umem_odp_map_dma_pages(struct ib_umem *umem, u64 start_offset, u64 bcnt,
			      u64 access_mask, unsigned long current_seq);

void ib_umem_odp_unmap_dma_pages(struct ib_umem *umem, u64 start_offset,
				 u64 bound);

typedef int (*umem_call_back)(struct ib_umem *item, u64 start, u64 end,
			      void *cookie);

int rbt_ib_umem_for_each_in_range(struct rb_root_cached *root,
				  u64 start, u64 end,
				  umem_call_back cb,
				  bool blockable, void *cookie);


struct ib_umem_odp *rbt_ib_umem_lookup(struct rb_root_cached *root,
				       u64 addr, u64 length);

static inline int ib_umem_mmu_notifier_retry(struct ib_umem *item,
					     unsigned long mmu_seq)
{
	

	
	if (!item->odp_data->mn_counters_active)
		return 1;

	if (unlikely(item->odp_data->notifiers_count))
		return 1;
	if (item->odp_data->notifiers_seq != mmu_seq)
		return 1;
	return 0;
}

#else 

static inline int ib_umem_odp_get(struct ib_ucontext *context,
				  struct ib_umem *umem,
				  int access)
{
	return -EINVAL;
}

static inline struct ib_umem *ib_alloc_odp_umem(struct ib_ucontext *context,
						unsigned long addr,
						size_t size)
{
	return ERR_PTR(-EINVAL);
}

static inline void ib_umem_odp_release(struct ib_umem *umem) {}

#endif 

#endif 
