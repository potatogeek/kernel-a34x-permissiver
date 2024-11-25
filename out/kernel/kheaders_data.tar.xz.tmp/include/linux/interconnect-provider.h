/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __LINUX_INTERCONNECT_PROVIDER_H
#define __LINUX_INTERCONNECT_PROVIDER_H

#include <linux/interconnect.h>

#define icc_units_to_bps(bw)  ((bw) * 1000ULL)

struct icc_node;
struct of_phandle_args;


struct icc_onecell_data {
	unsigned int num_nodes;
	struct icc_node *nodes[];
};

struct icc_node *of_icc_xlate_onecell(struct of_phandle_args *spec,
				      void *data);


struct icc_provider {
	struct list_head	provider_list;
	struct list_head	nodes;
	int (*set)(struct icc_node *src, struct icc_node *dst);
	int (*aggregate)(struct icc_node *node, u32 avg_bw, u32 peak_bw,
			 u32 *agg_avg, u32 *agg_peak);
	struct icc_node* (*xlate)(struct of_phandle_args *spec, void *data);
	struct device		*dev;
	int			users;
	void			*data;
};


struct icc_node {
	int			id;
	const char              *name;
	struct icc_node		**links;
	size_t			num_links;

	struct icc_provider	*provider;
	struct list_head	node_list;
	struct list_head	search_list;
	struct icc_node		*reverse;
	u8			is_traversed:1;
	struct hlist_head	req_list;
	u32			avg_bw;
	u32			peak_bw;
	void			*data;
};

#if IS_ENABLED(CONFIG_INTERCONNECT)

struct icc_node *icc_node_create(int id);
void icc_node_destroy(int id);
int icc_link_create(struct icc_node *node, const int dst_id);
int icc_link_destroy(struct icc_node *src, struct icc_node *dst);
void icc_node_add(struct icc_node *node, struct icc_provider *provider);
void icc_node_del(struct icc_node *node);
int icc_provider_add(struct icc_provider *provider);
int icc_provider_del(struct icc_provider *provider);

#else

static inline struct icc_node *icc_node_create(int id)
{
	return ERR_PTR(-ENOTSUPP);
}

void icc_node_destroy(int id)
{
}

static inline int icc_link_create(struct icc_node *node, const int dst_id)
{
	return -ENOTSUPP;
}

int icc_link_destroy(struct icc_node *src, struct icc_node *dst)
{
	return -ENOTSUPP;
}

void icc_node_add(struct icc_node *node, struct icc_provider *provider)
{
}

void icc_node_del(struct icc_node *node)
{
}

static inline int icc_provider_add(struct icc_provider *provider)
{
	return -ENOTSUPP;
}

static inline int icc_provider_del(struct icc_provider *provider)
{
	return -ENOTSUPP;
}

#endif 

#endif 
