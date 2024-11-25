#ifndef DEF_RDMAVT_INCQP_H
#define DEF_RDMAVT_INCQP_H



#include <rdma/rdma_vt.h>
#include <rdma/ib_pack.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdmavt_cq.h>

#define RVT_R_WRID_VALID        0
#define RVT_R_REWIND_SGE        1


#define RVT_R_REUSE_SGE 0x01
#define RVT_R_RDMAR_SEQ 0x02
#define RVT_R_RSP_NAK   0x04
#define RVT_R_RSP_SEND  0x08
#define RVT_R_COMM_EST  0x10


#define RVT_S_SIGNAL_REQ_WR	0x0001
#define RVT_S_BUSY		0x0002
#define RVT_S_TIMER		0x0004
#define RVT_S_RESP_PENDING	0x0008
#define RVT_S_ACK_PENDING	0x0010
#define RVT_S_WAIT_FENCE	0x0020
#define RVT_S_WAIT_RDMAR	0x0040
#define RVT_S_WAIT_RNR		0x0080
#define RVT_S_WAIT_SSN_CREDIT	0x0100
#define RVT_S_WAIT_DMA		0x0200
#define RVT_S_WAIT_PIO		0x0400
#define RVT_S_WAIT_TX		0x0800
#define RVT_S_WAIT_DMA_DESC	0x1000
#define RVT_S_WAIT_KMEM		0x2000
#define RVT_S_WAIT_PSN		0x4000
#define RVT_S_WAIT_ACK		0x8000
#define RVT_S_SEND_ONE		0x10000
#define RVT_S_UNLIMITED_CREDIT	0x20000
#define RVT_S_ECN		0x40000
#define RVT_S_MAX_BIT_MASK	0x800000




#define RVT_S_ANY_WAIT_IO \
	(RVT_S_WAIT_PIO | RVT_S_WAIT_TX | \
	 RVT_S_WAIT_DMA_DESC | RVT_S_WAIT_KMEM)


#define RVT_S_ANY_WAIT_SEND (RVT_S_WAIT_FENCE | RVT_S_WAIT_RDMAR | \
	RVT_S_WAIT_RNR | RVT_S_WAIT_SSN_CREDIT | RVT_S_WAIT_DMA | \
	RVT_S_WAIT_PSN | RVT_S_WAIT_ACK)

#define RVT_S_ANY_WAIT (RVT_S_ANY_WAIT_IO | RVT_S_ANY_WAIT_SEND)


#define RVT_OPCODE_QP_MASK 0xE0


#define RVT_POST_SEND_OK                0x01
#define RVT_POST_RECV_OK                0x02
#define RVT_PROCESS_RECV_OK             0x04
#define RVT_PROCESS_SEND_OK             0x08
#define RVT_PROCESS_NEXT_SEND_OK        0x10
#define RVT_FLUSH_SEND			0x20
#define RVT_FLUSH_RECV			0x40
#define RVT_PROCESS_OR_FLUSH_SEND \
	(RVT_PROCESS_SEND_OK | RVT_FLUSH_SEND)
#define RVT_SEND_OR_FLUSH_OR_RECV_OK \
	(RVT_PROCESS_SEND_OK | RVT_FLUSH_SEND | RVT_PROCESS_RECV_OK)


#define RVT_SEND_RESERVE_USED           IB_SEND_RESERVED_START
#define RVT_SEND_COMPLETION_ONLY	(IB_SEND_RESERVED_START << 1)


struct rvt_swqe {
	union {
		struct ib_send_wr wr;   
		struct ib_ud_wr ud_wr;
		struct ib_reg_wr reg_wr;
		struct ib_rdma_wr rdma_wr;
		struct ib_atomic_wr atomic_wr;
	};
	u32 psn;                
	u32 lpsn;               
	u32 ssn;                
	u32 length;             
	struct rvt_sge sg_list[0];
};


struct rvt_rwqe {
	u64 wr_id;
	u8 num_sge;
	struct ib_sge sg_list[0];
};


