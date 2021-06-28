serverdebug : server.c common.c utils.c
	gcc -g server.c common.c utils.c -o serverdebug.bin

server : server.c common.c utils.c
	gcc -O3 -static server.c common.c utils.c -o simptime

client : client.c common.c
	gcc -O3 -static client.c common.c -o client.bin

speedTest : speedTest.c
	gcc -O3 -static speedTest.c common.c -o sp
