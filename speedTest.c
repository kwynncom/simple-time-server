#include <stdio.h> // perror
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include <strings.h> // bzero
#include <unistd.h> // read, write, close

#include "common.h"
#include "config.h"

#define INBUFMAX KW_STS_TIME_MAX_BUF_SZ

void doit();

int main()
{ 
    doit();
}

void doit() {
    int sock = getBoundSock(0, "127.0.0.1");

    const char smsg = 'r';
    int readr, writer;
    char *outfmt = "%ld\n";;
	const int iters = 20;
	unsigned long ts[iters];
	const int tssz = sizeof(unsigned long);
	int i;

    for (i=0; i < iters; i++) {
        writer = write(sock, &smsg , 1   );
		readr  = read (sock, &ts[i], tssz);
    }

    for (i=0; i < iters; i++)
		printf(outfmt, ts[i]);

    close(sock); 
}