struct rvt_rwq {
	u32 head;               
	u32 tail;               
	struct rvt_rwqe wq[0];
};

struct rvt_rq {
	struct rvt_rwq *wq;
	u32 size;               
	u8 max_sge;
	
	spinlock_t lock ____cacheline_aligned_in_smp;
};


struct rvt_mmap_info {
	struct list_head pending_mmaps;
	struct ib_ucontext *context;
	void *obj;
	__u64 offset;
	struct kref ref;
	unsigned size;
};


struct rvt_ack_entry {
	struct rvt_sge rdma_sge;
	u64 atomic_data;
	u32 psn;
	u32 lpsn;
	u8 opcode;
	u8 sent;
};

#define	RC_QP_SCALING_INTERVAL	5

#define RVT_OPERATION_PRIV        0x00000001
#define RVT_OPERATION_ATOMIC      0x00000002
#define RVT_OPERATION_ATOMIC_SGE  0x00000004
#define RVT_OPERATION_LOCAL       0x00000008
#define RVT_OPERATION_USE_RESERVE 0x00000010

#define RVT_OPERATION_MAX (IB_WR_RESERVED10 + 1)



struct rvt_operation_params {
	size_t length;
	u32 qpt_support;
	u32 flags;
};


struct rvt_qp {
	struct ib_qp ibqp;
	void *priv; 
	
	struct rdma_ah_attr remote_ah_attr;
	struct rdma_ah_attr alt_ah_attr;
	struct rvt_qp __rcu *next;           
	struct rvt_swqe *s_wq;  
	struct rvt_mmap_info *ip;

	unsigned long timeout_jiffies;  

	int srate_mbps;		
	pid_t pid;		
	u32 remote_qpn;
	u32 qkey;               
	u32 s_size;             

	u16 pmtu;		
	u8 log_pmtu;		
	u8 state;               
	u8 allowed_ops;		
	u8 qp_access_flags;
	u8 alt_timeout;         
	u8 timeout;             
	u8 s_srate;
	u8 s_mig_state;
	u8 port_num;
	u8 s_pkey_index;        
	u8 s_alt_pkey_index;    
	u8 r_max_rd_atomic;     
	u8 s_max_rd_atomic;     
	u8 s_retry_cnt;         
	u8 s_rnr_retry_cnt;
	u8 r_min_rnr_timer;     
	u8 s_max_sge;           
	u8 s_draining;

	
	atomic_t refcount ____cacheline_aligned_in_smp;
	wait_queue_head_t wait;

	struct rvt_ack_entry *s_ack_queue;
	struct rvt_sge_state s_rdma_read_sge;

	spinlock_t r_lock ____cacheline_aligned_in_smp;      
	u32 r_psn;              
	unsigned long r_aflags;
	u64 r_wr_id;            
	u32 r_ack_psn;          
	u32 r_len;              
	u32 r_rcv_len;          
	u32 r_msn;              

	u8 r_state;             
	u8 r_flags;
	u8 r_head_ack_queue;    
	u8 r_adefered;          

	struct list_head rspwait;       

	struct rvt_sge_state r_sge;     
	struct rvt_rq r_rq;             

	
	spinlock_t s_hlock ____cacheline_aligned_in_smp;
	u32 s_head;             
	u32 s_next_psn;         
	u32 s_avail;            
	u32 s_ssn;              
	atomic_t s_reserved_used; 

