#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

void print_error(const char*, int);
void clean_up(char*, int);
void concat(char p[], char q[]);

int main(int argc, char* argv[]){
	int tokid = 0;
	char *filepath = "/tmp";
	key_t key;
	
	if((key=ftok(filepath, tokid))==-1){
		print_error("Cannot create token ", errno);
	}
	//printf("Token id: %d\n", key);

	long bufsz = sysconf(_SC_PAGESIZE);
	//printf("Page size: %ld\n", bufsz);

	int shmid;
	if((shmid=shmget(key, bufsz, IPC_CREAT | 0600))==-1){
		print_error("Cannot create shared memory ", errno);
	}
	//printf("Shared memory id: %d\n", shmid);

	char *shm = NULL;
	if((shm=shmat(shmid,0,0))==(void *)-1){
		print_error("Unable to attach to shared memory ", errno);
	}

	//clean_up(shm, shmid);
	printf("\nWelcome to the chat room. You should always check to see if there were previous messages for you by pressing 2 unless told otherwise by the chat room. Thanks, enjoy your chatting.\n");
	int input;
	char username[100];
	char user[100];
	printf("\nEnter your username: ");
	scanf("%s", &username);
	concat(user, username);
	//fgets(username);
	//printf("Username is: %s\n", username);
	int shmlen = strlen(shm);
	if(shmlen==0){
		printf("\nNo one is in the chat room. Please send a message to connect by pressing 1\n");
	}
	else if(shmlen>0){
		printf("\nSomeone is ready to chat. Press 2 to get message\n");
	}
	//int prevslen;
	//int curslen;
	
	//sleep(5);

	while(1){
	
	//Writing to shared memory
	printf("1. Send Message\n2. Get Message\n3. Quit\n");
	scanf("%d", &input);

	shmlen = strlen(shm);
	//printf("Shared memory bytes used: %d\n", shmlen);
	if(input==1){
	printf("\nMessage: %s\n", shm);

	char cbuf[100];
	//strcat(username, cbuf);
	printf("Enter message: \n");
	scanf("%s", &cbuf);
	//fgets(cbuf);
	//snprintf(cbuf, sizeof cbuf, "%s%s", cbuf, username);
	concat(username, cbuf);
	//cbuf = username;
	//printf("%s", cbuf);
	//printf("\nUSERNAME TEST: %s\n", username);
	int cbuflen = strlen(username);
	//printf("\nLength of string to write: %d\n", cbuflen);
	
	if(shmlen + cbuflen+1<bufsz){
		//printf("\nBefore write (%lu) %s: %s\n", strlen(shm), username, shm);
		//memcpy(shm+shmlen, username, strlen(username)+1);
		//clean_up(shm, shmid);
		memcpy(shm+shmlen, username, cbuflen+1);
		//printf("\n%s Message: %s\n", user, shm);
		//sleep(3);
	}
	else{
		printf("Buffer full\n");
		clean_up(shm, shmid);
		exit(0);
	}
	}//end of input==1
	else if(input==2){
	sleep(5);
	printf("\nMessage: %s\n", shm);
	}//end of input==2
	else{
		exit(0);
		clean_up(shm, shmid);
	}//end of else
	}//end of while
	
	exit(0);
}

void print_error(const char* str, int code){
	printf("%s: %s\n", str, strerror(code));
	exit(-1);
}

void clean_up(char *shm, int shmid){
	if(shmdt(shm)==-1){
		print_error("Unable to detach shared memory ", errno);
	}
	if(shmctl(shmid, IPC_RMID, 0)==-1){
		print_error("Cannot validate shared memory ", errno);
	}
}

void concat(char p[], char q[]){
	int c,d;
	c=0;

	while(p[c]!='\0'){
		c++;
	}

	p[c] = ' ';
	c++;

	d=0;

	while(q[d] != '\0'){
		p[c] = q[d];
		d++;
		c++;
	}
	p[c] = ' ';
	c++;
	p[c] = '\0';
}
