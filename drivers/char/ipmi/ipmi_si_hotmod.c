// SPDX-License-Identifier: GPL-2.0+
/*
 * ipmi_si_hotmod.c
 *
 * Handling for dynamically adding/removing IPMI devices through
 * a module parameter (and thus sysfs).
 */
<<<<<<< HEAD
#include <linux/moduleparam.h>
#include <linux/ipmi.h>
#include "ipmi_si.h"

#define PFX "ipmi_hotmod: "
=======

#define pr_fmt(fmt) "ipmi_hotmod: " fmt

#include <linux/moduleparam.h>
#include <linux/ipmi.h>
#include <linux/atomic.h>
#include "ipmi_si.h"
#include "ipmi_plat_data.h"
>>>>>>> upstream/android-13

static int hotmod_handler(const char *val, const struct kernel_param *kp);

module_param_call(hotmod, hotmod_handler, NULL, NULL, 0200);
<<<<<<< HEAD
MODULE_PARM_DESC(hotmod, "Add and remove interfaces.  See"
		 " Documentation/IPMI.txt in the kernel sources for the"
		 " gory details.");
=======
MODULE_PARM_DESC(hotmod,
		 "Add and remove interfaces.  See Documentation/driver-api/ipmi.rst in the kernel sources for the gory details.");
>>>>>>> upstream/android-13

/*
 * Parms come in as <op1>[:op2[:op3...]].  ops are:
 *   add|remove,kcs|bt|smic,mem|i/o,<address>[,<opt1>[,<opt2>[,...]]]
 * Options are:
 *   rsp=<regspacing>
 *   rsi=<regsize>
 *   rsh=<regshift>
 *   irq=<irq>
 *   ipmb=<ipmb addr>
 */
enum hotmod_op { HM_ADD, HM_REMOVE };
struct hotmod_vals {
	const char *name;
	const int  val;
};

static const struct hotmod_vals hotmod_ops[] = {
	{ "add",	HM_ADD },
	{ "remove",	HM_REMOVE },
	{ NULL }
};

static const struct hotmod_vals hotmod_si[] = {
	{ "kcs",	SI_KCS },
	{ "smic",	SI_SMIC },
	{ "bt",		SI_BT },
	{ NULL }
};

static const struct hotmod_vals hotmod_as[] = {
	{ "mem",	IPMI_MEM_ADDR_SPACE },
	{ "i/o",	IPMI_IO_ADDR_SPACE },
	{ NULL }
};

<<<<<<< HEAD
static int parse_str(const struct hotmod_vals *v, int *val, char *name,
		     char **curr)
=======
static int parse_str(const struct hotmod_vals *v, unsigned int *val, char *name,
		     const char **curr)
>>>>>>> upstream/android-13
{
	char *s;
	int  i;

	s = strchr(*curr, ',');
	if (!s) {
<<<<<<< HEAD
		pr_warn(PFX "No hotmod %s given.\n", name);
=======
		pr_warn("No hotmod %s given\n", name);
>>>>>>> upstream/android-13
		return -EINVAL;
	}
	*s = '\0';
	s++;
	for (i = 0; v[i].name; i++) {
		if (strcmp(*curr, v[i].name) == 0) {
			*val = v[i].val;
			*curr = s;
			return 0;
		}
	}

<<<<<<< HEAD
	pr_warn(PFX "Invalid hotmod %s '%s'\n", name, *curr);
=======
	pr_warn("Invalid hotmod %s '%s'\n", name, *curr);
>>>>>>> upstream/android-13
	return -EINVAL;
}

static int check_hotmod_int_op(const char *curr, const char *option,
<<<<<<< HEAD
			       const char *name, int *val)
=======
			       const char *name, unsigned int *val)
>>>>>>> upstream/android-13
{
	char *n;

	if (strcmp(curr, name) == 0) {
		if (!option) {
<<<<<<< HEAD
			pr_warn(PFX "No option given for '%s'\n", curr);
=======
			pr_warn("No option given for '%s'\n", curr);
>>>>>>> upstream/android-13
			return -EINVAL;
		}
		*val = simple_strtoul(option, &n, 0);
		if ((*n != '\0') || (*option == '\0')) {
<<<<<<< HEAD
			pr_warn(PFX "Bad option given for '%s'\n", curr);
=======
			pr_warn("Bad option given for '%s'\n", curr);
>>>>>>> upstream/android-13
			return -EINVAL;
		}
		return 1;
	}
	return 0;
}

