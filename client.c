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
    const int  smsgsize = sizeof(smsg);
    int readr, writer;
    long b, e, timer;
    timer = 0;
    char inbuf[INBUFMAX];
    char *outfmt;

    if (smsg == 'd') outfmt = "%ld\n%s%ld\n";
    else             outfmt = "%ld\n%ld\n\%ld\n";

    for (int i=0; i < 20; i++) {
        bzero(inbuf, INBUFMAX); 
        b = nanotime();
        writer = write(sock, &smsg, smsgsize);
        if (smsg == 'r') readr = read(sock, &timer, sizeof(timer));
        else             readr = read(sock, inbuf, INBUFMAX);
        e = nanotime();
        if (smsg == 'r') 
             printf(outfmt, b, timer, e);
        else printf(outfmt, b, inbuf, e);
    } 

    close(sock); 
}