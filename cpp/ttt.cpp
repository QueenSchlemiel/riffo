#include <iostream>
#include <string>
#include <sstream>

using namespace std;
int board[6][7];

class Player{
	public:
	string name;
	int score;

	void display(){
		cout << name <<  ": " << score;
	}

	void setName(string input){
		name = input;
	}

	void setScore(){
		score = 0;
	}

	void updateScore(int points){
		score += points;
	}
};

void initializeBoard(){
	for(int i=0; i<6; i++){
		for(int j=0; j<7; j++){
			board[i][j] = 0;
		}
	}
}

void printBoard(){
	cout << "\n-------\n";
	for(int i=0; i<6; i++){
		for(int j=0; j<7; j++){
			cout << board[i][j];
		}
		cout << "\n";
	}
	cout << "-------\n";
}

void check(int col){

}

int main(){
	
	Player p1;
	Player p2;

	string input;
	int num;

	//inialize player data
	cout << "What is player one's name? ";
	getline(cin, input);
	p1.setName(input);
	p1.setScore();
	cout << "What is player two's name? ";
	getline(cin, input);
	p2.setName(input);
	p2.setScore();

	p1.display();
	cout << "\n";
	p2.display();

	initializeBoard();
	printBoard();

	cout << "Flipping a coin to see who goes first...";
	cout << "Please enter 0 for heads or 1 for tails: ";
	cin >> num;
	while(1){
		if(cin.fail()){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
			cout<<"The input is not an integer"<<endl;
			cin>>num;
		}
		if(!cin.fail()){
			break;
		}
	}


}
