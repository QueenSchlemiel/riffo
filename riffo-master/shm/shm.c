/************************************
* Team: Laura Riffo and Briana Moore
* Last Modified: 2.4.19
*
* Shared Memory - chat room
* Works better with short messages
*************************************/

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

	/*****************************************
	* Get a key to sharewith other processors
	******************************************/

	int tokid = 0;
	char *filepath = "/tmp";
	key_t key;
	
	if((key=ftok(filepath, tokid))==-1){
		print_error("Cannot create token ", errno);
	}
	//printf("Token id: %d\n", key);

	/****************************************
	* Get an ID for the shared memory spaces
	*****************************************/

	long bufsz = sysconf(_SC_PAGESIZE);
	//printf("Page size: %ld\n", bufsz);

	int shmid;
	if((shmid=shmget(key, bufsz, IPC_CREAT | 0600))==-1){
		print_error("Cannot create shared memory ", errno);
	}
	//printf("Shared memory id: %d\n", shmid);

	/*******************************************
	* Attach: get a pointer to the memory space
	********************************************/

	char *shm = NULL;
	if((shm=shmat(shmid,0,0))==(void *)-1){
		print_error("Unable to attach to shared memory ", errno);
	}

	/**************************************************
	* Chat room up and writing and getting from memory
	***************************************************/

	//clean_up(shm, shmid);
	printf("\nWelcome to the chat room. You should always check to see if there were previous messages for you by pressing 2 unless told otherwise by the chat room. Thanks, enjoy your chatting.\n");

	int input;
	char username[100];
	char user[100];

	printf("\nEnter your username: ");
	fgets(username, 100, stdin);
	username[strlen(username)-1] = '\0';
	concat(user, username);
	
	//Check if there is already messages in memory
	int shmlen = strlen(shm);
	if(shmlen==0){
		printf("\nNo one is in the chat room. Please send a message to connect by pressing 1\n");
	}
	else if(shmlen>0){
		printf("\nSomeone is ready to chat. Press 2 to get message\n");
	}
	
	//sleep(5);
	
	//Start messaging
	while(1){
		//Writing to shared memory
		//Menu for user to work with
		//Made sending and getting messages easier
		printf("1. Send Message\n2. Get Message\n3. Quit\n");
		scanf("%d\n", &input);

		shmlen = strlen(shm);
		//printf("Shared memory bytes used: %d\n", shmlen);
		if(input==1){
			char cbuf[100];
			
			printf("Enter message: \n");
			fgets(cbuf, 100, stdin);
			concat(username, cbuf);
		
			int cbuflen = strlen(username);
				
			if(shmlen + cbuflen+1<bufsz){
				memcpy(shm+shmlen, username, cbuflen+1);
				printf("\nMessage: %s\n", shm);
			}
			else{
				printf("Buffer full\n");
				clean_up(shm, shmid);
				exit(0);
			}

			memset(username, 0, sizeof(username));
			strcpy(username,user);

		}//end of input==1
		else if(input==2){
			//sleep to give time for system to write message to memory
			//sleep(5);
			printf("\nMessage: %s\n", shm);
		}//end of input==2
		else{
			clean_up(shm, shmid);
			exit(0);
		}//end of else
	}//end of while
	
	exit(0);
}

/******************************
* Error printing if any errors
*******************************/
void print_error(const char* str, int code){
	printf("%s: %s\n", str, strerror(code));
	exit(-1);
}

/***************************************
* Clean up shared memory and pointer ID
****************************************/
void clean_up(char *shm, int shmid){
	if(shmdt(shm)==-1){
		print_error("Unable to detach shared memory ", errno);
	}
	if(shmctl(shmid, IPC_RMID, 0)==-1){
		print_error("Cannot validate shared memory ", errno);
	}
}


/*******************************
* Concatenates 2 strings into 1
********************************/
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

//END OF FILE
