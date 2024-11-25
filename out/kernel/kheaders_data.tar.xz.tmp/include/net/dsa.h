

#ifndef __LINUX_NET_DSA_H
#define __LINUX_NET_DSA_H

#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/of.h>
#include <linux/ethtool.h>
#include <linux/net_tstamp.h>
#include <linux/phy.h>
#include <net/devlink.h>
#include <net/switchdev.h>

struct tc_action;
struct phy_device;
struct fixed_phy_status;
struct phylink_link_state;

enum dsa_tag_protocol {
	DSA_TAG_PROTO_NONE = 0,
	DSA_TAG_PROTO_BRCM,
	DSA_TAG_PROTO_BRCM_PREPEND,
	DSA_TAG_PROTO_DSA,
	DSA_TAG_PROTO_EDSA,
	DSA_TAG_PROTO_KSZ,
	DSA_TAG_PROTO_LAN9303,
	DSA_TAG_PROTO_MTK,
	DSA_TAG_PROTO_QCA,
	DSA_TAG_PROTO_TRAILER,
	DSA_TAG_LAST,		
};

#define DSA_MAX_SWITCHES	4
#define DSA_MAX_PORTS		12

#define DSA_RTABLE_NONE		-1

struct dsa_chip_data {
	
	struct device	*host_dev;
	int		sw_addr;

	
	struct device	*netdev[DSA_MAX_PORTS];

	
	int		eeprom_len;

	
	struct device_node *of_node;

	
	char		*port_names[DSA_MAX_PORTS];
	struct device_node *port_dn[DSA_MAX_PORTS];

	
	s8		rtable[DSA_MAX_SWITCHES];
};

struct dsa_platform_data {
	
	struct device	*netdev;
	struct net_device *of_netdev;

	
	int		nr_chips;
	struct dsa_chip_data	*chip;
};

struct packet_type;
struct dsa_switch;

struct dsa_device_ops {
	struct sk_buff *(*xmit)(struct sk_buff *skb, struct net_device *dev);
	struct sk_buff *(*rcv)(struct sk_buff *skb, struct net_device *dev,
			       struct packet_type *pt);
	int (*flow_dissect)(const struct sk_buff *skb, __be16 *proto,
			    int *offset);
};

struct dsa_switch_tree {
	struct list_head	list;

	
	struct raw_notifier_head	nh;

	
	unsigned int index;

	
	struct kref refcount;

	
	bool setup;

	
	struct dsa_platform_data	*pd;

	
	struct dsa_port		*cpu_dp;

	
	struct dsa_switch	*ds[DSA_MAX_SWITCHES];
};


enum dsa_port_mall_action_type {
	DSA_PORT_MALL_MIRROR,
};


struct dsa_mall_mirror_tc_entry {
	u8 to_local_port;
	bool ingress;
};


struct dsa_mall_tc_entry {
	struct list_head list;
	unsigned long cookie;
	enum dsa_port_mall_action_type type;
	union {
		struct dsa_mall_mirror_tc_entry mirror;
	};
};


struct dsa_port {
	
	union {
		struct net_device *master;
		struct net_device *slave;
	};

	
	const struct dsa_device_ops *tag_ops;

	
	struct dsa_switch_tree *dst;
	struct sk_buff *(*rcv)(struct sk_buff *skb, struct net_device *dev,
			       struct packet_type *pt);

	enum {
		DSA_PORT_TYPE_UNUSED = 0,
		DSA_PORT_TYPE_CPU,
		DSA_PORT_TYPE_DSA,
		DSA_PORT_TYPE_USER,
	} type;

	struct dsa_switch	*ds;
	unsigned int		index;
	const char		*name;
	const struct dsa_port	*cpu_dp;
	const char		*mac;
	struct device_node	*dn;
	unsigned int		ageing_time;
	u8			stp_state;
	struct net_device	*bridge_dev;
	struct devlink_port	devlink_port;
	struct phylink		*pl;
	
	const struct ethtool_ops *orig_ethtool_ops;
};

