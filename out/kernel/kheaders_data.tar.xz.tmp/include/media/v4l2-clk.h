

#ifndef MEDIA_V4L2_CLK_H
#define MEDIA_V4L2_CLK_H

#include <linux/atomic.h>
#include <linux/export.h>
#include <linux/list.h>
#include <linux/mutex.h>

struct module;
struct device;

struct clk;
struct v4l2_clk {
	struct list_head list;
	const struct v4l2_clk_ops *ops;
	const char *dev_id;
	int enable;
	struct mutex lock; 
	atomic_t use_count;
	struct clk *clk;
	void *priv;
};

struct v4l2_clk_ops {
	struct module	*owner;
	int		(*enable)(struct v4l2_clk *clk);
	void		(*disable)(struct v4l2_clk *clk);
	unsigned long	(*get_rate)(struct v4l2_clk *clk);
	int		(*set_rate)(struct v4l2_clk *clk, unsigned long);
};

struct v4l2_clk *v4l2_clk_register(const struct v4l2_clk_ops *ops,
				   const char *dev_name,
				   void *priv);
void v4l2_clk_unregister(struct v4l2_clk *clk);
struct v4l2_clk *v4l2_clk_get(struct device *dev, const char *id);
void v4l2_clk_put(struct v4l2_clk *clk);
int v4l2_clk_enable(struct v4l2_clk *clk);
void v4l2_clk_disable(struct v4l2_clk *clk);
unsigned long v4l2_clk_get_rate(struct v4l2_clk *clk);
int v4l2_clk_set_rate(struct v4l2_clk *clk, unsigned long rate);

struct module;

struct v4l2_clk *__v4l2_clk_register_fixed(const char *dev_id,
			unsigned long rate, struct module *owner);
void v4l2_clk_unregister_fixed(struct v4l2_clk *clk);

static inline struct v4l2_clk *v4l2_clk_register_fixed(const char *dev_id,
							unsigned long rate)
{
	return __v4l2_clk_register_fixed(dev_id, rate, THIS_MODULE);
}

#define V4L2_CLK_NAME_SIZE 64

#define v4l2_clk_name_i2c(name, size, adap, client) snprintf(name, size, \
			  "%d-%04x", adap, client)

#define v4l2_clk_name_of(name, size, node) snprintf(name, size, \
			  "of-%pOF", node)

#endif
