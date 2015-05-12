#include <stdio.h>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace std;

typedef pair<int,int> pii;

list<int> possibleShots;
list<int> possiblePlayerNumbers;

bool picked[10] = {0};
int tenPowers[4] = {1,10,100,1000};
int tmp;
int myNum;
int myNumber[10] = {0};
vector<pii> shotResults;
vector<int> myShots;

void transformNumberToDigArray(int number, int* arr) {
	for(int i = 0; i < 10; i++)
		arr[i] = -1;
	for(int i = 0; i < 4; i++) {
		int digit = number % 10;
		arr[digit] = i;
		number /= 10;
	}
}

void generatePossibleShots(int lvl) {
	if(lvl == -1) {
		possibleShots.push_back(tmp);
		return;
	}

	for(int i = 0; i < 10; i++)
		if(!picked[i]) {
			if(lvl == 3 && i == 0)
				continue;
			picked[i] = 1;
			tmp += i * tenPowers[lvl];
			generatePossibleShots(lvl - 1);
			tmp -= i * tenPowers[lvl];
			picked[i] = 0;
		}
	return;
}

void rememberMyNumber(int number) {
	transformNumberToDigArray(number, myNumber);
}

void generateMyNumber() {
	int randomNumber = rand() % possibleShots.size();
	list<int>::iterator it = possibleShots.begin();
	for(int i = 0; i < randomNumber; i++)
		it++;
	myNum = *it;
	rememberMyNumber(*it);
}

void initializeNewGame() {
	generateMyNumber();
	possiblePlayerNumbers = list<int>(possibleShots.begin(), possibleShots.end());
	shotResults.clear();
	myShots.clear();
}

pii getResult(int shot) {
	int playersNumber[10] = {0};
	transformNumberToDigArray(shot, playersNumber);
	pii res(0,0);
	for(int i = 0; i < 10; i++)
		if(playersNumber[i] != -1 && myNumber[i] != -1) {
			res.first++;
			if(playersNumber[i] == myNumber[i])
				res.second++;
		}
	return res;
}

void win(int winner) {
	if(winner == 2)
		printf("No possible numbers can be found :( .\nEither you have mistaken somewhere or I have a bad creator.\nIf you are sure your answers were correct please send feedback to the creator to vanvector@gmail.com\n");
	else if(winner == 1)
		printf("Yahoo! I won!\nThank you for playing. My number was %d.\n", myNum);
	else
		printf("Congratulations! You are the winner!\n");
	printf("Do you want to play again? (y/n)\n");
	char s[560];
	gets(s);
	if(s[0] == 'n' || s[0] == 'N')
		exit(0);
	initializeNewGame();
}

pii getShotResult(int* a, int *b) {
	pii res(0,0);
	for(int i = 0; i < 10; i++)
		if(a[i] != -1 && b[i] != -1) {
			res.first++;
			if(a[i] == b[i])
				res.second++;
		}
	return res;
}

