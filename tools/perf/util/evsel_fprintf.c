// SPDX-License-Identifier: GPL-2.0
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <traceevent/event-parse.h>
#include "evsel.h"
<<<<<<< HEAD
=======
#include "util/evsel_fprintf.h"
#include "util/event.h"
>>>>>>> upstream/android-13
#include "callchain.h"
#include "map.h"
#include "strlist.h"
#include "symbol.h"
#include "srcline.h"

static int comma_fprintf(FILE *fp, bool *first, const char *fmt, ...)
{
	va_list args;
	int ret = 0;

	if (!*first) {
		ret += fprintf(fp, ",");
	} else {
		ret += fprintf(fp, ":");
		*first = false;
	}

	va_start(args, fmt);
	ret += vfprintf(fp, fmt, args);
	va_end(args);
	return ret;
}

static int __print_attr__fprintf(FILE *fp, const char *name, const char *val, void *priv)
{
	return comma_fprintf(fp, (bool *)priv, " %s: %s", name, val);
}

<<<<<<< HEAD
int perf_evsel__fprintf(struct perf_evsel *evsel,
			struct perf_attr_details *details, FILE *fp)
=======
int evsel__fprintf(struct evsel *evsel, struct perf_attr_details *details, FILE *fp)
>>>>>>> upstream/android-13
{
	bool first = true;
	int printed = 0;

	if (details->event_group) {
<<<<<<< HEAD
		struct perf_evsel *pos;

		if (!perf_evsel__is_group_leader(evsel))
			return 0;

		if (evsel->nr_members > 1)
			printed += fprintf(fp, "%s{", evsel->group_name ?: "");

		printed += fprintf(fp, "%s", perf_evsel__name(evsel));
		for_each_group_member(pos, evsel)
			printed += fprintf(fp, ",%s", perf_evsel__name(pos));

		if (evsel->nr_members > 1)
=======
		struct evsel *pos;

		if (!evsel__is_group_leader(evsel))
			return 0;

		if (evsel->core.nr_members > 1)
			printed += fprintf(fp, "%s{", evsel->group_name ?: "");

		printed += fprintf(fp, "%s", evsel__name(evsel));
		for_each_group_member(pos, evsel)
			printed += fprintf(fp, ",%s", evsel__name(pos));

		if (evsel->core.nr_members > 1)
>>>>>>> upstream/android-13
			printed += fprintf(fp, "}");
		goto out;
	}

<<<<<<< HEAD
	printed += fprintf(fp, "%s", perf_evsel__name(evsel));

	if (details->verbose) {
		printed += perf_event_attr__fprintf(fp, &evsel->attr,
=======
	printed += fprintf(fp, "%s", evsel__name(evsel));

	if (details->verbose) {
		printed += perf_event_attr__fprintf(fp, &evsel->core.attr,
>>>>>>> upstream/android-13
						    __print_attr__fprintf, &first);
	} else if (details->freq) {
		const char *term = "sample_freq";

<<<<<<< HEAD
		if (!evsel->attr.freq)
			term = "sample_period";

		printed += comma_fprintf(fp, &first, " %s=%" PRIu64,
					 term, (u64)evsel->attr.sample_freq);
	}

	if (details->trace_fields) {
		struct format_field *field;

		if (evsel->attr.type != PERF_TYPE_TRACEPOINT) {
=======
		if (!evsel->core.attr.freq)
			term = "sample_period";

		printed += comma_fprintf(fp, &first, " %s=%" PRIu64,
					 term, (u64)evsel->core.attr.sample_freq);
	}

	if (details->trace_fields) {
		struct tep_format_field *field;

		if (evsel->core.attr.type != PERF_TYPE_TRACEPOINT) {
>>>>>>> upstream/android-13
			printed += comma_fprintf(fp, &first, " (not a tracepoint)");
			goto out;
		}

		field = evsel->tp_format->format.fields;
		if (field == NULL) {
			printed += comma_fprintf(fp, &first, " (no trace field)");
			goto out;
		}

		printed += comma_fprintf(fp, &first, " trace_fields: %s", field->name);

		field = field->next;
		while (field) {
			printed += comma_fprintf(fp, &first, "%s", field->name);
			field = field->next;
		}
	}
out:
	fputc('\n', fp);
	return ++printed;
}

<<<<<<< HEAD
int sample__fprintf_callchain(struct perf_sample *sample, int left_alignment,
			      unsigned int print_opts, struct callchain_cursor *cursor,
			      FILE *fp)
=======
#ifndef PYTHON_PERF
int sample__fprintf_callchain(struct perf_sample *sample, int left_alignment,
			      unsigned int print_opts, struct callchain_cursor *cursor,
			      struct strlist *bt_stop_list, FILE *fp)
>>>>>>> upstream/android-13
{
	int printed = 0;
	struct callchain_cursor_node *node;
	int print_ip = print_opts & EVSEL__PRINT_IP;
	int print_sym = print_opts & EVSEL__PRINT_SYM;
	int print_dso = print_opts & EVSEL__PRINT_DSO;
	int print_symoffset = print_opts & EVSEL__PRINT_SYMOFFSET;
	int print_oneline = print_opts & EVSEL__PRINT_ONELINE;
	int print_srcline = print_opts & EVSEL__PRINT_SRCLINE;
	int print_unknown_as_addr = print_opts & EVSEL__PRINT_UNKNOWN_AS_ADDR;
	int print_arrow = print_opts & EVSEL__PRINT_CALLCHAIN_ARROW;
	int print_skip_ignored = print_opts & EVSEL__PRINT_SKIP_IGNORED;
	char s = print_oneline ? ' ' : '\t';
	bool first = true;

	if (sample->callchain) {
		struct addr_location node_al;

		callchain_cursor_commit(cursor);

		while (1) {
<<<<<<< HEAD
=======
			struct symbol *sym;
			struct map *map;
>>>>>>> upstream/android-13
			u64 addr = 0;

			node = callchain_cursor_current(cursor);
			if (!node)
				break;

<<<<<<< HEAD
			if (node->sym && node->sym->ignore && print_skip_ignored)
=======
			sym = node->ms.sym;
			map = node->ms.map;

			if (sym && sym->ignore && print_skip_ignored)
>>>>>>> upstream/android-13
				goto next;

			printed += fprintf(fp, "%-*.*s", left_alignment, left_alignment, " ");

			if (print_arrow && !first)
				printed += fprintf(fp, " <-");

			if (print_ip)
				printed += fprintf(fp, "%c%16" PRIx64, s, node->ip);

<<<<<<< HEAD
			if (node->map)
				addr = node->map->map_ip(node->map, node->ip);
=======
			if (map)
				addr = map->map_ip(map, node->ip);
>>>>>>> upstream/android-13

			if (print_sym) {
				printed += fprintf(fp, " ");
				node_al.addr = addr;
<<<<<<< HEAD
				node_al.map  = node->map;

				if (print_symoffset) {
					printed += __symbol__fprintf_symname_offs(node->sym, &node_al,
										  print_unknown_as_addr,
										  true, fp);
				} else {
					printed += __symbol__fprintf_symname(node->sym, &node_al,
=======
				node_al.map  = map;

				if (print_symoffset) {
					printed += __symbol__fprintf_symname_offs(sym, &node_al,
										  print_unknown_as_addr,
										  true, fp);
				} else {
					printed += __symbol__fprintf_symname(sym, &node_al,
>>>>>>> upstream/android-13
									     print_unknown_as_addr, fp);
				}
			}

<<<<<<< HEAD
			if (print_dso && (!node->sym || !node->sym->inlined)) {
				printed += fprintf(fp, " (");
				printed += map__fprintf_dsoname(node->map, fp);
=======
			if (print_dso && (!sym || !sym->inlined)) {
				printed += fprintf(fp, " (");
				printed += map__fprintf_dsoname(map, fp);
>>>>>>> upstream/android-13
				printed += fprintf(fp, ")");
			}

			if (print_srcline)
<<<<<<< HEAD
				printed += map__fprintf_srcline(node->map, addr, "\n  ", fp);

			if (node->sym && node->sym->inlined)
=======
				printed += map__fprintf_srcline(map, addr, "\n  ", fp);

			if (sym && sym->inlined)
>>>>>>> upstream/android-13
				printed += fprintf(fp, " (inlined)");

			if (!print_oneline)
				printed += fprintf(fp, "\n");

<<<<<<< HEAD
			if (symbol_conf.bt_stop_list &&
			    node->sym &&
			    strlist__has_entry(symbol_conf.bt_stop_list,
					       node->sym->name)) {
=======
			/* Add srccode here too? */
			if (bt_stop_list && sym &&
			    strlist__has_entry(bt_stop_list, sym->name)) {
>>>>>>> upstream/android-13
				break;
			}

			first = false;
next:
			callchain_cursor_advance(cursor);
		}
	}

	return printed;
}

int sample__fprintf_sym(struct perf_sample *sample, struct addr_location *al,
			int left_alignment, unsigned int print_opts,
<<<<<<< HEAD
			struct callchain_cursor *cursor, FILE *fp)
=======
			struct callchain_cursor *cursor, struct strlist *bt_stop_list, FILE *fp)
>>>>>>> upstream/android-13
{
	int printed = 0;
	int print_ip = print_opts & EVSEL__PRINT_IP;
	int print_sym = print_opts & EVSEL__PRINT_SYM;
	int print_dso = print_opts & EVSEL__PRINT_DSO;
	int print_symoffset = print_opts & EVSEL__PRINT_SYMOFFSET;
	int print_srcline = print_opts & EVSEL__PRINT_SRCLINE;
	int print_unknown_as_addr = print_opts & EVSEL__PRINT_UNKNOWN_AS_ADDR;

	if (cursor != NULL) {
<<<<<<< HEAD
		printed += sample__fprintf_callchain(sample, left_alignment,
						     print_opts, cursor, fp);
=======
		printed += sample__fprintf_callchain(sample, left_alignment, print_opts,
						     cursor, bt_stop_list, fp);
>>>>>>> upstream/android-13
	} else {
		printed += fprintf(fp, "%-*.*s", left_alignment, left_alignment, " ");

		if (print_ip)
			printed += fprintf(fp, "%16" PRIx64, sample->ip);

		if (print_sym) {
			printed += fprintf(fp, " ");
			if (print_symoffset) {
				printed += __symbol__fprintf_symname_offs(al->sym, al,
									  print_unknown_as_addr,
									  true, fp);
			} else {
				printed += __symbol__fprintf_symname(al->sym, al,
								     print_unknown_as_addr, fp);
			}
		}

		if (print_dso) {
			printed += fprintf(fp, " (");
			printed += map__fprintf_dsoname(al->map, fp);
			printed += fprintf(fp, ")");
		}

		if (print_srcline)
			printed += map__fprintf_srcline(al->map, al->addr, "\n  ", fp);
	}

	return printed;
}
<<<<<<< HEAD
=======
#endif /* PYTHON_PERF */
>>>>>>> upstream/android-13
