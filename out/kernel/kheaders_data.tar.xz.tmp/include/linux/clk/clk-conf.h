

#include <linux/types.h>

struct device_node;

#if defined(CONFIG_OF) && defined(CONFIG_COMMON_CLK)
int of_clk_set_defaults(struct device_node *node, bool clk_supplier);
#else
static inline int of_clk_set_defaults(struct device_node *node,
				      bool clk_supplier)
{
	return 0;
}
#endif