struct dsa_switch {
	struct device *dev;

	
	struct dsa_switch_tree	*dst;
	unsigned int		index;

	
	struct notifier_block	nb;

	
	void *priv;

	
	struct dsa_chip_data	*cd;

	
	const struct dsa_switch_ops	*ops;

	
	s8		rtable[DSA_MAX_SWITCHES];

	
	u32			phys_mii_mask;
	struct mii_bus		*slave_mii_bus;

	
	unsigned int ageing_time_min;
	unsigned int ageing_time_max;

	
	struct devlink		*devlink;

	
	unsigned int		num_tx_queues;

	unsigned long		*bitmap;
	unsigned long		_bitmap;

	
	size_t num_ports;
	struct dsa_port ports[];
};

static inline const struct dsa_port *dsa_to_port(struct dsa_switch *ds, int p)
{
	return &ds->ports[p];
}

static inline bool dsa_is_unused_port(struct dsa_switch *ds, int p)
{
	return dsa_to_port(ds, p)->type == DSA_PORT_TYPE_UNUSED;
}

static inline bool dsa_is_cpu_port(struct dsa_switch *ds, int p)
{
	return dsa_to_port(ds, p)->type == DSA_PORT_TYPE_CPU;
}

static inline bool dsa_is_dsa_port(struct dsa_switch *ds, int p)
{
	return dsa_to_port(ds, p)->type == DSA_PORT_TYPE_DSA;
}

static inline bool dsa_is_user_port(struct dsa_switch *ds, int p)
{
	return dsa_to_port(ds, p)->type == DSA_PORT_TYPE_USER;
}

static inline u32 dsa_user_ports(struct dsa_switch *ds)
{
	u32 mask = 0;
	int p;

	for (p = 0; p < ds->num_ports; p++)
		if (dsa_is_user_port(ds, p))
			mask |= BIT(p);

	return mask;
}


static inline unsigned int dsa_towards_port(struct dsa_switch *ds, int device,
					    int port)
{
	if (device == ds->index)
		return port;
	else
		return ds->rtable[device];
}


static inline unsigned int dsa_upstream_port(struct dsa_switch *ds, int port)
{
	const struct dsa_port *dp = dsa_to_port(ds, port);
	const struct dsa_port *cpu_dp = dp->cpu_dp;

	if (!cpu_dp)
		return port;

	return dsa_towards_port(ds, cpu_dp->ds->index, cpu_dp->index);
}

typedef int dsa_fdb_dump_cb_t(const unsigned char *addr, u16 vid,
			      bool is_static, void *data);
struct dsa_switch_ops {
#if IS_ENABLED(CONFIG_NET_DSA_LEGACY)
	
	const char	*(*probe)(struct device *dsa_dev,
				  struct device *host_dev, int sw_addr,
				  void **priv);
#endif

	enum dsa_tag_protocol (*get_tag_protocol)(struct dsa_switch *ds,
						  int port);

	int	(*setup)(struct dsa_switch *ds);
	u32	(*get_phy_flags)(struct dsa_switch *ds, int port);

	
	int	(*phy_read)(struct dsa_switch *ds, int port, int regnum);
	int	(*phy_write)(struct dsa_switch *ds, int port,
			     int regnum, u16 val);

	
	void	(*adjust_link)(struct dsa_switch *ds, int port,
				struct phy_device *phydev);
	void	(*fixed_link_update)(struct dsa_switch *ds, int port,
				struct fixed_phy_status *st);

	
	void	(*phylink_validate)(struct dsa_switch *ds, int port,
				    unsigned long *supported,
				    struct phylink_link_state *state);
	int	(*phylink_mac_link_state)(struct dsa_switch *ds, int port,
					  struct phylink_link_state *state);
	void	(*phylink_mac_config)(struct dsa_switch *ds, int port,
				      unsigned int mode,
				      const struct phylink_link_state *state);
	void	(*phylink_mac_an_restart)(struct dsa_switch *ds, int port);
	void	(*phylink_mac_link_down)(struct dsa_switch *ds, int port,
					 unsigned int mode,
					 phy_interface_t interface);
	void	(*phylink_mac_link_up)(struct dsa_switch *ds, int port,
				       unsigned int mode,
				       phy_interface_t interface,
				       struct phy_device *phydev);
	void	(*phylink_fixed_state)(struct dsa_switch *ds, int port,
				       struct phylink_link_state *state);
	
