# -*- makefile -*-
# Makefile for Sphinx documentation
#

<<<<<<< HEAD
subdir-y :=
=======
# for cleaning
subdir- := devicetree/bindings

# Check for broken documentation file references
ifeq ($(CONFIG_WARN_MISSING_DOCUMENTS),y)
$(shell $(srctree)/scripts/documentation-file-ref-check --warn)
endif

# Check for broken ABI files
ifeq ($(CONFIG_WARN_ABI_ERRORS),y)
$(shell $(srctree)/scripts/get_abi.pl validate --dir $(srctree)/Documentation/ABI)
endif
>>>>>>> upstream/android-13

# You can set these variables from the command line.
SPHINXBUILD   = sphinx-build
SPHINXOPTS    =
SPHINXDIRS    = .
<<<<<<< HEAD
_SPHINXDIRS   = $(patsubst $(srctree)/Documentation/%/conf.py,%,$(wildcard $(srctree)/Documentation/*/conf.py))
=======
_SPHINXDIRS   = $(sort $(patsubst $(srctree)/Documentation/%/index.rst,%,$(wildcard $(srctree)/Documentation/*/index.rst)))
>>>>>>> upstream/android-13
SPHINX_CONF   = conf.py
PAPER         =
BUILDDIR      = $(obj)/output
PDFLATEX      = xelatex
LATEXOPTS     = -interaction=batchmode

<<<<<<< HEAD
=======
ifeq ($(KBUILD_VERBOSE),0)
SPHINXOPTS    += "-q"
endif

>>>>>>> upstream/android-13
# User-friendly check for sphinx-build
HAVE_SPHINX := $(shell if which $(SPHINXBUILD) >/dev/null 2>&1; then echo 1; else echo 0; fi)

ifeq ($(HAVE_SPHINX),0)

.DEFAULT:
	$(warning The '$(SPHINXBUILD)' command was not found. Make sure you have Sphinx installed and in PATH, or set the SPHINXBUILD make variable to point to the full path of the '$(SPHINXBUILD)' executable.)
	@echo
<<<<<<< HEAD
	@./scripts/sphinx-pre-install
=======
	@$(srctree)/scripts/sphinx-pre-install
>>>>>>> upstream/android-13
	@echo "  SKIP    Sphinx $@ target."

else # HAVE_SPHINX

<<<<<<< HEAD
# User-friendly check for pdflatex
HAVE_PDFLATEX := $(shell if which $(PDFLATEX) >/dev/null 2>&1; then echo 1; else echo 0; fi)
=======
# User-friendly check for pdflatex and latexmk
HAVE_PDFLATEX := $(shell if which $(PDFLATEX) >/dev/null 2>&1; then echo 1; else echo 0; fi)
HAVE_LATEXMK := $(shell if which latexmk >/dev/null 2>&1; then echo 1; else echo 0; fi)

ifeq ($(HAVE_LATEXMK),1)
	PDFLATEX := latexmk -$(PDFLATEX)
endif #HAVE_LATEXMK
>>>>>>> upstream/android-13

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
KERNELDOC       = $(srctree)/scripts/kernel-doc
KERNELDOC_CONF  = -D kerneldoc_srctree=$(srctree) -D kerneldoc_bin=$(KERNELDOC)
ALLSPHINXOPTS   =  $(KERNELDOC_CONF) $(PAPEROPT_$(PAPER)) $(SPHINXOPTS)
# the i18n builder cannot share the environment and doctrees with the others
I18NSPHINXOPTS  = $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) .

# commands; the 'cmd' from scripts/Kbuild.include is not *loopable*
loop_cmd = $(echo-cmd) $(cmd_$(1)) || exit;

# $2 sphinx builder e.g. "html"
<<<<<<< HEAD
# $3 name of the build subfolder / e.g. "media", used as:
#    * dest folder relative to $(BUILDDIR) and
#    * cache folder relative to $(BUILDDIR)/.doctrees
# $4 dest subfolder e.g. "man" for man pages at media/man
# $5 reST source folder relative to $(srctree)/$(src),
#    e.g. "media" for the linux-tv book-set at ./Documentation/media

quiet_cmd_sphinx = SPHINX  $@ --> file://$(abspath $(BUILDDIR)/$3/$4)
      cmd_sphinx = $(MAKE) BUILDDIR=$(abspath $(BUILDDIR)) $(build)=Documentation/media $2 && \
	PYTHONDONTWRITEBYTECODE=1 \
	BUILDDIR=$(abspath $(BUILDDIR)) SPHINX_CONF=$(abspath $(srctree)/$(src)/$5/$(SPHINX_CONF)) \
=======
# $3 name of the build subfolder / e.g. "userspace-api/media", used as:
#    * dest folder relative to $(BUILDDIR) and
#    * cache folder relative to $(BUILDDIR)/.doctrees
# $4 dest subfolder e.g. "man" for man pages at userspace-api/media/man
# $5 reST source folder relative to $(srctree)/$(src),
#    e.g. "userspace-api/media" for the linux-tv book-set at ./Documentation/userspace-api/media

quiet_cmd_sphinx = SPHINX  $@ --> file://$(abspath $(BUILDDIR)/$3/$4)
      cmd_sphinx = $(MAKE) BUILDDIR=$(abspath $(BUILDDIR)) $(build)=Documentation/userspace-api/media $2 && \
	PYTHONDONTWRITEBYTECODE=1 \
	BUILDDIR=$(abspath $(BUILDDIR)) SPHINX_CONF=$(abspath $(srctree)/$(src)/$5/$(SPHINX_CONF)) \
	$(PYTHON3) $(srctree)/scripts/jobserver-exec \
	$(CONFIG_SHELL) $(srctree)/Documentation/sphinx/parallel-wrapper.sh \
