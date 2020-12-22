.PHONY:clean all
CC=gcc
HEADERS=-I
DEBUG=-g -ggdb
WALL=-Wall -W
CFLAGS=$(WALL) $(DEBUG)
L_CC=$(CC) $(CFLAGS) $(HEADERS)

all:server client

server:server.c
	$(CC) $< -o $@
client:client.c
	$(CC) $< -o $@
	
clean:
	@-if [ -f server ]; then rm server; fi
	@-if [ -f client ]; then rm client; fi
