/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef __LINUX_PCI_EP_CFS_H
#define __LINUX_PCI_EP_CFS_H

#include <linux/configfs.h>

#ifdef CONFIG_PCI_ENDPOINT_CONFIGFS
struct config_group *pci_ep_cfs_add_epc_group(const char *name);
void pci_ep_cfs_remove_epc_group(struct config_group *group);
struct config_group *pci_ep_cfs_add_epf_group(const char *name);
void pci_ep_cfs_remove_epf_group(struct config_group *group);
#else
static inline struct config_group *pci_ep_cfs_add_epc_group(const char *name)
{
	return 0;
}

static inline void pci_ep_cfs_remove_epc_group(struct config_group *group)
{
}

static inline struct config_group *pci_ep_cfs_add_epf_group(const char *name)
{
	return 0;
}

static inline void pci_ep_cfs_remove_epf_group(struct config_group *group)
{
}
#endif
#endif 
