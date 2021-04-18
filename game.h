#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include <vector>

namespace Ui {
class Game;
}

class Game : public QDialog {
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

private slots:
    void btnGameClicked();
    void on_btn_skip_clicked();

private:
    Ui::Game *ui;
    bool checkAccess(QPoint, QPoint);
    void setOriginalBorderColor(QPoint);
    void setSelectedBorderColor(QPoint);
    void makeStep(QPoint, QPoint);
    std::vector<std::vector<int>> fieldToNum();
    void setEmptyCell(QPoint);
    void setPlayerCell(QPoint);
    void setBotCell(QPoint);
    void setProtectedCell(QPoint);
    bool checkEnd();
    void changeIcon(QPushButton *, QIcon);
    void updateScore(int, int);
    int playerScore = 0;
    int botScore = 0;
    int selectionState = 0;
    int gameResult = 0;
};

#endif // GAME_H
