#include <stdio.h>
#include <linux/fs.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int fd = open("/dev/upperReverse", O_RDWR);
	write(fd, argv[1], strlen(argv[1]));
	read(fd, argv[1], strlen(argv[1]));
	printf("Read: %s\n", argv[1]);
}
