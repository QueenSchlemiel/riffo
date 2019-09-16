#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>



extern int errno;

struct ip_sum_struct{
	int a,b,c,d;
	int answer;
};

void* ip_runner(void* arg){
	struct ip_sum_struct *arg_struct = (struct ip_sum_struct*) arg;
	int sum = 0;
	
	int a = arg_struct->a;
	int b = arg_struct->b;
	int c = arg_struct->c;
	int d = arg_struct->d;
	//int success = sscanf(arg, "%i.%i.%i.%i", &a, &b, &c, &d);
	sum = a+b+c+d;
	
	//printf("Sum in runner is: %d\n", sum);

	arg_struct->answer = sum;
	pthread_exit(0);
}


void handler(){
	exit(0);
}

void print_error(char *str){
	printf("%s: %s\n", str, strerror(errno));
	exit(1);
}

int connect_to_server(char *addr){
	struct addrinfo *host_ai;
	struct addrinfo hint;
	int sockfd, err;

	hint.ai_flags = 0;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addrlen = NULL;
	hint.ai_next = NULL;

	if((err=getaddrinfo(addr,"final", &hint, &host_ai))!=0){
		printf("error: %s\n", gai_strerror(err));
		return 0;
	}

	if((sockfd=socket(host_ai->ai_addr->sa_family, SOCK_STREAM,0))<0){
		print_error("can't create socket");
	}

	if(connect(sockfd, host_ai->ai_addr, host_ai->ai_addrlen)!=0){
		printf("Can't connect\n");
		print_error("Error connecting");
	}

	freeaddrinfo(host_ai);
	return sockfd;
}

int main(int argc, char *argv[]){

	signal(SIGINT, handler);


	int sockfd;
	int len;
	char result[16] = "";
	int sum;
	char quote[256] = "";
	

	char user = "lriffo";
	
	sockfd = connect_to_server(argv[1]);
	len = send(sockfd, &user, 8, 0);
	if(len<0){
		close(sockfd);
		print_error("Error in sending data");
		printf("sent %d bytes\n", len);
	}

	int n = 0;
	n = recv(sockfd, &result, 16, 0);
	if(n<0){
		close(sockfd);
		print_error("Error receiving data");
	}
	printf("%s\n", result);


	int a,b,c,d;
	int success = sscanf(result, "%i.%i.%i.%i", &a, &b, &c, &d);
	//printf("IP ints: %d, %d, %d, %d\n", a,b,c,d);

	struct ip_sum_struct args;
	args.a = a;
	args.b = b;
	args.c = c;
	args.d = d;

	//use a thread to do this above
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, ip_runner, &args);

	//wait until done
	pthread_join(tid, NULL);
	printf("Sum is: %d\n", args.answer);
	

	//sum = 280;
	sum = args.answer;

	len = send(sockfd, &sum, 4, 0);
	if(len<0){
		close(sockfd);
		print_error("Error in sending data");
		printf("Sent %d bytes\n", len);
	}

	n = recv(sockfd, &quote, 256, 0);
	if(n<0){
		close(sockfd);
		print_error("Error receiving data");
	}
	printf("%s\n", quote);



	close(sockfd);


	exit(1);
} //end of main
