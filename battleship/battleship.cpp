#include <iostream>
#include <string>
#include <fstream>

using namespace std;
char gameboard[25][25];

int checkShips(){
	for(int i=0; i<25; i++){
		for(int j=0; j<25; j++){
			if(gameboard[i][j] == '#'){
				return 1;
			}
		}
	}
}

void print(){
	for(int i=0; i<25; i++){
		for(int j=0; j<25; j++){
			cout << gameboard[i][j];
		}
		cout << "\n";
	}
}

void printPlayerBoard(){
	for(int i=0; i<25; i++){
		for(int j=0; j<25; j++){
			if(gameboard[i][j] == '#'){
				cout << '~';
			}
			else{
				cout << gameboard[i][j];
			}
		}
		cout << "\n";
	}
}

void updateBoard(int x, int y){
	gameboard[x][y] = 'H';
}

void fire(int x, int y){
	if(gameboard[x][y] == '~'){
		cout << "MISS\n";
	}
	else if(gameboard[x][y] == '#'){
		cout << "HIT\n";
		updateBoard(x,y);
	}
}

int main(int argc, char** argv){
	int x,y;
	int notDestroyed = 1;

	//set gameboard to all ~ which is water
	/*for(int i=0; i<25; i++){
		for(int j=0; j<25; j++){
			gameboard[i][j] = '~';
		}
	}*/

	//setting some test variables
	//gameboard[1][2] = '#';
	//gameboard[3][4] = '#';
	//gameboard[2][3] = '#';
	//gameboard[1][3] = '#';

	//set up gameboard from file
	ifstream inFile;
	inFile.open("board.txt");
	if(!inFile){
		cerr << "Unable to open file board.txt";
		//exit(1);
	}

	for(int i=0; i<25; i++){
		for(int j=0; j<25; j++){
			inFile>>gameboard[i][j];
		}
	}
		
	inFile.close();
	print();

	while(notDestroyed==1){
		/*if(argc == 3){
			x = atoi(argv[1]);
			y = atoi(argv[2]);
		}*/

		printPlayerBoard();

		//get input from user
		cout << "Enter an x coord: ";
		cin >> x;
		cout << "Enter a y coord: ";
		cin >> y;

		fire(x,y);
		notDestroyed = checkShips();
	}

	cout << "You have destroyed all ships.\nThanks for playing.";
	return 0;
}

