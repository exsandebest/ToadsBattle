#include "bots.h"

toadsBattleBots::toadsBattleBots(int tSize, int complexity, int number){
	tableSize = tSize;
	botComplexity = complexity;
	playerNumber = number;
};

toadsBattleBots::toadsBattleBots(){
	tableSize = 0;
	botComplexity = 1;
	playerNumber = 2;
};

step toadsBattleBots::nextStep(const std::vector<std::vector<int> > &table){
	vector<step> steps = getStepsPool(table, playerNumber);

	return steps[steps.size() - 1]	
};

std::vector<std::vector<bool> > toadsBattleBots::getControlTable(const std::vector<std::vector<int> >& table,const  int queryPlayerNumber) {
    std::vector<std::vector<bool> > controlTable(tableSize);
    for (int i = 0; i < tableSize; ++i)
    	queryResult[i].resize(tableSize, false);

	for (int i = 0; i < tableSize; ++i) {
		for (int j = 0; j < tableSize; ++j) {
			if (table[i][j] == queryPlayerNumber) {
				for (int di = -2; di < 3 && i + di < tableSize && i + di >= 0; ++di) {
					for (int dj = -2; dj < 3 && j + dj < tableSize && j + dj >= 0; ++dj) {
						if (table[i + di][j + dj] == emptyCell || table[i + di][j + dj] == queryPlayerNumber)
							controlTable[i][j] = true; 
					}
				}
			}
		}
	}
	return controlTable;
}

std::vector<step> toadsBattleBots::getStepsPool(const std::vector<std::vector<int> >& table,const int queryPlayerNumber) {
	std::vector<step> queryResult;

	for (int i = 0; i < tableSize; ++i) {
		for (int j = 0; j < tableSize; ++j) {
			if (table[i][j] == queryPlayerNumber) {
				for (int di = -2; di < 3; ++di) {
					for (int dj = -2; dj < 3; ++dj) {
						if (table[i + di][j + dj] == emptyCell)
							queryResult.push_back(step(i, j, i + di, j + dj));
					}
				}
			}
		}
	}
	return queryResult;
}

std::vector<std::vector<int> > getAfterStepTable(const step playerStep, const std::vector<std::vector<int> >& table, const int queryPlayerNumber) {
	std::vector<std::vector<int> > queryResult  = table;
	
	// if (abs())
}

std::vector<std::vector<int> > 


enum {emptyCell = 0, firstPlayerCell = 1, secondPlayerCell = 2, protectedCell = 3};

	double countPlayerCells(const std::vector<std::vector<int> >& table);
	double countPlayerInfluence(const std::vector<std::vector<int> >& table);
	
	double firstLevelBotWeightFunction(const step probablyNextStep, const std::vector<std::vector<int> >& table);
	double secondLevelBotWeightFunction(const step probablyNextStep, const std::vector<std::vector<int> >& table);
	double thirdLevelBotWeightFunction(const step probablyNextStep, const std::vector<std::vector<int> >& table);