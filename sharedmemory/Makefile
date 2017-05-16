CC = gcc
#CFLAGS = -g -Wall

all: shm-server shm-client

shm-server: shm-server.c
	$(CC) $(CFLAGS) $< -o $@

shm-client: shm-client.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f shm-server shm-client *~