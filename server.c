#include <stdio.h>
#include <unistd.h>    // fork
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include<sys/wait.h> // SIGCHLD

#include "config.h"
#include "common.h"
#include "utils.h"

void sts_server(void);
void sts_loop_10(int sock, int isTCP);
void sts_loop_20(int chfd, int isTCP, int parfd);

void main(void) { sts_server(); }

void sts_server(void) {
 	nobodyOrDie(); // do this before the fork
    int fpid  = fork();
	int isTCP = fpid;
    int sock = getBoundSock(isTCP);
	sts_loop_10(sock, isTCP);
} 

void sts_loop_10(int sock, int isTCP) {

	if (!isTCP) { sts_loop_20(sock, isTCP, sock); return; }

	int tcpfd;
	while (1) {
		tcpfd = accept(sock, NULL, NULL);
		if (tcpfd < 0) { perror("server acccept failed...\n"); exit(8130);   }
		if (fork()) {
			signal(SIGCHLD, SIG_IGN); // prevent zombies.  See notes below. 
			close(tcpfd);
		}
		else  sts_loop_20(tcpfd, isTCP, sock);
	}
}

void sts_loop_20(int chfd, int isTCP, int parfd) { // only purpose of parfd is if we want to close

	const    int     obsz = KW_STS_TIME_MAX_BUF_SZ;
	unsigned char ob[obsz];
	unsigned char ib;
	const int     ibsz = 1;
	uint64_t		       t;
	const int tsz = sizeof(t);

    struct sockaddr_in6  addr;
	int addrsz =  sizeof(addr);

	int rr;

	void *obsw;
	int   obswsz;

	while (1) {

		bzero(ob, obsz); // I am nearly certain that this is necessary or weird things happen in telnet.

		rr = recvfrom(chfd, &ib, ibsz, 0, ( struct sockaddr *) &addr, &addrsz);  // 4th arg is flags

		if (rr != ibsz) sts_exit(isTCP, chfd, parfd, "recvfrom error - exiting...");

		t = nanotime();

		if (ib == 'r') { obsw = &t; obswsz = tsz ;							  }
		else           { obsw = ob; obswsz = obsz;   sprintf(ob, "%ld\n", t); }

		sendto(chfd, obsw, obswsz, 0, (const struct sockaddr *) &addr, addrsz);
	} 
}

// ID zombies with 
// ps -elf | grep Z
// test for zombies with 
// echo -n d | nc  -W 1    -4 localhost 8123
