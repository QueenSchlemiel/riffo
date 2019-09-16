/**************************
 * Name: Laura Riffo 
 * Date: February 18, 2019
 **************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem_helper.h"

void print_matrix(int semid, int len){
	printf("Laura's printed matrix\n");

	int i,j,k;
	int size = len*len;
	short arr[size];
	getAllSemaphores(semid,size,arr);
	//printf("*\n");
	for(i=0;i<len;i++){
		for(j=0;j<len;j++){
			k=arr[(i*len)+j];
			if(k==1||k==2){
				printf("%u", k);
			}
			else{
				printf("0");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void setPosition(int semid, short* arr, int len, int first, int val){
	int index;
	int check = 0;
	
	/*
	for(index=0;index<len*len;index++){
		if(getSemaphore(semid,index)!=val){
			check = 1;
		}
	}
	*/
	//printf("CHECK %d\n", check);


	printf("GLADIATOR ID: %d", val);

	if(first%len==(len-1) || first%len==(len-2)){
		//print_matrix(semid,len);
		for(index=1;first-index>=0;index++){
			setSemaphore(semid, (first-index), val);
			//print_matrix(semid,len);
		}
		for(index=1;first+index<len*len;index++){
			setSemaphore(semid, (first+index), val);
			//print_matrix(semid,len);
		}
		//sleep(100);
	}
	else if(first%len==0 || first%len==1){
		//print_matrix(semid,len);
		for(index=1;first+index<len*len;index++){
			setSemaphore(semid, (first+index), val);
			//print_matrix(semid,len);
		}
		for(index=1;first-index>=0;index++){
			setSemaphore(semid, (first-index), val);
			//print_matrix(semid,len);
		}
		//sleep(100);
	}
	else{
		//print_matrix(semid,len);
		for(index=1;(first-index)%len!=(len-1);index++){
			setSemaphore(semid, (first-index), val);
			//print_matrix(semid,len);
		}
		for(index=1;first+index<len*len;index++){
			setSemaphore(semid, (first+index), val);
			//print_matrix(semid,len);
		}
		for(index=1;first-index>=0; index++){
			setSemaphore(semid, (first-index), val);
			//print_matrix(semid,len);
		}
		for(index=0;index<len*len;index++){
			setSemaphore(semid, index, val);
			//print_matrix(semid,len);
		}
		//setSemaphore(semid, 0, val);
		//sleep(100);
	}
	setSemaphore(semid,0,val);
	//print_matrix(semid,len);
}

int main(int argc, char* argv[]) 
{

	if(argc != 4){
		printf("usage: %s semid gladiator_id arena_len\n", argv[0]);
		return 0;
	}

	int semid = atoi(argv[1]);
        int gladiator_id = atoi(argv[2]);
	int arena_len = atoi(argv[3]);
	int size = arena_len*arena_len;
	
	short arr[size];
	short* array = arr;
	getAllSemaphores(semid, size, array);
	
	//print_matrix(semid, arena_len);

	//printf("Laura's id: %d:", gladiator_id);
	
	int x;
	int start;
	for(x=0; x<size; x++){
		//printf("%d", array[x]);
		if(array[x]==gladiator_id){
			printf("Found at %d", x);
			start = x;
		}
	}
	
	/* Battle */
	printf("Laura print arena\n");
	
	setPosition(semid, array, arena_len, start, gladiator_id);

	return 0;
}

//End of File
