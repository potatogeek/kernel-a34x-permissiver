#include <asm/ucontext.h>

struct sigframe {
	struct ucontext uc;
	unsigned long retcode[4];
};

struct rt_sigframe {
	struct siginfo info;
	struct sigframe sig;
};
<<<<<<< HEAD
=======

extern struct page *get_signal_page(void);
>>>>>>> upstream/android-13