	void	(*get_strings)(struct dsa_switch *ds, int port,
			       u32 stringset, uint8_t *data);
	void	(*get_ethtool_stats)(struct dsa_switch *ds,
				     int port, uint64_t *data);
	int	(*get_sset_count)(struct dsa_switch *ds, int port, int sset);
	void	(*get_ethtool_phy_stats)(struct dsa_switch *ds,
					 int port, uint64_t *data);

	
	void	(*get_wol)(struct dsa_switch *ds, int port,
			   struct ethtool_wolinfo *w);
	int	(*set_wol)(struct dsa_switch *ds, int port,
			   struct ethtool_wolinfo *w);

	
	int	(*get_ts_info)(struct dsa_switch *ds, int port,
			       struct ethtool_ts_info *ts);

	
	int	(*suspend)(struct dsa_switch *ds);
	int	(*resume)(struct dsa_switch *ds);

	
	int	(*port_enable)(struct dsa_switch *ds, int port,
			       struct phy_device *phy);
	void	(*port_disable)(struct dsa_switch *ds, int port,
				struct phy_device *phy);

	
	int	(*set_mac_eee)(struct dsa_switch *ds, int port,
			       struct ethtool_eee *e);
	int	(*get_mac_eee)(struct dsa_switch *ds, int port,
			       struct ethtool_eee *e);

	
	int	(*get_eeprom_len)(struct dsa_switch *ds);
	int	(*get_eeprom)(struct dsa_switch *ds,
			      struct ethtool_eeprom *eeprom, u8 *data);
	int	(*set_eeprom)(struct dsa_switch *ds,
			      struct ethtool_eeprom *eeprom, u8 *data);

	
	int	(*get_regs_len)(struct dsa_switch *ds, int port);
	void	(*get_regs)(struct dsa_switch *ds, int port,
			    struct ethtool_regs *regs, void *p);

	
	int	(*set_ageing_time)(struct dsa_switch *ds, unsigned int msecs);
	int	(*port_bridge_join)(struct dsa_switch *ds, int port,
				    struct net_device *bridge);
	void	(*port_bridge_leave)(struct dsa_switch *ds, int port,
				     struct net_device *bridge);
	void	(*port_stp_state_set)(struct dsa_switch *ds, int port,
				      u8 state);
	void	(*port_fast_age)(struct dsa_switch *ds, int port);

	
	int	(*port_vlan_filtering)(struct dsa_switch *ds, int port,
				       bool vlan_filtering);
	int (*port_vlan_prepare)(struct dsa_switch *ds, int port,
				 const struct switchdev_obj_port_vlan *vlan);
	void (*port_vlan_add)(struct dsa_switch *ds, int port,
			      const struct switchdev_obj_port_vlan *vlan);
	int	(*port_vlan_del)(struct dsa_switch *ds, int port,
				 const struct switchdev_obj_port_vlan *vlan);
	
	int	(*port_fdb_add)(struct dsa_switch *ds, int port,
				const unsigned char *addr, u16 vid);
	int	(*port_fdb_del)(struct dsa_switch *ds, int port,
				const unsigned char *addr, u16 vid);
	int	(*port_fdb_dump)(struct dsa_switch *ds, int port,
				 dsa_fdb_dump_cb_t *cb, void *data);

	
	int (*port_mdb_prepare)(struct dsa_switch *ds, int port,
				const struct switchdev_obj_port_mdb *mdb);
	void (*port_mdb_add)(struct dsa_switch *ds, int port,
			     const struct switchdev_obj_port_mdb *mdb);
	int	(*port_mdb_del)(struct dsa_switch *ds, int port,
				const struct switchdev_obj_port_mdb *mdb);
	
