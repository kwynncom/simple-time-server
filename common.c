#include <time.h>
#include <stdlib.h> // exit
#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <strings.h> // bzero
#include <string.h> // strcmp
    
#include "config.h"
    
int getBoundSock(int isTCP, char *outaddr) {
    struct sockaddr_in saddr;
    int sock, type, prot;
    int isoutaddr = strcmp("", outaddr);

    if (!isTCP) { type = SOCK_DGRAM ; prot = 17; }
    else        { type = SOCK_STREAM; prot =  6; }

    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET; 
    if (!isoutaddr) saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else          saddr.sin_addr.s_addr = inet_addr(outaddr); 
    saddr.sin_port = htons(KW_STS_PORT);

    if ((sock = socket(AF_INET, type, prot)) < 0) { perror("socket creation failed"); exit(EXIT_FAILURE); }

    if (!isTCP) {
        struct timeval timeout;      
        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;

        if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) perror("setsockopt failed\n");
    }
    
    if (!isoutaddr) {
	if (bind(sock, (const struct sockaddr *)&saddr, sizeof(saddr)) < 0) { perror("bind failed"); exit(EXIT_FAILURE); }
	if (isTCP) if ((listen(sock, TCP_CONN_BACKLOG)) != 0) { printf("Listen failed...\n"); exit(EXIT_FAILURE);  } 
    } else  if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {  printf("connection with the server failed...\n"); exit(EXIT_FAILURE); } 
        
    return sock;
}

long nanotime() {
    struct timespec sts;
    if (clock_gettime(CLOCK_REALTIME, &sts) !=0) return 0;
    return sts.tv_sec * 1000000000 + sts.tv_nsec;
}
