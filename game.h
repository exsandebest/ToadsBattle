#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <bots.h>

namespace Ui {
class Game;
}

class Game : public QDialog {
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr, int lvl = 1);
    ~Game();

private slots:
    void btnGameClicked();
    void on_btn_skip_clicked();

private:
    //constants
    const bool debug = false;
    const static int fieldSize = 8;
    const int stepSize = 2;
    const int cloneSize = 1;
    const int grabSize = 1;
    const int maxProtectedCellCount = 3;
    const int animationDuration = 150; //ms
    const int standardDelay = 600; //ms
    const int dialogTimeout = 7; //s
    const QString playerCellSprite = ":img/cell_player.png";
    const QString emptyCellSprite = ":img/cell_empty.png";
    const QString protectedCellSprite = ":img/cell_protected.png";
    QString botCellSprite;

    Ui::Game *ui;
    int botLevel;
    toadsBattleBots *bot;
    QPushButton * field[fieldSize][fieldSize];

    int playerScore = 0;
    int botScore = 0;
    int selectionState = 0;
    int gameResult = 0;
    bool skipClicked = false;
    QPoint selected;

    bool checkAccess(QPoint, QPoint);
    void updateScore(int, int);
    void makeStep(QPoint, QPoint);
    bool checkEnd();
    std::vector<std::vector<int>> fieldToNum();

    void changeIcon(QPushButton *, QIcon);
    void setOriginalBorderColor(QPoint);
    void setSelectedBorderColor(QPoint);
    void setEmptyCell(QPoint);
    void setPlayerCell(QPoint);
    void setBotCell(QPoint);
    void setProtectedCell(QPoint);
};

#endif // GAME_H
