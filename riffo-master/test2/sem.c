#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>



void handler(){
	exit(0);
}

void printerror(const char* str, int c){
	printf("%s: %s\n", str, strerror(c));
	exit(-1);
}


int main(){
	char *filepath = "/tmp";
	int tokid = 0;
	key_t key;
	int semid;

	signal(SIGINT, handler);

	if((key=ftok(filepath, tokid)) == -1){
		printerror("cannot create token", errno);
		return -1;
	}

	if((semid=semget(key,1,0666|IPC_CREAT))==-1){
		printerror("cannot create semaphore", errno);
		return -1;
	}

	union semun{
		int val;
		struct semid_ds *buf;
		ushort *array;
	}arg;
	arg.val = 5;

	if(semctl(semid,0,SETVAL,arg)==-1){
		printerror("cannot set semaphore value", errno);
		return -1;
	}

	while(1){
		int semvalue=0;
		if((semvalue=semctl(semid,0,GETVAL))==-1){
			printerror("cannot get value", errno);
			return -1;
		}

		if(semvalue>0){
			struct sembuf op[1];
			op[0].sem_num=0;
			op[0].sem_op=-1;
			op[0].sem_flg=0;
			if(semop(semid,op,1)==-1){
				printerror("cannot perform op", errno);
				return -1;
			}
			printf("consumed\n");
		}
		sleep(1);
	}
}
