#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAXADDRLEN 256

extern int errno;

void print_error(char *);
int connect_to_server(char *);
void send_data(int, char[24], char[48], char[36], long);
int receive_data();

int main(int argc, char *argv[]) {
	int sockfd;

	while(1) {

		char f_name[24] = "";
		char l_name[48] = "";
		char email_addr[36] = "";
		long phone_num;
		int input; 

		printf("1. Insert new record\n2. List Contacts\n3. Exit\n");
		scanf("%d", &input);
		
		int code = 0;
		int len;

		switch(input) {
			case 1: 
				printf("Enter first name:\n");
				scanf("%s", &f_name);
				printf("Enter last name:\n");
				scanf("%s", &l_name);
				printf("Enter email address:\n");
				scanf("%s", &email_addr);	
				printf("Enter phone number:\n");
				scanf("%li", &phone_num);
				
				sockfd = connect_to_server(argv[1]);

				//sending information
				len = send(sockfd, &input, 4, 0);
				
				if(len < 0) {
				close(sockfd);
					print_error("error sending data");
					printf("sent %d bytes\n", len);
				}	
		
				len = send(sockfd, &f_name, 24, 0);
				if(len < 0) {
				close(sockfd);
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}

				len = send(sockfd, &l_name, 48, 0);
                		if(len < 0) {
				close(sockfd);
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}
		
				len = send(sockfd, &email_addr, 36, 0);
                		if(len < 0) {
				close(sockfd);
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}

				len = send(sockfd, &phone_num, 8, 0);
                		if(len < 0) {
				close(sockfd);
                        		print_error("error sending data");
					printf("sent %d bytes\n", len);
                		}
		
				//receiving data
				int n = 0;

				n = recv(sockfd, &code, 4, 0);

				if(n < 0) {
				close(sockfd);
					print_error("recv error");
				}

				close(sockfd);
				break;
			case 2:
				sockfd = connect_to_server(argv[1]);

				//sending information
				len = send(sockfd, &input, 4, 0);
				if(len < 0) {
				close(sockfd);
					print_error("error sending data");
					printf("sent %d bytes\n", len);
				}	
				
				//receiving number of contacts
				n = 0;
				int num;
				
				n = recv(sockfd, &num, 4, 0);
				if(n < 0) {
				close(sockfd);
					print_error("recv error");
				}

				printf("Number of contacts: %d\n", num);

				//receive contacts
				//116 bytes
				char result[120] = "";
				
				for(int i=0; i<num; i++){
					n = recv(sockfd, &result, 120, 0);
					if(n<0){
				close(sockfd);
						print_error("Error receiving contacts");
					}
					//printf("Hi from inside while loop\n");
					//printf("Bytes received %d: ", n);
					printf("%s", result);
					printf("\n");
				}

				close(sockfd);
				break;
			case 3:
				printf("Exiting gracefully\n");
				exit(0);
				break;
			default:
				exit(1);
				break;
		}

		switch(code) {
			case 0:
				printf("Success\n");
				break;
			case 1: 
				printf("Error - First name is null\n");
				break;
			case 2:
				printf("Error - Last name is null\n");
				break;
			case 3: 
				printf("Error - Record with first and last name already exist\n");
				break;
			default:
				exit(1);
		}
	
	}
    	
	exit(1);
}

int connect_to_server(char* addr){

	struct addrinfo *host_ai;
	struct addrinfo hint;
	int sockfd, err;
    
	hint.ai_flags = 0;
    	hint.ai_family = AF_INET;
    	hint.ai_socktype = SOCK_STREAM;
    	hint.ai_protocol = 0;
    	hint.ai_addrlen = 0;
    	hint.ai_canonname = NULL;
    	hint.ai_addr = NULL;
    	hint.ai_next = NULL;
	
	if ((err = getaddrinfo(addr, "contacts", &hint, &host_ai)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(err));
		return 0;
	}

	if ((sockfd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0)) < 0) {
		print_error("Error creating socket");
	}

	if (connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen) != 0) {
		printf("can't connect to %s\n", addr);
		print_error("Error connecting to server");
	}
	
	freeaddrinfo(host_ai);
	return sockfd;
}

void print_error(char *str) {
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}

// END OF FILE
