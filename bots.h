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
	~toadsBattleBots();
	step nextStep(const std::vector<std::vector<int> >& table, const int tableSize, const int botComplexity, const int playerNumber);
private:
	const double POTENTIAL_DANGER_FACTOR = 0.5;
	const double RANDOM_FACTOR = 0.01;
		
	std::vector<step> getStepsPool(const std::vector<std::vector<int> >& table, const int playerNumber);
	std::vector<std::vector<bool> > getInfluenceTable(const std::vector<std::vector<int> >& table, const int playerNumber);
	double countPlayerCells(const std::vector<std::vector<int> >& table, const int playerNumber);
	double countPlayerInfluence(const std::vector<std::vector<int> >& table, const int playerNumber);
	
	double firstLevelBotWeightFunction(const step probablyNextStep);
	double secondLevelBotWeightFunction(const step probablyNextStep);
	double thirdLevelBotWeightFunction(const step probablyNextStep);
};

#endif