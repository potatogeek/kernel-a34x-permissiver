#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 256

<<<<<<< HEAD
int main(int argc, char *argv[])
{
	int fd = open("/dev/urandom", O_RDONLY);
	int i;
	char buf[BUF_SIZE];
=======
static __attribute__((noinline))
void urandom_read(int fd, int count)
{
       char buf[BUF_SIZE];
       int i;

       for (i = 0; i < count; ++i)
               read(fd, buf, BUF_SIZE);
}

int main(int argc, char *argv[])
{
	int fd = open("/dev/urandom", O_RDONLY);
>>>>>>> upstream/android-13
	int count = 4;

	if (fd < 0)
		return 1;

	if (argc == 2)
		count = atoi(argv[1]);

<<<<<<< HEAD
	for (i = 0; i < count; ++i)
		read(fd, buf, BUF_SIZE);
=======
	urandom_read(fd, count);
>>>>>>> upstream/android-13

	close(fd);
	return 0;
}
