#include "bots.h"
#include <algorithm>
#include <random>

namespace toadsBattleBotsRandom {
    static std::random_device randomSeed;
    static std::mt19937 mersenneRandom(randomSeed());
}

toadsBattleBots::toadsBattleBots(int tSize, int complexity, int number){
    tableSize = tSize;
    analysisDepth = 2 * (complexity - 1);
    playerNumber = number;

    if (complexity == 1)
        predictionFunction = &toadsBattleBots::firstLevelBotPredictionFunction;
    else if (complexity == 2)
        predictionFunction = &toadsBattleBots::secondLevelBotPredictionFunction;
    else
        predictionFunction = &toadsBattleBots::thirdLevelBotPredictionFunction;
};

toadsBattleBots::toadsBattleBots(){
    tableSize = 0;
    analysisDepth = 0;
    playerNumber = 2;
    predictionFunction = &toadsBattleBots::firstLevelBotPredictionFunction;
};

step toadsBattleBots::nextStep(const std::vector<std::vector<int> > &table) {
    std::vector<step> steps = getStepsPool(table, this -> playerNumber);

    step queryResult = step(-1, -1, -1, -1);
    double bestValue = -INF, tmpValue;
    for (step stepToPredict : steps) {
        tmpValue = deepAnalysis(stepToPredict, table, analysisDepth, playerNumber);
        if (tmpValue > bestValue + EPS_MACHINE) {
            bestValue = tmpValue;
            queryResult = stepToPredict;
        }
    }
    return queryResult;
};

std::vector<std::vector<bool> > toadsBattleBots::getControlTable(const std::vector<std::vector<int> >& table,const  int queryPlayerNumber) {
    std::vector<std::vector<bool> > controlTable(tableSize);
    for (int i = 0; i < tableSize; ++i)
        controlTable[i].resize(tableSize, false);

    for (int i = 0; i < tableSize; ++i) {
        for (int j = 0; j < tableSize; ++j) {
            if (table[i][j] == queryPlayerNumber) {
                for (int di = std::max(-2, -i); di < std::min(3, tableSize - i) ; ++di) {
                    for (int dj = std::max(-2, -j); dj < std::min(3, tableSize - j); ++dj) {
                        if (table[i + di][j + dj] == emptyCell || table[i + di][j + dj] == queryPlayerNumber)
                            controlTable[i][j] = true;
                    }
                }
            }
        }
    }
    return controlTable;
}

std::vector<step> toadsBattleBots::getStepsPool(const std::vector<std::vector<int> >& table, const int queryPlayerNumber) {
    std::vector<step> queryResult;

    for (int i = 0; i < tableSize; ++i) {
        for (int j = 0; j < tableSize; ++j) {
            if (table[i][j] == queryPlayerNumber) {
                for (int di = std::max(-2, -i); di < std::min(3, tableSize - i); ++di) {
                    for (int dj = std::max(-2, -j); dj < std::min(3, tableSize - j); ++dj) {
                        if (table[i + di][j + dj] == emptyCell)
                            queryResult.push_back(step(i, j, i + di, j + dj));
                    }
                }
            }
        }
    }
    return queryResult;
}

std::vector<std::vector<int> > toadsBattleBots::getAfterStepTable(const step& playerStep, const std::vector<std::vector<int> >& table, const int queryPlayerNumber) {
    std::vector<std::vector<int> > queryResult  = table;
    int enemyPlayerNumber = queryPlayerNumber % 2 + 1;
    if (std::max(std::abs(playerStep.beginPoint.x - playerStep.endPoint.x), std::abs(playerStep.beginPoint.y - playerStep.endPoint.y)) == 2)
        queryResult[playerStep.beginPoint.x][playerStep.beginPoint.y] = emptyCell;
    queryResult[playerStep.endPoint.x][playerStep.endPoint.y] = queryPlayerNumber;

    for (int x = std::max(0, playerStep.endPoint.x - 1); x < std::min(tableSize, playerStep.endPoint.x + 2); ++x) {
        for (int y = std::max(0, playerStep.endPoint.y - 1); y < std::min(tableSize, playerStep.endPoint.y + 2); ++y) {
            if (queryResult[x][y] == enemyPlayerNumber)
                queryResult[x][y] = queryPlayerNumber;
        }
    }

    return queryResult;
}

int toadsBattleBots::countPlayerCells(const std::vector<std::vector<int> > &table, int queryPlayerNumber) {
    int result = 0;
    for (int i = 0; i < tableSize; ++i) {
        for (int j = 0; j < tableSize; ++j) {
            if (table[i][j] == queryPlayerNumber)
                ++result;
        }
    }
    return result;
}

inline int  toadsBattleBots::countPlayerControl(const std::vector<std::vector<int> > &table, int queryPlayerNumber) {
    std::vector<std::vector<bool> > controlTable = getControlTable(table, queryPlayerNumber);
    return countPlayerControl(controlTable);
}

