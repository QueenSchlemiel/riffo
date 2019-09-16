ALL: server contacts

server: contacts_server.c
	gcc -O contacts_server.c `mysql_config --cflags --libs` -o server

contacts: contacts.c
	gcc contacts.c -o contacts

clean:
	rm server contacts
