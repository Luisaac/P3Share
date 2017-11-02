CC=gcc
CFLAGS= -Wall -Werror -lrt -lpthread

all:shm_server shm_client
shm_server: shm_server.c
	$(CC) shm_server.c -o shm_server $(CFLAGS)
shm_client: shm_client.c
	$(CC) shm_client.c -o shm_client $(CFLAGS)
clean:
	rm -f shm_server
	rm -f shm_client

