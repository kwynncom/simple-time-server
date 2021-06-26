server : server.c common.c
	gcc -O3 -static server.c common.c -o server.bin

client : client.c common.c
	gcc -O3 -static client.c common.c -o client.bin

speedTest : speedTest.c
	gcc -O3 -static speedTest.c common.c -o sp
