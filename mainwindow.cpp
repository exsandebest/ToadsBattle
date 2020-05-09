#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rules.h"
#include "dialog.h"
#include "game.h"

int botLevel = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    QPixmap pix(":src/img/backgound_menu.png");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette pal;
    pal.setBrush(QPalette::Background, pix);
    this->setPalette(pal);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_level_clicked()
{
    if (botLevel == 1){
        botLevel = 2;
        ui->btn_level->setText("LEVEL: MEDIUM");
    } else if (botLevel == 2){
        botLevel = 3;
        ui->btn_level->setText("LEVEL: HARD");
    } else if (botLevel == 3){
        botLevel = 1;
        ui->btn_level->setText("LEVEL: EASY");
    }
}

void MainWindow::on_btn_rules_clicked()
{
    Rules rulesWindow;
    rulesWindow.exec();
}

void MainWindow::on_btn_startGame_clicked()
{
    this->hide();
    Dialog dialogWindow;
    dialogWindow.exec();
    Game gameWindow;
    gameWindow.exec();
    this->show();
}
