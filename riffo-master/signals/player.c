/*************************
*  Name: Laura Riffo
*  Last Modified: 2.1.19
**************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

//global variable
int HEALTH_LEVEL = 3;

void sig_handler(int signum);
void increase_health();
void decrease_health();

int main(int argc, char* argv[]){
	while(1){
		int a;
		//increase_health();
		signal(SIGUSR1, decrease_health);
		signal(SIGUSR2, increase_health);
		if(HEALTH_LEVEL==0){
			kill(getpid(), SIGKILL);
			exit(0);
			
		}
	}//forever loop

	return 0;
}

/******************************
* sig_handler to check the
* incoming signal
*******************************/
void sig_handler(int signum){
	if(signum==SIGUSR1){
		decrease_health();
	}
	else if(signum==SIGUSR2){
		increase_health();
	}
	else{
		printf("Signum was not SIGUSR1 or SIGUSR2\n");
	}
}

/********************************
* increase_health will utilize
* SIGUSR2 to add 1 to health
*********************************/
void increase_health(){
	if(HEALTH_LEVEL == 3){
		printf("Health Level Max\n");
	}
	else{
		HEALTH_LEVEL++;
	}
	printf("HEALTH: %d\n", HEALTH_LEVEL);
	//return HEALTH_LEVEL;
}

/***********************************
* decrease_health will utilize
* SIGUSR1 to subtract 1 from health
************************************/
void decrease_health(){
	if(HEALTH_LEVEL-1==0){
		HEALTH_LEVEL--;
		printf("You have killed the enemy\n");
		//kill the player
		kill(getpid(), SIGKILL);
	}
	else{
		HEALTH_LEVEL--;
	}
	printf("HEALTH: %d\n", HEALTH_LEVEL);
	//return HEALTH_LEVEL;
}
