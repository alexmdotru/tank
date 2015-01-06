CC = gcc
CFLAGS = -c -Wall -I include -F /Library/Frameworks

framework.o: common/framework.c
	$(CC) $(CFLAGS) $< -o $@

client.o: client/client.c
	$(CC) $(CFLAGS) $< -o $@

server.o: server/server.c
	$(CC) $(CFLAGS) $< -o $@
