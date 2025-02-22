
#ifndef __LINUX_PINCTRL_CONSUMER_H
#define __LINUX_PINCTRL_CONSUMER_H

#include <linux/err.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/pinctrl/pinctrl-state.h>


struct pinctrl;
struct pinctrl_state;
struct device;

#ifdef CONFIG_PINCTRL


extern int pinctrl_gpio_request(unsigned gpio);
extern void pinctrl_gpio_free(unsigned gpio);
extern int pinctrl_gpio_direction_input(unsigned gpio);
extern int pinctrl_gpio_direction_output(unsigned gpio);
extern int pinctrl_gpio_set_config(unsigned gpio, unsigned long config);

extern struct pinctrl * __must_check pinctrl_get(struct device *dev);
extern void pinctrl_put(struct pinctrl *p);
extern struct pinctrl_state * __must_check pinctrl_lookup_state(
							struct pinctrl *p,
							const char *name);
extern int pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *s);

extern struct pinctrl * __must_check devm_pinctrl_get(struct device *dev);
extern void devm_pinctrl_put(struct pinctrl *p);

#ifdef CONFIG_PM
extern int pinctrl_pm_select_default_state(struct device *dev);
extern int pinctrl_pm_select_sleep_state(struct device *dev);
extern int pinctrl_pm_select_idle_state(struct device *dev);
#else
static inline int pinctrl_pm_select_default_state(struct device *dev)
{
	return 0;
}
static inline int pinctrl_pm_select_sleep_state(struct device *dev)
{
	return 0;
}
static inline int pinctrl_pm_select_idle_state(struct device *dev)
{
	return 0;
}
#endif

#else 

static inline int pinctrl_gpio_request(unsigned gpio)
{
	return 0;
}

static inline void pinctrl_gpio_free(unsigned gpio)
{
}

static inline int pinctrl_gpio_direction_input(unsigned gpio)
{
	return 0;
}

static inline int pinctrl_gpio_direction_output(unsigned gpio)
{
	return 0;
}

static inline int pinctrl_gpio_set_config(unsigned gpio, unsigned long config)
{
	return 0;
}

static inline struct pinctrl * __must_check pinctrl_get(struct device *dev)
{
	return NULL;
}

static inline void pinctrl_put(struct pinctrl *p)
{
}

static inline struct pinctrl_state * __must_check pinctrl_lookup_state(
							struct pinctrl *p,
							const char *name)
{
	return NULL;
}

static inline int pinctrl_select_state(struct pinctrl *p,
				       struct pinctrl_state *s)
{
	return 0;
}

static inline struct pinctrl * __must_check devm_pinctrl_get(struct device *dev)
{
	return NULL;
}

static inline void devm_pinctrl_put(struct pinctrl *p)
{
}

static inline int pinctrl_pm_select_default_state(struct device *dev)
{
	return 0;
}

static inline int pinctrl_pm_select_sleep_state(struct device *dev)
{
	return 0;
}

static inline int pinctrl_pm_select_idle_state(struct device *dev)
{
	return 0;
}

#endif 

static inline struct pinctrl * __must_check pinctrl_get_select(
					struct device *dev, const char *name)
{
	struct pinctrl *p;
	struct pinctrl_state *s;
	int ret;

	p = pinctrl_get(dev);
	if (IS_ERR(p))
		return p;

	s = pinctrl_lookup_state(p, name);
	if (IS_ERR(s)) {
		pinctrl_put(p);
		return ERR_CAST(s);
	}

	ret = pinctrl_select_state(p, s);
	if (ret < 0) {
		pinctrl_put(p);
		return ERR_PTR(ret);
	}

	return p;
}

static inline struct pinctrl * __must_check pinctrl_get_select_default(
					struct device *dev)
{
	return pinctrl_get_select(dev, PINCTRL_STATE_DEFAULT);
}

static inline struct pinctrl * __must_check devm_pinctrl_get_select(
					struct device *dev, const char *name)
{
	struct pinctrl *p;
	struct pinctrl_state *s;
	int ret;

	p = devm_pinctrl_get(dev);
	if (IS_ERR(p))
		return p;

	s = pinctrl_lookup_state(p, name);
	if (IS_ERR(s)) {
		devm_pinctrl_put(p);
		return ERR_CAST(s);
	}

	ret = pinctrl_select_state(p, s);
	if (ret < 0) {
		devm_pinctrl_put(p);
		return ERR_PTR(ret);
	}

	return p;
}

static inline struct pinctrl * __must_check devm_pinctrl_get_select_default(
					struct device *dev)
{
	return devm_pinctrl_get_select(dev, PINCTRL_STATE_DEFAULT);
}

#endif 