int  toadsBattleBots::countPlayerControl(const std::vector<std::vector<bool> > &controlTable) {
    int result = 0;
    for (int i = 0; i < tableSize; ++i) {
        for (int j = 0; j < tableSize; ++j) {
            if (controlTable[i][j])
                ++result;
        }
    }
    return result;
}

double toadsBattleBots::countPartOfNonEmptyCells(const std::vector<std::vector<int> > &table) {
    int numberOfNonEmptyCells = tableSize * tableSize;
    for (int i = 0; i < tableSize; ++i) {
        for (int j = 0; j < tableSize; ++j) {
            if (table[i][j] != emptyCell)
                --numberOfNonEmptyCells;
        }
    }
    return ((double) numberOfNonEmptyCells) / ((double) tableSize) / ((double) tableSize);
}

double toadsBattleBots::countPositionFactor(const int x, const int y) {
    return (((double) std::min(x, tableSize - x - 1)) / (((double) tableSize) / 2.0) + ((double) std::min(y, tableSize - y - 1)) / (((double) tableSize) / 2.0)) / 2.0;
}

double toadsBattleBots::deepAnalysis(const step &presumableStep, const std::vector<std::vector<int> > &table, const int depth, const int playerNum) {
    if (depth == 0)
        return (this ->*predictionFunction)(presumableStep, table);
    std::vector<std::vector<int> > tableAfterStep = getAfterStepTable(presumableStep, table, playerNum);

    std::vector<step> pool = getStepsPool(table, playerNum % 2 + 1);

    if (pool.empty()) {
        if (countPlayerCells(tableAfterStep, playerNumber) > countPlayerCells(tableAfterStep, playerNumber % 2 + 1))
            return 1.0;
        else if (countPlayerCells(tableAfterStep, playerNumber) == countPlayerCells(tableAfterStep, playerNumber % 2 + 1))
            return 0.5;
        else
            return 0.0;
    }

    double medValue = 0.0;

    for (step& stepNow : pool) {
        medValue += deepAnalysis(stepNow, tableAfterStep, depth - 1, playerNum % 2 + 1);
    }
    return medValue / ((double) pool.size());
}

//predictions
double toadsBattleBots::firstLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> > &table) {
    int afterStepCells = countPlayerCells(getAfterStepTable(presumableStep, table, playerNumber), playerNumber);

    return ((double)afterStepCells) * (1.0 - RANDOM_FACTOR_FIRST_BOT) +
    RANDOM_FACTOR_FIRST_BOT * std::generate_canonical<double, 10>(toadsBattleBotsRandom::mersenneRandom);
}

double toadsBattleBots::secondLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> > &table) {
    std::vector<std::vector<int> > updatedTable = getAfterStepTable(presumableStep, table, playerNumber);
    int afterStepCells = countPlayerCells(updatedTable, playerNumber),
        afterStepEnemyCells = countPlayerCells(updatedTable, playerNumber % 2 + 1);
    double positionF = countPositionFactor(presumableStep.endPoint.x, presumableStep.endPoint.y);


    return RANDOM_FACTOR_SECOND_BOT * std::generate_canonical<double, 10>(toadsBattleBotsRandom::mersenneRandom) +
            positionF * POSITION_FACTOR_SECOND_BOT +
            ((double)(afterStepCells - afterStepEnemyCells)) / 6.0 * (1.0 - POSITION_FACTOR_SECOND_BOT - RANDOM_FACTOR_SECOND_BOT);
}

double toadsBattleBots::thirdLevelBotPredictionFunction(const step& presumableStep, const std::vector<std::vector<int> > &table) {
    std::vector<std::vector<int> > updatedTable = getAfterStepTable(presumableStep, table, playerNumber);
    std::vector<std::vector<bool> > updatedControlTable = getControlTable(updatedTable, playerNumber),
                                    updatedEnemyControlTable = getControlTable(updatedTable, playerNumber % 2 + 1);

    int afterStepCells = countPlayerCells(updatedTable, playerNumber),
            afterStepEnemyCells = countPlayerCells(updatedTable, playerNumber % 2 + 1),
            afterStepControl = countPlayerControl(updatedControlTable),
            afterStepEnemyControl = countPlayerControl(updatedEnemyControlTable);
    double positionF = countPositionFactor(presumableStep.endPoint.x, presumableStep.endPoint.y),
           nonEmpty = countPartOfNonEmptyCells(updatedTable),
           dangerFactor = (updatedEnemyControlTable[presumableStep.endPoint.x][presumableStep.endPoint.y]) ? POTENTIAL_DANGER_FACTOR : 1.0;


    return (RANDOM_FACTOR_THIRD_BOT * std::generate_canonical<double, 10>(toadsBattleBotsRandom::mersenneRandom) +
           POSITION_FACTOR_THIRD_BOT * positionF +
           (1.0 - RANDOM_FACTOR_THIRD_BOT - POSITION_FACTOR_THIRD_BOT) * ((1.0 - nonEmpty) * (afterStepControl - afterStepEnemyControl) +
                                                                           nonEmpty * (afterStepCells - afterStepEnemyCells))) * dangerFactor;
}