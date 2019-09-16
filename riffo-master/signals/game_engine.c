/************************
* Name: Laura Riffo
* Last Modified: 2.1.19
*************************/
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//needed these to be global in order to update it
pid_t pidArr[10];
int val = 0;

void child_handler(int sig);

int main(int argc, char* argv[]){
	int input;
	pid_t pid;
	int counter=1;
	char buff[8];
	//pid_t pidArr[10];
	//int val = 0; //placeholder for array

	/*******************************
	* Initialize array with all -1
	********************************/
	for(int i=0; i<10; i++){
		pidArr[i] = -1;
	}
	
	/***************************
	* While user does not
	* want to quit (input == 5)
	* Display options
	****************************/
	while(input!=5){
		printf("1. Spawn Player\n2. Injure Player\n3. Heal Player\n4. List Players\n5. Quit\n");
		scanf("%d", &input);

		printf("%d\n", input);		
		snprintf(buff, sizeof(buff), "%d", counter);
		signal(SIGCHLD, child_handler);
		

		/******************************
		* Creates a child process and
		* runs the player executable
		*******************************/
		if(input==1){
			pid=fork();
			if(pid==0){
				execl("./player","player",buff,NULL);
			}
			else if(pid==-1){
				printf("Failure: %d\n", pid);
			}
			else{
				printf("Player %d created.\n", pid);
				for(int i=0; i<10; i++){
					if(pidArr[i] == -1){
						pidArr[i] = pid;
						break;
					}
				}
				//pidArr[val] = pid;
				//val++;
			}
		}
		/********************************************
		* Displays a list of the engine's child PIDs
		* Uses SIGUSR1 to decrease health
		*********************************************/
		else if(input==2){
			pid_t pid2;
			//List the PIDs
			int i;
			for(i=0; i<10; i++){
				if(pidArr[i] != -1){
					printf("Enemy %d: %d\n", i+1, pidArr[i]);
				}
			}
			
			//Prompt user for a PID
			printf("Enter a PID\n");
			scanf("%d",&pid2);
			printf("PID Entered is: %d\n", pid2);
			//use SIGUSR1 to change that PID
			//int a = kill(pid2, SIGUSR1); 
			//signal(SIGUSR1, sig_handler);
			if(kill(pid2, SIGUSR1)==-1){ printf("Error with SIGUSR1\n");}
		}
		/********************************************
		* Displays a list of the engine's child PIDs
		* Uses SIGUSR2 to increase health
		*********************************************/
		else if(input==3){
			pid_t pid3;
			//List the PIDs
			int i;
			for(i=0; i<10; i++){
				if(pidArr[i] != -1){
					printf("Enemy %d: %d\n", i+1, pidArr[i]);
				}
			}
			//Prompt user for a PID
			printf("Enter a PID\n");
			scanf("%d",&pid3);
			printf("PID Entered is: %d\n", pid3);
			//use SIGUSR2 to change that PID
			//int a = kill(pid3, SIGUSR2);
			//signal(SIGUSR2, increase_health());
			if(kill(pid3,SIGUSR2)==-1){ printf("Error with SIGUSR2\n");}
		}
		/*****************************************
		* If user enters 4
		* Displays a list of engine's child PIDs
		******************************************/
		else if(input==4){
			//List the engine's child PIDs
			int i;
			for(i=0; i<10; i++){
				if(pidArr[i] != -1){
					printf("Enemy %d: %d\n", i+1, pidArr[i]);
				}
			}
		}
	}//end of while

	//kill all child processes
	int i;
	for(i=0; i<10; i++){
		if(pidArr[i] != -1){
			kill(pidArr[i], SIGKILL);
		}
	}
	printf("GAME OVER");

	return 0;
}

/*********************************************
* Signal handler for checking child processes
**********************************************/
void child_handler(int sig){
	pid_t pid;
	int i;

	pid = wait(NULL);
	printf("PID %d exited.\n", pid);
	for(i=0; i<10; i++){
		if (pidArr[i]==pid){
			pidArr[i] = -1;
		}
	}
	//return pid;
}
