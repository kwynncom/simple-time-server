#include <stdio.h>
#include <unistd.h>    // fork
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> // exit
#include <string.h> // strcmp

#include "config.h"
#include "common.h"

void sts_loop_20(int chfd, int isTCP, int parfd);

void sts_loop_10(int sock, int isTCP) {

	if (!isTCP) {
		sts_loop_20(sock, isTCP, sock);
		return;
	}

	int tcpfd;

	while (1) {
		tcpfd = accept(sock, NULL, NULL);
		if (tcpfd < 0) { perror("server acccept failed...\n"); exit(8130);   }
		if (fork()) close(tcpfd);
		else  sts_loop_20(tcpfd, isTCP, sock);
	}
}

void sts_loop_20(int chfd, int isTCP, int parfd) { // only purpose of parfd is if we want to close

	const    int     obsz = KW_STS_TIME_MAX_BUF_SZ;
	unsigned char ob[obsz];
	unsigned char ib;
	const int     ibsz = 1;
	unsigned long		   t;
	const int tsz = sizeof(t);

    struct sockaddr_in6  addr;
	int addrsz =  sizeof(addr);

	int rr;

	void *obsw;
	int   obswsz;

	if (tsz != 8) { printf("long is not 8 bytes - might lead to buffer overflow"); exit(8126); }

	while (1) {

		bzero(ob, obsz);

		rr = recvfrom(chfd, &ib, ibsz, 0, ( struct sockaddr *) &addr, &addrsz);  // 4th arg is flags

		if (rr != ibsz) {
			if (isTCP) {
				close( chfd);
				close(parfd);
				exit(0);
			}
			printf("recvfrom error.  Exiting...");
			exit(8128);
		}

		t = nanotime();

		if (ib == 'r') { obsw = &t; obswsz = tsz; }
		else           {
			sprintf(ob, "%ld\n", t);
			obsw   = ob;
			obswsz = obsz;
		}

		sendto(chfd, obsw, obswsz, 0, (const struct sockaddr *) &addr, addrsz);
	} 
}

void checkUID() {

	const int fsc = 3;
	__uid_t (*fs[/* 3 - cannot init me */])() = {&getuid, &geteuid, &getgid};

	for(int i=0; i < fsc; i++) 
		if (fs[i]() != NOBODY_NOGROUP_ID) { 
			printf("I am somebody but I should not be somebody.\n"); 
			exit(8126); 
		}
}

void sts_server(void) {

 	checkUID(); // do this before the fork

    int fpid  = fork();
    char *prots;
    if (fpid) prots = "tcp";
    else      prots = "udp";

    int isTCP = !strcmp(prots, "tcp");

    int sock = getBoundSock(isTCP);
	sts_loop_10(sock, isTCP);

} 

void main(void) {
	sts_server();
}