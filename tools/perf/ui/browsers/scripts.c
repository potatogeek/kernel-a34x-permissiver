// SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
#include <elf.h>
#include <inttypes.h>
#include <sys/ttydefaults.h>
#include <string.h>
#include "../../util/sort.h"
#include "../../util/util.h"
=======
#include "../../builtin.h"
#include "../../perf.h"
#include "../../util/util.h" // perf_exe()
#include "../util.h"
>>>>>>> upstream/android-13
#include "../../util/hist.h"
#include "../../util/debug.h"
#include "../../util/symbol.h"
#include "../browser.h"
<<<<<<< HEAD
#include "../helpline.h"
#include "../libslang.h"

/* 2048 lines should be enough for a script output */
#define MAX_LINES		2048

/* 160 bytes for one output line */
#define AVERAGE_LINE_LEN	160

struct script_line {
	struct list_head node;
	char line[AVERAGE_LINE_LEN];
};

struct perf_script_browser {
	struct ui_browser b;
	struct list_head entries;
	const char *script_name;
	int nr_lines;
};
=======
#include "../libslang.h"
#include "config.h"
#include <linux/string.h>
#include <linux/zalloc.h>
#include <stdlib.h>
>>>>>>> upstream/android-13

#define SCRIPT_NAMELEN	128
#define SCRIPT_MAX_NO	64
/*
 * Usually the full path for a script is:
 *	/home/username/libexec/perf-core/scripts/python/xxx.py
 *	/home/username/libexec/perf-core/scripts/perl/xxx.pl
 * So 256 should be long enough to contain the full path.
 */
#define SCRIPT_FULLPATH_LEN	256

<<<<<<< HEAD
=======
struct script_config {
	const char **names;
	char **paths;
	int index;
	const char *perf;
	char extra_format[256];
};

void attr_to_script(char *extra_format, struct perf_event_attr *attr)
{
	extra_format[0] = 0;
	if (attr->read_format & PERF_FORMAT_GROUP)
		strcat(extra_format, " -F +metric");
	if (attr->sample_type & PERF_SAMPLE_BRANCH_STACK)
		strcat(extra_format, " -F +brstackinsn --xed");
	if (attr->sample_type & PERF_SAMPLE_REGS_INTR)
		strcat(extra_format, " -F +iregs");
	if (attr->sample_type & PERF_SAMPLE_REGS_USER)
		strcat(extra_format, " -F +uregs");
	if (attr->sample_type & PERF_SAMPLE_PHYS_ADDR)
		strcat(extra_format, " -F +phys_addr");
}

static int add_script_option(const char *name, const char *opt,
			     struct script_config *c)
{
	c->names[c->index] = name;
	if (asprintf(&c->paths[c->index],
		     "%s script %s -F +metric %s %s",
		     c->perf, opt, symbol_conf.inline_name ? " --inline" : "",
		     c->extra_format) < 0)
		return -1;
	c->index++;
	return 0;
}

static int scripts_config(const char *var, const char *value, void *data)
{
	struct script_config *c = data;

	if (!strstarts(var, "scripts."))
		return -1;
	if (c->index >= SCRIPT_MAX_NO)
		return -1;
	c->names[c->index] = strdup(var + 7);
	if (!c->names[c->index])
		return -1;
	if (asprintf(&c->paths[c->index], "%s %s", value,
		     c->extra_format) < 0)
		return -1;
	c->index++;
	return 0;
}

>>>>>>> upstream/android-13
/*
 * When success, will copy the full path of the selected script
 * into  the buffer pointed by script_name, and return 0.
 * Return -1 on failure.
 */
