/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __LINUX_OF_CLK_H
#define __LINUX_OF_CLK_H

#if defined(CONFIG_COMMON_CLK) && defined(CONFIG_OF)

unsigned int of_clk_get_parent_count(struct device_node *np);
const char *of_clk_get_parent_name(struct device_node *np, int index);
void of_clk_init(const struct of_device_id *matches);

#else 

static inline unsigned int of_clk_get_parent_count(struct device_node *np)
{
	return 0;
}
static inline const char *of_clk_get_parent_name(struct device_node *np,
						 int index)
{
	return NULL;
}
static inline void of_clk_init(const struct of_device_id *matches) {}

#endif 

#endif 
