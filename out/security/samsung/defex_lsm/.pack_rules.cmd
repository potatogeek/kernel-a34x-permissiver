cmd_security/samsung/defex_lsm/pack_rules := clang -Wp,-MD,security/samsung/defex_lsm/.pack_rules.d -Isecurity/samsung/defex_lsm -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89 -DDEFEX_PERMISSIVE_IM -DDEFEX_PERMISSIVE_INT -DDEFEX_PED_ENABLE -DDEFEX_SAFEPLACE_ENABLE -DDEFEX_INTEGRITY_ENABLE -DDEFEX_IMMUTABLE_ENABLE -DDEFEX_LP_ENABLE -DDEFEX_UMH_RESTRICTION_ENABLE -DDEFEX_CACHES_ENABLE -DDEFEX_DEPENDING_ON_OEMUNLOCK -DDEFEX_RAMDISK_ENABLE -DDEFEX_SIGN_ENABLE -DDEFEX_DSMS_ENABLE -D__visible_for_testing=static   -o security/samsung/defex_lsm/pack_rules ../security/samsung/defex_lsm/pack_rules.c   

source_security/samsung/defex_lsm/pack_rules := ../security/samsung/defex_lsm/pack_rules.c

deps_security/samsung/defex_lsm/pack_rules := \
  /usr/include/errno.h \
  /usr/include/features.h \
  /usr/include/features-time64.h \
  /usr/include/bits/wordsize.h \
  /usr/include/bits/timesize.h \
  /usr/include/stdc-predef.h \
  /usr/include/sys/cdefs.h \
  /usr/include/bits/long-double.h \
  /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-64.h \
  /usr/include/bits/errno.h \
  /usr/include/linux/errno.h \
  /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h \
  /home/gabrieln/scone/toolchain/lib64/clang/11.0.1/include/limits.h \
  /usr/include/limits.h \
  /usr/include/bits/libc-header-start.h \
  /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h \
  /usr/include/linux/limits.h \
  /usr/include/bits/pthread_stack_min-dynamic.h \
  /usr/include/bits/pthread_stack_min.h \
  /usr/include/bits/posix2_lim.h \
  /usr/include/stdio.h \
  /home/gabrieln/scone/toolchain/lib64/clang/11.0.1/include/stddef.h \
  /home/gabrieln/scone/toolchain/lib64/clang/11.0.1/include/stdarg.h \
  /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h \
  /usr/include/bits/time64.h \
  /usr/include/bits/types/__fpos_t.h \
  /usr/include/bits/types/__mbstate_t.h \
  /usr/include/bits/types/__fpos64_t.h \
  /usr/include/bits/types/__FILE.h \
  /usr/include/bits/types/FILE.h \
  /usr/include/bits/types/struct_FILE.h \
  /usr/include/bits/types/cookie_io_functions_t.h \
  /usr/include/bits/stdio_lim.h \
  /usr/include/bits/floatn.h \
  /usr/include/bits/floatn-common.h \
  /usr/include/bits/stdio.h \
  /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h \
  /usr/include/bits/waitstatus.h \
  /usr/include/sys/types.h \
  /usr/include/bits/types/clock_t.h \
  /usr/include/bits/types/clockid_t.h \
  /usr/include/bits/types/time_t.h \
  /usr/include/bits/types/timer_t.h \
  /usr/include/bits/stdint-intn.h \
  /usr/include/endian.h \
  /usr/include/bits/endian.h \
  /usr/include/bits/endianness.h \
  /usr/include/bits/byteswap.h \
  /usr/include/bits/uintn-identity.h \
  /usr/include/sys/select.h \
  /usr/include/bits/select.h \
  /usr/include/bits/types/sigset_t.h \
  /usr/include/bits/types/__sigset_t.h \
  /usr/include/bits/types/struct_timeval.h \
  /usr/include/bits/types/struct_timespec.h \
  /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/thread-shared-types.h \
  /usr/include/bits/pthreadtypes-arch.h \
  /usr/include/bits/atomic_wide_counter.h \
  /usr/include/bits/struct_mutex.h \
  /usr/include/bits/struct_rwlock.h \
  /usr/include/alloca.h \
  /usr/include/bits/stdlib-bsearch.h \
  /usr/include/bits/stdlib-float.h \
  /usr/include/string.h \
  /usr/include/bits/types/locale_t.h \
  /usr/include/bits/types/__locale_t.h \
  /usr/include/strings.h \
  /usr/include/sys/stat.h \
  /usr/include/bits/stat.h \
  /usr/include/bits/struct_stat.h \
  ../security/samsung/defex_lsm/include/defex_rules.h \

security/samsung/defex_lsm/pack_rules: $(deps_security/samsung/defex_lsm/pack_rules)

$(deps_security/samsung/defex_lsm/pack_rules):