	spinlock_t s_lock ____cacheline_aligned_in_smp;
	u32 s_flags;
	struct rvt_sge_state *s_cur_sge;
	struct rvt_swqe *s_wqe;
	struct rvt_sge_state s_sge;     
	struct rvt_mregion *s_rdma_mr;
	u32 s_len;              
	u32 s_rdma_read_len;    
	u32 s_last_psn;         
	u32 s_sending_psn;      
	u32 s_sending_hpsn;     
	u32 s_psn;              
	u32 s_ack_rdma_psn;     
	u32 s_ack_psn;          
	u32 s_tail;             
	u32 s_cur;              
	u32 s_acked;            
	u32 s_last;             
	u32 s_lsn;              
	u32 s_ahgpsn;           
	u16 s_cur_size;         
	u16 s_rdma_ack_cnt;
	u8 s_hdrwords;         
	s8 s_ahgidx;
	u8 s_state;             
	u8 s_ack_state;         
	u8 s_nak_state;         
	u8 r_nak_state;         
	u8 s_retry;             
	u8 s_rnr_retry;         
	u8 s_num_rd_atomic;     
	u8 s_tail_ack_queue;    

	struct rvt_sge_state s_ack_rdma_sge;
	struct timer_list s_timer;
	struct hrtimer s_rnr_timer;

	atomic_t local_ops_pending; 

	
	struct rvt_sge r_sg_list[0] 
		____cacheline_aligned_in_smp;
};

struct rvt_srq {
	struct ib_srq ibsrq;
	struct rvt_rq rq;
	struct rvt_mmap_info *ip;
	
	u32 limit;
};

#define RVT_QPN_MAX                 BIT(24)
#define RVT_QPNMAP_ENTRIES          (RVT_QPN_MAX / PAGE_SIZE / BITS_PER_BYTE)
#define RVT_BITS_PER_PAGE           (PAGE_SIZE * BITS_PER_BYTE)
#define RVT_BITS_PER_PAGE_MASK      (RVT_BITS_PER_PAGE - 1)
#define RVT_QPN_MASK		    IB_QPN_MASK


struct rvt_qpn_map {
	void *page;
};

struct rvt_qpn_table {
	spinlock_t lock; 
	unsigned flags;         
	u32 last;               
	u32 nmaps;              
	u16 limit;
	u8  incr;
	
	struct rvt_qpn_map map[RVT_QPNMAP_ENTRIES];
};

struct rvt_qp_ibdev {
	u32 qp_table_size;
	u32 qp_table_bits;
	struct rvt_qp __rcu **qp_table;
	spinlock_t qpt_lock; 
	struct rvt_qpn_table qpn_table;
};


struct rvt_mcast_qp {
	struct list_head list;
	struct rvt_qp *qp;
};

struct rvt_mcast_addr {
	union ib_gid mgid;
	u16 lid;
};

struct rvt_mcast {
	struct rb_node rb_node;
	struct rvt_mcast_addr mcast_addr;
	struct list_head qp_list;
	wait_queue_head_t wait;
	atomic_t refcount;
	int n_attached;
};


static inline struct rvt_swqe *rvt_get_swqe_ptr(struct rvt_qp *qp,
						unsigned n)
{
	return (struct rvt_swqe *)((char *)qp->s_wq +
				     (sizeof(struct rvt_swqe) +
				      qp->s_max_sge *
				      sizeof(struct rvt_sge)) * n);
}


static inline struct rvt_rwqe *rvt_get_rwqe_ptr(struct rvt_rq *rq, unsigned n)
{
	return (struct rvt_rwqe *)
		((char *)rq->wq->wq +
		 (sizeof(struct rvt_rwqe) +
		  rq->max_sge * sizeof(struct ib_sge)) * n);
}


static inline bool rvt_is_user_qp(struct rvt_qp *qp)
{
	return !!qp->pid;
}


static inline void rvt_get_qp(struct rvt_qp *qp)
{
	atomic_inc(&qp->refcount);
}


static inline void rvt_put_qp(struct rvt_qp *qp)
{
	if (qp && atomic_dec_and_test(&qp->refcount))
		wake_up(&qp->wait);
}


static inline void rvt_put_swqe(struct rvt_swqe *wqe)
{
	int i;

	for (i = 0; i < wqe->wr.num_sge; i++) {
		struct rvt_sge *sge = &wqe->sg_list[i];

		rvt_put_mr(sge->mr);
	}
}


