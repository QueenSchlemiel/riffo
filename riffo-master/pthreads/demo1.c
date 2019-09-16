#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//the sum computed by the background thread
long long sum = 0;

//Thread function to generate sum of 0 to N
void* sum_runner(void * arg){
	long long *limit_ptr = (long long*) arg;
	long long limit = *limit_ptr;

	//long long sum = 0;
	for(long long i=0; i<=limit; i++){
		sum+=i;
	}

	//TODO: what to do with answer?
	//sum is a global variable so other threads can access

	pthread_exit(0);
	
}

int main(int argc, char **argv){
	if(argc<2){
		printf("Usage: %s <num>\n", argv[0]);
		exit(-1);
	}

	long long limit = atoll(argv[1]);
	
	//man pthread_create
	//uses a pointer to a function
	//accepts a void pointer
	//returns a void pointer
	
	//Thread ID
	pthread_t tid;

	//Create attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//considered 2nd thread
	pthread_create(&tid, &attr, sum_runner, &limit);


	//do other stuff here...

	//wait until thread is done
	pthread_join(tid, NULL);

	printf("sum is %lld\n", sum);
}
