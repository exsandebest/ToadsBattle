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
    const double POTENTIAL_DANGER_FACTOR = 0.97;
    const double POSITION_FACTOR_SECOND_BOT = 0.2;
    const double POSITION_FACTOR_THIRD_BOT = 0.02;
    const double MATCH_TIME_FACTOR = 0.2;
    const double RANDOM_FACTOR_FIRST_BOT = 0.5;
    const double RANDOM_FACTOR_SECOND_BOT = 0.1;
    const double RANDOM_FACTOR_THIRD_BOT = 0.01;
    const double INF = 1e18;
    const double EPS_MACHINE = 1e-7;
   
    //variables
    int tableSize, botComplexity, playerNumber, analysisDepth;
    double (toadsBattleBots::*predictionFunction)(const step&, const  std::vector<std::vector<int> >&);

    //get functions
    std::vector<step> getStepsPool(const std::vector<std::vector<int> >& table,int queryPlayerNumber);
    std::vector<std::vector<bool> > getControlTable(const std::vector<std::vector<int> >& table,int queryPlayerNumber);
    std::vector<std::vector<int> > getAfterStepTable(const step& playerStep, const std::vector<std::vector<int> >& table, int queryPlayerNumber);

    //characteristic count functions
    int countPlayerCells(const std::vector<std::vector<int> >& table, int queryPlayerNumber);
    int countPlayerControl(const std::vector<std::vector<bool> >& controlTable);
    int countPlayerControl(const std::vector<std::vector<int> >& table, int queryPlayerNumber);
    double countPartOfNonEmptyCells(const std::vector<std::vector<int> >& table);
    double countPositionFactor(const int x, const int y);
    
    //deep
    double deepAnalysis(const step& presumableStep, const std::vector<std::vector<int> >& table, const int depth, const int playerNum);

    //prediction functions
    double firstLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> >& table);
    double secondLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> >& table);
    double thirdLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> >& table);
};

#endif