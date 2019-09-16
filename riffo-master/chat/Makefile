All: server

server: server.c
	gcc -Wall -g -std=gnu99 -pthread -D_GNU_SOURCE server.c -o server -lrt

clean:
	rm server
