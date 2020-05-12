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
    //constructors & destructors
    toadsBattleBots();
    toadsBattleBots(int tSize, int complexity, int number);
    ~toadsBattleBots();

    //main method
    step nextStep(const std::vector<std::vector<int> >& table);
private:
    //constants
    const double POTENTIAL_DANGER_FACTOR = 0.5;
    const double RANDOM_FACTOR = 0.01;
    const double INF = 1e18;
    const double EPS_MACHINE = 1e-7;
    //variables
    int tableSize, botComplexity, playerNumber;
    double (toadsBattleBots::*predictionFunction)(const step&, const  std::vector<std::vector<int> >&);

    //get functions
    std::vector<step> getStepsPool(const std::vector<std::vector<int> >& table,int queryPlayerNumber);
    std::vector<std::vector<bool> > getControlTable(const std::vector<std::vector<int> >& table,int queryPlayerNumber);
    std::vector<std::vector<int> > getAfterStepTable(const step& playerStep, const std::vector<std::vector<int> >& table, int queryPlayerNumber);

    //characteristic count functions
    double countPlayerCells(const std::vector<std::vector<int> >& table, int queryPlayerNumber);
    double countPlayerControl(const std::vector<std::vector<bool> >& controlTable, int queryPlayerNumber);
    double countPlayerControl(const std::vector<std::vector<int> >& table, int queryPlayerNumber);

    //prediction functions
    double firstLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> >& table);
    double secondLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> >& table);
    double thirdLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> >& table);
};

#endif