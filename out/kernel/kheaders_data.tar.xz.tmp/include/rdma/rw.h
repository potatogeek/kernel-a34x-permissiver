
#ifndef _RDMA_RW_H
#define _RDMA_RW_H

#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#include <rdma/mr_pool.h>

struct rdma_rw_ctx {
	
	u32			nr_ops;

	
	u8			type;

	union {
		
		struct {
			struct ib_sge		sge;
			struct ib_rdma_wr	wr;
		} single;

		
		struct {
			struct ib_sge		*sges;
			struct ib_rdma_wr	*wrs;
		} map;

		
		struct rdma_rw_reg_ctx {
			struct ib_sge		sge;
			struct ib_rdma_wr	wr;
			struct ib_reg_wr	reg_wr;
			struct ib_send_wr	inv_wr;
			struct ib_mr		*mr;
		} *reg;

		struct {
			struct rdma_rw_reg_ctx	data;
			struct rdma_rw_reg_ctx	prot;
			struct ib_send_wr	sig_inv_wr;
			struct ib_mr		*sig_mr;
			struct ib_sge		sig_sge;
			struct ib_sig_handover_wr sig_wr;
		} *sig;
	};
};

int rdma_rw_ctx_init(struct rdma_rw_ctx *ctx, struct ib_qp *qp, u8 port_num,
		struct scatterlist *sg, u32 sg_cnt, u32 sg_offset,
		u64 remote_addr, u32 rkey, enum dma_data_direction dir);
void rdma_rw_ctx_destroy(struct rdma_rw_ctx *ctx, struct ib_qp *qp, u8 port_num,
		struct scatterlist *sg, u32 sg_cnt,
		enum dma_data_direction dir);

int rdma_rw_ctx_signature_init(struct rdma_rw_ctx *ctx, struct ib_qp *qp,
		u8 port_num, struct scatterlist *sg, u32 sg_cnt,
		struct scatterlist *prot_sg, u32 prot_sg_cnt,
		struct ib_sig_attrs *sig_attrs, u64 remote_addr, u32 rkey,
		enum dma_data_direction dir);
void rdma_rw_ctx_destroy_signature(struct rdma_rw_ctx *ctx, struct ib_qp *qp,
		u8 port_num, struct scatterlist *sg, u32 sg_cnt,
		struct scatterlist *prot_sg, u32 prot_sg_cnt,
		enum dma_data_direction dir);

struct ib_send_wr *rdma_rw_ctx_wrs(struct rdma_rw_ctx *ctx, struct ib_qp *qp,
		u8 port_num, struct ib_cqe *cqe, struct ib_send_wr *chain_wr);
int rdma_rw_ctx_post(struct rdma_rw_ctx *ctx, struct ib_qp *qp, u8 port_num,
		struct ib_cqe *cqe, struct ib_send_wr *chain_wr);

unsigned int rdma_rw_mr_factor(struct ib_device *device, u8 port_num,
		unsigned int maxpages);
void rdma_rw_init_qp(struct ib_device *dev, struct ib_qp_init_attr *attr);
int rdma_rw_init_mrs(struct ib_qp *qp, struct ib_qp_init_attr *attr);
void rdma_rw_cleanup_mrs(struct ib_qp *qp);

#endif 
