/*
 * server.c
 * derived from Advanced Programming in the UNIX Environment, 3rd Edition
 * 
 * Brie Moore and Laura Riffo 3/18/19
 */

#include <sys/types.h>	// getaddrinfo()
#include <netdb.h>	// getaddrinfo()
#include <sys/socket.h> // getaddrinfo()
#include <stdio.h>	// printf()
#include <stdlib.h> 	// exit()
#include <unistd.h>	// gethostname()
#include <arpa/inet.h>  // inet_ntop()
#include <string.h>	// strerror()
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
const char* getfirstName(char *);
const char* getlastName(char *);
void my_handler();

int main(int argc, char *argv[]) 
{
	signal(SIGINT, my_handler);
	char *host_name;

    	if (argc == 2) {
		host_name = argv[1];		
	}
	else {
		host_name = malloc(HOST_NAME_MAX);
		memset(host_name, 0, HOST_NAME_MAX);

		if (gethostname(host_name, HOST_NAME_MAX) < 0) {
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

	
	if ((getaddrinfo(host_name, "flat-file-db-server", &hint, &host_ai)) != 0) { 
		//printf("STRERROR: %s\n", strerror(errno));
		print_error("getaddrinfo error");
        	exit(1);
	}
	

	print_ip_addresses(host_ai);

	if ((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("unable to create socket"); 
		return(-1);
	}
	printf("socket created [%d]\n", host_fd);

        if (bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen) < 0) {
                print_error("unable to bind to socket");
                exit(1);
        }
	printf("bind returned success\n");

	freeaddrinfo(host_ai);

        if (listen(host_fd, QLEN) < 0) {
                print_error("listen failed");
                exit(1);
        }
	printf("listen returned success\n");

	int last_client_ip_int = 0;

	struct sockaddr client_sockaddr;
	socklen_t client_sockaddr_len = sizeof(client_sockaddr);
	int token = 0;
	int clfd;
	
	for (;;) {
		token = 0;
		printf("waiting for connection ...\n");
        	clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);
        	
		if (clfd < 0) {
			print_error("accept error");
			exit(1); 
		}
		printf("accepted connection, socket [%d]\n", clfd);

		if (client_sockaddr.sa_family != AF_INET) {
			printf("Can not onnect with IPv6 addresses\n");
			printf("Sending -1\n");

			int mssg = -1;
			int len = send(clfd, &mssg, 4, 0);
			if (len < 0) {
				print_error("error sending data");
			}
			printf("sent %d bytes\n", len);

			close(clfd);
			continue;
		}


		//receiving the command to add or quit
		int buffer;

		int n;
		//bzero(buffer,115);
		//recieving from the client
		n = recv(clfd, &buffer, 4, 0);
		
		if(n<0){
			print_error("Error reading from socket");
			exit(1);
		}
		//printf("Buffer %d\n", buffer);		
		/*if(buffer==2){
			//printf("Buffer %d", buffer);
			close(clfd);
			close(host_fd);
			exit(1);
		}*/
			
		
		//printf("Here is the sample: %d\n", buffer);


		//receiving the first name from the client
		char fName[24] = "";
		n = recv(clfd, &fName, 24, 0);

		if(n<0){
			print_error("Error reading fName");
		}

		int i;
		int counter = 0;
		printf("Here is the first name: ");
		
		//checking if first name is null
		for(i=0; i<24; i++){
			if(fName[i]=='\0'){
				counter++;
			}
			else{
				printf("%c", fName[i]);
			}
		}
		printf("\n");
		
		if(counter==24){
			token = 1;
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("Error in sending token");
			}
			//break;
		}

		//receiving last name from client
		char lName[48] = "";
		n = recv(clfd, &lName, 48, 0);

		if(n<0){
			print_error("Error reading lName");
		}
		

		counter = 0;
		printf("Here is the last name: ");

		//checking if last name is null
		for(i=0; i<48; i++){
			if(lName[i]=='\0'){
				counter++;
			}
			else{
				printf("%c", lName[i]);
			}
		}
		printf("\n");

		if(counter==48){
			token = 2;
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("Error in sending token");
			}
			//break;
		}


		//receiving email from client
		char email[36] = "";
		n = recv(clfd, &email, 36, 0);
		
		if(n<0){
			print_error("Error reading email");
		}

		printf("Here is the email: ");
		for(i=0; i<36; i++){
			printf("%c", email[i]);
		}
		printf("\n");


		//receiving the phone from the client
		long phone = 0;
		
		n = recv(clfd, &phone, 8, 0);

		if(n<0){
			print_error("Error reading phone");
		}

		printf("Here is the phone: %li\n", phone);		

		//writing to csv file
		char *filename = "contacts.csv";
		FILE *fp;
		fp = fopen(filename, "a+");
		
		char line[116];
		
		//reading from the file
		int counter2;
		while(fgets(line, 116, fp)){
			counter = 0;
			counter2 = 0;
			char* tmp = strdup(line);
			int j;
			const char* name = getfirstName(tmp);

			char val[24] = "";
			strcpy(val, name);

			for(j=0;j<24;j++){
				if(val[j] == fName[j]){
					counter++;
				}
			}
		
			
			//checking to see if the last name is in the csv file	
			free(tmp);
			tmp = strdup(line);
			const char* name2 = getlastName(tmp);
			
			char val2[48] = "";
			strcpy(val2, name2);

			for(j=0;j<48;j++){
				if(val2[j] == lName[j]){
					//printf("VAL2: %c\n", val2[j]);
					//printf("lName: %c\n", lName[j]);
					counter2++;
				}
			}
			
			free(tmp);
			
			if(counter == 24 && counter2 == 48){
				//printf("TEST\n");
				token = 3;
				int len = send(clfd, &token, 4, 0);
				if(len<0){
					print_error("Error with sending token 3");
				}
				printf("sent %d bytes\n", len);
				printf("This is the token: %d\n", token);
				break;
			}
		} 
				
		//fprintf(fp, "First Name, Last Name, Email, Phone Number\n");
		if(token == 0){
			fprintf(fp, "%s,%s,%s,%li\n", fName, lName, email, phone);
			printf("Wrote: %s, %s, %s, %li\n", fName, lName, email, phone);
			int len = send(clfd, &token, 4, 0);
			if(len<0){
				print_error("Error with sending token 0");
			}
			printf("sent %d bytes\n", len);
		}
		fclose(fp);
		
		//close(clfd);
	} 
	close(clfd);	
	close(host_fd);
}


