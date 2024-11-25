# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
always := gdb-scripts

SRCTREE := $(abspath $(srctree))

$(obj)/gdb-scripts:
ifneq ($(KBUILD_SRC),)
	$(Q)ln -fsn $(SRCTREE)/$(obj)/*.py $(objtree)/$(obj)
endif
	@:
=======

ifdef building_out_of_srctree

symlinks := $(patsubst $(srctree)/$(src)/%,%,$(wildcard $(srctree)/$(src)/*.py))

quiet_cmd_symlink = SYMLINK $@
      cmd_symlink = ln -fsn $(patsubst $(obj)/%,$(abspath $(srctree))/$(src)/%,$@) $@

always-y += $(symlinks)
$(addprefix $(obj)/, $(symlinks)): FORCE
	$(call if_changed,symlink)

endif
>>>>>>> upstream/android-13

quiet_cmd_gen_constants_py = GEN     $@
      cmd_gen_constants_py = \
	$(CPP) -E -x c -P $(c_flags) $< > $@ ;\
	sed -i '1,/<!-- end-c-headers -->/d;' $@

<<<<<<< HEAD
targets += constants.py
$(obj)/constants.py: $(SRCTREE)/$(obj)/constants.py.in FORCE
	$(call if_changed_dep,gen_constants_py)

build_constants_py: $(obj)/constants.py
	@:

clean-files := *.pyc *.pyo $(if $(KBUILD_SRC),*.py) $(obj)/constants.py
=======
always-y += constants.py
$(obj)/constants.py: $(src)/constants.py.in FORCE
	$(call if_changed_dep,gen_constants_py)

clean-files := *.pyc *.pyo
>>>>>>> upstream/android-13
