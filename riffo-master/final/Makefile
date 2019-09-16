ALL: client

client: client.c
	gcc -g -std=gnu99 -pthread -D_GNU_SOURCE client.c -o client -lrt

clean:
	rm client
