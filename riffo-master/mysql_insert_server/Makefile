ALL: server contacts test

server: mysql_insert_server.c
	gcc -O mysql_insert_server.c `mysql_config --cflags --libs` -o server

contacts: contacts.c
	gcc contacts.c -o contacts

test: testDB.c
	gcc -O testDB.c `mysql_config --cflags --libs` -o test

clean:
	rm server contacts