const char* getfirstName(char* line){
	const char* tok;
	int num = 1;
	for(tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
		if(!--num){
			//printf("Token in getFirstName: %s", tok);
			return tok;
		}
	}
	return NULL;
}


const char* getlastName(char* line){
	const char* tok;
	int num = 2;
	for(tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
		if(!--num){
			return tok;
		}
	}
	return NULL;
}
	


void print_ip_addresses(struct addrinfo *host_ai)
{
	struct addrinfo	*next_ai; 
	unsigned long *ipv4_addr;
	unsigned char *ipv6_addr;

	char ip_str[INET6_ADDRSTRLEN];

	//printf("Hello from print_ip_addresses\n");

	for (next_ai = host_ai; next_ai != NULL; next_ai = next_ai->ai_next) {

		memset(ip_str, '\0', INET6_ADDRSTRLEN);

		if (next_ai->ai_addr->sa_family == AF_INET) {
			printf("IPv4 ");
			ipv4_addr  = (unsigned long*) &(((struct sockaddr_in*)next_ai->ai_addr)->sin_addr);
			inet_ntop(AF_INET, ipv4_addr, ip_str, sizeof(ip_str));
		}
		else {
			printf("IPv6 ");
			ipv6_addr = (unsigned char*) &(((struct sockaddr_in6*)next_ai->ai_addr)->sin6_addr);
			inet_ntop(AF_INET6, ipv6_addr, ip_str, sizeof(ip_str));
		}

		printf("IP address: %s\n", ip_str);
	}
}

void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
}

//signal handler for ctrl-c
void my_handler(){
	close(host_fd);
	exit(0);
}