int takeShot() {
	srand( time(NULL) );
	vector<int> shotsToTake;
	if(possiblePlayerNumbers.size() == 1)
		return *possiblePlayerNumbers.begin();
	if(!myShots.size()) {
		shotsToTake = vector<int>(possiblePlayerNumbers.begin(), possiblePlayerNumbers.end());
		int n = rand() % shotsToTake.size();
		return shotsToTake[n];
	}
	list<int>::iterator itPS = possibleShots.begin();
	int total = possiblePlayerNumbers.size();
	int maxCoeff = -1;
	for(; itPS != possibleShots.end(); itPS++) {
		int PSArr[10] = {0};
		transformNumberToDigArray(*itPS, PSArr);
		int possibleResults[5][5] = {0};
		list<int>::iterator itPN = possiblePlayerNumbers.begin();	
		for(; itPN != possiblePlayerNumbers.end(); itPN++) {
			int PNArr[10] = {0};
			transformNumberToDigArray(*itPN, PNArr);
			pii ans = getShotResult(PSArr,PNArr);
			possibleResults[ans.first][ans.second]++;
		}
		int tempCoeff = 0;
		for(int i = 0; i < 5; i++)
			for(int j = 0; j < 5; j++)
				tempCoeff += possibleResults[i][j] * (total - possibleResults[i][j]);
		if(tempCoeff > maxCoeff) {
			maxCoeff = tempCoeff;
			shotsToTake.clear();
			shotsToTake.push_back(*itPS);
		} else if(tempCoeff == maxCoeff) {
			shotsToTake.push_back(*itPS);
		}
	}

	vector<int> shotsToTakePossible;
	for(int i = 0 ; i < shotsToTake.size(); i++) {
		list<int>::iterator it = possiblePlayerNumbers.begin();
		for(; it != possiblePlayerNumbers.end(); it++)
			if(shotsToTake[i] == *it)
				shotsToTakePossible.push_back(shotsToTake[i]);
	}
	if(shotsToTakePossible.size()) {
		int n = rand() % shotsToTakePossible.size();
		return shotsToTakePossible[n];
	}
	int n = rand() % shotsToTake.size();
	return shotsToTake[n];
}

int lastShotArr[10] = {0};

bool numberDoesntFitsLastResult(const int& value) {
	pii lastResult = shotResults[shotResults.size() - 1];
	int thisNumArr[10];
	transformNumberToDigArray(value, thisNumArr);

	pii expectedAns(0,0);
	for(int i = 0; i < 10; i++)
		if(thisNumArr[i] != -1 && lastShotArr[i] != -1) {
			expectedAns.first++;
			if(thisNumArr[i] == lastShotArr[i])
				expectedAns.second++;
		}

	if(lastResult.first != expectedAns.first || lastResult.second != expectedAns.second)
		return true;
	else
		return false;
}

void analizeLastShotResult() {
	int lastShot = myShots[myShots.size() - 1];
	transformNumberToDigArray(lastShot, lastShotArr);
	possiblePlayerNumbers.remove_if(numberDoesntFitsLastResult);
	if(possiblePlayerNumbers.size() == 0)
		win(2);
}

int main() {
	srand ( time(NULL) );
	generatePossibleShots(3);
	initializeNewGame();
	while(1) {
		printf("Please, take a shot.\n");
		char s[560];
		gets(s);
		int shot;
		sscanf(s,"%d",&shot);
		int found = 0;
		list<int>::iterator it = possibleShots.begin();
		for(; it != possibleShots.end(); it++)
			if(*it == shot) {
				found = 1;
				break;
			}
		if(!found) {
			printf("Wrong input.\n");
			continue;
		}
		else {
			pii res = getResult(shot);
			printf("Result: %d %d.\n", res.first, res.second);
			if(res.first == 4 && res.second == 4) {
				win(0);
				generateMyNumber();
				continue;
			}
		}
		printf("Let me think for a while...\n");
		int myShot = takeShot();
		myShots.push_back(myShot);
		printf("My shot is %d. Tell me the answer, for example '4 1'.\n",myShot);
		pii shotResult;
		while(1) {
			shotResult.first = shotResult.second = -1;
			gets(s);
			sscanf(s,"%d %d",&shotResult.first,&shotResult.second);
			bool answerIsOk = 1;
			if(shotResult.first >= shotResult.second 
				&& shotResult.first >= 0 && shotResult.first <= 4 
				&& shotResult.second >= 0 && shotResult.second <= 4 
				&& !(shotResult.first == 4 && shotResult.second == 3))
				answerIsOk = 1;
			else
				answerIsOk = 0;
			if(answerIsOk)
				break;
			else {
				printf("Invalid answer. Try again. I shoot '%d'.\n",myShot);
			}
		}
		shotResults.push_back(shotResult);
		if(shotResult.first == 4 && shotResult.second == 4) {
			win(1);
			continue;
		}
		analizeLastShotResult();
	}
	return 0;
}