<<<<<<< HEAD
static int list_scripts(char *script_name)
{
	char *buf, *names[SCRIPT_MAX_NO], *paths[SCRIPT_MAX_NO];
	int i, num, choice, ret = -1;
=======
static int list_scripts(char *script_name, bool *custom,
			struct evsel *evsel)
{
	char *buf, *paths[SCRIPT_MAX_NO], *names[SCRIPT_MAX_NO];
	int i, num, choice;
	int ret = 0;
	int max_std, custom_perf;
	char pbuf[256];
	const char *perf = perf_exe(pbuf, sizeof pbuf);
	struct script_config scriptc = {
		.names = (const char **)names,
		.paths = paths,
		.perf = perf
	};

	script_name[0] = 0;
>>>>>>> upstream/android-13

	/* Preset the script name to SCRIPT_NAMELEN */
	buf = malloc(SCRIPT_MAX_NO * (SCRIPT_NAMELEN + SCRIPT_FULLPATH_LEN));
	if (!buf)
<<<<<<< HEAD
		return ret;

	for (i = 0; i < SCRIPT_MAX_NO; i++) {
		names[i] = buf + i * (SCRIPT_NAMELEN + SCRIPT_FULLPATH_LEN);
		paths[i] = names[i] + SCRIPT_NAMELEN;
	}

	num = find_scripts(names, paths);
	if (num > 0) {
		choice = ui__popup_menu(num, names);
		if (choice < num && choice >= 0) {
			strcpy(script_name, paths[choice]);
			ret = 0;
		}
	}

	free(buf);
	return ret;
}

static void script_browser__write(struct ui_browser *browser,
				   void *entry, int row)
{
	struct script_line *sline = list_entry(entry, struct script_line, node);
	bool current_entry = ui_browser__is_current_entry(browser, row);

	ui_browser__set_color(browser, current_entry ? HE_COLORSET_SELECTED :
						       HE_COLORSET_NORMAL);

	ui_browser__write_nstring(browser, sline->line, browser->width);
}

static int script_browser__run(struct perf_script_browser *browser)
{
	int key;

	if (ui_browser__show(&browser->b, browser->script_name,
			     "Press ESC to exit") < 0)
		return -1;

	while (1) {
		key = ui_browser__run(&browser->b, 0);

		/* We can add some special key handling here if needed */
		break;
	}

	ui_browser__hide(&browser->b);
	return key;
}


int script_browse(const char *script_opt)
{
	char cmd[SCRIPT_FULLPATH_LEN*2], script_name[SCRIPT_FULLPATH_LEN];
	char *line = NULL;
	size_t len = 0;
	ssize_t retlen;
	int ret = -1, nr_entries = 0;
	FILE *fp;
	void *buf;
	struct script_line *sline;

	struct perf_script_browser script = {
		.b = {
			.refresh    = ui_browser__list_head_refresh,
			.seek	    = ui_browser__list_head_seek,
			.write	    = script_browser__write,
		},
		.script_name = script_name,
	};

	INIT_LIST_HEAD(&script.entries);

	/* Save each line of the output in one struct script_line object. */
	buf = zalloc((sizeof(*sline)) * MAX_LINES);
	if (!buf)
		return -1;
	sline = buf;

	memset(script_name, 0, SCRIPT_FULLPATH_LEN);
	if (list_scripts(script_name))
		goto exit;

	sprintf(cmd, "perf script -s %s ", script_name);

	if (script_opt)
		strcat(cmd, script_opt);

	if (input_name) {
		strcat(cmd, " -i ");
		strcat(cmd, input_name);
	}

	strcat(cmd, " 2>&1");

	fp = popen(cmd, "r");
	if (!fp)
		goto exit;

	while ((retlen = getline(&line, &len, fp)) != -1) {
		strncpy(sline->line, line, AVERAGE_LINE_LEN);

		/* If one output line is very large, just cut it short */
		if (retlen >= AVERAGE_LINE_LEN) {
			sline->line[AVERAGE_LINE_LEN - 1] = '\0';
			sline->line[AVERAGE_LINE_LEN - 2] = '\n';
		}
		list_add_tail(&sline->node, &script.entries);

		if (script.b.width < retlen)
			script.b.width = retlen;

		if (nr_entries++ >= MAX_LINES - 1)
			break;
		sline++;
	}

	if (script.b.width > AVERAGE_LINE_LEN)
		script.b.width = AVERAGE_LINE_LEN;

	free(line);
	pclose(fp);

	script.nr_lines = nr_entries;
	script.b.nr_entries = nr_entries;
	script.b.entries = &script.entries;

	ret = script_browser__run(&script);
exit:
	free(buf);
	return ret;
=======
		return -1;

	if (evsel)
		attr_to_script(scriptc.extra_format, &evsel->core.attr);
	add_script_option("Show individual samples", "", &scriptc);
	add_script_option("Show individual samples with assembler", "-F +insn --xed",
			  &scriptc);
	add_script_option("Show individual samples with source", "-F +srcline,+srccode",
			  &scriptc);
	perf_config(scripts_config, &scriptc);
	custom_perf = scriptc.index;
	add_script_option("Show samples with custom perf script arguments", "", &scriptc);
	i = scriptc.index;
	max_std = i;

	for (; i < SCRIPT_MAX_NO; i++) {
		names[i] = buf + (i - max_std) * (SCRIPT_NAMELEN + SCRIPT_FULLPATH_LEN);
		paths[i] = names[i] + SCRIPT_NAMELEN;
	}

	num = find_scripts(names + max_std, paths + max_std, SCRIPT_MAX_NO - max_std,
			SCRIPT_FULLPATH_LEN);
	if (num < 0)
		num = 0;
	choice = ui__popup_menu(num + max_std, (char * const *)names, NULL);
	if (choice < 0) {
		ret = -1;
		goto out;
	}
	if (choice == custom_perf) {
		char script_args[50];
		int key = ui_browser__input_window("perf script command",
				"Enter perf script command line (without perf script prefix)",
				script_args, "", 0);
		if (key != K_ENTER) {
			ret = -1;
			goto out;
		}
		sprintf(script_name, "%s script %s", perf, script_args);
	} else if (choice < num + max_std) {
		strcpy(script_name, paths[choice]);
	}
	*custom = choice >= max_std;

out:
	free(buf);
	for (i = 0; i < max_std; i++)
		zfree(&paths[i]);
	return ret;
}

void run_script(char *cmd)
{
	pr_debug("Running %s\n", cmd);
	SLang_reset_tty();
	if (system(cmd) < 0)
		pr_warning("Cannot run %s\n", cmd);
	/*
	 * SLang doesn't seem to reset the whole terminal, so be more
	 * forceful to get back to the original state.
	 */
	printf("\033[c\033[H\033[J");
	fflush(stdout);
	SLang_init_tty(0, 0, 0);
	SLsmg_refresh();
}

int script_browse(const char *script_opt, struct evsel *evsel)
{
	char *cmd, script_name[SCRIPT_FULLPATH_LEN];
	bool custom = false;

	memset(script_name, 0, SCRIPT_FULLPATH_LEN);
	if (list_scripts(script_name, &custom, evsel))
		return -1;

	if (asprintf(&cmd, "%s%s %s %s%s 2>&1 | less",
			custom ? "perf script -s " : "",
			script_name,
			script_opt ? script_opt : "",
			input_name ? "-i " : "",
			input_name ? input_name : "") < 0)
		return -1;

	run_script(cmd);
	free(cmd);

	return 0;
>>>>>>> upstream/android-13
}