<<<<<<< HEAD
static int hotmod_handler(const char *val, const struct kernel_param *kp)
{
	char *str = kstrdup(val, GFP_KERNEL);
	int  rv;
	char *next, *curr, *s, *n, *o;
	enum hotmod_op op;
	enum si_type si_type;
	int  addr_space;
	unsigned long addr;
	int regspacing;
	int regsize;
	int regshift;
	int irq;
	int ipmb;
	int ival;
	int len;

=======
static int parse_hotmod_str(const char *curr, enum hotmod_op *op,
			    struct ipmi_plat_data *h)
{
	char *s, *o;
	int rv;
	unsigned int ival;

	h->iftype = IPMI_PLAT_IF_SI;
	rv = parse_str(hotmod_ops, &ival, "operation", &curr);
	if (rv)
		return rv;
	*op = ival;

	rv = parse_str(hotmod_si, &ival, "interface type", &curr);
	if (rv)
		return rv;
	h->type = ival;

	rv = parse_str(hotmod_as, &ival, "address space", &curr);
	if (rv)
		return rv;
	h->space = ival;

	s = strchr(curr, ',');
	if (s) {
		*s = '\0';
		s++;
	}
	rv = kstrtoul(curr, 0, &h->addr);
	if (rv) {
		pr_warn("Invalid hotmod address '%s': %d\n", curr, rv);
		return rv;
	}

	while (s) {
		curr = s;
		s = strchr(curr, ',');
		if (s) {
			*s = '\0';
			s++;
		}
		o = strchr(curr, '=');
		if (o) {
			*o = '\0';
			o++;
		}
		rv = check_hotmod_int_op(curr, o, "rsp", &h->regspacing);
		if (rv < 0)
			return rv;
		else if (rv)
			continue;
		rv = check_hotmod_int_op(curr, o, "rsi", &h->regsize);
		if (rv < 0)
			return rv;
		else if (rv)
			continue;
		rv = check_hotmod_int_op(curr, o, "rsh", &h->regshift);
		if (rv < 0)
			return rv;
		else if (rv)
			continue;
		rv = check_hotmod_int_op(curr, o, "irq", &h->irq);
		if (rv < 0)
			return rv;
		else if (rv)
			continue;
		rv = check_hotmod_int_op(curr, o, "ipmb", &h->slave_addr);
		if (rv < 0)
			return rv;
		else if (rv)
			continue;

		pr_warn("Invalid hotmod option '%s'\n", curr);
		return -EINVAL;
	}

	h->addr_source = SI_HOTMOD;
	return 0;
}

static atomic_t hotmod_nr;

static int hotmod_handler(const char *val, const struct kernel_param *kp)
{
	int  rv;
	struct ipmi_plat_data h;
	char *str, *curr, *next;

	str = kstrdup(val, GFP_KERNEL);
>>>>>>> upstream/android-13
	if (!str)
		return -ENOMEM;

	/* Kill any trailing spaces, as we can get a "\n" from echo. */
<<<<<<< HEAD
	len = strlen(str);
	ival = len - 1;
	while ((ival >= 0) && isspace(str[ival])) {
		str[ival] = '\0';
		ival--;
	}

	for (curr = str; curr; curr = next) {
		regspacing = 1;
		regsize = 1;
		regshift = 0;
		irq = 0;
		ipmb = 0; /* Choose the default if not specified */
=======
	for (curr = strstrip(str); curr; curr = next) {
		enum hotmod_op op;
>>>>>>> upstream/android-13

		next = strchr(curr, ':');
		if (next) {
			*next = '\0';
			next++;
		}

<<<<<<< HEAD
		rv = parse_str(hotmod_ops, &ival, "operation", &curr);
		if (rv)
			break;
		op = ival;

		rv = parse_str(hotmod_si, &ival, "interface type", &curr);
		if (rv)
			break;
		si_type = ival;

		rv = parse_str(hotmod_as, &addr_space, "address space", &curr);
		if (rv)
			break;

		s = strchr(curr, ',');
		if (s) {
			*s = '\0';
			s++;
		}
		addr = simple_strtoul(curr, &n, 0);
		if ((*n != '\0') || (*curr == '\0')) {
			pr_warn(PFX "Invalid hotmod address '%s'\n", curr);
			break;
		}

		while (s) {
			curr = s;
			s = strchr(curr, ',');
			if (s) {
				*s = '\0';
				s++;
			}
			o = strchr(curr, '=');
			if (o) {
				*o = '\0';
				o++;
			}
			rv = check_hotmod_int_op(curr, o, "rsp", &regspacing);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "rsi", &regsize);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "rsh", &regshift);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "irq", &irq);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;
			rv = check_hotmod_int_op(curr, o, "ipmb", &ipmb);
			if (rv < 0)
				goto out;
			else if (rv)
				continue;

			rv = -EINVAL;
			pr_warn(PFX "Invalid hotmod option '%s'\n", curr);
			goto out;
		}

		if (op == HM_ADD) {
			struct si_sm_io io;

			memset(&io, 0, sizeof(io));
			io.addr_source = SI_HOTMOD;
			io.si_type = si_type;
			io.addr_data = addr;
			io.addr_type = addr_space;

			io.addr = NULL;
			io.regspacing = regspacing;
			if (!io.regspacing)
				io.regspacing = DEFAULT_REGSPACING;
			io.regsize = regsize;
			if (!io.regsize)
				io.regsize = DEFAULT_REGSIZE;
			io.regshift = regshift;
			io.irq = irq;
			if (io.irq)
				io.irq_setup = ipmi_std_irq_setup;
			io.slave_addr = ipmb;

			rv = ipmi_si_add_smi(&io);
			if (rv)
				goto out;
		} else {
			ipmi_si_remove_by_data(addr_space, si_type, addr);
		}
	}
	rv = len;
=======
		memset(&h, 0, sizeof(h));
		rv = parse_hotmod_str(curr, &op, &h);
		if (rv)
			goto out;

		if (op == HM_ADD) {
			ipmi_platform_add("hotmod-ipmi-si",
					  atomic_inc_return(&hotmod_nr),
					  &h);
		} else {
			struct device *dev;

			dev = ipmi_si_remove_by_data(h.space, h.type, h.addr);
			if (dev && dev_is_platform(dev)) {
				struct platform_device *pdev;

				pdev = to_platform_device(dev);
				if (strcmp(pdev->name, "hotmod-ipmi-si") == 0)
					platform_device_unregister(pdev);
			}
			put_device(dev);
		}
	}
	rv = strlen(val);
>>>>>>> upstream/android-13
out:
	kfree(str);
	return rv;
}
<<<<<<< HEAD
=======

void ipmi_si_hotmod_exit(void)
{
	ipmi_remove_platform_device_by_name("hotmod-ipmi-si");
}
>>>>>>> upstream/android-13
