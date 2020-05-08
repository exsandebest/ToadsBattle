#include "game.h"
#include "ui_game.h"
#include "basicstructures.h"
#include "bots.h"
#include "qmath.h"
#include <QPushButton>


extern int botLevel;

const int fieldSize = 8;
const int stepSize = 2;
const int cloneSize = 1;
const int grabSize = 1;
const QString playerSprite = ":/src/img/sprite_player.png";
const QString botSprite = ":/src/img/sprite_bot_" + QString::number(botLevel) + ".png";
const QString emptyCellSprite = ":/src/img/sprite_empty_cell.png";
const QString protectedCellSprite = ":/src/img/sprite_protected_cell.png";
toadsBattleBots *bot = new toadsBattleBots(); // fieldSize, botLevel, 2

int playerScore = 2;
int botScore = 2;
int selectionState = 0;
int step = 0;
QPoint selected;

QPushButton * field[fieldSize][fieldSize];


Game::Game(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    this->showMaximized();
    for (int i = 0; i < fieldSize; ++i){
        for (int j = 0; j < fieldSize; ++j){
            QPushButton * btn = new QPushButton();
            btn->setFlat(true);
            btn->setMaximumSize(QSize(100,100));
            btn->setProperty("coords", QPoint(i, j));
            btn->setProperty("state", emptyCell);
            btn->setObjectName("btnGame");
            btn->setIcon(QIcon(emptyCellSprite));
            btn->setStyleSheet("border-radius: 5px; border: 2px solid black;");
            QObject :: connect(btn, SIGNAL(clicked()), this, SLOT(btnGameClicked()));
            field[i][j] = btn;
            ui->layout_game->addWidget(btn, i,j);
        }
    }
    field[0][0]->setProperty("state", firstPlayerCell);
    field[0][fieldSize-1]->setProperty("state", firstPlayerCell);
    field[fieldSize-1][0]->setProperty("state", secondPlayerCell);
    field[fieldSize-1][fieldSize-1]->setProperty("state", secondPlayerCell);
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
        if (propertyState == protectedCell || propertyState == secondPlayerCell ||
                !checkAccess(selected, btn->property("coords").toPoint())) return;
        if (propertyState == firstPlayerCell){
            setOriginalBorderColor(selected);
            selected = btn->property("coords").toPoint();
            setSelectedBorderColor(selected);
            return;
        }
        // propertyState == emptyCell
        makeStep(selected, btn->property("coords").toPoint());
        checkEnd();
        class step botStep = bot->nextStep(fieldToNum(), fieldSize, botLevel, 2); // fieldtoNum()
        makeStep(QPoint(botStep.beginPoint.x, botStep.beginPoint.y), QPoint(botStep.endPoint.x, botStep.endPoint.y));
        checkEnd();
    }
}

bool Game::checkAccess(QPoint p1, QPoint p2){
    return (abs(p1.x() - p2.x()) <= stepSize && abs(p1.y() - p2.y()) <= stepSize);
}

void Game::setOriginalBorderColor(QPoint p){
    field[p.x()][p.y()]->setStyleSheet("border: 2px solid black;");
}

void Game::setSelectedBorderColor(QPoint p){
    field[p.x()][p.y()]->setStyleSheet("border: 2px solid red;");
}

void Game::makeStep(QPoint p1, QPoint p2){
    QPushButton * btn1 = field[p1.x()][p1.y()];
    QPushButton * btn2 = field[p2.x()][p2.y()];
    btn2->setProperty("state", btn1->property("state"));
    int target;
    if (btn1->property("state").toInt() == firstPlayerCell){
        setPlayerIcon(p2);
        target = secondPlayerCell;
    } else {
        setBotIcon(p2);
        target = firstPlayerCell;
    }

    if (abs(p1.x() - p2.x()) > cloneSize && abs(p2.x() - p2.y()) > cloneSize){
        btn1->setProperty("state", emptyCell);
        setEmptyIcon(p1);
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
                    setBotIcon(QPoint(i, j));
                    ++botScore;
                } else {
                    setPlayerIcon(QPoint(i, j));
                    ++playerScore;
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

void Game::setEmptyIcon(QPoint p) {
    field[p.x()][p.y()]->setIcon(QIcon(emptyCellSprite));
}

void Game::setPlayerIcon(QPoint p) {
    field[p.x()][p.y()]->setIcon(QIcon(playerSprite));
}

void Game::setBotIcon(QPoint p){
    field[p.x()][p.y()]->setIcon(QIcon(botSprite));
}

void Game::checkEnd(){
    for (int i = 0; i < fieldSize; ++i){
        for (int j = 0; j < fieldSize; ++j){
            if (field[i][j]->property("state").toInt() == emptyCell) return;
        }
    }
    //win someone
    this->close();
}
