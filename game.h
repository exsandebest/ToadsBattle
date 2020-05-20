#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <basicstructures.h>
#include <vector>

namespace Ui {
class Game;
}

class Game : public QDialog
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    bool checkAccess(QPoint, QPoint);
    void setOriginalBorderColor(QPoint);
    void setSelectedBorderColor(QPoint);
    void makeStep(QPoint, QPoint);
    std::vector<std::vector<int>> fieldToNum();
    void setEmptyCell(QPoint);
    void setPlayerCell(QPoint);
    void setBotCell(QPoint);
    void checkEnd();
    void btnAnimation(QPushButton *, int);
    void updateScore(int, int);

private slots:
    void btnGameClicked();

    void on_btn_skip_clicked();

private:
    Ui::Game *ui;
};

#endif // GAME_H