static inline void rvt_qp_wqe_reserve(
	struct rvt_qp *qp,
	struct rvt_swqe *wqe)
{
	atomic_inc(&qp->s_reserved_used);
}


static inline void rvt_qp_wqe_unreserve(
	struct rvt_qp *qp,
	struct rvt_swqe *wqe)
{
	if (unlikely(wqe->wr.send_flags & RVT_SEND_RESERVE_USED)) {
		atomic_dec(&qp->s_reserved_used);
		
		smp_mb__after_atomic();
	}
}

extern const enum ib_wc_opcode ib_rvt_wc_opcode[];


static inline void rvt_qp_swqe_complete(
	struct rvt_qp *qp,
	struct rvt_swqe *wqe,
	enum ib_wc_opcode opcode,
	enum ib_wc_status status)
{
	if (unlikely(wqe->wr.send_flags & RVT_SEND_RESERVE_USED))
		return;
	if (!(qp->s_flags & RVT_S_SIGNAL_REQ_WR) ||
	    (wqe->wr.send_flags & IB_SEND_SIGNALED) ||
	     status != IB_WC_SUCCESS) {
		struct ib_wc wc;

		memset(&wc, 0, sizeof(wc));
		wc.wr_id = wqe->wr.wr_id;
		wc.status = status;
		wc.opcode = opcode;
		wc.qp = &qp->ibqp;
		wc.byte_len = wqe->length;
		rvt_cq_enter(ibcq_to_rvtcq(qp->ibqp.send_cq), &wc,
			     status != IB_WC_SUCCESS);
	}
}


static inline int rvt_cmp_msn(u32 a, u32 b)
{
	return (((int)a) - ((int)b)) << 8;
}


__be32 rvt_compute_aeth(struct rvt_qp *qp);


void rvt_get_credit(struct rvt_qp *qp, u32 aeth);


static inline u32 rvt_div_round_up_mtu(struct rvt_qp *qp, u32 len)
{
	return (len + qp->pmtu - 1) >> qp->log_pmtu;
}


static inline u32 rvt_div_mtu(struct rvt_qp *qp, u32 len)
{
	return len >> qp->log_pmtu;
}


static inline unsigned long rvt_timeout_to_jiffies(u8 timeout)
{
	if (timeout > 31)
		timeout = 31;

	return usecs_to_jiffies(1U << timeout) * 4096UL / 1000UL;
}

extern const int  ib_rvt_state_ops[];

struct rvt_dev_info;
int rvt_get_rwqe(struct rvt_qp *qp, bool wr_id_only);
void rvt_comm_est(struct rvt_qp *qp);
int rvt_error_qp(struct rvt_qp *qp, enum ib_wc_status err);
void rvt_rc_error(struct rvt_qp *qp, enum ib_wc_status err);
unsigned long rvt_rnr_tbl_to_usec(u32 index);
enum hrtimer_restart rvt_rc_rnr_retry(struct hrtimer *t);
void rvt_add_rnr_timer(struct rvt_qp *qp, u32 aeth);
void rvt_del_timers_sync(struct rvt_qp *qp);
void rvt_stop_rc_timers(struct rvt_qp *qp);
void rvt_add_retry_timer(struct rvt_qp *qp);


struct rvt_qp_iter {
	struct rvt_qp *qp;
	
	struct rvt_dev_info *rdi;
	
	void (*cb)(struct rvt_qp *qp, u64 v);
	
	u64 v;
	
	int specials;
	
	int n;
};

struct rvt_qp_iter *rvt_qp_iter_init(struct rvt_dev_info *rdi,
				     u64 v,
				     void (*cb)(struct rvt_qp *qp, u64 v));
int rvt_qp_iter_next(struct rvt_qp_iter *iter);
void rvt_qp_iter(struct rvt_dev_info *rdi,
		 u64 v,
		 void (*cb)(struct rvt_qp *qp, u64 v));
void rvt_qp_mr_clean(struct rvt_qp *qp, u32 lkey);
#endif          
