#include <stdio.h>
//#include "parent.c"

int main(char *argc, char *argv[]){
	printf("\nInside child method: ");
	//printf("Printing argument 1 from child.c: " + argv[1] + "\n");
	printf(argv[1]);
	printf("\n");

	return 0;	
}
