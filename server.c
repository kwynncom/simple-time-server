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
	int wr, rr;

	if (tsz != 8) { printf("long is not 8 bytes - might lead to buffer overflow"); exit(8125); }

	while (1) {
		rr = read(cfd, &ib, 1);
		t = nanotime();
		if (ib == 'r') wr = write(cfd, &t, tsz); 
		else {
			sprintf(ob, "%ld\n", t);
			wr = write(cfd, ob, obsz);
		}
	}
}

void sts_final_loop_udp(int sock, const struct sockaddr_in addr) {

	const int     obsz = KW_STS_TIME_MAX_BUF_SZ;
	unsigned char ob[obsz];
	unsigned char ib;
	unsigned long t;
	const int tsz = sizeof(t);
	int addrsz = sizeof(addr);

	if (tsz != 8) { printf("long is not 8 bytes - might lead to buffer overflow"); exit(8126); }

	while(1) {
		recvfrom(sock, &ib, 1, 0 /* flags */, ( struct sockaddr *) &addr, &addrsz); 
		t = nanotime();
		if (ib == 'r') sendto(sock, &t, tsz, 0, (const struct sockaddr *) &addr, addrsz); 
		else {
			sprintf(ob, "%ld\n", t);
			sendto(sock, ob, obsz, 0, (const struct sockaddr *) &addr, addrsz); 
		}         
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
    struct sockaddr_in6  caddr; // only UDP uses
    int caddrsz = sizeof(caddr); // same
    char inbuf[3];
    int inbufsz = sizeof(inbuf);
    int outbufssz = 30;
    char outbufs[outbufssz];
    int readr, connfd, writer;
    long t;
    int sizet = sizeof(t);
 
	if (sizet != 8) { printf("long is not 8 bytes - might lead to buffer overflow"); exit(8126); }

    if (isTCP) {
        while(1) {
            if ((connfd = accept(sock, (struct sockaddr  *)&caddr, &caddrsz)) < 0) { printf("server acccept failed...\n"); exit(1);   } 
            if (!fork()) {
                close(sock); // child process does not need socket
				sts_final_loop_tcp(connfd);
                close(connfd);
            } // if !fork
            else close(connfd);
        } // while outer
    } // TCP
    else {
        do {
            recvfrom(sock, &inbuf, inbufsz, 0 /* flags */, ( struct sockaddr *) &caddr, &caddrsz); 
            t = nanotime();
            if (inbuf[0] == 'r') sendto(sock, &t, sizet, 0, (const struct sockaddr *) &caddr, caddrsz); 
            else {
                sprintf(outbufs, "%ld\n", t);
                writer = sendto(sock, outbufs, strlen(outbufs), 0, (const struct sockaddr *) &caddr, caddrsz); 
            }         
        } while (1);
    }

    close(sock);
}

void main(void) {
	sts_server();
}