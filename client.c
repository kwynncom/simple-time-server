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
    int sock = getBoundSock(0, "::1"); // for IPv4 addresses, need to convert to IPv6 like I did in my SNTP client

    const char smsg = 'a';
    const int  smsgsize = sizeof(smsg);
    int readr, writer;
    long b, e, timer;
	int tsz = sizeof(timer);
    timer = 0;
    char inbuf[INBUFMAX];
    // const char *outfmt = "%ld\n%s%ld\n";
	const char *outfmt = "%s";

    for (int i=0; i < 1000000; i++) {
        bzero(inbuf, INBUFMAX); 
        // b = nanotime();
        writer = write(sock, &smsg, smsgsize);
        if (smsg == 'r') readr = read(sock, &timer, tsz);
        else             readr = read(sock, inbuf, INBUFMAX);
        // e = nanotime();
        if (smsg == 'r') 
             fwrite(&timer, tsz, 1, stdout); // just server time
        else printf(outfmt, inbuf);
    } 

    close(sock); 
}