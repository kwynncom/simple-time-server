server : server.c common.c
	gcc -O3 server.c common.c -o server.bin

client : client.c common.c
	gcc -O3 client.c common.c -o client.bin
