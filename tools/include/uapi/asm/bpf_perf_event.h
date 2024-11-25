#if defined(__aarch64__)
#include "../../arch/arm64/include/uapi/asm/bpf_perf_event.h"
#elif defined(__s390__)
#include "../../arch/s390/include/uapi/asm/bpf_perf_event.h"
<<<<<<< HEAD
=======
#elif defined(__riscv)
#include "../../arch/riscv/include/uapi/asm/bpf_perf_event.h"
>>>>>>> upstream/android-13
#else
#include <uapi/asm-generic/bpf_perf_event.h>
#endif
