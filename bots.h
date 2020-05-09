#ifndef TOADS_BATTLE_BOTS_H
#define TOADS_BATTLE_BOTS_H

//includes begin
#include <vector>
#include "basicstructures.h"
//includes end

enum {emptyCell = 0, firstPlayerCell = 1, secondPlayerCell = 2, protectedCell = 3};

class toadsBattleBots
{
public:
	toadsBattleBots();
	toadsBattleBots(int tSize, int complexity, int number);
	~toadsBattleBots();
	step nextStep(const std::vector<std::vector<int> >& table);
private:
	const double POTENTIAL_DANGER_FACTOR = 0.5;
	const double RANDOM_FACTOR = 0.01;

	int tableSize, botComplexity, playerNumber;
		
	std::vector<step> getStepsPool(const std::vector<std::vector<int> >& table);
	std::vector<std::vector<bool> > getInfluenceTable(const std::vector<std::vector<int> >& table);
	double countPlayerCells(const std::vector<std::vector<int> >& table);
	double countPlayerInfluence(const std::vector<std::vector<int> >& table);
	
	double firstLevelBotWeightFunction(const step probablyNextStep, const std::vector<std::vector<int> >& table);
	double secondLevelBotWeightFunction(const step probablyNextStep, const std::vector<std::vector<int> >& table);
	double thirdLevelBotWeightFunction(const step probablyNextStep, const std::vector<std::vector<int> >& table);
};

#endif