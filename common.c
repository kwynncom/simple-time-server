#include <time.h>
#include <stdlib.h> // exit
#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <strings.h> // bzero
#include <string.h> // strcmp
    
#include "config.h"
    
int getBoundSock(int isTCP, char *myinaddr) {
    
	struct sockaddr_in6 saddr;
    int sock, sockType;
    int isinaddr = strcmp("", myinaddr);

    if (!isTCP) sockType = SOCK_DGRAM ; 
    else        sockType = SOCK_STREAM; 

    saddr.sin6_family = AF_INET6;

	if (!isinaddr) saddr.sin6_addr = in6addr_any;

// 	if  (inet_pton(AF_INET6, addrStr , &addro.sin6_addr) != 1) { fprintf(stderr, "bad IP address\n"); exit(EXIT_FAILURE); }
	else if (inet_pton(AF_INET6, myinaddr, &saddr.sin6_addr) != 1)  { fprintf(stderr, "bad IP address\n"); exit(EXIT_FAILURE); }
//	else           saddr.sin6_addr = inet_addr(myinaddr);

    saddr.sin6_port = htons(KW_STS_PORT);

    if ((sock = socket(AF_INET6, sockType, 0)) < 0) { perror("socket creation failed"); exit(EXIT_FAILURE); }

	if (!isinaddr) {
		if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) { perror("bind failed"); exit(EXIT_FAILURE); }
		if (isTCP) if ((listen(sock, TCP_CONN_BACKLOG)) != 0) { perror("Listen failed...\n"); exit(EXIT_FAILURE);  } 
	} else  if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {  printf("connection with the server failed...\n"); exit(EXIT_FAILURE); } 

    return sock;
}

uint64_t nanotime() {
    struct timespec sts;
    if (clock_gettime(CLOCK_REALTIME, &sts) != 0) return 0;
    return sts.tv_sec * 1000000000 + sts.tv_nsec;
}

char *getAddr(int argc, char **argv) { // 2021/08/28 11:50pm

	int argl;
	char *arg;

	if (argc < 2) arg = KW_STS_DEFAULT_SERVER_IP4;
	else		  arg = argv[1];

	argl = strlen(arg);
	
	if (argl < 7 || argl > 39) // "1.2.3.4" is 7 chars; IPv6 max 39 chars
		{ fprintf(stderr, "bad IP length of %d\n", argl); exit(EXIT_FAILURE);}

	if (strstr(arg, ".") == NULL) return arg;

	if (argl > 15) { fprintf(stderr, "bad IPv4 address - too long\n"); exit(EXIT_FAILURE);}

	const char *ip46p = "::FFFF:";
	const int bsz = strlen(ip46p) + 15 + 1;
	char *sbuf = (char *)malloc(bsz);
	sbuf = strcat(sbuf, ip46p);
	sbuf = strcat(sbuf, arg);

	return sbuf;
}
