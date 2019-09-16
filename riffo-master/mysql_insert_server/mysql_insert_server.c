#include <mysql/mysql.h>
#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <errno.h>

#define QLEN 128

#ifndef HOST_NAME_MAX
	#define HOST_NAME_MAX 256
#endif

extern int errno;
int host_fd;

void print_ip_addresses(struct addrinfo *ai);
void print_error(char *);
void my_handler();

int main(int argc, char *argv[])
{
  MYSQL *con = mysql_init(NULL);
  char *hostname = "rds-mysql-test.cbrlhmklwlid.us-east-2.rds.amazonaws.com";
  char *username = "QueenSchlemiel";
  char *password = "gcsWarrior11";
  char *schema = "contacts";

  signal(SIGINT, my_handler);
  char *host_name;
  if(argc == 2){ 
	host_name = argv[1];
  }
  else {
	host_name = malloc(HOST_NAME_MAX);
	memset(host_name, 0, HOST_NAME_MAX);
	if(gethostname(host_name, HOST_NAME_MAX) < 0){
		print_error("gethostname error");
		return(-1);
	}
  }

  printf("host name: %s\n", host_name);

  struct addrinfo *host_ai;
  struct addrinfo hint;

  hint.ai_flags = 0;
  hint.ai_family = 0;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;
  hint.ai_addrlen = 0;
  hint.ai_canonname = NULL;
  hint.ai_addr = NULL;
  hint.ai_next = NULL;

  if((getaddrinfo(host_name, "contacts", &hint, &host_ai)) != 0){
	print_error("getaddrinfo error");
	exit(1);
  }

  print_ip_addresses(host_ai);

  if((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0){
	print_error("unable to create socket");
	return(-1);
  }
  printf("socket created [%d]\n", host_fd);

  if(bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen) < 0){
	print_error("unable to bind to socket");
	exit(1);
  }
  printf("bind returned success\n");

  freeaddrinfo(host_ai);

  if(listen(host_fd, QLEN) < 0){
	print_error("listen failed");
	exit(1);
  }
  printf("listen returned success\n");

  int last_client_ip_int = 0;

  struct sockaddr client_sockaddr;
  socklen_t client_sockaddr_len = sizeof(client_sockaddr);
  int token = 0;
  int clfd;
  int len;

  for (;;){
	token = 0;
	printf("Waiting for connection...\n");
	clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);

	if(clfd<0){
		print_error("accept error");
		exit(1);
	}
	printf("accepted connection, socket [%d]\n", clfd);

	if(client_sockaddr.sa_family != AF_INET){
		printf("Cannot connect with IPv^ addresses \n");
		printf("Sending -1\n");

		int mssg = -1;
		len = send(clfd, &mssg, 4, 0);
		if(len<0){
			print_error("error sending data");
		}
		printf("sent %d bytes\n", len);
		close(clfd);
		continue;
	}

	int n;
	int command;
	n = recv(clfd, &command, 4, 0);
	if(n<0){
		print_error("error reading from socket");
		exit(1);
	}
	
	if(command==1){
		//receiving first name
		char fName[24] = "";
		n = recv(clfd, &fName, 24, 0);
		if(n<0){
			print_error("error reading first name");
			exit(1);
		}

		//checking first name for null
		int i, counter;
		counter = 0;

		for(i=0; i<24; i++){
			if(fName[i]=='\0'){
				counter++;
			}
		}
		if(counter==24){
			token = 1;
			len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("error sending token");
			}
		}

		//receiving last name
		char lName[48] = "";
		n = recv(clfd, &lName, 48, 0);
		if(n<0){
			print_error("error reading last name");
		}

		//checking last name for null
		counter = 0;
		for(i=0; i<48; i++){
			if(lName[i]=='\0'){
				counter++;
			}
		}
		if(counter==48){
			token = 2;
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("error in sending token");
			}
		}

		//receiving email
		char email[36] = "";
		n = recv(clfd, &email, 36, 0);
		if(n<0){
			print_error("error reading email");
		}

		//receiving phone
		long phone = 0;
		n = recv(clfd, &phone, 8, 0);
		if(n<0){
			print_error("error reading phone");
		}


		//create sql entry and add to database

		if (con == NULL){
			fprintf(stderr, "%s\n", mysql_error(con));
			exit(1);
		}

		if (mysql_real_connect(con, hostname, username, password, schema, 0, NULL, 0) == NULL){
			fprintf(stderr, "%s\n", mysql_error(con));
			mysql_close(con);
			exit(1);
		}


		char query[200]; 
		sprintf(query, "INSERT INTO Contacts_T VALUES('%s','%s','%s',%li)", fName, lName, email, phone);

		if(mysql_query(con, query)){
			fprintf(stderr, "%s\n", mysql_error(con));
			token = 3;
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("Error sending token");
			}
			printf("sent %d bytes\n", len);
			continue;
  		}

		if(token==0){
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("Error sending token");
			}
			printf("sent %d bytes\n", len);
		}
		mysql_close(con);
		close(clfd);
		continue;
	}//end if(command==1)
	else if (command==2){
		//printf("Received code to print contacts: %d", command);
		//retrieve all from database
		if (con == NULL){
			fprintf(stderr, "%s\n", mysql_error(con));
			exit(1);
		}

		if (mysql_real_connect(con, hostname, username, password, schema, 0, NULL, 0) == NULL){
			fprintf(stderr, "%s\n", mysql_error(con));
			mysql_close(con);
			exit(1);
		}

		//char query[200]; 
		if(mysql_query(con, "SELECT * FROM Contacts_T")){
			fprintf(stderr, "%s\n", mysql_error(con));
			mysql_close(con);
			continue;
		}

		MYSQL_RES *result = mysql_store_result(con);

		if(result == NULL){
			fprintf(stderr, "%s\n", mysql_error(con));
			mysql_close(con);
			exit(1);
		}


		//printf("Result is: %s", result);
		//char contacts[120] = "";		

		int num_fields = mysql_num_fields(result);
		MYSQL_ROW row;
		int num = mysql_num_rows(result);
		printf("%d\n", num);

		int len = send(clfd, &num, 4, 0);
		if(len<0){
			print_error("Error in sending num of contacts");
		}

		while((row=mysql_fetch_row(result))){
			char contacts[120] = "";		
			/*for(int i=0; i<num_fields; i++){
				printf("%s ", row[i] ? row[i] : "NULL");
			}
			printf("\n");*/
			
			for(int i=0; i<num_fields; i++){
				strcat(contacts, row[i]);
				strcat(contacts, " ");
			}
			
			printf("%s", contacts);
			printf("\n");
			int len = send(clfd, &contacts, 120, 0);
			if(len<0){
				print_error("Error sending fName");
			}
			
		}

		if(token==0){
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("Error sending token");
			}
			printf("sent %d bytes\n", len);
		}
		mysql_close(con);
		close(clfd);
		continue;
	} //end else if(command==2)
  }//end for(;;)
  mysql_close(con);
  close(clfd);
  close(host_fd);
  exit(0);
} //end of main

void print_ip_addresses(struct addrinfo *host_ai){
	struct addrinfo *next_ai;
	unsigned long *ipv4_addr;
	unsigned char *ipv6_addr;

	char ip_str[INET6_ADDRSTRLEN];

	for(next_ai = host_ai; next_ai != NULL; next_ai = next_ai->ai_next){
		memset(ip_str, '\0', INET6_ADDRSTRLEN);
		if(next_ai->ai_addr->sa_family == AF_INET){
			printf("IPv4 ");
			ipv4_addr = (unsigned long*) &(((struct sockaddr_in*)next_ai->ai_addr)->sin_addr);
			inet_ntop(AF_INET, ipv4_addr, ip_str, sizeof(ip_str));
		}
		else{
			printf("IPv6 ");
			ipv6_addr = (unsigned char*) &(((struct sockaddr_in6*)next_ai->ai_addr)->sin6_addr);
			inet_ntop(AF_INET6, ipv6_addr, ip_str, sizeof(ip_str));
		}
		printf("IP address: %s\n", ip_str);
	}
}

void print_error(char *str){
	printf("%s: %s\n", str, strerror(errno));
}

//signal handler
void my_handler(){
	close(host_fd);
	exit(0);
}
