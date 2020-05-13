#include "game.h"
#include "ui_game.h"
#include "basicstructures.h"
#include "bots.h"
#include "qmath.h"
#include <QPushButton>
#include "end.h"
#include <QPropertyAnimation>

extern int botLevel;

const int fieldSize = 8;
const int stepSize = 2;
const int cloneSize = 1;
const int grabSize = 1;
const int animationDuration = 200;
const QString playerCellSprite = ":img/cell_player.png";
const QString emptyCellSprite = ":img/cell_empty.png";
const QString protectedCellSprite = ":img/sprite_protected_cell.png";
QString botCellSprite = "";
toadsBattleBots *bot = new toadsBattleBots(fieldSize, botLevel, 2);

int playerScore = 2;
int botScore = 2;
int selectionState = 0;
int gameResult = 0;
QPoint selected;

QPushButton * field[fieldSize][fieldSize];


Game::Game(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    this->showMaximized();
    botCellSprite = ":img/cell_bot_" + QString::number(botLevel) + ".png";
    for (int i = 0; i < fieldSize; ++i){
        for (int j = 0; j < fieldSize; ++j){
            QPushButton * btn = new QPushButton();
            btn->setFlat(true);
            btn->setMaximumSize(QSize(100,100));
            btn->setProperty("coords", QPoint(i, j));
            btn->setObjectName("btnGame");
            btn->setIconSize(QSize(100, 100));
            QObject :: connect(btn, SIGNAL(clicked()), this, SLOT(btnGameClicked()));
            field[i][j] = btn;
            setEmptyCell(QPoint(i, j));
            setOriginalBorderColor(QPoint(i, j));
            ui->layout_game->addWidget(btn, i,j);
        }
    }
    setPlayerCell(QPoint(0, 0));
    setPlayerCell(QPoint(fieldSize - 1, 0));
    setBotCell(QPoint(0, fieldSize - 1));
    setBotCell(QPoint(fieldSize - 1, fieldSize - 1));
}

Game::~Game()
{
    delete ui;
}

void Game::btnGameClicked(){
    QObject * btn = sender();
    int propertyState = btn->property("state").toInt();
    if (selectionState == 0){
        if (propertyState == emptyCell || propertyState == secondPlayerCell || propertyState == protectedCell) return;
        selectionState = 1;
        selected = btn->property("coords").toPoint();
        setSelectedBorderColor(selected);
    } else if (selectionState == 1) {
        if (propertyState == protectedCell || propertyState == secondPlayerCell) {
            return;
        } else if (propertyState == firstPlayerCell){
            setOriginalBorderColor(selected);
            selected = btn->property("coords").toPoint();
            setSelectedBorderColor(selected);
            return;
        } else if (!checkAccess(selected, btn->property("coords").toPoint())){
            return;
        }
        // propertyState == emptyCell
        setOriginalBorderColor(selected);
        makeStep(selected, btn->property("coords").toPoint());
        checkEnd();
        class step botStep = bot->nextStep(fieldToNum());
        makeStep(QPoint(botStep.beginPoint.x, botStep.beginPoint.y), QPoint(botStep.endPoint.x, botStep.endPoint.y));
        checkEnd();
    }
}

bool Game::checkAccess(QPoint p1, QPoint p2){
    return (abs(p1.x() - p2.x()) <= stepSize && abs(p1.y() - p2.y()) <= stepSize);
}

void Game::setOriginalBorderColor(QPoint p){
    field[p.x()][p.y()]->setStyleSheet("border: 1px solid black;");
}

void Game::setSelectedBorderColor(QPoint p){
    field[p.x()][p.y()]->setStyleSheet("border: 3px solid red;");
}

void Game::makeStep(QPoint p1, QPoint p2){
    QPushButton * btn1 = field[p1.x()][p1.y()];
    QPushButton * btn2 = field[p2.x()][p2.y()];
    btn2->setProperty("state", btn1->property("state"));
    int target;
    if (btn1->property("state").toInt() == firstPlayerCell){
        setPlayerCell(p2);
        target = secondPlayerCell;
    } else {
        setBotCell(p2);
        target = firstPlayerCell;
    }

    if (abs(p1.x() - p2.x()) > cloneSize || abs(p1.y() - p2.y()) > cloneSize){
        setEmptyCell(p1);
    } else {
        if (target == secondPlayerCell){
            ++playerScore;
        } else {
            ++botScore;
        }
    }

    for (int i = qMax(0, p2.x() - grabSize); i <= qMin(fieldSize - 1, p2.x() + grabSize); ++i){
        for (int j = qMax(0, p2.y() - grabSize); j <= qMin(fieldSize - 1, p2.y() + grabSize); ++j){
            if (field[i][j]->property("state").toInt() == target){
                if (target == firstPlayerCell){
                    setBotCell(QPoint(i, j));
                    ++botScore;
                    --playerScore;
                } else {
                    setPlayerCell(QPoint(i, j));
                    ++playerScore;
                    --botScore;
                }
            }
        }
    }
}

std::vector<std::vector<int>> Game::fieldToNum(){
    std::vector<std::vector<int>> v;
    for (int i = 0; i < fieldSize; ++i){
        std::vector<int> t;
        for (int j = 0; j < fieldSize; ++j){
            t.push_back(field[i][j]->property("state").toInt());
        }
        v.push_back(t);
    }
    return v;
}

void Game::setProtectedCell(QPoint p){
    QPushButton * btn = field[p.x()][p.y()];
    btn->setIcon(QIcon(protectedCellSprite));
    btn->setProperty("state", protectedCell);
}

void Game::setEmptyCell(QPoint p) {
    QPushButton * btn = field[p.x()][p.y()];
    btn->setIcon(QIcon(emptyCellSprite));
    btn->setProperty("state", emptyCell);
}

void Game::setPlayerCell(QPoint p) {
    QPushButton * btn = field[p.x()][p.y()];
    btn->setIcon(QIcon(playerCellSprite));
    btn->setProperty("state", firstPlayerCell);
}

void Game::setBotCell(QPoint p){
    QPushButton * btn = field[p.x()][p.y()];
    btn->setIcon(QIcon(botCellSprite));
    btn->setProperty("state", secondPlayerCell);
}

void Game::checkEnd(){
    if (playerScore == 0 || botScore == 0) goto out;
    for (int i = 0; i < fieldSize; ++i){
        for (int j = 0; j < fieldSize; ++j){
            if (field[i][j]->property("state").toInt() == emptyCell) return;
        }
    }
    out:;
    if (playerScore > botScore){
        gameResult = 1;
    } else if (playerScore < botScore) {
        gameResult = -1;
    } else if (playerScore == botScore){
        gameResult = 0;
    }
    End endWindow;
    endWindow.exec();
    this->close();
}
