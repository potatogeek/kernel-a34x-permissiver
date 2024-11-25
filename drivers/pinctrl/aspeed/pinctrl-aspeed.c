<<<<<<< HEAD
/*
 * Copyright (C) 2016 IBM Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2016 IBM Corp.
>>>>>>> upstream/android-13
 */

#include <linux/mfd/syscon.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "../core.h"
#include "pinctrl-aspeed.h"

<<<<<<< HEAD
static const char *const aspeed_pinmux_ips[] = {
	[ASPEED_IP_SCU] = "SCU",
	[ASPEED_IP_GFX] = "GFX",
	[ASPEED_IP_LPC] = "LPC",
};

=======
>>>>>>> upstream/android-13
int aspeed_pinctrl_get_groups_count(struct pinctrl_dev *pctldev)
{
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);

<<<<<<< HEAD
	return pdata->ngroups;
=======
	return pdata->pinmux.ngroups;
>>>>>>> upstream/android-13
}

const char *aspeed_pinctrl_get_group_name(struct pinctrl_dev *pctldev,
		unsigned int group)
{
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);

<<<<<<< HEAD
	return pdata->groups[group].name;
=======
	return pdata->pinmux.groups[group].name;
>>>>>>> upstream/android-13
}

int aspeed_pinctrl_get_group_pins(struct pinctrl_dev *pctldev,
				  unsigned int group, const unsigned int **pins,
				  unsigned int *npins)
{
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);

<<<<<<< HEAD
	*pins = &pdata->groups[group].pins[0];
	*npins = pdata->groups[group].npins;
=======
	*pins = &pdata->pinmux.groups[group].pins[0];
	*npins = pdata->pinmux.groups[group].npins;
>>>>>>> upstream/android-13

	return 0;
}

void aspeed_pinctrl_pin_dbg_show(struct pinctrl_dev *pctldev,
				 struct seq_file *s, unsigned int offset)
{
	seq_printf(s, " %s", dev_name(pctldev->dev));
}

int aspeed_pinmux_get_fn_count(struct pinctrl_dev *pctldev)
{
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);

<<<<<<< HEAD
	return pdata->nfunctions;
=======
	return pdata->pinmux.nfunctions;
>>>>>>> upstream/android-13
}

const char *aspeed_pinmux_get_fn_name(struct pinctrl_dev *pctldev,
				      unsigned int function)
{
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);

<<<<<<< HEAD
	return pdata->functions[function].name;
=======
	return pdata->pinmux.functions[function].name;
>>>>>>> upstream/android-13
}

int aspeed_pinmux_get_fn_groups(struct pinctrl_dev *pctldev,
				unsigned int function,
				const char * const **groups,
				unsigned int * const num_groups)
{
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);

<<<<<<< HEAD
	*groups = pdata->functions[function].groups;
	*num_groups = pdata->functions[function].ngroups;
=======
	*groups = pdata->pinmux.functions[function].groups;
	*num_groups = pdata->pinmux.functions[function].ngroups;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static inline void aspeed_sig_desc_print_val(
		const struct aspeed_sig_desc *desc, bool enable, u32 rv)
{
	pr_debug("Want %s%X[0x%08X]=0x%X, got 0x%X from 0x%08X\n",
			aspeed_pinmux_ips[desc->ip], desc->reg,
			desc->mask, enable ? desc->enable : desc->disable,
			(rv & desc->mask) >> __ffs(desc->mask), rv);
}

/**
 * Query the enabled or disabled state of a signal descriptor
 *
 * @desc: The signal descriptor of interest
 * @enabled: True to query the enabled state, false to query disabled state
 * @map: The IP block's regmap instance
 *
 * Return: 1 if the descriptor's bitfield is configured to the state
 * selected by @enabled, 0 if not, and less than zero if an unrecoverable
 * failure occurred
 *
 * Evaluation of descriptor state is non-trivial in that it is not a binary
 * outcome: The bitfields can be greater than one bit in size and thus can take
 * a value that is neither the enabled nor disabled state recorded in the
 * descriptor (typically this means a different function to the one of interest
 * is enabled). Thus we must explicitly test for either condition as required.
 */
