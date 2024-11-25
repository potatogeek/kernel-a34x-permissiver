

#ifndef _IB_CACHE_H
#define _IB_CACHE_H

#include <rdma/ib_verbs.h>

int rdma_query_gid(struct ib_device *device, u8 port_num, int index,
		   union ib_gid *gid);
const struct ib_gid_attr *rdma_find_gid(struct ib_device *device,
					const union ib_gid *gid,
					enum ib_gid_type gid_type,
					struct net_device *ndev);
const struct ib_gid_attr *rdma_find_gid_by_port(struct ib_device *ib_dev,
						const union ib_gid *gid,
						enum ib_gid_type gid_type,
						u8 port,
						struct net_device *ndev);
const struct ib_gid_attr *rdma_find_gid_by_filter(
	struct ib_device *device, const union ib_gid *gid, u8 port_num,
	bool (*filter)(const union ib_gid *gid, const struct ib_gid_attr *,
		       void *),
	void *context);


int ib_get_cached_pkey(struct ib_device    *device_handle,
		       u8                   port_num,
		       int                  index,
		       u16                 *pkey);


int ib_find_cached_pkey(struct ib_device    *device,
			u8                   port_num,
			u16                  pkey,
			u16                 *index);


int ib_find_exact_cached_pkey(struct ib_device    *device,
			      u8                   port_num,
			      u16                  pkey,
			      u16                 *index);


int ib_get_cached_lmc(struct ib_device *device,
		      u8                port_num,
		      u8                *lmc);


int ib_get_cached_port_state(struct ib_device *device,
			      u8                port_num,
			      enum ib_port_state *port_active);

bool rdma_is_zero_gid(const union ib_gid *gid);
const struct ib_gid_attr *rdma_get_gid_attr(struct ib_device *device,
					    u8 port_num, int index);
void rdma_put_gid_attr(const struct ib_gid_attr *attr);
void rdma_hold_gid_attr(const struct ib_gid_attr *attr);

#endif 
