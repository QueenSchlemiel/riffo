ALL: server contacts

server: flat-file-db-server.c
	gcc flat-file-db-server.c -o fileserver

contacts: contacts.c
	gcc contacts.c -o contacts

clean:
	rm fileserver contacts
