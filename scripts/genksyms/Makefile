# SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
hostprogs-y	:= genksyms
always		:= $(hostprogs-y)
=======
hostprogs-always-y	+= genksyms
>>>>>>> upstream/android-13

genksyms-objs	:= genksyms.o parse.tab.o lex.lex.o

# FIXME: fix the ambiguous grammar in parse.y and delete this hack
#
# Suppress shift/reduce, reduce/reduce conflicts warnings
# unless W=1 is specified.
#
# Just in case, run "$(YACC) --version" without suppressing stderr
# so that 'bison: not found' will be displayed if it is missing.
<<<<<<< HEAD
ifeq ($(findstring 1,$(KBUILD_ENABLE_EXTRA_GCC_CHECKS)),)
=======
ifeq ($(findstring 1,$(KBUILD_EXTRA_WARN)),)
>>>>>>> upstream/android-13

quiet_cmd_bison_no_warn = $(quiet_cmd_bison)
      cmd_bison_no_warn = $(YACC) --version >/dev/null; \
			  $(cmd_bison) 2>/dev/null

<<<<<<< HEAD
$(obj)/parse.tab.c: $(src)/parse.y FORCE
	$(call if_changed,bison_no_warn)

quiet_cmd_bison_h_no_warn = $(quiet_cmd_bison_h)
      cmd_bison_h_no_warn = $(YACC) --version >/dev/null; \
			    $(cmd_bison_h) 2>/dev/null

$(obj)/parse.tab.h: $(src)/parse.y FORCE
	$(call if_changed,bison_h_no_warn)

endif

# -I needed for generated C source (shipped source)
HOSTCFLAGS_parse.tab.o := -I$(src)
HOSTCFLAGS_lex.lex.o := -I$(src)
=======
$(obj)/pars%.tab.c $(obj)/pars%.tab.h: $(src)/pars%.y FORCE
	$(call if_changed,bison_no_warn)

endif

# -I needed for generated C source to include headers in source tree
HOSTCFLAGS_parse.tab.o := -I $(srctree)/$(src)
HOSTCFLAGS_lex.lex.o := -I $(srctree)/$(src)
>>>>>>> upstream/android-13

# dependencies on generated files need to be listed explicitly
$(obj)/lex.lex.o: $(obj)/parse.tab.h
