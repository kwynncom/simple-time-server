#include <stdlib.h> // exit
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "config.h"

void nobodyOrDie() {

	const int fsc = 3;						//         1         2        3
	__uid_t (*fs[/* 3 - cannot init me */])() = {&getuid, &geteuid, &getgid};

	for(int i=0; i < fsc; i++) 
		if (fs[i]() != NOBODY_NOGROUP_ID) { 
			printf("only the nobody, nobody (effective), and nogroup should run this\n"); 
			exit(1000); 
		}
}

void sts_exit(int isTCP, int chfd, int parfd, const char *errMsg) {

	close( chfd);
	close(parfd);

	if (isTCP) exit(0);

	perror(errMsg);
	exit(8129);
}
