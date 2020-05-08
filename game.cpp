#include "game.h"
#include "ui_game.h"
#include "basicstructures.h"
#include "bots.h"
#include "math.h"
#include <QPushButton>


extern int botLevel;

const int fieldSize = 8;
const int stepSize = 2;
const QString playerSprite = ":/src/img/sprite_player.png";
const QString botSprite = ":/src/img/sprite_bot_" + QString::number(botLevel) + ".png";
const QString emptyCellSprite = ":/src/img/sprite_empty_cell.png";
const QString protectedCellSprite = ":/src/img/sprite_protected_cell.png";

int playerScore = 2;
int botScore = 2;
int selectionState = 0;
QPoint selected;

QPushButton * field[fieldSize][fieldSize];
int fieldNum[fieldSize][fieldSize];


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
            btn->setStyleSheet("border-radius: 5px;border: 2px normal white;");
            QObject :: connect(btn, SIGNAL(clicked()), this, SLOT(btnGameClicked()));
            field[i][j] = btn;
            fieldNum[i][j] = emptyCell;
            ui->layout_game->addWidget(btn, i,j);
        }
    }
}

Game::~Game()
{
    delete ui;
}

void Game::btnGameClicked(){
    QObject * btn = sender();
    if (selectionState == 0){
        if (btn->property("state") == emptyCell || btn->property("state") == secondPlayerCell || btn->property("state") == protectedCell) return;
        selectionState = 1;
        selected = btn->property("coords").toPoint();
        setSelectedBorderColor(selected);
    } else if (selectionState == 1) {
        if (btn->property("state") == protectedCell || !checkAccess(selected, btn->property("coords").toPoint())) return;
        if (btn->property("state") == firstPlayerCell){
            setOriginalBorderColor(selected);
            selected = btn->property("coords").toPoint();
            setSelectedBorderColor(selected);
            return;
        }
    }
}

bool Game::checkAccess(QPoint p1, QPoint p2){
    return (abs(p1.x() - p2.x()) <= stepSize && abs(p1.y() - p2.y()) <= stepSize);
}

void Game::setOriginalBorderColor(QPoint p){
    QPushButton * btn = field[p.x()][p.y()];
    //Change styleSheet
}

void Game::setSelectedBorderColor(QPoint p){
    QPushButton * btn = field[p.x()][p.y()];
    //Change styleSheet
}

void Game::makeStep(QPoint p1, QPoint p2){
    QPushButton * btn1 = field[p1.x()][p1.y()];
    QPushButton * btn2 = field[p2.x()][p2.y()];
    btn2->setProperty("state", btn1->property("state"));
    //Change icon
    fieldNum[p2.x()][p2.y()] = fieldNum[p1.x()][p1.y()];

    if (abs(p1.x() - p2.x()) > 1 && abs(p2.x() - p2.y()) > 1){
        btn1->setProperty("state", emptyCell);
        //Change icon
        fieldNum[p1.x()][p1.y()] = emptyCell;
    }
    //Update nearest cells
}
