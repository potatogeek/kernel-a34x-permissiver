<<<<<<< HEAD
#!/usr/bin/python

from os import getenv
from subprocess import Popen, PIPE
from re import sub

def clang_has_option(option):
    return [o for o in Popen(['clang', option], stderr=PIPE).stderr.readlines() if "unknown argument" in o] == [ ]

cc = getenv("CC")
if cc == "clang":
    from _sysconfigdata import build_time_vars
    build_time_vars["CFLAGS"] = sub("-specs=[^ ]+", "", build_time_vars["CFLAGS"])
    if not clang_has_option("-mcet"):
        build_time_vars["CFLAGS"] = sub("-mcet", "", build_time_vars["CFLAGS"])
    if not clang_has_option("-fcf-protection"):
        build_time_vars["CFLAGS"] = sub("-fcf-protection", "", build_time_vars["CFLAGS"])
=======
from os import getenv, path
from subprocess import Popen, PIPE
from re import sub

cc = getenv("CC")
cc_is_clang = b"clang version" in Popen([cc.split()[0], "-v"], stderr=PIPE).stderr.readline()
src_feature_tests  = getenv('srctree') + '/tools/build/feature'

def clang_has_option(option):
    cc_output = Popen([cc, option, path.join(src_feature_tests, "test-hello.c") ], stderr=PIPE).stderr.readlines()
    return [o for o in cc_output if ((b"unknown argument" in o) or (b"is not supported" in o))] == [ ]

if cc_is_clang:
    from distutils.sysconfig import get_config_vars
    vars = get_config_vars()
    for var in ('CFLAGS', 'OPT'):
        vars[var] = sub("-specs=[^ ]+", "", vars[var])
        if not clang_has_option("-mcet"):
            vars[var] = sub("-mcet", "", vars[var])
        if not clang_has_option("-fcf-protection"):
            vars[var] = sub("-fcf-protection", "", vars[var])
        if not clang_has_option("-fstack-clash-protection"):
            vars[var] = sub("-fstack-clash-protection", "", vars[var])
        if not clang_has_option("-fstack-protector-strong"):
            vars[var] = sub("-fstack-protector-strong", "", vars[var])
        if not clang_has_option("-fno-semantic-interposition"):
            vars[var] = sub("-fno-semantic-interposition", "", vars[var])
        if not clang_has_option("-ffat-lto-objects"):
            vars[var] = sub("-ffat-lto-objects", "", vars[var])
>>>>>>> upstream/android-13

from distutils.core import setup, Extension

from distutils.command.build_ext   import build_ext   as _build_ext
from distutils.command.install_lib import install_lib as _install_lib

class build_ext(_build_ext):
    def finalize_options(self):
        _build_ext.finalize_options(self)
        self.build_lib  = build_lib
        self.build_temp = build_tmp

class install_lib(_install_lib):
    def finalize_options(self):
        _install_lib.finalize_options(self)
        self.build_dir = build_lib


cflags = getenv('CFLAGS', '').split()
# switch off several checks (need to be at the end of cflags list)
<<<<<<< HEAD
cflags += ['-fno-strict-aliasing', '-Wno-write-strings', '-Wno-unused-parameter', '-Wno-redundant-decls' ]
if cc != "clang":
=======
cflags += ['-fno-strict-aliasing', '-Wno-write-strings', '-Wno-unused-parameter', '-Wno-redundant-decls', '-DPYTHON_PERF' ]
if not cc_is_clang:
>>>>>>> upstream/android-13
    cflags += ['-Wno-cast-function-type' ]

src_perf  = getenv('srctree') + '/tools/perf'
build_lib = getenv('PYTHON_EXTBUILD_LIB')
build_tmp = getenv('PYTHON_EXTBUILD_TMP')
libtraceevent = getenv('LIBTRACEEVENT')
libapikfs = getenv('LIBAPI')
<<<<<<< HEAD
=======
libperf = getenv('LIBPERF')
>>>>>>> upstream/android-13

ext_sources = [f.strip() for f in open('util/python-ext-sources')
				if len(f.strip()) > 0 and f[0] != '#']

# use full paths with source files
ext_sources = list(map(lambda x: '%s/%s' % (src_perf, x) , ext_sources))

<<<<<<< HEAD
perf = Extension('perf',
		  sources = ext_sources,
		  include_dirs = ['util/include'],
		  extra_compile_args = cflags,
		  extra_objects = [libtraceevent, libapikfs],
=======
extra_libraries = []
if '-DHAVE_LIBNUMA_SUPPORT' in cflags:
    extra_libraries = [ 'numa' ]
if '-DHAVE_LIBCAP_SUPPORT' in cflags:
    extra_libraries += [ 'cap' ]

perf = Extension('perf',
		  sources = ext_sources,
		  include_dirs = ['util/include'],
		  libraries = extra_libraries,
		  extra_compile_args = cflags,
		  extra_objects = [libtraceevent, libapikfs, libperf],
>>>>>>> upstream/android-13
                 )

setup(name='perf',
      version='0.1',
      description='Interface with the Linux profiling infrastructure',
      author='Arnaldo Carvalho de Melo',
      author_email='acme@redhat.com',
      license='GPLv2',
      url='http://perf.wiki.kernel.org',
      ext_modules=[perf],
      cmdclass={'build_ext': build_ext, 'install_lib': install_lib})
