

#ifndef __LINUX_MFD_SYSCON_H__
#define __LINUX_MFD_SYSCON_H__

#include <linux/err.h>
#include <linux/errno.h>

struct device_node;

#ifdef CONFIG_MFD_SYSCON
extern struct regmap *syscon_node_to_regmap(struct device_node *np);
extern struct regmap *syscon_regmap_lookup_by_compatible(const char *s);
extern struct regmap *syscon_regmap_lookup_by_pdevname(const char *s);
extern struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property);
#else
static inline struct regmap *syscon_node_to_regmap(struct device_node *np)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_compatible(const char *s)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_pdevname(const char *s)
{
	return ERR_PTR(-ENOTSUPP);
}

static inline struct regmap *syscon_regmap_lookup_by_phandle(
					struct device_node *np,
					const char *property)
{
	return ERR_PTR(-ENOTSUPP);
}
#endif

#endif 
