#include <unistd.h>
#include <stdio.h>

void main(void) {

	__uid_t (*fs[1])() = {&getuid};   

	printf("%d %d %d\n", getuid(), geteuid(), getgid());
}
