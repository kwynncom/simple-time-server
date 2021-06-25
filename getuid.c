#include <unistd.h>
#include <stdio.h>

void main(void) {
	printf("%d %d %d\n", getuid(), geteuid(), getgid());
}
