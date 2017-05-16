CC = gcc
#CFLAGS = -g -Wall

all: sem-server sem-client

sem-server: sem-server.c
	$(CC) $(CFLAGS) $< -o $@

sem-client: sem-client.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f sem-server sem-client *~