static int aspeed_sig_desc_eval(const struct aspeed_sig_desc *desc,
				 bool enabled, struct regmap *map)
{
	int ret;
	unsigned int raw;
	u32 want;

	if (!map)
		return -ENODEV;

	ret = regmap_read(map, desc->reg, &raw);
	if (ret)
		return ret;

	aspeed_sig_desc_print_val(desc, enabled, raw);
	want = enabled ? desc->enable : desc->disable;

	return ((raw & desc->mask) >> __ffs(desc->mask)) == want;
}

/**
 * Query the enabled or disabled state for a mux function's signal on a pin
 *
 * @expr: An expression controlling the signal for a mux function on a pin
 * @enabled: True to query the enabled state, false to query disabled state
 * @maps: The list of regmap instances
 *
 * Return: 1 if the expression composed by @enabled evaluates true, 0 if not,
 * and less than zero if an unrecoverable failure occurred.
 *
 * A mux function is enabled or disabled if the function's signal expression
 * for each pin in the function's pin group evaluates true for the desired
 * state. An signal expression evaluates true if all of its associated signal
 * descriptors evaluate true for the desired state.
 *
 * If an expression's state is described by more than one bit, either through
 * multi-bit bitfields in a single signal descriptor or through multiple signal
 * descriptors of a single bit then it is possible for the expression to be in
 * neither the enabled nor disabled state. Thus we must explicitly test for
 * either condition as required.
 */
static int aspeed_sig_expr_eval(const struct aspeed_sig_expr *expr,
				 bool enabled, struct regmap * const *maps)
{
	int i;
	int ret;

	for (i = 0; i < expr->ndescs; i++) {
		const struct aspeed_sig_desc *desc = &expr->descs[i];

		ret = aspeed_sig_desc_eval(desc, enabled, maps[desc->ip]);
		if (ret <= 0)
			return ret;
	}

	return 1;
}

/**
 * Configure a pin's signal by applying an expression's descriptor state for
 * all descriptors in the expression.
 *
 * @expr: The expression associated with the function whose signal is to be
 *        configured
 * @enable: true to enable an function's signal through a pin's signal
 *          expression, false to disable the function's signal
 * @maps: The list of regmap instances for pinmux register access.
 *
 * Return: 0 if the expression is configured as requested and a negative error
 * code otherwise
 */