>>>>>>> upstream/android-13
	$(SPHINXBUILD) \
	-b $2 \
	-c $(abspath $(srctree)/$(src)) \
	-d $(abspath $(BUILDDIR)/.doctrees/$3) \
	-D version=$(KERNELVERSION) -D release=$(KERNELRELEASE) \
	$(ALLSPHINXOPTS) \
	$(abspath $(srctree)/$(src)/$5) \
	$(abspath $(BUILDDIR)/$3/$4)

htmldocs:
<<<<<<< HEAD
=======
	@$(srctree)/scripts/sphinx-pre-install --version-check
>>>>>>> upstream/android-13
	@+$(foreach var,$(SPHINXDIRS),$(call loop_cmd,sphinx,html,$(var),,$(var)))

linkcheckdocs:
	@$(foreach var,$(SPHINXDIRS),$(call loop_cmd,sphinx,linkcheck,$(var),,$(var)))

latexdocs:
<<<<<<< HEAD
=======
	@$(srctree)/scripts/sphinx-pre-install --version-check
>>>>>>> upstream/android-13
	@+$(foreach var,$(SPHINXDIRS),$(call loop_cmd,sphinx,latex,$(var),latex,$(var)))

ifeq ($(HAVE_PDFLATEX),0)

pdfdocs:
	$(warning The '$(PDFLATEX)' command was not found. Make sure you have it installed and in PATH to produce PDF output.)
	@echo "  SKIP    Sphinx $@ target."

else # HAVE_PDFLATEX

pdfdocs: latexdocs
<<<<<<< HEAD
	$(foreach var,$(SPHINXDIRS), $(MAKE) PDFLATEX=$(PDFLATEX) LATEXOPTS="$(LATEXOPTS)" -C $(BUILDDIR)/$(var)/latex || exit;)
=======
	@$(srctree)/scripts/sphinx-pre-install --version-check
	$(foreach var,$(SPHINXDIRS), \
	   $(MAKE) PDFLATEX="$(PDFLATEX)" LATEXOPTS="$(LATEXOPTS)" -C $(BUILDDIR)/$(var)/latex || exit; \
	   mkdir -p $(BUILDDIR)/$(var)/pdf; \
	   mv $(subst .tex,.pdf,$(wildcard $(BUILDDIR)/$(var)/latex/*.tex)) $(BUILDDIR)/$(var)/pdf/; \
	)
>>>>>>> upstream/android-13

endif # HAVE_PDFLATEX

epubdocs:
<<<<<<< HEAD
	@+$(foreach var,$(SPHINXDIRS),$(call loop_cmd,sphinx,epub,$(var),epub,$(var)))

xmldocs:
=======
	@$(srctree)/scripts/sphinx-pre-install --version-check
	@+$(foreach var,$(SPHINXDIRS),$(call loop_cmd,sphinx,epub,$(var),epub,$(var)))

xmldocs:
	@$(srctree)/scripts/sphinx-pre-install --version-check
>>>>>>> upstream/android-13
	@+$(foreach var,$(SPHINXDIRS),$(call loop_cmd,sphinx,xml,$(var),xml,$(var)))

endif # HAVE_SPHINX

# The following targets are independent of HAVE_SPHINX, and the rules should
# work or silently pass without Sphinx.

refcheckdocs:
	$(Q)cd $(srctree);scripts/documentation-file-ref-check

cleandocs:
	$(Q)rm -rf $(BUILDDIR)
<<<<<<< HEAD
	$(Q)$(MAKE) BUILDDIR=$(abspath $(BUILDDIR)) $(build)=Documentation/media clean
=======
	$(Q)$(MAKE) BUILDDIR=$(abspath $(BUILDDIR)) $(build)=Documentation/userspace-api/media clean
>>>>>>> upstream/android-13

dochelp:
	@echo  ' Linux kernel internal documentation in different formats from ReST:'
	@echo  '  htmldocs        - HTML'
	@echo  '  latexdocs       - LaTeX'
	@echo  '  pdfdocs         - PDF'
	@echo  '  epubdocs        - EPUB'
	@echo  '  xmldocs         - XML'
<<<<<<< HEAD
	@echo  '  linkcheckdocs   - check for broken external links (will connect to external hosts)'
	@echo  '  refcheckdocs    - check for references to non-existing files under Documentation'
=======
	@echo  '  linkcheckdocs   - check for broken external links'
	@echo  '                    (will connect to external hosts)'
	@echo  '  refcheckdocs    - check for references to non-existing files under'
	@echo  '                    Documentation'
>>>>>>> upstream/android-13
	@echo  '  cleandocs       - clean all generated files'
	@echo
	@echo  '  make SPHINXDIRS="s1 s2" [target] Generate only docs of folder s1, s2'
	@echo  '  valid values for SPHINXDIRS are: $(_SPHINXDIRS)'
	@echo
	@echo  '  make SPHINX_CONF={conf-file} [target] use *additional* sphinx-build'
	@echo  '  configuration. This is e.g. useful to build with nit-picking config.'
	@echo
	@echo  '  Default location for the generated documents is Documentation/output'
