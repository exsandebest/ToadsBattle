#include "game.h"
#include "ui_game.h"
#include "basicstructures.h"
#include "bots.h"
#include "qmath.h"
#include <QPushButton>
#include "end.h"
#include <QPropertyAnimation>
#include <QDebug>
#include <QThread>

extern int botLevel;

const bool debug = false;

const int fieldSize = 8;
const int stepSize = 2;
const int cloneSize = 1;
const int grabSize = 1;
const int animationDuration = 150;
const int standardDelay = 600;
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
    ui->lbl_player->setPixmap(QPixmap(":img/toad_main.png").scaled(ui->lbl_player->size(), Qt::KeepAspectRatio));
    ui->lbl_bot->setPixmap(QPixmap(":img/toad_" + QString::number(botLevel) + ".png").scaled(ui->lbl_bot->size(), Qt::KeepAspectRatio));
    for (int i = 0; i < fieldSize; ++i){
        for (int j = 0; j < fieldSize; ++j){
            QPushButton * btn = new QPushButton();
            btn->setFlat(true);
            btn->setMaximumSize(QSize(100,100));
            btn->setProperty("coords", QPoint(i, j));
            btn->setObjectName("btnGame");
            btn->setIconSize(QSize(100, 100));
            btn->setIcon(QIcon(emptyCellSprite));
            btn->setProperty("state", emptyCell);
            QObject :: connect(btn, SIGNAL(clicked()), this, SLOT(btnGameClicked()));
            field[i][j] = btn;
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
    if (selectionState == -1) return;
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
        selectionState = -1;
        setOriginalBorderColor(selected);
        makeStep(selected, btn->property("coords").toPoint());
        checkEnd();
        QApplication::processEvents();
        QThread::msleep(standardDelay / botLevel);
        class step botStep = bot->nextStep(fieldToNum());
        setSelectedBorderColor(QPoint(botStep.beginPoint.x, botStep.beginPoint.y));
        QApplication::processEvents();
        QThread::msleep(standardDelay / botLevel);
        setOriginalBorderColor(QPoint(botStep.beginPoint.x, botStep.beginPoint.y));
        QApplication::processEvents();
        makeStep(QPoint(botStep.beginPoint.x, botStep.beginPoint.y), QPoint(botStep.endPoint.x, botStep.endPoint.y));
        checkEnd();
        QApplication::processEvents();
        selectionState = 0;
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
    int me = btn1->property("state").toInt();
    int target;
    if (me == firstPlayerCell){
        target = secondPlayerCell;
    } else {
        target = firstPlayerCell;
    }
    if (abs(p1.x() - p2.x()) > cloneSize || abs(p1.y() - p2.y()) > cloneSize){
        setEmptyCell(p1);
    } else {
        if (me == firstPlayerCell){
            updateScore(1, 1);
        } else {
            updateScore(2, 1);
        }
    }
    btn2->setProperty("state", me);
    if (me == firstPlayerCell){
        if (debug) qDebug() << "Player step:" << p1 << p2;
        setPlayerCell(p2);
        target = secondPlayerCell;
    } else {
        if (debug) qDebug() << "Bot step:" << p1 << p2;
        setBotCell(p2);
        target = firstPlayerCell;
    }
    for (int i = qMax(0, p2.x() - grabSize); i <= qMin(fieldSize - 1, p2.x() + grabSize); ++i){
        for (int j = qMax(0, p2.y() - grabSize); j <= qMin(fieldSize - 1, p2.y() + grabSize); ++j){
            if (field[i][j]->property("state").toInt() == target){
                if (target == firstPlayerCell){
                    if (debug) qDebug() << "Transformation : Player -> Bot : " << QPoint(i, j);
                    setBotCell(QPoint(i, j));
                    updateScore(2, 1);
                    updateScore(1, -1);
                } else {
                    if (debug) qDebug() << "Transformation : Bot -> Player : " << QPoint(i, j);
                    setPlayerCell(QPoint(i, j));
                    updateScore(1, 1);
                    updateScore(2, -1);
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

void Game::updateScore(int player, int k){
    if (player == 1){
        playerScore += k;
        ui->lbl_playerScore->setText(QString::number(playerScore));
    } else if (player == 2){
        botScore += k;
        ui->lbl_botScore->setText(QString::number(botScore));
    }
}

void Game::btnAnimation(QPushButton * btn, int type){
    QPropertyAnimation * animation = new QPropertyAnimation(btn, "iconSize");
    animation->setDuration(animationDuration);
    if (type == 1){
        animation->setStartValue(QSize(100, 100));
        animation->setEndValue(QSize(50, 50));
    } else if (type == 2) {
        animation->setStartValue(QSize(50, 50));
        animation->setEndValue(QSize(100, 100));
    }
    animation->start();
    while (animation->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
    delete animation;
}

void Game::setProtectedCell(QPoint p){
    QPushButton * btn = field[p.x()][p.y()];
    btnAnimation(btn, 1);
    btn->setIcon(QIcon(protectedCellSprite));
    btnAnimation(btn, 2);
    btn->setProperty("state", protectedCell);
}

void Game::setEmptyCell(QPoint p) {
    QPushButton * btn = field[p.x()][p.y()];
    btnAnimation(btn, 1);
    btn->setIcon(QIcon(emptyCellSprite));
    btnAnimation(btn, 2);
    btn->setProperty("state", emptyCell);
}

void Game::setPlayerCell(QPoint p) {
    QPushButton * btn = field[p.x()][p.y()];
    btnAnimation(btn, 1);
    btn->setIcon(QIcon(playerCellSprite));
    btnAnimation(btn, 2);
    btn->setProperty("state", firstPlayerCell);
}

void Game::setBotCell(QPoint p){
    QPushButton * btn = field[p.x()][p.y()];
    btnAnimation(btn, 1);
    btn->setIcon(QIcon(botCellSprite));
    btnAnimation(btn, 2);
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
