#include <stdio.h>
#include <unistd.h>    // fork
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> // exit
#include <string.h> // strcmp

#include "config.h"
#include "common.h"

void sts_final_loop_tcp(int cfd) {

	const int     obsz = KW_STS_TIME_MAX_BUF_SZ;
	unsigned char ob[obsz];
	unsigned char ib;
	unsigned long t;
	const int tsz = sizeof(t);
	int wr;
	const int rcsz = 1;

	if (tsz != 8) { printf("long is not 8 bytes - might lead to buffer overflow"); exit(8125); }

	while (1) {
		if (read(cfd, &ib, rcsz) != rcsz) {
			perror("read error.  Exiting...");
			exit(8128);
		}
		t = nanotime();
		if (ib == 'r') wr = write(cfd, &t, tsz); 
		else {
			sprintf(ob, "%ld\n", t);
			wr = write(cfd, ob, obsz);
		}
	}
}

void sts_final_loop(int sock, int isTCP) {

	const int     obsz = KW_STS_TIME_MAX_BUF_SZ;
	unsigned char ob[obsz];
	unsigned char ib;
	unsigned long t;
	const int tsz = sizeof(t);

    struct sockaddr_in6  addr;
	int addrsz = sizeof(addr);
	const int rcsz = 1;
	int connfd;
	int tempsock;
	int rr;

	if (tsz != 8) { printf("long is not 8 bytes - might lead to buffer overflow"); exit(8126); }

	while(1) {

		tempsock = sock;

		if (isTCP) {
			connfd = accept(sock, (struct sockaddr  *)&addr, &addrsz);
			if (connfd < 0) { printf("server acccept failed...\n"); exit(1);   }
			tempsock = connfd;
		}
		
		rr = recvfrom(tempsock, &ib, rcsz, 0 /* flags */, ( struct sockaddr *) &addr, &addrsz); 

		if (rr != rcsz) {
			printf("recvfrom error.  Exiting...");
			exit(8128);
		}
		t = nanotime();
		if (ib == 'r') sendto(tempsock, &t, tsz, 0, (const struct sockaddr *) &addr, addrsz); 
		else {
			sprintf(ob, "%ld\n", t);
			sendto(tempsock, ob, obsz, 0, (const struct sockaddr *) &addr, addrsz); 
		}     

		if (isTCP) close(tempsock);
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

// 	checkUID(); // do this before the fork

    int fpid  = fork();
    char *prots;
    if (fpid) prots = "tcp";
    else      prots = "udp";

    int isTCP = !strcmp(prots, "tcp");

    int sock = getBoundSock(isTCP);
	sts_final_loop(sock, isTCP);

} 

void main(void) {
	sts_server();
}