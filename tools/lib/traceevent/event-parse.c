// SPDX-License-Identifier: LGPL-2.1
/*
 * Copyright (C) 2009, 2010 Red Hat Inc, Steven Rostedt <srostedt@redhat.com>
 *
 *
 *  The parts for function graph printing was taken and modified from the
 *  Linux Kernel that were written by
 *    - Copyright (C) 2009  Frederic Weisbecker,
 *  Frederic Weisbecker gave his permission to relicense the code to
 *  the Lesser General Public License.
 */
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
<<<<<<< HEAD
#include <linux/string.h>
=======
>>>>>>> upstream/android-13
#include <linux/time64.h>

#include <netinet/in.h>
#include "event-parse.h"
<<<<<<< HEAD
#include "event-utils.h"
=======

#include "event-parse-local.h"
#include "event-utils.h"
#include "trace-seq.h"
>>>>>>> upstream/android-13

static const char *input_buf;
static unsigned long long input_buf_ptr;
static unsigned long long input_buf_siz;

static int is_flag_field;
static int is_symbolic_field;

static int show_warning = 1;

#define do_warning(fmt, ...)				\
	do {						\
		if (show_warning)			\
			warning(fmt, ##__VA_ARGS__);	\
	} while (0)

#define do_warning_event(event, fmt, ...)			\
	do {							\
		if (!show_warning)				\
			continue;				\
								\
		if (event)					\
			warning("[%s:%s] " fmt, event->system,	\
				event->name, ##__VA_ARGS__);	\
		else						\
			warning(fmt, ##__VA_ARGS__);		\
	} while (0)

<<<<<<< HEAD
static void init_input_buf(const char *buf, unsigned long long size)
=======
/**
 * init_input_buf - init buffer for parsing
 * @buf: buffer to parse
 * @size: the size of the buffer
 *
 * Initializes the internal buffer that tep_read_token() will parse.
 */
__hidden void init_input_buf(const char *buf, unsigned long long size)
>>>>>>> upstream/android-13
{
	input_buf = buf;
	input_buf_siz = size;
	input_buf_ptr = 0;
}

<<<<<<< HEAD
const char *tep_get_input_buf(void)
=======
__hidden const char *get_input_buf(void)
>>>>>>> upstream/android-13
{
	return input_buf;
}

<<<<<<< HEAD
unsigned long long tep_get_input_buf_ptr(void)
=======
__hidden unsigned long long get_input_buf_ptr(void)
>>>>>>> upstream/android-13
{
	return input_buf_ptr;
}

struct event_handler {
	struct event_handler		*next;
	int				id;
	const char			*sys_name;
	const char			*event_name;
	tep_event_handler_func		func;
	void				*context;
};

struct func_params {
	struct func_params	*next;
	enum tep_func_arg_type	type;
};

struct tep_function_handler {
	struct tep_function_handler	*next;
	enum tep_func_arg_type		ret_type;
	char				*name;
	tep_func_handler		func;
	struct func_params		*params;
	int				nr_args;
};

static unsigned long long
process_defined_func(struct trace_seq *s, void *data, int size,
<<<<<<< HEAD
		     struct event_format *event, struct print_arg *arg);

static void free_func_handle(struct tep_function_handler *func);

/**
 * tep_buffer_init - init buffer for parsing
 * @buf: buffer to parse
 * @size: the size of the buffer
 *
 * For use with tep_read_token(), this initializes the internal
 * buffer that tep_read_token() will parse.
 */
void tep_buffer_init(const char *buf, unsigned long long size)
{
	init_input_buf(buf, size);
}

=======
		     struct tep_event *event, struct tep_print_arg *arg);

static void free_func_handle(struct tep_function_handler *func);

>>>>>>> upstream/android-13
void breakpoint(void)
{
	static int x;
	x++;
}

<<<<<<< HEAD
struct print_arg *alloc_arg(void)
{
	return calloc(1, sizeof(struct print_arg));
}

struct cmdline {
=======
static struct tep_print_arg *alloc_arg(void)
{
	return calloc(1, sizeof(struct tep_print_arg));
}

struct tep_cmdline {
>>>>>>> upstream/android-13
	char *comm;
	int pid;
};

static int cmdline_cmp(const void *a, const void *b)
{
<<<<<<< HEAD
	const struct cmdline *ca = a;
	const struct cmdline *cb = b;
=======
	const struct tep_cmdline *ca = a;
	const struct tep_cmdline *cb = b;
>>>>>>> upstream/android-13

	if (ca->pid < cb->pid)
		return -1;
	if (ca->pid > cb->pid)
		return 1;

	return 0;
}

<<<<<<< HEAD
=======
/* Looking for where to place the key */
static int cmdline_slot_cmp(const void *a, const void *b)
{
	const struct tep_cmdline *ca = a;
	const struct tep_cmdline *cb = b;
	const struct tep_cmdline *cb1 = cb + 1;

	if (ca->pid < cb->pid)
		return -1;

	if (ca->pid > cb->pid) {
		if (ca->pid <= cb1->pid)
			return 0;
		return 1;
	}

	return 0;
}

>>>>>>> upstream/android-13
struct cmdline_list {
	struct cmdline_list	*next;
	char			*comm;
	int			pid;
};

<<<<<<< HEAD
static int cmdline_init(struct tep_handle *pevent)
{
	struct cmdline_list *cmdlist = pevent->cmdlist;
	struct cmdline_list *item;
	struct cmdline *cmdlines;
	int i;

	cmdlines = malloc(sizeof(*cmdlines) * pevent->cmdline_count);
=======
static int cmdline_init(struct tep_handle *tep)
{
	struct cmdline_list *cmdlist = tep->cmdlist;
	struct cmdline_list *item;
	struct tep_cmdline *cmdlines;
	int i;

	cmdlines = malloc(sizeof(*cmdlines) * tep->cmdline_count);
>>>>>>> upstream/android-13
	if (!cmdlines)
		return -1;

	i = 0;
	while (cmdlist) {
		cmdlines[i].pid = cmdlist->pid;
		cmdlines[i].comm = cmdlist->comm;
		i++;
		item = cmdlist;
		cmdlist = cmdlist->next;
		free(item);
	}

<<<<<<< HEAD
	qsort(cmdlines, pevent->cmdline_count, sizeof(*cmdlines), cmdline_cmp);

	pevent->cmdlines = cmdlines;
	pevent->cmdlist = NULL;
=======
	qsort(cmdlines, tep->cmdline_count, sizeof(*cmdlines), cmdline_cmp);

	tep->cmdlines = cmdlines;
	tep->cmdlist = NULL;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static const char *find_cmdline(struct tep_handle *pevent, int pid)
{
	const struct cmdline *comm;
	struct cmdline key;
=======
static const char *find_cmdline(struct tep_handle *tep, int pid)
{
	const struct tep_cmdline *comm;
	struct tep_cmdline key;
>>>>>>> upstream/android-13

	if (!pid)
		return "<idle>";

<<<<<<< HEAD
	if (!pevent->cmdlines && cmdline_init(pevent))
=======
	if (!tep->cmdlines && cmdline_init(tep))
>>>>>>> upstream/android-13
		return "<not enough memory for cmdlines!>";

	key.pid = pid;

<<<<<<< HEAD
	comm = bsearch(&key, pevent->cmdlines, pevent->cmdline_count,
		       sizeof(*pevent->cmdlines), cmdline_cmp);
=======
	comm = bsearch(&key, tep->cmdlines, tep->cmdline_count,
		       sizeof(*tep->cmdlines), cmdline_cmp);
>>>>>>> upstream/android-13

	if (comm)
		return comm->comm;
	return "<...>";
}

/**
<<<<<<< HEAD
 * tep_pid_is_registered - return if a pid has a cmdline registered
 * @pevent: handle for the pevent
 * @pid: The pid to check if it has a cmdline registered with.
 *
 * Returns 1 if the pid has a cmdline mapped to it
 * 0 otherwise.
 */
int tep_pid_is_registered(struct tep_handle *pevent, int pid)
{
	const struct cmdline *comm;
	struct cmdline key;

	if (!pid)
		return 1;

	if (!pevent->cmdlines && cmdline_init(pevent))
		return 0;

	key.pid = pid;

	comm = bsearch(&key, pevent->cmdlines, pevent->cmdline_count,
		       sizeof(*pevent->cmdlines), cmdline_cmp);

	if (comm)
		return 1;
	return 0;
=======
 * tep_is_pid_registered - return if a pid has a cmdline registered
 * @tep: a handle to the trace event parser context
 * @pid: The pid to check if it has a cmdline registered with.
 *
 * Returns true if the pid has a cmdline mapped to it
 * false otherwise.
 */
bool tep_is_pid_registered(struct tep_handle *tep, int pid)
{
	const struct tep_cmdline *comm;
	struct tep_cmdline key;

	if (!pid)
		return true;

	if (!tep->cmdlines && cmdline_init(tep))
		return false;

	key.pid = pid;

	comm = bsearch(&key, tep->cmdlines, tep->cmdline_count,
		       sizeof(*tep->cmdlines), cmdline_cmp);

	if (comm)
		return true;
	return false;
>>>>>>> upstream/android-13
}

/*
 * If the command lines have been converted to an array, then
 * we must add this pid. This is much slower than when cmdlines
 * are added before the array is initialized.
 */
<<<<<<< HEAD
static int add_new_comm(struct tep_handle *pevent, const char *comm, int pid)
{
	struct cmdline *cmdlines = pevent->cmdlines;
	const struct cmdline *cmdline;
	struct cmdline key;
=======
static int add_new_comm(struct tep_handle *tep,
			const char *comm, int pid, bool override)
{
	struct tep_cmdline *cmdlines = tep->cmdlines;
	struct tep_cmdline *cmdline;
	struct tep_cmdline key;
	char *new_comm;
	int cnt;
>>>>>>> upstream/android-13

	if (!pid)
		return 0;

	/* avoid duplicates */
	key.pid = pid;

<<<<<<< HEAD
	cmdline = bsearch(&key, pevent->cmdlines, pevent->cmdline_count,
		       sizeof(*pevent->cmdlines), cmdline_cmp);
	if (cmdline) {
		errno = EEXIST;
		return -1;
	}

	cmdlines = realloc(cmdlines, sizeof(*cmdlines) * (pevent->cmdline_count + 1));
=======
	cmdline = bsearch(&key, tep->cmdlines, tep->cmdline_count,
			  sizeof(*tep->cmdlines), cmdline_cmp);
	if (cmdline) {
		if (!override) {
			errno = EEXIST;
			return -1;
		}
		new_comm = strdup(comm);
		if (!new_comm) {
			errno = ENOMEM;
			return -1;
		}
		free(cmdline->comm);
		cmdline->comm = new_comm;

		return 0;
	}

	cmdlines = realloc(cmdlines, sizeof(*cmdlines) * (tep->cmdline_count + 1));
>>>>>>> upstream/android-13
	if (!cmdlines) {
		errno = ENOMEM;
		return -1;
	}
<<<<<<< HEAD
	pevent->cmdlines = cmdlines;

	cmdlines[pevent->cmdline_count].comm = strdup(comm);
	if (!cmdlines[pevent->cmdline_count].comm) {
=======
	tep->cmdlines = cmdlines;

	key.comm = strdup(comm);
	if (!key.comm) {
>>>>>>> upstream/android-13
		errno = ENOMEM;
		return -1;
	}

<<<<<<< HEAD
	cmdlines[pevent->cmdline_count].pid = pid;
		
	if (cmdlines[pevent->cmdline_count].comm)
		pevent->cmdline_count++;

	qsort(cmdlines, pevent->cmdline_count, sizeof(*cmdlines), cmdline_cmp);
=======
	if (!tep->cmdline_count) {
		/* no entries yet */
		tep->cmdlines[0] = key;
		tep->cmdline_count++;
		return 0;
	}

	/* Now find where we want to store the new cmdline */
	cmdline = bsearch(&key, tep->cmdlines, tep->cmdline_count - 1,
			  sizeof(*tep->cmdlines), cmdline_slot_cmp);

	cnt = tep->cmdline_count;
	if (cmdline) {
		/* cmdline points to the one before the spot we want */
		cmdline++;
		cnt -= cmdline - tep->cmdlines;

	} else {
		/* The new entry is either before or after the list */
		if (key.pid > tep->cmdlines[tep->cmdline_count - 1].pid) {
			tep->cmdlines[tep->cmdline_count++] = key;
			return 0;
		}
		cmdline = &tep->cmdlines[0];
	}
	memmove(cmdline + 1, cmdline, (cnt * sizeof(*cmdline)));
	*cmdline = key;

	tep->cmdline_count++;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
/**
 * tep_register_comm - register a pid / comm mapping
 * @pevent: handle for the pevent
 * @comm: the command line to register
 * @pid: the pid to map the command line to
 *
 * This adds a mapping to search for command line names with
 * a given pid. The comm is duplicated.
 */
int tep_register_comm(struct tep_handle *pevent, const char *comm, int pid)
{
	struct cmdline_list *item;

	if (pevent->cmdlines)
		return add_new_comm(pevent, comm, pid);
=======
static int _tep_register_comm(struct tep_handle *tep,
			      const char *comm, int pid, bool override)
{
	struct cmdline_list *item;

	if (tep->cmdlines)
		return add_new_comm(tep, comm, pid, override);
>>>>>>> upstream/android-13

	item = malloc(sizeof(*item));
	if (!item)
		return -1;

	if (comm)
		item->comm = strdup(comm);
	else
		item->comm = strdup("<...>");
	if (!item->comm) {
		free(item);
		return -1;
	}
	item->pid = pid;
<<<<<<< HEAD
	item->next = pevent->cmdlist;

	pevent->cmdlist = item;
	pevent->cmdline_count++;
=======
	item->next = tep->cmdlist;

	tep->cmdlist = item;
	tep->cmdline_count++;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
int tep_register_trace_clock(struct tep_handle *pevent, const char *trace_clock)
{
	pevent->trace_clock = strdup(trace_clock);
	if (!pevent->trace_clock) {
		errno = ENOMEM;
		return -1;
	}
	return 0;
=======
/**
 * tep_register_comm - register a pid / comm mapping
 * @tep: a handle to the trace event parser context
 * @comm: the command line to register
 * @pid: the pid to map the command line to
 *
 * This adds a mapping to search for command line names with
 * a given pid. The comm is duplicated. If a command with the same pid
 * already exist, -1 is returned and errno is set to EEXIST
 */
int tep_register_comm(struct tep_handle *tep, const char *comm, int pid)
{
	return _tep_register_comm(tep, comm, pid, false);
}

/**
 * tep_override_comm - register a pid / comm mapping
 * @tep: a handle to the trace event parser context
 * @comm: the command line to register
 * @pid: the pid to map the command line to
 *
 * This adds a mapping to search for command line names with
 * a given pid. The comm is duplicated. If a command with the same pid
 * already exist, the command string is udapted with the new one
 */
int tep_override_comm(struct tep_handle *tep, const char *comm, int pid)
{
	if (!tep->cmdlines && cmdline_init(tep)) {
		errno = ENOMEM;
		return -1;
	}
	return _tep_register_comm(tep, comm, pid, true);
>>>>>>> upstream/android-13
}

struct func_map {
	unsigned long long		addr;
	char				*func;
	char				*mod;
};

struct func_list {
	struct func_list	*next;
	unsigned long long	addr;
	char			*func;
	char			*mod;
};

static int func_cmp(const void *a, const void *b)
{
	const struct func_map *fa = a;
	const struct func_map *fb = b;

	if (fa->addr < fb->addr)
		return -1;
	if (fa->addr > fb->addr)
		return 1;

	return 0;
}

/*
 * We are searching for a record in between, not an exact
 * match.
 */
static int func_bcmp(const void *a, const void *b)
{
	const struct func_map *fa = a;
	const struct func_map *fb = b;

	if ((fa->addr == fb->addr) ||

	    (fa->addr > fb->addr &&
	     fa->addr < (fb+1)->addr))
		return 0;

	if (fa->addr < fb->addr)
		return -1;

	return 1;
}

<<<<<<< HEAD
static int func_map_init(struct tep_handle *pevent)
=======
static int func_map_init(struct tep_handle *tep)
>>>>>>> upstream/android-13
{
	struct func_list *funclist;
	struct func_list *item;
	struct func_map *func_map;
	int i;

<<<<<<< HEAD
	func_map = malloc(sizeof(*func_map) * (pevent->func_count + 1));
	if (!func_map)
		return -1;

	funclist = pevent->funclist;
=======
	func_map = malloc(sizeof(*func_map) * (tep->func_count + 1));
	if (!func_map)
		return -1;

	funclist = tep->funclist;
>>>>>>> upstream/android-13

	i = 0;
	while (funclist) {
		func_map[i].func = funclist->func;
		func_map[i].addr = funclist->addr;
		func_map[i].mod = funclist->mod;
		i++;
		item = funclist;
		funclist = funclist->next;
		free(item);
	}

<<<<<<< HEAD
	qsort(func_map, pevent->func_count, sizeof(*func_map), func_cmp);
=======
	qsort(func_map, tep->func_count, sizeof(*func_map), func_cmp);
>>>>>>> upstream/android-13

	/*
	 * Add a special record at the end.
	 */
<<<<<<< HEAD
	func_map[pevent->func_count].func = NULL;
	func_map[pevent->func_count].addr = 0;
	func_map[pevent->func_count].mod = NULL;

	pevent->func_map = func_map;
	pevent->funclist = NULL;
=======
	func_map[tep->func_count].func = NULL;
	func_map[tep->func_count].addr = 0;
	func_map[tep->func_count].mod = NULL;

	tep->func_map = func_map;
	tep->funclist = NULL;
>>>>>>> upstream/android-13

	return 0;
}

static struct func_map *
<<<<<<< HEAD
__find_func(struct tep_handle *pevent, unsigned long long addr)
=======
__find_func(struct tep_handle *tep, unsigned long long addr)
>>>>>>> upstream/android-13
{
	struct func_map *func;
	struct func_map key;

<<<<<<< HEAD
	if (!pevent->func_map)
		func_map_init(pevent);

	key.addr = addr;

	func = bsearch(&key, pevent->func_map, pevent->func_count,
		       sizeof(*pevent->func_map), func_bcmp);
=======
	if (!tep->func_map)
		func_map_init(tep);

	key.addr = addr;

	func = bsearch(&key, tep->func_map, tep->func_count,
		       sizeof(*tep->func_map), func_bcmp);
>>>>>>> upstream/android-13

	return func;
}

struct func_resolver {
	tep_func_resolver_t	*func;
	void			*priv;
	struct func_map		map;
};

/**
 * tep_set_function_resolver - set an alternative function resolver
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @resolver: function to be used
 * @priv: resolver function private state.
 *
 * Some tools may have already a way to resolve kernel functions, allow them to
<<<<<<< HEAD
 * keep using it instead of duplicating all the entries inside
 * pevent->funclist.
 */
int tep_set_function_resolver(struct tep_handle *pevent,
=======
 * keep using it instead of duplicating all the entries inside tep->funclist.
 */
int tep_set_function_resolver(struct tep_handle *tep,
>>>>>>> upstream/android-13
			      tep_func_resolver_t *func, void *priv)
{
	struct func_resolver *resolver = malloc(sizeof(*resolver));

	if (resolver == NULL)
		return -1;

	resolver->func = func;
	resolver->priv = priv;

<<<<<<< HEAD
	free(pevent->func_resolver);
	pevent->func_resolver = resolver;
=======
	free(tep->func_resolver);
	tep->func_resolver = resolver;
>>>>>>> upstream/android-13

	return 0;
}

/**
 * tep_reset_function_resolver - reset alternative function resolver
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 *
 * Stop using whatever alternative resolver was set, use the default
 * one instead.
 */
<<<<<<< HEAD
void tep_reset_function_resolver(struct tep_handle *pevent)
{
	free(pevent->func_resolver);
	pevent->func_resolver = NULL;
}

static struct func_map *
find_func(struct tep_handle *pevent, unsigned long long addr)
{
	struct func_map *map;

	if (!pevent->func_resolver)
		return __find_func(pevent, addr);

	map = &pevent->func_resolver->map;
	map->mod  = NULL;
	map->addr = addr;
	map->func = pevent->func_resolver->func(pevent->func_resolver->priv,
						&map->addr, &map->mod);
=======
void tep_reset_function_resolver(struct tep_handle *tep)
{
	free(tep->func_resolver);
	tep->func_resolver = NULL;
}

static struct func_map *
find_func(struct tep_handle *tep, unsigned long long addr)
{
	struct func_map *map;

	if (!tep->func_resolver)
		return __find_func(tep, addr);

	map = &tep->func_resolver->map;
	map->mod  = NULL;
	map->addr = addr;
	map->func = tep->func_resolver->func(tep->func_resolver->priv,
					     &map->addr, &map->mod);
>>>>>>> upstream/android-13
	if (map->func == NULL)
		return NULL;

	return map;
}

/**
 * tep_find_function - find a function by a given address
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @addr: the address to find the function with
 *
 * Returns a pointer to the function stored that has the given
 * address. Note, the address does not have to be exact, it
 * will select the function that would contain the address.
 */
<<<<<<< HEAD
const char *tep_find_function(struct tep_handle *pevent, unsigned long long addr)
{
	struct func_map *map;

	map = find_func(pevent, addr);
=======
const char *tep_find_function(struct tep_handle *tep, unsigned long long addr)
{
	struct func_map *map;

	map = find_func(tep, addr);
>>>>>>> upstream/android-13
	if (!map)
		return NULL;

	return map->func;
}

/**
 * tep_find_function_address - find a function address by a given address
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @addr: the address to find the function with
 *
 * Returns the address the function starts at. This can be used in
 * conjunction with tep_find_function to print both the function
 * name and the function offset.
 */
unsigned long long
<<<<<<< HEAD
tep_find_function_address(struct tep_handle *pevent, unsigned long long addr)
{
	struct func_map *map;

	map = find_func(pevent, addr);
=======
tep_find_function_address(struct tep_handle *tep, unsigned long long addr)
{
	struct func_map *map;

	map = find_func(tep, addr);
>>>>>>> upstream/android-13
	if (!map)
		return 0;

	return map->addr;
}

/**
 * tep_register_function - register a function with a given address
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @function: the function name to register
 * @addr: the address the function starts at
 * @mod: the kernel module the function may be in (NULL for none)
 *
 * This registers a function name with an address and module.
 * The @func passed in is duplicated.
 */
<<<<<<< HEAD
int tep_register_function(struct tep_handle *pevent, char *func,
=======
int tep_register_function(struct tep_handle *tep, char *func,
>>>>>>> upstream/android-13
			  unsigned long long addr, char *mod)
{
	struct func_list *item = malloc(sizeof(*item));

	if (!item)
		return -1;

<<<<<<< HEAD
	item->next = pevent->funclist;
=======
	item->next = tep->funclist;
>>>>>>> upstream/android-13
	item->func = strdup(func);
	if (!item->func)
		goto out_free;

	if (mod) {
		item->mod = strdup(mod);
		if (!item->mod)
			goto out_free_func;
	} else
		item->mod = NULL;
	item->addr = addr;

<<<<<<< HEAD
	pevent->funclist = item;
	pevent->func_count++;
=======
	tep->funclist = item;
	tep->func_count++;
>>>>>>> upstream/android-13

	return 0;

out_free_func:
	free(item->func);
	item->func = NULL;
out_free:
	free(item);
	errno = ENOMEM;
	return -1;
}

/**
 * tep_print_funcs - print out the stored functions
<<<<<<< HEAD
 * @pevent: handle for the pevent
 *
 * This prints out the stored functions.
 */
void tep_print_funcs(struct tep_handle *pevent)
{
	int i;

	if (!pevent->func_map)
		func_map_init(pevent);

	for (i = 0; i < (int)pevent->func_count; i++) {
		printf("%016llx %s",
		       pevent->func_map[i].addr,
		       pevent->func_map[i].func);
		if (pevent->func_map[i].mod)
			printf(" [%s]\n", pevent->func_map[i].mod);
=======
 * @tep: a handle to the trace event parser context
 *
 * This prints out the stored functions.
 */
void tep_print_funcs(struct tep_handle *tep)
{
	int i;

	if (!tep->func_map)
		func_map_init(tep);

	for (i = 0; i < (int)tep->func_count; i++) {
		printf("%016llx %s",
		       tep->func_map[i].addr,
		       tep->func_map[i].func);
		if (tep->func_map[i].mod)
			printf(" [%s]\n", tep->func_map[i].mod);
>>>>>>> upstream/android-13
		else
			printf("\n");
	}
}

struct printk_map {
	unsigned long long		addr;
	char				*printk;
};

struct printk_list {
	struct printk_list	*next;
	unsigned long long	addr;
	char			*printk;
};

static int printk_cmp(const void *a, const void *b)
{
	const struct printk_map *pa = a;
	const struct printk_map *pb = b;

	if (pa->addr < pb->addr)
		return -1;
	if (pa->addr > pb->addr)
		return 1;

	return 0;
}

<<<<<<< HEAD
static int printk_map_init(struct tep_handle *pevent)
=======
static int printk_map_init(struct tep_handle *tep)
>>>>>>> upstream/android-13
{
	struct printk_list *printklist;
	struct printk_list *item;
	struct printk_map *printk_map;
	int i;

<<<<<<< HEAD
	printk_map = malloc(sizeof(*printk_map) * (pevent->printk_count + 1));
	if (!printk_map)
		return -1;

	printklist = pevent->printklist;
=======
	printk_map = malloc(sizeof(*printk_map) * (tep->printk_count + 1));
	if (!printk_map)
		return -1;

	printklist = tep->printklist;
>>>>>>> upstream/android-13

	i = 0;
	while (printklist) {
		printk_map[i].printk = printklist->printk;
		printk_map[i].addr = printklist->addr;
		i++;
		item = printklist;
		printklist = printklist->next;
		free(item);
	}

<<<<<<< HEAD
	qsort(printk_map, pevent->printk_count, sizeof(*printk_map), printk_cmp);

	pevent->printk_map = printk_map;
	pevent->printklist = NULL;
=======
	qsort(printk_map, tep->printk_count, sizeof(*printk_map), printk_cmp);

	tep->printk_map = printk_map;
	tep->printklist = NULL;
>>>>>>> upstream/android-13

	return 0;
}

static struct printk_map *
<<<<<<< HEAD
find_printk(struct tep_handle *pevent, unsigned long long addr)
=======
find_printk(struct tep_handle *tep, unsigned long long addr)
>>>>>>> upstream/android-13
{
	struct printk_map *printk;
	struct printk_map key;

<<<<<<< HEAD
	if (!pevent->printk_map && printk_map_init(pevent))
=======
	if (!tep->printk_map && printk_map_init(tep))
>>>>>>> upstream/android-13
		return NULL;

	key.addr = addr;

<<<<<<< HEAD
	printk = bsearch(&key, pevent->printk_map, pevent->printk_count,
			 sizeof(*pevent->printk_map), printk_cmp);
=======
	printk = bsearch(&key, tep->printk_map, tep->printk_count,
			 sizeof(*tep->printk_map), printk_cmp);
>>>>>>> upstream/android-13

	return printk;
}

/**
 * tep_register_print_string - register a string by its address
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @fmt: the string format to register
 * @addr: the address the string was located at
 *
 * This registers a string by the address it was stored in the kernel.
 * The @fmt passed in is duplicated.
 */
<<<<<<< HEAD
int tep_register_print_string(struct tep_handle *pevent, const char *fmt,
=======
int tep_register_print_string(struct tep_handle *tep, const char *fmt,
>>>>>>> upstream/android-13
			      unsigned long long addr)
{
	struct printk_list *item = malloc(sizeof(*item));
	char *p;

	if (!item)
		return -1;

<<<<<<< HEAD
	item->next = pevent->printklist;
=======
	item->next = tep->printklist;
>>>>>>> upstream/android-13
	item->addr = addr;

	/* Strip off quotes and '\n' from the end */
	if (fmt[0] == '"')
		fmt++;
	item->printk = strdup(fmt);
	if (!item->printk)
		goto out_free;

	p = item->printk + strlen(item->printk) - 1;
	if (*p == '"')
		*p = 0;

	p -= 2;
	if (strcmp(p, "\\n") == 0)
		*p = 0;

<<<<<<< HEAD
	pevent->printklist = item;
	pevent->printk_count++;
=======
	tep->printklist = item;
	tep->printk_count++;
>>>>>>> upstream/android-13

	return 0;

out_free:
	free(item);
	errno = ENOMEM;
	return -1;
}

/**
 * tep_print_printk - print out the stored strings
<<<<<<< HEAD
 * @pevent: handle for the pevent
 *
 * This prints the string formats that were stored.
 */
void tep_print_printk(struct tep_handle *pevent)
{
	int i;

	if (!pevent->printk_map)
		printk_map_init(pevent);

	for (i = 0; i < (int)pevent->printk_count; i++) {
		printf("%016llx %s\n",
		       pevent->printk_map[i].addr,
		       pevent->printk_map[i].printk);
	}
}

static struct event_format *alloc_event(void)
{
	return calloc(1, sizeof(struct event_format));
}

static int add_event(struct tep_handle *pevent, struct event_format *event)
{
	int i;
	struct event_format **events = realloc(pevent->events, sizeof(event) *
					       (pevent->nr_events + 1));
	if (!events)
		return -1;

	pevent->events = events;

	for (i = 0; i < pevent->nr_events; i++) {
		if (pevent->events[i]->id > event->id)
			break;
	}
	if (i < pevent->nr_events)
		memmove(&pevent->events[i + 1],
			&pevent->events[i],
			sizeof(event) * (pevent->nr_events - i));

	pevent->events[i] = event;
	pevent->nr_events++;

	event->pevent = pevent;
=======
 * @tep: a handle to the trace event parser context
 *
 * This prints the string formats that were stored.
 */
void tep_print_printk(struct tep_handle *tep)
{
	int i;

	if (!tep->printk_map)
		printk_map_init(tep);

	for (i = 0; i < (int)tep->printk_count; i++) {
		printf("%016llx %s\n",
		       tep->printk_map[i].addr,
		       tep->printk_map[i].printk);
	}
}

static struct tep_event *alloc_event(void)
{
	return calloc(1, sizeof(struct tep_event));
}

static int add_event(struct tep_handle *tep, struct tep_event *event)
{
	int i;
	struct tep_event **events = realloc(tep->events, sizeof(event) *
					    (tep->nr_events + 1));
	if (!events)
		return -1;

	tep->events = events;

	for (i = 0; i < tep->nr_events; i++) {
		if (tep->events[i]->id > event->id)
			break;
	}
	if (i < tep->nr_events)
		memmove(&tep->events[i + 1],
			&tep->events[i],
			sizeof(event) * (tep->nr_events - i));

	tep->events[i] = event;
	tep->nr_events++;

	event->tep = tep;
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int event_item_type(enum event_type type)
{
	switch (type) {
	case EVENT_ITEM ... EVENT_SQUOTE:
		return 1;
	case EVENT_ERROR ... EVENT_DELIM:
=======
static int event_item_type(enum tep_event_type type)
{
	switch (type) {
	case TEP_EVENT_ITEM ... TEP_EVENT_SQUOTE:
		return 1;
	case TEP_EVENT_ERROR ... TEP_EVENT_DELIM:
>>>>>>> upstream/android-13
	default:
		return 0;
	}
}

<<<<<<< HEAD
static void free_flag_sym(struct print_flag_sym *fsym)
{
	struct print_flag_sym *next;
=======
static void free_flag_sym(struct tep_print_flag_sym *fsym)
{
	struct tep_print_flag_sym *next;
>>>>>>> upstream/android-13

	while (fsym) {
		next = fsym->next;
		free(fsym->value);
		free(fsym->str);
		free(fsym);
		fsym = next;
	}
}

<<<<<<< HEAD
static void free_arg(struct print_arg *arg)
{
	struct print_arg *farg;
=======
static void free_arg(struct tep_print_arg *arg)
{
	struct tep_print_arg *farg;
>>>>>>> upstream/android-13

	if (!arg)
		return;

	switch (arg->type) {
<<<<<<< HEAD
	case PRINT_ATOM:
		free(arg->atom.atom);
		break;
	case PRINT_FIELD:
		free(arg->field.name);
		break;
	case PRINT_FLAGS:
=======
	case TEP_PRINT_ATOM:
		free(arg->atom.atom);
		break;
	case TEP_PRINT_FIELD:
		free(arg->field.name);
		break;
	case TEP_PRINT_FLAGS:
>>>>>>> upstream/android-13
		free_arg(arg->flags.field);
		free(arg->flags.delim);
		free_flag_sym(arg->flags.flags);
		break;
<<<<<<< HEAD
	case PRINT_SYMBOL:
		free_arg(arg->symbol.field);
		free_flag_sym(arg->symbol.symbols);
		break;
	case PRINT_HEX:
	case PRINT_HEX_STR:
		free_arg(arg->hex.field);
		free_arg(arg->hex.size);
		break;
	case PRINT_INT_ARRAY:
=======
	case TEP_PRINT_SYMBOL:
		free_arg(arg->symbol.field);
		free_flag_sym(arg->symbol.symbols);
		break;
	case TEP_PRINT_HEX:
	case TEP_PRINT_HEX_STR:
		free_arg(arg->hex.field);
		free_arg(arg->hex.size);
		break;
	case TEP_PRINT_INT_ARRAY:
>>>>>>> upstream/android-13
		free_arg(arg->int_array.field);
		free_arg(arg->int_array.count);
		free_arg(arg->int_array.el_size);
		break;
<<<<<<< HEAD
	case PRINT_TYPE:
		free(arg->typecast.type);
		free_arg(arg->typecast.item);
		break;
	case PRINT_STRING:
	case PRINT_BSTRING:
		free(arg->string.string);
		break;
	case PRINT_BITMASK:
		free(arg->bitmask.bitmask);
		break;
	case PRINT_DYNAMIC_ARRAY:
	case PRINT_DYNAMIC_ARRAY_LEN:
		free(arg->dynarray.index);
		break;
	case PRINT_OP:
=======
	case TEP_PRINT_TYPE:
		free(arg->typecast.type);
		free_arg(arg->typecast.item);
		break;
	case TEP_PRINT_STRING:
	case TEP_PRINT_BSTRING:
		free(arg->string.string);
		break;
	case TEP_PRINT_BITMASK:
		free(arg->bitmask.bitmask);
		break;
	case TEP_PRINT_DYNAMIC_ARRAY:
	case TEP_PRINT_DYNAMIC_ARRAY_LEN:
		free(arg->dynarray.index);
		break;
	case TEP_PRINT_OP:
>>>>>>> upstream/android-13
		free(arg->op.op);
		free_arg(arg->op.left);
		free_arg(arg->op.right);
		break;
<<<<<<< HEAD
	case PRINT_FUNC:
=======
	case TEP_PRINT_FUNC:
>>>>>>> upstream/android-13
		while (arg->func.args) {
			farg = arg->func.args;
			arg->func.args = farg->next;
			free_arg(farg);
		}
		break;

<<<<<<< HEAD
	case PRINT_NULL:
=======
	case TEP_PRINT_NULL:
>>>>>>> upstream/android-13
	default:
		break;
	}

	free(arg);
}

<<<<<<< HEAD
static enum event_type get_type(int ch)
{
	if (ch == '\n')
		return EVENT_NEWLINE;
	if (isspace(ch))
		return EVENT_SPACE;
	if (isalnum(ch) || ch == '_')
		return EVENT_ITEM;
	if (ch == '\'')
		return EVENT_SQUOTE;
	if (ch == '"')
		return EVENT_DQUOTE;
	if (!isprint(ch))
		return EVENT_NONE;
	if (ch == '(' || ch == ')' || ch == ',')
		return EVENT_DELIM;

	return EVENT_OP;
=======
static enum tep_event_type get_type(int ch)
{
	if (ch == '\n')
		return TEP_EVENT_NEWLINE;
	if (isspace(ch))
		return TEP_EVENT_SPACE;
	if (isalnum(ch) || ch == '_')
		return TEP_EVENT_ITEM;
	if (ch == '\'')
		return TEP_EVENT_SQUOTE;
	if (ch == '"')
		return TEP_EVENT_DQUOTE;
	if (!isprint(ch))
		return TEP_EVENT_NONE;
	if (ch == '(' || ch == ')' || ch == ',')
		return TEP_EVENT_DELIM;

	return TEP_EVENT_OP;
>>>>>>> upstream/android-13
}

static int __read_char(void)
{
	if (input_buf_ptr >= input_buf_siz)
		return -1;

	return input_buf[input_buf_ptr++];
}

<<<<<<< HEAD
static int __peek_char(void)
=======
/**
 * peek_char - peek at the next character that will be read
 *
 * Returns the next character read, or -1 if end of buffer.
 */
__hidden int peek_char(void)
>>>>>>> upstream/android-13
{
	if (input_buf_ptr >= input_buf_siz)
		return -1;

	return input_buf[input_buf_ptr];
}

<<<<<<< HEAD
/**
 * tep_peek_char - peek at the next character that will be read
 *
 * Returns the next character read, or -1 if end of buffer.
 */
int tep_peek_char(void)
{
	return __peek_char();
}

=======
>>>>>>> upstream/android-13
static int extend_token(char **tok, char *buf, int size)
{
	char *newtok = realloc(*tok, size);

	if (!newtok) {
		free(*tok);
		*tok = NULL;
		return -1;
	}

	if (!*tok)
		strcpy(newtok, buf);
	else
		strcat(newtok, buf);
	*tok = newtok;

	return 0;
}

<<<<<<< HEAD
static enum event_type force_token(const char *str, char **tok);

static enum event_type __read_token(char **tok)
=======
static enum tep_event_type force_token(const char *str, char **tok);

static enum tep_event_type __read_token(char **tok)
>>>>>>> upstream/android-13
{
	char buf[BUFSIZ];
	int ch, last_ch, quote_ch, next_ch;
	int i = 0;
	int tok_size = 0;
<<<<<<< HEAD
	enum event_type type;
=======
	enum tep_event_type type;
>>>>>>> upstream/android-13

	*tok = NULL;


	ch = __read_char();
	if (ch < 0)
<<<<<<< HEAD
		return EVENT_NONE;

	type = get_type(ch);
	if (type == EVENT_NONE)
=======
		return TEP_EVENT_NONE;

	type = get_type(ch);
	if (type == TEP_EVENT_NONE)
>>>>>>> upstream/android-13
		return type;

	buf[i++] = ch;

	switch (type) {
<<<<<<< HEAD
	case EVENT_NEWLINE:
	case EVENT_DELIM:
		if (asprintf(tok, "%c", ch) < 0)
			return EVENT_ERROR;

		return type;

	case EVENT_OP:
		switch (ch) {
		case '-':
			next_ch = __peek_char();
=======
	case TEP_EVENT_NEWLINE:
	case TEP_EVENT_DELIM:
		if (asprintf(tok, "%c", ch) < 0)
			return TEP_EVENT_ERROR;

		return type;

	case TEP_EVENT_OP:
		switch (ch) {
		case '-':
			next_ch = peek_char();
>>>>>>> upstream/android-13
			if (next_ch == '>') {
				buf[i++] = __read_char();
				break;
			}
			/* fall through */
		case '+':
		case '|':
		case '&':
		case '>':
		case '<':
			last_ch = ch;
<<<<<<< HEAD
			ch = __peek_char();
=======
			ch = peek_char();
>>>>>>> upstream/android-13
			if (ch != last_ch)
				goto test_equal;
			buf[i++] = __read_char();
			switch (last_ch) {
			case '>':
			case '<':
				goto test_equal;
			default:
				break;
			}
			break;
		case '!':
		case '=':
			goto test_equal;
		default: /* what should we do instead? */
			break;
		}
		buf[i] = 0;
		*tok = strdup(buf);
		return type;

 test_equal:
<<<<<<< HEAD
		ch = __peek_char();
=======
		ch = peek_char();
>>>>>>> upstream/android-13
		if (ch == '=')
			buf[i++] = __read_char();
		goto out;

<<<<<<< HEAD
	case EVENT_DQUOTE:
	case EVENT_SQUOTE:
=======
	case TEP_EVENT_DQUOTE:
	case TEP_EVENT_SQUOTE:
>>>>>>> upstream/android-13
		/* don't keep quotes */
		i--;
		quote_ch = ch;
		last_ch = 0;
 concat:
		do {
			if (i == (BUFSIZ - 1)) {
				buf[i] = 0;
				tok_size += BUFSIZ;

				if (extend_token(tok, buf, tok_size) < 0)
<<<<<<< HEAD
					return EVENT_NONE;
=======
					return TEP_EVENT_NONE;
>>>>>>> upstream/android-13
				i = 0;
			}
			last_ch = ch;
			ch = __read_char();
			buf[i++] = ch;
			/* the '\' '\' will cancel itself */
			if (ch == '\\' && last_ch == '\\')
				last_ch = 0;
		} while (ch != quote_ch || last_ch == '\\');
		/* remove the last quote */
		i--;

		/*
		 * For strings (double quotes) check the next token.
		 * If it is another string, concatinate the two.
		 */
<<<<<<< HEAD
		if (type == EVENT_DQUOTE) {
=======
		if (type == TEP_EVENT_DQUOTE) {
>>>>>>> upstream/android-13
			unsigned long long save_input_buf_ptr = input_buf_ptr;

			do {
				ch = __read_char();
			} while (isspace(ch));
			if (ch == '"')
				goto concat;
			input_buf_ptr = save_input_buf_ptr;
		}

		goto out;

<<<<<<< HEAD
	case EVENT_ERROR ... EVENT_SPACE:
	case EVENT_ITEM:
=======
	case TEP_EVENT_ERROR ... TEP_EVENT_SPACE:
	case TEP_EVENT_ITEM:
>>>>>>> upstream/android-13
	default:
		break;
	}

<<<<<<< HEAD
	while (get_type(__peek_char()) == type) {
=======
	while (get_type(peek_char()) == type) {
>>>>>>> upstream/android-13
		if (i == (BUFSIZ - 1)) {
			buf[i] = 0;
			tok_size += BUFSIZ;

			if (extend_token(tok, buf, tok_size) < 0)
<<<<<<< HEAD
				return EVENT_NONE;
=======
				return TEP_EVENT_NONE;
>>>>>>> upstream/android-13
			i = 0;
		}
		ch = __read_char();
		buf[i++] = ch;
	}

 out:
	buf[i] = 0;
	if (extend_token(tok, buf, tok_size + i + 1) < 0)
<<<<<<< HEAD
		return EVENT_NONE;

	if (type == EVENT_ITEM) {
=======
		return TEP_EVENT_NONE;

	if (type == TEP_EVENT_ITEM) {
>>>>>>> upstream/android-13
		/*
		 * Older versions of the kernel has a bug that
		 * creates invalid symbols and will break the mac80211
		 * parsing. This is a work around to that bug.
		 *
		 * See Linux kernel commit:
		 *  811cb50baf63461ce0bdb234927046131fc7fa8b
		 */
		if (strcmp(*tok, "LOCAL_PR_FMT") == 0) {
			free(*tok);
			*tok = NULL;
			return force_token("\"%s\" ", tok);
		} else if (strcmp(*tok, "STA_PR_FMT") == 0) {
			free(*tok);
			*tok = NULL;
			return force_token("\" sta:%pM\" ", tok);
		} else if (strcmp(*tok, "VIF_PR_FMT") == 0) {
			free(*tok);
			*tok = NULL;
			return force_token("\" vif:%p(%d)\" ", tok);
		}
	}

	return type;
}

<<<<<<< HEAD
static enum event_type force_token(const char *str, char **tok)
=======
static enum tep_event_type force_token(const char *str, char **tok)
>>>>>>> upstream/android-13
{
	const char *save_input_buf;
	unsigned long long save_input_buf_ptr;
	unsigned long long save_input_buf_siz;
<<<<<<< HEAD
	enum event_type type;
=======
	enum tep_event_type type;
>>>>>>> upstream/android-13
	
	/* save off the current input pointers */
	save_input_buf = input_buf;
	save_input_buf_ptr = input_buf_ptr;
	save_input_buf_siz = input_buf_siz;

	init_input_buf(str, strlen(str));

	type = __read_token(tok);

	/* reset back to original token */
	input_buf = save_input_buf;
	input_buf_ptr = save_input_buf_ptr;
	input_buf_siz = save_input_buf_siz;

	return type;
}

<<<<<<< HEAD
static void free_token(char *tok)
=======
/**
 * free_token - free a token returned by tep_read_token
 * @token: the token to free
 */
__hidden void free_token(char *tok)
>>>>>>> upstream/android-13
{
	if (tok)
		free(tok);
}

<<<<<<< HEAD
static enum event_type read_token(char **tok)
{
	enum event_type type;

	for (;;) {
		type = __read_token(tok);
		if (type != EVENT_SPACE)
			return type;

		free_token(*tok);
	}

	/* not reached */
	*tok = NULL;
	return EVENT_NONE;
}

/**
 * tep_read_token - access to utilites to use the pevent parser
=======
/**
 * read_token - access to utilities to use the tep parser
>>>>>>> upstream/android-13
 * @tok: The token to return
 *
 * This will parse tokens from the string given by
 * tep_init_data().
 *
 * Returns the token type.
 */
<<<<<<< HEAD
enum event_type tep_read_token(char **tok)
{
	return read_token(tok);
}

/**
 * tep_free_token - free a token returned by tep_read_token
 * @token: the token to free
 */
void tep_free_token(char *token)
{
	free_token(token);
}

/* no newline */
static enum event_type read_token_item(char **tok)
{
	enum event_type type;

	for (;;) {
		type = __read_token(tok);
		if (type != EVENT_SPACE && type != EVENT_NEWLINE)
=======
__hidden enum tep_event_type read_token(char **tok)
{
	enum tep_event_type type;

	for (;;) {
		type = __read_token(tok);
		if (type != TEP_EVENT_SPACE)
			return type;

		free_token(*tok);
	}

	/* not reached */
	*tok = NULL;
	return TEP_EVENT_NONE;
}

/* no newline */
static enum tep_event_type read_token_item(char **tok)
{
	enum tep_event_type type;

	for (;;) {
		type = __read_token(tok);
		if (type != TEP_EVENT_SPACE && type != TEP_EVENT_NEWLINE)
>>>>>>> upstream/android-13
			return type;
		free_token(*tok);
		*tok = NULL;
	}

	/* not reached */
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_NONE;
}

static int test_type(enum event_type type, enum event_type expect)
=======
	return TEP_EVENT_NONE;
}

static int test_type(enum tep_event_type type, enum tep_event_type expect)
>>>>>>> upstream/android-13
{
	if (type != expect) {
		do_warning("Error: expected type %d but read %d",
		    expect, type);
		return -1;
	}
	return 0;
}

<<<<<<< HEAD
static int test_type_token(enum event_type type, const char *token,
		    enum event_type expect, const char *expect_tok)
=======
static int test_type_token(enum tep_event_type type, const char *token,
		    enum tep_event_type expect, const char *expect_tok)
>>>>>>> upstream/android-13
{
	if (type != expect) {
		do_warning("Error: expected type %d but read %d",
		    expect, type);
		return -1;
	}

	if (strcmp(token, expect_tok) != 0) {
		do_warning("Error: expected '%s' but read '%s'",
		    expect_tok, token);
		return -1;
	}
	return 0;
}

<<<<<<< HEAD
static int __read_expect_type(enum event_type expect, char **tok, int newline_ok)
{
	enum event_type type;
=======
static int __read_expect_type(enum tep_event_type expect, char **tok, int newline_ok)
{
	enum tep_event_type type;
>>>>>>> upstream/android-13

	if (newline_ok)
		type = read_token(tok);
	else
		type = read_token_item(tok);
	return test_type(type, expect);
}

<<<<<<< HEAD
static int read_expect_type(enum event_type expect, char **tok)
=======
static int read_expect_type(enum tep_event_type expect, char **tok)
>>>>>>> upstream/android-13
{
	return __read_expect_type(expect, tok, 1);
}

<<<<<<< HEAD
static int __read_expected(enum event_type expect, const char *str,
			   int newline_ok)
{
	enum event_type type;
=======
static int __read_expected(enum tep_event_type expect, const char *str,
			   int newline_ok)
{
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token;
	int ret;

	if (newline_ok)
		type = read_token(&token);
	else
		type = read_token_item(&token);

	ret = test_type_token(type, token, expect, str);

	free_token(token);

	return ret;
}

<<<<<<< HEAD
static int read_expected(enum event_type expect, const char *str)
=======
static int read_expected(enum tep_event_type expect, const char *str)
>>>>>>> upstream/android-13
{
	return __read_expected(expect, str, 1);
}

<<<<<<< HEAD
static int read_expected_item(enum event_type expect, const char *str)
=======
static int read_expected_item(enum tep_event_type expect, const char *str)
>>>>>>> upstream/android-13
{
	return __read_expected(expect, str, 0);
}

static char *event_read_name(void)
{
	char *token;

<<<<<<< HEAD
	if (read_expected(EVENT_ITEM, "name") < 0)
		return NULL;

	if (read_expected(EVENT_OP, ":") < 0)
		return NULL;

	if (read_expect_type(EVENT_ITEM, &token) < 0)
=======
	if (read_expected(TEP_EVENT_ITEM, "name") < 0)
		return NULL;

	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return NULL;

	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
>>>>>>> upstream/android-13
		goto fail;

	return token;

 fail:
	free_token(token);
	return NULL;
}

static int event_read_id(void)
{
	char *token;
	int id;

<<<<<<< HEAD
	if (read_expected_item(EVENT_ITEM, "ID") < 0)
		return -1;

	if (read_expected(EVENT_OP, ":") < 0)
		return -1;

	if (read_expect_type(EVENT_ITEM, &token) < 0)
=======
	if (read_expected_item(TEP_EVENT_ITEM, "ID") < 0)
		return -1;

	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return -1;

	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
>>>>>>> upstream/android-13
		goto fail;

	id = strtoul(token, NULL, 0);
	free_token(token);
	return id;

 fail:
	free_token(token);
	return -1;
}

<<<<<<< HEAD
static int field_is_string(struct format_field *field)
{
	if ((field->flags & FIELD_IS_ARRAY) &&
=======
static int field_is_string(struct tep_format_field *field)
{
	if ((field->flags & TEP_FIELD_IS_ARRAY) &&
>>>>>>> upstream/android-13
	    (strstr(field->type, "char") || strstr(field->type, "u8") ||
	     strstr(field->type, "s8")))
		return 1;

	return 0;
}

<<<<<<< HEAD
static int field_is_dynamic(struct format_field *field)
=======
static int field_is_dynamic(struct tep_format_field *field)
>>>>>>> upstream/android-13
{
	if (strncmp(field->type, "__data_loc", 10) == 0)
		return 1;

	return 0;
}

<<<<<<< HEAD
static int field_is_long(struct format_field *field)
=======
static int field_is_long(struct tep_format_field *field)
>>>>>>> upstream/android-13
{
	/* includes long long */
	if (strstr(field->type, "long"))
		return 1;

	return 0;
}

static unsigned int type_size(const char *name)
{
<<<<<<< HEAD
	/* This covers all FIELD_IS_STRING types. */
=======
	/* This covers all TEP_FIELD_IS_STRING types. */
>>>>>>> upstream/android-13
	static struct {
		const char *type;
		unsigned int size;
	} table[] = {
		{ "u8",   1 },
		{ "u16",  2 },
		{ "u32",  4 },
		{ "u64",  8 },
		{ "s8",   1 },
		{ "s16",  2 },
		{ "s32",  4 },
		{ "s64",  8 },
		{ "char", 1 },
		{ },
	};
	int i;

	for (i = 0; table[i].type; i++) {
		if (!strcmp(table[i].type, name))
			return table[i].size;
	}

	return 0;
}

<<<<<<< HEAD
static int event_read_fields(struct event_format *event, struct format_field **fields)
{
	struct format_field *field = NULL;
	enum event_type type;
	char *token;
	char *last_token;
	int count = 0;
=======
static int append(char **buf, const char *delim, const char *str)
{
	char *new_buf;

	new_buf = realloc(*buf, strlen(*buf) + strlen(delim) + strlen(str) + 1);
	if (!new_buf)
		return -1;
	strcat(new_buf, delim);
	strcat(new_buf, str);
	*buf = new_buf;
	return 0;
}

static int event_read_fields(struct tep_event *event, struct tep_format_field **fields)
{
	struct tep_format_field *field = NULL;
	enum tep_event_type type;
	char *token;
	char *last_token;
	char *delim = " ";
	int count = 0;
	int ret;
>>>>>>> upstream/android-13

	do {
		unsigned int size_dynamic = 0;

		type = read_token(&token);
<<<<<<< HEAD
		if (type == EVENT_NEWLINE) {
=======
		if (type == TEP_EVENT_NEWLINE) {
>>>>>>> upstream/android-13
			free_token(token);
			return count;
		}

		count++;

<<<<<<< HEAD
		if (test_type_token(type, token, EVENT_ITEM, "field"))
=======
		if (test_type_token(type, token, TEP_EVENT_ITEM, "field"))
>>>>>>> upstream/android-13
			goto fail;
		free_token(token);

		type = read_token(&token);
		/*
		 * The ftrace fields may still use the "special" name.
		 * Just ignore it.
		 */
<<<<<<< HEAD
		if (event->flags & EVENT_FL_ISFTRACE &&
		    type == EVENT_ITEM && strcmp(token, "special") == 0) {
=======
		if (event->flags & TEP_EVENT_FL_ISFTRACE &&
		    type == TEP_EVENT_ITEM && strcmp(token, "special") == 0) {
>>>>>>> upstream/android-13
			free_token(token);
			type = read_token(&token);
		}

<<<<<<< HEAD
		if (test_type_token(type, token, EVENT_OP, ":") < 0)
			goto fail;

		free_token(token);
		if (read_expect_type(EVENT_ITEM, &token) < 0)
=======
		if (test_type_token(type, token, TEP_EVENT_OP, ":") < 0)
			goto fail;

		free_token(token);
		if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
>>>>>>> upstream/android-13
			goto fail;

		last_token = token;

		field = calloc(1, sizeof(*field));
		if (!field)
			goto fail;

		field->event = event;

		/* read the rest of the type */
		for (;;) {
			type = read_token(&token);
<<<<<<< HEAD
			if (type == EVENT_ITEM ||
			    (type == EVENT_OP && strcmp(token, "*") == 0) ||
=======
			if (type == TEP_EVENT_ITEM ||
			    (type == TEP_EVENT_OP && strcmp(token, "*") == 0) ||
>>>>>>> upstream/android-13
			    /*
			     * Some of the ftrace fields are broken and have
			     * an illegal "." in them.
			     */
<<<<<<< HEAD
			    (event->flags & EVENT_FL_ISFTRACE &&
			     type == EVENT_OP && strcmp(token, ".") == 0)) {

				if (strcmp(token, "*") == 0)
					field->flags |= FIELD_IS_POINTER;

				if (field->type) {
					char *new_type;
					new_type = realloc(field->type,
							   strlen(field->type) +
							   strlen(last_token) + 2);
					if (!new_type) {
						free(last_token);
						goto fail;
					}
					field->type = new_type;
					strcat(field->type, " ");
					strcat(field->type, last_token);
					free(last_token);
				} else
					field->type = last_token;
				last_token = token;
				continue;
			}

=======
			    (event->flags & TEP_EVENT_FL_ISFTRACE &&
			     type == TEP_EVENT_OP && strcmp(token, ".") == 0)) {

				if (strcmp(token, "*") == 0)
					field->flags |= TEP_FIELD_IS_POINTER;

				if (field->type) {
					ret = append(&field->type, delim, last_token);
					free(last_token);
					if (ret < 0)
						goto fail;
				} else
					field->type = last_token;
				last_token = token;
				delim = " ";
				continue;
			}

			/* Handle __attribute__((user)) */
			if ((type == TEP_EVENT_DELIM) &&
			    strcmp("__attribute__", last_token) == 0 &&
			    token[0] == '(') {
				int depth = 1;
				int ret;

				ret = append(&field->type, " ", last_token);
				ret |= append(&field->type, "", "(");
				if (ret < 0)
					goto fail;

				delim = " ";
				while ((type = read_token(&token)) != TEP_EVENT_NONE) {
					if (type == TEP_EVENT_DELIM) {
						if (token[0] == '(')
							depth++;
						else if (token[0] == ')')
							depth--;
						if (!depth)
							break;
						ret = append(&field->type, "", token);
						delim = "";
					} else {
						ret = append(&field->type, delim, token);
						delim = " ";
					}
					if (ret < 0)
						goto fail;
					free(last_token);
					last_token = token;
				}
				continue;
			}
>>>>>>> upstream/android-13
			break;
		}

		if (!field->type) {
			do_warning_event(event, "%s: no type found", __func__);
			goto fail;
		}
		field->name = field->alias = last_token;

<<<<<<< HEAD
		if (test_type(type, EVENT_OP))
			goto fail;

		if (strcmp(token, "[") == 0) {
			enum event_type last_type = type;
			char *brackets = token;
			char *new_brackets;
			int len;

			field->flags |= FIELD_IS_ARRAY;

			type = read_token(&token);

			if (type == EVENT_ITEM)
=======
		if (test_type(type, TEP_EVENT_OP))
			goto fail;

		if (strcmp(token, "[") == 0) {
			enum tep_event_type last_type = type;
			char *brackets = token;

			field->flags |= TEP_FIELD_IS_ARRAY;

			type = read_token(&token);

			if (type == TEP_EVENT_ITEM)
>>>>>>> upstream/android-13
				field->arraylen = strtoul(token, NULL, 0);
			else
				field->arraylen = 0;

		        while (strcmp(token, "]") != 0) {
<<<<<<< HEAD
				if (last_type == EVENT_ITEM &&
				    type == EVENT_ITEM)
					len = 2;
				else
					len = 1;
				last_type = type;

				new_brackets = realloc(brackets,
						       strlen(brackets) +
						       strlen(token) + len);
				if (!new_brackets) {
					free(brackets);
					goto fail;
				}
				brackets = new_brackets;
				if (len == 2)
					strcat(brackets, " ");
				strcat(brackets, token);
=======
				const char *delim;

				if (last_type == TEP_EVENT_ITEM &&
				    type == TEP_EVENT_ITEM)
					delim = " ";
				else
					delim = "";

				last_type = type;

				ret = append(&brackets, delim, token);
				if (ret < 0) {
					free(brackets);
					goto fail;
				}
>>>>>>> upstream/android-13
				/* We only care about the last token */
				field->arraylen = strtoul(token, NULL, 0);
				free_token(token);
				type = read_token(&token);
<<<<<<< HEAD
				if (type == EVENT_NONE) {
=======
				if (type == TEP_EVENT_NONE) {
					free(brackets);
>>>>>>> upstream/android-13
					do_warning_event(event, "failed to find token");
					goto fail;
				}
			}

			free_token(token);

<<<<<<< HEAD
			new_brackets = realloc(brackets, strlen(brackets) + 2);
			if (!new_brackets) {
				free(brackets);
				goto fail;
			}
			brackets = new_brackets;
			strcat(brackets, "]");
=======
			ret = append(&brackets, "", "]");
			if (ret < 0) {
				free(brackets);
				goto fail;
			}
>>>>>>> upstream/android-13

			/* add brackets to type */

			type = read_token(&token);
			/*
			 * If the next token is not an OP, then it is of
			 * the format: type [] item;
			 */
<<<<<<< HEAD
			if (type == EVENT_ITEM) {
				char *new_type;
				new_type = realloc(field->type,
						   strlen(field->type) +
						   strlen(field->name) +
						   strlen(brackets) + 2);
				if (!new_type) {
					free(brackets);
					goto fail;
				}
				field->type = new_type;
				strcat(field->type, " ");
				strcat(field->type, field->name);
				size_dynamic = type_size(field->name);
				free_token(field->name);
				strcat(field->type, brackets);
				field->name = field->alias = token;
				type = read_token(&token);
			} else {
				char *new_type;
				new_type = realloc(field->type,
						   strlen(field->type) +
						   strlen(brackets) + 1);
				if (!new_type) {
					free(brackets);
					goto fail;
				}
				field->type = new_type;
				strcat(field->type, brackets);
=======
			if (type == TEP_EVENT_ITEM) {
				ret = append(&field->type, " ", field->name);
				if (ret < 0) {
					free(brackets);
					goto fail;
				}
				ret = append(&field->type, "", brackets);

				size_dynamic = type_size(field->name);
				free_token(field->name);
				field->name = field->alias = token;
				type = read_token(&token);
			} else {
				ret = append(&field->type, "", brackets);
				if (ret < 0) {
					free(brackets);
					goto fail;
				}
>>>>>>> upstream/android-13
			}
			free(brackets);
		}

		if (field_is_string(field))
<<<<<<< HEAD
			field->flags |= FIELD_IS_STRING;
		if (field_is_dynamic(field))
			field->flags |= FIELD_IS_DYNAMIC;
		if (field_is_long(field))
			field->flags |= FIELD_IS_LONG;

		if (test_type_token(type, token,  EVENT_OP, ";"))
			goto fail;
		free_token(token);

		if (read_expected(EVENT_ITEM, "offset") < 0)
			goto fail_expect;

		if (read_expected(EVENT_OP, ":") < 0)
			goto fail_expect;

		if (read_expect_type(EVENT_ITEM, &token))
=======
			field->flags |= TEP_FIELD_IS_STRING;
		if (field_is_dynamic(field))
			field->flags |= TEP_FIELD_IS_DYNAMIC;
		if (field_is_long(field))
			field->flags |= TEP_FIELD_IS_LONG;

		if (test_type_token(type, token,  TEP_EVENT_OP, ";"))
			goto fail;
		free_token(token);

		if (read_expected(TEP_EVENT_ITEM, "offset") < 0)
			goto fail_expect;

		if (read_expected(TEP_EVENT_OP, ":") < 0)
			goto fail_expect;

		if (read_expect_type(TEP_EVENT_ITEM, &token))
>>>>>>> upstream/android-13
			goto fail;
		field->offset = strtoul(token, NULL, 0);
		free_token(token);

<<<<<<< HEAD
		if (read_expected(EVENT_OP, ";") < 0)
			goto fail_expect;

		if (read_expected(EVENT_ITEM, "size") < 0)
			goto fail_expect;

		if (read_expected(EVENT_OP, ":") < 0)
			goto fail_expect;

		if (read_expect_type(EVENT_ITEM, &token))
=======
		if (read_expected(TEP_EVENT_OP, ";") < 0)
			goto fail_expect;

		if (read_expected(TEP_EVENT_ITEM, "size") < 0)
			goto fail_expect;

		if (read_expected(TEP_EVENT_OP, ":") < 0)
			goto fail_expect;

		if (read_expect_type(TEP_EVENT_ITEM, &token))
>>>>>>> upstream/android-13
			goto fail;
		field->size = strtoul(token, NULL, 0);
		free_token(token);

<<<<<<< HEAD
		if (read_expected(EVENT_OP, ";") < 0)
			goto fail_expect;

		type = read_token(&token);
		if (type != EVENT_NEWLINE) {
			/* newer versions of the kernel have a "signed" type */
			if (test_type_token(type, token, EVENT_ITEM, "signed"))
=======
		if (read_expected(TEP_EVENT_OP, ";") < 0)
			goto fail_expect;

		type = read_token(&token);
		if (type != TEP_EVENT_NEWLINE) {
			/* newer versions of the kernel have a "signed" type */
			if (test_type_token(type, token, TEP_EVENT_ITEM, "signed"))
>>>>>>> upstream/android-13
				goto fail;

			free_token(token);

<<<<<<< HEAD
			if (read_expected(EVENT_OP, ":") < 0)
				goto fail_expect;

			if (read_expect_type(EVENT_ITEM, &token))
				goto fail;

			if (strtoul(token, NULL, 0))
				field->flags |= FIELD_IS_SIGNED;

			free_token(token);
			if (read_expected(EVENT_OP, ";") < 0)
				goto fail_expect;

			if (read_expect_type(EVENT_NEWLINE, &token))
=======
			if (read_expected(TEP_EVENT_OP, ":") < 0)
				goto fail_expect;

			if (read_expect_type(TEP_EVENT_ITEM, &token))
				goto fail;

			if (strtoul(token, NULL, 0))
				field->flags |= TEP_FIELD_IS_SIGNED;

			free_token(token);
			if (read_expected(TEP_EVENT_OP, ";") < 0)
				goto fail_expect;

			if (read_expect_type(TEP_EVENT_NEWLINE, &token))
>>>>>>> upstream/android-13
				goto fail;
		}

		free_token(token);

<<<<<<< HEAD
		if (field->flags & FIELD_IS_ARRAY) {
			if (field->arraylen)
				field->elementsize = field->size / field->arraylen;
			else if (field->flags & FIELD_IS_DYNAMIC)
				field->elementsize = size_dynamic;
			else if (field->flags & FIELD_IS_STRING)
				field->elementsize = 1;
			else if (field->flags & FIELD_IS_LONG)
				field->elementsize = event->pevent ?
						     event->pevent->long_size :
=======
		if (field->flags & TEP_FIELD_IS_ARRAY) {
			if (field->arraylen)
				field->elementsize = field->size / field->arraylen;
			else if (field->flags & TEP_FIELD_IS_DYNAMIC)
				field->elementsize = size_dynamic;
			else if (field->flags & TEP_FIELD_IS_STRING)
				field->elementsize = 1;
			else if (field->flags & TEP_FIELD_IS_LONG)
				field->elementsize = event->tep ?
						     event->tep->long_size :
>>>>>>> upstream/android-13
						     sizeof(long);
		} else
			field->elementsize = field->size;

		*fields = field;
		fields = &field->next;

	} while (1);

	return 0;

fail:
	free_token(token);
fail_expect:
	if (field) {
		free(field->type);
		free(field->name);
		free(field);
	}
	return -1;
}

<<<<<<< HEAD
static int event_read_format(struct event_format *event)
=======
static int event_read_format(struct tep_event *event)
>>>>>>> upstream/android-13
{
	char *token;
	int ret;

<<<<<<< HEAD
	if (read_expected_item(EVENT_ITEM, "format") < 0)
		return -1;

	if (read_expected(EVENT_OP, ":") < 0)
		return -1;

	if (read_expect_type(EVENT_NEWLINE, &token))
=======
	if (read_expected_item(TEP_EVENT_ITEM, "format") < 0)
		return -1;

	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return -1;

	if (read_expect_type(TEP_EVENT_NEWLINE, &token))
>>>>>>> upstream/android-13
		goto fail;
	free_token(token);

	ret = event_read_fields(event, &event->format.common_fields);
	if (ret < 0)
		return ret;
	event->format.nr_common = ret;

	ret = event_read_fields(event, &event->format.fields);
	if (ret < 0)
		return ret;
	event->format.nr_fields = ret;

	return 0;

 fail:
	free_token(token);
	return -1;
}

<<<<<<< HEAD
static enum event_type
process_arg_token(struct event_format *event, struct print_arg *arg,
		  char **tok, enum event_type type);

static enum event_type
process_arg(struct event_format *event, struct print_arg *arg, char **tok)
{
	enum event_type type;
=======
static enum tep_event_type
process_arg_token(struct tep_event *event, struct tep_print_arg *arg,
		  char **tok, enum tep_event_type type);

static enum tep_event_type
process_arg(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token;

	type = read_token(&token);
	*tok = token;

	return process_arg_token(event, arg, tok, type);
}

<<<<<<< HEAD
static enum event_type
process_op(struct event_format *event, struct print_arg *arg, char **tok);
=======
static enum tep_event_type
process_op(struct tep_event *event, struct tep_print_arg *arg, char **tok);
>>>>>>> upstream/android-13

/*
 * For __print_symbolic() and __print_flags, we need to completely
 * evaluate the first argument, which defines what to print next.
 */
<<<<<<< HEAD
static enum event_type
process_field_arg(struct event_format *event, struct print_arg *arg, char **tok)
{
	enum event_type type;

	type = process_arg(event, arg, tok);

	while (type == EVENT_OP) {
=======
static enum tep_event_type
process_field_arg(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	enum tep_event_type type;

	type = process_arg(event, arg, tok);

	while (type == TEP_EVENT_OP) {
>>>>>>> upstream/android-13
		type = process_op(event, arg, tok);
	}

	return type;
}

<<<<<<< HEAD
static enum event_type
process_cond(struct event_format *event, struct print_arg *top, char **tok)
{
	struct print_arg *arg, *left, *right;
	enum event_type type;
=======
static enum tep_event_type
process_cond(struct tep_event *event, struct tep_print_arg *top, char **tok)
{
	struct tep_print_arg *arg, *left, *right;
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token = NULL;

	arg = alloc_arg();
	left = alloc_arg();
	right = alloc_arg();

	if (!arg || !left || !right) {
		do_warning_event(event, "%s: not enough memory!", __func__);
		/* arg will be freed at out_free */
		free_arg(left);
		free_arg(right);
		goto out_free;
	}

<<<<<<< HEAD
	arg->type = PRINT_OP;
=======
	arg->type = TEP_PRINT_OP;
>>>>>>> upstream/android-13
	arg->op.left = left;
	arg->op.right = right;

	*tok = NULL;
	type = process_arg(event, left, &token);

 again:
<<<<<<< HEAD
	if (type == EVENT_ERROR)
		goto out_free;

	/* Handle other operations in the arguments */
	if (type == EVENT_OP && strcmp(token, ":") != 0) {
=======
	if (type == TEP_EVENT_ERROR)
		goto out_free;

	/* Handle other operations in the arguments */
	if (type == TEP_EVENT_OP && strcmp(token, ":") != 0) {
>>>>>>> upstream/android-13
		type = process_op(event, left, &token);
		goto again;
	}

<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_OP, ":"))
=======
	if (test_type_token(type, token, TEP_EVENT_OP, ":"))
>>>>>>> upstream/android-13
		goto out_free;

	arg->op.op = token;

	type = process_arg(event, right, &token);

	top->op.right = arg;

	*tok = token;
	return type;

out_free:
	/* Top may point to itself */
	top->op.right = NULL;
	free_token(token);
	free_arg(arg);
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_array(struct event_format *event, struct print_arg *top, char **tok)
{
	struct print_arg *arg;
	enum event_type type;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_array(struct tep_event *event, struct tep_print_arg *top, char **tok)
{
	struct tep_print_arg *arg;
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token = NULL;

	arg = alloc_arg();
	if (!arg) {
		do_warning_event(event, "%s: not enough memory!", __func__);
		/* '*tok' is set to top->op.op.  No need to free. */
		*tok = NULL;
<<<<<<< HEAD
		return EVENT_ERROR;
=======
		return TEP_EVENT_ERROR;
>>>>>>> upstream/android-13
	}

	*tok = NULL;
	type = process_arg(event, arg, &token);
<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_OP, "]"))
=======
	if (test_type_token(type, token, TEP_EVENT_OP, "]"))
>>>>>>> upstream/android-13
		goto out_free;

	top->op.right = arg;

	free_token(token);
	type = read_token_item(&token);
	*tok = token;

	return type;

out_free:
	free_token(token);
	free_arg(arg);
<<<<<<< HEAD
	return EVENT_ERROR;
=======
	return TEP_EVENT_ERROR;
>>>>>>> upstream/android-13
}

static int get_op_prio(char *op)
{
	if (!op[1]) {
		switch (op[0]) {
		case '~':
		case '!':
			return 4;
		case '*':
		case '/':
		case '%':
			return 6;
		case '+':
		case '-':
			return 7;
			/* '>>' and '<<' are 8 */
		case '<':
		case '>':
			return 9;
			/* '==' and '!=' are 10 */
		case '&':
			return 11;
		case '^':
			return 12;
		case '|':
			return 13;
		case '?':
			return 16;
		default:
			do_warning("unknown op '%c'", op[0]);
			return -1;
		}
	} else {
		if (strcmp(op, "++") == 0 ||
		    strcmp(op, "--") == 0) {
			return 3;
		} else if (strcmp(op, ">>") == 0 ||
			   strcmp(op, "<<") == 0) {
			return 8;
		} else if (strcmp(op, ">=") == 0 ||
			   strcmp(op, "<=") == 0) {
			return 9;
		} else if (strcmp(op, "==") == 0 ||
			   strcmp(op, "!=") == 0) {
			return 10;
		} else if (strcmp(op, "&&") == 0) {
			return 14;
		} else if (strcmp(op, "||") == 0) {
			return 15;
		} else {
			do_warning("unknown op '%s'", op);
			return -1;
		}
	}
}

<<<<<<< HEAD
static int set_op_prio(struct print_arg *arg)
{

	/* single ops are the greatest */
	if (!arg->op.left || arg->op.left->type == PRINT_NULL)
=======
static int set_op_prio(struct tep_print_arg *arg)
{

	/* single ops are the greatest */
	if (!arg->op.left || arg->op.left->type == TEP_PRINT_NULL)
>>>>>>> upstream/android-13
		arg->op.prio = 0;
	else
		arg->op.prio = get_op_prio(arg->op.op);

	return arg->op.prio;
}

/* Note, *tok does not get freed, but will most likely be saved */
<<<<<<< HEAD
static enum event_type
process_op(struct event_format *event, struct print_arg *arg, char **tok)
{
	struct print_arg *left, *right = NULL;
	enum event_type type;
=======
static enum tep_event_type
process_op(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	struct tep_print_arg *left, *right = NULL;
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token;

	/* the op is passed in via tok */
	token = *tok;

<<<<<<< HEAD
	if (arg->type == PRINT_OP && !arg->op.left) {
=======
	if (arg->type == TEP_PRINT_OP && !arg->op.left) {
>>>>>>> upstream/android-13
		/* handle single op */
		if (token[1]) {
			do_warning_event(event, "bad op token %s", token);
			goto out_free;
		}
		switch (token[0]) {
		case '~':
		case '!':
		case '+':
		case '-':
			break;
		default:
			do_warning_event(event, "bad op token %s", token);
			goto out_free;

		}

		/* make an empty left */
		left = alloc_arg();
		if (!left)
			goto out_warn_free;

<<<<<<< HEAD
		left->type = PRINT_NULL;
=======
		left->type = TEP_PRINT_NULL;
>>>>>>> upstream/android-13
		arg->op.left = left;

		right = alloc_arg();
		if (!right)
			goto out_warn_free;

		arg->op.right = right;

		/* do not free the token, it belongs to an op */
		*tok = NULL;
		type = process_arg(event, right, tok);

	} else if (strcmp(token, "?") == 0) {

		left = alloc_arg();
		if (!left)
			goto out_warn_free;

		/* copy the top arg to the left */
		*left = *arg;

<<<<<<< HEAD
		arg->type = PRINT_OP;
=======
		arg->type = TEP_PRINT_OP;
>>>>>>> upstream/android-13
		arg->op.op = token;
		arg->op.left = left;
		arg->op.prio = 0;

		/* it will set arg->op.right */
		type = process_cond(event, arg, tok);

	} else if (strcmp(token, ">>") == 0 ||
		   strcmp(token, "<<") == 0 ||
		   strcmp(token, "&") == 0 ||
		   strcmp(token, "|") == 0 ||
		   strcmp(token, "&&") == 0 ||
		   strcmp(token, "||") == 0 ||
		   strcmp(token, "-") == 0 ||
		   strcmp(token, "+") == 0 ||
		   strcmp(token, "*") == 0 ||
		   strcmp(token, "^") == 0 ||
		   strcmp(token, "/") == 0 ||
		   strcmp(token, "%") == 0 ||
		   strcmp(token, "<") == 0 ||
		   strcmp(token, ">") == 0 ||
		   strcmp(token, "<=") == 0 ||
		   strcmp(token, ">=") == 0 ||
		   strcmp(token, "==") == 0 ||
		   strcmp(token, "!=") == 0) {

		left = alloc_arg();
		if (!left)
			goto out_warn_free;

		/* copy the top arg to the left */
		*left = *arg;

<<<<<<< HEAD
		arg->type = PRINT_OP;
=======
		arg->type = TEP_PRINT_OP;
>>>>>>> upstream/android-13
		arg->op.op = token;
		arg->op.left = left;
		arg->op.right = NULL;

		if (set_op_prio(arg) == -1) {
<<<<<<< HEAD
			event->flags |= EVENT_FL_FAILED;
=======
			event->flags |= TEP_EVENT_FL_FAILED;
>>>>>>> upstream/android-13
			/* arg->op.op (= token) will be freed at out_free */
			arg->op.op = NULL;
			goto out_free;
		}

		type = read_token_item(&token);
		*tok = token;

		/* could just be a type pointer */
		if ((strcmp(arg->op.op, "*") == 0) &&
<<<<<<< HEAD
		    type == EVENT_DELIM && (strcmp(token, ")") == 0)) {
			char *new_atom;

			if (left->type != PRINT_ATOM) {
				do_warning_event(event, "bad pointer type");
				goto out_free;
			}
			new_atom = realloc(left->atom.atom,
					    strlen(left->atom.atom) + 3);
			if (!new_atom)
				goto out_warn_free;

			left->atom.atom = new_atom;
			strcat(left->atom.atom, " *");
=======
		    type == TEP_EVENT_DELIM && (strcmp(token, ")") == 0)) {
			int ret;

			if (left->type != TEP_PRINT_ATOM) {
				do_warning_event(event, "bad pointer type");
				goto out_free;
			}
			ret = append(&left->atom.atom, " ", "*");
			if (ret < 0)
				goto out_warn_free;

>>>>>>> upstream/android-13
			free(arg->op.op);
			*arg = *left;
			free(left);

			return type;
		}

		right = alloc_arg();
		if (!right)
			goto out_warn_free;

		type = process_arg_token(event, right, tok, type);
<<<<<<< HEAD
		if (type == EVENT_ERROR) {
=======
		if (type == TEP_EVENT_ERROR) {
>>>>>>> upstream/android-13
			free_arg(right);
			/* token was freed in process_arg_token() via *tok */
			token = NULL;
			goto out_free;
		}

<<<<<<< HEAD
		if (right->type == PRINT_OP &&
		    get_op_prio(arg->op.op) < get_op_prio(right->op.op)) {
			struct print_arg tmp;
=======
		if (right->type == TEP_PRINT_OP &&
		    get_op_prio(arg->op.op) < get_op_prio(right->op.op)) {
			struct tep_print_arg tmp;
>>>>>>> upstream/android-13

			/* rotate ops according to the priority */
			arg->op.right = right->op.left;

			tmp = *arg;
			*arg = *right;
			*right = tmp;

			arg->op.left = right;
		} else {
			arg->op.right = right;
		}

	} else if (strcmp(token, "[") == 0) {

		left = alloc_arg();
		if (!left)
			goto out_warn_free;

		*left = *arg;

<<<<<<< HEAD
		arg->type = PRINT_OP;
=======
		arg->type = TEP_PRINT_OP;
>>>>>>> upstream/android-13
		arg->op.op = token;
		arg->op.left = left;

		arg->op.prio = 0;

		/* it will set arg->op.right */
		type = process_array(event, arg, tok);

	} else {
		do_warning_event(event, "unknown op '%s'", token);
<<<<<<< HEAD
		event->flags |= EVENT_FL_FAILED;
=======
		event->flags |= TEP_EVENT_FL_FAILED;
>>>>>>> upstream/android-13
		/* the arg is now the left side */
		goto out_free;
	}

<<<<<<< HEAD
	if (type == EVENT_OP && strcmp(*tok, ":") != 0) {
=======
	if (type == TEP_EVENT_OP && strcmp(*tok, ":") != 0) {
>>>>>>> upstream/android-13
		int prio;

		/* higher prios need to be closer to the root */
		prio = get_op_prio(*tok);

		if (prio > arg->op.prio)
			return process_op(event, arg, tok);

		return process_op(event, right, tok);
	}

	return type;

out_warn_free:
	do_warning_event(event, "%s: not enough memory!", __func__);
out_free:
	free_token(token);
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_entry(struct event_format *event __maybe_unused, struct print_arg *arg,
	      char **tok)
{
	enum event_type type;
	char *field;
	char *token;

	if (read_expected(EVENT_OP, "->") < 0)
		goto out_err;

	if (read_expect_type(EVENT_ITEM, &token) < 0)
		goto out_free;
	field = token;

	arg->type = PRINT_FIELD;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_entry(struct tep_event *event __maybe_unused, struct tep_print_arg *arg,
	      char **tok)
{
	enum tep_event_type type;
	char *field;
	char *token;

	if (read_expected(TEP_EVENT_OP, "->") < 0)
		goto out_err;

	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto out_free;
	field = token;

	arg->type = TEP_PRINT_FIELD;
>>>>>>> upstream/android-13
	arg->field.name = field;

	if (is_flag_field) {
		arg->field.field = tep_find_any_field(event, arg->field.name);
<<<<<<< HEAD
		arg->field.field->flags |= FIELD_IS_FLAG;
		is_flag_field = 0;
	} else if (is_symbolic_field) {
		arg->field.field = tep_find_any_field(event, arg->field.name);
		arg->field.field->flags |= FIELD_IS_SYMBOLIC;
=======
		arg->field.field->flags |= TEP_FIELD_IS_FLAG;
		is_flag_field = 0;
	} else if (is_symbolic_field) {
		arg->field.field = tep_find_any_field(event, arg->field.name);
		arg->field.field->flags |= TEP_FIELD_IS_SYMBOLIC;
>>>>>>> upstream/android-13
		is_symbolic_field = 0;
	}

	type = read_token(&token);
	*tok = token;

	return type;

 out_free:
	free_token(token);
 out_err:
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static int alloc_and_process_delim(struct event_format *event, char *next_token,
				   struct print_arg **print_arg)
{
	struct print_arg *field;
	enum event_type type;
=======
	return TEP_EVENT_ERROR;
}

static int alloc_and_process_delim(struct tep_event *event, char *next_token,
				   struct tep_print_arg **print_arg)
{
	struct tep_print_arg *field;
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token;
	int ret = 0;

	field = alloc_arg();
	if (!field) {
		do_warning_event(event, "%s: not enough memory!", __func__);
		errno = ENOMEM;
		return -1;
	}

	type = process_arg(event, field, &token);

<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_DELIM, next_token)) {
=======
	if (test_type_token(type, token, TEP_EVENT_DELIM, next_token)) {
>>>>>>> upstream/android-13
		errno = EINVAL;
		ret = -1;
		free_arg(field);
		goto out_free_token;
	}

	*print_arg = field;

out_free_token:
	free_token(token);

	return ret;
}

<<<<<<< HEAD
static char *arg_eval (struct print_arg *arg);
=======
static char *arg_eval (struct tep_print_arg *arg);
>>>>>>> upstream/android-13

static unsigned long long
eval_type_str(unsigned long long val, const char *type, int pointer)
{
	int sign = 0;
	char *ref;
	int len;

	len = strlen(type);

	if (pointer) {

		if (type[len-1] != '*') {
			do_warning("pointer expected with non pointer type");
			return val;
		}

		ref = malloc(len);
		if (!ref) {
			do_warning("%s: not enough memory!", __func__);
			return val;
		}
		memcpy(ref, type, len);

		/* chop off the " *" */
		ref[len - 2] = 0;

		val = eval_type_str(val, ref, 0);
		free(ref);
		return val;
	}

	/* check if this is a pointer */
	if (type[len - 1] == '*')
		return val;

	/* Try to figure out the arg size*/
	if (strncmp(type, "struct", 6) == 0)
		/* all bets off */
		return val;

	if (strcmp(type, "u8") == 0)
		return val & 0xff;

	if (strcmp(type, "u16") == 0)
		return val & 0xffff;

	if (strcmp(type, "u32") == 0)
		return val & 0xffffffff;

	if (strcmp(type, "u64") == 0 ||
	    strcmp(type, "s64") == 0)
		return val;

	if (strcmp(type, "s8") == 0)
		return (unsigned long long)(char)val & 0xff;

	if (strcmp(type, "s16") == 0)
		return (unsigned long long)(short)val & 0xffff;

	if (strcmp(type, "s32") == 0)
		return (unsigned long long)(int)val & 0xffffffff;

	if (strncmp(type, "unsigned ", 9) == 0) {
		sign = 0;
		type += 9;
	}

	if (strcmp(type, "char") == 0) {
		if (sign)
			return (unsigned long long)(char)val & 0xff;
		else
			return val & 0xff;
	}

	if (strcmp(type, "short") == 0) {
		if (sign)
			return (unsigned long long)(short)val & 0xffff;
		else
			return val & 0xffff;
	}

	if (strcmp(type, "int") == 0) {
		if (sign)
			return (unsigned long long)(int)val & 0xffffffff;
		else
			return val & 0xffffffff;
	}

	return val;
}

/*
 * Try to figure out the type.
 */
static unsigned long long
<<<<<<< HEAD
eval_type(unsigned long long val, struct print_arg *arg, int pointer)
{
	if (arg->type != PRINT_TYPE) {
=======
eval_type(unsigned long long val, struct tep_print_arg *arg, int pointer)
{
	if (arg->type != TEP_PRINT_TYPE) {
>>>>>>> upstream/android-13
		do_warning("expected type argument");
		return 0;
	}

	return eval_type_str(val, arg->typecast.type, pointer);
}

<<<<<<< HEAD
static int arg_num_eval(struct print_arg *arg, long long *val)
=======
static int arg_num_eval(struct tep_print_arg *arg, long long *val)
>>>>>>> upstream/android-13
{
	long long left, right;
	int ret = 1;

	switch (arg->type) {
<<<<<<< HEAD
	case PRINT_ATOM:
		*val = strtoll(arg->atom.atom, NULL, 0);
		break;
	case PRINT_TYPE:
=======
	case TEP_PRINT_ATOM:
		*val = strtoll(arg->atom.atom, NULL, 0);
		break;
	case TEP_PRINT_TYPE:
>>>>>>> upstream/android-13
		ret = arg_num_eval(arg->typecast.item, val);
		if (!ret)
			break;
		*val = eval_type(*val, arg, 0);
		break;
<<<<<<< HEAD
	case PRINT_OP:
=======
	case TEP_PRINT_OP:
>>>>>>> upstream/android-13
		switch (arg->op.op[0]) {
		case '|':
			ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			if (arg->op.op[1])
				*val = left || right;
			else
				*val = left | right;
			break;
		case '&':
			ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			if (arg->op.op[1])
				*val = left && right;
			else
				*val = left & right;
			break;
		case '<':
			ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			switch (arg->op.op[1]) {
			case 0:
				*val = left < right;
				break;
			case '<':
				*val = left << right;
				break;
			case '=':
				*val = left <= right;
				break;
			default:
				do_warning("unknown op '%s'", arg->op.op);
				ret = 0;
			}
			break;
		case '>':
			ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			switch (arg->op.op[1]) {
			case 0:
				*val = left > right;
				break;
			case '>':
				*val = left >> right;
				break;
			case '=':
				*val = left >= right;
				break;
			default:
				do_warning("unknown op '%s'", arg->op.op);
				ret = 0;
			}
			break;
		case '=':
			ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;

			if (arg->op.op[1] != '=') {
				do_warning("unknown op '%s'", arg->op.op);
				ret = 0;
			} else
				*val = left == right;
			break;
		case '!':
			ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;

			switch (arg->op.op[1]) {
			case '=':
				*val = left != right;
				break;
			default:
				do_warning("unknown op '%s'", arg->op.op);
				ret = 0;
			}
			break;
		case '-':
			/* check for negative */
<<<<<<< HEAD
			if (arg->op.left->type == PRINT_NULL)
=======
			if (arg->op.left->type == TEP_PRINT_NULL)
>>>>>>> upstream/android-13
				left = 0;
			else
				ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			*val = left - right;
			break;
		case '+':
<<<<<<< HEAD
			if (arg->op.left->type == PRINT_NULL)
=======
			if (arg->op.left->type == TEP_PRINT_NULL)
>>>>>>> upstream/android-13
				left = 0;
			else
				ret = arg_num_eval(arg->op.left, &left);
			if (!ret)
				break;
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			*val = left + right;
			break;
		case '~':
			ret = arg_num_eval(arg->op.right, &right);
			if (!ret)
				break;
			*val = ~right;
			break;
		default:
			do_warning("unknown op '%s'", arg->op.op);
			ret = 0;
		}
		break;

<<<<<<< HEAD
	case PRINT_NULL:
	case PRINT_FIELD ... PRINT_SYMBOL:
	case PRINT_STRING:
	case PRINT_BSTRING:
	case PRINT_BITMASK:
=======
	case TEP_PRINT_NULL:
	case TEP_PRINT_FIELD ... TEP_PRINT_SYMBOL:
	case TEP_PRINT_STRING:
	case TEP_PRINT_BSTRING:
	case TEP_PRINT_BITMASK:
>>>>>>> upstream/android-13
	default:
		do_warning("invalid eval type %d", arg->type);
		ret = 0;

	}
	return ret;
}

<<<<<<< HEAD
static char *arg_eval (struct print_arg *arg)
=======
static char *arg_eval (struct tep_print_arg *arg)
>>>>>>> upstream/android-13
{
	long long val;
	static char buf[24];

	switch (arg->type) {
<<<<<<< HEAD
	case PRINT_ATOM:
		return arg->atom.atom;
	case PRINT_TYPE:
		return arg_eval(arg->typecast.item);
	case PRINT_OP:
=======
	case TEP_PRINT_ATOM:
		return arg->atom.atom;
	case TEP_PRINT_TYPE:
		return arg_eval(arg->typecast.item);
	case TEP_PRINT_OP:
>>>>>>> upstream/android-13
		if (!arg_num_eval(arg, &val))
			break;
		sprintf(buf, "%lld", val);
		return buf;

<<<<<<< HEAD
	case PRINT_NULL:
	case PRINT_FIELD ... PRINT_SYMBOL:
	case PRINT_STRING:
	case PRINT_BSTRING:
	case PRINT_BITMASK:
=======
	case TEP_PRINT_NULL:
	case TEP_PRINT_FIELD ... TEP_PRINT_SYMBOL:
	case TEP_PRINT_STRING:
	case TEP_PRINT_BSTRING:
	case TEP_PRINT_BITMASK:
>>>>>>> upstream/android-13
	default:
		do_warning("invalid eval type %d", arg->type);
		break;
	}

	return NULL;
}

<<<<<<< HEAD
static enum event_type
process_fields(struct event_format *event, struct print_flag_sym **list, char **tok)
{
	enum event_type type;
	struct print_arg *arg = NULL;
	struct print_flag_sym *field;
=======
static enum tep_event_type
process_fields(struct tep_event *event, struct tep_print_flag_sym **list, char **tok)
{
	enum tep_event_type type;
	struct tep_print_arg *arg = NULL;
	struct tep_print_flag_sym *field;
>>>>>>> upstream/android-13
	char *token = *tok;
	char *value;

	do {
		free_token(token);
		type = read_token_item(&token);
<<<<<<< HEAD
		if (test_type_token(type, token, EVENT_OP, "{"))
=======
		if (test_type_token(type, token, TEP_EVENT_OP, "{"))
>>>>>>> upstream/android-13
			break;

		arg = alloc_arg();
		if (!arg)
			goto out_free;

		free_token(token);
		type = process_arg(event, arg, &token);

<<<<<<< HEAD
		if (type == EVENT_OP)
			type = process_op(event, arg, &token);

		if (type == EVENT_ERROR)
			goto out_free;

		if (test_type_token(type, token, EVENT_DELIM, ","))
=======
		if (type == TEP_EVENT_OP)
			type = process_op(event, arg, &token);

		if (type == TEP_EVENT_ERROR)
			goto out_free;

		if (test_type_token(type, token, TEP_EVENT_DELIM, ","))
>>>>>>> upstream/android-13
			goto out_free;

		field = calloc(1, sizeof(*field));
		if (!field)
			goto out_free;

		value = arg_eval(arg);
		if (value == NULL)
			goto out_free_field;
		field->value = strdup(value);
		if (field->value == NULL)
			goto out_free_field;

		free_arg(arg);
		arg = alloc_arg();
		if (!arg)
			goto out_free;

		free_token(token);
		type = process_arg(event, arg, &token);
<<<<<<< HEAD
		if (test_type_token(type, token, EVENT_OP, "}"))
=======
		if (test_type_token(type, token, TEP_EVENT_OP, "}"))
>>>>>>> upstream/android-13
			goto out_free_field;

		value = arg_eval(arg);
		if (value == NULL)
			goto out_free_field;
		field->str = strdup(value);
		if (field->str == NULL)
			goto out_free_field;
		free_arg(arg);
		arg = NULL;

		*list = field;
		list = &field->next;

		free_token(token);
		type = read_token_item(&token);
<<<<<<< HEAD
	} while (type == EVENT_DELIM && strcmp(token, ",") == 0);
=======
	} while (type == TEP_EVENT_DELIM && strcmp(token, ",") == 0);
>>>>>>> upstream/android-13

	*tok = token;
	return type;

out_free_field:
	free_flag_sym(field);
out_free:
	free_arg(arg);
	free_token(token);
	*tok = NULL;

<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_flags(struct event_format *event, struct print_arg *arg, char **tok)
{
	struct print_arg *field;
	enum event_type type;
	char *token = NULL;

	memset(arg, 0, sizeof(*arg));
	arg->type = PRINT_FLAGS;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_flags(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	struct tep_print_arg *field;
	enum tep_event_type type;
	char *token = NULL;

	memset(arg, 0, sizeof(*arg));
	arg->type = TEP_PRINT_FLAGS;
>>>>>>> upstream/android-13

	field = alloc_arg();
	if (!field) {
		do_warning_event(event, "%s: not enough memory!", __func__);
		goto out_free;
	}

	type = process_field_arg(event, field, &token);

	/* Handle operations in the first argument */
<<<<<<< HEAD
	while (type == EVENT_OP)
		type = process_op(event, field, &token);

	if (test_type_token(type, token, EVENT_DELIM, ","))
=======
	while (type == TEP_EVENT_OP)
		type = process_op(event, field, &token);

	if (test_type_token(type, token, TEP_EVENT_DELIM, ","))
>>>>>>> upstream/android-13
		goto out_free_field;
	free_token(token);

	arg->flags.field = field;

	type = read_token_item(&token);
	if (event_item_type(type)) {
		arg->flags.delim = token;
		type = read_token_item(&token);
	}

<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_DELIM, ","))
		goto out_free;

	type = process_fields(event, &arg->flags.flags, &token);
	if (test_type_token(type, token, EVENT_DELIM, ")"))
=======
	if (test_type_token(type, token, TEP_EVENT_DELIM, ","))
		goto out_free;

	type = process_fields(event, &arg->flags.flags, &token);
	if (test_type_token(type, token, TEP_EVENT_DELIM, ")"))
>>>>>>> upstream/android-13
		goto out_free;

	free_token(token);
	type = read_token_item(tok);
	return type;

out_free_field:
	free_arg(field);
out_free:
	free_token(token);
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_symbols(struct event_format *event, struct print_arg *arg, char **tok)
{
	struct print_arg *field;
	enum event_type type;
	char *token = NULL;

	memset(arg, 0, sizeof(*arg));
	arg->type = PRINT_SYMBOL;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_symbols(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	struct tep_print_arg *field;
	enum tep_event_type type;
	char *token = NULL;

	memset(arg, 0, sizeof(*arg));
	arg->type = TEP_PRINT_SYMBOL;
>>>>>>> upstream/android-13

	field = alloc_arg();
	if (!field) {
		do_warning_event(event, "%s: not enough memory!", __func__);
		goto out_free;
	}

	type = process_field_arg(event, field, &token);

<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_DELIM, ","))
=======
	if (test_type_token(type, token, TEP_EVENT_DELIM, ","))
>>>>>>> upstream/android-13
		goto out_free_field;

	arg->symbol.field = field;

	type = process_fields(event, &arg->symbol.symbols, &token);
<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_DELIM, ")"))
=======
	if (test_type_token(type, token, TEP_EVENT_DELIM, ")"))
>>>>>>> upstream/android-13
		goto out_free;

	free_token(token);
	type = read_token_item(tok);
	return type;

out_free_field:
	free_arg(field);
out_free:
	free_token(token);
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_hex_common(struct event_format *event, struct print_arg *arg,
		   char **tok, enum print_arg_type type)
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_hex_common(struct tep_event *event, struct tep_print_arg *arg,
		   char **tok, enum tep_print_arg_type type)
>>>>>>> upstream/android-13
{
	memset(arg, 0, sizeof(*arg));
	arg->type = type;

	if (alloc_and_process_delim(event, ",", &arg->hex.field))
		goto out;

	if (alloc_and_process_delim(event, ")", &arg->hex.size))
		goto free_field;

	return read_token_item(tok);

free_field:
	free_arg(arg->hex.field);
	arg->hex.field = NULL;
out:
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_hex(struct event_format *event, struct print_arg *arg, char **tok)
{
	return process_hex_common(event, arg, tok, PRINT_HEX);
}

static enum event_type
process_hex_str(struct event_format *event, struct print_arg *arg,
		char **tok)
{
	return process_hex_common(event, arg, tok, PRINT_HEX_STR);
}

static enum event_type
process_int_array(struct event_format *event, struct print_arg *arg, char **tok)
{
	memset(arg, 0, sizeof(*arg));
	arg->type = PRINT_INT_ARRAY;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_hex(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	return process_hex_common(event, arg, tok, TEP_PRINT_HEX);
}

static enum tep_event_type
process_hex_str(struct tep_event *event, struct tep_print_arg *arg,
		char **tok)
{
	return process_hex_common(event, arg, tok, TEP_PRINT_HEX_STR);
}

static enum tep_event_type
process_int_array(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	memset(arg, 0, sizeof(*arg));
	arg->type = TEP_PRINT_INT_ARRAY;
>>>>>>> upstream/android-13

	if (alloc_and_process_delim(event, ",", &arg->int_array.field))
		goto out;

	if (alloc_and_process_delim(event, ",", &arg->int_array.count))
		goto free_field;

	if (alloc_and_process_delim(event, ")", &arg->int_array.el_size))
		goto free_size;

	return read_token_item(tok);

free_size:
	free_arg(arg->int_array.count);
	arg->int_array.count = NULL;
free_field:
	free_arg(arg->int_array.field);
	arg->int_array.field = NULL;
out:
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_dynamic_array(struct event_format *event, struct print_arg *arg, char **tok)
{
	struct format_field *field;
	enum event_type type;
	char *token;

	memset(arg, 0, sizeof(*arg));
	arg->type = PRINT_DYNAMIC_ARRAY;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_dynamic_array(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	struct tep_format_field *field;
	enum tep_event_type type;
	char *token;

	memset(arg, 0, sizeof(*arg));
	arg->type = TEP_PRINT_DYNAMIC_ARRAY;
>>>>>>> upstream/android-13

	/*
	 * The item within the parenthesis is another field that holds
	 * the index into where the array starts.
	 */
	type = read_token(&token);
	*tok = token;
<<<<<<< HEAD
	if (type != EVENT_ITEM)
=======
	if (type != TEP_EVENT_ITEM)
>>>>>>> upstream/android-13
		goto out_free;

	/* Find the field */

	field = tep_find_field(event, token);
	if (!field)
		goto out_free;

	arg->dynarray.field = field;
	arg->dynarray.index = 0;

<<<<<<< HEAD
	if (read_expected(EVENT_DELIM, ")") < 0)
=======
	if (read_expected(TEP_EVENT_DELIM, ")") < 0)
>>>>>>> upstream/android-13
		goto out_free;

	free_token(token);
	type = read_token_item(&token);
	*tok = token;
<<<<<<< HEAD
	if (type != EVENT_OP || strcmp(token, "[") != 0)
=======
	if (type != TEP_EVENT_OP || strcmp(token, "[") != 0)
>>>>>>> upstream/android-13
		return type;

	free_token(token);
	arg = alloc_arg();
	if (!arg) {
		do_warning_event(event, "%s: not enough memory!", __func__);
		*tok = NULL;
<<<<<<< HEAD
		return EVENT_ERROR;
	}

	type = process_arg(event, arg, &token);
	if (type == EVENT_ERROR)
		goto out_free_arg;

	if (!test_type_token(type, token, EVENT_OP, "]"))
=======
		return TEP_EVENT_ERROR;
	}

	type = process_arg(event, arg, &token);
	if (type == TEP_EVENT_ERROR)
		goto out_free_arg;

	if (!test_type_token(type, token, TEP_EVENT_OP, "]"))
>>>>>>> upstream/android-13
		goto out_free_arg;

	free_token(token);
	type = read_token_item(tok);
	return type;

 out_free_arg:
	free_arg(arg);
 out_free:
	free_token(token);
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_dynamic_array_len(struct event_format *event, struct print_arg *arg,
			  char **tok)
{
	struct format_field *field;
	enum event_type type;
	char *token;

	if (read_expect_type(EVENT_ITEM, &token) < 0)
		goto out_free;

	arg->type = PRINT_DYNAMIC_ARRAY_LEN;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_dynamic_array_len(struct tep_event *event, struct tep_print_arg *arg,
			  char **tok)
{
	struct tep_format_field *field;
	enum tep_event_type type;
	char *token;

	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto out_free;

	arg->type = TEP_PRINT_DYNAMIC_ARRAY_LEN;
>>>>>>> upstream/android-13

	/* Find the field */
	field = tep_find_field(event, token);
	if (!field)
		goto out_free;

	arg->dynarray.field = field;
	arg->dynarray.index = 0;

<<<<<<< HEAD
	if (read_expected(EVENT_DELIM, ")") < 0)
=======
	if (read_expected(TEP_EVENT_DELIM, ")") < 0)
>>>>>>> upstream/android-13
		goto out_err;

	free_token(token);
	type = read_token(&token);
	*tok = token;

	return type;

 out_free:
	free_token(token);
 out_err:
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_paren(struct event_format *event, struct print_arg *arg, char **tok)
{
	struct print_arg *item_arg;
	enum event_type type;
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_paren(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	struct tep_print_arg *item_arg;
	enum tep_event_type type;
>>>>>>> upstream/android-13
	char *token;

	type = process_arg(event, arg, &token);

<<<<<<< HEAD
	if (type == EVENT_ERROR)
		goto out_free;

	if (type == EVENT_OP)
		type = process_op(event, arg, &token);

	if (type == EVENT_ERROR)
		goto out_free;

	if (test_type_token(type, token, EVENT_DELIM, ")"))
=======
	if (type == TEP_EVENT_ERROR)
		goto out_free;

	if (type == TEP_EVENT_OP)
		type = process_op(event, arg, &token);

	if (type == TEP_EVENT_ERROR)
		goto out_free;

	if (test_type_token(type, token, TEP_EVENT_DELIM, ")"))
>>>>>>> upstream/android-13
		goto out_free;

	free_token(token);
	type = read_token_item(&token);

	/*
	 * If the next token is an item or another open paren, then
	 * this was a typecast.
	 */
	if (event_item_type(type) ||
<<<<<<< HEAD
	    (type == EVENT_DELIM && strcmp(token, "(") == 0)) {
=======
	    (type == TEP_EVENT_DELIM && strcmp(token, "(") == 0)) {
>>>>>>> upstream/android-13

		/* make this a typecast and contine */

		/* prevous must be an atom */
<<<<<<< HEAD
		if (arg->type != PRINT_ATOM) {
			do_warning_event(event, "previous needed to be PRINT_ATOM");
=======
		if (arg->type != TEP_PRINT_ATOM) {
			do_warning_event(event, "previous needed to be TEP_PRINT_ATOM");
>>>>>>> upstream/android-13
			goto out_free;
		}

		item_arg = alloc_arg();
		if (!item_arg) {
			do_warning_event(event, "%s: not enough memory!",
					 __func__);
			goto out_free;
		}

<<<<<<< HEAD
		arg->type = PRINT_TYPE;
=======
		arg->type = TEP_PRINT_TYPE;
>>>>>>> upstream/android-13
		arg->typecast.type = arg->atom.atom;
		arg->typecast.item = item_arg;
		type = process_arg_token(event, item_arg, &token, type);

	}

	*tok = token;
	return type;

 out_free:
	free_token(token);
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}


static enum event_type
process_str(struct event_format *event __maybe_unused, struct print_arg *arg,
	    char **tok)
{
	enum event_type type;
	char *token;

	if (read_expect_type(EVENT_ITEM, &token) < 0)
		goto out_free;

	arg->type = PRINT_STRING;
	arg->string.string = token;
	arg->string.offset = -1;

	if (read_expected(EVENT_DELIM, ")") < 0)
=======
	return TEP_EVENT_ERROR;
}


static enum tep_event_type
process_str(struct tep_event *event __maybe_unused, struct tep_print_arg *arg,
	    char **tok)
{
	enum tep_event_type type;
	char *token;

	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto out_free;

	arg->type = TEP_PRINT_STRING;
	arg->string.string = token;
	arg->string.offset = -1;

	if (read_expected(TEP_EVENT_DELIM, ")") < 0)
>>>>>>> upstream/android-13
		goto out_err;

	type = read_token(&token);
	*tok = token;

	return type;

 out_free:
	free_token(token);
 out_err:
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_bitmask(struct event_format *event __maybe_unused, struct print_arg *arg,
	    char **tok)
{
	enum event_type type;
	char *token;

	if (read_expect_type(EVENT_ITEM, &token) < 0)
		goto out_free;

	arg->type = PRINT_BITMASK;
	arg->bitmask.bitmask = token;
	arg->bitmask.offset = -1;

	if (read_expected(EVENT_DELIM, ")") < 0)
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_bitmask(struct tep_event *event __maybe_unused, struct tep_print_arg *arg,
		char **tok)
{
	enum tep_event_type type;
	char *token;

	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto out_free;

	arg->type = TEP_PRINT_BITMASK;
	arg->bitmask.bitmask = token;
	arg->bitmask.offset = -1;

	if (read_expected(TEP_EVENT_DELIM, ")") < 0)
>>>>>>> upstream/android-13
		goto out_err;

	type = read_token(&token);
	*tok = token;

	return type;

 out_free:
	free_token(token);
 out_err:
	*tok = NULL;
<<<<<<< HEAD
	return EVENT_ERROR;
}

static struct tep_function_handler *
find_func_handler(struct tep_handle *pevent, char *func_name)
{
	struct tep_function_handler *func;

	if (!pevent)
		return NULL;

	for (func = pevent->func_handlers; func; func = func->next) {
=======
	return TEP_EVENT_ERROR;
}

static struct tep_function_handler *
find_func_handler(struct tep_handle *tep, char *func_name)
{
	struct tep_function_handler *func;

	if (!tep)
		return NULL;

	for (func = tep->func_handlers; func; func = func->next) {
>>>>>>> upstream/android-13
		if (strcmp(func->name, func_name) == 0)
			break;
	}

	return func;
}

<<<<<<< HEAD
static void remove_func_handler(struct tep_handle *pevent, char *func_name)
=======
static void remove_func_handler(struct tep_handle *tep, char *func_name)
>>>>>>> upstream/android-13
{
	struct tep_function_handler *func;
	struct tep_function_handler **next;

<<<<<<< HEAD
	next = &pevent->func_handlers;
=======
	next = &tep->func_handlers;
>>>>>>> upstream/android-13
	while ((func = *next)) {
		if (strcmp(func->name, func_name) == 0) {
			*next = func->next;
			free_func_handle(func);
			break;
		}
		next = &func->next;
	}
}

<<<<<<< HEAD
static enum event_type
process_func_handler(struct event_format *event, struct tep_function_handler *func,
		     struct print_arg *arg, char **tok)
{
	struct print_arg **next_arg;
	struct print_arg *farg;
	enum event_type type;
	char *token;
	int i;

	arg->type = PRINT_FUNC;
=======
static enum tep_event_type
process_func_handler(struct tep_event *event, struct tep_function_handler *func,
		     struct tep_print_arg *arg, char **tok)
{
	struct tep_print_arg **next_arg;
	struct tep_print_arg *farg;
	enum tep_event_type type;
	char *token;
	int i;

	arg->type = TEP_PRINT_FUNC;
>>>>>>> upstream/android-13
	arg->func.func = func;

	*tok = NULL;

	next_arg = &(arg->func.args);
	for (i = 0; i < func->nr_args; i++) {
		farg = alloc_arg();
		if (!farg) {
			do_warning_event(event, "%s: not enough memory!",
					 __func__);
<<<<<<< HEAD
			return EVENT_ERROR;
=======
			return TEP_EVENT_ERROR;
>>>>>>> upstream/android-13
		}

		type = process_arg(event, farg, &token);
		if (i < (func->nr_args - 1)) {
<<<<<<< HEAD
			if (type != EVENT_DELIM || strcmp(token, ",") != 0) {
=======
			if (type != TEP_EVENT_DELIM || strcmp(token, ",") != 0) {
>>>>>>> upstream/android-13
				do_warning_event(event,
					"Error: function '%s()' expects %d arguments but event %s only uses %d",
					func->name, func->nr_args,
					event->name, i + 1);
				goto err;
			}
		} else {
<<<<<<< HEAD
			if (type != EVENT_DELIM || strcmp(token, ")") != 0) {
=======
			if (type != TEP_EVENT_DELIM || strcmp(token, ")") != 0) {
>>>>>>> upstream/android-13
				do_warning_event(event,
					"Error: function '%s()' only expects %d arguments but event %s has more",
					func->name, func->nr_args, event->name);
				goto err;
			}
		}

		*next_arg = farg;
		next_arg = &(farg->next);
		free_token(token);
	}

	type = read_token(&token);
	*tok = token;

	return type;

err:
	free_arg(farg);
	free_token(token);
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_function(struct event_format *event, struct print_arg *arg,
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_builtin_expect(struct tep_event *event, struct tep_print_arg *arg, char **tok)
{
	enum tep_event_type type;
	char *token = NULL;

	/* Handle __builtin_expect( cond, #) */
	type = process_arg(event, arg, &token);

	if (type != TEP_EVENT_DELIM || token[0] != ',')
		goto out_free;

	free_token(token);

	/* We don't care what the second parameter is of the __builtin_expect() */
	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto out_free;

	if (read_expected(TEP_EVENT_DELIM, ")") < 0)
		goto out_free;

	free_token(token);
	type = read_token_item(tok);
	return type;

out_free:
	free_token(token);
	*tok = NULL;
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_function(struct tep_event *event, struct tep_print_arg *arg,
>>>>>>> upstream/android-13
		 char *token, char **tok)
{
	struct tep_function_handler *func;

	if (strcmp(token, "__print_flags") == 0) {
		free_token(token);
		is_flag_field = 1;
		return process_flags(event, arg, tok);
	}
	if (strcmp(token, "__print_symbolic") == 0) {
		free_token(token);
		is_symbolic_field = 1;
		return process_symbols(event, arg, tok);
	}
	if (strcmp(token, "__print_hex") == 0) {
		free_token(token);
		return process_hex(event, arg, tok);
	}
	if (strcmp(token, "__print_hex_str") == 0) {
		free_token(token);
		return process_hex_str(event, arg, tok);
	}
	if (strcmp(token, "__print_array") == 0) {
		free_token(token);
		return process_int_array(event, arg, tok);
	}
	if (strcmp(token, "__get_str") == 0) {
		free_token(token);
		return process_str(event, arg, tok);
	}
	if (strcmp(token, "__get_bitmask") == 0) {
		free_token(token);
		return process_bitmask(event, arg, tok);
	}
	if (strcmp(token, "__get_dynamic_array") == 0) {
		free_token(token);
		return process_dynamic_array(event, arg, tok);
	}
	if (strcmp(token, "__get_dynamic_array_len") == 0) {
		free_token(token);
		return process_dynamic_array_len(event, arg, tok);
	}
<<<<<<< HEAD

	func = find_func_handler(event->pevent, token);
=======
	if (strcmp(token, "__builtin_expect") == 0) {
		free_token(token);
		return process_builtin_expect(event, arg, tok);
	}

	func = find_func_handler(event->tep, token);
>>>>>>> upstream/android-13
	if (func) {
		free_token(token);
		return process_func_handler(event, func, arg, tok);
	}

	do_warning_event(event, "function %s not defined", token);
	free_token(token);
<<<<<<< HEAD
	return EVENT_ERROR;
}

static enum event_type
process_arg_token(struct event_format *event, struct print_arg *arg,
		  char **tok, enum event_type type)
=======
	return TEP_EVENT_ERROR;
}

static enum tep_event_type
process_arg_token(struct tep_event *event, struct tep_print_arg *arg,
		  char **tok, enum tep_event_type type)
>>>>>>> upstream/android-13
{
	char *token;
	char *atom;

	token = *tok;

	switch (type) {
<<<<<<< HEAD
	case EVENT_ITEM:
=======
	case TEP_EVENT_ITEM:
>>>>>>> upstream/android-13
		if (strcmp(token, "REC") == 0) {
			free_token(token);
			type = process_entry(event, arg, &token);
			break;
		}
		atom = token;
		/* test the next token */
		type = read_token_item(&token);

		/*
		 * If the next token is a parenthesis, then this
		 * is a function.
		 */
<<<<<<< HEAD
		if (type == EVENT_DELIM && strcmp(token, "(") == 0) {
=======
		if (type == TEP_EVENT_DELIM && strcmp(token, "(") == 0) {
>>>>>>> upstream/android-13
			free_token(token);
			token = NULL;
			/* this will free atom. */
			type = process_function(event, arg, atom, &token);
			break;
		}
		/* atoms can be more than one token long */
<<<<<<< HEAD
		while (type == EVENT_ITEM) {
			char *new_atom;
			new_atom = realloc(atom,
					   strlen(atom) + strlen(token) + 2);
			if (!new_atom) {
				free(atom);
				*tok = NULL;
				free_token(token);
				return EVENT_ERROR;
			}
			atom = new_atom;
			strcat(atom, " ");
			strcat(atom, token);
=======
		while (type == TEP_EVENT_ITEM) {
			int ret;

			ret = append(&atom, " ", token);
			if (ret < 0) {
				free(atom);
				*tok = NULL;
				free_token(token);
				return TEP_EVENT_ERROR;
			}
>>>>>>> upstream/android-13
			free_token(token);
			type = read_token_item(&token);
		}

<<<<<<< HEAD
		arg->type = PRINT_ATOM;
		arg->atom.atom = atom;
		break;

	case EVENT_DQUOTE:
	case EVENT_SQUOTE:
		arg->type = PRINT_ATOM;
		arg->atom.atom = token;
		type = read_token_item(&token);
		break;
	case EVENT_DELIM:
=======
		arg->type = TEP_PRINT_ATOM;
		arg->atom.atom = atom;
		break;

	case TEP_EVENT_DQUOTE:
	case TEP_EVENT_SQUOTE:
		arg->type = TEP_PRINT_ATOM;
		arg->atom.atom = token;
		type = read_token_item(&token);
		break;
	case TEP_EVENT_DELIM:
>>>>>>> upstream/android-13
		if (strcmp(token, "(") == 0) {
			free_token(token);
			type = process_paren(event, arg, &token);
			break;
		}
<<<<<<< HEAD
	case EVENT_OP:
		/* handle single ops */
		arg->type = PRINT_OP;
=======
	case TEP_EVENT_OP:
		/* handle single ops */
		arg->type = TEP_PRINT_OP;
>>>>>>> upstream/android-13
		arg->op.op = token;
		arg->op.left = NULL;
		type = process_op(event, arg, &token);

		/* On error, the op is freed */
<<<<<<< HEAD
		if (type == EVENT_ERROR)
=======
		if (type == TEP_EVENT_ERROR)
>>>>>>> upstream/android-13
			arg->op.op = NULL;

		/* return error type if errored */
		break;

<<<<<<< HEAD
	case EVENT_ERROR ... EVENT_NEWLINE:
	default:
		do_warning_event(event, "unexpected type %d", type);
		return EVENT_ERROR;
=======
	case TEP_EVENT_ERROR ... TEP_EVENT_NEWLINE:
	default:
		do_warning_event(event, "unexpected type %d", type);
		return TEP_EVENT_ERROR;
>>>>>>> upstream/android-13
	}
	*tok = token;

	return type;
}

<<<<<<< HEAD
static int event_read_print_args(struct event_format *event, struct print_arg **list)
{
	enum event_type type = EVENT_ERROR;
	struct print_arg *arg;
=======
static int event_read_print_args(struct tep_event *event, struct tep_print_arg **list)
{
	enum tep_event_type type = TEP_EVENT_ERROR;
	struct tep_print_arg *arg;
>>>>>>> upstream/android-13
	char *token;
	int args = 0;

	do {
<<<<<<< HEAD
		if (type == EVENT_NEWLINE) {
=======
		if (type == TEP_EVENT_NEWLINE) {
>>>>>>> upstream/android-13
			type = read_token_item(&token);
			continue;
		}

		arg = alloc_arg();
		if (!arg) {
			do_warning_event(event, "%s: not enough memory!",
					 __func__);
			return -1;
		}

		type = process_arg(event, arg, &token);

<<<<<<< HEAD
		if (type == EVENT_ERROR) {
=======
		if (type == TEP_EVENT_ERROR) {
>>>>>>> upstream/android-13
			free_token(token);
			free_arg(arg);
			return -1;
		}

		*list = arg;
		args++;

<<<<<<< HEAD
		if (type == EVENT_OP) {
			type = process_op(event, arg, &token);
			free_token(token);
			if (type == EVENT_ERROR) {
=======
		if (type == TEP_EVENT_OP) {
			type = process_op(event, arg, &token);
			free_token(token);
			if (type == TEP_EVENT_ERROR) {
>>>>>>> upstream/android-13
				*list = NULL;
				free_arg(arg);
				return -1;
			}
			list = &arg->next;
			continue;
		}

<<<<<<< HEAD
		if (type == EVENT_DELIM && strcmp(token, ",") == 0) {
=======
		if (type == TEP_EVENT_DELIM && strcmp(token, ",") == 0) {
>>>>>>> upstream/android-13
			free_token(token);
			*list = arg;
			list = &arg->next;
			continue;
		}
		break;
<<<<<<< HEAD
	} while (type != EVENT_NONE);

	if (type != EVENT_NONE && type != EVENT_ERROR)
=======
	} while (type != TEP_EVENT_NONE);

	if (type != TEP_EVENT_NONE && type != TEP_EVENT_ERROR)
>>>>>>> upstream/android-13
		free_token(token);

	return args;
}

<<<<<<< HEAD
static int event_read_print(struct event_format *event)
{
	enum event_type type;
	char *token;
	int ret;

	if (read_expected_item(EVENT_ITEM, "print") < 0)
		return -1;

	if (read_expected(EVENT_ITEM, "fmt") < 0)
		return -1;

	if (read_expected(EVENT_OP, ":") < 0)
		return -1;

	if (read_expect_type(EVENT_DQUOTE, &token) < 0)
=======
static int event_read_print(struct tep_event *event)
{
	enum tep_event_type type;
	char *token;
	int ret;

	if (read_expected_item(TEP_EVENT_ITEM, "print") < 0)
		return -1;

	if (read_expected(TEP_EVENT_ITEM, "fmt") < 0)
		return -1;

	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return -1;

	if (read_expect_type(TEP_EVENT_DQUOTE, &token) < 0)
>>>>>>> upstream/android-13
		goto fail;

 concat:
	event->print_fmt.format = token;
	event->print_fmt.args = NULL;

	/* ok to have no arg */
	type = read_token_item(&token);

<<<<<<< HEAD
	if (type == EVENT_NONE)
		return 0;

	/* Handle concatenation of print lines */
	if (type == EVENT_DQUOTE) {
=======
	if (type == TEP_EVENT_NONE)
		return 0;

	/* Handle concatenation of print lines */
	if (type == TEP_EVENT_DQUOTE) {
>>>>>>> upstream/android-13
		char *cat;

		if (asprintf(&cat, "%s%s", event->print_fmt.format, token) < 0)
			goto fail;
		free_token(token);
		free_token(event->print_fmt.format);
		event->print_fmt.format = NULL;
		token = cat;
		goto concat;
	}
			     
<<<<<<< HEAD
	if (test_type_token(type, token, EVENT_DELIM, ","))
=======
	if (test_type_token(type, token, TEP_EVENT_DELIM, ","))
>>>>>>> upstream/android-13
		goto fail;

	free_token(token);

	ret = event_read_print_args(event, &event->print_fmt.args);
	if (ret < 0)
		return -1;

	return ret;

 fail:
	free_token(token);
	return -1;
}

/**
 * tep_find_common_field - return a common field by event
 * @event: handle for the event
 * @name: the name of the common field to return
 *
 * Returns a common field from the event by the given @name.
<<<<<<< HEAD
 * This only searchs the common fields and not all field.
 */
struct format_field *
tep_find_common_field(struct event_format *event, const char *name)
{
	struct format_field *format;
=======
 * This only searches the common fields and not all field.
 */
struct tep_format_field *
tep_find_common_field(struct tep_event *event, const char *name)
{
	struct tep_format_field *format;
>>>>>>> upstream/android-13

	for (format = event->format.common_fields;
	     format; format = format->next) {
		if (strcmp(format->name, name) == 0)
			break;
	}

	return format;
}

/**
 * tep_find_field - find a non-common field
 * @event: handle for the event
 * @name: the name of the non-common field
 *
 * Returns a non-common field by the given @name.
 * This does not search common fields.
 */
<<<<<<< HEAD
struct format_field *
tep_find_field(struct event_format *event, const char *name)
{
	struct format_field *format;
=======
struct tep_format_field *
tep_find_field(struct tep_event *event, const char *name)
{
	struct tep_format_field *format;
>>>>>>> upstream/android-13

	for (format = event->format.fields;
	     format; format = format->next) {
		if (strcmp(format->name, name) == 0)
			break;
	}

	return format;
}

/**
 * tep_find_any_field - find any field by name
 * @event: handle for the event
 * @name: the name of the field
 *
 * Returns a field by the given @name.
<<<<<<< HEAD
 * This searchs the common field names first, then
 * the non-common ones if a common one was not found.
 */
struct format_field *
tep_find_any_field(struct event_format *event, const char *name)
{
	struct format_field *format;
=======
 * This searches the common field names first, then
 * the non-common ones if a common one was not found.
 */
struct tep_format_field *
tep_find_any_field(struct tep_event *event, const char *name)
{
	struct tep_format_field *format;
>>>>>>> upstream/android-13

	format = tep_find_common_field(event, name);
	if (format)
		return format;
	return tep_find_field(event, name);
}

/**
 * tep_read_number - read a number from data
<<<<<<< HEAD
 * @pevent: handle for the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @ptr: the raw data
 * @size: the size of the data that holds the number
 *
 * Returns the number (converted to host) from the
 * raw data.
 */
<<<<<<< HEAD
unsigned long long tep_read_number(struct tep_handle *pevent,
				   const void *ptr, int size)
{
=======
unsigned long long tep_read_number(struct tep_handle *tep,
				   const void *ptr, int size)
{
	unsigned long long val;

>>>>>>> upstream/android-13
	switch (size) {
	case 1:
		return *(unsigned char *)ptr;
	case 2:
<<<<<<< HEAD
		return data2host2(pevent, ptr);
	case 4:
		return data2host4(pevent, ptr);
	case 8:
		return data2host8(pevent, ptr);
=======
		return data2host2(tep, *(unsigned short *)ptr);
	case 4:
		return data2host4(tep, *(unsigned int *)ptr);
	case 8:
		memcpy(&val, (ptr), sizeof(unsigned long long));
		return data2host8(tep, val);
>>>>>>> upstream/android-13
	default:
		/* BUG! */
		return 0;
	}
}

/**
 * tep_read_number_field - read a number from data
 * @field: a handle to the field
 * @data: the raw data to read
 * @value: the value to place the number in
 *
 * Reads raw data according to a field offset and size,
 * and translates it into @value.
 *
 * Returns 0 on success, -1 otherwise.
 */
<<<<<<< HEAD
int tep_read_number_field(struct format_field *field, const void *data,
=======
int tep_read_number_field(struct tep_format_field *field, const void *data,
>>>>>>> upstream/android-13
			  unsigned long long *value)
{
	if (!field)
		return -1;
	switch (field->size) {
	case 1:
	case 2:
	case 4:
	case 8:
<<<<<<< HEAD
		*value = tep_read_number(field->event->pevent,
=======
		*value = tep_read_number(field->event->tep,
>>>>>>> upstream/android-13
					 data + field->offset, field->size);
		return 0;
	default:
		return -1;
	}
}

<<<<<<< HEAD
static int get_common_info(struct tep_handle *pevent,
			   const char *type, int *offset, int *size)
{
	struct event_format *event;
	struct format_field *field;
=======
static int get_common_info(struct tep_handle *tep,
			   const char *type, int *offset, int *size)
{
	struct tep_event *event;
	struct tep_format_field *field;
>>>>>>> upstream/android-13

	/*
	 * All events should have the same common elements.
	 * Pick any event to find where the type is;
	 */
<<<<<<< HEAD
	if (!pevent->events) {
=======
	if (!tep->events) {
>>>>>>> upstream/android-13
		do_warning("no event_list!");
		return -1;
	}

<<<<<<< HEAD
	event = pevent->events[0];
=======
	event = tep->events[0];
>>>>>>> upstream/android-13
	field = tep_find_common_field(event, type);
	if (!field)
		return -1;

	*offset = field->offset;
	*size = field->size;

	return 0;
}

<<<<<<< HEAD
static int __parse_common(struct tep_handle *pevent, void *data,
=======
static int __parse_common(struct tep_handle *tep, void *data,
>>>>>>> upstream/android-13
			  int *size, int *offset, const char *name)
{
	int ret;

	if (!*size) {
<<<<<<< HEAD
		ret = get_common_info(pevent, name, offset, size);
		if (ret < 0)
			return ret;
	}
	return tep_read_number(pevent, data + *offset, *size);
}

static int trace_parse_common_type(struct tep_handle *pevent, void *data)
{
	return __parse_common(pevent, data,
			      &pevent->type_size, &pevent->type_offset,
			      "common_type");
}

static int parse_common_pid(struct tep_handle *pevent, void *data)
{
	return __parse_common(pevent, data,
			      &pevent->pid_size, &pevent->pid_offset,
			      "common_pid");
}

static int parse_common_pc(struct tep_handle *pevent, void *data)
{
	return __parse_common(pevent, data,
			      &pevent->pc_size, &pevent->pc_offset,
			      "common_preempt_count");
}

static int parse_common_flags(struct tep_handle *pevent, void *data)
{
	return __parse_common(pevent, data,
			      &pevent->flags_size, &pevent->flags_offset,
			      "common_flags");
}

static int parse_common_lock_depth(struct tep_handle *pevent, void *data)
{
	return __parse_common(pevent, data,
			      &pevent->ld_size, &pevent->ld_offset,
			      "common_lock_depth");
}

static int parse_common_migrate_disable(struct tep_handle *pevent, void *data)
{
	return __parse_common(pevent, data,
			      &pevent->ld_size, &pevent->ld_offset,
=======
		ret = get_common_info(tep, name, offset, size);
		if (ret < 0)
			return ret;
	}
	return tep_read_number(tep, data + *offset, *size);
}

static int trace_parse_common_type(struct tep_handle *tep, void *data)
{
	return __parse_common(tep, data,
			      &tep->type_size, &tep->type_offset,
			      "common_type");
}

static int parse_common_pid(struct tep_handle *tep, void *data)
{
	return __parse_common(tep, data,
			      &tep->pid_size, &tep->pid_offset,
			      "common_pid");
}

static int parse_common_pc(struct tep_handle *tep, void *data)
{
	return __parse_common(tep, data,
			      &tep->pc_size, &tep->pc_offset,
			      "common_preempt_count");
}

static int parse_common_flags(struct tep_handle *tep, void *data)
{
	return __parse_common(tep, data,
			      &tep->flags_size, &tep->flags_offset,
			      "common_flags");
}

static int parse_common_lock_depth(struct tep_handle *tep, void *data)
{
	return __parse_common(tep, data,
			      &tep->ld_size, &tep->ld_offset,
			      "common_lock_depth");
}

static int parse_common_migrate_disable(struct tep_handle *tep, void *data)
{
	return __parse_common(tep, data,
			      &tep->ld_size, &tep->ld_offset,
>>>>>>> upstream/android-13
			      "common_migrate_disable");
}

static int events_id_cmp(const void *a, const void *b);

/**
 * tep_find_event - find an event by given id
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @id: the id of the event
 *
 * Returns an event that has a given @id.
 */
<<<<<<< HEAD
struct event_format *tep_find_event(struct tep_handle *pevent, int id)
{
	struct event_format **eventptr;
	struct event_format key;
	struct event_format *pkey = &key;

	/* Check cache first */
	if (pevent->last_event && pevent->last_event->id == id)
		return pevent->last_event;

	key.id = id;

	eventptr = bsearch(&pkey, pevent->events, pevent->nr_events,
			   sizeof(*pevent->events), events_id_cmp);

	if (eventptr) {
		pevent->last_event = *eventptr;
=======
struct tep_event *tep_find_event(struct tep_handle *tep, int id)
{
	struct tep_event **eventptr;
	struct tep_event key;
	struct tep_event *pkey = &key;

	/* Check cache first */
	if (tep->last_event && tep->last_event->id == id)
		return tep->last_event;

	key.id = id;

	eventptr = bsearch(&pkey, tep->events, tep->nr_events,
			   sizeof(*tep->events), events_id_cmp);

	if (eventptr) {
		tep->last_event = *eventptr;
>>>>>>> upstream/android-13
		return *eventptr;
	}

	return NULL;
}

/**
 * tep_find_event_by_name - find an event by given name
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @sys: the system name to search for
 * @name: the name of the event to search for
 *
 * This returns an event with a given @name and under the system
 * @sys. If @sys is NULL the first event with @name is returned.
 */
<<<<<<< HEAD
struct event_format *
tep_find_event_by_name(struct tep_handle *pevent,
		       const char *sys, const char *name)
{
	struct event_format *event;
	int i;

	if (pevent->last_event &&
	    strcmp(pevent->last_event->name, name) == 0 &&
	    (!sys || strcmp(pevent->last_event->system, sys) == 0))
		return pevent->last_event;

	for (i = 0; i < pevent->nr_events; i++) {
		event = pevent->events[i];
=======
struct tep_event *
tep_find_event_by_name(struct tep_handle *tep,
		       const char *sys, const char *name)
{
	struct tep_event *event = NULL;
	int i;

	if (tep->last_event &&
	    strcmp(tep->last_event->name, name) == 0 &&
	    (!sys || strcmp(tep->last_event->system, sys) == 0))
		return tep->last_event;

	for (i = 0; i < tep->nr_events; i++) {
		event = tep->events[i];
>>>>>>> upstream/android-13
		if (strcmp(event->name, name) == 0) {
			if (!sys)
				break;
			if (strcmp(event->system, sys) == 0)
				break;
		}
	}
<<<<<<< HEAD
	if (i == pevent->nr_events)
		event = NULL;

	pevent->last_event = event;
=======
	if (i == tep->nr_events)
		event = NULL;

	tep->last_event = event;
>>>>>>> upstream/android-13
	return event;
}

static unsigned long long
<<<<<<< HEAD
eval_num_arg(void *data, int size, struct event_format *event, struct print_arg *arg)
{
	struct tep_handle *pevent = event->pevent;
	unsigned long long val = 0;
	unsigned long long left, right;
	struct print_arg *typearg = NULL;
	struct print_arg *larg;
=======
eval_num_arg(void *data, int size, struct tep_event *event, struct tep_print_arg *arg)
{
	struct tep_handle *tep = event->tep;
	unsigned long long val = 0;
	unsigned long long left, right;
	struct tep_print_arg *typearg = NULL;
	struct tep_print_arg *larg;
>>>>>>> upstream/android-13
	unsigned long offset;
	unsigned int field_size;

	switch (arg->type) {
<<<<<<< HEAD
	case PRINT_NULL:
		/* ?? */
		return 0;
	case PRINT_ATOM:
		return strtoull(arg->atom.atom, NULL, 0);
	case PRINT_FIELD:
=======
	case TEP_PRINT_NULL:
		/* ?? */
		return 0;
	case TEP_PRINT_ATOM:
		return strtoull(arg->atom.atom, NULL, 0);
	case TEP_PRINT_FIELD:
>>>>>>> upstream/android-13
		if (!arg->field.field) {
			arg->field.field = tep_find_any_field(event, arg->field.name);
			if (!arg->field.field)
				goto out_warning_field;
			
		}
		/* must be a number */
<<<<<<< HEAD
		val = tep_read_number(pevent, data + arg->field.field->offset,
				      arg->field.field->size);
		break;
	case PRINT_FLAGS:
	case PRINT_SYMBOL:
	case PRINT_INT_ARRAY:
	case PRINT_HEX:
	case PRINT_HEX_STR:
		break;
	case PRINT_TYPE:
		val = eval_num_arg(data, size, event, arg->typecast.item);
		return eval_type(val, arg, 0);
	case PRINT_STRING:
	case PRINT_BSTRING:
	case PRINT_BITMASK:
		return 0;
	case PRINT_FUNC: {
=======
		val = tep_read_number(tep, data + arg->field.field->offset,
				      arg->field.field->size);
		break;
	case TEP_PRINT_FLAGS:
	case TEP_PRINT_SYMBOL:
	case TEP_PRINT_INT_ARRAY:
	case TEP_PRINT_HEX:
	case TEP_PRINT_HEX_STR:
		break;
	case TEP_PRINT_TYPE:
		val = eval_num_arg(data, size, event, arg->typecast.item);
		return eval_type(val, arg, 0);
	case TEP_PRINT_STRING:
	case TEP_PRINT_BSTRING:
	case TEP_PRINT_BITMASK:
		return 0;
	case TEP_PRINT_FUNC: {
>>>>>>> upstream/android-13
		struct trace_seq s;
		trace_seq_init(&s);
		val = process_defined_func(&s, data, size, event, arg);
		trace_seq_destroy(&s);
		return val;
	}
<<<<<<< HEAD
	case PRINT_OP:
=======
	case TEP_PRINT_OP:
>>>>>>> upstream/android-13
		if (strcmp(arg->op.op, "[") == 0) {
			/*
			 * Arrays are special, since we don't want
			 * to read the arg as is.
			 */
			right = eval_num_arg(data, size, event, arg->op.right);

			/* handle typecasts */
			larg = arg->op.left;
<<<<<<< HEAD
			while (larg->type == PRINT_TYPE) {
=======
			while (larg->type == TEP_PRINT_TYPE) {
>>>>>>> upstream/android-13
				if (!typearg)
					typearg = larg;
				larg = larg->typecast.item;
			}

			/* Default to long size */
<<<<<<< HEAD
			field_size = pevent->long_size;

			switch (larg->type) {
			case PRINT_DYNAMIC_ARRAY:
				offset = tep_read_number(pevent,
=======
			field_size = tep->long_size;

			switch (larg->type) {
			case TEP_PRINT_DYNAMIC_ARRAY:
				offset = tep_read_number(tep,
>>>>>>> upstream/android-13
						   data + larg->dynarray.field->offset,
						   larg->dynarray.field->size);
				if (larg->dynarray.field->elementsize)
					field_size = larg->dynarray.field->elementsize;
				/*
				 * The actual length of the dynamic array is stored
				 * in the top half of the field, and the offset
				 * is in the bottom half of the 32 bit field.
				 */
				offset &= 0xffff;
				offset += right;
				break;
<<<<<<< HEAD
			case PRINT_FIELD:
=======
			case TEP_PRINT_FIELD:
>>>>>>> upstream/android-13
				if (!larg->field.field) {
					larg->field.field =
						tep_find_any_field(event, larg->field.name);
					if (!larg->field.field) {
						arg = larg;
						goto out_warning_field;
					}
				}
				field_size = larg->field.field->elementsize;
				offset = larg->field.field->offset +
					right * larg->field.field->elementsize;
				break;
			default:
				goto default_op; /* oops, all bets off */
			}
<<<<<<< HEAD
			val = tep_read_number(pevent,
=======
			val = tep_read_number(tep,
>>>>>>> upstream/android-13
					      data + offset, field_size);
			if (typearg)
				val = eval_type(val, typearg, 1);
			break;
		} else if (strcmp(arg->op.op, "?") == 0) {
			left = eval_num_arg(data, size, event, arg->op.left);
			arg = arg->op.right;
			if (left)
				val = eval_num_arg(data, size, event, arg->op.left);
			else
				val = eval_num_arg(data, size, event, arg->op.right);
			break;
		}
 default_op:
		left = eval_num_arg(data, size, event, arg->op.left);
		right = eval_num_arg(data, size, event, arg->op.right);
		switch (arg->op.op[0]) {
		case '!':
			switch (arg->op.op[1]) {
			case 0:
				val = !right;
				break;
			case '=':
				val = left != right;
				break;
			default:
				goto out_warning_op;
			}
			break;
		case '~':
			val = ~right;
			break;
		case '|':
			if (arg->op.op[1])
				val = left || right;
			else
				val = left | right;
			break;
		case '&':
			if (arg->op.op[1])
				val = left && right;
			else
				val = left & right;
			break;
		case '<':
			switch (arg->op.op[1]) {
			case 0:
				val = left < right;
				break;
			case '<':
				val = left << right;
				break;
			case '=':
				val = left <= right;
				break;
			default:
				goto out_warning_op;
			}
			break;
		case '>':
			switch (arg->op.op[1]) {
			case 0:
				val = left > right;
				break;
			case '>':
				val = left >> right;
				break;
			case '=':
				val = left >= right;
				break;
			default:
				goto out_warning_op;
			}
			break;
		case '=':
			if (arg->op.op[1] != '=')
				goto out_warning_op;

			val = left == right;
			break;
		case '-':
			val = left - right;
			break;
		case '+':
			val = left + right;
			break;
		case '/':
			val = left / right;
			break;
		case '%':
			val = left % right;
			break;
		case '*':
			val = left * right;
			break;
		default:
			goto out_warning_op;
		}
		break;
<<<<<<< HEAD
	case PRINT_DYNAMIC_ARRAY_LEN:
		offset = tep_read_number(pevent,
=======
	case TEP_PRINT_DYNAMIC_ARRAY_LEN:
		offset = tep_read_number(tep,
>>>>>>> upstream/android-13
					 data + arg->dynarray.field->offset,
					 arg->dynarray.field->size);
		/*
		 * The total allocated length of the dynamic array is
		 * stored in the top half of the field, and the offset
		 * is in the bottom half of the 32 bit field.
		 */
		val = (unsigned long long)(offset >> 16);
		break;
<<<<<<< HEAD
	case PRINT_DYNAMIC_ARRAY:
		/* Without [], we pass the address to the dynamic data */
		offset = tep_read_number(pevent,
=======
	case TEP_PRINT_DYNAMIC_ARRAY:
		/* Without [], we pass the address to the dynamic data */
		offset = tep_read_number(tep,
>>>>>>> upstream/android-13
					 data + arg->dynarray.field->offset,
					 arg->dynarray.field->size);
		/*
		 * The total allocated length of the dynamic array is
		 * stored in the top half of the field, and the offset
		 * is in the bottom half of the 32 bit field.
		 */
		offset &= 0xffff;
		val = (unsigned long long)((unsigned long)data + offset);
		break;
	default: /* not sure what to do there */
		return 0;
	}
	return val;

out_warning_op:
	do_warning_event(event, "%s: unknown op '%s'", __func__, arg->op.op);
	return 0;

out_warning_field:
	do_warning_event(event, "%s: field %s not found",
			 __func__, arg->field.name);
	return 0;
}

struct flag {
	const char *name;
	unsigned long long value;
};

static const struct flag flags[] = {
	{ "HI_SOFTIRQ", 0 },
	{ "TIMER_SOFTIRQ", 1 },
	{ "NET_TX_SOFTIRQ", 2 },
	{ "NET_RX_SOFTIRQ", 3 },
	{ "BLOCK_SOFTIRQ", 4 },
	{ "IRQ_POLL_SOFTIRQ", 5 },
	{ "TASKLET_SOFTIRQ", 6 },
	{ "SCHED_SOFTIRQ", 7 },
	{ "HRTIMER_SOFTIRQ", 8 },
	{ "RCU_SOFTIRQ", 9 },

	{ "HRTIMER_NORESTART", 0 },
	{ "HRTIMER_RESTART", 1 },
};

static long long eval_flag(const char *flag)
{
	int i;

	/*
	 * Some flags in the format files do not get converted.
	 * If the flag is not numeric, see if it is something that
	 * we already know about.
	 */
	if (isdigit(flag[0]))
		return strtoull(flag, NULL, 0);

	for (i = 0; i < (int)(sizeof(flags)/sizeof(flags[0])); i++)
		if (strcmp(flags[i].name, flag) == 0)
			return flags[i].value;

	return -1LL;
}

static void print_str_to_seq(struct trace_seq *s, const char *format,
			     int len_arg, const char *str)
{
	if (len_arg >= 0)
		trace_seq_printf(s, format, len_arg, str);
	else
		trace_seq_printf(s, format, str);
}

<<<<<<< HEAD
static void print_bitmask_to_seq(struct tep_handle *pevent,
=======
static void print_bitmask_to_seq(struct tep_handle *tep,
>>>>>>> upstream/android-13
				 struct trace_seq *s, const char *format,
				 int len_arg, const void *data, int size)
{
	int nr_bits = size * 8;
	int str_size = (nr_bits + 3) / 4;
	int len = 0;
	char buf[3];
	char *str;
	int index;
	int i;

	/*
	 * The kernel likes to put in commas every 32 bits, we
	 * can do the same.
	 */
	str_size += (nr_bits - 1) / 32;

	str = malloc(str_size + 1);
	if (!str) {
		do_warning("%s: not enough memory!", __func__);
		return;
	}
	str[str_size] = 0;

	/* Start out with -2 for the two chars per byte */
	for (i = str_size - 2; i >= 0; i -= 2) {
		/*
		 * data points to a bit mask of size bytes.
		 * In the kernel, this is an array of long words, thus
<<<<<<< HEAD
		 * endianess is very important.
		 */
		if (pevent->file_bigendian)
=======
		 * endianness is very important.
		 */
		if (tep->file_bigendian)
>>>>>>> upstream/android-13
			index = size - (len + 1);
		else
			index = len;

		snprintf(buf, 3, "%02x", *((unsigned char *)data + index));
		memcpy(str + i, buf, 2);
		len++;
		if (!(len & 3) && i > 0) {
			i--;
			str[i] = ',';
		}
	}

	if (len_arg >= 0)
		trace_seq_printf(s, format, len_arg, str);
	else
		trace_seq_printf(s, format, str);

	free(str);
}

static void print_str_arg(struct trace_seq *s, void *data, int size,
<<<<<<< HEAD
			  struct event_format *event, const char *format,
			  int len_arg, struct print_arg *arg)
{
	struct tep_handle *pevent = event->pevent;
	struct print_flag_sym *flag;
	struct format_field *field;
=======
			  struct tep_event *event, const char *format,
			  int len_arg, struct tep_print_arg *arg)
{
	struct tep_handle *tep = event->tep;
	struct tep_print_flag_sym *flag;
	struct tep_format_field *field;
>>>>>>> upstream/android-13
	struct printk_map *printk;
	long long val, fval;
	unsigned long long addr;
	char *str;
	unsigned char *hex;
	int print;
	int i, len;

	switch (arg->type) {
<<<<<<< HEAD
	case PRINT_NULL:
		/* ?? */
		return;
	case PRINT_ATOM:
		print_str_to_seq(s, format, len_arg, arg->atom.atom);
		return;
	case PRINT_FIELD:
=======
	case TEP_PRINT_NULL:
		/* ?? */
		return;
	case TEP_PRINT_ATOM:
		print_str_to_seq(s, format, len_arg, arg->atom.atom);
		return;
	case TEP_PRINT_FIELD:
>>>>>>> upstream/android-13
		field = arg->field.field;
		if (!field) {
			field = tep_find_any_field(event, arg->field.name);
			if (!field) {
				str = arg->field.name;
				goto out_warning_field;
			}
			arg->field.field = field;
		}
		/* Zero sized fields, mean the rest of the data */
		len = field->size ? : size - field->offset;

		/*
		 * Some events pass in pointers. If this is not an array
		 * and the size is the same as long_size, assume that it
		 * is a pointer.
		 */
<<<<<<< HEAD
		if (!(field->flags & FIELD_IS_ARRAY) &&
		    field->size == pevent->long_size) {
=======
		if (!(field->flags & TEP_FIELD_IS_ARRAY) &&
		    field->size == tep->long_size) {
>>>>>>> upstream/android-13

			/* Handle heterogeneous recording and processing
			 * architectures
			 *
			 * CASE I:
			 * Traces recorded on 32-bit devices (32-bit
			 * addressing) and processed on 64-bit devices:
			 * In this case, only 32 bits should be read.
			 *
			 * CASE II:
			 * Traces recorded on 64 bit devices and processed
			 * on 32-bit devices:
			 * In this case, 64 bits must be read.
			 */
<<<<<<< HEAD
			addr = (pevent->long_size == 8) ?
=======
			addr = (tep->long_size == 8) ?
>>>>>>> upstream/android-13
				*(unsigned long long *)(data + field->offset) :
				(unsigned long long)*(unsigned int *)(data + field->offset);

			/* Check if it matches a print format */
<<<<<<< HEAD
			printk = find_printk(pevent, addr);
=======
			printk = find_printk(tep, addr);
>>>>>>> upstream/android-13
			if (printk)
				trace_seq_puts(s, printk->printk);
			else
				trace_seq_printf(s, "%llx", addr);
			break;
		}
		str = malloc(len + 1);
		if (!str) {
			do_warning_event(event, "%s: not enough memory!",
					 __func__);
			return;
		}
		memcpy(str, data + field->offset, len);
		str[len] = 0;
		print_str_to_seq(s, format, len_arg, str);
		free(str);
		break;
<<<<<<< HEAD
	case PRINT_FLAGS:
=======
	case TEP_PRINT_FLAGS:
>>>>>>> upstream/android-13
		val = eval_num_arg(data, size, event, arg->flags.field);
		print = 0;
		for (flag = arg->flags.flags; flag; flag = flag->next) {
			fval = eval_flag(flag->value);
			if (!val && fval < 0) {
				print_str_to_seq(s, format, len_arg, flag->str);
				break;
			}
			if (fval > 0 && (val & fval) == fval) {
				if (print && arg->flags.delim)
					trace_seq_puts(s, arg->flags.delim);
				print_str_to_seq(s, format, len_arg, flag->str);
				print = 1;
				val &= ~fval;
			}
		}
		if (val) {
			if (print && arg->flags.delim)
				trace_seq_puts(s, arg->flags.delim);
			trace_seq_printf(s, "0x%llx", val);
		}
		break;
<<<<<<< HEAD
	case PRINT_SYMBOL:
=======
	case TEP_PRINT_SYMBOL:
>>>>>>> upstream/android-13
		val = eval_num_arg(data, size, event, arg->symbol.field);
		for (flag = arg->symbol.symbols; flag; flag = flag->next) {
			fval = eval_flag(flag->value);
			if (val == fval) {
				print_str_to_seq(s, format, len_arg, flag->str);
				break;
			}
		}
		if (!flag)
			trace_seq_printf(s, "0x%llx", val);
		break;
<<<<<<< HEAD
	case PRINT_HEX:
	case PRINT_HEX_STR:
		if (arg->hex.field->type == PRINT_DYNAMIC_ARRAY) {
			unsigned long offset;
			offset = tep_read_number(pevent,
=======
	case TEP_PRINT_HEX:
	case TEP_PRINT_HEX_STR:
		if (arg->hex.field->type == TEP_PRINT_DYNAMIC_ARRAY) {
			unsigned long offset;
			offset = tep_read_number(tep,
>>>>>>> upstream/android-13
				data + arg->hex.field->dynarray.field->offset,
				arg->hex.field->dynarray.field->size);
			hex = data + (offset & 0xffff);
		} else {
			field = arg->hex.field->field.field;
			if (!field) {
				str = arg->hex.field->field.name;
				field = tep_find_any_field(event, str);
				if (!field)
					goto out_warning_field;
				arg->hex.field->field.field = field;
			}
			hex = data + field->offset;
		}
		len = eval_num_arg(data, size, event, arg->hex.size);
		for (i = 0; i < len; i++) {
<<<<<<< HEAD
			if (i && arg->type == PRINT_HEX)
=======
			if (i && arg->type == TEP_PRINT_HEX)
>>>>>>> upstream/android-13
				trace_seq_putc(s, ' ');
			trace_seq_printf(s, "%02x", hex[i]);
		}
		break;

<<<<<<< HEAD
	case PRINT_INT_ARRAY: {
		void *num;
		int el_size;

		if (arg->int_array.field->type == PRINT_DYNAMIC_ARRAY) {
			unsigned long offset;
			struct format_field *field =
				arg->int_array.field->dynarray.field;
			offset = tep_read_number(pevent,
=======
	case TEP_PRINT_INT_ARRAY: {
		void *num;
		int el_size;

		if (arg->int_array.field->type == TEP_PRINT_DYNAMIC_ARRAY) {
			unsigned long offset;
			struct tep_format_field *field =
				arg->int_array.field->dynarray.field;
			offset = tep_read_number(tep,
>>>>>>> upstream/android-13
						 data + field->offset,
						 field->size);
			num = data + (offset & 0xffff);
		} else {
			field = arg->int_array.field->field.field;
			if (!field) {
				str = arg->int_array.field->field.name;
				field = tep_find_any_field(event, str);
				if (!field)
					goto out_warning_field;
				arg->int_array.field->field.field = field;
			}
			num = data + field->offset;
		}
		len = eval_num_arg(data, size, event, arg->int_array.count);
		el_size = eval_num_arg(data, size, event,
				       arg->int_array.el_size);
		for (i = 0; i < len; i++) {
			if (i)
				trace_seq_putc(s, ' ');

			if (el_size == 1) {
				trace_seq_printf(s, "%u", *(uint8_t *)num);
			} else if (el_size == 2) {
				trace_seq_printf(s, "%u", *(uint16_t *)num);
			} else if (el_size == 4) {
				trace_seq_printf(s, "%u", *(uint32_t *)num);
			} else if (el_size == 8) {
				trace_seq_printf(s, "%"PRIu64, *(uint64_t *)num);
			} else {
				trace_seq_printf(s, "BAD SIZE:%d 0x%x",
						 el_size, *(uint8_t *)num);
				el_size = 1;
			}

			num += el_size;
		}
		break;
	}
<<<<<<< HEAD
	case PRINT_TYPE:
		break;
	case PRINT_STRING: {
		int str_offset;

		if (arg->string.offset == -1) {
			struct format_field *f;
=======
	case TEP_PRINT_TYPE:
		break;
	case TEP_PRINT_STRING: {
		int str_offset;

		if (arg->string.offset == -1) {
			struct tep_format_field *f;
>>>>>>> upstream/android-13

			f = tep_find_any_field(event, arg->string.string);
			arg->string.offset = f->offset;
		}
<<<<<<< HEAD
		str_offset = data2host4(pevent, data + arg->string.offset);
=======
		str_offset = data2host4(tep, *(unsigned int *)(data + arg->string.offset));
>>>>>>> upstream/android-13
		str_offset &= 0xffff;
		print_str_to_seq(s, format, len_arg, ((char *)data) + str_offset);
		break;
	}
<<<<<<< HEAD
	case PRINT_BSTRING:
		print_str_to_seq(s, format, len_arg, arg->string.string);
		break;
	case PRINT_BITMASK: {
=======
	case TEP_PRINT_BSTRING:
		print_str_to_seq(s, format, len_arg, arg->string.string);
		break;
	case TEP_PRINT_BITMASK: {
>>>>>>> upstream/android-13
		int bitmask_offset;
		int bitmask_size;

		if (arg->bitmask.offset == -1) {
<<<<<<< HEAD
			struct format_field *f;
=======
			struct tep_format_field *f;
>>>>>>> upstream/android-13

			f = tep_find_any_field(event, arg->bitmask.bitmask);
			arg->bitmask.offset = f->offset;
		}
<<<<<<< HEAD
		bitmask_offset = data2host4(pevent, data + arg->bitmask.offset);
		bitmask_size = bitmask_offset >> 16;
		bitmask_offset &= 0xffff;
		print_bitmask_to_seq(pevent, s, format, len_arg,
				     data + bitmask_offset, bitmask_size);
		break;
	}
	case PRINT_OP:
=======
		bitmask_offset = data2host4(tep, *(unsigned int *)(data + arg->bitmask.offset));
		bitmask_size = bitmask_offset >> 16;
		bitmask_offset &= 0xffff;
		print_bitmask_to_seq(tep, s, format, len_arg,
				     data + bitmask_offset, bitmask_size);
		break;
	}
	case TEP_PRINT_OP:
>>>>>>> upstream/android-13
		/*
		 * The only op for string should be ? :
		 */
		if (arg->op.op[0] != '?')
			return;
		val = eval_num_arg(data, size, event, arg->op.left);
		if (val)
			print_str_arg(s, data, size, event,
				      format, len_arg, arg->op.right->op.left);
		else
			print_str_arg(s, data, size, event,
				      format, len_arg, arg->op.right->op.right);
		break;
<<<<<<< HEAD
	case PRINT_FUNC:
=======
	case TEP_PRINT_FUNC:
>>>>>>> upstream/android-13
		process_defined_func(s, data, size, event, arg);
		break;
	default:
		/* well... */
		break;
	}

	return;

out_warning_field:
	do_warning_event(event, "%s: field %s not found",
			 __func__, arg->field.name);
}

static unsigned long long
process_defined_func(struct trace_seq *s, void *data, int size,
<<<<<<< HEAD
		     struct event_format *event, struct print_arg *arg)
=======
		     struct tep_event *event, struct tep_print_arg *arg)
>>>>>>> upstream/android-13
{
	struct tep_function_handler *func_handle = arg->func.func;
	struct func_params *param;
	unsigned long long *args;
	unsigned long long ret;
<<<<<<< HEAD
	struct print_arg *farg;
=======
	struct tep_print_arg *farg;
>>>>>>> upstream/android-13
	struct trace_seq str;
	struct save_str {
		struct save_str *next;
		char *str;
	} *strings = NULL, *string;
	int i;

	if (!func_handle->nr_args) {
		ret = (*func_handle->func)(s, NULL);
		goto out;
	}

	farg = arg->func.args;
	param = func_handle->params;

	ret = ULLONG_MAX;
	args = malloc(sizeof(*args) * func_handle->nr_args);
	if (!args)
		goto out;

	for (i = 0; i < func_handle->nr_args; i++) {
		switch (param->type) {
		case TEP_FUNC_ARG_INT:
		case TEP_FUNC_ARG_LONG:
		case TEP_FUNC_ARG_PTR:
			args[i] = eval_num_arg(data, size, event, farg);
			break;
		case TEP_FUNC_ARG_STRING:
			trace_seq_init(&str);
			print_str_arg(&str, data, size, event, "%s", -1, farg);
			trace_seq_terminate(&str);
			string = malloc(sizeof(*string));
			if (!string) {
				do_warning_event(event, "%s(%d): malloc str",
						 __func__, __LINE__);
				goto out_free;
			}
			string->next = strings;
			string->str = strdup(str.buffer);
			if (!string->str) {
				free(string);
				do_warning_event(event, "%s(%d): malloc str",
						 __func__, __LINE__);
				goto out_free;
			}
			args[i] = (uintptr_t)string->str;
			strings = string;
			trace_seq_destroy(&str);
			break;
		default:
			/*
			 * Something went totally wrong, this is not
			 * an input error, something in this code broke.
			 */
			do_warning_event(event, "Unexpected end of arguments\n");
			goto out_free;
		}
		farg = farg->next;
		param = param->next;
	}

	ret = (*func_handle->func)(s, args);
out_free:
	free(args);
	while (strings) {
		string = strings;
		strings = string->next;
		free(string->str);
		free(string);
	}

 out:
	/* TBD : handle return type here */
	return ret;
}

<<<<<<< HEAD
static void free_args(struct print_arg *args)
{
	struct print_arg *next;
=======
static void free_args(struct tep_print_arg *args)
{
	struct tep_print_arg *next;
>>>>>>> upstream/android-13

	while (args) {
		next = args->next;

		free_arg(args);
		args = next;
	}
}

<<<<<<< HEAD
static struct print_arg *make_bprint_args(char *fmt, void *data, int size, struct event_format *event)
{
	struct tep_handle *pevent = event->pevent;
	struct format_field *field, *ip_field;
	struct print_arg *args, *arg, **next;
	unsigned long long ip, val;
	char *ptr;
	void *bptr;
	int vsize;

	field = pevent->bprint_buf_field;
	ip_field = pevent->bprint_ip_field;
=======
static struct tep_print_arg *make_bprint_args(char *fmt, void *data, int size, struct tep_event *event)
{
	struct tep_handle *tep = event->tep;
	struct tep_format_field *field, *ip_field;
	struct tep_print_arg *args, *arg, **next;
	unsigned long long ip, val;
	char *ptr;
	void *bptr;
	int vsize = 0;

	field = tep->bprint_buf_field;
	ip_field = tep->bprint_ip_field;
>>>>>>> upstream/android-13

	if (!field) {
		field = tep_find_field(event, "buf");
		if (!field) {
			do_warning_event(event, "can't find buffer field for binary printk");
			return NULL;
		}
		ip_field = tep_find_field(event, "ip");
		if (!ip_field) {
			do_warning_event(event, "can't find ip field for binary printk");
			return NULL;
		}
<<<<<<< HEAD
		pevent->bprint_buf_field = field;
		pevent->bprint_ip_field = ip_field;
	}

	ip = tep_read_number(pevent, data + ip_field->offset, ip_field->size);
=======
		tep->bprint_buf_field = field;
		tep->bprint_ip_field = ip_field;
	}

	ip = tep_read_number(tep, data + ip_field->offset, ip_field->size);
>>>>>>> upstream/android-13

	/*
	 * The first arg is the IP pointer.
	 */
	args = alloc_arg();
	if (!args) {
		do_warning_event(event, "%s(%d): not enough memory!",
				 __func__, __LINE__);
		return NULL;
	}
	arg = args;
	arg->next = NULL;
	next = &arg->next;

<<<<<<< HEAD
	arg->type = PRINT_ATOM;
=======
	arg->type = TEP_PRINT_ATOM;
>>>>>>> upstream/android-13
		
	if (asprintf(&arg->atom.atom, "%lld", ip) < 0)
		goto out_free;

	/* skip the first "%ps: " */
	for (ptr = fmt + 5, bptr = data + field->offset;
	     bptr < data + size && *ptr; ptr++) {
		int ls = 0;

		if (*ptr == '%') {
 process_again:
			ptr++;
			switch (*ptr) {
			case '%':
				break;
			case 'l':
				ls++;
				goto process_again;
			case 'L':
				ls = 2;
				goto process_again;
			case '0' ... '9':
				goto process_again;
			case '.':
				goto process_again;
			case 'z':
			case 'Z':
				ls = 1;
				goto process_again;
			case 'p':
				ls = 1;
				if (isalnum(ptr[1])) {
					ptr++;
					/* Check for special pointers */
					switch (*ptr) {
					case 's':
					case 'S':
<<<<<<< HEAD
					case 'f':
					case 'F':
						break;
=======
					case 'x':
						break;
					case 'f':
					case 'F':
						/*
						 * Pre-5.5 kernels use %pf and
						 * %pF for printing symbols
						 * while kernels since 5.5 use
						 * %pfw for fwnodes. So check
						 * %p[fF] isn't followed by 'w'.
						 */
						if (ptr[1] != 'w')
							break;
						/* fall through */
>>>>>>> upstream/android-13
					default:
						/*
						 * Older kernels do not process
						 * dereferenced pointers.
						 * Only process if the pointer
						 * value is a printable.
						 */
						if (isprint(*(char *)bptr))
							goto process_string;
					}
				}
				/* fall through */
			case 'd':
			case 'u':
<<<<<<< HEAD
			case 'x':
			case 'i':
=======
			case 'i':
			case 'x':
			case 'X':
			case 'o':
>>>>>>> upstream/android-13
				switch (ls) {
				case 0:
					vsize = 4;
					break;
				case 1:
<<<<<<< HEAD
					vsize = pevent->long_size;
=======
					vsize = tep->long_size;
>>>>>>> upstream/android-13
					break;
				case 2:
					vsize = 8;
					break;
				default:
					vsize = ls; /* ? */
					break;
				}
			/* fall through */
			case '*':
				if (*ptr == '*')
					vsize = 4;

				/* the pointers are always 4 bytes aligned */
				bptr = (void *)(((unsigned long)bptr + 3) &
						~3);
<<<<<<< HEAD
				val = tep_read_number(pevent, bptr, vsize);
=======
				val = tep_read_number(tep, bptr, vsize);
>>>>>>> upstream/android-13
				bptr += vsize;
				arg = alloc_arg();
				if (!arg) {
					do_warning_event(event, "%s(%d): not enough memory!",
						   __func__, __LINE__);
					goto out_free;
				}
				arg->next = NULL;
<<<<<<< HEAD
				arg->type = PRINT_ATOM;
=======
				arg->type = TEP_PRINT_ATOM;
>>>>>>> upstream/android-13
				if (asprintf(&arg->atom.atom, "%lld", val) < 0) {
					free(arg);
					goto out_free;
				}
				*next = arg;
				next = &arg->next;
				/*
				 * The '*' case means that an arg is used as the length.
				 * We need to continue to figure out for what.
				 */
				if (*ptr == '*')
					goto process_again;

				break;
			case 's':
 process_string:
				arg = alloc_arg();
				if (!arg) {
					do_warning_event(event, "%s(%d): not enough memory!",
						   __func__, __LINE__);
					goto out_free;
				}
				arg->next = NULL;
<<<<<<< HEAD
				arg->type = PRINT_BSTRING;
=======
				arg->type = TEP_PRINT_BSTRING;
>>>>>>> upstream/android-13
				arg->string.string = strdup(bptr);
				if (!arg->string.string)
					goto out_free;
				bptr += strlen(bptr) + 1;
				*next = arg;
				next = &arg->next;
			default:
				break;
			}
		}
	}

	return args;

out_free:
	free_args(args);
	return NULL;
}

static char *
get_bprint_format(void *data, int size __maybe_unused,
<<<<<<< HEAD
		  struct event_format *event)
{
	struct tep_handle *pevent = event->pevent;
	unsigned long long addr;
	struct format_field *field;
	struct printk_map *printk;
	char *format;

	field = pevent->bprint_fmt_field;
=======
		  struct tep_event *event)
{
	struct tep_handle *tep = event->tep;
	unsigned long long addr;
	struct tep_format_field *field;
	struct printk_map *printk;
	char *format;

	field = tep->bprint_fmt_field;
>>>>>>> upstream/android-13

	if (!field) {
		field = tep_find_field(event, "fmt");
		if (!field) {
			do_warning_event(event, "can't find format field for binary printk");
			return NULL;
		}
<<<<<<< HEAD
		pevent->bprint_fmt_field = field;
	}

	addr = tep_read_number(pevent, data + field->offset, field->size);

	printk = find_printk(pevent, addr);
	if (!printk) {
		if (asprintf(&format, "%%pf: (NO FORMAT FOUND at %llx)\n", addr) < 0)
=======
		tep->bprint_fmt_field = field;
	}

	addr = tep_read_number(tep, data + field->offset, field->size);

	printk = find_printk(tep, addr);
	if (!printk) {
		if (asprintf(&format, "%%ps: (NO FORMAT FOUND at %llx)\n", addr) < 0)
>>>>>>> upstream/android-13
			return NULL;
		return format;
	}

<<<<<<< HEAD
	if (asprintf(&format, "%s: %s", "%pf", printk->printk) < 0)
=======
	if (asprintf(&format, "%s: %s", "%ps", printk->printk) < 0)
>>>>>>> upstream/android-13
		return NULL;

	return format;
}

<<<<<<< HEAD
static void print_mac_arg(struct trace_seq *s, int mac, void *data, int size,
			  struct event_format *event, struct print_arg *arg)
{
	unsigned char *buf;
	const char *fmt = "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x";

	if (arg->type == PRINT_FUNC) {
		process_defined_func(s, data, size, event, arg);
		return;
	}

	if (arg->type != PRINT_FIELD) {
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d",
				 arg->type);
		return;
	}

	if (mac == 'm')
		fmt = "%.2x%.2x%.2x%.2x%.2x%.2x";
=======
static int print_mac_arg(struct trace_seq *s, const char *format,
			 void *data, int size, struct tep_event *event,
			 struct tep_print_arg *arg)
{
	const char *fmt = "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x";
	bool reverse = false;
	unsigned char *buf;
	int ret = 0;

	if (arg->type == TEP_PRINT_FUNC) {
		process_defined_func(s, data, size, event, arg);
		return 0;
	}

	if (arg->type != TEP_PRINT_FIELD) {
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d",
				 arg->type);
		return 0;
	}

	if (format[0] == 'm') {
		fmt = "%.2x%.2x%.2x%.2x%.2x%.2x";
	} else if (format[0] == 'M' && format[1] == 'F') {
		fmt = "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x";
		ret++;
	}
	if (format[1] == 'R') {
		reverse = true;
		ret++;
	}

>>>>>>> upstream/android-13
	if (!arg->field.field) {
		arg->field.field =
			tep_find_any_field(event, arg->field.name);
		if (!arg->field.field) {
			do_warning_event(event, "%s: field %s not found",
					 __func__, arg->field.name);
<<<<<<< HEAD
			return;
=======
			return ret;
>>>>>>> upstream/android-13
		}
	}
	if (arg->field.field->size != 6) {
		trace_seq_printf(s, "INVALIDMAC");
<<<<<<< HEAD
		return;
	}
	buf = data + arg->field.field->offset;
	trace_seq_printf(s, fmt, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
}

static void print_ip4_addr(struct trace_seq *s, char i, unsigned char *buf)
=======
		return ret;
	}

	buf = data + arg->field.field->offset;
	if (reverse)
		trace_seq_printf(s, fmt, buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]);
	else
		trace_seq_printf(s, fmt, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

	return ret;
}

static int parse_ip4_print_args(struct tep_handle *tep,
				const char *ptr, bool *reverse)
{
	int ret = 0;

	*reverse = false;

	/* hnbl */
	switch (*ptr) {
	case 'h':
		if (tep->file_bigendian)
			*reverse = false;
		else
			*reverse = true;
		ret++;
		break;
	case 'l':
		*reverse = true;
		ret++;
		break;
	case 'n':
	case 'b':
		ret++;
		/* fall through */
	default:
		*reverse = false;
		break;
	}

	return ret;
}

static void print_ip4_addr(struct trace_seq *s, char i, bool reverse, unsigned char *buf)
>>>>>>> upstream/android-13
{
	const char *fmt;

	if (i == 'i')
		fmt = "%03d.%03d.%03d.%03d";
	else
		fmt = "%d.%d.%d.%d";

<<<<<<< HEAD
	trace_seq_printf(s, fmt, buf[0], buf[1], buf[2], buf[3]);
=======
	if (reverse)
		trace_seq_printf(s, fmt, buf[3], buf[2], buf[1], buf[0]);
	else
		trace_seq_printf(s, fmt, buf[0], buf[1], buf[2], buf[3]);

>>>>>>> upstream/android-13
}

static inline bool ipv6_addr_v4mapped(const struct in6_addr *a)
{
	return ((unsigned long)(a->s6_addr32[0] | a->s6_addr32[1]) |
		(unsigned long)(a->s6_addr32[2] ^ htonl(0x0000ffff))) == 0UL;
}

static inline bool ipv6_addr_is_isatap(const struct in6_addr *addr)
{
	return (addr->s6_addr32[2] | htonl(0x02000000)) == htonl(0x02005EFE);
}

static void print_ip6c_addr(struct trace_seq *s, unsigned char *addr)
{
	int i, j, range;
	unsigned char zerolength[8];
	int longest = 1;
	int colonpos = -1;
	uint16_t word;
	uint8_t hi, lo;
	bool needcolon = false;
	bool useIPv4;
	struct in6_addr in6;

	memcpy(&in6, addr, sizeof(struct in6_addr));

	useIPv4 = ipv6_addr_v4mapped(&in6) || ipv6_addr_is_isatap(&in6);

	memset(zerolength, 0, sizeof(zerolength));

	if (useIPv4)
		range = 6;
	else
		range = 8;

	/* find position of longest 0 run */
	for (i = 0; i < range; i++) {
		for (j = i; j < range; j++) {
			if (in6.s6_addr16[j] != 0)
				break;
			zerolength[i]++;
		}
	}
	for (i = 0; i < range; i++) {
		if (zerolength[i] > longest) {
			longest = zerolength[i];
			colonpos = i;
		}
	}
	if (longest == 1)		/* don't compress a single 0 */
		colonpos = -1;

	/* emit address */
	for (i = 0; i < range; i++) {
		if (i == colonpos) {
			if (needcolon || i == 0)
				trace_seq_printf(s, ":");
			trace_seq_printf(s, ":");
			needcolon = false;
			i += longest - 1;
			continue;
		}
		if (needcolon) {
			trace_seq_printf(s, ":");
			needcolon = false;
		}
		/* hex u16 without leading 0s */
		word = ntohs(in6.s6_addr16[i]);
		hi = word >> 8;
		lo = word & 0xff;
		if (hi)
			trace_seq_printf(s, "%x%02x", hi, lo);
		else
			trace_seq_printf(s, "%x", lo);

		needcolon = true;
	}

	if (useIPv4) {
		if (needcolon)
			trace_seq_printf(s, ":");
<<<<<<< HEAD
		print_ip4_addr(s, 'I', &in6.s6_addr[12]);
=======
		print_ip4_addr(s, 'I', false, &in6.s6_addr[12]);
>>>>>>> upstream/android-13
	}

	return;
}

static void print_ip6_addr(struct trace_seq *s, char i, unsigned char *buf)
{
	int j;

	for (j = 0; j < 16; j += 2) {
		trace_seq_printf(s, "%02x%02x", buf[j], buf[j+1]);
		if (i == 'I' && j < 14)
			trace_seq_printf(s, ":");
	}
}

/*
 * %pi4   print an IPv4 address with leading zeros
 * %pI4   print an IPv4 address without leading zeros
 * %pi6   print an IPv6 address without colons
 * %pI6   print an IPv6 address with colons
 * %pI6c  print an IPv6 address in compressed form with colons
 * %pISpc print an IP address based on sockaddr; p adds port.
 */
static int print_ipv4_arg(struct trace_seq *s, const char *ptr, char i,
<<<<<<< HEAD
			  void *data, int size, struct event_format *event,
			  struct print_arg *arg)
{
	unsigned char *buf;

	if (arg->type == PRINT_FUNC) {
		process_defined_func(s, data, size, event, arg);
		return 0;
	}

	if (arg->type != PRINT_FIELD) {
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d", arg->type);
		return 0;
=======
			  void *data, int size, struct tep_event *event,
			  struct tep_print_arg *arg)
{
	bool reverse = false;
	unsigned char *buf;
	int ret;

	ret = parse_ip4_print_args(event->tep, ptr, &reverse);

	if (arg->type == TEP_PRINT_FUNC) {
		process_defined_func(s, data, size, event, arg);
		return ret;
	}

	if (arg->type != TEP_PRINT_FIELD) {
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d", arg->type);
		return ret;
>>>>>>> upstream/android-13
	}

	if (!arg->field.field) {
		arg->field.field =
			tep_find_any_field(event, arg->field.name);
		if (!arg->field.field) {
			do_warning("%s: field %s not found",
				   __func__, arg->field.name);
<<<<<<< HEAD
			return 0;
=======
			return ret;
>>>>>>> upstream/android-13
		}
	}

	buf = data + arg->field.field->offset;

	if (arg->field.field->size != 4) {
		trace_seq_printf(s, "INVALIDIPv4");
<<<<<<< HEAD
		return 0;
	}
	print_ip4_addr(s, i, buf);

	return 0;
}

static int print_ipv6_arg(struct trace_seq *s, const char *ptr, char i,
			  void *data, int size, struct event_format *event,
			  struct print_arg *arg)
=======
		return ret;
	}

	print_ip4_addr(s, i, reverse, buf);
	return ret;

}

static int print_ipv6_arg(struct trace_seq *s, const char *ptr, char i,
			  void *data, int size, struct tep_event *event,
			  struct tep_print_arg *arg)
>>>>>>> upstream/android-13
{
	char have_c = 0;
	unsigned char *buf;
	int rc = 0;

	/* pI6c */
	if (i == 'I' && *ptr == 'c') {
		have_c = 1;
		ptr++;
		rc++;
	}

<<<<<<< HEAD
	if (arg->type == PRINT_FUNC) {
=======
	if (arg->type == TEP_PRINT_FUNC) {
>>>>>>> upstream/android-13
		process_defined_func(s, data, size, event, arg);
		return rc;
	}

<<<<<<< HEAD
	if (arg->type != PRINT_FIELD) {
=======
	if (arg->type != TEP_PRINT_FIELD) {
>>>>>>> upstream/android-13
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d", arg->type);
		return rc;
	}

	if (!arg->field.field) {
		arg->field.field =
			tep_find_any_field(event, arg->field.name);
		if (!arg->field.field) {
			do_warning("%s: field %s not found",
				   __func__, arg->field.name);
			return rc;
		}
	}

	buf = data + arg->field.field->offset;

	if (arg->field.field->size != 16) {
		trace_seq_printf(s, "INVALIDIPv6");
		return rc;
	}

	if (have_c)
		print_ip6c_addr(s, buf);
	else
		print_ip6_addr(s, i, buf);

	return rc;
}

static int print_ipsa_arg(struct trace_seq *s, const char *ptr, char i,
<<<<<<< HEAD
			  void *data, int size, struct event_format *event,
			  struct print_arg *arg)
=======
			  void *data, int size, struct tep_event *event,
			  struct tep_print_arg *arg)
>>>>>>> upstream/android-13
{
	char have_c = 0, have_p = 0;
	unsigned char *buf;
	struct sockaddr_storage *sa;
<<<<<<< HEAD
	int rc = 0;
=======
	bool reverse = false;
	int rc = 0;
	int ret;
>>>>>>> upstream/android-13

	/* pISpc */
	if (i == 'I') {
		if (*ptr == 'p') {
			have_p = 1;
			ptr++;
			rc++;
		}
		if (*ptr == 'c') {
			have_c = 1;
			ptr++;
			rc++;
		}
	}
<<<<<<< HEAD

	if (arg->type == PRINT_FUNC) {
=======
	ret = parse_ip4_print_args(event->tep, ptr, &reverse);
	ptr += ret;
	rc += ret;

	if (arg->type == TEP_PRINT_FUNC) {
>>>>>>> upstream/android-13
		process_defined_func(s, data, size, event, arg);
		return rc;
	}

<<<<<<< HEAD
	if (arg->type != PRINT_FIELD) {
=======
	if (arg->type != TEP_PRINT_FIELD) {
>>>>>>> upstream/android-13
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d", arg->type);
		return rc;
	}

	if (!arg->field.field) {
		arg->field.field =
			tep_find_any_field(event, arg->field.name);
		if (!arg->field.field) {
			do_warning("%s: field %s not found",
				   __func__, arg->field.name);
			return rc;
		}
	}

	sa = (struct sockaddr_storage *) (data + arg->field.field->offset);

	if (sa->ss_family == AF_INET) {
		struct sockaddr_in *sa4 = (struct sockaddr_in *) sa;

		if (arg->field.field->size < sizeof(struct sockaddr_in)) {
			trace_seq_printf(s, "INVALIDIPv4");
			return rc;
		}

<<<<<<< HEAD
		print_ip4_addr(s, i, (unsigned char *) &sa4->sin_addr);
=======
		print_ip4_addr(s, i, reverse, (unsigned char *) &sa4->sin_addr);
>>>>>>> upstream/android-13
		if (have_p)
			trace_seq_printf(s, ":%d", ntohs(sa4->sin_port));


	} else if (sa->ss_family == AF_INET6) {
		struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *) sa;

		if (arg->field.field->size < sizeof(struct sockaddr_in6)) {
			trace_seq_printf(s, "INVALIDIPv6");
			return rc;
		}

		if (have_p)
			trace_seq_printf(s, "[");

		buf = (unsigned char *) &sa6->sin6_addr;
		if (have_c)
			print_ip6c_addr(s, buf);
		else
			print_ip6_addr(s, i, buf);

		if (have_p)
			trace_seq_printf(s, "]:%d", ntohs(sa6->sin6_port));
	}

	return rc;
}

static int print_ip_arg(struct trace_seq *s, const char *ptr,
<<<<<<< HEAD
			void *data, int size, struct event_format *event,
			struct print_arg *arg)
{
	char i = *ptr;  /* 'i' or 'I' */
	char ver;
	int rc = 0;

	ptr++;
	rc++;

	ver = *ptr;
	ptr++;
	rc++;

	switch (ver) {
	case '4':
		rc += print_ipv4_arg(s, ptr, i, data, size, event, arg);
		break;
	case '6':
		rc += print_ipv6_arg(s, ptr, i, data, size, event, arg);
		break;
	case 'S':
		rc += print_ipsa_arg(s, ptr, i, data, size, event, arg);
=======
			void *data, int size, struct tep_event *event,
			struct tep_print_arg *arg)
{
	char i = *ptr;  /* 'i' or 'I' */
	int rc = 1;

	/* IP version */
	ptr++;

	switch (*ptr) {
	case '4':
		rc += print_ipv4_arg(s, ptr + 1, i, data, size, event, arg);
		break;
	case '6':
		rc += print_ipv6_arg(s, ptr + 1, i, data, size, event, arg);
		break;
	case 'S':
		rc += print_ipsa_arg(s, ptr + 1, i, data, size, event, arg);
>>>>>>> upstream/android-13
		break;
	default:
		return 0;
	}

	return rc;
}

<<<<<<< HEAD
=======
static const int guid_index[16] = {3, 2, 1, 0, 5, 4, 7, 6, 8, 9, 10, 11, 12, 13, 14, 15};
static const int uuid_index[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

static int print_uuid_arg(struct trace_seq *s, const char *ptr,
			void *data, int size, struct tep_event *event,
			struct tep_print_arg *arg)
{
	const int *index = uuid_index;
	char *format = "%02x";
	int ret = 0;
	char *buf;
	int i;

	switch (*(ptr + 1)) {
	case 'L':
		format = "%02X";
		/* fall through */
	case 'l':
		index = guid_index;
		ret++;
		break;
	case 'B':
		format = "%02X";
		/* fall through */
	case 'b':
		ret++;
		break;
	}

	if (arg->type == TEP_PRINT_FUNC) {
		process_defined_func(s, data, size, event, arg);
		return ret;
	}

	if (arg->type != TEP_PRINT_FIELD) {
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d", arg->type);
		return ret;
	}

	if (!arg->field.field) {
		arg->field.field =
			tep_find_any_field(event, arg->field.name);
		if (!arg->field.field) {
			do_warning("%s: field %s not found",
				   __func__, arg->field.name);
			return ret;
		}
	}

	if (arg->field.field->size != 16) {
		trace_seq_printf(s, "INVALIDUUID");
		return ret;
	}

	buf = data + arg->field.field->offset;

	for (i = 0; i < 16; i++) {
		trace_seq_printf(s, format, buf[index[i]] & 0xff);
		switch (i) {
		case 3:
		case 5:
		case 7:
		case 9:
			trace_seq_printf(s, "-");
			break;
		}
	}

	return ret;
}

static int print_raw_buff_arg(struct trace_seq *s, const char *ptr,
			      void *data, int size, struct tep_event *event,
			      struct tep_print_arg *arg, int print_len)
{
	int plen = print_len;
	char *delim = " ";
	int ret = 0;
	char *buf;
	int i;
	unsigned long offset;
	int arr_len;

	switch (*(ptr + 1)) {
	case 'C':
		delim = ":";
		ret++;
		break;
	case 'D':
		delim = "-";
		ret++;
		break;
	case 'N':
		delim = "";
		ret++;
		break;
	}

	if (arg->type == TEP_PRINT_FUNC) {
		process_defined_func(s, data, size, event, arg);
		return ret;
	}

	if (arg->type != TEP_PRINT_DYNAMIC_ARRAY) {
		trace_seq_printf(s, "ARG TYPE NOT FIELD BUT %d", arg->type);
		return ret;
	}

	offset = tep_read_number(event->tep,
				 data + arg->dynarray.field->offset,
				 arg->dynarray.field->size);
	arr_len = (unsigned long long)(offset >> 16);
	buf = data + (offset & 0xffff);

	if (arr_len < plen)
		plen = arr_len;

	if (plen < 1)
		return ret;

	trace_seq_printf(s, "%02x", buf[0] & 0xff);
	for (i = 1; i < plen; i++)
		trace_seq_printf(s, "%s%02x", delim, buf[i] & 0xff);

	return ret;
}

>>>>>>> upstream/android-13
static int is_printable_array(char *p, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len && p[i]; i++)
		if (!isprint(p[i]) && !isspace(p[i]))
		    return 0;
	return 1;
}

void tep_print_field(struct trace_seq *s, void *data,
<<<<<<< HEAD
		     struct format_field *field)
{
	unsigned long long val;
	unsigned int offset, len, i;
	struct tep_handle *pevent = field->event->pevent;

	if (field->flags & FIELD_IS_ARRAY) {
		offset = field->offset;
		len = field->size;
		if (field->flags & FIELD_IS_DYNAMIC) {
			val = tep_read_number(pevent, data + offset, len);
=======
		     struct tep_format_field *field)
{
	unsigned long long val;
	unsigned int offset, len, i;
	struct tep_handle *tep = field->event->tep;

	if (field->flags & TEP_FIELD_IS_ARRAY) {
		offset = field->offset;
		len = field->size;
		if (field->flags & TEP_FIELD_IS_DYNAMIC) {
			val = tep_read_number(tep, data + offset, len);
>>>>>>> upstream/android-13
			offset = val;
			len = offset >> 16;
			offset &= 0xffff;
		}
<<<<<<< HEAD
		if (field->flags & FIELD_IS_STRING &&
=======
		if (field->flags & TEP_FIELD_IS_STRING &&
>>>>>>> upstream/android-13
		    is_printable_array(data + offset, len)) {
			trace_seq_printf(s, "%s", (char *)data + offset);
		} else {
			trace_seq_puts(s, "ARRAY[");
			for (i = 0; i < len; i++) {
				if (i)
					trace_seq_puts(s, ", ");
				trace_seq_printf(s, "%02x",
						 *((unsigned char *)data + offset + i));
			}
			trace_seq_putc(s, ']');
<<<<<<< HEAD
			field->flags &= ~FIELD_IS_STRING;
		}
	} else {
		val = tep_read_number(pevent, data + field->offset,
				      field->size);
		if (field->flags & FIELD_IS_POINTER) {
			trace_seq_printf(s, "0x%llx", val);
		} else if (field->flags & FIELD_IS_SIGNED) {
=======
			field->flags &= ~TEP_FIELD_IS_STRING;
		}
	} else {
		val = tep_read_number(tep, data + field->offset,
				      field->size);
		if (field->flags & TEP_FIELD_IS_POINTER) {
			trace_seq_printf(s, "0x%llx", val);
		} else if (field->flags & TEP_FIELD_IS_SIGNED) {
>>>>>>> upstream/android-13
			switch (field->size) {
			case 4:
				/*
				 * If field is long then print it in hex.
				 * A long usually stores pointers.
				 */
<<<<<<< HEAD
				if (field->flags & FIELD_IS_LONG)
=======
				if (field->flags & TEP_FIELD_IS_LONG)
>>>>>>> upstream/android-13
					trace_seq_printf(s, "0x%x", (int)val);
				else
					trace_seq_printf(s, "%d", (int)val);
				break;
			case 2:
				trace_seq_printf(s, "%2d", (short)val);
				break;
			case 1:
				trace_seq_printf(s, "%1d", (char)val);
				break;
			default:
				trace_seq_printf(s, "%lld", val);
			}
		} else {
<<<<<<< HEAD
			if (field->flags & FIELD_IS_LONG)
=======
			if (field->flags & TEP_FIELD_IS_LONG)
>>>>>>> upstream/android-13
				trace_seq_printf(s, "0x%llx", val);
			else
				trace_seq_printf(s, "%llu", val);
		}
	}
}

void tep_print_fields(struct trace_seq *s, void *data,
<<<<<<< HEAD
		      int size __maybe_unused, struct event_format *event)
{
	struct format_field *field;
=======
		      int size __maybe_unused, struct tep_event *event)
{
	struct tep_format_field *field;
>>>>>>> upstream/android-13

	field = event->format.fields;
	while (field) {
		trace_seq_printf(s, " %s=", field->name);
		tep_print_field(s, data, field);
		field = field->next;
	}
}

<<<<<<< HEAD
static void pretty_print(struct trace_seq *s, void *data, int size, struct event_format *event)
{
	struct tep_handle *pevent = event->pevent;
	struct print_fmt *print_fmt = &event->print_fmt;
	struct print_arg *arg = print_fmt->args;
	struct print_arg *args = NULL;
	const char *ptr = print_fmt->format;
	unsigned long long val;
	struct func_map *func;
	const char *saveptr;
	struct trace_seq p;
	char *bprint_fmt = NULL;
	char format[32];
	int show_func;
	int len_as_arg;
	int len_arg;
	int len;
	int ls;

	if (event->flags & EVENT_FL_FAILED) {
=======
static int print_function(struct trace_seq *s, const char *format,
			  void *data, int size, struct tep_event *event,
			  struct tep_print_arg *arg)
{
	struct func_map *func;
	unsigned long long val;

	val = eval_num_arg(data, size, event, arg);
	func = find_func(event->tep, val);
	if (func) {
		trace_seq_puts(s, func->func);
		if (*format == 'F' || *format == 'S')
			trace_seq_printf(s, "+0x%llx", val - func->addr);
	} else {
		if (event->tep->long_size == 4)
			trace_seq_printf(s, "0x%lx", (long)val);
		else
			trace_seq_printf(s, "0x%llx", (long long)val);
	}

	return 0;
}

static int print_arg_pointer(struct trace_seq *s, const char *format, int plen,
			     void *data, int size,
			     struct tep_event *event, struct tep_print_arg *arg)
{
	unsigned long long val;
	int ret = 1;

	if (arg->type == TEP_PRINT_BSTRING) {
		trace_seq_puts(s, arg->string.string);
		return 0;
	}
	while (*format) {
		if (*format == 'p') {
			format++;
			break;
		}
		format++;
	}

	switch (*format) {
	case 'F':
	case 'f':
	case 'S':
	case 's':
		ret += print_function(s, format, data, size, event, arg);
		break;
	case 'M':
	case 'm':
		ret += print_mac_arg(s, format, data, size, event, arg);
		break;
	case 'I':
	case 'i':
		ret += print_ip_arg(s, format, data, size, event, arg);
		break;
	case 'U':
		ret += print_uuid_arg(s, format, data, size, event, arg);
		break;
	case 'h':
		ret += print_raw_buff_arg(s, format, data, size, event, arg, plen);
		break;
	default:
		ret = 0;
		val = eval_num_arg(data, size, event, arg);
		trace_seq_printf(s, "%p", (void *)(intptr_t)val);
		break;
	}

	return ret;

}

static int print_arg_number(struct trace_seq *s, const char *format, int plen,
			    void *data, int size, int ls,
			    struct tep_event *event, struct tep_print_arg *arg)
{
	unsigned long long val;

	val = eval_num_arg(data, size, event, arg);

	switch (ls) {
	case -2:
		if (plen >= 0)
			trace_seq_printf(s, format, plen, (char)val);
		else
			trace_seq_printf(s, format, (char)val);
		break;
	case -1:
		if (plen >= 0)
			trace_seq_printf(s, format, plen, (short)val);
		else
			trace_seq_printf(s, format, (short)val);
		break;
	case 0:
		if (plen >= 0)
			trace_seq_printf(s, format, plen, (int)val);
		else
			trace_seq_printf(s, format, (int)val);
		break;
	case 1:
		if (plen >= 0)
			trace_seq_printf(s, format, plen, (long)val);
		else
			trace_seq_printf(s, format, (long)val);
		break;
	case 2:
		if (plen >= 0)
			trace_seq_printf(s, format, plen, (long long)val);
		else
			trace_seq_printf(s, format, (long long)val);
		break;
	default:
		do_warning_event(event, "bad count (%d)", ls);
		event->flags |= TEP_EVENT_FL_FAILED;
	}
	return 0;
}


static void print_arg_string(struct trace_seq *s, const char *format, int plen,
			     void *data, int size,
			     struct tep_event *event, struct tep_print_arg *arg)
{
	struct trace_seq p;

	/* Use helper trace_seq */
	trace_seq_init(&p);
	print_str_arg(&p, data, size, event,
		      format, plen, arg);
	trace_seq_terminate(&p);
	trace_seq_puts(s, p.buffer);
	trace_seq_destroy(&p);
}

static int parse_arg_format_pointer(const char *format)
{
	int ret = 0;
	int index;
	int loop;

	switch (*format) {
	case 'F':
	case 'S':
	case 'f':
	case 's':
		ret++;
		break;
	case 'M':
	case 'm':
		/* [mM]R , [mM]F */
		switch (format[1]) {
		case 'R':
		case 'F':
			ret++;
			break;
		}
		ret++;
		break;
	case 'I':
	case 'i':
		index = 2;
		loop = 1;
		switch (format[1]) {
		case 'S':
			/*[S][pfs]*/
			while (loop) {
				switch (format[index]) {
				case 'p':
				case 'f':
				case 's':
					ret++;
					index++;
					break;
				default:
					loop = 0;
					break;
				}
			}
			/* fall through */
		case '4':
			/* [4S][hnbl] */
			switch (format[index]) {
			case 'h':
			case 'n':
			case 'l':
			case 'b':
				ret++;
				index++;
				break;
			}
			if (format[1] == '4') {
				ret++;
				break;
			}
			/* fall through */
		case '6':
			/* [6S]c */
			if (format[index] == 'c')
				ret++;
			ret++;
			break;
		}
		ret++;
		break;
	case 'U':
		switch (format[1]) {
		case 'L':
		case 'l':
		case 'B':
		case 'b':
			ret++;
			break;
		}
		ret++;
		break;
	case 'h':
		switch (format[1]) {
		case 'C':
		case 'D':
		case 'N':
			ret++;
			break;
		}
		ret++;
		break;
	default:
		break;
	}

	return ret;
}

static void free_parse_args(struct tep_print_parse *arg)
{
	struct tep_print_parse *del;

	while (arg) {
		del = arg;
		arg = del->next;
		free(del->format);
		free(del);
	}
}

static int parse_arg_add(struct tep_print_parse **parse, char *format,
			 enum tep_print_parse_type type,
			 struct tep_print_arg *arg,
			 struct tep_print_arg *len_as_arg,
			 int ls)
{
	struct tep_print_parse *parg = NULL;

	parg = calloc(1, sizeof(*parg));
	if (!parg)
		goto error;
	parg->format = strdup(format);
	if (!parg->format)
		goto error;
	parg->type = type;
	parg->arg = arg;
	parg->len_as_arg = len_as_arg;
	parg->ls = ls;
	*parse = parg;
	return 0;
error:
	if (parg) {
		free(parg->format);
		free(parg);
	}
	return -1;
}

static int parse_arg_format(struct tep_print_parse **parse,
			    struct tep_event *event,
			    const char *format, struct tep_print_arg **arg)
{
	struct tep_print_arg *len_arg = NULL;
	char print_format[32];
	const char *start = format;
	int ret = 0;
	int ls = 0;
	int res;
	int len;

	format++;
	ret++;
	for (; *format; format++) {
		switch (*format) {
		case '#':
			/* FIXME: need to handle properly */
			break;
		case 'h':
			ls--;
			break;
		case 'l':
			ls++;
			break;
		case 'L':
			ls = 2;
			break;
		case '.':
		case 'z':
		case 'Z':
		case '0' ... '9':
		case '-':
			break;
		case '*':
			/* The argument is the length. */
			if (!*arg) {
				do_warning_event(event, "no argument match");
				event->flags |= TEP_EVENT_FL_FAILED;
				goto out_failed;
			}
			if (len_arg) {
				do_warning_event(event, "argument already matched");
				event->flags |= TEP_EVENT_FL_FAILED;
				goto out_failed;
			}
			len_arg = *arg;
			*arg = (*arg)->next;
			break;
		case 'p':
			if (!*arg) {
				do_warning_event(event, "no argument match");
				event->flags |= TEP_EVENT_FL_FAILED;
				goto out_failed;
			}
			res = parse_arg_format_pointer(format + 1);
			if (res > 0) {
				format += res;
				ret += res;
			}
			len = ((unsigned long)format + 1) -
				(unsigned long)start;
			/* should never happen */
			if (len > 31) {
				do_warning_event(event, "bad format!");
				event->flags |= TEP_EVENT_FL_FAILED;
				len = 31;
			}
			memcpy(print_format, start, len);
			print_format[len] = 0;

			parse_arg_add(parse, print_format,
				      PRINT_FMT_ARG_POINTER, *arg, len_arg, ls);
			*arg = (*arg)->next;
			ret++;
			return ret;
		case 'd':
		case 'u':
		case 'i':
		case 'x':
		case 'X':
		case 'o':
			if (!*arg) {
				do_warning_event(event, "no argument match");
				event->flags |= TEP_EVENT_FL_FAILED;
				goto out_failed;
			}

			len = ((unsigned long)format + 1) -
				(unsigned long)start;

			/* should never happen */
			if (len > 30) {
				do_warning_event(event, "bad format!");
				event->flags |= TEP_EVENT_FL_FAILED;
				len = 31;
			}
			memcpy(print_format, start, len);
			print_format[len] = 0;

			if (event->tep->long_size == 8 && ls == 1 &&
			    sizeof(long) != 8) {
				char *p;

				/* make %l into %ll */
				if (ls == 1 && (p = strchr(print_format, 'l')))
					memmove(p+1, p, strlen(p)+1);
				ls = 2;
			}
			if (ls < -2 || ls > 2) {
				do_warning_event(event, "bad count (%d)", ls);
				event->flags |= TEP_EVENT_FL_FAILED;
			}
			parse_arg_add(parse, print_format,
				      PRINT_FMT_ARG_DIGIT, *arg, len_arg, ls);
			*arg = (*arg)->next;
			ret++;
			return ret;
		case 's':
			if (!*arg) {
				do_warning_event(event, "no matching argument");
				event->flags |= TEP_EVENT_FL_FAILED;
				goto out_failed;
			}

			len = ((unsigned long)format + 1) -
				(unsigned long)start;

			/* should never happen */
			if (len > 31) {
				do_warning_event(event, "bad format!");
				event->flags |= TEP_EVENT_FL_FAILED;
				len = 31;
			}

			memcpy(print_format, start, len);
			print_format[len] = 0;

			parse_arg_add(parse, print_format,
					PRINT_FMT_ARG_STRING, *arg, len_arg, 0);
			*arg = (*arg)->next;
			ret++;
			return ret;
		default:
			snprintf(print_format, 32, ">%c<", *format);
			parse_arg_add(parse, print_format,
					PRINT_FMT_STRING, NULL, NULL, 0);
			ret++;
			return ret;
		}
		ret++;
	}

out_failed:
	return ret;

}

static int parse_arg_string(struct tep_print_parse **parse, const char *format)
{
	struct trace_seq s;
	int ret = 0;

	trace_seq_init(&s);
	for (; *format; format++) {
		if (*format == '\\') {
			format++;
			ret++;
			switch (*format) {
			case 'n':
				trace_seq_putc(&s, '\n');
				break;
			case 't':
				trace_seq_putc(&s, '\t');
				break;
			case 'r':
				trace_seq_putc(&s, '\r');
				break;
			case '\\':
				trace_seq_putc(&s, '\\');
				break;
			default:
				trace_seq_putc(&s, *format);
				break;
			}
		} else if (*format == '%') {
			if (*(format + 1) == '%') {
				trace_seq_putc(&s, '%');
				format++;
				ret++;
			} else
				break;
		} else
			trace_seq_putc(&s, *format);

		ret++;
	}
	trace_seq_terminate(&s);
	parse_arg_add(parse, s.buffer, PRINT_FMT_STRING, NULL, NULL, 0);
	trace_seq_destroy(&s);

	return ret;
}

static struct tep_print_parse *
parse_args(struct tep_event *event, const char *format, struct tep_print_arg *arg)
{
	struct tep_print_parse *parse_ret = NULL;
	struct tep_print_parse **parse = NULL;
	int ret;
	int len;

	len = strlen(format);
	while (*format) {
		if (!parse_ret)
			parse = &parse_ret;
		if (*format == '%' && *(format + 1) != '%')
			ret = parse_arg_format(parse, event, format, &arg);
		else
			ret = parse_arg_string(parse, format);
		if (*parse)
			parse = &((*parse)->next);

		len -= ret;
		if (len > 0)
			format += ret;
		else
			break;
	}
	return parse_ret;
}

static void print_event_cache(struct tep_print_parse *parse, struct trace_seq *s,
			      void *data, int size, struct tep_event *event)
{
	int len_arg;

	while (parse) {
		if (parse->len_as_arg)
			len_arg = eval_num_arg(data, size, event, parse->len_as_arg);
		switch (parse->type) {
		case PRINT_FMT_ARG_DIGIT:
			print_arg_number(s, parse->format,
					parse->len_as_arg ? len_arg : -1, data,
					 size, parse->ls, event, parse->arg);
			break;
		case PRINT_FMT_ARG_POINTER:
			print_arg_pointer(s, parse->format,
					  parse->len_as_arg ? len_arg : 1,
					  data, size, event, parse->arg);
			break;
		case PRINT_FMT_ARG_STRING:
			print_arg_string(s, parse->format,
					 parse->len_as_arg ? len_arg : -1,
					 data, size, event, parse->arg);
			break;
		case PRINT_FMT_STRING:
		default:
			trace_seq_printf(s, "%s", parse->format);
			break;
		}
		parse = parse->next;
	}
}

static void pretty_print(struct trace_seq *s, void *data, int size, struct tep_event *event)
{
	struct tep_print_parse *parse = event->print_fmt.print_cache;
	struct tep_print_arg *args = NULL;
	char *bprint_fmt = NULL;

	if (event->flags & TEP_EVENT_FL_FAILED) {
>>>>>>> upstream/android-13
		trace_seq_printf(s, "[FAILED TO PARSE]");
		tep_print_fields(s, data, size, event);
		return;
	}

<<<<<<< HEAD
	if (event->flags & EVENT_FL_ISBPRINT) {
		bprint_fmt = get_bprint_format(data, size, event);
		args = make_bprint_args(bprint_fmt, data, size, event);
		arg = args;
		ptr = bprint_fmt;
	}

	for (; *ptr; ptr++) {
		ls = 0;
		if (*ptr == '\\') {
			ptr++;
			switch (*ptr) {
			case 'n':
				trace_seq_putc(s, '\n');
				break;
			case 't':
				trace_seq_putc(s, '\t');
				break;
			case 'r':
				trace_seq_putc(s, '\r');
				break;
			case '\\':
				trace_seq_putc(s, '\\');
				break;
			default:
				trace_seq_putc(s, *ptr);
				break;
			}

		} else if (*ptr == '%') {
			saveptr = ptr;
			show_func = 0;
			len_as_arg = 0;
 cont_process:
			ptr++;
			switch (*ptr) {
			case '%':
				trace_seq_putc(s, '%');
				break;
			case '#':
				/* FIXME: need to handle properly */
				goto cont_process;
			case 'h':
				ls--;
				goto cont_process;
			case 'l':
				ls++;
				goto cont_process;
			case 'L':
				ls = 2;
				goto cont_process;
			case '*':
				/* The argument is the length. */
				if (!arg) {
					do_warning_event(event, "no argument match");
					event->flags |= EVENT_FL_FAILED;
					goto out_failed;
				}
				len_arg = eval_num_arg(data, size, event, arg);
				len_as_arg = 1;
				arg = arg->next;
				goto cont_process;
			case '.':
			case 'z':
			case 'Z':
			case '0' ... '9':
			case '-':
				goto cont_process;
			case 'p':
				if (pevent->long_size == 4)
					ls = 1;
				else
					ls = 2;

				if (isalnum(ptr[1]))
					ptr++;

				if (arg->type == PRINT_BSTRING) {
					trace_seq_puts(s, arg->string.string);
					arg = arg->next;
					break;
				}

				if (*ptr == 'F' || *ptr == 'f' ||
				    *ptr == 'S' || *ptr == 's') {
					show_func = *ptr;
				} else if (*ptr == 'M' || *ptr == 'm') {
					print_mac_arg(s, *ptr, data, size, event, arg);
					arg = arg->next;
					break;
				} else if (*ptr == 'I' || *ptr == 'i') {
					int n;

					n = print_ip_arg(s, ptr, data, size, event, arg);
					if (n > 0) {
						ptr += n - 1;
						arg = arg->next;
						break;
					}
				}

				/* fall through */
			case 'd':
			case 'i':
			case 'x':
			case 'X':
			case 'u':
				if (!arg) {
					do_warning_event(event, "no argument match");
					event->flags |= EVENT_FL_FAILED;
					goto out_failed;
				}

				len = ((unsigned long)ptr + 1) -
					(unsigned long)saveptr;

				/* should never happen */
				if (len > 31) {
					do_warning_event(event, "bad format!");
					event->flags |= EVENT_FL_FAILED;
					len = 31;
				}

				memcpy(format, saveptr, len);
				format[len] = 0;

				val = eval_num_arg(data, size, event, arg);
				arg = arg->next;

				if (show_func) {
					func = find_func(pevent, val);
					if (func) {
						trace_seq_puts(s, func->func);
						if (show_func == 'F')
							trace_seq_printf(s,
							       "+0x%llx",
							       val - func->addr);
						break;
					}
				}
				if (pevent->long_size == 8 && ls == 1 &&
				    sizeof(long) != 8) {
					char *p;

					/* make %l into %ll */
					if (ls == 1 && (p = strchr(format, 'l')))
						memmove(p+1, p, strlen(p)+1);
					else if (strcmp(format, "%p") == 0)
						strcpy(format, "0x%llx");
					ls = 2;
				}
				switch (ls) {
				case -2:
					if (len_as_arg)
						trace_seq_printf(s, format, len_arg, (char)val);
					else
						trace_seq_printf(s, format, (char)val);
					break;
				case -1:
					if (len_as_arg)
						trace_seq_printf(s, format, len_arg, (short)val);
					else
						trace_seq_printf(s, format, (short)val);
					break;
				case 0:
					if (len_as_arg)
						trace_seq_printf(s, format, len_arg, (int)val);
					else
						trace_seq_printf(s, format, (int)val);
					break;
				case 1:
					if (len_as_arg)
						trace_seq_printf(s, format, len_arg, (long)val);
					else
						trace_seq_printf(s, format, (long)val);
					break;
				case 2:
					if (len_as_arg)
						trace_seq_printf(s, format, len_arg,
								 (long long)val);
					else
						trace_seq_printf(s, format, (long long)val);
					break;
				default:
					do_warning_event(event, "bad count (%d)", ls);
					event->flags |= EVENT_FL_FAILED;
				}
				break;
			case 's':
				if (!arg) {
					do_warning_event(event, "no matching argument");
					event->flags |= EVENT_FL_FAILED;
					goto out_failed;
				}

				len = ((unsigned long)ptr + 1) -
					(unsigned long)saveptr;

				/* should never happen */
				if (len > 31) {
					do_warning_event(event, "bad format!");
					event->flags |= EVENT_FL_FAILED;
					len = 31;
				}

				memcpy(format, saveptr, len);
				format[len] = 0;
				if (!len_as_arg)
					len_arg = -1;
				/* Use helper trace_seq */
				trace_seq_init(&p);
				print_str_arg(&p, data, size, event,
					      format, len_arg, arg);
				trace_seq_terminate(&p);
				trace_seq_puts(s, p.buffer);
				trace_seq_destroy(&p);
				arg = arg->next;
				break;
			default:
				trace_seq_printf(s, ">%c<", *ptr);

			}
		} else
			trace_seq_putc(s, *ptr);
	}

	if (event->flags & EVENT_FL_FAILED) {
out_failed:
		trace_seq_printf(s, "[FAILED TO PARSE]");
	}

	if (args) {
=======
	if (event->flags & TEP_EVENT_FL_ISBPRINT) {
		bprint_fmt = get_bprint_format(data, size, event);
		args = make_bprint_args(bprint_fmt, data, size, event);
		parse = parse_args(event, bprint_fmt, args);
	}

	print_event_cache(parse, s, data, size, event);

	if (event->flags & TEP_EVENT_FL_ISBPRINT) {
		free_parse_args(parse);
>>>>>>> upstream/android-13
		free_args(args);
		free(bprint_fmt);
	}
}

<<<<<<< HEAD
/**
 * tep_data_lat_fmt - parse the data for the latency format
 * @pevent: a handle to the pevent
 * @s: the trace_seq to write to
 * @record: the record to read from
 *
=======
/*
>>>>>>> upstream/android-13
 * This parses out the Latency format (interrupts disabled,
 * need rescheduling, in hard/soft interrupt, preempt count
 * and lock depth) and places it into the trace_seq.
 */
<<<<<<< HEAD
void tep_data_lat_fmt(struct tep_handle *pevent,
		      struct trace_seq *s, struct tep_record *record)
=======
static void data_latency_format(struct tep_handle *tep, struct trace_seq *s,
				char *format, struct tep_record *record)
>>>>>>> upstream/android-13
{
	static int check_lock_depth = 1;
	static int check_migrate_disable = 1;
	static int lock_depth_exists;
	static int migrate_disable_exists;
	unsigned int lat_flags;
<<<<<<< HEAD
	unsigned int pc;
	int lock_depth;
	int migrate_disable;
=======
	struct trace_seq sq;
	unsigned int pc;
	int lock_depth = 0;
	int migrate_disable = 0;
>>>>>>> upstream/android-13
	int hardirq;
	int softirq;
	void *data = record->data;

<<<<<<< HEAD
	lat_flags = parse_common_flags(pevent, data);
	pc = parse_common_pc(pevent, data);
	/* lock_depth may not always exist */
	if (lock_depth_exists)
		lock_depth = parse_common_lock_depth(pevent, data);
	else if (check_lock_depth) {
		lock_depth = parse_common_lock_depth(pevent, data);
=======
	trace_seq_init(&sq);
	lat_flags = parse_common_flags(tep, data);
	pc = parse_common_pc(tep, data);
	/* lock_depth may not always exist */
	if (lock_depth_exists)
		lock_depth = parse_common_lock_depth(tep, data);
	else if (check_lock_depth) {
		lock_depth = parse_common_lock_depth(tep, data);
>>>>>>> upstream/android-13
		if (lock_depth < 0)
			check_lock_depth = 0;
		else
			lock_depth_exists = 1;
	}

	/* migrate_disable may not always exist */
	if (migrate_disable_exists)
<<<<<<< HEAD
		migrate_disable = parse_common_migrate_disable(pevent, data);
	else if (check_migrate_disable) {
		migrate_disable = parse_common_migrate_disable(pevent, data);
=======
		migrate_disable = parse_common_migrate_disable(tep, data);
	else if (check_migrate_disable) {
		migrate_disable = parse_common_migrate_disable(tep, data);
>>>>>>> upstream/android-13
		if (migrate_disable < 0)
			check_migrate_disable = 0;
		else
			migrate_disable_exists = 1;
	}

	hardirq = lat_flags & TRACE_FLAG_HARDIRQ;
	softirq = lat_flags & TRACE_FLAG_SOFTIRQ;

<<<<<<< HEAD
	trace_seq_printf(s, "%c%c%c",
=======
	trace_seq_printf(&sq, "%c%c%c",
>>>>>>> upstream/android-13
	       (lat_flags & TRACE_FLAG_IRQS_OFF) ? 'd' :
	       (lat_flags & TRACE_FLAG_IRQS_NOSUPPORT) ?
	       'X' : '.',
	       (lat_flags & TRACE_FLAG_NEED_RESCHED) ?
	       'N' : '.',
	       (hardirq && softirq) ? 'H' :
	       hardirq ? 'h' : softirq ? 's' : '.');

	if (pc)
<<<<<<< HEAD
		trace_seq_printf(s, "%x", pc);
	else
		trace_seq_putc(s, '.');

	if (migrate_disable_exists) {
		if (migrate_disable < 0)
			trace_seq_putc(s, '.');
		else
			trace_seq_printf(s, "%d", migrate_disable);
=======
		trace_seq_printf(&sq, "%x", pc);
	else
		trace_seq_printf(&sq, ".");

	if (migrate_disable_exists) {
		if (migrate_disable < 0)
			trace_seq_printf(&sq, ".");
		else
			trace_seq_printf(&sq, "%d", migrate_disable);
>>>>>>> upstream/android-13
	}

	if (lock_depth_exists) {
		if (lock_depth < 0)
<<<<<<< HEAD
			trace_seq_putc(s, '.');
		else
			trace_seq_printf(s, "%d", lock_depth);
	}

=======
			trace_seq_printf(&sq, ".");
		else
			trace_seq_printf(&sq, "%d", lock_depth);
	}

	if (sq.state == TRACE_SEQ__MEM_ALLOC_FAILED) {
		s->state = TRACE_SEQ__MEM_ALLOC_FAILED;
		return;
	}

	trace_seq_terminate(&sq);
	trace_seq_puts(s, sq.buffer);
	trace_seq_destroy(&sq);
>>>>>>> upstream/android-13
	trace_seq_terminate(s);
}

/**
 * tep_data_type - parse out the given event type
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @rec: the record to read from
 *
 * This returns the event id from the @rec.
 */
<<<<<<< HEAD
int tep_data_type(struct tep_handle *pevent, struct tep_record *rec)
{
	return trace_parse_common_type(pevent, rec->data);
}

/**
 * tep_data_event_from_type - find the event by a given type
 * @pevent: a handle to the pevent
 * @type: the type of the event.
 *
 * This returns the event form a given @type;
 */
struct event_format *tep_data_event_from_type(struct tep_handle *pevent, int type)
{
	return tep_find_event(pevent, type);
=======
int tep_data_type(struct tep_handle *tep, struct tep_record *rec)
{
	return trace_parse_common_type(tep, rec->data);
>>>>>>> upstream/android-13
}

/**
 * tep_data_pid - parse the PID from record
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @rec: the record to parse
 *
 * This returns the PID from a record.
 */
<<<<<<< HEAD
int tep_data_pid(struct tep_handle *pevent, struct tep_record *rec)
{
	return parse_common_pid(pevent, rec->data);
=======
int tep_data_pid(struct tep_handle *tep, struct tep_record *rec)
{
	return parse_common_pid(tep, rec->data);
>>>>>>> upstream/android-13
}

/**
 * tep_data_preempt_count - parse the preempt count from the record
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @rec: the record to parse
 *
 * This returns the preempt count from a record.
 */
<<<<<<< HEAD
int tep_data_preempt_count(struct tep_handle *pevent, struct tep_record *rec)
{
	return parse_common_pc(pevent, rec->data);
=======
int tep_data_preempt_count(struct tep_handle *tep, struct tep_record *rec)
{
	return parse_common_pc(tep, rec->data);
>>>>>>> upstream/android-13
}

/**
 * tep_data_flags - parse the latency flags from the record
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @rec: the record to parse
 *
 * This returns the latency flags from a record.
 *
 *  Use trace_flag_type enum for the flags (see event-parse.h).
 */
<<<<<<< HEAD
int tep_data_flags(struct tep_handle *pevent, struct tep_record *rec)
{
	return parse_common_flags(pevent, rec->data);
=======
int tep_data_flags(struct tep_handle *tep, struct tep_record *rec)
{
	return parse_common_flags(tep, rec->data);
>>>>>>> upstream/android-13
}

/**
 * tep_data_comm_from_pid - return the command line from PID
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @pid: the PID of the task to search for
 *
 * This returns a pointer to the command line that has the given
 * @pid.
 */
<<<<<<< HEAD
const char *tep_data_comm_from_pid(struct tep_handle *pevent, int pid)
{
	const char *comm;

	comm = find_cmdline(pevent, pid);
	return comm;
}

static struct cmdline *
pid_from_cmdlist(struct tep_handle *pevent, const char *comm, struct cmdline *next)
=======
const char *tep_data_comm_from_pid(struct tep_handle *tep, int pid)
{
	const char *comm;

	comm = find_cmdline(tep, pid);
	return comm;
}

static struct tep_cmdline *
pid_from_cmdlist(struct tep_handle *tep, const char *comm, struct tep_cmdline *next)
>>>>>>> upstream/android-13
{
	struct cmdline_list *cmdlist = (struct cmdline_list *)next;

	if (cmdlist)
		cmdlist = cmdlist->next;
	else
<<<<<<< HEAD
		cmdlist = pevent->cmdlist;
=======
		cmdlist = tep->cmdlist;
>>>>>>> upstream/android-13

	while (cmdlist && strcmp(cmdlist->comm, comm) != 0)
		cmdlist = cmdlist->next;

<<<<<<< HEAD
	return (struct cmdline *)cmdlist;
=======
	return (struct tep_cmdline *)cmdlist;
>>>>>>> upstream/android-13
}

/**
 * tep_data_pid_from_comm - return the pid from a given comm
<<<<<<< HEAD
 * @pevent: a handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @comm: the cmdline to find the pid from
 * @next: the cmdline structure to find the next comm
 *
 * This returns the cmdline structure that holds a pid for a given
 * comm, or NULL if none found. As there may be more than one pid for
 * a given comm, the result of this call can be passed back into
<<<<<<< HEAD
 * a recurring call in the @next paramater, and then it will find the
 * next pid.
 * Also, it does a linear seach, so it may be slow.
 */
struct cmdline *tep_data_pid_from_comm(struct tep_handle *pevent, const char *comm,
				       struct cmdline *next)
{
	struct cmdline *cmdline;
=======
 * a recurring call in the @next parameter, and then it will find the
 * next pid.
 * Also, it does a linear search, so it may be slow.
 */
struct tep_cmdline *tep_data_pid_from_comm(struct tep_handle *tep, const char *comm,
					   struct tep_cmdline *next)
{
	struct tep_cmdline *cmdline;
>>>>>>> upstream/android-13

	/*
	 * If the cmdlines have not been converted yet, then use
	 * the list.
	 */
<<<<<<< HEAD
	if (!pevent->cmdlines)
		return pid_from_cmdlist(pevent, comm, next);
=======
	if (!tep->cmdlines)
		return pid_from_cmdlist(tep, comm, next);
>>>>>>> upstream/android-13

	if (next) {
		/*
		 * The next pointer could have been still from
		 * a previous call before cmdlines were created
		 */
<<<<<<< HEAD
		if (next < pevent->cmdlines ||
		    next >= pevent->cmdlines + pevent->cmdline_count)
=======
		if (next < tep->cmdlines ||
		    next >= tep->cmdlines + tep->cmdline_count)
>>>>>>> upstream/android-13
			next = NULL;
		else
			cmdline  = next++;
	}

	if (!next)
<<<<<<< HEAD
		cmdline = pevent->cmdlines;

	while (cmdline < pevent->cmdlines + pevent->cmdline_count) {
=======
		cmdline = tep->cmdlines;

	while (cmdline < tep->cmdlines + tep->cmdline_count) {
>>>>>>> upstream/android-13
		if (strcmp(cmdline->comm, comm) == 0)
			return cmdline;
		cmdline++;
	}
	return NULL;
}

/**
 * tep_cmdline_pid - return the pid associated to a given cmdline
<<<<<<< HEAD
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @cmdline: The cmdline structure to get the pid from
 *
 * Returns the pid for a give cmdline. If @cmdline is NULL, then
 * -1 is returned.
 */
<<<<<<< HEAD
int tep_cmdline_pid(struct tep_handle *pevent, struct cmdline *cmdline)
=======
int tep_cmdline_pid(struct tep_handle *tep, struct tep_cmdline *cmdline)
>>>>>>> upstream/android-13
{
	struct cmdline_list *cmdlist = (struct cmdline_list *)cmdline;

	if (!cmdline)
		return -1;

	/*
	 * If cmdlines have not been created yet, or cmdline is
	 * not part of the array, then treat it as a cmdlist instead.
	 */
<<<<<<< HEAD
	if (!pevent->cmdlines ||
	    cmdline < pevent->cmdlines ||
	    cmdline >= pevent->cmdlines + pevent->cmdline_count)
=======
	if (!tep->cmdlines ||
	    cmdline < tep->cmdlines ||
	    cmdline >= tep->cmdlines + tep->cmdline_count)
>>>>>>> upstream/android-13
		return cmdlist->pid;

	return cmdline->pid;
}

<<<<<<< HEAD
/**
 * tep_event_info - parse the data into the print format
 * @s: the trace_seq to write to
 * @event: the handle to the event
 * @record: the record to read from
 *
 * This parses the raw @data using the given @event information and
 * writes the print format into the trace_seq.
 */
void tep_event_info(struct trace_seq *s, struct event_format *event,
		    struct tep_record *record)
{
	int print_pretty = 1;

	if (event->pevent->print_raw || (event->flags & EVENT_FL_PRINTRAW))
		tep_print_fields(s, record->data, record->size, event);
	else {

		if (event->handler && !(event->flags & EVENT_FL_NOHANDLE))
=======
/*
 * This parses the raw @data using the given @event information and
 * writes the print format into the trace_seq.
 */
static void print_event_info(struct trace_seq *s, char *format, bool raw,
			     struct tep_event *event, struct tep_record *record)
{
	int print_pretty = 1;

	if (raw || (event->flags & TEP_EVENT_FL_PRINTRAW))
		tep_print_fields(s, record->data, record->size, event);
	else {

		if (event->handler && !(event->flags & TEP_EVENT_FL_NOHANDLE))
>>>>>>> upstream/android-13
			print_pretty = event->handler(s, record, event,
						      event->context);

		if (print_pretty)
			pretty_print(s, record->data, record->size, event);
	}

	trace_seq_terminate(s);
}

<<<<<<< HEAD
static bool is_timestamp_in_us(char *trace_clock, bool use_trace_clock)
{
	if (!use_trace_clock)
		return true;

	if (!strcmp(trace_clock, "local") || !strcmp(trace_clock, "global")
	    || !strcmp(trace_clock, "uptime") || !strcmp(trace_clock, "perf"))
		return true;

	/* trace_clock is setting in tsc or counter mode */
	return false;
}

/**
 * tep_find_event_by_record - return the event from a given record
 * @pevent: a handle to the pevent
=======
/**
 * tep_find_event_by_record - return the event from a given record
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @record: The record to get the event from
 *
 * Returns the associated event for a given record, or NULL if non is
 * is found.
 */
<<<<<<< HEAD
struct event_format *
tep_find_event_by_record(struct tep_handle *pevent, struct tep_record *record)
=======
struct tep_event *
tep_find_event_by_record(struct tep_handle *tep, struct tep_record *record)
>>>>>>> upstream/android-13
{
	int type;

	if (record->size < 0) {
		do_warning("ug! negative record size %d", record->size);
		return NULL;
	}

<<<<<<< HEAD
	type = trace_parse_common_type(pevent, record->data);

	return tep_find_event(pevent, type);
}

/**
 * tep_print_event_task - Write the event task comm, pid and CPU
 * @pevent: a handle to the pevent
 * @s: the trace_seq to write to
 * @event: the handle to the record's event
 * @record: The record to get the event from
 *
 * Writes the tasks comm, pid and CPU to @s.
 */
void tep_print_event_task(struct tep_handle *pevent, struct trace_seq *s,
			  struct event_format *event,
			  struct tep_record *record)
{
	void *data = record->data;
	const char *comm;
	int pid;

	pid = parse_common_pid(pevent, data);
	comm = find_cmdline(pevent, pid);

	if (pevent->latency_format) {
		trace_seq_printf(s, "%8.8s-%-5d %3d",
		       comm, pid, record->cpu);
	} else
		trace_seq_printf(s, "%16s-%-5d [%03d]", comm, pid, record->cpu);
}

/**
 * tep_print_event_time - Write the event timestamp
 * @pevent: a handle to the pevent
 * @s: the trace_seq to write to
 * @event: the handle to the record's event
 * @record: The record to get the event from
 * @use_trace_clock: Set to parse according to the @pevent->trace_clock
 *
 * Writes the timestamp of the record into @s.
 */
void tep_print_event_time(struct tep_handle *pevent, struct trace_seq *s,
			  struct event_format *event,
			  struct tep_record *record,
			  bool use_trace_clock)
{
	unsigned long secs;
	unsigned long usecs;
	unsigned long nsecs;
	int p;
	bool use_usec_format;

	use_usec_format = is_timestamp_in_us(pevent->trace_clock,
							use_trace_clock);
	if (use_usec_format) {
		secs = record->ts / NSEC_PER_SEC;
		nsecs = record->ts - secs * NSEC_PER_SEC;
	}

	if (pevent->latency_format) {
		tep_data_lat_fmt(pevent, s, record);
	}

	if (use_usec_format) {
		if (pevent->flags & TEP_NSEC_OUTPUT) {
			usecs = nsecs;
			p = 9;
		} else {
			usecs = (nsecs + 500) / NSEC_PER_USEC;
			/* To avoid usecs larger than 1 sec */
			if (usecs >= USEC_PER_SEC) {
				usecs -= USEC_PER_SEC;
				secs++;
			}
			p = 6;
		}

		trace_seq_printf(s, " %5lu.%0*lu:", secs, p, usecs);
	} else
		trace_seq_printf(s, " %12llu:", record->ts);
}

/**
 * tep_print_event_data - Write the event data section
 * @pevent: a handle to the pevent
 * @s: the trace_seq to write to
 * @event: the handle to the record's event
 * @record: The record to get the event from
 *
 * Writes the parsing of the record's data to @s.
 */
void tep_print_event_data(struct tep_handle *pevent, struct trace_seq *s,
			  struct event_format *event,
			  struct tep_record *record)
{
	static const char *spaces = "                    "; /* 20 spaces */
	int len;

	trace_seq_printf(s, " %s: ", event->name);

	/* Space out the event names evenly. */
	len = strlen(event->name);
	if (len < 20)
		trace_seq_printf(s, "%.*s", 20 - len, spaces);

	tep_event_info(s, event, record);
}

void tep_print_event(struct tep_handle *pevent, struct trace_seq *s,
		     struct tep_record *record, bool use_trace_clock)
{
	struct event_format *event;

	event = tep_find_event_by_record(pevent, record);
	if (!event) {
		int i;
		int type = trace_parse_common_type(pevent, record->data);

		do_warning("ug! no event found for type %d", type);
		trace_seq_printf(s, "[UNKNOWN TYPE %d]", type);
		for (i = 0; i < record->size; i++)
			trace_seq_printf(s, " %02x",
					 ((unsigned char *)record->data)[i]);
		return;
	}

	tep_print_event_task(pevent, s, event, record);
	tep_print_event_time(pevent, s, event, record, use_trace_clock);
	tep_print_event_data(pevent, s, event, record);
=======
	type = trace_parse_common_type(tep, record->data);

	return tep_find_event(tep, type);
}

/*
 * Writes the timestamp of the record into @s. Time divisor and precision can be
 * specified as part of printf @format string. Example:
 *	"%3.1000d" - divide the time by 1000 and print the first 3 digits
 *	before the dot. Thus, the timestamp "123456000" will be printed as
 *	"123.456"
 */
static void print_event_time(struct tep_handle *tep, struct trace_seq *s,
				 char *format, struct tep_event *event,
				 struct tep_record *record)
{
	unsigned long long time;
	char *divstr;
	int prec = 0, pr;
	int div = 0;
	int p10 = 1;

	if (isdigit(*(format + 1)))
		prec = atoi(format + 1);
	divstr = strchr(format, '.');
	if (divstr && isdigit(*(divstr + 1)))
		div = atoi(divstr + 1);
	time = record->ts;
	if (div) {
		time += div / 2;
		time /= div;
	}
	pr = prec;
	while (pr--)
		p10 *= 10;

	if (p10 > 1 && p10 < time)
		trace_seq_printf(s, "%5llu.%0*llu", time / p10, prec, time % p10);
	else
		trace_seq_printf(s, "%12llu", time);
}

struct print_event_type {
	enum {
		EVENT_TYPE_INT = 1,
		EVENT_TYPE_STRING,
		EVENT_TYPE_UNKNOWN,
	} type;
	char format[32];
};

static void print_string(struct tep_handle *tep, struct trace_seq *s,
			 struct tep_record *record, struct tep_event *event,
			 const char *arg, struct print_event_type *type)
{
	const char *comm;
	int pid;

	if (strncmp(arg, TEP_PRINT_LATENCY, strlen(TEP_PRINT_LATENCY)) == 0) {
		data_latency_format(tep, s, type->format, record);
	} else if (strncmp(arg, TEP_PRINT_COMM, strlen(TEP_PRINT_COMM)) == 0) {
		pid = parse_common_pid(tep, record->data);
		comm = find_cmdline(tep, pid);
		trace_seq_printf(s, type->format, comm);
	} else if (strncmp(arg, TEP_PRINT_INFO_RAW, strlen(TEP_PRINT_INFO_RAW)) == 0) {
		print_event_info(s, type->format, true, event, record);
	} else if (strncmp(arg, TEP_PRINT_INFO, strlen(TEP_PRINT_INFO)) == 0) {
		print_event_info(s, type->format, false, event, record);
	} else if  (strncmp(arg, TEP_PRINT_NAME, strlen(TEP_PRINT_NAME)) == 0) {
		trace_seq_printf(s, type->format, event->name);
	} else {
		trace_seq_printf(s, "[UNKNOWN TEP TYPE %s]", arg);
	}

}

static void print_int(struct tep_handle *tep, struct trace_seq *s,
		      struct tep_record *record, struct tep_event *event,
		      int arg, struct print_event_type *type)
{
	int param;

	switch (arg) {
	case TEP_PRINT_CPU:
		param = record->cpu;
		break;
	case TEP_PRINT_PID:
		param = parse_common_pid(tep, record->data);
		break;
	case TEP_PRINT_TIME:
		return print_event_time(tep, s, type->format, event, record);
	default:
		return;
	}
	trace_seq_printf(s, type->format, param);
}

static int tep_print_event_param_type(char *format,
				      struct print_event_type *type)
{
	char *str = format + 1;
	int i = 1;

	type->type = EVENT_TYPE_UNKNOWN;
	while (*str) {
		switch (*str) {
		case 'd':
		case 'u':
		case 'i':
		case 'x':
		case 'X':
		case 'o':
			type->type = EVENT_TYPE_INT;
			break;
		case 's':
			type->type = EVENT_TYPE_STRING;
			break;
		}
		str++;
		i++;
		if (type->type != EVENT_TYPE_UNKNOWN)
			break;
	}
	memset(type->format, 0, 32);
	memcpy(type->format, format, i < 32 ? i : 31);
	return i;
}

/**
 * tep_print_event - Write various event information
 * @tep: a handle to the trace event parser context
 * @s: the trace_seq to write to
 * @record: The record to get the event from
 * @format: a printf format string. Supported event fileds:
 *	TEP_PRINT_PID, "%d" - event PID
 *	TEP_PRINT_CPU, "%d" - event CPU
 *	TEP_PRINT_COMM, "%s" - event command string
 *	TEP_PRINT_NAME, "%s" - event name
 *	TEP_PRINT_LATENCY, "%s" - event latency
 *	TEP_PRINT_TIME, %d - event time stamp. A divisor and precision
 *			can be specified as part of this format string:
 *			"%precision.divisord". Example:
 *			"%3.1000d" - divide the time by 1000 and print the first
 *			3 digits before the dot. Thus, the time stamp
 *			"123456000" will be printed as "123.456"
 *	TEP_PRINT_INFO, "%s" - event information. If any width is specified in
 *			the format string, the event information will be printed
 *			in raw format.
 * Writes the specified event information into @s.
 */
void tep_print_event(struct tep_handle *tep, struct trace_seq *s,
		     struct tep_record *record, const char *fmt, ...)
{
	struct print_event_type type;
	char *format = strdup(fmt);
	char *current = format;
	char *str = format;
	int offset;
	va_list args;
	struct tep_event *event;

	if (!format)
		return;

	event = tep_find_event_by_record(tep, record);
	va_start(args, fmt);
	while (*current) {
		current = strchr(str, '%');
		if (!current) {
			trace_seq_puts(s, str);
			break;
		}
		memset(&type, 0, sizeof(type));
		offset = tep_print_event_param_type(current, &type);
		*current = '\0';
		trace_seq_puts(s, str);
		current += offset;
		switch (type.type) {
		case EVENT_TYPE_STRING:
			print_string(tep, s, record, event,
				     va_arg(args, char*), &type);
			break;
		case EVENT_TYPE_INT:
			print_int(tep, s, record, event,
				  va_arg(args, int), &type);
			break;
		case EVENT_TYPE_UNKNOWN:
		default:
			trace_seq_printf(s, "[UNKNOWN TYPE]");
			break;
		}
		str = current;

	}
	va_end(args);
	free(format);
>>>>>>> upstream/android-13
}

static int events_id_cmp(const void *a, const void *b)
{
<<<<<<< HEAD
	struct event_format * const * ea = a;
	struct event_format * const * eb = b;
=======
	struct tep_event * const * ea = a;
	struct tep_event * const * eb = b;
>>>>>>> upstream/android-13

	if ((*ea)->id < (*eb)->id)
		return -1;

	if ((*ea)->id > (*eb)->id)
		return 1;

	return 0;
}

static int events_name_cmp(const void *a, const void *b)
{
<<<<<<< HEAD
	struct event_format * const * ea = a;
	struct event_format * const * eb = b;
=======
	struct tep_event * const * ea = a;
	struct tep_event * const * eb = b;
>>>>>>> upstream/android-13
	int res;

	res = strcmp((*ea)->name, (*eb)->name);
	if (res)
		return res;

	res = strcmp((*ea)->system, (*eb)->system);
	if (res)
		return res;

	return events_id_cmp(a, b);
}

static int events_system_cmp(const void *a, const void *b)
{
<<<<<<< HEAD
	struct event_format * const * ea = a;
	struct event_format * const * eb = b;
=======
	struct tep_event * const * ea = a;
	struct tep_event * const * eb = b;
>>>>>>> upstream/android-13
	int res;

	res = strcmp((*ea)->system, (*eb)->system);
	if (res)
		return res;

	res = strcmp((*ea)->name, (*eb)->name);
	if (res)
		return res;

	return events_id_cmp(a, b);
}

<<<<<<< HEAD
struct event_format **tep_list_events(struct tep_handle *pevent, enum event_sort_type sort_type)
{
	struct event_format **events;
	int (*sort)(const void *a, const void *b);

	events = pevent->sort_events;

	if (events && pevent->last_type == sort_type)
		return events;

	if (!events) {
		events = malloc(sizeof(*events) * (pevent->nr_events + 1));
		if (!events)
			return NULL;

		memcpy(events, pevent->events, sizeof(*events) * pevent->nr_events);
		events[pevent->nr_events] = NULL;

		pevent->sort_events = events;

		/* the internal events are sorted by id */
		if (sort_type == EVENT_SORT_ID) {
			pevent->last_type = sort_type;
=======
static struct tep_event **list_events_copy(struct tep_handle *tep)
{
	struct tep_event **events;

	if (!tep)
		return NULL;

	events = malloc(sizeof(*events) * (tep->nr_events + 1));
	if (!events)
		return NULL;

	memcpy(events, tep->events, sizeof(*events) * tep->nr_events);
	events[tep->nr_events] = NULL;
	return events;
}

static void list_events_sort(struct tep_event **events, int nr_events,
			     enum tep_event_sort_type sort_type)
{
	int (*sort)(const void *a, const void *b);

	switch (sort_type) {
	case TEP_EVENT_SORT_ID:
		sort = events_id_cmp;
		break;
	case TEP_EVENT_SORT_NAME:
		sort = events_name_cmp;
		break;
	case TEP_EVENT_SORT_SYSTEM:
		sort = events_system_cmp;
		break;
	default:
		sort = NULL;
	}

	if (sort)
		qsort(events, nr_events, sizeof(*events), sort);
}

/**
 * tep_list_events - Get events, sorted by given criteria.
 * @tep: a handle to the tep context
 * @sort_type: desired sort order of the events in the array
 *
 * Returns an array of pointers to all events, sorted by the given
 * @sort_type criteria. The last element of the array is NULL. The returned
 * memory must not be freed, it is managed by the library.
 * The function is not thread safe.
 */
struct tep_event **tep_list_events(struct tep_handle *tep,
				   enum tep_event_sort_type sort_type)
{
	struct tep_event **events;

	if (!tep)
		return NULL;

	events = tep->sort_events;
	if (events && tep->last_type == sort_type)
		return events;

	if (!events) {
		events = list_events_copy(tep);
		if (!events)
			return NULL;

		tep->sort_events = events;

		/* the internal events are sorted by id */
		if (sort_type == TEP_EVENT_SORT_ID) {
			tep->last_type = sort_type;
>>>>>>> upstream/android-13
			return events;
		}
	}

<<<<<<< HEAD
	switch (sort_type) {
	case EVENT_SORT_ID:
		sort = events_id_cmp;
		break;
	case EVENT_SORT_NAME:
		sort = events_name_cmp;
		break;
	case EVENT_SORT_SYSTEM:
		sort = events_system_cmp;
		break;
	default:
		return events;
	}

	qsort(events, pevent->nr_events, sizeof(*events), sort);
	pevent->last_type = sort_type;
=======
	list_events_sort(events, tep->nr_events, sort_type);
	tep->last_type = sort_type;
>>>>>>> upstream/android-13

	return events;
}

<<<<<<< HEAD
static struct format_field **
get_event_fields(const char *type, const char *name,
		 int count, struct format_field *list)
{
	struct format_field **fields;
	struct format_field *field;
=======

/**
 * tep_list_events_copy - Thread safe version of tep_list_events()
 * @tep: a handle to the tep context
 * @sort_type: desired sort order of the events in the array
 *
 * Returns an array of pointers to all events, sorted by the given
 * @sort_type criteria. The last element of the array is NULL. The returned
 * array is newly allocated inside the function and must be freed by the caller
 */
struct tep_event **tep_list_events_copy(struct tep_handle *tep,
					enum tep_event_sort_type sort_type)
{
	struct tep_event **events;

	if (!tep)
		return NULL;

	events = list_events_copy(tep);
	if (!events)
		return NULL;

	/* the internal events are sorted by id */
	if (sort_type == TEP_EVENT_SORT_ID)
		return events;

	list_events_sort(events, tep->nr_events, sort_type);

	return events;
}

static struct tep_format_field **
get_event_fields(const char *type, const char *name,
		 int count, struct tep_format_field *list)
{
	struct tep_format_field **fields;
	struct tep_format_field *field;
>>>>>>> upstream/android-13
	int i = 0;

	fields = malloc(sizeof(*fields) * (count + 1));
	if (!fields)
		return NULL;

	for (field = list; field; field = field->next) {
		fields[i++] = field;
		if (i == count + 1) {
			do_warning("event %s has more %s fields than specified",
				name, type);
			i--;
			break;
		}
	}

	if (i != count)
		do_warning("event %s has less %s fields than specified",
			name, type);

	fields[i] = NULL;

	return fields;
}

/**
 * tep_event_common_fields - return a list of common fields for an event
 * @event: the event to return the common fields of.
 *
 * Returns an allocated array of fields. The last item in the array is NULL.
 * The array must be freed with free().
 */
<<<<<<< HEAD
struct format_field **tep_event_common_fields(struct event_format *event)
=======
struct tep_format_field **tep_event_common_fields(struct tep_event *event)
>>>>>>> upstream/android-13
{
	return get_event_fields("common", event->name,
				event->format.nr_common,
				event->format.common_fields);
}

/**
 * tep_event_fields - return a list of event specific fields for an event
 * @event: the event to return the fields of.
 *
 * Returns an allocated array of fields. The last item in the array is NULL.
 * The array must be freed with free().
 */
<<<<<<< HEAD
struct format_field **tep_event_fields(struct event_format *event)
=======
struct tep_format_field **tep_event_fields(struct tep_event *event)
>>>>>>> upstream/android-13
{
	return get_event_fields("event", event->name,
				event->format.nr_fields,
				event->format.fields);
}

<<<<<<< HEAD
static void print_fields(struct trace_seq *s, struct print_flag_sym *field)
=======
static void print_fields(struct trace_seq *s, struct tep_print_flag_sym *field)
>>>>>>> upstream/android-13
{
	trace_seq_printf(s, "{ %s, %s }", field->value, field->str);
	if (field->next) {
		trace_seq_puts(s, ", ");
		print_fields(s, field->next);
	}
}

/* for debugging */
<<<<<<< HEAD
static void print_args(struct print_arg *args)
=======
static void print_args(struct tep_print_arg *args)
>>>>>>> upstream/android-13
{
	int print_paren = 1;
	struct trace_seq s;

	switch (args->type) {
<<<<<<< HEAD
	case PRINT_NULL:
		printf("null");
		break;
	case PRINT_ATOM:
		printf("%s", args->atom.atom);
		break;
	case PRINT_FIELD:
		printf("REC->%s", args->field.name);
		break;
	case PRINT_FLAGS:
=======
	case TEP_PRINT_NULL:
		printf("null");
		break;
	case TEP_PRINT_ATOM:
		printf("%s", args->atom.atom);
		break;
	case TEP_PRINT_FIELD:
		printf("REC->%s", args->field.name);
		break;
	case TEP_PRINT_FLAGS:
>>>>>>> upstream/android-13
		printf("__print_flags(");
		print_args(args->flags.field);
		printf(", %s, ", args->flags.delim);
		trace_seq_init(&s);
		print_fields(&s, args->flags.flags);
		trace_seq_do_printf(&s);
		trace_seq_destroy(&s);
		printf(")");
		break;
<<<<<<< HEAD
	case PRINT_SYMBOL:
=======
	case TEP_PRINT_SYMBOL:
>>>>>>> upstream/android-13
		printf("__print_symbolic(");
		print_args(args->symbol.field);
		printf(", ");
		trace_seq_init(&s);
		print_fields(&s, args->symbol.symbols);
		trace_seq_do_printf(&s);
		trace_seq_destroy(&s);
		printf(")");
		break;
<<<<<<< HEAD
	case PRINT_HEX:
=======
	case TEP_PRINT_HEX:
>>>>>>> upstream/android-13
		printf("__print_hex(");
		print_args(args->hex.field);
		printf(", ");
		print_args(args->hex.size);
		printf(")");
		break;
<<<<<<< HEAD
	case PRINT_HEX_STR:
=======
	case TEP_PRINT_HEX_STR:
>>>>>>> upstream/android-13
		printf("__print_hex_str(");
		print_args(args->hex.field);
		printf(", ");
		print_args(args->hex.size);
		printf(")");
		break;
<<<<<<< HEAD
	case PRINT_INT_ARRAY:
=======
	case TEP_PRINT_INT_ARRAY:
>>>>>>> upstream/android-13
		printf("__print_array(");
		print_args(args->int_array.field);
		printf(", ");
		print_args(args->int_array.count);
		printf(", ");
		print_args(args->int_array.el_size);
		printf(")");
		break;
<<<<<<< HEAD
	case PRINT_STRING:
	case PRINT_BSTRING:
		printf("__get_str(%s)", args->string.string);
		break;
	case PRINT_BITMASK:
		printf("__get_bitmask(%s)", args->bitmask.bitmask);
		break;
	case PRINT_TYPE:
		printf("(%s)", args->typecast.type);
		print_args(args->typecast.item);
		break;
	case PRINT_OP:
=======
	case TEP_PRINT_STRING:
	case TEP_PRINT_BSTRING:
		printf("__get_str(%s)", args->string.string);
		break;
	case TEP_PRINT_BITMASK:
		printf("__get_bitmask(%s)", args->bitmask.bitmask);
		break;
	case TEP_PRINT_TYPE:
		printf("(%s)", args->typecast.type);
		print_args(args->typecast.item);
		break;
	case TEP_PRINT_OP:
>>>>>>> upstream/android-13
		if (strcmp(args->op.op, ":") == 0)
			print_paren = 0;
		if (print_paren)
			printf("(");
		print_args(args->op.left);
		printf(" %s ", args->op.op);
		print_args(args->op.right);
		if (print_paren)
			printf(")");
		break;
	default:
		/* we should warn... */
		return;
	}
	if (args->next) {
		printf("\n");
		print_args(args->next);
	}
}

static void parse_header_field(const char *field,
			       int *offset, int *size, int mandatory)
{
	unsigned long long save_input_buf_ptr;
	unsigned long long save_input_buf_siz;
	char *token;
	int type;

	save_input_buf_ptr = input_buf_ptr;
	save_input_buf_siz = input_buf_siz;

<<<<<<< HEAD
	if (read_expected(EVENT_ITEM, "field") < 0)
		return;
	if (read_expected(EVENT_OP, ":") < 0)
		return;

	/* type */
	if (read_expect_type(EVENT_ITEM, &token) < 0)
=======
	if (read_expected(TEP_EVENT_ITEM, "field") < 0)
		return;
	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return;

	/* type */
	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
>>>>>>> upstream/android-13
		goto fail;
	free_token(token);

	/*
	 * If this is not a mandatory field, then test it first.
	 */
	if (mandatory) {
<<<<<<< HEAD
		if (read_expected(EVENT_ITEM, field) < 0)
			return;
	} else {
		if (read_expect_type(EVENT_ITEM, &token) < 0)
=======
		if (read_expected(TEP_EVENT_ITEM, field) < 0)
			return;
	} else {
		if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
>>>>>>> upstream/android-13
			goto fail;
		if (strcmp(token, field) != 0)
			goto discard;
		free_token(token);
	}

<<<<<<< HEAD
	if (read_expected(EVENT_OP, ";") < 0)
		return;
	if (read_expected(EVENT_ITEM, "offset") < 0)
		return;
	if (read_expected(EVENT_OP, ":") < 0)
		return;
	if (read_expect_type(EVENT_ITEM, &token) < 0)
		goto fail;
	*offset = atoi(token);
	free_token(token);
	if (read_expected(EVENT_OP, ";") < 0)
		return;
	if (read_expected(EVENT_ITEM, "size") < 0)
		return;
	if (read_expected(EVENT_OP, ":") < 0)
		return;
	if (read_expect_type(EVENT_ITEM, &token) < 0)
		goto fail;
	*size = atoi(token);
	free_token(token);
	if (read_expected(EVENT_OP, ";") < 0)
		return;
	type = read_token(&token);
	if (type != EVENT_NEWLINE) {
		/* newer versions of the kernel have a "signed" type */
		if (type != EVENT_ITEM)
=======
	if (read_expected(TEP_EVENT_OP, ";") < 0)
		return;
	if (read_expected(TEP_EVENT_ITEM, "offset") < 0)
		return;
	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return;
	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto fail;
	*offset = atoi(token);
	free_token(token);
	if (read_expected(TEP_EVENT_OP, ";") < 0)
		return;
	if (read_expected(TEP_EVENT_ITEM, "size") < 0)
		return;
	if (read_expected(TEP_EVENT_OP, ":") < 0)
		return;
	if (read_expect_type(TEP_EVENT_ITEM, &token) < 0)
		goto fail;
	*size = atoi(token);
	free_token(token);
	if (read_expected(TEP_EVENT_OP, ";") < 0)
		return;
	type = read_token(&token);
	if (type != TEP_EVENT_NEWLINE) {
		/* newer versions of the kernel have a "signed" type */
		if (type != TEP_EVENT_ITEM)
>>>>>>> upstream/android-13
			goto fail;

		if (strcmp(token, "signed") != 0)
			goto fail;

		free_token(token);

<<<<<<< HEAD
		if (read_expected(EVENT_OP, ":") < 0)
			return;

		if (read_expect_type(EVENT_ITEM, &token))
			goto fail;

		free_token(token);
		if (read_expected(EVENT_OP, ";") < 0)
			return;

		if (read_expect_type(EVENT_NEWLINE, &token))
=======
		if (read_expected(TEP_EVENT_OP, ":") < 0)
			return;

		if (read_expect_type(TEP_EVENT_ITEM, &token))
			goto fail;

		free_token(token);
		if (read_expected(TEP_EVENT_OP, ";") < 0)
			return;

		if (read_expect_type(TEP_EVENT_NEWLINE, &token))
>>>>>>> upstream/android-13
			goto fail;
	}
 fail:
	free_token(token);
	return;

 discard:
	input_buf_ptr = save_input_buf_ptr;
	input_buf_siz = save_input_buf_siz;
	*offset = 0;
	*size = 0;
	free_token(token);
}

/**
 * tep_parse_header_page - parse the data stored in the header page
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @buf: the buffer storing the header page format string
 * @size: the size of @buf
 * @long_size: the long size to use if there is no header
 *
 * This parses the header page format for information on the
 * ring buffer used. The @buf should be copied from
 *
 * /sys/kernel/debug/tracing/events/header_page
 */
<<<<<<< HEAD
int tep_parse_header_page(struct tep_handle *pevent, char *buf, unsigned long size,
=======
int tep_parse_header_page(struct tep_handle *tep, char *buf, unsigned long size,
>>>>>>> upstream/android-13
			  int long_size)
{
	int ignore;

	if (!size) {
		/*
		 * Old kernels did not have header page info.
		 * Sorry but we just use what we find here in user space.
		 */
<<<<<<< HEAD
		pevent->header_page_ts_size = sizeof(long long);
		pevent->header_page_size_size = long_size;
		pevent->header_page_data_offset = sizeof(long long) + long_size;
		pevent->old_format = 1;
=======
		tep->header_page_ts_size = sizeof(long long);
		tep->header_page_size_size = long_size;
		tep->header_page_data_offset = sizeof(long long) + long_size;
		tep->old_format = 1;
>>>>>>> upstream/android-13
		return -1;
	}
	init_input_buf(buf, size);

<<<<<<< HEAD
	parse_header_field("timestamp", &pevent->header_page_ts_offset,
			   &pevent->header_page_ts_size, 1);
	parse_header_field("commit", &pevent->header_page_size_offset,
			   &pevent->header_page_size_size, 1);
	parse_header_field("overwrite", &pevent->header_page_overwrite,
			   &ignore, 0);
	parse_header_field("data", &pevent->header_page_data_offset,
			   &pevent->header_page_data_size, 1);
=======
	parse_header_field("timestamp", &tep->header_page_ts_offset,
			   &tep->header_page_ts_size, 1);
	parse_header_field("commit", &tep->header_page_size_offset,
			   &tep->header_page_size_size, 1);
	parse_header_field("overwrite", &tep->header_page_overwrite,
			   &ignore, 0);
	parse_header_field("data", &tep->header_page_data_offset,
			   &tep->header_page_data_size, 1);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
static int event_matches(struct event_format *event,
=======
static int event_matches(struct tep_event *event,
>>>>>>> upstream/android-13
			 int id, const char *sys_name,
			 const char *event_name)
{
	if (id >= 0 && id != event->id)
		return 0;

	if (event_name && (strcmp(event_name, event->name) != 0))
		return 0;

	if (sys_name && (strcmp(sys_name, event->system) != 0))
		return 0;

	return 1;
}

static void free_handler(struct event_handler *handle)
{
	free((void *)handle->sys_name);
	free((void *)handle->event_name);
	free(handle);
}

<<<<<<< HEAD
static int find_event_handle(struct tep_handle *pevent, struct event_format *event)
{
	struct event_handler *handle, **next;

	for (next = &pevent->handlers; *next;
=======
static int find_event_handle(struct tep_handle *tep, struct tep_event *event)
{
	struct event_handler *handle, **next;

	for (next = &tep->handlers; *next;
>>>>>>> upstream/android-13
	     next = &(*next)->next) {
		handle = *next;
		if (event_matches(event, handle->id,
				  handle->sys_name,
				  handle->event_name))
			break;
	}

	if (!(*next))
		return 0;

	pr_stat("overriding event (%d) %s:%s with new print handler",
		event->id, event->system, event->name);

	event->handler = handle->func;
	event->context = handle->context;

	*next = handle->next;
	free_handler(handle);

	return 1;
}

/**
<<<<<<< HEAD
 * __tep_parse_format - parse the event format
=======
 * parse_format - parse the event format
>>>>>>> upstream/android-13
 * @buf: the buffer storing the event format string
 * @size: the size of @buf
 * @sys: the system the event belongs to
 *
 * This parses the event format and creates an event structure
 * to quickly parse raw data for a given event.
 *
 * These files currently come from:
 *
 * /sys/kernel/debug/tracing/events/.../.../format
 */
<<<<<<< HEAD
enum tep_errno __tep_parse_format(struct event_format **eventp,
				  struct tep_handle *pevent, const char *buf,
				  unsigned long size, const char *sys)
{
	struct event_format *event;
=======
static enum tep_errno parse_format(struct tep_event **eventp,
				   struct tep_handle *tep, const char *buf,
				   unsigned long size, const char *sys)
{
	struct tep_event *event;
>>>>>>> upstream/android-13
	int ret;

	init_input_buf(buf, size);

	*eventp = event = alloc_event();
	if (!event)
		return TEP_ERRNO__MEM_ALLOC_FAILED;

	event->name = event_read_name();
	if (!event->name) {
		/* Bad event? */
		ret = TEP_ERRNO__MEM_ALLOC_FAILED;
		goto event_alloc_failed;
	}

	if (strcmp(sys, "ftrace") == 0) {
<<<<<<< HEAD
		event->flags |= EVENT_FL_ISFTRACE;

		if (strcmp(event->name, "bprint") == 0)
			event->flags |= EVENT_FL_ISBPRINT;
=======
		event->flags |= TEP_EVENT_FL_ISFTRACE;

		if (strcmp(event->name, "bprint") == 0)
			event->flags |= TEP_EVENT_FL_ISBPRINT;
>>>>>>> upstream/android-13
	}
		
	event->id = event_read_id();
	if (event->id < 0) {
		ret = TEP_ERRNO__READ_ID_FAILED;
		/*
		 * This isn't an allocation error actually.
		 * But as the ID is critical, just bail out.
		 */
		goto event_alloc_failed;
	}

	event->system = strdup(sys);
	if (!event->system) {
		ret = TEP_ERRNO__MEM_ALLOC_FAILED;
		goto event_alloc_failed;
	}

<<<<<<< HEAD
	/* Add pevent to event so that it can be referenced */
	event->pevent = pevent;
=======
	/* Add tep to event so that it can be referenced */
	event->tep = tep;
>>>>>>> upstream/android-13

	ret = event_read_format(event);
	if (ret < 0) {
		ret = TEP_ERRNO__READ_FORMAT_FAILED;
		goto event_parse_failed;
	}

	/*
	 * If the event has an override, don't print warnings if the event
	 * print format fails to parse.
	 */
<<<<<<< HEAD
	if (pevent && find_event_handle(pevent, event))
=======
	if (tep && find_event_handle(tep, event))
>>>>>>> upstream/android-13
		show_warning = 0;

	ret = event_read_print(event);
	show_warning = 1;

	if (ret < 0) {
		ret = TEP_ERRNO__READ_PRINT_FAILED;
		goto event_parse_failed;
	}

<<<<<<< HEAD
	if (!ret && (event->flags & EVENT_FL_ISFTRACE)) {
		struct format_field *field;
		struct print_arg *arg, **list;
=======
	if (!ret && (event->flags & TEP_EVENT_FL_ISFTRACE)) {
		struct tep_format_field *field;
		struct tep_print_arg *arg, **list;
>>>>>>> upstream/android-13

		/* old ftrace had no args */
		list = &event->print_fmt.args;
		for (field = event->format.fields; field; field = field->next) {
			arg = alloc_arg();
			if (!arg) {
<<<<<<< HEAD
				event->flags |= EVENT_FL_FAILED;
				return TEP_ERRNO__OLD_FTRACE_ARG_FAILED;
			}
			arg->type = PRINT_FIELD;
			arg->field.name = strdup(field->name);
			if (!arg->field.name) {
				event->flags |= EVENT_FL_FAILED;
=======
				event->flags |= TEP_EVENT_FL_FAILED;
				return TEP_ERRNO__OLD_FTRACE_ARG_FAILED;
			}
			arg->type = TEP_PRINT_FIELD;
			arg->field.name = strdup(field->name);
			if (!arg->field.name) {
				event->flags |= TEP_EVENT_FL_FAILED;
>>>>>>> upstream/android-13
				free_arg(arg);
				return TEP_ERRNO__OLD_FTRACE_ARG_FAILED;
			}
			arg->field.field = field;
			*list = arg;
			list = &arg->next;
		}
<<<<<<< HEAD
		return 0;
	}

	return 0;

 event_parse_failed:
	event->flags |= EVENT_FL_FAILED;
=======
	}

	if (!(event->flags & TEP_EVENT_FL_ISBPRINT))
		event->print_fmt.print_cache = parse_args(event,
							  event->print_fmt.format,
							  event->print_fmt.args);

	return 0;

 event_parse_failed:
	event->flags |= TEP_EVENT_FL_FAILED;
>>>>>>> upstream/android-13
	return ret;

 event_alloc_failed:
	free(event->system);
	free(event->name);
	free(event);
	*eventp = NULL;
	return ret;
}

static enum tep_errno
<<<<<<< HEAD
__parse_event(struct tep_handle *pevent,
	      struct event_format **eventp,
	      const char *buf, unsigned long size,
	      const char *sys)
{
	int ret = __tep_parse_format(eventp, pevent, buf, size, sys);
	struct event_format *event = *eventp;
=======
__parse_event(struct tep_handle *tep,
	      struct tep_event **eventp,
	      const char *buf, unsigned long size,
	      const char *sys)
{
	int ret = parse_format(eventp, tep, buf, size, sys);
	struct tep_event *event = *eventp;
>>>>>>> upstream/android-13

	if (event == NULL)
		return ret;

<<<<<<< HEAD
	if (pevent && add_event(pevent, event)) {
=======
	if (tep && add_event(tep, event)) {
>>>>>>> upstream/android-13
		ret = TEP_ERRNO__MEM_ALLOC_FAILED;
		goto event_add_failed;
	}

#define PRINT_ARGS 0
	if (PRINT_ARGS && event->print_fmt.args)
		print_args(event->print_fmt.args);

	return 0;

event_add_failed:
<<<<<<< HEAD
	tep_free_format(event);
=======
	free_tep_event(event);
>>>>>>> upstream/android-13
	return ret;
}

/**
 * tep_parse_format - parse the event format
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @eventp: returned format
 * @buf: the buffer storing the event format string
 * @size: the size of @buf
 * @sys: the system the event belongs to
 *
 * This parses the event format and creates an event structure
 * to quickly parse raw data for a given event.
 *
 * These files currently come from:
 *
 * /sys/kernel/debug/tracing/events/.../.../format
 */
<<<<<<< HEAD
enum tep_errno tep_parse_format(struct tep_handle *pevent,
				struct event_format **eventp,
				const char *buf,
				unsigned long size, const char *sys)
{
	return __parse_event(pevent, eventp, buf, size, sys);
=======
enum tep_errno tep_parse_format(struct tep_handle *tep,
				struct tep_event **eventp,
				const char *buf,
				unsigned long size, const char *sys)
{
	return __parse_event(tep, eventp, buf, size, sys);
>>>>>>> upstream/android-13
}

/**
 * tep_parse_event - parse the event format
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @buf: the buffer storing the event format string
 * @size: the size of @buf
 * @sys: the system the event belongs to
 *
 * This parses the event format and creates an event structure
 * to quickly parse raw data for a given event.
 *
 * These files currently come from:
 *
 * /sys/kernel/debug/tracing/events/.../.../format
 */
<<<<<<< HEAD
enum tep_errno tep_parse_event(struct tep_handle *pevent, const char *buf,
			       unsigned long size, const char *sys)
{
	struct event_format *event = NULL;
	return __parse_event(pevent, &event, buf, size, sys);
}

#undef _PE
#define _PE(code, str) str
static const char * const tep_error_str[] = {
	TEP_ERRORS
};
#undef _PE

int tep_strerror(struct tep_handle *pevent __maybe_unused,
		 enum tep_errno errnum, char *buf, size_t buflen)
{
	int idx;
	const char *msg;

	if (errnum >= 0) {
		str_error_r(errnum, buf, buflen);
		return 0;
	}

	if (errnum <= __TEP_ERRNO__START ||
	    errnum >= __TEP_ERRNO__END)
		return -1;

	idx = errnum - __TEP_ERRNO__START - 1;
	msg = tep_error_str[idx];
	snprintf(buf, buflen, "%s", msg);

	return 0;
}

int get_field_val(struct trace_seq *s, struct format_field *field,
=======
enum tep_errno tep_parse_event(struct tep_handle *tep, const char *buf,
			       unsigned long size, const char *sys)
{
	struct tep_event *event = NULL;
	return __parse_event(tep, &event, buf, size, sys);
}

int get_field_val(struct trace_seq *s, struct tep_format_field *field,
>>>>>>> upstream/android-13
		  const char *name, struct tep_record *record,
		  unsigned long long *val, int err)
{
	if (!field) {
		if (err)
			trace_seq_printf(s, "<CANT FIND FIELD %s>", name);
		return -1;
	}

	if (tep_read_number_field(field, record->data, val)) {
		if (err)
			trace_seq_printf(s, " %s=INVALID", name);
		return -1;
	}

	return 0;
}

/**
 * tep_get_field_raw - return the raw pointer into the data field
 * @s: The seq to print to on error
 * @event: the event that the field is for
 * @name: The name of the field
 * @record: The record with the field name.
 * @len: place to store the field length.
 * @err: print default error if failed.
 *
 * Returns a pointer into record->data of the field and places
 * the length of the field in @len.
 *
 * On failure, it returns NULL.
 */
<<<<<<< HEAD
void *tep_get_field_raw(struct trace_seq *s, struct event_format *event,
			const char *name, struct tep_record *record,
			int *len, int err)
{
	struct format_field *field;
=======
void *tep_get_field_raw(struct trace_seq *s, struct tep_event *event,
			const char *name, struct tep_record *record,
			int *len, int err)
{
	struct tep_format_field *field;
>>>>>>> upstream/android-13
	void *data = record->data;
	unsigned offset;
	int dummy;

	if (!event)
		return NULL;

	field = tep_find_field(event, name);

	if (!field) {
		if (err)
			trace_seq_printf(s, "<CANT FIND FIELD %s>", name);
		return NULL;
	}

	/* Allow @len to be NULL */
	if (!len)
		len = &dummy;

	offset = field->offset;
<<<<<<< HEAD
	if (field->flags & FIELD_IS_DYNAMIC) {
		offset = tep_read_number(event->pevent,
					    data + offset, field->size);
=======
	if (field->flags & TEP_FIELD_IS_DYNAMIC) {
		offset = tep_read_number(event->tep,
					 data + offset, field->size);
>>>>>>> upstream/android-13
		*len = offset >> 16;
		offset &= 0xffff;
	} else
		*len = field->size;

	return data + offset;
}

/**
 * tep_get_field_val - find a field and return its value
 * @s: The seq to print to on error
 * @event: the event that the field is for
 * @name: The name of the field
 * @record: The record with the field name.
 * @val: place to store the value of the field.
 * @err: print default error if failed.
 *
 * Returns 0 on success -1 on field not found.
 */
<<<<<<< HEAD
int tep_get_field_val(struct trace_seq *s, struct event_format *event,
		      const char *name, struct tep_record *record,
		      unsigned long long *val, int err)
{
	struct format_field *field;
=======
int tep_get_field_val(struct trace_seq *s, struct tep_event *event,
		      const char *name, struct tep_record *record,
		      unsigned long long *val, int err)
{
	struct tep_format_field *field;
>>>>>>> upstream/android-13

	if (!event)
		return -1;

	field = tep_find_field(event, name);

	return get_field_val(s, field, name, record, val, err);
}

/**
 * tep_get_common_field_val - find a common field and return its value
 * @s: The seq to print to on error
 * @event: the event that the field is for
 * @name: The name of the field
 * @record: The record with the field name.
 * @val: place to store the value of the field.
 * @err: print default error if failed.
 *
 * Returns 0 on success -1 on field not found.
 */
<<<<<<< HEAD
int tep_get_common_field_val(struct trace_seq *s, struct event_format *event,
			     const char *name, struct tep_record *record,
			     unsigned long long *val, int err)
{
	struct format_field *field;
=======
int tep_get_common_field_val(struct trace_seq *s, struct tep_event *event,
			     const char *name, struct tep_record *record,
			     unsigned long long *val, int err)
{
	struct tep_format_field *field;
>>>>>>> upstream/android-13

	if (!event)
		return -1;

	field = tep_find_common_field(event, name);

	return get_field_val(s, field, name, record, val, err);
}

/**
 * tep_get_any_field_val - find a any field and return its value
 * @s: The seq to print to on error
 * @event: the event that the field is for
 * @name: The name of the field
 * @record: The record with the field name.
 * @val: place to store the value of the field.
 * @err: print default error if failed.
 *
 * Returns 0 on success -1 on field not found.
 */
<<<<<<< HEAD
int tep_get_any_field_val(struct trace_seq *s, struct event_format *event,
			  const char *name, struct tep_record *record,
			  unsigned long long *val, int err)
{
	struct format_field *field;
=======
int tep_get_any_field_val(struct trace_seq *s, struct tep_event *event,
			  const char *name, struct tep_record *record,
			  unsigned long long *val, int err)
{
	struct tep_format_field *field;
>>>>>>> upstream/android-13

	if (!event)
		return -1;

	field = tep_find_any_field(event, name);

	return get_field_val(s, field, name, record, val, err);
}

/**
 * tep_print_num_field - print a field and a format
 * @s: The seq to print to
 * @fmt: The printf format to print the field with.
 * @event: the event that the field is for
 * @name: The name of the field
 * @record: The record with the field name.
 * @err: print default error if failed.
 *
<<<<<<< HEAD
 * Returns: 0 on success, -1 field not found, or 1 if buffer is full.
 */
int tep_print_num_field(struct trace_seq *s, const char *fmt,
			struct event_format *event, const char *name,
			struct tep_record *record, int err)
{
	struct format_field *field = tep_find_field(event, name);
=======
 * Returns positive value on success, negative in case of an error,
 * or 0 if buffer is full.
 */
int tep_print_num_field(struct trace_seq *s, const char *fmt,
			struct tep_event *event, const char *name,
			struct tep_record *record, int err)
{
	struct tep_format_field *field = tep_find_field(event, name);
>>>>>>> upstream/android-13
	unsigned long long val;

	if (!field)
		goto failed;

	if (tep_read_number_field(field, record->data, &val))
		goto failed;

	return trace_seq_printf(s, fmt, val);

 failed:
	if (err)
		trace_seq_printf(s, "CAN'T FIND FIELD \"%s\"", name);
	return -1;
}

/**
 * tep_print_func_field - print a field and a format for function pointers
 * @s: The seq to print to
 * @fmt: The printf format to print the field with.
 * @event: the event that the field is for
 * @name: The name of the field
 * @record: The record with the field name.
 * @err: print default error if failed.
 *
<<<<<<< HEAD
 * Returns: 0 on success, -1 field not found, or 1 if buffer is full.
 */
int tep_print_func_field(struct trace_seq *s, const char *fmt,
			 struct event_format *event, const char *name,
			 struct tep_record *record, int err)
{
	struct format_field *field = tep_find_field(event, name);
	struct tep_handle *pevent = event->pevent;
=======
 * Returns positive value on success, negative in case of an error,
 * or 0 if buffer is full.
 */
int tep_print_func_field(struct trace_seq *s, const char *fmt,
			 struct tep_event *event, const char *name,
			 struct tep_record *record, int err)
{
	struct tep_format_field *field = tep_find_field(event, name);
	struct tep_handle *tep = event->tep;
>>>>>>> upstream/android-13
	unsigned long long val;
	struct func_map *func;
	char tmp[128];

	if (!field)
		goto failed;

	if (tep_read_number_field(field, record->data, &val))
		goto failed;

<<<<<<< HEAD
	func = find_func(pevent, val);
=======
	func = find_func(tep, val);
>>>>>>> upstream/android-13

	if (func)
		snprintf(tmp, 128, "%s/0x%llx", func->func, func->addr - val);
	else
		sprintf(tmp, "0x%08llx", val);

	return trace_seq_printf(s, fmt, tmp);

 failed:
	if (err)
		trace_seq_printf(s, "CAN'T FIND FIELD \"%s\"", name);
	return -1;
}

static void free_func_handle(struct tep_function_handler *func)
{
	struct func_params *params;

	free(func->name);

	while (func->params) {
		params = func->params;
		func->params = params->next;
		free(params);
	}

	free(func);
}

/**
 * tep_register_print_function - register a helper function
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @func: the function to process the helper function
 * @ret_type: the return type of the helper function
 * @name: the name of the helper function
 * @parameters: A list of enum tep_func_arg_type
 *
 * Some events may have helper functions in the print format arguments.
 * This allows a plugin to dynamically create a way to process one
 * of these functions.
 *
 * The @parameters is a variable list of tep_func_arg_type enums that
 * must end with TEP_FUNC_ARG_VOID.
 */
<<<<<<< HEAD
int tep_register_print_function(struct tep_handle *pevent,
=======
int tep_register_print_function(struct tep_handle *tep,
>>>>>>> upstream/android-13
				tep_func_handler func,
				enum tep_func_arg_type ret_type,
				char *name, ...)
{
	struct tep_function_handler *func_handle;
	struct func_params **next_param;
	struct func_params *param;
	enum tep_func_arg_type type;
	va_list ap;
	int ret;

<<<<<<< HEAD
	func_handle = find_func_handler(pevent, name);
=======
	func_handle = find_func_handler(tep, name);
>>>>>>> upstream/android-13
	if (func_handle) {
		/*
		 * This is most like caused by the users own
		 * plugins updating the function. This overrides the
		 * system defaults.
		 */
		pr_stat("override of function helper '%s'", name);
<<<<<<< HEAD
		remove_func_handler(pevent, name);
=======
		remove_func_handler(tep, name);
>>>>>>> upstream/android-13
	}

	func_handle = calloc(1, sizeof(*func_handle));
	if (!func_handle) {
		do_warning("Failed to allocate function handler");
		return TEP_ERRNO__MEM_ALLOC_FAILED;
	}

	func_handle->ret_type = ret_type;
	func_handle->name = strdup(name);
	func_handle->func = func;
	if (!func_handle->name) {
		do_warning("Failed to allocate function name");
		free(func_handle);
		return TEP_ERRNO__MEM_ALLOC_FAILED;
	}

	next_param = &(func_handle->params);
	va_start(ap, name);
	for (;;) {
		type = va_arg(ap, enum tep_func_arg_type);
		if (type == TEP_FUNC_ARG_VOID)
			break;

		if (type >= TEP_FUNC_ARG_MAX_TYPES) {
			do_warning("Invalid argument type %d", type);
			ret = TEP_ERRNO__INVALID_ARG_TYPE;
			goto out_free;
		}

		param = malloc(sizeof(*param));
		if (!param) {
			do_warning("Failed to allocate function param");
			ret = TEP_ERRNO__MEM_ALLOC_FAILED;
			goto out_free;
		}
		param->type = type;
		param->next = NULL;

		*next_param = param;
		next_param = &(param->next);

		func_handle->nr_args++;
	}
	va_end(ap);

<<<<<<< HEAD
	func_handle->next = pevent->func_handlers;
	pevent->func_handlers = func_handle;
=======
	func_handle->next = tep->func_handlers;
	tep->func_handlers = func_handle;
>>>>>>> upstream/android-13

	return 0;
 out_free:
	va_end(ap);
	free_func_handle(func_handle);
	return ret;
}

/**
 * tep_unregister_print_function - unregister a helper function
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @func: the function to process the helper function
 * @name: the name of the helper function
 *
 * This function removes existing print handler for function @name.
 *
 * Returns 0 if the handler was removed successully, -1 otherwise.
 */
<<<<<<< HEAD
int tep_unregister_print_function(struct tep_handle *pevent,
=======
int tep_unregister_print_function(struct tep_handle *tep,
>>>>>>> upstream/android-13
				  tep_func_handler func, char *name)
{
	struct tep_function_handler *func_handle;

<<<<<<< HEAD
	func_handle = find_func_handler(pevent, name);
	if (func_handle && func_handle->func == func) {
		remove_func_handler(pevent, name);
=======
	func_handle = find_func_handler(tep, name);
	if (func_handle && func_handle->func == func) {
		remove_func_handler(tep, name);
>>>>>>> upstream/android-13
		return 0;
	}
	return -1;
}

<<<<<<< HEAD
static struct event_format *search_event(struct tep_handle *pevent, int id,
					 const char *sys_name,
					 const char *event_name)
{
	struct event_format *event;

	if (id >= 0) {
		/* search by id */
		event = tep_find_event(pevent, id);
=======
static struct tep_event *search_event(struct tep_handle *tep, int id,
				      const char *sys_name,
				      const char *event_name)
{
	struct tep_event *event;

	if (id >= 0) {
		/* search by id */
		event = tep_find_event(tep, id);
>>>>>>> upstream/android-13
		if (!event)
			return NULL;
		if (event_name && (strcmp(event_name, event->name) != 0))
			return NULL;
		if (sys_name && (strcmp(sys_name, event->system) != 0))
			return NULL;
	} else {
<<<<<<< HEAD
		event = tep_find_event_by_name(pevent, sys_name, event_name);
=======
		event = tep_find_event_by_name(tep, sys_name, event_name);
>>>>>>> upstream/android-13
		if (!event)
			return NULL;
	}
	return event;
}

/**
 * tep_register_event_handler - register a way to parse an event
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @id: the id of the event to register
 * @sys_name: the system name the event belongs to
 * @event_name: the name of the event
 * @func: the function to call to parse the event information
 * @context: the data to be passed to @func
 *
 * This function allows a developer to override the parsing of
 * a given event. If for some reason the default print format
 * is not sufficient, this function will register a function
 * for an event to be used to parse the data instead.
 *
 * If @id is >= 0, then it is used to find the event.
 * else @sys_name and @event_name are used.
<<<<<<< HEAD
 */
int tep_register_event_handler(struct tep_handle *pevent, int id,
			       const char *sys_name, const char *event_name,
			       tep_event_handler_func func, void *context)
{
	struct event_format *event;
	struct event_handler *handle;

	event = search_event(pevent, id, sys_name, event_name);
=======
 *
 * Returns:
 *  TEP_REGISTER_SUCCESS_OVERWRITE if an existing handler is overwritten
 *  TEP_REGISTER_SUCCESS if a new handler is registered successfully
 *  negative TEP_ERRNO_... in case of an error
 *
 */
int tep_register_event_handler(struct tep_handle *tep, int id,
			       const char *sys_name, const char *event_name,
			       tep_event_handler_func func, void *context)
{
	struct tep_event *event;
	struct event_handler *handle;

	event = search_event(tep, id, sys_name, event_name);
>>>>>>> upstream/android-13
	if (event == NULL)
		goto not_found;

	pr_stat("overriding event (%d) %s:%s with new print handler",
		event->id, event->system, event->name);

	event->handler = func;
	event->context = context;
<<<<<<< HEAD
	return 0;
=======
	return TEP_REGISTER_SUCCESS_OVERWRITE;
>>>>>>> upstream/android-13

 not_found:
	/* Save for later use. */
	handle = calloc(1, sizeof(*handle));
	if (!handle) {
		do_warning("Failed to allocate event handler");
		return TEP_ERRNO__MEM_ALLOC_FAILED;
	}

	handle->id = id;
	if (event_name)
		handle->event_name = strdup(event_name);
	if (sys_name)
		handle->sys_name = strdup(sys_name);

	if ((event_name && !handle->event_name) ||
	    (sys_name && !handle->sys_name)) {
		do_warning("Failed to allocate event/sys name");
		free((void *)handle->event_name);
		free((void *)handle->sys_name);
		free(handle);
		return TEP_ERRNO__MEM_ALLOC_FAILED;
	}

	handle->func = func;
<<<<<<< HEAD
	handle->next = pevent->handlers;
	pevent->handlers = handle;
	handle->context = context;

	return -1;
=======
	handle->next = tep->handlers;
	tep->handlers = handle;
	handle->context = context;

	return TEP_REGISTER_SUCCESS;
>>>>>>> upstream/android-13
}

static int handle_matches(struct event_handler *handler, int id,
			  const char *sys_name, const char *event_name,
			  tep_event_handler_func func, void *context)
{
	if (id >= 0 && id != handler->id)
		return 0;

	if (event_name && (strcmp(event_name, handler->event_name) != 0))
		return 0;

	if (sys_name && (strcmp(sys_name, handler->sys_name) != 0))
		return 0;

	if (func != handler->func || context != handler->context)
		return 0;

	return 1;
}

/**
 * tep_unregister_event_handler - unregister an existing event handler
<<<<<<< HEAD
 * @pevent: the handle to the pevent
=======
 * @tep: a handle to the trace event parser context
>>>>>>> upstream/android-13
 * @id: the id of the event to unregister
 * @sys_name: the system name the handler belongs to
 * @event_name: the name of the event handler
 * @func: the function to call to parse the event information
 * @context: the data to be passed to @func
 *
 * This function removes existing event handler (parser).
 *
 * If @id is >= 0, then it is used to find the event.
 * else @sys_name and @event_name are used.
 *
 * Returns 0 if handler was removed successfully, -1 if event was not found.
 */
<<<<<<< HEAD
int tep_unregister_event_handler(struct tep_handle *pevent, int id,
				 const char *sys_name, const char *event_name,
				 tep_event_handler_func func, void *context)
{
	struct event_format *event;
	struct event_handler *handle;
	struct event_handler **next;

	event = search_event(pevent, id, sys_name, event_name);
=======
int tep_unregister_event_handler(struct tep_handle *tep, int id,
				 const char *sys_name, const char *event_name,
				 tep_event_handler_func func, void *context)
{
	struct tep_event *event;
	struct event_handler *handle;
	struct event_handler **next;

	event = search_event(tep, id, sys_name, event_name);
>>>>>>> upstream/android-13
	if (event == NULL)
		goto not_found;

	if (event->handler == func && event->context == context) {
		pr_stat("removing override handler for event (%d) %s:%s. Going back to default handler.",
			event->id, event->system, event->name);

		event->handler = NULL;
		event->context = NULL;
		return 0;
	}

not_found:
<<<<<<< HEAD
	for (next = &pevent->handlers; *next; next = &(*next)->next) {
=======
	for (next = &tep->handlers; *next; next = &(*next)->next) {
>>>>>>> upstream/android-13
		handle = *next;
		if (handle_matches(handle, id, sys_name, event_name,
				   func, context))
			break;
	}

	if (!(*next))
		return -1;

	*next = handle->next;
	free_handler(handle);

	return 0;
}

/**
<<<<<<< HEAD
 * tep_alloc - create a pevent handle
 */
struct tep_handle *tep_alloc(void)
{
	struct tep_handle *pevent = calloc(1, sizeof(*pevent));

	if (pevent)
		pevent->ref_count = 1;

	return pevent;
}

void tep_ref(struct tep_handle *pevent)
{
	pevent->ref_count++;
}

void tep_free_format_field(struct format_field *field)
=======
 * tep_alloc - create a tep handle
 */
struct tep_handle *tep_alloc(void)
{
	struct tep_handle *tep = calloc(1, sizeof(*tep));

	if (tep) {
		tep->ref_count = 1;
		tep->host_bigendian = tep_is_bigendian();
	}

	return tep;
}

void tep_ref(struct tep_handle *tep)
{
	tep->ref_count++;
}

int tep_get_ref(struct tep_handle *tep)
{
	if (tep)
		return tep->ref_count;
	return 0;
}

__hidden void free_tep_format_field(struct tep_format_field *field)
>>>>>>> upstream/android-13
{
	free(field->type);
	if (field->alias != field->name)
		free(field->alias);
	free(field->name);
	free(field);
}

<<<<<<< HEAD
static void free_format_fields(struct format_field *field)
{
	struct format_field *next;

	while (field) {
		next = field->next;
		tep_free_format_field(field);
=======
static void free_format_fields(struct tep_format_field *field)
{
	struct tep_format_field *next;

	while (field) {
		next = field->next;
		free_tep_format_field(field);
>>>>>>> upstream/android-13
		field = next;
	}
}

<<<<<<< HEAD
static void free_formats(struct format *format)
=======
static void free_formats(struct tep_format *format)
>>>>>>> upstream/android-13
{
	free_format_fields(format->common_fields);
	free_format_fields(format->fields);
}

<<<<<<< HEAD
void tep_free_format(struct event_format *event)
=======
__hidden void free_tep_event(struct tep_event *event)
>>>>>>> upstream/android-13
{
	free(event->name);
	free(event->system);

	free_formats(&event->format);

	free(event->print_fmt.format);
	free_args(event->print_fmt.args);
<<<<<<< HEAD

=======
	free_parse_args(event->print_fmt.print_cache);
>>>>>>> upstream/android-13
	free(event);
}

/**
<<<<<<< HEAD
 * tep_free - free a pevent handle
 * @pevent: the pevent handle to free
 */
void tep_free(struct tep_handle *pevent)
=======
 * tep_free - free a tep handle
 * @tep: the tep handle to free
 */
void tep_free(struct tep_handle *tep)
>>>>>>> upstream/android-13
{
	struct cmdline_list *cmdlist, *cmdnext;
	struct func_list *funclist, *funcnext;
	struct printk_list *printklist, *printknext;
	struct tep_function_handler *func_handler;
	struct event_handler *handle;
	int i;

<<<<<<< HEAD
	if (!pevent)
		return;

	cmdlist = pevent->cmdlist;
	funclist = pevent->funclist;
	printklist = pevent->printklist;

	pevent->ref_count--;
	if (pevent->ref_count)
		return;

	if (pevent->cmdlines) {
		for (i = 0; i < pevent->cmdline_count; i++)
			free(pevent->cmdlines[i].comm);
		free(pevent->cmdlines);
=======
	if (!tep)
		return;

	cmdlist = tep->cmdlist;
	funclist = tep->funclist;
	printklist = tep->printklist;

	tep->ref_count--;
	if (tep->ref_count)
		return;

	if (tep->cmdlines) {
		for (i = 0; i < tep->cmdline_count; i++)
			free(tep->cmdlines[i].comm);
		free(tep->cmdlines);
>>>>>>> upstream/android-13
	}

	while (cmdlist) {
		cmdnext = cmdlist->next;
		free(cmdlist->comm);
		free(cmdlist);
		cmdlist = cmdnext;
	}

<<<<<<< HEAD
	if (pevent->func_map) {
		for (i = 0; i < (int)pevent->func_count; i++) {
			free(pevent->func_map[i].func);
			free(pevent->func_map[i].mod);
		}
		free(pevent->func_map);
=======
	if (tep->func_map) {
		for (i = 0; i < (int)tep->func_count; i++) {
			free(tep->func_map[i].func);
			free(tep->func_map[i].mod);
		}
		free(tep->func_map);
>>>>>>> upstream/android-13
	}

	while (funclist) {
		funcnext = funclist->next;
		free(funclist->func);
		free(funclist->mod);
		free(funclist);
		funclist = funcnext;
	}

<<<<<<< HEAD
	while (pevent->func_handlers) {
		func_handler = pevent->func_handlers;
		pevent->func_handlers = func_handler->next;
		free_func_handle(func_handler);
	}

	if (pevent->printk_map) {
		for (i = 0; i < (int)pevent->printk_count; i++)
			free(pevent->printk_map[i].printk);
		free(pevent->printk_map);
=======
	while (tep->func_handlers) {
		func_handler = tep->func_handlers;
		tep->func_handlers = func_handler->next;
		free_func_handle(func_handler);
	}

	if (tep->printk_map) {
		for (i = 0; i < (int)tep->printk_count; i++)
			free(tep->printk_map[i].printk);
		free(tep->printk_map);
>>>>>>> upstream/android-13
	}

	while (printklist) {
		printknext = printklist->next;
		free(printklist->printk);
		free(printklist);
		printklist = printknext;
	}

<<<<<<< HEAD
	for (i = 0; i < pevent->nr_events; i++)
		tep_free_format(pevent->events[i]);

	while (pevent->handlers) {
		handle = pevent->handlers;
		pevent->handlers = handle->next;
		free_handler(handle);
	}

	free(pevent->trace_clock);
	free(pevent->events);
	free(pevent->sort_events);
	free(pevent->func_resolver);

	free(pevent);
}

void tep_unref(struct tep_handle *pevent)
{
	tep_free(pevent);
=======
	for (i = 0; i < tep->nr_events; i++)
		free_tep_event(tep->events[i]);

	while (tep->handlers) {
		handle = tep->handlers;
		tep->handlers = handle->next;
		free_handler(handle);
	}

	free(tep->events);
	free(tep->sort_events);
	free(tep->func_resolver);
	free_tep_plugin_paths(tep);

	free(tep);
}

void tep_unref(struct tep_handle *tep)
{
	tep_free(tep);
>>>>>>> upstream/android-13
}