	int	(*get_rxnfc)(struct dsa_switch *ds, int port,
			     struct ethtool_rxnfc *nfc, u32 *rule_locs);
	int	(*set_rxnfc)(struct dsa_switch *ds, int port,
			     struct ethtool_rxnfc *nfc);

	
	int	(*port_mirror_add)(struct dsa_switch *ds, int port,
				   struct dsa_mall_mirror_tc_entry *mirror,
				   bool ingress);
	void	(*port_mirror_del)(struct dsa_switch *ds, int port,
				   struct dsa_mall_mirror_tc_entry *mirror);

	
	int	(*crosschip_bridge_join)(struct dsa_switch *ds, int sw_index,
					 int port, struct net_device *br);
	void	(*crosschip_bridge_leave)(struct dsa_switch *ds, int sw_index,
					  int port, struct net_device *br);

	
	int	(*port_hwtstamp_get)(struct dsa_switch *ds, int port,
				     struct ifreq *ifr);
	int	(*port_hwtstamp_set)(struct dsa_switch *ds, int port,
				     struct ifreq *ifr);
	bool	(*port_txtstamp)(struct dsa_switch *ds, int port,
				 struct sk_buff *clone, unsigned int type);
	bool	(*port_rxtstamp)(struct dsa_switch *ds, int port,
				 struct sk_buff *skb, unsigned int type);
};

struct dsa_switch_driver {
	struct list_head	list;
	const struct dsa_switch_ops *ops;
};

#if IS_ENABLED(CONFIG_NET_DSA_LEGACY)

void register_switch_driver(struct dsa_switch_driver *type);
void unregister_switch_driver(struct dsa_switch_driver *type);
struct mii_bus *dsa_host_dev_to_mii_bus(struct device *dev);

#else
static inline void register_switch_driver(struct dsa_switch_driver *type) { }
static inline void unregister_switch_driver(struct dsa_switch_driver *type) { }
static inline struct mii_bus *dsa_host_dev_to_mii_bus(struct device *dev)
{
	return NULL;
}
#endif
struct net_device *dsa_dev_to_net_device(struct device *dev);


static inline bool netdev_uses_dsa(struct net_device *dev)
{
#if IS_ENABLED(CONFIG_NET_DSA)
	return dev->dsa_ptr && dev->dsa_ptr->rcv;
#endif
	return false;
}

struct dsa_switch *dsa_switch_alloc(struct device *dev, size_t n);
void dsa_unregister_switch(struct dsa_switch *ds);
int dsa_register_switch(struct dsa_switch *ds);
#ifdef CONFIG_PM_SLEEP
int dsa_switch_suspend(struct dsa_switch *ds);
int dsa_switch_resume(struct dsa_switch *ds);
#else
static inline int dsa_switch_suspend(struct dsa_switch *ds)
{
	return 0;
}
static inline int dsa_switch_resume(struct dsa_switch *ds)
{
	return 0;
}
#endif 

enum dsa_notifier_type {
	DSA_PORT_REGISTER,
	DSA_PORT_UNREGISTER,
};

struct dsa_notifier_info {
	struct net_device *dev;
};

struct dsa_notifier_register_info {
	struct dsa_notifier_info info;	
	struct net_device *master;
	unsigned int port_number;
	unsigned int switch_number;
};

static inline struct net_device *
dsa_notifier_info_to_dev(const struct dsa_notifier_info *info)
{
	return info->dev;
}

#if IS_ENABLED(CONFIG_NET_DSA)
int register_dsa_notifier(struct notifier_block *nb);
int unregister_dsa_notifier(struct notifier_block *nb);
int call_dsa_notifiers(unsigned long val, struct net_device *dev,
		       struct dsa_notifier_info *info);
#else
static inline int register_dsa_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int unregister_dsa_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int call_dsa_notifiers(unsigned long val, struct net_device *dev,
				     struct dsa_notifier_info *info)
{
	return NOTIFY_DONE;
}
#endif


#define BRCM_TAG_SET_PORT_QUEUE(p, q)	((p) << 8 | q)
#define BRCM_TAG_GET_PORT(v)		((v) >> 8)
#define BRCM_TAG_GET_QUEUE(v)		((v) & 0xff)


int dsa_port_get_phy_strings(struct dsa_port *dp, uint8_t *data);
int dsa_port_get_ethtool_phy_stats(struct dsa_port *dp, uint64_t *data);
int dsa_port_get_phy_sset_count(struct dsa_port *dp);
void dsa_port_phylink_mac_change(struct dsa_switch *ds, int port, bool up);

#endif