static int aspeed_sig_expr_set(const struct aspeed_sig_expr *expr,
				bool enable, struct regmap * const *maps)
{
	int ret;
	int i;

	for (i = 0; i < expr->ndescs; i++) {
		const struct aspeed_sig_desc *desc = &expr->descs[i];
		u32 pattern = enable ? desc->enable : desc->disable;
		u32 val = (pattern << __ffs(desc->mask));

		if (!maps[desc->ip])
			return -ENODEV;

		/*
		 * Strap registers are configured in hardware or by early-boot
		 * firmware. Treat them as read-only despite that we can write
		 * them. This may mean that certain functions cannot be
		 * deconfigured and is the reason we re-evaluate after writing
		 * all descriptor bits.
		 *
		 * Port D and port E GPIO loopback modes are the only exception
		 * as those are commonly used with front-panel buttons to allow
		 * normal operation of the host when the BMC is powered off or
		 * fails to boot. Once the BMC has booted, the loopback mode
		 * must be disabled for the BMC to control host power-on and
		 * reset.
		 */
		if (desc->ip == ASPEED_IP_SCU && desc->reg == HW_STRAP1 &&
		    !(desc->mask & (BIT(21) | BIT(22))))
			continue;

		if (desc->ip == ASPEED_IP_SCU && desc->reg == HW_STRAP2)
			continue;

		/* On AST2500, Set bits in SCU7C are cleared from SCU70 */
		if (desc->ip == ASPEED_IP_SCU && desc->reg == HW_STRAP1) {
			unsigned int rev_id;

			ret = regmap_read(maps[ASPEED_IP_SCU],
				HW_REVISION_ID, &rev_id);
			if (ret < 0)
				return ret;

			if (0x04 == (rev_id >> 24)) {
				u32 value = ~val & desc->mask;

				if (value) {
					ret = regmap_write(maps[desc->ip],
						HW_REVISION_ID, value);
					if (ret < 0)
						return ret;
				}
			}
		}

		ret = regmap_update_bits(maps[desc->ip], desc->reg,
					 desc->mask, val);

		if (ret)
			return ret;
	}

	ret = aspeed_sig_expr_eval(expr, enable, maps);
=======
static int aspeed_sig_expr_enable(struct aspeed_pinmux_data *ctx,
				  const struct aspeed_sig_expr *expr)
{
	int ret;

	pr_debug("Enabling signal %s for %s\n", expr->signal,
		 expr->function);

	ret = aspeed_sig_expr_eval(ctx, expr, true);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	if (!ret)
<<<<<<< HEAD
		return -EPERM;
=======
		return aspeed_sig_expr_set(ctx, expr, true);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int aspeed_sig_expr_enable(const struct aspeed_sig_expr *expr,
				   struct regmap * const *maps)
{
	int ret;

	ret = aspeed_sig_expr_eval(expr, true, maps);
	if (ret < 0)
		return ret;

	if (!ret)
		return aspeed_sig_expr_set(expr, true, maps);

	return 0;
}

static int aspeed_sig_expr_disable(const struct aspeed_sig_expr *expr,
				    struct regmap * const *maps)
{
	int ret;

	ret = aspeed_sig_expr_eval(expr, true, maps);
=======
static int aspeed_sig_expr_disable(struct aspeed_pinmux_data *ctx,
				   const struct aspeed_sig_expr *expr)
{
	int ret;

	pr_debug("Disabling signal %s for %s\n", expr->signal,
		 expr->function);

	ret = aspeed_sig_expr_eval(ctx, expr, true);
>>>>>>> upstream/android-13
	if (ret < 0)
		return ret;

	if (ret)
<<<<<<< HEAD
		return aspeed_sig_expr_set(expr, false, maps);
=======
		return aspeed_sig_expr_set(ctx, expr, false);
>>>>>>> upstream/android-13

	return 0;
}

/**
<<<<<<< HEAD
 * Disable a signal on a pin by disabling all provided signal expressions.
 *
 * @exprs: The list of signal expressions (from a priority level on a pin)
 * @maps: The list of regmap instances for pinmux register access.
 *
 * Return: 0 if all expressions are disabled, otherwise a negative error code
 */
static int aspeed_disable_sig(const struct aspeed_sig_expr **exprs,
			       struct regmap * const *maps)
=======
 * aspeed_disable_sig() - Disable a signal on a pin by disabling all provided
 * signal expressions.
 *
 * @ctx: The pinmux context
 * @exprs: The list of signal expressions (from a priority level on a pin)
 *
 * Return: 0 if all expressions are disabled, otherwise a negative error code
 */
static int aspeed_disable_sig(struct aspeed_pinmux_data *ctx,
			      const struct aspeed_sig_expr **exprs)
>>>>>>> upstream/android-13
{
	int ret = 0;

	if (!exprs)
		return true;

	while (*exprs && !ret) {
<<<<<<< HEAD
		ret = aspeed_sig_expr_disable(*exprs, maps);
=======
		ret = aspeed_sig_expr_disable(ctx, *exprs);
>>>>>>> upstream/android-13
		exprs++;
	}

	return ret;
}

/**
<<<<<<< HEAD
 * Search for the signal expression needed to enable the pin's signal for the
 * requested function.
=======
 * aspeed_find_expr_by_name - Search for the signal expression needed to
 * enable the pin's signal for the requested function.
>>>>>>> upstream/android-13
 *
 * @exprs: List of signal expressions (haystack)
 * @name: The name of the requested function (needle)
 *
 * Return: A pointer to the signal expression whose function tag matches the
 * provided name, otherwise NULL.
 *
 */
static const struct aspeed_sig_expr *aspeed_find_expr_by_name(
		const struct aspeed_sig_expr **exprs, const char *name)
{
	while (*exprs) {
		if (strcmp((*exprs)->function, name) == 0)
			return *exprs;
		exprs++;
	}

	return NULL;
}

static char *get_defined_attribute(const struct aspeed_pin_desc *pdesc,
				   const char *(*get)(
					   const struct aspeed_sig_expr *))
{
	char *found = NULL;
	size_t len = 0;
	const struct aspeed_sig_expr ***prios, **funcs, *expr;

	prios = pdesc->prios;

	while ((funcs = *prios)) {
		while ((expr = *funcs)) {
			const char *str = get(expr);
			size_t delta = strlen(str) + 2;
			char *expanded;

			expanded = krealloc(found, len + delta + 1, GFP_KERNEL);
			if (!expanded) {
				kfree(found);
				return expanded;
			}

			found = expanded;
			found[len] = '\0';
			len += delta;

			strcat(found, str);
			strcat(found, ", ");

			funcs++;
		}
		prios++;
	}

	if (len < 2) {
		kfree(found);
		return NULL;
	}

	found[len - 2] = '\0';

	return found;
}

static const char *aspeed_sig_expr_function(const struct aspeed_sig_expr *expr)
{
	return expr->function;
}

static char *get_defined_functions(const struct aspeed_pin_desc *pdesc)
{
	return get_defined_attribute(pdesc, aspeed_sig_expr_function);
}

static const char *aspeed_sig_expr_signal(const struct aspeed_sig_expr *expr)
{
	return expr->signal;
}

static char *get_defined_signals(const struct aspeed_pin_desc *pdesc)
{
	return get_defined_attribute(pdesc, aspeed_sig_expr_signal);
}

int aspeed_pinmux_set_mux(struct pinctrl_dev *pctldev, unsigned int function,
			  unsigned int group)
{
	int i;
	int ret;
<<<<<<< HEAD
	const struct aspeed_pinctrl_data *pdata =
		pinctrl_dev_get_drvdata(pctldev);
	const struct aspeed_pin_group *pgroup = &pdata->groups[group];
	const struct aspeed_pin_function *pfunc =
		&pdata->functions[function];
=======
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);
	const struct aspeed_pin_group *pgroup = &pdata->pinmux.groups[group];
	const struct aspeed_pin_function *pfunc =
		&pdata->pinmux.functions[function];
>>>>>>> upstream/android-13

	for (i = 0; i < pgroup->npins; i++) {
		int pin = pgroup->pins[i];
		const struct aspeed_pin_desc *pdesc = pdata->pins[pin].drv_data;
		const struct aspeed_sig_expr *expr = NULL;
		const struct aspeed_sig_expr **funcs;
		const struct aspeed_sig_expr ***prios;

<<<<<<< HEAD
		pr_debug("Muxing pin %d for %s\n", pin, pfunc->name);
=======
		pr_debug("Muxing pin %s for %s\n", pdesc->name, pfunc->name);
>>>>>>> upstream/android-13

		if (!pdesc)
			return -EINVAL;

		prios = pdesc->prios;

		if (!prios)
			continue;

		/* Disable functions at a higher priority than that requested */
		while ((funcs = *prios)) {
			expr = aspeed_find_expr_by_name(funcs, pfunc->name);

			if (expr)
				break;

<<<<<<< HEAD
			ret = aspeed_disable_sig(funcs, pdata->maps);
=======
			ret = aspeed_disable_sig(&pdata->pinmux, funcs);
>>>>>>> upstream/android-13
			if (ret)
				return ret;

			prios++;
		}

		if (!expr) {
			char *functions = get_defined_functions(pdesc);
			char *signals = get_defined_signals(pdesc);

			pr_warn("No function %s found on pin %s (%d). Found signal(s) %s for function(s) %s\n",
				pfunc->name, pdesc->name, pin, signals,
				functions);
			kfree(signals);
			kfree(functions);

			return -ENXIO;
		}

<<<<<<< HEAD
		ret = aspeed_sig_expr_enable(expr, pdata->maps);
		if (ret)
			return ret;
=======
		ret = aspeed_sig_expr_enable(&pdata->pinmux, expr);
		if (ret)
			return ret;

		pr_debug("Muxed pin %s as %s for %s\n", pdesc->name, expr->signal,
			 expr->function);
>>>>>>> upstream/android-13
	}

	return 0;
}

static bool aspeed_expr_is_gpio(const struct aspeed_sig_expr *expr)
{
	/*
<<<<<<< HEAD
	 * The signal type is GPIO if the signal name has "GPI" as a prefix.
	 * strncmp (rather than strcmp) is used to implement the prefix
	 * requirement.
	 *
	 * expr->signal might look like "GPIOB1" in the GPIO case.
	 * expr->signal might look like "GPIT0" in the GPI case.
	 */
	return strncmp(expr->signal, "GPI", 3) == 0;
=======
	 * We need to differentiate between GPIO and non-GPIO signals to
	 * implement the gpio_request_enable() interface. For better or worse
	 * the ASPEED pinctrl driver uses the expression names to determine
	 * whether an expression will mux a pin for GPIO.
	 *
	 * Generally we have the following - A GPIO such as B1 has:
	 *
	 *    - expr->signal set to "GPIOB1"
	 *    - expr->function set to "GPIOB1"
	 *
	 * Using this fact we can determine whether the provided expression is
	 * a GPIO expression by testing the signal name for the string prefix
	 * "GPIO".
	 *
	 * However, some GPIOs are input-only, and the ASPEED datasheets name
	 * them differently. An input-only GPIO such as T0 has:
	 *
	 *    - expr->signal set to "GPIT0"
	 *    - expr->function set to "GPIT0"
	 *
	 * It's tempting to generalise the prefix test from "GPIO" to "GPI" to
	 * account for both GPIOs and GPIs, but in doing so we run aground on
	 * another feature:
	 *
	 * Some pins in the ASPEED BMC SoCs have a "pass-through" GPIO
	 * function where the input state of one pin is replicated as the
	 * output state of another (as if they were shorted together - a mux
	 * configuration that is typically enabled by hardware strapping).
	 * This feature allows the BMC to pass e.g. power button state through
	 * to the host while the BMC is yet to boot, but take control of the
	 * button state once the BMC has booted by muxing each pin as a
	 * separate, pin-specific GPIO.
	 *
	 * Conceptually this pass-through mode is a form of GPIO and is named
	 * as such in the datasheets, e.g. "GPID0". This naming similarity
	 * trips us up with the simple GPI-prefixed-signal-name scheme
	 * discussed above, as the pass-through configuration is not what we
	 * want when muxing a pin as GPIO for the GPIO subsystem.
	 *
	 * On e.g. the AST2400, a pass-through function "GPID0" is grouped on
	 * balls A18 and D16, where we have:
	 *
	 *    For ball A18:
	 *    - expr->signal set to "GPID0IN"
	 *    - expr->function set to "GPID0"
	 *
	 *    For ball D16:
	 *    - expr->signal set to "GPID0OUT"
	 *    - expr->function set to "GPID0"
	 *
	 * By contrast, the pin-specific GPIO expressions for the same pins are
	 * as follows:
	 *
	 *    For ball A18:
	 *    - expr->signal looks like "GPIOD0"
	 *    - expr->function looks like "GPIOD0"
	 *
	 *    For ball D16:
	 *    - expr->signal looks like "GPIOD1"
	 *    - expr->function looks like "GPIOD1"
	 *
	 * Testing both the signal _and_ function names gives us the means
	 * differentiate the pass-through GPIO pinmux configuration from the
	 * pin-specific configuration that the GPIO subsystem is after: An
	 * expression is a pin-specific (non-pass-through) GPIO configuration
	 * if the signal prefix is "GPI" and the signal name matches the
	 * function name.
	 */
	return !strncmp(expr->signal, "GPI", 3) &&
			!strcmp(expr->signal, expr->function);
>>>>>>> upstream/android-13
}

static bool aspeed_gpio_in_exprs(const struct aspeed_sig_expr **exprs)
{
	if (!exprs)
		return false;

	while (*exprs) {
		if (aspeed_expr_is_gpio(*exprs))
			return true;
		exprs++;
	}

	return false;
}

int aspeed_gpio_request_enable(struct pinctrl_dev *pctldev,
			       struct pinctrl_gpio_range *range,
			       unsigned int offset)
{
	int ret;
<<<<<<< HEAD
	const struct aspeed_pinctrl_data *pdata =
		pinctrl_dev_get_drvdata(pctldev);
=======
	struct aspeed_pinctrl_data *pdata = pinctrl_dev_get_drvdata(pctldev);
>>>>>>> upstream/android-13
	const struct aspeed_pin_desc *pdesc = pdata->pins[offset].drv_data;
	const struct aspeed_sig_expr ***prios, **funcs, *expr;

	if (!pdesc)
		return -EINVAL;

	prios = pdesc->prios;

	if (!prios)
		return -ENXIO;

<<<<<<< HEAD
=======
	pr_debug("Muxing pin %s for GPIO\n", pdesc->name);

>>>>>>> upstream/android-13
	/* Disable any functions of higher priority than GPIO */
	while ((funcs = *prios)) {
		if (aspeed_gpio_in_exprs(funcs))
			break;

<<<<<<< HEAD
		ret = aspeed_disable_sig(funcs, pdata->maps);
=======
		ret = aspeed_disable_sig(&pdata->pinmux, funcs);
>>>>>>> upstream/android-13
		if (ret)
			return ret;

		prios++;
	}

	if (!funcs) {
		char *signals = get_defined_signals(pdesc);

		pr_warn("No GPIO signal type found on pin %s (%d). Found: %s\n",
			pdesc->name, offset, signals);
		kfree(signals);

		return -ENXIO;
	}

	expr = *funcs;

	/*
	 * Disabling all higher-priority expressions is enough to enable the
	 * lowest-priority signal type. As such it has no associated
	 * expression.
	 */
<<<<<<< HEAD
	if (!expr)
		return 0;
=======
	if (!expr) {
		pr_debug("Muxed pin %s as GPIO\n", pdesc->name);
		return 0;
	}
>>>>>>> upstream/android-13

	/*
	 * If GPIO is not the lowest priority signal type, assume there is only
	 * one expression defined to enable the GPIO function
	 */
<<<<<<< HEAD
	return aspeed_sig_expr_enable(expr, pdata->maps);
=======
	ret = aspeed_sig_expr_enable(&pdata->pinmux, expr);
	if (ret)
		return ret;

	pr_debug("Muxed pin %s as %s\n", pdesc->name, expr->signal);

	return 0;
>>>>>>> upstream/android-13
}

int aspeed_pinctrl_probe(struct platform_device *pdev,
			 struct pinctrl_desc *pdesc,
			 struct aspeed_pinctrl_data *pdata)
{
	struct device *parent;
	struct pinctrl_dev *pctl;

	parent = pdev->dev.parent;
	if (!parent) {
		dev_err(&pdev->dev, "No parent for syscon pincontroller\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	pdata->maps[ASPEED_IP_SCU] = syscon_node_to_regmap(parent->of_node);
	if (IS_ERR(pdata->maps[ASPEED_IP_SCU])) {
		dev_err(&pdev->dev, "No regmap for syscon pincontroller parent\n");
		return PTR_ERR(pdata->maps[ASPEED_IP_SCU]);
	}

=======
	pdata->scu = syscon_node_to_regmap(parent->of_node);
	if (IS_ERR(pdata->scu)) {
		dev_err(&pdev->dev, "No regmap for syscon pincontroller parent\n");
		return PTR_ERR(pdata->scu);
	}

	pdata->pinmux.maps[ASPEED_IP_SCU] = pdata->scu;

>>>>>>> upstream/android-13
	pctl = pinctrl_register(pdesc, &pdev->dev, pdata);

	if (IS_ERR(pctl)) {
		dev_err(&pdev->dev, "Failed to register pinctrl\n");
		return PTR_ERR(pctl);
	}

	platform_set_drvdata(pdev, pdata);

	return 0;
}

static inline bool pin_in_config_range(unsigned int offset,
		const struct aspeed_pin_config *config)
{
	return offset >= config->pins[0] && offset <= config->pins[1];
}

static inline const struct aspeed_pin_config *find_pinconf_config(
		const struct aspeed_pinctrl_data *pdata,
		unsigned int offset,
		enum pin_config_param param)
{
	unsigned int i;

	for (i = 0; i < pdata->nconfigs; i++) {
		if (param == pdata->configs[i].param &&
				pin_in_config_range(offset, &pdata->configs[i]))
			return &pdata->configs[i];
	}

	return NULL;
}

<<<<<<< HEAD
/**
 * @param: pinconf configuration parameter
 * @arg: The supported argument for @param, or -1 if any value is supported
 * @val: The register value to write to configure @arg for @param
 *
 * The map is to be used in conjunction with the configuration array supplied
 * by the driver implementation.
 */
struct aspeed_pin_config_map {
	enum pin_config_param param;
	s32 arg;
	u32 val;
};

enum aspeed_pin_config_map_type { MAP_TYPE_ARG, MAP_TYPE_VAL };

/* Aspeed consistently both:
 *
 * 1. Defines "disable bits" for internal pull-downs
 * 2. Uses 8mA or 16mA drive strengths
 */
static const struct aspeed_pin_config_map pin_config_map[] = {
	{ PIN_CONFIG_BIAS_PULL_DOWN,  0, 1 },
	{ PIN_CONFIG_BIAS_PULL_DOWN, -1, 0 },
	{ PIN_CONFIG_BIAS_DISABLE,   -1, 1 },
	{ PIN_CONFIG_DRIVE_STRENGTH,  8, 0 },
	{ PIN_CONFIG_DRIVE_STRENGTH, 16, 1 },
};

static const struct aspeed_pin_config_map *find_pinconf_map(
=======
enum aspeed_pin_config_map_type { MAP_TYPE_ARG, MAP_TYPE_VAL };

static const struct aspeed_pin_config_map *find_pinconf_map(
		const struct aspeed_pinctrl_data *pdata,
>>>>>>> upstream/android-13
		enum pin_config_param param,
		enum aspeed_pin_config_map_type type,
		s64 value)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(pin_config_map); i++) {
		const struct aspeed_pin_config_map *elem;
		bool match;

		elem = &pin_config_map[i];
=======
	for (i = 0; i < pdata->nconfmaps; i++) {
		const struct aspeed_pin_config_map *elem;
		bool match;

		elem = &pdata->confmaps[i];
>>>>>>> upstream/android-13

		switch (type) {
		case MAP_TYPE_ARG:
			match = (elem->arg == -1 || elem->arg == value);
			break;
		case MAP_TYPE_VAL:
			match = (elem->val == value);
			break;
		}

		if (param == elem->param && match)
			return elem;
	}

	return NULL;
}

int aspeed_pin_config_get(struct pinctrl_dev *pctldev, unsigned int offset,
		unsigned long *config)
{
	const enum pin_config_param param = pinconf_to_config_param(*config);
	const struct aspeed_pin_config_map *pmap;
	const struct aspeed_pinctrl_data *pdata;
	const struct aspeed_pin_config *pconf;
	unsigned int val;
	int rc = 0;
	u32 arg;

	pdata = pinctrl_dev_get_drvdata(pctldev);
	pconf = find_pinconf_config(pdata, offset, param);
	if (!pconf)
		return -ENOTSUPP;

<<<<<<< HEAD
	rc = regmap_read(pdata->maps[ASPEED_IP_SCU], pconf->reg, &val);
	if (rc < 0)
		return rc;

	pmap = find_pinconf_map(param, MAP_TYPE_VAL,
			(val & BIT(pconf->bit)) >> pconf->bit);
=======
	rc = regmap_read(pdata->scu, pconf->reg, &val);
	if (rc < 0)
		return rc;

	pmap = find_pinconf_map(pdata, param, MAP_TYPE_VAL,
			(val & pconf->mask) >> __ffs(pconf->mask));
>>>>>>> upstream/android-13

	if (!pmap)
		return -EINVAL;

	if (param == PIN_CONFIG_DRIVE_STRENGTH)
		arg = (u32) pmap->arg;
	else if (param == PIN_CONFIG_BIAS_PULL_DOWN)
		arg = !!pmap->arg;
	else
		arg = 1;

	if (!arg)
		return -EINVAL;

	*config = pinconf_to_config_packed(param, arg);

	return 0;
}

int aspeed_pin_config_set(struct pinctrl_dev *pctldev, unsigned int offset,
		unsigned long *configs, unsigned int num_configs)
{
	const struct aspeed_pinctrl_data *pdata;
	unsigned int i;
	int rc = 0;

	pdata = pinctrl_dev_get_drvdata(pctldev);

	for (i = 0; i < num_configs; i++) {
		const struct aspeed_pin_config_map *pmap;
		const struct aspeed_pin_config *pconf;
		enum pin_config_param param;
		unsigned int val;
		u32 arg;

		param = pinconf_to_config_param(configs[i]);
		arg = pinconf_to_config_argument(configs[i]);

		pconf = find_pinconf_config(pdata, offset, param);
		if (!pconf)
			return -ENOTSUPP;

<<<<<<< HEAD
		pmap = find_pinconf_map(param, MAP_TYPE_ARG, arg);

		if (unlikely(WARN_ON(!pmap)))
			return -EINVAL;

		val = pmap->val << pconf->bit;

		rc = regmap_update_bits(pdata->maps[ASPEED_IP_SCU], pconf->reg,
				BIT(pconf->bit), val);
=======
		pmap = find_pinconf_map(pdata, param, MAP_TYPE_ARG, arg);

		if (WARN_ON(!pmap))
			return -EINVAL;

		val = pmap->val << __ffs(pconf->mask);

		rc = regmap_update_bits(pdata->scu, pconf->reg,
					pconf->mask, val);
>>>>>>> upstream/android-13

		if (rc < 0)
			return rc;

<<<<<<< HEAD
		pr_debug("%s: Set SCU%02X[%d]=%d for param %d(=%d) on pin %d\n",
				__func__, pconf->reg, pconf->bit, pmap->val,
				param, arg, offset);
=======
		pr_debug("%s: Set SCU%02X[0x%08X]=0x%X for param %d(=%d) on pin %d\n",
				__func__, pconf->reg, pconf->mask,
				val, param, arg, offset);
>>>>>>> upstream/android-13
	}

	return 0;
}

int aspeed_pin_config_group_get(struct pinctrl_dev *pctldev,
		unsigned int selector,
		unsigned long *config)
{
	const unsigned int *pins;
	unsigned int npins;
	int rc;

	rc = aspeed_pinctrl_get_group_pins(pctldev, selector, &pins, &npins);
	if (rc < 0)
		return rc;

	if (!npins)
		return -ENODEV;

	rc = aspeed_pin_config_get(pctldev, pins[0], config);

	return rc;
}

int aspeed_pin_config_group_set(struct pinctrl_dev *pctldev,
		unsigned int selector,
		unsigned long *configs,
		unsigned int num_configs)
{
	const unsigned int *pins;
	unsigned int npins;
	int rc;
	int i;

	pr_debug("%s: Fetching pins for group selector %d\n",
			__func__, selector);
	rc = aspeed_pinctrl_get_group_pins(pctldev, selector, &pins, &npins);
	if (rc < 0)
		return rc;

	for (i = 0; i < npins; i++) {
		rc = aspeed_pin_config_set(pctldev, pins[i], configs,
				num_configs);
		if (rc < 0)
			return rc;
	}

	return 0;
}
