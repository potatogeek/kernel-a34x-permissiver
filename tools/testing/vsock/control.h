<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>

void control_init(const char *control_host, const char *control_port,
		  bool server);
void control_cleanup(void);
void control_writeln(const char *str);
char *control_readln(void);
void control_expectln(const char *str);
<<<<<<< HEAD
=======
bool control_cmpln(char *line, const char *str, bool fail);
>>>>>>> upstream/android-13

#endif /* CONTROL_H */
