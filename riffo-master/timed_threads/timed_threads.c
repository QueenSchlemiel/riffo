#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct sum_runner_struct{
	long long limit;
	long long answer;
};

//thread function to generate sum of 0 to N
void* sum_runner(void* arg){
	struct sum_runner_struct *arg_struct = (struct sum_runner_struct*) arg;

	long long sum = 0;
	//accessing limit
	for(long long i=0; i<=arg_struct->limit; i++){
		sum += i;
	}

	//accessing answer
	arg_struct->answer = sum;
	pthread_exit(0);
}

int main(int argc, char **argv){
	time_t seconds1;
	time(&seconds1);

	if(argc<2){
		printf("Usage: %s <num 1> <num2> ... <num-n>\n", argv[0]);
		exit(-1);
	}

	int num_args = argc - 1;
	//long long limit = atoll(argv[1]);

	//launch thread
	pthread_t tid[num_args];
	struct sum_runner_struct args[num_args];
	
	for (int i=0; i<num_args; i++){
		args[i].limit = atoll(argv[i+1]);

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[i], &attr, sum_runner, &args[i]);
	}

	//wait until thread is done
	for(int i=0; i<num_args; i++){
		pthread_join(tid[i], NULL);
		printf("Sum for thread %d is %lld\n", i, args[i].answer);	
	}

	time_t seconds2;
	time(&seconds2);
	time_t result = seconds2 - seconds1;
	//printf("First time is: %ld\n", seconds1);
	//printf("Second time is: %ld\n", seconds2);
	printf("Difference in time is: %ld\n", result);

}
