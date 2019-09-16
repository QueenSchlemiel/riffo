#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <features.h>

struct prime_struct {
	int number;
	bool isPrime;
	//pthread_t test;	//for testing purposes
};

struct time_spec {
	time_t tv_sec;
	long tv_nsec;
};

/* 
 * calculates (a * b) % c taking into account that a * b might overflow 
 */
long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {    
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
/* 
 * modular exponentiation
 */
long long modulo(long long base, long long exponent, long long mod)
{
    long long x = 1;
    long long y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
 
/*
 * Miller-Rabin Primality test, iteration signifies the accuracy
 */

void* Miller(void* arg)
{
	
     struct prime_struct *number = (struct prime_struct*) arg;	

     //for testing purposes
     /*	
  	pthread_t self;
	  if((self = pthread_self()) == number->test) {
		  //printf("pthread id: %ld\n", number->test);
		  sleep(20);
	  }
    */

    long long s;

    long long p = number->number;

    if (p <= 2)
    {
        number->isPrime = false;
      	return;
    }
    if (p != 2 && p % 2==0)
    {
        number->isPrime = false;
      	return;
    }
    if (p != 3 && p % 3 ==0)
    {
        number->isPrime = false;
        return;
    }
    if (p != 5 && p % 5 ==0)
    {
        number->isPrime = false;
	      return;
    }
    s = p - 1;

    while (s % 2 == 0)
    {
        s /= 2;
    }

    long long a = rand() % (p - 1) + 1, temp = s;
    long long mod = modulo(a, temp, p);
    while (temp != p - 1 && mod != 1 && mod != p - 1)
    {
        mod = mulmod(mod, mod, p);
        temp *= 2;
    }
    if (mod != p - 1 && temp % 2 == 0)
    {
        number->isPrime = false;
    }
    number->isPrime = true;
}
//Main
int main(int argc, char **argv)
{
	struct timespec ts;
	int s;

	if(clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		printf("could not get time\n");
		exit(-1);
	}

	time_t seconds;
	time(&seconds);
	
	if(argc<2){
		printf("Usage: %s <num1><num2>...<num-n>\n", argv[0]);
		exit(-1);
	}

  long long num = atoll(argv[1]);

	int usr_time;
	int flag = 0;

	if(argc == 3){
		usr_time = atoi(argv[2]);
		flag = 1;
	}	    

	struct prime_struct arg;
	pthread_t tid[4];

	arg.number = num;
	arg.isPrime = false;

	for(int i = 0; i < 4; i++) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[i], &attr, Miller, &arg);
		
		//for testing purposes
		/*
		if(i == 0) {
			arg.test = tid[i];
		}*/
	}
	
	//if user types a time limit
	if(flag == 1) {
		int counter = 0;
		ts.tv_sec += usr_time;

		for(int i = 0; i < 4; i++) {
			
			printf("timeset out: %ld\n", ts.tv_sec);
			s = pthread_timedjoin_np(tid[i], NULL, &ts);
			
			if(s == 0) {
				counter++;
			}
			else if(s != 0) {
				if(counter > 0) {
					pthread_cancel(tid[i]);
					break;
				}
				else {	
					printf("unknown\n");
					exit(0);
				}
			}
							
		}	
	} else {
		for(int i = 0; i < 4; i++) {
			pthread_join(tid[i], NULL);
		}
	}

    	if (arg.isPrime == true){
        	printf("prime detected\n");
    	}else{
        	printf("composite detected\n");
    	}

	time_t seconds2;
	time(&seconds2);
	time_t result = seconds2 - seconds;
	printf("Program run time is: %ld\n", result);
    
	return 0;
}
