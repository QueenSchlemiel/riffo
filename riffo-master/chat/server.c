//SERVER CODE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>



//globals
#define total_rooms 10
#define total_users 16
#define QLEN 128

#ifndef HOST_NAME_MAX
	#define HOST_NAME_MAX 256
#endif

extern int errno;
int host_fd;

//port 8896

//struct to keep track of each chat room
struct chat_room{
	int id;
	pthread_t tid;
	int num_users; //keep track of how many users in room
	int clfds[16]; //keep track of clfds of clients
};


//sends the message to each client in the specified chat room
/*void * send_message(int sock_fd){

	//sends to all clfds in specified chat_room
	//utilizes struct clfds[]

}*/

//uses a thread to receive the messages in real time
/*void * recv_message(){

	//if message received then call send_message()

}*/


//prints the error using errno
void print_error(char *str){
	printf("%s: %s\n", str, strerror(errno));
}


int main(int argc, char **argv){

	struct rooms_arr[10];
	//pointer to struct?

	int chat_id;
	char *host_name;

	if(argc<2){
		printf("Please enter a group id\n");
		exit(1);
	}
	else{
		
		//do we specify the host_name or does it have to come from usr?
		host_name = argv[1];
		chat_id = atoi(argv[2]);
	}


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

	//getting the address info needed
	if((getaddrinfo(host_name, "chat", &hint, &host_ai)) != 0){
		print_error("getaddrinfo error");
		exit(1);
	}

	//creating the socket
	if((host_fd = socket(host_ai->ai_addr->sa_family, SOCK_STREAM, 0))<0){
		print_error("unable to create socket");
		return(-1);
	}
	printf("socket created [%d]\n", host_fd);

	//binding to the socket
	if(bind(host_fd, host_ai->ai_addr, host_ai->ai_addrlen)<0){
		print_error("unable to bind to socket");
		exit(1);
	}
	printf("bind returned success\n");

	//free the address info
	freeaddrinfo(host_ai);

	//start listening on the port
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

	//keep accepting clients for rooms
	for(;;){
		token = 0;
		printf("Waiting for connection...\n");
		clfd = accept(host_fd, &client_sockaddr, &client_sockaddr_len);

		if(clfd<0){
			print_error("accept error");
			exit(1);
		}
		printf("Accepted connection, socket [%d]\n", clfd);

		if(client_sockaddr.sa_family != AF_INET){
			printf("Cannot connect with IPv^ addresses\n");
			printf("Sending -1\n");
			
			int mssg = -1;
			if((len = send(clfd, &mssg, 4, 0))<0){
				print_error("error in sending data");
			}
			printf("sent %d bytes\n", len);
			close(clfd);
			continue;
		}


		//what do we need to receive from the client?
		int message;
		message = recv(clfd, &message, 100, 0);
		if(message<0){
			print_error("error reading from socket");
			exit(1);
		}
	




	} //end infinite for loop

} //end main
