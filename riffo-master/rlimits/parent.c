#include <sys/resource.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	struct rlimit resource;

	int a = getrlimit(RLIMIT_NPROC, &resource);
	printf("Limit is: current (%d), max (%d)\n", resource.rlim_cur, resource.rlim_max);

	//Change the limit
	resource.rlim_cur = 7;
	
	int b = setrlimit(RLIMIT_NPROC, &resource);
	int c = getrlimit(RLIMIT_NPROC, &resource);
	if(a==-1 || b==-1 || c==-1){
		printf("Error with setlimit and getlimit\n");
	}

	printf("New limit is: current (%11d), max (%11d)\n", (long long int) resource.rlim_cur, (long long int) resource.rlim_max);
	//printf("setrlimit: %d\n", a);
	
	int counter = 1;
	int status;
	pid_t pid;
	char buff[8];
	while (1){
		printf("Creating child: %d\n", counter);
		pid = fork();
		//printf("PID: %d\n", pid);
		snprintf(buff, sizeof(buff), "%d", counter);
		if(pid==0){
			status = execl("./child","child",buff, (char *) NULL);
			printf("Inside child process - Child PID: %d\n", pid);
			if(status==-1){
				printf("Error with execl: %d\n", pid);
				exit(0);
			}
			//exit(0);
			//sleep(3);
		}
		else if(pid>0){
			printf("Parent process: %d\n", pid);
			counter++;
		}
		else if(pid==-1){
			//printf("Child %d: ", counter);
			printf("Failure PID: %d\n", pid);
			printf("Fork = -1 Error: %s\n", strerror(errno));
			//sleep(4);
			exit(0);
			//sleep(4);
		}
		else{
			printf("Child %d successfully created with pid %d: \n", counter, pid);
			//counter++;
		}
	}
	sleep(4);
	kill(pid, SIGKILL);
	return 0;
}
