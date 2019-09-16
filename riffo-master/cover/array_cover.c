/**************************
 * Name: Laura Riffo
 * Last Modified: 1/18/19
 * ************************/

#include <stdio.h>
void print_matrix(int* arr, int len){
	int index;
	for(index=0; index<len*len; index++){
		printf("%d", arr[index]);
		if(index%len==(len-1)){
			printf("\n");
		}
	}
	printf("\n");
}

void addID(int* arr, int len, int first, int val){
	int index;
	if(first%len==(len-1) || first%len==(len-2)){
		for(index=1; first-index>=0; index++){
			arr[first-index] = val;
			print_matrix(arr,len);
		}
		for(index=1; first+index<len*len; index++){
			arr[first+index] = val;
			print_matrix(arr,len);
		}
	}
	else if(first%len==0 || first%len==1){
		for(index=1; first+index<len*len; index++){
			arr[first+index] = val;
			print_matrix(arr,len);
		}
		for(index=1; first-index>=0; index++){
			arr[first-index] = val;
			print_matrix(arr,len);
		}
	}
	else{
		for(index=1; (first-index)%len!=(len-1); index++){
			arr[first-index] = val;
			print_matrix(arr,len);
		}
		for(index=1; first+index<len*len; index++){
			arr[first+index] = val;
			print_matrix(arr,len);
		}
		for(index=1; first-index>=0; index++){
			if(arr[first-index]!=val){
				arr[first-index] = val;
				print_matrix(arr,len);
			}
		}
	}
}

int main(int argc, char *argv[]){
	int k,i,id;
	
	k = atoi(argv[1]);
	i = atoi(argv[2]);
	id = atoi(argv[3]);

	int* arr = (int*) malloc(k*k*sizeof(int));
	memset(arr,0,k*k*sizeof(int));

	print_matrix(arr,k);
	arr[i] = id;
	print_matrix(arr,k);

	addID(arr,k,i,id);

	free(arr);
	return 0;
}
