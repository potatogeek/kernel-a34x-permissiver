/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _PERF_UTIL_TRACE_EVENT_H
#define _PERF_UTIL_TRACE_EVENT_H

#include <traceevent/event-parse.h>
#include "parse-events.h"

struct machine;
struct perf_sample;
union perf_event;
struct perf_tool;
struct thread;
<<<<<<< HEAD
struct plugin_list;

struct trace_event {
	struct tep_handle	*pevent;
	struct plugin_list	*plugin_list;
=======
struct tep_plugin_list;
struct evsel;

struct trace_event {
	struct tep_handle	*pevent;
	struct tep_plugin_list	*plugin_list;
>>>>>>> upstream/android-13
};

int trace_event__init(struct trace_event *t);
void trace_event__cleanup(struct trace_event *t);
int trace_event__register_resolver(struct machine *machine,
				   tep_func_resolver_t *func);
<<<<<<< HEAD
struct event_format*
trace_event__tp_format(const char *sys, const char *name);

struct event_format *trace_event__tp_format_id(int id);

int bigendian(void);

void event_format__fprintf(struct event_format *event,
			   int cpu, void *data, int size, FILE *fp);

void event_format__print(struct event_format *event,
=======
struct tep_event*
trace_event__tp_format(const char *sys, const char *name);

struct tep_event *trace_event__tp_format_id(int id);

int bigendian(void);

void event_format__fprintf(struct tep_event *event,
			   int cpu, void *data, int size, FILE *fp);

void event_format__print(struct tep_event *event,
>>>>>>> upstream/android-13
			 int cpu, void *data, int size);

int parse_ftrace_file(struct tep_handle *pevent, char *buf, unsigned long size);
int parse_event_file(struct tep_handle *pevent,
		     char *buf, unsigned long size, char *sys);

unsigned long long
<<<<<<< HEAD
raw_field_value(struct event_format *event, const char *name, void *data);
=======
raw_field_value(struct tep_event *event, const char *name, void *data);
>>>>>>> upstream/android-13

void parse_proc_kallsyms(struct tep_handle *pevent, char *file, unsigned int size);
void parse_ftrace_printk(struct tep_handle *pevent, char *file, unsigned int size);
void parse_saved_cmdline(struct tep_handle *pevent, char *file, unsigned int size);

ssize_t trace_report(int fd, struct trace_event *tevent, bool repipe);

<<<<<<< HEAD
struct event_format *trace_find_next_event(struct tep_handle *pevent,
					   struct event_format *event);
unsigned long long read_size(struct event_format *event, void *ptr, int size);
=======
unsigned long long read_size(struct tep_event *event, void *ptr, int size);
>>>>>>> upstream/android-13
unsigned long long eval_flag(const char *flag);

int read_tracing_data(int fd, struct list_head *pattrs);

struct tracing_data {
	/* size is only valid if temp is 'true' */
	ssize_t size;
	bool temp;
	char temp_file[50];
};

struct tracing_data *tracing_data_get(struct list_head *pattrs,
				      int fd, bool temp);
int tracing_data_put(struct tracing_data *tdata);


struct addr_location;

struct perf_session;
struct perf_stat_config;

struct scripting_ops {
	const char *name;
<<<<<<< HEAD
	int (*start_script) (const char *script, int argc, const char **argv);
=======
	const char *dirname; /* For script path .../scripts/<dirname>/... */
	int (*start_script)(const char *script, int argc, const char **argv,
			    struct perf_session *session);
>>>>>>> upstream/android-13
	int (*flush_script) (void);
	int (*stop_script) (void);
	void (*process_event) (union perf_event *event,
			       struct perf_sample *sample,
<<<<<<< HEAD
			       struct perf_evsel *evsel,
			       struct addr_location *al);
	void (*process_stat)(struct perf_stat_config *config,
			     struct perf_evsel *evsel, u64 tstamp);
	void (*process_stat_interval)(u64 tstamp);
=======
			       struct evsel *evsel,
			       struct addr_location *al,
			       struct addr_location *addr_al);
	void (*process_switch)(union perf_event *event,
			       struct perf_sample *sample,
			       struct machine *machine);
	void (*process_auxtrace_error)(struct perf_session *session,
				       union perf_event *event);
	void (*process_stat)(struct perf_stat_config *config,
			     struct evsel *evsel, u64 tstamp);
	void (*process_stat_interval)(u64 tstamp);
	void (*process_throttle)(union perf_event *event,
				 struct perf_sample *sample,
				 struct machine *machine);
>>>>>>> upstream/android-13
	int (*generate_script) (struct tep_handle *pevent, const char *outfile);
};

extern unsigned int scripting_max_stack;

int script_spec_register(const char *spec, struct scripting_ops *ops);

<<<<<<< HEAD
=======
void script_fetch_insn(struct perf_sample *sample, struct thread *thread,
		       struct machine *machine);

>>>>>>> upstream/android-13
void setup_perl_scripting(void);
void setup_python_scripting(void);

struct scripting_context {
	struct tep_handle *pevent;
	void *event_data;
<<<<<<< HEAD
};

=======
	union perf_event *event;
	struct perf_sample *sample;
	struct evsel *evsel;
	struct addr_location *al;
	struct addr_location *addr_al;
	struct perf_session *session;
};

void scripting_context__update(struct scripting_context *scripting_context,
			       union perf_event *event,
			       struct perf_sample *sample,
			       struct evsel *evsel,
			       struct addr_location *al,
			       struct addr_location *addr_al);

>>>>>>> upstream/android-13
int common_pc(struct scripting_context *context);
int common_flags(struct scripting_context *context);
int common_lock_depth(struct scripting_context *context);

<<<<<<< HEAD
=======
#define SAMPLE_FLAGS_BUF_SIZE 64
int perf_sample__sprintf_flags(u32 flags, char *str, size_t sz);

>>>>>>> upstream/android-13
#endif /* _PERF_UTIL_TRACE_EVENT_H */
