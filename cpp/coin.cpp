#include <iostream>
#include <stdlib.h>
#include <ctime>

int main(){
	srand(time(NULL));
	int coin = rand()%2;

	//num==0 then heads
	//num!=0 tails
	if(coin==0){
		std::cout<<"Heads\n";
	}
	else{
		std::cout<<"Tails\n";
	}
}
